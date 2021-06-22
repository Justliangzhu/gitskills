// YYQXLink.h: interface for the YYQXLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYQXLINK_H__0A36CD60_6C28_4724_9C9B_3DCE44E01761__INCLUDED_)
#define AFX_YYQXLINK_H__0A36CD60_6C28_4724_9C9B_3DCE44E01761__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SPIRAL.h"
#define S_F 1
#define S_S 2
#define C_F 3
#define C_S 4
class YYQXLink : public BAS_DRAW_FUN  
{
public:
	YYQXLink();
	virtual ~YYQXLink();
public:	
	double GQN1,GQE1,GQN2,GQE2;//ֱԲ/���� Բ/��ֱ��
	SPIRAL pSPIRAL1,pSPIRAL2;
/*
	double PtOnCirN1,PtOnCirE1,PtOnCirN2,PtOnCirE2;////ֱ/��Բ�� Բֱ/����
*/

	double JiaZXLength,FwjJiaZX;//��ֱ�߳�����ֱ�߷�λ��
	void SetCir1(double n,double e,double r);
	void SetCir2(double n,double e,double r);
	void SetA(double a1,double a2);
	void SetQXClass(int qx);
	void CalQX();
	bool ERRFlag;
protected:
	bool OK1,OK2;
	//Բ�ġ��뾶����������
	double CenN1,CenE1,CenN2,CenE2,R1,R2,A1,A2;
	double p1,p2;//������
	double Lo1,Lo2,Xm1,Ym1,Xm2,Ym2;//�������߲���
	double M1,M2;//Բ����ֱ���ϵ�ͶӰ��(��ֱ�����Բ�Ĺ��е�)��ֱ������
	//���߷�ʽ:������������
	//1/2/3/4  //1:S ��ʱ�룻2:S ˳ʱ�� 3:C ��ʱ�룻4:C ˳ʱ��
	int QXClass;
};

#endif // !defined(AFX_YYQXLINK_H__0A36CD60_6C28_4724_9C9B_3DCE44E01761__INCLUDED_)
