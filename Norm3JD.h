// Norm3JD.h: interface for the Norm3JD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NORM3JD_H__3CA589EA_569F_4179_AF9A_7DB1CCC50307__INCLUDED_)
#define AFX_NORM3JD_H__3CA589EA_569F_4179_AF9A_7DB1CCC50307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
//��ͨ�������㷨���߼���ģ�ͣ�����Բ���͡��������������������꣬
//�м佻���Ӧ�����߰뾶��ǰ�󻺺�����Aֵ
class Norm3JD : public BAS_DRAW_FUN  
{
public:
//	Norm3JD();
	Norm3JD(double n1,double e1,double n0,double e0,double n2,double e2,double r,double a1,double a2);
	virtual ~Norm3JD();
private:
	//����������
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	//�м佻���Ӧ�İ뾶����������Aֵ������
	double R,A1,A2,Lo1,Lo2;
public:
	//ǰ��߳�
	double DistJD10,DistJD02;
	//ת��
	double aa;
	//����ת
	int LeftOrRight;
	//ǰ��ߵķ�λ��
	double fb,fe;
	//ǰ���г�
	double T1,T2;
};

#endif // !defined(AFX_NORM3JD_H__3CA589EA_569F_4179_AF9A_7DB1CCC50307__INCLUDED_)
