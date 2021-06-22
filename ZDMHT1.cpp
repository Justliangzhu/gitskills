// ZDMHT1.cpp: implementation of the CZDMHT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZDMHT1.h"
#include "HdmDes.h"
#include   <direct.h> 
#include "CWorkDir.h"
#include "MyLayout.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#include "CWorkDir.h"
#endif // VERSION_CAD2016
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TABLE_LENGTH	60

#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
	else if ((x) > 0)    \
	y =1;             \
	else                 \
	y = -1;           \
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZDMHT::CZDMHT()
{
	TopFW = 40.0;
	//NTKHt = 277;
	//NTKLen = 380;
	//WTKLen = 420;
	//WTKHt = 297;
	NTKHt = 554;
	NTKLen = 780;
	WTKLen = 840;
	WTKHt = 594;
	m_Frame_Center_X = 780.0 / 2.2;
	m_Frame_Center_Y = 554.0 / 2.0;
	pDBS=pZDBS=pYDBS=NULL;
	m_IsNoteScale = false;
	m_Isbuju_ZDM = 0;
}

CZDMHT::~CZDMHT()
{

}

void CZDMHT::FormSdai()//����Sdai����
{
	int     i;
	double  mlh, mlq, sxstar, begin;

	if(DrawMode==0)
		Xd_ori = X_inori+TABLE_LENGTH;
	else
	{
		if(IsFirst)
			Xd_ori = X_inori + TABLE_LENGTH;
		else
			Xd_ori = X_inori+5;
	}
	NSDAI=0;
	begin = m_Scml;
	sxstar=Xd_ori;

	SDAI[NSDAI][0]=0.0;
	SDAI[NSDAI][1] = sxstar;
	SDAI[NSDAI][2] = m_Scml;
	NSDAI++;
	for (i=1; i<m_pm->DLNum-1; i++)  
	{
		if(m_pm->DLArray[i].TYLC>m_Scml&&m_pm->DLArray[i].TYLC<m_Ecml)
		{
			mlh = fmod(m_pm->DLArray[i].ELC+0.00001 , 1000.0);
			mlq = fmod(m_pm->DLArray[i].BLC+0.00001, 1000.0);
			if(fabs(mlh-mlq)<=0.001)
				SDAI[NSDAI][0]=0.0;
			else
				SDAI[NSDAI][0]=mlh-mlq;
			SDAI[NSDAI][0] = 0.0;
			//			if (SDAI[NSDAI][0]>=100.0) SDAI[i][0]-=100.0;
			//			if (SDAI[NSDAI][0]< 0.0) //����
			//				SDAI[NSDAI][0] = 0.0;
			SDAI[NSDAI][1] = sxstar + (m_pm->DLArray[i].TYLC-begin+SDAI[NSDAI][0])/X_scale;
			SDAI[NSDAI][2] = m_pm->DLArray[i].TYLC;
			sxstar=SDAI[NSDAI][1];
			begin=m_pm->DLArray[i].TYLC;			
			NSDAI++;
		}
	}   /* end for */
	SDAI[NSDAI][0]=0.0;
	SDAI[NSDAI][1] = sxstar + (m_Ecml-begin)/X_scale;
	SDAI[NSDAI][2] = m_Ecml;
	NSDAI++;
}

void CZDMHT::draw_box()
{
	struct Table tab;  	
	int i,j;
	AcGePoint3d spt,ept;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-�����׿�");

	tab.nrow=WXPnum;

	if(WayorRamp==0)//��·					
		for(i=0;i<WXPnum;i++)
			tab.RowH[i]=wxp[i].Lht;
	else//�ѵ�
	{
		for(i=0;i<WXPnum;i++)
		{
			if(wxp[i].wxpinf==1)//��Ƹ߳�
			{
				double DesHt=0;
				for(j=0;j<8;j++)
					if(InfLmPt[j])
						DesHt = DesHt+wxp[i].Lht;
				tab.RowH[i]=DesHt;	
			}
			else
				tab.RowH[i]=wxp[i].Lht;
		}
	}

	tab.ColWid[0] = TABLE_LENGTH;

	if(DrawMode==0)//����
	{
		tab.ncol=1;
		tab.Nstr=WXPnum;
		for(i=0;i<tab.Nstr;i++)
		{
			tab.Str[i].ntext=1;
			tab.Str[i].rownum=i+1,tab.Str[i].colnum=1;
			tab.Str[i].fitinf = 0, tab.Str[i].text = wxp[i].text;
			tab.Str[i].texth = 6.0;	//20190710	�޸ı�����Ĭ���ָ�Ϊ6.0
			if(wxp[i].wxpinf==5)//��·ƽ��
			{
				tab.Str[i].ntext=2;
				tab.Str[i].text = "ֱ �� ��";
				tab.Str[i].text1 = "ƽ �� ��";
			}
		}
	}
	else//ʩ��ͼ
	{
		if(IsFirst)//��һ��ͼ
		{
			//tab.ncol=2;
			tab.ncol = 1;	//20190710�޸�
			//tab.ColWid[0] = 5;
			tab.Nstr = WXPnum;
			for(i=0;i<WXPnum;i++)
			{
				tab.Str[i].ntext=1;
				tab.Str[i].rownum=i+1,tab.Str[i].colnum=1;
				tab.Str[i].fitinf = 0, tab.Str[i].text = wxp[i].text;
				//tab.Str[i].texth = 3.5;
				tab.Str[i].texth = 6.0;		//20190710	�޸ı�����Ĭ���ָ�Ϊ6.0
				//if(wxp[i].wxpinf==5)//��·ƽ��	20190710	��ʱ����
				//	tab.Str[i].fitinf=1;
			}
			tab.LDpt[X] = X_inori, tab.LDpt[Y] = Y_inori;
		}
		else
		{
			tab.ncol = 1;
			tab.ColWid[0] = 22 / X_scale;
			tab.Nstr = WXPnum;
			for (i = 0; i < WXPnum; i++)
			{
				tab.Str[i].ntext = 1;
				tab.Str[i].rownum = i + 1, tab.Str[i].colnum = 1;
				tab.Str[i].fitinf = 0, tab.Str[i].text.Format(L"%d", WXPnum - i);
				tab.Str[i].texth = 6.0;	//20190710	�޸ı�����Ĭ���ָ�Ϊ6.0
			}
			tab.LDpt[X] = X_inori - (22 - 5) / X_scale, tab.LDpt[Y] = Y_inori;
		}
	}
	
	maketable(tab, 3, 0.0, 0.0, LayerName);

	for (i = 0; i < WXPnum; i++)
	{
		spt[X] = Xd_ori, spt[Y] = Y_inori + wxp[i].bottomht;
		ept[X] = SDAI[NSDAI - 1][1];
		ept[Y] = spt[Y];
		makeline(spt, ept, 3, 0.0, LayerName);
	}
	spt[Y] = Y_inori + wxp[WXPnum].bottomht;
	ept[Y] = spt[Y];
	makeline(spt, ept, 3, 0.0, LayerName);


}

/*---------------------------------------------------------------*
*   convert mileage cml to screen coordinate  screen_x                                  *
*---------------------------------------------------------------*/
double CZDMHT::mltosx(int mess, double cml) const
{
	int     i;
	double   sx=-1.0;

	if (cml <= SDAI[0][2])
	{
		sx = SDAI[0][1];
		return (sx);
	}
	if (cml >= SDAI[NSDAI-1][2])
	{
		sx = SDAI[NSDAI-1][1];
		return (sx);
	}
	for (i=0; i<NSDAI;i++)
	{
		if (cml>SDAI[i][2])
			sx = SDAI[i][1]+(cml-SDAI[i][2])/X_scale;
		else if (fabs(cml-SDAI[i][2])<=0.01 && i<NSDAI-1)
		{
			if (mess==0)//���ƶ���ǰ
				sx = SDAI[i][1] - SDAI[i][0]/X_scale;
			else//���ƶ�����
				sx = SDAI[i][1];
		}
		else if (i==NSDAI-1 && fabs(cml-SDAI[i][2])<=0.01)
			sx = SDAI[i][1];
	}
	return (sx);
}

/*---------------------------------------------------------------*
*   convert level  mz to screen coordinate  screen_y            *
*---------------------------------------------------------------*/
double CZDMHT::ztosy(double cml,double mz)        
{
	int     i;
	double  tmp, end, maxht;


	if(cml<BREAK[0][0]-0.1||cml>BREAK[NBREAK-1][0]+0.1)
	{
		//		ads_printf(L"%0.2lf %lf %lf��̳�����Χ\n",cml,BREAK[0][0],BREAK[NBREAK-1][0]);
		return (Y_inori+wxp[WXPnum].bottomht);
	}

	maxht = (NTKHt - (wxp[WXPnum].bottomht + TopFW)) * Y_scale;
	for (i = 0; i < NBREAK; i++)
	{
		end = BREAK[i + 1][0];
		if (cml <= end + 0.001)
		{
			if (mz > BREAK[i][1] + maxht)
				tmp = Y_inori + NTKHt - TopFW;
			else if (mz < BREAK[i][1])
				tmp = Y_inori + wxp[WXPnum].bottomht;
			else
				tmp = Y_inori + wxp[WXPnum].bottomht + (mz - BREAK[i][1]) / Y_scale;

			return (tmp);
		}
	}
}
/*	20190710�޸�	Ϊ�����������������ԭ�����ö���
	1.���������ö��᣺����Ƹ߳���������90%��ߣ���10%��Χ����Ȼ�о��ӵײ�Ҳ�����ۣ���Χ�ڴ���ʱ�������ñ��
	2.�������ö���ʱ��������㻭�������ϣ�������Ƹ߳��е㻭��������
*/
void  CZDMHT::Y_break()//�������
{
	int     i=0, idai=0;
	double  Ymin, Ymax, tmp,ploth,Hfw,dploth;
	double  cml;
	//��ʼ��
	for (i=0; i<100; i++)
	{
		BREAK[i][0] = 0.0;
		BREAK[i][1] = 0.0;
	}
	i = 0;

	ploth = NTKHt-(wxp[WXPnum].bottomht+TopFW);
	//dploth=ploth/2.0;//ʹ�����ռ���м�һ��
	dploth = ploth * 0.9;//ʹ�����ռ���м�һ��
	Hfw = dploth*Y_scale;
	//ads_printf(L"ploh=%lf Y_scale=%lf  Hfw=%lf NTKHt=%lf (wxp[WXPnum].bottomht+TopFW)=%lf\n",ploth,dploth,Hfw,NTKHt,(wxp[WXPnum].bottomht+TopFW));

	cml = m_Scml;//��·������
	NBREAK=0;
	BREAK[NBREAK][0]=cml;
	Ymin=99999, Ymax=-9999;
	while (cml<m_Ecml+0.01) 
	{
		if(pDBS->NPD>0)
			tmp = des_level(cml,pDBS);
		else//���¶�,���ݵ�����
			tmp = CalDmxH(cml);

		if(tmp>0.0)
		{

			if (tmp < Ymin) 
				Ymin = tmp;
			if (tmp > Ymax) 
				Ymax =tmp;
		}

		if (Ymax-Ymin>Hfw)//����߳�����Χ ����һ����
		{
			BREAK[NBREAK][1] = int((Ymin - ploth*Y_scale / 10.0) / 2) * 2;
			NBREAK++;
			BREAK[NBREAK][0]=cml;
			Ymin=Ymax=tmp;
		}
		cml += 1;
	}
	//���һ������	ȡ��Ƹ߳��е���Ϊ��ͼ����
	//BREAK[NBREAK][1] = floor((Ymin - ploth*Y_scale / 10.0) / 2) * 2;		//20190710	��10�޸ĳ�2
	double MidLevel = (Ymin + Ymax) / 2 - ploth * Y_scale * 0.5;
	BREAK[NBREAK][1] = floor(MidLevel / 2) * 2;
	NBREAK++;
	BREAK[NBREAK][0]=m_Ecml;
	NBREAK++;
}

double CZDMHT::CalDmxH(double cml)//���������ݵ�����
{
	int i=0;
	while(i<pDBS->NTW && cml> pDBS->TWB[i].cml)
		i++;
	double dmxh;

	if(i>0)
	{
		if(fabs(pDBS->TWB[i].cml-pDBS->TWB[i-1].cml)>0.001)
			//			dmxh = Dmx[(i-1)*2+1] + (cml-Dmx[(i-1)*2])*(Dmx[i*2+1]-Dmx[(i-1)*2+1])/(Dmx[i*2]-Dmx[(i-1)*2]);
				dmxh = pDBS->TWB[i-1].DMGC + (cml-pDBS->TWB[i-1].cml)*
				(pDBS->TWB[i].DMGC-pDBS->TWB[i-1].DMGC)/(pDBS->TWB[i].cml-pDBS->TWB[i-1].cml);
		else
			dmxh = pDBS->TWB[i-1].DMGC; 
	}
	else
		dmxh = pDBS->TWB[0].DMGC;
	//	ads_printf(L"cml=%lf i=%d dmxh=%lf %lf\n",cml,i,dmxh,Dmx[i*2]);
	return dmxh;
}

/*---------------------------------------------------------*
*    not consider vertical curve  calculate the design level at certain mileage *
*---------------------------------------------------------*/
double CZDMHT::des_level (double cml,XLDataBase *pDBStmp)//�����������߼�����Ƹ߳�
	/* cml; continue mileage  */
{
	int     j = 0;
	double  z = 0.0;

	//20190821	����������֧
	if (fabs(pDBStmp->PDB[0].cml - cml) < 0.01)
		return (pDBStmp->PDB[0].Level);
	else if (fabs(pDBStmp->PDB[pDBStmp->NPD - 1].cml - cml) < 0.01)
		return (pDBStmp->PDB[pDBStmp->NPD - 1].Level);
	else if (cml< pDBStmp->PDB[0].cml-0.1 || cml>pDBStmp->PDB[pDBStmp->NPD-1].cml+0.1) 
	{
		z=-1.0;
	}
	else  
	{
		for(j=1; j<pDBStmp->NPD; j++)
			if (pDBStmp->PDB[j-1].cml<=cml&&cml<=pDBStmp->PDB[j].cml)
			{
				z = pDBStmp->PDB[j-1].Level +(cml-pDBStmp->PDB[j-1].cml)*(pDBStmp->PDB[j].Level-pDBStmp->PDB[j-1].Level)
					/(pDBStmp->PDB[j].cml-pDBStmp->PDB[j-1].cml);
				break;
			}
	}
	return (z);

}

void  CZDMHT::axleCb()//���ƶ��� ����
{
	int i,inf;
	double ydori,ploth,nj,ni,bsx,j,levleft;
	AcGePoint3d spt, ept;
	ACHAR str[50];
	ACHAR LayerN[50];

	//_stprintf(LayerN, L"���%d",CurTNo);
	_stprintf(LayerN, L"����ͼ-���");
	ploth = NTKHt-(wxp[WXPnum].bottomht+TopFW);//��ͼ�����
	ydori = Y_inori+wxp[WXPnum].bottomht;

	nj = floor(ploth/10)-1.0;
	ni = 10.0*Y_scale;
	inf = 1;
	for (i=0; i<NBREAK; i++) 
	{

		if(i==0)//ǰһ��������ʼ����⣬����ʼ��̴�����һ������
		{
			bsx = mltosx(0,m_Scml);
			spt[X]=bsx, spt[Y]=ydori;
			ept[X]=spt[X],ept[Y]=ydori+nj*10;
			makeline(spt,ept,3,0.3,LayerN);
			for (j=ni; j<=nj*ni; j+=ni)
			{
				spt[Y] = ydori+j/Y_scale;
				ept[X] = spt[X]-1.0;
				ept[Y] = spt[Y];
				makeline(spt,ept,3,0,LayerN);
				levleft = BREAK[i][1] + j;		
				ept[X] = spt[X]-2.5;
				ept[Y] = spt[Y]-1.2;
				_stprintf(str, L"%0.0lf",levleft);
				int ys;
				ys = fmod(levleft,10);
				if(ys==0||ys==10)
					maketext(ept,str,0,2.5,3,4, L"HZ",LayerN);
			}
		}
		else if(i==NBREAK-1)//���һ�����᲻��
			break;
		else
		{
			bsx = mltosx(0,BREAK[i][0]);
			spt[X]=bsx, spt[Y]=ydori;
			ept[X]=spt[X],ept[Y]=ydori+nj*10;
			makeline(spt,ept,3,0.3,LayerN);
			for (j=ni; j<=nj*ni; j+=ni)
			{
				spt[Y] = ydori+j/Y_scale;
				ept[X] = spt[X]-1.0;
				ept[Y] = spt[Y];
				makeline(spt,ept,3,0,LayerN);
				levleft = BREAK[i][1] + j;		
				ept[X] = spt[X]-2.5;
				ept[Y] = spt[Y]-1.2;
				_stprintf(str, L"%0.0lf",levleft);
				int ys;
				ys = fmod(levleft,10);
				if(ys==0||ys==10)
					maketext(ept,str,0,2.5,3,4, L"HZ",LayerN);
			}
		}

	}
}

void  CZDMHT::axleSg(double Texth)//����ʩ������
{
	double ZiGao = Texth;
	int i,inf;
	double ydori,ploth,nj,ni,bsx,j,levleft;
	AcGePoint3d spt, ept;
	ACHAR str[50];
	bool drawinf;	
	ACHAR LayerN[50];

	//_stprintf(LayerN, L"���%d",CurTNo);
	_stprintf(LayerN, L"����ͼ-���");
	ploth = NTKHt - (wxp[WXPnum].bottomht + TopFW);//��ͼ�����
	ydori = Y_inori + wxp[WXPnum].bottomht;

	nj = floor(ploth / 10) - 1.0;
	ni = 10.0 * Y_scale;
	inf = 1;
	for (i = 0; i < NBREAK; i++)
	{
		if (i == 0)//��һ����
		{
			//�������	20170710�޸�
			bsx = mltosx(0, m_Scml);
			//2010904������������ƶ�
			bsx -= 20 / X_scale;
			//������
			double One_Length = 1.0 /*/ X_scale*/;
			spt[X] = bsx, spt[Y] = ydori;
			ept[X] = spt[X], ept[Y] = ydori + nj * 10;
			makeline(spt, ept, 3, 0.0, LayerN);
			//������
			spt[X] = bsx - 2 * One_Length;
			ept[X] = spt[X];
			makeline(spt, ept, 3, 0.0, LayerN);
			//��������
			spt[X] = bsx, spt[Y] = ydori + nj * 10;
			ept[X] = bsx - 2 * One_Length, ept[Y] = spt[Y];
			makeline(spt, ept, 3, 0.0, LayerN);
			drawinf = true;
			for (j = 0; j <= nj * ni; j += ni)
			{
				if (drawinf)
				{
					spt[X] = bsx - 1.5 * One_Length;
					ept[X] = spt[X];
				}
				else
				{
					spt[X] = bsx - 0.5 * One_Length;
					ept[X] = spt[X];
				}
				spt[Y] = ydori + j / Y_scale;
				ept[Y] = ydori + (j + ni) / Y_scale;
				if (j < nj * ni)
					makeline(spt, ept, 3, 1, LayerN);
				drawinf = !drawinf;

				levleft = BREAK[i][1] + j;
				ept[X] = bsx - 5.0 * One_Length;
				ept[Y] = ydori + j / Y_scale - 1.2;
				_stprintf(str, L"%0.0lf", levleft);
				if (j > 0)
					maketext(ept, str, 0, ZiGao, 3, 4, L"HZ", LayerN);
			}
		}
		else if (i == NBREAK - 1)//���һ�����᲻��
			break;
		else //�м���
		{
			bsx = mltosx(0, BREAK[i][0]);
			spt[X] = bsx, spt[Y] = ydori;
			ept[X] = spt[X], ept[Y] = ydori + nj * 10;
			makeline(spt, ept, 3, 0.3, LayerN);
			for (j = ni; j <= nj * ni; j += ni)
			{
				spt[Y] = ydori + j / Y_scale;
				ept[X] = spt[X] - 1.0;
				ept[Y] = spt[Y];
				makeline(spt, ept, 3, 0, LayerN);
				levleft = BREAK[i][1] + j;
				ept[X] = spt[X] - 2.0;
				ept[Y] = spt[Y] - 1.2;
				_stprintf(str, L"%0.0lf", levleft);
				maketext(ept, str, 0, ZiGao, 3, 4, L"HZ", LayerN);
			}
		}
	}

	//20190718	���Ӽ���ͼ����������
	m_Frame_Center_Y = (wxp[WXPnum].bottomht + nj * 10 + TopFW) / 2.0;
	//20190718	���Ӽ�¼��������
	double StartX = mltosx(0, m_Scml);
	double EndX = mltosx(0, m_Ecml);
	m_Frame_Center_X = (TABLE_LENGTH + StartX - EndX) / 2.0;
}

void  CZDMHT::draw_des_slop(XLDataBase *pDBStmp,double DefTextH)//���¶��� 
{
	int i,sno,eno,iyb; 
	double sht,eht,ybml,ybmz,cml;
	AcGePoint3d spt,ept,Pt;
	AcGePoint3dArray ptarr;
	ptarr.setLogicalLength(2);
	ACHAR layern[50];

	if(m_Scml > pDBStmp->PDB[pDBStmp->NPD-1].cml || m_Ecml<pDBStmp->PDB[0].cml)
		return;
	//_stprintf(layern, L"�¶���%d",CurTNo);
	_stprintf(layern, L"����ͼ-�¶���");

	//��������ȷ���¶�������
	sno = 0;
	for (i = 0; i<pDBStmp->NPD; i++)
	{
		if (pDBStmp->PDB[i].cml>m_Scml + 0.01)
		{
			sno = i;
			sht = des_level(m_Scml, pDBStmp);
			spt[X] = mltosx(0, m_Scml), spt[Y] = ztosy(m_Scml, sht);

			break;
		}
	}

	//���յ����ȷ���¶����յ��
	eno = pDBStmp->NPD - 1;
	eht = pDBStmp->PDB[eno].Level;
	for (i = 0; i < pDBStmp->NPD; i++)
	{
		if (pDBStmp->PDB[i].cml >= m_Ecml)
		{
			eno = i;
			eht = des_level(m_Ecml, pDBStmp);
			break;
		}
	}

	//20190904����	����������յ������
	//�����������������
	AcGePoint3d LineSPt(spt.x, spt.y, 0.0), LineEPt(spt.x, spt.y + 10.0 / Y_scale, 0.0);
	makeline(LineSPt, LineEPt, CYANBLUE, 0.0, layern);
	ACHAR Text[256];
	CString tempCStr = m_pm->Trs_ProjMl_To_Ckml(m_Scml);
	if(m_VDrawPara.IsGutterMode)
		_stprintf(Text, L"ˮ���������  %s", tempCStr);
	else
		_stprintf(Text, L"��·�������  %s", tempCStr);
	LineSPt.x -= 3.0;
	LineSPt.y += 1.0 / Y_scale;
	maketext(LineSPt, Text, pi / 2.0, DefTextH, CYANBLUE, 1, L"����", layern);
	//�����յ�����������
	ept[X] = mltosx(0, m_Ecml), ept[Y] = ztosy(m_Ecml, eht);
	LineSPt.set(ept.x, ept.y, 0.0);
	LineEPt.set(ept.x, ept.y + 10.0 / Y_scale, 0.0);
	makeline(LineSPt, LineEPt, CYANBLUE, 0.0, layern);
	tempCStr = m_pm->Trs_ProjMl_To_Ckml(m_Ecml); 
	if (m_VDrawPara.IsGutterMode)
		_stprintf(Text, L"ˮ�������յ�  %s", tempCStr);
	else
		_stprintf(Text, L"��·�����յ�  %s", tempCStr);
	
	LineSPt.x -= 3.0;
	LineSPt.y += 1.0 / Y_scale;
	maketext(LineSPt, Text, pi / 2.0, DefTextH, CYANBLUE, 1, L"����", layern);



	iyb = 1;
	ybml = BREAK[iyb][0];

	double lw;

	lw = 0.0;

	for (i = sno; i < eno; i++)
	{
		cml = pDBStmp->PDB[i].cml;
		while (ybml - cml < 0.0 && iyb < NBREAK)//��̳�������
		{
			ybmz = des_level(ybml, pDBStmp);
			ept[X] = mltosx(0, ybml);
			ept[Y] = ztosy(ybml, ybmz);
			ptarr[0].set(spt.x, spt.y, 0);
			ptarr[1].set(ept.x, ept.y, 0);
			make2dline(ptarr, 2, lw, lw, layern);

			spt[X] = ept[X], spt[Y] = ztosy(ybml + 0.1, ybmz);
			if (iyb++ >= NBREAK) iyb = NBREAK - 1;
			ybml = BREAK[iyb][0];
		}
		ept[X] = mltosx(0, cml);
		ept[Y] = ztosy(cml, pDBStmp->PDB[i].Level);
		Pt.x = ept[X], Pt.y = ept[Y];
		makecircle(Pt, 0.75, 2, layern);
		ptarr[0].set(spt.x, spt.y, 0);
		ptarr[1].set(ept.x, ept.y, 0);
		make2dline(ptarr, 2, lw, lw, layern);
		spt[X] = ept[X], spt[Y] = ept[Y];
	}
	cml = m_Ecml < pDBStmp->PDB[eno].cml ? m_Ecml : pDBStmp->PDB[eno].cml;
	while (ybml - cml < 0.0 && iyb < NBREAK)//��̳�������
	{
		ybmz = des_level(ybml, pDBStmp);
		ept.x = mltosx(0, ybml);
		ept.y = ztosy(ybml, ybmz);
		ptarr[0].set(spt.x, spt.y, 0);
		ptarr[1].set(ept.x, ept.y, 0);
		make2dline(ptarr, 2, lw, lw, layern);

		spt[X] = ept.x, spt.y = ztosy(ybml + 0.1, ybmz);
		if (iyb++ >= NBREAK) iyb = NBREAK - 1;
		ybml = BREAK[iyb][0];
	}
	ept[X] = mltosx(0, cml);
	ept[Y] = ztosy(cml, eht);
	ptarr[0].set(spt.x, spt.y, 0);
	ptarr[1].set(ept.x, ept.y, 0);
	make2dline(ptarr, 2, lw, lw, layern);

	//20190821����	�����ӳ���
	if(!m_VDrawPara.IsGutterMode)
		DrawExtendLine(pDBStmp, 20.0, 20.0);

	Draw_Des_VerCurve(sno, eno, pDBStmp);
}

void CZDMHT::Draw_Des_VerCurve(int sno,int eno,XLDataBase *pDBStmp)
{
	int i;
	double di,T,i1,i2,sml,eml,Zsml,Zeml;
	ACHAR layern[50];
	//_stprintf(layern, L"�¶���%d",CurTNo);
	_stprintf(layern, L"����ͼ-�¶���");

	sno=sno-1;
	eno=eno+1;
	if(sno<=0)
	{
		//	Zsml = pDBStmp->PDB[0].cml;
		sno = 1;
	}
	//	else
	Zsml = m_Scml;

	if(eno>pDBStmp->NPD-1)
		eno = pDBStmp->NPD-1;

	for(i=sno;i<eno;i++)
	{
		if(pDBStmp->PDB[i].Rshu>= 0.0)
		{
			i1=(pDBStmp->PDB[i].Level-pDBStmp->PDB[i-1].Level)/(pDBStmp->PDB[i].cml-pDBStmp->PDB[i-1].cml);
			i2=(pDBStmp->PDB[i+1].Level-pDBStmp->PDB[i].Level)/(pDBStmp->PDB[i+1].cml-pDBStmp->PDB[i].cml);
			di=fabs(i1-i2);
			T = pDBStmp->PDB[i].Rshu*di/2.0;
			sml = pDBStmp->PDB[i].cml-T;

			if(sml < m_Scml)
				sml = m_Scml;
			if(sml > m_Ecml)
				sml = m_Ecml;
			eml = pDBStmp->PDB[i].cml+T;
			if(eml > m_Ecml)
				eml = m_Ecml;
			if(eml < m_Scml)
				eml = m_Scml;

			Zeml = sml;
			if(Zeml>Zsml)
				DrawVerCurLine(Zsml,Zeml,2,layern,0.7,10000,0,pDBStmp);
			if(eml>sml)
				DrawVerCurLine(sml,eml,2,layern,0.7,5,0,pDBStmp);
			Zsml = eml;
		}
	}

	//	if(eno==pDBStmp->NPD-1)
	//		Zeml = pDBStmp->PDB[eno].cml;
	//	else
	Zeml = m_Ecml;

	DrawVerCurLine(Zsml,Zeml,2,layern,0.7,10000,0,pDBStmp);
}

void CZDMHT::draw_vertical_factors(int wxpi)//����������
{
	int i;
	double di,T,i1,i2,sml,eml,E,sdmlys,edmlys,ym;
	CString GH;
	ACHAR Rstr[20],Tstr[20],Estr[20],qstr[20],hstr[20],str[80];
	AcGePoint3d spt,ept,tpt;
	double vtexth = 5.0;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-������");

	//ym = Y_inori + NTKHt - TopFW;
	//yb = Y_inori+wxp[wxpi].bottomht;
	//ym = Y_inori + wxp[wxpi].bottomht + 0.5*wxp[wxpi].Lht;
	if (wxpi == -1)	//������Ҳ�л���ѡ�����ʱ���������
	{
		double GridHeight = NTKHt - wxp[WXPnum].bottomht - TopFW;
		int GridNum = floor(GridHeight / 10) - 1;	//��������
		GridHeight = GridNum * 10;
		ym = Y_inori + wxp[WXPnum].bottomht + GridHeight;

		if (!m_VDrawPara.IsDrawCurveInf)	//20190910	���������������Ҫ��
			return;
	}
	else
		ym = Y_inori + wxp[wxpi].bottomht + 0.5 * wxp[wxpi].Lht;

	for (i = 1; i<(pDBS->NPD-1); i++)
	{
		i1 = (pDBS->PDB[i].Level - pDBS->PDB[i - 1].Level) / (pDBS->PDB[i].cml - pDBS->PDB[i - 1].cml);
		i2 = (pDBS->PDB[i + 1].Level - pDBS->PDB[i].Level) / (pDBS->PDB[i + 1].cml - pDBS->PDB[i].cml);
		di = fabs(i1 - i2);
		T = pDBS->PDB[i].Rshu * di / 2.0;
		if (fabs(pDBS->PDB[i].Rshu) < 0.01)
			E = 0.0;
		else
			E = T*T / 2.0 / pDBS->PDB[i].Rshu;
		sml = pDBS->PDB[i].cml - T;
		eml = pDBS->PDB[i].cml + T;
		if (eml <= m_Scml || sml >= m_Ecml)
			continue;
		sdmlys = fabs(m_pm->XLC(sml, GH));
		sdmlys = sdmlys - floor(sdmlys / 1000)*1000.0;
		ads_rtos(sdmlys, 2, 2, qstr);
		edmlys = fabs(m_pm->XLC(eml, GH));
		edmlys = edmlys - floor(edmlys / 1000)*1000.0;
		ads_rtos(edmlys, 2, 2, hstr);

		spt.x = mltosx(0, sml); spt.y = ym;
		ept.x = mltosx(0, eml); ept.y = ym;
		makeline(spt, ept, 2, 0, LayerName);
		ads_rtos(pDBS->PDB[i].Rshu, 2, 3, Rstr);
		ads_rtos(T, 2, 2, Tstr);
		ads_rtos(E, 2, 2, Estr);
		if (i1 > i2)//͹����
		{
			tpt.x = spt.x; tpt.y = ym - 5.0;
			if (sml > m_Scml)
			{
				makeline(spt, tpt, 2, 0, LayerName);
				tpt.x = spt.x - 2.0;
				tpt.y = ym - 2.5;
				_stprintf(str, L"+%s", qstr);
				maketext(tpt, str, 0.5*pi, vtexth, 2, 3, L"����", LayerName);
			}
			tpt.x = ept.x; tpt.y = ym - 5.0;
			if (eml < m_Ecml)
			{
				makeline(ept, tpt, 2, 0, LayerName);
				tpt.x = ept.x + 2.0;
				tpt.y = ym - 2.5;
				_stprintf(str, L"+%s", hstr);
				maketext(tpt, str, 0.5*pi, vtexth, 2, 3, L"����", LayerName);
			}

			_stprintf(str, L"R-%s T-%s E-%s", Rstr, Tstr, Estr);
			tpt.x = mltosx(0, pDBS->PDB[i].cml);
			tpt.y = ym - 2.5;
			if (pDBS->PDB[i].cml>m_Scml && pDBS->PDB[i].cml < m_Ecml)
			{
				maketext(tpt, str, 0, vtexth, 2, 3, L"����", LayerName);
				spt.x = tpt.x;
				tpt.y = ym + 5.0;
				makeline(spt, tpt, 2, 0, LayerName);
			}
		}
		else
		{
			tpt.x = spt.x; tpt.y = ym + 5.0;
			if (sml > m_Scml)
			{
				makeline(spt, tpt, 2, 0, LayerName);
				tpt.x = spt.x - 2.0;
				tpt.y = ym + 2.5;
				_stprintf(str, L"+%s", qstr);
				maketext(tpt, str, 0.5*pi, vtexth, 2, 3, L"����", LayerName);
			}
			tpt.x = ept.x; tpt.y = ym + 5.0;
			if (eml < m_Ecml)
			{
				makeline(ept, tpt, 2, 0, LayerName);
				tpt.x = ept.x + 2.0;
				tpt.y = ym + 2.5;
				_stprintf(str, L"+%s", hstr);
				maketext(tpt, str, 0.5*pi, vtexth, 2, 3, L"����", LayerName);
			}

			_stprintf(str, L"R-%s T-%s E-%s", Rstr, Tstr, Estr);
			tpt.x = mltosx(0, pDBS->PDB[i].cml);
			tpt.y = ym + 2.5;
			if (pDBS->PDB[i].cml>m_Scml && pDBS->PDB[i].cml < m_Ecml)
			{
				maketext(tpt, str, 0, vtexth, 2, 3, L"����", LayerName);
				spt.x = tpt.x;
				tpt.y = ym - 5.0;
				makeline(spt, tpt, 2, 0, LayerName);
			}
		}
	}
}

void CZDMHT::DrawDZQ()//�ϴ�����
{
	int i;
	CString DesLevel;

	ads_real TextHt=3.0;
	ACHAR mes[500];
	double sml,eml,cml,desh,mz;
	AcGePoint3d spt,ept,cpt;
	int j;
	ACHAR layern[50];
	//_stprintf(layern, L"������%d",CurTNo);
	_stprintf(layern, L"����ͼ-������");

	for(i=0;i<pDBS->BriNum;i++)
	{	
		sml = m_pm->TYLC(pDBS->Bri[i].StDml);
		eml = m_pm->TYLC(pDBS->Bri[i].EdDml);
		if(eml>=m_Scml&&eml<=m_Ecml)
		{
			spt.x = mltosx(0,eml);
			desh = rdes_level(eml,pDBS);
			spt.y = ztosy(eml,desh)-2.0;
			ept.x = spt.x;
			mz  = CalDmxH(eml);
			ept.y = ztosy(eml,mz);
			makeline(spt,ept,4,0,layern);
		}
		if(!(sml>m_Ecml || eml<m_Scml))
		{
			if(sml<m_Scml)
				sml=m_Scml;
			if(eml>m_Ecml)
				eml=m_Ecml;
			DrawVerCurLine(sml,eml,4,layern,0.0,10, -2.0,pDBS);//����������

			GetCmlKKSY(i);

			for(j=0;j<nBriL;j++)
			{
				if(BrgLArr[j].sml>=m_Scml&&BrgLArr[j].sml<=m_Ecml)
				{
					spt.x = mltosx(0,BrgLArr[j].sml);
					desh = rdes_level(BrgLArr[j].sml,pDBS);
					spt.y = ztosy(BrgLArr[j].sml,desh)-2.0;
					ept.x = spt.x;
					mz  = CalDmxH(BrgLArr[j].sml);
					ept.y = ztosy(BrgLArr[j].sml,mz);
					makeline(spt,ept,4,0,layern);
				}
			}

			cml=m_pm->TYLC(pDBS->Bri[i].CenDml);
			if(cml>=m_Scml&&cml<=m_Ecml)
			{
				desh=rdes_level(cml,pDBS);
				spt[X]=mltosx(0,cml),spt[Y]=ztosy(cml,desh);
				ept[X]=spt[X],ept[Y]=spt[Y]+50.0;
				makeline(spt,ept,4,0,layern);

				cpt[X]=spt[X]+0.5*TextHt+1.0;	
				cpt[Y]=(spt[Y]+ept[Y])/2.0;			  			  			  
				_stprintf(mes, L"%s%s",pDBS->Bri[i].CKml,pDBS->Bri[i].BriName);
				maketext(cpt,mes,0.5*pi,TextHt,4,3, L"����",layern);
				cpt[X]=spt[X]-0.5*TextHt-1.0;	
				_stprintf(mes, L"%s",pDBS->Bri[i].KKSY);
				maketext(cpt,mes,0.5*pi,TextHt,4,3, L"����",layern);

				desh=_wtof(pDBS->Bri[i].SWHt);
				if(desh>0.1)
				{
					cpt[X]=spt[X]-3.0,cpt[Y]=ztosy(cml,desh);
					ept[X]=cpt[X]+20, ept[Y]=cpt[Y];
					spt[Y]=cpt[Y];
					makeline(cpt,ept,4,0,layern);//��ˮλ��
					cpt[X] = spt[X]-1,cpt[Y]=spt[Y]-0.5;
					ept[X] = spt[X]+1,ept[Y]=cpt[Y];
					makeline(cpt,ept,4,0,layern);//��ˮλ��
					cpt[X] = spt[X]-0.5,cpt[Y]=spt[Y]-1.0;
					ept[X] = spt[X]+0.5,ept[Y]=cpt[Y];
					makeline(cpt,ept,4,0,layern);//��ˮλ��
					cpt[X] = spt[X]-0.2,cpt[Y]=spt[Y]-1.5;
					ept[X] = spt[X]+0.2,ept[Y]=cpt[Y];
					makeline(cpt,ept,4,0,layern);//��ˮλ��

					ept[X]=spt[X]-1,ept[Y]=spt[Y]+2;
					cpt[X]=spt[X]+1,cpt[Y]=ept[Y];
					makeline(spt,ept,4,0,layern);
					makeline(spt,cpt,4,0,layern);//��С������
					makeline(ept,cpt,4,0,layern);//��С������
					_stprintf(mes, L"%s %s",DesLevel,pDBS->Bri[i].SWHt);				  
					spt[X]=spt[X]+1;
					spt[Y]=spt[Y]+0.5;
					maketext(spt,mes,0,2.5,4,1, L"HZ",layern);
				}
			}
		}
	}
} 

void CZDMHT::DrawTun()//�����
{
	int i;

	ads_real TextHt=2.5;
	ACHAR mes[80];
	double sml,eml,desh,mz,ys;
	AcGePoint3d spt,ept,cpt;
	ACHAR layern[50];
	//_stprintf(layern, L"���%d",CurTNo);
	_stprintf(layern, L"����ͼ-���");

	for(i=0;i<pDBS->TunNum;i++)
	{
		sml=m_pm->TYLC(pDBS->Tun[i].StDml);
		eml=m_pm->TYLC(pDBS->Tun[i].EdDml);

		if(eml>=m_Scml&&eml<=m_Ecml)
		{
			spt.x = mltosx(0,eml);
			desh = rdes_level(eml,pDBS);
			spt.y = ztosy(eml,desh);
			ept.x = spt.x;
			mz  = CalDmxH(eml);
			ept.y = ztosy(eml,mz);
			makeline(spt,ept,3,0,layern);
			ys = fmod(fabs(pDBS->Tun[i].EdDml+0.000001),1000);
			_stprintf(mes, L"+%0.2lf",ys);
			spt.x=spt.x+TextHt+1.0;
			spt.y = spt.y+1;
			maketext(spt,mes,0.5*pi,TextHt,3,1, L"����",layern);
		}
		if(sml>=m_Scml&&sml<=m_Ecml)
		{
			spt.x = mltosx(0,sml);
			desh = rdes_level(sml,pDBS);
			spt.y = ztosy(sml,desh);
			ept.x = spt.x;
			mz  = CalDmxH(sml);
			ept.y = ztosy(sml,mz);
			makeline(spt,ept,3,0,layern);
			ys = fmod(fabs(pDBS->Tun[i].StDml+0.000001),1000);
			_stprintf(mes, L"+%0.2lf",ys);
			spt.x=spt.x-0.5*TextHt;
			spt.y = spt.y+1;
			maketext(spt,mes,0.5*pi,TextHt,3,1, L"����",layern);
		}
		if(!(sml>m_Ecml || eml<m_Scml))
		{
			if(sml<m_Scml)
				sml=m_Scml;
			if(eml>m_Ecml)
				eml=m_Ecml;
			DrawVerCurLine(sml,eml,3,layern,0.0,10, 8,pDBS);//����������
			DrawVerCurLine(sml,eml,3,layern,0.0,10, 6,pDBS);//����������
		}
	}

}
/*----------------------------------------------------------*
* Name:  determain the location of character C in string
variable STR[], if found, return subscript of C in STR[],
otherwise, return 0
*----------------------------------------------------------*/
int CZDMHT::F_num(ACHAR *str, ACHAR c)
{
	int i, len, num=0, smark, c1;

	smark = toupper(c);
	len = _tcslen(str);
	for(i=len-1; i>=0; i--)  
	{
		c1 = toupper(str[i]);
		if (c1 == smark)
		{num=i+1; break;}
	}
	return(num);
}

void CZDMHT::GetCmlKKSY(int ibri)
{
	int len,j,k;  //BrgArray 0��Ӧ��β�� KNum-1��Ӧ����ͷ��

	ACHAR str1[256],brg[180],brg1[90];
	CString ks,kc;

	KNum = 0;
	int pos=pDBS->Bri[ibri].KKSY.FindOneOf(L"-");

	if(pos>0)
	{
		ks = pDBS->Bri[ibri].KKSY.Left(pos);
		len = pDBS->Bri[ibri].KKSY.GetLength();
		kc =  pDBS->Bri[ibri].KKSY.Right(len-pos-1);
		BrgArray[KNum].klen = _wtof(kc);
		BrgArray[KNum].ks = _wtoi(ks);
		KNum++;
	}
	else
	{
		_tcscpy(str1,pDBS->Bri[ibri].KKSY);
		while(len=F_num(str1,'+'))
		{
			for(j=len,k=0; j<_tcslen(str1);k++, j++)
				brg[k] = str1[j];  
			str1[len-1] ='\0' ;
			brg[k] = '\0';

			len = F_num(brg,'x');
			for(k=0;k<len;k++)
				brg1[k] = brg[k];
			brg1[k]='\0';
			BrgArray[KNum].ks = _wtoi(brg1);
			for(j=len,k=0; j<_tcslen(brg);k++, j++)
				brg1[k] = brg[j];  	
			brg1[k]='\0';
			BrgArray[KNum].klen = _wtof(brg1);
			KNum++;
		}

		len = F_num(str1,'x');
		for(k=0;k<len;k++)
			brg1[k] = str1[k];
		brg1[k]='\0';
		BrgArray[KNum].ks = _wtoi(brg1);
		for(j=len,k=0; j<_tcslen(str1);k++, j++)
			brg1[k] = str1[j];  	
		brg1[k]='\0';
		BrgArray[KNum].klen = _wtof(brg1);
		KNum++;
	}

	nBriL = 0; 
	double sml;
	sml = m_pm->TYLC(pDBS->Bri[ibri].StDml);

	for(j=KNum-1;j>=0;j--)
	{
		for(k=0;k<BrgArray[j].ks;k++)
		{
			BrgLArr[nBriL].sml = sml;
			sml = sml + BrgArray[j].klen;
			nBriL++;
		}
	}
}
//��sml��eml���¶��� ���������ߣ����᣻Zoff��ʾ���¶��߾��� �������¶�����
void CZDMHT::DrawVerCurLine(double sml,double eml,int Color,CString LayerName, double LWid,double Len,double Zoff,XLDataBase *pDBStmp)
{
	int iyb; 
	double ybml,ybmz,cml,desh;	
	AcGePoint3d spt,ept;
	AcGePoint3dArray ptarr;
	ptarr.setLogicalLength(2);
	ACHAR LayerN[80];

	_tcscpy(LayerN,LayerName);

	cml = sml;
	iyb=1;
	ybml=BREAK[iyb][0];
	while (ybml<cml  && iyb<NBREAK)
	{
		if (iyb++>=NBREAK) iyb=NBREAK-1;
		ybml=BREAK[iyb][0];		
	}
	spt[X]=mltosx(0,cml);
	desh = rdes_level(cml,pDBStmp);
	spt[Y]=ztosy(cml,desh)+Zoff;
	cml +=Len;
	while(cml<eml)
	{
		while(ybml-cml<0.0 && iyb<NBREAK)//��̳�������
		{
			ybmz = rdes_level(ybml,pDBStmp);
			ept[X] = mltosx(0,ybml);
			ept[Y] = ztosy(ybml,ybmz)+Zoff;
			ptarr[0].set(spt.x,spt.y,0);
			ptarr[1].set(ept.x,ept.y,0);
			make2dline(ptarr,Color,LWid,LWid,LayerN);

			spt[X]=ept[X],spt[Y]=ztosy(ybml+0.1,ybmz)+Zoff;
			if (iyb++>=NBREAK) iyb=NBREAK-1;
			ybml=BREAK[iyb][0];
		}  
		ept[X] = mltosx(0,cml);
		desh = rdes_level(cml,pDBStmp);
		ept[Y] = ztosy(cml, desh)+Zoff;
		ptarr[0].set(spt.x,spt.y,0);
		ptarr[1].set(ept.x,ept.y,0);
		make2dline(ptarr,Color,LWid,LWid,LayerN);
		spt[X]=ept[X],spt[Y]=ept[Y];
		cml +=Len;
	}

	cml = eml;
	while(ybml-cml<0.0 && iyb<NBREAK)//��̳�������
	{
		ybmz = rdes_level(ybml,pDBStmp);
		ept[X] = mltosx(0,ybml);
		ept[Y] = ztosy(ybml,ybmz)+Zoff;
		ptarr[0].set(spt.x,spt.y,0);
		ptarr[1].set(ept.x,ept.y,0);
		make2dline(ptarr,Color,LWid,LWid,LayerN);
		//       makeline(spt,ept,3,AcDb::kLnWtByLayer, L"�¶���");

		spt[X]=ept[X],spt[Y]=ztosy(ybml+0.1,ybmz)+Zoff;
		if (iyb++>=NBREAK) iyb=NBREAK-1;
		ybml=BREAK[iyb][0];
	}  
	ept[X] = mltosx(0,cml);
	cml=cml-0.01;
	desh = rdes_level(cml,pDBStmp);
	ept[Y] = ztosy(cml, desh)+Zoff;
	ptarr[0].set(spt.x,spt.y,0);
	ptarr[1].set(ept.x,ept.y,0);
	make2dline(ptarr,Color,LWid,LWid,LayerN);
}

//   consider vertical curve  calculate the design level at certain mileage 
double CZDMHT::rdes_level (double cml,XLDataBase *pDBStmp)//���������߼�����Ƹ߳�
	/* cml; continue mileage  */
{
	int   j;
	double     z=-1.0;

	//if (fabs(cml-(pDBStmp->PDB[pDBStmp->NPD-1].cml)<0.01))
	//{
	// cml=pDBStmp->PDB[pDBStmp->NPD-1].cml-0.001;
	//}
	if (cml< pDBStmp->PDB[0].cml-0.1 || cml>pDBStmp->PDB[pDBStmp->NPD-1].cml+0.1)
	{
		if (cml< pDBStmp->PDB[0].cml-0.1)
		{
			z=pDBStmp->PDB[0].Level;
		}
		if (cml>pDBStmp->PDB[pDBStmp->NPD-1].cml+0.1)
		{
			z=pDBStmp->PDB[pDBStmp->NPD-1].Level;
		}
		return z;
	}
	else
	{
		for(j=1; j<pDBStmp->NPD; j++)
			if (pDBStmp->PDB[j-1].cml-0.1<=cml&&cml<=pDBStmp->PDB[j].cml+0.1)
			{

				z = pDBStmp->PDB[j-1].Level+(cml-pDBStmp->PDB[j-1].cml)*
					(pDBStmp->PDB[j].Level-pDBStmp->PDB[j-1].Level)/(pDBStmp->PDB[j].cml-pDBStmp->PDB[j-1].cml);              
				z+= DLEVEL(j,cml,pDBStmp);
				break;				
			}
	}
	return (z);
}
/*---------------------------------------------------------*
*        calculate the design level at certain mileage    �߳�����ֵ*
*---------------------------------------------------------*/
double CZDMHT::DLEVEL(int icm, double cml,XLDataBase *pDBStmp)
{
	double vr1, vr2, di1,di2, vt1,vt2, ds, ds1, dds,s1,s2,pdc;
	int kr;

	if(icm==1)//������ڵ�һ�¶���
	{
		vr1=1.0e8;
		di1=0.0;
		vr2 = pDBStmp->PDB[icm].Rshu;
		di2 = (pDBStmp->PDB[icm+1].Level-pDBStmp->PDB[icm].Level)/
			(pDBStmp->PDB[icm+1].cml-pDBStmp->PDB[icm].cml) - 
			(pDBStmp->PDB[icm].Level-pDBStmp->PDB[icm-1].Level)/
			(pDBStmp->PDB[icm].cml-pDBStmp->PDB[icm-1].cml);
		ds = cml-pDBStmp->PDB[icm-1].cml;
	}
	else if (icm==pDBStmp->NPD-1)//����������һ�¶���
	{
		vr1 = pDBStmp->PDB[icm-1].Rshu;
		di1 = (pDBStmp->PDB[icm].Level-pDBStmp->PDB[icm-1].Level)/
			(pDBStmp->PDB[icm].cml-pDBStmp->PDB[icm-1].cml) - 
			(pDBStmp->PDB[icm-1].Level-pDBStmp->PDB[icm-2].Level)/
			(pDBStmp->PDB[icm-1].cml-pDBStmp->PDB[icm-2].cml);
		vr2 = 1.0e8;
		di2 = 0.0;
		ds = cml-pDBStmp->PDB[icm-1].cml;
	}
	else 
	{
		vr1 = pDBStmp->PDB[icm-1].Rshu;
		vr2 = pDBStmp->PDB[icm].Rshu;
		di1 = (pDBStmp->PDB[icm].Level-pDBStmp->PDB[icm-1].Level)/
			(pDBStmp->PDB[icm].cml-pDBStmp->PDB[icm-1].cml) - 
			(pDBStmp->PDB[icm-1].Level-pDBStmp->PDB[icm-2].Level)/
			(pDBStmp->PDB[icm-1].cml-pDBStmp->PDB[icm-2].cml);
		di2 = (pDBStmp->PDB[icm+1].Level-pDBStmp->PDB[icm].Level)/
			(pDBStmp->PDB[icm+1].cml-pDBStmp->PDB[icm].cml) - 
			(pDBStmp->PDB[icm].Level-pDBStmp->PDB[icm-1].Level)/
			(pDBStmp->PDB[icm].cml-pDBStmp->PDB[icm-1].cml);
		ds = cml-pDBStmp->PDB[icm-1].cml;
	}
	sign(s1, di1);
	sign(s2, di2);
	if(vr1>=0.0)
		vt1 = fabs(vr1*di1/2.0);
	else
		vt1 = fabs(vr1);

	if(vr2>=0.0)
		vt2 = fabs(vr2*di2/2.0);
	else
		vt2 = fabs(vr2);

	pdc=pDBStmp->PDB[icm].cml-pDBStmp->PDB[icm-1].cml;
	if (ds<vt1) //����ǰ��������
	{
		ds1 = vt1-ds;
		kr  = 1;
	}
	else if (ds>pdc-vt2) //���ں���������
	{
		ds1 = ds-pdc+vt2;
		kr  = 2;
	}
	else //���ڼ�ֱ����
	{
		ds1 = 0.0;
		kr  = 0;
	}
	if (kr==1 && vr1>0.0)
		dds = s1*ds1*ds1/2.0/vr1;
	else if (kr==2 && vr2>0.0)
		dds = s2*ds1*ds1/2.0/vr2;
	else
		dds = 0.0;
	return (dds);
}

void  CZDMHT::draw_earthline()//������� 
{
	int i,sno,eno,iyb; 
	double sht,eht,ybml,ybmz,cml;
	ACHAR layern[50];
	AcGePoint2dArray Ptarr;
	AcGePoint2d spt,ept;
	//_stprintf(layern, L"������%d",CurTNo);
	_stprintf(layern, L"����ͼ-������");

	sno = 0;
	sht = pDBS->TWB[sno].DMGC;

	for(i=0; i<pDBS->NTW;i++)
	{
		if(pDBS->TWB[i].cml>m_Scml+0.01)//��������ȷ������������
		{
			sno=i;
			if(i>0)
				sht= pDBS->TWB[i-1].DMGC+(m_Scml-pDBS->TWB[i-1].cml)*
				(pDBS->TWB[i].DMGC-pDBS->TWB[i-1].DMGC)/(pDBS->TWB[i].cml-pDBS->TWB[i-1].cml);
			else
				sht = pDBS->TWB[0].DMGC;
			break;
		}
	}
	if(i==pDBS->NTW)
		return;

	eno=pDBS->NTW-1;
	eht=pDBS->TWB[eno].DMGC;
	for (i = 0; i < pDBS->NTW; i++)
	{
		if (pDBS->TWB[i].cml >= m_Ecml - 0.1)//���յ����ȷ���������յ��
		{
			eno = i;
			if (i > 0)
				eht = pDBS->TWB[i - 1].DMGC + (m_Ecml - pDBS->TWB[i - 1].cml)*
				(pDBS->TWB[i].DMGC - pDBS->TWB[i - 1].DMGC) / (pDBS->TWB[i].cml - pDBS->TWB[i - 1].cml);
			else
				eht = pDBS->TWB[0].DMGC;
			break;
		}
	}

	iyb = 1;
	ybml = BREAK[iyb][0];
	if (m_Scml >= pDBS->TWB[0].cml)
		spt[X] = mltosx(0, m_Scml), spt[Y] = ztosy(m_Scml, sht);
	else
		spt[X] = mltosx(0, pDBS->TWB[0].cml), spt[Y] = ztosy(pDBS->TWB[0].cml, pDBS->TWB[0].DMGC);
	Ptarr.append(spt);
	for (i = sno; i < eno; i++)
	{
		cml = pDBS->TWB[i].cml;
		while (ybml - cml < 0.0 && iyb < NBREAK)//��̳�������
		{
			ybmz = pDBS->TWB[i - 1].DMGC + (ybml - pDBS->TWB[i - 1].cml)*
				(pDBS->TWB[i].DMGC - pDBS->TWB[i - 1].DMGC) / (pDBS->TWB[i].cml - pDBS->TWB[i - 1].cml);
			ept[X] = mltosx(0, ybml);
			ept[Y] = ztosy(ybml, ybmz);
			Ptarr.append(ept);
			//				makeline(spt,ept,0,0,layern);
			makepolyline(Ptarr, 0.0, 0.0, layern);
			Ptarr.removeSubArray(0, Ptarr.length() - 1);
			spt[X] = ept[X], spt[Y] = ztosy(ybml + 0.1, ybmz);
			Ptarr.append(spt);
			if (iyb++ >= NBREAK) 
				iyb = NBREAK - 1;
			ybml = BREAK[iyb][0];
		}
		ept[X] = mltosx(0, cml);
		ept[Y] = ztosy(cml, pDBS->TWB[i].DMGC);
		Ptarr.append(spt);
		//			makeline(spt,ept,0,0,layern);
		spt[X] = ept[X], spt[Y] = ept[Y];
	}
	if (m_Ecml <= pDBS->TWB[pDBS->NTW - 1].cml)
		ept[X] = mltosx(0, m_Ecml), ept[Y] = ztosy(m_Ecml, eht);
	else
		ept[X] = mltosx(0, pDBS->TWB[pDBS->NTW - 1].cml), ept[Y] = ztosy(pDBS->TWB[pDBS->NTW - 1].cml, pDBS->TWB[pDBS->NTW - 1].DMGC);
	Ptarr.append(ept);
	makepolyline(Ptarr, 0.0, 0.0, layern);
	Ptarr.removeSubArray(0, Ptarr.length() - 1);
	//		makeline(spt,ept,0,0,layern);
}

void  CZDMHT::draw_bxearthline()//����ߵ����� 
{
	int i,sno,eno,iyb,j; 
	double sht,eht,ybml,ybmz,cml,ltscale;
	AcGePoint2dArray Ptarr;
	AcGePoint2d spt,ept;
	ACHAR layern[50],linetype[50];
	//    AcCmColor color;

	for(j=0;j<NBxDmx;j++)
	{
		sno = 0;
		sht = BxDBS[j]->TWB[sno].DMGC;
		if(BxDBS[j]->ZJ < 0)
		{
			//_stprintf(layern, L"����ߵ�����%d",CurTNo);	
			_stprintf(layern, L"����ͼ-����ߵ�����%d");
			_tcscpy(linetype, L"dashed");
			ltscale = 0.1;
		}
		else
		{
			//_stprintf(layern, L"�ұ��ߵ�����%d",CurTNo);
			_stprintf(layern, L"����ͼ-�ұ��ߵ�����");
			_tcscpy(linetype, L"dashdot2");
			ltscale = 0.5;
		}
		//		color.setColorIndex(BxDBS[j]->Color);			
		//		CreateLayer(layern,linetype,color);
		for(i=0; i<BxDBS[j]->NTW;i++)
		{
			if(BxDBS[j]->TWB[i].cml>m_Scml+0.01)//��������ȷ������������
			{
				sno=i;
				if(i>0)
					sht= BxDBS[j]->TWB[i-1].DMGC+(m_Scml-BxDBS[j]->TWB[i-1].cml)*
					(BxDBS[j]->TWB[i].DMGC-BxDBS[j]->TWB[i-1].DMGC)/(BxDBS[j]->TWB[i].cml-BxDBS[j]->TWB[i-1].cml);
				else
					sht = BxDBS[j]->TWB[0].DMGC;
				break;
			}
		}
		if(i==BxDBS[j]->NTW)
			continue;

		eno=BxDBS[j]->NTW-1;
		eht=BxDBS[j]->TWB[eno].DMGC;
		for(i=0; i<BxDBS[j]->NTW;i++)
			if(BxDBS[j]->TWB[i].cml>=m_Ecml-0.1)//���յ����ȷ���������յ��
			{
				eno=i;
				if(i>0)
					eht = BxDBS[j]->TWB[i-1].DMGC+(m_Ecml-BxDBS[j]->TWB[i-1].cml)*
					(BxDBS[j]->TWB[i].DMGC-BxDBS[j]->TWB[i-1].DMGC)/(BxDBS[j]->TWB[i].cml-BxDBS[j]->TWB[i-1].cml);
				else
					eht = BxDBS[j]->TWB[0].DMGC;
				break;
			}

			if(sno>=eno)
				continue;

			iyb=1;
			ybml=BREAK[iyb][0];

			if(m_Scml >= BxDBS[j]->TWB[0].cml)
				spt[X]=mltosx(0,m_Scml),spt[Y]=ztosy(m_Scml,sht);
			else
				spt[X]=mltosx(0,BxDBS[j]->TWB[0].cml),spt[Y]=ztosy(BxDBS[j]->TWB[0].cml,BxDBS[j]->TWB[0].DMGC);		
			Ptarr.append(spt);
			for(i=sno;i<eno;i++)
			{
				cml = BxDBS[j]->TWB[i].cml;
				while(ybml-cml<0.0 && iyb<NBREAK)//��̳�������
				{
					ybmz = BxDBS[j]->TWB[i-1].DMGC+(ybml-BxDBS[j]->TWB[i-1].cml)*
						(BxDBS[j]->TWB[i].DMGC-BxDBS[j]->TWB[i-1].DMGC)/(BxDBS[j]->TWB[i].cml-BxDBS[j]->TWB[i-1].cml);
					ept[X] = mltosx(0,ybml);
					ept[Y] = ztosy(ybml,ybmz);
					Ptarr.append(ept);

					makepolyline(Ptarr,BxDBS[j]->Color,0.0,layern,linetype,ltscale);
					Ptarr.removeSubArray(0, Ptarr.length()-1);
					spt[X]=ept[X],spt[Y]=ztosy(ybml+0.1,ybmz);
					Ptarr.append(spt);
					if (iyb++>=NBREAK) iyb=NBREAK-1;
					ybml=BREAK[iyb][0];
				}  
				ept[X] = mltosx(0,cml);
				ept[Y] = ztosy(cml, BxDBS[j]->TWB[i].DMGC);
				Ptarr.append(ept);

				spt[X]=ept[X],spt[Y]=ept[Y];
			}
			if(m_Ecml <= BxDBS[j]->TWB[BxDBS[j]->NTW-1].cml)
				ept[X]=mltosx(0,m_Ecml),ept[Y]=ztosy(m_Ecml,eht);
			else
				ept[X]=mltosx(0,BxDBS[j]->TWB[BxDBS[j]->NTW-1].cml),ept[Y]=ztosy(BxDBS[j]->TWB[BxDBS[j]->NTW-1].cml,BxDBS[j]->TWB[BxDBS[j]->NTW-1].DMGC);
			Ptarr.append(ept);
			makepolyline(Ptarr,BxDBS[j]->Color,0.0,layern,linetype,ltscale);
			Ptarr.removeSubArray(0, Ptarr.length()-1);
	}
}

void CZDMHT::DrawLCZH_Sg(int wxpi)//����ʩ��ͼ���׮��
{
	int   i, km, mt, mess=0;
	double  tmp;
	ACHAR    str[80],ss[5]=L"+000",sjh[2]=L"+";
	double yb,dml,ys;
	AcGePoint3d spt;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-���׮��");

	yb = Y_inori+wxp[wxpi].bottomht;
	for(i=0;i<pDBS->NTW;i++)
	{
		if (isBiaoZhuGaiLiCheng(m_Scml,m_Ecml,pDBS->TWB[i].cml) == false)
			continue;

		//20190829	�����������׮�Ƿ����
		if (!m_VDrawPara.IsDrawFeaturePile && i > 0 && i + 1 < pDBS->NTW)
		{
			if (IsFeaturePile(pDBS->TWB[i].cml))
				continue;
		}

		if(pDBS->TWB[i].cml > m_Scml+2.0 && pDBS->TWB[i].cml < m_Ecml-2.0)
		{
			spt.x=mltosx(0,pDBS->TWB[i].cml);
			spt.y=yb+0.5;
			dml = fabs(pDBS->TWB[i].ml);
			km  = (int) floor (dml /1000);
			mt  = (int) floor ((dml - km*1000.0)/100) ;
			tmp = (int) floor (dml - km*1000.0 - mt*100) ;   
			if (fabs(tmp)<0.001 && mt==0) 
				_stprintf (str, L"%d%s", km,ss);
			else
				_stprintf (str, L"%0.2lf", dml-km*1000.0);
			//				 ads_rtos(dml-km*1000.0,2,2,str);

			maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);
		}
	}
	spt.x = mltosx(0, m_Scml) + wxp[wxpi].texth;	//20190710	�����ָߣ���Խ��
	spt.y=yb+0.5;
	km  = (int) floor (m_Sdml /1000);
	ys = m_Sdml - km * 1000.0;
	if(ys < 0.1)
		_stprintf (str, L"%d%s000", km,sjh);
	else if(ys < 10)
		_stprintf (str, L"%d%s00%0.2lf", km,sjh,ys);
	else if(ys < 100)
		_stprintf (str, L"%d%s0%0.2lf", km,sjh,ys);
	else
		_stprintf (str, L"%d%s%0.2lf", km,sjh,ys);
	maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);

	spt.x = mltosx(0, m_Ecml)/* - 0.5 * wxp[wxpi].texth*/;	//20190710	��ȥ�ָߣ���Խ��
	spt.y=yb+0.5;
	km  = (int) floor (m_Edml /1000);
	ys = m_Edml-km*1000.0;
	if(ys < 0.1)
		_stprintf (str, L"%d%s000", km,sjh);
	else if(ys < 10)
		_stprintf (str, L"%d%s00%0.2lf", km,sjh,ys);
	else if(ys < 100)
		_stprintf (str, L"%d%s0%0.2lf", km,sjh,ys);
	else
		_stprintf (str, L"%d%s%0.2lf", km,sjh,ys);
	maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);
}

void CZDMHT::DrawLCZH_Cb(int wxpi)//���Ƴ���ͼ���׮��
{
	int   i, km, mess=0;
	ACHAR    str[80],ss[5]=L"+000",sjh[2]=L"+";
	double yb,dml,ys;
	AcGePoint3d spt,ept;
	CString GH;
	int ibm=0;
	double BmCml[1000];
	double gwht;
	int gwnum;
	double len = 10*X_scale;
	double AgwScml,AgwEcml;		
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-���׮��");

	gwht=NTKHt-wxp[WXPnum].bottomht-TopFW;
	yb = Y_inori+wxp[wxpi].bottomht;

	for(i=0;i<m_pm->NLCB;i++)
	{
		if(m_pm->pLCB[i].lc > m_Scml && m_pm->pLCB[i].lc<m_Ecml)
		{
			spt.x = mltosx(0,m_pm->pLCB[i].lc);
			spt.y = yb;
			ept.x = spt.x;
			ept.y = yb+2.0;
			makeline(spt,ept,4,0);
			spt.y = yb+0.5*wxp[wxpi].Lht;
			if(m_pm->pLCB[i].lcflag>0)
			{
				if(m_pm->pLCB[i].lcflag==10)//����׮
				{
					dml = m_pm->XLC(m_pm->pLCB[i].lc,GH);
					km  = (int) floor (dml /1000);
					_stprintf(str, L"%s%d",GH,km);
					maketext(spt,str,0,wxp[wxpi].texth,4,3, L"����", LayerName);
				}
				else//����׮
				{			
					_stprintf(str, L"%d",m_pm->pLCB[i].lcflag);
					maketext(spt,str,0,wxp[wxpi].texth,4,3, L"����", LayerName);
				}
				//		BmCml[ibm]=m_pm->pLCB[i].lc;


				//				if(infwgx)//������
				//				{
				//				
				//					if(ibm==0)//��һ������
				//					{
				//                         AgwScml=m_Scml;
				//						 AgwEcml=m_pm->pLCB[i].lc;
				//
				//						 double sx,ex;
				//						 sx = mltosx(0,AgwScml);
				//						 ex = spt.x;
				//						 while(ex>sx+0.001)
				//						 {
				//							 spt[X]=ex;
				//							 spt[Y]=Y_inori+wxp[WXPnum].bottomht;
				//							 ept[X]=spt[X];
				//							 ept[Y]=Y_inori+wxp[WXPnum].bottomht+gwht;
				//							 makeline(spt,ept,8,0, L"������", L"DOT3");
				//							 ex = ex - 10;
				//						 }
				//		
				//					}
				//					else
				//					{
				//						AgwScml=BmCml[ibm-1];
				//					    AgwEcml=m_pm->pLCB[i].lc;	
				//                    //���ױ�֮��ȷ�5��
				//						double sx,ex;
				//						sx = mltosx(0,AgwScml);
				//						ex = spt.x;
				//						double ladd = (ex-sx)/5.0;
				//    					for(int k=0;k<=5;k++)//draw ����
				//						{
				//							spt[X]=sx+k*ladd;
				//							spt[Y]=Y_inori+wxp[WXPnum].bottomht;
				//							ept[X]=spt[X];
				//							ept[Y]=Y_inori+wxp[WXPnum].bottomht+gwht;
				//							if(k==0||k==5)
				//								makeline(spt,ept,1,0.5, L"������");
				//							else
				//				         	    makeline(spt,ept,8,0, L"������", L"DOT3");
				//
				//						}
				//					}
				//        
				//				}
				//				ibm++;


			}

		}

	}


	//	if(infwgx && ibm>0)//������
	//	{
	//	   AgwScml=BmCml[ibm-1];
	//	   AgwEcml=m_Ecml;
	//			
	//	   double sx,ex;
	//	   sx = mltosx(0,AgwScml);
	//	   ex = mltosx(0,AgwEcml);
	//	   while(sx<ex+0.001)
	//		{
	//			spt[X]=sx;
	//			spt[Y]=Y_inori+wxp[WXPnum].bottomht;
	//			ept[X]=spt[X];
	//			ept[Y]=Y_inori+wxp[WXPnum].bottomht+gwht;
	//			makeline(spt,ept,8,0, L"������", L"DOT3");
	//			sx = sx + 10;
	//		}
	//	}
	spt.x = mltosx(0,m_Scml);
	spt.y = yb;
	ept.x = spt.x;
	ept.y = yb+2.0;
	makeline(spt,ept,4,0, LayerName);
	spt.x = mltosx(0,m_Ecml);
	spt.y = yb;
	ept.x = spt.x;
	ept.y = yb+2.0;
	makeline(spt,ept,4,0, LayerName);

	spt.x=mltosx(0,m_Scml)+2.0;
	spt.y=yb+0.5;
	km  = (int) floor (m_Sdml /1000);
	ys = m_Sdml-km*1000.0;
	_stprintf(str, L"%s%d",SGH,km);
	maketext(spt,str,0.5*pi,1.8,4,1, L"����", LayerName);
	if(ys < 0.1)
		_stprintf (str, L"%s000", sjh);
	else if(ys < 10)
		_stprintf (str, L"%s00%0.2lf", sjh,ys);
	else if(ys < 100)
		_stprintf (str, L"%s0%0.2lf", sjh,ys);
	else
		_stprintf (str, L"%s%0.2lf", sjh,ys);
	spt.x+=2.0;
	maketext(spt,str,0.5*pi,1.8,4,1, L"����", LayerName);

	spt.x=mltosx(0,m_Ecml)-2.5;
	spt.y=yb+0.5;
	km  = (int) floor (m_Edml /1000);
	_stprintf(str, L"%s%d",EGH,km);
	maketext(spt,str,0.5*pi,1.8,4,1, L"����", LayerName);
	spt.x+=2.0;
	ys = m_Edml-km*1000.0;
	if(ys < 0.1)
		_stprintf (str, L"%s000", sjh);
	else if(ys < 10)
		_stprintf (str, L"%s00%0.2lf", sjh,ys);
	else if(ys < 100)
		_stprintf (str, L"%s0%0.2lf", sjh,ys);
	else
		_stprintf (str, L"%s%0.2lf", sjh,ys);
	maketext(spt,str,0.5*pi,1.8,4,1, L"����", LayerName);




}

void CZDMHT::ReadLMK(CString LMKTABname)
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(m_pm->mdbname,LMKTABname,m_pm->RoadName);
	//   ads_printf(L"lmk=%d\n",xlmdb.NLMK);
	FILE *fpr;
	CString name=m_pm->mdbname;
	name.MakeUpper();
	int pos,len,k;
	pos = name.Find(L"DATA");
	len = name.GetLength();
	name=name.Right(len-pos-5);
	CString fname;
	ReadWorkdir();
	fname =Cworkdir + "\\DATA\\" + "BXLMKD." + name + m_pm->RoadName;
	if((fpr=_wfopen(fname, L"r"))!=NULL)
	{
		BXLMKD bxtmp;
		fwscanf(fpr, L"%d",&BxNum);
		pBXArr = new BXLMKD[BxNum];
		for(k=0;k<BxNum;k++)
		{
			fwscanf(fpr, L"%lf%lf%d%d%d",&pBXArr[k].bxsml,&pBXArr[k].bxeml,&pBXArr[k].LorR,&pBXArr[k].bxinf,&pBXArr[k].ptnum);
			pBXArr[k].ptarr = new double[bxtmp.ptnum*3];
			for(int j=0;j<pBXArr[k].ptnum;j++)
				fwscanf(fpr, L"%lf%lf%lf",&pBXArr[k].ptarr[j*3],&pBXArr[k].ptarr[j*3+1],&pBXArr[k].ptarr[j*3+2]);
		}
		fclose(fpr); 
	}	

	if(xlmdb.NLMK<1)
		return;

	if(LMKTABname == "��·���ֶα�")
	{
		NZLMK = xlmdb.NLMK;
		ZLMK = new LMKdata[xlmdb.NLMK];
		for(int i=0;i<xlmdb.NLMK;i++)
		{
			ZLMK[i] = xlmdb.lmk[i];	
			ZLMK[i].cml = m_pm->TYLC(ZLMK[i].xlc);
		}
	}
	else
	{
		NYLMK = xlmdb.NLMK;
		YLMK = new LMKdata[xlmdb.NLMK];
		for(int i=0;i<xlmdb.NLMK;i++)
		{
			YLMK[i] = xlmdb.lmk[i];	
			YLMK[i].cml = m_pm->TYLC(YLMK[i].xlc);
		}
	}

	if(xlmdb.lmk)
		delete []xlmdb.lmk;
	xlmdb.lmk = NULL;
	xlmdb.NLMK = 0;

}

void CZDMHT::DrawBox_Desh(int wxpi)//����ʩ��ͼ��Ƹ߳�
{
	int   i,j, mess=0;
	ACHAR    str[80];
	double yb,desh;
	AcGePoint3d spt;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-��Ƹ߳�");

	yb = Y_inori+wxp[wxpi].bottomht;
	if(WayorRamp==0)//��·
	{
		for(i=0;i<pDBS->NTW;i++)
		{
			if (isBiaoZhuGaiLiCheng(m_Scml,m_Ecml,pDBS->TWB[i].cml) == false)
				continue;

			//20190829	�����������׮�Ƿ����
			if (!m_VDrawPara.IsDrawFeaturePile && i > 0 && i + 1 < pDBS->NTW)
			{
				if(IsFeaturePile(pDBS->TWB[i].cml))
					continue;
			}


			if(pDBS->TWB[i].cml >= m_Scml - 0.01 && pDBS->TWB[i].cml <= m_Ecml + 0.01)
			{
				spt.x=mltosx(0,pDBS->TWB[i].cml);
				if(fabs(pDBS->TWB[i].cml-m_Scml)<1)
					spt.x +=wxp[wxpi].texth;

				spt.y=yb+0.5;
				desh = rdes_level(pDBS->TWB[i].cml,pDBS);
				_stprintf (str, L"%0.2lf", desh);
				maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);
			}
		}
	}
	else//�ѵ�
	{
		HdmDes hdm;
		double RHF[10];
		AcGePoint3d ptb,pte;
		NZLMK=NYLMK=0;
		ZLMK=YLMK=NULL;
		//�������·���ֶ�
		ReadLMK("��·���ֶα�");
		ReadLMK("��·���ֶα�");

		for(i=0;i<pDBS->NTW;i++)
		{
			if(pDBS->TWB[i].cml >= m_Scml && pDBS->TWB[i].cml <= m_Ecml)
			{
				spt.x=mltosx(0,pDBS->TWB[i].cml);
				if(fabs(pDBS->TWB[i].cml-m_Scml)<1)
					spt.x +=wxp[wxpi].texth;
				if(fabs(pDBS->TWB[i].cml-m_Ecml)<1)
					spt.x -=0.5*wxp[wxpi].texth;
				desh = rdes_level(pDBS->TWB[i].cml,pDBS);
				hdm.CalLMPT(pDBS->TWB[i].cml,desh,NZCG,ZCG,NZJK,
					ZJK,NZLMK,ZLMK,RHF,-1,BxNum,pBXArr,m_pm);
				double DesHt=yb;
				for(j=0;j<4;j++)//���4��
				{
					if(InfLmPt[j])
					{
						ptb.x = Xd_ori-10;
						ptb.y=DesHt;
						pte.x=SDAI[NSDAI-1][1];
						pte.y=DesHt;
						makeline(ptb,pte,3,0.0, LayerName);
						spt.y=DesHt+0.5;
						_stprintf(str, L"%0.2lf", RHF[9-j*2]);
						maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);

						_stprintf(str, L"%s",LmPtName[j]);
						ptb.x = Xd_ori - 5;
						ptb.y = DesHt + 0.5*wxp[wxpi].Lht;
						maketext(ptb,str,0,3.5,2,3, L"HZ", LayerName);
						DesHt = DesHt+wxp[wxpi].Lht;
					}
				}

				hdm.CalLMPT(pDBS->TWB[i].cml,desh,NYCG,YCG,NYJK,
					YJK,NYLMK,YLMK,RHF,1,BxNum,pBXArr,m_pm);
				for(j=4;j<8;j++)//�Ҳ�4��
				{
					if(InfLmPt[j])
					{
						ptb.x = Xd_ori-10;
						ptb.y=DesHt;
						pte.x=SDAI[NSDAI-1][1];
						pte.y=DesHt;
						makeline(ptb,pte,3,0.0, LayerName);
						spt.y=DesHt+0.5;
						if(j==4)
							_stprintf(str, L"%0.2lf", RHF[3]);
						else if(j==5)
							_stprintf(str, L"%0.2lf", RHF[5]);
						else if(j==6)
							_stprintf(str, L"%0.2lf", RHF[7]);
						else
							_stprintf(str, L"%0.2lf", RHF[9]);
						maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);
						_stprintf(str, L"%s",LmPtName[j]);
						ptb.x = Xd_ori - 5;
						ptb.y = DesHt + 0.5*wxp[wxpi].Lht;
						maketext(ptb,str,0,3.5,2,3, L"HZ", LayerName);
						DesHt = DesHt+wxp[wxpi].Lht;
					}
				}
				ptb.x = Xd_ori-10; ptb.y = yb;
				pte.x = ptb.x; pte.y = DesHt;
				makeline(ptb,pte,3,0, LayerName);
				ptb.x = Xd_ori-20; ptb.y = yb + 0.5*(DesHt-yb);
				_stprintf(str, L"�� �� �� ��");
				maketext(ptb,str,0.5*pi,3.5,2,3, L"����", LayerName);
			}
		}
	}
}

void CZDMHT::DrawBox_DMGC(int wxpi)//���Ƶ���߳�
{
	int   i,mess=0;
	ACHAR    str[80];
	double yb;
	AcGePoint3d spt;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-����߳�");

	yb = Y_inori+wxp[wxpi].bottomht;
	for(i=0;i<pDBS->NTW;i++)
	{
		if (isBiaoZhuGaiLiCheng(m_Scml,m_Ecml,pDBS->TWB[i].cml) == false)
			continue;

		//20190829	�����������׮�Ƿ����
		if (!m_VDrawPara.IsDrawFeaturePile && i > 0 && i + 1 < pDBS->NTW)
		{
			if (IsFeaturePile(pDBS->TWB[i].cml))
				continue;
		}

		if(pDBS->TWB[i].cml >= m_Scml - 0.01 && pDBS->TWB[i].cml <= m_Ecml + 0.01)
		{
			spt.x=mltosx(0,pDBS->TWB[i].cml);
			if(fabs(pDBS->TWB[i].cml-m_Scml)<1)
				spt.x += wxp[wxpi].texth;
			//if(fabs(pDBS->TWB[i].cml-m_Ecml)<1)
			//	spt.x -= 0.2*wxp[wxpi].texth;
			spt.y=yb+0.5;			
			_stprintf (str, L"%0.2lf", pDBS->TWB[i].DMGC);
			maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,1, L"����", LayerName);
		}
	}
}

void CZDMHT::DrawBox_SLOP(int wxpi)//������Ŀ���¶�
{
	int   i, sno,eno;
	double  qzz,hzz,sxpinf;
	ACHAR    str[80],pdstr[40],pzcstr[40],pbcstr[40];
	double yb,ym,yu,ytmp,ys,i1;
	AcGePoint3d spt,ept;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-�¶��³�");

	yb = Y_inori+wxp[wxpi].bottomht;
	yu = yb+wxp[wxpi].Lht;
	ym = 0.5*(yb+yu);

	if(fabs(pDBS->PDB[0].cml-m_Scml)<0.1)
	{
		//spt.x = mltosx(0,m_Scml)+2.0;
		spt.x = mltosx(0,m_Scml) + wxp[wxpi].texth;
		spt.y = ym;
		_stprintf(str, L"%0.2lf",pDBS->PDB[0].Level);
		//maketext(spt,str,0.5*pi,1.5,2,3, L"����");
		maketext(spt,str,0.5*pi,wxp[wxpi].texth,2,3, L"����", LayerName);
	}
	if(fabs(pDBS->PDB[pDBS->NPD-1].cml-m_Ecml)<0.1)
	{
		//spt.x = mltosx(0,m_Ecml)-0.5;
		spt.x = mltosx(0,m_Ecml);
		spt.y = ym;
		ys = fmod(fabs(pDBS->PDB[pDBS->NPD-1].ml)+0.001,1000.0);
		_stprintf(str, L"+%0.2lf",ys);
		//maketext(spt,str,0.5*pi,1.5,2,3, L"����");
		maketext(spt,str,0.5*pi,wxp[wxpi].texth,2,3, L"����", LayerName);
	}

	sno = 0;
	for(i=0; i<pDBS->NPD;i++)
		if(pDBS->PDB[i].cml>m_Scml+0.01)//��������ȷ���¶�������
		{
			sno=i;
			break;
		}

		eno=pDBS->NPD-1;
		for(i=0; i<pDBS->NPD;i++)
			if(pDBS->PDB[i].cml>=m_Ecml)//���յ����ȷ���¶����յ��
			{
				eno=i;
				break;
			}

			for(i=sno;i<=eno;i++)
			{
				if(pDBS->PDB[i].cml>m_Scml&&pDBS->PDB[i].cml<m_Ecml)
				{//���µ��ڱ�ͼ��
					spt.x = mltosx(0,pDBS->PDB[i].cml);
					spt.y = yb;
					ept.x = spt.x;
					ept.y = yu;
					makeline(spt,ept,2,0, LayerName);
					//spt.x-=1.5;
					spt.x -= wxp[wxpi].texth;
					spt.y = ym;
					ys = fmod(fabs(pDBS->PDB[i].ml)+0.001,1000.0);
					_stprintf(str, L"+%0.2lf",ys);
					//maketext(spt,str,0.5*pi,1.5,2,3, L"����");
					maketext(spt,str,0.5*pi,wxp[wxpi].texth,2,3, L"����", LayerName);
					//ept.x +=1.5;
					ept.x += wxp[wxpi].texth;
					ept.y = ym;
					_stprintf(str, L"%0.2lf",pDBS->PDB[i].Level);
					//maketext(ept,str,0.5*pi,1.5,2,3, L"����");
					maketext(ept,str,0.5*pi,wxp[wxpi].texth,2,3, L"����", LayerName);
				}

				i1=(pDBS->PDB[i].Level-pDBS->PDB[i-1].Level)*100/(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml);
				if(i1 > 0.00001)//����
				{
					ytmp = yb;
					sxpinf = 1;
				}
				else if(i1 < -0.000001)//����
				{
					ytmp = yu;
					sxpinf = -1;
				}
				else//ƽ��
				{
					ytmp = ym;
					//sxpinf = 0.5;
					sxpinf = 0.0;	//20190710	�޸�Ϊ0.0	ƽ��Ӧ��ֱ�ӻ�ֱ�ߣ�
				}

				ads_rtos(i1,2,(int)(pDBS->PDB[0].Rshu+0.001),pdstr);
				//_stprintf(pdstr, L"%0.3lf%c",i1,'%');
				ads_rtos(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml,2,2,pzcstr);

				if(pDBS->PDB[i-1].cml<m_Scml&&pDBS->PDB[i].cml>m_Ecml)//ǰһ�����µ�͵�ǰ���µ㲻�ڱ�ͼ��
				{
					spt.x = mltosx(0,m_Scml);
					qzz = sxpinf*(m_Scml - pDBS->PDB[i-1].cml)*wxp[wxpi].Lht/(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml);
					spt.y = ytmp+qzz;
					hzz = sxpinf*(m_Ecml-pDBS->PDB[i-1].cml)*wxp[wxpi].Lht/(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml);
					ept.x = mltosx(0,m_Ecml);
					ept.y = ytmp+hzz;
					makeline(spt,ept,2,0, LayerName);
					spt.x = mltosx(0,0.5*(m_Scml+m_Ecml));
					spt.y = ym+0.25*wxp[wxpi].Lht;
					maketext(spt,pdstr,0,wxp[wxpi].texth,2,3, L"����", LayerName);
					spt.y = ym-0.25*wxp[wxpi].Lht;
					ads_rtos(m_Ecml-m_Scml,2,2,pbcstr);
					//_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
					if (_tcscmp(pbcstr, pzcstr)==0)
						_stprintf(str, L"%s",pbcstr);
					else
						_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
					maketext(spt,str,0,wxp[wxpi].texth,2,3, L"����", LayerName);
				}
				else if(pDBS->PDB[i-1].cml<m_Scml)//ǰһ�����µ㲻�ڱ�ͼ�ں͵�ǰ���µ��ڱ�ͼ��
				{
					spt.x = mltosx(0,m_Scml);
					qzz = sxpinf*(m_Scml - pDBS->PDB[i-1].cml)*wxp[wxpi].Lht/(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml);
					spt.y = ytmp+qzz;    
					ept.x = mltosx(0,pDBS->PDB[i].cml);
					ept.y = ytmp+sxpinf*wxp[wxpi].Lht;
					makeline(spt,ept,2,0, LayerName);
					if(fabs(qzz)<0.5*wxp[wxpi].Lht)
					{
						spt.x = mltosx(0,0.5*(pDBS->PDB[i].cml+pDBS->PDB[i-1].cml));
						spt.y = ym+0.25*wxp[wxpi].Lht;
						maketext(spt,pdstr,0,wxp[wxpi].texth,2,3, L"����", LayerName);
						spt.y = ym-0.25*wxp[wxpi].Lht;
						ads_rtos(pDBS->PDB[i].cml-m_Scml,2,2,pbcstr);
						//_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						if (_tcscmp(pbcstr, pzcstr)==0)
							_stprintf(str, L"%s",pbcstr);
						else
							_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						maketext(spt,str,0,wxp[wxpi].texth,2,3, L"����", LayerName);
					}
					else
					{
						spt.x = mltosx(0,m_Scml)+5*wxp[wxpi].texth;
						spt.y = ym+0.25*wxp[wxpi].Lht;
						maketext(spt,pdstr,0,wxp[wxpi].texth,2,3, L"����", LayerName);
						spt.x = mltosx(0,pDBS->PDB[i].cml)-5*wxp[wxpi].texth;
						spt.y = ym-0.25*wxp[wxpi].Lht;
						ads_rtos(pDBS->PDB[i].cml-m_Scml,2,2,pbcstr);
						//_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						if (_tcscmp(pbcstr, pzcstr)==0)
							_stprintf(str, L"%s",pbcstr);
						else
							_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						maketext(spt,str,0,wxp[wxpi].texth,2,3, L"����", LayerName);
					}
				}
				else if(pDBS->PDB[i].cml>m_Ecml)//ǰһ�����µ��ڱ�ͼ�ں͵�ǰ���µ㲻�ڱ�ͼ��
				{
					spt.x = mltosx(0,pDBS->PDB[i-1].cml);
					spt.y = ytmp;    
					hzz = sxpinf*(m_Ecml-pDBS->PDB[i-1].cml)*wxp[wxpi].Lht/(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml);
					ept.x = mltosx(0,m_Ecml);
					ept.y = ytmp+hzz;
					makeline(spt,ept,2,0, LayerName);
					if(fabs(hzz)>0.5*wxp[wxpi].Lht)
					{
						spt.x = mltosx(0,0.5*(pDBS->PDB[i].cml+pDBS->PDB[i-1].cml));
						spt.y = ym+0.25*wxp[wxpi].Lht;
						maketext(spt,pdstr,0,wxp[wxpi].texth,2,3, L"����", LayerName);
						spt.y = ym-0.25*wxp[wxpi].Lht;
						ads_rtos(m_Ecml-pDBS->PDB[i-1].cml,2,2,pbcstr);
						//_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						if (_tcscmp(pbcstr, pzcstr)==0)
							_stprintf(str, L"%s",pbcstr);
						else
							_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						maketext(spt,str,0,wxp[wxpi].texth,2,3, L"����", LayerName);
					}
					else
					{
						spt.x = mltosx(0,pDBS->PDB[i-1].cml)+5*wxp[wxpi].texth;
						spt.y = ym+0.25*wxp[wxpi].Lht;
						maketext(spt,pdstr,0,wxp[wxpi].texth,2,3, L"����", LayerName);
						spt.x = mltosx(0,pDBS->PDB[i].cml)-5*wxp[wxpi].texth;
						spt.y = ym-0.25*wxp[wxpi].Lht;
						ads_rtos(m_Ecml-pDBS->PDB[i-1].cml,2,2,pbcstr);
						//_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						if (_tcscmp(pbcstr, pzcstr)==0)
							_stprintf(str, L"%s",pbcstr);
						else
							_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
						maketext(spt,str,0,wxp[wxpi].texth,2,3, L"����", LayerName);
					}				 
				}
				else//���ڱ�ͼ��
				{
					spt.x = mltosx(0,pDBS->PDB[i-1].cml);
					spt.y = ytmp;    
					hzz = sxpinf*wxp[wxpi].Lht;
					ept.x = mltosx(0,pDBS->PDB[i].cml);
					ept.y = ytmp+hzz;
					makeline(spt,ept,2,0, LayerName);

					spt.x = mltosx(0,0.5*(pDBS->PDB[i].cml+pDBS->PDB[i-1].cml));
					spt.y = ym+0.25*wxp[wxpi].Lht;
					maketext(spt,pdstr,0,wxp[wxpi].texth,2,3, L"����", LayerName);
					spt.y = ym-0.25*wxp[wxpi].Lht;
					ads_rtos(pDBS->PDB[i].cml-pDBS->PDB[i-1].cml,2,2,pbcstr);
					if (_tcscmp(pbcstr, pzcstr)==0)
						_stprintf(str, L"%s",pbcstr);
					else
						_stprintf(str, L"%s(%s)",pbcstr,pzcstr);
					maketext(spt,str,0,wxp[wxpi].texth,2,3, L"����", LayerName);
				}
			}
}

void CZDMHT::DrawBox_DZGK(int wxpi)
{
	FILE *fpr;
	double ym,sdml,edml,scml,ecml;
	ACHAR str[800];
	AcGePoint3d spt,ept,tpt;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-���ʸſ�");

	fpr = _wfopen(m_dzgkname, L"r");
	if(fpr)
	{
		ym  = Y_inori+wxp[wxpi].bottomht+0.5*wxp[wxpi].Lht;//���м�߶�  		
		while(!feof(fpr))
		{
			fwscanf(fpr, L"%lf%lf",&sdml,&edml);
			fwscanf(fpr, L"%s",str);
			scml = m_pm->TYLC(sdml);
			ecml = m_pm->TYLC(edml);
			if(scml<=m_Scml && ecml >=m_Ecml)
			{
				spt.x = mltosx(0,m_Scml)+1.0;
				spt.y = ym;
				ept.x = mltosx(0,m_Ecml)-1.0;
				ept.y = ym;
				//				 maketext(spt,ept,str,0.0,2.5);	
				tpt.x = 0.5*(spt.x+ept.x);
				tpt.y = spt.y;
				maketext(tpt, str, 0, wxp[wxpi].texth, 0, 3, LayerName);
			}
			else if(scml<m_Scml && ecml < m_Ecml && ecml > m_Scml)
			{
				spt.x = mltosx(0,ecml);
				spt.y = ym - 0.5*wxp[wxpi].Lht;
				ept.x = spt.x;
				ept.y = ym + 0.5*wxp[wxpi].Lht;
				makeline(spt,ept,0,0, LayerName);
				spt.x = mltosx(0,m_Scml)+1.0;
				spt.y = ym;
				ept.x = mltosx(0,ecml)-1.0;
				ept.y = ym;
				//				 maketext(spt,ept,str,0.0,2.5);	
				tpt.x = 0.5*(spt.x+ept.x);
				tpt.y = spt.y;
				maketext(tpt,str,0,wxp[wxpi].texth,0,3, LayerName);
			}
			else if(scml>m_Scml && ecml > m_Ecml && scml < m_Ecml)
			{
				spt.x = mltosx(0,scml);
				spt.y = ym - 0.5*wxp[wxpi].Lht;
				ept.x = spt.x;
				ept.y = ym + 0.5*wxp[wxpi].Lht;
				makeline(spt,ept,0,0, LayerName);
				spt.x = mltosx(0,scml)+1.0;
				spt.y = ym;
				ept.x = mltosx(0,m_Ecml)-1.0;
				ept.y = ym;
				//				 maketext(spt,ept,str,0.0,2.5);	
				tpt.x = 0.5*(spt.x+ept.x);
				tpt.y = spt.y;
				maketext(tpt,str,0,wxp[wxpi].texth,0,3, LayerName);
			}
			else if(scml>m_Scml && ecml < m_Ecml)
			{
				spt.x = mltosx(0,scml);
				spt.y = ym - 0.5*wxp[wxpi].Lht;
				ept.x = spt.x;
				ept.y = ym + 0.5*wxp[wxpi].Lht;
				makeline(spt, ept, 0, 0, LayerName);
				spt.x = mltosx(0,ecml);
				spt.y = ym - 0.5*wxp[wxpi].Lht;
				ept.x = spt.x;
				ept.y = ym + 0.5*wxp[wxpi].Lht;
				makeline(spt, ept, 0, 0, LayerName);
				spt.x = mltosx(0,scml)+1.0;
				spt.y = ym;
				ept.x = mltosx(0,ecml)-1.0;
				ept.y = ym;
				//				 maketext(spt,ept,str,0.0,2.5);	
				tpt.x = 0.5*(spt.x+ept.x);
				tpt.y = spt.y;
				maketext(tpt, str, 0, wxp[wxpi].texth, 0, 3, LayerName);
			}
		}	
		fclose(fpr);
	}
}

void CZDMHT::DrawBox_PLAN(int wxpi)//������Ŀ��ƽ��
{
	double  ym,  yl, yr;
	double  sspt[3],eept[3];
	double  len;
	double  ang,l0;
	ACHAR    str[80],gh[20],mm[4],ss[4],Rstr[40],ZjStr[30], ACHAR_LorR[2];
	int i,sno,eno,du;
	AcGePoint3dArray Ptarr;
	AcGePoint3d spt,ept,tpt,tmpt;
	ACHAR LayerName_Line[256], LayerName_Text[256], LayerName_Auxiliary[256];
	_tcscpy(LayerName_Line, L"����ͼ-ֱ�߼�ƽ����-ƽ������");
	_tcscpy(LayerName_Text, L"����ͼ-ֱ�߼�ƽ����-ƽ����Ϣ");
	_tcscpy(LayerName_Auxiliary, L"����ͼ-ֱ�߼�ƽ����");

	ym  = Y_inori+wxp[wxpi].bottomht+0.5*wxp[wxpi].Lht;//ƽ�����м�߶�   
	yr  = ym+wxp[wxpi].Lht/4.0;//ƽ�����ϲ�Բ���߸߶�,����turn right
	yl  = ym-wxp[wxpi].Lht/4.0;//ƽ�����²�Բ���߸߶�,����turn left

	sno = 0;
	for (i = 1; i <= m_pm->XYNum; i++)
	{
		if (m_pm->XYArray[i][6] > m_Scml + 0.01)//��������ȷ����Ԫ����
		{
			sno = i;
			break;
		}
	}
	eno = m_pm->XYNum;
	for (i = 0; i <= m_pm->XYNum; i++)
	{
		if (m_pm->XYArray[i][6] >= m_Ecml)//���յ����ȷ����Ԫ�յ��
		{
			eno = i;
			break;
		}
	}

	for (i = sno - 1; i<eno; i++)
	{
		len = m_pm->XYArray[i + 1][6] - m_pm->XYArray[i][6];
		spt[X] = mltosx(0, m_pm->XYArray[i][6]);
		ept[X] = mltosx(0, m_pm->XYArray[i + 1][6]);
		tpt[X] = mltosx(0, 0.5*(m_pm->XYArray[i + 1][6] + m_pm->XYArray[i][6]));
		if (m_pm->XYArray[i][0] < 1.2) //��ǰ��i����ԪΪֱ��
		{
			spt[Y] = ept[Y] = ym;
			tpt[Y] = ym + 0.5 * wxp[wxpi].texth + 1.0;
			ads_rtos(len, 2, 2, str);
			if (i == sno - 1)//��ʼ��Ԫ�����ڱ�ͼ����
			{
				spt[X] = mltosx(0, m_Scml);
				if (m_pm->XYArray[i + 1][6] - m_Scml>m_Scml - m_pm->XYArray[i][6])//��Ԫ����ڱ�����
				{
					maketext(tpt, str, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
					//20190710	����ֱ���������ֻ���
					_tcscpy(str, L"R-��");
					tpt[Y] = ym - 0.5 * wxp[wxpi].texth - 1.0;
					maketext(tpt, str, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}
			}
			else if (i == eno - 1)//��ֹ��Ԫ�����ڱ�ͼ����
			{
				ept[X] = mltosx(0, m_Ecml);
				if (m_Ecml - m_pm->XYArray[i][6] > m_pm->XYArray[i + 1][6] - m_Ecml)//��Ԫ����ڱ�����
				{
					maketext(tpt, str, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
					//20190710	����ֱ���������ֻ���
					_tcscpy(str, L"R-��");
					tpt[Y] = ym - 0.5 * wxp[wxpi].texth - 1.0;
					maketext(tpt, str, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}
			}
			else //�ڱ�ͼ���ڵ���Ԫ
			{
				maketext(tpt, str, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				//20190710	����ֱ���������ֻ���
				_tcscpy(str, L"R-��");
				tpt[Y] = ym - 0.5 * wxp[wxpi].texth - 1.0;
				maketext(tpt, str, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
			}
			makeline(spt, ept, 4, 0.3, LayerName_Line);
			if (m_pm->XYArray[i + 1][6]>m_Scml && m_pm->XYArray[i + 1][6]<m_Ecml)//�յ��һ����
			{
				spt.x = mltosx(0, m_pm->XYArray[i + 1][6]);
				spt.y = ym;
				ept.x = spt.x; ept.y = ym + 2;
				makeline(spt, ept, 4, 0.0, LayerName_Auxiliary);		//ͻ����ֱ��	20190710	�Ƿ񲢲���Ҫ
			}
		}
		else if (m_pm->XYArray[i][0] < 2.2)//��ǰ��i����ԪΪԲ
		{
			if (m_pm->XYArray[i][4] < 0)//��ת
			{
				spt[Y] = ept[Y] = yl;
				if (m_pm->XYArray[i + 1][6]>m_Scml && m_pm->XYArray[i + 1][6]<m_Ecml)//�յ��һ����
				{
					tmpt.x = ept.x;
					tmpt.y = yl + 2.0;
					makeline(ept, tmpt, 4, 0.0, LayerName_Auxiliary);
				}
				tpt[Y] = yl + 1.8 * wxp[wxpi].texth + 2.5;	//20190711	�޸ĳ�1.8
				tmpt.x = tpt.x;
				tmpt.y = yl - 0.5 * wxp[wxpi].texth - 1.0;
				_tcscpy(ACHAR_LorR, L"Z");
			}
			else//��ת
			{
				spt[Y] = ept[Y] = yr;
				if (m_pm->XYArray[i + 1][6] > m_Scml && m_pm->XYArray[i + 1][6] < m_Ecml)//�յ��һ����
				{
					tmpt.x = ept.x;
					tmpt.y = yr + 2.0;
					makeline(ept, tmpt, 4, 0.0, LayerName_Auxiliary);
				}
				tpt[Y] = yr - 1.5 * wxp[wxpi].texth - 2.5;
				tmpt.x = tpt.x;
				tmpt.y = yr + 0.5 * wxp[wxpi].texth + 1.0;
				_tcscpy(ACHAR_LorR, L"Y");
			}

			int jdn = m_pm->XYJDNO[i];
			ads_rtos(m_pm->XYArray[i][1], 2, 2, Rstr);
			atodmss(dms_rad(fabs(m_pm->JdCenArray[jdn].afa)), du, mm, ss);
			_stprintf(ZjStr, L"a-%d��%s%s(%s)", du, mm, ss, ACHAR_LorR);	//�������ұ�־

			_stprintf(str, L"R-%s\n%s\nL-%0.2lf", Rstr, ZjStr, m_pm->JdCenArray[jdn].L);
			_stprintf(gh, L"JD%0.0lf", m_pm->JdCenArray[jdn].JdNo);
			if (i == sno - 1)//��ʼ��Ԫ�����ڱ�ͼ����
			{
				spt[X] = mltosx(0, m_Scml);
				if (m_pm->XYArray[i + 1][6] - m_Scml>m_Scml - m_pm->XYArray[i][6])//��Ԫ����ڱ�����
				{
					MakeMText(tpt, str, 0, wxp[wxpi].texth, 100, 4, AcDbMText::AttachmentPoint::kMiddleCenter, L"����", LayerName_Text, 0.8);
					//maketext(tpt,str,0,wxp[wxpi].texth,4,3);
					maketext(tmpt, gh, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}
			}
			else if (i == eno - 1)//��ֹ��Ԫ�����ڱ�ͼ����
			{
				ept[X] = mltosx(0, m_Ecml);
				if (m_Ecml - m_pm->XYArray[i][6] > m_pm->XYArray[i + 1][6] - m_Ecml)//��Ԫ����ڱ�����
				{
					MakeMText(tpt, str, 0, wxp[wxpi].texth, 100, 4, AcDbMText::AttachmentPoint::kMiddleCenter, L"����", LayerName_Text, 0.8);
					//maketext(tpt,str,0,wxp[wxpi].texth,4,3);
					maketext(tmpt, gh, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}
			}
			else //�ڱ�ͼ���ڵ���Ԫ
			{
				MakeMText(tpt, str, 0, wxp[wxpi].texth, 100, 4, AcDbMText::AttachmentPoint::kMiddleCenter, L"����", LayerName_Text, 0.8);
				//maketext(tpt,str,0,wxp[wxpi].texth,4,3);
				maketext(tmpt, gh, 0, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
			}

			makeline(spt, ept, 4, 0.3, LayerName_Line);

			if (fabs(m_pm->XYArray[i - 1][0] - 1)<0.01 || ((fabs(m_pm->XYArray[i - 1][0] - 4)<0.01 || fabs(m_pm->XYArray[i - 1][0] - 2)<0.01) && m_pm->XYArray[i - 1][4] * m_pm->XYArray[i][4]<-0.1))//ǰһ��ԪΪֱ�� ��Ϊ�󻺻�Բ��ת���෴
			{
				if (m_pm->XYArray[i][4]<0)//��ת
				{
					spt[Y] = ym;
					tpt[X] = spt[X];
					tpt[Y] = yl;
				}
				else//��ת
				{
					spt[Y] = ym;
					tpt[X] = spt[X];
					tpt[Y] = yr;
				}
				if (m_pm->XYArray[i][6]>m_Scml)
					makeline(spt, tpt, 4, 0.3, LayerName_Line);
			}
			if (fabs(m_pm->XYArray[i + 1][0] - 1) < 0.01 || ((fabs(m_pm->XYArray[i + 1][0] - 3)<0.01 || fabs(m_pm->XYArray[i + 1][0] - 2)<0.01) && m_pm->XYArray[i][4] * m_pm->XYArray[i + 1][4]<-0.1))//��һ��ԪΪֱ�� ��Ϊǰ����ת���෴
			{
				if (m_pm->XYArray[i][4]<0)//��ת
				{
					ept[Y] = ym;
					tpt[X] = ept[X];
					tpt[Y] = yl;
				}
				else//��ת
				{
					ept[Y] = ym;
					tpt[X] = ept[X];
					tpt[Y] = yr;
				}
				if (m_pm->XYArray[i + 1][6]<m_Ecml)
					makeline(ept, tpt, 4, 0.3, LayerName_Line);
			}
		}
		else if (m_pm->XYArray[i][0]<3.2)//��ǰ��i����ԪΪǰ��
		{
			spt[Y] = ym;
			if (m_pm->XYArray[i][4]<0)//��ת
			{
				ept[Y] = yl;
				tpt[Y] = 0.5*(ym + yl);
				if (m_pm->XYArray[i + 1][6]>m_Scml && m_pm->XYArray[i + 1][6]<m_Ecml)//�յ��һ����
				{
					tmpt.x = ept.x;
					tmpt.y = yl + 2.0;
					makeline(ept, tmpt, 4, 0.0, LayerName_Line);
				}
			}
			else//��ת
			{
				ept[Y] = yr;
				tpt[Y] = 0.5*(ym + yr);
				if (m_pm->XYArray[i + 1][6]>m_Scml && m_pm->XYArray[i + 1][6]<m_Ecml)//�յ��һ����
				{
					tmpt.x = ept.x;
					tmpt.y = yr + 2.0;
					makeline(ept, tmpt, 4, 0.0, LayerName_Line);
				}
			}

			l0 = pzLinep->XYArray[i][1] * pzLinep->XYArray[i][1] / pzLinep->XYArray[i + 1][7];
			_stprintf(Rstr, L"l-%0.2lf", l0);
			_stprintf(str, L"A-%0.2lf", pzLinep->XYArray[i][1]);
			if (i == sno - 1)//��ʼ��Ԫ�����ڱ�ͼ����
			{
				spt[X] = mltosx(0, m_Scml);
				spt[Y] = ym + (m_Scml - m_pm->XYArray[i][6])*(ept[Y] - ym) / len;
				sspt[0] = spt[X];
				sspt[1] = spt[Y];
				eept[0] = ept[X];
				eept[1] = ept[Y];
				ang = ads_angle(sspt, eept);
				if (m_pm->XYArray[i + 1][6] - m_Scml>m_Scml - m_pm->XYArray[i][6])//��Ԫ����ڱ�����
				{
					tmpt.x = tpt.x;
					tmpt.y = tpt.y + 0.5*wxp[wxpi].texth + 1.0;
					maketext(tmpt, str, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
					tmpt.y = tpt.y - 0.5*wxp[wxpi].texth - 1.0;
					maketext(tmpt, Rstr, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}

			}
			else if (i == eno - 1)//��ֹ��Ԫ�����ڱ�ͼ����
			{
				ept[X] = mltosx(0, m_Ecml);
				ept[Y] = ym + (m_Ecml - m_pm->XYArray[i][6])*(ept[Y] - ym) / len;
				sspt[0] = spt[X];
				sspt[1] = spt[Y];
				eept[0] = ept[X];
				eept[1] = ept[Y];
				ang = ads_angle(sspt, eept);
				if (m_Ecml - m_pm->XYArray[i][6] > m_pm->XYArray[i + 1][6] - m_Ecml)//��Ԫ����ڱ�����
				{
					tmpt.x = tpt.x;
					tmpt.y = tpt.y + 0.5*wxp[wxpi].texth + 1.0;
					maketext(tmpt, str, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
					tmpt.y = tpt.y - 0.5*wxp[wxpi].texth - 1.0;
					maketext(tmpt, Rstr, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}
			}
			else //�ڱ�ͼ���ڵ���Ԫ
			{
				sspt[0] = spt[X];
				sspt[1] = spt[Y];
				eept[0] = ept[X];
				eept[1] = ept[Y];
				ang = ads_angle(sspt, eept);
				tmpt.x = tpt.x;
				tmpt.y = tpt.y + 0.5*wxp[wxpi].texth + 1.0;
				maketext(tmpt, str, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				tmpt.y = tpt.y - 0.5*wxp[wxpi].texth - 1.0;
				maketext(tmpt, Rstr, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
			}

			makeline(spt, ept, 4, 0.3, LayerName_Line);

		}
		else if (pzLinep->XYArray[i][0]<4.2)//��ǰ��i����ԪΪ��
		{
			ept[Y] = ym;
			if (pzLinep->XYArray[i][4]<0)//��ת
			{
				spt[Y] = yl;
				tpt[Y] = 0.5*(ym + yl);
				if (m_pm->XYArray[i + 1][6]>m_Scml && m_pm->XYArray[i + 1][6]<m_Ecml)//�յ��һ����
				{
					tmpt.x = ept.x;
					tmpt.y = ym + 2.0;
					makeline(ept, tmpt, 4, 0.0, LayerName_Line);
				}
			}
			else//��ת
			{
				spt[Y] = yr;
				tpt[Y] = 0.5*(ym + yr);
				if (m_pm->XYArray[i + 1][6]>m_Scml && m_pm->XYArray[i + 1][6]<m_Ecml)//�յ��һ����
				{
					tmpt.x = ept.x;
					tmpt.y = ym + 2.0;
					makeline(ept, tmpt, 4, 0.0, LayerName_Line);
				}
			}
			l0 = pzLinep->XYArray[i][1] * pzLinep->XYArray[i][1] / pzLinep->XYArray[i][7];
			_stprintf(Rstr, L"l-%0.2lf", l0);
			_stprintf(str, L"A-%0.2lf", pzLinep->XYArray[i][1]);

			if (i == sno - 1)//��ʼ��Ԫ�����ڱ�ͼ����
			{
				spt[X] = mltosx(0, m_Scml);
				spt[Y] = spt[Y] + (m_Scml - m_pm->XYArray[i][6])*(ym - spt[Y]) / len;
				sspt[0] = spt[X];
				sspt[1] = spt[Y];
				eept[0] = ept[X];
				eept[1] = ept[Y];
				ang = ads_angle(sspt, eept);
				if (m_pm->XYArray[i + 1][6] - m_Scml>m_Scml - m_pm->XYArray[i][6])//��Ԫ����ڱ�����
				{
					tmpt.x = tpt.x;
					tmpt.y = tpt.y + 0.5*wxp[wxpi].texth + 1.0;
					maketext(tmpt, str, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
					tmpt.y = tpt.y - 0.5*wxp[wxpi].texth - 1.0;
					maketext(tmpt, Rstr, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}

			}
			else if (i == eno - 1)//��ֹ��Ԫ�����ڱ�ͼ����
			{
				ept[X] = mltosx(0, m_Ecml);
				ept[Y] = spt[Y] + (m_Ecml - m_pm->XYArray[i][6])*(ym - spt[Y]) / len;
				sspt[0] = spt[X];
				sspt[1] = spt[Y];
				eept[0] = ept[X];
				eept[1] = ept[Y];
				ang = ads_angle(sspt, eept);
				if (m_Ecml - m_pm->XYArray[i][6] > m_pm->XYArray[i + 1][6] - m_Ecml)//��Ԫ����ڱ�����
				{
					tmpt.x = tpt.x;
					tmpt.y = tpt.y + 0.5*wxp[wxpi].texth + 1.0;
					maketext(tmpt, str, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
					tmpt.y = tpt.y - 0.5*wxp[wxpi].texth - 1.0;
					maketext(tmpt, Rstr, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				}
			}
			else //�ڱ�ͼ���ڵ���Ԫ
			{
				sspt[0] = spt[X];
				sspt[1] = spt[Y];
				eept[0] = ept[X];
				eept[1] = ept[Y];
				ang = ads_angle(sspt, eept);
				tmpt.x = tpt.x;
				tmpt.y = tpt.y + 0.5*wxp[wxpi].texth + 1.0;
				maketext(tmpt, str, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
				tmpt.y = tpt.y - 0.5*wxp[wxpi].texth - 1.0;
				maketext(tmpt, Rstr, ang, wxp[wxpi].texth, 4, 3, L"����", LayerName_Text);
			}

			makeline(spt, ept, 4, 0.3, LayerName_Line);

		}
		/*
		else if(pzLinep->XYArray[i][0]<5.2)//��ǰ��i����ԪΪ��������R��->RС
		{
		if(pzLinep->XYArray[i][4]<0)//��ת
		{
		spt[Y]=ept[Y]=yl;
		tpt[Y] = ym +texth;
		}
		else//��ת
		{
		spt[Y]=ept[Y]=yr;
		tpt[Y] = ym - 1.0 -texth;
		}
		G_makeline(pWd,spt,ept,1);
		}
		else//��ǰ��i����ԪΪ��������RС->R��
		{
		if(pzLinep->XYArray[i][4]<0)//��ת
		{
		spt[Y]=ept[Y]=yl;
		tpt[Y] = ym +texth;
		}
		else//��ת
		{
		spt[Y]=ept[Y]=yr;
		tpt[Y] = ym - 1.0 -texth;
		}
		G_makeline(pWd,spt,ept,1);
		}*/

	}



}

void CZDMHT::DrawBox_Cg(int wxpi)//������Ŀ�г���
{
	double  ym;
	double  len;
	ACHAR    str[80];
	int i,sno,eno;	
	AcGePoint3d spt,ept,tpt,tmpt;//�Ҳ���� ����ɫ ���ɫ
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-����");

	ym  = Y_inori+wxp[wxpi].bottomht+0.5*wxp[wxpi].Lht;//ƽ�����м�߶� 

	//�Ҳ� �м䳬����
	sno = 0;
	for (i = 1; i < NYCG; i++)
		if (YCG[i].cml >= m_Scml)//��������ȷ����Ԫ����
		{
			sno = i;
			break;
		}
	eno = NYCG - 1;
	for (i = 0; i < NYCG; i++)
		if (YCG[i].cml >= m_Ecml)//���յ����ȷ����Ԫ�յ��
		{
			eno = i;
			break;
		}

	for (i = sno - 1; i < eno; i++)
	{
		spt.x = mltosx(0, YCG[i].cml);
		spt.y = ym + YCG[i].cg;
		ept.x = mltosx(0, YCG[i + 1].cml);
		ept.y = ym + YCG[i + 1].cg;
		len = YCG[i + 1].cml - YCG[i].cml;
		if (i == sno - 1)//�в����ڱ�ͼ����
		{
			spt.x = mltosx(0, m_Scml);
			spt.y = spt.y + (m_Scml - YCG[i].cml) * (ept.y - spt.y) / len;
			makeline(spt, ept, 2, 0, LayerName);
		}
		else if (i == eno - 1)//�в����ڱ�ͼ����
		{
			ept.x = mltosx(0, m_Ecml);
			ept.y = spt.y + (m_Ecml - YCG[i].cml) * (ept.y - spt.y) / len;
			makeline(spt, ept, 2, 0, LayerName);
		}
		else
			makeline(spt, ept, 2, 0, LayerName);

		if (i < eno - 1)
		{
			tpt.x = ept.x;
			tpt.y = ym;
			makeline(ept, tpt, 2, 0, LayerName);
			if (YCG[i + 1].cg > 0.1)
			{
				_stprintf(str, L"+%0.0lf%s", YCG[i + 1].cg, L"%");
				tpt.y = ept.y + wxp[wxpi].texth + 1.0;
				maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 2, 3, L"����", LayerName);
			}
			else
			{
				_stprintf(str, L"%0.0lf%s", YCG[i + 1].cg, L"%");
				tpt.y = ept.y - wxp[wxpi].texth - 1.0;
				maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 2, 3, L"����", LayerName);
			}
		}
		spt.y = ept.y = ym;
		if (YCG[i].cg >= 0 && YCG[i + 1].cg >= 0)//�м��߻�������
			makeline(spt, ept, 4, 0, LayerName, L"HIDDEN2");
		else if (YCG[i].cg * YCG[i + 1].cg < 0)
		{
			double cml = YCG[i].cml + fabs(YCG[i].cg) * (YCG[i + 1].cml - YCG[i].cml) / fabs(YCG[i + 1].cg - YCG[i].cg);
			tpt.x = mltosx(0, cml);
			tpt.y = ym;
			if (YCG[i].cg > 0)//ǰ���ֻ������� �󲿷ֻ���ʵ��
			{
				makeline(spt, tpt, 4, 0, LayerName, L"HIDDEN2");
				makeline(tpt, ept, 4, 0, LayerName);

			}
			else//ǰ���ֻ���ʵ�� �󲿷ֻ�������
			{
				makeline(spt, tpt, 4, 0, LayerName);
				makeline(tpt, ept, 4, 0, LayerName, L"HIDDEN2");
			}
			if (cml > m_Scml && cml < m_Ecml)
			{
				tpt.y = ym + 0.5 * wxp[wxpi].texth + 1.0;
				_stprintf(str, L"0%s", L"%");
				maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 2, 3, L"����", LayerName);
			}
		}
		else //�м��߻���ʵ��
			makeline(spt, ept, 4, 0, LayerName);
	}

	//��೬����
	sno = 0;
	for (i = 1; i < NZCG; i++)
		if (ZCG[i].cml >= m_Scml)//��������ȷ����Ԫ����
		{
			sno = i;
			break;
		}

	eno = NZCG - 1;
	for (i = 0; i < NZCG; i++)
		if (ZCG[i].cml >= m_Ecml)//���յ����ȷ����Ԫ�յ��
		{
			eno = i;
			break;
		}

	for (i = sno - 1; i < eno; i++)
	{
		spt.x = mltosx(0, ZCG[i].cml);
		spt.y = ym + ZCG[i].cg;
		ept.x = mltosx(0, ZCG[i + 1].cml);
		ept.y = ym + ZCG[i + 1].cg;
		len = ZCG[i + 1].cml - ZCG[i].cml;
		if (i == sno - 1)//�в����ڱ�ͼ����
		{
			spt.x = mltosx(0, m_Scml);
			spt.y = spt.y + (m_Scml - ZCG[i].cml) * (ept.y - spt.y) / len;
		}
		else if (i == eno - 1)//�в����ڱ�ͼ����
		{
			ept.x = mltosx(0, m_Ecml);
			ept.y = spt.y + (m_Ecml - ZCG[i].cml) * (ept.y - spt.y) / len;
		}

		if (ZCG[i].cg <= 0 && ZCG[i + 1].cg <= 0)//�߻�������
			makeline(spt, ept, 1, 0, LayerName, L"HIDDEN2");
		else if (ZCG[i].cg * ZCG[i + 1].cg < 0)
		{
			double cml = ZCG[i].cml + fabs(ZCG[i].cg) * (ZCG[i + 1].cml - ZCG[i].cml) / fabs(ZCG[i + 1].cg - ZCG[i].cg);
			tpt.x = mltosx(0, cml);
			tpt.y = ym;
			if (ZCG[i].cg < 0)//ǰ���ֻ������� �󲿷ֻ���ʵ��
			{
				if (cml > m_Scml && cml < m_Ecml)
				{
					makeline(spt, tpt, 1, 0, LayerName, L"HIDDEN2");
					makeline(tpt, ept, 1, 0, LayerName);
					tpt.y = ym + 0.5 * wxp[wxpi].texth + 1.0;
					_stprintf(str, L"0%s", L"%");
					maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 1, 3, L"����", LayerName);
				}
				else if (cml <= m_Scml)
					makeline(spt, ept, 1, 0, LayerName);
				else
					makeline(spt, ept, 1, 0, LayerName, L"HIDDEN2");
			}
			else//ǰ���ֻ���ʵ�� �󲿷ֻ�������
			{
				if (cml > m_Scml && cml < m_Ecml)
				{
					makeline(spt, tpt, 1, 0, LayerName);
					makeline(tpt, ept, 1, 0, LayerName, L"HIDDEN2");
					tpt.y = ym + 0.5 * wxp[wxpi].texth + 1.0;
					_stprintf(str, L"0%s", L"%");
					maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 1, 3, L"����", LayerName);
				}
				else if (cml <= m_Scml)
					makeline(spt, ept, 1, 0, LayerName, L"HIDDEN2");
				else
					makeline(spt, ept, 1, 0, LayerName);
			}
		}
		else
			makeline(spt, ept, 1, 0, LayerName);


		if (i < eno - 1)
		{
			tpt.x = ept.x;
			tpt.y = ym;
			makeline(ept, tpt, 2, 0, LayerName);
			if (ZCG[i + 1].cg > 0.1)
			{
				_stprintf(str, L"+%0.0lf%s", ZCG[i + 1].cg, L"%");
				tpt.y = ept.y + wxp[wxpi].texth + 1.0;
				maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 1, 3, L"����", LayerName);
			}
			else
			{
				_stprintf(str, L"%0.0lf%s", ZCG[i + 1].cg, L"%");
				tpt.y = ept.y - wxp[wxpi].texth - 1.0;
				maketext(tpt, str, 0.5 * pi, wxp[wxpi].texth, 1, 3, L"����", LayerName);
			}
		}
	}
}
void CZDMHT::DrawTK()
{
	AcGePoint3d pt,spt,ept;
	AcGePoint3dArray Ptarr;
	ACHAR str[256];

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"layer", RTSTR, L"make", RTSTR, L"ͼ��", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"layer", RTSTR, L"make", RTSTR, L"ͼ��", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010	
	

	_stprintf(str, L"%s",m_TKName);
	AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(str, m_VDrawPara.FrameBlockName);
	CString skml,ekml;
	skml = m_pm->XLC(m_Scml);
	ekml = m_pm->XLC(m_Ecml);

	if(DrawMode&&!IsFirst)
	{
		pt.x = X_inori + NTKLen / 2.0;
		pt.y = Y_inori + NTKHt / 2.0;
		if(pBlockId>0)
		{
			AcGePoint3d InsertPt(pt.x,pt.y,0.0);
			AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt,pBlockId);
			pBkRr->setScaleFactors(AcGeScale3d(1,1,1));
			pBkRr->setLayer(L"ͼ��");
			pBkRr->setColorIndex(0);
			BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
		}

		//pt.x = X_inori + NTKLen - 5 - 45;
		//_stprintf(str, L"%d",CurTNo);
		//pt.y = Y_inori + NTKHt - 10.5;
		//if(m_bzym)	//��עҳ��
		//	maketext(pt,str,0,4,0,3, L"����", L"ͼ��");	
		//_stprintf(str, L"%s-%s", skml, ekml);
		//pt.x = X_inori + NTKLen - 5 - 30;
		//pt.y = Y_inori + NTKHt - 3.5;
		//maketext(pt,str,0,4,0,3, L"����", L"ͼ��");		//20190711	���������ʱ����
	}
	else
	{
			pt.x = X_inori + NTKLen / 2.0;
			pt.y = Y_inori + NTKHt / 2.0;
			if (pBlockId > 0)
			{
				AcGePoint3d InsertPt(pt.x, pt.y, 0.0);
				AcDbBlockReference *pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
				pBkRr->setScaleFactors(AcGeScale3d(1, 1, 1));
				pBkRr->setLayer(L"ͼ��");
				pBkRr->setColorIndex(0);
				BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
			}
		
   
		//pt.x = X_inori + NTKLen - 45;
		//_stprintf(str, L"%d", CurTNo);
		//pt.y = Y_inori + NTKHt - 10.5;
		//if(m_bzym)	//��עҳ��
		//	maketext(pt,str,0,4,0,3, L"����", L"ͼ��");
		//_stprintf(str, L"%s-%s",skml,ekml);
		//pt.x = X_inori+NTKLen-30;
		//pt.y=Y_inori+NTKHt-3.5;
		//maketext(pt,str,0,4,0,3, L"����", L"ͼ��");	//���������ʱ����
	}
}

/*---------------------------------------------------------------*
*          draw break-chain                                     *
*---------------------------------------------------------------*/
void CZDMHT::draw_dai()
{
	double ZiGao = 5.0;
	int     i, km;
	double  cml,dml,mz,hund;
	double  x1, x2, y1, y2, y3,yl, xt1,xt2,yt;
	ACHAR    str[10], s[40];
	AcGePoint3d pt1,pt2,pt3,pt4;
	AcGePoint3dArray Ptarr;
	ACHAR layern[50];

	//_stprintf(layern, L"����%d",CurTNo);
	_stprintf(layern, L"����ͼ-����");

	yl = Y_inori+wxp[WXPnum].bottomht;
	//for (i=1; i<m_pm->DLNum; i++)  //20190904	��� һ������������
	for (i = 1; i + 1 < m_pm->DLNum; i++)
	{
		str[0]='\0';
		cml = m_pm->DLArray[i].TYLC; 
		if(cml >=m_Scml && cml<=m_Ecml)
		{
			dml = m_pm->DLArray[i].ELC-m_pm->DLArray[i].BLC;
			x1 = mltosx(0,cml);
			x2 = mltosx(1,cml);
			mz = rdes_level(cml,pDBS);
			y1 = ztosy(cml,mz);
			if (fabs(x2 - x1) < 0.01 || i == m_pm->DLNum - 1)		//��ǰ�Ϻ������ͬ�����������һ������	
			{
				pt1.x = x1;pt1.y=y1;
				pt2.x = x1;pt2.y=yl;
				makeline(pt1,pt2,4,0,layern);
				if (i!=m_pm->DLNum-1) 
				{
					pt1.x = x2; pt1.y=y1;
					pt2.x = x2; pt2.y=yl;
					makeline(pt1,pt2,4,0,layern);
				}
				if (i!=m_pm->DLNum-1) 
				{
					xt1 = x1-1.0;
					xt2 = x2+4.0;
				}
				else 
				{
					xt1 = x1-1.0;
					xt2 = x1+4.0;
				}
			}
			else 
			{
				y3=yl+10.0;
				y2=y3+10.0;
				pt1[0]=x1;  pt1[1]= y1;
				pt2[0]=x1;  pt2[1]= y2;
				pt3[0]=x2;  pt3[1]= y3;
				pt4[0]=x2;  pt4[1]= yl;
				Ptarr.append(pt1);
				Ptarr.append(pt2);
				Ptarr.append(pt3);
				Ptarr.append(pt4);
				make2dline(Ptarr,4,0.0,0.0,layern);
				Ptarr.removeSubArray(0, Ptarr.length()-1);
				pt1[0]=x2;  pt1[1]= y1;
				pt2[0]=x2;  pt2[1]= y2;
				pt3[0]=x1;  pt3[1]= y3;
				pt4[0]=x1;  pt4[1]= yl;
				Ptarr.append(pt1);
				Ptarr.append(pt2);
				Ptarr.append(pt3);
				Ptarr.append(pt4);
				make2dline(Ptarr,4,0.0,0.0,layern);
				xt1 = x2-1.0;
				xt2 = x1+4.0;
			}

			yt = (y1+yl)/2.0;
			km   = (int) floor (m_pm->DLArray[i].BLC/1000);
			hund = m_pm->DLArray[i].BLC - km*1000.0;
			if (hund<10.0)
				if (hund-floor(hund)<=0.0001)
					_stprintf(str, L"00%1.0f", hund);
				else
					_stprintf(str, L"00%4.2f", hund);
			else if (hund<99.99)
				if (hund-floor(hund)<=0.0001)
					_stprintf(str, L"0%2.0f", hund);
				else
					_stprintf(str, L"0%5.2f", hund);
			else
				if (hund-floor(hund)<=0.0001)
					_stprintf(str, L"%3.0f", hund);
				else
					_stprintf(str, L"%6.2f", hund);

			_stprintf(s, L"=%s%d+%s  ",m_pm->DLArray[i].BGH,km, str);

			pt1.x = xt1;pt1.y=yt;
			maketext(pt1,s,0.5*pi,ZiGao,4,4, L"HZ",layern);

			km   = (int) floor (m_pm->DLArray[i].ELC/1000);
			hund = m_pm->DLArray[i].ELC - km*1000.0;
			str[0]='\0';
			if (hund<10.0)
				if (hund-floor(hund)<=0.0001)
					_stprintf(str, L"00%1.0f", hund);
				else
					_stprintf(str, L"00%4.2f", hund);
			else if (hund<100.0)
				if (hund-floor(hund)<=0.0001)
					_stprintf(str, L"0%2.0f", hund);
				else
					_stprintf(str, L"0%5.2f", hund);
			else
				if (hund-floor(hund)<=0.0001)
					_stprintf(str, L"%3.0f", hund);
				else
					_stprintf(str, L"%6.2f", hund);

			_stprintf(s, L"%s%d+%s",m_pm->DLArray[i].EGH,km, str);
			pt1.x = xt1;pt1.y=yt+1.0;
			maketext(pt1,s,0.5*pi,ZiGao,4,1, L"HZ",layern);								   			

			if (fabs(dml)>0.1) 
			{
				if (fabs(dml)-floor(fabs(dml))<=0.001)
				{
					if(dml > 0.1)
						_stprintf(s, L"���� %4.0lfm", fabs(dml));
					else 
						_stprintf(s, L"���� %4.0lfm", fabs(dml));
				}
				else
				{	
					if(dml > 0.1)
						_stprintf(s, L"���� %7.2lfm", fabs(dml));
					else
						_stprintf(s, L"���� %7.2lfm", fabs(dml));
				}
				pt1.x = xt2;pt1.y=yt;
				maketext(pt1,s,0.5*pi,ZiGao,4,3, L"HZ",layern);										   
			}
		}
	};  /* end for  */
}

void CZDMHT::WriteBlock()
{
	ads_point Lfup, Lfbt, Rtup, Rtbt;
	Lfup[Z] = Lfbt[Z] = Rtup[Z] = Rtbt[Z] = 0.0;
	ACHAR bkname[180], filename[256];
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	Lfup[X] = X_inori - 50; Lfup[Y] = Y_inori + WTKHt;
	Lfbt[X] = Lfup[X]; Lfbt[Y] = Y_inori - 50;
	Rtup[X] = X_inori + WTKLen; Rtup[Y] = Y_inori + WTKHt;
	Rtbt[X] = Rtup[X]; Rtbt[Y] = Y_inori - 50;

	m_OutName.Format(L"ZDM%0.1lf-", m_Sdml);
	_stprintf(bkname, L"%s%0.1lf", m_OutName, m_Edml);
	ads_point pt;
	pt[X]=X_inori-30;
	pt[Y]=Y_inori-10;
	//pt[X]=0;
	//pt[Y]=0;
	pt[Z]=0.0;
	//AfxMessageBox(L"1");
	//acedCommandS(RTSTR, L"-block",RTSTR,bkname,RTPOINT,pt,RTSTR, L"WP",RTPOINT,Lfup,RTPOINT,
	//	Rtup,RTPOINT,Rtbt,RTPOINT,Lfbt,RTSTR, L" ",RTSTR, L" ",0);
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"-block", RTSTR, bkname, RTPOINT, pt, RTSTR, L"w", RTPOINT, Lfbt,
		RTPOINT, Rtup, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"-block", RTSTR, bkname, RTPOINT, pt, RTSTR, L"w", RTPOINT, Lfbt,
		RTPOINT, Rtup, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010	

	//acedCommandS(RTSTR, L"wblock",RTSTR,filename,RTSTR,bkname,RTPOINT,pt,0);
	//acedCommandS(RTNONE);


	_stprintf(filename , L"%s\\RESULT\\%s\\%s",Cworkdir,Pro,bkname);
	pt[X]=0.0;
	pt[Y]=0.0;
	CString filepath= Cworkdir+"\\result"+"\\";
	filepath+=Pro;
	::_wmkdir(filepath);
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"-wblock", RTSTR, filename, RTSTR, bkname, RTPOINT, pt, 0);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-wblock", RTSTR, filename, RTSTR, bkname, RTPOINT, pt, 0);
	ads_command(RTNONE);
#endif // VERSION_CAD2010	
	ads_printf(L"�ļ��ѱ�����%s!", filename);
}
void CZDMHT::DrawCulvet()//����С�ź�
{
	int i;
	ACHAR str[256];
	double lenth=45,sx,sy,mz;
	AcGePoint3d spt,ept;
	CString Kml;
	ACHAR ckml[80];
	ACHAR layern[50];

	_stprintf(layern, L"С�ź�%d",CurTNo);

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"layer", RTSTR, L"make", RTSTR, layern, RTSTR, L"", RTNONE);
	acedCommandS(RTSTR, L"color", RTSHORT, 4, RTNONE);  // CYAN
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"layer", RTSTR, L"make", RTSTR, layern, RTSTR, L"", RTNONE);
	ads_command(RTSTR, L"color", RTSHORT, 4, RTNONE);  // CYAN
#endif // VERSION_CAD2010	


	for (i = 0; i < pDBS->NCU; i++)
	{
		if (pDBS->CUL[i].cml >= m_Scml&&pDBS->CUL[i].cml <= m_Ecml)
		{			
			Kml = m_pm->XLC(pDBS->CUL[i].cml);
			_stprintf(ckml, L"%s", Kml);
			_stprintf(str, L"%s%s", pDBS->CUL[i].KKMS, pDBS->CUL[i].QHMS);
			lenth = _tcslen(str) * 2 * 1.1;
			sx = mltosx(0, pDBS->CUL[i].cml);
			sy = ztosy(pDBS->CUL[i].cml, pDBS->CUL[i].ht);
			if(pDBS->CUL[i].QHType=="Բ�ܺ�")
			{
				draw_circle(sx,sy);
				mz = rdes_level(pDBS->CUL[i].cml,pDBS);
				sy = ztosy(pDBS->CUL[i].cml,mz);				  
			}
			else if(pDBS->CUL[i].QHType=="�ǰ庭")
			{
				draw_culvert(sx,sy);
				mz = rdes_level(pDBS->CUL[i].cml,pDBS);
				sy = ztosy(pDBS->CUL[i].cml,mz);					  
			}
			else if(pDBS->CUL[i].QHType=="����")
			{
				draw_arc(sx,sy);	
				mz = rdes_level(pDBS->CUL[i].cml,pDBS);
				sy = ztosy(pDBS->CUL[i].cml,mz);	
			}
			else if(pDBS->CUL[i].QHType=="��������")
			{
				mz = rdes_level(pDBS->CUL[i].cml,pDBS);
				sy = ztosy(pDBS->CUL[i].cml,mz);
				sy = sy + 35.0;
				draw_cross(sx,sy);
			}
			else if(pDBS->CUL[i].QHType=="���彻��")
			{
				mz = rdes_level(pDBS->CUL[i].cml,pDBS);
				sy = ztosy(pDBS->CUL[i].cml,mz);
				sy = sy + 35.0;
				draw_cross(sx,sy);
			}
			else if(pDBS->CUL[i].QHType=="�亭")
			{
				draw_boxcult(sx,sy);	
				mz = rdes_level(pDBS->CUL[i].cml,pDBS);
				sy = ztosy(pDBS->CUL[i].cml,mz);	
			}
			spt[X]=sx;spt[Y]=sy;
			ept[X]=sx;ept[Y]=sy+lenth;
			makeline(spt,ept,4,0,layern);
			spt[X] = sx-2.5;
			spt[Y] = sy+0.5*lenth;
			maketext(spt,str,0.5*pi,3,4,3, L"����",layern);
			spt[X] = sx+2.5;
			maketext(spt,ckml,0.5*pi,3,4,3, L"����",layern);	
		}
	}
}

//-----------------------------------
void CZDMHT::draw_circle(double sx, double sy)
{
	AcGePoint3d pt;

	pt[0] = sx;  pt[1] = sy + 2.5;
	makecircle(pt,2.5,4);
	//	acedCommandS(RTSTR, L"circle",RTPOINT,pt,RTREAL,2.5,0);
	return;
}
//----------------------------------------
void CZDMHT::draw_culvert(double sx, double sy)
{
	AcGePoint3d pt1,pt2;
	AcGePoint3dArray ptarr;
	ACHAR layern[50];

	_stprintf(layern, L"С�ź�%d",CurTNo);
	pt1[0] = sx - 2.5; pt1[1] = sy;
	ptarr.append(pt1);
	pt2[0] = sx + 2.5; pt2[1] = sy;
	ptarr.append(pt2);
	//	acedCommandS(RTSTR, L"pline",RTPOINT,pt1,0);
	//	acedCommandS(RTPOINT,pt2,0);
	pt1[0] = pt2[0]; pt1[1] = sy +5.0;
	ptarr.append(pt1);
	//	acedCommandS(RTPOINT,pt1,RTSTR, L"",0);
	make2dline(ptarr,4,0,0,layern);
	pt1[0] = sx - 3.5; pt1[1] = sy+5.0;
	pt2[0] = sx + 3.5; pt2[1] = sy+5.0;
	makeline(pt1,pt2,4,0,layern);
	//	acedCommandS(RTSTR, L"line",RTPOINT,pt1,RTPOINT,pt2,RTSTR, L"",0);

	pt1[0] = sx - 2.5; pt1[1] = sy+5.0;
	pt2[0] = sx - 2.5; pt2[1] = sy;
	makeline(pt1,pt2,4,0,layern);
	//	acedCommandS(RTSTR, L"line",RTPOINT,pt1,RTPOINT,pt2,RTSTR, L"",0);
}

//-------------------------------------------
void CZDMHT::draw_boxcult(double sx, double sy)
{
	AcGePoint3d pt;
	AcGePoint3dArray ptarr;
	ACHAR layern[50];

	_stprintf(layern, L"С�ź�%d",CurTNo);

	pt[0] = sx-2.5; pt[1] = sy;
	ptarr.append(pt);
	//	acedCommandS(RTSTR, L"pline",RTPOINT,pt,0);
	pt[0] = sx+2.5; pt[1] = sy;
	ptarr.append(pt);
	//	acedCommandS(RTPOINT,pt,0);
	pt[0] = sx+2.5; pt[1] = sy+5.0;
	ptarr.append(pt);
	//	acedCommandS(RTPOINT,pt,0);
	pt[0] = sx-2.5; pt[1] = sy+5.0;
	ptarr.append(pt);
	//	acedCommandS(RTPOINT,pt,0);
	pt[0] = sx-2.5; pt[1] = sy;
	ptarr.append(pt);
	make2dline(ptarr,4,0,0,layern);
	//	acedCommandS(RTPOINT,pt,RTSTR, L"",0);
}

//-------------------------------------------
void CZDMHT::draw_arc(double sx, double sy)
{
	ads_point pt;

	pt[0] = sx - 2.5;
	pt[1] = sy + 2.5;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"pline", RTPOINT, pt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"pline", RTPOINT, pt, 0);
#endif // VERSION_CAD2010	
	
	pt[0] = sx - 2.5;
	pt[1] = sy ;
#ifdef VERSION_CAD2016
	acedCommandS(RTPOINT, pt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTPOINT, pt, 0);
#endif // VERSION_CAD2010	
	pt[0] = sx + 2.5;
	pt[1] = sy ;
#ifdef VERSION_CAD2016
	acedCommandS(RTPOINT, pt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTPOINT, pt, 0);
#endif // VERSION_CAD2010	
	pt[0] = sx + 2.5;
	pt[1] = sy + 2.5 ;
#ifdef VERSION_CAD2016
	acedCommandS(RTPOINT, pt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTPOINT, pt, 0);
#endif // VERSION_CAD2010	
	pt[0] = sx - 2.5;
	pt[1] = sy + 2.5;    
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"arc", RTPOINT, pt, RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"arc", RTPOINT, pt, RTSTR, L"", 0);
#endif // VERSION_CAD2010	
	
}

//---------------------------------------------
void CZDMHT::draw_cross(double sx, double sy)
{
	ads_point pt;

	pt[0] = sx - 4.0;  
	pt[1] = sy;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"line", RTPOINT, pt, RTSTR, L"@1.0<120.0", RTSTR, L"", 0);
	acedCommandS(RTSTR, L"line", RTPOINT, pt, RTSTR, L"@8.0<0.0", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"line", RTPOINT, pt, RTSTR, L"@1.0<120.0", RTSTR, L"", 0);
	ads_command(RTSTR, L"line", RTPOINT, pt, RTSTR, L"@8.0<0.0", RTSTR, L"", 0);
#endif // VERSION_CAD2010	

	pt[0] = sx + 4.0;  
	pt[1] = sy;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"line", RTPOINT, pt, RTSTR, L"@1.0<30.0", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"line", RTPOINT, pt, RTSTR, L"@1.0<30.0", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
	
}

void CZDMHT::GrdrawDesL(double xcml, double zori, int sbk)
{
	int i,sno,eno; 
	double sht,eht,cml,ydori;
	ads_point spt,ept;

	ydori = Y_inori+wxp[WXPnum].bottomht;
	for(i=0; i<pDBS->NPD;i++)
		if(pDBS->PDB[i].cml>xcml+0.1)//�ɶ������ȷ���¶�������
		{
			sno=i;
			sht= des_level(xcml,pDBS);
			break;
		}
		eno=pDBS->NPD-1;
		eht=pDBS->PDB[eno].Level;
		for(i=0; i<pDBS->NPD;i++)
			if(pDBS->PDB[i].cml>=BREAK[sbk+1][0]+0.1)//���յ����ȷ���¶����յ��
			{
				eno=i;
				eht= des_level(BREAK[sbk+1][0],pDBS);
				break;
			}

			spt[X]=mltosx(0,xcml),spt[Y]=ydori+(sht-zori)/Y_scale;
			for(i=sno;i<eno;i++)
			{
				cml = pDBS->PDB[i].cml;
				ept[X] = mltosx(0,cml);
				ept[Y] = ydori+(pDBS->PDB[i].Level-zori)/Y_scale;
				ads_grdraw(spt,ept,3,1);
				spt[X]=ept[X],spt[Y]=ept[Y];
			}
			ept[X] = mltosx(0,BREAK[sbk+1][0]);
			ept[Y] = ydori+(eht-zori)/Y_scale;
			ads_grdraw(spt,ept,3,1);
}

void CZDMHT::GrdrawEarth(double xcml,double zori,int sbk)
{
	int i,sno,eno; 
	double sht,eht,cml,ydori;
	ads_point spt,ept;

	ydori = Y_inori+wxp[WXPnum].bottomht;
	for (i = 0; i<pDBS->NTW; i++)
	{
		if (pDBS->TWB[i].cml>xcml + 0.1)//�ɶ������ȷ������������
		{
			sno = i;
			sht = pDBS->TWB[i - 1].DMGC + (xcml - pDBS->TWB[i - 1].cml)*(pDBS->TWB[i].DMGC - pDBS->TWB[i - 1].DMGC) / (pDBS->TWB[i].cml - pDBS->TWB[i - 1].cml);
			break;
		}
	}

	eno = pDBS->NTW - 1;
	eht = pDBS->TWB[eno].DMGC;



	for (i = 0; i < pDBS->NTW; i++)
	{
		if (pDBS->TWB[i].cml >= BREAK[sbk + 1][0] - 0.1)//�ɶ������ȷ���������յ��
		{
			eno = i;
			eht = pDBS->TWB[i - 1].DMGC + (BREAK[sbk + 1][0] - pDBS->TWB[i - 1].cml)*(pDBS->TWB[i].DMGC - pDBS->TWB[i - 1].DMGC) / (pDBS->TWB[i].cml - pDBS->TWB[i - 1].cml);
			break;
		}
	}
		
	spt[X] = mltosx(0, xcml), spt[Y] = ydori + (sht - zori) / Y_scale;
	for (i = sno; i < eno; i++)
	{
		cml = pDBS->TWB[i].cml;
		ept[X] = mltosx(0, cml);
		ept[Y] = ydori + (pDBS->TWB[i].DMGC - zori) / Y_scale;
		ads_grdraw(spt, ept, 1, 1);
		spt[X] = ept[X], spt[Y] = ept[Y];
	}
	ept[X] = mltosx(0, BREAK[sbk + 1][0]), ept[Y] = ydori + (eht - zori) / Y_scale;
	ads_grdraw(spt, ept, 1, 1);

}

/*---------------------------------------------------------------*
*   convert screen coordinate  screen_x to mileage cml               
*---------------------------------------------------------------*/
double CZDMHT::sxtocml(double sx)
{
	double cml;
	int i;

	cml = -10000000;
	for(i=0;i<NSDAI-1;i++)
	{
		if(sx>=SDAI[i][1]&&sx<=SDAI[i+1][1])
		{
			cml=SDAI[i][2]+(sx-SDAI[i][1])*X_scale;
			break;
		}
	}

	return cml;
}

void  CZDMHT::DelAxle(ads_point PT)
{

	double cml;
	int i,sbk;

	sbk = NBREAK;
	/*
	rc=ads_getpoint(NULL, L"\n��ѡ�����λ��:",PT); 
	if(rc==RTCAN)
	return;
	if(DrawZorY==-1)
	cml = sxtocml(-PT[X]);
	else*/

	cml = sxtocml(PT[X]);
	for(i=0; i<NBREAK; i++)
	{
		if(BREAK[i][0] > cml )
		{
			sbk = i-1;
			break;
		}
	}
	if(sbk>=0&&sbk<NBREAK-1)
	{
		if(fabs(cml-BREAK[sbk][0])<fabs(BREAK[sbk+1][0]-cml))
		{
			if(sbk==0)
				return;//��һ���᲻��ɾ��
			for(i=sbk;i<NBREAK-1;i++)
			{
				BREAK[i][0]=BREAK[i+1][0];
				BREAK[i][1]=BREAK[i+1][1];
			}
		}
		else
		{
			for(i=sbk+1;i<NBREAK-1;i++)
			{
				BREAK[i][0]=BREAK[i+1][0];
				BREAK[i][1]=BREAK[i+1][1];
			}
		}
		NBREAK-=1;

		ACHAR layern[50];
		_stprintf(layern, L"���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"������%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"����ߵ�����%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"�ұ��ߵ�����%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"�¶���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"������%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"���%d",CurTNo);
		dlayer(layern);		
		_stprintf(layern, L"С�ź�%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"����%d",CurTNo);
		dlayer(layern);
		if(DrawMode)
			axleSg();
		else
			axleCb();
		if(pDBS->NTW>0)
			draw_earthline();
		draw_bxearthline();
		if(pDBS->NPD>0)
			draw_des_slop(pDBS);
		if(pZDBS&&pZDBS->NPD>0)
			draw_des_slop(pZDBS);
		if(pYDBS&&pZDBS->NPD>0)
			draw_des_slop(pZDBS);
		if(pDBS->NPD>0)
		{
			DrawDZQ();
			DrawTun();
			DrawCulvet();
			draw_dai();
		}		
	}
}

void  CZDMHT::AddAxle(ads_point PT)//���Ӷ���
{	
	double cml,ydori,zori;
	int i,sbk,type,track=1;
	struct resbuf entdata;// ����϶�ʱ�Ķ�̬����
	sbk = NBREAK;
	/*
	rc=ads_getpoint(NULL, L"\n��ѡ�����λ��:",PT); 
	if(rc==RTCAN)
	return;*/


	cml = sxtocml(PT[X]);
	for(i=0; i<NBREAK; i++)
	{
		if(BREAK[i][0] > cml )
		{
			sbk = i-1;
			break;
		}
	}
	//	ads_printf(L"cml=%lf sbk=%d\n",cml,sbk);
	//	tmp = ydori+(mz-BREAK[i][1])/Y_scale;
	if(sbk>=0&&sbk<NBREAK-1)
	{
		ydori = Y_inori+wxp[WXPnum].bottomht;
		ads_point  spt,ept,tmppt;
		while (track>0) 
		{  //����϶�ѭ��
			ads_grread (track, &type, &entdata); 
			tmppt[0]=entdata.resval.rpoint[0];
			tmppt[1]=entdata.resval.rpoint[1];
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"REDRAW", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"REDRAW", RTNONE);
#endif // VERSION_CAD2010	
			
			spt[X]=PT[X],spt[Y]=ydori;
			ept[X]=PT[X],ept[Y]=Y_inori+NTKHt-TopFW;
			ads_grdraw (spt , ept, 3, 1);
			zori = BREAK[sbk][1]-(tmppt[1]-PT[Y]); 
			if(pDBS->NTW>0)
				GrdrawEarth(cml,zori,sbk);
			if(pDBS->NPD>0)
				GrdrawDesL(cml,zori,sbk); 
			if (type==3)  //  ��������������һ��
			{  
				zori = BREAK[sbk][1]-(tmppt[1]-PT[Y]); 
				track=0;
			}
		}
		for(i=NBREAK-1; i > sbk; i--)
		{
			BREAK[i+1][0]=BREAK[i][0];
			BREAK[i+1][1]=BREAK[i][1];
		}

		BREAK[sbk+1][0]=int(cml/(10*X_scale))*10.0*X_scale, BREAK[sbk+1][1]=int(zori/2)*2.0;
		NBREAK +=1;
		ACHAR layern[50];
		_stprintf(layern, L"���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"������%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"����ߵ�����%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"�ұ��ߵ�����%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"�¶���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"������%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"С�ź�%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"����%d",CurTNo);
		dlayer(layern);
		if(DrawMode)
			axleSg();
		else
			axleCb();
		if(pDBS->NTW>0)
			draw_earthline();
		draw_bxearthline();
		if(pDBS->NPD>0)
			draw_des_slop(pDBS);
		if(pZDBS&&pZDBS->NPD>0)
			draw_des_slop(pZDBS);
		if(pYDBS&&pYDBS->NPD>0)
			draw_des_slop(pYDBS);
		if(pDBS->NPD>0)
		{
			DrawDZQ();
			DrawTun();
			DrawCulvet();
			draw_dai();
		}
	}
}

void  CZDMHT::AjustAxle(ads_point PT)//��������
{	
	double cml,ydori,zori;
	int i,sbk,type,track=1;
	struct resbuf entdata;// ����϶�ʱ�Ķ�̬����
	sbk = NBREAK;
	/*
	rc=ads_getpoint(NULL, L"\n��ѡ�����λ��:",PT); 
	if(rc==RTCAN)
	return;*/

	cml = sxtocml(PT[X]);
	for(i=0; i<NBREAK; i++)
	{
		if(BREAK[i][0] > cml )
		{
			sbk = i-1;
			break;
		}
	}
	//	ads_printf(L"cml=%lf sbk=%d\n",cml,sbk);
	//	tmp = ydori+(mz-BREAK[i][1])/Y_scale;
	if(sbk>=0&&sbk<NBREAK-1)
	{
		ydori = Y_inori+wxp[WXPnum].bottomht;
		ads_point  spt,ept,tmppt;
		while (track>0) 
		{  //����϶�ѭ��
			ads_grread (track, &type, &entdata); 
			tmppt[0]=entdata.resval.rpoint[0];
			tmppt[1]=entdata.resval.rpoint[1];
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"REDRAW", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"REDRAW", RTNONE);
#endif // VERSION_CAD2010	
			spt[X] = PT[X], spt[Y] = ydori;
			ept[X] = PT[X], ept[Y] = Y_inori + NTKHt - TopFW;
			ads_grdraw (spt , ept, 3, 1);
			zori = BREAK[sbk][1] - (tmppt[1] - PT[Y]) * Y_scale;
			if(pDBS->NTW>0)
				GrdrawEarth(cml,zori,sbk);
			if(pDBS->NPD>0)
				GrdrawDesL(cml,zori,sbk); 
			if (type==3)  //  ��������������һ��
			{  
				zori = BREAK[sbk][1] - (tmppt[1] - PT[Y]) * Y_scale;
				track=0;
			}
		}
		//for(i=NBREAK-1; i > sbk; i--)
		//{
		//	BREAK[i+1][0]=BREAK[i][0];
		//	BREAK[i+1][1]=BREAK[i][1];
		//}
		//      if(sbk>0)
		//    BREAK[sbk][0]=int(cml/(10*X_scale))*10.0*X_scale;
		//else
		//	BREAK[sbk][0]=m_Scml;
		//BREAK[sbk][1]=int(zori/2)*2.0;
		BREAK[sbk][1] = zori;
		//		NBREAK +=1;
		ACHAR layern[50];
		_stprintf(layern, L"���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"������%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"����ߵ�����%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"�ұ��ߵ�����%d",CurTNo);
		dlayer(layern);		
		_stprintf(layern, L"�¶���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"������%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"���%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"С�ź�%d",CurTNo);
		dlayer(layern);
		_stprintf(layern, L"����%d",CurTNo);
		dlayer(layern);
		if(DrawMode)
			axleSg();//����ʩ������
		else
			axleCb();
		if(pDBS->NTW>0)
			draw_earthline();
		draw_bxearthline();
		if(pDBS->NPD>0)
			draw_des_slop(pDBS);
		if(pZDBS&&pZDBS->NPD>0)
			draw_des_slop(pZDBS);
		if(pYDBS&&pYDBS->NPD>0)
			draw_des_slop(pYDBS);
		if(pDBS->NPD>0)
		{
			DrawDZQ();
			DrawTun();
			DrawCulvet();
			draw_dai();
		}

	}
}

void CZDMHT::DrawZDM(double YbreakTextH,double DefTextH)//��ʼ�� ���ݶ���
{
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L".BLIPMODE", RTSTR, L"OFF", RTNONE);
	acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	ads_command(RTSTR, L".BLIPMODE", RTSTR, L"OFF", RTNONE);
	ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2010	

	int i,j;
	wxp[0].bottomht=0;	
	if(WayorRamp==0)//��·
		for(i=0;i<WXPnum;i++)
			wxp[i+1].bottomht=wxp[i].bottomht+wxp[i].Lht;
	else//�ѵ�
	{
		for(i=0;i<WXPnum;i++)
		{
			if(wxp[i].wxpinf==1)//��Ƹ߳�
			{
				double DesHt=0;
				for(j=0;j<8;j++)
					if(InfLmPt[j])
						DesHt = DesHt+wxp[i].Lht;
				wxp[i+1].bottomht=wxp[i].bottomht+DesHt;	
			}
			else
				wxp[i+1].bottomht=wxp[i].bottomht+wxp[i].Lht;
		}
	}

	//20190813	���µ�����ͼ�����Ȼ�ͼ���ٸı����ԭ��
	//if (m_VDrawPara.Is_buju_ZDM == 0)
	DrawTK();		//����ͼ��
	X_inori_Buju = X_inori; Y_inori_Buju = Y_inori;
	X_inori = X_inori + NTKLen / 2.0 - (m_Ecml - m_Scml + 60) / 2  / X_scale;	//��֤ͼ�������������ݶ������Ļ����ص�
	Y_inori += 10.0;

	m_Sdml = m_pm->XLC(m_Scml,SGH);
	m_Edml = m_pm->XLC(m_Ecml,EGH);

	FormSdai();	//�Ӷ�����ת����CAD��ͼ����ϵ
	Y_break();	//���ɶ���
	if(DrawMode)
		axleSg(YbreakTextH);//���ƶ���
	else
		axleCb();//���ƶ���

	Draw_Grid();	//����������
	Draw_CtrH();    //���Ƹ߳̿��Ƶ�
	draw_des_slop(pDBS,DefTextH);		//���¶���
	if (pZDBS && pZDBS > 0)
		draw_des_slop(pZDBS);	//�����¶���
	if (pYDBS && pYDBS > 0)
		draw_des_slop(pYDBS);	//�����¶���
	draw_earthline();			//������� 
	draw_bxearthline();			//����ߵ�����

	for (i = 0; i < WXPnum; i++)		//����������
	{		
		if(wxp[i].wxpinf==0)
		{
			wxp[i].text="�� �� �� ��";
			if(m_dzgkname!="")
				DrawBox_DZGK(i);//���Ƶ��ʸſ�
		}
		else if(wxp[i].wxpinf==1)
		{
			if(WayorRamp==0)//��·	
				wxp[i].text="�� �� �� ��";
			else
				wxp[i].text="";
			DrawBox_Desh(i);//����ʩ��ͼ��Ƹ߳�
		}
		else if(wxp[i].wxpinf==2)
		{	
			wxp[i].text="�� �� �� ��";
			DrawBox_DMGC(i);
		}
		else if(wxp[i].wxpinf==3)
		{
			wxp[i].text="�¶�(%)�³�(m)";	//20190711�޸�	��ģ�屣��һ��
			DrawBox_SLOP(i);
		}
		else if(wxp[i].wxpinf==4)
		{
			wxp[i].text="�� �� ׮ ��";
			if(DrawMode)
				DrawLCZH_Sg(i);
			else
				DrawLCZH_Cb(i);
		}
		else if(wxp[i].wxpinf==5)
		{
			wxp[i].text="ֱ�߼�ƽ����";
			if(m_pm->JdCenNum>0)
				DrawBox_PLAN(i);
		}
		else if(wxp[i].wxpinf==6)
		{
			wxp[i].text="�� ��";
			DrawBox_Cg(i);
		}
		else if(wxp[i].wxpinf==8)
		{
			wxp[i].text="������";
			draw_vertical_factors(i);
		}
		else
		{
			wxp[i].text="���ڸ�";
			DrawBox_TWH(i);
		}
	}

	draw_dai();		//���ƶ���
	DrawDZQ();		//���ƴ�����
	DrawTun();		//�������
	draw_box();		//����������Ŀ��	ע���������draw_vertical_factors()����ǰ	20190711
	draw_vertical_factors();	//����������
	DrawScale();	//���Ʊ�����

	DrawCulvet();	//����С�ź�

	if (m_VDrawPara.Is_buju_ZDM == 1) //���沼�ֳ�ͼ 20190925
		DrawZDMOutput();
}

void CZDMHT::DrawBox_TWH(int wxpi)//����ʩ��ͼ��Ƹ߳�
{
	int   i,j, mess=0;
	ACHAR    str[80];
	double yb,desh,earh,twh;
	AcGePoint3d spt;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-���ڸ�");

	yb = Y_inori+wxp[wxpi].bottomht;
	if(WayorRamp==0)//��·
	{
		for(i=0;i<pDBS->NTW;i++)
		{
			if(pDBS->TWB[i].cml >= m_Scml - 0.1 && pDBS->TWB[i].cml <= m_Ecml + 0.1)
			{
				//20190829	�����������׮�Ƿ����
				if (!m_VDrawPara.IsDrawFeaturePile && i > 0 && i + 1 < pDBS->NTW)
				{
					if (IsFeaturePile(pDBS->TWB[i].cml))
						continue;
				}

				spt.x=mltosx(0,pDBS->TWB[i].cml);
				if(fabs(pDBS->TWB[i].cml-m_Scml)<1)
					spt.x +=wxp[wxpi].texth;
				if(fabs(pDBS->TWB[i].cml-m_Ecml)<1)
					spt.x -=0.5*wxp[wxpi].texth;
				spt.y=yb+0.5*wxp[wxpi].Lht;

				desh = rdes_level(pDBS->TWB[i].cml,pDBS);
				earh = pDBS->TWB[i].DMGC;
				twh = desh - earh;
				_stprintf (str, L"%0.2lf", twh);
				maketext(spt,str,0.5*pi,wxp[wxpi].texth,4,2, L"����", LayerName);
			}
		}
	}
}
void CZDMHT::DrawScale()
{
	if (!m_IsNoteScale)
		return;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"����ͼ-������");

	//ȷ������ԭ��
	AcGePoint3d OriPt(0.0, 0.0, 0.0);
	OriPt.x = mltosx(0, 0.0) - 60 / X_scale;
	OriPt.y = Y_inori + wxp[WXPnum].bottomht + 10 * X_scale;

	//д��	�ָ���ʱ���ó�5
	ACHAR Text[256];
	_stprintf(Text, L"H:1:%d", (int)(X_scale * 1000 * 2.0));
	maketext(OriPt, Text, 0.0, 5.0, GREEN, 3, L"����", LayerName);
	
	OriPt.y += 7.0;
	_stprintf(Text, L"V:1:%d", (int)(Y_scale * 1000 * 2.0));
	maketext(OriPt, Text, 0.0, 5.0, GREEN, 3, L"����", LayerName);
}
bool CZDMHT::isBiaoZhuGaiLiCheng(double cml_Start,double cml_End,double cml,double LiChengBuChang)
{
	//����20mһ����̻���
	int Num = int(fabs(cml_Start - cml_End) / LiChengBuChang);
	Num += 5;
	//���cml����������̣���ֱ�ӷ���true
	int cml_ZhengShu = int(cml);
	if (fabs(cml - cml_ZhengShu) > 0.01)
		return true;
	for (int i = 0;i < Num;i++)
	{
		if (fabs(cml - 20.0 * i- cml_Start) < 0.0001)//20�ı���
			return true;
	}
	return false;
}

//20190821����	�����ӳ���
void CZDMHT::DrawExtendLine(XLDataBase* pDBStmp, double PreLen, double AftLen)
{
	//��⣬������ʱ������
	if (pDBStmp->NPD <= 1)
		return;

	//����ͼ����	20190917
	ACHAR layern[50];
	_stprintf(layern, L"����ͼ-�¶���");

	//���ò���
	AcGePoint3d SlopeSPt(0.0, 0.0, 0.0), SlopeEPt(0.0, 0.0, 0.0), LinePt(0.0, 0.0, 0.0);
	double Degree = 0.0;

	//�������
	SlopeSPt.x = mltosx(0, pDBStmp->PDB[0].cml);
	SlopeSPt.y = ztosy(pDBStmp->PDB[0].cml, pDBStmp->PDB[0].Level);

	Degree = (pDBStmp->PDB[1].Level - pDBStmp->PDB[0].Level) / (pDBStmp->PDB[1].cml - pDBStmp->PDB[0].cml);

	LinePt.x = SlopeSPt.x - AftLen / X_scale;
	LinePt.y = SlopeSPt.y - Degree * PreLen / Y_scale;

	makeline(SlopeSPt, LinePt, 0, 0.0, layern);

	//�����յ�
	SlopeEPt.x = mltosx(0, pDBStmp->PDB[pDBStmp->NPD - 1].cml);
	SlopeEPt.y = ztosy(pDBStmp->PDB[pDBStmp->NPD - 1].cml, pDBStmp->PDB[pDBStmp->NPD - 1].Level);

	Degree = (pDBStmp->PDB[pDBStmp->NPD - 1].Level - pDBStmp->PDB[pDBStmp->NPD - 2].Level)
		/ (pDBStmp->PDB[pDBStmp->NPD - 1].cml - pDBStmp->PDB[pDBStmp->NPD - 2].cml);

	LinePt.x = SlopeEPt.x + AftLen / X_scale;
	LinePt.y = SlopeEPt.y + Degree * AftLen / Y_scale;

	makeline(SlopeEPt, LinePt, 0, 0.0, layern);
}

//20190829����	����Ƿ�������׮	����20������Ϊ����׮
bool CZDMHT::IsFeaturePile(double Cml)
{
	double Times = Cml / 20;
	if (fabs((int)Times - Times) > 0.001)
		return true;
	return false;

}
double CZDMHT::Trs_Coor_System(double Ori_Para, int Mile_Level_Flag, int Trs_Flag) const
{
	double VSCALE= Y_scale, HSCALE= X_scale, K0=0, X0= Xd_ori, H0=60, Y0= Y_inori + wxp[WXPnum].bottomht;	//�̱߳���  ˮƽ����  �������  �����Ӧͼ��X����	
	double Answer = 0.0;
	if (Mile_Level_Flag == MILE)
	{
		if (Trs_Flag == TO_DRAW)
			Answer = (Ori_Para - K0) / HSCALE + X0;
		else if (Trs_Flag == TO_SLOPE)
			Answer = (Ori_Para - X0) * HSCALE + K0;
		else
			ads_printf(L"�����������������̲���ת��ʧ�ܣ�");
	}
	else if (Mile_Level_Flag == LEVEL)
	{
		if (Trs_Flag == TO_DRAW)
			Answer = /*(Ori_Para - H0) / VSCALE + */Y0+ (Ori_Para-BREAK[0][1])*10;
		else if (Trs_Flag == TO_SLOPE)
			Answer = (Ori_Para - Y0) * VSCALE + H0;
		else
			ads_printf(L"���������������̲߳���ת��ʧ�ܣ�");
	}
	else
		ads_printf(L"������������������ת��ʧ�ܣ�");

	return Answer;
}
AcGePoint2d CZDMHT::Data_realtodraw(AcGePoint2d pt)
{
	AcGePoint2d pk;
	pk.x = Trs_Coor_System(pt.x, MILE, TO_DRAW);
	pk.y = Trs_Coor_System(pt.y, LEVEL, TO_DRAW);
	//pk.x=(pt.x-K0)/HBVSCALE+X0;
	//pk.y=(pt.y-H0)+Y0;
	return pk;
}
void CZDMHT::Draw_CtrH()
{
	
	if (!isdrawctrh)
		return;
	//���ò���
	//��ȡ����������
	XLDataBase DBS;
	DBS.Read_XLDbs(pzLinep->mdbname, L"�߳̿��Ƶ��", pzLinep->RoadName);
	
	AcGePoint2d OriPt;
	AcGePoint3d OriPt3D, TriRightTop, TriLeftTop, TempPt;
	AcGeVector3d VecRightTop, VecLeftTop;
	VecRightTop.normalize();
	VecLeftTop.normalize();
	ACHAR LevelText[256];
	double TriLen = 5.0;

	for (int i = 0; i < DBS.m_LevelCtrlPtArray.size(); i++)
	{
		//�������޵�
		if (DBS.m_LevelCtrlPtArray[i].Style == 1 || DBS.m_LevelCtrlPtArray[i].Style == 3)
		{
			VecRightTop.set(cos(pi / 3), sin(pi / 3), 0.0);
			VecLeftTop.set(cos(pi * 2 / 3), sin(pi * 2 / 3), 0.0);
			VecRightTop.normalize();
			VecLeftTop.normalize();
			//�����ͼԭ��
			OriPt.set(DBS.m_LevelCtrlPtArray[i].XCLc, DBS.m_LevelCtrlPtArray[i].DesHSX);
			OriPt = Data_realtodraw(OriPt);
			/*OriPt.x += DBS.m_LevelCtrlPtArray[i].Cml;*/
			OriPt3D.set(OriPt.x, OriPt.y, 0.0);
			TriRightTop = OriPt3D + VecRightTop * TriLen;
			TriLeftTop = OriPt3D + VecLeftTop * TriLen;

			//����������
			/*makeline1(OriPt3D, TriRightTop,RED);
			makeline1(TriRightTop, TriLeftTop, RED);
			makeline1(TriLeftTop, OriPt3D, RED);*/
			makeline(OriPt3D, TriRightTop, RED, 0, L"����ͼ-���Ʊ��");
			makeline(TriRightTop, TriLeftTop, RED, 0, L"����ͼ-���Ʊ��");
			makeline(TriLeftTop, OriPt3D, RED, 0, L"����ͼ-���Ʊ��");

			//����ֱ��
			TempPt = OriPt3D;
			TempPt.x += 30.0;
			makeline(OriPt3D, TempPt, RED, 0, L"����ͼ-���Ʊ��");
			//makeline1(OriPt3D, TempPt, RED);
			//��������
			_stprintf(LevelText, L"�߳����ޣ�%.3lf", DBS.m_LevelCtrlPtArray[i].DesHSX);
			TempPt.x = OriPt3D.x + 2.0;
			TempPt.y = OriPt3D.y + 1.0;
			//G_maketext(pWd, TempPt, LevelText, 0.0, 2.0, RED, 2);
			maketext(TempPt, LevelText, 0.0, 2.0, RED, 1, L"����", L"����ͼ-���Ʊ��");
			TempPt.y += 5.0;
			_tcscpy(LevelText, DBS.m_LevelCtrlPtArray[i].Note);
			//G_maketext(pWd, TempPt, LevelText, 0.0, 2.0, RED, 2);
			maketext(TempPt, LevelText, 0.0, 2.0, RED, 1, L"����", L"����ͼ-���Ʊ��");
		}

		//�������޵�
		if (DBS.m_LevelCtrlPtArray[i].Style == 2 || DBS.m_LevelCtrlPtArray[i].Style == 3)
		{
			VecRightTop.set(cos(-pi / 3), sin(-pi / 3), 0.0);
			VecLeftTop.set(cos(-pi * 2 / 3), sin(-pi * 2 / 3), 0.0);
			VecRightTop.normalize();
			VecLeftTop.normalize();
			//�����ͼԭ��
			OriPt.set(DBS.m_LevelCtrlPtArray[i].XCLc, DBS.m_LevelCtrlPtArray[i].DesHXX);
			OriPt = Data_realtodraw(OriPt);
			OriPt3D.set(OriPt.x, OriPt.y, 0.0);
			TriRightTop = OriPt3D + VecRightTop * TriLen;
			TriLeftTop = OriPt3D + VecLeftTop * TriLen;


			makeline(OriPt3D, TriRightTop, RED, 0, L"����ͼ-���Ʊ��");
			makeline(TriRightTop, TriLeftTop, RED, 0, L"����ͼ-���Ʊ��");
			makeline(TriLeftTop, OriPt3D, RED, 0, L"����ͼ-���Ʊ��");
			/*makeline1(OriPt3D, TriRightTop, RED);
			makeline1(TriRightTop, TriLeftTop, RED);
			makeline1(TriLeftTop, OriPt3D, RED);*/

			//����ֱ��
			TempPt = OriPt3D;
			TempPt.x += 30.0;
			makeline(OriPt3D, TempPt, RED, 0, L"����ͼ-���Ʊ��");
			//makeline1( OriPt3D, TempPt, RED);

			//��������
			_stprintf(LevelText, L"�߳����ޣ�%.3lf", DBS.m_LevelCtrlPtArray[i].DesHXX);
			TempPt.x = OriPt3D.x + 2.0;
			TempPt.y = OriPt3D.y - 3.0;
			maketext(TempPt, LevelText, 0.0, 2.0, RED,1,L"����",L"����ͼ-���Ʊ��");
			TempPt.y -= 5.0;
			_tcscpy(LevelText, DBS.m_LevelCtrlPtArray[i].Note);
			maketext(TempPt, LevelText, 0.0, 2.0, RED, 1, L"����", L"����ͼ-���Ʊ��");
		}
	}

}
void CZDMHT::Draw_Grid() const
{
	if (!infwgx)
		return;

	AcGePoint3d spt, ept;

	ACHAR NetLayerName[16];
	_tcscpy(NetLayerName, L"����ͼ-������");
	double gwht;	//����߶�
	int gwnum;
	double len = 10 * X_scale;
	gwht = NTKHt - wxp[WXPnum].bottomht - TopFW;
	gwnum = floor(gwht / 10) - 1;	//��������
	gwht = gwnum * 10;

	ACHAR lineFname[1024];
	_stprintf(lineFname, L"%s\\lib\\rail.lin", SysPath);
	acdbHostApplicationServices()->workingDatabase()->loadLineTypeFile(L"DOT3", lineFname);

	//������	20190905	��ԸĹ�ģʽ�ֱ�������ռ����
	double sx, ex, bmsml, bmeml;
	bmsml = floor(m_Scml / len) * len;
	if (m_VDrawPara.IsGutterMode)
		sx = mltosx(0, bmsml);
	else
		sx = mltosx(0, bmsml) - 20 / X_scale;
	bmeml = floor(m_Ecml / len) * len;
	if (m_VDrawPara.IsGutterMode)
		ex = mltosx(0, bmsml) + (bmeml - bmsml) / X_scale + 20 * 1 / X_scale;
	else
		ex = mltosx(0, bmsml) + (bmeml - bmsml) / X_scale + 20 * 2 / X_scale;


	for (int i = 0; i <= gwnum; i++)//draw ����
	{
		if (i % 2 == 0 || i == gwnum)//ż��
		{
			spt[X] = Xd_ori, spt[Y] = Y_inori + wxp[WXPnum].bottomht + i * 10;
			ept[X] = sx, ept[Y] = spt[Y];
			makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
			spt[X] = sx, ept[X] = ex;
			makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
			//spt[X] = ex;
			//ept[X] = SDAI[NSDAI - 1][1];
			//ept[Y] = spt[Y];
			//makeline(spt, ept, 8, 0, L"������", L"DOT3");
		}
	}

	double BmCml[1000];
	int ibm = 0;

	//������������
	spt[X] = mltosx(0, m_Scml);
	spt[Y] = Y_inori + wxp[WXPnum].bottomht;
	ept[X] = spt[X];
	ept[Y] = Y_inori + wxp[WXPnum].bottomht + gwht;
	makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");

	//20191009	10m����
	for (double CurX = sx + 20.0; CurX < mltosx(0, m_Scml) - 0.01; CurX += 20)
	{
		spt[X] = CurX;
		ept[X] = spt[X];
		makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
	}

	for (int i = 0; i < m_pm->NLCB; i++)
	{
		//if (m_pm->pLCB[i].lc > m_Scml && m_pm->pLCB[i].lc < m_Ecml)
		if (m_pm->pLCB[i].lc > m_Scml && m_pm->pLCB[i].lc < m_Ecml)
		{
			spt.x = mltosx(0, m_pm->pLCB[i].lc);
			if (m_pm->pLCB[i].lcflag > 0 && m_pm->pLCB[i].lcflag < 10)
			{
				BmCml[ibm] = m_pm->pLCB[i].lc;
				if (ibm == 0)//��һ������
				{
					sx = mltosx(0, m_Scml);
					ex = spt.x;
					int k = 0;
					while (ex > sx + 0.001)
					{
						spt[X] = ex;
						spt[Y] = Y_inori + wxp[WXPnum].bottomht;
						ept[X] = spt[X];
						ept[Y] = Y_inori + wxp[WXPnum].bottomht + gwht;
						if (k % 2 == 0)
						{
							if (k > 0)
								makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
							else
								makeline(spt, ept, 1, 0.5, NetLayerName);
						}
						ex = ex - 10;
						k++;
					}

				}
				else
				{
					//���ױ�֮��
					double sx, ex;
					sx = mltosx(0, BmCml[ibm - 1]);
					ex = spt.x;
					int k = 0;
					while (sx < ex + 0.001)
					{
						spt[X] = sx;
						spt[Y] = Y_inori + wxp[WXPnum].bottomht;
						ept[X] = spt[X];
						ept[Y] = Y_inori + wxp[WXPnum].bottomht + gwht;
						if (k % 2 == 0)
						{
							if (k > 0)
								makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
							else
								makeline(spt, ept, 1, 0.5, NetLayerName);
						}
						sx = sx + 10;
						k++;
					}
				}
				ibm++;
			}
		}

	}
	if (ibm > 0)//������
	{
		double StartX, EndX;
		StartX = mltosx(0, BmCml[ibm - 1]);
		if (m_VDrawPara.IsGutterMode)
			EndX = mltosx(0, bmsml) + (bmeml - bmsml) / X_scale + 20 * 1 / X_scale;	//20190904	֮ǰ�Ѿ�������յ�
		else
			EndX = mltosx(0, bmsml) + (bmeml - bmsml) / X_scale + 20 * 2 / X_scale;	//20190904	֮ǰ�Ѿ�������յ�

		int k = 0;
		while (StartX < EndX + 1)
		{
			spt[X] = StartX;
			spt[Y] = Y_inori + wxp[WXPnum].bottomht;
			ept[X] = spt[X];
			ept[Y] = Y_inori + wxp[WXPnum].bottomht + gwht;
			if (k % 2 == 0)
			{
				if (k > 0)
					makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
				else
					makeline(spt, ept, 1, 0.5, NetLayerName);
			}
			StartX = StartX + 10;
			k++;
		}
	}
	else //����Ϊ0�����
	{
		double StartX, EndX;
		StartX = mltosx(0, bmsml);
		if (m_VDrawPara.IsGutterMode)
			EndX = mltosx(0, bmsml) + (bmeml - bmsml) / X_scale + 20 * 1 / X_scale;	//20190904	֮ǰ�Ѿ�������յ�
		else
			EndX = mltosx(0, bmsml) + (bmeml - bmsml) / X_scale + 20 * 2 / X_scale;	//20190904	֮ǰ�Ѿ�������յ�

		int k = 0;
		while (StartX < EndX + 1)
		{
			spt[X] = StartX;
			spt[Y] = Y_inori + wxp[WXPnum].bottomht;
			ept[X] = spt[X];
			ept[Y] = Y_inori + wxp[WXPnum].bottomht + gwht;
			if (k % 2 == 0)
			{
				makeline(spt, ept, 8, 0, NetLayerName, L"DOT3");
			}
			StartX = StartX + 10;
			k++;
		}
	}

	//
}

void CZDMHT::DrawZDMOutput() 
{
	//�رղ�׽����
	BAS_DRAW_FUN::Set_OSMODE(0);
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
	acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);	//��������ʾ��
	ads_command(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);	//ʮ�ּ�ģʽ��
	ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);	//��������ϵ
#endif // VERSION_CAD2010
	AcGePoint3d pt;
	pt.x = X_inori_Buju + NTKLen / 2.0;
	pt.y = Y_inori_Buju + NTKHt / 2.0;
	m_Sdml = m_pm->XLC(m_Scml, SGH); //��ͼ��ʼ�������
	m_Edml = m_pm->XLC(m_Ecml, EGH); //��ͼ��ֹ�������
	CString skml, ekml;
	skml = m_pm->XLC(m_Scml); 
	ekml = m_pm->XLC(m_Ecml);
	STuKuang* TK = NULL;
	TK = new STuKuang[1];

	TK->StartTYLC = m_Sdml;;
	TK->EndTYLC = m_Edml;
	TK->StartXCLC= m_pm->XLC(m_Scml); //��ʼ�ֳ����
	TK->EndXCLC = m_pm->XLC(m_Ecml); //��ֹ�ֳ����

	TK->CenX = pt.x;
	TK->CenY = pt.y;
	TK->LtUpX = TK->CenX - NTKLen / 2.0 - 50.0;
	TK->LtUpY = TK->CenY + NTKHt / 2.0 + 10.0;

	TK->LtBmX = TK->CenX - NTKLen / 2.0 - 50.0;
	TK->LtBmY = TK->CenY - NTKHt / 2.0 - 30.0;

	TK->RtUpX = TK->CenX + NTKLen / 2.0 + 10.0;
	TK->RtUpY = TK->CenY + NTKHt / 2.0 + 10.0;

	TK->RtBmX = TK->CenX + NTKLen / 2.0 + 10.0;
	TK->RtBmY = TK->CenY - NTKHt / 2.0 - 30.0;

	TransFormTK_ZM(TK,411.48 / 2.0, 284.90 / 2.0, 1);

	STuKuang *CurTK = &TK[0];
	ACHAR LayoutName[256];
	_stprintf(LayoutName, L"ZDM_%s��%s", skml, ekml);
	CMyLayout::CreateLayout(LayoutName);

#ifdef VERSION_CAD2010 
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	ads_command(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif	

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);  //��������ϵ
	ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE); //�������������ʵ��
#endif // VERSION_CAD2010

	ads_point p1, p2, p3, p4, z1, z2, ori, orix, oriy;
	double xmin, xmax, ymin, ymax;
	p1[X] = CurTK->newLtUpX;
	p1[Y] = CurTK->newLtUpY;

	p2[X] = CurTK->newRtUpX;
	p2[Y] = CurTK->newRtUpY;

	p3[X] = CurTK->newRtBmX;
	p3[Y] = CurTK->newRtBmY;

	p4[X] = CurTK->newLtBmX;
	p4[Y] = CurTK->newLtBmY;

	double jiao;
	BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);
	jiao += 0.5*PI;
	ori[X] = CurTK->LtBmX;
	ori[Y] = CurTK->LtBmY;

	orix[X] = CurTK->RtBmX;
	orix[Y] = CurTK->RtBmY;

	oriy[X] = ori[X] + 10 * cos(jiao);
	oriy[Y] = ori[Y] + 10 * sin(jiao);

	//BAS_DRAW_FUN::getMinMaxXY(p1,p2,p3,p4,xmin,xmax,ymin,ymax);
	xmin = 0.0;
	xmax = (p3[X] - p4[X]);//*K;
	ymin = 0.0;
	ymax = (p2[Y] - p3[Y]);//*K;

	if (p1[X] < p4[X])xmin -= (p4[X] - p1[X]);//*K;
	if (p2[X] > p3[X])xmax += (p2[X] - p3[X]);//*K;					

	z1[X] = xmin;
	z1[Y] = ymin;
	z2[X] = xmax;
	z2[Y] = ymax;

#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
	ads_command(RTSTR, L"MSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

	ads_command(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);
	ads_command(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
	ads_command(RTSTR, L"PSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010
	ACHAR dwgPath[80];
	_tcscpy(dwgPath, m_TKName);
	AcDbObjectId pBlockId;
	pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"");
	if (pBlockId > 0)
	{
		AcGePoint3d InsertPt(411.48 / 2.0, 284.90 / 2.0, 0.0);
		AcDbBlockReference *pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setScaleFactors(AcGeScale3d(1, 1, 1));
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr, 0, 1);
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010

	if (TK)delete[]TK;
}


void CZDMHT::TransFormTK_ZM(STuKuang *TK,double CenX, double CenY, double scale)
{
	AcGeVector3d norm(0, 0, 1);
	norm.normalize();

	STuKuang *CurTK = &TK[0];

	CurTK->newCenX = CenX;
	CurTK->newCenY = CenY;

	AcGeMatrix3d xformR, xformT, xformS;
	double jiao;
	xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);

	AcGeVector3d  moveBy(CurTK->newCenX - CurTK->CenX, CurTK->newCenY - CurTK->CenY, 0.0);
	AcGePoint3d PP(CurTK->CenX, CurTK->CenY, 0.0);
	xformR.setToRotation(-1.0*jiao, norm, PP);
	xformT.setToTranslation(moveBy);
	CurTK->xform = xformT * xformR;

	AcGePoint3d StPt(CurTK->StX, CurTK->StY, 0.0);
	StPt.transformBy(CurTK->xform);
	CurTK->newStX = StPt.x;
	CurTK->newStY = StPt.y;

	AcGePoint3d EdPt(CurTK->EdX, CurTK->EdY, 0.0);
	EdPt.transformBy(CurTK->xform);
	CurTK->newEdX = EdPt.x;
	CurTK->newEdY = EdPt.y;

	AcGePoint3d LtUpPt(CurTK->LtUpX, CurTK->LtUpY, 0.0);
	LtUpPt.transformBy(CurTK->xform);
	CurTK->newLtUpX = LtUpPt.x;
	CurTK->newLtUpY = LtUpPt.y;

	AcGePoint3d LtBmPt(CurTK->LtBmX, CurTK->LtBmY, 0.0);
	LtBmPt.transformBy(CurTK->xform);
	CurTK->newLtBmX = LtBmPt.x;
	CurTK->newLtBmY = LtBmPt.y;

	AcGePoint3d RtUpPt(CurTK->RtUpX, CurTK->RtUpY, 0.0);
	RtUpPt.transformBy(CurTK->xform);
	CurTK->newRtUpX = RtUpPt.x;
	CurTK->newRtUpY = RtUpPt.y;

	AcGePoint3d RtBmPt(CurTK->RtBmX, CurTK->RtBmY, 0.0);
	RtBmPt.transformBy(CurTK->xform);
	CurTK->newRtBmX = RtBmPt.x;
	CurTK->newRtBmY = RtBmPt.y;
}