// BCARC.h: interface for the BCARC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCARC_H__209CA20D_3A99_4F8E_A7BC_9FFBB122D209__INCLUDED_)
#define AFX_BCARC_H__209CA20D_3A99_4F8E_A7BC_9FFBB122D209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.H"
class BCARC : public BAS_DRAW_FUN 
{
public:
	void EraseArc();
	BCARC();
	virtual ~BCARC();
//protected:
	double R;//�뾶
	double N0,E0;//���е�����
	double OutN,OutE;//���е���������һ������
	double CenN,CenE;//Բ������
	double startAngle,endAngle;//���սǶ�
	bool OK1,OK2,OK3;
	AcDbArc *pArc;
	AcDbObjectId eId;
	double Len;
public:
	//���û����ĵ�����
	void SetMidPoint(double n0,double e0);
	//���ð뾶
	virtual void SetRadius(double r);
	//���û��е���������һ������
	void SetOutPoint(double outn,double oute);
	virtual void UpdateData();
};

#endif // !defined(AFX_BCARC_H__209CA20D_3A99_4F8E_A7BC_9FFBB122D209__INCLUDED_)
