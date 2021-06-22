// LLTuLink.h: interface for the LLTuLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LLTULINK_H__2BF8467A_1D03_4A33_B11E_821E6E3E8EC2__INCLUDED_)
#define AFX_LLTULINK_H__2BF8467A_1D03_4A33_B11E_821E6E3E8EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JD_CENTER.h"
//////////////////
//  ͹������    //
//////////////////
class LLTuLink : public BAS_DRAW_FUN  
{
public:
	LLTuLink();
	virtual ~LLTuLink();
	LLTuLink(double n1,double e1,double n0,double e0,double n2,double e2,double a1,double a2);
public:
	void CreateSpiral();		
	void CreateXL();
//	void AppendXL(JD_CENTER *JdCenObj,int jdno);
	void Set(double n1,double e1,double n0,double e0,double n2,double e2,double a1,double a2);
	bool ERRFlag;
	SPIRAL pSPIRAL1,pSPIRAL2;//ǰ�󻺺�����
	double R,A1,A2,T1,T2,Lo1,Lo2;//Բ���߰뾶��ǰ�����󻺣�ǰ�У����У�ǰ�������󻺳�
private:
	double afa,afa1,afa2;//�м佻���ת�ǣ�ǰ��ת�ǣ���ת��
	int LRFlag;//����ת����-1,��+1;
	double fwj1,fwj2;//ǰ��ߵķ�λ�ǣ�1-��0��0-��2
	double N1,E1,N0,E0,N2,E2;//��������
	double Nzh,Ezh,Nhz,Ehz;//ZH�����꣬HZ������
};

#endif // !defined(AFX_LLTULINK_H__2BF8467A_1D03_4A33_B11E_821E6E3E8EC2__INCLUDED_)
