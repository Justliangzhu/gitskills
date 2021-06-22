// BCTPR.h: interface for the BCTPR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCTPR_H__47530C59_B321_4A26_A850_3FE755668067__INCLUDED_)
#define AFX_BCTPR_H__47530C59_B321_4A26_A850_3FE755668067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class BCTPR : public BAS_DRAW_FUN  
{
public:
	void EraseArc();		
	BCTPR();
	virtual ~BCTPR();
//protected:
	//�뾶
	double R;
	//���߶˵�
	double N1,E1,N2,E2;
	//����һ��
	double NonArc,EonArc;
	//Բ��
	double CenN,CenE;
	double startAngle,endAngle;
	bool OK1,OK2,OK3;
	AcDbArc *pArc;
	AcDbObjectId eId;
	double Len;
public:
	//���ð뾶
	void SetRadius(double r);
	//��������
	void SetQX(double n1,double e1,double n2,double e2);
	//���û���һ��
	void SetPointOnArc(double n,double e);
	//����
	void UpdateData();
};

#endif // !defined(AFX_BCTPR_H__47530C59_B321_4A26_A850_3FE755668067__INCLUDED_)
