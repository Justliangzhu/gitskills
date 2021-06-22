// calcg.cpp: implementation of the calcg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "calcg.h"
#include "hdmdes.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

calcg::calcg(JD_CENTER *pline,double zxcgjbl,double hxcgjbl,int mode,int suphmode)
{
	NZLMK = 0;
	ZLMK = NULL;
	NYLMK = 0;
	YLMK = NULL;
	pm = pline;
	ZXCGJBL = zxcgjbl;//ֱ�߶γ���ʱ���߽�����
	CGHXZXJBL = hxcgjbl;//���Ͷ���С���߽�����1/330
	ZXCGLenMode = mode;//ֱ�߶γ���ʱ�ĳ���ȡ��0-��ȡ������1-1m�������� ,2-5m��������
	SuphMode = suphmode;
}

calcg::~calcg()
{

}

/*-------------------------------------------------------------------------*/
int calcg::indata()
{
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(pm->mdbname,"���߼����׼");
	if (xlmdb.NuphData < 1)
	{
		ads_alert(L"�������ó��߼����׼���ټ��㳬�߼ӿ�ֶ�!");
		return 0;
	}
	NSUPH = xlmdb.NuphData - 1;
	int i;
	for (i = 0; i < NSUPH; i++)
	{
		SuphData[i][0] = xlmdb.SuphData[i][0];
		SuphData[i][1] = xlmdb.SuphData[i][1];
	}

	xlmdb.Read_XLDbs(pm->mdbname,"�ӿ�����׼");
	if (xlmdb.NJK < 1)
	{
		ads_alert(L"�������üӿ�����׼���ټ��㳬�߼ӿ�ֶ�!");
		return 0;
	}
	NJK = xlmdb.NJK - 1;
	for (i = 0; i < NJK; i++)
	{
		JKData[i][0] = xlmdb.JKData[i][0];
		JKData[i][1] = xlmdb.JKData[i][1];
	}

	//3.��ȡ·�������
	xlmdb.Read_XLDbs(pm->mdbname,"��·���ֶα�",pm->RoadName);
	if(xlmdb.NLMK<1)
	{
		ads_alert(L"����������·���ֶ����ݣ��ټ��㳬�߼ӿ�ֶ�!");
		return 0;
	}
	NZLMK = xlmdb.NLMK;
	ZLMK = new LMKdata[NZLMK];
	for (i = 0; i < NZLMK; i++)
		ZLMK[i] = xlmdb.lmk[i];

	xlmdb.Read_XLDbs(pm->mdbname,"��·���ֶα�",pm->RoadName);
	if(xlmdb.NLMK<1)
	{
		ads_alert(L"����������·���ֶ����ݣ��ټ��㳬�߼ӿ�ֶ�!");
		return 0;
	}
	NYLMK = xlmdb.NLMK;
	YLMK = new LMKdata[NYLMK];
	for (i = 0; i < NYLMK; i++)
		YLMK[i] = xlmdb.lmk[i];
	if(xlmdb.lmk) 
		delete []xlmdb.lmk;
	xlmdb.lmk = NULL;

	return 1;
}
//------------------------------------------------------------------
void calcg::freeroom()
{
	if(ZLMK) delete []ZLMK;
	ZLMK = NULL;
	if(YLMK) delete []YLMK;
	YLMK = NULL;
}

//-------------------------------------------------------------
//ȷ���õ��ڷֶ������еĵڼ���,���طֶ����

//*-------------------------------------------------------------------------*/
void calcg::Cal_Suph() //�����ݶ���ͼ���賬������
{
	// TODO: Add your control notification handler code here
	int i;

	JKfactor = 1;
	ZcgNm=0;
	ZjkNm=0;

	//�������г��߻��Ͷ�
	SetSuphData(-1, Zcg, ZcgNm, Zjk, ZjkNm);
	for (i = 1; i < ZcgNm - 1; i++)
	{
		if(fabs(Zcg[i].cg-Zcg[i-1].cg)<0.0001 && fabs(Zcg[i].cg-Zcg[i+1].cg)<0.0001)
		{
			for (int j = i; j < ZcgNm - 1; j++)
				Zcg[j] = Zcg[j+1];
			ZcgNm--;
			i--;
		}
	}


	for(i=1; i<ZjkNm-1; i++)
	{
		if(fabs(Zjk[i].cg-Zjk[i-1].cg)<0.0001 && fabs(Zjk[i].cg-Zjk[i+1].cg)<0.0001)
		{
			for(int j=i; j<ZjkNm-1; j++)
				Zjk[j] = Zjk[j+1];
			ZjkNm--;
			i--;
		}

	}

	//�������г��߻��Ͷ�
	YcgNm=0;
	YjkNm=0;
	SetSuphData(1,Ycg,YcgNm,Yjk,YjkNm);


	for (i = 1; i < YcgNm - 1; i++)
	{
		if (fabs(Ycg[i].cg - Ycg[i - 1].cg) < 0.0001 && fabs(Ycg[i].cg - Ycg[i + 1].cg) < 0.0001)
		{
			for (int j = i; j < YcgNm - 1; j++)
				Ycg[j] = Ycg[j + 1];
			YcgNm--;
			i--;
		}
	}

	for (i = 1; i < YjkNm - 1; i++)
	{
		if (fabs(Yjk[i].cg - Yjk[i - 1].cg) < 0.0001 && fabs(Yjk[i].cg - Yjk[i + 1].cg) < 0.0001)
		{
			for (int j = i; j < YjkNm - 1; j++)
				Yjk[j] = Yjk[j + 1];
			YjkNm--;
			i--;
		}
	}

	freeroom();

		//ads_alert(L"4");

		// ads_printf(L"1:YjkNm=%d\n",YjkNm);




		// ads_printf(L"2:YjkNm=%d\n",YjkNm);
		/*	AcGePoint2d mpt;
		AcGePoint2dArray  pArr;
		for(i=0; i<ZjkNm; i++)
		{
		mpt.x = Zjk[i].cml/10;
		mpt.y = Zjk[i].cg;
		pArr.append(mpt);
		}					
		int NUM=pArr.length();
		AcDbPolyline *pPline=new AcDbPolyline(NUM);
		for(i=0;i<NUM;i++)
		{
		pPline->addVertexAt(i,pArr[i]);
		pPline->setWidthsAt(i,0.0,0.0);
		}	
		AcDbObjectId plId;
		pPline->setColorIndex(1);	
		plId=AddEntityToDbs(pPline);

		pArr.removeSubArray(0,pArr.length()-1);
		for(i=0; i<YjkNm; i++)
		{
		mpt.x = Yjk[i].cml/10;
		mpt.y = Yjk[i].cg;
		pArr.append(mpt);
		}					
		NUM=pArr.length();
		pPline=new AcDbPolyline(NUM);
		for(i=0;i<NUM;i++)
		{
		pPline->addVertexAt(i,pArr[i]);
		pPline->setWidthsAt(i,0.0,0.0);
		}

		pPline->setColorIndex(2);	
		plId=AddEntityToDbs(pPline);*/


		

		//��ʾ������
		//	_tcscpy(mes,"���߼ӿ�������!");
		//	AfxMessageBox(mes);
}

void calcg::SetCirCg(int ZorR,int i, double Supi, struct CgSZ *Cg,int& cgnm)
{
	//   if((ZorR<0&&Fend[i+1][10]>0)||(ZorR>0&&Fend[i+1][10]<0))
	if((ZorR<0&&pm->XYArray[i+1][4]>0)||(ZorR>0&&pm->XYArray[i+1][4]<0))//���
		//left lane right turn or right lane left turn
	{
		Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=Supi;
		cgnm++;
	}
	else   //right lane right turn or left lane left turn���ڲ�
	{
		Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Supi;
		cgnm++;
	}
}

void calcg::SetFirstCirCg(int ZorR, int i,double Ig, struct CgSZ *Cg,int& cgnm)
{
	double Supi;

	Supi = FindSuph(pm->XYArray[i][1]);	 
	if(Supi>0.00001)//�г���
	{
		if(Supi < Ig)
			Supi = Ig;
		if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]>0<0))
			//left lane right turn or right lane left turn ���������
		{
			Cg[cgnm].cml=pm->XYArray[0][4],Cg[cgnm].cg=Supi;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=Supi;
			cgnm++;
		}
		else   //right lane right turn or left lane left turn �������ڲ�
		{
			Cg[cgnm].cml=pm->XYArray[0][4],Cg[cgnm].cg=-Supi;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Supi;
			cgnm++;
		}
	}
	else//no ����
	{
		Cg[cgnm].cml=pm->XYArray[0][4],Cg[cgnm].cg=-Ig;
		cgnm++;  
		Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
		cgnm++;  
	}
}

void calcg::SetSpiralCg(int ZorR, int qorh, double B,double Supi, int i, double Ig, double IgOpp, struct CgSZ *Cg,int& cgnm)
{
	double lc,DeltaI,qml;

	if(Supi < 0.001)
		return;
	DeltaI = Ig + Supi;
	lc = B*DeltaI*0.01*CGHXZXJBL;//��󳬸߻��Ͷγ���


	if(qorh < 0)//the prior of this spiral is circle ��ǰ��
	{
		qml = pm->XYArray[i][6];
		if(lc>=pm->XYArray[i+1][6]-qml)//���߽���������Ҫ��,�������߼����߻��Ͷ�
			Cg[cgnm].cml=pm->XYArray[i+1][6]-(Ig+IgOpp)*(pm->XYArray[i+1][6]-qml)/(DeltaI); 
		else//���������е�Lc����Ϊ���߻��Ͷ�
		{	
			lc = B*(Ig+IgOpp)*0.01*CGHXZXJBL;
			lc=AdjustLc(lc);
			Cg[cgnm].cml=pm->XYArray[i+1][6]-lc;
		}
		if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
			//left lane right turn or right lane left turn
		{
			Cg[cgnm].cg=IgOpp;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
			cgnm++;
		}
		else   //right lane right turn or left lane left turn
		{
			Cg[cgnm].cg=-IgOpp;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
			cgnm++;
		}	    
	}
	else //the next of this spiral is circle ��ǰǰ��
	{
		qml =pm->XYArray[i][6];
		if(lc>=pm->XYArray[i+1][6]-qml)//�������߼����߻��Ͷ�
			Cg[cgnm].cml=qml+(Ig+IgOpp)*(pm->XYArray[i+1][6]-qml)/(DeltaI);
		else//���������е�Lc����Ϊ���߻��Ͷ�
		{	
			lc = B*(Ig+IgOpp)*0.01*CGHXZXJBL;
			lc=AdjustLc(lc);
			Cg[cgnm].cml=qml+lc;	
		}
		if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
			//left lane right turn or right lane left turn
		{
			Cg[cgnm].cg=IgOpp;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=Supi;
			cgnm++;
		}
		else   //right lane right turn or left lane left turn
		{
			Cg[cgnm].cg=-IgOpp;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Supi;
			cgnm++;
		}	    
	}
}

//����������Ͷγ� ���ݹ淶�������߻��Ͷγ�
double calcg::AdjustLc(double lc)
{
	double len;

	len = lc;
	if(ZXCGLenMode==1)//1m����
		len = floor(len);
	else if(ZXCGLenMode==2)//5�ױ���
		len = floor(len/5.0) * 5;
	if(len < 10)
		len = 10;



	return len;
}

void calcg::CalZYLinkOnZX(int i,double Ig,double IgOpp,double B,int ZorR, struct CgSZ *Cg,int &cgnm,struct CgSZ *Jk,int &jknm)
{
	double Supi = FindSuph(pm->XYArray[i+1][1]);
	double Jkval = FindJK(pm->XYArray[i+1][1]);
	bool IfQXWC=true;
	if((ZorR<0&&pm->XYArray[i+1][4]>0)||(ZorR>0&&pm->XYArray[i+1][4]<0))
		IfQXWC = true;
	else
		IfQXWC = false;
	if(Supi>0.00001)//�г���
	{
		if(Supi < Ig)
			Supi = Ig;
		double  DeltaI = Ig + Supi;
		double lc = B*DeltaI*0.01*ZXCGJBL;//���߻��Ͷγ���
		lc=AdjustLc(lc);


		if(!IfQXWC)
		{
			lc = lc - B*(Ig+IgOpp)*0.01*ZXCGJBL;
			lc=AdjustLc(lc);
		}

		if(pm->XYArray[i+1][6]-lc > pm->XYArray[i][6])
		{
			Cg[cgnm].cml=pm->XYArray[i+1][6]-lc,Cg[cgnm].cg=-Ig;
			cgnm++;
		}
		SetCirCg(ZorR,i,Supi,Cg,cgnm);

		if(Jkval>0.001&&!IfQXWC)//�мӿ� �����ڲ�
		{
			Jk[jknm].cml=pm->XYArray[i+1][6]-lc,Jk[jknm].cg=0;
			jknm++;	       
		}
	}
	else //no ����
	{	
		Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
		cgnm++;
		if(Jkval>0.001&&!IfQXWC)//�мӿ� �����ڲ�
		{
			double lc =Jkval*15;//�ӿ���ɶγ���
			if(lc < 20)
				lc=20;
			Jk[jknm].cml=pm->XYArray[i+1][6]-lc,Jk[jknm].cg=0;
			jknm++;	       			
		}
	}
	if(Jkval>0.001&&!IfQXWC)
	{
		Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=Jkval;
		jknm++;	
	}
}

void calcg::CalYZLinkOnZX(int i,double Ig,double IgOpp,double B,int ZorR, struct CgSZ *Cg,int &cgnm,struct CgSZ *Jk,int &jknm)
{
	double	Supi = FindSuph(pm->XYArray[i-1][1]);
	double Jkval = FindJK(pm->XYArray[i-1][1]);
	bool IfQXWC=true;
	if((ZorR<0&&pm->XYArray[i-1][4]>0)||(ZorR>0&&pm->XYArray[i-1][4]<0))//
		IfQXWC = true;
	else
		IfQXWC = false;
	if(Supi>0.00001)//�г���
	{
		if(Supi < Ig)
			Supi = Ig;
		double	    DeltaI = Ig + Supi;
		double      lc = B*DeltaI*0.01*ZXCGJBL;//���߻��Ͷγ���
		lc = AdjustLc(lc);

		if(!IfQXWC)
		{
			lc = lc - B*(Ig+IgOpp)*0.01*ZXCGJBL;
		}

		if(pm->XYArray[i][6]+lc > pm->XYArray[i+1][6])
		{
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
			cgnm++;
		}
		else
		{
			Cg[cgnm].cml=pm->XYArray[i][6]+lc,Cg[cgnm].cg=-Ig;//����һ��ͻ���
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
			cgnm++;
		}

		if(Jkval>0.001&&!IfQXWC)//�мӿ� �����ڲ�
		{
			Jk[jknm].cml=pm->XYArray[i][6]+lc,Jk[jknm].cg=0;
			jknm++;	       
		}
	}
	else //no ����
	{
		Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
		cgnm++;
		if(Jkval>0.001&&ZorR*pm->XYArray[i-1][4]>0.001)//�мӿ� �����ڲ�
		{
			double lc =Jkval*15;//�ӿ���ɶγ���
			if(lc < 20)
				lc=20;
			Jk[jknm].cml=pm->XYArray[i][6]+lc,Jk[jknm].cg=0;
			jknm++;	       			
		}
	}
	if(Jkval>0.001&&!IfQXWC)
	{
		Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
		jknm++;	
	}

}

//ǰ��Բ���޳��� ����ֱ���ϼӿ���ɶ�
void calcg::JKCalYZYLinkOnZX(int i,int ZorR,struct CgSZ *Jk,int &jknm)
{
	double pJkval = FindJK(pm->XYArray[i-1][1]);
	double nJkval = FindJK(pm->XYArray[i+1][1]);
	double plc = pJkval*15;//ǰ�ӿ���ɶγ���
	if(plc < 20 && pJkval > 0.001)
		plc=20;
	double nlc = nJkval*15;//��ӿ���ɶγ���
	if(nlc < 20 && nJkval > 0.001)
		nlc=20;
	if(pJkval>0.001||nJkval>0.001)//ǰ����мӿ�
	{
		if(ZorR*pm->XYArray[i-1][4]>0.001&&ZorR*pm->XYArray[i+1][4]>0.001)//ǰ��Ϊ�����ڲ�
		{
			if(plc+nlc < pm->XYArray[i+1][6]-pm->XYArray[i][6])
			{
				Jk[jknm].cml=pm->XYArray[i][6]+plc,Jk[jknm].cg=0;
				jknm++;
				Jk[jknm].cml=pm->XYArray[i+1][6]-nlc,Jk[jknm].cg=0;
				jknm++;				 
			}
		}
		else if(ZorR*pm->XYArray[i-1][4]>0.001)//ǰ�����ڲ�
		{
			if(plc < pm->XYArray[i+1][6]-pm->XYArray[i][6])	
			{
				Jk[jknm].cml=pm->XYArray[i][6]+plc,Jk[jknm].cg=0;
				jknm++;
			}
		}
		else if(ZorR*pm->XYArray[i+1][4]>0.001)//�������ڲ�
		{
			Jk[jknm].cml=pm->XYArray[i+1][6]-nlc,Jk[jknm].cg=0;
			jknm++;		
		}
		else//ǰ��Ϊ�������
		{  ;
		}
	}
	if(ZorR*pm->XYArray[i+1][4]>0.001)//�������ڲ�
		Jk[jknm].cg=nJkval;
	else//���������
		Jk[jknm].cg = 0;
	Jk[jknm].cml=pm->XYArray[i+1][6];
	jknm++;	
}

void calcg::CalYZYLinkOnZX(int i,double Ig,double IgOpp,double B,int ZorR, struct CgSZ *Cg,int &cgnm,struct CgSZ *Jk,int &jknm)
{
	double pSupi = FindSuph(pm->XYArray[i-1][1]);
	double nSupi = FindSuph(pm->XYArray[i+1][1]);
	double pJkval = FindJK(pm->XYArray[i-1][1]);
	double nJkval = FindJK(pm->XYArray[i+1][1]);
	if(pSupi>0.00001 && nSupi>0.00001)//both�г���
	{
		if(pSupi < Ig)
			pSupi = Ig;
		double DeltaI = Ig + pSupi;
		double plc = B*DeltaI*0.01*ZXCGJBL;//prior���߻��Ͷγ���
		plc = AdjustLc(plc);
		bool IfQXWC = true;
		if((ZorR<0&&pm->XYArray[i-1][4]>0)||(ZorR>0&&pm->XYArray[i-1][4]<0))//
			IfQXWC = true;
		else
			IfQXWC = false;
		if(!IfQXWC)
			plc= plc - B*(Ig+IgOpp)*0.01*ZXCGJBL;

		if(nSupi < Ig)
			nSupi = Ig;
		DeltaI = Ig + nSupi;
		double nlc = B*DeltaI*0.01*ZXCGJBL;//next���߻��Ͷγ���
		nlc = AdjustLc(nlc);
		if((ZorR<0&&pm->XYArray[i+1][4]>0)||(ZorR>0&&pm->XYArray[i+1][4]<0))//
			IfQXWC = true;
		else
			IfQXWC = false;
		if(!IfQXWC)
			nlc= nlc - B*(Ig+IgOpp)*0.01*ZXCGJBL;
		if(plc+nlc <pm->XYArray[i+1][6]-pm->XYArray[i][6])
		{
			Cg[cgnm].cml=pm->XYArray[i][6]+plc,Cg[cgnm].cg=-Ig;
			cgnm++;
			Cg[cgnm].cml=pm->XYArray[i+1][6]-nlc,Cg[cgnm].cg=-Ig;
			cgnm++;
			SetCirCg(ZorR,i,nSupi,Cg,cgnm);
			if(pJkval > 0.001 && !IfQXWC)//�����ڲ�
			{
				Jk[jknm].cml=pm->XYArray[i][6]+plc,Jk[jknm].cg=0;
				jknm++;				
			}
			if(nJkval > 0.001 && !IfQXWC)//�����ڲ�
			{
				Jk[jknm].cml=pm->XYArray[i+1][6]-nlc,Jk[jknm].cg=0;
				jknm++;				
			}
			Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
			jknm++;			
		}
		else//���߻��Ͷγ���sum > line lenth
		{
			SetCirCg(ZorR,i,nSupi,Cg,cgnm);	
			if(!IfQXWC)
				Jk[jknm].cg=nJkval;
			else
				Jk[jknm].cg=0;

			Jk[jknm].cml=pm->XYArray[i+1][6];
			jknm++;		
		}
	}
	else if(pSupi>0.00001 )//prior�г���	
		CalYZLinkOnZX(i,Ig,IgOpp, B, ZorR,Cg,cgnm,Jk,jknm);
	else if(nSupi>0.00001 )//next�г���
		CalZYLinkOnZX(i,Ig,IgOpp, B, ZorR,Cg,cgnm,Jk,jknm);
	else//no����
	{
		Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
		cgnm++;
		JKCalYZYLinkOnZX(i,ZorR,Jk,jknm);
	}
}
//-----------------------------------------------------------------------------
//    1.����·�� 2.�޻������� //2000.6.20
//-----------------------------------------------------------------------------
void calcg::SetSuphData(int ZorR, struct CgSZ *Cg,int& cgnm,struct CgSZ *Jk,int& jknm)
{
	int i;
	double B,Vj,cml;
	double Fgdw,Zlydw,Cxdw,Yljw,Tljw,Ig,DeltaI,lc,Supi,nSupi,nlc;
	int ziwid,yiwid;
	double ZIg,YIg,IgOpp,Jkval,nJkval;
	bool IfQXWC=true;//true ����·����������� false �ڲ�


	for(i=1; i<pm->XYNum; i++)
	{
		cml = pm->XYArray[i][4];//Fend[i][1];��Ԫ������
		Vj = pm->DV;

		ziwid = FindIlmk(cml,ZLMK,NZLMK);
		ZIg = ZLMK[ziwid].lmhp;
		yiwid = FindIlmk(cml,YLMK,NYLMK);
		YIg = YLMK[yiwid].lmhp;

		if(ZorR<0) //��೵��
		{
			Fgdw = ZLMK[ziwid].fgdk;		//�ָ�����
			Zlydw = ZLMK[ziwid].lydk;		//·Ե����
			Cxdw = ZLMK[ziwid].cxdk + Zlydw;	//���е��� + ·Ե����
			Yljw = ZLMK[ziwid].yljk;		//Ӳ·���
			Tljw = ZLMK[ziwid].tljk;		//��·���
			B=cal_B(Fgdw,SuphMode,Cxdw,Yljw,Tljw);
			Ig = ZIg;		//��·�����
			IgOpp = YIg;	//��·�����
		}
		else
		{
			Fgdw = YLMK[ziwid].fgdk;
			Zlydw = YLMK[ziwid].lydk;
			Cxdw = YLMK[ziwid].cxdk + Zlydw;
			Yljw = YLMK[ziwid].yljk; 
			Tljw = YLMK[ziwid].tljk; 
			B=cal_B(Fgdw,SuphMode,Cxdw,Yljw,Tljw);
			Ig = YIg;
			IgOpp = ZIg;
		}
		//	if(Cxdw+Yljw > 4)
		//        JKfactor=1 ;
		//	else 
		//		 JKfactor=0.5;
		JKfactor=1 ;

		if(pm->XYArray[i][0]<1.1)//Ϊֱ�߷ֶ�
		{
			if(i==1)//��һ�ֶ�,ֻ�������ֶι�ϵ
			{
				//ֱ�����Ϊ���߷ֶε�
				Cg[cgnm].cml=pm->XYArray[0][4],Cg[cgnm].cg=-Ig;
				cgnm++;

				Jk[jknm].cml=pm->XYArray[0][4],Jk[jknm].cg=0;
				jknm++;
				if(pm->XYNum==2)
				{
					//ֱ���յ����ó��߷ֶε�
					Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
					cgnm++;
					Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
					jknm++;
				}
				else
				{
					if(pm->XYArray[i+1][0]>2.9)//���Ϊ����
					{
						//ֱ���յ����ó��߷ֶε�
						Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
						cgnm++;
						Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
						jknm++;
					}
					else//���Ϊcircle
					{
						CalZYLinkOnZX(i,Ig,IgOpp,B,ZorR,Cg,cgnm,Jk,jknm);
						//		  ads_printf(L"jknum=%d %lf\n",jknm,Jk[jknm-1].cml,Jk[jknm-1].cg);
					}

				}
			}
			else if(i==pm->XYNum-1)//���ֶ�,ֻ������ǰ�ֶι�ϵ
			{
				if(pm->XYArray[i-1][0]>2.9)//prior��Ϊ����
				{
					Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
					cgnm++;
					Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
					jknm++;
				}
				else//prior��Ϊcircle
					CalYZLinkOnZX(i,Ig,IgOpp, B, ZorR,Cg,cgnm,Jk,jknm);
			}
			else//�м�ֶ�,����ǰ���ϵ
			{
				if(pm->XYArray[i+1][6]-pm->XYArray[i][6]> 0.5) //�����ٽ�״̬,line lengh too small won't be dealt
				{
					// if(fabs(pm->XYArray[i-1][0]-4)<0.01 && fabs(pm->XYArray[i-1][0]-3)<0.01)//the prior and next is spiral 
					//��ֱ��
					if(pm->XYArray[i-1][0]>2.9 && pm->XYArray[i+1][0]>2.9)//the prior and next is spiral 
					{
						Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
						cgnm++;
						Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
						jknm++;
					}
					//��ֱԲ
					else if(pm->XYArray[i-1][0]>2.9 && fabs(pm->XYArray[i+1][0]-2)<0.01)//prior is spiral next is circle
						CalZYLinkOnZX(i,Ig,IgOpp,B,ZorR,Cg,cgnm,Jk,jknm);
					//Բֱ��
					else if(fabs(pm->XYArray[i-1][0]-2)<0.01 && pm->XYArray[i+1][0]>2.9)//prior circle next spiral
						CalYZLinkOnZX(i,Ig,IgOpp, B, ZorR,Cg,cgnm,Jk,jknm);
					//ԲֱԲ
					else if(fabs(pm->XYArray[i-1][0]-2)<0.01 && fabs(pm->XYArray[i+1][0]-2)<0.01)
						//prior and next circle
						CalYZYLinkOnZX(i,Ig,IgOpp,B,ZorR, Cg,cgnm,Jk,jknm);
				}
				else // ���е� ��һС��ֱ�� ����Ϊ��
				{
					Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=0.0;
					cgnm++;

					Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0.0;
					jknm++;
				}

			}
		}
		else if(fabs(pm->XYArray[i][0]-3)<0.01)//present segment is ����ǰspiral
		{
			if(i==1)//��һ�ֶ�,ֻ�������ֶι�ϵ
			{
				Cg[cgnm].cml=pm->XYArray[0][4],Cg[cgnm].cg=-Ig;
				cgnm++;

				Jk[jknm].cml=pm->XYArray[0][4],Jk[jknm].cg=0.0;
				jknm++;
			}
			Supi = FindSuph(pm->XYArray[i][2]);	   
			if(fabs(pm->XYArray[i-1][0]-4)<0.01)
				SetSpiralCg(ZorR, 1, B,Supi, i,0, IgOpp, Cg, cgnm);//S������ C������
			else
				SetSpiralCg(ZorR, 1, B,Supi, i,Ig, IgOpp, Cg, cgnm);
			Jkval = FindJK(pm->XYArray[i][2]);
			if(ZorR*pm->XYArray[i][4] > 0.001)//�����ڲ�
				Jk[jknm].cg=Jkval;
			else
				Jk[jknm].cg= 0;
			Jk[jknm].cml=pm->XYArray[i+1][6];
			jknm++;		   
		}
		else if(fabs(pm->XYArray[i][0]-4)<0.01)//present segment is ������spiral
		{
			Supi = FindSuph(pm->XYArray[i][2]);
			Jkval = FindJK(pm->XYArray[i][2]);
			if(i==1)//��һ�ֶ�,ֻ�������ֶι�ϵ
			{
				Cg[cgnm].cml=pm->XYArray[0][4];
				if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
					Cg[cgnm].cg=Supi;
				else
					Cg[cgnm].cg=-Supi;
				cgnm++;

				Jk[jknm].cml=pm->XYArray[0][4];
				if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
					Jk[jknm].cg=Jkval;
				else
					Jk[jknm].cg= 0;
				jknm++;
			}  
			if(fabs(pm->XYArray[i+1][0]-3)<0.01)
				SetSpiralCg(ZorR, -1, B,Supi, i,0, IgOpp, Cg, cgnm);//S������ C������
			else
				SetSpiralCg(ZorR, -1, B,Supi, i,Ig, IgOpp, Cg, cgnm);

			Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0.0;
			jknm++;	   
		}
		else if(pm->XYArray[i][0]>4.9)//present segment is ������spiral
		{
			if(i==1)//��һ�ֶ�,ֻ�������ֶι�ϵ
			{
				if(fabs(pm->XYArray[i][0]-5)<0.01)
				{
					Supi = FindSuph(pm->XYArray[i][2]);
					Jkval = FindJK(pm->XYArray[i][2]);
				}
				else
				{
					Supi = FindSuph(pm->XYArray[i][3]);
					Jkval = FindJK(pm->XYArray[i][3]);
				}
				Cg[cgnm].cml=pm->XYArray[0][4];
				if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
					Cg[cgnm].cg=Supi;
				else
					Cg[cgnm].cg=-Supi;
				cgnm++;

				Jk[jknm].cml=pm->XYArray[0][4];
				if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
					Jk[jknm].cg=Jkval;
				else
					Jk[jknm].cg= 0;
				jknm++;		  
			}
			Supi = FindSuph(pm->XYArray[i+1][1]);
			Jkval = FindJK(pm->XYArray[i+1][1]);
			SetCirCg(ZorR,  i,  Supi, Cg, cgnm);
			if(ZorR*pm->XYArray[i][4] > 0.001)//�����ڲ�
				Jk[jknm].cg=Jkval;
			else
				Jk[jknm].cg= 0;
			Jk[jknm].cml=pm->XYArray[i+1][6];
			jknm++;		   	   
		}
		else //present segment is circle
		{
			if(i==1)//��һ�ֶ�,ֻ�������ֶι�ϵ
			{
				SetFirstCirCg(ZorR, i,Ig, Cg, cgnm);
				Jkval = FindJK(pm->XYArray[i][1]);
				if(ZorR * pm->XYArray[i][4]>0)////�����ڲ�
				{
					Jk[jknm].cml=pm->XYArray[0][4];
					Jk[jknm].cg=Jkval; jknm++;
					Jk[jknm].cml=pm->XYArray[i+1][6];
					Jk[jknm].cg=Jkval; jknm++;
				}
				else//�������
				{
					Jk[jknm].cml=pm->XYArray[0][4];
					Jk[jknm].cg=0; jknm++;
					Jk[jknm].cml=pm->XYArray[i+1][6];
					Jk[jknm].cg=0; jknm++;
				}
			}
			else
			{
				if(fabs(pm->XYArray[i+1][0]-2)<0.01)//the next is circle
				{
					Supi = FindSuph(pm->XYArray[i][1]);
					nSupi = FindSuph(pm->XYArray[i+1][1]);
					Jkval = FindJK(pm->XYArray[i][1]);
					nJkval = FindJK(pm->XYArray[i+1][1]);
					if(Supi > 0.01 && nSupi <0.01)
					{
						Cg[cgnm].cml=pm->XYArray[i+1][6];
						if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
							Cg[cgnm].cg=Supi;
						else
							Cg[cgnm].cg=-Supi;			   
						cgnm++;
						CalYZLinkOnZX(i+1,Ig,IgOpp, B, ZorR,Cg,cgnm,Jk,jknm);
						cgnm--;
						jknm--;
					}
					else if(Supi < 0.01 && nSupi >0.01)
						CalZYLinkOnZX(i,Ig,IgOpp, B, ZorR,Cg,cgnm,Jk,jknm);
					else if(Supi < 0.01 && nSupi <0.01)
					{
						Cg[cgnm].cml=pm->XYArray[i+1][6];
						Cg[cgnm].cg=-Ig;			   
						cgnm++;
						if(ZorR*pm->XYArray[i][4]>0.001)////�����ڲ�
						{
							Jk[jknm].cml=pm->XYArray[i+1][6];
							Jk[jknm].cg=nJkval; jknm++;
						}
						else
						{
							Jk[jknm].cml=pm->XYArray[i+1][6];
							Jk[jknm].cg=0; jknm++;
						}
					}
					else
					{
						if(Supi < Ig)
							Supi = Ig;
						if(nSupi < Ig)
							nSupi = Ig;
						if(pm->XYArray[i][4]*pm->XYArray[i+1][4]>0.0)//ͬ��
						{
							if(fabs(Supi-nSupi)<0.01)//���²�
							{
								Cg[cgnm].cml=pm->XYArray[i+1][6];
								if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
									Cg[cgnm].cg=Supi;
								else
									Cg[cgnm].cg=-Supi;			   
								cgnm++;

								if(fabs(Jkval-nJkval)>0.001)//�мӿ��
								{
									double lc =fabs(Jkval-nJkval)*15;//�ӿ���ɶγ���
									if(lc < 20)
										lc=20;
									if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
									{
										Jk[jknm].cml=pm->XYArray[i+1][6]-lc,Jk[jknm].cg=Jkval;
										jknm++;
									}
								}
								Jk[jknm].cml=pm->XYArray[i+1][6];
								if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
									Jk[jknm].cg=nJkval;
								else
									Jk[jknm].cg=0;
								jknm++;						   
							}
							else
							{
								DeltaI = fabs(nSupi-Supi);
								lc = B*DeltaI*0.01*ZXCGJBL;//���߻��Ͷγ���   
								lc=AdjustLc(lc);
								if(pm->XYArray[i][1]>pm->XYArray[i+1][1])//the present circle larger
								{                   
									if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
										//left lane right turn or right lane left turn
									{	                   
										Cg[cgnm].cml=pm->XYArray[i+1][6]-lc,Cg[cgnm].cg=Supi;
										cgnm++;
										Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=nSupi;
										cgnm++;
									}
									else   //right lane right turn or left lane left turn
									{
										Cg[cgnm].cml=pm->XYArray[i+1][6]-lc,Cg[cgnm].cg=-Supi;
										cgnm++;
										Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-nSupi;
										cgnm++;
									}
									if(fabs(Jkval-nJkval)>0.001)//�мӿ��
									{
										if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
										{
											Jk[jknm].cml=pm->XYArray[i+1][6]-lc,Jk[jknm].cg=Jkval;
											jknm++;	 
										}
									}
									Jk[jknm].cml=pm->XYArray[i+1][6];
									if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
										Jk[jknm].cg=nJkval;
									else
										Jk[jknm].cg=0;
									jknm++;
								}
								else
								{             
									if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
										//left lane right turn or right lane left turn
									{	                   
										Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=Supi;
										cgnm++;
										Cg[cgnm].cml=pm->XYArray[i+1][6]+lc,Cg[cgnm].cg=nSupi;
										cgnm++;
									}
									else   //right lane right turn or left lane left turn
									{
										Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Supi;
										cgnm++;
										Cg[cgnm].cml=pm->XYArray[i+1][6]+lc,Cg[cgnm].cg=-nSupi;
										cgnm++;
									}
									if(fabs(Jkval-nJkval)>0.001)//�мӿ��
									{
										if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
										{
											Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=Jkval;
											jknm++;
											Jk[jknm].cml=pm->XYArray[i+1][6]+lc,Jk[jknm].cg=nJkval;
											jknm++;	 
										}
									}
								}
							}
						}
						else//��Բ����
						{
							DeltaI = Supi;
							lc = B*Supi*0.01*ZXCGJBL;//���߻��Ͷγ���  
							lc=AdjustLc(lc);
							nlc = B*nSupi*0.01*ZXCGJBL;//���߻��Ͷγ���  
							nlc=AdjustLc(nlc);
							if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
								//left lane right turn or right lane left turn
							{	                   
								Cg[cgnm].cml=pm->XYArray[i+1][6]-lc,Cg[cgnm].cg=Supi;
								cgnm++;
								Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=0;
								cgnm++;
								Cg[cgnm].cml=pm->XYArray[i+1][6]+nlc,Cg[cgnm].cg=-nSupi;
								cgnm++;
							}
							else   //right lane right turn or left lane left turn
							{
								Cg[cgnm].cml=pm->XYArray[i+1][6]-lc,Cg[cgnm].cg=-Supi;
								cgnm++;
								Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=0;
								cgnm++;
								Cg[cgnm].cml=pm->XYArray[i+1][6]+nlc,Cg[cgnm].cg=nSupi;
								cgnm++;
							}
							//					   if(fabs(Jkval-nJkval)>0.001)//�мӿ��
							//					   {
							if(ZorR*pm->XYArray[i][4]>0.001)//��ǰ�����ڲ� ��Բ�������
							{
								Jk[jknm].cml=pm->XYArray[i+1][6]-lc,Jk[jknm].cg=Jkval;
								jknm++;
								Jk[jknm].cml=pm->XYArray[i+1][6],Jk[jknm].cg=0;
								jknm++;	 
							}
							else//��ǰ������� ��Բ�����ڲ�
							{
								Jk[jknm].cml=pm->XYArray[i+1][6];				
								Jk[jknm].cg=0;
								jknm++;
								Jk[jknm].cml=pm->XYArray[i+1][6]+nlc,Jk[jknm].cg=nJkval;
								jknm++;	 
							}
							/*
							}
							else
							{
							Jk[jknm].cml=pm->XYArray[i+1][6];
							if(ZorR*pm->XYArray[i][4]>0)//�����ڲ�
							Jk[jknm].cg=nJkval;
							else
							Jk[jknm].cg=0;
							jknm++;
							}*/

						}
					}        
				}
				else//��һ��Ԫ��Բ
				{
					Supi = FindSuph(pm->XYArray[i][1]);
					if(Supi>0.00001)//�г���
					{
						if(Supi < Ig)
							Supi = Ig;
						if((ZorR<0&&pm->XYArray[i][4]>0)||(ZorR>0&&pm->XYArray[i][4]<0))
							//left lane right turn or right lane left turn ���������
						{
							Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=Supi;
							cgnm++;
						}
						else   //right lane right turn or left lane left turn �������ڲ�
						{
							Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Supi;
							cgnm++;
						}
					}
					else
					{
						Cg[cgnm].cml=pm->XYArray[i+1][6],Cg[cgnm].cg=-Ig;
						cgnm++;  
					}
					Jkval = FindJK(pm->XYArray[i][1]);
					Jk[jknm].cml=pm->XYArray[i+1][6];
					if(ZorR*pm->XYArray[i][4]>0.001)//�����ڲ�
						Jk[jknm].cg=Jkval;
					else
						Jk[jknm].cg=0;
					jknm++;
				}
			}
		}
	}
	CheckIfMlOver(Cg,cgnm);
	CheckIfMlOver(Jk,jknm);

}

//-----------------------------------------------------------
int calcg::FindIlmk(double cml,LMKdata lmk[],int nlmk)
{
	int i;
	double ml;

	for(i=0; i<nlmk; i++)
	{
		ml = lmk[i].xlc;//�ֶ����ֳ����
		ml = pm->TYLC(ml);
		if(cml<ml || fabs(ml-cml)<0.001)
			//������̴��ڵ�I���ֶ���
				return i;
	}

	return i-1;
}

//-----------------------------------------------------------------------------
//���ݰ뾶�����ٲ���ӿ�ֵ
//-----------------------------------------------------------------------------
double calcg::FindJK(double R)
{
	int i;
	double jk=0.0;

	if(NJK<1)
		return 0.0;//����ӿ�


	for(i=1; i<NJK; i++)
	{
		if(R>JKData[i][0]-0.1)
		{
			jk = JKData[i-1][1];
			break;
		}
	}

	if(R<JKData[NJK-1][0])	
		jk = JKData[NJK-1][1];
	if(R>=JKData[0][0])
		jk = 0.0;//����ӿ�
	//	ads_printf(L"i=%d R=%lf Jk=%lf\n",i,R,jk);
	return jk*JKfactor;

}
//-----------------------------------------------------------------------------
//���ݰ뾶�����ٲ������ֵ
//-----------------------------------------------------------------------------
double calcg::FindSuph(double R)
{
	int i;
	double cg=0.0;

	if(NSUPH<1)
		return 0.0;//���賬��

	for(i=1; i<NSUPH; i++)
	{
		if(R>SuphData[i][0]-0.1)
		{
			cg = SuphData[i-1][1];
			break;
		}
	}

	if(R<SuphData[NSUPH-1][0])
	{
		ads_printf(L"����: R-%0.1lfС��ƽ������С�뾶�����ߺ��°����ֵ���!\n",R);
		cg = SuphData[NSUPH-1][1];
	}
	if(R>=SuphData[0][0])
		cg = 0.0;//���賬��

	//	ads_printf(L"i=%d R=%lf cg=%lf\n",i,R,cg);
	return cg;

}

/**--------------------------------------------------------------------*/
//   suph_mode ���߷�ʽ ���м�ָ��� 1--���ڱ�Ե��ת 2---��������ת 3---�����Ե��ת
//      ���м�ָ��� 4--������ָ�����Ե��ת 5---���м��������ת 6---�Ƹ����г���������ת
/*--------------------------------------------------------------------*/
double calcg::cal_B(double Zjdw , int suph_mode , double Cxdw , double Yljw , double Tljw)
{
	double b;

	if(suph_mode==4 || suph_mode==5)// ����,���Ե��ת,bΪ����·����
		b = (Cxdw)*2  ;  
	if(suph_mode==3)// ��������ת 
		b = Cxdw ;  //bΪ���·���� 
	if(suph_mode==0)//������ָ�����Ե��ת 
		b = Cxdw ;  
	if(suph_mode==1)//���м��������ת
		b = Cxdw+ Zjdw/2.0 ;  //bΪ���·���� 
	if(suph_mode==2)//�Ƹ����г���������ת
		b = Cxdw/2.0 ;  //bΪ���·���� 

	return b;
}

/*--------------------------------------------------------------*/
//ͬ��:��������·����µ����ѵ�����(��ʼ-�Ƕ�)
void calcg::AdjustCGByZX(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[])
{
	xlpoint PZ;
	double zxsml,zxeml,zxlmhp,dtmp;
	int LorR;
	XLDataBase zxmdb;
	HdmDes hdm;
	AcGePoint3dArray ptarr,ptarr1;
	AcGePoint3d spt,ept,pt;
	double zdcml;
	zdcml = pm->TYLC(ZDdml);
	if(pm->DLArray[0].ELC < zdcml)
	{
		PZ.lc = zxpm->TYLC(ZXdml);
		zxeml = PZ.lc;
		zxpm->xlpoint_pz(&PZ);
		LorR = zxpm->Judge2XLorR(PZ,pm->XYArray,pm->XYNum);
		if(LorR<0)
			//��������೬��
				zxmdb.Read_XLDbs(zxpm->mdbname,"�󳬸߷ֶα�",zxpm->RoadName);		
		else
			zxmdb.Read_XLDbs(zxpm->mdbname,"�ҳ��߷ֶα�",zxpm->RoadName);		
		if(zxmdb.NCG>0)
		{
			hdm.CalLMHP(PZ.lc,zxmdb.NCG,zxmdb.CG,zxlmhp,dtmp);
			//2.����ZDdml��Ӧ���ߺ���
			ept.x = ZDdml, ept.y=-zxlmhp;
			//3.�����ѵ����sml��Ӧ���ߺ���
			PZ.lc = zxpm->PROJ_ML(pm->JdCenArray[0].N,pm->JdCenArray[0].E);
			zxsml = PZ.lc;
			hdm.CalLMHP(PZ.lc,zxmdb.NCG,zxmdb.CG,zxlmhp,dtmp);
			spt.x = pm->DLArray[0].ELC, spt.y=-zxlmhp;
			ptarr.append(spt);
			//4.��ȡ���ߴ�sml-zddml�ĳ��߷ֶ�����
			double zdcml,zddml;
			int i;
			for(i=0; i<zxmdb.NCG; i++)
			{
				double cml = zxpm->TYLC(zxmdb.CG[i].xlc);
				if(cml>zxsml && cml<zxeml)
				{
					PZ.lc = cml;
					zxpm->xlpoint_pz(&PZ);
					zdcml = pm->PROJ_ML(PZ.x,PZ.y);
					zddml = pm->XLC1(zdcml);
					pt.x = zddml, pt.y = -zxmdb.CG[i].cg;
					ptarr.append(pt);
				}
				else if(cml>zxeml)
					break;											
			}
			ptarr.append(ept);
			for(i=0; i<ptarr.length(); i++)
			{
				pt.x = ptarr[i].x , pt.y = -ptarr[i].y;
				ptarr1.append(pt);
			}

			if(LorR > 0)
			{
				double eml;
				eml = pm->TYLC(ptarr[ptarr.length()-1].x);
				//5.������ǰ�󳬸߷ֶ�

				for(i=0;i<NZCG;i++)
				{ 
					double cml = pm->TYLC(ZCG[i].xlc);
					if(cml>eml)
					{
						pt.x = ZCG[i].xlc, pt.y = ZCG[i].cg;
						ptarr.append(pt);
					}
				}
				if(ptarr.length()>0)
				{
					NZCG = ptarr.length();
					for(i=0; i<NZCG; i++)
					{
						ZCG[i].xlc = ptarr[i].x;
						ZCG[i].cg =  ptarr[i].y;
					}		   

				}
				//������ǰ�ҳ��߷ֶ�
				for(i=0;i<NYCG;i++)
				{ 
					double cml = pm->TYLC(YCG[i].xlc);
					if(cml>eml)
					{
						pt.x = YCG[i].xlc, pt.y = YCG[i].cg;
						ptarr1.append(pt);
					}
				}
				if(ptarr1.length()>0)
				{
					NYCG = ptarr1.length();
					for(i=0; i<NYCG; i++)
					{
						YCG[i].xlc = ptarr1[i].x;
						YCG[i].cg =  ptarr1[i].y;
					}		   				
				}
			}
			else
			{
				double eml;
				eml = pm->TYLC(ptarr[ptarr.length()-1].x);
				//5.������ǰ�󳬸߷ֶ�

				for(i=0;i<NYCG;i++)
				{ 
					double cml = pm->TYLC(YCG[i].xlc);
					if(cml>eml)
					{
						pt.x = YCG[i].xlc, pt.y = YCG[i].cg;
						ptarr.append(pt);
					}
				}
				if(ptarr.length()>0)
				{
					NYCG = ptarr.length();
					for(i=0; i<NYCG; i++)
					{
						YCG[i].xlc = ptarr[i].x;
						YCG[i].cg =  ptarr[i].y;
					}		   

				}
				//������ǰ�ҳ��߷ֶ�
				for(i=0;i<NZCG;i++)
				{ 
					double cml = pm->TYLC(ZCG[i].xlc);
					if(cml>eml)
					{
						pt.x = ZCG[i].xlc, pt.y = ZCG[i].cg;
						ptarr1.append(pt);
					}
				}
				if(ptarr1.length()>0)
				{
					NZCG = ptarr1.length();
					for(i=0; i<NZCG; i++)
					{
						ZCG[i].xlc = ptarr1[i].x;
						ZCG[i].cg =  ptarr1[i].y;
					}		   				
				}
			}

		}
		else
		{
			ads_printf(L"���ȼ���%s-%s���߷ֶ�����!\n",zxpm->mdbname,zxpm->RoadName);
			return;
		}
	}

}

//ͬ��:��������·����µ����ѵ�����(�Ƕ�-��ֹ)
void calcg::AdjustCGByZX1(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[])
{
	xlpoint PZ;
	double zxsml,zxeml,zxlmhp,dtmp,zdcml;
	int LorR;
	XLDataBase zxmdb;
	HdmDes hdm;
	AcGePoint3dArray ptarr,ptarr1,ptarr2;
	AcGePoint3d spt,ept,pt;

	zdcml = pm->TYLC(ZDdml);

	if(pm->XLLength > zdcml)
	{
		PZ.lc = zxpm->TYLC(ZXdml);
		zxsml = PZ.lc;
		zxpm->xlpoint_pz(&PZ);
		LorR = zxpm->Judge2XLorR(PZ,pm->XYArray,pm->XYNum);
		if(LorR<0)
			//��������೬��
				zxmdb.Read_XLDbs(zxpm->mdbname,"�󳬸߷ֶα�",zxpm->RoadName);		
		else
			zxmdb.Read_XLDbs(zxpm->mdbname,"�ҳ��߷ֶα�",zxpm->RoadName);		
		if(zxmdb.NCG>0)
		{
			hdm.CalLMHP(PZ.lc,zxmdb.NCG,zxmdb.CG,zxlmhp,dtmp);
			//2.����ZDdml��Ӧ���ߺ���
			spt.x = ZDdml, spt.y=-zxlmhp;
			ptarr.append(spt);
			//3.�����ѵ����sml��Ӧ���ߺ���
			PZ.lc = zxpm->PROJ_ML(pm->XYArray[pm->XYNum][8],pm->XYArray[pm->XYNum][9]);
			zxeml = PZ.lc;
			hdm.CalLMHP(PZ.lc,zxmdb.NCG,zxmdb.CG,zxlmhp,dtmp);
			ept.x = pm->DLArray[pm->DLNum-1].BLC, ept.y=-zxlmhp;
			//4.��ȡ���ߴ�sml-zddml�ĳ��߷ֶ�����
			double zdcml,zddml;
			int i;
			for(i=0; i<zxmdb.NCG; i++)
			{
				double cml = zxpm->TYLC(zxmdb.CG[i].xlc);
				if(cml>zxsml && cml<zxeml)
				{
					PZ.lc = cml;
					zxpm->xlpoint_pz(&PZ);
					zdcml = pm->PROJ_ML(PZ.x,PZ.y);
					zddml = pm->XLC1(zdcml);
					pt.x = zddml, pt.y = -zxmdb.CG[i].cg;
					ptarr.append(pt);
				}
				else if(cml>zxeml)
					break;											
			}
			ptarr.append(ept);
			for(i=0; i<ptarr.length(); i++)
			{
				pt.x = ptarr[i].x , pt.y = -ptarr[i].y;
				ptarr1.append(pt);

			}
			if(LorR > 0)
			{
				double sml;
				sml = pm->TYLC(ptarr[0].x);
				//5.������ǰ�󳬸߷ֶ�
				for(i=0;i<NZCG;i++)
				{ 
					double cml = pm->TYLC(ZCG[i].xlc);
					if(cml<sml)
					{
						pt.x = ZCG[i].xlc, pt.y = ZCG[i].cg;
						ptarr2.append(pt);
					}
				}
				for(i=0;i<ptarr.length();i++)
					ptarr2.append(ptarr[i]);
				if(ptarr2.length()>0)
				{
					NZCG = ptarr2.length();
					for(i=0; i<NZCG; i++)
					{
						ZCG[i].xlc = ptarr2[i].x;
						ZCG[i].cg =  ptarr2[i].y;
					}		   

				}
				ptarr2.removeSubArray(0,ptarr2.length()-1);
				//������ǰ�ҳ��߷ֶ�
				for(i=0;i<NYCG;i++)
				{ 
					double cml = pm->TYLC(YCG[i].xlc);
					if(cml<sml)
					{
						pt.x = YCG[i].xlc, pt.y = YCG[i].cg;
						ptarr2.append(pt);
					}
				}
				for(i=0;i<ptarr1.length();i++)
					ptarr2.append(ptarr1[i]);
				if(ptarr2.length()>0)
				{
					NYCG = ptarr2.length();
					for(i=0; i<NYCG; i++)
					{
						YCG[i].xlc = ptarr2[i].x;
						YCG[i].cg =  ptarr2[i].y;
					}		   				
				}
			}
			else
			{
				double sml;
				sml = pm->TYLC(ptarr[0].x);
				//5.������ǰ�󳬸߷ֶ�
				for(i=0;i<NYCG;i++)
				{ 
					double cml = pm->TYLC(YCG[i].xlc);
					if(cml<sml)
					{
						pt.x = YCG[i].xlc, pt.y = YCG[i].cg;
						ptarr2.append(pt);
					}
				}
				for(i=0;i<ptarr.length();i++)
					ptarr2.append(ptarr[i]);
				if(ptarr2.length()>0)
				{
					NYCG = ptarr2.length();
					for(i=0; i<NYCG; i++)
					{
						YCG[i].xlc = ptarr2[i].x;
						YCG[i].cg =  ptarr2[i].y;
					}		   

				}
				ptarr2.removeSubArray(0,ptarr2.length()-1);
				//������ǰ�ҳ��߷ֶ�
				for(i=0;i<NZCG;i++)
				{ 
					double cml = pm->TYLC(ZCG[i].xlc);
					if(cml<sml)
					{
						pt.x = ZCG[i].xlc, pt.y = ZCG[i].cg;
						ptarr2.append(pt);
					}
				}
				for(i=0;i<ptarr1.length();i++)
					ptarr2.append(ptarr1[i]);
				if(ptarr2.length()>0)
				{
					NZCG = ptarr2.length();
					for(i=0; i<NZCG; i++)
					{
						ZCG[i].xlc = ptarr2[i].x;
						ZCG[i].cg =  ptarr2[i].y;
					}		   				
				}

			}

		}
		else
		{
			ads_printf(L"���ȼ���%s-%s���߷ֶ�����!\n",zxpm->mdbname,zxpm->RoadName);
			return;
		}
	}

}

//���㷴��
void calcg::AdjustCGByZX_FX(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,double fldlen,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[])
{

	xlpoint PZ;
	double zxeml,zxlmhp,dtmp;
	int LorR;
	XLDataBase zxmdb;
	HdmDes hdm;
	AcGePoint3dArray ptarr,ptarr1;
	AcGePoint3d spt,ept,pt;
	double zdcml;
	double zdlmhp;
	zdcml = pm->TYLC(ZDdml);
	if(pm->DLArray[0].ELC < zdcml)
	{
		PZ.lc = zxpm->TYLC(ZXdml);
		zxeml = PZ.lc;
		zxpm->xlpoint_pz(&PZ);
		LorR = zxpm->Judge2XLorR(PZ,pm->XYArray,pm->XYNum);
		if(LorR<0)
			//��������೬��
				zxmdb.Read_XLDbs(zxpm->mdbname,"�󳬸߷ֶα�",zxpm->RoadName);		
		else
			zxmdb.Read_XLDbs(zxpm->mdbname,"�ҳ��߷ֶα�",zxpm->RoadName);		
		if(zxmdb.NCG>0)
		{
			hdm.CalLMHP(PZ.lc,zxmdb.NCG,zxmdb.CG,zxlmhp,dtmp);
			//2.����ZDdml��Ӧ���ߺ���
			if(zxlmhp>0.0)
				zdlmhp = 2.0;
			else
				zdlmhp = -2.0;

			int i=0; 
			double cml = pm->TYLC(ZCG[i].xlc);
			while(i<NZCG && cml<zdcml)
			{
				i++;
				cml = pm->TYLC(ZCG[i].xlc);
			}
			if(i>0 && i<NZCG && fabs(pm->TYLC(ZCG[i-1].xlc)-zdcml)<0.001)
			{
				ZCG[i-1].xlc = ZDdml;
				if(LorR>0)
					ZCG[i-1].cg = zdlmhp;
				else
					ZCG[i-1].cg = -zdlmhp;
			}
			else if(i>0)
			{
				for(int j=NZCG; j>i; j--)
					ZCG[j] = ZCG[j-1];
				ZCG[i-1].xlc = ZDdml;
				if(LorR>0)
					ZCG[i-1].cg = zdlmhp;
				else
					ZCG[i-1].cg = -zdlmhp;
				NZCG++;
			}

			cml = pm->TYLC(YCG[i].xlc);
			while(i<NYCG && cml<zdcml)
			{
				i++;
				cml = pm->TYLC(YCG[i].xlc);
			}
			if(i>0 && i<NYCG && fabs(pm->TYLC(YCG[i-1].xlc)-zdcml)<0.001)
			{
				YCG[i-1].xlc = ZDdml;
				if(LorR>0)
					YCG[i-1].cg = -zdlmhp;
				else
					YCG[i-1].cg = zdlmhp;
			}
			else if(i>0)
			{
				for(int j=NYCG; j>i; j--)
					YCG[j] = YCG[j-1];
				YCG[i-1].xlc = ZDdml;
				if(LorR>0)
					YCG[i-1].cg = -zdlmhp;
				else
					YCG[i-1].cg = zdlmhp;
				NYCG++;
			}
			double zdflddml,zdfldcml,zxflddml;
			zdfldcml = zdcml - fldlen;
			zdflddml = pm->XLC1(zdfldcml);
			PZ.lc = zdfldcml;
			pm->xlpoint_pz(&PZ);
			double zxcml;
			zxcml = zxpm->PROJ_ML(PZ.x,PZ.y);
			zxflddml = zxpm->XLC1(zxcml);
			AdjustCGByZX(zxpm,pm,zxflddml,zdflddml,NZCG,ZCG,NYCG,YCG);

		}
	}

}


void calcg::AdjustCGByZX_FX1(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,double fldlen,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[])
{

	xlpoint PZ;
	double zxeml,zxlmhp,dtmp;
	int LorR;
	XLDataBase zxmdb;
	HdmDes hdm;
	AcGePoint3dArray ptarr,ptarr1;
	AcGePoint3d spt,ept,pt;
	double zdcml;
	double zdlmhp;
	zdcml = pm->TYLC(ZDdml);
	if(pm->XLLength > zdcml)
	{
		PZ.lc = zxpm->TYLC(ZXdml);
		zxeml = PZ.lc;
		zxpm->xlpoint_pz(&PZ);
		LorR = zxpm->Judge2XLorR(PZ,pm->XYArray,pm->XYNum);
		if(LorR<0)
			//��������೬��
				zxmdb.Read_XLDbs(zxpm->mdbname,"�󳬸߷ֶα�",zxpm->RoadName);		
		else
			zxmdb.Read_XLDbs(zxpm->mdbname,"�ҳ��߷ֶα�",zxpm->RoadName);		
		if(zxmdb.NCG>0)
		{
			hdm.CalLMHP(PZ.lc,zxmdb.NCG,zxmdb.CG,zxlmhp,dtmp);
			if(zxlmhp>0.0)
				zdlmhp = 2.0;
			else
				zdlmhp = -2.0;

			int i=0; 
			double cml = pm->TYLC(ZCG[i].xlc);
			while(i<NZCG && cml<zdcml)
			{
				i++;
				cml = pm->TYLC(ZCG[i].xlc);
			}
			if(i>0 && i<NZCG && fabs(pm->TYLC(ZCG[i-1].xlc)-zdcml)<0.001)
			{
				ZCG[i-1].xlc = ZDdml;
				if(LorR>0)
					ZCG[i-1].cg = zdlmhp;
				else
					ZCG[i-1].cg = -zdlmhp;
			}
			else if(i>0)
			{
				for(int j=NZCG; j>i; j--)
					ZCG[j] = ZCG[j-1];
				ZCG[i-1].xlc = ZDdml;
				if(LorR>0)
					ZCG[i-1].cg = zdlmhp;
				else
					ZCG[i-1].cg = -zdlmhp;
				NZCG++;
			}

			cml = pm->TYLC(YCG[i].xlc);
			while(i<NYCG && cml<zdcml)
			{
				i++;
				cml = pm->TYLC(YCG[i].xlc);
			}
			if(i>0 && i<NYCG && fabs(pm->TYLC(YCG[i-1].xlc)-zdcml)<0.001)
			{
				YCG[i-1].xlc = ZDdml;
				if(LorR>0)
					YCG[i-1].cg = -zdlmhp;
				else
					YCG[i-1].cg = zdlmhp;
			}
			else if(i>0)
			{
				for(int j=NYCG; j>i; j--)
					YCG[j] = YCG[j-1];
				YCG[i-1].xlc = ZDdml;
				if(LorR>0)
					YCG[i-1].cg = -zdlmhp;
				else
					YCG[i-1].cg = zdlmhp;
				NYCG++;
			}
			double zdflddml,zdfldcml,zxflddml;
			zdfldcml = zdcml + fldlen;
			zdflddml = pm->XLC1(zdfldcml);
			PZ.lc = zdfldcml;
			pm->xlpoint_pz(&PZ);
			double zxcml;
			zxcml = zxpm->PROJ_ML(PZ.x,PZ.y);
			zxflddml = zxpm->XLC1(zxcml);
			AdjustCGByZX1(zxpm,pm,zxflddml,zdflddml,NZCG,ZCG,NYCG,YCG);

		}
	}

}

void calcg::CheckIfMlOver(CgSZ *Cg,int cgnm)
{
	for(int i=0; i<cgnm; i++)
	{
		if(Cg[i].cml < pm->DLArray[0].ELC)
		{
			Cg[i].cml = pm->DLArray[0].ELC;
			Cg[i].xlc = pm->DLArray[0].ELC;
		}

		if(Cg[i].cml > pm->DLArray[0].ELC+pm->XLLength)
		{
			Cg[i].cml = pm->DLArray[0].ELC+pm->XLLength;
			Cg[i].xlc = pm->DLArray[pm->DLNum-1].BLC;
		}
	}
}
