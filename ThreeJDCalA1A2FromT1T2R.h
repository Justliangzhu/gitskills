// ThreeJDCalA1A2FromT1T2R.h: interface for the ThreeJDCalA1A2FromT1T2R class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEJDCALA1A2FROMT1T2R_H__07DAA8D3_6582_421D_98A8_40D3FD0DC851__INCLUDED_)
#define AFX_THREEJDCALA1A2FROMT1T2R_H__07DAA8D3_6582_421D_98A8_40D3FD0DC851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
#include "SPIRAL.h"
#include "ZYLink.h"
//�������������꼰�м佻���Ӧ�����߰뾶���г������㻺�����ߵ�Aֵ
class ThreeJDCalA1A2FromT1T2R : public BAS_DRAW_FUN  
{
public:
	ThreeJDCalA1A2FromT1T2R();
	virtual ~ThreeJDCalA1A2FromT1T2R();
public:
	//����ǰһ�����������
	void SetPreJd(double n,double e);
	//�����м�����������
	void SetMidJd(double n,double e);
	//���ú�һ�����������
	void SetAftJd(double n,double e);
	//�����м佻���Ӧ�����߰뾶��
	void SetRadius(double r);
	//����ǰ���г�
	void SetT1T2(double t1,double t2);

	void UpdateData();//����
	
	//�м佻���Ӧ�Ļ�������Aֵ
	double A1,A2;
	//�Ƿ�������
	bool ERRFlag;
private:
	//����Ƿ�������в���
	bool OK1,OK2,OK3,OK4,OK5;
	//��������
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	
	double T1,T2,R;	

	int LeftOrRight;
	//ֱ��������
	double zhN,zhE;
	//��ֱ������
	double hzN,hzE;
	double fb,fe;//ǰһ���ߵķ�λ��/��һ���ߵ���λ��
	//Բ������
	double CenN,CenE;
	bool DieDai(double a11,double a12);
	int count;
	double func(double A1);
};

#endif // !defined(AFX_THREEJDCALA1A2FROMT1T2R_H__07DAA8D3_6582_421D_98A8_40D3FD0DC851__INCLUDED_)
