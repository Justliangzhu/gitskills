// BCPPP.h: interface for the BCPPP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCPPP_H__749F300B_1FBE_4C37_A428_AF350893FCE4__INCLUDED_)
#define AFX_BCPPP_H__749F300B_1FBE_4C37_A428_AF350893FCE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class BCPPP : public BAS_DRAW_FUN  
{
public:
	void EraseArc();
	BCPPP();
	virtual ~BCPPP();
//protected:
	//��������(���У���)����
	double NonArc1,EonArc1,NonArc0,EonArc0,NonArc2,EonArc2;
	//�뾶��Բ������
	double CenN,CenE,R;
	//Բ�������յ�������X��ļн�
	double startAngle,endAngle;
	//Բ���뻡�е������X��н�
	double MidAngle;
	//�������������е�//�������
	AcGePoint3dArray PtArray;
	AcDbArc *pArc;
	AcDbObjectId eId;
	bool GetCirByThreePointOnArc(double &r,double &CN,double &CE,double N1,double E1,
		double N2,double E2,double N3,double E3);
	double DV;//��Ƴ���
	double Len;
public:
	//���ӻ�������
	void AppendPointNearArc(double N,double E);
	//����
	void UpdateData();
};

#endif // !defined(AFX_BCPPP_H__749F300B_1FBE_4C37_A428_AF350893FCE4__INCLUDED_)
