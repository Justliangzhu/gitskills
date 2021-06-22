// ThreeJDCalThisAFromThatA_PreT_R.h: interface for the ThreeJDCalThisAFromThatA_PreT_R class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEJDCALTHISAFROMTHATA_PRET_R_H__788D7386_E570_4A84_B3D7_3BDBFDD46E08__INCLUDED_)
#define AFX_THREEJDCALTHISAFROMTHATA_PRET_R_H__788D7386_E570_4A84_B3D7_3BDBFDD46E08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

//���������������ꡢ�м佻���Ӧ�����߰뾶��ǰ�г���
//����֪һ���������ߵ�����¼�����һ����������
class ThreeJDCalThisAFromThatA_PreT_R : public BAS_DRAW_FUN  
{
public:
	//����ת��
	void SetAfa(double aa);
	//����ǰ�г������߰뾶
	void SetPreT_R(double t1,double r);
	//����ǰ�������ߵ�Aֵ
	void SetA1(double a);
	//���ú󻺺����ߵ�Aֵ
	void SetA2(double a);
	//��ѯǰ�������ߵ�Aֵ
	bool GetA1(double &a);
	//��ѯ�󻺺����ߵ�Aֵ
	bool GetA2(double &a);
public:
	ThreeJDCalThisAFromThatA_PreT_R();
	virtual ~ThreeJDCalThisAFromThatA_PreT_R();
private:
	int count;
	double DieDaiA1(double a11,double a12);
	double DieDaiA2(double a21,double a22);
	double T1,T2,afa,R,A1,A2;
	bool OKAfa,OKA1,OKA2,OKPreT;
};

#endif // !defined(AFX_THREEJDCALTHISAFROMTHATA_PRET_R_H__788D7386_E570_4A84_B3D7_3BDBFDD46E08__INCLUDED_)
