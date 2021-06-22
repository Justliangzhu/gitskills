// JDLuanLink.h: interface for the JDLuanLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDLUANLINK_H__047ABBC6_50AC_40F4_9037_E43D10A3BC9C__INCLUDED_)
#define AFX_JDLUANLINK_H__047ABBC6_50AC_40F4_9037_E43D10A3BC9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "YYLuanLink.h"
#include "ZYLink.h"
#include "SPIRAL.h"
//�������������㼰��һ������ĺ��г���������
class JDLuanLink : public BAS_DRAW_FUN  
{
public:
	JDLuanLink();
	virtual ~JDLuanLink();
	double GetCenter_dR2(double a);
	double func(double x);
private:
	double PreR,PreN,PreE;//ǰһ������İ뾶������
	double CurR,CurN,CurE;//��ǰ����İ뾶������
	double AftN,AftE;//��һ�����������
	double PreT;//ǰһ������ĺ��г�
	//////////////////////////////////////////////////////////////////////////
	double PreFwj,AftFwj;//ǰһ�����ߵķ�λ��
	int LeftOrRight;//����ת
	bool OKPreJD,OKCurJD,OKAftJD,OKPreT;//�Ƿ������Щ����
	int count;//��������
	double DieDai(double a11,double a12);
public:
	bool ERRFlag;
	//��ǰ�����ӦԲ���ߵ�Բ������
	double CurCenN,CurCenE;
	//��Բ������
	double HYN,HYE;
	//���������г���ǰA����A
	double ArcLength,AftT,PreA,AftA;
	//����ǰһ����������꼰�뾶
	void SetPreJD(double n,double e,double r);
	//���õ�ǰ��������ꡢ�뾶��ǰ�󻺺����ߵ�Aֵ
	void SetCurJD(double n,double e,double r,double a1,double a2);
	//���ú�һ�����������
	void SetAftJD(double n,double e);
	//����ǰһ������ĺ��г�
	void SetPreT(double t);
	//��ʼ���㻺�ͻ�������
	void UpdateData();

	//////////////////////////////////////////////////////////////////////////
	//���������㣬��һ�������㹹�����ͣ�ǰһ���뾶����һ���뾶��ǰA����A�����غ�һ��Բ��Բ��,Բ���߳�	
	static void GetCenter(double JdN1,double JdE1,double JdN0,double JdE0,
		double JdN2,double JdE2,double r1,double r2,double a1,double a2,
		double &cenN,double &cenE,double &l,double &T2);
	void SetPreFwj(double pfwj);
	void SetAftFwj(double pfwj);
};

#endif // !defined(AFX_JDLUANLINK_H__047ABBC6_50AC_40F4_9037_E43D10A3BC9C__INCLUDED_)
