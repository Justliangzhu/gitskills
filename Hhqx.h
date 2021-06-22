// Hhqx.h: interface for the CHhqx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HHQX_H__322E7E84_39AA_4F7C_A28B_318935D84970__INCLUDED_)
#define AFX_HHQX_H__322E7E84_39AA_4F7C_A28B_318935D84970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicFun.h"




class CHhqx : public CBasicFun  
{
public:
	
	double m_StartN;		//���N����
	double m_StartE;		//���E����
	double m_StartFwj;		//��㷽λ��
	double m_StartL;		//��������յ㳤��
	double m_StartR;		//���뾶

	double m_EndN;			//�յ�N����
	double m_EndE;			//�յ�E����
	double m_EndFwj;		//�յ㷽λ��
	double m_EndL;			//�յ������յ㳤��
	double m_EndR;			//�յ�뾶
	
	double m_L0;			//���������ܳ�(�Բ��յ��������������յ�)
	int m_LOrR;          //����ƫ��Ϣ ��ƫΪ-1����ƫΪ+1
	int m_Mod;			//�����������ͱ��0-6��0--���������� 1--�벨���� 2--ȫ������  3--���������  4--�ߴ������� 5--�ߴ�����ʽ 6-- S�� 


	double m_BaseN;			//������յ�N����
	double m_BaseE;			//������յ�E����
	double m_BaseFwj;		//������յ㷽λ��

	

								
	
/***********************���캯��***************************************************/
	CHhqx();
	CHhqx(double dStartN,double dStartE,double dStartFwj,double dL0,
		   double dStartR,double dEndR,int dLOrR,int iMod);
	
	virtual ~CHhqx();

	/* ���뻺�����ߵ���ز����������������ߣ�����㿪ʼ��*/
	void Set(double dStartN,double dStartE,double dStartFwj,double dL0,
		   double dStartR,double dEndR,int iLOrR,int iMod);

	/* ���뻺�����ߵ���ز����������������ߣ����յ㿪ʼ��*/
	void SetByEnd(double dEndN,double dEndE,double dEndFwj,double dL0,
			double dEndR,double dStartR,int iLOrR,int iMod);



/***********************�������㺯��***************************************************/
	
    /*���������յ�ĳ���dLToBasePt������߽�*/
	double CalQxj(double dLToBasePt)const;

    /*���������յ�ĳ���dLToBasePt��ĺ��,����Chebftjf���ֺ���*/
	double CalX(double dLToBasePt)const;
	/*���������յ�ĳ���dLToBasePt����ݾ࣬����Chebftjf���ֺ���*/
	double CalY(double dLToBasePt)const;
	/*���ڼ��㺯����[a,x]�����ϵĻ���*/
	    double Chebftjf(double a,double b,double x,int ixory)const;
	    /*���б�ѩ�������㺯���ڸ�����x����ֵ*/
	         double Chebev(double a,double b,double c[],int m,double x)const;
	        /*���ڼ��㺯���������ֵ��б�ѩ��ƽ�������ϵ������ѡ����ֳ���ʹ����������a����ֵΪ0*/		
	             void Chint(double cint1[],double a,double b,double c[],int n)const;
				 /*���ڼ����б�ѩ������ϵ��*/
					 void Chebft(double c[],double a,double b,int n,int ixory)const;
	                //�������յ���ΪdLToBasePt�������ǵ�����������ֵ,Ϊ��������ݾ���׼����
	                      double FunXY(double dLToBasePt,int ixory)const;


	/*����뾶ΪdRl���ĵ������յ�ĳ���dLToBasePt ����CalQulv��RtflspLToBasePt����*/
	double CalLToBasePt(double dRl)const;
    	/*�ú���Ϊ��λ���ⷽ�̣�Ҫ����rtflspLToBasePtFun��������������ʰ뾶ΪRLToBasePtʱ�õ�����յ�����߳���LToBasePt*/
    	double RtflspLToBasePt(double x1,double x2,double xacc,double dRl)const;
	   /*������յ�ĳ���dLToBasePt�������*/
    	double CalQulv(double dLToBasePt)const;
    
	/*���뵽���յ�����߳�dLToBasePt����ȡ�õ�Ĵ������*/
	bool   GetPoint(double &Np,double &Ep,double dLToBasePt)const;
	/*���뵽���յ�����߳�dLToBasePt����ȡ�õ�����귽λ��*/
	double GetFwj(double dLToBasePt)const;
	/*��λ���������㻺�����������߷�λ��ΪdFwj��������յ�ĳ���dLToBasePt*/
	double RtflspGetFwj(double x1,double x2,double xacc,double dFwj)const;

/***********************��ͶӰ��������㺯��***************************************************/

	/*���㻺��������һ�㣨Nout,Eout���ڻ��������ϵ�ͶӰ�������ͶӰ�㵽���յ�����߳�*/
	bool GetPointTy(double &dNp1,double &dEp1,double &dLtoBasePt1,double &dNp2,double &dEp2,double &dLtoBasePt2,int &iNjd,double dNout,double dEout);
     	    /*�ƽ�ָ�����Ѱ��-Getafa��������Сֵ����뻺�����߳�xmin;*/
	         double GoldenGetAfa(double& xmin, double ax, double bx, double cx, double tol, double dNout,double dEout);
		    /*��λ�����Getafa-pi/2=0ʱ��dLtoBasePt*/
	         double RtflspGetafa(double &dNp,double &dEp,double x1,double x2,double xacc,double dNout,double dEout);
		     /*���㻺��(dNp,dEp)�뻺��������һ��(dNout,dEout)���ɵ�ֱ����(dNp,dEp)����������γɵļн�(0-pi֮��)*/
	         double Getafa(double &dNp,double &dEp,double dLtoBasePt,double dNout,double dEout);
			 /*���㻺�����������Ͼ�����յ����߳�ΪdLToBasePt�ĵ�����(dNout,dEout)����*/
			 double DisPtToHhqx(double dLToBasePt,double dNout,double dEout);
			 /*�ƽ�ָ�����CHhqx::GoldenDisPtToHhqx�����ļ�Сֵ */
			 double GoldenDisPtToHhqx(double& xmin, double ax, double bx, double cx, double tol, double dNout,double dEout);
			 /*�����(dNout,dEout)���������ߵ���̾���*/
			 double MinDisPtToHhqx(double &dLToBasePt,double dNout,double dEout);


/***********************�󽻼��㺯��***************************************************/
	/*��ֱ�߶��뻺�����ߵĽ���*/    
	bool GetJdZxHhqx(double &dNp1,double &dEp1,double &dLtoBasePt1,double &dNp2,double &dEp2,double &dLtoBasePt2,int &iNjd,double dNstar,double dEstar,double dNend,double dEend)const;
	/*��ֱ�߶��뻺�����ߵ�һ������*/
	bool GetJd1ZxHhqx(double &dNp1,double &dEp1,double &dLtoBasePt1,double dNstart,double dEstart,double dNend,double dEend,int iFromwhere)const;

	/*�󻺺�������Բ���Ľ�������*/
	bool GetJdArcHhqx(double &dNp1,double &dEp1,double &dNp2,double &dEp2,int &iNjd,double dNcircle,double dEcircle,double dRcircle,double dAngleStart,double dAngleEnd)const;
	
	/*�����������ߵĽ�������*/
	bool GetJdHhqxHhqxHuitou(CArray<Jd,Jd> &HHJd,CHhqx Hhqx1,CHhqx Hhqx2);
	bool GetJdHhqxHhqx(double &dN1,double &dE1,double &dLtoBasePt1,double &dN2,double &dE2,double &dLtoBasePt2,int &iNjd,const CHhqx Hhqx1,const CHhqx Hhqx2,double dHhqx1QjStartL,double dHhqx1QjEndL,double dHhqx2QjStartL,double dHhqx2QjEndL)const;
    	/*�Ե�һ�������������Ϊԭ�㣬��һ���������ߵ�������߷���ΪX�Ὠ��XOY����ϵ������ڶ����������ߺ͵�һ��������������ͬ��X�����µ�Y�����ֵ*/
    	double LtoDeta(const CHhqx Hhqx1,const CHhqx Hhqx2,double dLtoBasePt,int iFromwhere1)const;
    	/*�ú���Ϊ��λ�����LtoDeta����ֵΪ0ʱ��LtoBasePt*/
	    double RtflspLtoDeta(double x1,double x2,double xacc,const CHhqx Sp1,const CHhqx Sp2,int iFromwhere1)const;
    	/*�Ե�һ�������������Ϊԭ�㣬��һ���������ߵ�������߷���ΪX�Ὠ��XOY����ϵ������ڸ�����ϵ������������X��ȡֵ��Χ�Ľ���*/
	    bool GoldenQj(double &al,double &cl,const CHhqx Hhqx1,const CHhqx Hhqx2,double dHhqx1QjStartL,double dHhqx1QjEndL,double dHhqx2QjStartL,double dHhqx2QjEndL,int iFromwhere2)const;
    	/*�ƽ�ָ��������LtoDeta�����ļ�Сֵ�ͼ�Сֵ����Ӧ��dLtoBasePt*/
	    double GoldenLtoDeta(double& xminormin,double ax, double bx, double cx, double tol, int maxormin, const CHhqx Hhqx1,const CHhqx Hhqx2,int iFromwhere1)const;
		double RtflspCalQxj(double x1,double x2,double xacc,double dQxj)const;


};

#endif // !defined(AFX_HHQX_H__322E7E84_39AA_4F7C_A28B_318935D84970__INCLUDED_)
