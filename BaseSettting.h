// BaseSettting.h: interface for the BaseSettting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASESETTTING_H__3EC311D4_6251_4F9D_B67F_BA890A30AA4C__INCLUDED_)
#define AFX_BASESETTTING_H__3EC311D4_6251_4F9D_B67F_BA890A30AA4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BaseSettting  
{
public:

	static int LCTextColor;//里程字色
	static int LCLineColor;//里程线色
	static int QXYSTextColor;//曲线要素字色
	static int TZZTextColor;//特征点字色
	static int TZZLineColor;//特征线字色
	static int DLBTextColor;//断链字色
	static int DLBLineColor;//断链线字色
	static int JDLineColor;//交点连线色
	static int JDCirRadius;//交点圆的半径
	static int JDTDFS;//交点拖动方式
	static double JiaDianLMD;//夹点灵敏度
	static int Rstep;//R拖动步长
	static void Read();
	static void Write();

	BaseSettting();
	virtual ~BaseSettting();
};

#endif // !defined(AFX_BASESETTTING_H__3EC311D4_6251_4F9D_B67F_BA890A30AA4C__INCLUDED_)
