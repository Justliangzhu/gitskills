// BCCreateXLEntity.h: interface for the BCCreateXLEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCCREATEXLENTITY_H__388117D9_F66C_4000_9F92_B1D1E0EAE173__INCLUDED_)
#define AFX_BCCREATEXLENTITY_H__388117D9_F66C_4000_9F92_B1D1E0EAE173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JD_CENTER.h"

class BCCreateXLEntity : public BAS_DRAW_FUN  
{
public:
	//����
	void UpdateData(int mode=1);
	//����Բ�����꣬�뾶��Բ����Բ�����յ�������X��ļн�
	void SetARC(double cn,double ce,double r,double startangle,double endangle);
	void SetARC_A(double cn, double ce, double r,double startangle, double endangle,double a1,double a2);
		
	BCCreateXLEntity();
	virtual ~BCCreateXLEntity();
protected:
	double DV;
	//��������
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	//Բ�����յ�
	double N1,E1,N2,E2;
	//�뾶��Բ������
	double CenN,CenE,R;
	bool OK1;
	//Բ�������յ�������X��ļн�
	double startAngle,endAngle;
	//ǰ���ֱ�߳�
	double JL1,JL2;
	//ǰ�󻺺����߳�
	//double Lo1,Lo2;
	double A1,A2;
public:
	JD_CENTER *pGtzx;
	AcDbObjectId eId;
};

#endif // !defined(AFX_BCCREATEXLENTITY_H__388117D9_F66C_4000_9F92_B1D1E0EAE173__INCLUDED_)
