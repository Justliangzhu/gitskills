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

	static int LCTextColor;//�����ɫ
	static int LCLineColor;//�����ɫ
	static int QXYSTextColor;//����Ҫ����ɫ
	static int TZZTextColor;//��������ɫ
	static int TZZLineColor;//��������ɫ
	static int DLBTextColor;//������ɫ
	static int DLBLineColor;//��������ɫ
	static int JDLineColor;//��������ɫ
	static int JDCirRadius;//����Բ�İ뾶
	static int JDTDFS;//�����϶���ʽ
	static double JiaDianLMD;//�е�������
	static int Rstep;//R�϶�����
	static void Read();
	static void Write();

	BaseSettting();
	virtual ~BaseSettting();
};

#endif // !defined(AFX_BASESETTTING_H__3EC311D4_6251_4F9D_B67F_BA890A30AA4C__INCLUDED_)
