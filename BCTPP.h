// BCTPP.h: interface for the BCTPP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCTPP_H__AE8FAD10_4E73_4C4C_BAAD_8F1BC9A4532F__INCLUDED_)
#define AFX_BCTPP_H__AE8FAD10_4E73_4C4C_BAAD_8F1BC9A4532F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class BCTPP : public BAS_DRAW_FUN  
{
public:
	void EraseArc();
	double DV;
	BCTPP();
	virtual ~BCTPP();
//protected:
	//�뾶
	double R;
	//���߶˵�
	double N1,E1,N2,E2;
	//����һ��
	double NonArc1,EonArc1;
	double NonArc2,EonArc2;
	//Բ��
	double CenN,CenE;
	double startAngle,endAngle;
	bool OK1,OK2,OK3;
	AcDbArc *pArc;
	AcDbObjectId eId;
	double Len;
public:
	//��������
	void SetQX(double n1,double e1,double n2,double e2);
	//���û���һ��
	void SetFirstPointOnArc(double n,double e);
	//���û���һ��
	void SetSecondPointOnArc(double n,double e);
	//����
	void UpdateData();
};

#endif // !defined(AFX_BCTPP_H__AE8FAD10_4E73_4C4C_BAAD_8F1BC9A4532F__INCLUDED_)
