#include "stdafx.h"
#include "RoadDesignWorkingDatabase.h"

//构造函数
RoadDesignWorkingDatabase::RoadDesignWorkingDatabase(void)
{
}

/*===================================================================
函数名:CreateHorizontalRoad
------------------------------------------------------------
函数功能:创建道路平面指针
------------------------------------------------------------
输入:	CString		MdbName			方案路径
		CString		RoadName		道路名（默认为“主线”）
		bool		IsReadFromMdb	是否直接从数据库读取
------------------------------------------------------------
输出:	JD_CENTER*	返回值	道路平面指针
------------------------------------------------------------
算法描述:	1.检测现有道路平面指针数组中是否存在需要创建
			方案的平面指针
			2.如果有，直接返回
			3.如果没有，从数据库读取数据创建道路平面指针
------------------------------------------------------------
编写日期:20190805	ljx
------------------------------------------------------------
备注：
=====================================================================*/
JD_CENTER* RoadDesignWorkingDatabase::CreateHorizontalRoad(CString MdbName, CString RoadName, bool IsReadFromMdb)
{
	//20191023新增	直接从数据库读取
	if (IsReadFromMdb)
	{
		JD_CENTER* pRoad = Create_Road_Pointer(MdbName, RoadName);
		if (!pRoad)
			return NULL;
		//寻找原先是否有该线路，如果有，替换
		for (int i = 0; i < m_RoadArray.size(); i++)
		{
			if (_tcscmp(m_RoadArray[i]->mdbname, MdbName) == 0
				&& _tcscmp(m_RoadArray[i]->RoadName, RoadName) == 0)
			{
				m_RoadArray[i] = pRoad;
				return pRoad;
			}
		}
		//如果没有，增加
		m_RoadArray.push_back(pRoad);
		return pRoad;
	}

	//检测指定道路方案平面指针是否存在
	int Road_Index = 0;
	if (Is_Horizontal_Road_Exist(MdbName, RoadName, Road_Index))
		return m_RoadArray[Road_Index];

	//创建道路方案平面指针
	JD_CENTER* pRoad = Create_Road_Pointer(MdbName, RoadName);
	if (pRoad)
		m_RoadArray.push_back(pRoad);

	return pRoad;
}

//设置道路平面指针
void RoadDesignWorkingDatabase::SetHorizontalRoad(JD_CENTER* pRoad)
{
	//寻找原先是否有该线路，如果有，替换
	for (int i = 0; i < m_RoadArray.size(); i++)
	{
		if (_tcscmp(m_RoadArray[i]->mdbname, pRoad->mdbname) == 0
			&& _tcscmp(m_RoadArray[i]->RoadName, pRoad->RoadName) == 0)
		{
			m_RoadArray[i] = pRoad;
			return;
		}
	}
	m_RoadArray.push_back(pRoad);
}

/*===================================================================
函数名:GetGradeAndVelocity
------------------------------------------------------------
函数功能:获取道路等级和设计速度参数
------------------------------------------------------------
输入:	CString		MdbName		方案路径
		CString		RoadName	道路名
------------------------------------------------------------
输出:	CString&	Grade				道路等级
		int&	DesignVelocity		设计速度
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetGradeAndVelocity(CString MdbName, CString RoadName, CString& Grade, int& DesignVelocity)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		Grade = m_RoadDatabaseArray[RoadIndex].Grade;
		DesignVelocity = m_RoadDatabaseArray[RoadIndex].DesignVelocity;
		if (!Grade.IsEmpty() && DesignVelocity != 0)
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Road_Grade_And_Velocity(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		Grade = m_RoadDatabaseArray[RoadIndex].Grade;
		DesignVelocity = m_RoadDatabaseArray[RoadIndex].DesignVelocity;
		return true;
	}

	return false;
}


/*===================================================================
函数名:GetRoadWidthArray
------------------------------------------------------------
函数功能:获取路面宽数据
------------------------------------------------------------
输入:	CString		MdbName				方案名
		int			LorR				左侧或右侧
		CString		RoadName			道路名（默认为“主线”）
------------------------------------------------------------
输出:	std::vector<LMKdata>&	RoadWidthArray	道路宽数据
		bool								返回值			是否成功获取
------------------------------------------------------------
算法描述:	1.检测m_RoadDatabaseArray中是否有该方案
				1.1有该方案，检测是否有道路宽数据
					1.1.1有，直接返回true
					1.1.2没有，转2（分两种情况：1.该数据之前没读取过；2.本来就没该数据）
				1.2没有该方案，转2
			2.尝试从数据库读取
				2.1读取成功，创建方案数据库，返回true
				2.2读取失败，转3
			3.返回false
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetRoadWidthArray(CString MdbName, int LorR, std::vector<LMKdata>& RoadWidthArray, CString RoadName)
{
	//清空数组
	RoadWidthArray.clear();

	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		RoadWidthArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array : m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array;
		if (RoadWidthArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Road_Width_Data(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		RoadWidthArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array : m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array;
		return true;
	}
	
	return false;
}

/*===================================================================
函数名:SetRoadWidthArray
------------------------------------------------------------
函数功能:设置路面宽数据
------------------------------------------------------------
输入:	CString MdbName		方案名
		CString RoadName	道路名
		int LorR			左侧或右侧
		std::vector<LMKdata> RoadWidthArray	道路宽度数组
		bool	IsSaveToMdb	是否存储至数据库，默认为false
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetRoadWidthArray(CString MdbName, CString RoadName, int LorR, std::vector<LMKdata> RoadWidthArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);
	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array = RoadWidthArray;
	else
		m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array = RoadWidthArray;

	if (IsSaveToMdb)
		Write_Road_Width_Data(MdbName, RoadName);
}

/*===================================================================
函数名:GetRoadSuphStd
------------------------------------------------------------
函数功能:获取道路超高标准数据
------------------------------------------------------------
输入:	CString MdbName		方案名
		CString RoadName	道路名
------------------------------------------------------------
输出:	std::vector<std::vector<double>>&	SurpArray	超高标准数据
		CString&	IG			最大横坡
		CString& MAXSuphI	最大超高值
		bool	返回值		true-读取成功 false-读取失败
------------------------------------------------------------
算法描述:	1.检测m_RoadDatabaseArray中是否有该方案
				1.1有该方案，检测是否有超高标准数据
					1.1.1有，直接返回true
					1.1.2没有，转2（分两种情况：1.该数据之前没读取过；2.本来就没该数据）
				1.2没有该方案，转2
			2.尝试从数据库读取
				2.1读取成功，创建方案数据库，返回true
				2.2读取失败，转3
			3.返回false
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetRoadSuphStd(CString MdbName, CString RoadName, std::vector<std::vector<double>>& SurpArray, CString& IG, CString& MAXSuphI)
{
	//清空数组
	SurpArray.clear();

	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		SurpArray = m_RoadDatabaseArray[RoadIndex].SurpArray;
		if (SurpArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Road_Suph_Data(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		SurpArray = m_RoadDatabaseArray[RoadIndex].SurpArray;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetRoadSuphStd
------------------------------------------------------------
函数功能:设置超高标准数据
------------------------------------------------------------
输入:	CString MdbName		方案名
		CString RoadName	道路名
		std::vector<std::vector<double>> SurpArray	超高数据
		CString IG			最大横坡
		CString MAXSuphI	最大超高值
		bool IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetRoadSuphStd(CString MdbName, CString RoadName, std::vector<std::vector<double>> SurpArray, 
	CString IG, CString MAXSuphI, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].SurpArray = SurpArray;
	m_RoadDatabaseArray[RoadIndex].IG = IG;
	m_RoadDatabaseArray[RoadIndex].MAXSuphI = MAXSuphI;

	if (IsSaveToMdb)
		Write_Road_Suph_Data(MdbName, RoadName);
}

/*===================================================================
函数名:GetRoadWidenStd
------------------------------------------------------------
函数功能:获取道路加宽信息
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<std::vector<double>>& WidenArray	加宽数组
		CString&	WidenType	加宽类型
		bool		返回值		true-读取成功 false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetRoadWidenStd(CString MdbName, CString RoadName, std::vector<std::vector<double>>& WidenArray, CString& WidenType)
{
	//清空数组
	WidenArray.clear();

	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		WidenArray = m_RoadDatabaseArray[RoadIndex].WidenArray;
		WidenType = m_RoadDatabaseArray[RoadIndex].WidenType;
		if (WidenArray.size() && !WidenType.IsEmpty())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Road_Widen_Data(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		WidenArray = m_RoadDatabaseArray[RoadIndex].SurpArray;
		WidenType = m_RoadDatabaseArray[RoadIndex].WidenType;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetRoadWidenStd
------------------------------------------------------------
函数功能:设置道路加宽标准数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<std::vector<double>> WidenArray	加宽数组
		CString		WidenType	加宽类型
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetRoadWidenStd(CString MdbName, CString RoadName, std::vector<std::vector<double>> WidenArray, CString WidenType, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].WidenArray = WidenArray;
	m_RoadDatabaseArray[RoadIndex].WidenType = WidenType;

	if (IsSaveToMdb)
		Write_Road_Widen_Data(MdbName, RoadName);
}

/*===================================================================
函数名:GetSuphAndWidenPara
------------------------------------------------------------
函数功能:获取道路超高计算数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	CString&	HxCgJbl			缓和段最小超高渐变率
		CString&	ZxCgJbl			直线段超高渐变率
		int&		WidenMode		加宽方式
		int&		SuphMode		超高方式
		int&		SupNWidLenMode	取整方式
		bool		返回值			true-读取成功 false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetSuphAndWidenPara(CString MdbName, CString RoadName, 
	CString& HxCgJbl, CString& ZxCgJbl, int& WidenMode, int& SuphMode, int& SupNWidLenMode)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		HxCgJbl = m_RoadDatabaseArray[RoadIndex].HxCgJbl;
		ZxCgJbl = m_RoadDatabaseArray[RoadIndex].ZxCgJbl;
		WidenMode = m_RoadDatabaseArray[RoadIndex].WidenMode;
		SuphMode = m_RoadDatabaseArray[RoadIndex].SuphMode;
		SupNWidLenMode = m_RoadDatabaseArray[RoadIndex].SupNWidLenMode;
		if (!HxCgJbl.IsEmpty() && !ZxCgJbl.IsEmpty())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Road_SuphCal_Data(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		HxCgJbl = m_RoadDatabaseArray[RoadIndex].HxCgJbl;
		ZxCgJbl = m_RoadDatabaseArray[RoadIndex].ZxCgJbl;
		WidenMode = m_RoadDatabaseArray[RoadIndex].WidenMode;
		SuphMode = m_RoadDatabaseArray[RoadIndex].SuphMode;
		SupNWidLenMode = m_RoadDatabaseArray[RoadIndex].SupNWidLenMode;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetSuphAndWidenPara
------------------------------------------------------------
函数功能:设置道路超高计算数据
------------------------------------------------------------
输入:	CString		MdbName			方案名
		CString		RoadName		道路名
		CString		HxCgJbl			缓和段最小超高渐变率
		CString		ZxCgJbl			直线段超高渐变率
		int			WidenMode		加宽方式
		int			SuphMode		超高方式
		int			SupNWidLenMode	取整方式
		bool		IsSaveToMdb		是否存储至数据库
------------------------------------------------------------
输出:	
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetSuphAndWidenPara(CString MdbName, CString RoadName, 
	CString HxCgJbl, CString ZxCgJbl, int WidenMode, int SuphMode, int SupNWidLenMode, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].HxCgJbl = HxCgJbl;
	m_RoadDatabaseArray[RoadIndex].ZxCgJbl = ZxCgJbl;
	m_RoadDatabaseArray[RoadIndex].WidenMode = WidenMode;
	m_RoadDatabaseArray[RoadIndex].SuphMode = SuphMode;
	m_RoadDatabaseArray[RoadIndex].SupNWidLenMode = SupNWidLenMode;

	if (IsSaveToMdb)
		Write_Road_SuphCal_Data(MdbName, RoadName);
}

/*===================================================================
函数名:GetSuphResult
------------------------------------------------------------
函数功能:获取道路超高计算结果
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左侧或右侧
------------------------------------------------------------
输出:	std::vector<CgSZ>&		SuphResultArray		超高计算结果数据
		bool		返回值		true-读取成功 false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetSuphResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ>& SuphResultArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		SuphResultArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].SuphResultArray_L : m_RoadDatabaseArray[RoadIndex].SuphResultArray_R;
		if (SuphResultArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Suph_Result(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		SuphResultArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].SuphResultArray_L : m_RoadDatabaseArray[RoadIndex].SuphResultArray_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetSuphResult
------------------------------------------------------------
函数功能:设置道路超高计算结果
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左侧或右侧
		std::vector<CgSZ>&		SuphResultArray		超高计算结果数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetSuphResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ> SuphResultArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].SuphResultArray_L = SuphResultArray;
	else
		m_RoadDatabaseArray[RoadIndex].SuphResultArray_R = SuphResultArray;

	if (IsSaveToMdb)
		Write_Suph_Result(MdbName, RoadName);
}

/*===================================================================
函数名:GetWidenResult
------------------------------------------------------------
函数功能:获取道路加宽计算结果
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左侧或右侧
------------------------------------------------------------
输出:	std::vector<CgSZ>&		SuphResultArray		加宽计算结果数据
		bool		返回值		true-读取成功 false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetWidenResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ>& WidenResultArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		WidenResultArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].WidenResultArray_L : m_RoadDatabaseArray[RoadIndex].WidenResultArray_R;
		if (WidenResultArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Widen_Result(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		WidenResultArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].WidenResultArray_L : m_RoadDatabaseArray[RoadIndex].WidenResultArray_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetWidenResult
------------------------------------------------------------
函数功能:设置道路加宽计算结果
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左侧或右侧
		std::vector<CgSZ>&		WidenResultArray		加宽计算结果数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetWidenResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ> WidenResultArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].WidenResultArray_L = WidenResultArray;
	else
		m_RoadDatabaseArray[RoadIndex].WidenResultArray_R = WidenResultArray;

	if (IsSaveToMdb)
		Write_Widen_Result(MdbName, RoadName);
}

bool RoadDesignWorkingDatabase::GetHorizontalDarwPara(CString MdbName, CString RoadName, DrawHorizontalPara& HDrawPara)
{
	//尝试从数据库中读取数据
	if (Read_Horizontal_Draw_Para(MdbName, RoadName))
	{
		int RoadIndex;
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		HDrawPara = m_RoadDatabaseArray[RoadIndex].HDrawPara;
		return true;
	}
	else
		return false;
}

void RoadDesignWorkingDatabase::SetHorizontalDrawPara(CString MdbName, CString RoadName, DrawHorizontalPara HDrawPara, bool IsSaveToFile)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].HDrawPara = HDrawPara;

	if (IsSaveToFile)
		Write_Horizontal_Draw_Para(MdbName, RoadName);
}

/*===================================================================
函数名:GetEarthLine
------------------------------------------------------------
函数功能:获取纵断面地面线数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<DMXYS>&		EarthLine	地面线点数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetEarthLine(CString MdbName, CString RoadName, std::vector<DMXYS>& EarthLine)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		EarthLine = m_RoadDatabaseArray[RoadIndex].EarthLine;
		if (EarthLine.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_EarthLine(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		EarthLine = m_RoadDatabaseArray[RoadIndex].EarthLine;
		return true;
	}

	return false;
}

/*===================================================================
函数名:GetEarthLine
------------------------------------------------------------
函数功能:获取纵断面地面线数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<DMXYS>		EarthLine	地面线点数组
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetEarthLine(CString MdbName, CString RoadName, std::vector<DMXYS> EarthLine, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].EarthLine = EarthLine;

	if (IsSaveToMdb)
		Write_EarthLine(MdbName, RoadName);
}

/*===================================================================
函数名:GetSlopeData
------------------------------------------------------------
函数功能:获取纵断面坡度数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<PDTAB_ROAD>&		EarthLine	坡度数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetSlopeData(CString MdbName, CString RoadName, std::vector<PDTAB_ROAD>& SlopeArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		SlopeArray = m_RoadDatabaseArray[RoadIndex].SlopeArray;
		if (SlopeArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Slope_Data(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		SlopeArray = m_RoadDatabaseArray[RoadIndex].SlopeArray;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetSlopeData
------------------------------------------------------------
函数功能:设置纵断面坡度数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	std::vector<PDTAB_ROAD>		EarthLine	坡度数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetSlopeData(CString MdbName, CString RoadName, std::vector<PDTAB_ROAD> SlopeArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].SlopeArray = SlopeArray;

	if (IsSaveToMdb)
		Write_Slope_Data(MdbName, RoadName);
}

/*===================================================================
函数名:GetBriData
------------------------------------------------------------
函数功能:获取桥梁数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<PDTAB_ROAD>&		EarthLine	坡度数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetBriData(CString MdbName, CString RoadName, std::vector<BRITAB>& BriArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		BriArray = m_RoadDatabaseArray[RoadIndex].BriArray;
		if (BriArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Bridge(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		BriArray = m_RoadDatabaseArray[RoadIndex].BriArray;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetBriData
------------------------------------------------------------
函数功能:设置桥梁数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	std::vector<PDTAB_ROAD>		EarthLine	坡度数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetBriData(CString MdbName, CString RoadName, std::vector<BRITAB> BriArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].BriArray = BriArray;

	if (IsSaveToMdb)
		Write_Bridge(MdbName, RoadName);
}

/*===================================================================
函数名:GetTunData
------------------------------------------------------------
函数功能:获取隧道数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<PDTAB_ROAD>&		EarthLine	坡度数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetTunData(CString MdbName, CString RoadName, std::vector<TUNTAB>& TunArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		TunArray = m_RoadDatabaseArray[RoadIndex].TunArray;
		if (TunArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_Tunnel(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		TunArray = m_RoadDatabaseArray[RoadIndex].TunArray;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetTunData
------------------------------------------------------------
函数功能:设置隧道数据
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	std::vector<PDTAB_ROAD>		EarthLine	坡度数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetTunData(CString MdbName, CString RoadName, std::vector<TUNTAB> TunArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].TunArray = TunArray;

	if (IsSaveToMdb)
		Write_Tunnel(MdbName, RoadName);
}

//获取控制标高数据
bool RoadDesignWorkingDatabase::GetLevelCtrlData(CString MdbName, CString RoadName, std::vector<GCKZPt>& LevelCtrlPtArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	
	//if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	//{
	//	LevelCtrlPtArray = m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray;
	//	if (LevelCtrlPtArray.size())
	//		return true;
	//}*/

	//2.尝试从数据库中读取数据
	if (Read_LevelCtrlData(MdbName, RoadName))
	{
		int RoadIndex = -1;
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LevelCtrlPtArray = m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray;
		return true;
	}

	return false;
}

//设置设置标高数据
void RoadDesignWorkingDatabase::SetLevelCtrlData(CString MdbName, CString RoadName, std::vector<GCKZPt> LevelCtrlPtArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray = LevelCtrlPtArray;

	if (IsSaveToMdb)
		Write_LevelCtrlData(MdbName, RoadName);
}

//获取接坡信息数据
bool RoadDesignWorkingDatabase::GetJPData(CString MdbName, CString RoadName, std::vector<JPInfor>& JPArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		JPArray = m_RoadDatabaseArray[RoadIndex].JPArray;
		if (JPArray.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_JPData(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		JPArray = m_RoadDatabaseArray[RoadIndex].JPArray;
		return true;
	}

	return false;
} 

//设置接坡信息数据
void RoadDesignWorkingDatabase::SetJPData(CString MdbName, CString RoadName, std::vector<JPInfor> JPArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].JPArray = JPArray;

	if (IsSaveToMdb)
		Write_JPData(MdbName, RoadName);
}

/*===================================================================
函数名:GetSFYSXS
------------------------------------------------------------
函数功能:获取松方压实系数
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	double*		YSXS		松方压实系数
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：	这个比较特殊，无法检测是否存在，直接从数据库读取
=====================================================================*/
bool RoadDesignWorkingDatabase::GetSFYSXS(CString MdbName, CString RoadName, double* YSXS)
{
	//尝试从数据库中读取数据
	int RoadIndex = -1;
	if (Read_SFYSXS(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		for (int i = 0; i < 6; i++)
			YSXS[i] = m_RoadDatabaseArray[RoadIndex].YSXS[i];
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetSFYSXS
------------------------------------------------------------
函数功能:设置松方压实系数
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		double*		YSXS		松方压实系数
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：	20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetSFYSXS(CString MdbName, CString RoadName, double* YSXS, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	for (int i = 0; i < 6; i++)
		m_RoadDatabaseArray[RoadIndex].YSXS[i] = YSXS[i];

	if (IsSaveToMdb)
		Write_SFYSXS(MdbName, RoadName);
}

/*===================================================================
函数名:GetFillType
------------------------------------------------------------
函数功能:获取填方分类
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<LDTLdata>&	FillType		填方分类数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetFillType(CString MdbName, CString RoadName, std::vector<LDTLdata>& FillType)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		FillType = m_RoadDatabaseArray[RoadIndex].FillType;
		if (FillType.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_FillType(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		FillType = m_RoadDatabaseArray[RoadIndex].FillType;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetFillType
------------------------------------------------------------
函数功能:设置填方分类
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<LDTLdata>	FillType	填方分类数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetFillType(CString MdbName, CString RoadName, std::vector<LDTLdata> FillType, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].FillType = FillType;

	if (IsSaveToMdb)
		Write_FillType(MdbName, RoadName);
}

/*===================================================================
函数名:GetYongDiFDB
------------------------------------------------------------
函数功能:获取用地面积汇总分段表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<YongDiFDB>& EarthSection	用地面积汇总分段数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetYongDiFDB(CString MdbName, CString RoadName, std::vector<YongDiFDB>& EarthSection)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		EarthSection = m_RoadDatabaseArray[RoadIndex].EarthSection;
		if (EarthSection.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_YongDiFDB(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		EarthSection = m_RoadDatabaseArray[RoadIndex].EarthSection;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetYongDiFDB
------------------------------------------------------------
函数功能:设置用地面积汇总分段表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<YongDiFDB> EarthSection	用地面积汇总分段数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetYongDiFDB(CString MdbName, CString RoadName, std::vector<YongDiFDB> EarthSection, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].EarthSection = EarthSection;

	if (IsSaveToMdb)
		Write_YongDiFDB(MdbName, RoadName);
}

/*===================================================================
函数名:GetEarthType
------------------------------------------------------------
函数功能:获取用地类型表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<YongDiLXB>& EarthType		用地类型数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetEarthType(CString MdbName, CString RoadName, std::vector<YongDiLXB>& EarthType)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		EarthType = m_RoadDatabaseArray[RoadIndex].EarthType;
		if (EarthType.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_EarthType(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		EarthType = m_RoadDatabaseArray[RoadIndex].EarthType;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetEarthType
------------------------------------------------------------
函数功能:设置用地类型表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<YongDiLXB> EarthType		用地类型数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:	
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetEarthType(CString MdbName, CString RoadName, std::vector<YongDiLXB> EarthType, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].EarthType = EarthType;

	if (IsSaveToMdb)
		Write_EarthType(MdbName, RoadName);
}

/*===================================================================
函数名:GetCutType
------------------------------------------------------------
函数功能:获取挖方分类
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<WFFLdata>&	CutType			挖方分类数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetCutType(CString MdbName, CString RoadName, std::vector<WFFLdata>& CutType)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		CutType = m_RoadDatabaseArray[RoadIndex].CutType;
		if (CutType.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_CutType(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		CutType = m_RoadDatabaseArray[RoadIndex].CutType;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetCutType
------------------------------------------------------------
函数功能:设置挖方分类
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<YongDiLXB> EarthType		用地类型数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetCutType(CString MdbName, CString RoadName, std::vector<WFFLdata> CutType, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].CutType = CutType;

	if (IsSaveToMdb)
		Write_CutType(MdbName, RoadName);
}

/*===================================================================
函数名:GetGLDMLZ
------------------------------------------------------------
函数功能:获取关联断面路肩拉直
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<GLDMLZdata>& GLDMLZ			关联断面路肩拉直数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetGLDMLZ(CString MdbName, CString RoadName, std::vector<GLDMLZdata>& GLDMLZ)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		GLDMLZ = m_RoadDatabaseArray[RoadIndex].GLDMLZ;
		if (GLDMLZ.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_GLDMLZ(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		GLDMLZ = m_RoadDatabaseArray[RoadIndex].GLDMLZ;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetGLDMLZ
------------------------------------------------------------
函数功能:设置关联断面路肩拉直
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<GLDMLZdata> GLDMLZ	关联断面路肩拉直数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetGLDMLZ(CString MdbName, CString RoadName, std::vector<GLDMLZdata> GLDMLZ, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].GLDMLZ = GLDMLZ;

	if (IsSaveToMdb)
		Write_GLDMLZ(MdbName, RoadName);
}

/*===================================================================
函数名:GetGLDMLQBPTP
------------------------------------------------------------
函数功能:获取关联断面路堑边坡推平
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<GLDMLQBPTPdata>& GLDMLQBPTP		关联断面路堑边坡推平数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetGLDMLQBPTP(CString MdbName, CString RoadName, std::vector<GLDMLQBPTPdata>& GLDMLQBPTP)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		GLDMLQBPTP = m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP;
		if (GLDMLQBPTP.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_GLDMLQBPTP(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		GLDMLQBPTP = m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetGLDMLQBPTP
------------------------------------------------------------
函数功能:设置关联断面路堑边坡推平
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<GLDMLQBPTPdata>& GLDMLQBPTP		关联断面路堑边坡推平数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetGLDMLQBPTP(CString MdbName, CString RoadName, std::vector<GLDMLQBPTPdata> GLDMLQBPTP, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP = GLDMLQBPTP;

	if (IsSaveToMdb)
		Write_GLDMLQBPTP(MdbName, RoadName);
}

/*===================================================================
函数名:GetGLDMXJG
------------------------------------------------------------
函数功能:获取关联断面线间沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	std::vector<GLDMXJGdata>& GLDMXJG		关联断面线间沟数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetGLDMXJG(CString MdbName, CString RoadName, std::vector<GLDMXJGdata>& GLDMXJG)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		GLDMXJG = m_RoadDatabaseArray[RoadIndex].GLDMXJG;
		if (GLDMXJG.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_GLDMXJG(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		GLDMXJG = m_RoadDatabaseArray[RoadIndex].GLDMXJG;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetGLDMXJG
------------------------------------------------------------
函数功能:设置关联断面线间沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		std::vector<GLDMXJGdata> GLDMXJG		关联断面线间沟数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetGLDMXJG(CString MdbName, CString RoadName, std::vector<GLDMXJGdata> GLDMXJG, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].GLDMXJG = GLDMXJG;

	if (IsSaveToMdb)
		Write_GLDMXJG(MdbName, RoadName);
}

/*===================================================================
函数名:GetHTHD
------------------------------------------------------------
函数功能:获取清表土厚度
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<hthddata>&	HTHD		清表土厚度数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetHTHD(CString MdbName, CString RoadName, int LorR, std::vector<hthddata>& HTHD)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		HTHD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].HTHD_L : m_RoadDatabaseArray[RoadIndex].HTHD_R;
		if (HTHD.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_HTHD(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		HTHD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].HTHD_L : m_RoadDatabaseArray[RoadIndex].HTHD_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetHTHD
------------------------------------------------------------
函数功能:设置清表土厚度
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<hthddata>	HTHD		清表土厚度数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetHTHD(CString MdbName, CString RoadName, int LorR, std::vector<hthddata> HTHD, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if(LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].HTHD_L = HTHD;
	else
		m_RoadDatabaseArray[RoadIndex].HTHD_R = HTHD;

	if (IsSaveToMdb)
		Write_HTHD(MdbName, RoadName);
}

/*===================================================================
函数名:GetGouPD
------------------------------------------------------------
函数功能:获取沟底坡
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<GouPDdata>& GouPD		沟底坡数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetGouPD(CString MdbName, CString RoadName, int LorR, std::vector<GouPDdata>& GouPD)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		GouPD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].GouPD_L : m_RoadDatabaseArray[RoadIndex].GouPD_R;
		if (GouPD.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_GouPD(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		GouPD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].GouPD_L : m_RoadDatabaseArray[RoadIndex].GouPD_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetGouPD
------------------------------------------------------------
函数功能:设置沟底坡
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<GouPDdata>& GouPD		沟底坡数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetGouPD(CString MdbName, CString RoadName, int LorR, std::vector<GouPDdata> GouPD, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].GouPD_L = GouPD;
	else
		m_RoadDatabaseArray[RoadIndex].GouPD_R = GouPD;

	if (IsSaveToMdb)
		Write_GouPD(MdbName, RoadName);
}

/*===================================================================
函数名:GetCTKD
------------------------------------------------------------
函数功能:获取超填宽度表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<ctkddata>&	CTKD		超填宽度表数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetCTKD(CString MdbName, CString RoadName, int LorR, std::vector<ctkddata>& CTKD)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		CTKD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].CTKD_L : m_RoadDatabaseArray[RoadIndex].CTKD_R;
		if (CTKD.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_CTKD(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		CTKD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].CTKD_L : m_RoadDatabaseArray[RoadIndex].CTKD_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetCTKD
------------------------------------------------------------
函数功能:设置超填宽度表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<ctkddata>&	CTKD		超填宽度表数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetCTKD(CString MdbName, CString RoadName, int LorR, std::vector<ctkddata> CTKD, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].CTKD_L = CTKD;
	else
		m_RoadDatabaseArray[RoadIndex].CTKD_R = CTKD;

	if (IsSaveToMdb)
		Write_CTKD(MdbName, RoadName);
}

/*===================================================================
函数名:GetARCBP
------------------------------------------------------------
函数功能:获取弧形边坡表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<arcbpata>&	ARCBP		弧形边坡表数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetARCBP(CString MdbName, CString RoadName, int LorR, std::vector<arcbpata>& ARCBP)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		ARCBP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].ARCBP_L : m_RoadDatabaseArray[RoadIndex].ARCBP_R;
		if (ARCBP.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_ARCBP(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		ARCBP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].ARCBP_L : m_RoadDatabaseArray[RoadIndex].ARCBP_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetARCBP
------------------------------------------------------------
函数功能:设置弧形边坡表
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<arcbpata>&	ARCBP		弧形边坡表数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetARCBP(CString MdbName, CString RoadName, int LorR, std::vector<arcbpata> ARCBP, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].ARCBP_L = ARCBP;
	else
		m_RoadDatabaseArray[RoadIndex].ARCBP_R = ARCBP;

	if (IsSaveToMdb)
		Write_ARCBP(MdbName, RoadName);
}

/*===================================================================
函数名:GetDQ
------------------------------------------------------------
函数功能:获取挡墙
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<dqdata>&	DQ		挡墙数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetDQ(CString MdbName, CString RoadName, int LorR, std::vector<dqdata>& DQ)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		DQ = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].DQ_L : m_RoadDatabaseArray[RoadIndex].DQ_R;
		if (DQ.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_DQ(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		DQ = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].DQ_L : m_RoadDatabaseArray[RoadIndex].DQ_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetDQ
------------------------------------------------------------
函数功能:设置挡墙
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<dqdata>&	DQ		挡墙数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetDQ(CString MdbName, CString RoadName, int LorR, std::vector<dqdata> DQ, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].DQ_L = DQ;
	else
		m_RoadDatabaseArray[RoadIndex].DQ_R = DQ;

	if (IsSaveToMdb)
		Write_DQ(MdbName, RoadName);
}

/*===================================================================
函数名:GetLDTP
------------------------------------------------------------
函数功能:获取低填路堤自动填平
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LDTPData>&	LDTP		低填路堤自动填平数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLDTP(CString MdbName, CString RoadName, int LorR, std::vector<LDTPData>& LDTP)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LDTP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LDTP_L : m_RoadDatabaseArray[RoadIndex].LDTP_R;
		if (LDTP.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LDTP(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LDTP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LDTP_L : m_RoadDatabaseArray[RoadIndex].LDTP_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLDTP
------------------------------------------------------------
函数功能:设置低填路堤自动填平
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LDTPData>&	LDTP		低填路堤自动填平数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLDTP(CString MdbName, CString RoadName, int LorR, std::vector<LDTPData> LDTP, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LDTP_L = LDTP;
	else
		m_RoadDatabaseArray[RoadIndex].LDTP_R = LDTP;

	if (IsSaveToMdb)
		Write_LDTP(MdbName, RoadName);
}

/*===================================================================
函数名:GetLQTP
------------------------------------------------------------
函数功能:获取低挖路堑自动推平
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LQTPData>& LQTP		低挖路堑自动推平数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLQTP(CString MdbName, CString RoadName, int LorR, std::vector<LQTPData>& LQTP)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LQTP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LQTP_L : m_RoadDatabaseArray[RoadIndex].LQTP_R;
		if (LQTP.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LQTP(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LQTP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LQTP_L : m_RoadDatabaseArray[RoadIndex].LQTP_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLQTP
------------------------------------------------------------
函数功能:设置低挖路堑自动推平
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LQTPData>& LQTP		低挖路堑自动推平数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLQTP(CString MdbName, CString RoadName, int LorR, std::vector<LQTPData> LQTP, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LQTP_L = LQTP;
	else
		m_RoadDatabaseArray[RoadIndex].LQTP_R = LQTP;

	if (IsSaveToMdb)
		Write_LQTP(MdbName, RoadName);
}

/*===================================================================
函数名:GetTLJJK
------------------------------------------------------------
函数功能:获取低填路堤自动加宽土路肩
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<TLJJKData>& TLJJK		低填路堤自动加宽土路肩数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetTLJJK(CString MdbName, CString RoadName, int LorR, std::vector<TLJJKData>& TLJJK)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		TLJJK = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].TLJJK_L : m_RoadDatabaseArray[RoadIndex].TLJJK_R;
		if (TLJJK.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_TLJJK(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		TLJJK = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].TLJJK_L : m_RoadDatabaseArray[RoadIndex].TLJJK_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetTLJJK
------------------------------------------------------------
函数功能:设置低填路堤自动加宽土路肩
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<TLJJKData> TLJJK		低填路堤自动加宽土路肩数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetTLJJK(CString MdbName, CString RoadName, int LorR, std::vector<TLJJKData> TLJJK, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].TLJJK_L = TLJJK;
	else
		m_RoadDatabaseArray[RoadIndex].TLJJK_R = TLJJK;

	if (IsSaveToMdb)
		Write_TLJJK(MdbName, RoadName);
}

/*===================================================================
函数名:GetLMHD
------------------------------------------------------------
函数功能:获取路面厚度
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LMHdata>& LMHD		路面厚度数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLMHD(CString MdbName, CString RoadName, int LorR, std::vector<LMHdata>& LMHD)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LMHD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LMHD_L : m_RoadDatabaseArray[RoadIndex].LMHD_R;
		if (LMHD.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LMHD(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LMHD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LMHD_L : m_RoadDatabaseArray[RoadIndex].LMHD_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLMHD
------------------------------------------------------------
函数功能:设置路面厚度
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LMHdata> LMHD		路面厚度数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLMHD(CString MdbName, CString RoadName, int LorR, std::vector<LMHdata> LMHD, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LMHD_L = LMHD;
	else
		m_RoadDatabaseArray[RoadIndex].LMHD_R = LMHD;

	if (IsSaveToMdb)
		Write_LMHD(MdbName, RoadName);
}

/*===================================================================
函数名:GetCRSWID
------------------------------------------------------------
函数功能:获取地面线内插宽
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<crswiddata>& CRSWID		地面线内插宽数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetCRSWID(CString MdbName, CString RoadName, int LorR, std::vector<crswiddata>& CRSWID)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		CRSWID = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].CRSWID_L : m_RoadDatabaseArray[RoadIndex].CRSWID_R;
		if (CRSWID.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_CRSWID(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		CRSWID = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].CRSWID_L : m_RoadDatabaseArray[RoadIndex].CRSWID_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetCRSWID
------------------------------------------------------------
函数功能:设置地面线内插宽
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<crswiddata> CRSWID		地面线内插宽数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetCRSWID(CString MdbName, CString RoadName, int LorR, std::vector<crswiddata> CRSWID, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].CRSWID_L = CRSWID;
	else
		m_RoadDatabaseArray[RoadIndex].CRSWID_R = CRSWID;

	if (IsSaveToMdb)
		Write_CRSWID(MdbName, RoadName);
}

/*===================================================================
函数名:GetDM
------------------------------------------------------------
函数功能:获取横断面限制宽度
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<dmdata>& DM		横断面限制宽度数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetDM(CString MdbName, CString RoadName, int LorR, std::vector<dmdata>& DM)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		DM = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].DM_L : m_RoadDatabaseArray[RoadIndex].DM_R;
		if (DM.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_DM(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		DM = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].DM_L : m_RoadDatabaseArray[RoadIndex].DM_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetDM
------------------------------------------------------------
函数功能:设置横断面限制宽度
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<dmdata>& DM		横断面限制宽度数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetDM(CString MdbName, CString RoadName, int LorR, std::vector<dmdata> DM, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].DM_L = DM;
	else
		m_RoadDatabaseArray[RoadIndex].DM_R = DM;

	if (IsSaveToMdb)
		Write_DM(MdbName, RoadName);
}

/*===================================================================
函数名:GetPWD
------------------------------------------------------------
函数功能:获取用地加宽
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<pwddata>& PWD		用地加宽数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetPWD(CString MdbName, CString RoadName, int LorR, std::vector<pwddata>& PWD)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		PWD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].PWD_L : m_RoadDatabaseArray[RoadIndex].PWD_R;
		if (PWD.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_PWD(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		PWD = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].PWD_L : m_RoadDatabaseArray[RoadIndex].PWD_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetPWD
------------------------------------------------------------
函数功能:设置用地加宽
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<pwddata> PWD		用地加宽数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetPWD(CString MdbName, CString RoadName, int LorR, std::vector<pwddata> PWD, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].PWD_L = PWD;
	else
		m_RoadDatabaseArray[RoadIndex].PWD_R = PWD;

	if (IsSaveToMdb)
		Write_PWD(MdbName, RoadName);
}

/*===================================================================
函数名:GetLDBP
------------------------------------------------------------
函数功能:获取路堤边坡
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LDBPdata>&	LDBP		路堤边坡数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLDBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata>& LDBP)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LDBP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LDBP_L : m_RoadDatabaseArray[RoadIndex].LDBP_R;
		if (LDBP.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LDBP(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LDBP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LDBP_L : m_RoadDatabaseArray[RoadIndex].LDBP_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLDBP
------------------------------------------------------------
函数功能:设置路堤边坡
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LDBPdata>	LDBP		路堤边坡数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLDBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata> LDBP, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LDBP_L = LDBP;
	else
		m_RoadDatabaseArray[RoadIndex].LDBP_R = LDBP;

	if (IsSaveToMdb)
		Write_LDBP(MdbName, RoadName);
}

/*===================================================================
函数名:GetLQBP
------------------------------------------------------------
函数功能:获取路堑边坡
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LDBPdata>&	LQBP		路堑边坡数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLQBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata>& LQBP)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LQBP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LQBP_L : m_RoadDatabaseArray[RoadIndex].LQBP_R;
		if (LQBP.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LQBP(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LQBP = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LQBP_L : m_RoadDatabaseArray[RoadIndex].LQBP_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLQBP
------------------------------------------------------------
函数功能:设置路堑边坡
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LDBPdata>	LQBP		路堑边坡数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLQBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata> LQBP, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LQBP_L = LQBP;
	else
		m_RoadDatabaseArray[RoadIndex].LQBP_R = LQBP;

	if (IsSaveToMdb)
		Write_LQBP(MdbName, RoadName);
}

/*===================================================================
函数名:GetLDSG
------------------------------------------------------------
函数功能:获取排水沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LDSGdata>& LDSG		排水沟数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLDSG(CString MdbName, CString RoadName, int LorR, std::vector<LDSGdata>& LDSG)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LDSG = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LDSG_L : m_RoadDatabaseArray[RoadIndex].LDSG_R;
		if (LDSG.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LDSG(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LDSG = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LDSG_L : m_RoadDatabaseArray[RoadIndex].LDSG_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLDSG
------------------------------------------------------------
函数功能:设置排水沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LDSGdata>	LDSG		排水沟数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLDSG(CString MdbName, CString RoadName, int LorR, std::vector<LDSGdata> LDSG, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LDSG_L = LDSG;
	else
		m_RoadDatabaseArray[RoadIndex].LDSG_R = LDSG;

	if (IsSaveToMdb)
		Write_LDSG(MdbName, RoadName);
}

/*===================================================================
函数名:GetLQSG
------------------------------------------------------------
函数功能:获取侧沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LQSGdata>& LQSG		侧沟数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetLQSG(CString MdbName, CString RoadName, int LorR, std::vector<LQSGdata>& LQSG)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		LQSG = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LQSG_L : m_RoadDatabaseArray[RoadIndex].LQSG_R;
		if (LQSG.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_LQSG(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		LQSG = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].LQSG_L : m_RoadDatabaseArray[RoadIndex].LQSG_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetLQSG
------------------------------------------------------------
函数功能:设置侧沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LQSGdata>	LQSG		侧沟数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetLQSG(CString MdbName, CString RoadName, int LorR, std::vector<LQSGdata> LQSG, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].LQSG_L = LQSG;
	else
		m_RoadDatabaseArray[RoadIndex].LQSG_R = LQSG;

	if (IsSaveToMdb)
		Write_LQSG(MdbName, RoadName);
}

/*===================================================================
函数名:GetTG
------------------------------------------------------------
函数功能:获取天沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<TGdata>		TG		天沟数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetTG(CString MdbName, CString RoadName, int LorR, std::vector<TGdata>& TG)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		TG = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].TG_L : m_RoadDatabaseArray[RoadIndex].TG_R;
		if (TG.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_TG(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		TG = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].TG_L : m_RoadDatabaseArray[RoadIndex].TG_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetTG
------------------------------------------------------------
函数功能:设置天沟
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<TGdata>		TG		天沟数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetTG(CString MdbName, CString RoadName, int LorR, std::vector<TGdata> TG, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].TG_L = TG;
	else
		m_RoadDatabaseArray[RoadIndex].TG_R = TG;

	if (IsSaveToMdb)
		Write_TG(MdbName, RoadName);
}


/*===================================================================
函数名:GetRoadLayer
------------------------------------------------------------
函数功能:获取路面分层
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<RoadLayerData>		RoadLayerArray		路面分层数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20191017
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetRoadLayer(CString MdbName, CString RoadName, int LorR, std::vector<RoadLayerData>& RoadLayerArray)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	//int RoadIndex = -1;
	//if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	//{
	//	RoadLayerArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].RoadLayer_L : m_RoadDatabaseArray[RoadIndex].RoadLayer_R;
	//	if (RoadLayerArray.size())
	//		return true;
	//}

	int RoadIndex = -1;
	//2.尝试从数据库中读取数据
	if (Read_RoadLayer(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		RoadLayerArray = (LorR == LEFT) ? m_RoadDatabaseArray[RoadIndex].RoadLayer_L : m_RoadDatabaseArray[RoadIndex].RoadLayer_R;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetTG
------------------------------------------------------------
函数功能:设置路面分层
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<RoadLayerData>		RoadLayerArray		路面分层数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20191017
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::SetRoadLayer(CString MdbName, CString RoadName, int LorR, std::vector<RoadLayerData> RoadLayerArray, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	if (LorR == LEFT)
		m_RoadDatabaseArray[RoadIndex].RoadLayer_L = RoadLayerArray;
	else
		m_RoadDatabaseArray[RoadIndex].RoadLayer_R = RoadLayerArray;

	if (IsSaveToMdb)
		Write_RoadLayer(MdbName, RoadName);
}

/*===================================================================
函数名:GetSationSequence
------------------------------------------------------------
函数功能:获取桩号序列
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
------------------------------------------------------------
输出:	std::vector<LCBdata>&	StationSequence	桩号序列数据
		bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::GetSationSequence(CString MdbName, CString RoadName, std::vector<LCBdata>& StationSequence)
{
	//1.检测m_RoadDatabaseArray中是否有该方案
	int RoadIndex = -1;
	if (Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		StationSequence = m_RoadDatabaseArray[RoadIndex].StationSequence;
		if (StationSequence.size())
			return true;
	}

	//2.尝试从数据库中读取数据
	if (Read_StationSequence(MdbName, RoadName))
	{
		Is_Database_Exist(MdbName, RoadName, RoadIndex);
		StationSequence = m_RoadDatabaseArray[RoadIndex].StationSequence;
		return true;
	}

	return false;
}

/*===================================================================
函数名:SetSationSequence
------------------------------------------------------------
函数功能:设置桩号序列
------------------------------------------------------------
输入:	CString		MdbName		方案名
		CString		RoadName	道路名
		int			LorR		左右侧	-1左侧	1右侧
		std::vector<LCBdata>	StationSequence	桩号序列数据
		bool		IsSaveToMdb	是否存储至数据库
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：		20190816	增加单独存储数据库的选项
=====================================================================*/
void RoadDesignWorkingDatabase::SetSationSequence(CString MdbName, CString RoadName, std::vector<LCBdata> StationSequence, bool IsSaveToMdb)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	m_RoadDatabaseArray[RoadIndex].StationSequence = StationSequence;

	if (IsSaveToMdb)
		Write_StationSequence(MdbName, RoadName);
}

/*===================================================================
函数名:Is_Database_Exist
------------------------------------------------------------
函数功能:道路数据库是否存在
------------------------------------------------------------
输入:	CString		MdbName		方案路径
		CString		RoadName	道路名
------------------------------------------------------------
输出:	int&		Road_Index	方案数据库索引号
		bool		返回值		true-存在	false-不存在
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Is_Database_Exist(CString MdbName, CString RoadName, int& Road_Index)
{
	Road_Index = -1;

	for (int i = 0; i < m_RoadDatabaseArray.size(); i++)
	{
		if (m_RoadDatabaseArray[i].MdbName == MdbName &&
			m_RoadDatabaseArray[i].RoadName == RoadName)
		{
			Road_Index = i;
			return true;
		}
	}

	return false;
}


/*===================================================================
函数名:Is_Database_Exist_And_Create
------------------------------------------------------------
函数功能:活动数据库是否存在，如果不存在则创建
------------------------------------------------------------
输入:	CString		MdbName		方案路径
		CString		RoadName	道路名
------------------------------------------------------------
输出:	int			返回值		数据库索引值
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
int RoadDesignWorkingDatabase::Is_Database_Exist_And_Create(CString MdbName, CString RoadName)
{
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
	{
		CreateDatabase(MdbName, RoadName);
		RoadIndex = m_RoadDatabaseArray.size() - 1;
	}

	return RoadIndex;
}

/*===================================================================
函数名:CreateDatabase
------------------------------------------------------------
函数功能:创建活动数据库
------------------------------------------------------------
输入:	CString MdbName		方案数据库
		CString RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：	数据库是否存在不在此处检测，调用该函数前请注意检测
		是否存在该数据库，避免重复创建
=====================================================================*/
void RoadDesignWorkingDatabase::CreateDatabase(CString MdbName, CString RoadName)
{
	//追加一个活动数据库
	RoadDataBase tempDatabase;
	tempDatabase.MdbName = MdbName;
	tempDatabase.RoadName = RoadName;
	m_RoadDatabaseArray.push_back(tempDatabase);
}

//析构函数
RoadDesignWorkingDatabase::~RoadDesignWorkingDatabase(void)
{
}

/*===================================================================
函数名:SaveAllData
------------------------------------------------------------
函数功能:保存数据到数据库
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：暂时未开放单个数据存储功能
=====================================================================*/
void RoadDesignWorkingDatabase::SaveAllData(CString MdbName, CString RoadName)
{
	//存储平面数据
	Write_Road_Width_Data(MdbName, RoadName);
	Write_Road_Suph_Data(MdbName, RoadName);
	Write_Road_Widen_Data(MdbName, RoadName);
	Write_Road_SuphCal_Data(MdbName, RoadName);
	Write_Suph_Result(MdbName, RoadName);
	Write_Widen_Result(MdbName, RoadName);

	//存储纵面数据
	Write_EarthLine(MdbName, RoadName);
	Write_Slope_Data(MdbName, RoadName);
	Write_Bridge(MdbName, RoadName);
	Write_Tunnel(MdbName, RoadName);

	//存横断面数据
	//写入松方压实系数
	Write_SFYSXS(MdbName, RoadName);

	//写入填方分类系数
	Write_FillType(MdbName, RoadName);

	//写入用地面积汇总分段表
	Write_YongDiFDB(MdbName, RoadName);

	//写入用地类型表
	Write_EarthType(MdbName, RoadName);

	//写入挖方分类
	Write_CutType(MdbName, RoadName);

	//写入关联断面路肩拉直
	Write_GLDMLZ(MdbName, RoadName);

	//写入关联断面路堑边坡推平
	Write_GLDMLQBPTP(MdbName, RoadName);

	//写入关联断面线间沟
	Write_GLDMXJG(MdbName, RoadName);

	//写入清表土厚度
	Write_HTHD(MdbName, RoadName);

	//写入沟底坡
	Write_GouPD(MdbName, RoadName);

	//写入超填宽度表
	Write_CTKD(MdbName, RoadName);

	//写入弧形边坡表
	Write_ARCBP(MdbName, RoadName);

	//写入挡墙
	Write_DQ(MdbName, RoadName);

	//写入低填路堤自动填平
	Write_LDTP(MdbName, RoadName);

	//写入低挖路堑自动推平
	Write_LQTP(MdbName, RoadName);

	//写入低填路堤自动加宽土路肩
	Write_TLJJK(MdbName, RoadName);

	//写入路面厚度
	Write_LMHD(MdbName, RoadName);

	//写入地面线内插宽
	Write_CRSWID(MdbName, RoadName);

	//写入横断面限制宽度
	Write_DM(MdbName, RoadName);

	//写入用地加宽
	Write_PWD(MdbName, RoadName);

	//写入路堤边坡
	Write_LDBP(MdbName, RoadName);

	//写入路堑边坡
	Write_LQBP(MdbName, RoadName);

	//写入排水沟
	Write_LDSG(MdbName, RoadName);

	//写入侧沟
	Write_LQSG(MdbName, RoadName);

	//写入天沟
	Write_TG(MdbName, RoadName);

	//写入序列
	Write_StationSequence(MdbName, RoadName);
}

/*===================================================================
函数名:DeleteWorkingDatabase
------------------------------------------------------------
函数功能:删除活动数据库
------------------------------------------------------------
输入:	CString		MdbName		方案路径
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-删除成功	false-删除失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::DeleteWorkingDatabase(CString MdbName, CString RoadName)
{
	int RoadIndex = -1;
	if(!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return false;

	m_RoadDatabaseArray.erase(m_RoadDatabaseArray.begin() + RoadIndex);
	return true;
}

/*===================================================================
函数名:Read_Road_Grade_And_Velocity
------------------------------------------------------------
函数功能:从数据库读取道路的等级和设计速度标准
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
bool RoadDesignWorkingDatabase::Read_Road_Grade_And_Velocity(CString MdbName, CString RoadName)
{
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//读取线路属性
	if (!m_DataBase.Read_XLDbs(MdbName, L"线路属性"))
	{
		AfxMessageBox(L"读取道路技术标准失败！");
		return false;
	}
	m_RoadDatabaseArray[RoadIndex].DesignVelocity = m_DataBase.RoadXZ.DV;
	m_RoadDatabaseArray[RoadIndex].Grade = m_DataBase.RoadXZ.Grade;

	return false;
}

/*===================================================================
函数名:Is_Horizontal_Road_Exist
------------------------------------------------------------
函数功能:指定道路平面指针是否存在
------------------------------------------------------------
输入:	CString		MdbName		方案路径
		CString		RoadName	道路名
------------------------------------------------------------
输出:	int&		Road_Index	方案道路平面指针索引号
		bool		返回值		true-存在	false-不存在
------------------------------------------------------------
算法描述:遍历查找
------------------------------------------------------------
编写日期:20190805	ljx
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Is_Horizontal_Road_Exist(CString MdbName, CString RoadName, int& Road_Index)
{
	Road_Index = -1;

	for (int i = 0; i < m_RoadArray.size(); i++)
	{
		if (_tcscmp(m_RoadArray[i]->mdbname, MdbName) == 0
			&& _tcscmp(m_RoadArray[i]->RoadName, RoadName) == 0)
		{
			Road_Index = i;
			return true;
		}
	}

	return false;
}

/*===================================================================
函数名:Create_Road_Pointer
------------------------------------------------------------
函数功能:创建道路平面指针
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	JD_CENTER*	返回值		道路平面指针
------------------------------------------------------------
算法描述:	1.从数据库中读取指定方案和道路名的控制点表和断链表
			2.数据存在时，创建道路平面指针
			3.读取道路属性表，赋予道路属性
------------------------------------------------------------
编写日期:20180805	ljx
------------------------------------------------------------
备注：	1.如果没有读取到平面交点信息，会返回空指针
		2.20191023修改	增加线元数据的读入
=====================================================================*/
JD_CENTER* RoadDesignWorkingDatabase::Create_Road_Pointer(CString MdbName, CString RoadName)
{
	JD_CENTER* pRoad = NULL;

	m_DataBase.Read_XLDbs(MdbName, L"控制点表", RoadName);
	m_DataBase.Read_XLDbs(MdbName, L"断链表", RoadName);
	m_DataBase.Read_XLDbs(MdbName, L"线元表", RoadName);

	if (m_DataBase.NJdCen > 0)
	{
		//创建道路平面指针
		pRoad = new JD_CENTER;
		pRoad->setMdbName(MdbName);
		pRoad->setRoadName(RoadName);
		pRoad->SetJdCenterArray(m_DataBase.JdCenArray, m_DataBase.NJdCen);

		//设置断链
		if (m_DataBase.NDL > 0)
		{
			pRoad->setDLB(m_DataBase.DLArray, m_DataBase.NDL);
		}

		//读取线路属性
		m_DataBase.Read_XLDbs(MdbName, L"线路属性");
		pRoad->DV = m_DataBase.RoadXZ.DV;
		pRoad->Grade = m_DataBase.RoadXZ.Grade;
	}
	else
	{
		pRoad = new JD_CENTER;
		pRoad->XYNum = m_DataBase.XYNum;
		if (pRoad->XYNum < 2)
			return NULL;
		if (pRoad->XYArray)
			delete[]pRoad->XYArray;
		pRoad->XYArray = new double[pRoad->XYNum + 1][10];
		for (int i = 0; i <= pRoad->XYNum; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				pRoad->XYArray[i][j] = m_DataBase.XYArray[i][j];
			}

		}
		pRoad->setRoadName(RoadName);
		pRoad->setMdbName(MdbName);

		if (m_DataBase.NDL > 0)
		{
			pRoad->setDLB(m_DataBase.DLArray, m_DataBase.NDL);
		}

		pRoad->DLNum = m_DataBase.NDL;
		pRoad->IfCalXY = false;
		pRoad->SHOWJD = false;
		int DV;
		CString grade;
		pRoad->ReadRoadDV(DV, grade);
		pRoad->DV = DV;
		pRoad->Grade = grade;
	}

	return pRoad;
}

/*===================================================================
函数名:Read_Road_Width_Data
------------------------------------------------------------
函数功能:读取道路宽数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		是否成功读取路面宽数据
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：	1.必须左右侧都有数据，才算读取成功
		2.读取失败该活动数据库的路面宽数组数据会清空
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Road_Width_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);
		
	//从数据库中读取左路面宽数据
	if (m_DataBase.Read_XLDbs(MdbName, "左路面宽分段表", RoadName))
	{
		if (m_DataBase.NLMK > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array.clear();
			for (int i = 0; i < m_DataBase.NLMK; i++)
				m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array.push_back(m_DataBase.lmk[i]);
		}
		else
			return false;
	}
	else
		return false;

	//从数据库中读取右路面宽数据
	if (m_DataBase.Read_XLDbs(MdbName, "右路面宽分段表", RoadName))
	{
		if (m_DataBase.NLMK > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array.clear();
			for (int i = 0; i < m_DataBase.NLMK; i++)
				m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array.push_back(m_DataBase.lmk[i]);
		}
		else
		{
			return false;
		}
	}
	else
		return false;


	return true;
}

/*===================================================================
函数名:Write_Road_Width_Data
------------------------------------------------------------
函数功能:写入道路宽数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Road_Width_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在	只要有一侧数据为空，不予保存
	if (!m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array.size()
		|| !m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array.size())
		return;

	//保存数据	左侧
	m_DataBase.NLMK = m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array.size();
	if (m_DataBase.lmk)
		delete[]m_DataBase.lmk;
	m_DataBase.lmk = new LMKdata[m_DataBase.NLMK];
	for (int i = 0; i < m_DataBase.NLMK; i++)
		m_DataBase.lmk[i] = m_RoadDatabaseArray[RoadIndex].Left_Road_Width_Array[i];
	m_DataBase.Write_XLDbs(MdbName, L"左路面宽分段表", RoadName);

	//保存数据	右侧
	m_DataBase.NLMK = m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array.size();
	if (m_DataBase.lmk)
		delete[]m_DataBase.lmk;
	m_DataBase.lmk = new LMKdata[m_DataBase.NLMK];
	for (int i = 0; i < m_DataBase.NLMK; i++)
		m_DataBase.lmk[i] = m_RoadDatabaseArray[RoadIndex].Right_Road_Width_Array[i];
	m_DataBase.Write_XLDbs(MdbName, L"右路面宽分段表", RoadName);
}

/*===================================================================
函数名:Read_Road_Suph_Data
------------------------------------------------------------
函数功能:从数据库读取超高标准数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		是否成功读取数据
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Road_Suph_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取超高标准数据
	if (m_DataBase.Read_XLDbs(MdbName, L"超高计算标准"))
	{
		if (m_DataBase.NuphData > 0)//有数据
		{
			//将数据库中的数值赋值给成员数组
			ACHAR str[20];
			ads_rtos(m_DataBase.SuphData[m_DataBase.NuphData - 1][0], 2, 3, str);//数据库中最后一行记录路拱横坡和最大超高
			m_RoadDatabaseArray[RoadIndex].IG = str;
			ads_rtos(m_DataBase.SuphData[m_DataBase.NuphData - 1][1], 2, 3, str);//数据库中最后一行记录路拱横坡和最大超高
			m_RoadDatabaseArray[RoadIndex].MAXSuphI = str;

			//超高数据
			m_RoadDatabaseArray[RoadIndex].SurpArray.clear();
			std::vector<double> tempVec;
			tempVec.resize(2);
			int NSUPH = m_DataBase.NuphData - 1;
			for (int i = 0; i < NSUPH; i++)
			{
				tempVec[0] = m_DataBase.SuphData[i][0];
				tempVec[1] = m_DataBase.SuphData[i][1];
				m_RoadDatabaseArray[RoadIndex].SurpArray.push_back(tempVec);
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

/*===================================================================
函数名:Write_Road_Suph_Data
------------------------------------------------------------
函数功能:写入超高标准数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	请在调用该函数前确保IG和MAXSuphI数据的正确性
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Road_Suph_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].SurpArray.size())
		return;

	//写入数据库
	m_DataBase.NuphData = m_RoadDatabaseArray[RoadIndex].SurpArray.size() + 1;
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].SurpArray.size(); i++)
	{
		m_DataBase.SuphData[i][0] = m_RoadDatabaseArray[RoadIndex].SurpArray[i][0];
		m_DataBase.SuphData[i][1] = m_RoadDatabaseArray[RoadIndex].SurpArray[i][1];
	}
	m_DataBase.SuphData[m_DataBase.NuphData - 1][0] = _wtof(m_RoadDatabaseArray[RoadIndex].IG);
	m_DataBase.SuphData[m_DataBase.NuphData - 1][1] = _wtof(m_RoadDatabaseArray[RoadIndex].MAXSuphI);
	m_DataBase.Write_XLDbs(MdbName, L"超高计算标准", L"");
}

/*===================================================================
函数名:Read_Road_Widen_Data
------------------------------------------------------------
函数功能:从数据库读取加宽标准数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		是否成功读取数据
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Road_Widen_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取超高标准数据
	if (m_DataBase.Read_XLDbs(MdbName, L"加宽计算标准"))
	{
		if (m_DataBase.NJK > 1)//数据库中有超高数据
		{
			m_RoadDatabaseArray[RoadIndex].WidenArray.clear();
			//获取设计标准
			CString Grade = L"";
			int DesignVelocity = 0;
			GetGradeAndVelocity(MdbName, RoadName, Grade, DesignVelocity);

			//从数据库中读取
			int NJK = m_DataBase.NJK - 1;

			//获取加宽类别
			if (Grade != "匝道")
			{

				if (m_DataBase.JKData[m_DataBase.NJK - 1][0] < 1.1)//数据库中最后一行记录加宽类别
					m_RoadDatabaseArray[RoadIndex].WidenType = "一类加宽";
				else if (m_DataBase.JKData[m_DataBase.NJK - 1][0] < 2.1)
					m_RoadDatabaseArray[RoadIndex].WidenType = "二类加宽";
				else
					m_RoadDatabaseArray[RoadIndex].WidenType = "三类加宽";
			}
			else
			{
				if (m_DataBase.JKData[m_DataBase.NJK - 1][0] < 1.1)//数据库中最后一行记录加宽类别
					m_RoadDatabaseArray[RoadIndex].WidenType = "按单车道加宽";
				else if (m_DataBase.JKData[m_DataBase.NJK - 1][0] < 2.1)
					m_RoadDatabaseArray[RoadIndex].WidenType = "按双车道加宽";
			}

			//转换赋值
			std::vector<double> tempVec;
			tempVec.resize(2);
			for (int i = 0; i < NJK; i++)
			{
				tempVec[0] = m_DataBase.JKData[i][0];
				tempVec[1] = m_DataBase.JKData[i][1];
				m_RoadDatabaseArray[RoadIndex].WidenArray.push_back(tempVec);
			}

			return true;
		}
	}
	else
		return false;

	return false;
}

/*===================================================================
函数名:Write_Road_Widen_Data
------------------------------------------------------------
函数功能:写入加宽标准数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	请在调用该函数前确保IG和MAXSuphI数据的正确性
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Road_Widen_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].WidenArray.size())
		return;

	//获取道路等级信息
	CString Grade = L"";
	int DesignVelocity = 0;
	GetGradeAndVelocity(MdbName, RoadName, Grade, DesignVelocity);

	//写入数据库
	m_DataBase.NJK = m_RoadDatabaseArray[RoadIndex].WidenArray.size() + 1;
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].WidenArray.size(); i++)
	{
		m_DataBase.JKData[i][0] = m_RoadDatabaseArray[RoadIndex].WidenArray[i][0];
		m_DataBase.JKData[i][1] = m_RoadDatabaseArray[RoadIndex].WidenArray[i][1];
	}

	if (Grade != "匝道")
	{
		if (m_RoadDatabaseArray[RoadIndex].WidenType == "一类加宽")
			m_DataBase.JKData[m_DataBase.NJK - 1][0] = 1;
		else if (m_RoadDatabaseArray[RoadIndex].WidenType == "二类加宽")
			m_DataBase.JKData[m_DataBase.NJK - 1][0] = 2;
		else
			m_DataBase.JKData[m_DataBase.NJK - 1][0] = 3;
	}
	else
	{
		if (m_RoadDatabaseArray[RoadIndex].WidenType == "按单车道加宽")
			m_DataBase.JKData[m_DataBase.NJK - 1][0] = 1;
		else if (m_RoadDatabaseArray[RoadIndex].WidenType == "按双车道加宽")
			m_DataBase.JKData[m_DataBase.NJK - 1][0] = 2;
	}

	m_DataBase.JKData[m_DataBase.NJK - 1][1] = 0;
	m_DataBase.Write_XLDbs(MdbName, L"加宽计算标准", L"");
}


/*===================================================================
函数名:Read_Road_SuphCal_Data
------------------------------------------------------------
函数功能:从数据库读取超高计算参数数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		是否成功读取数据
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Road_SuphCal_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取超高标准数据
	if(!m_DataBase.Read_XLDbs(MdbName, L"超高计算参数"))
		return false;

	//传递数据
	m_RoadDatabaseArray[RoadIndex].HxCgJbl = m_DataBase.CalCGCs.m_HxCgJbl;
	m_RoadDatabaseArray[RoadIndex].ZxCgJbl = m_DataBase.CalCGCs.m_ZxCgJbl;
	m_RoadDatabaseArray[RoadIndex].WidenMode = m_DataBase.CalCGCs.JkMode;
	m_RoadDatabaseArray[RoadIndex].SuphMode = m_DataBase.CalCGCs.CgMode;
	m_RoadDatabaseArray[RoadIndex].SuphMode = m_DataBase.CalCGCs.CgLenMode;

	return true;
}

/*===================================================================
函数名:Write_Road_Widen_Data
------------------------------------------------------------
函数功能:写入加宽标准数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Road_SuphCal_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;
	
	m_DataBase.CalCGCs.m_HxCgJbl = m_RoadDatabaseArray[RoadIndex].HxCgJbl;
	m_DataBase.CalCGCs.m_ZxCgJbl = m_RoadDatabaseArray[RoadIndex].ZxCgJbl;
	m_DataBase.CalCGCs.JkMode = m_RoadDatabaseArray[RoadIndex].WidenMode;
	m_DataBase.CalCGCs.CgMode = m_RoadDatabaseArray[RoadIndex].SuphMode;
	m_DataBase.CalCGCs.CgLenMode = m_RoadDatabaseArray[RoadIndex].SuphMode;
	m_DataBase.Write_XLDbs(MdbName, L"超高计算参数", L"");
}

/*===================================================================
函数名:Read_SuphN_Result
------------------------------------------------------------
函数功能:从数据库总读取超高计算结果数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		是否成功读取数据
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	1.必须左右侧都有数据，才算读取成功
		2.读取失败该活动数据库的路面宽数组数据会清空
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Suph_Result(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左超高分段数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左超高分段表", RoadName))
	{
		if (m_DataBase.NCG > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].SuphResultArray_L.clear();
			for (int i = 0; i < m_DataBase.NCG; i++)
				m_RoadDatabaseArray[RoadIndex].SuphResultArray_L.push_back(m_DataBase.CG[i]);
		}
		else
			return false;
	}
	else
		return false;


	//从数据库中读取右超高分段数据
	if (m_DataBase.Read_XLDbs(MdbName, "右超高分段表", RoadName))
	{
		if (m_DataBase.NCG > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].SuphResultArray_R.clear();
			for (int i = 0; i < m_DataBase.NCG; i++)
				m_RoadDatabaseArray[RoadIndex].SuphResultArray_R.push_back(m_DataBase.CG[i]);
		}
		else
		{
			return false;
		}
	}
	else
		return false;

	return true;
}

/*===================================================================
函数名:Read_Widen_Result
------------------------------------------------------------
函数功能:从数据库总读取加宽计算结果数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		是否成功读取数据
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：	1.必须左右侧都有数据，才算读取成功
		2.读取失败该活动数据库的路面宽数组数据会清空
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Widen_Result(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左加宽分段数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左加宽分段表", RoadName))
	{
		if (m_DataBase.NCG > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].WidenResultArray_L.clear();
			for (int i = 0; i < m_DataBase.NCG; i++)
				m_RoadDatabaseArray[RoadIndex].WidenResultArray_L.push_back(m_DataBase.CG[i]);
		}
		else
			return false;
	}
	else
		return false;


	//从数据库中读取右加宽分段数据
	if (m_DataBase.Read_XLDbs(MdbName, "右加宽分段表", RoadName))
	{
		if (m_DataBase.NCG > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].WidenResultArray_R.clear();
			for (int i = 0; i < m_DataBase.NCG; i++)
				m_RoadDatabaseArray[RoadIndex].WidenResultArray_R.push_back(m_DataBase.CG[i]);
		}
		else
		{
			return false;
		}
	}
	else
		return false;

	return true;
}

/*===================================================================
函数名:Write_Suph_Result
------------------------------------------------------------
函数功能:写入超高结果数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Suph_Result(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].SuphResultArray_L.size() || !m_RoadDatabaseArray[RoadIndex].SuphResultArray_R.size())
		return;

	//写入数据	左侧
	m_DataBase.NCG = m_RoadDatabaseArray[RoadIndex].SuphResultArray_L.size();
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].SuphResultArray_L.size(); i++)
	{
		m_DataBase.CG[i].xlc = m_RoadDatabaseArray[RoadIndex].SuphResultArray_L[i].xlc;
		m_DataBase.CG[i].cg = m_RoadDatabaseArray[RoadIndex].SuphResultArray_L[i].cg;
	}
	m_DataBase.Write_XLDbs(MdbName, L"左超高分段表", RoadName);

	//写入数据	右侧
	m_DataBase.NCG = m_RoadDatabaseArray[RoadIndex].SuphResultArray_R.size();
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].SuphResultArray_R.size(); i++)
	{
		m_DataBase.CG[i].xlc = m_RoadDatabaseArray[RoadIndex].SuphResultArray_R[i].xlc;
		m_DataBase.CG[i].cg = m_RoadDatabaseArray[RoadIndex].SuphResultArray_R[i].cg;
	}
	m_DataBase.Write_XLDbs(MdbName, L"右超高分段表", RoadName);
}

/*===================================================================
函数名:Write_Widen_Result
------------------------------------------------------------
函数功能:写入加宽结果数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Widen_Result(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].WidenResultArray_L.size() || !m_RoadDatabaseArray[RoadIndex].WidenResultArray_R.size())
		return;

	//写入数据	左侧
	m_DataBase.NCG = m_RoadDatabaseArray[RoadIndex].WidenResultArray_L.size();
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].WidenResultArray_L.size(); i++)
	{
		m_DataBase.CG[i].xlc = m_RoadDatabaseArray[RoadIndex].WidenResultArray_L[i].xlc;
		m_DataBase.CG[i].cg = m_RoadDatabaseArray[RoadIndex].WidenResultArray_L[i].cg;
	}
	m_DataBase.Write_XLDbs(MdbName, L"左加宽分段表", RoadName);

	//写入数据	右侧
	m_DataBase.NCG = m_RoadDatabaseArray[RoadIndex].WidenResultArray_R.size();
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].WidenResultArray_R.size(); i++)
	{
		m_DataBase.CG[i].xlc = m_RoadDatabaseArray[RoadIndex].WidenResultArray_R[i].xlc;
		m_DataBase.CG[i].cg = m_RoadDatabaseArray[RoadIndex].WidenResultArray_R[i].cg;
	}
	m_DataBase.Write_XLDbs(MdbName, L"右加宽分段表", RoadName);
}

bool RoadDesignWorkingDatabase::Read_Horizontal_Draw_Para(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	CString FileName = MdbName;
	FileName.Replace(L".mdb", L".pmcs");

	if (!Read_Para_File(FileName, m_RoadDatabaseArray[RoadIndex].HDrawPara))
	{
		CString tempCStr = SysPath;
		FileName = tempCStr + L"\\lib\\模板.pmcs";
		if (!Read_Para_File(FileName, m_RoadDatabaseArray[RoadIndex].HDrawPara))
		{
			AfxMessageBox(FileName + L"不存在!");
			return false;
		}
	}

	return true;
}

bool RoadDesignWorkingDatabase::Read_Para_File(CString FileName, DrawHorizontalPara& HDrawPara)
{
	FILE* fp = NULL;
	fp = _wfopen(FileName, L"r");
	ACHAR TempChr[20];
	if (fp)
	{
		fwscanf(fp, L"%d\n", &HDrawPara.DrawMode);//绘图方式 0  线位图 1 平面总体图 2 用地图
		fwscanf(fp, L"%d\n", &HDrawPara.DrawAll);//是否绘所有线路0-是 1-非
		fwscanf(fp, L"%d\n", &HDrawPara.ZDKMode);//绘制占地图方式
		fwscanf(fp, L"%s\n", TempChr);//道路名
		HDrawPara.RoadName = TempChr;
		fwscanf(fp, L"%lf\n", &HDrawPara.QD);//起点里程
		fwscanf(fp, L"%lf\n", &HDrawPara.ZD);//终点里程
		fwscanf(fp, L"%s\n", TempChr);//比例
		HDrawPara.SCALE = TempChr;
		fwscanf(fp, L"%s\n", TempChr);//小数位数
		HDrawPara.XSW = TempChr;
		fwscanf(fp, L"%lf\n", &HDrawPara.WHT);//字高
		fwscanf(fp, L"%s\n", TempChr);//里程标注样式
		HDrawPara.LCBZ = TempChr;
		fwscanf(fp, L"%lf\n", &HDrawPara.ZcLen);//左侧用地标注线长
		fwscanf(fp, L"%lf\n", &HDrawPara.YcLen);//右侧用地标注线长
		fwscanf(fp, L"%s\n", TempChr);//线路中线宽度
		HDrawPara.ZxWid = TempChr;
		fwscanf(fp, L"%s\n", TempChr);//用地桩线宽度
		HDrawPara.YDXWid = TempChr;
		fwscanf(fp, L"%d\n", &HDrawPara.DrawDZQ);
		fwscanf(fp, L"%d\n", &HDrawPara.DrawTun);
		fwscanf(fp, L"%d\n", &HDrawPara.DrawCulvet);
		fwscanf(fp, L"%d\n", &HDrawPara.DrawBM);
		fwscanf(fp, L"%d\n", &HDrawPara.DrawDX);
		fwscanf(fp, L"%d\n", &HDrawPara.DelEntInf);
		fwscanf(fp, L"%d\n", &HDrawPara.BZRAL);
		fwscanf(fp, L"%d\n", &HDrawPara.DrawJD);
		fwscanf(fp, L"%d\n", &HDrawPara.drawfldzdk);
		fclose(fp);
		return true;
	}
	else
		return false;
}

bool RoadDesignWorkingDatabase::Write_Horizontal_Draw_Para(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return false;

	CString FileName = MdbName;
	FileName.Replace(L".mdb", L".pmcs");
	FILE* fp = NULL;
	fp = _wfopen(FileName, L"w");
	if (fp)
	{
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawMode);//绘图方式 0  线位图 1 平面总体图 2 用地图
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawAll);//是否绘所有线路0-是 1-非
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.ZDKMode);//绘制占地图方式
		fwprintf(fp, L"%s\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.RoadName);//道路名
		fwprintf(fp, L"%lf\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.QD);//起点里程
		fwprintf(fp, L"%lf\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.ZD);//终点里程
		fwprintf(fp, L"%s\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.SCALE);//比例
		fwprintf(fp, L"%s\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.XSW);//小数位数
		fwprintf(fp, L"%lf\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.WHT);//字高
		fwprintf(fp, L"%s\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.LCBZ);//里程标注样式
		fwprintf(fp, L"%lf\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.ZcLen);//左侧用地标注线长
		fwprintf(fp, L"%lf\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.YcLen);//右侧用地标注线长
		fwprintf(fp, L"%s\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.ZxWid);//线路中线宽度
		fwprintf(fp, L"%s\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.YDXWid);//用地桩线宽度
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawDZQ);//绘大中桥
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawTun);//绘隧道
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawCulvet);//绘小桥涵
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawBM);//绘水准点
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawDX);//绘导线
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DelEntInf);//删除界内实体
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.BZRAL);//标注半径缓长
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.DrawJD);//绘交点
		fwprintf(fp, L"%d\n", m_RoadDatabaseArray[RoadIndex].HDrawPara.drawfldzdk);//绘分离段占地宽
		fclose(fp);
		return true;
	}
	else
		return false;
}

/*===================================================================
函数名:Read_EarthLine
------------------------------------------------------------
函数功能:读取地面线数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_EarthLine(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//生成平面指针
	JD_CENTER* pRoad = CreateHorizontalRoad(MdbName, RoadName);
	if (!pRoad)
		return false;

	//从数据库中读取纵地面线数据
	if (m_DataBase.Read_XLDbs(MdbName, L"纵地面线表", RoadName))
	{
		if (m_DataBase.NTW > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].EarthLine.clear();

			CString gh;
			double xlc;
			DMXYS tempEarth;
			for (int i = 0; i < m_DataBase.NTW; i++)
			{
				tempEarth.lc = pRoad->TYLC(m_DataBase.TWB[i].ml);
				tempEarth.level = m_DataBase.TWB[i].DMGC;

				xlc = pRoad->XLC(tempEarth.lc, gh);
				_tcscpy(tempEarth.ckml, L"\0");
				_tcscpy(tempEarth.ckml, pRoad->LCchr(gh, xlc, 2));

				m_RoadDatabaseArray[RoadIndex].EarthLine.push_back(tempEarth);
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

/*===================================================================
函数名:Write_EarthLine
------------------------------------------------------------
函数功能:写入地面线数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_EarthLine(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].EarthLine.size())
		return;

	//生成平面指针
	JD_CENTER* pRoad = CreateHorizontalRoad(MdbName, RoadName);
	if (!pRoad)
		return;

	//设置数据库数据
	m_DataBase.NTW = m_RoadDatabaseArray[RoadIndex].EarthLine.size();
	if (m_DataBase.TWB) 
	{ 
		delete[]m_DataBase.TWB;
		m_DataBase.TWB = NULL;
	}
	m_DataBase.TWB = new XLDataBase::TWTAB[m_DataBase.NTW];

	//传递数据，写入数据库
	CString str;
	for (int i = 0; i < m_DataBase.NTW; i++)
	{
		double ml = pRoad->XLC1(m_RoadDatabaseArray[RoadIndex].EarthLine[i].lc);
		str.Format(L"%0.2lf", ml);
		m_DataBase.TWB[i].ml = _wtof(str);
		str.Format(L"%0.2lf", m_RoadDatabaseArray[RoadIndex].EarthLine[i].level);
		m_DataBase.TWB[i].DMGC = _wtof(str);

	}
	m_DataBase.Write_XLDbs(MdbName, L"纵地面线表", RoadName);
}

/*===================================================================
函数名:Read_Slope_Data
------------------------------------------------------------
函数功能:读取坡度数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	默认读取的是全幅数据
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Slope_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取坡度数据
	if (m_DataBase.Read_XLDbs(MdbName, L"坡度表", RoadName, L"全幅"))
	{
		if (m_DataBase.NPD > 0)//有数据
		{
			m_RoadDatabaseArray[RoadIndex].SlopeArray.clear();

			CString gh;
			double xlc;
			PDTAB_ROAD tempSlope;
			for (int i = 0; i < m_DataBase.NPD; i++)
			{
				tempSlope.ml = m_DataBase.PDB[i].ml;
				tempSlope.cml = m_DataBase.PDB[i].cml;
				tempSlope.Level = m_DataBase.PDB[i].Level;
				tempSlope.Rshu = m_DataBase.PDB[i].Rshu;
				tempSlope.RLen = m_DataBase.PDB[i].RLen;
				tempSlope.degree = m_DataBase.PDB[i].degree;
				tempSlope.length = m_DataBase.PDB[i].length;
				m_RoadDatabaseArray[RoadIndex].SlopeArray.push_back(tempSlope);
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

/*===================================================================
函数名:Write_Slope_Data
------------------------------------------------------------
函数功能:写入坡度数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	1.默认读取的是全幅数据
		2.数据需要在外部计算好
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Slope_Data(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].SlopeArray.size())
		return;

	//设置数据库数据
	m_DataBase.NPD = m_RoadDatabaseArray[RoadIndex].SlopeArray.size();
	if (m_DataBase.PDB)
	{
		delete[]m_DataBase.PDB;
		m_DataBase.PDB = NULL;
	}
	m_DataBase.PDB = new XLDataBase::PDTAB[m_DataBase.NTW];

	//传递数据，写入数据库
	for (int i = 0; i < m_DataBase.NPD; i++)
	{
		m_DataBase.PDB[i].ml = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].ml;
		m_DataBase.PDB[i].cml = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].cml;
		m_DataBase.PDB[i].Level = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].Level;
		m_DataBase.PDB[i].Rshu = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].Rshu;
		m_DataBase.PDB[i].RLen = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].RLen;
		m_DataBase.PDB[i].degree = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].degree;
		m_DataBase.PDB[i].length = m_RoadDatabaseArray[RoadIndex].SlopeArray[i].length;
	}
	m_DataBase.Write_XLDbs(MdbName, L"坡度表", RoadName, L"全幅");
}

/*===================================================================
函数名:Read_Bridge
------------------------------------------------------------
函数功能:读取桥梁数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：	
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Bridge(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取大中桥数据
	if (m_DataBase.Read_XLDbs(MdbName, L"大中桥", RoadName))
	{
		m_RoadDatabaseArray[RoadIndex].BriArray.clear();
		for (int i = 0; i < m_DataBase.BriNum; i++)
			m_RoadDatabaseArray[RoadIndex].BriArray.push_back(m_DataBase.Bri[i]);
	}
	else
		return false;

	if (m_RoadDatabaseArray[RoadIndex].BriArray.size())
		return true;
	else
		return false;
}

/*===================================================================
函数名:Write_Bridge
------------------------------------------------------------
函数功能:写入桥梁数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Bridge(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].BriArray.size())
		return;

	//设置数据库数据
	m_DataBase.BriNum = m_RoadDatabaseArray[RoadIndex].BriArray.size();
	if (m_DataBase.Bri)
	{
		delete[]m_DataBase.Bri;
		m_DataBase.Bri = NULL;
	}
	m_DataBase.Bri = new BRITAB[m_DataBase.BriNum];

	//传递数据，写入数据库
	for (int i = 0; i < m_DataBase.BriNum; i++)
		m_DataBase.Bri[i] = m_RoadDatabaseArray[RoadIndex].BriArray[i];
	m_DataBase.Write_XLDbs(MdbName, L"大中桥", RoadName);
}

/*===================================================================
函数名:Read_Tunnel
------------------------------------------------------------
函数功能:读取隧道数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_Tunnel(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取隧道数据
	if (m_DataBase.Read_XLDbs(MdbName, L"隧道", RoadName))
	{
		m_RoadDatabaseArray[RoadIndex].TunArray.clear();
		for (int i = 0; i < m_DataBase.TunNum; i++)
			m_RoadDatabaseArray[RoadIndex].TunArray.push_back(m_DataBase.Tun[i]);
	}
	else
		return false;

	if (m_RoadDatabaseArray[RoadIndex].TunArray.size())
		return true;
	else
		return false;
}

/*===================================================================
函数名:Write_Tunnel
------------------------------------------------------------
函数功能:写入隧道数据
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_Tunnel(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].TunArray.size())
		return;

	//设置数据库数据
	m_DataBase.TunNum = m_RoadDatabaseArray[RoadIndex].TunArray.size();
	if (m_DataBase.Tun)
	{
		delete[]m_DataBase.Tun;
		m_DataBase.Tun = NULL;
	}
	m_DataBase.Tun = new TUNTAB[m_DataBase.TunNum];

	//传递数据，写入数据库
	for (int i = 0; i < m_DataBase.BriNum; i++)
		m_DataBase.Tun[i] = m_RoadDatabaseArray[RoadIndex].TunArray[i];
	m_DataBase.Write_XLDbs(MdbName, L"隧道", RoadName);
}

//读取控制标高数据
bool RoadDesignWorkingDatabase::Read_LevelCtrlData(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取高程控制点表数据
	if (m_DataBase.Read_XLDbs(MdbName, L"高程控制点表", RoadName))
	{
		m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray = m_DataBase.m_LevelCtrlPtArray;
	}
	else
		return false;

	//计算连续里程
	JD_CENTER* pRoad = CreateHorizontalRoad(MdbName, RoadName);
	if (pRoad)
	{
		ACHAR CKML[64];
		for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray.size(); i++)
		{
			_tcscpy(CKML, pRoad->LCchr(m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray[i].GH, m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray[i].XCLc, 3));
			m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray[i].Cml = pRoad->Trs_Ckml_To_ProjMl(CKML);
		}
	}

	if (m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray.size())
		return true;
	else
		return false;
}

//写入控制标高数据
void RoadDesignWorkingDatabase::Write_LevelCtrlData(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//设置数据库数据
	m_DataBase.m_LevelCtrlPtArray = m_RoadDatabaseArray[RoadIndex].LevelCtrlPtArray;
	m_DataBase.Write_XLDbs(MdbName, L"高程控制点表", RoadName);
}

//读取接坡信息数据
bool RoadDesignWorkingDatabase::Read_JPData(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取高程控制点表数据
	if (m_DataBase.Read_XLDbs(MdbName, L"接坡信息表", RoadName))
	{
		m_RoadDatabaseArray[RoadIndex].JPArray = m_DataBase.m_JPArray;
	}
	else
		return false;

	if (m_RoadDatabaseArray[RoadIndex].JPArray.size())
		return true;
	else
		return false;
}

//写入接坡信息数据
void RoadDesignWorkingDatabase::Write_JPData(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//设置数据库数据
	m_DataBase.m_JPArray = m_RoadDatabaseArray[RoadIndex].JPArray;
	m_DataBase.Write_XLDbs(MdbName, L"接坡信息表", RoadName);
}

/*===================================================================
函数名:Read_SFYSXS
------------------------------------------------------------
函数功能:读取松方压实系数
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_SFYSXS(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取松方压实系数数据
	if (m_DataBase.Read_XLDbs(MdbName, L"松方压实系数", RoadName))
	{
		for (int i = 0; i < 6; i++)
			m_RoadDatabaseArray[RoadIndex].YSXS[i] = m_DataBase.YSXS[i];
	}
	else
		return false;

	return true;
}

/*===================================================================
函数名:Write_SFYSXS
------------------------------------------------------------
函数功能:写入松方压实系数
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：	这个静态double无法检测是否存在数据，请在外部确定
		录入数据正确后再调用该函数保存数据库
=====================================================================*/
void RoadDesignWorkingDatabase::Write_SFYSXS(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//传递数据，写入数据库
	for (int i = 0; i < 6; i++)
		m_DataBase.YSXS[i] = m_RoadDatabaseArray[RoadIndex].YSXS[i];
	m_DataBase.Write_XLDbs(MdbName, L"松方压实系数", RoadName);
}

/*===================================================================
函数名:Read_FillType
------------------------------------------------------------
函数功能:读取填方分类系数
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_FillType(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取填方分类数据
	if (m_DataBase.Read_XLDbs(MdbName, L"填方分类", RoadName))
		m_RoadDatabaseArray[RoadIndex].FillType = m_DataBase.m_FillType;		
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].FillType.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_FillType
------------------------------------------------------------
函数功能:写入填方分类系数
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：	
=====================================================================*/
void RoadDesignWorkingDatabase::Write_FillType(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].FillType.size())
		return;

	//传递数据，写入数据库
	m_DataBase.m_FillType = m_RoadDatabaseArray[RoadIndex].FillType;
	m_DataBase.Write_XLDbs(MdbName, L"填方分类", RoadName);
}


/*===================================================================
函数名:Read_YongDiFDB
------------------------------------------------------------
函数功能:读取用地面积汇总分段表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_YongDiFDB(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取用地面积汇总分段数据
	if (m_DataBase.Read_XLDbs(MdbName, L"用地面积汇总分段表", RoadName))
		m_RoadDatabaseArray[RoadIndex].EarthSection = m_DataBase.m_EarthSection;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].EarthSection.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_YongDiFDB
------------------------------------------------------------
函数功能:写入用地面积汇总分段表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_YongDiFDB(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].EarthSection.size())
		return;

	//传递数据，写入数据库
	m_DataBase.m_EarthSection = m_RoadDatabaseArray[RoadIndex].EarthSection;
	m_DataBase.Write_XLDbs(MdbName, L"用地面积汇总分段表", RoadName);
}

/*===================================================================
函数名:Read_EarthType
------------------------------------------------------------
函数功能:读取用地类型表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_EarthType(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取用地类型数据
	if (m_DataBase.Read_XLDbs(MdbName, L"用地类型表", RoadName))
	{
		m_RoadDatabaseArray[RoadIndex].EarthType.resize(m_DataBase.YDLXSum);
		for (int i = 0; i < m_DataBase.YDLXSum; i++)
		{
			m_RoadDatabaseArray[RoadIndex].EarthType[i].dml = m_DataBase.YDLXB[i].dml;
			m_RoadDatabaseArray[RoadIndex].EarthType[i].YDLX = m_DataBase.YDLXB[i].YDLX;
		}
	}
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].EarthType.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_EarthType
------------------------------------------------------------
函数功能:写入用地类型表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_EarthType(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].EarthType.size())
		return;

	//设置数据库数据
	m_DataBase.YDLXSum = m_RoadDatabaseArray[RoadIndex].EarthType.size();
	if (m_DataBase.YDLXB)
	{
		delete[]m_DataBase.YDLXB;
		m_DataBase.YDLXB = NULL;
	}
	m_DataBase.YDLXB = new YongDiLXB[m_DataBase.YDLXSum];

	//传递数据，写入数据库
	for (int i = 0; i < m_RoadDatabaseArray[RoadIndex].EarthType.size(); i++)
	{
		m_DataBase.YDLXB[i].dml = m_RoadDatabaseArray[RoadIndex].EarthType[i].dml;
		m_DataBase.YDLXB[i].YDLX = m_RoadDatabaseArray[RoadIndex].EarthType[i].YDLX;
	}
	m_DataBase.Write_XLDbs(MdbName, L"用地类型表", RoadName);
}

/*===================================================================
函数名:Read_CutType
------------------------------------------------------------
函数功能:读取挖方分类
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_CutType(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取挖方分类数据
	if (m_DataBase.Read_XLDbs(MdbName, L"挖方分类", RoadName))
		m_RoadDatabaseArray[RoadIndex].CutType = m_DataBase.m_CutType;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].CutType.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_CutType
------------------------------------------------------------
函数功能:写入挖方分类
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_CutType(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].CutType.size())
		return;

	//传递数据，写入数据库
	m_DataBase.m_CutType = m_RoadDatabaseArray[RoadIndex].CutType;
	m_DataBase.Write_XLDbs(MdbName, L"挖方分类", RoadName);
}

/*===================================================================
函数名:Read_GLDMLZ
------------------------------------------------------------
函数功能:读取关联断面路肩拉直
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_GLDMLZ(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取关联断面路肩拉直数据
	if (m_DataBase.Read_XLDbs(MdbName, L"关联断面路肩拉直", RoadName))
		m_RoadDatabaseArray[RoadIndex].GLDMLZ = m_DataBase.m_GLDMLZ;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].GLDMLZ.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_GLDMLZ
------------------------------------------------------------
函数功能:写入关联断面路肩拉直
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_GLDMLZ(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].GLDMLZ.size())
		return;

	//传递数据，写入数据库
	m_DataBase.m_GLDMLZ = m_RoadDatabaseArray[RoadIndex].GLDMLZ;
	m_DataBase.Write_XLDbs(MdbName, L"关联断面路肩拉直", RoadName);
}

/*===================================================================
函数名:Read_GLDMLQBPTP
------------------------------------------------------------
函数功能:读取关联断面路堑边坡推平
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_GLDMLQBPTP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取关联断面路堑边坡推平数据
	if (m_DataBase.Read_XLDbs(MdbName, L"关联断面路堑边坡推平", RoadName))
		m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP = m_DataBase.m_GLDMLQBPTP;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_GLDMLQBPTP
------------------------------------------------------------
函数功能:写入关联断面路堑边坡推平
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_GLDMLQBPTP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP.size())
		return;

	//传递数据，写入数据库
	m_DataBase.m_GLDMLQBPTP = m_RoadDatabaseArray[RoadIndex].GLDMLQBPTP;
	m_DataBase.Write_XLDbs(MdbName, L"关联断面路堑边坡推平", RoadName);
}

/*===================================================================
函数名:Read_GLDMXJG
------------------------------------------------------------
函数功能:读取关联断面线间沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_GLDMXJG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取联断面线间沟数据
	if (m_DataBase.Read_XLDbs(MdbName, L"关联断面线间沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].GLDMXJG = m_DataBase.m_GLDMXJG;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].GLDMXJG.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_GLDMXJG
------------------------------------------------------------
函数功能:写入关联断面线间沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_GLDMXJG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].GLDMXJG.size())
		return;

	//传递数据，写入数据库
	m_DataBase.m_GLDMXJG = m_RoadDatabaseArray[RoadIndex].GLDMXJG;
	m_DataBase.Write_XLDbs(MdbName, L"关联断面线间沟", RoadName);
}

/*===================================================================
函数名:Read_HTHD
------------------------------------------------------------
函数功能:读取清表土厚度
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_HTHD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左清表土厚度数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左清表土厚度", RoadName))
		m_RoadDatabaseArray[RoadIndex].HTHD_L = m_DataBase.m_HTHD;
	else
		return false;

	//从数据库中读取右清表土厚度数据
	if(m_DataBase.Read_XLDbs(MdbName, "右清表土厚度", RoadName))
		m_RoadDatabaseArray[RoadIndex].HTHD_R = m_DataBase.m_HTHD;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].HTHD_L.size() && !m_RoadDatabaseArray[RoadIndex].HTHD_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_HTHD
------------------------------------------------------------
函数功能:写入清表土厚度
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_HTHD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].HTHD_L.size() || !m_RoadDatabaseArray[RoadIndex].HTHD_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_HTHD = m_RoadDatabaseArray[RoadIndex].HTHD_L;
	m_DataBase.Write_XLDbs(MdbName, L"左清表土厚度", RoadName);

	//写入数据	右侧
	m_DataBase.m_HTHD = m_RoadDatabaseArray[RoadIndex].HTHD_R;
	m_DataBase.Write_XLDbs(MdbName, L"右清表土厚度", RoadName);
}

/*===================================================================
函数名:Read_GouPD
------------------------------------------------------------
函数功能:读取沟底坡
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_GouPD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左沟底坡数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左沟底坡", RoadName))
		m_RoadDatabaseArray[RoadIndex].GouPD_L = m_DataBase.m_GouPD;
	else
		return false;

	//从数据库中读取右沟底坡数据
	if (m_DataBase.Read_XLDbs(MdbName, "右沟底坡", RoadName))
		m_RoadDatabaseArray[RoadIndex].GouPD_R = m_DataBase.m_GouPD;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].GouPD_L.size() && !m_RoadDatabaseArray[RoadIndex].GouPD_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_GouPD
------------------------------------------------------------
函数功能:写入沟底坡
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_GouPD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].GouPD_L.size() || !m_RoadDatabaseArray[RoadIndex].GouPD_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_GouPD = m_RoadDatabaseArray[RoadIndex].GouPD_L;
	m_DataBase.Write_XLDbs(MdbName, L"左沟底坡", RoadName);

	//写入数据	右侧
	m_DataBase.m_GouPD = m_RoadDatabaseArray[RoadIndex].GouPD_R;
	m_DataBase.Write_XLDbs(MdbName, L"右沟底坡", RoadName);
}

/*===================================================================
函数名:Read_CTKD
------------------------------------------------------------
函数功能:读取超填宽度表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_CTKD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左超填宽度数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左超填宽度表", RoadName))
		m_RoadDatabaseArray[RoadIndex].CTKD_L = m_DataBase.m_CTKD;
	else
		return false;

	//从数据库中读取右超填宽度数据
	if (m_DataBase.Read_XLDbs(MdbName, "右超填宽度表", RoadName))
		m_RoadDatabaseArray[RoadIndex].CTKD_R = m_DataBase.m_CTKD;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].CTKD_L.size() && !m_RoadDatabaseArray[RoadIndex].CTKD_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_CTKD
------------------------------------------------------------
函数功能:写入超填宽度表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_CTKD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].CTKD_L.size() || !m_RoadDatabaseArray[RoadIndex].CTKD_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_CTKD = m_RoadDatabaseArray[RoadIndex].CTKD_L;
	m_DataBase.Write_XLDbs(MdbName, L"左超填宽度表", RoadName);

	//写入数据	右侧
	m_DataBase.m_CTKD = m_RoadDatabaseArray[RoadIndex].CTKD_R;
	m_DataBase.Write_XLDbs(MdbName, L"右超填宽度表", RoadName);
}

/*===================================================================
函数名:Read_ARCBP
------------------------------------------------------------
函数功能:读取弧形边坡表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_ARCBP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左弧形边坡数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左弧形边坡表", RoadName))
		m_RoadDatabaseArray[RoadIndex].ARCBP_L = m_DataBase.m_ARCBP;
	else
		return false;

	//从数据库中读取右弧形边坡数据
	if (m_DataBase.Read_XLDbs(MdbName, "右弧形边坡表", RoadName))
		m_RoadDatabaseArray[RoadIndex].ARCBP_R = m_DataBase.m_ARCBP;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].ARCBP_L.size() && !m_RoadDatabaseArray[RoadIndex].ARCBP_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_ARCBP
------------------------------------------------------------
函数功能:写入弧形边坡表
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_ARCBP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].ARCBP_L.size() || !m_RoadDatabaseArray[RoadIndex].ARCBP_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_ARCBP = m_RoadDatabaseArray[RoadIndex].ARCBP_L;
	m_DataBase.Write_XLDbs(MdbName, L"左弧形边坡表", RoadName);

	//写入数据	右侧
	m_DataBase.m_ARCBP = m_RoadDatabaseArray[RoadIndex].ARCBP_R;
	m_DataBase.Write_XLDbs(MdbName, L"右弧形边坡表", RoadName);
}

/*===================================================================
函数名:Read_DQ
------------------------------------------------------------
函数功能:读取挡墙
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_DQ(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左挡墙数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左挡墙", RoadName))
		m_RoadDatabaseArray[RoadIndex].DQ_L = m_DataBase.m_DQ;
	else
		return false;

	//从数据库中读取右挡墙数据
	if (m_DataBase.Read_XLDbs(MdbName, "右挡墙", RoadName))
		m_RoadDatabaseArray[RoadIndex].DQ_R = m_DataBase.m_DQ;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].DQ_L.size() && !m_RoadDatabaseArray[RoadIndex].DQ_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_DQ
------------------------------------------------------------
函数功能:写入挡墙
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_DQ(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].DQ_L.size() || !m_RoadDatabaseArray[RoadIndex].DQ_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_DQ = m_RoadDatabaseArray[RoadIndex].DQ_L;
	m_DataBase.Write_XLDbs(MdbName, L"左挡墙", RoadName);

	//写入数据	右侧
	m_DataBase.m_DQ = m_RoadDatabaseArray[RoadIndex].DQ_R;
	m_DataBase.Write_XLDbs(MdbName, L"右挡墙", RoadName);
}

/*===================================================================
函数名:Read_LDTP
------------------------------------------------------------
函数功能:读取低填路堤自动填平
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LDTP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左低填路堤自动填平数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左低填路堤自动填平", RoadName))
		m_RoadDatabaseArray[RoadIndex].LDTP_L = m_DataBase.m_LDTP;
	else
		return false;

	//从数据库中读取右低填路堤自动填平数据
	if (m_DataBase.Read_XLDbs(MdbName, "右低填路堤自动填平", RoadName))
		m_RoadDatabaseArray[RoadIndex].LDTP_R = m_DataBase.m_LDTP;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LDTP_L.size() && !m_RoadDatabaseArray[RoadIndex].LDTP_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LDTP
------------------------------------------------------------
函数功能:写入低填路堤自动填平
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LDTP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LDTP_L.size() || !m_RoadDatabaseArray[RoadIndex].LDTP_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LDTP = m_RoadDatabaseArray[RoadIndex].LDTP_L;
	m_DataBase.Write_XLDbs(MdbName, L"左低填路堤自动填平", RoadName);

	//写入数据	右侧
	m_DataBase.m_LDTP = m_RoadDatabaseArray[RoadIndex].LDTP_R;
	m_DataBase.Write_XLDbs(MdbName, L"右低填路堤自动填平", RoadName);
}

/*===================================================================
函数名:Read_LQTP
------------------------------------------------------------
函数功能:读取低挖路堑自动推平
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LQTP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左低挖路堑自动推平数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左低挖路堑自动推平", RoadName))
		m_RoadDatabaseArray[RoadIndex].LQTP_L = m_DataBase.m_LQTP;
	else
		return false;

	//从数据库中读取右低挖路堑自动推平数据
	if (m_DataBase.Read_XLDbs(MdbName, "右低挖路堑自动推平", RoadName))
		m_RoadDatabaseArray[RoadIndex].LQTP_R = m_DataBase.m_LQTP;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LQTP_L.size() && !m_RoadDatabaseArray[RoadIndex].LQTP_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LQTP
------------------------------------------------------------
函数功能:写入低挖路堑自动推平
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LQTP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LQTP_L.size() || !m_RoadDatabaseArray[RoadIndex].LQTP_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LQTP = m_RoadDatabaseArray[RoadIndex].LQTP_L;
	m_DataBase.Write_XLDbs(MdbName, L"左低挖路堑自动推平", RoadName);

	//写入数据	右侧
	m_DataBase.m_LQTP = m_RoadDatabaseArray[RoadIndex].LQTP_R;
	m_DataBase.Write_XLDbs(MdbName, L"右低挖路堑自动推平", RoadName);
}

/*===================================================================
函数名:Read_TLJJK
------------------------------------------------------------
函数功能:读取低填路堤自动加宽土路肩
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_TLJJK(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左低填路堤自动加宽土路肩数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左低填路堤自动加宽土路肩", RoadName))
		m_RoadDatabaseArray[RoadIndex].TLJJK_L = m_DataBase.m_TLJJK;
	else
		return false;

	//从数据库中读取右低填路堤自动加宽土路肩数据
	if (m_DataBase.Read_XLDbs(MdbName, "右低填路堤自动加宽土路肩", RoadName))
		m_RoadDatabaseArray[RoadIndex].TLJJK_R = m_DataBase.m_TLJJK;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].TLJJK_L.size() && !m_RoadDatabaseArray[RoadIndex].TLJJK_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_TLJJK
------------------------------------------------------------
函数功能:写入低填路堤自动加宽土路肩
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_TLJJK(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].TLJJK_L.size() || !m_RoadDatabaseArray[RoadIndex].TLJJK_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_TLJJK = m_RoadDatabaseArray[RoadIndex].TLJJK_L;
	m_DataBase.Write_XLDbs(MdbName, L"左低填路堤自动加宽土路肩", RoadName);

	//写入数据	右侧
	m_DataBase.m_TLJJK = m_RoadDatabaseArray[RoadIndex].TLJJK_R;
	m_DataBase.Write_XLDbs(MdbName, L"右低填路堤自动加宽土路肩", RoadName);
}

/*===================================================================
函数名:Read_LMHD
------------------------------------------------------------
函数功能:读取路面厚度
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LMHD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左路面厚度数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左路面厚度", RoadName))
		m_RoadDatabaseArray[RoadIndex].LMHD_L = m_DataBase.m_LMHD;
	else
		return false;

	//从数据库中读取右路面厚度数据
	if (m_DataBase.Read_XLDbs(MdbName, "右路面厚度", RoadName))
		m_RoadDatabaseArray[RoadIndex].LMHD_R = m_DataBase.m_LMHD;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LMHD_L.size() && !m_RoadDatabaseArray[RoadIndex].LMHD_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LMHD
------------------------------------------------------------
函数功能:写入路面厚度
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LMHD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LMHD_L.size() || !m_RoadDatabaseArray[RoadIndex].LMHD_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LMHD = m_RoadDatabaseArray[RoadIndex].LMHD_L;
	m_DataBase.Write_XLDbs(MdbName, L"左路面厚度", RoadName);

	//写入数据	右侧
	m_DataBase.m_LMHD = m_RoadDatabaseArray[RoadIndex].LMHD_R;
	m_DataBase.Write_XLDbs(MdbName, L"右路面厚度", RoadName);
}

/*===================================================================
函数名:Read_CRSWID
------------------------------------------------------------
函数功能:读取地面线内插宽
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_CRSWID(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左地面线内插宽数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左地面线内插宽", RoadName))
		m_RoadDatabaseArray[RoadIndex].CRSWID_L = m_DataBase.m_CRSWID;
	else
		return false;

	//从数据库中读取右地面线内插宽数据
	if (m_DataBase.Read_XLDbs(MdbName, "右地面线内插宽", RoadName))
		m_RoadDatabaseArray[RoadIndex].CRSWID_R = m_DataBase.m_CRSWID;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].CRSWID_L.size() && !m_RoadDatabaseArray[RoadIndex].CRSWID_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_CRSWID
------------------------------------------------------------
函数功能:写入地面线内插宽
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_CRSWID(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].CRSWID_L.size() || !m_RoadDatabaseArray[RoadIndex].CRSWID_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_CRSWID = m_RoadDatabaseArray[RoadIndex].CRSWID_L;
	m_DataBase.Write_XLDbs(MdbName, L"左地面线内插宽", RoadName);

	//写入数据	右侧
	m_DataBase.m_CRSWID = m_RoadDatabaseArray[RoadIndex].CRSWID_R;
	m_DataBase.Write_XLDbs(MdbName, L"右地面线内插宽", RoadName);
}

/*===================================================================
函数名:Read_DM
------------------------------------------------------------
函数功能:读取横断面限制宽度
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_DM(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左横断面限制宽度数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左横断面限制宽度", RoadName))
		m_RoadDatabaseArray[RoadIndex].DM_L = m_DataBase.m_DM;
	else
		return false;

	//从数据库中读取右横断面限制宽度数据
	if (m_DataBase.Read_XLDbs(MdbName, "右横断面限制宽度", RoadName))
		m_RoadDatabaseArray[RoadIndex].DM_R = m_DataBase.m_DM;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].DM_L.size() && !m_RoadDatabaseArray[RoadIndex].DM_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_DM
------------------------------------------------------------
函数功能:写入横断面限制宽度
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_DM(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].DM_L.size() || !m_RoadDatabaseArray[RoadIndex].DM_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_DM = m_RoadDatabaseArray[RoadIndex].DM_L;
	m_DataBase.Write_XLDbs(MdbName, L"左横断面限制宽度", RoadName);

	//写入数据	右侧
	m_DataBase.m_DM = m_RoadDatabaseArray[RoadIndex].DM_R;
	m_DataBase.Write_XLDbs(MdbName, L"右横断面限制宽度", RoadName);
}

/*===================================================================
函数名:Read_PWD
------------------------------------------------------------
函数功能:读取用地加宽
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_PWD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左用地加宽数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左用地加宽", RoadName))
		m_RoadDatabaseArray[RoadIndex].PWD_L = m_DataBase.m_PWD;
	else
		return false;

	//从数据库中读取右用地加宽数据
	if (m_DataBase.Read_XLDbs(MdbName, "右用地加宽", RoadName))
		m_RoadDatabaseArray[RoadIndex].PWD_R = m_DataBase.m_PWD;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].PWD_R.size() && !m_RoadDatabaseArray[RoadIndex].PWD_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_PWD
------------------------------------------------------------
函数功能:写入用地加宽
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_PWD(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].PWD_L.size() || !m_RoadDatabaseArray[RoadIndex].PWD_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_PWD = m_RoadDatabaseArray[RoadIndex].PWD_L;
	m_DataBase.Write_XLDbs(MdbName, L"左用地加宽", RoadName);

	//写入数据	右侧
	m_DataBase.m_PWD = m_RoadDatabaseArray[RoadIndex].PWD_R;
	m_DataBase.Write_XLDbs(MdbName, L"右用地加宽", RoadName);
}

/*===================================================================
函数名:Read_LDBP
------------------------------------------------------------
函数功能:读取路堤边坡
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LDBP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左路堤边坡数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左路堤边坡", RoadName))
		m_RoadDatabaseArray[RoadIndex].LDBP_L = m_DataBase.m_LDBP;
	else
		return false;

	//从数据库中读取右路堤边坡数据
	if (m_DataBase.Read_XLDbs(MdbName, "右路堤边坡", RoadName))
		m_RoadDatabaseArray[RoadIndex].LDBP_R = m_DataBase.m_LDBP;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LDBP_L.size() && !m_RoadDatabaseArray[RoadIndex].LDBP_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LDBP
------------------------------------------------------------
函数功能:写入路堤边坡
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LDBP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LDBP_L.size() || !m_RoadDatabaseArray[RoadIndex].LDBP_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LDBP = m_RoadDatabaseArray[RoadIndex].LDBP_L;
	m_DataBase.Write_XLDbs(MdbName, L"左路堤边坡", RoadName);

	//写入数据	右侧
	m_DataBase.m_LDBP = m_RoadDatabaseArray[RoadIndex].LDBP_R;
	m_DataBase.Write_XLDbs(MdbName, L"右路堤边坡", RoadName);
}

/*===================================================================
函数名:Read_LQBP
------------------------------------------------------------
函数功能:读取路堑边坡
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LQBP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左路堑边坡数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左路堑边坡", RoadName))
		m_RoadDatabaseArray[RoadIndex].LQBP_L = m_DataBase.m_LQBP;
	else
		return false;

	//从数据库中读取右路堑边坡数据
	if (m_DataBase.Read_XLDbs(MdbName, "右路堑边坡", RoadName))
		m_RoadDatabaseArray[RoadIndex].LQBP_R = m_DataBase.m_LQBP;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LQBP_L.size() && !m_RoadDatabaseArray[RoadIndex].LQBP_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LQBP
------------------------------------------------------------
函数功能:写入路堑边坡
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LQBP(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LQBP_L.size() || !m_RoadDatabaseArray[RoadIndex].LQBP_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LQBP = m_RoadDatabaseArray[RoadIndex].LQBP_L;
	m_DataBase.Write_XLDbs(MdbName, L"左路堑边坡", RoadName);

	//写入数据	右侧
	m_DataBase.m_LQBP = m_RoadDatabaseArray[RoadIndex].LQBP_R;
	m_DataBase.Write_XLDbs(MdbName, L"右路堑边坡", RoadName);
}

/*===================================================================
函数名:Read_LDSG
------------------------------------------------------------
函数功能:读取排水沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LDSG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左排水沟数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左排水沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].LDSG_L = m_DataBase.m_LDSG;
	else
		return false;

	//从数据库中读取右排水沟数据
	if (m_DataBase.Read_XLDbs(MdbName, "右排水沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].LDSG_R = m_DataBase.m_LDSG;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LDSG_L.size() && !m_RoadDatabaseArray[RoadIndex].LDSG_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LDSG
------------------------------------------------------------
函数功能:写入排水沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LDSG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LDSG_L.size() || !m_RoadDatabaseArray[RoadIndex].LDSG_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LDSG = m_RoadDatabaseArray[RoadIndex].LDSG_L;
	m_DataBase.Write_XLDbs(MdbName, L"左排水沟", RoadName);

	//写入数据	右侧
	m_DataBase.m_LDSG = m_RoadDatabaseArray[RoadIndex].LDSG_R;
	m_DataBase.Write_XLDbs(MdbName, L"右排水沟", RoadName);
}

/*===================================================================
函数名:Read_LQSG
------------------------------------------------------------
函数功能:读取侧沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_LQSG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左侧沟数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左侧沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].LQSG_L = m_DataBase.m_LQSG;
	else
		return false;

	//从数据库中读取右侧沟数据
	if (m_DataBase.Read_XLDbs(MdbName, "右侧沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].LQSG_R = m_DataBase.m_LQSG;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].LQSG_L.size() && !m_RoadDatabaseArray[RoadIndex].LQSG_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_LDSG
------------------------------------------------------------
函数功能:写入排水沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_LQSG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].LQSG_L.size() || !m_RoadDatabaseArray[RoadIndex].LQSG_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_LQSG = m_RoadDatabaseArray[RoadIndex].LQSG_L;
	m_DataBase.Write_XLDbs(MdbName, L"左侧沟", RoadName);

	//写入数据	右侧
	m_DataBase.m_LQSG = m_RoadDatabaseArray[RoadIndex].LQSG_R;
	m_DataBase.Write_XLDbs(MdbName, L"右侧沟", RoadName);
}

/*===================================================================
函数名:Read_TG
------------------------------------------------------------
函数功能:读取天沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_TG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左天沟数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左天沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].TG_L = m_DataBase.m_TG;
	else
		return false;

	//从数据库中读取右天沟数据
	if (m_DataBase.Read_XLDbs(MdbName, "右天沟", RoadName))
		m_RoadDatabaseArray[RoadIndex].TG_R = m_DataBase.m_TG;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].TG_L.size() && !m_RoadDatabaseArray[RoadIndex].TG_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_TG
------------------------------------------------------------
函数功能:写入天沟
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190809
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_TG(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].TG_L.size() || !m_RoadDatabaseArray[RoadIndex].TG_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_TG = m_RoadDatabaseArray[RoadIndex].TG_L;
	m_DataBase.Write_XLDbs(MdbName, L"左天沟", RoadName);

	//写入数据	右侧
	m_DataBase.m_TG = m_RoadDatabaseArray[RoadIndex].TG_R;
	m_DataBase.Write_XLDbs(MdbName, L"右天沟", RoadName);
}

/*===================================================================
函数名:Read_RoadLayer
------------------------------------------------------------
函数功能:读取路面分层
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20191017
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_RoadLayer(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左路面分层数据
	if (m_DataBase.Read_XLDbs(MdbName, L"左路面分层表", RoadName))
		m_RoadDatabaseArray[RoadIndex].RoadLayer_L = m_DataBase.m_RoadLayerArray;
	else
		return false;

	//从数据库中读取右路面分层数据
	if (m_DataBase.Read_XLDbs(MdbName, "右路面分层表", RoadName))
		m_RoadDatabaseArray[RoadIndex].RoadLayer_R = m_DataBase.m_RoadLayerArray;
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].RoadLayer_L.size() && !m_RoadDatabaseArray[RoadIndex].RoadLayer_R.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_RoadLayer
------------------------------------------------------------
函数功能:写入路面分层
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20191017
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_RoadLayer(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].RoadLayer_L.size() || !m_RoadDatabaseArray[RoadIndex].RoadLayer_R.size())
		return;

	//写入数据	左侧
	m_DataBase.m_RoadLayerArray = m_RoadDatabaseArray[RoadIndex].RoadLayer_L;
	m_DataBase.Write_XLDbs(MdbName, L"左路面分层表", RoadName);

	//写入数据	右侧
	m_DataBase.m_RoadLayerArray = m_RoadDatabaseArray[RoadIndex].RoadLayer_R;
	m_DataBase.Write_XLDbs(MdbName, L"右路面分层表", RoadName);
}

/*===================================================================
函数名:Read_StationSequence
------------------------------------------------------------
函数功能:读取桩号序列
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:	bool		返回值		true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadDesignWorkingDatabase::Read_StationSequence(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = Is_Database_Exist_And_Create(MdbName, RoadName);

	//从数据库中读取左加宽分段数据
	if (m_DataBase.Read_XLDbs(MdbName, L"桩号序列", RoadName))
	{
		for (int i = 0; i < m_DataBase.NSTA; i++)
			m_RoadDatabaseArray[RoadIndex].StationSequence.push_back(m_DataBase.STA[i]);
	}
	else
		return false;

	if (!m_RoadDatabaseArray[RoadIndex].StationSequence.size())
		return false;
	else
		return true;
}

/*===================================================================
函数名:Write_StationSequence
------------------------------------------------------------
函数功能:写入桩号序列
------------------------------------------------------------
输入:	CString		MdbName		线路方案名
		CString		RoadName	道路名
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadDesignWorkingDatabase::Write_StationSequence(CString MdbName, CString RoadName)
{
	//检测数据库是否存在
	int RoadIndex = -1;
	if (!Is_Database_Exist(MdbName, RoadName, RoadIndex))
		return;

	//检测数据是否存在
	if (!m_RoadDatabaseArray[RoadIndex].StationSequence.size())
		return;

	//设置数据库数据
	m_DataBase.NSTA = m_RoadDatabaseArray[RoadIndex].StationSequence.size();
	if (m_DataBase.STA)
	{
		delete[]m_DataBase.STA;
		m_DataBase.STA = NULL;
	}
	m_DataBase.STA = new LCBdata[m_DataBase.NSTA];

	//写入数据
	for (int i = 0; i < m_DataBase.NSTA; i++)
		m_DataBase.STA[i] = m_RoadDatabaseArray[RoadIndex].StationSequence[i];
	m_DataBase.Write_XLDbs(MdbName, L"桩号序列", RoadName);
}






