#include "stdafx.h"
#include "CProfileOpt.h"
#include "calcg.h"
#include "HdmDes.h"
#include "GTZDK.h"
#include "GTZDM.h"
#include "MyLayout.h"
#include "SetDrawQXYS.h"
#include "CWorkDir.h"
#include "RoadDesignWorkingDatabase.h"
#include "JD_CENTER.h"
#include "HdmDes.h"
#include "XLDataBase_JYX.h"

#ifdef VERSION_CAD2016
#include "acedCmdNF.h"

#endif // VERSION_CAD2016

#define PRESION 0.000001
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库
extern triDTM_road ptriDTM;		//数模
extern CString Cworkdir, Pro;
extern ACHAR SysPath[1024];
extern AcDbVoidPtrArray DesLineInfor;//设计线实体信息，ID

CProfileOpt::CProfileOpt()
{
	m_PopulationSize = 100; //种群规模
	m_SlopePtSum = 100; //变坡点总数
	m_EvoluGeneration = 200; //进化代数
	m_EndDesH = 0.0;    //终点设计高程
	m_StartDesH = 0.0; //起点设计高程
	m_PdXSWS = 3;    //坡度小数位数
	CrossProbablity = 0.0;//交叉概率
	MutationPrabality = 0.0;//变异概率
	m_CutCost = 20;//挖方单价，每立方米
	m_FillCost = 15;//填方单价，每立方米
	m_BridgeCost = 28000.0;//桥梁单价，每延米
	m_TunnelCost = 30000.0;//隧道单价，每延米
	m_MinBriFill = 8.0;//设桥最小填高
	m_MinBriLeth = 16.0;//自动设桥最小桥长
	m_MinTunFill = 30.0;//设隧最小挖深
	m_MinTunLeth = 20.0;//自动设隧最小隧长
	m_TunFill = 12.0;//线隧分界填高
	m_BriFill = 8.0;//桥线分界填高
	m_HbPdc = 1.1;//坡度合并的最小坡度代数差
	m_PlanMDB = _T(""); //方案数据库
	m_IsSetBaseBriHigh = TRUE; //是否设置桥梁限制高度
	m_IsSetBaseTunLen = TRUE; // 是否设置隧道限制长度
	m_BaseBriHigh = 50.0;   //桥梁基本高度
	m_BaseTunLen = 10000.0; //隧道基本长度
	m_StaMaxSlope = 1.5;  //车站限坡
	m_ENDGeneration = 20; //终止代数
	m_ENDMONEY = 1.0;   //终止时费用的差值
	m_EndType = 0;    //所选择的条件
	m_JXorRXD = 0;  //为0时是基线，为1时是绕行段
	m_JHDS = " ";
	m_MaxBrigTwg = 150; //最大桥梁高度
	m_MaxBriTwg = 150;
	temp_mileage = 0.0;
	m_CrossStyle = TRUE;
	m_MinBiLen = 20.0;
	TempStopCreat = FALSE; //优化在生成初始方案时退出，停止生成
	TempStopOpt = FALSE; //优化在进化时退出
	Thread_Num = 0;
	m_TKNum = -1;
	GCKZD = NULL;
	CM = NULL;
	m_BpdLC = 0;
	m_Stop = 0;
	m_ProId = NULL;
	m_BriId = NULL;
	m_TunId = NULL;
	Cost_xs = 1.0;//工程费用所占比例
	Oprate_xs = 0.0;//运营费用所占比例
	m_IsSQST = FALSE;
	MaxX = 0;//信封区域最高点
	MinX = 0;//信封区域最低点
	m_SearchStartIndex = 0;
	m_IsDrawProfile = FALSE;
	m_Ex = TRUE;

	m_pPlain = NULL; //平面指针
	//m_pProfile = NULL; //纵断面指针
	//pdmx = NULL; //地面线指针
	m_LjorGm = 0;//路肩坡或者是轨面坡
	m_JPInforArray.clear();
	m_DMXLine.clear();
	m_SlopeArray.clear();
	m_BPDArray.clear();
	//修改后需进行后续判断
	m_IsProNowPlan = FALSE; //是否优化现有的方案 
	m_IsSetDesH = FALSE; //是否自动设计高程
	m_Xlstye = -1; //默认为基线
	m_BpdLC = 1;
	m_PdXSWS = 2;//坡度小数位数
	m_DesHXsw = 4;//设计标高小数位数
}


CProfileOpt::~CProfileOpt()
{
}

/*===================================================================
函数名:StartOptimization
------------------------------------------------------------
函数功能:优化纵断面主控函数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：
=====================================================================*/
void CProfileOpt::StartOptimization(vector<PDTAB_ROAD>&m_SlopeTAB,vector<BRITAB>&OutBriArray, vector<TUNTAB>&OutTunArray)
{

	//m_TKNum = pdmx->Zdmno; //图框号
	m_PlanMDB = m_pPlain->mdbname; //数据库
	m_RoadName = m_pPlain->RoadName;//线路名
	//计算纵断面设计标准
	basdraw.SetZdmMinPC(m_pPlain->DV, m_PrfDesData.MAXPD, m_PrfDesData.YBPCMin, m_PrfDesData.JXPCMin, m_PrfDesData.T_YBRshuMin,
		m_PrfDesData.T_JXRShu, m_PrfDesData.A_YBRshuMin, m_PrfDesData.A_JXRShu, m_PrfDesData.YBLRshuMin, m_PrfDesData.JXLRshuMin);

	m_MaxLongitudinalSlope = _wtof(m_PrfDesData.MAXPD);//最大纵坡
	m_MinLongitudinalSlopeL = _wtof(m_PrfDesData.YBPCMin);//最小纵坡长
	m_VerticalCurveRadius = _wtof(m_PrfDesData.A_YBRshuMin); //竖曲线半径

	TempStopCreat = FALSE;//优化在生成初始方案时退出，停止生成
	TempStopOpt = FALSE;//优化在进化时退出
	pzLinep = m_pPlain;
	temp_mileage = 0.0;

	m_HbPdc = 0.2; //暂时按0.2处理
	m_MaxLongitudinalSlope = m_MaxLongitudinalSlope / 100;
	m_MaxSlopeDifference = _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001;
	m_StaMaxSlope = 0;  //不需要考虑车站
	//m_StaMaxSlope = m_StaMaxSlope / 1000;  //不需要考虑车站

	//获取起终里程、高程
	GetStAndEdLC(m_Startml, m_Endml, m_StartDesH, m_EndDesH); 

	//获得接坡约束范围内的地面线
	std::vector<DMXYS> LSDMX; //临时地面线表
	LSDMX.clear();
	LSDMX = m_DMXLine;
	m_DMXLine.clear();
	DMXYS tempDMXYS;
	for (int i = 0; i < LSDMX.size(); i++)
	{
		if (LSDMX[i].lc >= m_Startml && LSDMX[i].lc <= m_Endml)
		{
			tempDMXYS.lc = LSDMX[i].lc;
			tempDMXYS.level = LSDMX[i].level;
			m_DMXLine.push_back(tempDMXYS);
		}
	}


	DealEartH(m_DMXLine.size(), m_DMXLine); //处理地面线

	//2.生成地面线和桩号序列 
	m_CrossSection.road = new HdmDes::RoadHdmdata[1];
	m_CrossSection.road[0].NCRS = m_DMXLine.size();

	m_CrossSection.road[0].pm = m_pPlain;
	m_CrossSection.road[0].hdmdes = new CRSDAT[m_DMXLine.size()];
	for (int i = 0; i < m_DMXLine.size(); i++)
	{
		m_CrossSection.road[0].hdmdes[i].cml = m_DMXLine[i].lc;
		m_CrossSection.road[0].hdmdes[i].newLYd = m_DMXLine[i].level;
		m_CrossSection.road[0].hdmdes[i].newRYd = m_DMXLine[i].level;
	}

	Read_GCKZD();//读高程控制点

	GetGCKZD(m_Startml, m_StartDesH, m_Endml, m_EndDesH);//获得高程控制点数组

	////////////////////////////用信封区域调整地面线，平顺地面线//////////////////////////////////////////
	GetDMXfromEnvelop(m_Startml, m_StartDesH);//用信封区域调整地面线
	//给纵断面地面线点赋值
	ZMdmxPt.clear();
	ZMdmxPt.resize(m_CrossSection.road[0].NCRS);
	for (int Crsi = 0; Crsi < m_CrossSection.road[0].NCRS; Crsi++)
	{
		ZMdmxPt[Crsi].mile = tempCRS[Crsi].cml;
		ZMdmxPt[Crsi].high = tempCRS[Crsi].EarH;
	}

	//找有效地面线范围
	GetDMXRange();

	//得到缓和曲线和车站的分段(不考虑)
	//GetTransitionAndStation();

	//平顺地面线
	PsDmx(m_MinLongitudinalSlopeL, 10);

	bool ContinueOrStrat = true;//true-重新生成纵断面进行优化，false-在现有方案的基础上进行优化
	m_IsProNowPlan = true; //优化现有方案
	//3.初始化种群
	if (m_IsProNowPlan)//优化现有方案
	{
		ContinueOrStrat = false;
		//赋初值 
		GetFirstPlan(m_PopulationSize);
		//获得已有方案
		GetPlan();
		//得到种群的里程数组
		CreateProfileMileage(ContinueOrStrat);
		//用两种方法生成初始方案
		CreatPlanByTwoMethod(ContinueOrStrat);
	}
	else //重新生成方案进行优化
	{
		ForFirstPlan();
		CreateProfileMileage(ContinueOrStrat);
		CreatPlanByTwoMethod(ContinueOrStrat);
	}

	ReleaseRoom(0);

	if (!TempStopCreat)//如果优化在生成方案时提前退出，则不用再进行以下优化运算
	{
		//4.计算每个个体的土石方工程量
		ACHAR pszLabel[80];
		_tcscpy(pszLabel, L"计算初始方案工程费用");
		acedSetStatusBarProgressMeter(pszLabel, 0, 100);

		for (int ProfileIndex = 0; ProfileIndex < m_PopulationSize; ProfileIndex++)
		{
			double FillArea = 0.0;
			double CutArea = 0.0;
			CalEarthWork(m_ProfilePlanArray[ProfileIndex], FillArea, CutArea);
			if (TempStopOpt)
				break;

			acedSetStatusBarProgressMeterPos((ProfileIndex + 1)*100.0 / m_PopulationSize);
		}

		acedSetStatusBarProgressMeterPos(100);
		acedRestoreStatusBar();

		//5. 进化
		if (!TempStopOpt)//不提前退出则继续往下优化
		{
			//基于土石方费用的排序
			Taxis(m_ProfilePlanArray);

			Cal_Satisfaction(m_ProfilePlanArray);//计算方案满意度

			Taxis_Satisfaction(m_ProfilePlanArray);//基于满意度的排序

			m_BestPlan.clear();//每一代最优方案数组

			for (int iPlan = 0; iPlan < m_ProfilePlanArray.size(); iPlan++)
			{
				CalAllPD(m_ProfilePlanArray[iPlan]);
			}

			m_EndType = 1;
			if (!m_EndType)//按进化代数终止优化
			{
				ACHAR pszLabel[80];
				_tcscpy(pszLabel, L"纵断面优化进程");
				acedSetStatusBarProgressMeter(pszLabel, 0, 100);

				for (int i = 0; i < m_EvoluGeneration; i++)//进化
				{
					//计算适应度值
					CalFitness();
					double diff = 0.0;
					CalDiff_Satisfaction(diff);

					//计算选择概率
					CalChooseProbability(diff);

					//生成下一代
					if (!CreateNextPopulation_Satisfaction(diff))
						break;

					double TempCost = m_ProfilePlanArray[0].Cost / 10000;
					ads_printf(L"进化到第%d代  工程总费用为%0.3lf万元！如需退出，请按Esc！\n", i + 1, TempCost);
					acedSetStatusBarProgressMeterPos((i + 1)*100.0 / m_EvoluGeneration);
				}
				acedSetStatusBarProgressMeterPos(100);
				acedRestoreStatusBar();

			}
			else//按费用差值终止进化
			{
				int i = 0;
				while (EndAlgorithm())
				{ 
					//计算适应度值 
					CalFitness();

					double diff = 0.0;
					CalDiff_Satisfaction(diff);

					//计算选择概率
					CalChooseProbability(diff);

					//将此代最优方案的费用保存
					m_BestPlan.push_back(m_ProfilePlanArray[0].Satisfaction);

					//生成下一代
					if (!CreateNextPopulation_Satisfaction(diff))
						break;  // 这边的用break？

					double TempCost = m_ProfilePlanArray[0].Cost / 10000;
					ads_printf(L"进化到第%d代  工程总费用为%0.3lf万元\n", i + 1, TempCost);

					//费用差提示
					int BestPlanNum = m_BestPlan.size();
					if (BestPlanNum > m_ENDGeneration)
					{
						//如果连续m_ENDGeneration代的费用差值小于m_ENDMONEY，则循环结束
						double money = fabs(m_BestPlan[BestPlanNum - 1] - m_BestPlan[BestPlanNum - (m_ENDGeneration + 1)]);
						double TempM = money / m_BestPlan[BestPlanNum - 1] * 1000;
						ads_printf(L"费用差为千分之%0.1lf\n", TempM);
					}
					i++;
				}
			}
		}

		//进化完成后的处理
		if (!TempStopCreat)
		{
			LastBestPlan = m_ProfilePlanArray[0];

			ReleaseRoom(1);

			//为了避免在计算坡度折减时反复将现场里程转换为连续里程，将最终的隧道数组赋值到m_section_Tun_Bri中
			CString TemSkml,TemEkml;
			m_section_Tun_Bri.clear();
			m_section_Tun_Bri.resize(LastBestPlan.TunArray.size());
			for (int i = 0; i < m_section_Tun_Bri.size(); i++)
			{
				TemSkml = LastBestPlan.TunArray[i].SKml;
				TemEkml = LastBestPlan.TunArray[i].EKml;
				m_section_Tun_Bri[i].Style = 4;
				m_section_Tun_Bri[i].minS = m_pPlain->TYLC(TemSkml);
				m_section_Tun_Bri[i].maxS = m_pPlain->TYLC(TemEkml);
			}

			//按限制坡度调整
			AdjustPlanByLimitGrad(LastBestPlan);

			//删除隧道内的凹形坡
			DeleteSlopePtInTun(LastBestPlan);

			//按坡度代数差调整
			AdjustPlanByDsc(LastBestPlan);

			////合并坡度
			//MergeSlope(LastBestPlan);

			CalLastPlanCost(LastBestPlan);//计算最终费用

			CString LastCost;
			double TempCost = LastBestPlan.Cost / 10000;
			if (TempStopOpt)//用户提前结束优化所得的结果
				LastCost.Format(L"用户提前结束优化，最终工程总费用为%0.3lf万元", TempCost);
			else
			{
				LastCost.Format(L"优化已完成，最终工程总费用为%0.3lf万元", TempCost);
			}

			ads_printf(LastCost);

			m_SlopeTAB.clear(); OutBriArray.clear(); OutTunArray.clear();
			//传出坡度数组
			if (LastBestPlan.SlopePtArray.size() > 0)
			{
				m_SlopeTAB.resize(LastBestPlan.SlopePtArray.size());
				for (int i = 0; i < LastBestPlan.SlopePtArray.size(); i++)
				{
					m_SlopeTAB[i].ml = LastBestPlan.SlopePtArray[i].Mileage;
					m_SlopeTAB[i].Level = LastBestPlan.SlopePtArray[i].DesH;
					m_SlopeTAB[i].Rshu = LastBestPlan.SlopePtArray[i].VerticalCurveRadius;
				}
			}
			//传出桥梁数组
			if(LastBestPlan.BriArray.size() >0)
				OutBriArray.assign(LastBestPlan.BriArray.begin(), LastBestPlan.BriArray.end());
			//传出隧道数组
			if(LastBestPlan.TunArray.size() > 0)
				OutTunArray.assign(LastBestPlan.TunArray.begin(), LastBestPlan.TunArray.end());
			//WriteXLDBS();//将最终方案写入数据库
			//DrawZDM();
			ReleaseRoom(2);

		}
	}
}

/*===================================================================
函数名:GetStAndEdLC
------------------------------------------------------------
函数功能:获取优化起终里程、高程
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：
=====================================================================*/
void CProfileOpt::GetStAndEdLC(double &StartLC,double &EndLC,double &StLevel,double &EdLevel)
{
	if (m_JPInforArray.size() < 1)
	{
		//BPDnum = 2
		StartLC = pzLinep->DLArray[0].ELC;  StLevel = m_BPDArray[0].Level;
		EndLC = pzLinep->XLLength;  EdLevel = m_BPDArray[1].Level;
	}
	else
	{
		if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 1) //前后约束
		{
			//BPDnum = 4
			int PDnum;
			PDnum = (int)m_BPDArray.size();
			if (PDnum == 4)
			{
				StartLC = m_BPDArray[1].ml; StLevel = m_BPDArray[1].Level;
				EndLC = m_BPDArray[2].ml; EdLevel = m_BPDArray[2].Level;
			}
			else
			{
				//由于坡长限制，求变坡点时考虑
				StartLC = m_BPDArray[0].ml; StLevel = m_BPDArray[0].Level;
				EndLC = m_BPDArray[PDnum - 1].ml; EdLevel = m_BPDArray[PDnum - 1].Level;
			}
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 0) //前约束后不约束
		{
			//BPDnum = 3
			StartLC = m_BPDArray[1].ml;  StLevel = m_BPDArray[1].Level;
			EndLC = m_BPDArray[2].ml;  EdLevel = m_BPDArray[2].Level;
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 0 && m_JPInforArray[0].Is_JP_Aft == 1) //前不约束后约束
		{
			//BPDnum = 3
			StartLC = m_BPDArray[0].ml; StLevel = m_BPDArray[0].Level;
			EndLC = m_BPDArray[1].ml; EdLevel = m_BPDArray[1].Level;
		}
		else
		{
			//BPDnum = 2
			StartLC = pzLinep->DLArray[0].ELC;  StLevel = m_BPDArray[0].Level;
			EndLC = pzLinep->XLLength;  EdLevel = m_BPDArray[1].Level;
		}
	}
}



//处理地面线（当地面标高为-100时，取前一个正常点的标高）
void CProfileOpt::DealEartH(int NCRS, vector<DMXYS>&DMXArray)
{
	int i = 0;
	if (DMXArray.size() == 0)
		return;
	
	//起点地面标高数据没有
	if (DMXArray[0].level < -99.0)
	{
		DMXArray[0].level = 0.0;
		i = 0;
		for (i = 1; i < NCRS; i++)
		{
			if (DMXArray[i].level > -99.0)
			{
				DMXArray[0].level = DMXArray[i].level;
				break;
			}
		}
	}
	for (i = 1; i < NCRS; i++)
	{
		if (DMXArray[i].level < -99.0)
			DMXArray[i].level = DMXArray[i - 1].level;
	}
}

//读高程控制点表
void CProfileOpt::Read_GCKZD()
{

	//从数据库中读取高程控制点表数据 (控制点)
	XLDataBase DBS;
	DBS.Read_XLDbs(m_pPlain->mdbname, "高程控制点表", m_pPlain->RoadName);
	GCKZDTAB.clear();
	GCKZDTAB = DBS.m_LevelCtrlPtArray;
	NGCKZD = GCKZDTAB.size();
	double cml = 0.0;
	ACHAR temp1[80];
	if (GCKZDTAB.size() > 0)
	{
		int i;
		for (i = 0; i < GCKZDTAB.size(); i++)//求高程控制点的统一里程
		{
			_tcscpy(temp1, m_pPlain->LCchr(GCKZDTAB[i].GH, GCKZDTAB[i].XCLc, 3, 1));
			cml = m_pPlain->Trs_Ckml_To_ProjMl(temp1);
			GCKZDTAB[i].Cml = cml;
		}

		for (i = 0; i < NGCKZD - 1; i++)//排序
		{
			for (int j = i + 1; j < NGCKZD; j++)
			{
				if (GCKZDTAB[i].Cml > GCKZDTAB[j].Cml)
				{
					GCKZPt temp;
					temp = GCKZDTAB[i];
					GCKZDTAB[i] = GCKZDTAB[j];
					GCKZDTAB[j] = temp;
				}
			}
		}
	}
}

void CProfileOpt::GetGCKZD(double StartM, double StartH, double EndM, double EndH)//获得高程控制点数组
{
	////////////////////////////////////////高程控制点的处理///////////////////////////////////////////

	vector<GCKZPt> SX_GCKZDArry, XX_GCKZDArry;//上下限高程控制点数组
	GCKZPt TempGCKZD;
	SX_GCKZDArry.clear();
	XX_GCKZDArry.clear();

	TempGCKZD.Cml = StartM;
	TempGCKZD.DesHSX = StartH;
	TempGCKZD.DesHXX = StartH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//加入数据库中控制点
	for (int i = 0; i < NGCKZD; i++)//将起终点按控制点考虑
	{
		if (GCKZDTAB[i].Cml > StartM && GCKZDTAB[i].Cml < EndM)
		{
			if (abs(GCKZDTAB[i].Style) == 3 || abs(GCKZDTAB[i].Style) == 4)
			{
				TempGCKZD = GCKZDTAB[i];
				TempGCKZD.Style = 1;
				if (CheckKZDIsInEnvelop(TempGCKZD.Cml, TempGCKZD.DesHSX, StartM, StartH, EndM, EndH))
					SX_GCKZDArry.push_back(TempGCKZD);
				TempGCKZD = GCKZDTAB[i];
				TempGCKZD.Style = 2;
				if (CheckKZDIsInEnvelop(TempGCKZD.Cml, TempGCKZD.DesHXX, StartM, StartH, EndM, EndH))
					XX_GCKZDArry.push_back(TempGCKZD);
			}
			else if (abs(GCKZDTAB[i].Style) == 1) //上限点
			{
				TempGCKZD = GCKZDTAB[i];
				if (CheckKZDIsInEnvelop(TempGCKZD.Cml, TempGCKZD.DesHSX, StartM, StartH, EndM, EndH))
					SX_GCKZDArry.push_back(TempGCKZD);
			}
			else
			{
				TempGCKZD = GCKZDTAB[i];
				if (CheckKZDIsInEnvelop(TempGCKZD.Cml, TempGCKZD.DesHXX, StartM, StartH, EndM, EndH))
					XX_GCKZDArry.push_back(TempGCKZD);
			}
		}
	}

	//加入永久桥梁控制点
	for (int i = 0; i < m_BriGCKZDArray.size(); i++)
	{
		TempGCKZD = m_BriGCKZDArray[i];
		if (CheckKZDIsInEnvelop(TempGCKZD.Cml, TempGCKZD.DesHSX, StartM, StartH, EndM, EndH))
		{
			SX_GCKZDArry.push_back(TempGCKZD);
		}
	}

	//第一个试算方案时，加入信封区域自动设置的桥梁控制点
	if (m_TryNum == 0)
	{
		for (int i = 0; i < m_AutoSetBriGCKZDArray.size(); i++)
		{
			TempGCKZD = m_AutoSetBriGCKZDArray[i];
			if (CheckKZDIsInEnvelop(TempGCKZD.Cml, TempGCKZD.DesHSX, StartM, StartH, EndM, EndH))
			{
				SX_GCKZDArry.push_back(TempGCKZD);
			}
		}
	}

	TempGCKZD.Cml = EndM;
	TempGCKZD.DesHSX = EndH;
	TempGCKZD.DesHXX = EndH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

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
				SX_GCKZDArry.erase(SX_GCKZDArry.begin()+ i);
			else
				SX_GCKZDArry.erase(SX_GCKZDArry.begin()+ i + 1);
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

	if (m_TryNum == 0)
	{
		RemoveGCKZD(SX_GCKZDArry, true);
		RemoveGCKZD(XX_GCKZDArry, false);
	}



	Ngckzd = SX_GCKZDArry.size() + XX_GCKZDArry.size();
	if (GCKZD != NULL)
	{
		delete[]GCKZD;
		GCKZD = NULL;
	}
	GCKZD = new GCKZPt[Ngckzd];
	KZDSum = 0;
	for (int i = 0; i < SX_GCKZDArry.size(); i++)//将起终点按控制点考虑
	{
		GCKZD[KZDSum] = SX_GCKZDArry[i];
		KZDSum++;
	}
	for (int i = 0; i < XX_GCKZDArry.size(); i++)//将起终点按控制点考虑
	{
		GCKZD[KZDSum] = XX_GCKZDArry[i];
		KZDSum++;
	}

	SX_GCKZDArry.clear();
	XX_GCKZDArry.clear();

	if (fabs(StartM - m_Startml) < 0.1)
		GetEnvelop();
	GetDMXfromEnvelop(StartM, StartH);
	GetEnvelop();



}

//检测高程控制点是否在原始信封区域内
bool CProfileOpt::CheckKZDIsInEnvelop(double Mileage, double& Lever, double StartMileage, double StartLever, double EndMileage, double EndLever)
{
	double BefLineEnd_Mileage, BefLineEnd_Lever;
	double AftLine_Start_Mileage, AftLine_Start_Lever;
	double MaxInterMileage, MinInterMileage;
	double MaxLever, MinLever;
	double TempH;

	if (Mileage > EndMileage || Mileage < StartMileage)
	{
		return false;
	}

	BefLineEnd_Mileage = EndMileage;
	BefLineEnd_Lever = StartLever + (EndMileage - StartMileage) * m_MaxLongitudinalSlope;

	AftLine_Start_Mileage = StartMileage;
	AftLine_Start_Lever = EndLever + (EndMileage - StartMileage) * m_MaxLongitudinalSlope;

	INTERS(&MaxInterMileage, &TempH,
		StartMileage, StartLever, BefLineEnd_Mileage, BefLineEnd_Lever,
		AftLine_Start_Mileage, AftLine_Start_Lever, EndMileage, EndLever);

	if (Mileage < MaxInterMileage)
		MaxLever = StartLever + (Mileage - StartMileage) * m_MaxLongitudinalSlope;
	else
		MaxLever = EndLever + (EndMileage - Mileage) * m_MaxLongitudinalSlope;

	if (MaxLever + 0.01 < Lever)
	{
		Lever = MaxLever;
		return true;
	}

	BefLineEnd_Mileage = EndMileage;
	BefLineEnd_Lever = StartLever - (EndMileage - StartMileage) * m_MaxLongitudinalSlope;

	AftLine_Start_Mileage = StartMileage;
	AftLine_Start_Lever = EndLever - (EndMileage - StartMileage) * m_MaxLongitudinalSlope;

	INTERS(&MinInterMileage, &TempH,
		StartMileage, StartLever, BefLineEnd_Mileage, BefLineEnd_Lever,
		AftLine_Start_Mileage, AftLine_Start_Lever, EndMileage, EndLever);

	if (Mileage < MinInterMileage)
		MinLever = StartLever - (Mileage - StartMileage) * m_MaxLongitudinalSlope;
	else
		MinLever = EndLever - (EndMileage - Mileage) * m_MaxLongitudinalSlope;

	if (MinLever - 0.01 > Lever)
	{
		Lever = MinLever;
		return true;
	}

	return true;
}

//清除不合理的高程控制点
//GCKZDArry	控制点数组
//IsSXKZD	是否上限控制点
void CProfileOpt::RemoveGCKZD(vector<GCKZPt>& GCKZDArry, bool IsSXKZD)
{

	int index = -1;
	index = FindMaxSlopeGCKZDIndex(GCKZDArry, IsSXKZD);
	while (index != -1)
	{
		if (IsSXKZD)
		{
			if (GCKZDArry[index].DesHSX > GCKZDArry[index + 1].DesHSX)
			{
				//	ads_printf("里程%lf,上限标高%lf被清除!\n",GCKZDArry[index].Cml,GCKZDArry[index].DesHSX);
				GCKZDArry.erase(GCKZDArry.begin() + index);
			}
			else
			{
				//	ads_printf("里程%lf,上限标高%lf被清除!\n",GCKZDArry[index+1].Cml,GCKZDArry[index+1].DesHSX);
				GCKZDArry.erase(GCKZDArry.begin() + index + 1);
			}
		}
		else
		{
			if (GCKZDArry[index].DesHXX < GCKZDArry[index + 1].DesHXX)
			{
				//	ads_printf("里程%lf,下限标高%lf被清除!\n",GCKZDArry[index].Cml,GCKZDArry[index].DesHXX);
				GCKZDArry.erase(GCKZDArry.begin() +index);
			}
			else
			{
				//	ads_printf("里程%lf,下限标高%lf被清除!\n",GCKZDArry[index+1].Cml,GCKZDArry[index+1].DesHXX);
				GCKZDArry.erase(GCKZDArry.begin()+index + 1);
				
			}
		}
		index = FindMaxSlopeGCKZDIndex(GCKZDArry, IsSXKZD);
	}
}
//搜索高程控制点中违规最严重的高程控制点，当都不违规时，返回-1
//GCKZDArry	控制点数组
//IsSXKZD	是否上限控制点
int CProfileOpt::FindMaxSlopeGCKZDIndex(vector<GCKZPt>& GCKZDArry, bool IsSXKZD)
{
	int GCKZDSum = GCKZDArry.size();
	if (GCKZDSum < 3)
		return -1;
	int MaxSlopeIndex = -1;
	double MaxSlope = m_MaxLongitudinalSlope;
	double Slope;
	if (IsSXKZD)
	{
		for (int i = 0; i < GCKZDSum - 1; i++)
		{
			Slope = (GCKZDArry[i + 1].DesHSX - GCKZDArry[i].DesHSX) / (GCKZDArry[i + 1].Cml - GCKZDArry[i].Cml);
			Slope = fabs(Slope);
			if (Slope > MaxSlope + 0.0000001)
			{
				MaxSlope = Slope;
				MaxSlopeIndex = i;
			}
		}
	}

	else
	{
		for (int i = 0; i < GCKZDSum - 1; i++)
		{
			Slope = (GCKZDArry[i + 1].DesHXX - GCKZDArry[i].DesHXX) / (GCKZDArry[i + 1].Cml - GCKZDArry[i].Cml);
			Slope = fabs(Slope);
			if (Slope > MaxSlope + 0.0000001)
			{
				MaxSlope = Slope;
				MaxSlopeIndex = i;
			}
		}
	}

	return MaxSlopeIndex;

}

//得到信封区域
void CProfileOpt::GetEnvelop()
{
	CString Mes;

	MaxEnvPtArray.clear();
	MinEnvPtArray.clear();

	EnvPt TempEnvPt;

	//循环将各个高程控制点加入，每加入一个高程控制点同时，需要将当前控制点与前一个高程控制点之间的点加入
	//两直线求交计算两控制点中间的点
	double Lengh = m_Endml - m_Startml;
	double BefLineEnd_Mileage, BefLineEnd_Lever;//前一控制点沿着最大最小坡往后走Lengh之后的里程和高程
	double AftLine_Start_Mileage, AftLine_Start_Mileage_Lever;//当前控制点沿着最大最小坡往前走Lengh之后的里程和高程
	int PerMaxPt = 0;//前一个上限控制点或必经点
	int PerMinPt = 0;//前一个下限控制点或必经点
	int Res = 0;
	double MaxSlope_SX = m_MaxLongitudinalSlope;
	double MaxSlope_XX = m_MaxLongitudinalSlope;
	double MAX_Cml, MAX_H, MIN_Cml, MIN_H;

	//计算上限点
	//将起点加入
	TempEnvPt.Mileage = GCKZD[0].Cml;
	TempEnvPt.Lever = GCKZD[0].DesHSX;
	MaxEnvPtArray.push_back(TempEnvPt);
	for (int iKZD = 1; iKZD < Ngckzd; iKZD++)
	{
		if (abs(GCKZD[iKZD].Style) == 1)//当前为上限
		{
			if (m_ProfilePlanArray.size() < 1)
			{
				GetJD(GCKZD[PerMaxPt].Cml, GCKZD[PerMaxPt].DesHSX, GCKZD[iKZD].Cml, GCKZD[iKZD].DesHSX, MAX_Cml, MAX_H, MIN_Cml, MIN_H);
				TempEnvPt.Mileage = MAX_Cml;
				TempEnvPt.Lever = MAX_H;
				MaxEnvPtArray.push_back(TempEnvPt);

				TempEnvPt.Mileage = GCKZD[iKZD].Cml;
				TempEnvPt.Lever = GCKZD[iKZD].DesHSX;
				MaxEnvPtArray.push_back(TempEnvPt);
			}
			else
			{
				vector<EnvPt> GCKZDArray;
				EnvPt tempEnvpt;

				CalEnvPtArray(GCKZD[PerMaxPt], GCKZD[iKZD], GCKZDArray);

				for (int i = 0;i< GCKZDArray.size();i++)
				{
					tempEnvpt.Lever = GCKZDArray[i].Lever;
					tempEnvpt.Mileage = GCKZDArray[i].Mileage;
					MaxEnvPtArray.push_back(tempEnvpt);
				}
				PerMaxPt = iKZD;
			}
		}
		else
		{
			PerMinPt = iKZD;
			break;
		}
	}

	//将起点加入下限点数组
	TempEnvPt.Mileage = GCKZD[PerMinPt].Cml;
	TempEnvPt.Lever = GCKZD[PerMinPt].DesHXX;
	MinEnvPtArray.push_back(TempEnvPt);
	//处理下限点
	for (int iKZD = PerMinPt + 1; iKZD < Ngckzd; iKZD++)
	{
		if (abs(GCKZD[iKZD].Style) == 2)//当前为下限
		{
			if (m_ProfilePlanArray.size() < 1)
			{
				GetJD(GCKZD[PerMinPt].Cml, GCKZD[PerMinPt].DesHXX, GCKZD[iKZD].Cml, GCKZD[iKZD].DesHXX, MAX_Cml, MAX_H, MIN_Cml, MIN_H);
				TempEnvPt.Mileage = MIN_Cml;
				TempEnvPt.Lever = MIN_H;
				MinEnvPtArray.push_back(TempEnvPt);

				TempEnvPt.Mileage = GCKZD[iKZD].Cml;
				TempEnvPt.Lever = GCKZD[iKZD].DesHXX;
				MinEnvPtArray.push_back(TempEnvPt);
				break;  
			}
			else
			{
				 vector<EnvPt> GCKZDArray;
				 EnvPt tempEnvpt_1;
				CalEnvPtArray(GCKZD[PerMinPt], GCKZD[iKZD], GCKZDArray);
				for (int i = 0; i < GCKZDArray.size(); i++)
				{
					tempEnvpt_1.Lever = GCKZDArray[i].Lever;
					tempEnvpt_1.Mileage = GCKZDArray[i].Mileage;
					MinEnvPtArray.push_back(tempEnvpt_1);
				}
				PerMinPt = iKZD;
			}
		}
		else
		{
			break;
		}
	}

}

//获取交点
void CProfileOpt::GetJD(double Scml, double SLever, double Ecml, double ELever,
	double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H)
{
	///////////////////////////////////////得到信封区域的交点//////////////////////////////////

	double QZminH = 0.0;//起终点确定的最小高程
	double QZmaxH = 0.0;//起终点确定的最大高程
	double x1 = Scml;
	double y1 = SLever;

	double maxx2 = Ecml;
	double maxy2 = y1 + (Ecml - Scml)*m_MaxLongitudinalSlope;

	double x3 = Ecml;
	double y3 = ELever;

	double maxx4 = x1;
	double maxy4 = y3 + (Ecml - Scml)*m_MaxLongitudinalSlope;

	BAS_DRAW_FUN::INTERS(&MAX_cml, &MAX_H, x1, y1, maxx2, maxy2, x3, y3, maxx4, maxy4);//计算信封区域的最高点
	if (MAX_cml < Scml)
	{
		MAX_cml = Scml;
		MAX_H = SLever;
	}
	if (MAX_cml > Ecml)
	{
		MAX_cml = Ecml;
		MAX_H = ELever;
	}

	double minx2 = m_Endml;
	double miny2 = y1 - m_pPlain->XLLength*m_MaxLongitudinalSlope;
	//double minx2 = m_pPlain->JDarray[0][3] + m_pPlain->xl_length;
	//double miny2 = y1 - m_pPlain->xl_length*m_MaxLongitudinalSlope;

	double minx4 = x1;
	double miny4 = y3 - m_pPlain->XLLength*m_MaxLongitudinalSlope;

	double MinY;
	BAS_DRAW_FUN::INTERS(&MIN_cml, &MIN_H, x1, y1, minx2, miny2, x3, y3, minx4, miny4);//计算信封区域的最低点
	if (MIN_cml < Scml)
	{
		MIN_cml = Scml;
		MIN_H = SLever;
	}
	if (MIN_cml > Ecml)
	{
		MIN_cml = Ecml;
		MIN_H = ELever;
	}
}

//计算两控制点之间的信封区域点
void CProfileOpt::CalEnvPtArray(GCKZPt SPt, GCKZPt Ept, vector<EnvPt>& EnvPtArray)
{
	EnvPtArray.clear();
	EnvPt TempEnvPt;
	double MaxSlope_SX, MaxSlope_XX;

	//判断是否上限控制点
	bool IsSXPt = true;
	if (abs(SPt.Style) == 1)
		IsSXPt = true;
	else
		IsSXPt = false;
	int StartIndex, EndIndex;

	vector<EnvPt> PLine1;
	vector<EnvPt> PLine2;


	//将起点加入
	if (IsSXPt)
	{
		vector<double> MaxSlope_XX_Array;
		MaxSlope_XX_Array.clear();

		TempEnvPt.Mileage = SPt.Cml;
		TempEnvPt.Lever = SPt.DesHSX;
		PLine1.push_back(TempEnvPt);

		for (int iBPD = 0; iBPD < m_SlopePtSum; iBPD++)
		{
			if (iBPD > 0 && m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage > SPt.Cml + 1.0)
			{
				CalLimitGrade(m_ProfilePlanArray[0], m_ProfilePlanArray[0].SlopePtArray[iBPD - 1].Mileage, m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage,
					MaxSlope_SX, MaxSlope_XX);
				MaxSlope_XX_Array.push_back(MaxSlope_XX);

				TempEnvPt.Lever = TempEnvPt.Lever + MaxSlope_SX * (m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage - TempEnvPt.Mileage);
				TempEnvPt.Mileage = m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage;

				if (m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage > Ept.Cml + 1.0)
				{
					TempEnvPt.Lever = TempEnvPt.Lever + MaxSlope_SX * (Ept.Cml - TempEnvPt.Mileage);
					TempEnvPt.Mileage = Ept.Cml;
					PLine1.push_back(TempEnvPt);
					break;
				}
				PLine1.push_back(TempEnvPt);
			}
		}

		TempEnvPt.Mileage = Ept.Cml;
		TempEnvPt.Lever = Ept.DesHSX;
		PLine2.push_back(TempEnvPt);
		for (int i = PLine1.size() - 1; i > 0; i--)
		{
			TempEnvPt.Mileage = PLine1[i - 1].Mileage;
			TempEnvPt.Lever = TempEnvPt.Lever + MaxSlope_XX_Array[i - 1] * (PLine1[i].Mileage - PLine1[i - 1].Mileage);
			PLine2.push_back(TempEnvPt);
		}

		bool IsHaveJD = false;
		EnvPt IntersPt;
		for (int i = 0; i < PLine1.size() - 1; i++)
		{
			for (int j = 0; j < PLine2.size() - 1; j++)
			{
				double Min_Mileage, Max_Mileage;
				Min_Mileage = max(PLine1[i].Mileage, PLine2[j + 1].Mileage);
				Max_Mileage = min(PLine1[i + 1].Mileage, PLine2[j].Mileage);
				if (Max_Mileage > Min_Mileage)
				{
					int res = INTERS(&IntersPt.Mileage, &IntersPt.Lever,
						PLine1[i].Mileage, PLine1[i].Lever, PLine1[i + 1].Mileage, PLine1[i + 1].Lever,
						PLine2[j].Mileage, PLine2[j].Lever, PLine2[j + 1].Mileage, PLine2[j + 1].Lever);
					if (res == 0)
					{
						IsHaveJD = true;
						break;
					}
				}
			}
			if (IsHaveJD)
				break;
		}
		if (IsHaveJD)
		{
			for (int i = 0; i < PLine1.size(); i++)
			{
				if (PLine1[i].Mileage < IntersPt.Mileage)
				{
					TempEnvPt.Mileage = PLine1[i].Mileage;
					TempEnvPt.Lever = PLine1[i].Lever;
					EnvPtArray.push_back(TempEnvPt);
				}
				else
					break;
			}
			TempEnvPt.Mileage = IntersPt.Mileage;
			TempEnvPt.Lever = IntersPt.Lever;
			EnvPtArray.push_back(TempEnvPt);

			for (int i = PLine2.size() - 1; i >= 0; i--)
			{

				if (PLine2[i].Mileage > IntersPt.Mileage)
				{
					TempEnvPt.Mileage = PLine2[i].Mileage;
					TempEnvPt.Lever = PLine2[i].Lever;
					EnvPtArray.push_back(TempEnvPt);
				}
			}
		}
		else
		{
			EnvPtArray.clear();

			TempEnvPt.Mileage = SPt.Cml;
			TempEnvPt.Lever = SPt.DesHSX;
			EnvPtArray.push_back(TempEnvPt);

			TempEnvPt.Mileage = Ept.Cml;
			TempEnvPt.Lever = Ept.DesHSX;
			EnvPtArray.push_back(TempEnvPt);
		}

		MaxSlope_XX_Array.clear();
	}
	else
	{
		vector<double> MaxSlope_SX_Array;
		MaxSlope_SX_Array.clear();

		TempEnvPt.Mileage = SPt.Cml;
		TempEnvPt.Lever = SPt.DesHXX;
		PLine1.push_back(TempEnvPt);

		for (int iBPD = 0; iBPD < m_SlopePtSum; iBPD++)
		{
			if (iBPD > 0 && m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage > SPt.Cml + 1.0)
			{
				CalLimitGrade(m_ProfilePlanArray[0], m_ProfilePlanArray[0].SlopePtArray[iBPD - 1].Mileage, m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage,
					MaxSlope_SX, MaxSlope_XX);
				MaxSlope_SX_Array.push_back(MaxSlope_SX);

				TempEnvPt.Lever = TempEnvPt.Lever - MaxSlope_XX * (m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage - TempEnvPt.Mileage);
				TempEnvPt.Mileage = m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage;

				if (m_ProfilePlanArray[0].SlopePtArray[iBPD].Mileage > Ept.Cml + 1.0)
				{
					TempEnvPt.Lever = TempEnvPt.Lever - MaxSlope_XX * (Ept.Cml - TempEnvPt.Mileage);
					TempEnvPt.Mileage = Ept.Cml;
					PLine1.push_back(TempEnvPt);
					break;
				}
				PLine1.push_back(TempEnvPt);
			}
		}

		TempEnvPt.Mileage = Ept.Cml;
		TempEnvPt.Lever = Ept.DesHXX;
		PLine2.push_back(TempEnvPt);
		for (int i = PLine1.size() - 1; i > 0; i--)
		{
			TempEnvPt.Mileage = PLine1[i - 1].Mileage;
			TempEnvPt.Lever = TempEnvPt.Lever - MaxSlope_SX_Array[i - 1] * (PLine1[i].Mileage - PLine1[i - 1].Mileage);
			PLine2.push_back(TempEnvPt);
		}

		bool IsHaveJD = false;
		EnvPt IntersPt;
		for (int i = 0; i < PLine1.size() - 1; i++)
		{
			for (int j = 0; j < PLine2.size() - 1; j++)
			{
				double Min_Mileage, Max_Mileage;
				Min_Mileage = max(PLine1[i].Mileage, PLine2[j + 1].Mileage);
				Max_Mileage = min(PLine1[i + 1].Mileage, PLine2[j].Mileage);
				if (Max_Mileage > Min_Mileage)
				{
					int res = INTERS(&IntersPt.Mileage, &IntersPt.Lever,
						PLine1[i].Mileage, PLine1[i].Lever, PLine1[i + 1].Mileage, PLine1[i + 1].Lever,
						PLine2[j].Mileage, PLine2[j].Lever, PLine2[j + 1].Mileage, PLine2[j + 1].Lever);
					if (res == 0)
					{
						IsHaveJD = true;
						break;
					}
				}
			}
			if (IsHaveJD)
				break;
		}
		if (IsHaveJD)
		{
			for (int i = 0; i < PLine1.size(); i++)
			{
				if (PLine1[i].Mileage < IntersPt.Mileage)
				{
					TempEnvPt.Mileage = PLine1[i].Mileage;
					TempEnvPt.Lever = PLine1[i].Lever;
					EnvPtArray.push_back(TempEnvPt);
				}
				else
					break;
			}
			TempEnvPt.Mileage = IntersPt.Mileage;
			TempEnvPt.Lever = IntersPt.Lever;
			EnvPtArray.push_back(TempEnvPt);

			for (int i = PLine2.size() - 1; i >= 0; i--)
			{

				if (PLine2[i].Mileage > IntersPt.Mileage)
				{
					TempEnvPt.Mileage = PLine2[i].Mileage;
					TempEnvPt.Lever = PLine2[i].Lever;
					EnvPtArray.push_back(TempEnvPt);
				}
			}
		}

		else
		{
			EnvPtArray.clear();

			TempEnvPt.Mileage = SPt.Cml;
			TempEnvPt.Lever = SPt.DesHXX;
			EnvPtArray.push_back(TempEnvPt);

			TempEnvPt.Mileage = Ept.Cml;
			TempEnvPt.Lever = Ept.DesHXX;
			EnvPtArray.push_back(TempEnvPt);
		}

		MaxSlope_SX_Array.clear();
	}
	//DrawEnvPtArray(EnvPtArray);

	PLine1.clear();
	PLine2.clear();
}

//计算考虑折减之后的上下行坡度
void CProfileOpt::CalLimitGrade(ProfilePlan& Profile, double StartMileage, double EndMileage, double& MaxSlope_SX, double& MaxSlope_XX)
{
	double SlopeSml, SlopeEml;
	double LenInTun;
	double MinMileage, MaxMileage;
	int SlopeIndex;
	bool IsAft;

	MaxSlope_SX = m_MaxLongitudinalSlope;
	MaxSlope_XX = m_MaxLongitudinalSlope;

	if (StartMileage - EndMileage > 0.01)
		IsAft = false;
	else
		IsAft = true;

	SlopeSml = min(StartMileage, EndMileage);
	SlopeEml = max(StartMileage, EndMileage);

	StartMileage = SlopeSml;
	EndMileage = SlopeEml;

	//for (int i = 0; i < m_Station.size(); i++) //不考虑车站
	//{
	//	MinMileage = max(m_Station[i].stratMileage, SlopeSml);
	//	MaxMileage = min(m_Station[i].endMileage, SlopeEml);

	//	if (MaxMileage > MinMileage + 0.1)
	//	{
	//		MaxSlope_SX = m_StaMaxSlope;
	//		MaxSlope_XX = m_StaMaxSlope;
	//		return;
	//	}
	//}


	for (int i = 0; i < m_SlopePtSum; i++)
	{
		if (Profile.SlopePtArray[i].Mileage > StartMileage - 0.01)
		{
			if (fabs(Profile.SlopePtArray[i].Mileage - StartMileage) < 0.01)
			{
				if (IsAft)
				{
					SlopeSml = Profile.SlopePtArray[i].Mileage;
					SlopeEml = Profile.SlopePtArray[i + 1].Mileage;
					SlopeIndex = i;
				}
				else
				{
					SlopeSml = Profile.SlopePtArray[i - 1].Mileage;
					SlopeEml = Profile.SlopePtArray[i].Mileage;
					SlopeIndex = i - 1;
				}
			}
			else
			{
				SlopeSml = Profile.SlopePtArray[i - 1].Mileage;
				SlopeEml = Profile.SlopePtArray[i].Mileage;
				SlopeIndex = i - 1;
			}
			break;
		}
	}

	bool IsInTun = false;
	LenInTun = 0.0;
	for (int iTun = 0; iTun < m_section_Tun_Bri.size(); iTun++)
	{
		IsInTun = false;
		if (m_section_Tun_Bri[iTun].minS > SlopeEml)
			break;

		MinMileage = max(m_section_Tun_Bri[iTun].minS, SlopeSml);
		MaxMileage = min(m_section_Tun_Bri[iTun].maxS, SlopeEml);
		if (MaxMileage - MinMileage > 1.0)
			IsInTun = true;
		else if (SlopeSml - m_section_Tun_Bri[iTun].minS > -0.01 && SlopeSml - m_section_Tun_Bri[iTun].maxS < 0.01)
			IsInTun = true;
		else if (SlopeEml - m_section_Tun_Bri[iTun].minS > -0.01 && SlopeEml - m_section_Tun_Bri[iTun].maxS < 0.01)
			IsInTun = true;

		if (IsInTun)
		{
			if (m_section_Tun_Bri[iTun].maxS - m_section_Tun_Bri[iTun].minS > LenInTun)
			{
				LenInTun = m_section_Tun_Bri[iTun].maxS - m_section_Tun_Bri[iTun].minS;
			}
		}

	}
	double TunZJ = 1.0; //(待定)

	//if (LenInTun > 1.0)
	//{
	//	TunZJ = GetTunZJXS(LenInTun);
	//}

	MaxSlope_SX = TunZJ * m_MaxLongitudinalSlope; //- Profile.SlopePtArray[SlopeIndex + 1].QXZJ_SX / 1000.0;
	MaxSlope_XX = TunZJ * m_MaxLongitudinalSlope; //- Profile.SlopePtArray[SlopeIndex + 1].QXZJ_XX / 1000.0;

	//考虑坡度取整后的限制坡度
	double scl = pow(10.0, m_PdXSWS + 3);
	MaxSlope_SX = (int)(MaxSlope_SX*scl + 0.000001) / scl;
	MaxSlope_XX = (int)(MaxSlope_XX*scl + 0.000001) / scl;

}

////从线规中取隧道折减系数
//double CProfileOpt::GetTunZJXS(double tunlen)
//{
//	double zjxs = 1.0;
//	int i = 0;
//	if (m_pPlain->mGuiFanCS.ZheJian == true)//要折减
//	{
//		while (i<4 && m_pPlain->mGuiFanCS.SuiDaoFJ[i] > 1.0)
//		{
//			if (tunlen < m_pPlain->mGuiFanCS.SuiDaoFJ[i])
//				break;
//			i++;
//		}
//		if (m_pPlain->mGuiFanCS.QianYin == 1)
//			zjxs = m_pPlain->mGuiFanCS.SDXiShuD[i];
//		else if (m_pPlain->mGuiFanCS.QianYin == 2)
//			zjxs = m_pPlain->mGuiFanCS.SDXiShuN[i];
//	}
//	return zjxs;
//}

//用信封区域调整地面线数组
void CProfileOpt::GetDMXfromEnvelop(double Scml, double SLever) 
{
	GetJD(Scml, SLever);
	tempCRS.clear();
	TempCRS OneCrs;
	double MinH, MaxH;
	vector<double> MinHArray;
	MinHArray.clear();

	for (int iCrs = 0; iCrs < m_CrossSection.road[0].NCRS; iCrs++)
	{
		if (m_CrossSection.road[0].hdmdes[iCrs].cml > Scml - 0.01)
		{
			OneCrs.cml = m_CrossSection.road[0].hdmdes[iCrs].cml;
			OneCrs.EarH = m_CrossSection.road[0].hdmdes[iCrs].newLYd;
			GetEnvelopRange(SLever, -100.0, OneCrs.cml, MinH, MaxH);
			OneCrs.DesH = MaxH;
			MinHArray.push_back(MinH);//最小值

			tempCRS.push_back(OneCrs);//最大值
		}
	}


	vector<TempCRS>tempCRSarray;
	tempCRSarray.resize(tempCRS.size());
	int i;

	for (i = 0; i < tempCRS.size(); i++)
	{
		tempCRSarray[i] = tempCRS[i];
	}

	vector<TUNTAB> Tun_Array;
	int NTun = 0;
	AutoSetTUNforEnvelop(m_pPlain, m_MinTunFill, m_MinTunLeth, m_TunFill, NTun, Tun_Array); //最高点判断隧道
	

	if (m_TryNum == 0)
	{
		for (i = 0; i < tempCRS.size(); i++)
		{
			tempCRS[i].DesH = MinHArray[i];
		}

		vector<BRITAB> Bri_Array;//最低点判断桥梁
		int NBri = 0;
		AutoSetBRIforEnvelop(m_pPlain, m_MinBriFill, m_MinBriLeth, m_BriFill, NBri, Bri_Array);
	}

	subsection TempTun;
	bool IsInTun = false;
	int iTun;
	for (iTun = 0; iTun < m_section_Tun_Bri.size(); iTun++)
	{
		if (tempCRS[0].cml > m_section_Tun_Bri[iTun].minS && tempCRS[0].cml < m_section_Tun_Bri[iTun].maxS)
		{
			IsInTun = true;
			TempTun = m_section_Tun_Bri[iTun];
			break;
		}
	}
	m_section_Tun_Bri.clear();
	//m_section_Tun_Bri.SetSize(NTun+NBri);
	m_section_Tun_Bri.resize(NTun);
	int StartNum = 0;
	if (IsInTun && NTun > 0)
	{
		m_section_Tun_Bri[0] = TempTun;
		StartNum = 1;
	}
	BAS_DRAW_FUN ob;
	CString Skml, Ekml;
	for (i = StartNum; i < Tun_Array.size(); i++)
	{
		Skml = Tun_Array[i].SKml;
		Ekml = Tun_Array[i].EKml;
		double scml = m_pPlain->TYLC(Skml);
		double ecml = m_pPlain->TYLC(Ekml);
		m_section_Tun_Bri[i].minS = scml;
		m_section_Tun_Bri[i].maxS = ecml;
	}

	iTun = 0;
	if (m_section_Tun_Bri.size() > 0)
	{
		for (int j = 0; j < tempCRS.size(); j++)
		{
			if (iTun < m_section_Tun_Bri.size() && tempCRS[j].cml >= m_section_Tun_Bri[iTun].minS && tempCRS[j].cml <= m_section_Tun_Bri[iTun].maxS)
				tempCRS[j].EarH = tempCRSarray[j].DesH;
			else if (tempCRS[j].cml > m_section_Tun_Bri[iTun].maxS)
			{
				iTun++;
				if (iTun == m_section_Tun_Bri.size())
				{
					break;
				}
			}
		}
	}

	subsection temp;
	for (i = 0; i + 1 < m_section_Tun_Bri.size() ; i++)
	{
		for (int j = i; j < m_section_Tun_Bri.size(); j++)
		{
			if (m_section_Tun_Bri[i].minS > m_section_Tun_Bri[j].minS)
			{
				temp = m_section_Tun_Bri[i];
				m_section_Tun_Bri[i] = m_section_Tun_Bri[j];
				m_section_Tun_Bri[j] = temp;
			}
		}
	}
}

void CProfileOpt::GetJD(double Scml, double SLever)
{
	///////////////////////////////////////得到信封区域的交点//////////////////////////////////

	double QZminH = 0.0;//起终点确定的最小高程
	double QZmaxH = 0.0;//起终点确定的最大高程
	double x1 = Scml;
	double y1 = SLever;

	double maxx2 = m_Endml;
	double maxy2 = y1 + m_pPlain->XLLength*m_MaxLongitudinalSlope;

	double x3 = m_Endml;
	double y3 = m_EndDesH;

	double maxx4 = x1;
	double maxy4 = y3 + m_pPlain->XLLength*m_MaxLongitudinalSlope;

	double MaxY;
	BAS_DRAW_FUN::INTERS(&MaxX, &MaxY, x1, y1, maxx2, maxy2, x3, y3, maxx4, maxy4);//计算信封区域的最高点

	double minx2 = m_Endml;
	double miny2 = y1 - m_pPlain->XLLength*m_MaxLongitudinalSlope;

	double minx4 = x1;
	double miny4 = y3 - m_pPlain->XLLength*m_MaxLongitudinalSlope;

	double MinY;
	BAS_DRAW_FUN::INTERS(&MinX, &MinY, x1, y1, minx2, miny2, x3, y3, minx4, miny4);//计算信封区域的最低点

	// 	  ads_printf("minx=%lf,maxx=%lf\n",MinX,MaxX);
}

//得到信封区域所确定的里程范围
bool CProfileOpt::GetEnvelopRange(double Fronthigh, double FrontMileage, double Mileage, double& MinH, double& MaxH)
{
	MinH = 1e99;
	MaxH = -1e99;
	int MaxEnvPtSum = MaxEnvPtArray.size();
	int MinEnvPtSum = MinEnvPtArray.size();
	if (MaxEnvPtSum == 0 || MinEnvPtSum == 0)
		return false;

	int i = 0;

	if (Mileage < MaxEnvPtArray[0].Mileage || Mileage < MinEnvPtArray[0].Mileage)
	{
		MaxH = m_StartDesH;
		MinH = m_StartDesH;
		return false;
	}

	if (Mileage > MaxEnvPtArray[MaxEnvPtSum - 1].Mileage + 0.001 || Mileage > MinEnvPtArray[MinEnvPtSum - 1].Mileage + 0.001)
	{
		MaxH = m_EndDesH;
		MinH = m_EndDesH;
		return false;
	}

	for (i = 1; i < MaxEnvPtSum; i++)
	{
		if (Mileage < MaxEnvPtArray[i].Mileage)
		{
			if (fabs(MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage) < 0.1)
			{
				MaxH = MaxEnvPtArray[i].Lever;
			}
			else
			{
				double a, b, c, d, e;
				a = MaxEnvPtArray[i - 1].Lever;
				b = MaxEnvPtArray[i - 1].Mileage;
				c = MaxEnvPtArray[i].Lever;
				d = MaxEnvPtArray[i].Mileage;
				MaxH = MaxEnvPtArray[i - 1].Lever +
					(Mileage - MaxEnvPtArray[i - 1].Mileage) * (MaxEnvPtArray[i].Lever - MaxEnvPtArray[i - 1].Lever) / (MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage);
			}
			break;
		}
	}



	for (i = 1; i < MinEnvPtSum; i++)
	{
		if (Mileage < MinEnvPtArray[i].Mileage)
		{
			if (fabs(MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage) < 0.1)
			{
				MinH = MinEnvPtArray[i].Lever;
			}
			else
			{
				MinH = MinEnvPtArray[i - 1].Lever +
					(Mileage - MinEnvPtArray[i - 1].Mileage) * (MinEnvPtArray[i].Lever - MinEnvPtArray[i - 1].Lever) / (MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage);
			}
			break;
		}
	}

	if (FrontMileage > -0.1)
		JustEnvelopRangeByKZD(FrontMileage, Fronthigh, Mileage, MinH, MaxH);

	return true;
}

void CProfileOpt::JustEnvelopRangeByKZD(double FrontMileage, double Fronthigh, double NowMileage, double& MinH, double& MaxH)
{
	double MinPD = 1e99;
	double MaxPD = -1e99;
	bool IsHasKZD = false;
	double Temp;
	for (int i = 0; i < MaxEnvPtArray.size(); i++)
	{
		if (MaxEnvPtArray[i].Mileage > FrontMileage && MaxEnvPtArray[i].Mileage < NowMileage)
		{
			IsHasKZD = true;
			Temp = (MaxEnvPtArray[i].Lever - Fronthigh) / (NowMileage - FrontMileage);
			if (Temp < MinPD)
				MinPD = Temp;
		}
	}

	if (IsHasKZD)
	{
		Temp = Fronthigh + MinPD * (NowMileage - FrontMileage);
		MaxH = min(Temp, MaxH);

	}

	IsHasKZD = false;

	for (int i = 0; i < MinEnvPtArray.size(); i++)
	{
		if (MinEnvPtArray[i].Mileage > FrontMileage && MinEnvPtArray[i].Mileage < NowMileage)
		{
			IsHasKZD = true;
			Temp = (MinEnvPtArray[i].Lever - Fronthigh) / (NowMileage - FrontMileage);
			if (Temp > MaxPD)
				MaxPD = Temp;
		}
	}

	if (IsHasKZD)
	{
		Temp = Fronthigh + MaxPD * (NowMileage - FrontMileage);
		MinH = max(Temp, MinH);
	}

}

void CProfileOpt::AutoSetTUNforEnvelop(JD_CENTER *pz, double Th, double MinLen, double STH, int& NTun, vector<TUNTAB> &Tun_Array)//自动设置隧道
{
	Tun_Array.clear();
	TUNTAB TempTun;
	NTun = 0;
	Th = -1.0*Th;
	STH = -1.0*STH;
	//计算填挖高
	double TunMl[500][2];
	double sml, eml, len, Btwh;
	ACHAR GH[8];
	int i;
	for (i = 0; i < tempCRS.size(); i++)
	{
		double cml = tempCRS[i].cml;
		double earH = tempCRS[i].EarH;
		double desh = tempCRS[i].DesH;
		double twh = desh - earH;
		if (twh <= Th)//需要设置隧道
		{
			//找起点
			if (i < 1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh = twh;
				while (n > 0 && Stwh < STH)//找终点
				{
					Btwh = Stwh;
					n--;
					cml = tempCRS[n].cml;
					earH = tempCRS[n].EarH;
					desh = tempCRS[n].DesH;
					Stwh = desh - earH;
				}
				if (Stwh > STH)
					sml = cml + (tempCRS[n + 1].cml - cml)*(STH - Stwh) / (Btwh - Stwh);
				else
					sml = cml;
				sml = fabs(sml - cml) < fabs(tempCRS[n + 1].cml - sml) ? cml : tempCRS[n + 1].cml;

			}

			while (i < tempCRS.size() - 1 && twh <= STH)//找终点
			{
				Btwh = twh;
				i++;
				cml = tempCRS[i].cml;
				earH = tempCRS[i].EarH;
				desh = tempCRS[i].DesH;
				twh = desh - earH;
			}
			if (twh > Th && i < tempCRS.size() - 1)
				eml = tempCRS[i - 1].cml + (cml - tempCRS[i - 1].cml)*(STH - Btwh) / (twh - Btwh);
			else
				eml = tempCRS[i].cml;
			eml = fabs(eml - cml) < fabs(tempCRS[i - 1].cml - eml) ? cml : tempCRS[i - 1].cml;

			len = eml - sml;
			if (len > MinLen)//隧道长至少20m
			{
				////TempTun.StDml = XLC1(sml, GH, pz->NDL, pz->DLB);
				//TempTun.StDml = pz->XLC1(sml);
				//TempTun.BGH = GH;
				//_tcscpy(TempTun.SKml, LCchr(GH, TempTun.StDml, 3, 1));
				////TempTun.EdDml = XLC1(eml, GH, pz->NDL, pz->DLB);
				//TempTun.EdDml = pz->XLC1(eml);
				//TempTun.EGH = GH;
				//_tcscpy(TempTun.EKml, LCchr(GH, TempTun.EdDml, 3, 1));
				//TempTun.TunName.Format(L"隧道%d", NTun + 1);

				_tcscpy(GH, L"K");
				TempTun.StDml = sml;
				TempTun.BGH = L"K";
				_tcscpy(TempTun.SKml, LCchr(GH, TempTun.StDml, 3, 1));
				TempTun.EdDml = eml;
				TempTun.EGH = L"K";
				_tcscpy(TempTun.EKml, LCchr(GH, TempTun.EdDml, 3, 1));
				TempTun.TunName.Format(L"隧道%d", NTun + 1);

				TunMl[NTun][0] = sml, TunMl[NTun][1] = eml;
				NTun++;
				Tun_Array.push_back(TempTun);
			}
		}
		Btwh = twh;
	}
}

//自动设计桥梁
void CProfileOpt::AutoSetBRIforEnvelop(JD_CENTER* pz, double Th, double MinLen, double STH, int& NBri, vector<BRITAB> &Bri_Array)
{
	// TODO: Implement the command
	//计算填挖高
	Bri_Array.clear();
	double BriMl[500][2];
	double sml, eml, len, Btwh;
	ACHAR GH[8];
	int i;
	double cml = 0.0;
	double earH = 0.0;
	double desh = 0.0;
	double twh = 0.0;

	double MinH, MinHCml;//最小地面线标高，最小地面线标高处的里程
	MinH = 1e99;
	double AveH = 0.0;
	int PtNum = 0;
	m_AutoSetBriGCKZDArray.clear();

	BRITAB tempBri;

	NBri = 0;
	for (i = 0; i < tempCRS.size(); i++)
	{
		cml = tempCRS[i].cml;
		earH = tempCRS[i].EarH;
		desh = tempCRS[i].DesH;
		twh = desh - earH;
		if (twh >= Th)//可能设桥
		{
			//找起点
			if (i < 1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh = twh;
				while (n > 0 && Stwh > STH)//找起点
				{
					Btwh = Stwh;
					n--;
					cml = tempCRS[n].cml;
					earH = tempCRS[n].EarH;
					desh = tempCRS[n].DesH;
					Stwh = desh - earH;
				}
				if (Stwh < STH)
					sml = cml + (tempCRS[n + 1].cml - cml)*(STH - Stwh) / (Btwh - Stwh);
				else
					sml = cml;
				sml = fabs(sml - cml) < fabs(tempCRS[n + 1].cml - sml) ? cml : tempCRS[n + 1].cml;
			}
			MinHCml = sml;

			while (i<tempCRS.size() - 1 && twh>STH)//找终点
			{
				Btwh = twh;
				i++;
				cml = tempCRS[i].cml;
				earH = tempCRS[i].EarH;
				desh = tempCRS[i].DesH;
				twh = desh - earH;

				if (earH < MinH)
				{
					MinH = earH;
					MinHCml = cml;
				}

				AveH += earH;
				PtNum++;
			}
			if (twh < STH)
				eml = cml - (cml - tempCRS[i - 1].cml)*(STH - twh) / (Btwh - twh);
			else
				eml = tempCRS[i].cml;

			eml = fabs(eml - cml) < fabs(tempCRS[i - 1].cml - eml) ? cml : tempCRS[i - 1].cml;


			len = eml - sml;
			if (len > MinLen)//桥长至少10m
			{

				//tempBri.StDml = pz->XLC1(sml);
				//tempBri.BGH = GH;
				//_tcscpy(tempBri.SKml, LCchr(GH, tempBri.StDml, 3, 1));
				////tempBri.EdDml = XLC1(eml, GH, pz->NDL, pz->DLB);
				//tempBri.EdDml = pz->XLC1(eml);
				//tempBri.EGH = GH;
				//_tcscpy(tempBri.EKml, LCchr(GH, tempBri.EdDml, 3, 1));
				///*tempBri.CenDml = XLC1(0.5*(sml + eml), GH, pz->NDL, pz->DLB);*/
				//tempBri.CenDml = pz->XLC1(0.5*(sml + eml));
				//tempBri.CGH = GH;
				//_tcscpy(tempBri.CKml, LCchr(GH, tempBri.CenDml, 3, 1));
				//tempBri.BriLen = eml - sml;
				///*tempBri.BriName.Format(L"桥梁%d",NBri+1);*/
				//tempBri.BriName.Format(L"桥梁%d", NBri + 1);

				_tcscpy(GH, L"K");
				tempBri.StDml = sml;
				tempBri.BGH = L"K";
				_tcscpy(tempBri.SKml, LCchr(GH, tempBri.StDml, 3, 1));
				tempBri.EdDml = eml;
				tempBri.EGH = L"K";
				_tcscpy(tempBri.EKml, LCchr(GH, tempBri.EdDml, 3, 1));
				tempBri.CenDml = 0.5*(sml + eml);
				tempBri.CGH = L"K";
				_tcscpy(tempBri.CKml, LCchr(GH, tempBri.CenDml, 3, 1));
				tempBri.BriLen = eml - sml;
				tempBri.BriName.Format(L"桥梁%d", NBri + 1);

				BriMl[NBri][0] = sml, BriMl[NBri][1] = eml;

				NBri++;
				Bri_Array.push_back(tempBri);

				if (m_TryNum == 0)
				{
					bool IsSetKZDInBri = false;
					for (int iKZD = 0; iKZD < NGCKZD; iKZD++)//检测用户是否已经在桥梁范围内设置控制点，如已设置，不自动设置控制点
					{
						if (GCKZDTAB[iKZD].Cml > sml && GCKZDTAB[iKZD].Cml < eml)
						{
							IsSetKZDInBri = true;
							break;
						}
					}
					for (int iKZD = 0; iKZD < m_BriGCKZDArray.size(); iKZD++)//检测用户是否已经在桥梁范围内设置控制点，如已设置，不自动设置控制点
					{
						if (m_BriGCKZDArray[iKZD].Cml > sml && m_BriGCKZDArray[iKZD].Cml < eml)
						{
							IsSetKZDInBri = true;
							break;
						}
					}
					if (!IsSetKZDInBri && len > 20.0 && Bri_Array.size()>0)//500.0
					{
						//////////////////////////////////////////////////////////////////////////
						//自动增加一个上限控制标高
						GCKZPt BriGCKZD;
						BriGCKZD.Cml = 0.5 * (eml + sml);
						AveH = AveH / PtNum;
						BriGCKZD.DesHSX = MinH + m_MaxBriTwg;
						BriGCKZD.DesHXX = 0.0;
						BriGCKZD.Style = 1;
						m_AutoSetBriGCKZDArray.push_back(BriGCKZD);
					}
				}


				MinH = 1e99;
				PtNum = 0;
				AveH = 0.0;

			}

		}
		Btwh = twh;
	}
}

//找有效地面线范围
void CProfileOpt::GetDMXRange()
{
	PsDmx(m_MinLongitudinalSlopeL, 10);//平顺地面线
	DMX_SubSection TempSection;
	bool FindMax = false;
	bool FindMin = false;
	for (int i = 0; i < PSdmxPt.size() - 1; i++)
	{
		if (i == 0)
		{
			if (PSdmxPt[i].high > PSdmxPt[i + 1].high)
			{
				FindMin = true;
			}
			else
			{
				FindMax = true;
			}
			TempSection.StratMileage = PSdmxPt[0].mile;
			TempSection.StratDesH = PSdmxPt[0].high;
		}

		if (FindMax == true)//找最大值
		{
			if (PSdmxPt[i].high > PSdmxPt[i + 1].high)
			{
				TempSection.EndMileage = PSdmxPt[i].mile;
				TempSection.EndDesH = PSdmxPt[i].high;
				TempSection.Lenth = TempSection.EndMileage - TempSection.StratMileage;
				FindMax = false;
				FindMin = true;

				m_dmx_SubSection.push_back(TempSection);
				TempSection.StratMileage = TempSection.EndMileage;
				TempSection.StratDesH = TempSection.EndDesH;
				continue;
			}

		}

		if (FindMin == true)//找最小值
		{
			if (PSdmxPt[i].high < PSdmxPt[i + 1].high)
			{
				TempSection.EndMileage = PSdmxPt[i].mile;
				TempSection.EndDesH = PSdmxPt[i].high;
				TempSection.Lenth = TempSection.EndMileage - TempSection.StratMileage;
				FindMin = false;
				FindMax = true;

				m_dmx_SubSection.push_back(TempSection);
				TempSection.StratMileage = TempSection.EndMileage;
				TempSection.StratDesH = TempSection.EndDesH;
				continue;
			}
		}
	}

	for (int j = 0; j < m_dmx_SubSection.size(); j++)
	{
		if (m_dmx_SubSection[j].Lenth < m_MinLongitudinalSlopeL)
		{
			m_dmx_SubSection.erase(m_dmx_SubSection.begin() + j);
			j--;
		}
	}
}

//平顺地面线；
void CProfileOpt::PsDmx(double ps_R, int ps_N)
{
	int i, j;
	gridPoint tmp;
	tmp.mile = 0.00; tmp.high = 0.00;

	PSdmxPt.clear();
	// 	PSdmxPt.Add(tmp);

	for (i = 0; i < ZMdmxPt.size(); i++)    //第一次平顺
	{
		tmp.mile = ZMdmxPt[i].mile;
		tmp.high = GetPsHigh(i, ps_R);
		PSdmxPt.push_back(tmp);
	}

	for (i = 1; i < ps_N; i++)                 //如果平顺次数大于1，继续平顺
	{
		for (j = 0; j < PSdmxPt.size(); j++)
		{
			tmp.mile = PSdmxPt[j].mile;
			tmp.high = GetPsHigh(j, ps_R, 1);
			//PSdmxPt.push_back(tmp);
			PSdmxPt[j] = tmp;
		}
	}

}

//求得某点的平顺后高程
double CProfileOpt::GetPsHigh(int i, double R)
{
	double totalL(0.0), totalR(0.0), dltX(0.0);
	double FLeft(0.0), FRight(0.0);
	double high;
	int j;
	//计算左侧
	j = 1;
	if (i - j > 0)
	{
		dltX = fabs(ZMdmxPt[i - j].mile - ZMdmxPt[i].mile);
	}

	while (i - j > 0 && dltX <= R)
	{
		totalL += ZMdmxPt[i-j].high * (R - dltX) / R;
		FLeft += (R - dltX) / R;
		j++;
		if (i - j > 0)
		{
			dltX = fabs(ZMdmxPt[i - j].mile - ZMdmxPt[i].mile);
		}
	}

	dltX = 0.0;
	//计算右侧
	j = 1;
	if (i + j <= ZMdmxPt.size() - 1)
	{
		dltX = fabs(ZMdmxPt[i + j].mile - ZMdmxPt[i].mile);
	}

	while (i + j <= ZMdmxPt.size() - 1 && dltX <= R)
	{
		totalR += ZMdmxPt[i + j].high * (R - dltX) / R;
		FRight += (R - dltX) / R;
		j++;
		if (i + j <= ZMdmxPt.size() - 1)
		{
			dltX = fabs(ZMdmxPt[i + j].mile - ZMdmxPt[i].mile);
		}
	}
	//求得高程
	high = (totalL + totalR + ZMdmxPt[i].high) / (FLeft + FRight + 1.0);
	return high;
}

//求得某点的平顺后高程
double CProfileOpt::GetPsHigh(int i, double R, int tp)
{
	double totalL(0.0), totalR(0.0), dltX(0.0);
	double FLeft(0.0), FRight(0.0);
	double high;
	int j;
	//计算左侧
	j = 1;
	if (i - j > 0)
	{
		dltX = fabs(PSdmxPt[i - j].mile - PSdmxPt[i].mile);
	}

	while (i - j > 0 && dltX <= R)
	{
		totalL += PSdmxPt[i - j].high * (R - dltX) / R;
		FLeft += (R - dltX) / R;
		j++;
		if (i - j > 0)
		{
			dltX = fabs(PSdmxPt[i - j].mile - PSdmxPt[i].mile);
		}
	}

	dltX = 0.0;
	//计算右侧
	j = 1;
	if (i + j <= PSdmxPt.size() - 1)
	{
		dltX = fabs(PSdmxPt[i + j].mile - PSdmxPt[i].mile);
	}

	while (i + j <= PSdmxPt.size() - 1 && dltX <= R)
	{
		totalR += PSdmxPt[i + j].high * (R - dltX) / R;
		FRight += (R - dltX) / R;
		j++;
		if (i + j <= PSdmxPt.size() - 1)
		{
			dltX = fabs(PSdmxPt[i + j].mile - PSdmxPt[i].mile);
		}
	}
	//求得高程
	high = (totalL + totalR + PSdmxPt[i].high) / (FLeft + FRight + 1.0);
	return high;
}

//得到缓和曲线和车站的分段
void CProfileOpt::GetTransitionAndStation()
{
	int n = 0;
	m_subsection.clear();
	m_subsection.resize(2 * m_pPlain->JdCenNum - 4);
	for (int i = 1; i < m_pPlain->JdCenNum - 1; i++)
	{
		m_subsection[n].minS = m_pPlain->QXB[i][0];
		m_subsection[n].maxS = m_pPlain->QXB[i][1];
		m_subsection[n].Style = 1;
		n = n + 1;

		m_subsection[n].minS = m_pPlain->QXB[i][2];
		m_subsection[n].maxS = m_pPlain->QXB[i][3];
		m_subsection[n].Style = 1;
		n = n + 1;
	}

	SectionLine();
}

void CProfileOpt::SectionLine()//线路分段，赋限坡值 
{
	XLDataBase_JYX xldata;
	xldata.Read_XLDbs(m_PlanMDB, L"车站",m_RoadName);
	int num = xldata.StaNum;
	m_Station.clear();
	m_Station.resize(num);

	ACHAR temp1[80];
	ACHAR temp[8];
	CString kml;
	double cml = 0.0;
	if (num > 0)
	{
		for (int i = 0; i < num; i++)
		{
			_tcscpy(temp, xldata.Sta[i].BGH);
			_tcscpy(temp1, LCchr(temp, xldata.Sta[i].StDml, 3, 1));
			kml = temp1;
			cml = m_pPlain->TYLC(kml);
			if (cml < m_Startml + 0.00001 && m_Xlstye == -1)
				m_Station[i].stratMileage = m_Startml;
			else
				m_Station[i].stratMileage = cml;

			_tcscpy(temp, xldata.Sta[i].EGH);
			_tcscpy(temp1, LCchr(temp, xldata.Sta[i].EdDml, 3, 1));
			kml = temp1;
			cml = m_pPlain->TYLC(kml);
			if (cml > m_Endml + 0.00001 && m_Xlstye == -1)
				m_Station[i].endMileage = m_Endml;
			else
				m_Station[i].endMileage = cml;

			m_Station[i].XP = m_StaMaxSlope;
		}
	}
}


//初始化方案
void CProfileOpt::GetFirstPlan(int PopulationSize)
{
	if (m_ProfilePlanArray.size() > 0)
	{
		for (int i = 0; i < m_ProfilePlanArray.size(); i++)
		{
			m_ProfilePlanArray[i].SlopePtArray.clear();
			m_ProfilePlanArray[i].BriArray.clear();
			m_ProfilePlanArray[i].TunArray.clear();
		}
		m_ProfilePlanArray.clear();
	}

	m_ProfilePlanArray.resize(PopulationSize);
	double xl_length = m_Endml - m_Startml;
	if (m_IsProNowPlan) //优化现有方案
		m_SlopePtSum = m_SlopeArray.size();
	else
		m_SlopePtSum = int(xl_length / m_MinLongitudinalSlopeL) + 1;
	for (int ProfileIndex = 0; ProfileIndex < PopulationSize; ProfileIndex++)
	{
		m_ProfilePlanArray[ProfileIndex].S_high = 0.0;
		m_ProfilePlanArray[ProfileIndex].X_high = 0.0;
		m_ProfilePlanArray[ProfileIndex].Satisfaction = 0.0;

		m_ProfilePlanArray[ProfileIndex].Cost = 0.0;
		m_ProfilePlanArray[ProfileIndex].Fitness = 0;
		m_ProfilePlanArray[ProfileIndex].ProfileIndex = ProfileIndex;
		m_ProfilePlanArray[ProfileIndex].SelProbability = 0.0;

		m_ProfilePlanArray[ProfileIndex].SlopePtArray.resize(m_SlopePtSum);
		for (int SlopePtIndex = 0; SlopePtIndex < m_SlopePtSum; SlopePtIndex++)
		{
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].Mileage = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].VerticalCurveRadius = m_VerticalCurveRadius;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].DesH = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].Gradient = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].Style = 1;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].LimitGrade = m_MaxLongitudinalSlope;//所有坡度均赋值为最大坡度值
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].QXZJ_SX = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].QXZJ_XX = 0.0;

		}
		m_ProfilePlanArray[ProfileIndex].SlopePtArray[0].VerticalCurveRadius = 0.0;
		m_ProfilePlanArray[ProfileIndex].SlopePtArray[m_SlopePtSum - 1].VerticalCurveRadius = 0.0;
	}
}

void CProfileOpt::GetPlan()
{
	if (m_SlopeArray.size() <= 0)
		return;
	for (int i=0;i< m_SlopeArray.size();i++)
	{
		m_ProfilePlanArray[0].SlopePtArray[i].Mileage = m_SlopeArray[i].ml; //里程
		//m_ProfilePlanArray[0].SlopePtArray[i].VerticalCurveRadius = m_SlopeArray[i].Rshu; //竖曲线
		m_ProfilePlanArray[0].SlopePtArray[i].DesH = m_SlopeArray[i].Level; //高程
		//m_ProfilePlanArray[0].SlopePtArray[i].Gradient = m_SlopeArray[i].degree; //坡度
	}
	m_SlopePtSum = m_ProfilePlanArray[0].SlopePtArray.size();
	for (int i = 0; i < m_SlopePtSum; i++)//给坡段性质赋值
	{
		m_ProfilePlanArray[0].SlopePtArray[i].Style = 1;
	}


	////给站坪坡性质赋值
	//for (int i = 0; i < m_Station.GetSize(); i++)
	//{
	//	for (int j = 0; j < m_SlopePtSum - 1; j++)
	//	{
	//		if (m_Station[i].stratMileage + 0.0001 > m_ProfilePlanArray[0].SlopePtArray[j].Mileage && m_Station[i].endMileage < m_ProfilePlanArray[0].SlopePtArray[j + 1].Mileage + 0.0001)
	//		{
	//			m_ProfilePlanArray[0].SlopePtArray[j + 1].Style = 0;//站坪段,用后一个变坡点表示的
	//			break;
	//		}
	//	}
	//}

	//CompensationGrade(m_ProfilePlanArray[0]);//按曲线折减坡度最大值
}

//给整个种群的里程赋值
void CProfileOpt::CreateProfileMileage(bool ContinueOrStrat)
{
	int tempIndex = 0;
	if (ContinueOrStrat == false)
		tempIndex = 1;

	for (int VerticalIndex = tempIndex; VerticalIndex < m_PopulationSize; VerticalIndex++)
	{
		// CreateSloptPtMileage(VerticalIndex);
		GetPlanPtMileage(VerticalIndex);
	}
}

//利用地面线生成第索引为VerticalIndex的方案里程
void CProfileOpt::GetPlanPtMileage(int VerticalIndex)
{
	if (VerticalIndex == 0)
	{
		for (int i = 0; i < ChangePt.size(); i++)
		{
			m_ProfilePlanArray[0].SlopePtArray[i].Mileage = ChangePt[i].mile;
		}

		JudgeSection();//调整小于最小坡段长的坡段
		//if (m_Xlstye == -1)
		//	AdjustSlopeBySta(m_ProfilePlanArray[0]);//按车站位置调整坡段 (不考虑)

		if (m_BpdLC != 0)//变坡点里程取整问题
		{
			AdjustSlopeMileage(m_ProfilePlanArray[0]);
		}

		//AdjustSlopeByHHQX(m_ProfilePlanArray[0]);//按缓和曲线调整坡段 (不考虑)
		 
		//CompensationGrade(m_ProfilePlanArray[0]);//按曲线折减坡度最大值 (不考虑)(待定)

		GetMaxGrade(m_ProfilePlanArray[0]);

		//////////////////////////////用信封区域调整地面线，平顺地面线//////////////////////////////////////////


		//if (m_Xlstye != -1)//为绕行段时，对里程数组进行处理 (不考虑)
		//{
		//	GetRxdDesh();
		//	GetRxdMileageArray();
		//}

		////线路终点在车站上时，最后一个坡段不参与优化
		//if (m_ProfilePlanArray[0].SlopePtArray[m_ProfilePlanArray[0].SlopePtArray.size() - 1].Style == 0)
		//{
		//	int slopnum = m_ProfilePlanArray[0].SlopePtArray.size();
		//	temp_mileage = m_ProfilePlanArray[0].SlopePtArray[slopnum - 1].Mileage;
		//	m_ProfilePlanArray[0].SlopePtArray.erase(m_ProfilePlanArray[0].SlopePtArray.begin() + slopnum - 1);//去掉终点

		//	GetGCKZD(m_ProfilePlanArray[0].SlopePtArray[0].Mileage, m_StartDesH, m_ProfilePlanArray[0].SlopePtArray[slopnum - 1].Mileage, m_EndDesH);
		//	
		//}
	}
	else
	{
		m_ProfilePlanArray[VerticalIndex].SlopePtArray = m_ProfilePlanArray[0].SlopePtArray;
	}
}

// 调整初始方案的分段
void CProfileOpt::JudgeSection()
{
	int TempSlopeNum = 0;
	double TempFrontLenth = 0.0;
	double TempNowLenth = 0.0;
	double TempBackLenth = 0.0;
	for (int i = 0; i < m_ProfilePlanArray[0].SlopePtArray.size() - 1; i++)
	{

		if (i == 0)
		{
			TempNowLenth = m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage - m_ProfilePlanArray[0].SlopePtArray[i].Mileage;
			TempBackLenth = m_ProfilePlanArray[0].SlopePtArray[i + 2].Mileage - m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage;
			if (TempNowLenth < m_MinLongitudinalSlopeL)
			{
				double tempLenth = m_MinLongitudinalSlopeL - TempNowLenth;
				if (TempBackLenth - tempLenth >= m_MinLongitudinalSlopeL)
				{
					m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage = m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage + tempLenth;
				}
				else
				{
					m_ProfilePlanArray[0].SlopePtArray.erase(m_ProfilePlanArray[0].SlopePtArray.begin() + i + 1);
					i--;
				}
			}
		}
		else if (i == m_ProfilePlanArray[0].SlopePtArray.size() - 2)
		{
			TempNowLenth = m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage - m_ProfilePlanArray[0].SlopePtArray[i].Mileage;
			TempFrontLenth = m_ProfilePlanArray[0].SlopePtArray[i].Mileage - m_ProfilePlanArray[0].SlopePtArray[i - 1].Mileage;

			if (TempNowLenth < m_MinLongitudinalSlopeL)
			{
				double tempLenth = m_MinLongitudinalSlopeL - TempNowLenth;
				if (TempFrontLenth - tempLenth >= m_MinLongitudinalSlopeL)
				{
					m_ProfilePlanArray[0].SlopePtArray[i].Mileage = m_ProfilePlanArray[0].SlopePtArray[i].Mileage - tempLenth;
				}
				else
				{
					m_ProfilePlanArray[0].SlopePtArray.erase(m_ProfilePlanArray[0].SlopePtArray.begin() + i);
					i--;
				}
			}
		}
		else
		{
			TempFrontLenth = m_ProfilePlanArray[0].SlopePtArray[i].Mileage - m_ProfilePlanArray[0].SlopePtArray[i - 1].Mileage;
			TempNowLenth = m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage - m_ProfilePlanArray[0].SlopePtArray[i].Mileage;
			TempBackLenth = m_ProfilePlanArray[0].SlopePtArray[i + 2].Mileage - m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage;
			double temp = m_ProfilePlanArray[0].SlopePtArray[i].Mileage;
			if (TempNowLenth < m_MinLongitudinalSlopeL)
			{
				double tempLenth = m_MinLongitudinalSlopeL - TempNowLenth;
				double maxLenth = max(TempFrontLenth, TempBackLenth);
				if (maxLenth - tempLenth >= m_MinLongitudinalSlopeL)//可以调整
				{
					if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，则当前坡段往前移动
					{
						m_ProfilePlanArray[0].SlopePtArray[i].Mileage = m_ProfilePlanArray[0].SlopePtArray[i].Mileage - tempLenth;
					}
					else//当前坡段往后移动
					{
						m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage = m_ProfilePlanArray[0].SlopePtArray[i + 1].Mileage + tempLenth;
					}
				}
				else
				{
					if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，合并到后一坡段
					{
						m_ProfilePlanArray[0].SlopePtArray.erase(m_ProfilePlanArray[0].SlopePtArray.begin() + i + 1);
					}
					else
					{
						m_ProfilePlanArray[0].SlopePtArray.erase(m_ProfilePlanArray[0].SlopePtArray.begin() + i);
					}
					i--;
				}
			}

		}
	}
}

void CProfileOpt::AdjustSlopeMileage(ProfilePlan& Plan)//调整方案里程取整
{
	double FrontMileage = 0.0;
	double NowMileage = 0.0;
	double BackMileage = 0.0;
	double ModifyFrontMileage = 0.0;//往前移后的里程
	double ModifyBackMileage = 0.0;//往后移动后的里程
	for (int i = 1; i < Plan.SlopePtArray.size() - 1; i++)
	{
		FrontMileage = Plan.SlopePtArray[i - 1].Mileage;
		BackMileage = Plan.SlopePtArray[i + 1].Mileage;
		NowMileage = Plan.SlopePtArray[i].Mileage;
		double NowLC = Plan.SlopePtArray[i].Mileage;

		int temp = 0;//商
		double temp1 = 0.0;//余数
		double temp2 = 0.0;//取多少的倍数
		if (m_BpdLC == 1)//取10m的倍数
			temp2 = 10;
		else if (m_BpdLC == 2)//取50m的倍数
			temp2 = 50;
		else if (m_BpdLC == 3)//取100m的倍数
			temp2 = 100;

		temp = NowLC / temp2;
		temp1 = NowLC - temp * temp2;
		if (Plan.SlopePtArray[i].Style == 0 || Plan.SlopePtArray[i + 1].Style == 0)//前或后有一个站坪坡
		{
			if (Plan.SlopePtArray[i].Style == 0)//前一个坡段为站坪坡,只能往后调整
			{
				ModifyBackMileage = (temp + 1)*temp2;
				if (BackMileage - ModifyBackMileage < m_MinLongitudinalSlopeL)//调整后，后一个坡段小于最小坡段长，则删除当前变坡点
				{
					if (i < Plan.SlopePtArray.size() - 1)
					{
						Plan.SlopePtArray[i + 1].Mileage = ModifyBackMileage;
					}
					Plan.SlopePtArray.erase(Plan.SlopePtArray.begin() + i);
					i--;
				}
				else
					Plan.SlopePtArray[i].Mileage = ModifyBackMileage;
			}
			else//后一个坡段为站坪坡,只能往前调整
			{
				ModifyFrontMileage = temp * temp2;
				if (ModifyFrontMileage - FrontMileage < m_MinLongitudinalSlopeL)//调整后，前一个坡段小于最小坡段长，则删除当前变坡点
				{
					if (i > 1)
					{
						Plan.SlopePtArray[i - 1].Mileage = ModifyFrontMileage;
					}
					Plan.SlopePtArray.erase(Plan.SlopePtArray.begin() + i);
					i--;
				}
				else
					Plan.SlopePtArray[i].Mileage = ModifyFrontMileage;
			}
		}
		else//前后均不是站坪坡
		{
			//将变坡点移到较近的整桩上
			if (temp1 > temp2 / 2.0)//往后移动
			{
				ModifyBackMileage = (temp + 1)*temp2;
				if (BackMileage - ModifyBackMileage > m_MinLongitudinalSlopeL)
					Plan.SlopePtArray[i].Mileage = ModifyBackMileage;
				else//往反方向移动
				{
					ModifyFrontMileage = temp * temp2;
					if (ModifyFrontMileage - FrontMileage < m_MinLongitudinalSlopeL)//不满足最小坡段长的要求
					{
						Plan.SlopePtArray.erase(Plan.SlopePtArray.begin() + i);
						i--;
					}
					else
						Plan.SlopePtArray[i].Mileage = ModifyFrontMileage;
				}
			}
			else//往前移动
			{
				ModifyFrontMileage = temp * temp2;
				if (ModifyFrontMileage - FrontMileage > m_MinLongitudinalSlopeL)
					Plan.SlopePtArray[i].Mileage = ModifyFrontMileage;
				else//往反方向移动
				{
					ModifyBackMileage = (temp + 1)*temp2;
					if (BackMileage - ModifyBackMileage < m_MinLongitudinalSlopeL)//不满足最小坡段长的要求
					{
						Plan.SlopePtArray.erase(Plan.SlopePtArray.begin() + i);
						i--;
					}
					else
						Plan.SlopePtArray[i].Mileage = ModifyBackMileage;
				}
			}
		}
	}
}

//坡度折减后，计算整个线路的最大限坡（折减后整条线路的最大限坡一定会小于规定的最大限坡值）
void CProfileOpt::GetMaxGrade(ProfilePlan& TempPlan)
{
	double DeshSum = 0.0;
	int SlopNum = TempPlan.SlopePtArray.size();
	for (int ISlop = 1; ISlop < SlopNum; ISlop++)
	{
		double temp_mileage = TempPlan.SlopePtArray[ISlop].Mileage - TempPlan.SlopePtArray[ISlop - 1].Mileage;
		DeshSum = DeshSum + temp_mileage * TempPlan.SlopePtArray[ISlop].LimitGrade;
	}

	double len = TempPlan.SlopePtArray[SlopNum - 1].Mileage - TempPlan.SlopePtArray[0].Mileage;

}

//用两种方法生成初始方案
void CProfileOpt::CreatPlanByTwoMethod(bool ContinueOrStrat)
{
	int temp = 0;
	if (m_PopulationSize / 2 > 30)
		temp = 30;
	else
		temp = m_PopulationSize / 2;

	int TempNum = 1;
	int TempIndex = 0;
	if (ContinueOrStrat == false)//如果是优化现有方案，则从第二个方案开始生成
		TempIndex = 1;

	ACHAR pszLabel[80];
	_tcscpy(pszLabel, L"生成初始方案群");
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);
	m_BriGCKZDArray.clear();

	Thread_Num = temp;

	m_TryNum = 0;
	for (int VerticalIndex = 0; VerticalIndex < m_PopulationSize; VerticalIndex++)
	{
		CreatPlanMiu(VerticalIndex);

		if (TempStopCreat)
			break;

		if (VerticalIndex > -1)
			acedSetStatusBarProgressMeterPos(VerticalIndex*100.0 / m_PopulationSize);
	}
	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
}

//按期望生成方案
void CProfileOpt::CreatPlanMiu(int &VerticalIndex)
{
	bool temp = true;
	if (VerticalIndex == 0)
	{
		temp = IniProfile(VerticalIndex);
		double FillArea, CutArea;
		CalEarthWork(m_ProfilePlanArray[0], FillArea, CutArea);
		m_TryNum++;
		temp = IniProfile(VerticalIndex);
	}
	else
	{
		m_TryNum++;
		bool temp = IniProfile(VerticalIndex);
	}

	if (!temp)
	{
		VerticalIndex--;
	}

}

//初始化一个纵断面方案
bool CProfileOpt::IniProfile(int VerticalIndex)
{
	m_SlopePtSum = m_ProfilePlanArray[0].SlopePtArray.size();
	for (int SlopePtIndex = 0; SlopePtIndex < m_SlopePtSum; SlopePtIndex++)
	{
		if (SlopePtIndex == 0)//起点
			m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].DesH = m_StartDesH;
		else if (SlopePtIndex == m_SlopePtSum - 1)//终点
			m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].DesH = m_EndDesH;
		else
		{
			bool tempD = CreateSloptPtDesH(VerticalIndex, SlopePtIndex);
			if (tempD == false)
				return false;
		}

	}
	return true;
}

//生成第索引为SlopePtIndex的変坡点设计高程
bool CProfileOpt::CreateSloptPtDesH(int VerticalIndex, int SlopePtIndex)
{
	//////////////////////////////////////////////////////////////////////////
	//在这里添加代码
	double minH, maxH;
	double temp;
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	FrontMileage = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 1].Mileage;
	Fronthigh = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 1].DesH;
	NowMileage = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].Mileage;
	//ads_printf("第%d方案，%d变坡点，里程%lf，前变坡点里程%lf,变坡点标高%lf\n",VerticalIndex,SlopePtIndex, NowMileage,FrontMileage,Fronthigh);

	double MiuDesH = 0.0;

	GetGCKZD(m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 1].Mileage,
		m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 1].DesH,
		m_Endml,
		m_EndDesH);

	Env_GetMiu(m_ProfilePlanArray[VerticalIndex], SlopePtIndex, MiuDesH);
	bool tempDR = CalSloptPtDesHRange(VerticalIndex, SlopePtIndex, minH, maxH);
	//ads_printf("里程%lf,下限%lf,上限%lf\n",NowMileage,minH,maxH);


	/************************************************************************/
	/* 依据坡度代数差确定当前变坡点的最小最大高程  */
	/************************************************************************/

	double miu = 0.0;//期望
	bool cheekKZD = true;//用于后面判断变坡点在高程必经点以前否
	bool cheek = true;//高程控制点在该坡段内时为false
	double cml = 0.0;
	double Gckzd = 0.0;
	double tempPD = 0.0;
	if (maxH < minH)
	{
		double templever = minH;
		minH = maxH;
		maxH = templever;
	}

	if (MiuDesH >= minH + PRESION && MiuDesH + PRESION <= maxH)
	{
		miu = MiuDesH;
		// 		double sigma = 1.0/3.0*min(miu - minH, maxH - miu);	//方差 

		do
		{
			double sigma = (maxH - minH) / 3.0;	//方差 

			if (fabs(minH - maxH) < 0.1)
				temp = (minH + maxH) / 2.0;
			else
				temp = NormalRandom(miu, sigma, minH, maxH);
		} while (!(temp >= minH && temp <= maxH));

	}
	else if (MiuDesH + PRESION < minH)
	{
		if (maxH - minH > 0.5)
			temp = minH + 0.2;
		else
			temp = minH;
	}
	else
	{
		if (maxH - minH > 0.5)
			temp = maxH - 0.2;
		else
			temp = maxH;
	}


	///////////////////////////////调整坡度，计算最终设计高程///////////////////////////////////
	double TempPd = (temp - Fronthigh) / (NowMileage - FrontMileage) * 100;
	double TempH = temp;
	if (cheek == false)
	{
		TempH = (NowMileage - FrontMileage)*(Gckzd - Fronthigh) / (cml - FrontMileage) + Fronthigh;
		m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].DesH = TempH;
	}
	else
	{

		m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].DesH = TempH;
	}

	return true;
}

void CProfileOpt::Env_GetMiu(ProfilePlan &TempPlan, int PDNum, double &Miu)
{
	double FrontMileage;//前一个变坡点的里程
	double Fronthigh;//前一个变坡点的高程
	double NowMileage;//当前变坡点的里程
	FrontMileage = TempPlan.SlopePtArray[PDNum - 1].Mileage;
	Fronthigh = TempPlan.SlopePtArray[PDNum - 1].DesH;
	NowMileage = TempPlan.SlopePtArray[PDNum].Mileage;
	GetDMX1(NowMileage, 20 + 10 * (m_TryNum % 20));
	double sum1 = 0.0;
	double sum2 = 0.0;
	for (int i = 0; i < m_CRS_PT.size(); i++)
	{
		sum1 = sum1 + (Fronthigh - m_CRS_PT[i].Earh) * (m_CRS_PT[i].Cml - FrontMileage);
		sum2 = sum2 + (m_CRS_PT[i].Cml - FrontMileage) * (m_CRS_PT[i].Cml - FrontMileage);
	}

	double K = 0.0;
	if (sum2 < 1.0)
		K = 0.0;
	else
		K = -sum1 / sum2;//斜率
	Miu = K * (TempPlan.SlopePtArray[PDNum].Mileage - FrontMileage) + Fronthigh;
}

//先用信封区域调整地面线，再提取有效地面线，不考虑平顺,根据地面线点数拟合坡度的期望值
void CProfileOpt::GetDMX1(double StratM, int Num)
{
	/////////////////////////////////////提取有效地面线/////////////////////////////
	vector<CRSPT> Temp_CRS_PT;//临时有效横断面数组
	Temp_CRS_PT.clear();
	//int EndIndex = m_CrossSection.road[0].NCRS - 1;
	int EndIndex = tempCRS.size() - 1;
	CRSPT crs_pt;
	for (int Index = 0; Index <= EndIndex; Index++)
	{
		if (tempCRS[Index].cml >= StratM)
		{
			crs_pt.Style = 0;
			for (int i = 0; i < m_section_Tun_Bri.size(); i++)
			{
				if (tempCRS[Index].cml > m_section_Tun_Bri[i].minS && tempCRS[Index].cml < m_section_Tun_Bri[i].maxS)
				{
					crs_pt.Style = 1;//在桥隧范围内
				}
			}
			crs_pt.Cml = tempCRS[Index].cml;
			crs_pt.Earh = tempCRS[Index].EarH;
			Temp_CRS_PT.push_back(crs_pt);
		}
	}

	m_CRS_PT.clear();


	int TempI = 0;

	for (int i = 0; i < Temp_CRS_PT.size(); i++)
	{
		//if (Temp_CRS_PT[i].Style==0)
		{
			crs_pt = Temp_CRS_PT[i];
			m_CRS_PT.push_back(crs_pt);
			TempI++;
			if (TempI > Num)
			{
				break;
			}
		}
	}

	if (m_CRS_PT.size() < 1)//直到终点也未搜索出一个有效点，将终点加入到数组里
	{
		crs_pt.Cml = m_Endml;
		crs_pt.Earh = m_EndDesH;
		m_CRS_PT.push_back(crs_pt);
	}

	Temp_CRS_PT.clear();
}

//计算索引为SlopePtIndex的変坡点设计高程范围
bool CProfileOpt::CalSloptPtDesHRange(int VerticalIndex, int SlopePtIndex, double& minH, double& maxH)
{
	//////////////////////////////////////////////////////////////////////////
	//在这里添加代码
	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;
	FrontMileage = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 1].Mileage;
	Fronthigh = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 1].DesH;
	NowMileage = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].Mileage;

	double temppd = 0.0;
	if (m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].Style == 0)
		temppd = m_StaMaxSlope;//站坪坡度值
	else
		temppd = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex].LimitGrade;

	if (SlopePtIndex == 1)
	{
		minH = Fronthigh - (NowMileage - FrontMileage)*temppd;//最小高程
		maxH = Fronthigh + (NowMileage - FrontMileage)*temppd;//最大高程
	}
	else
	{

		//if (Ngckzd>2)
		//{
		//	maxH = Fronthigh+(NowMileage-FrontMileage)*temppd;//最大高程
		//	minH = Fronthigh-(NowMileage-FrontMileage)*temppd;//最小高程
		//}
		//else
		{
			FrontfrontMileage = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 2].Mileage;
			Frontfronthigh = m_ProfilePlanArray[VerticalIndex].SlopePtArray[SlopePtIndex - 2].DesH;
			FrontPD = (Fronthigh - Frontfronthigh) / (FrontMileage - FrontfrontMileage);
			tempPD = min(temppd, (FrontPD + m_MaxSlopeDifference));//考虑了最大坡度代数差
			maxH = Fronthigh + (NowMileage - FrontMileage)*tempPD;//最大高程

			tempPD = max(-temppd, (FrontPD - m_MaxSlopeDifference));//考虑了最大坡度代数差
			minH = Fronthigh + (NowMileage - FrontMileage)*tempPD;//最小高程	
		}


	}

	////////////////////////////考虑信封区域//////////////////////////////////////////
	double TempMinH;//临时最小高程
	double TempMaxH;//临时最大高程
	GetEnvelopRange(Fronthigh, FrontMileage, NowMileage, TempMinH, TempMaxH);
	//ads_printf("里程%lf,信封下限%lf,信封上限%lf\n",NowMileage,TempMinH,TempMaxH);
	minH = max(minH, TempMinH);
	maxH = min(TempMaxH, maxH);
	if (maxH < minH)//无法同时考虑坡度代数差与信封区域，只考虑信封区域
	{
		maxH = TempMaxH;
		minH = TempMinH;
	}

	//if (minH>maxH+0.001)
	//		return false;
	return true;
}

//得到正态分布随机数
double CProfileOpt::NormalRandom(double miu, double sigma, double min, double max)
{
	double x;
	double dScope;
	double y;
	do
	{
		x = AverageRandom(min, max);
		y = Normal(x, miu, sigma);
		double temp = Normal(miu, miu, sigma);
		dScope = AverageRandom(0, Normal(miu, miu, sigma));
	} while (dScope > y);
	return x;
}
//得到均匀分布随机数
double CProfileOpt::AverageRandom(double min, double max)
{
	int minInteger = (int)(min * 10000);
	int maxInteger = (int)(max * 10000);

	int randInteger = rand()*rand();

	int diffInteger = maxInteger - minInteger;

	int resultInteger = randInteger % diffInteger + minInteger;

	return resultInteger / 10000.0;
}

//计算x的概率密度函数
double CProfileOpt::Normal(double x, double miu, double sigma)
{
	return 1.0 / sqrt(2 * PI*sigma)*exp(-1 * (x - miu)*(x - miu) / (2 * sigma*sigma));
}


//计算土石方工程量 (待定)
bool CProfileOpt::CalEarthWork(ProfilePlan& TempPlan, double& FillArea, double& CutArea)
{
	if (CM)
	{
		delete[]CM;
		CM = NULL;
	}
	NCM = 0;
	if (TempPlan.SlopePtArray.size() < 2)
	{
		FillArea = 1e99;
		CutArea = 1e99;
		return false;
	}
	m_SlopePtSum = TempPlan.SlopePtArray.size();
	double(*BPD_array)[4] = new double[m_SlopePtSum][4];
	for (int SlopePtIndex = 0; SlopePtIndex < m_SlopePtSum; SlopePtIndex++)
	{
		BPD_array[SlopePtIndex][0] = TempPlan.SlopePtArray[SlopePtIndex].Mileage;
		BPD_array[SlopePtIndex][1] = TempPlan.SlopePtArray[SlopePtIndex].DesH;
		BPD_array[SlopePtIndex][2] = TempPlan.SlopePtArray[SlopePtIndex].VerticalCurveRadius;
	}

	TrnBpdToCM(m_SlopePtSum, BPD_array);
	m_CrossSection.NCM = NCM;
	m_CrossSection.CM = CM;
	delete BPD_array;

	FillArea = CutArea = 0.0;
	//if (!m_CrossSection.CalBasCrossRHF()) //计算横断面路面点
	//{
	//	ads_alert(L"生成横断面路面出错!");
	//	return false;
	//}

	for (int i =0;i < m_CrossSection.road[0].NCRS; i++)
	{
		//计算路面点
		m_CrossSection.road[0].hdmdes[i].ZDesH = m_CrossSection.Cal_des_level(m_CrossSection.road[0].hdmdes[i].cml);//考虑竖曲线  
		m_CrossSection.road[0].hdmdes[i].RHFL[0] = 0.0; //分隔带边缘
		m_CrossSection.road[0].hdmdes[i].RHFL[1] = m_CrossSection.road[0].hdmdes[i].ZDesH;
		m_CrossSection.road[0].hdmdes[i].RHFL[2] = 10.0; //车行道边缘
		m_CrossSection.road[0].hdmdes[i].RHFL[3] = m_CrossSection.road[0].hdmdes[i].ZDesH;
		m_CrossSection.road[0].hdmdes[i].RHFL[4] = 10.0; //硬路肩边缘
		m_CrossSection.road[0].hdmdes[i].RHFL[5] = m_CrossSection.road[0].hdmdes[i].ZDesH;
		m_CrossSection.road[0].hdmdes[i].RHFL[6] = 10.0; //土路肩边缘
		m_CrossSection.road[0].hdmdes[i].RHFL[7] = m_CrossSection.road[0].hdmdes[i].ZDesH;

		m_CrossSection.road[0].hdmdes[i].RHFR[0] = 0.0; //分隔带边缘;
		m_CrossSection.road[0].hdmdes[i].RHFR[1] = m_CrossSection.road[0].hdmdes[i].ZDesH;
		m_CrossSection.road[0].hdmdes[i].RHFR[2] = 10.0; //车行道边缘
		m_CrossSection.road[0].hdmdes[i].RHFR[3] = m_CrossSection.road[0].hdmdes[i].ZDesH;
		m_CrossSection.road[0].hdmdes[i].RHFR[4] = 10.0; //硬路肩边缘
		m_CrossSection.road[0].hdmdes[i].RHFR[5] = m_CrossSection.road[0].hdmdes[i].ZDesH;
		m_CrossSection.road[0].hdmdes[i].RHFR[6] = 10.0; //土路肩边缘
		m_CrossSection.road[0].hdmdes[i].RHFR[7] = m_CrossSection.road[0].hdmdes[i].ZDesH;
	}
	

	AutoSetBRI(m_pPlain, m_MinBriFill, m_MinBriLeth, m_BriFill, TempPlan.BriSum, TempPlan); //自动设桥
	AutoSetTUN(m_pPlain, m_MinTunFill, m_MinTunLeth, m_TunFill, TempPlan.TunSum, TempPlan); //自动设隧


	if (m_CrossSection.SimpDesCrossForOpt() != RTNORM) //简单设计横断面
	{
		ads_alert(L"横断面设计出错!");
		return false;
	}


	double CrsCml, Sml, Eml;
	int StartIndex, EndIndex;
	int SearchIndex = 0;
	double BriSml, BriEml;
	double TunSml, TunEml;
	bool IsFindStart, IsFindEnd;
	CString Tempskml, Tempekml;
	for (int SlopePtIndex = 1; SlopePtIndex < m_SlopePtSum - 1; SlopePtIndex++)
	{
		Sml = TempPlan.SlopePtArray[SlopePtIndex - 1].Mileage;
		Eml = TempPlan.SlopePtArray[SlopePtIndex + 1].Mileage;

		IsFindStart = IsFindEnd = false;

		for (int CrsIndex = SearchIndex; CrsIndex < m_CrossSection.road[0].NCRS - 1; CrsIndex++)
		{
			CrsCml = m_CrossSection.road[0].hdmdes[CrsIndex].cml;
			if (!IsFindStart && CrsCml > Sml - 0.001)
			{
				IsFindStart = true;
				StartIndex = CrsIndex;
			}
			if (IsFindStart && !IsFindEnd && CrsCml > Eml - 0.001)
			{
				IsFindEnd = true;
				EndIndex = CrsIndex;
			}
			if (IsFindStart && IsFindEnd)
			{
				m_CrossSection.Cal_TSFJSBForOpt(StartIndex, EndIndex, FillArea, CutArea);
				TempPlan.SlopePtArray[SlopePtIndex].Cost = FillArea * m_FillCost + CutArea * m_CutCost;
				SearchIndex = StartIndex;
				break;
			}
		}

		double TempSml, TempEml;

		for (int iBri = 0; iBri < TempPlan.BriSum; iBri++)
		{
			Tempskml = TempPlan.BriArray[iBri].SKml;
			Tempekml = TempPlan.BriArray[iBri].EKml;
			BriSml = m_pPlain->TYLC(Tempskml);
			BriEml = m_pPlain->TYLC(Tempekml);
			TempSml = max(BriSml, Sml);
			TempEml = min(BriEml, Eml);
			if (TempEml - TempSml > 0.0)
			{
				TempPlan.SlopePtArray[SlopePtIndex].Cost += (TempEml - TempSml) * TempPlan.BriArray[iBri].BriCost;
			}
		}

		for (int iTun = 0; iTun < TempPlan.TunSum; iTun++)
		{
			Tempskml = TempPlan.TunArray[iTun].SKml;
			Tempekml = TempPlan.TunArray[iTun].EKml;

			TunSml = m_pPlain->TYLC(Tempskml);
			TunEml = m_pPlain->TYLC(Tempekml);
			TempSml = max(TunSml, Sml);
			TempEml = min(TunEml, Eml);
			if (TempEml - TempSml > 0.0)
			{
				TempPlan.SlopePtArray[SlopePtIndex].Cost += (TempEml - TempSml) * TempPlan.TunArray[iTun].TunCost;
			}
		}
	}

	m_CrossSection.Cal_TSFJSBForOpt(0, m_CrossSection.road[0].NCRS - 1, FillArea, CutArea);

	double tempCost;
	tempCost = fabs(FillArea) * m_FillCost + fabs(CutArea) * m_CutCost;

	for (int iBri = 0; iBri < TempPlan.BriSum; iBri++)
	{
		tempCost += TempPlan.BriArray[iBri].BriLen * TempPlan.BriArray[iBri].BriCost;
	}

	for (int iTun = 0; iTun < TempPlan.TunSum; iTun++)
	{
		Tempskml = TempPlan.TunArray[iTun].SKml;
		Tempekml = TempPlan.TunArray[iTun].EKml;
		TunSml = m_pPlain->TYLC(Tempskml);
		TunEml = m_pPlain->TYLC(Tempekml);
		tempCost += (TunEml - TunSml) * TempPlan.TunArray[iTun].TunCost;
	}
	TempPlan.Cost = fabs(tempCost);

	return true;
}

/**************************************************************************
将变坡点数组BPD_array[i][3]转换为CM[i*5]数组
输入参数：
BPD_array[i][0]		里程
BPD_array[i][1]		高程
BPD_array[i][2]		竖曲线半径
输出参数：
CM[i*5]				里程
CM[i*5+1]			高程
CM[i*5+2]			坡度
CM[i*5+3]			坡长(该变坡点与前一变坡点的里程差)
CM[i*5+4]			竖曲线半径
备注：i=0的点只有里程和高程；坡度，坡长，竖曲线半径均为0
**************************************************************************/
void CProfileOpt::TrnBpdToCM(int NBPD, double BPD_array[][4])
{
	double LC1, H1, LC2, H2, R;
	CM = new double[(NBPD + 1) * 5];
	LC1 = BPD_array[0][0];
	H1 = BPD_array[0][1];
	CM[0] = LC1, CM[1] = H1, CM[2] = 0.0, CM[3] = 0.0, CM[4] = 0.0;

	for (int i = 1; i < NBPD; i++)
	{
		LC2 = BPD_array[i][0];
		H2 = BPD_array[i][1];
		R = BPD_array[i][2];
		CM[i * 5] = LC2, CM[i * 5 + 1] = H2;
		CM[i * 5 + 2] = (H2 - H1) / (LC2 - LC1);
		CM[i * 5 + 3] = LC2 - LC1;
		CM[i * 5 + 4] = R;
		H1 = H2, LC1 = LC2;
	}
	CM[NBPD * 5] = CM[NBPD * 5 + 1] = CM[NBPD * 5 + 2] = CM[NBPD * 5 + 3] = CM[NBPD * 5 + 4] = 0.0;
	NCM = NBPD + 1;
}

//////////////////////////////////////////////////////////////////////////
//自动设置桥梁
//自动设置隧道
//STH 线桥分界挖深
//Th  桥梁最小高度
//MinLen   桥梁的最小长度
void CProfileOpt::AutoSetBRI(JD_CENTER* pz, double Th, double MinLen, double STH, int& NBri, ProfilePlan &TempPlan)//自动设计桥梁
{
	// TODO: Implement the command
	//计算填挖高
	vector<BRITAB> Bri_Array;
	TempPlan.BriArray.clear();
	double BriMl[500][2];
	double sml, eml, len, Btwh;
	ACHAR GH[8];
	int i;
	double cml = 0.0;
	double earH = 0.0;
	double desh = 0.0;
	double twh = 0.0;

	double MinH, MinHCml;//最小地面线标高，最小地面线标高处的里程
	MinH = 1e99;
	double AveH = 0.0;
	int PtNum = 0;
	double MaxTWh = 0.0;

	BRITAB tempBri;

	NBri = 0;
	for (i = 0; i + 1 < m_CrossSection.road[0].NCRS ; i++)
	{
		cml = m_CrossSection.road[0].hdmdes[i].cml;
		earH = m_CrossSection.road[0].hdmdes[i].newLYd;
		desh = m_CrossSection.road[0].hdmdes[i].ZDesH;
		twh = desh - earH;
		if (twh >= Th)//可能设桥
		{
			double Temptwh = twh;
			//找起点
			if (i < 1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh = twh;
				while (n > 0 && Stwh > STH)//找起点
				{
					Btwh = Stwh;
					n--;
					cml = m_CrossSection.road[0].hdmdes[n].cml;
					earH = m_CrossSection.road[0].hdmdes[n].newLYd;
					desh = m_CrossSection.road[0].hdmdes[i].ZDesH;
					Stwh = desh - earH;
					Temptwh = max(Temptwh, Stwh);
				}
				if (Stwh < STH)
					sml = cml + (m_CrossSection.road[0].hdmdes[n + 1].cml - cml)*(STH - Stwh) / (Btwh - Stwh);
				else
					sml = cml;
				sml = fabs(sml - cml) < fabs(m_CrossSection.road[0].hdmdes[n + 1].cml - sml) ? cml : m_CrossSection.road[0].hdmdes[n + 1].cml;
			}

			MinHCml = sml;

			while (i<m_CrossSection.road[0].NCRS - 1 && twh>STH)//找终点
			{
				Btwh = twh;
				i++;
				cml = m_CrossSection.road[0].hdmdes[i].cml;
				earH = m_CrossSection.road[0].hdmdes[i].newLYd;
				desh = m_CrossSection.road[0].hdmdes[i].ZDesH;
				twh = desh - earH;
				Temptwh = max(Temptwh, twh);

				if (earH < MinH)
				{
					MinH = earH;
					MinHCml = cml;
				}
				if (twh > MaxTWh)
				{
					MaxTWh = twh;
				}

				AveH += earH;
				PtNum++;
			}
			if (twh < STH)
				eml = cml - (cml - m_CrossSection.road[0].hdmdes[i - 1].cml)*(STH - twh) / (Btwh - twh);
			else
				eml = m_CrossSection.road[0].hdmdes[i].cml;

			eml = fabs(eml - cml) < fabs(m_CrossSection.road[0].hdmdes[i - 1].cml - eml) ? cml : m_CrossSection.road[0].hdmdes[i - 1].cml;


			len = eml - sml;
			if (len > MinLen && len < fabs(m_Endml - m_Startml))//桥长至少10m 且小于道路长
			{
				//tempBri.StDml =_wtof(m_pPlain->XLC(sml));
				//tempBri.BGH = L"K";
				//_tcscpy(tempBri.SKml, LCchr(GH, tempBri.StDml, 3, 1));
				//tempBri.EdDml = _wtof(m_pPlain->XLC(eml));
				//tempBri.EGH = GH;
				//_tcscpy(tempBri.EKml, LCchr(GH, tempBri.EdDml, 3, 1));
				//tempBri.CenDml = _wtof(m_pPlain->XLC(0.5*(sml + eml)));
				//tempBri.CGH=GH;
				//_tcscpy(tempBri.CKml, LCchr(GH, tempBri.CenDml, 3, 1));
				//tempBri.BriLen = eml - sml;
				//tempBri.BriName.Format(L"桥梁%d", NBri + 1);
				_tcscpy(GH, L"K");
				tempBri.StDml = sml;
				tempBri.BGH = L"K";
				_tcscpy(tempBri.SKml, LCchr(GH, tempBri.StDml, 3, 1));
				tempBri.EdDml =eml;
				tempBri.EGH = L"K";
				_tcscpy(tempBri.EKml, LCchr(GH, tempBri.EdDml, 3, 1));
				tempBri.CenDml =0.5*(sml + eml);
				tempBri.CGH = L"K";
				_tcscpy(tempBri.CKml, LCchr(GH, tempBri.CenDml, 3, 1));
				tempBri.BriLen = eml - sml;
				tempBri.BriName.Format(L"桥梁%d", NBri + 1);

				tempBri.BriCost = m_BridgeCost;
				if (m_IsSetBaseBriHigh && Temptwh > m_BaseBriHigh)//按桥长折算桥梁的费用
				{
					tempBri.BriCost = m_BridgeCost * Temptwh / m_BaseBriHigh;

				}

				BriMl[NBri][0] = sml, BriMl[NBri][1] = eml;
				NBri++;
				Bri_Array.push_back(tempBri);

				if (m_TryNum == 0 && len > m_MinBiLen && MaxTWh > 100.0)
				{
					bool IsSetKZDInBri = false;
					for (int iKZD = 0; iKZD < NGCKZD; iKZD++)//检测用户是否已经在桥梁范围内设置控制点，如已设置，不自动设置控制点
					{
						if (GCKZDTAB[iKZD].Cml > sml && GCKZDTAB[iKZD].Cml < eml)
						{
							IsSetKZDInBri = true;
							break;
						}
					}
					//////////////////////////////////////////////////////////////////////////
					if (Bri_Array.size() >0)
					{
						//自动增加一个上限控制标高
						GCKZPt BriGCKZD;
						BriGCKZD.Cml = 0.5 * (eml + sml);
						AveH = AveH / PtNum;
						BriGCKZD.DesHSX = MinH + m_MaxBriTwg;
						BriGCKZD.DesHXX = 0.0;
						BriGCKZD.Style = 1;
						m_BriGCKZDArray.push_back(BriGCKZD);
					}
				}


				MinH = 1e99;
				PtNum = 0;
				AveH = 0.0;
				MaxTWh = 0.0;
			}

		}
		Btwh = twh;
	}

	if (Bri_Array.size() > 0)
	{
		TempPlan.BriArray.resize(Bri_Array.size());
		for (i = 0; i < Bri_Array.size(); i++)
		{
			TempPlan.BriArray[i] = Bri_Array[i];
		}
	}

	Bri_Array.clear();

	//if (m_CrossSection.road[0].AllBri)
	//{
	//	delete[]m_CrossSection.road[0].AllBri;
	//	m_CrossSection.road[0].AllBri = NULL;
	//	m_CrossSection.road[0].NAllBri = 0;
	//}

	if (NBri > 0)
	{
		m_CrossSection.road[0].NAllBri = NBri;
		m_CrossSection.road[0].AllBri = new BRITAB[NBri];
		for (int iBri = 0; iBri < NBri; iBri++)
		{
			m_CrossSection.road[0].AllBri[iBri] = TempPlan.BriArray[iBri];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//自动设置隧道
//STH 线隧分界挖深
//Th  隧道最小挖深
//MinLen   隧的最小长度
void CProfileOpt::AutoSetTUN(JD_CENTER *pz, double Th, double MinLen, double STH, int& NTun, ProfilePlan &TempPlan)//自动设置隧道
{
	vector<TUNTAB> Tun_Array;//隧道数组
	Tun_Array.clear();
	TUNTAB TempTun;
	NTun = 0;
	Th = -1.0*Th;
	STH = -1.0*STH;
	//计算填挖高
	double TunMl[500][2];
	double sml, eml, len, Btwh;
	ACHAR GH[8];
	int i;
	for (i = 0; i < m_CrossSection.road[0].NCRS - 1; i++)
	{
		double cml = m_CrossSection.road[0].hdmdes[i].cml;
		double earH = m_CrossSection.road[0].hdmdes[i].newLYd;
		double desh = m_CrossSection.road[0].hdmdes[i].ZDesH;
		double twh = desh - earH;
		if (twh <= Th)//需要设置隧道
		{
			//找起点
			if (i < 1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh = twh;
				while (n > 0 && Stwh < STH)//找终点
				{
					Btwh = Stwh;
					n--;
					cml = m_CrossSection.road[0].hdmdes[n].cml;
					earH = m_CrossSection.road[0].hdmdes[n].newLYd;
					desh = m_CrossSection.road[0].hdmdes[n].ZDesH;
					Stwh = desh - earH;
				}
				if (Stwh > STH)
					sml = cml + (m_CrossSection.road[0].hdmdes[n + 1].cml - cml)*(STH - Stwh) / (Btwh - Stwh);
				else
					sml = cml;
				sml = fabs(sml - cml) < fabs(m_CrossSection.road[0].hdmdes[n + 1].cml - sml) ? cml : m_CrossSection.road[0].hdmdes[n + 1].cml;

			}

			while (i < m_CrossSection.road[0].NCRS - 1 && twh <= STH)//找终点
			{
				Btwh = twh;
				i++;
				cml = m_CrossSection.road[0].hdmdes[i].cml;
				earH = m_CrossSection.road[0].hdmdes[i].newLYd;
				desh = m_CrossSection.road[0].hdmdes[i].ZDesH;
				twh = desh - earH;
			}
			if (twh > Th && i < m_CrossSection.road[0].NCRS - 1)
				eml = m_CrossSection.road[0].hdmdes[i - 1].cml + (cml - m_CrossSection.road[0].hdmdes[i - 1].cml)*(STH - Btwh) / (twh - Btwh);
			else
				eml = m_CrossSection.road[0].hdmdes[i].cml;
			eml = fabs(eml - cml) < fabs(m_CrossSection.road[0].hdmdes[i - 1].cml - eml) ? cml : m_CrossSection.road[0].hdmdes[i - 1].cml;

			len = eml - sml;
			if (len > MinLen && len < fabs(m_Endml - m_Startml) )//隧道长至少20m 需小于道路长
			{
				//TempTun.StDml = _wtof(m_pPlain->XLC(sml));
				//TempTun.BGH = GH;
				//_tcscpy(TempTun.SKml, LCchr(GH, TempTun.StDml, 3, 1));
				//TempTun.EdDml = _wtof(m_pPlain->XLC(eml));
				//TempTun.EGH = GH;
				//_tcscpy(TempTun.EKml, LCchr(GH, TempTun.EdDml, 3, 1));
				//TempTun.TunName.Format(L"隧道%d", NTun + 1);
				_tcscpy(GH,L"K");
				TempTun.StDml =sml;
				TempTun.BGH = L"K";
				_tcscpy(TempTun.SKml, LCchr(GH, TempTun.StDml, 3, 1));
				TempTun.EdDml =eml;
				TempTun.EGH = L"K";
				_tcscpy(TempTun.EKml, LCchr(GH, TempTun.EdDml, 3, 1));
				TempTun.TunName.Format(L"隧道%d", NTun + 1);

				TempTun.TunCost = m_TunnelCost;
				if (m_IsSetBaseTunLen && len > m_BaseTunLen)
				{
					TempTun.TunCost = len / m_BaseTunLen * m_TunnelCost;
				}

				TunMl[NTun][0] = sml, TunMl[NTun][1] = eml;
				NTun++;
				Tun_Array.push_back(TempTun);
			}
		}
		Btwh = twh;

	}

	TempPlan.TunArray.clear();
	if (Tun_Array.size() > 0)
	{
		TempPlan.TunArray.resize(Tun_Array.size());
		for (i = 0; i < Tun_Array.size(); i++)
		{
			TempPlan.TunArray[i] = Tun_Array[i];
		}
	}

	Tun_Array.clear();

	//if (m_CrossSection.road[0].AllTun)
	//{
	//	delete[]m_CrossSection.road[0].AllTun;
	//	m_CrossSection.road[0].AllTun = NULL;
	//	m_CrossSection.road[0].NAllTun = 0;
	//}

	if (NTun > 0)
	{
		m_CrossSection.road[0].NAllTun = NTun;
		m_CrossSection.road[0].AllTun = new TUNTAB[NTun];
		for (int iTun = 0; iTun < NTun; iTun++)
		{
			m_CrossSection.road[0].AllTun[iTun] = TempPlan.TunArray[iTun];
		}
	}

}

//释放该类的动态数组空间
//Stage=0	初始方案生成后释放
//Stage=1	遗传结束后释放
//Stage=2	调整完方案计算完工程造价后释放
void CProfileOpt::ReleaseRoom(int Stage)
{
	if (Stage == 0)//生成初始方案后释放
	{
		ZMdmxPt.clear();
		PSdmxPt.clear();
		ThreeDiffQuotientPt.clear();
		OneDiffQuotientPt.clear();
		ChangePt.clear();
		JDdwxPt.clear();
		FwdPt.clear();
	}


	//遗传结束后释放
	else if (Stage == 1)
	{
		if (m_ProfilePlanArray.size() > 0)
		{
			for (int i = 0; i < m_ProfilePlanArray.size(); i++)
			{
				m_ProfilePlanArray[i].SlopePtArray.clear();
				m_ProfilePlanArray[i].BriArray.clear();
				m_ProfilePlanArray[i].TunArray.clear();
			}
			m_ProfilePlanArray.clear();
		}

		if (CrossPlanArray.size() > 0)
		{
			for (int i = 0; i < CrossPlanArray.size(); i++)
			{
				CrossPlanArray[i].SlopePtArray.clear();
				CrossPlanArray[i].BriArray.clear();
				CrossPlanArray[i].TunArray.clear();
			}
			CrossPlanArray.clear();
		}

		if (ModifyPlanArray.size() > 0)
		{
			for (int i = 0; i < ModifyPlanArray.size(); i++)
			{
				ModifyPlanArray[i].SlopePtArray.clear();
				ModifyPlanArray[i].BriArray.clear();
				ModifyPlanArray[i].TunArray.clear();
			}
			ModifyPlanArray.clear();
		}

		if (SelPlanArray.size() > 0)
		{
			for (int i = 0; i < SelPlanArray.size(); i++)
			{
				SelPlanArray[i].SlopePtArray.clear();
				SelPlanArray[i].BriArray.clear();
				SelPlanArray[i].TunArray.clear();
			}
			SelPlanArray.clear();
		}

		if (SelPlanArray1.size() > 0)
		{
			for (int i = 0; i < SelPlanArray1.size(); i++)
			{
				SelPlanArray1[i].SlopePtArray.clear();
				SelPlanArray1[i].BriArray.clear();
				SelPlanArray1[i].TunArray.clear();
			}
			SelPlanArray1.clear();
		}

		if (envelope.size() > 0)
			envelope.clear();

		if (FwdPt.size() > 0)
			FwdPt.clear();

		if (JDdwxPt.size() > 0)
			JDdwxPt.clear();

		Ngckzd = 0;
		if (GCKZD)
		{
			delete[]GCKZD;// 这边崩了
			GCKZD = NULL;
		}

		m_AutoSetBriGCKZDArray.clear();
		m_BriGCKZDArray.clear();
		MaxEnvPtArray.clear();
		MinEnvPtArray.clear();
		m_dmx_SubSection.clear();
		m_CRS_PT.clear();
		tempCRS.clear();
	}


	else if (Stage == 2)
	{
		//计算完工程造价释放
		NCM = 0;
		if (CM)
		{
			delete CM;
			CM = NULL;
		}

		LastBestPlan.BriArray.clear();
		LastBestPlan.TunArray.clear();
		LastBestPlan.SlopePtArray.clear();
		m_section_Tun_Bri.clear();
	}

}


//排序
void CProfileOpt::Taxis(vector<ProfilePlan>&Array)
{
	int temp1;
	temp1 = Array.size();
	ProfilePlan temp2;

	for (int i = 0; i < temp1 - 1; i++)
	{
		for (int j = i + 1; j < temp1; j++)
		{
			if (Array[i].Cost > Array[j].Cost)
			{
				temp2 = Array[j];
				Array[j] = Array[i];
				Array[i] = temp2;
			}
		}
	}
}

void CProfileOpt::Cal_Satisfaction(vector<ProfilePlan>&Array)//计算Sw，Sw是考虑运营和工程结合后的满意度（运营计算）
{
	Get_Operate_H(Array);
	double MinCost = Array[0].Cost;
	for (int IPlan = 0; IPlan < Array.size(); IPlan++)
	{
		double s1 = 1 - fabs(Array[IPlan].Cost - MinCost) / MinCost;
		double s2 = 1 - fabs(Array[IPlan].S_high - Min_Operate_H) / Min_Operate_H;

		Array[IPlan].Satisfaction = Cost_xs * s1 + Oprate_xs * s2;
		if (Array[IPlan].Satisfaction < 0)
		{
			Array[IPlan].Satisfaction = 0.0;
		}
	}
}

void CProfileOpt::Get_Operate_H(vector<ProfilePlan>&Array)//获得每个方案的克服高度（运营计算）
{
	for (int IPlan = 0; IPlan < Array.size(); IPlan++)
	{
		double sum_S = 0.0;
		double sum_X = 0.0;
		//计算上、下行方向的克服高度
		for (int ISlop = 0; ISlop < Array[IPlan].SlopePtArray.size() - 1; ISlop++)
		{
			double high = Array[IPlan].SlopePtArray[ISlop + 1].DesH - Array[IPlan].SlopePtArray[ISlop].DesH;

			if (high > 0)
			{
				sum_S = sum_S + high;//计算上行克服高度
			}
			else
			{
				sum_X = sum_X + high;//计算下行克服高度	
			}
		}
		Array[IPlan].S_high = sum_S;
		Array[IPlan].X_high = fabs(sum_X);

		//得到所有方案中最小的克服高度值
		if (IPlan == 0)
			Min_Operate_H = Array[IPlan].S_high;
		else
		{
			if (Min_Operate_H - Array[IPlan].S_high > 0.00001)
			{
				Min_Operate_H = Array[IPlan].S_high;
			}
		}

	}
}

//基于满意度的排序
void CProfileOpt::Taxis_Satisfaction(vector<ProfilePlan>&Array)
{
	int temp1;
	temp1 = Array.size();
	ProfilePlan temp2;

	for (int i = 0; i < temp1 - 1; i++)
	{
		for (int j = i + 1; j < temp1; j++)
		{
			if (Array[i].Satisfaction < Array[j].Satisfaction)//满意度越大，越靠前,方案越优
			{
				temp2 = Array[j];
				Array[j] = Array[i];
				Array[i] = temp2;
			}
			else if (Array[i].Satisfaction == Array[j].Satisfaction)
			{
				if (Array[i].Cost > Array[j].Cost)//若满意度一样，则比较方案的工程费用值，工程费用小的方案优
				{
					temp2 = Array[j];
					Array[j] = Array[i];
					Array[i] = temp2;
				}
			}
		}
	}
}

//计算坡度
void CProfileOpt::CalAllPD(ProfilePlan &tempPlan)
{
	int TempSlopNum = tempPlan.SlopePtArray.size();//变坡点数			
	for (int i = 0; i < TempSlopNum - 1; i++)
	{
		double templengh = tempPlan.SlopePtArray[i + 1].Mileage - tempPlan.SlopePtArray[i].Mileage;
		double temphigh = tempPlan.SlopePtArray[i + 1].DesH - tempPlan.SlopePtArray[i].DesH;
		tempPlan.SlopePtArray[i].Gradient = temphigh / templengh;
	}
}


//得到适应度值
void CProfileOpt::CalFitness()//方案越优，适应度值越大
{
	for (int ProfileIndex = 0; ProfileIndex < m_ProfilePlanArray.size(); ProfileIndex++)
	{
		m_ProfilePlanArray[ProfileIndex].Fitness = ProfileIndex + 1;
	}
}

//计算基于满意度的多样性
void CProfileOpt::CalDiff_Satisfaction(double& Diff)
{
	double sum, average;
	sum = 0.0;
	average = 0.0;
	int temp1 = m_ProfilePlanArray.size();
	int ProfileIndex;
	for (ProfileIndex = 0; ProfileIndex < temp1; ProfileIndex++)
		sum = sum + m_ProfilePlanArray[ProfileIndex].Satisfaction;
	average = sum / temp1;

	int n = 0;
	sum = 0.0;
	for (ProfileIndex = 0; ProfileIndex < temp1; ProfileIndex++)
	{
		if (m_ProfilePlanArray[ProfileIndex].Satisfaction < average)
		{
			sum = sum + m_ProfilePlanArray[ProfileIndex].Satisfaction;
			n = n + 1;
		}
	}
	average = sum / n;

	double MinSatisfaction = m_ProfilePlanArray[0].Satisfaction;
	if (average == 0.0)
		Diff = 0.0;
	else
		Diff = MinSatisfaction / average - 1;
}

//计算选择概率
void CProfileOpt::CalChooseProbability(double diff)
{
	double temp1, temp2;
	temp1 = 1 + pow(10, -6 * (diff + 0.12));
	temp2 = 2 - temp1;
	for (int ProfileIndex = 0; ProfileIndex < m_PopulationSize; ProfileIndex++)
	{
		int n = m_ProfilePlanArray[ProfileIndex].Fitness;
		m_ProfilePlanArray[ProfileIndex].SelProbability = (temp2 + (temp1 - temp2)*(m_PopulationSize - n) / (m_PopulationSize - 1)) / m_PopulationSize;
	}
}

bool CProfileOpt::CreateNextPopulation_Satisfaction(double diff)//生成下一代
{
	//1.选择算子
	SelOperator();
	//AfxMessageBox("选择");

	int tempnum = SelPlanArray.size();
	SelPlanArray1.resize(tempnum);
	for (int i = 0; i < tempnum; i++)
	{
		SelPlanArray1[i] = SelPlanArray[i];
	}

	if (SelPlanArray.size() < 2)
	{
		Tempbreak = TRUE;
		ads_printf(L"选择出的子代少于2个,优化提前退出，请增加种群规模，重新进行优化!");
		return false;
	}

	//根据多样性确定交叉、变异的概率
	if (diff >= 0 && diff <= 0.02)
	{
		CrossProbablity = 0.5;
		MutationPrabality = 0.5;
	}
	else if (diff > 0.02 && diff <= 0.1)
	{
		CrossProbablity = 0.6;
		MutationPrabality = 0.4;
	}
	else if (diff > 0.1 && diff <= 0.2)
	{
		CrossProbablity = 0.75;
		MutationPrabality = 0.25;
	}
	else if (diff > 0.2 && diff <= 0.5)
	{
		CrossProbablity = 0.85;
		MutationPrabality = 0.15;
	}
	else
	{
		CrossProbablity = 0.9;
		MutationPrabality = 0.1;
	}

	//新开线程进行变异运算
	// CWinThread *pThread1=AfxBeginThread(RUNTIME_CLASS(CModifyThread));
	ModifyOperator();

	//2.交叉算子
	CrossOperator_Satisfaction();
	//计算交叉得到的方案工程费用
	for (int ProfileIndex = 0; ProfileIndex < CrossPlanArray.size(); ProfileIndex++)
	{
		double FillArea, CutArea;
		CalEarthWork(CrossPlanArray[ProfileIndex], FillArea, CutArea);
		if (TempStopOpt)//提前退出
			return false;
	}


	//计算变异得到的方案工程费用
	for (int ProfileIndex = 0; ProfileIndex < ModifyPlanArray.size(); ProfileIndex++)
	{
		double FillArea, CutArea;
		CalEarthWork(ModifyPlanArray[ProfileIndex], FillArea, CutArea);

		if (TempStopOpt)//提前退出
			return false;
	}

	//3.变异算子
	// 	ModifyOperator();
	if (TempStopOpt)//提前退出
	{
		return false;
	}

	vector<ProfilePlan>TempPlanArray;//临时种群数组

	if (TempPlanArray.size() > 0)
	{
		for (int i = 0; i < TempPlanArray.size(); i++)
		{
			TempPlanArray[i].SlopePtArray.clear();
			TempPlanArray[i].BriArray.clear();
			TempPlanArray[i].TunArray.clear();
		}
		TempPlanArray.clear();
	}

	//TempPlanArray.Append(SelPlanArray);
	//TempPlanArray.Append(CrossPlanArray);
	//TempPlanArray.Append(ModifyPlanArray);

	TempPlanArray.insert(TempPlanArray.end(), SelPlanArray.begin(), SelPlanArray.end());
	TempPlanArray.insert(TempPlanArray.end(), CrossPlanArray.begin(), CrossPlanArray.end());
	TempPlanArray.insert(TempPlanArray.end(), ModifyPlanArray.begin(), ModifyPlanArray.end());



	//排序
	Taxis(TempPlanArray);

	Cal_Satisfaction(TempPlanArray);//计算方案满意度

	Taxis_Satisfaction(TempPlanArray);//基于满意度的排序

	for (int ProfileIndex = 0; ProfileIndex < TempPlanArray.size(); ProfileIndex++)
	{
		// 		  m_SlopePtSum = TempPlanArray[ProfileIndex].SlopePtArray.size();
		for (int SlopePtIndex = 0; SlopePtIndex < TempPlanArray[ProfileIndex].SlopePtArray.size(); SlopePtIndex++)
		{
			TempPlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].VerticalCurveRadius = m_VerticalCurveRadius;
		}
		if (TempPlanArray[ProfileIndex].SlopePtArray.size() > 1)
		{
			TempPlanArray[ProfileIndex].SlopePtArray[0].VerticalCurveRadius = 0.0;
			TempPlanArray[ProfileIndex].SlopePtArray[m_SlopePtSum - 1].VerticalCurveRadius = 0.0;
		}
	}

	if (m_ProfilePlanArray.size() > 0)
	{
		for (int i = 0; i < m_ProfilePlanArray.size(); i++)
		{
			m_ProfilePlanArray[i].SlopePtArray.clear();
			m_ProfilePlanArray[i].BriArray.clear();
			m_ProfilePlanArray[i].TunArray.clear();
		}
		m_ProfilePlanArray.clear();
	}

	m_ProfilePlanArray.resize(m_PopulationSize);
	for (int i = 0; i < m_PopulationSize; i++)
	{
		m_ProfilePlanArray[i] = TempPlanArray[i];
	}
	//DrawProfile(m_ProfilePlanArray[0]);
	return true;
}

//选择算子
void CProfileOpt::SelOperator()
{
	vector<int>TempXB;//临时的下标数组

	if (TempXB.size() > 0)
	{
		TempXB.clear();
	}
	TempXB.resize(m_PopulationSize + 1);

	//按种群大小进行轮盘赌操作，得到被选中方案的下标
	int ProfileIndex;
	for (ProfileIndex = 0; ProfileIndex < m_PopulationSize; ProfileIndex++)
	{
		int i = 0;
		i = Roulette_Wheel_Selection();
		TempXB[ProfileIndex] = i;
	}

	//保留前一代的最佳方案
	TempXB[m_PopulationSize] = 0;

	//对下标进行修正，删除相同的下标
	for (ProfileIndex = 0; ProfileIndex < TempXB.size() - 1; ProfileIndex++)
	{
		for (int j = ProfileIndex + 1; j < TempXB.size(); j++)
		{
			if (TempXB[ProfileIndex] == TempXB[j])
			{
				TempXB.erase(TempXB.begin() + j);
				j--;
			}
		}
	}

	//得到进行了选择算子后的纵断面方案数组
	if (SelPlanArray.size() > 0)
	{
		for (int i = 0; i < SelPlanArray.size(); i++)
		{
			SelPlanArray[i].SlopePtArray.clear();
			SelPlanArray[i].BriArray.clear();
			SelPlanArray[i].TunArray.clear();

		}
		SelPlanArray.clear();
	}
	int n = TempXB.size();
	if (TempXB.size() > 0)
	{
		SelPlanArray.resize(n);
	}

	for (ProfileIndex = 0; ProfileIndex < TempXB.size(); ProfileIndex++)
	{
		ProfilePlan tempPlan;
		int tempxb;
		tempxb = TempXB[ProfileIndex];
		SelPlanArray[ProfileIndex] = m_ProfilePlanArray[tempxb];
	}

	//对数组进行排序
	Taxis(SelPlanArray);
	Taxis_Satisfaction(SelPlanArray);

}

//轮盘赌操作
int CProfileOpt::Roulette_Wheel_Selection()
{
	double sum = 0.0;
	double tempSuiJS = 0.0;
	int i = 0;
	tempSuiJS = AverageRandom(0, 1);
	for (int ProfileIndex = 0; ProfileIndex < m_PopulationSize; ProfileIndex++)
	{
		sum = sum + m_ProfilePlanArray[ProfileIndex].SelProbability;
		if (tempSuiJS < sum)
		{
			i = ProfileIndex;
			break;
		}
	}
	return i;
}

//变异算子
void CProfileOpt::ModifyOperator()
{
	int ModifyNum = 0;
	ModifyNum = int(m_PopulationSize * MutationPrabality);
	int CrossNum = 0;
	CrossNum = int(m_PopulationSize * CrossProbablity);

	int TempModifyNum = m_PopulationSize - SelPlanArray1.size() - CrossNum;
	ModifyNum = max(ModifyNum, TempModifyNum);

	if (ModifyPlanArray.size() > 0)
	{
		for (int i = 0; i < ModifyPlanArray.size(); i++)
		{
			ModifyPlanArray[i].SlopePtArray.clear();
			ModifyPlanArray[i].BriArray.clear();
			ModifyPlanArray[i].TunArray.clear();

		}
		ModifyPlanArray.clear();
	}

	if (ModifyNum > 0)
		ModifyPlanArray.resize(ModifyNum);

	ProfilePlan TempPlan;
	for (int ProfileIndex = 0; ProfileIndex < ModifyPlanArray.size(); ProfileIndex++)
	{
		OneModifyH(TempPlan);
		ModifyPlanArray[ProfileIndex] = TempPlan;
		double FillArea, CutArea;
		CalEarthWork(ModifyPlanArray[ProfileIndex], FillArea, CutArea);

		if (TempStopOpt)//提前退出
			return;
	}


	return;
}

bool CProfileOpt::OneModifyH(ProfilePlan& TempPlan)//单次均匀变异得到新的方案
{
	double temp;
	int temp1 = (int)AverageRandom(0, SelPlanArray1.size());//方案号
	int n = SelPlanArray1[temp1].SlopePtArray.size() - 2;//变坡点个数
	int ModifySlopNum = (int)n*0.5;

	TempPlan = SelPlanArray1[temp1];

	int temp2;
	for (int i = 0; i < ModifySlopNum; i++)//按百分之十变异变坡点
	{
		temp2 = (int)AverageRandom(1, n);//变坡点号

		double minH, maxH;
		double FrontMileage;//前一个变坡点的里程
		double Fronthigh;//前一个变坡点的高程
		double NowMileage;//当前变坡点的里程
		FrontMileage = TempPlan.SlopePtArray[temp2 - 1].Mileage;
		Fronthigh = TempPlan.SlopePtArray[temp2 - 1].DesH;
		NowMileage = TempPlan.SlopePtArray[temp2].Mileage;

		//double tempPD = (m_EndDesH - Fronthigh) / (m_pPlain->JDarray[0][3] + m_pPlain->xl_length - FrontMileage);
		double tempPD = (m_EndDesH - Fronthigh) / (m_Endml - FrontMileage);
		double miu = Fronthigh + tempPD * (NowMileage - FrontMileage);//以对角线确定的高程为期望值

		double frontminH = 0.0;
		double frontmaxH = 0.0;

		FrontGetRange(TempPlan, temp2, frontminH, frontmaxH);

		double backminH = 0.0;
		double backmaxH = 0.0;
		BackGetRange(TempPlan, temp2, backminH, backmaxH);

		minH = max(frontminH, backminH);
		maxH = min(frontmaxH, backmaxH);

		if (minH > maxH + PRESION)
		{
			i--;
			continue;
		}
		else
		{
			if (miu<minH + PRESION || miu + PRESION>maxH)
				miu = (minH + maxH) / 2.0;

			double sigma;//方差
			sigma = 1.0 / 3.0*min(miu - minH, maxH - miu);

			if (sigma < 0.01 || fabs(minH - maxH) < PRESION)
				temp = (minH + maxH) / 2.0;
			else
				temp = AverageRandom(minH, maxH);
		}


		if (fabs(temp - TempPlan.SlopePtArray[temp2].DesH) < 0.1)//假如变异不成功，则再变异一次
		{
			i--;
			continue;
		}

		TempPlan.SlopePtArray[temp2].DesH = temp;
		CalAllPD(TempPlan);


	}
	return true;
}

//由前一个变坡点得到高程范围
bool CProfileOpt::FrontGetRange(ProfilePlan& TempP, int NowPdNum, double& minH, double& maxH)
{
	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程

	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;

	double temppd = 0.0;

	if (TempP.SlopePtArray[NowPdNum].Style == 0)
		temppd = m_StaMaxSlope;
	else
		temppd = TempP.SlopePtArray[NowPdNum].LimitGrade;

	if (NowPdNum == 1)
	{
		FrontMileage = TempP.SlopePtArray[NowPdNum - 1].Mileage;
		Fronthigh = TempP.SlopePtArray[NowPdNum - 1].DesH;
		NowMileage = TempP.SlopePtArray[NowPdNum].Mileage;

		minH = Fronthigh - (NowMileage - FrontMileage)*temppd;//最小高程
		maxH = Fronthigh + (NowMileage - FrontMileage)*temppd;//最大高程
	}
	else
	{
		FrontfrontMileage = TempP.SlopePtArray[NowPdNum - 2].Mileage;
		Frontfronthigh = TempP.SlopePtArray[NowPdNum - 2].DesH;

		FrontMileage = TempP.SlopePtArray[NowPdNum - 1].Mileage;
		Fronthigh = TempP.SlopePtArray[NowPdNum - 1].DesH;

		FrontPD = (Fronthigh - Frontfronthigh) / (FrontMileage - FrontfrontMileage);
		NowMileage = TempP.SlopePtArray[NowPdNum].Mileage;

		//if (Ngckzd>2)
		//{
		//	tempPD = temppd;
		//	maxH = Fronthigh+(NowMileage-FrontMileage)*tempPD;//最大高程

		//	tempPD = -temppd;
		//	minH = Fronthigh+(NowMileage-FrontMileage)*tempPD;//最小高程
		//}

		//else
		{
			tempPD = min(temppd, (FrontPD + m_MaxSlopeDifference));//考虑了最大坡度代数差
			maxH = Fronthigh + (NowMileage - FrontMileage)*tempPD;//最大高程

			tempPD = max(-temppd, (FrontPD - m_MaxSlopeDifference));//考虑了最大坡度代数差
			minH = Fronthigh + (NowMileage - FrontMileage)*tempPD;//最小高程
		}




	}

	double TempMinH;//临时最小高程
	double TempMaxH;//临时最大高程
	GetGCKZD(FrontMileage, Fronthigh, m_Endml, m_EndDesH);
	GetEnvelopRange(Fronthigh, FrontMileage, NowMileage, TempMinH, TempMaxH);

	minH = max(minH, TempMinH);
	maxH = min(TempMaxH, maxH);
	if (minH > maxH + 0.001)//如果信封区域与坡度代数差无法同时考虑，只考虑信封区域
	{
		minH = TempMinH;
		maxH = TempMaxH;
	}
	return true;

}

//由后一个变坡点得到高程范围
bool CProfileOpt::BackGetRange(ProfilePlan& TempP, int NowPdNum, double& minH, double& maxH)
{
	double BackbackMileage = 0.0;//前两个变坡点的里程
	double Backbackthigh = 0.0;//前两个变坡点的高程

	double BackMileage = 0.0;//前一个变坡点的里程
	double Backhigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double BackPD = 0.0;//前一个坡度值
	double tempPD = 0.0;
	double FrontMileage = TempP.SlopePtArray[NowPdNum - 1].Mileage;
	double Fronthigh = TempP.SlopePtArray[NowPdNum - 1].DesH;
	double temppd = 0.0;

	if (TempP.SlopePtArray[NowPdNum + 1].Style == 0)
		temppd = m_StaMaxSlope;
	else
		temppd = TempP.SlopePtArray[NowPdNum + 1].LimitGrade;

	if (NowPdNum == TempP.SlopePtArray.size() - 2)//变坡点在倒数第二个上
	{
		BackMileage = TempP.SlopePtArray[NowPdNum + 1].Mileage;
		Backhigh = TempP.SlopePtArray[NowPdNum + 1].DesH;
		NowMileage = TempP.SlopePtArray[NowPdNum].Mileage;

		minH = Backhigh - (BackMileage - NowMileage)*temppd;//最小高程
		maxH = Backhigh + (BackMileage - NowMileage)*temppd;//最大高程
	}
	else
	{
		BackbackMileage = TempP.SlopePtArray[NowPdNum + 2].Mileage;
		Backbackthigh = TempP.SlopePtArray[NowPdNum + 2].DesH;

		BackMileage = TempP.SlopePtArray[NowPdNum + 1].Mileage;
		Backhigh = TempP.SlopePtArray[NowPdNum + 1].DesH;

		BackPD = (Backbackthigh - Backhigh) / (BackbackMileage - BackMileage);

		NowMileage = TempP.SlopePtArray[NowPdNum].Mileage;

		//if (Ngckzd>2)
		//{
		//	tempPD = temppd;
		//	maxH = Backhigh+(BackMileage-NowMileage)*tempPD;//最大高程

		//	tempPD = -temppd;
		//	minH = Backhigh+(BackMileage-NowMileage)*tempPD;//最小高程
		//}

		//else
		{
			tempPD = min(temppd, -(BackPD - m_MaxSlopeDifference));//考虑了最大坡度代数差
			maxH = Backhigh + (BackMileage - NowMileage)*tempPD;//最大高程

			tempPD = max(-temppd, -(BackPD + m_MaxSlopeDifference));//考虑了最大坡度代数差
			minH = Backhigh + (BackMileage - NowMileage)*tempPD;//最小高程
		}
	}


	return true;

}

//交叉算子
void CProfileOpt::CrossOperator()
{
	int CrossNum = 0;
	CrossNum = int(m_PopulationSize * CrossProbablity);
	ProfilePlan tempPlan;
	ProfilePlan tempPlan1, tempPlan2;

	double FillArea, CutArea;

	if (CrossPlanArray.size() > 0)
	{
		for (int i = 0; i < CrossPlanArray.size(); i++)
		{
			CrossPlanArray[i].SlopePtArray.clear();
			CrossPlanArray[i].BriArray.clear();
			CrossPlanArray[i].TunArray.clear();
		}
		CrossPlanArray.clear();
	}

	if (CrossNum > 0)
	{
		CrossPlanArray.resize(CrossNum);
		bool getbest = false;
		for (int i = 0; i < CrossNum; i++)
		{
			if (i == 0)
			{
				getbest = true;//用于保证每代进化当中，至少有一次选中最优方案
			}

			//////////////////////////////算术交叉和启发式交叉混合利用/////////////////////////////

			int ProbabiltyNum = 0;
			ProbabiltyNum = (int)AverageRandom(1, 5);
			if (ProbabiltyNum == 1)//算术交叉占1/4的概率
			{
				bool temp = ArithmeticCross(tempPlan1, tempPlan2, getbest);

				if (temp == false)
				{
					i--;
					continue;
				}
				CalEarthWork(tempPlan1, FillArea, CutArea);
				CalEarthWork(tempPlan2, FillArea, CutArea);

				if (tempPlan1.Cost > tempPlan2.Cost)
				{
					CrossPlanArray[i] = tempPlan2;
				}
				else
				{
					CrossPlanArray[i] = tempPlan1;
				}
			}
			else//启发式交叉占3/4的概率
			{
				bool temp = HeuristicCross(tempPlan, getbest);

				if (temp == false)
				{
					i--;
					continue;
				}
				//CalEarthWork(tempPlan,FillArea,CutArea);

				CrossPlanArray[i] = tempPlan;
			}

			if (TempStopOpt)//提前退出
				return;

		}
	}


}

bool CProfileOpt::ArithmeticCross(ProfilePlan& tempPlan1, ProfilePlan& tempPlan2, bool GetBest)//算术交叉
{
	//随机得到两个方案
	int  temp1, temp2;
	temp1 = 0;
	temp2 = 0;
	int num = SelPlanArray.size() - 1;

	double CrossPra;//两个方案之间的交叉概率
	CrossPra = AverageRandom(0, 1);
	while (temp1 == temp2)
	{
		if (!GetBest)
			temp1 = (int)AverageRandom(0, num);//方案2的序号
		temp2 = (int)AverageRandom(0, num);//方案2的序号
	}

	double TempDesH = 0.0;
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double minH = 0.0;
	double maxH = 0.0;

	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程

	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;

	tempPlan1 = SelPlanArray[temp2];
	int i;
	for (i = 1; i < SelPlanArray[temp2].SlopePtArray.size() - 1; i++)
	{
		TempDesH = (1 - CrossPra)*SelPlanArray[temp1].SlopePtArray[i].DesH + CrossPra * SelPlanArray[temp2].SlopePtArray[i].DesH;

		NowMileage = tempPlan1.SlopePtArray[i].Mileage;

		double frontminH = 0.0;
		double frontmaxH = 0.0;

		FrontGetRange(tempPlan1, i, frontminH, frontmaxH);

		double backminH = 0.0;
		double backmaxH = 0.0;

		BackGetRange(tempPlan1, i, backminH, backmaxH);

		minH = max(frontminH, backminH);
		maxH = min(frontmaxH, backmaxH);

		if (minH > maxH + PRESION)
		{
			TempDesH = 0.5*(maxH + minH);
			//return false;
		}

		else if (TempDesH > maxH + PRESION)
		{
			TempDesH = CrossPra * (maxH - SelPlanArray[temp2].SlopePtArray[i].DesH) + SelPlanArray[temp2].SlopePtArray[i].DesH;
		}
		else if (TempDesH + PRESION < minH)
		{
			TempDesH = -CrossPra * (SelPlanArray[temp2].SlopePtArray[i].DesH - minH) + SelPlanArray[temp2].SlopePtArray[i].DesH;
		}
		tempPlan1.SlopePtArray[i].DesH = TempDesH;
		CalAllPD(tempPlan1);
	}

	tempPlan2 = SelPlanArray[temp1];
	for (i = 1; i < SelPlanArray[temp1].SlopePtArray.size() - 1; i++)
	{
		TempDesH = (1 - CrossPra)*SelPlanArray[temp1].SlopePtArray[i].DesH + CrossPra * SelPlanArray[temp1].SlopePtArray[i].DesH;

		double frontminH = 0.0;
		double frontmaxH = 0.0;

		FrontGetRange(tempPlan2, i, frontminH, frontmaxH);

		double backminH = 0.0;
		double backmaxH = 0.0;
		BackGetRange(tempPlan2, i, backminH, backmaxH);

		minH = max(frontminH, backminH);

		maxH = min(frontmaxH, backmaxH);

		if (minH > maxH + PRESION)
		{
			TempDesH = 0.5*(maxH + minH);
			//return false;
		}

		else if (TempDesH > maxH + PRESION)
		{
			TempDesH = CrossPra * (maxH - SelPlanArray[temp1].SlopePtArray[i].DesH) + SelPlanArray[temp1].SlopePtArray[i].DesH;
		}
		else if (TempDesH + PRESION < minH)
		{
			TempDesH = -CrossPra * (SelPlanArray[temp1].SlopePtArray[i].DesH - minH) + SelPlanArray[temp1].SlopePtArray[i].DesH;
		}

		tempPlan2.SlopePtArray[i].DesH = TempDesH;
		CalAllPD(tempPlan2);
	}
	return true;

}

bool CProfileOpt::HeuristicCross(ProfilePlan& tempPlan, bool GetBest)//启发式交叉
{
	//随机得到两个方案
	int  temp1, temp2;
	int num = SelPlanArray.size() - 1;

	double CrossPra;//两个方案之间的交叉概率
	CrossPra = AverageRandom(0, 1);
	// 	ads_printf("CrossPra=%lf\n",CrossPra);
	temp1 = 0;
	temp2 = 0;
	while (temp1 == temp2)
	{
		// 		if (!GetBest)		
		temp1 = (int)AverageRandom(0, num);//方案2的序号
		temp2 = (int)AverageRandom(0, num);//方案2的序号
	}

	double TempDesH = 0.0;
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double minH = 0.0;
	double maxH = 0.0;

	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程

	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;

	if (SelPlanArray[temp1].Cost > SelPlanArray[temp2].Cost)//temp2方案优于temp1方案
		tempPlan = SelPlanArray[temp2];
	else
		tempPlan = SelPlanArray[temp1];

	for (int i = 1; i < SelPlanArray[temp2].SlopePtArray.size() - 1; i++)
	{
		//////////////////////////////////////////////////////////////////////////
		if (SelPlanArray[temp1].SlopePtArray[i].Cost > SelPlanArray[temp2].SlopePtArray[i].Cost)//方案temp2对应的坡段较优
			TempDesH = CrossPra * (SelPlanArray[temp2].SlopePtArray[i].DesH - SelPlanArray[temp1].SlopePtArray[i].DesH) + tempPlan.SlopePtArray[i].DesH;
		else
			TempDesH = CrossPra * (SelPlanArray[temp1].SlopePtArray[i].DesH - SelPlanArray[temp2].SlopePtArray[i].DesH) + tempPlan.SlopePtArray[i].DesH;

		NowMileage = tempPlan.SlopePtArray[i].Mileage;

		double frontminH = 0.0;
		double frontmaxH = 0.0;
		FrontGetRange(tempPlan, i, frontminH, frontmaxH);

		double backminH = 0.0;
		double backmaxH = 0.0;
		BackGetRange(tempPlan, i, backminH, backmaxH);

		minH = max(frontminH, backminH);
		maxH = min(frontmaxH, backmaxH);


		if (minH > maxH + PRESION)
		{
			TempDesH = 0.5*(maxH + minH);
		}

		else if (TempDesH > maxH + PRESION)
		{
			if (SelPlanArray[temp1].SlopePtArray[i].Cost > SelPlanArray[temp2].SlopePtArray[i].Cost)
				TempDesH = CrossPra * (maxH - tempPlan.SlopePtArray[i].DesH) + tempPlan.SlopePtArray[i].DesH;
			else
				TempDesH = CrossPra * (maxH - tempPlan.SlopePtArray[i].DesH) + tempPlan.SlopePtArray[i].DesH;
		}
		else if (TempDesH + PRESION < minH)
		{
			if (SelPlanArray[temp1].SlopePtArray[i].Cost > SelPlanArray[temp2].SlopePtArray[i].Cost)
				TempDesH = -CrossPra * (tempPlan.SlopePtArray[i].DesH - minH) + tempPlan.SlopePtArray[i].DesH;
			else
				TempDesH = -CrossPra * (tempPlan.SlopePtArray[i].DesH - minH) + tempPlan.SlopePtArray[i].DesH;
		}
		tempPlan.SlopePtArray[i].DesH = TempDesH;
		CalAllPD(tempPlan);

	}
	return true;
}

//交叉算子
void CProfileOpt::CrossOperator_Satisfaction()
{
	int CrossNum = 0;
	CrossNum = int(m_PopulationSize * CrossProbablity);
	ProfilePlan tempPlan;
	ProfilePlan tempPlan1, tempPlan2;

	if (CrossPlanArray.size() > 0)
	{
		for (int i = 0; i < CrossPlanArray.size(); i++)
		{
			CrossPlanArray[i].SlopePtArray.clear();
			CrossPlanArray[i].BriArray.clear();
			CrossPlanArray[i].TunArray.clear();
		}
		CrossPlanArray.clear();
	}

	if (CrossNum > 0)
	{
		CrossPlanArray.resize(CrossNum);
		bool getbest = false;
		int m_CrossNum = 0;
		for (int i = 0; i < CrossNum; i++)
		{
			//if (::PeekMessage(&msg, m_hWnd, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
			//{
			//	if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
			//	{
			//		if (AfxMessageBox(L"优化还没完成，是否真的需要退出优化？", MB_YESNO) == IDYES)
			//		{
			//			TempStopOpt = TRUE;//已退出优化
			//			break;
			//		}
			//	}
			//}

			if (i == 0)
				getbest = true;//用于保证每代进化当中，至少有一次选中最优方案

			if (m_CrossNum > CrossNum * 3)
				m_CrossStyle = false;
			else
				m_CrossStyle = true;

			/////////////////////////////////只利用启发式交叉////////////////////////////////////
			bool temp = HeuristicCross_Satisfaction(tempPlan, getbest);

			if (temp == false)
			{
				m_CrossNum++;
				i--;
				continue;
			}
			else
				m_CrossNum = 0;

			CrossPlanArray[i] = tempPlan;
		}
	}
}

bool CProfileOpt::HeuristicCross_Satisfaction(ProfilePlan& tempPlan, bool GetBest)//启发式交叉
{
	//随机得到两个方案
	int  temp1, temp2;
	int num = SelPlanArray.size() - 1;

	double CrossPra;//两个方案之间的交叉概率
	CrossPra = AverageRandom(0, 1);
	temp1 = 0;
	temp2 = 0;
	while (temp1 == temp2)
	{
		// 		if (!GetBest)		
		temp1 = (int)AverageRandom(0, num);//方案2的序号
		temp2 = (int)AverageRandom(0, num);//方案2的序号
	}

	double TempDesH = 0.0;
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double minH = 0.0;
	double maxH = 0.0;

	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程

	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;

	if (SelPlanArray[temp1].Satisfaction < SelPlanArray[temp2].Satisfaction)//temp2方案优于temp1方案
		tempPlan = SelPlanArray[temp2];
	else
		tempPlan = SelPlanArray[temp1];

	for (int i = 1; i < SelPlanArray[temp2].SlopePtArray.size() - 1; i++)
	{
		//////////////////////////////////////////////////////////////////////////
		if (SelPlanArray[temp1].SlopePtArray[i].Cost > SelPlanArray[temp2].SlopePtArray[i].Cost)//方案temp2对应的坡段较优
			TempDesH = CrossPra * (SelPlanArray[temp2].SlopePtArray[i].DesH - SelPlanArray[temp1].SlopePtArray[i].DesH) + tempPlan.SlopePtArray[i].DesH;
		else
			TempDesH = CrossPra * (SelPlanArray[temp1].SlopePtArray[i].DesH - SelPlanArray[temp2].SlopePtArray[i].DesH) + tempPlan.SlopePtArray[i].DesH;

		NowMileage = tempPlan.SlopePtArray[i].Mileage;

		double frontminH = 0.0;
		double frontmaxH = 0.0;
		FrontGetRange(tempPlan, i, minH, maxH);


		//double backminH=0.0;
		//double backmaxH=0.0;
		//BackGetRange(tempPlan,i,backminH,backmaxH); 

		//minH = max(frontminH,backminH);
		//maxH = min(frontmaxH,backmaxH);	

		if (minH > maxH + PRESION)
		{
			TempDesH = 0.5*(maxH + minH);
			//if (m_CrossStyle)
			//            return false;
			//else//如交叉多次都得不到交叉方案，则直接选用较优方案进入下一代优化
			// return true;
		}

		if (TempDesH > maxH + PRESION)
		{
			TempDesH = maxH;
			//if (SelPlanArray[temp1].SlopePtArray[i].Cost>SelPlanArray[temp2].SlopePtArray[i].Cost)
			// TempDesH =  CrossPra*(maxH - tempPlan.SlopePtArray[i].DesH)+tempPlan.SlopePtArray[i].DesH;
			//else
			// TempDesH =  CrossPra*(maxH - tempPlan.SlopePtArray[i].DesH)+tempPlan.SlopePtArray[i].DesH;
		}
		else if (TempDesH + PRESION < minH)
		{
			TempDesH = minH;
			//if (SelPlanArray[temp1].SlopePtArray[i].Cost>SelPlanArray[temp2].SlopePtArray[i].Cost)
			// TempDesH =  -CrossPra*(tempPlan.SlopePtArray[i].DesH-minH)+tempPlan.SlopePtArray[i].DesH;
			//else
			// TempDesH =  -CrossPra*(tempPlan.SlopePtArray[i].DesH-minH)+tempPlan.SlopePtArray[i].DesH;
		}
		tempPlan.SlopePtArray[i].DesH = TempDesH;
	}
	CalAllPD(tempPlan);
	return true;
}

//判断算法是否结束
bool CProfileOpt::EndAlgorithm()
{
	int BestPlanNum = m_BestPlan.size();
	if (BestPlanNum > m_ENDGeneration)
	{
		//如果连续m_ENDGeneration代的费用差值小于m_ENDMONEY，则循环结束
		double Money = m_BestPlan[BestPlanNum - 1] * m_ENDMONEY / 1000.0;
		if (fabs(m_BestPlan[BestPlanNum - 1] - m_BestPlan[BestPlanNum - (m_ENDGeneration + 1)]) < Money)//？？？
			return false;
	}
	return true;
}

//考虑限制坡度调整方案
void CProfileOpt::AdjustPlanByLimitGrad(ProfilePlan& Profile)
{
	double MaxSlope_SX, MaxSlope_XX;
	double Slope = 0.0;
	for (int i = 0; i < Profile.SlopePtArray.size() - 2; i++)
	{
		CalLimitGrade(Profile, Profile.SlopePtArray[i].Mileage, Profile.SlopePtArray[i + 1].Mileage, MaxSlope_SX, MaxSlope_XX); //(待定)
		Slope = (Profile.SlopePtArray[i + 1].DesH - Profile.SlopePtArray[i].DesH) /
			(Profile.SlopePtArray[i + 1].Mileage - Profile.SlopePtArray[i].Mileage);
		if (Slope > MaxSlope_SX)
		{
			Profile.SlopePtArray[i + 1].DesH = Profile.SlopePtArray[i].DesH +
				MaxSlope_SX * (Profile.SlopePtArray[i + 1].Mileage - Profile.SlopePtArray[i].Mileage);
		}
		if (Slope < -MaxSlope_XX)
		{
			Profile.SlopePtArray[i + 1].DesH = Profile.SlopePtArray[i].DesH -
				MaxSlope_XX * (Profile.SlopePtArray[i + 1].Mileage - Profile.SlopePtArray[i].Mileage);
		}
	}
}

void CProfileOpt::DeleteSlopePtInTun(ProfilePlan& TempPlan)//删除隧道内的变坡点
{

	double SlopeSml = 0.0;
	double SlopeEml = 0.0;

	for (int i = 0; i < TempPlan.TunArray.size(); i++)//处理隧道
	{
		double TunSml = m_section_Tun_Bri[i].minS;
		double TunEml = m_section_Tun_Bri[i].maxS;
		int StartIndex = -1;
		int EndIndex = -1;
		for (int iSolpe = 0; iSolpe < TempPlan.SlopePtArray.size() - 1; iSolpe++)
		{
			double tempMileage = TempPlan.SlopePtArray[iSolpe].Mileage;
			if (tempMileage > TunSml + 0.1 && tempMileage < TunEml - 0.1 && StartIndex == -1)//变坡点在隧道范围内，而且是第一次出现
			{
				if (TempPlan.SlopePtArray[iSolpe].Style != 0)//并且不在车站内
				{
					StartIndex = iSolpe;//隧道范围内的第一个变坡点
				}
			}
			if (tempMileage > TunEml + 0.1)
			{
				EndIndex = iSolpe - 1;
				if (StartIndex > -1 && EndIndex >= StartIndex)
				{
					if (SlopePtCanDel(TempPlan, StartIndex, EndIndex))//如果隧道内的点可以直接删除，将隧道内的点全部删除
						//TempPlan.SlopePtArray.RemoveAt(StartIndex, EndIndex - StartIndex + 1);
						TempPlan.SlopePtArray.erase(TempPlan.SlopePtArray.begin() + StartIndex, TempPlan.SlopePtArray.begin()+EndIndex - StartIndex + 1);
					else//删除隧道内的凹形坡
					{
						for (int iBPD = StartIndex; iBPD < EndIndex; iBPD++)
						{
							if (iBPD > 0 && iBPD < TempPlan.SlopePtArray.size() - 1)
							{
								double BefGrid, AftGrid;
								BefGrid = (TempPlan.SlopePtArray[iBPD].DesH - TempPlan.SlopePtArray[iBPD - 1].DesH) /
									(TempPlan.SlopePtArray[iBPD].Mileage - TempPlan.SlopePtArray[iBPD - 1].Mileage);
								AftGrid = (TempPlan.SlopePtArray[iBPD + 1].DesH - TempPlan.SlopePtArray[iBPD].DesH) /
									(TempPlan.SlopePtArray[iBPD + 1].Mileage - TempPlan.SlopePtArray[iBPD].Mileage);
								if (BefGrid<-0.000001 && AftGrid>0.000001)//隧道内的凹形坡
								{
									if (SlopePtCanDel(TempPlan, iBPD, iBPD))
									{
										TempPlan.SlopePtArray.erase(TempPlan.SlopePtArray.begin() + iBPD);
										iBPD--;
										EndIndex--;
									}
								}
							}

						}
					}
				}
				break;
			}
		}
	}
}


//判断Profile方案的SlopetIndex变坡点能否被删除
bool CProfileOpt::SlopePtCanDel(ProfilePlan& Profile, int StartIndex, int EndIndex)
{
	int SlopePtSum = Profile.SlopePtArray.size();
	if (StartIndex < 1 || EndIndex > SlopePtSum - 2)
	{
		return false;
	}

	double MAXSlope_SX, MAXSlope_XX;
	double Slope, BefSlope, AftSlope;
	CalLastPlanLimitGrade(Profile.SlopePtArray[StartIndex - 1].Mileage, Profile.SlopePtArray[EndIndex + 1].Mileage,
		MAXSlope_SX, MAXSlope_XX);

	Slope = (Profile.SlopePtArray[EndIndex + 1].DesH - Profile.SlopePtArray[StartIndex - 1].DesH) /
		(Profile.SlopePtArray[EndIndex + 1].Mileage - Profile.SlopePtArray[StartIndex - 1].Mileage);
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
		BefSlope = (Profile.SlopePtArray[StartIndex - 1].DesH - Profile.SlopePtArray[StartIndex - 2].DesH) /
			(Profile.SlopePtArray[StartIndex - 1].Mileage - Profile.SlopePtArray[StartIndex - 2].Mileage);

		if (fabs(Slope - BefSlope) > m_MaxSlopeDifference + 0.0000001)
			return false;
	}

	if (EndIndex < SlopePtSum - 2)
	{
		AftSlope = (Profile.SlopePtArray[EndIndex + 2].DesH - Profile.SlopePtArray[EndIndex + 1].DesH) /
			(Profile.SlopePtArray[EndIndex + 2].Mileage - Profile.SlopePtArray[EndIndex + 1].Mileage);

		if (fabs(AftSlope - Slope) > m_MaxSlopeDifference + 0.0000001)
			return false;
	}

	return true;
}


//计算最终方案的上下限制坡度
void CProfileOpt::CalLastPlanLimitGrade(double StartMileage, double EndMileage, double& MaxSlope_SX, double& MaxSlope_XX)
{
	bool IsInTun = false;
	double LenInTun = 0.0;
	double MinMileage, MaxMileage;

	//for (int i = 0; i < m_Station.size(); i++) (不考虑车站)
	//{
	//	MinMileage = max(m_Station[i].stratMileage, StartMileage);
	//	MaxMileage = min(m_Station[i].endMileage, EndMileage);

	//	if (MaxMileage > MinMileage + 0.1)
	//	{
	//		MaxSlope_SX = m_StaMaxSlope;
	//		MaxSlope_XX = m_StaMaxSlope;
	//		return;
	//	}
	//}

	double QXZJ_SX = 0.0;
	double QXZJ_XX = 0.0;
	//CalQXZJ(StartMileage, EndMileage, &QXZJ_SX, &QXZJ_XX);

	for (int iTun = 0; iTun < m_section_Tun_Bri.size(); iTun++)
	{
		IsInTun = false;
		if (m_section_Tun_Bri[iTun].minS > EndMileage)
			break;

		MinMileage = max(m_section_Tun_Bri[iTun].minS, StartMileage);
		MaxMileage = min(m_section_Tun_Bri[iTun].maxS, EndMileage);
		if (MaxMileage - MinMileage > 1.0)
			IsInTun = true;
		else if (StartMileage - m_section_Tun_Bri[iTun].minS > -0.01 && StartMileage - m_section_Tun_Bri[iTun].maxS < 0.01)
			IsInTun = true;
		else if (EndMileage - m_section_Tun_Bri[iTun].minS > -0.01 && EndMileage - m_section_Tun_Bri[iTun].maxS < 0.01)
			IsInTun = true;

		if (IsInTun)
		{
			if (m_section_Tun_Bri[iTun].maxS - m_section_Tun_Bri[iTun].minS > LenInTun)
			{
				LenInTun = m_section_Tun_Bri[iTun].maxS - m_section_Tun_Bri[iTun].minS;
			}
		}

	}
	double TunZJ = 1.0;
	if (LenInTun > 1.0)
	{
		//TunZJ = GetTunZJXS(LenInTun);
	}

	MaxSlope_SX = TunZJ * m_MaxLongitudinalSlope - QXZJ_SX / 1000.0;
	MaxSlope_XX = TunZJ * m_MaxLongitudinalSlope - QXZJ_XX / 1000.0;

	//考虑坡度取整后的限制坡度
	double scl = pow(10.0, m_PdXSWS + 3);
	MaxSlope_SX = (int)(MaxSlope_SX*scl + 0.000001) / scl;
	MaxSlope_XX = (int)(MaxSlope_XX*scl + 0.000001) / scl;
}

//计算坡度折减
//D1 坡度起点里程  D2坡度终点里程  *U1 下行坡度折减值 *U2上行坡度折减值
//若下行限坡BIP 上行限坡CIP(为负数）则当前坡度i可行域  CIP+ *U2  < i < BIP - *U1
void CProfileOpt::CalQXZJ(double D1, double D2, double *U1, double *U2)
{
//	double DD1, DD2, RR, DDI, LCC, SUM, U, U3, U4, PDC;
//	int IRS, BO1, BO2, K1, K2, KK1, KK2, i, j;
//	double ZH[5 * MAXBPDNUM], HZ[5 * MAXBPDNUM], LS1[5 * MAXBPDNUM], LS2[5 * MAXBPDNUM], RADIO[5 * MAXBPDNUM];
//	int NQXS;
//	double BIP, CIP;//上，下行限坡
//
//	if (!m_pPlain->mGuiFanCS.ZheJian)
//	{
//		*U1 = *U2 = 0.0;
//		return;
//	}
//	LCC = m_pPlain->mGuiFanCS.DaoFaXian - 50.0;
//	BIP = m_MaxLongitudinalSlope;
//	CIP = -1 * m_MaxLongitudinalSlope;
//
//	// 列车长=到发现有效长-50.0;
//	BO1 = 1;
//	BO2 = 1;
//	K2 = 0;
//	PDC = D2 - D1;
//	SUM = 0.0;
//	U = 0.0;
//	U3 = 0.0;
//	U4 = 0.0;
//
//
//	NQXS = pzLinep->NJD;
//	for (i = 0; i < pzLinep->NJD; i++)
//	{
//		ZH[i] = pzLinep->QXB[i][0];
//		HZ[i] = pzLinep->QXB[i][3];
//		LS1[i] = pzLinep->QXB[i][1] - pzLinep->QXB[i][0];
//		LS2[i] = pzLinep->QXB[i][3] - pzLinep->QXB[i][2];
//		RADIO[i] = pzLinep->JDarray[i][4];
//		//  ads_printf("i=%d %lf %lf ls1-%lf %lf R-%lf\n",i,ZH[i],HZ[i],LS1[i],LS2[i],RADIO[i]);
//	}
//	i = 1;
//	//判断坡段起点在那个曲线上k1 曲线号 
//	while (D1 > ZH[i] + 0.5*LS1[i])
//	{
//		i = i + 1;
//		if (i == NQXS - 1) break;
//	};
//
//	if (i == NQXS - 1) { K1 = NQXS - 2; K2 = NQXS - 2; }
//	else K1 = i - 1;
//	//  ads_printf("1:i=%d K1=%d\n",i,K1);
//	//相对D1 如果在曲线范围内 BO1 = 0 else BO1 = 1  
//	//相对D2 如果在曲线范围内 BO2 = 0 else BO2 = 1  
//	if (D1 < HZ[K1] - 0.5*LS2[K1]) { BO1 = 0; K1 = K1 - 1; };
//	for (i = K1 + 1; i < NQXS - 1; i++)
//	{
//		if (D2 <= ZH[i] + 0.5*LS1[i])
//			//坡度终点在本曲线前面
//		{
//			BO2 = 1;
//			K2 = i - 1;
//			goto l40;
//		}
//		else if (D2 > HZ[i] - 0.5*LS2[i])
//			//坡度终点在本曲线后面 SUM 坡度范围内累计偏角
//		{
//			if (D1 > ZH[i] + 0.5*LS1[i]) SUM = SUM + (HZ[i] - 0.5*LS2[i] - D1) / fabs(RADIO[i]);
//			else                           SUM = SUM + (HZ[i] - ZH[i] - 0.5*(LS1[i] + LS2[i])) / fabs(RADIO[i]);
//		}
//		else
//			//坡度终点在本曲线里面
//		{
//			if (D1 > ZH[i] + 0.5*LS1[i]) SUM = SUM + PDC / fabs(RADIO[i]);
//			else                           SUM = SUM + (D2 - ZH[i] - 0.5*LS1[i]) / fabs(RADIO[i]);
//			BO2 = 0;
//			K2 = i;
//		};
//		if (i == NQXS - 2)
//		{
//			K2 = NQXS - 2;
//			if (HZ[K2] + 0.5*LS2[K2] <= D2)
//			{
//				BO2 = 1;
//				goto l40;
//			};
//		};
//		if (HZ[i] - 0.5*LS2[i] >= D2) goto l40;
//	}; /* exit for */
//l40:  IRS = K2 - K1;
//	//	  ads_printf("2:K2=%d K1=%d Bo1=%d Bo2=%d\n",K2,K1,BO1,BO2);
//	//      本坡段所跨越的曲线数
//	/*THE FALLOWING IS IRS=0 (NO RADIS) */
//	/*  IF IRS <> 0   DDI  当前曲线范围内的坡度长度*/
//	if (BO1 == 1 && BO2 == 1)  DDI = HZ[K2] - 0.5*LS2[K2] - ZH[K1 + 1] - 0.5*LS1[K1 + 1];
//	else if (BO1 == 1 && BO2 == 0)  DDI = D2 - ZH[K1 + 1] - 0.5*LS1[K1 + 1];
//	else if (BO1 == 0 && BO2 == 1)  DDI = HZ[K2] - 0.5*LS2[K2] - D1;
//	else  DDI = D2 - D1;
//	//ads_printf("k1=%d k2=%d bip=%lf cip=%lf ZH=%lf bo1=%d IRS=%d DDI=%lf LCC=%lf DFXYXC=%lf\n",K1,K2,BIP,CIP,ZH[K1],BO1,IRS,DDI,LCC,DFXYXC);
//
//	/*-----THE FALLOWING IS IRS=1 　坡度内只有一个曲线 */
//	// LCC 列车长＝到发线有效长－50
//	if (IRS == 1)
//	{
//		//	  ads_printf("pdc=%lf SUM=%lf\n",PDC,SUM);
//		if ((BO1 == 0 && BO2 == 0) || DDI > LCC)  U = 600.0 / fabs(RADIO[K2]);//坡段起终点都在曲线上，坡段长大于列车长
//		else
//		{
//			PDC = min(PDC, LCC);
//			U = 10.5*SUM * 180 / pi / PDC;
//		};
//		RR = fabs(RADIO[K2]);
//		// U3 U4 小半径折减值
//		LRADCON(RR, BIP, CIP, &U3, &U4);
//		*U1 = U + U3;
//		*U2 = U + U4;
//		//		  ads_printf("U=%lf U3=%lf %lf\n",U,U3,*U1);
//	}
//	else if (IRS > 1)
//	{
//		//小于列车长，取坡度长列车长最小值
//		//	  ads_printf("pdc=%lf lcc=%lf\n",PDC,LCC);
//		if (DDI <= LCC)
//		{
//			RR = 4000.0;
//			PDC = min(PDC, LCC);
//			U = 10.50*SUM * 180 / pi / PDC;
//			for (j = K1 + 1; j <= K2; j++) RR = min(fabs(RADIO[j]), RR);
//			LRADCON(RR, BIP, CIP, &U3, &U4);
//			*U1 = U + U3;
//			*U2 = U + U4;
//		}
//		else
//			/*----THE FALLOWING IS (IRS>1 AND  DDI>SSL) */
//			//大于列车长，取进行最不利组合
//		{
//			KK1 = K1 + 1;
//			if (BO1 == 1)
//			{
//				DD1 = ZH[KK1] + 0.5*LS1[KK1];
//			}
//			else
//			{
//				DD1 = D1;
//			};
//
//			*U1 = 0.0;
//			*U2 = 0.0;
//			DD2 = DD1 + LCC;
//			//  ads_printf("1\n");
//			do
//			{
//				U = 0.0;
//				if (DD2 >= D2) break;
//				RR = 4000.0;			  //ads_printf("2\n");
//				for (j = KK1; j <= K2; j++)
//				{
//					if (HZ[j] - 0.5*LS2[j] <= DD2)
//					{
//						if (j == KK1) U = 10.50*(HZ[j] - 0.5*LS2[j] - DD1) / fabs(RADIO[j]) * 180 / pi / LCC + U;
//						else            U = 10.50*(HZ[j] - ZH[j] - 0.5*(LS1[j] + LS2[j])) / fabs(RADIO[j]) * 180 / pi / LCC + U;
//						RR = min(fabs(RADIO[j]), RR);
//					}
//					else if (ZH[j] + 0.5*LS1[j] <= DD2)
//					{
//						if (j == KK1 && BO1 == 0) U = 600.0 / fabs(RADIO[KK1]);
//						else U = 10.50*(DD2 - ZH[j] - 0.5*LS1[j]) / fabs(RADIO[j]) * 180 / pi / LCC + U;
//						RR = min(fabs(RADIO[j]), RR);
//					}
//					else goto l210;
//				};
//l210:         LRADCON(RR, BIP, CIP, &U3, &U4);
//				U3 = U + U3;
//				U4 = U + U4;
//				*U1 = max(*U1, U3);
//				*U2 = max(*U2, U4);
//				KK1 = KK1 + 1;
//				DD1 = ZH[KK1] + 0.5*LS1[KK1];
//				DD2 = DD1 + LCC;
//			} while (DD2 < D2);
//
//			KK2 = K2;
//			if (BO2 == 1) DD2 = HZ[K2] - 0.5*LS2[K2];
//			else DD2 = D2;
//			DD1 = DD2 - LCC;
//			do
//			{
//				U = 0.0;
//				if (DD1 < D1) break;
//				RR = 4000.0;
//				for (j = KK2; j >= K1; j = j - 1)
//				{
//					if (ZH[j] + 0.5*LS1[j] > DD1)
//					{
//						if (j == K2) U = 10.50*(DD2 - ZH[j] - 0.5*LS1[j]) / fabs(RADIO[j]) * 180 / pi / LCC + U;
//						else           U = 10.50*(HZ[j] - ZH[j] - 0.5*(LS1[j] + LS2[j])) / fabs(RADIO[j]) * 180 / pi / LCC + U;
//						RR = min(fabs(RADIO[j]), RR);
//					}
//					else if (HZ[j] - 0.5*LS2[j] >= DD1)
//					{
//						if (BO2 == 0 && j == K2) U = 600.0 / fabs(RADIO[j]);
//						else   U = 10.50*(HZ[j] - 0.5*LS2[j] - DD1) / fabs(RADIO[j]) * 180 / pi / LCC + U;
//						RR = min(fabs(RADIO[j]), RR);
//					}
//					else goto l310;
//				};
//l310:              LRADCON(RR, BIP, CIP, &U3, &U4);
//				U3 = U + U3;
//				U4 = U + U4;
//				*U1 = max(*U1, U3);
//				*U2 = max(*U2, U4);
//				KK2 = KK2 - 1;
//				DD2 = HZ[KK2] - 0.5*LS2[KK2];
//				DD1 = DD2 - LCC;
//			} while (DD1 > D1);
//		};/* exit if ddi > ssl */
//	} /* EXIT IF IRS > 1 */
//	else if (IRS == 0)
//		// 没有曲线折减值为 ０　
//	{
//		*U1 = 0.0;
//		*U2 = 0.0;
//	};
//	//	  ads_printf("####%lf %lf\n",*U1,*U2);
}

//考虑坡度代数差调整坡度
void CProfileOpt::AdjustPlanByDsc(ProfilePlan& Profile)
{
	double y1, y2, y3, x1, x2, x3;
	double a, b;
	double dsc = 0.0;
	double MaxLever, MinLever;
	double BefGradient, AftGradient;

	for (int i = 1; i < Profile.SlopePtArray.size() - 1; i++)
	{

		y1 = Profile.SlopePtArray[i - 1].DesH;
		y2 = Profile.SlopePtArray[i].DesH;
		y3 = Profile.SlopePtArray[i + 1].DesH;

		x1 = Profile.SlopePtArray[i - 1].Mileage;
		x2 = Profile.SlopePtArray[i].Mileage;
		x3 = Profile.SlopePtArray[i + 1].Mileage;

		a = x2 - x1;
		b = x3 - x2;

		BefGradient = (y2 - y1) / (x2 - x1);
		AftGradient = (y3 - y2) / (x3 - x2);

		dsc = BefGradient - AftGradient;
		if (dsc > m_MaxSlopeDifference)
		{
			if (SlopePtLeverRange(Profile, i, MaxLever, MinLever))
				Profile.SlopePtArray[i].DesH = MaxLever;
		}
		if (dsc < -m_MaxSlopeDifference)
		{
			if (SlopePtLeverRange(Profile, i, MaxLever, MinLever))
				Profile.SlopePtArray[i].DesH = MinLever;
		}
	}
}

//计算Profile方案的Index变坡点的可动范围，不可动返回false
bool CProfileOpt::SlopePtLeverRange(ProfilePlan& Profile, int Index, double& MaxLever, double& MinLever)
{
	int SlopePtSum = Profile.SlopePtArray.size();
	if (Index < 1)
	{
		MaxLever = m_StartDesH;
		MinLever = m_StartDesH;
		return false;
	}
	if (Index > SlopePtSum - 2)
	{
		MaxLever = m_EndDesH;
		MinLever = m_EndDesH;
		return false;
	}

	double MAXSlope_SX, MAXSlope_XX;
	double Slope, BefSlope, AftSlope;
	CalLastPlanLimitGrade(Profile.SlopePtArray[Index - 1].Mileage, Profile.SlopePtArray[Index].Mileage,
		MAXSlope_SX, MAXSlope_XX);

	//////////////////////////////////////////////////////////////////////////
	//由前往后推标高范围
	double Front_MinLever, Front_MaxLever;
	double FrontMileage, Fronthigh;
	double NowMileage;
	if (Index == 1)//只需考虑限制坡度要求
	{
		FrontMileage = Profile.SlopePtArray[Index - 1].Mileage;
		Fronthigh = Profile.SlopePtArray[Index - 1].DesH;
		NowMileage = Profile.SlopePtArray[Index].Mileage;

		Front_MinLever = Fronthigh - (NowMileage - FrontMileage)*MAXSlope_XX;//最小高程
		Front_MaxLever = Fronthigh + (NowMileage - FrontMileage)*MAXSlope_XX;//最大高程
	}
	else
	{
		double FrontfrontMileage = Profile.SlopePtArray[Index - 2].Mileage;
		double Frontfronthigh = Profile.SlopePtArray[Index - 2].DesH;

		FrontMileage = Profile.SlopePtArray[Index - 1].Mileage;
		Fronthigh = Profile.SlopePtArray[Index - 1].DesH;

		double FrontPD = (Fronthigh - Frontfronthigh) / (FrontMileage - FrontfrontMileage);
		NowMileage = Profile.SlopePtArray[Index].Mileage;
		double tempPD = 0;//肃报
		tempPD = min(tempPD, (FrontPD + MAXSlope_SX));//考虑了最大坡度代数差
		Front_MaxLever = Fronthigh + (NowMileage - FrontMileage)*tempPD;//最大高程

		tempPD = max(-tempPD, (FrontPD - MAXSlope_XX));//考虑了最大坡度代数差
		Front_MinLever = Fronthigh + (NowMileage - FrontMileage)*tempPD;//最小高程
	}

	//////////////////////////////////////////////////////////////////////////
	//由后变坡点往前推	
	double Back_MinLever, Back_MaxLever;
	double BackMileage, Backhigh;

	if (Index == Profile.SlopePtArray.size() - 2)//变坡点在倒数第二个上
	{
		BackMileage = Profile.SlopePtArray[Index + 1].Mileage;
		Backhigh = Profile.SlopePtArray[Index + 1].DesH;
		NowMileage = Profile.SlopePtArray[Index].Mileage;

		Back_MinLever = Backhigh - (BackMileage - NowMileage)*MAXSlope_SX;//最小高程
		Back_MaxLever = Backhigh + (BackMileage - NowMileage)*MAXSlope_XX;//最大高程
	}
	else
	{
		double BackbackMileage = Profile.SlopePtArray[Index + 2].Mileage;
		double Backbackthigh = Profile.SlopePtArray[Index + 2].DesH;

		BackMileage = Profile.SlopePtArray[Index + 1].Mileage;
		Backhigh = Profile.SlopePtArray[Index + 1].DesH;

		double BackPD = (Backbackthigh - Backhigh) / (BackbackMileage - BackMileage);

		NowMileage = Profile.SlopePtArray[Index].Mileage;

		double tempPD = min(MAXSlope_XX, -(BackPD - m_MaxSlopeDifference));//考虑了最大坡度代数差
		Back_MaxLever = Backhigh + (BackMileage - NowMileage)*tempPD;//最大高程

		tempPD = max(-MAXSlope_SX, -(BackPD + m_MaxSlopeDifference));//考虑了最大坡度代数差
		Back_MinLever = Backhigh + (BackMileage - NowMileage)*tempPD;//最小高程
	}

	MinLever = max(Front_MinLever, Back_MinLever);
	MaxLever = min(Front_MaxLever, Back_MaxLever);

	if (MaxLever < MinLever + 0.0001)
		return false;

	//考虑该变坡点本身的坡度代数差
	double a = NowMileage - FrontMileage;
	double b = BackMileage - NowMileage;

	double Now_MaxLever = (m_MaxSlopeDifference*a*b + b * Fronthigh + a * Backhigh) / (a + b);
	double Now_MinLever = (-m_MaxSlopeDifference * a*b + b * Fronthigh + a * Backhigh) / (a + b);

	MaxLever = min(MaxLever, Now_MaxLever);
	MinLever = max(MinLever, Now_MinLever);

	if (MaxLever < MinLever + 0.0001)
		return false;

	return true;
}

void CProfileOpt::MergeSlope(ProfilePlan& Profile)//合并坡度
{
	double BefGrade = 0.0;
	double AftGrade = 0.0;
	for (int i = 1; i < Profile.SlopePtArray.size() - 2; i++)
	{
		AftGrade = (Profile.SlopePtArray[i + 1].DesH - Profile.SlopePtArray[i].DesH)
			/ (Profile.SlopePtArray[i + 1].Mileage - Profile.SlopePtArray[i].Mileage);
		BefGrade = (Profile.SlopePtArray[i].DesH - Profile.SlopePtArray[i - 1].DesH)
			/ (Profile.SlopePtArray[i].Mileage - Profile.SlopePtArray[i - 1].Mileage);
		if (fabs(AftGrade - BefGrade) < m_HbPdc)
		{
			if (SlopePtCanDel(Profile, i, i))
			{
				Profile.SlopePtArray.erase(Profile.SlopePtArray.begin() + i);
				i--;
			}
		}
	}
}

//计算最终方案的费用
void CProfileOpt::CalLastPlanCost(ProfilePlan &TempPlan)
{
	double FillArea = 0.0;
	double CutArea = 0.0;
	CalEarthWork(TempPlan, FillArea, CutArea);
}

// 按地面线，给初始方案赋值
void CProfileOpt::ForFirstPlan()
{
	GetThreeDiffQuotientPt();

	GetOneDiffQuotientPt();

	ChangePt.clear();
	gridPoint temppt;

	/////////////////////////////////加入起点////////////////////////////
	temppt.mile = m_Startml;
	temppt.high = m_StartDesH;
	ChangePt.push_back(temppt);

	for (int Crsi = 0; Crsi < OneDiffQuotientPt.size() - 1; Crsi++)
	{
		if (OneDiffQuotientPt[Crsi].high*1.0e10*OneDiffQuotientPt[Crsi + 1].high*1.0e10 < 0.000001)
		{
			ChangePt.push_back(OneDiffQuotientPt[Crsi]);
		}
	}

	temppt.mile = m_Endml;
	temppt.high = m_EndDesH;
	ChangePt.push_back(temppt);
	////////////////////////////////加入终点///////////////////////////////

	if (m_ProfilePlanArray.size() > 0)
	{
		for (int i = 0; i < m_ProfilePlanArray.size(); i++)
		{
			m_ProfilePlanArray[i].SlopePtArray.clear();
			m_ProfilePlanArray[i].BriArray.clear();
			m_ProfilePlanArray[i].TunArray.clear();
		}
		m_ProfilePlanArray.clear();
	}

	m_ProfilePlanArray.resize(m_PopulationSize);
	m_SlopePtSum = ChangePt.size();
	// 	ads_printf("qqq mileage=%lf\n",ChangePt[m_SlopePtSum-1].mile);

	for (int ProfileIndex = 0; ProfileIndex < m_PopulationSize; ProfileIndex++)
	{
		m_ProfilePlanArray[ProfileIndex].S_high = 0.0;
		m_ProfilePlanArray[ProfileIndex].X_high = 0.0;
		m_ProfilePlanArray[ProfileIndex].Satisfaction = 0.0;

		m_ProfilePlanArray[ProfileIndex].Cost = 0.0;
		m_ProfilePlanArray[ProfileIndex].Fitness = 0;
		m_ProfilePlanArray[ProfileIndex].ProfileIndex = ProfileIndex;
		m_ProfilePlanArray[ProfileIndex].SelProbability = 0.0;

		m_ProfilePlanArray[ProfileIndex].SlopePtArray.resize(m_SlopePtSum);
		for (int SlopePtIndex = 0; SlopePtIndex < m_SlopePtSum; SlopePtIndex++)
		{
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].Mileage = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].VerticalCurveRadius = m_VerticalCurveRadius;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].DesH = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].Gradient = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].Style = 1;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].LimitGrade = m_MaxLongitudinalSlope;//所有坡度均赋值为最大坡度值
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].QXZJ_SX = 0.0;
			m_ProfilePlanArray[ProfileIndex].SlopePtArray[SlopePtIndex].QXZJ_XX = 0.0;
		}
		m_ProfilePlanArray[ProfileIndex].SlopePtArray[0].VerticalCurveRadius = 0.0;
		m_ProfilePlanArray[ProfileIndex].SlopePtArray[m_SlopePtSum - 1].VerticalCurveRadius = 0.0;
	}
}

//计算平顺地面线的三阶差商
void CProfileOpt::GetThreeDiffQuotientPt()
{
	PsDmx(m_MinLongitudinalSlopeL, 5);
	ThreeDiffQuotientPt.clear();
	gridPoint temp;
	for (int i = 0; i < PSdmxPt.size(); i++)
	{
		if (i == 0)
		{
			temp.high = (PSdmxPt[i + 1].high - PSdmxPt[i].high) / (PSdmxPt[i + 1].mile - PSdmxPt[i].mile);
			temp.mile = PSdmxPt[i].mile;
		}
		else if (i == PSdmxPt.size() - 1)
		{
			temp.high = (PSdmxPt[i].high - PSdmxPt[i - 1].high) / (PSdmxPt[i].mile - PSdmxPt[i - 1].mile);
			temp.mile = PSdmxPt[i].mile;
		}
		else
		{
			double temp1 = PSdmxPt[i].high - PSdmxPt[i - 1].high;
			double temp2 = PSdmxPt[i + 1].mile - PSdmxPt[i].mile;
			double temp3 = PSdmxPt[i + 1].high - PSdmxPt[i].high;
			double temp4 = PSdmxPt[i].mile - PSdmxPt[i - 1].mile;
			double temp5 = PSdmxPt[i + 1].mile - PSdmxPt[i - 1].mile;
			temp.high = (temp1*temp2 / temp4 + temp3 * temp4 / temp2) / temp5;
			temp.mile = PSdmxPt[i].mile;
		}
		ThreeDiffQuotientPt.push_back(temp);
	}
}

//计算一阶差商
void CProfileOpt::GetOneDiffQuotientPt()
{
	OneDiffQuotientPt.clear();
	gridPoint temp;
	for (int i = 0; i < ThreeDiffQuotientPt.size(); i++)
	{
		if (i == 0)
		{
			temp.high = (ThreeDiffQuotientPt[i + 1].high - ThreeDiffQuotientPt[i].high) / (ThreeDiffQuotientPt[i + 1].mile - ThreeDiffQuotientPt[i].mile);
			temp.mile = ThreeDiffQuotientPt[i].mile;
		}
		else if (i == ThreeDiffQuotientPt.size() - 1)
		{
			temp.high = (ThreeDiffQuotientPt[i].high - ThreeDiffQuotientPt[i - 1].high) / (ThreeDiffQuotientPt[i].mile - ThreeDiffQuotientPt[i - 1].mile);
			temp.mile = ThreeDiffQuotientPt[i].mile;
		}
		else
		{
			double temp1 = ThreeDiffQuotientPt[i + 1].high - ThreeDiffQuotientPt[i - 1].high;
			double temp2 = ThreeDiffQuotientPt[i + 1].mile - ThreeDiffQuotientPt[i - 1].mile;
			temp.high = temp1 / temp2;
			temp.mile = ThreeDiffQuotientPt[i].mile;
		}
		OneDiffQuotientPt.push_back(temp);
	}
}