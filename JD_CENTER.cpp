// JD_CENTER.cpp: implementation of the JD_CENTER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "JD_CENTER.h"
#include "PMDIALOG.h"
#include "LLTuLink.h"
#include "xldatabase.h"
//#include "RailPmPoint.h"
#include "PmPointer.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
	else if ((x) > 0)    \
	y =1;             \
	else                 \
	y = -1;           \
}

extern CString Cworkdir,Pro;
extern PMDIALOG* PmDlg;
int N;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void JD_CENTER::ReadLMK(CString LMKTABname)
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(mdbname,LMKTABname,RoadName);
	//   ads_printf(L"lmk=%d\n",xlmdb.NLMK);
	FILE *fpr;
	CString name=mdbname;
	name.MakeUpper();
	int pos,len,k;
	pos = name.Find(L"DATA");
	len = name.GetLength();
	name=name.Right(len-pos-5);
	CString fname;
	ReadWorkdir();
	fname =Cworkdir + "\\DATA\\" + "BXLMKD." + name + RoadName;
	if((fpr=_wfopen(fname, L"r"))!=NULL)
	{
		BXLMKD bxtmp;
		fwscanf(fpr, L"%d",&BxNum);
		pBXArr = new BXLMKD[BxNum];
		for(k=0;k<BxNum;k++)
		{
			fwscanf(fpr, L"%lf%lf%d%d%d",&pBXArr[k].bxsml,&pBXArr[k].bxeml,&pBXArr[k].LorR,&pBXArr[k].bxinf,&pBXArr[k].ptnum);
			pBXArr[k].ptarr = new double[pBXArr[k].ptnum*3];
			for(int j=0;j<pBXArr[k].ptnum;j++)
				fwscanf(fpr, L"%lf%lf%lf",&pBXArr[k].ptarr[j*3],&pBXArr[k].ptarr[j*3+1],&pBXArr[k].ptarr[j*3+2]);
			//			pBXArr.Add(bxtmp);
		}
		fclose(fpr); 
	}	
	if(xlmdb.NLMK<1)
		return;

	if(LMKTABname == "左路面宽分段表")
	{
		NZLMK = xlmdb.NLMK;
		ZLMK = new LMKdata[xlmdb.NLMK];
		for(int i=0;i<xlmdb.NLMK;i++)
		{
			ZLMK[i] = xlmdb.lmk[i];	
			ZLMK[i].cml = TYLC(ZLMK[i].xlc);
		}
	}
	else
	{
		NYLMK = xlmdb.NLMK;
		YLMK = new LMKdata[xlmdb.NLMK];
		for(int i=0;i<xlmdb.NLMK;i++)
		{
			YLMK[i] = xlmdb.lmk[i];	
			YLMK[i].cml = TYLC(YLMK[i].xlc);
		}
	}

	if(xlmdb.lmk)
		delete []xlmdb.lmk;
	xlmdb.lmk = NULL;
	xlmdb.NLMK = 0;

}

JD_CENTER::JD_CENTER(JD_CENTER *pJC)
{

	m_InterSectData = pJC->m_InterSectData;
	m_RelRailName = pJC->m_RelRailName;
	if(pJC!=NULL)
	{
		this->JdCenNum=pJC->JdCenNum;
		this->JdCenArray.Append(pJC->JdCenArray);
		_tcscpy(this->LCGH,pJC->LCGH);
		_tcscpy(this->JDGH,pJC->JDGH);
		this->DLNum=pJC->DLNum;
		this->DLArray.Append(pJC->DLArray);
		this->SCALE=pJC->SCALE;
		this->texth=pJC->texth;
		this->DLBlength=pJC->DLBlength;
		this->NLC=pJC->NLC;
		this->DrawLeftOrRight=pJC->DrawLeftOrRight;
		this->JD0=pJC->JD0;
		this->SHOWJD=pJC->SHOWJD;
		this->FLDNo=pJC->FLDNo;
		_tcscpy(RoadName,pJC->RoadName);//道路名
		_tcscpy(mdbname,pJC->mdbname);
	}
	IfCalXY = TRUE;
	IfDelJZX = FALSE;
	PreJD = -1;
	Ls1 = 0;
	NZLMK=NYLMK=0;
	ZLMK=YLMK=NULL;
	ZjkNm=YjkNm=0;//超高加宽
	Zjk=Yjk=NULL;
	RtjMin=0;
	//读横断面路面宽分段
}

JD_CENTER::JD_CENTER()
{	

	m_InterSectData.CurDml = 0.0;
	_tcscpy(m_InterSectData.InterCkml, L"\0");
	m_InterSectData.CurDesH = m_InterSectData.DetaH = m_InterSectData.InterDesH = 0.0;
	m_InterSectData.JiaJiao = m_InterSectData.CurFwj = m_InterSectData.InterFwj = 0.0;
	m_InterSectData.InterN = m_InterSectData.interE = 0.0;
	m_InterSectData.MinAng = m_InterSectData.MinDetaH = 0.0;
	m_NCM=0;
	m_RelRailName.Empty();
	XYArray=NULL;
	XYJDNO=NULL;
	ZQZJB=NULL;
	_tcscpy(RoadName , L"未命名道路");
	_tcscpy(mdbname , L"未命名");
	_stprintf(LCGH, L"K");
	_stprintf(JDGH, L"JD");
	SCALE=2.0;
	texth=6.0;
	NLC=3;
	DLBlength=18.0;
	DrawLeftOrRight=-1;
	JD0=1;
	FLDNo=0;
	SHOWJD=true;
	JdCenNum=0;
	XYNum = 0;
	RtjMin=0;

	//断链表
	DLArray.SetSize(2);
	setDLM(0,0.0,0.0,LCGH,LCGH);
	setDLM(1,1.0E4,1.0E4,LCGH,LCGH);

	DLNum=2;
	//	Chinese_Engbz_type = 2;

	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS", L"acadiso.lin");//
	AcCmColor color;
	color.setColorIndex(130);

	CreateLayer(L"交点连线", L"CONTINUOUS",color,AcDb::kLnWt000);

	CreateLayer(L"线路中线", L"CONTINUOUS",color,AcDb::kLnWt000);


	CreateLayer(L"里程", L"CONTINUOUS",color,AcDb::kLnWt000);

	CreateLayer(L"特征点位置", L"CONTINUOUS",color,AcDb::kLnWt000);

	CreateLayer(L"交点", L"CONTINUOUS",color,AcDb::kLnWt000);	
	IfCalXY = TRUE;
	IfDelJZX = FALSE;
	PreJD = -1;
	Ls1 = 0;
	NZLMK=NYLMK=0;
	ZLMK=YLMK=NULL;
	Zjk=NULL;
	Yjk=NULL;
	Zcg=NULL;
	Ycg=NULL;
	pBXArr=NULL;
	m_RelRailName.Empty();
}

JD_CENTER::JD_CENTER(double xy[][10],int xyn)
{	

	m_InterSectData.CurDml = 0.0;
	_tcscpy(m_InterSectData.InterCkml, L"\0");
	m_InterSectData.CurDesH = m_InterSectData.DetaH = m_InterSectData.InterDesH = 0.0;
	m_InterSectData.JiaJiao = m_InterSectData.CurFwj = m_InterSectData.InterFwj = 0.0;
	m_InterSectData.InterN = m_InterSectData.interE = 0.0;
	m_InterSectData.MinAng = m_InterSectData.MinDetaH = 0.0;

	m_RelRailName.Empty();
	XYArray=NULL;
	XYJDNO=NULL;
	ZQZJB=NULL;
	RtjMin=0;
	_tcscpy(RoadName , L"未命名道路");
	_tcscpy(mdbname , L"未命名");
	_stprintf(LCGH, L"K");
	_stprintf(JDGH, L"JD");
	SCALE=2.0;
	texth=6.0;
	NLC=3;
	DLBlength=18.0;
	DrawLeftOrRight=-1;
	JD0=1;
	FLDNo=0;
	SHOWJD=true;
	JdCenNum=0;
	XYNum = 0;
	//断链表
	DLArray.SetSize(2);
	setDLM(0,0.0,0.0,LCGH,LCGH);
	setDLM(1,1.0E4,1.0E4,LCGH,LCGH);

	DLNum=2;	
	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS", L"acadiso.lin");//

	IfCalXY = TRUE;
	IfDelJZX = FALSE;
	PreJD = -1;
	Ls1 = 0;
	NZLMK=NYLMK=0;
	BxNum = 0;
	ZLMK=YLMK=NULL;

	XYNum = xyn;
	if(XYNum>0)
		XYArray = new double[XYNum+1][10];
	int j;
	for(int i=0;i<=xyn;i++)
		for(j=0;j<10;j++)
			XYArray[i][j]=xy[i][j];
	XLLength = XYArray[XYNum][6];
	JD0 = (int)(XYArray[0][5]+0.001);
	LenOfLine = XLLength - XYArray[0][4];
}

JD_CENTER::~JD_CENTER()
{
	if(XYArray)
	{
		delete []XYArray;
		XYArray=NULL;
	}
	if(XYJDNO)
	{
		delete []XYJDNO;
		XYJDNO=NULL;
	}

	if(ZQZJB)
	{
		delete []ZQZJB;
		ZQZJB=NULL;
	}
}

//传入交点圆心数据
void JD_CENTER::SetJdCenterArray(JdCenter *JCAy, int num)
{
	JdCenArray.RemoveAll();
	if (num<1)
	{
		return;
	}
	for (int i = 0; i < num; i++)
	{
		JdCenArray.Add(JCAy[i]);
		//if (JdCenArray[i].JDXZ == IsCenter); //回头判断
		//	JdCenArray[i].IsOrNotHuiTou = 0;
	}

	if (fabs(JdCenArray[0].A1) < 0.00001)
		JdCenArray[0].A1 = 0.0;

	JD0 = (int)(JdCenArray[0].A2+0.1);//a2起始交点号
	JdCenArray[0].A2=0.0;

	if (fabs(JdCenArray[num - 1].A1) < 0.00001)
		JdCenArray[num - 1].A1 = 0.0;
	JdCenArray[num - 1].A2 = 0.0;
	JdCenArray[num - 1].IsOrNotHuiTou = 0;
	JdCenNum = num;


	DLArray.SetSize(2);
	setDLM(0,JdCenArray[0].A1,JdCenArray[0].A1,LCGH,LCGH);
	setDLM(1,1.0E4,1.0E4,LCGH,LCGH);
	CalXYArray();
}

//传入断链表
void JD_CENTER::setDLB(DLBZ *dlb,int ndl)
{
	assertWriteEnabled();
	DLArray.RemoveAll();
	DLNum=ndl;
	for(int i=0;i<DLNum;i++) 
	{
		DLArray.Add(dlb[i]);
	}

	GetDLTylc();
}

//求出交点
//求出交点
/*
void JD_CENTER::CalJiaoDian()
{
CString Alert;
int i;
double fb,fe;
JdCenNum=JdCenArray.GetSize();
//	if(JdCenNum<3)return;
double (*BianArray)[4]=new double[JdCenNum-1][4];
JdCenter *PreJC,*CurJC,*AftJC;
//起点
PreJC=&JdCenArray[0];
PreJC->JDN=PreJC->N;
PreJC->JDE=PreJC->E;
//终点
AftJC=&JdCenArray[JdCenNum-1];
AftJC->JDN=AftJC->N;
AftJC->JDE=AftJC->E;

if(JdCenNum==2)
{
BianArray[0][0]=PreJC->N;
BianArray[0][1]=PreJC->E;
BianArray[0][2]=AftJC->N;
BianArray[0][3]=AftJC->E;
}
for(i=1;i<JdCenNum-1;i++)//起终点除外
{
PreJC=&JdCenArray[i-1];
CurJC=&JdCenArray[i];
AftJC=&JdCenArray[i+1];
//ads_printf(L"22222  A1=%lf A2=%lf\n",CurJC->A1,CurJC->A2);

if(CurJC->LinkAfter==Luan_Link)
{
if(AftJC->A1<0.0001)AftJC->A1=CurJC->A2;
CurJC->A2=0.0;
}

if(i==1)//求前一条边
{
BianArray[0][0]=PreJC->N;
BianArray[0][1]=PreJC->E;
if(CurJC->JDXZ==IsJiaoDian)
{
BianArray[0][2]=CurJC->N;
BianArray[0][3]=CurJC->E;
}
else
{
double fwj=GetFwjByJd_Center_A_LR(BianArray[0][0],BianArray[0][1],CurJC->N,CurJC->E,
CurJC->R,CurJC->A1,CurJC->LeftOrRight);


BianArray[0][2]=BianArray[0][0]+10.0*cos(fwj);
BianArray[0][3]=BianArray[0][1]+10.0*sin(fwj);
}
}

//求后一条边
if(CurJC->JDXZ==IsJiaoDian)
{
BianArray[i][0]=CurJC->N;
BianArray[i][1]=CurJC->E;
if(AftJC->JDXZ==IsJiaoDian)
{
BianArray[i][2]=AftJC->N;
BianArray[i][3]=AftJC->E;
}
else//center
{
double fwj=GetFwjByJd_Center_A_LR(BianArray[i][0],BianArray[i][1],AftJC->N,AftJC->E,
AftJC->R,AftJC->A1,AftJC->LeftOrRight);
BianArray[i][2]=BianArray[i][0]+10.0*cos(fwj);
BianArray[i][3]=BianArray[i][1]+10.0*sin(fwj);
}

}
else//center
{
if(AftJC->JDXZ==IsJiaoDian)
{
BianArray[i][2]=AftJC->N;
BianArray[i][3]=AftJC->E;

double fwj=GetFwjByJd_Center_A_LR(BianArray[i][2],BianArray[i][3],CurJC->N,CurJC->E,
CurJC->R,CurJC->A2,-1*CurJC->LeftOrRight);
BianArray[i][0]=BianArray[i][2]+10.0*cos(fwj);
BianArray[i][1]=BianArray[i][3]+10.0*sin(fwj);
}
else//center
{
if(CurJC->LinkAfter==S_Link)
{
YYSLink YYSObj( CurJC->N,CurJC->E,CurJC->R,
AftJC->N,AftJC->E,AftJC->R,
CurJC->A2,AftJC->A1,CurJC->LeftOrRight);					
if(YYSObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
//						AfxMessageBox(Alert);
acutPrintf(L"%s\n",Alert);
return;
}

BianArray[i][0]=YYSObj.pSPIRAL1.StartN;
BianArray[i][1]=YYSObj.pSPIRAL1.StartE;
//acutPrintf(L"GQ: %lf,%lf\n",BianArray[i][1],BianArray[i][0]);
BianArray[i][2]=BianArray[i][0]+10.0*cos(YYSObj.pSPIRAL1.StartFwj+PI);
BianArray[i][3]=BianArray[i][1]+10.0*sin(YYSObj.pSPIRAL1.StartFwj+PI);


}
else if(CurJC->LinkAfter==C_Link)
{
YYCLink YYCObj( CurJC->N,CurJC->E,CurJC->R,
AftJC->N,AftJC->E,AftJC->R,
CurJC->A2,AftJC->A1,CurJC->LeftOrRight);					
if(YYCObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
//						AfxMessageBox(Alert);
acutPrintf(L"%s\n",Alert);
return;
}

BianArray[i][0]=YYCObj.pSPIRAL1.StartN;
BianArray[i][1]=YYCObj.pSPIRAL1.StartE;
BianArray[i][2]=BianArray[i][0]+10.0*cos(YYCObj.pSPIRAL1.StartFwj+PI);
BianArray[i][3]=BianArray[i][1]+10.0*sin(YYCObj.pSPIRAL1.StartFwj+PI);
}
else if(CurJC->LinkAfter==Luan_Link)
{
YYLuanLink YYLuanObj(CurJC->N,CurJC->E,CurJC->R,
AftJC->N,AftJC->E,AftJC->R,CurJC->LeftOrRight);

if(YYLuanObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
//						AfxMessageBox(Alert);
acutPrintf(L"%s\n",Alert);
return;
}
if(CurJC->R>AftJC->R)
{
BianArray[i][0]=YYLuanObj.pSPIRAL.StartN;
BianArray[i][1]=YYLuanObj.pSPIRAL.StartE;
BianArray[i][2]=BianArray[i][0]+10.0*cos(YYLuanObj.pSPIRAL.StartFwj);
BianArray[i][3]=BianArray[i][1]+10.0*sin(YYLuanObj.pSPIRAL.StartFwj);

}
else
{
BianArray[i][0]=YYLuanObj.pSPIRAL.EndN;
BianArray[i][1]=YYLuanObj.pSPIRAL.EndE;
BianArray[i][2]=BianArray[i][0]+10.0*cos(YYLuanObj.pSPIRAL.EndFwj+PI);
BianArray[i][3]=BianArray[i][1]+10.0*sin(YYLuanObj.pSPIRAL.EndFwj+PI);
}
}
else//NotRelation
{
YYQXLink YYQXObj;
YYQXObj.SetCir1(CurJC->N,CurJC->E,CurJC->R);
YYQXObj.SetCir2(AftJC->N,AftJC->E,AftJC->R);
YYQXObj.SetA(CurJC->A2,AftJC->A1);

if(CurJC->LeftOrRight<0 && AftJC->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
else if(CurJC->LeftOrRight>0 && AftJC->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
else if(CurJC->LeftOrRight<0 && AftJC->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
else YYQXObj.SetQXClass(S_S);

YYQXObj.CalQX();

if(YYQXObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
if(CurJC->A2>0.0)
{
BianArray[i][0]=YYQXObj.pSPIRAL1.BaseN;
BianArray[i][1]=YYQXObj.pSPIRAL1.BaseE;
}
else
{
BianArray[i][0]=YYQXObj.GQN1;
BianArray[i][1]=YYQXObj.GQE1;
}
if(AftJC->A1>0.0)
{
BianArray[i][2]=YYQXObj.pSPIRAL2.BaseN;
BianArray[i][3]=YYQXObj.pSPIRAL2.BaseE;					
}
else
{
BianArray[i][2]=YYQXObj.GQN2;
BianArray[i][3]=YYQXObj.GQE2;
}
}
}
}

//求交点坐标
BAS_DRAW_FUN::INTERS(&CurJC->JDN,&CurJC->JDE,
BianArray[i-1][0],BianArray[i-1][1],BianArray[i-1][2],BianArray[i-1][3],
BianArray[i][0],BianArray[i][1],BianArray[i][2],BianArray[i][3]);
ads_printf(L"###### %lf %lf\n",CurJC->JDN,CurJC->JDE);
}


if(JdCenNum==2)
{
PreJC=&JdCenArray[0];
CurJC=&JdCenArray[1];
xyddaa(PreJC->JDN,PreJC->JDE,CurJC->JDN,CurJC->JDE,&fb);
PreJC->AftFwj=fb;
CurJC->PreFwj=fb+PI;	
StdJiao(CurJC->PreFwj);
}
for(i=1;i<JdCenNum-1;i++)//起终点除外
{
PreJC=&JdCenArray[i-1];
CurJC=&JdCenArray[i];
AftJC=&JdCenArray[i+1];
if(i==1)
{
char mes[280];
PreJC->DistToAfterJD=xyddaa(PreJC->JDN,PreJC->JDE,CurJC->JDN,CurJC->JDE,&fb);

xyddaa(BianArray[i-1][0],BianArray[i-1][1],BianArray[i-1][2],BianArray[i-1][3],&fb);//ph 041230

PreJC->AftFwj=fb;
}

CurJC->DistToAfterJD=xyddaa(CurJC->JDN,CurJC->JDE,AftJC->JDN,AftJC->JDE,&fe);
xyddaa(BianArray[i][0],BianArray[i][1],BianArray[i][2],BianArray[i][3],&fe);//ph 041230
if(i==JdCenNum-2){AftJC->PreFwj=fe+PI;	StdJiao(AftJC->PreFwj);}

CurJC->PreFwj=fb+PI;	StdJiao(CurJC->PreFwj);
CurJC->AftFwj=fe;	StdJiao(CurJC->AftFwj);

CurJC->afa=CurJC->AftFwj-(CurJC->PreFwj+PI);


/*	double s;
s=s2-s1;
if(fabs(s)>pi)
{
if(s<0.0) //偏角小于-PI
{
s=2.0*pi+s;
}
else      //偏角大于PI
{
s=s-2.0*pi;
}
}*/

/*		if (CurJC->afa<-pi) CurJC->afa=CurJC->afa+2*pi;
else if (CurJC->afa>pi) CurJC->afa=CurJC->afa-2*pi;
if(CurJC->JDXZ==IsJiaoDian)
{
if(CurJC->afa<0.0) CurJC->LeftOrRight=LEFT;
else CurJC->LeftOrRight=RIGHT;
}


//		CurJC->afa=fabs(CurJC->afa);
fb=fe;
}
delete []BianArray;
}*/
//计算所有交点的NE坐标和交点的前后方位角	20190708注释
void JD_CENTER::CalJiaoDian()
{
	CString Alert;
	int i;
	double fb,fe;
	JdCenNum=JdCenArray.GetSize();
	//	if(JdCenNum<3)return;
	double (*BianArray)[4]=new double[JdCenNum-1][4];
	JdCenter *PreJC,*CurJC,*AftJC;
	//起点
	PreJC=&JdCenArray[0];
	PreJC->JDN=PreJC->N;
	PreJC->JDE=PreJC->E;
	//终点
	AftJC=&JdCenArray[JdCenNum-1];
	AftJC->JDN=AftJC->N;
	AftJC->JDE=AftJC->E;
	if(JdCenNum==2)
	{
		BianArray[0][0]=PreJC->N;
		BianArray[0][1]=PreJC->E;
		BianArray[0][2]=AftJC->N;
		BianArray[0][3]=AftJC->E;
	}
	for(i=1;i<JdCenNum-1;i++)//起终点除外
	{
		PreJC=&JdCenArray[i-1];
		CurJC=&JdCenArray[i];
		AftJC=&JdCenArray[i+1];

		//		if(CurJC->LinkAfter==Luan_Link)
		//		{
		//			if(AftJC->A1<0.0001)AftJC->A1=CurJC->A2;
		//			CurJC->A2=0.0;
		//		}2006.9.22改

		if(i==1)//求前一条边
		{
			BianArray[0][0]=PreJC->N;
			BianArray[0][1]=PreJC->E;
			if(CurJC->JDXZ==IsJiaoDian)
			{
				BianArray[0][2]=CurJC->N;
				BianArray[0][3]=CurJC->E;
			}
			else
			{
				CurJC->IsOrNotHuiTou=0;
				double fwj=GetFwjByJd_Center_A_LR(BianArray[0][0],BianArray[0][1],CurJC->N,CurJC->E,
					CurJC->R, CurJC->A1, CurJC->LeftOrRight);
				BianArray[0][2] = BianArray[0][0] + 400.0 * cos(fwj);
				BianArray[0][3] = BianArray[0][1] + 400.0 * sin(fwj);
			}
		}
		//求后一条边
		if(CurJC->JDXZ==IsJiaoDian)
		{
			BianArray[i][0]=CurJC->N;
			BianArray[i][1]=CurJC->E;
			if(AftJC->JDXZ==IsJiaoDian)
			{
				BianArray[i][2]=AftJC->N;
				BianArray[i][3]=AftJC->E;
			}
			else//center
			{
				double fwj=GetFwjByJd_Center_A_LR(BianArray[i][0],BianArray[i][1],AftJC->N,AftJC->E,
					AftJC->R,AftJC->A1,AftJC->LeftOrRight);
				BianArray[i][2]=BianArray[i][0]+400.0*cos(fwj);
				BianArray[i][3]=BianArray[i][1]+400.0*sin(fwj);
			}
		}
		else//center
		{
			CurJC->IsOrNotHuiTou=0;
			if(AftJC->JDXZ==IsJiaoDian)
			{
				BianArray[i][2]=AftJC->N;
				BianArray[i][3]=AftJC->E;

				double fwj=GetFwjByJd_Center_A_LR(BianArray[i][2],BianArray[i][3],CurJC->N,CurJC->E,
					CurJC->R,CurJC->A2,-1*CurJC->LeftOrRight);
				BianArray[i][0]=BianArray[i][2]+400.0*cos(fwj);
				BianArray[i][1]=BianArray[i][3]+400.0*sin(fwj);

			}
			else//center
			{
				if(CurJC->LinkAfter==S_Link)
				{
					YYSLink YYSObj( CurJC->N,CurJC->E,CurJC->R,
						AftJC->N,AftJC->E,AftJC->R,
						CurJC->A2,AftJC->A1,CurJC->LeftOrRight);					
					if(YYSObj.ERRFlag)
					{
						Alert.Format(L"第%d个交点出错!",i);
						acutPrintf(L"%s\n",Alert);
						return;
					}

					BianArray[i][0]=YYSObj.pSPIRAL1.StartN;
					BianArray[i][1]=YYSObj.pSPIRAL1.StartE;
					//acutPrintf(L"GQ: %lf,%lf\n",BianArray[i][1],BianArray[i][0]);
					BianArray[i][2]=BianArray[i][0]+400.0*cos(YYSObj.pSPIRAL1.StartFwj+PI);
					BianArray[i][3]=BianArray[i][1]+400.0*sin(YYSObj.pSPIRAL1.StartFwj+PI);

				}
				else if(CurJC->LinkAfter==C_Link)
				{
					YYCLink YYCObj( CurJC->N,CurJC->E,CurJC->R,
						AftJC->N,AftJC->E,AftJC->R,
						CurJC->A2,AftJC->A1,CurJC->LeftOrRight);					
					if(YYCObj.ERRFlag)
					{
						Alert.Format(L"第%d个交点出错!",i);
						acutPrintf(L"%s\n",Alert);
						return;
					}

					BianArray[i][0]=YYCObj.pSPIRAL1.StartN;
					BianArray[i][1]=YYCObj.pSPIRAL1.StartE;
					BianArray[i][2]=BianArray[i][0]+400.0*cos(YYCObj.pSPIRAL1.StartFwj+PI);
					BianArray[i][3]=BianArray[i][1]+400.0*sin(YYCObj.pSPIRAL1.StartFwj+PI);
				}
				else if(CurJC->LinkAfter==Luan_Link)
				{
					YYLuanLink YYLuanObj(CurJC->N,CurJC->E,CurJC->R,
						AftJC->N,AftJC->E,AftJC->R,CurJC->LeftOrRight);
					//ads_printf(L"$$%0.2lf %lf %lf %lf %lf %lf %d %lf\n",CurJC->N,CurJC->E,CurJC->R,AftJC->N,AftJC->E,AftJC->R,CurJC->LeftOrRight,YYLuanObj.pSPIRAL.A);
					if(YYLuanObj.ERRFlag)
					{
						Alert.Format(L"第%d个交点出错!",i);
						acutPrintf(L"%s\n",Alert);
						//	return;
					}
					if(CurJC->R>AftJC->R)
					{
						BianArray[i][0]=YYLuanObj.pSPIRAL.StartN;
						BianArray[i][1]=YYLuanObj.pSPIRAL.StartE;
						BianArray[i][2]=BianArray[i][0]+400.0*cos(YYLuanObj.pSPIRAL.StartFwj);
						BianArray[i][3]=BianArray[i][1]+400.0*sin(YYLuanObj.pSPIRAL.StartFwj);
					}
					else
					{
						BianArray[i][0]=YYLuanObj.pSPIRAL.EndN;
						BianArray[i][1]=YYLuanObj.pSPIRAL.EndE;
						BianArray[i][2]=BianArray[i][0]+400.0*cos(YYLuanObj.pSPIRAL.EndFwj+PI);
						BianArray[i][3]=BianArray[i][1]+400.0*sin(YYLuanObj.pSPIRAL.EndFwj+PI);
					}

				}
				else//NotRelation
				{
					YYQXLink YYQXObj;
					YYQXObj.SetCir1(CurJC->N,CurJC->E,CurJC->R);
					YYQXObj.SetCir2(AftJC->N,AftJC->E,AftJC->R);
					YYQXObj.SetA(CurJC->A2,AftJC->A1);

					if(CurJC->LeftOrRight<0 && AftJC->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
					else if(CurJC->LeftOrRight>0 && AftJC->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
					else if(CurJC->LeftOrRight<0 && AftJC->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
					else YYQXObj.SetQXClass(S_S);

					YYQXObj.CalQX();

					if(YYQXObj.ERRFlag)
					{
						Alert.Format(L"第%d个交点出错!",i);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					if(CurJC->A2>0.0)
					{
						BianArray[i][0]=YYQXObj.pSPIRAL1.BaseN;
						BianArray[i][1]=YYQXObj.pSPIRAL1.BaseE;
					}
					else
					{
						BianArray[i][0]=YYQXObj.GQN1;
						BianArray[i][1]=YYQXObj.GQE1;
					}
					if(AftJC->A1>0.0)
					{
						BianArray[i][2]=YYQXObj.pSPIRAL2.BaseN;
						BianArray[i][3]=YYQXObj.pSPIRAL2.BaseE;					
					}
					else
					{
						BianArray[i][2]=YYQXObj.GQN2;
						BianArray[i][3]=YYQXObj.GQE2;
					}
				}
			}
		}
		//求交点坐标
		BAS_DRAW_FUN::INTERS(&CurJC->JDN,&CurJC->JDE,
			BianArray[i-1][0],BianArray[i-1][1],BianArray[i-1][2],BianArray[i-1][3],
			BianArray[i][0],BianArray[i][1],BianArray[i][2],BianArray[i][3]);

		/*

		AcGePoint3d pt1,pt2;
		pt1.x = BianArray[i-1][1];
		pt1.y = BianArray[i-1][0];
		pt2.x = BianArray[i-1][3];
		pt2.y = BianArray[i-1][2];
		AcDbObjectId ID;
		makeline(pt1,pt2,ID,5);	*/
	}
	delete []BianArray;

	if(JdCenNum==2)
	{
		PreJC=&JdCenArray[0];
		CurJC=&JdCenArray[1];
		PreJC->DistToAfterJD=xyddaa(PreJC->JDN,PreJC->JDE,CurJC->JDN,CurJC->JDE,&fb);
		PreJC->AftFwj=fb;
		CurJC->PreFwj=fb+PI;	StdJiao(CurJC->PreFwj);
	}

	for(i=1;i<JdCenNum-1;i++)//起终点除外
	{
		PreJC=&JdCenArray[i-1];
		CurJC=&JdCenArray[i];
		AftJC=&JdCenArray[i+1];

		if(i==1)
		{
			if(CurJC->IsOrNotHuiTou<=2)//不回头或同向回头
				PreJC->DistToAfterJD=xyddaa(PreJC->JDN,PreJC->JDE,CurJC->JDN,CurJC->JDE,&fb);
			else//逆向回头
				PreJC->DistToAfterJD=xyddaa(CurJC->JDN,CurJC->JDE,PreJC->JDN,PreJC->JDE,&fb);
			PreJC->AftFwj=fb;
		}

		CurJC->DistToAfterJD=xyddaa(CurJC->JDN,CurJC->JDE,AftJC->JDN,AftJC->JDE,&fe);

		if(CurJC->JDXZ==IsJiaoDian)
		{
			if(AftJC->IsOrNotHuiTou>2)//逆向回头
				CurJC->DistToAfterJD=xyddaa(AftJC->JDN,AftJC->JDE,CurJC->JDN,CurJC->JDE,&fe);
			else//后一交点不回头
			{
				if(CurJC->IsOrNotHuiTou==2||CurJC->IsOrNotHuiTou==4)//右转回头曲线
				{
					double zj;
					zj = ZXJ(fe,fb);
					if(zj>0.0)
						zj += pi;
					else
						zj = 2*pi+zj;
					fe=fb+zj;				
					if(fe>2*PI)
						fe-= 2*PI;
				}

				if(CurJC->IsOrNotHuiTou==1||CurJC->IsOrNotHuiTou==3)//左转回头曲线
				{
					double zj;
					zj = ZXJ(fe,fb);
					if(zj>0.0)
						zj = zj - 2*pi ;
					else
						zj = zj-pi;
					fe=fb+zj;				
					if(fe<0)
						fe+= 2*PI;
				}

			}
		}


		if(i==JdCenNum-2){AftJC->PreFwj=fe+PI;	StdJiao(AftJC->PreFwj);}

		CurJC->PreFwj=fb+PI;	StdJiao(CurJC->PreFwj);
		CurJC->AftFwj=fe;	StdJiao(CurJC->AftFwj);

		double pfwj;
		pfwj =CurJC->PreFwj+PI;
		StdJiao(pfwj);

		CurJC->afa=ZXJ(CurJC->AftFwj,pfwj);
		//	CurJC->afa=	CurJC->AftFwj-(CurJC->PreFwj+PI);
		//	if (CurJC->afa<-pi) CurJC->afa=CurJC->afa+2*pi;
		//	if (CurJC->afa>pi) CurJC->afa=CurJC->afa-2*pi;

		if(CurJC->JDXZ==IsJiaoDian)
		{
			if(CurJC->afa<0.0)CurJC->LeftOrRight=LEFT;
			else CurJC->LeftOrRight=RIGHT;

			if(CurJC->IsOrNotHuiTou)
			{
				if(CurJC->IsOrNotHuiTou==1 || CurJC->IsOrNotHuiTou==3)//左转回头
				{
					if(CurJC->LeftOrRight==-1)//原算的左转
						CurJC->afa = -pi + CurJC->afa;
					else
						CurJC->afa = 2*pi - CurJC->afa;
					CurJC->LeftOrRight=-1;
				}
				else if(CurJC->IsOrNotHuiTou==2 || CurJC->IsOrNotHuiTou==4)//右转回头
				{
					if(CurJC->LeftOrRight==-1)//原算的左转
						CurJC->afa = 2*pi + CurJC->afa;
					else
						CurJC->afa = pi + CurJC->afa;
					CurJC->LeftOrRight=1;
				}                
			}
		}

		//		CurJC->afa=fabs(CurJC->afa);
		//若2*beita>=afa修正A值
		double beta1,beta2;
		if(CurJC->R>0.0001 && CurJC->JDXZ==IsJiaoDian && CurJC->LinkAfter==NotRelation )
		{
			beta1 = 0.5*CurJC->A1*CurJC->A1/CurJC->R/CurJC->R;
			beta2 = 0.5*CurJC->A2*CurJC->A2/CurJC->R/CurJC->R;
			if(beta1+beta2>fabs(CurJC->afa))
			{
				//设计成凸行
				ads_printf(L"%d个控制点两缓和曲线转角和大于转角,设计成凸形!\n",i);

				LLTuLink tu_type(PreJC->JDN,PreJC->JDE,CurJC->JDN,CurJC->JDE,
					AftJC->JDN,AftJC->JDE,CurJC->A1,CurJC->A2);

			}
		}

		fb=fe;
	}

	for(i=0;i<JdCenNum;i++)
	{
		double hfwj = JdCenArray[i].AftFwj+PI;
		if(hfwj>2*PI)
			hfwj-=2*PI;
		//	ads_printf(L"i=%d %d prfwj=%lf hfwj=%lf %lf %lf\n",i,JdCenArray[i].LeftOrRight,JdCenArray[i].PreFwj,hfwj,JdCenArray[i].JDN,JdCenArray[i].JDE);
	}


	//	ads_printf(L"@@@@@@@@@@@@@@%lf %lf\n",JdCenArray[JdCenNum-1].N,JdCenArray[JdCenNum-1].E);
}

void JD_CENTER::CalJiaoDian_New()
{
	CString Alert;
	int i;
	double fb, fe;
	JdCenNum = JdCenArray.GetSize();
	//	if(JdCenNum<3)return;
	double(*BianArray)[4] = new double[JdCenNum - 1][4];
	JdCenter* PreJC, * CurJC, * AftJC;
	//起点
	PreJC = &JdCenArray[0];
	PreJC->JDN = PreJC->N;
	PreJC->JDE = PreJC->E;
	//终点
	AftJC = &JdCenArray[JdCenNum - 1];
	AftJC->JDN = AftJC->N;
	AftJC->JDE = AftJC->E;
	if (JdCenNum == 2)
	{
		BianArray[0][0] = PreJC->N;
		BianArray[0][1] = PreJC->E;
		BianArray[0][2] = AftJC->N;
		BianArray[0][3] = AftJC->E;
	}
	for (i = 1; i < JdCenNum - 1; i++)//起终点除外
	{
		PreJC = &JdCenArray[i - 1];
		CurJC = &JdCenArray[i];
		AftJC = &JdCenArray[i + 1];

		if (i == 1)//求前一条边
		{
			BianArray[0][0] = PreJC->N;
			BianArray[0][1] = PreJC->E;

			BianArray[0][2] = CurJC->N;
			BianArray[0][3] = CurJC->E;
		}

		//求后一条边
		BianArray[i][0] = CurJC->N;
		BianArray[i][1] = CurJC->E;
		BianArray[i][2] = AftJC->N;
		BianArray[i][3] = AftJC->E;

		//求交点坐标
		BAS_DRAW_FUN::INTERS(&CurJC->JDN, &CurJC->JDE,
			BianArray[i - 1][0], BianArray[i - 1][1], BianArray[i - 1][2], BianArray[i - 1][3],
			BianArray[i][0], BianArray[i][1], BianArray[i][2], BianArray[i][3]);
	}
	delete[]BianArray;

	if (JdCenNum == 2)
	{
		PreJC = &JdCenArray[0];
		CurJC = &JdCenArray[1];
		PreJC->DistToAfterJD = xyddaa(PreJC->JDN, PreJC->JDE, CurJC->JDN, CurJC->JDE, &fb);
		PreJC->AftFwj = fb;
		CurJC->PreFwj = fb + PI;	
		StdJiao(CurJC->PreFwj);
	}

	for (i = 1; i < JdCenNum - 1; i++)//起终点除外
	{
		PreJC = &JdCenArray[i - 1];
		CurJC = &JdCenArray[i];
		AftJC = &JdCenArray[i + 1];

		if (i == 1)
		{
			if (CurJC->IsOrNotHuiTou <= 2)//不回头或同向回头
				PreJC->DistToAfterJD = xyddaa(PreJC->JDN, PreJC->JDE, CurJC->JDN, CurJC->JDE, &fb);
			else//逆向回头
				PreJC->DistToAfterJD = xyddaa(CurJC->JDN, CurJC->JDE, PreJC->JDN, PreJC->JDE, &fb);
			PreJC->AftFwj = fb;
		}

		CurJC->DistToAfterJD = xyddaa(CurJC->JDN, CurJC->JDE, AftJC->JDN, AftJC->JDE, &fe);

		if (AftJC->IsOrNotHuiTou > 2)//逆向回头
			CurJC->DistToAfterJD = xyddaa(AftJC->JDN, AftJC->JDE, CurJC->JDN, CurJC->JDE, &fe);
		else//后一交点不回头
		{
			if (CurJC->IsOrNotHuiTou == 2 || CurJC->IsOrNotHuiTou == 4)//右转回头曲线
			{
				double zj;
				zj = ZXJ(fe, fb);
				if (zj > 0.0)
					zj += pi;
				else
					zj = 2 * pi + zj;
				fe = fb + zj;
				if (fe > 2 * PI)
					fe -= 2 * PI;
			}

			if (CurJC->IsOrNotHuiTou == 1 || CurJC->IsOrNotHuiTou == 3)//左转回头曲线
			{
				double zj;
				zj = ZXJ(fe, fb);
				if (zj > 0.0)
					zj = zj - 2 * pi;
				else
					zj = zj - pi;
				fe = fb + zj;
				if (fe < 0)
					fe += 2 * PI;
			}

		}

		if (i == JdCenNum - 2) 
		{ 
			AftJC->PreFwj = fe + PI;	
			StdJiao(AftJC->PreFwj); 
		}

		CurJC->PreFwj = fb + PI;	
		StdJiao(CurJC->PreFwj);
		CurJC->AftFwj = fe;	
		StdJiao(CurJC->AftFwj);

		double pfwj;
		pfwj = CurJC->PreFwj + PI;
		StdJiao(pfwj);

		CurJC->afa = ZXJ(CurJC->AftFwj, pfwj);

		if (CurJC->afa < 0.0)CurJC->LeftOrRight = LEFT;
		else CurJC->LeftOrRight = RIGHT;

		if (CurJC->IsOrNotHuiTou)
		{
			if (CurJC->IsOrNotHuiTou == 1 || CurJC->IsOrNotHuiTou == 3)//左转回头
			{
				if (CurJC->LeftOrRight == -1)//原算的左转
					CurJC->afa = -pi + CurJC->afa;
				else
					CurJC->afa = 2 * pi - CurJC->afa;
				CurJC->LeftOrRight = -1;
			}
			else if (CurJC->IsOrNotHuiTou == 2 || CurJC->IsOrNotHuiTou == 4)//右转回头
			{
				if (CurJC->LeftOrRight == -1)//原算的左转
					CurJC->afa = 2 * pi + CurJC->afa;
				else
					CurJC->afa = pi + CurJC->afa;
				CurJC->LeftOrRight = 1;
			}
		}


		//若2*beita>=afa修正A值
		//double beta1, beta2;
		//if (CurJC->R > 0.0001 && CurJC->JDXZ == IsJiaoDian && CurJC->LinkAfter == NotRelation)
		//{
		//	beta1 = 0.5 * CurJC->A1 * CurJC->A1 / CurJC->R / CurJC->R;
		//	beta2 = 0.5 * CurJC->A2 * CurJC->A2 / CurJC->R / CurJC->R;
		//	if (beta1 + beta2 > fabs(CurJC->afa))
		//	{
		//		//设计成凸行
		//		ads_printf(L"%d个控制点两缓和曲线转角和大于转角,设计成凸形!\n", i);

		//		LLTuLink tu_type(PreJC->JDN, PreJC->JDE, CurJC->JDN, CurJC->JDE,
		//			AftJC->JDN, AftJC->JDE, CurJC->A1, CurJC->A2);

		//	}
		//}

		fb = fe;
	}

	for (i = 0; i < JdCenNum; i++)
	{
		double hfwj = JdCenArray[i].AftFwj + PI;
		if (hfwj > 2 * PI)
			hfwj -= 2 * PI;
	}
}

//计算所有的A值

///////////////////////////////////////////////////////////////////////////////////////
//从1号交点开始循环，到倒数第二个交点止
//一、如果当前为交点
//从上一个缓止点计算到当前交点的缓止点
//二、如果当前为圆心
//(一)与前面连接
//1、如果与前面 圆心 为卵型、S、C连接：
//    从前一段不完整缓和曲线算起
//2、交点 无关
//    从上一个缓止点算起
//(二)与后面连接
//1、无关、S、C
//   算至后缓
//2、与后面圆心成卵型连接
//   算至圆弧终点至
///////////////////////////////////////////////////////////////////////////////////////
//从1号交点开始循环，到倒数第二个交点止
//一、如果当前为交点
//从上一个缓直点计算到当前交点的缓止点
//
//二、如果当前为圆心
//(一)与前面连接
//1、如果与前面 圆心 为卵型、S、C连接：
//    从前一段不完整缓和曲线算起
//2、交点 无关
//    从上一个缓止点算起
//(二)与后面连接
//1、无关、S、C
//   算至后缓
//2、与后面圆心成卵型连接
//   算至圆弧终点至
void JD_CENTER::CalXYArray()
{
	//检测交点数量
	double temp = 0;
	if(JdCenNum<2)
	{
		AfxMessageBox(L"控制点个数小于两个!");
		JdCenNum=0;
		return;
	}
	//赋值起点的初始里程？
	if(DLNum > 0 )
	{
		if(fabs(JdCenArray[0].A1-DLArray[0].ELC)>0.001)
			JdCenArray[0].A1 = DLArray[0].ELC;
	}
	//处理只有两个交点的情况
	if(JdCenNum==2)
	{
		double dN,dE;
		dN=JdCenArray[0].N-JdCenArray[1].N;
		dE=JdCenArray[0].E-JdCenArray[1].E;
		if(sqrt(dN*dN+dE*dE)<0.1)
		{
			AfxMessageBox(L"控制点数据有误!");
			JdCenNum=0;
			return;
		}
	}

	vector<JdCenter> watchvec;
	for (int i = 0; i < JdCenArray.GetSize(); i++)
		watchvec.push_back(JdCenArray[i]);

	CalJiaoDian();
	//CalJiaoDian_New();
	watchvec.clear();
	for (int i = 0; i < JdCenArray.GetSize(); i++)
		watchvec.push_back(JdCenArray[i]);

	int i;
	//计算切线长
	Cal_AllJD_T();
	int k;
	double aa,BPMTL[10],INDATA[5],l,T1,T2,Lo1,Lo2,L,R,
		Nzh,Ezh;
	struct xlpoint PB0,PEZ;
	double N1,E1,N2,E2;//圆上两点
	double A1,A2;//前后缓
	SPIRAL PreSpiral,AftSpiral;
	JdCenter *PreJdCen,*CurJdCen,*AfterJdCen;
	CString Alert;
	JdCenNum=JdCenArray.GetSize();

	if(XYNum>0)
	{
		if(XYArray)
			delete []XYArray;
		XYArray = NULL;
	}

	XYNum=0;
	if(XYJDNO)
	{
		delete []XYJDNO;
		XYJDNO=NULL;
	}

	//为线元数组开空间，稍开大些
	XYArray=new double[JdCenNum*4][10];
	/*for (int i=0;i<JdCenNum*4;i++)
	{
	for(int j=0;j<10;j++)
	{
	XYArray[i][j]=0.0;
	}
	}*/
	for (int k=0;k<JdCenNum*4;k++)
	{
		for(int j=0;j<10;j++)
		{
			XYArray[k][j]=0.0;
		}
	}
	XYJDNO=new int[JdCenNum*4];
	XYNum=0;//线路起点
	XYArray[XYNum][0]=0.0;
	XYArray[XYNum][1]=JdCenArray[0].N;
	XYArray[XYNum][2]=JdCenArray[0].E;
	CurJdCen=&JdCenArray[0];
	AfterJdCen=&JdCenArray[1];
	CurJdCen->JdNo=JD0-1;
	//	CurJdCen->A1=0.0;
	CurJdCen->A2=0.0;
	CurJdCen->T1=0.0;
	CurJdCen->T2=0.0;
	XYArray[XYNum][3]=CurJdCen->AftFwj;//起点方位角
	XYArray[XYNum][4]=CurJdCen->A1;//线路起点里程
	//	XYArray[XYNum][5]=0.0;
	XYArray[XYNum][5]=JD0;//起始交点号
	XYArray[XYNum][6]=0.0;
	XYArray[XYNum][7]=0.0;
	XYArray[XYNum][8]=0.0;
	XYArray[XYNum][9]=0.0;
	XYJDNO[XYNum]=0;//对应起点
	XYNum++;
	PEZ.lc=XYArray[XYNum-1][4];
	PEZ.x =XYArray[XYNum-1][1];
	PEZ.y =XYArray[XYNum-1][2];
	PEZ.a =XYArray[XYNum-1][3];
	PEZ.r =1e40;
	//首先计算所有交点的前后两条边的方位角PreFwj,AftFwj/以当前交点为起点

	/*for( i=1;i<JdCenNum-1;i++)*/

	for(i=1 ;i<JdCenNum-1;i++)
	{
		//获取当前JD/CENTER指针
		CurJdCen=&JdCenArray[i];		
		CurJdCen->JdNo=i+JD0-1;//-1.0;//初值

		//为交点
		if(CurJdCen->JDXZ==IsJiaoDian)
		{
			bool HasJiaZX=true;
			//获取前、后JD/CENTER指针
			PreJdCen=&JdCenArray[i-1];
			AfterJdCen=&JdCenArray[i+1];      
			if( (PreJdCen->LinkAfter==S_Link || PreJdCen->LinkAfter==C_Link)
				&& CurJdCen->A1*CurJdCen->A2<0.1)
			{
				//当前交点的前切
				CurJdCen->T1 = PreJdCen->DistToAfterJD - PreJdCen->T2;
				if(CurJdCen->T1<0.0001)
				{  
					Alert.Format(L"1第%d个交点出错!",i);//acutPrintf(L"%s\n",Alert);
					return;
				}

				//保证前切
				if(fabs(CurJdCen->A1)<0.001 && fabs(CurJdCen->A2)<0.001)
				{
					//调整半径
					double ta=0.5*(PI-fabs(CurJdCen->afa));
					if(ta<0.001)
					{
						Alert.Format(L"2第%d个交点出错!",i);
						//AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					CurJdCen->R=CurJdCen->T1*tan(ta);
				}
				else if(fabs(CurJdCen->A1)<0.001)
				{
					//调整A2
					CurJdCen->A1=0.0;
					ThreeJDCalThisAFromThatA_PreT_R CalObj;
					CalObj.SetA1(0.0);
					CalObj.SetAfa(fabs(CurJdCen->afa));
					CalObj.SetPreT_R(CurJdCen->T1,CurJdCen->R);
					double a2;
					if(CalObj.GetA2(a2))CurJdCen->A2=a2;
				}
				else if(fabs(CurJdCen->A2)<0.001)
				{
					//调整A1
					CurJdCen->A2=0.0;
					ThreeJDCalThisAFromThatA_PreT_R CalObj;
					CalObj.SetA2(0.0);
					CalObj.SetAfa(fabs(CurJdCen->afa));
					CalObj.SetPreT_R(CurJdCen->T1,CurJdCen->R);
					double a1;
					if(CalObj.GetA1(a1))CurJdCen->A1=a1;
				}
			}			

			if(CurJdCen->LinkAfter==S_Link || CurJdCen->LinkAfter==C_Link)
			{
				if(i>=2 && (PreJdCen->LinkAfter==S_Link
					|| PreJdCen->LinkAfter==C_Link 
					/*|| PreJdCen->LinkAfter==Luan_Link*/))
				{
					if( CurJdCen->A1*CurJdCen->A2>0.1 )
					{								
						double tempT21,tempT12;
						//	tempT12=PreJdCen->T2;
						if(PreJdCen->LinkAfter==Luan_Link)
						{
							/*
							double cenN,cenE,Arcl;
							JDLuanLink::GetCenter(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,
							AfterJdCen->N,AfterJdCen->E,PreJdCen->R,CurJdCen->R,CurJdCen->A1,
							CurJdCen->A2,cenN,cenE,Arcl,tempT12);
							*/
							/*
							JDLuanLink JDLuanObj;
							JDLuanObj.SetPreJD(PreJdCen->JDN,PreJdCen->JDE,PreJdCen->R);
							JDLuanObj.SetCurJD(CurJdCen->JDN,CurJdCen->JDE,CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
							JDLuanObj.SetAftJD(AfterJdCen->JDN,AfterJdCen->JDE);
							CurJdCen->T1=PreJdCen->DistToAfterJD-PreJdCen->T2;
							JDLuanObj.SetPreT(CurJdCen->T1);
							JDLuanObj.UpdateData();
							if(JDLuanObj.ERRFlag)
							{
							Alert.Format(L"第%d个交点出错!",i);
							acutPrintf(L"%s\n",Alert);
							return;
							}
							CurJdCen->A1=JDLuanObj.PreA;
							CurJdCen->A2=JDLuanObj.AftA;
							CurJdCen->T2=JDLuanObj.AftT;
							*/
						}
						tempT12=PreJdCen->T2;

						Norm3JD Norm3jdObj(CurJdCen->JDN,CurJdCen->JDE,AfterJdCen->JDN,
							AfterJdCen->JDE,JdCenArray[i+2].JDN,JdCenArray[i+2].JDE,
							AfterJdCen->R,AfterJdCen->A1,AfterJdCen->A2);

						tempT21=Norm3jdObj.T1;
						double t1,t2;
						//						double PreDistJd,AftDistJd,tf;
						//						PreDistJd=xyddaa(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,&tf);
						//						AftDistJd=xyddaa(CurJdCen->JDN,CurJdCen->JDE,AfterJdCen->JDN,AfterJdCen->JDE,&tf);
						//						t1=PreDistJd-tempT12;
						//						t2=AftDistJd-tempT21;
						t1=PreJdCen->DistToAfterJD-tempT12;
						t2=CurJdCen->DistToAfterJD-tempT21;

						if(t1<-1.0||t2<-1.0)
						{
							Alert.Format(L"3第%d个交点出错!",i);
							//						    AfxMessageBox(Alert);
							acutPrintf(L"%s\n",Alert);
							return;
						}

						N = i;					
						ThreeJDCalA1A2FromT1T2R ThreeJdObj;
						ThreeJdObj.SetPreJd(PreJdCen->JDN,PreJdCen->JDE);
						ThreeJdObj.SetMidJd(CurJdCen->JDN,CurJdCen->JDE);
						ThreeJdObj.SetAftJd(AfterJdCen->JDN,AfterJdCen->JDE);
						ThreeJdObj.SetRadius(CurJdCen->R);
						ThreeJdObj.SetT1T2(t1,t2);
						ThreeJdObj.UpdateData();
						if(!ThreeJdObj.ERRFlag)
						{
							CurJdCen->A1=ThreeJdObj.A1;
							CurJdCen->A2=ThreeJdObj.A2;							
						}			
					}
				}
				else
				{	
					if(i+2<=JdCenNum-1)
					{
						FourJDLocationC_S JDCSObj;
						JDCSObj.SetFourJD(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,
							AfterJdCen->JDN,AfterJdCen->JDE,JdCenArray[i+2].JDN,JdCenArray[i+2].JDE);					
						JDCSObj.SetPreJDCS(CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
						JDCSObj.SetAftJDCS(AfterJdCen->R,AfterJdCen->A1,AfterJdCen->A2);
						JDCSObj.UpdateData();
						if(JDCSObj.ERRFlag!=true)
						{
							CurJdCen->A2=JDCSObj.A12;
							AfterJdCen->A1=JDCSObj.A21;
						}
					}
				}				
			}
			//求当前交点的转角


			//		if(PreJdCen->JDXZ==IsJiaoDian && PreJdCen->LinkAfter==Luan_Link && CurJdCen->A1>0.0)
			//			{//交点法计算卵型曲线
			//				double Arcl;
			//				double R1,R2;
			//				JDLuanLink JDLuanObj;
			//				JDLuanObj.SetPreJD(PreJdCen->JDN,PreJdCen->JDE,PreJdCen->R);
			//				JDLuanObj.SetCurJD(CurJdCen->JDN,CurJdCen->JDE,CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
			//				JDLuanObj.SetAftJD(AfterJdCen->JDN,AfterJdCen->JDE);
			//				JDLuanObj.SetPreFwj(CurJdCen->PreFwj);
			//				JDLuanObj.SetAftFwj(CurJdCen->AftFwj);
			//				CurJdCen->T1=fabs(PreJdCen->DistToAfterJD-PreJdCen->T2);
			//				JDLuanObj.SetPreT(CurJdCen->T1);
			//				
			//
			//				JDLuanObj.UpdateData();
			//
			//				if(JDLuanObj.ERRFlag)
			//				{
			//					Alert.Format(L"4第%d个交点出错!",i);
			//					//AfxMessageBox(Alert); 
			//					acutPrintf(L"%s\n",Alert);
			//					return;
			//				}
			//				CurJdCen->A1=JDLuanObj.PreA;
			//				CurJdCen->A2=JDLuanObj.AftA;
			//				CurJdCen->T2=JDLuanObj.AftT;
			//				CurJdCen->CenN=JDLuanObj.CurCenN;
			//				CurJdCen->CenE=JDLuanObj.CurCenE;
			//				Arcl=JDLuanObj.ArcLength;
			////ads_printf(L"A1=%lf %lf %lf %lf\n", CurJdCen->A1,CurJdCen->A2,CurJdCen->T2,Arcl);
			//				PEZ.a=CurJdCen->PreFwj+PI;
			//
			//				PEZ.x=JDLuanObj.HYN;
			//				PEZ.y=JDLuanObj.HYE;
			//
			//				R1=PreJdCen->R;
			//				R2=CurJdCen->R;
			//				A1=CurJdCen->A1;
			//				A2=CurJdCen->A2;
			//				Lo1=fabs(A1*A1/R1-A1*A1/R2);
			//				Lo2=A2*A2/R2;
			//				
			/*
			//求弧长
			double tempT2;
			JDLuanLink::GetCenter(,CurJdCen->JDN,CurJdCen->JDE,
			AfterJdCen->N,AfterJdCen->E,PreJdCen->R,CurJdCen->R,CurJdCen->A1,
			CurJdCen->A2,cenN,cenE,Arcl,tempT2);
			*/


			//前缓
			/*				l=Lo1;
			if(l>0.0009)
			{
			if(R1>R2)XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
			else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
			XYArray[XYNum][1] = A1;
			XYArray[XYNum][2] = R1;
			XYArray[XYNum][3] = R2;
			XYArray[XYNum][4] = CurJdCen->LeftOrRight;
			XYArray[XYNum][5] = PEZ.a ;
			XYArray[XYNum][6] = PEZ.lc;
			XYArray[XYNum][7] = PEZ.r ;
			XYArray[XYNum][8] = PEZ.x ;
			XYArray[XYNum][9] = PEZ.y ;
			XYJDNO[XYNum]=i;//对应交点/圆心号

			//求该线元终点
			for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
			XYNum++;
			PB0=PEZ;
			xlzbxy(&PB0,&PEZ,l,INDATA,0);
			}
			/*
			//求出圆心坐标
			CurJdCen->CenN=cenN;
			CurJdCen->CenE=cenE;
			*/
			//ARC
			/*    		l=Arcl;
			if(l>0.0009)
			{
			CurJdCen->CirXYNo=XYNum;
			XYArray[XYNum][0] = XianYuan::ARC;
			XYArray[XYNum][1] = R2;
			XYArray[XYNum][2] = l;
			XYArray[XYNum][3] = 0.0;
			XYArray[XYNum][4] = CurJdCen->LeftOrRight;
			XYArray[XYNum][5] = PEZ.a ;
			XYArray[XYNum][6] = PEZ.lc;
			XYArray[XYNum][7] = PEZ.r ;
			XYArray[XYNum][8] = PEZ.x ;
			XYArray[XYNum][9] = PEZ.y ;
			XYJDNO[XYNum]=i;//对应交点/圆心号
			//求该线元终点
			for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
			XYNum++;
			PB0=PEZ;
			xlzbxy(&PB0,&PEZ,l,INDATA,0);
			}
			//后缓
			l=Lo2;
			if(l>0.0009)
			{
			XYArray[XYNum][0] = XianYuan::AftSPIRAL;
			XYArray[XYNum][1] = A2;
			XYArray[XYNum][2] = R2;
			XYArray[XYNum][3] = 0.0;
			XYArray[XYNum][4] = CurJdCen->LeftOrRight;
			XYArray[XYNum][5] = PEZ.a ;
			XYArray[XYNum][6] = PEZ.lc;
			XYArray[XYNum][7] = PEZ.r ;
			XYArray[XYNum][8] = PEZ.x ;
			XYArray[XYNum][9] = PEZ.y ;
			XYJDNO[XYNum]=i;//对应交点/圆心号
			//求该线元终点
			for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
			XYNum++;
			PB0=PEZ;
			xlzbxy(&PB0,&PEZ,l,INDATA,0);
			}
			continue;
			}
			//回头
			//	if(CurJdCen->IsOrNotHuiTou)
			//				aa=2.0*PI-fabs(aa);
			//			else aa=fabs(aa);
			*/

			aa=CurJdCen->afa;

			aa=fabs(aa);

			//求前切、后切、曲线长
			R=CurJdCen->R;
			A1=CurJdCen->A1;
			A2=CurJdCen->A2;
			Lo1=A1*A1/R;
			Lo2=A2*A2/R;
			
			if (IfDelJZX&&PreJD==i)
			{
				Lo2 = Ls1 + 2*(Lo2-Ls1);
				CurJdCen->A2 = sqrt(Lo2*R);
				A2 = CurJdCen->A2;
				CurJdCen->Ls2 = CurJdCen->A2*CurJdCen->A2 / R;
				temp = Lo2 - Ls1;
			}
			if (IfDelJZX&&PreJD+1 == i)
			{
				Lo1 = Lo1-0.5*temp;
				CurJdCen->A1 = sqrt(Lo1*R);
				A1 = CurJdCen->A1;
				CurJdCen->Ls1 = CurJdCen->A1*CurJdCen->A1 / R;
				IfDelJZX = FALSE;
			}
			getBPMTL(dms_rad(aa),R,Lo1,Lo2,&BPMTL[0]);
			T1=BPMTL[6];//前切长
			T2=BPMTL[7];//后切长
			L=BPMTL[8];//曲线长

			double RB,RE;
			RB=1e50;
			RE=1e50;
			int qorh=0;
			double tt;//切线约束
			if(PreJdCen->LinkAfter==Luan_Link || CurJdCen->LinkAfter==Luan_Link)
			{
				if(PreJdCen->JDXZ==IsJiaoDian && PreJdCen->LinkAfter==Luan_Link && CurJdCen->A1>1e-6 )
				{
					RB = PreJdCen->R;
					//调整A1
					Lo1 = fabs((CurJdCen->A1*CurJdCen->A1)/RB - (CurJdCen->A1*CurJdCen->A1)/R);
					qorh=-1;
					tt = PreJdCen->DistToAfterJD-PreJdCen->T2;
					//ads_printf(L"1: R=%lf dis=%lf pt2=%lf tt=%lf\n",R,PreJdCen->DistToAfterJD,PreJdCen->T2,tt);
				}
				else
					RB = 1e50;

				if(CurJdCen->JDXZ==IsJiaoDian && CurJdCen->LinkAfter==Luan_Link && CurJdCen->A2>1e-6 )
				{
					RE = AfterJdCen->R;
					Lo2 = fabs((CurJdCen->A2*CurJdCen->A2)/RE - (CurJdCen->A2*CurJdCen->A2)/R);
					qorh=1;
					tt = CurJdCen->DistToAfterJD-AfterJdCen->T1;//?????
					//ads_printf(L"2: R=%lf dis=%lf at1=%lf tt=%lf\n",R,CurJdCen->DistToAfterJD,AfterJdCen->T1,tt);
				}
				else
					RE = 1e50;

				get_T_Len(dms_rad(aa),R,RB,Lo1,RE,Lo2,T1,T2,L);
				//	 ads_printf(L"111: aa=%lf R=%lf RB=%lf Lo1=%lf RE=%lf Lo2=%lf L=%lf, T1=%lf T2=%lf\n",
				//					 aa,R,RB,Lo1,RE,Lo2,L,T1,T2);

				//							if(qorh==-1)
				//							{
				//								if(fabs(T1-tt)>PRECISION)//需要调整缓长
				//								{
				//									double lo=Adjust_L_JDLuan(aa,R,RB,Lo1,RE,Lo2,tt,qorh);
				//									if(lo>0.0)//有解
				//									{
				//										Lo1 = lo;
				//										get_T_Len(dms_rad(aa),R,RB,Lo1,RE,Lo2,T1,T2,L);
				//										A1=sqrt(RB*R/fabs(RB-R)*Lo1);
				//										CurJdCen->A1=A1;
				//										CurJdCen->Ls1=Lo1;
				//									}
				//								}
				//							}
				//							if(qorh==1)
				//							{
				//								if(fabs(T2-tt)>PRECISION)//需要调整缓长
				//								{
				//									double lo=Adjust_L_JDLuan(aa,R,RB,Lo1,RE,Lo2,tt,qorh);
				//									if(lo>0.0)//有解
				//									{
				//										Lo2 = lo;
				//										get_T_Len(dms_rad(aa),R,RB,Lo1,RE,Lo2,T1,T2,L);
				//										A2=sqrt(RE*R/fabs(RE-R)*Lo2);
				//										CurJdCen->A2=A2;
				//										CurJdCen->Ls2=Lo2;
				//									}
				//								}
				//							}


			}
			//考虑最后一段缓和曲线为不完整缓和曲线的情况
			if(i==JdCenNum-2)
			{
				if(AfterJdCen->R>0.001)
				{
					RE = AfterJdCen->R;
					Lo2 = fabs((CurJdCen->A2*CurJdCen->A2)/RE - (CurJdCen->A2*CurJdCen->A2)/R);
				}
				get_T_Len(dms_rad(aa),R,RB,Lo1,RE,Lo2,T1,T2,L);
				//	 ads_printf(L"111: aa=%lf R=%lf RB=%lf Lo1=%lf RE=%lf Lo2=%lf L=%lf\n",
				//		 aa,R,RB,Lo1,RE,Lo2,L);
			}


			CurJdCen->T1=T1;
			CurJdCen->T2=T2;
			CurJdCen->L=L;
			//求直缓点的坐标
			Nzh=CurJdCen->N+T1*cos(CurJdCen->PreFwj);
			Ezh=CurJdCen->E+T1*sin(CurJdCen->PreFwj);


			//计算前一段夹直线长

			l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );

			//	 ads_printf(L"&&&& %lf %lf %lf %lf %lf l=%lf\n",Nzh,Ezh,T1,PEZ.x,PEZ.y,l);
			/*
			PEZ.x=Nzh+l*cos(CurJdCen->PreFwj);
			PEZ.y=Ezh+l*sin(CurJdCen->PreFwj);
			PEZ.a=CurJdCen->PreFwj+PI;
			StdJiao(PEZ.a);*/

			if(PreJdCen->LinkAfter==NotRelation && l>0.0009)
			{
				XYArray[XYNum][0] = XianYuan::LINE;
				XYArray[XYNum][1] = l;
				XYArray[XYNum][2] = 0.0;
				XYArray[XYNum][3] = 0.0;
				XYArray[XYNum][4] = 0.0;
				XYArray[XYNum][5] = PEZ.a ;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r ;
				XYArray[XYNum][8] = PEZ.x ;
				XYArray[XYNum][9] = PEZ.y ;
				XYJDNO[XYNum]=i;//对应交点/圆心号
				//求该线元终点
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];


				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);
			}
			else
			{
				//acutPrintf(L"第%d个交点与前面的夹直线不够!\n",i);
			}
			//前缓  
			l=Lo1;
			if(l>0.0009)
			{
				if(RB>1e6)
				{
					XYArray[XYNum][0] = XianYuan::PreSPIRAL;
					XYArray[XYNum][1] = A1;
					XYArray[XYNum][2] = R;
					XYArray[XYNum][3] = 0.0;
					XYArray[XYNum][4] = CurJdCen->LeftOrRight;
					XYArray[XYNum][5] = PEZ.a ;
					XYArray[XYNum][6] = PEZ.lc;
					XYArray[XYNum][7] = PEZ.r ;
					XYArray[XYNum][8] = PEZ.x ;
					XYArray[XYNum][9] = PEZ.y ;
					XYJDNO[XYNum]=i;//对应交点/圆心号
					//求该线元终点
					//					for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
					//					XYNum++;
					//					PB0=PEZ;
					//					xlzbxy(&PB0,&PEZ,l,INDATA,0);
				}
				else
				{
					if(RB > R) XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
					else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
					XYArray[XYNum][1] = A1;
					XYArray[XYNum][2] = RB ;
					XYArray[XYNum][3] =  R;
					XYArray[XYNum][4] = CurJdCen->LeftOrRight;
					XYArray[XYNum][5] = PEZ.a ;
					XYArray[XYNum][6] = PEZ.lc;
					XYArray[XYNum][7] = PEZ.r ;
					XYArray[XYNum][8] = PEZ.x ;
					XYArray[XYNum][9] = PEZ.y ;
					XYJDNO[XYNum]=i;//对应交点/圆心号


				}
				//求该线元终点
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);

			}
			//求出圆心坐标
			/*
			SPIRAL pSpiral(PEZ.x,PEZ.y,PEZ.a,XYArray[XYNum-1][1],VERY_MAX,R,CurJdCen->LeftOrRight);
			double JiaoGQToCen;//缓和曲线与当前圆弧公切点到圆心的方位角
			JiaoGQToCen=pSpiral.GetQXFwj(pSpiral.EndL)+CurJdCen->LeftOrRight*0.5*PI;
			CurJdCen->CenN=pSpiral.EndN+CurJdCen->R*cos(JiaoGQToCen);
			CurJdCen->CenE=pSpiral.EndE+CurJdCen->R*sin(JiaoGQToCen);
			*/
			double JiaoGQToCen;//缓和曲线与当前圆弧公切点到圆心的方位角
			JiaoGQToCen=PEZ.a+CurJdCen->LeftOrRight*0.5*PI;
			CurJdCen->CenN=PEZ.x+CurJdCen->R*cos(JiaoGQToCen);
			CurJdCen->CenE=PEZ.y+CurJdCen->R*sin(JiaoGQToCen);
			//ARC

			l=L-Lo1-Lo2;

			if(l>0.0009)
			{
				CurJdCen->CirXYNo=XYNum;
				XYArray[XYNum][0] = XianYuan::ARC;
				XYArray[XYNum][1] = R;
				XYArray[XYNum][2] = l;
				XYArray[XYNum][3] = 0.0;
				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a ;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r ;
				XYArray[XYNum][8] = PEZ.x ;
				XYArray[XYNum][9] = PEZ.y ;
				XYJDNO[XYNum]=i;//对应交点/圆心号
				//求该线元终点
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);
			}
			//后缓
			l=Lo2;
			if(l>0.0009)
			{
				if(RE>1e6)
				{
					XYArray[XYNum][0] = XianYuan::AftSPIRAL;
					XYArray[XYNum][1] = A2;
					XYArray[XYNum][2] = R;
					XYArray[XYNum][3] = 0.0;
					XYArray[XYNum][4] = CurJdCen->LeftOrRight;
					XYArray[XYNum][5] = PEZ.a ;
					XYArray[XYNum][6] = PEZ.lc;
					XYArray[XYNum][7] = PEZ.r ;
					XYArray[XYNum][8] = PEZ.x ;
					XYArray[XYNum][9] = PEZ.y ;
					//ads_printf(L"后缓%f,%f\n",XYArray[XYNum][8],XYArray[XYNum][9]);
					XYJDNO[XYNum]=i;//对应交点/圆心号
					//求该线元终点
				}
				else
				{
					if(RE>R) XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
					else XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;

					XYArray[XYNum][1] = A2;
					XYArray[XYNum][2] =  R;
					XYArray[XYNum][3] = RE;
					XYArray[XYNum][4] = CurJdCen->LeftOrRight;
					XYArray[XYNum][5] = PEZ.a ;
					XYArray[XYNum][6] = PEZ.lc;
					XYArray[XYNum][7] = PEZ.r ;
					XYArray[XYNum][8] = PEZ.x ;
					XYArray[XYNum][9] = PEZ.y ;
					//ads_printf(L"后缓%f,%f\n",XYArray[XYNum][8],XYArray[XYNum][9]);
					XYJDNO[XYNum]=i;//对应交点/圆心号
				}
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);

			}
		}
		//为圆心
		if(CurJdCen->JDXZ==IsCenter)
		{
			CurJdCen->CenN=CurJdCen->N;
			CurJdCen->CenE=CurJdCen->E;

			int StartXYNo,EndXYNo;//该交点的起终线元号
			StartXYNo=XYNum;
			bool IsAllPreSpiral=true;
			bool IsAllAftSpiral=true;
			bool IsHasJiaZX=true;
			//获取前、后JD/CENTER指针
			PreJdCen=&JdCenArray[i-1];
			AfterJdCen=&JdCenArray[i+1];
			bool HasAfterSpiral=true;
			//如果前面是交点 ,且不为卵型连接
			if(PreJdCen->JDXZ==IsJiaoDian/*&&PreJdCen->LinkAfter!=Luan_Link*/)
			{
				IsHasJiaZX=true;
				//if(fabs(xyddaa(CurJdCen->N,CurJdCen->E,PreJdCen->N,PreJdCen->E,&tj)-CurJdCen->R)<0.001)
				if(CurJdCen->A1<0.001)
				{
					//直圆公切
					/*
					A1=0.0;
					N1=PreJdCen->N;
					E1=PreJdCen->E;
					Nzh=N1;
					Ezh=E1;
					*/
					A1=0.0;
					double fwjCenToQD=PreJdCen->AftFwj-CurJdCen->LeftOrRight*0.5*PI;
					N1=CurJdCen->CenN+CurJdCen->R*cos(fwjCenToQD);
					E1=CurJdCen->CenE+CurJdCen->R*sin(fwjCenToQD);
					Nzh=N1;
					Ezh=E1;

					Lo1=0.0;
					CurJdCen->A1=0.0;
				}
				else
				{

					ZYLink ZYObj(PreJdCen->N,PreJdCen->E,PreJdCen->AftFwj,
						CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->LeftOrRight );

					if(ZYObj.ERRFlag)
					{
						Alert.Format(L"5第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					CurJdCen->A1=ZYObj.pSPIRAL.A;
					A1=CurJdCen->A1;				
					N1=ZYObj.pSPIRAL.EndN;
					E1=ZYObj.pSPIRAL.EndE;
					Nzh=ZYObj.pSPIRAL.BaseN;
					Ezh=ZYObj.pSPIRAL.BaseE;
					Lo1=A1*A1/CurJdCen->R;
				}
			}
			else//前面是圆心
			{
				IsHasJiaZX=false;
				//由后接关系分情况处理
				//一、S型
				if(PreJdCen->LinkAfter==S_Link)
				{
					A1=CurJdCen->A1;
					Lo1=A1*A1/CurJdCen->R;
				}
				//二、C型
				if(PreJdCen->LinkAfter==C_Link)
				{
					A1=CurJdCen->A1;
					Lo1=A1*A1/CurJdCen->R;					
				}
				//三、卵型
				if(PreJdCen->LinkAfter==Luan_Link)
				{
					A1=CurJdCen->A1;
					Lo1=fabs(A1*A1/CurJdCen->R-A1*A1/PreJdCen->R);
					IsAllPreSpiral=false;			
				}
				/*
				Nzh=ZYObj.pSPIRAL.BaseN;
				Ezh=ZYObj.pSPIRAL.BaseE;
				*/
				//四、有夹直线
				if(PreJdCen->LinkAfter==NotRelation)
				{
					IsHasJiaZX=true;
					YYQXLink YYQXObj;
					YYQXObj.SetCir1(PreJdCen->N,PreJdCen->E,PreJdCen->R);
					YYQXObj.SetCir2(CurJdCen->N,CurJdCen->E,CurJdCen->R);
					YYQXObj.SetA(PreJdCen->A2,CurJdCen->A1);

					if(PreJdCen->LeftOrRight<0 && CurJdCen->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
					else if(PreJdCen->LeftOrRight>0 && CurJdCen->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
					else if(PreJdCen->LeftOrRight<0 && CurJdCen->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
					else YYQXObj.SetQXClass(S_S);

					YYQXObj.CalQX();

					if(YYQXObj.ERRFlag)
					{
						Alert.Format(L"6第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}

					A1=CurJdCen->A1;
					if(CurJdCen->A1>0.0009)
					{
						N1=YYQXObj.pSPIRAL2.EndN;//PtOnCirN2;
						E1=YYQXObj.pSPIRAL2.EndE;//PtOnCirE2;
						Nzh=YYQXObj.pSPIRAL2.StartN;//GQN2;
						Ezh=YYQXObj.pSPIRAL2.StartE;//GQE2;
					}
					else
					{
						N1=YYQXObj.GQN2;
						E1=YYQXObj.GQE2;
						Nzh=YYQXObj.GQN2;
						Ezh=YYQXObj.GQE2;
					}
					Lo1=A1*A1/CurJdCen->R;
				}
			}
			//如果后一个为交点
			if(AfterJdCen->JDXZ==IsJiaoDian)
			{
				//求后一条边的方位角
				/*

				AfterJdCen->PreFwj=GetFwjByJd_Center_A_LR(AfterJdCen->N,AfterJdCen->E,
				CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->A2,
				-1*CurJdCen->LeftOrRight);		*/


				double tj;
				if(fabs(xyddaa(CurJdCen->N,CurJdCen->E,AfterJdCen->N,AfterJdCen->E,&tj)-CurJdCen->R)<0.001)
				{

					A2=0.0;
					N2=AfterJdCen->N;
					E2=AfterJdCen->E;
					Lo2=0.0;
					CurJdCen->A2=0.0;
				}
				else
				{
					ZYLink ZYObj(AfterJdCen->N,AfterJdCen->E,AfterJdCen->PreFwj,
						CurJdCen->N,CurJdCen->E,CurJdCen->R,-1*CurJdCen->LeftOrRight );

					if(ZYObj.ERRFlag)
					{
						Alert.Format(L"7第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					CurJdCen->A2=ZYObj.pSPIRAL.A;
					A2=CurJdCen->A2;
					if(A2>0.001)
					{
						N2=ZYObj.pSPIRAL.EndN;
						E2=ZYObj.pSPIRAL.EndE;
						Lo2=A2*A2/CurJdCen->R;
					}
					else
					{
						N2=CurJdCen->N+CurJdCen->R*cos(AfterJdCen->PreFwj+CurJdCen->LeftOrRight*0.5*pi);
						E2=CurJdCen->E+CurJdCen->R*sin(AfterJdCen->PreFwj+CurJdCen->LeftOrRight*0.5*pi);
						Lo2=0.0;
					}

				}
			}
			else//如果后一个为圆心
			{
				//			double NHY,EHY;
				//由后接关系分情况处理
				//一、S型
				if(CurJdCen->LinkAfter==S_Link)
				{
					YYSLink YYSObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
						AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,
						CurJdCen->A2,AfterJdCen->A1,CurJdCen->LeftOrRight );

					if(YYSObj.ERRFlag)
					{
						Alert.Format(L"第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					//得到真实缓和曲线参数
					CurJdCen->A2=YYSObj.A1;
					AfterJdCen->A1=YYSObj.A2;

					A2=YYSObj.A1;
					N2=YYSObj.PtOnCir1N;
					E2=YYSObj.PtOnCir1E;
					Lo2=A2*A2/CurJdCen->R;
				}
				//二、C型
				if(CurJdCen->LinkAfter==C_Link)
				{
					YYCLink YYCObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
						AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,
						CurJdCen->A2,AfterJdCen->A1,CurJdCen->LeftOrRight );

					if(YYCObj.ERRFlag)
					{
						Alert.Format(L"9第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					//得到真实缓和曲线参数
					CurJdCen->A2=YYCObj.A1;
					AfterJdCen->A1=YYCObj.A2;

					A2=YYCObj.A1;
					N2=YYCObj.pSPIRAL1.EndN;
					E2=YYCObj.pSPIRAL1.EndE;
					Lo2=A2*A2/CurJdCen->R;
				}
				//三、卵型
				if(CurJdCen->LinkAfter==Luan_Link)
				{
					HasAfterSpiral=false;
					IsAllAftSpiral=false;
					YYLuanLink YYLuanObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
						AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,															
						CurJdCen->LeftOrRight );

					if(YYLuanObj.ERRFlag)
					{
						Alert.Format(L"10第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					//得到缓和曲线参数					
					CurJdCen->A2=0.0;
					AfterJdCen->A1=YYLuanObj.A;

					A2=YYLuanObj.A;
					if(CurJdCen->R>AfterJdCen->R)
					{
						N2=YYLuanObj.pSPIRAL.StartN;
						E2=YYLuanObj.pSPIRAL.StartE;
					}
					else
					{
						N2=YYLuanObj.pSPIRAL.EndN;
						E2=YYLuanObj.pSPIRAL.EndE;
					}
					Lo2=A2*A2/CurJdCen->R;
				}
				//四、有夹直线
				if(CurJdCen->LinkAfter==NotRelation)
				{
					YYQXLink YYQXObj;
					YYQXObj.SetCir1(CurJdCen->N,CurJdCen->E,CurJdCen->R);
					YYQXObj.SetCir2(AfterJdCen->N,AfterJdCen->E,AfterJdCen->R);
					YYQXObj.SetA(CurJdCen->A2,AfterJdCen->A1);

					if(CurJdCen->LeftOrRight<0 && AfterJdCen->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
					else if(CurJdCen->LeftOrRight>0 && AfterJdCen->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
					else if(CurJdCen->LeftOrRight<0 && AfterJdCen->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
					else YYQXObj.SetQXClass(S_S);

					YYQXObj.CalQX();

					if(YYQXObj.ERRFlag)
					{
						Alert.Format(L"11第%d个交点出错!",i);
						//						AfxMessageBox(Alert);
						acutPrintf(L"%s\n",Alert);
						return;
					}
					//得到真实缓和曲线参数

					A2=CurJdCen->A2;
					if(CurJdCen->A2>0.0009)
					{
						N2=YYQXObj.pSPIRAL1.EndN;//PtOnCirN1;
						E2=YYQXObj.pSPIRAL1.EndE;//PtOnCirE1;
					}
					else
					{
						N2=YYQXObj.GQN1;
						E2=YYQXObj.GQE1;
					}
					Lo2=A2*A2/CurJdCen->R;
				}
			}

			EndXYNo=XYNum;//初值
			PEZ.a=CurJdCen->PreFwj+PI;

			//夹直线
			//计算前一段夹直线长
			if(IsHasJiaZX)
			{
				l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );

				if(l>0.0009)
				{
					EndXYNo=XYNum;

					XYArray[XYNum][0] = XianYuan::LINE;
					XYArray[XYNum][1] = l;
					XYArray[XYNum][2] = 0.0;
					XYArray[XYNum][3] = 0.0;
					XYArray[XYNum][4] = 0.0;
					XYArray[XYNum][5] = PEZ.a ;
					XYArray[XYNum][6] = PEZ.lc;
					XYArray[XYNum][7] = PEZ.r ;
					XYArray[XYNum][8] = PEZ.x ;
					XYArray[XYNum][9] = PEZ.y ;

					XYJDNO[XYNum]=i;//对应交点/圆心号
					//求该线元终点
					for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
					XYNum++;
					PB0=PEZ;
					xlzbxy(&PB0,&PEZ,l,INDATA,0);
				}
			}
			//前缓
			l=Lo1;
			if(l>0.0009)
			{
				EndXYNo=XYNum;

				if(IsAllPreSpiral)//完整前缓
				{
					XYArray[XYNum][0] = XianYuan::PreSPIRAL;
					XYArray[XYNum][1] = A1;
					XYArray[XYNum][2] = CurJdCen->R;
					XYArray[XYNum][3] = 0.0;
				}
				else
				{
					if(PreJdCen->R>CurJdCen->R)
						XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
					else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
					XYArray[XYNum][1] = A1;
					XYArray[XYNum][2] = PreJdCen->R;
					XYArray[XYNum][3] = CurJdCen->R;
				}

				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a ;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r ;
				XYArray[XYNum][8] = PEZ.x ;
				XYArray[XYNum][9] = PEZ.y ;
				XYJDNO[XYNum]=i;//对应交点/圆心号

				//求该线元终点
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);

			}
			//用于计算圆曲线长			
			N1=PEZ.x;	E1=PEZ.y;
			//ARC
			l=GetArcLengthBy2PointOnCircleAndLR(CurJdCen->N,CurJdCen->E,CurJdCen->R,
				N1,E1,N2,E2,CurJdCen->LeftOrRight);

			if(l>0.0009)
			{
				//判断是否回头
				//	if(l>CurJdCen->R*PI)CurJdCen->IsOrNotHuiTou=true;
				//	else CurJdCen->IsOrNotHuiTou=false;
				EndXYNo=XYNum;
				CurJdCen->CirXYNo=XYNum;
				XYArray[XYNum][0] = XianYuan::ARC;
				XYArray[XYNum][1] = CurJdCen->R;
				XYArray[XYNum][2] = l;
				XYArray[XYNum][3] = 0.0;
				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a ;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r ;
				XYArray[XYNum][8] = PEZ.x ;
				XYArray[XYNum][9] = PEZ.y ;
				XYJDNO[XYNum]=i;//对应交点/圆心号
				//求该线元终点
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);
			}
			//后缓
			l=Lo2;
			if(l>0.0&&HasAfterSpiral)
			{
				EndXYNo=XYNum;

				if(IsAllAftSpiral)//完整后缓
				{
					XYArray[XYNum][0] = XianYuan::AftSPIRAL;
					XYArray[XYNum][1] = A2;
					XYArray[XYNum][2] = CurJdCen->R;
					XYArray[XYNum][3] = 0.0;
				}
				else
				{
					if(CurJdCen->R>AfterJdCen->R)
						XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
					else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
					XYArray[XYNum][1] = A2;
					XYArray[XYNum][2] = CurJdCen->R;
					XYArray[XYNum][3] = AfterJdCen->R;
				}

				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a ;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r ;
				XYArray[XYNum][8] = PEZ.x ;
				XYArray[XYNum][9] = PEZ.y ;
				XYJDNO[XYNum]=i;//对应交点/圆心号
				//求该线元终点
				for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
				XYNum++;
				PB0=PEZ;
				xlzbxy(&PB0,&PEZ,l,INDATA,0);

			}
			//计算交点
			/*
			CurJdCen->PreFwj=XYArray[StartXYNo][5]+PI;
			CurJdCen->AftFwj=PEZ.a;*/


			double n1,e1,n2,e2,n3,e3,n4,e4;//两切线

			int ixy;
			ixy=StartXYNo;
			if(XYArray[ixy][0]<1.5) ixy++;
			n1=XYArray[ixy][8];
			e1=XYArray[ixy][9];

			n2=n1+1000.0*cos(CurJdCen->PreFwj);
			e2=e1+1000.0*sin(CurJdCen->PreFwj);

			n3=PEZ.x;
			e3=PEZ.y;

			n4=n3+1000.0*cos(CurJdCen->AftFwj);
			e4=e3+1000.0*sin(CurJdCen->AftFwj);

			//			INTERS(&CurJdCen->JDN,&CurJdCen->JDE,n1,e1,n2,e2,n3,e3,n4,e4);

			double tmpjiao;
			CurJdCen->T1=xyddaa(n1,e1,CurJdCen->JDN,CurJdCen->JDE,&tmpjiao);
			CurJdCen->T2=xyddaa(n3,e3,CurJdCen->JDN,CurJdCen->JDE,&tmpjiao);
			CurJdCen->L=PEZ.lc-XYArray[StartXYNo][6];
		}
		/*
		CurJdCen->JdNo=JD0+iJD;
		iJD++;
		*/
	}

	CurJdCen=&JdCenArray[JdCenNum-1];
	//夹直线
	Nzh=JdCenArray[JdCenNum-1].N;//终点
	Ezh=JdCenArray[JdCenNum-1].E;
	//计算前一段夹直线长
	l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
	if(l>0.0009)
	{
		//	xyddaa(PEZ.x,PEZ.y,Nzh,Ezh,&PEZ.a);
		XYArray[XYNum][0] = XianYuan::LINE;
		XYArray[XYNum][1] = l;
		XYArray[XYNum][2] = 0.0;
		XYArray[XYNum][3] = 0.0;
		XYArray[XYNum][4] = 0.0;
		XYArray[XYNum][5] = PEZ.a ;
		XYArray[XYNum][6] = PEZ.lc;
		XYArray[XYNum][7] = PEZ.r ;
		XYArray[XYNum][8] = PEZ.x ;
		XYArray[XYNum][9] = PEZ.y ;
		XYJDNO[XYNum]=i;//对应最后一个交点
		//求该线元终点
		for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
		XYNum++;
		PB0=PEZ;
		xlzbxy(&PB0,&PEZ,l,INDATA,0);
	}

	//最后一个线元为终点
	XYArray[XYNum][0] = 0.0;
	XYArray[XYNum][1] = 0.0;
	XYArray[XYNum][2] = 0.0;
	XYArray[XYNum][3] = 0.0;
	XYArray[XYNum][4] = 0.0;
	XYArray[XYNum][5] = PEZ.a ;
	XYArray[XYNum][6] = PEZ.lc;
	XYArray[XYNum][7] = PEZ.r ;
	XYArray[XYNum][8] = PEZ.x ;
	XYArray[XYNum][9] = PEZ.y ;
	XYJDNO[XYNum]=i;//对应最后一个交点
	//求该线元终点
	for (k=0;k<5;k++) 
		INDATA[k]=XYArray[XYNum][k];
	//	XYNum++;
	PB0=PEZ;
	xlzbxy(&PB0,&PEZ,l,INDATA,0);
	GetDLTylc();

	//交点
	CurJdCen=&JdCenArray[JdCenNum-1];
	//CurJdCen->JDN=PEZ.x;
	//CurJdCen->JDE=PEZ.y;
	CurJdCen->A1=0.0;
	CurJdCen->A2=0.0;
	CurJdCen->T1=0.0;
	CurJdCen->T2=0.0;
	CurJdCen->JdNo = JdCenNum - 1 + JD0 - 1;//20190707添加

	//计算线路终点的统一里程
	int j=(int)(XYArray[XYNum-1][0]+0.001);

	if(j==XianYuan::LINE)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1];
	else if(j==XianYuan::ARC)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][2];
	else if(j==XianYuan::AftSPIRAL)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/XYArray[XYNum-1][2];
	else if(j==XianYuan::PreSPIRAL)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/XYArray[XYNum-1][2];
	else 
	{
		//终点投影
		XLLength=XYArray[XYNum-1][6]+fabs(XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/(XYArray[XYNum-1][2])-XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/(XYArray[XYNum-1][3]));
	}
	if(XYNum>0) XYArray[0][4]=DLArray[0].ELC;
	LenOfLine = XLLength - XYArray[0][4];//线路长度 
	//	OutData();
	CString TGH;
	DLNum=DLArray.GetSize();

	double endXlc=fabs(XLC(XLLength,TGH));

	setDLM(DLNum-1,endXlc,endXlc,DLArray[DLNum-1].BGH,DLArray[DLNum-1].EGH);
	/*
	acutPrintf(L"%s ",DLArray[DLNum-1].BGH);
	acutPrintf(L"%lf ",DLArray[DLNum-1].BLC);
	acutPrintf(L"%s ",DLArray[DLNum-1].EGH);
	acutPrintf(L"%lf ",DLArray[DLNum-1].ELC);
	acutPrintf(L"\n");
	*/   	

	if(XYNum>0)
		XYArray[0][4]=DLArray[0].ELC;
	/*
	if(JdCenNum>2)
	{
	if(JdCenArray[1].T1>JdCenArray[0].DistToAfterJD)
	{
	JdCenArray[0].JDN=XYArray[1][8];//JdCenArray[1].JDN+JdCenArray[1].T1*cos(JdCenArray[1].PreFwj);
	JdCenArray[0].JDE=XYArray[1][9];//JdCenArray[1].JDE+JdCenArray[1].T1*sin(JdCenArray[1].PreFwj);
	JdCenArray[0].N=JdCenArray[0].JDN;
	JdCenArray[0].E=JdCenArray[0].JDE;
	}

	if(JdCenArray[JdCenNum-2].T2>JdCenArray[JdCenNum-2].DistToAfterJD)
	{
	JdCenArray[JdCenNum-1].JDN=JdCenArray[JdCenNum-2].JDN+JdCenArray[JdCenNum-2].T2*cos(JdCenArray[JdCenNum-2].AftFwj);
	JdCenArray[JdCenNum-1].JDE=JdCenArray[JdCenNum-2].JDE+JdCenArray[JdCenNum-2].T2*sin(JdCenArray[JdCenNum-2].AftFwj);
	JdCenArray[JdCenNum-1].N=JdCenArray[JdCenNum-1].JDN;
	JdCenArray[JdCenNum-1].E=JdCenArray[JdCenNum-1].JDE;
	}
	}*/

	//	CalJiaoDian();

	//OutData();

}

void JD_CENTER::CalXYArray_OnlyJD()
{
	//检测交点数量
	if (JdCenNum < 2)
	{
		AfxMessageBox(L"控制点个数小于两个!");
		JdCenNum = 0;
		return;
	}
	//赋值起点的初始里程？
	if (DLNum > 0)
	{
		if (fabs(JdCenArray[0].A1 - DLArray[0].ELC) > 0.001)
			JdCenArray[0].A1 = DLArray[0].ELC;
	}
	//处理只有两个交点的情况
	if (JdCenNum == 2)
	{
		double dN, dE;
		dN = JdCenArray[0].N - JdCenArray[1].N;
		dE = JdCenArray[0].E - JdCenArray[1].E;
		if (sqrt(dN * dN + dE * dE) < 0.1)
		{
			AfxMessageBox(L"控制点数据有误!");
			JdCenNum = 0;
			return;
		}
	}

	vector<JdCenter> watchvec;
	for (int i = 0; i < JdCenArray.GetSize(); i++)
		watchvec.push_back(JdCenArray[i]);

	CalJiaoDian();
	//CalJiaoDian_New();
	for (int i = 0; i < JdCenArray.GetSize(); i++)
		watchvec.push_back(JdCenArray[i]);

	int i;
	//计算切线长
	Cal_AllJD_T();
	int k;
	double aa, BPMTL[10], INDATA[5], l, T1, T2, Lo1, Lo2, L, R,
		Nzh, Ezh;
	struct xlpoint PB0, PEZ;
	double N1, E1, N2, E2;//圆上两点
	double A1, A2;//前后缓
	SPIRAL PreSpiral, AftSpiral;
	JdCenter* PreJdCen, * CurJdCen, * AfterJdCen;
	CString Alert;
	JdCenNum = JdCenArray.GetSize();

	if (XYNum > 0)
	{
		if (XYArray)
			delete[]XYArray;
		XYArray = NULL;
	}

	XYNum = 0;
	if (XYJDNO)
	{
		delete[]XYJDNO;
		XYJDNO = NULL;
	}

	//为线元数组开空间，稍开大些
	XYArray = new double[JdCenNum * 4][10];

	for (int k = 0; k < JdCenNum * 4; k++)
	{
		for (int j = 0; j < 10; j++)
		{
			XYArray[k][j] = 0.0;
		}
	}
	XYJDNO = new int[JdCenNum * 4];
	XYNum = 0;//线路起点
	XYArray[XYNum][0] = 0.0;
	XYArray[XYNum][1] = JdCenArray[0].N;
	XYArray[XYNum][2] = JdCenArray[0].E;
	CurJdCen = &JdCenArray[0];
	AfterJdCen = &JdCenArray[1];
	CurJdCen->JdNo = JD0 - 1;
	//	CurJdCen->A1=0.0;
	CurJdCen->A2 = 0.0;
	CurJdCen->T1 = 0.0;
	CurJdCen->T2 = 0.0;
	XYArray[XYNum][3] = CurJdCen->AftFwj;//起点方位角
	XYArray[XYNum][4] = CurJdCen->A1;//线路起点里程
	//	XYArray[XYNum][5]=0.0;
	XYArray[XYNum][5] = JD0;//起始交点号
	XYArray[XYNum][6] = 0.0;
	XYArray[XYNum][7] = 0.0;
	XYArray[XYNum][8] = 0.0;
	XYArray[XYNum][9] = 0.0;
	XYJDNO[XYNum] = 0;//对应起点
	XYNum++;
	PEZ.lc = XYArray[XYNum - 1][4];
	PEZ.x = XYArray[XYNum - 1][1];
	PEZ.y = XYArray[XYNum - 1][2];
	PEZ.a = XYArray[XYNum - 1][3];
	PEZ.r = 1e40;
	//首先计算所有交点的前后两条边的方位角PreFwj,AftFwj/以当前交点为起点

	/*for( i=1;i<JdCenNum-1;i++)*/

	for (i = 1; i < JdCenNum - 1; i++)
	{
		//获取当前JD/CENTER指针
		CurJdCen = &JdCenArray[i];
		CurJdCen->JdNo = i + JD0 - 1;//-1.0;//初值

		bool HasJiaZX = true;
		//获取前、后JD/CENTER指针
		PreJdCen = &JdCenArray[i - 1];
		AfterJdCen = &JdCenArray[i + 1];

		aa = CurJdCen->afa;
		aa = fabs(aa);

		//求前切、后切、曲线长
		R = CurJdCen->R;
		A1 = CurJdCen->A1;
		A2 = CurJdCen->A2;
		if (R < 0.01)
		{
			Lo1 = 0.0;
			Lo2 = 0.0;
		}
		else
		{
			Lo1 = A1 * A1 / R;
			Lo2 = A2 * A2 / R;
		}

		getBPMTL(dms_rad(aa), R, Lo1, Lo2, &BPMTL[0]);
		T1 = BPMTL[6];//前切长
		T2 = BPMTL[7];//后切长
		L = BPMTL[8];//曲线长

		double RB, RE;
		RB = 1e50;
		RE = 1e50;
		int qorh = 0;
		double tt;//切线约束

		//考虑最后一段缓和曲线为不完整缓和曲线的情况
		if (i == JdCenNum - 2)
		{
			if (AfterJdCen->R > 0.001)
			{
				RE = AfterJdCen->R;
				Lo2 = fabs((CurJdCen->A2 * CurJdCen->A2) / RE - (CurJdCen->A2 * CurJdCen->A2) / R);
				get_T_Len(dms_rad(aa), R, RB, Lo1, RE, Lo2, T1, T2, L);	//20190904从外面移到了里面
			}
		}


		CurJdCen->T1 = T1;
		CurJdCen->T2 = T2;
		CurJdCen->L = L;
		//求直缓点的坐标
		Nzh = CurJdCen->N + T1 * cos(CurJdCen->PreFwj);
		Ezh = CurJdCen->E + T1 * sin(CurJdCen->PreFwj);


		//计算前一段夹直线长

		l = sqrt((Nzh - PEZ.x) * (Nzh - PEZ.x) + (Ezh - PEZ.y) * (Ezh - PEZ.y));


		if (PreJdCen->LinkAfter == NotRelation && l > 0.0009)
		{
			//20190904新增	如果前一段是直线，那么不能用前一段的终点作为方位角
			double Alpha = 0.0;
			xyddaa(PEZ.x, PEZ.y, Nzh, Ezh, &PEZ.a);


			XYArray[XYNum][0] = XianYuan::LINE;
			XYArray[XYNum][1] = l;
			XYArray[XYNum][2] = 0.0;
			XYArray[XYNum][3] = 0.0;
			XYArray[XYNum][4] = 0.0;
			XYArray[XYNum][5] = PEZ.a;
			//XYArray[XYNum][5] = Alpha;
			XYArray[XYNum][6] = PEZ.lc;
			XYArray[XYNum][7] = PEZ.r;
			XYArray[XYNum][8] = PEZ.x;
			XYArray[XYNum][9] = PEZ.y;
			XYJDNO[XYNum] = i;//对应交点/圆心号
			//求该线元终点
			for (k = 0; k < 5; k++) INDATA[k] = XYArray[XYNum][k];


			XYNum++;
			PB0 = PEZ;
			xlzbxy(&PB0, &PEZ, l, INDATA, 0);
		}
		else
		{
			//acutPrintf(L"第%d个交点与前面的夹直线不够!\n",i);
		}
		//前缓  
		l = Lo1;
		if (l > 0.0009)
		{
			if (RB > 1e6)
			{
				XYArray[XYNum][0] = XianYuan::PreSPIRAL;
				XYArray[XYNum][1] = A1;
				XYArray[XYNum][2] = R;
				XYArray[XYNum][3] = 0.0;
				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r;
				XYArray[XYNum][8] = PEZ.x;
				XYArray[XYNum][9] = PEZ.y;
				XYJDNO[XYNum] = i;//对应交点/圆心号
			}
			else
			{
				if (RB > R) XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
				else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
				XYArray[XYNum][1] = A1;
				XYArray[XYNum][2] = RB;
				XYArray[XYNum][3] = R;
				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r;
				XYArray[XYNum][8] = PEZ.x;
				XYArray[XYNum][9] = PEZ.y;
				XYJDNO[XYNum] = i;//对应交点/圆心号


			}
			//求该线元终点
			for (k = 0; k < 5; k++) 
				INDATA[k] = XYArray[XYNum][k];
			XYNum++;
			PB0 = PEZ;
			xlzbxy(&PB0, &PEZ, l, INDATA, 0);

		}
		double JiaoGQToCen;//缓和曲线与当前圆弧公切点到圆心的方位角
		JiaoGQToCen = PEZ.a + CurJdCen->LeftOrRight * 0.5 * PI;
		CurJdCen->CenN = PEZ.x + CurJdCen->R * cos(JiaoGQToCen);
		CurJdCen->CenE = PEZ.y + CurJdCen->R * sin(JiaoGQToCen);
		//ARC

		l = L - Lo1 - Lo2;

		if (l > 0.0009)
		{
			CurJdCen->CirXYNo = XYNum;
			XYArray[XYNum][0] = XianYuan::ARC;
			XYArray[XYNum][1] = R;
			XYArray[XYNum][2] = l;
			XYArray[XYNum][3] = 0.0;
			XYArray[XYNum][4] = CurJdCen->LeftOrRight;
			XYArray[XYNum][5] = PEZ.a;
			XYArray[XYNum][6] = PEZ.lc;
			XYArray[XYNum][7] = PEZ.r;
			XYArray[XYNum][8] = PEZ.x;
			XYArray[XYNum][9] = PEZ.y;
			XYJDNO[XYNum] = i;//对应交点/圆心号
			//求该线元终点
			for (k = 0; k < 5; k++) INDATA[k] = XYArray[XYNum][k];
			XYNum++;
			PB0 = PEZ;
			xlzbxy(&PB0, &PEZ, l, INDATA, 0);
		}
		//后缓
		l = Lo2;
		if (l > 0.0009)
		{
			if (RE > 1e6)
			{
				XYArray[XYNum][0] = XianYuan::AftSPIRAL;
				XYArray[XYNum][1] = A2;
				XYArray[XYNum][2] = R;
				XYArray[XYNum][3] = 0.0;
				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r;
				XYArray[XYNum][8] = PEZ.x;
				XYArray[XYNum][9] = PEZ.y;
				//ads_printf(L"后缓%f,%f\n",XYArray[XYNum][8],XYArray[XYNum][9]);
				XYJDNO[XYNum] = i;//对应交点/圆心号
				//求该线元终点
			}
			else
			{
				if (RE > R) XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
				else XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;

				XYArray[XYNum][1] = A2;
				XYArray[XYNum][2] = R;
				XYArray[XYNum][3] = RE;
				XYArray[XYNum][4] = CurJdCen->LeftOrRight;
				XYArray[XYNum][5] = PEZ.a;
				XYArray[XYNum][6] = PEZ.lc;
				XYArray[XYNum][7] = PEZ.r;
				XYArray[XYNum][8] = PEZ.x;
				XYArray[XYNum][9] = PEZ.y;
				//ads_printf(L"后缓%f,%f\n",XYArray[XYNum][8],XYArray[XYNum][9]);
				XYJDNO[XYNum] = i;//对应交点/圆心号
			}
			for (k = 0; k < 5; k++) 
				INDATA[k] = XYArray[XYNum][k];
			XYNum++;
			PB0 = PEZ;
			xlzbxy(&PB0, &PEZ, l, INDATA, 0);

		}


	}

	CurJdCen = &JdCenArray[JdCenNum - 1];
	//夹直线
	Nzh = JdCenArray[JdCenNum - 1].N;//终点
	Ezh = JdCenArray[JdCenNum - 1].E;
	//计算前一段夹直线长
	l = sqrt((Nzh - PEZ.x) * (Nzh - PEZ.x) + (Ezh - PEZ.y) * (Ezh - PEZ.y));
	if (l > 0.0009)
	{
		xyddaa(PEZ.x,PEZ.y,Nzh,Ezh,&PEZ.a);

		XYArray[XYNum][0] = XianYuan::LINE;
		XYArray[XYNum][1] = l;
		XYArray[XYNum][2] = 0.0;
		XYArray[XYNum][3] = 0.0;
		XYArray[XYNum][4] = 0.0;
		XYArray[XYNum][5] = PEZ.a;
		XYArray[XYNum][6] = PEZ.lc;
		XYArray[XYNum][7] = PEZ.r;
		XYArray[XYNum][8] = PEZ.x;
		XYArray[XYNum][9] = PEZ.y;
		XYJDNO[XYNum] = i;//对应最后一个交点
		//求该线元终点
		for (k = 0; k < 5; k++) 
			INDATA[k] = XYArray[XYNum][k];
		XYNum++;
		PB0 = PEZ;
		xlzbxy(&PB0, &PEZ, l, INDATA, 0);
	}

	//最后一个线元为终点
	XYArray[XYNum][0] = 0.0;
	XYArray[XYNum][1] = 0.0;
	XYArray[XYNum][2] = 0.0;
	XYArray[XYNum][3] = 0.0;
	XYArray[XYNum][4] = 0.0;
	XYArray[XYNum][5] = PEZ.a;
	XYArray[XYNum][6] = PEZ.lc;
	XYArray[XYNum][7] = PEZ.r;
	XYArray[XYNum][8] = PEZ.x;
	XYArray[XYNum][9] = PEZ.y;
	XYJDNO[XYNum] = i;//对应最后一个交点
	//求该线元终点
	for (k = 0; k < 5; k++)
		INDATA[k] = XYArray[XYNum][k];
	//	XYNum++;
	PB0 = PEZ;
	xlzbxy(&PB0, &PEZ, l, INDATA, 0);
	GetDLTylc();

	//交点
	CurJdCen = &JdCenArray[JdCenNum - 1];
	//CurJdCen->JDN=PEZ.x;
	//CurJdCen->JDE=PEZ.y;
	CurJdCen->A1 = 0.0;
	CurJdCen->A2 = 0.0;
	CurJdCen->T1 = 0.0;
	CurJdCen->T2 = 0.0;
	CurJdCen->JdNo = JdCenNum - 1 + JD0 - 1;//20190707添加

	//计算线路终点的统一里程
	int j = (int)(XYArray[XYNum - 1][0] + 0.001);

	if (j == XianYuan::LINE)
		XLLength = XYArray[XYNum - 1][6] + XYArray[XYNum - 1][1];
	else if (j == XianYuan::ARC)
		XLLength = XYArray[XYNum - 1][6] + XYArray[XYNum - 1][2];
	else if (j == XianYuan::AftSPIRAL)
		XLLength = XYArray[XYNum - 1][6] + XYArray[XYNum - 1][1] * XYArray[XYNum - 1][1] / XYArray[XYNum - 1][2];
	else if (j == XianYuan::PreSPIRAL)
		XLLength = XYArray[XYNum - 1][6] + XYArray[XYNum - 1][1] * XYArray[XYNum - 1][1] / XYArray[XYNum - 1][2];
	else
	{
		//终点投影
		XLLength = XYArray[XYNum - 1][6] + fabs(XYArray[XYNum - 1][1] * XYArray[XYNum - 1][1] / (XYArray[XYNum - 1][2]) - XYArray[XYNum - 1][1] * XYArray[XYNum - 1][1] / (XYArray[XYNum - 1][3]));
	}
	if (XYNum > 0) XYArray[0][4] = DLArray[0].ELC;
	LenOfLine = XLLength - XYArray[0][4];//线路长度 
	//	OutData();
	CString TGH;
	DLNum = DLArray.GetSize();

	double endXlc = fabs(XLC(XLLength, TGH));

	setDLM(DLNum - 1, endXlc, endXlc, DLArray[DLNum - 1].BGH, DLArray[DLNum - 1].EGH);

	if (XYNum > 0)
		XYArray[0][4] = DLArray[0].ELC;

}
/*

void JD_CENTER::CalXYArray()
{
if(JdCenNum<2)
{
AfxMessageBox(L"控制点个数小于两个!");
JdCenNum=0;
return;
}
if(JdCenNum==2)
{
double dN,dE;
dN=JdCenArray[0].N-JdCenArray[1].N;
dE=JdCenArray[0].E-JdCenArray[1].E;
if(sqrt(dN*dN+dE*dE)<0.1)
{
AfxMessageBox(L"控制点数据有误!");
JdCenNum=0;
return;
}
}
CalJiaoDian();
int i,k;
double aa,BPMTL[10],INDATA[5],l,T1,T2,Lo1,Lo2,L,R,
Nzh,Ezh;
struct xlpoint PB0,PEZ;
double N1,E1,N2,E2;//圆上两点
double A1,A2;//前后缓
SPIRAL PreSpiral,AftSpiral;
JdCenter *PreJdCen,*CurJdCen,*AfterJdCen;
CString Alert;
JdCenNum=JdCenArray.GetSize();
XYNum=0;
if(XYArray)
{
delete []XYArray;
XYArray=NULL;
}

if(XYJDNO)
{
delete []XYJDNO;
XYJDNO=NULL;
}
ads_printf(L"AAAAA: %lf %lf\n",
JdCenArray[JdCenNum-1].JDE,JdCenArray[JdCenNum-1].JDN);

//为线元数组开空间，稍开大些
XYArray=new double[JdCenNum*4][10];
XYJDNO=new int[JdCenNum*4];
XYNum=0;//线路起点
XYArray[XYNum][0]=0.0;
XYArray[XYNum][1]=JdCenArray[0].N;
XYArray[XYNum][2]=JdCenArray[0].E;
CurJdCen=&JdCenArray[0];
AfterJdCen=&JdCenArray[1];
CurJdCen->JdNo=JD0-1;
//	CurJdCen->A1=0.0;
CurJdCen->A2=0.0;
CurJdCen->T1=0.0;
CurJdCen->T2=0.0;
XYArray[XYNum][3]=CurJdCen->AftFwj;//起点方位角
XYArray[XYNum][4]=CurJdCen->A1;//线路起点里程
XYArray[XYNum][5]=0.0;
XYArray[XYNum][6]=0.0;
XYArray[XYNum][7]=0.0;
XYArray[XYNum][8]=0.0;
XYArray[XYNum][9]=0.0;
XYJDNO[XYNum]=0;//对应起点
XYNum++;

PEZ.lc=XYArray[XYNum-1][4];
PEZ.x =XYArray[XYNum-1][1];
PEZ.y =XYArray[XYNum-1][2];
PEZ.a =XYArray[XYNum-1][3];
PEZ.r =1e40;
//首先计算所有交点的前后两条边的方位角PreFwj,AftFwj/以当前交点为起点
for(i=1;i<JdCenNum-1;i++)
{
//获取当前JD/CENTER指针
CurJdCen=&JdCenArray[i];

CurJdCen->JdNo=i+JD0-1;//-1.0;//初值

//为交点
if(CurJdCen->JDXZ==IsJiaoDian)
{
bool HasJiaZX=true;
//获取前、后JD/CENTER指针
PreJdCen=&JdCenArray[i-1];
AfterJdCen=&JdCenArray[i+1];

if( (PreJdCen->LinkAfter==S_Link || PreJdCen->LinkAfter==C_Link)
&& CurJdCen->A1*CurJdCen->A2<0.1)
{
//当前交点的前切
CurJdCen->T1=PreJdCen->DistToAfterJD-PreJdCen->T2;
if(CurJdCen->T1<0.0001){Alert.Format(L"第%d个交点出错!",i);acutPrintf(L"%s\n",Alert);return;}

//保证前切
if(fabs(CurJdCen->A1)<0.0001 && fabs(CurJdCen->A2)<0.0001)
{
//调整半径
double ta=0.5*(PI-fabs(CurJdCen->afa));
if(ta<0.0001){Alert.Format(L"第%d个交点出错!",i);acutPrintf(L"%s\n",Alert);return;}
CurJdCen->R=CurJdCen->T1*tan(ta);
}
else if(fabs(CurJdCen->A1)<0.0001)
{
//调整A2
CurJdCen->A1=0.0;
ThreeJDCalThisAFromThatA_PreT_R CalObj;
CalObj.SetA1(0.0);
CalObj.SetAfa(fabs(CurJdCen->afa));
CalObj.SetPreT_R(CurJdCen->T1,CurJdCen->R);
double a2;
if(CalObj.GetA2(a2))CurJdCen->A2=a2;
}
else if(fabs(CurJdCen->A2)<0.0001)
{
//调整A1
CurJdCen->A2=0.0;
ThreeJDCalThisAFromThatA_PreT_R CalObj;
CalObj.SetA2(0.0);
CalObj.SetAfa(fabs(CurJdCen->afa));
CalObj.SetPreT_R(CurJdCen->T1,CurJdCen->R);
double a1;
if(CalObj.GetA1(a1))CurJdCen->A1=a1;
}
}			

if(CurJdCen->LinkAfter==S_Link || CurJdCen->LinkAfter==C_Link)
{
if(i>=2 && (PreJdCen->LinkAfter==S_Link
|| PreJdCen->LinkAfter==C_Link 
/ *|| PreJdCen->LinkAfter==Luan_Link* /))
{
if( CurJdCen->A1*CurJdCen->A2>0.1 )
{								
double tempT21,tempT12;
//	tempT12=PreJdCen->T2;
if(PreJdCen->LinkAfter==Luan_Link)
{
/ *
double cenN,cenE,Arcl;
JDLuanLink::GetCenter(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,
AfterJdCen->N,AfterJdCen->E,PreJdCen->R,CurJdCen->R,CurJdCen->A1,
CurJdCen->A2,cenN,cenE,Arcl,tempT12);
* /
/ *
JDLuanLink JDLuanObj;
JDLuanObj.SetPreJD(PreJdCen->JDN,PreJdCen->JDE,PreJdCen->R);
JDLuanObj.SetCurJD(CurJdCen->JDN,CurJdCen->JDE,CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
JDLuanObj.SetAftJD(AfterJdCen->JDN,AfterJdCen->JDE);
CurJdCen->T1=PreJdCen->DistToAfterJD-PreJdCen->T2;
JDLuanObj.SetPreT(CurJdCen->T1);
JDLuanObj.UpdateData();
if(JDLuanObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
CurJdCen->A1=JDLuanObj.PreA;
CurJdCen->A2=JDLuanObj.AftA;
CurJdCen->T2=JDLuanObj.AftT;
* /
}
tempT12=PreJdCen->T2;

Norm3JD Norm3jdObj(CurJdCen->JDN,CurJdCen->JDE,AfterJdCen->JDN,
AfterJdCen->JDE,JdCenArray[i+2].JDN,JdCenArray[i+2].JDE,
AfterJdCen->R,AfterJdCen->A1,AfterJdCen->A2);
tempT21=Norm3jdObj.T1;
double t1,t2;
//						double PreDistJd,AftDistJd,tf;
//						PreDistJd=xyddaa(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,&tf);
//						AftDistJd=xyddaa(CurJdCen->JDN,CurJdCen->JDE,AfterJdCen->JDN,AfterJdCen->JDE,&tf);
//						t1=PreDistJd-tempT12;
//						t2=AftDistJd-tempT21;
t1=PreJdCen->DistToAfterJD-tempT12;
t2=CurJdCen->DistToAfterJD-tempT21;

if(t1<0.0||t2<0.0){Alert.Format(L"第%d个交点出错!",i);acutPrintf(L"%s\n",Alert);return;}

ThreeJDCalA1A2FromT1T2R ThreeJdObj;
ThreeJdObj.SetPreJd(PreJdCen->JDN,PreJdCen->JDE);
ThreeJdObj.SetMidJd(CurJdCen->JDN,CurJdCen->JDE);
ThreeJdObj.SetAftJd(AfterJdCen->JDN,AfterJdCen->JDE);
ThreeJdObj.SetRadius(CurJdCen->R);
ThreeJdObj.SetT1T2(t1,t2);
ThreeJdObj.UpdateData();
if(!ThreeJdObj.ERRFlag)
{
CurJdCen->A1=ThreeJdObj.A1;
CurJdCen->A2=ThreeJdObj.A2;							
}
}
}
else
{	
if(i+2<=JdCenNum-1)
{
FourJDLocationC_S JDCSObj;
JDCSObj.SetFourJD(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,
AfterJdCen->N,AfterJdCen->E,JdCenArray[i+2].N,JdCenArray[i+2].E);					
JDCSObj.SetPreJDCS(CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
JDCSObj.SetAftJDCS(AfterJdCen->R,AfterJdCen->A1,AfterJdCen->A2);
JDCSObj.UpdateData();
if(JDCSObj.ERRFlag!=true)
{
CurJdCen->A2=JDCSObj.A12;
AfterJdCen->A1=JDCSObj.A21;
}
}
}				
}

//求当前交点的转角
aa=CurJdCen->afa;

if(PreJdCen->JDXZ==IsJiaoDian && PreJdCen->LinkAfter==Luan_Link)
{//交点法计算卵型曲线

double Arcl;
double R1,R2;
JDLuanLink JDLuanObj;
JDLuanObj.SetPreJD(PreJdCen->JDN,PreJdCen->JDE,PreJdCen->R);
JDLuanObj.SetCurJD(CurJdCen->JDN,CurJdCen->JDE,CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
JDLuanObj.SetAftJD(AfterJdCen->JDN,AfterJdCen->JDE);
CurJdCen->T1=PreJdCen->DistToAfterJD-PreJdCen->T2;
JDLuanObj.SetPreT(CurJdCen->T1);
JDLuanObj.UpdateData();
if(JDLuanObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
CurJdCen->A1=JDLuanObj.PreA;
CurJdCen->A2=JDLuanObj.AftA;
CurJdCen->T2=JDLuanObj.AftT;
CurJdCen->CenN=JDLuanObj.CurCenN;
CurJdCen->CenE=JDLuanObj.CurCenE;
Arcl=JDLuanObj.ArcLength;
PEZ.a=CurJdCen->PreFwj+PI;

PEZ.x=JDLuanObj.HYN;
PEZ.y=JDLuanObj.HYE;

R1=PreJdCen->R;
R2=CurJdCen->R;
A1=CurJdCen->A1;
A2=CurJdCen->A2;
Lo1=fabs(A1*A1/R1-A1*A1/R2);
Lo2=A2*A2/R2;

/ *
//求弧长
double tempT2;
JDLuanLink::GetCenter(,CurJdCen->JDN,CurJdCen->JDE,
AfterJdCen->N,AfterJdCen->E,PreJdCen->R,CurJdCen->R,CurJdCen->A1,
CurJdCen->A2,cenN,cenE,Arcl,tempT2);
* /				

//前缓
l=Lo1;
if(l>0.0)
{
if(R1>R2)XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
XYArray[XYNum][1] = A1;
XYArray[XYNum][2] = R1;
XYArray[XYNum][3] = R2;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号

//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
/ *
//求出圆心坐标
CurJdCen->CenN=cenN;
CurJdCen->CenE=cenE;
* /
//ARC
l=Arcl;
if(l>0.0)
{
CurJdCen->CirXYNo=XYNum;
XYArray[XYNum][0] = XianYuan::ARC;
XYArray[XYNum][1] = R2;
XYArray[XYNum][2] = l;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//后缓
l=Lo2;
if(l>0.0)
{
XYArray[XYNum][0] = XianYuan::AftSPIRAL;
XYArray[XYNum][1] = A2;
XYArray[XYNum][2] = R2;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
continue;
}
//回头
if(CurJdCen->IsOrNotHuiTou)aa=2.0*PI-fabs(aa);
else aa=fabs(aa);
//求前切、后切、曲线长
R=CurJdCen->R;
A1=CurJdCen->A1;
A2=CurJdCen->A2;
Lo1=A1*A1/R;
Lo2=A2*A2/R;
getBPMTL(dms_rad(aa),R,Lo1,Lo2,&BPMTL[0]);
T1=BPMTL[6];//前切长
T2=BPMTL[7];//后切长
L=BPMTL[8];//曲线长
CurJdCen->T1=T1;
CurJdCen->T2=T2;
CurJdCen->L=L;
//求直缓点的坐标
Nzh=CurJdCen->N+T1*cos(CurJdCen->PreFwj);
Ezh=CurJdCen->E+T1*sin(CurJdCen->PreFwj);


//计算前一段夹直线长
l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
PEZ.x=Nzh+l*cos(CurJdCen->PreFwj);
PEZ.y=Ezh+l*sin(CurJdCen->PreFwj);
PEZ.a=CurJdCen->PreFwj+PI;
StdJiao(PEZ.a);
if(PreJdCen->LinkAfter==NotRelation && l>0.0)
{
XYArray[XYNum][0] = XianYuan::LINE;
XYArray[XYNum][1] = l;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
else
{
//acutPrintf(L"第%d个交点与前面的夹直线不够!\n",i);
}
//前缓
l=Lo1;
if(l>0.0)
{
XYArray[XYNum][0] = XianYuan::PreSPIRAL;
XYArray[XYNum][1] = A1;
XYArray[XYNum][2] = R;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//求出圆心坐标
/ *
SPIRAL pSpiral(PEZ.x,PEZ.y,PEZ.a,XYArray[XYNum-1][1],VERY_MAX,R,CurJdCen->LeftOrRight);
double JiaoGQToCen;//缓和曲线与当前圆弧公切点到圆心的方位角
JiaoGQToCen=pSpiral.GetQXFwj(pSpiral.EndL)+CurJdCen->LeftOrRight*0.5*PI;
CurJdCen->CenN=pSpiral.EndN+CurJdCen->R*cos(JiaoGQToCen);
CurJdCen->CenE=pSpiral.EndE+CurJdCen->R*sin(JiaoGQToCen);
* /
double JiaoGQToCen;//缓和曲线与当前圆弧公切点到圆心的方位角
JiaoGQToCen=PEZ.a+CurJdCen->LeftOrRight*0.5*PI;
CurJdCen->CenN=PEZ.x+CurJdCen->R*cos(JiaoGQToCen);
CurJdCen->CenE=PEZ.y+CurJdCen->R*sin(JiaoGQToCen);
//ARC
l=L-Lo1-Lo2;

if(l>0.0)
{
CurJdCen->CirXYNo=XYNum;
XYArray[XYNum][0] = XianYuan::ARC;
XYArray[XYNum][1] = R;
XYArray[XYNum][2] = l;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//后缓
l=Lo2;
if(l>0.0)
{
XYArray[XYNum][0] = XianYuan::AftSPIRAL;
XYArray[XYNum][1] = A2;
XYArray[XYNum][2] = R;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
}
//为圆心
if(CurJdCen->JDXZ==IsCenter)
{
CurJdCen->CenN=CurJdCen->N;
CurJdCen->CenE=CurJdCen->E;

int StartXYNo,EndXYNo;//该交点的起终线元号
StartXYNo=XYNum;
bool IsAllPreSpiral=true;
bool IsAllAftSpiral=true;
bool IsHasJiaZX=true;
//获取前、后JD/CENTER指针
PreJdCen=&JdCenArray[i-1];
AfterJdCen=&JdCenArray[i+1];
bool HasAfterSpiral=true;
//如果前面是交点 ,且不为卵型连接
if(PreJdCen->JDXZ==IsJiaoDian/ *&&PreJdCen->LinkAfter!=Luan_Link* /)
{
IsHasJiaZX=true;
//if(fabs(xyddaa(CurJdCen->N,CurJdCen->E,PreJdCen->N,PreJdCen->E,&tj)-CurJdCen->R)<0.001)
if(CurJdCen->A1<0.001)
{
//直圆公切
/ *
A1=0.0;
N1=PreJdCen->N;
E1=PreJdCen->E;
Nzh=N1;
Ezh=E1;
* /
A1=0.0;
double fwjCenToQD=PreJdCen->AftFwj-CurJdCen->LeftOrRight*0.5*PI;
N1=CurJdCen->CenN+CurJdCen->R*cos(fwjCenToQD);
E1=CurJdCen->CenE+CurJdCen->R*sin(fwjCenToQD);
Nzh=N1;
Ezh=E1;

Lo1=0.0;
CurJdCen->A1=0.0;
}
else
{
ZYLink ZYObj(PreJdCen->N,PreJdCen->E,PreJdCen->AftFwj,
CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->LeftOrRight );


if(ZYObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
A1=CurJdCen->A1;
N1=ZYObj.pSPIRAL.EndN;
E1=ZYObj.pSPIRAL.EndE;
Nzh=ZYObj.pSPIRAL.BaseN;
Ezh=ZYObj.pSPIRAL.BaseE;
Lo1=A1*A1/CurJdCen->R;
}
}
else//前面是圆心
{
IsHasJiaZX=false;
//由后接关系分情况处理
//一、S型
if(PreJdCen->LinkAfter==S_Link)
{
A1=CurJdCen->A1;
Lo1=A1*A1/CurJdCen->R;
}
//二、C型
if(PreJdCen->LinkAfter==C_Link)
{
A1=CurJdCen->A1;
Lo1=A1*A1/CurJdCen->R;					
}
//三、卵型
if(PreJdCen->LinkAfter==Luan_Link)
{
A1=CurJdCen->A1;
Lo1=fabs(A1*A1/CurJdCen->R-A1*A1/PreJdCen->R);
IsAllPreSpiral=false;			
}
/ *
Nzh=ZYObj.pSPIRAL.BaseN;
Ezh=ZYObj.pSPIRAL.BaseE;
* /
//四、有夹直线
if(PreJdCen->LinkAfter==NotRelation)
{
IsHasJiaZX=true;
YYQXLink YYQXObj;
YYQXObj.SetCir1(PreJdCen->N,PreJdCen->E,PreJdCen->R);
YYQXObj.SetCir2(CurJdCen->N,CurJdCen->E,CurJdCen->R);
YYQXObj.SetA(PreJdCen->A2,CurJdCen->A1);

if(PreJdCen->LeftOrRight<0 && CurJdCen->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
else if(PreJdCen->LeftOrRight>0 && CurJdCen->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
else if(PreJdCen->LeftOrRight<0 && CurJdCen->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
else YYQXObj.SetQXClass(S_S);

YYQXObj.CalQX();

if(YYQXObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}

A1=CurJdCen->A1;
if(CurJdCen->A1>0.0)
{
N1=YYQXObj.pSPIRAL2.EndN;//PtOnCirN2;
E1=YYQXObj.pSPIRAL2.EndE;//PtOnCirE2;
Nzh=YYQXObj.pSPIRAL2.StartN;//GQN2;
Ezh=YYQXObj.pSPIRAL2.StartE;//GQE2;
}
else
{
N1=YYQXObj.GQN2;
E1=YYQXObj.GQE2;
Nzh=YYQXObj.GQN2;
Ezh=YYQXObj.GQE2;
}
Lo1=A1*A1/CurJdCen->R;
}
}
//如果后一个为交点
if(AfterJdCen->JDXZ==IsJiaoDian)
{
//求后一条边的方位角
AfterJdCen->PreFwj=GetFwjByJd_Center_A_LR(AfterJdCen->N,AfterJdCen->E,
CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->A2,
-1*CurJdCen->LeftOrRight);		

double tj;
if(fabs(xyddaa(CurJdCen->N,CurJdCen->E,AfterJdCen->N,AfterJdCen->E,&tj)-CurJdCen->R)<0.001)
{
A2=0.0;
N2=AfterJdCen->N;
E2=AfterJdCen->E;
Lo2=0.0;
CurJdCen->A2=0.0;
}
else
{
ZYLink ZYObj(AfterJdCen->N,AfterJdCen->E,AfterJdCen->PreFwj,
CurJdCen->N,CurJdCen->E,CurJdCen->R,-1*CurJdCen->LeftOrRight );

if(ZYObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
A2=CurJdCen->A2;
N2=ZYObj.pSPIRAL.EndN;
E2=ZYObj.pSPIRAL.EndE;
Lo2=A2*A2/CurJdCen->R;
}
}
else//如果后一个为圆心
{
//			double NHY,EHY;
//由后接关系分情况处理
//一、S型
if(CurJdCen->LinkAfter==S_Link)
{
YYSLink YYSObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,
CurJdCen->A2,AfterJdCen->A1,CurJdCen->LeftOrRight );

if(YYSObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到真实缓和曲线参数
CurJdCen->A2=YYSObj.A1;
AfterJdCen->A1=YYSObj.A2;

A2=YYSObj.A1;
N2=YYSObj.PtOnCir1N;
E2=YYSObj.PtOnCir1E;
Lo2=A2*A2/CurJdCen->R;
}
//二、C型
if(CurJdCen->LinkAfter==C_Link)
{
YYCLink YYCObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,
CurJdCen->A2,AfterJdCen->A1,CurJdCen->LeftOrRight );

if(YYCObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到真实缓和曲线参数
CurJdCen->A2=YYCObj.A1;
AfterJdCen->A1=YYCObj.A2;

A2=YYCObj.A1;
N2=YYCObj.pSPIRAL1.EndN;
E2=YYCObj.pSPIRAL1.EndE;
Lo2=A2*A2/CurJdCen->R;
}
//三、卵型
if(CurJdCen->LinkAfter==Luan_Link)
{
HasAfterSpiral=false;
IsAllAftSpiral=false;
YYLuanLink YYLuanObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,															
CurJdCen->LeftOrRight );

if(YYLuanObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到缓和曲线参数					
CurJdCen->A2=0.0;
AfterJdCen->A1=YYLuanObj.A;

A2=YYLuanObj.A;
if(CurJdCen->R>AfterJdCen->R)
{
N2=YYLuanObj.pSPIRAL.StartN;
E2=YYLuanObj.pSPIRAL.StartE;
}
else
{
N2=YYLuanObj.pSPIRAL.EndN;
E2=YYLuanObj.pSPIRAL.EndE;
}
Lo2=A2*A2/CurJdCen->R;
}
//四、有夹直线
if(CurJdCen->LinkAfter==NotRelation)
{
YYQXLink YYQXObj;
YYQXObj.SetCir1(CurJdCen->N,CurJdCen->E,CurJdCen->R);
YYQXObj.SetCir2(AfterJdCen->N,AfterJdCen->E,AfterJdCen->R);
YYQXObj.SetA(CurJdCen->A2,AfterJdCen->A1);

if(CurJdCen->LeftOrRight<0 && AfterJdCen->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
else if(CurJdCen->LeftOrRight>0 && AfterJdCen->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
else if(CurJdCen->LeftOrRight<0 && AfterJdCen->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
else YYQXObj.SetQXClass(S_S);

YYQXObj.CalQX();

if(YYQXObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到真实缓和曲线参数

A2=CurJdCen->A2;
if(CurJdCen->A2>0.0)
{
N2=YYQXObj.pSPIRAL1.EndN;//PtOnCirN1;
E2=YYQXObj.pSPIRAL1.EndE;//PtOnCirE1;
}
else
{
N2=YYQXObj.GQN1;
E2=YYQXObj.GQE1;
}
Lo2=A2*A2/CurJdCen->R;
}
}
EndXYNo=XYNum;//初值
PEZ.a=CurJdCen->PreFwj+PI;
//夹直线
//计算前一段夹直线长
if(IsHasJiaZX)
{
l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
if(l>0.0)
{
EndXYNo=XYNum;

XYArray[XYNum][0] = XianYuan::LINE;
XYArray[XYNum][1] = l;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
}
//前缓
l=Lo1;
if(l>0.0)
{
EndXYNo=XYNum;

if(IsAllPreSpiral)//完整前缓
{
XYArray[XYNum][0] = XianYuan::PreSPIRAL;
XYArray[XYNum][1] = A1;
XYArray[XYNum][2] = CurJdCen->R;
XYArray[XYNum][3] = 0.0;
}
else
{
if(PreJdCen->R>CurJdCen->R)
XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
XYArray[XYNum][1] = A1;
XYArray[XYNum][2] = PreJdCen->R;
XYArray[XYNum][3] = CurJdCen->R;
}
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//用于计算圆曲线长			
N1=PEZ.x;	E1=PEZ.y;
//ARC
l=GetArcLengthBy2PointOnCircleAndLR(CurJdCen->N,CurJdCen->E,CurJdCen->R,
N1,E1,N2,E2,CurJdCen->LeftOrRight);
if(l>0.0)
{
//判断是否回头
if(l>CurJdCen->R*PI)CurJdCen->IsOrNotHuiTou=true;
else CurJdCen->IsOrNotHuiTou=false;
EndXYNo=XYNum;
CurJdCen->CirXYNo=XYNum;
XYArray[XYNum][0] = XianYuan::ARC;
XYArray[XYNum][1] = CurJdCen->R;
XYArray[XYNum][2] = l;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//后缓
l=Lo2;
if(l>0.0&&HasAfterSpiral)
{
EndXYNo=XYNum;

if(IsAllAftSpiral)//完整后缓
{
XYArray[XYNum][0] = XianYuan::AftSPIRAL;
XYArray[XYNum][1] = A2;
XYArray[XYNum][2] = CurJdCen->R;
XYArray[XYNum][3] = 0.0;
}
else
{
if(CurJdCen->R>AfterJdCen->R)
XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
XYArray[XYNum][1] = A2;
XYArray[XYNum][2] = CurJdCen->R;
XYArray[XYNum][3] = AfterJdCen->R;
}
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//计算交点
CurJdCen->PreFwj=XYArray[StartXYNo][5]+PI;
CurJdCen->AftFwj=PEZ.a;
double n1,e1,n2,e2,n3,e3,n4,e4;//两切线

int i;
i=StartXYNo;
if(XYArray[i][0]<1.5)i++;
n1=XYArray[i][8];
e1=XYArray[i][9];

n2=n1+1000.0*cos(CurJdCen->PreFwj);
e2=e1+1000.0*sin(CurJdCen->PreFwj);

n3=PEZ.x;
e3=PEZ.y;

n4=n3+1000.0*cos(CurJdCen->AftFwj);
e4=e3+1000.0*sin(CurJdCen->AftFwj);

INTERS(&CurJdCen->JDN,&CurJdCen->JDE,n1,e1,n2,e2,n3,e3,n4,e4);

double tmpjiao;
CurJdCen->T1=xyddaa(n1,e1,CurJdCen->JDN,CurJdCen->JDE,&tmpjiao);
CurJdCen->T2=xyddaa(n3,e3,CurJdCen->JDN,CurJdCen->JDE,&tmpjiao);
CurJdCen->L=PEZ.lc-XYArray[StartXYNo][6];
}
/ *
CurJdCen->JdNo=JD0+iJD;
iJD++;
* /
}
CurJdCen=&JdCenArray[JdCenNum-1];
//夹直线
Nzh=JdCenArray[JdCenNum-1].N;//终点
Ezh=JdCenArray[JdCenNum-1].E;

ads_printf(L"@@@@ ZDN=%lf ZDE=%lf\n",
JdCenArray[JdCenNum-1].N,JdCenArray[JdCenNum-1].E);

//计算前一段夹直线长
l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
if(l>0.0)
{
XYArray[XYNum][0] = XianYuan::LINE;
XYArray[XYNum][1] = l;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应最后一个交点
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//最后一个线元为终点
XYArray[XYNum][0] = 0.0;
XYArray[XYNum][1] = 0.0;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应最后一个交点
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
//XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
GetDLTylc();

//交点
CurJdCen=&JdCenArray[JdCenNum-1];
CurJdCen->JDN=PEZ.x;
CurJdCen->JDE=PEZ.y;
CurJdCen->A1=0.0;
CurJdCen->A2=0.0;
CurJdCen->T1=0.0;
CurJdCen->T2=0.0;
//计算线路终点的统一里程
int j=(int)(XYArray[XYNum-1][0]+0.001);
if(j==XianYuan::LINE)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1];
else if(j==XianYuan::ARC)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][2];
else if(j==XianYuan::AftSPIRAL)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/XYArray[XYNum-1][2];
else 
{
//终点投影
XLLength=PROJ_ML(PEZ.x,PEZ.y);
}

if(XYNum>0) XYArray[0][4]=DLArray[0].ELC;

LenOfLine = XLLength - XYArray[0][4];//线路长度 

CString TGH;
DLNum=DLArray.GetSize();
double endXlc=XLC(XLLength,TGH);
//	double endXlc=XLC(XYArray[XYNum-1][6],TGH);
//修正最后一个断链
setDLM(DLNum-1,endXlc,endXlc,DLArray[DLNum-1].BGH,DLArray[DLNum-1].EGH);
//	
//	acutPrintf(L"%s ",DLArray[DLNum-1].BGH);
//	acutPrintf(L"%lf ",DLArray[DLNum-1].BLC);
//	acutPrintf(L"%s ",DLArray[DLNum-1].EGH);
//	acutPrintf(L"%lf ",DLArray[DLNum-1].ELC);
//	acutPrintf(L"\n");


if(JdCenNum>2)
{
if(JdCenArray[1].T1>JdCenArray[0].DistToAfterJD)
{
JdCenArray[0].JDN=XYArray[1][8];//JdCenArray[1].JDN+JdCenArray[1].T1*cos(JdCenArray[1].PreFwj);
JdCenArray[0].JDE=XYArray[1][9];//JdCenArray[1].JDE+JdCenArray[1].T1*sin(JdCenArray[1].PreFwj);
JdCenArray[0].N=JdCenArray[0].JDN;
JdCenArray[0].E=JdCenArray[0].JDE;
}

if(JdCenArray[JdCenNum-2].T2>JdCenArray[JdCenNum-2].DistToAfterJD)
{
JdCenArray[JdCenNum-1].JDN=JdCenArray[JdCenNum-2].JDN+JdCenArray[JdCenNum-2].T2*cos(JdCenArray[JdCenNum-2].AftFwj);
JdCenArray[JdCenNum-1].JDE=JdCenArray[JdCenNum-2].JDE+JdCenArray[JdCenNum-2].T2*sin(JdCenArray[JdCenNum-2].AftFwj);
JdCenArray[JdCenNum-1].N=JdCenArray[JdCenNum-1].JDN;
JdCenArray[JdCenNum-1].E=JdCenArray[JdCenNum-1].JDE;
}
}

ads_printf(L"BBBBB: %lf %lf\n",
JdCenArray[JdCenNum-1].JDE,JdCenArray[JdCenNum-1].JDN);

//	CalJiaoDian();
//	OutData();
//	acutPrintf(L"\nOK!");
}
*/

//过圆外一点做切线，若圆曲线配了缓和曲线，考虑缓和曲线的内移量
double JD_CENTER::GetFwjByJd_Center_A_LR(double jdn, double jde, double cn, double ce,
										 double R, double A, int lr)
{
	double JiaoJdToCen,OC;
	//     fwj        dist
	if(R<0.001)
	{
		//	AfxMessageBox(L"R=0");
		ads_printf(L"R=0\n");
		return 0.0;
	}
	/*
	if(A<0.00001)
	{
	OC = xyddaa(jdn,jde,cn,ce,&JiaoJdToCen);
	return JiaoJdToCen-lr*0.5*PI;
	}
	*/
	double BigR;
	if(A<0.00001)
		BigR=R;
	else
	{
		double l=A*A/R;	
		double t=l/(2*R);
		BigR=spiral_y(A,l)+R*cos(t);	
	}
	OC = xyddaa(jdn,jde,cn,ce,&JiaoJdToCen);
	//	ads_printf(L"oc=%lf br=%lf %lf lr=%d\n",OC,BigR,JiaoJdToCen,lr);
	double bta;
	if(OC>BigR)
	{
		if(BigR<0.00001)
			bta=0.0;
		else 
			bta = asin(BigR/OC);	
		if(lr<0)//left
		{
			return JiaoJdToCen+bta;
		}
		else//right
		{
			return JiaoJdToCen-bta;
		}
	}
	if(fabs(OC-BigR)<0.01)
		return JiaoJdToCen-lr*0.5*PI;
	return 0.0;
}

Adesk::Boolean JD_CENTER::subWorldDraw(AcGiWorldDraw* pWd)
{
	BAS_DRAW_FUN::JdCenter OneJDTemp;
	std::vector<JdCenter> watch;
	for(int i=0;i<JdCenNum;i++)
	{
		OneJDTemp=JdCenArray[i];
		
		
		if (JdCenArray[i].R>1)
		{
			/*if (IfDelJZX&&i == PreJD)
			{
				continue;
			}*/
			JdCenArray[i].Ls1 = JdCenArray[i].A1*JdCenArray[i].A1 / JdCenArray[i].R;
			JdCenArray[i].Ls2 = JdCenArray[i].A2*JdCenArray[i].A2 / JdCenArray[i].R;
			
		}
		else
		{
			JdCenArray[i].Ls1=JdCenArray[i].Ls2=0;
		}
		watch.push_back(JdCenArray[i]);
	}
	

	if(IfCalXY&&JdCenNum>=2)
	{

		CalXYArray();
		/*		for(int i=0; i<XYNum; i++)
		{
		//	fwprintf(fpw, L"%d %lf %lf %lf %lf %lf\n",i,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,JdCenArray[i].R,JdCenArray[i].A2);
		ads_printf(L"3------------%d %d ",XYNum,i);
		for(int j=0; j<10; j++)
		ads_printf(L"%lf ",XYArray[i][j]);
		ads_printf(L"\n");
		}*/
	}
	else
	{
		if(XYNum>0)
		{
			int k,j;
			double l,INDATA2[5];
			XYArray[0][4]=XYArray[1][6]=DLArray[0].ELC;
			for(int i=1;i<XYNum;i++)
			{
				for (k=0;k<5;k++) INDATA2[k]=XYArray[i][k];
				l=Get_INDATA_L(INDATA2,&j);
				XYArray[i+1][6]= XYArray[i][6]+l;
			}
		}
	}
	//	for(int i=0; i<XYNum; i++)
	//	{
	//		//	fwprintf(fpw, L"%d %lf %lf %lf %lf %lf\n",i,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,JdCenArray[i].R,JdCenArray[i].A2);
	//		ads_printf(L"3------------%d %d ",XYNum,i);
	//		for(int j=0; j<10; j++)
	//			ads_printf(L"%lf ",XYArray[i][j]);
	//		ads_printf(L"\n");
	//	}
	if (XYNum>0) 
	{
		XYArray[0][4]=DLArray[0].ELC;
		LenOfLine = XYArray[XYNum][6] - XYArray[0][4];//线路长度 
		//	OutData();
		CString TGH;			
		double endXlc=fabs(XLC(XYArray[XYNum][6],TGH));	
		if(DLNum>0)
			setDLM(DLNum-1,endXlc,endXlc,DLArray[DLNum-1].BGH,DLArray[DLNum-1].EGH);

		Draw(pWd);

		bool IsInterSect = false;
		IsInterSect = Caljd();
		if (IsInterSect)
		{
			DrawInterSect(pWd);
		}

	}



	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}
/*
函数功能：求线路与铁路的交点
传入参数：无
传出参数：无
*/
bool JD_CENTER::Caljd()
{
	if (m_RelRailName.IsEmpty())
		return false;
	//CString RailName = m_RelRailName;			//铁路名称
	//CString JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);	//基线或者二线
	//CString ProStr = RailName.Mid(0, RailName.GetLength() - 2);	//方案名
	//CString RailMdbName = Cworkdir/*.Mid(0, Cworkdir.GetLength()-5)*/;
	//RailMdbName = RailMdbName + L"\\DATA\\" + ProStr + L".mdb";	//铁路方案路径名
	//20200523	考虑兼容性处理铁路路径
	CString RailName, JXorEX, ProStr, RailMdbName;
	if (m_RelRailName.ReverseFind('\\') == -1)	//如果是老的类型
	{
		RailName = m_RelRailName;			//铁路名称
		JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);	//基线或者二线
		ProStr = RailName.Mid(0, RailName.GetLength() - 2);	//方案名
		RailMdbName = Cworkdir/*.Mid(0, Cworkdir.GetLength()-5)*/;
		RailMdbName = RailMdbName + L"\\DATA\\" + ProStr + L".mdb";	//铁路方案路径名
	}
	else	//目前采取的全路径名
	{
		RailName = GetProByMdbName(m_RelRailName);
		JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);	//基线或者二线
		ProStr = RailName.Mid(0, RailName.GetLength() - 2);	//方案名
		RailMdbName = m_RelRailName;
		RailMdbName.Replace(RailName, ProStr);
	}

	GTZX_ROAD* pSJJX = NULL;
	GTZX2_ROAD* pSJEX = NULL;
	GTZX_JYX_ROAD* pJYJX=NULL;
	GTZX2_JYX_ROAD* pJYEX=NULL;
	bool IsInterSect = false;
	//bool IsJYX = PmPointer::SJXorJYX(RailMdbName, 5, false);
	bool IsJYX = m_IsJYX;
	if (JXorEX==L"基线")
	{
		if (IsJYX)
		{
			//ads_printf(L"1\n");
			pJYJX=PmPointer::GetGTSJJX(RailMdbName);	//既有继续

			if (pJYJX)
				IsInterSect = PmPointer::CalIntersectPt(this,pJYJX);
		}
		else
		{
			pSJJX = PmPointer::GetSJJX(RailMdbName);	//设计基线
			if (pSJJX)
			{
				IsInterSect = PmPointer::CalIntersectPt(this, pSJJX);
				//检测实体生成
				//2019.10.2赵璐
				//if (DRAWRAIL==FALSE)
				//{
				//	AcDbObjectId TieluID = NULL;
				//	TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJJX);
				//	acedGetAcadDwgView()->SetFocus();
				//	double XMin = pSJJX->JDarray[0][2], XMax = 0.00, YMin = pSJJX->JDarray[0][1], YMax = 0.00;
				//	for (int i = 0; i < pSJJX->NJD; i++)
				//	{
				//		if (XMin > pSJJX->JDarray[i][2])
				//			XMin = pSJJX->JDarray[i][2];

				//		if (XMax < pSJJX->JDarray[i][2])
				//			XMax = pSJJX->JDarray[i][2];

				//		if (YMin > pSJJX->JDarray[i][1])
				//			YMin = pSJJX->JDarray[i][1];

				//		if (YMax < pSJJX->JDarray[i][1])
				//			YMax = pSJJX->JDarray[i][1];
				//	}


				//	/*pSJJX = NULL;*/

				//	BAS_DRAW_FUN_JYX MZOOM;
				//	MZOOM.mZOOM(XMin, YMin, XMax, YMax);
				//}
				//
				///*pSJJX->setLayer(L"铁路平面设计专用层");*/
				
			}
			
		}
	}
	else if (JXorEX==L"左线")
	{
		if (IsJYX)
		{
			pJYEX=PmPointer::GetGTSJEX(RailMdbName);	//既有二线
			if (pJYEX)
				IsInterSect = PmPointer::CalIntersectPt(this,pJYEX);
		}
		else
		{
			pSJEX = PmPointer::GetSJEX(RailMdbName, -1);	//设计二线
			if (pSJEX)
				IsInterSect = PmPointer::CalIntersectPt(this,pSJEX);
		}

	}
	else if (JXorEX==L"右线")
	{
		if (IsJYX)
		{
			pJYEX=PmPointer::GetGTSJEX(RailMdbName);	//既有二线
			if (pJYEX)
				IsInterSect = PmPointer::CalIntersectPt(this,pJYEX);
		}
		else
		{
			pSJEX = PmPointer::GetSJEX(RailMdbName, 1);	//设计二线
			if (pSJEX)
				IsInterSect = PmPointer::CalIntersectPt(this, pSJEX);
		}
	}
	else
	{
		//	AfxMessageBox(L"对应铁路线路名设置有误");
		return false;
	}
	return IsInterSect;
}

//绘线位、标注
void JD_CENTER::Draw(AcGiWorldDraw* pWd)
{
	//	if(IfCalXY&&JdCenNum<2)return;	
	int i=0,j=0;
	xlpoint PB,PE;
	double INDATA[5];
	double de,aa,dd,c,n;

	if(SHOWJD)
	{
		// 绘交点连线
		AcGePoint3d Pt,PtText,*verts=new AcGePoint3d[JdCenNum];
		AcGeVector3d  normal(0.0,0.0,1.0);
		for (i = 0; i < JdCenNum; i++) 
		{
			verts[i].x=JdCenArray[i].JDE; verts[i].y=JdCenArray[i].JDN;	verts[i].z=0.0;		
		}

		if (pWd!= NULL) 
		{
			pWd->subEntityTraits().setSelectionMarker(1);
			if(_tcscmp(RoadName, L"主线")==0)pWd->subEntityTraits().setColor(BaseSettting::JDLineColor);	
			else pWd->subEntityTraits().setColor(0);
			pWd->geometry().polyline(JdCenNum,verts);
		}
		else
		{
			AcGePoint2dArray Pt2dAy;
			for(i=0;i<JdCenNum;i++)Pt2dAy.append(AcGePoint2d(verts[i].x,verts[i].y));
			makepolyline(Pt2dAy,BaseSettting::JDLineColor,0.0, L"交点连线");			
		}

		// 绘交点处圆
		for (i = 0; i < JdCenNum; i++) 
		{
			//if(JdCenArray[i].JdNo<0.0)continue;//辅助交点

			Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
			ACHAR JDSTR[20];
			double b;
			if(i>0 && i<JdCenNum-1)
			{
				double tempaa=JdCenArray[i].PreFwj+PI;
				if(tempaa>2.0*PI)tempaa-=2.0*PI;

				aa= JdCenArray[i].AftFwj-tempaa;
				if (aa<-pi) aa=aa+2*pi;
				if  (aa>pi) aa=aa-2*pi;

				if (aa>0) {  c=1; dd=texth;  }  //右偏
				else { c=-1; dd=texth+texth; }; //左偏
				de=tempaa+aa*0.5;

				_stprintf(JDSTR, L"%s%.0lf",JDGH,JdCenArray[i].JdNo); 
				n=_tcslen(JDSTR); 
				PtText.x=Pt[X]-n*0.5*texth*sin(de)+dd*sin(de-c*pi*0.5) ; 
				PtText.y=Pt[Y]-n*0.5*texth*cos(de)+dd*cos(de-c*pi*0.5);

				b= 0.5*pi-de;
				//		b= de;
				//		b=2.5*PI-de+DrawLeftOrRight*0.5*PI;
				// G_maketext(pWd,p1,ch1,de,texth ,4,1);   
				//	maketext( p1,ch1,da,texth ,4,1);  // 交点号

				/*

				PtText=Pt;
				_stprintf(JDSTR, L"%s%.0lf",JDGH,JdCenArray[i].JdNo);
				b=0.5*(JdCenArray[i].PreFwj+JdCenArray[i].AftFwj);
				PtText.x+=JdCenArray[i].LeftOrRight*(BaseSettting::JDCirRadius+texth)*sin(b);
				PtText.y+=JdCenArray[i].LeftOrRight*(BaseSettting::JDCirRadius+texth)*cos(b);
				b=2.5*PI-b+DrawLeftOrRight*0.5*PI;*/
			}
			if(pWd!=NULL)
			{
				pWd->geometry().circle(Pt,BaseSettting::JDCirRadius,normal);
				if(i>0 && i<JdCenNum-1)
				{
					if(_tcscmp(RoadName, L"主线")==0)
					{
						pWd->subEntityTraits().setColor(BaseSettting::JDLineColor);	
						G_maketext(pWd,PtText,JDSTR,b,texth,BaseSettting::JDLineColor,0);
					}
					else
					{
						pWd->subEntityTraits().setColor(0);	
						G_maketext(pWd,PtText,JDSTR,b,texth,0,0);
					}
				}
			}
			else
			{
				makecircle(Pt,BaseSettting::JDCirRadius,BaseSettting::JDLineColor);
				if(i>0 && i<JdCenNum-1)
					maketext(PtText,JDSTR,b,texth,BaseSettting::JDLineColor,3);
			}
		}
	}

	// 绘线位
	PB.x=XYArray[0][1]; //N
	PB.y=XYArray[0][2]; // E
	PB.a=XYArray[0][3];// FA
	PB.lc=XYArray[0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for(i=1;i<XYNum;i++)
	{
		for (j=0;j<5;j++)INDATA[j]=XYArray[i][j];
		PB.a=XYArray[i][5];
		PB.x=XYArray[i][8];
		PB.y=XYArray[i][9];
		if(pWd==NULL)
			GUXWXY(INDATA,&PB,&PE,1); //绘线路中线
		else
		{
			int icolor=-1;
			int XYClass=(int)(XYArray[i][0]+0.001);
			double Rmin,A;
			if(XYClass==3)//前缓
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][2];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			else if(XYClass==4)//后缓
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][2];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			else if(XYClass==5)//5-不完整缓R大->R小
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][3];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			else if(XYClass==6)// 6-不完整缓R小->R大
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][2];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			G_WXY(pWd,INDATA,&PB,&PE,icolor); //绘线路中线
		}
		//PB=PE;
	}

	BZLC(pWd,100.0,DrawLeftOrRight,1,1);//字头朝向为-1时倒向 

	BZTZD(pWd,0/*-1*DrawLeftOrRight*/,1);

	xlpoint  PZtmp; 
	AcGePoint3d  ptb,pte,Pt;
	AcGePoint3d MidPt,StartPt,EndPt,APt1,APt2;//线路走向
	double B;
	double xl_length=XYArray[XYNum][6]-XYArray[1][6];
	PZtmp.lc=XYArray[1][6]+0.5*xl_length;  // 统一里程
	xlpoint_pz(&PZtmp);  
	B=PZtmp.a;
	pte[X]=PZtmp.y+xl_length*sin(B+pi*0.5)/4.0;
	pte[Y]=PZtmp.x+xl_length*cos(B+pi*0.5)/4.0;
	ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
	//////////////////////////////////////////////////////////////////////////
	//线路走向
	if(pWd)
	{
		MidPt.x=0.5*(ptb.x+pte.x);
		MidPt.y=0.5*(ptb.y+pte.y);
		MidPt.z=0.0;

		StartPt.x=MidPt.x-xl_length*sin(B)/6.0;
		StartPt.y=MidPt.y-xl_length*cos(B)/6.0;
		StartPt.z=0.0;

		EndPt.x=MidPt.x+xl_length*sin(B)/6.0;
		EndPt.y=MidPt.y+xl_length*cos(B)/6.0;
		EndPt.z=0.0;

		double ArrowJ=0.03*PI;
		double ArrowLen=xl_length/15.0;
		APt1.x=EndPt.x+ArrowLen*sin(B+PI+ArrowJ);
		APt1.y=EndPt.y+ArrowLen*cos(B+PI+ArrowJ);
		APt1.z=0.0;

		APt2.x=EndPt.x+ArrowLen*sin(B+PI-ArrowJ);
		APt2.y=EndPt.y+ArrowLen*cos(B+PI-ArrowJ);
		APt2.z=0.0;

		AcGePoint3d ArrowEndPt;
		ArrowEndPt.x=EndPt.x+0.6*ArrowLen*sin(B+PI);
		ArrowEndPt.y=EndPt.y+0.6*ArrowLen*cos(B+PI);
		ArrowEndPt.z=0.0;
		int icolor=1;
		if(FLDNo>0)icolor=0;
		G_makeline(pWd,StartPt,EndPt,icolor);
		G_makeline(pWd,APt1,EndPt,icolor);
		G_makeline(pWd,APt2,EndPt,icolor);		 
		G_makeline(pWd,APt1,ArrowEndPt,icolor);
		G_makeline(pWd,APt2,ArrowEndPt,icolor);
		//////////////////////////////////////////////////////////////////////////

		G_makeline(pWd,ptb,pte,icolor);
		Pt[Y]=pte[Y]+xl_length*cos(B)/20.0;
		Pt[X]=pte[X]+xl_length*sin(B)/20.0;
		ptb[Y]=pte[Y]+xl_length*cos(B+pi)/20.0;
		ptb[X]=pte[X]+xl_length*sin(B+pi)/20.0;
		G_makeline(pWd,Pt,ptb,icolor);
		pte[Y]=Pt[Y]+xl_length*cos(B+pi*0.5)/20.0;
		pte[X]=Pt[X]+xl_length*sin(B+pi*0.5)/20.0;
		G_makeline(pWd,Pt,pte,icolor);
		Pt[Y]=ptb[Y]+xl_length*cos(B+pi*0.5)/20.0;
		Pt[X]=ptb[X]+xl_length*sin(B+pi*0.5)/20.0;
		G_makeline(pWd,ptb,Pt,icolor);
		pte[Y]=Pt[Y]+xl_length*cos(B)/10.0;
		pte[X]=Pt[X]+xl_length*sin(B)/10.0;
		G_makeline(pWd,Pt,pte,icolor);
	}

	CString name,Dir;
	double b;
	ACHAR str[256];
	name=mdbname;
	name.MakeUpper();
	ReadWorkdir();
	Dir=Cworkdir;
	Dir.MakeUpper();
	Cworkdir=Dir;
	name.Replace(Cworkdir,NULL);
	name.Replace(L"\\DATA\\", NULL);
	name.Replace(L".MDB", NULL);

	CString str1 = RoadName;
	name +=  L"方案" + str1;
	_tcscpy(str,name);
	Pt[Y]=Pt[Y]+xl_length*cos(B-pi*0.5)/42.0;
	Pt[X]=Pt[X]+xl_length*sin(B-pi*0.5)/42.0;
	b=B+pi*0.5;
	b=pi*0.5-b; 
	if (b<0.0) b=2*pi+b;	  if (b>2*pi) b=b-2*pi;		         
	G_maketext(pWd,Pt,str,b+pi*0.5,xl_length/80.0,4,2);	

}

void JD_CENTER::OutData()
{
	FILE *fp;
	//fp=_wfopen(L"C:\\交点.txt", L"w");
	//	JdCenNum=JdCenArray.GetSize();
	//	fwprintf(fp, L"%d\n",JdCenNum);
	//	for(int i=0;i<JdCenNum;i++)
	//	{
	//		fwprintf(fp, L"%d %d %lf %lf %lf %lf %lf %d %d %d\n",
	//			i,JdCenArray[i].JDXZ,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,
	//			JdCenArray[i].R,JdCenArray[i].A2,JdCenArray[i].LeftOrRight,JdCenArray[i].LinkAfter,
	//			JdCenArray[i].CirXYNo);
	//	}
	//	fclose(fp);
	//	if(JdCenNum<2)return;
	if(XYNum>=2)
	{
		fp=_wfopen(L"C:\\线元.txt", L"w");
		fwprintf(fp, L"%d\n",XYNum);
		for(int i=0;i<=XYNum;i++)
		{
			for(int j=0;j<11;j++)fwprintf(fp, L"%lf ",XYArray[i][j]);
			fwprintf(fp, L"%d\n",XYJDNO[i]);
			//		fwprintf(fp, L"\n");
		}
		fclose(fp);
	}

	//	fp=_wfopen(L"C:\\断链.txt", L"w");
	//	fwprintf(fp, L"%d\n",DLNum);
	//	for(i=0;i<DLNum;i++)
	//	{
	//		fwprintf(fp, L"%d ",i);
	//		fwprintf(fp, L"%s ",DLArray[i].BGH);
	//		fwprintf(fp, L"%lf ",DLArray[i].BLC);
	//		fwprintf(fp, L"%s ",DLArray[i].EGH);
	//		fwprintf(fp, L"%lf ",DLArray[i].ELC);
	//		fwprintf(fp, L"\n");
	//	}
	//	fclose(fp);
}

void JD_CENTER::DrawSEML(int zybz, int textdir,double texth,int xsw)
{
	AcGePoint3d ptb, pte, TextPt;
	int nch = 0;
	double B = 0.0;
	double b = 0.0;
	xlpoint PZtmp;
	ACHAR SCkml[40], ECkml[40];
	CString CkmlStr;
	PZtmp.lc = XYArray[0][4];
	xlpoint_pz(&PZtmp);
	CkmlStr = LCchr(DLArray[0].EGH, DLArray[0].ELC, xsw);
	_stprintf(SCkml, L"道路改移起点%s", CkmlStr);
	nch = _tcslen(SCkml);
	B = PZtmp.a;
	b = B + pi * 0.5;
	b = pi * 0.5 - b;
	if (zybz == -1) b = b + pi;
	if (b < 0.0) b = 2 * pi + b;
	if (b > 2 * pi) b = b - 2 * pi;
	if (textdir < 0)b += pi;
	ptb[X] = PZtmp.y;
	ptb[Y] = PZtmp.x;
	pte[X] = PZtmp.y + nch * texth * sin(B + pi * 0.5 * zybz);
	pte[Y] = PZtmp.x + nch * texth * cos(B + pi * 0.5 * zybz);
	TextPt[X] = PZtmp.y + 2 * texth * sin(B + pi * 0.5 * zybz);
	TextPt[Y] = PZtmp.x + 2 * texth * cos(B + pi * 0.5 * zybz);
	TextPt[X] = TextPt[X] + 1.0 * sin(B + pi);
	TextPt[Y] = TextPt[Y] + 1.0 * cos(B + pi);
	maketext(TextPt, SCkml, b, texth, 1, 1, L"宋体", L"里程");
	makeline(ptb, pte, 1, 0.0, L"里程");

	PZtmp.lc = XLLength;
	xlpoint_pz(&PZtmp);
	CkmlStr = LCchr(DLArray[DLArray.GetSize() - 1].BGH, DLArray[DLArray.GetSize() - 1].BLC, xsw);
	_stprintf(ECkml, L"道路改移终点%s", CkmlStr);
	nch = _tcslen(ECkml);
	B = PZtmp.a;
	b = B + pi * 0.5;
	b = pi * 0.5 - b;
	if (zybz == -1) b = b + pi;
	if (b < 0.0) b = 2 * pi + b;
	if (b > 2 * pi) b = b - 2 * pi;
	if (textdir < 0)b += pi;
	ptb[X] = PZtmp.y;
	ptb[Y] = PZtmp.x;
	pte[X] = PZtmp.y + nch * texth * sin(B + pi * 0.5 * zybz);
	pte[Y] = PZtmp.x + nch * texth * cos(B + pi * 0.5 * zybz);
	TextPt[X] = PZtmp.y + 2 * texth * sin(B + pi * 0.5 * zybz);
	TextPt[Y] = PZtmp.x + 2 * texth * cos(B + pi * 0.5 * zybz);
	TextPt[X] = TextPt[X] + 1.2 * texth * sin(B);
	TextPt[Y] = TextPt[Y] + 1.2 * texth * cos(B);
	maketext(TextPt, ECkml, b, texth, 1, 1, L"宋体", L"里程");
	makeline(ptb, pte, 1, 0.0, L"里程");

}

void JD_CENTER::BZLC(double Scml,double Ecml,double dlc,int zybz,int type,int textdir,double texth,int xsw,double factor,int inf)//字头朝向为-1时倒向  
	//  标注公里百米里程标   inf=0;绘线位图 inf=1 平面图
	//    zybz=1,-1  标在右边，左边
{
	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,BZlc,lc,b,LCB,DDlength,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[20],chr[50],ch[50];
	xlpoint  PZtmp; 
	int nch=0;
	CString tmp;
	double endXLXCLC=XLC(XLLength,tmp);
	AcDbObjectId eId;
	//	   NLCB=0;
	BOOL ESMB_YorN = TRUE;
	_tcscpy(GH,DLArray[0].EGH);
	LCB=DLArray[0].ELC;
	WB=1.0*SCALE;//1.5*texth;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// 断链处统一里程
	DDlength=LCB;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"平面图-里程");
	// 逐个断链段标注
	for (iDL=0;iDL<DLNum-1;iDL++)
	{		
		DL=DLArray[iDL].ELC-DLArray[iDL].BLC;
		LCB= DLArray[iDL].ELC;  //本段起点里程 
		_tcscpy(GH,DLArray[iDL].EGH);
		_tcscpy(GH1,DLArray[iDL].BGH);

		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< DLNum-1)  // 不为起 终点
		{
			// 绘出 iDL点
			lc=DDlength;
			//////////////////////////////
			PZtmp.lc=lc;  // 统一里程
			k=xlpoint_pz(&PZtmp);  
			if (k>0&& k<=6)  //有解
			{

				ACHAR BLCChr[256],ELCChr[256];
				_tcscpy(ch, L"\0");
				_tcscpy(ch,LCchr(GH1,DLArray[iDL].BLC,xsw));  // 断前里程
				_tcscpy(chr, L"  ");
				_tcscat(chr,ch); 
				_tcscpy(BLCChr,chr);
				if(textdir<0)b+=pi;

				_tcscpy(ch, L"\0");   _tcscpy(ch,LCchr(GH,LCB,xsw));     // 断后里程
				_tcscpy(chr, L" =");  _tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   
				if (fabs( DL)>0.001)
				{
					if ( DL>0.0)   
					{ 
						_tcscat(chr, L" 短链 "); 
						RToS( DL,NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr, L" m");  
					}
					if ( DL<0.0)
					{ 
						_tcscat(chr, L" 长链 "); 
						RToS(fabs(DL),NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr, L" m");
					}
				}
				_tcscpy(ELCChr,chr);

				nch=_tcslen(chr);
				PZtmp.lc=lc;  // 统一里程
				k=xlpoint_pz(&PZtmp);  
				B=PZtmp.a;		  

				//断链直线
				ptb[X]=PZtmp.y;
				ptb[Y]=PZtmp.x; 
				pte[X]=PZtmp.y+nch*texth*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+nch*texth*cos(B+pi*0.5*zybz);

				AcGePoint3d TextBPt,TextEPt;
				PZtmp.lc=lc+zybz*0.8*texth;
				k=xlpoint_pz(&PZtmp); 
				B=PZtmp.a;
				TextBPt[X]=PZtmp.y+(0.5*nch*texth)*sin(B+pi*0.5*zybz) ;
				TextBPt[Y]=PZtmp.x+(0.5*nch*texth)*cos(B+pi*0.5*zybz) ;

				PZtmp.lc=lc-zybz*0.8*texth;
				k=xlpoint_pz(&PZtmp);
				B=PZtmp.a;
				TextEPt[X]=PZtmp.y+(0.5*nch*texth)*sin(B+pi*0.5*zybz) ;
				TextEPt[Y]=PZtmp.x+(0.5*nch*texth)*cos(B+pi*0.5*zybz) ;

				b=B+pi*0.5;
				b=pi*0.5-b; 
				if (zybz==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;
				if (b>2*pi) b=b-2*pi;
				if(textdir<0)b+=pi;

				if(lc>Scml-0.01 && lc <Ecml+0.01)
				{
					maketext(TextBPt,BLCChr,b,texth,-1,3, L"宋体", LayerName);
					maketext(TextEPt,ELCChr,b,texth,-1,3, L"宋体", LayerName);
					makeline(ptb,pte,-1,0.0, LayerName);
				}
			}
		}
		zlc=(long int)(LCB/dlc+0.0001);
		BZlc=zlc*dlc;  // 实际里程 
		if ((BZlc-LCB)<0.0001)  {BZlc=BZlc+dlc;}

		_tcscpy(GH,DLArray[iDL].EGH);  //冠号
		AcDbBlockReference *pBlkRef=NULL;
		AcDbObjectId pBlockId;
		ACHAR name[256];
		_stprintf(name, L"%s\\dwg\\公里标.dwg",SysPath);
		pBlockId=BAS_DRAW_FUN::CreateBlockFromDwgFile(name, L"公里标");	

		BZW = 99;
		if(BZlc>=DLArray[iDL+1].BLC)BZW=-99;	
		while (BZW>0)
		{
			// 统一里程
			//pLCB[NLCB].lcflag=100;
			lc=DDlength+BZlc-LCB; 
			PZtmp.lc=lc;  // 统一里程
			//pLCB[NLCB].lc=lc;			 
			k=xlpoint_pz(&PZtmp);   
			//pLCB[NLCB].x=PZtmp.y;
			//pLCB[NLCB].y=PZtmp.x;
			BZW=k;    //  K<0  无解
			if (k>0&& k<=6)  //有解
			{
				// ads_printf(L"\n i=%d %10.3f %10.3f  %10.3f %10.3f ",iDL,PZtmp.lc, BZlc,LCB,DLArray[iDL+1].BLC );  
				B=PZtmp.a;
				pt[X]=PZtmp.y+0.4*texth*sin(B+pi*0.5*zybz);
				pt[Y]=PZtmp.x+0.4*texth*cos(B+pi*0.5*zybz);
				pte[X]=PZtmp.y+(WB)*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+(WB)*cos(B+pi*0.5*zybz);
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 

				if(lc>Scml-0.01 && lc <Ecml+0.01)
					makeline(ptb,pt,-1,0.0, LayerName);

				pt=pte;
				b=B+pi*0.5;
				b=pi*0.5-b; 
				if (zybz==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;
				if (b>2*pi) b=b-2*pi;
				// ads_printf(L"\n %10.3f %10.3f %10.3f %10.4f",PZtmp.lc,PZtmp.x,PZtmp.y,dms_rad(b));

				ads_rtos(BZlc,2,0,chr);  
				// _tcscpy(ch,GH); _tcscat(ch,chr);
				// 里程百米标
				ii=_tcslen(chr);if(ii>20) ii=19;
				if (dlc>=100.0&&ii>=2)
				{
					chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // 公里标
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH); _tcscat(ch,chr); 
						//						acutPrintf(L"%s\n",ch);
					}//pLCB[NLCB].lcflag=1000;}
					else if(ii>3 && chr[ii-3]!='0')    // 百米标
					{ ch[0]=chr[ii-3];ch[1]='\0';};		    
				}
				else 
				{
					_tcscpy(ch,GH);
					if ( ii<=2)
					{
						_tcscpy(ch, L"+"); ch[1]='0';
						ch[2]=chr[ii-2]; ch[3]=chr[ii-1];ch[4]='\0';
						ESMB_YorN = FALSE;
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp, L"+"); chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2]; chrtmp[3]=chr[ii-1];chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
						{
							chr[ii-3]='\0';
							_tcscpy(ch,GH);
							_tcscat(ch,chr);
							_tcscat(ch,chrtmp); //pLCB[NLCB].lcflag=1000;
						}
						else if (ii>=3/*&&chr[ii-3]!='0'*/)
						{_tcscpy(ch,chrtmp);}

						//二十米标时标注整公里
						int LCSJ = BZlc;
						if(LCSJ % 1000 == 0)
							ESMB_YorN = true;
						else
							ESMB_YorN = false;
					}
				}
				if(type<=1)b=b-pi*0.5;
				int istr=_tcslen(ch);
				//				double dist=2*WB+0.5*0.8*istr*texth;//字符串中心到线路中心的距离
				//double dist=2*WB+0.5*texth;//字符串中心到线路中心的距离
				//	double dist=WB+0.9*texth;//字符串中心到线路中心的距离
				double dist=1.0*texth;//字符串中心到线路中心的距离
				pte[X]=PZtmp.y+dist*sin(B+pi*0.5*zybz);//+0.5*texth*sin(B);
				pte[Y]=PZtmp.x+dist*cos(B+pi*0.5*zybz);//+0.5*texth*cos(B);
				if(textdir<0)b+=pi;
				if(lc>Scml-0.01 && lc <Ecml+0.01)
				{	
					if(inf==0&&istr >=2 && ESMB_YorN == TRUE)
					{
						pte[X]=PZtmp.y+(dist+30*factor)*sin(B+pi*0.5*zybz);//+0.5*texth*sin(B);
						pte[Y]=PZtmp.x+(dist+30*factor)*cos(B+pi*0.5*zybz);//+0.5*texth*cos(B);

						if(pBlockId>0)
						{
							pBlkRef = new AcDbBlockReference;
							pBlkRef->setBlockTableRecord(pBlockId);						
							pt.x = PZtmp.y;pt.y = PZtmp.x;
							pBlkRef->setScaleFactors(AcGeScale3d(factor,factor,1));
							pBlkRef->setPosition(pt);
							pBlkRef->setRotation(b);
							pBlkRef->setLayer(LayerName);
							AddEntityToDbs(pBlkRef);
						}
					}
					maketext(pte,ch,b,texth,-1,3, L"宋体", LayerName);
				}

			}
			BZlc=BZlc+dlc;
			if(BZlc>=DLArray[iDL+1].BLC)BZW=-99;			
		}
		DDlength=DDlength+DLArray[iDL+1].BLC-LCB;//断链处统一里程。} // IDL		
	}	
}

void JD_CENTER::BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,int textdir)//字头朝向为-1时倒向  
	//  标注公里百米里程标  
	//    zybz=1,-1  标在右边，左边
{
	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,BZlc,lc,b,LCB,DDlength,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[20],chr[50],ch[50];
	xlpoint  PZtmp; 
	int nch=0;
	CString tmp;
	double endXLXCLC=fabs(XLC(XLLength,tmp));
	AcDbObjectId eId;
	//	   NLCB=0;

	_tcscpy(GH,DLArray[0].EGH);
	LCB=DLArray[0].ELC;
	WB=1.0*SCALE;//1.5*texth;

	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// 断链处统一里程
	DDlength=LCB;
	// 逐个断链段标注
	for (iDL=0;iDL<DLNum-1;iDL++)
	{		
		DL=DLArray[iDL].ELC-DLArray[iDL].BLC;
		LCB= DLArray[iDL].ELC;  //本段起点里程 
		_tcscpy(GH,DLArray[iDL].EGH);
		//AfxMessageBox(GH);
		_tcscpy(GH1,DLArray[iDL].BGH);

		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< DLNum-1)  // 不为起 终点
		{
			// 绘出 iDL点
			lc=DDlength;
			//////////////////////////////
			PZtmp.lc=lc;  // 统一里程
			k=xlpoint_pz(&PZtmp);  
			if (k>0&& k<=6)  //有解
			{

				ACHAR BLCChr[256],ELCChr[256];
				_tcscpy(ch, L"\0");
				_tcscpy(ch,LCchr(GH1,DLArray[iDL].BLC,NLC));  // 断前里程
				_tcscpy(chr, L"  ");
				_tcscat(chr,ch); 
				_tcscpy(BLCChr,chr);
				if(textdir<0)b+=pi;

				_tcscpy(ch, L"\0");   _tcscpy(ch,LCchr(GH,LCB,NLC));     // 断后里程
				_tcscpy(chr, L" =");  _tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   
				if (fabs( DL)>0.001)
				{
					if ( DL>0.0)   
					{ 
						_tcscat(chr, L" 短链 "); 
						RToS( DL,NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr, L" m");  
					}
					if ( DL<0.0)
					{ 
						_tcscat(chr, L" 长链 "); 
						RToS(fabs(DL),NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr, L" m");
					}
				}
				_tcscpy(ELCChr,chr);

				nch=_tcslen(chr);
				PZtmp.lc=lc;  // 统一里程
				k=xlpoint_pz(&PZtmp);  
				B=PZtmp.a;		  

				//断链直线
				ptb[X]=PZtmp.y;
				ptb[Y]=PZtmp.x; 
				pte[X]=PZtmp.y+nch*texth*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+nch*texth*cos(B+pi*0.5*zybz);

				AcGePoint3d TextBPt,TextEPt;
				PZtmp.lc=lc+zybz*0.8*texth;
				k=xlpoint_pz(&PZtmp); 
				B=PZtmp.a;
				TextBPt[X]=PZtmp.y+(0.5*nch*texth)*sin(B+pi*0.5*zybz) ;
				TextBPt[Y]=PZtmp.x+(0.5*nch*texth)*cos(B+pi*0.5*zybz) ;

				PZtmp.lc=lc-zybz*0.8*texth;
				k=xlpoint_pz(&PZtmp);
				B=PZtmp.a;
				TextEPt[X]=PZtmp.y+(0.5*nch*texth)*sin(B+pi*0.5*zybz) ;
				TextEPt[Y]=PZtmp.x+(0.5*nch*texth)*cos(B+pi*0.5*zybz) ;

				b=B+pi*0.5;
				b=pi*0.5-b; 
				if (zybz==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;
				if (b>2*pi) b=b-2*pi;
				if(textdir<0)b+=pi;

				//				if((PZtmp.lc+0.01)>=sLC1x&&(PZtmp.lc-0.01)<=eLC1x)
				if(pWd==NULL)maketext(TextBPt,BLCChr,b,texth,-1,3, L"宋体", L"里程");  
				else G_maketext(pWd,TextBPt,BLCChr,b,texth,BaseSettting::DLBTextColor,1);

				//				if((PZtmp.lc+0.01)>=sLC1x&&(PZtmp.lc-0.01)<=eLC1x)

				if(pWd==NULL)maketext(TextEPt,ELCChr,b,texth,-1,3, L"宋体", L"里程");
				else G_maketext(pWd,TextEPt,ELCChr,b,texth,BaseSettting::DLBTextColor,1);

				//				if((PZtmp.lc+0.01)>=sLC1x&&(PZtmp.lc-0.01)<=eLC1x)
				{
					if(pWd==NULL)makeline(ptb,pte,-1,0.0, L"里程");
					else G_makeline(pWd,ptb,pte,BaseSettting::DLBLineColor);
				}

			}
		}
		zlc=(long int)(LCB/dlc+0.0001);
		BZlc=zlc*dlc;  // 实际里程 
		if ((BZlc-LCB)<0.0001)  {BZlc=BZlc+dlc;}
		BZW = 99;
		_tcscpy(GH,DLArray[iDL].EGH);  //冠号
		if(BZlc>=DLArray[iDL+1].BLC)BZW=-99;	
		while (BZW>0)
		{
			// 统一里程
			//pLCB[NLCB].lcflag=100;
			lc=DDlength+BZlc-LCB; 
			PZtmp.lc=lc;  // 统一里程
			//pLCB[NLCB].lc=lc;			 
			k=xlpoint_pz(&PZtmp);   
			//pLCB[NLCB].x=PZtmp.y;
			//pLCB[NLCB].y=PZtmp.x;
			BZW=k;    //  K<0  无解

			if (k>0&& k<=6)  //有解
			{
				// ads_printf(L"\n i=%d %10.3f %10.3f  %10.3f %10.3f ",iDL,PZtmp.lc, BZlc,LCB,DLArray[iDL+1].BLC );  
				B=PZtmp.a;
				pt[X]=PZtmp.y+0.4*texth*sin(B+pi*0.5*zybz);
				pt[Y]=PZtmp.x+0.4*texth*cos(B+pi*0.5*zybz);
				pte[X]=PZtmp.y+(WB)*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+(WB)*cos(B+pi*0.5*zybz);
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 

				//				if((PZtmp.lc+0.01)>=sLC1x&&(PZtmp.lc-0.01)<=eLC1x)
				if(pWd==NULL)makeline(ptb,pt,-1,0.0, L"里程");
				else G_makeline(pWd,ptb,pt,BaseSettting::LCLineColor);

				pt=pte;
				b=B+pi*0.5;
				b=pi*0.5-b; 
				if (zybz==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;
				if (b>2*pi) b=b-2*pi;
				// ads_printf(L"\n %10.3f %10.3f %10.3f %10.4f",PZtmp.lc,PZtmp.x,PZtmp.y,dms_rad(b));

				ads_rtos(BZlc,2,0,chr);  
				// _tcscpy(ch,GH); _tcscat(ch,chr);
				// 里程百米标
				ii=_tcslen(chr);if(ii>20) ii=19;
				if (dlc>=100.0&&ii>=2)
				{
					chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // 公里标
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH); _tcscat(ch,chr); 
						//						acutPrintf(L"%s\n",ch);
					}//pLCB[NLCB].lcflag=1000;}
					else if(ii>3 && chr[ii-3]!='0')    // 百米标
					{ ch[0]=chr[ii-3];ch[1]='\0';};		    
				}
				else 
				{
					_tcscpy(ch,GH);
					if ( ii<=2)
					{
						_tcscpy(ch, L"+"); ch[1]='0';
						ch[2]=chr[ii-2]; ch[3]=chr[ii-1];ch[4]='\0';
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp, L"+"); chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2]; chrtmp[3]=chr[ii-1];chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
						{
							chr[ii-3]='\0';
							_tcscpy(ch,GH);
							_tcscat(ch,chr);
							_tcscat(ch,chrtmp); //pLCB[NLCB].lcflag=1000;
						}
						else if (ii>3&&chr[ii-3]!='0')
						{_tcscpy(ch,chrtmp);}
					}
				}
				if(type<=1)b=b-pi*0.5;
				int istr=_tcslen(ch);
				//				double dist=2*WB+0.5*0.8*istr*texth;//字符串中心到线路中心的距离
				double dist=2*WB+0.5*texth;//字符串中心到线路中心的距离
				pte[X]=PZtmp.y+dist*sin(B+pi*0.5*zybz);//+0.5*texth*sin(B);
				pte[Y]=PZtmp.x+dist*cos(B+pi*0.5*zybz);//+0.5*texth*cos(B);
				if(textdir<0)b+=pi;
				//				if((PZtmp.lc+0.01)>=sLC1x&&(PZtmp.lc-0.01)<=eLC1x)
				if(pWd==NULL)maketext(pte,ch,b,texth,-1,3, L"宋体", L"里程");
				else G_maketext(pWd,pte,ch,b,texth,BaseSettting::LCTextColor,1);
			}
			BZlc=BZlc+dlc;
			if(BZlc>=DLArray[iDL+1].BLC)BZW=-99;			
		}
		DDlength=DDlength+DLArray[iDL+1].BLC-LCB;//断链处统一里程。} // IDL		
	}	
}

void JD_CENTER::setDLM(int iDL,double BLC,double ELC,CString BGH,CString EGH)
	//void JD_CENTER::setDLM(int iDL,double BLC,double ELC,char *BGH,char *EGH)
{  

	DLNum=DLArray.GetSize();
	if(iDL==0)//起点		
	{
		if(JdCenNum>0)
			JdCenArray[0].A1=ELC;
	}
	if (iDL>=0&& iDL<DLNum)
	{
		DLArray[iDL].BGH=BGH;
		DLArray[iDL].EGH=EGH;
		DLArray[iDL].BLC=BLC;
		DLArray[iDL].ELC=ELC;
	}
	GetDLTylc();
}

//  由断链数据表 求某现场里程的统一里程
double JD_CENTER::TYLC(double dml)
{   
	double TYLCB = 0, LCB, LCE, TLC, XLC;  // LCB 断链的统一里程
	int iDL = 0, bz;
	//起点里程
	CString GH;

	TLC = fabs(dml);
	XLC = TLC;
	bz = -99;
	for (iDL = 0; iDL < DLNum; iDL++)   // 
	{
		//该断链点统一里程
		TYLCB = DLArray[iDL].TYLC;

		//该断链后链里程
		LCB = DLArray[iDL].ELC;
		//下一断链的前链里程
		if (iDL < DLNum - 1)
			LCE = DLArray[iDL + 1].BLC;
		if ((iDL < DLNum - 1 && XLC >= LCB && XLC <= LCE) || (iDL == DLNum - 1 && XLC >= LCB))  // 落在该段落内
		{   
			if (bz < 0)
			{  
				TLC = TYLCB + XLC - LCB;
				bz = 1;
			}  // 第一次求到
			else 
			{ 
				bz = 2;
				//				ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
				//					TLC,TYLCB+XLC-LCB);  
				if (dml < 0.0)
					TLC = TYLCB + XLC - LCB;
			}
		}
	}
	return TLC;     
}

//  由断链数据表 求某现场里程的统一里程
/*
double JD_CENTER::TYLC(CString ckml)
{   
double TYLCB=0,LCB, LCE,TLC,XLC;  // LCB 断链的统一里程
int iDL=0,bz;
//起点里程
CString GH;
split_ckml(ckml,&XLC,GH);
TLC=XLC;
bz=-99;
for (iDL=0;iDL<DLNum;iDL++)   // 
{
//该断链点统一里程
TYLCB=DLArray[iDL].TYLC;

//该断链后链里程
LCB=DLArray[iDL].ELC;

//下一断链的前链里程
if(iDL<DLNum-1)LCE=DLArray[iDL+1].BLC; 
if( ( iDL<DLNum-1 && XLC>=LCB && XLC<=LCE ) || (iDL==DLNum-1 && XLC>=LCB) )  // 落在该段落内
{   
if( bz<0) {  TLC= TYLCB+XLC-LCB;  bz=1; }  // 第一次求到
else 
{ 
bz=2;
ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
TLC,TYLCB+XLC-LCB);  
if(_tcscmp(GH,DLArray[iDL].EGH)==0)TLC=TYLCB+XLC-LCB;
}
}
}
return TLC;     
}*/


int JD_CENTER::xlpoint_pz(struct xlpoint *PZ) 
	// 求某一统一里程的坐标,并返回线元类型
{ 
	return BAS_DRAW_FUN::xlpoint_pz(XYArray,XYNum,PZ);
	/*
	double l,dlc,lc;
	int i,j,k,bz=-99;
	double INDATA2[5],jslc;
	struct xlpoint PB0,PEZ,*PBI,*PEI;
	PB0.lc=XYArray[0][4];
	PB0.x=XYArray[0][1];
	PB0.y=XYArray[0][2];
	PB0.a=rad(XYArray[0][3]);
	PB0.r=1e40;
	dlc=XYArray[0][0];
	jslc=PZ->lc;
	PBI=&PB0;PEI=&PEZ; 
	i=0; bz=99;
	do
	{ 
	i=i+1; 
	if (i>=XYNum) bz=-99;
	if (bz>0)
	{
	// 第 i 线元
	for (k=0;k<5;k++) INDATA2[k]=XYArray[i][k];
	l=Get_INDATA_L(INDATA2,&j);
	if(i>=XYNum-1)
	l += 0.1;//最后一个线元加一个毫米
	PBI->a  = XYArray[i][5];
	PBI->lc = XYArray[i][6];
	PBI->r  = XYArray[i][7];
	PBI->x  = XYArray[i][8];
	PBI->y  = XYArray[i][9];
	// l 为该线元的长度 m

	if( ( jslc <= PBI->lc+l ) && (jslc>=PBI->lc) ) 
	// 计算点在 i 线元上
	{  lc=jslc-PBI->lc;
	if (lc<=0.0) lc=0.000000000001;
	xlzbxy(PBI,PZ,lc,INDATA2,0);
	k=999;
	bz=-99;
	};
	}  // bz>0
	}while (bz>0);

	if (k==999) return j; //有解 j 为 线元类型
	else
	{  	return (-9); // 无解
	};
	*/
}

//插入特征里程
void JD_CENTER::INSertLC(double cml)
{
	AcGePoint3d  ptb,pt,pte;
	double WB,b;
	int  KK=7,zybz=1;
	ACHAR chr[50];
	xlpoint  P0tmp,PZtmp;

	int textdir=1 ;
	//标注线长
	WB=10*texth;

	P0tmp.lc=cml; 
	xlpoint_pz(&P0tmp);
	ptb.x = P0tmp.y;
	ptb.y = P0tmp.x;

	zybz =-1;
	// 绘制方向	

	b=P0tmp.a+pi*0.5*zybz;
	pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);

	makeline(ptb,pte,-1,0.0, L"特征点位置");

	_tcscpy(chr,XLC(cml));
	b=P0tmp.a;
	if(textdir>0)
		b = P0tmp.a + pi;
	else 
		b = P0tmp.a;
	pt[X]=0.5*(ptb[X]+pte[X])+0.9*texth*sin(b);
	pt[Y]=0.5*(ptb[Y]+pte[Y])+0.9*texth*cos(b);	        
	b=P0tmp.a-pi*0.5;

	b=pi*0.5-b;

	if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi; 

	maketext(pt,chr,b,texth,-1,3, L"宋体", L"特征点位置");

}
//  标特征点里程
void JD_CENTER::BZTZD(double Scml,double Ecml,int inf,int textdir,double texth,int xsw)//-1，倒字头  //  标注里程
	// bz_zybz==0   按曲线偏向标注   bz_zybz=1,-1  标在右边，左边 inf=0;绘线位图 inf=1 平面图
{
	//设置参数
	AcGePoint3d  ptb,pt,pte,pt1;
	double WB,b,l,INDATA[5],LCB;
	int i, j,k , KK=7,zybz=1;
	ACHAR chr[50],ch[40],tch[6], GH[8],DGH[8],chAR[50],chtmp[40],chAR1[50];
	xlpoint  P0tmp,PZtmp,Ptmid;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"平面图-特征桩");

	_tcscpy(GH, DLArray[0].EGH);
	LCB = DLArray[0].ELC;

	//标注线长
	if(inf==0)
		WB = 10 * texth;
	else 
		WB = 0.4 * texth;

	P0tmp.lc = XYArray[0][4];
	P0tmp.x = XYArray[0][1];
	P0tmp.y = XYArray[0][2];
	P0tmp.a = (XYArray[0][3]);
	P0tmp.r = 1e40;

	//逐个线元操作
	for (i = 1; i <= XYNum; i++)
	{
		//获取线元信息
		P0tmp.lc = XYArray[i][6];
		P0tmp.x = XYArray[i][8];
		P0tmp.y = XYArray[i][9];
		P0tmp.a = XYArray[i][5];
		P0tmp.r = XYArray[i][7];
		for (k = 0; k < 5; k++)
			INDATA[k] = XYArray[i][k]; j = (int)(XYArray[i][0] + 0.01);

		//计算本线元的长度
		l = 0.0;
		if (j==1) //直线
		{
			l=INDATA[1]; 
			_tcscpy(tch, L"Z");
			_tcscpy(chAR, L"L=");
			ads_rtos(XYArray[i][1],2,xsw,chtmp);
			_tcscat(chAR,chtmp);;
		}
		if (j==2) 
		{
			l=INDATA[2]; 
			_tcscpy(tch, L"Y"); 
			_tcscpy(chAR, L"R=");
			ads_rtos(XYArray[i][1],2,xsw,chtmp);
			//RToS(,xsw,chtmp);
			_tcscat(chAR,chtmp);
		}
		//  求A=？
		if (j == 3)
			l = INDATA[1] * INDATA[1] / INDATA[2];
		if (j == 4)
			l = INDATA[1] * INDATA[1] / INDATA[2];
		if (j == 5)
			l = INDATA[1] * INDATA[1] * (INDATA[2] - INDATA[3]) / (INDATA[2] * INDATA[3]);
		if (j == 6)
			l = INDATA[1] * INDATA[1] * (INDATA[3] - INDATA[2]) / (INDATA[2] * INDATA[3]);

		if (j>=3)
		{
			_tcscpy(tch, L"H");
			_tcscpy(chAR, L"A=");
			ads_rtos(XYArray[i][1],2,xsw,chtmp);
			_tcscat(chAR,chtmp);
			_tcscpy(chAR1, L"Ls=");
			ads_rtos(l,2,xsw,chtmp);
			_tcscat(chAR1,chtmp);
		}

		ptb[X] = P0tmp.y;
		ptb[Y] = P0tmp.x;
		ptb[Z] = 0.0;
		pt[Z] = 0.0;
		pte[Z] = 0.0;
		xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // 某线元终点
		zybz = 0;

		// 绘制方向	
		if(inf==0)
		{
			j = (int)(XYArray[i][0] + 0.01);  // 类型 
			if (j > 1 && j <= 6)
				zybz = (int)(XYArray[i][4] + 0.01);
			if(j==1)
				zybz = (int)(XYArray[i - 1][4] + 0.01);
			if (zybz == 0 || zybz > 1 || zybz < -1)
				zybz = -1;
		}
		else 
			zybz = 1;

		if(l>0.01)
		{
			xlzbxy(&P0tmp, &Ptmid, 0.5*l, INDATA, 0);	//计算线元中点坐标			
			//标注A，R
			b = Ptmid.a;

			pt[X] = Ptmid.y + 1.25*texth*sin(b + zybz*0.5*pi);
			pt[Y] = Ptmid.x + 1.25*texth*cos(b + zybz*0.5*pi);
			pt1[X] = Ptmid.y + 1.25*texth*sin(b - zybz*0.5*pi);
			pt1[Y] = Ptmid.x + 1.25*texth*cos(b - zybz*0.5*pi);
			b = FwjtoAngX(b);
			if(textdir<0)
				b += pi;

			maketext(pt, chAR, b, texth, 1, 3, L"宋体", LayerName);


			if(j>=3)
			{
				maketext(pt1,chAR1,b,texth,1,3, L"宋体", LayerName);
			}
		}				

		b = P0tmp.a + pi*0.5*zybz;
		pte[X] = ptb[X] + WB*sin(b);
		pte[Y] = ptb[Y] + WB*cos(b);
		int ibflag = (int)(XYArray[i - 1][0] + 0.01);  // 类型 
		int ieflag = (int)(XYArray[i][0] + 0.01);  // 类型 

		//特征桩的直线？
		if(SCALE>=10&&ibflag>=3&&ieflag==2){}
		else if(SCALE>=10&&ibflag==2&&ieflag>=3){}
		else
		{
			if(XYArray[i][6] > Scml-0.01 && XYArray[i][6]<Ecml+0.01)
				makeline(ptb,pte,-1,0.0, LayerName);
		}

		ACHAR QXflag[50];
		if (i > 1 && i <= XYNum - 1)
		{
			j = (int)(XYArray[i - 1][0] + 0.01);
			if (j>=3) 
			{
				_tcscpy(ch, L" S"); 
				/*if (Chinese_Engbz_type==2)*/
				_tcscpy(ch, L" H"); 
			}
			if (j==1) 
			{ 
				_tcscpy(ch, L" T"); 
				/*if (Chinese_Engbz_type==2)*/
				_tcscpy(ch, L" Z"); 
			}
			if (j==2) 
			{ 
				_tcscpy(ch, L" C"); 
				/*if (Chinese_Engbz_type==2)*/
				_tcscpy(ch, L" Y"); 
			}
			_tcscpy(chr,ch);
			_tcscat(chr,tch);

			_tcscpy(QXflag,chr);

			if (_tcsncmp(chr, L" YY",3)==0) 
			{  
				_tcscpy(chr, L" GQ");   
			}
			if (_tcsncmp(chr, L" C.C.",5)==0) 
			{  
				_tcscpy(chr, L" P.C.C.");
			}
			if (_tcsncmp(chr, L" S.S.",5)==0&&(j==3||j==4) ) 
			{ 
				_tcscpy(chr, L" PRC");
			}
			if (_tcsncmp(chr, L" HH",3)==0&&(j==3||j==4))
			{
				//_tcscpy(chr, L" FGD");
				_tcscpy(chr, L" GQ");
			}
		}
		else if (i == 1)
			_tcscpy(chr, L"QD"); 
		else
			_tcscpy(chr, L"ZD"); 
		_tcscpy(DGH, chr);
		CString str;
		double lc=P0tmp.lc;
		lc = fabs(XLC(lc, str));
		_tcscpy(GH, str);
		lc = lc - (int)(lc / 1000 + 0.0001)*1000.0;
		_tcscpy(ch, L"\0");
		if (inf == 0)
		{
			RToS(lc,xsw,ch);
			_tcscat(chr, L" +");
			_tcscat(chr,ch);
			b=P0tmp.a;
			if(textdir>0)b=P0tmp.a+pi;
			else b=P0tmp.a;	
			pt[X]=0.5*(ptb[X]+pte[X])+0.9*texth*sin(b);
			pt[Y]=0.5*(ptb[Y]+pte[Y])+0.9*texth*cos(b);	        
			b=P0tmp.a-pi*0.5;
			//if(zybz==1)b+=zybz*pi;	
			b=pi*0.5-b;
		}
		else
		{	
			b=P0tmp.a+pi*0.5;
			if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi; 					
			pt[X]=ptb[X]+(WB+0.9*texth)*sin(b);
			pt[Y]=ptb[Y]+(WB+0.9*texth)*cos(b);	
			b=P0tmp.a+pi*0.5;
			b=pi-b;				
		}
		if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi; 
		if(SCALE>=10&&_tcscmp(QXflag, L" HY")==0){}
		else if(SCALE>=10&&_tcscmp(QXflag, L" YH")==0){}
		else
		{
			if(P0tmp.lc>Scml-0.01&&P0tmp.lc<Ecml+0.01)
			{
				if(textdir<0)b+=pi;
				maketext(pt,chr,b,texth,-1,3, L"宋体", LayerName);
			}
		}

		P0tmp=PZtmp;
	}
}

//  标特征点里程
void JD_CENTER::BZTZD(AcGiWorldDraw* pWd,int bz_zybz,int textdir)//-1，倒字头  //  标注里程
	// bz_zybz==0   按曲线偏向标注   bz_zybz=1,-1  标在右边，左边
{
	AcGePoint3d  ptb,pt,pte,pt1;
	double WB,b,l,INDATA[5],LCB,lc;
	int i, j,k , KK=7,zybz=1;
	ACHAR chr[50],ch[40],tch[6], GH[8],DGH[8],chAR[50],chtmp[40],lcchr[20],chAR1[50];
	xlpoint  P0tmp,PZtmp,Ptmid;
	zybz=bz_zybz;
	if( zybz<-1|| zybz>1)  zybz=-1;

	_tcscpy(GH,DLArray[0].EGH);
	LCB=DLArray[0].ELC;

	//标注线长
	P0tmp.lc=XYArray[0][4]; 
	P0tmp.x=XYArray[0][1];P0tmp.y=XYArray[0][2];
	P0tmp.a=(XYArray[0][3]);P0tmp.r=1e40;
	_tcscpy(chtmp,XLC(P0tmp.lc));
	WB=1.5*_tcslen(chtmp)*texth;
	//	int endJD=EJD;
	for (i=1;i<XYNum;i++)
	{
		zybz=1;		
		P0tmp.lc=XYArray[i][6]; 
		P0tmp.x=XYArray[i][8];
		P0tmp.y=XYArray[i][9];
		P0tmp.a=XYArray[i][5];
		P0tmp.r=XYArray[i][7];

		for (k=0;k<5;k++)INDATA[k]=XYArray[i][k];j=(int)(XYArray[i][0]+0.01);
		l=0.0;

		if (j>=3)
		{
			_tcscpy(tch, L"H");
			_tcscpy(chAR, L"A = ");
			ads_rtos(XYArray[i][1],2,NLC,chtmp);
			//RToS();
			_tcscat(chAR,chtmp);
		}
		if (j==1) //直线
		{
			l=INDATA[1]; 
			_tcscpy(tch, L"Z");
			_tcscpy(chAR, L"L=");
			ads_rtos(XYArray[i][1],2,NLC,chtmp);
			//RToS();
			_tcscat(chAR,chtmp);
			//			_stprintf(chAR, L"L=%0.3lf",XYArray[i][1]);
			//			_tcscpy(chAR, L"R = ");
			//			_tcscat(chAR, L"∞");
		}
		if (j==2) 
		{
			l=INDATA[2]; 
			_tcscpy(tch, L"Y"); 
			_tcscpy(chAR, L"R=");
			ads_rtos(XYArray[i][1],2,NLC,chtmp);
			//RToS(XYArray[i][1],NLC,chtmp);
			_tcscat(chAR,chtmp);
		}

		//  求A=？
		if (j==3)l=INDATA[1]*INDATA[1]/INDATA[2];
		if (j==4)l=INDATA[1]*INDATA[1]/INDATA[2];
		if (j==5)l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
		if (j==6)l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);
		if (j>=3)
		{
			_tcscpy(tch, L"H");
			_tcscpy(chAR, L"A=");
			ads_rtos(XYArray[i][1],2,3,chtmp);
			_tcscat(chAR,chtmp);

			_tcscpy(chAR1, L"Ls=");
			ads_rtos(l,2,3,chtmp);
			_tcscat(chAR1,chtmp);

		}

		ptb[X]=P0tmp.y;ptb[Y]=P0tmp.x; ptb[Z]=0.0;pt[Z]=0.0;pte[Z]=0.0;
		xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // 某线元终点
		if(l>0.01)
		{
			xlzbxy(&P0tmp,&Ptmid,0.5*l,INDATA,0); // 某线元中点

			//标注A，R
			b=Ptmid.a;

			pt[X]=Ptmid.y+1.25*texth*sin(b+zybz*0.5*pi);
			pt[Y]=Ptmid.x+1.25*texth*cos(b+zybz*0.5*pi);


			pt1[X]=Ptmid.y+1.25*texth*sin(b-zybz*0.5*pi);
			pt1[Y]=Ptmid.x+1.25*texth*cos(b-zybz*0.5*pi);

			b=FwjtoAngX(b);
			if(textdir<0)b+=pi;

			if(pWd==NULL)
			{
				//if(j==1 || j==2)maketext(pt,chAR,b,texth,-1,3, L"宋体", L"特征点位置");
			}
			else
			{
				G_maketext(pWd,pt,chAR,b,texth,BaseSettting::QXYSTextColor,1);
				if(j>=3)
				{

					G_maketext(pWd,pt1,chAR1,b,texth,BaseSettting::QXYSTextColor,1);
				}
			}
		}

		// 绘制方向		
		zybz=bz_zybz;
		if(zybz==0)
		{
			if (j>1&&j<=6)
			{
				if(XYArray[i][4]<0.0)zybz=-1;
				else zybz=1;
			}
		}

		if (bz_zybz==0) 
		{ 
			j=(int)(XYArray[i][0]+0.01);  // 类型 
			if (j>1&&j<=6)zybz=(int)(XYArray[i][4]+0.01);
			if(j==1)zybz=(int)(XYArray[i-1][4]+0.01);	 
			if(zybz==0||zybz>1||zybz<-1) zybz=-1;
		}

		b=P0tmp.a+pi*0.5*zybz;
		pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
		int ibflag=(int)(XYArray[i-1][0]+0.01);  // 类型 
		int ieflag=(int)(XYArray[i][0]+0.01);  // 类型 

		if(SCALE>=10&&ibflag>=3&&ieflag==2){}
		else if(SCALE>=10&&ibflag==2&&ieflag>=3){}
		else
		{
			//			if(L0>0.001&&(P0tmp.lc+0.01)>=sLC1x&&(P0tmp.lc-0.01)<=eLC1x)
			//	if(i>1)//不画起点
			{
				if(pWd==NULL)makeline(ptb,pte,-1,0.0, L"特征点位置");
				else G_makeline(pWd,ptb,pte,BaseSettting::TZZLineColor);
			}
		}		


		if(i==1)//起点
		{
			if(_tcscmp(DLArray[0].BGH,DLArray[0].EGH)!=0 || fabs(DLArray[0].ELC-DLArray[0].BLC)>0.01)
			{
				b=P0tmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=P0tmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				//G_maketext(pWd,pt,chr,b,texth,4,1);
				b=P0tmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makeline(pWd,ptb,pte,3);

				ACHAR BlcStr[80],ElcStr[80];
				_tcscpy(BlcStr, L"  ");
				_tcscat(BlcStr,LCchr(DLArray[0].BGH,DLArray[0].BLC,NLC));//起点断前里程
				_tcscpy(ElcStr, L" =QD ");
				_tcscat(ElcStr,LCchr(DLArray[0].EGH,DLArray[0].ELC,NLC));//起点断后里程

				b=P0tmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=P0tmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(BlcStr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(BlcStr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,BlcStr,b,texth,4,1);	

				b=P0tmp.a;
				pt[X]=ptb[X]+1.25*texth*sin(b); pt[Y]=ptb[Y]+1.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(ElcStr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(ElcStr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,ElcStr,b,texth,4,1);	

			}
			else
			{
				//				_tcscpy(chr, L" B.P"); if(Chinese_Engbz_type==2)
				_tcscpy(chr, L" QD");
				if (KK==7) 
				{
					lc=P0tmp.lc;     _tcscpy(ch,XLC(lc));
					//_tcscpy(ch, L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC));
					_tcscat(chr, L" ");_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"
					_tcscpy(lcchr,ch);                 // e.f lcchr="K10+123.45"
				};
				b=P0tmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=P0tmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
				}			 
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,chr,b,texth,4,0);			 

				b=P0tmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makeline(pWd,ptb,pte,3);
			}
		}
		else if (i>1 && i<=XYNum-1)
		{
			j=(int)(XYArray[i-1][0]+0.01);
			if (j>=3) { _tcscpy(ch, L" S"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch, L" H"); };
			if (j==1) { _tcscpy(ch, L" T"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch, L" Z"); };
			if (j==2) { _tcscpy(ch, L" C"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch, L" Y"); };
			_tcscpy(chr,ch);_tcscat(chr,tch);
			ACHAR QXflag[50];
			_tcscpy(QXflag,chr);

			if (_tcsncmp(chr, L" YY",3)==0) {  _tcscpy(chr, L" GQ");   };
			if (_tcsncmp(chr, L" C.C.",5)==0) {  _tcscpy(chr, L" P.C.C.");};
			if (_tcsncmp(chr, L" S.S.",5)==0&&(j==3||j==4) ) {  _tcscpy(chr, L" PRC");};
			if (_tcsncmp(chr, L" HH",3)==0&&(j==3||j==4))
			{
				//_tcscpy(chr, L" FGD");
				_tcscpy(chr, L" GQ");
			};
			_tcscpy(DGH,chr);
			CString str;
			double lc=P0tmp.lc;   lc=fabs(XLC(lc,str)); _tcscpy(GH,str);
			//	lc=lc-(int)(lc/1000+0.0001)*1000.0;
			//	_tcscpy(ch, L"\0");

			//	RToS(lc,2,ch);
			//			_tcscat(chr, L" +");_tcscat(chr,ch);
			_tcscpy(ch,XLC(P0tmp.lc));
			_tcscat(chr, L" ");_tcscat(chr,ch);

			b=P0tmp.a;
			if(textdir>0)b=P0tmp.a+pi;
			else b=P0tmp.a;

			//20191021新增	 重新计算长度
			double tempWB = _tcslen(chr) * texth;
			double TempAngle = P0tmp.a + pi * 0.5 * zybz;
			pte[X] = ptb[X] + tempWB * sin(TempAngle);
			pte[Y] = ptb[Y] + tempWB * cos(TempAngle);
			pt[X] = 0.5 * (ptb[X] + pte[X]) + 0.75 * texth * sin(b);
			pt[Y] = 0.5 * (ptb[Y] + pte[Y]) + 0.75 * texth * cos(b);
			b = P0tmp.a - pi * 0.5;
			//if(zybz==1)b+=zybz*pi;	
			b = pi * 0.5 - b;
			if (b < 0.0)
				b = 2 * pi + b;
			if (b > 2 * pi)
				b = b - 2 * pi;
			if(SCALE>=10&&_tcscmp(QXflag, L" HY")==0){}
			else if(SCALE>=10&&_tcscmp(QXflag, L" YH")==0){}
			else
			{
				//				if((P0tmp.lc+0.01)>=sLC1x&&(P0tmp.lc-0.01)<=eLC1x)
				{
					if(textdir<0)b+=pi;
					if(pWd==NULL)
						maketext(pt,chr,b,texth,-1,3, L"宋体", L"特征点位置");
					else 
						G_maketext(pWd,pt,chr,b,texth,BaseSettting::TZZTextColor,1);
				}
			}
		}
		if (i==XYNum-1) 
		{
			int nn=DLNum-1;
			if(_tcscmp(DLArray[nn].BGH,DLArray[nn].EGH)!=0 || fabs(DLArray[nn].ELC-DLArray[nn].BLC)>0.01)
			{
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; ptb[Z]=0.0;
				b=PZtmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				//G_maketext(pWd,pt,chr,b,texth,4,1);
				b=PZtmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makeline(pWd,ptb,pte,3);

				ACHAR BlcStr[80],ElcStr[80];
				_tcscpy(BlcStr, L" ZD ");
				_tcscat(BlcStr,LCchr(DLArray[nn].BGH,DLArray[nn].BLC,3));//起点断前里程
				_tcscpy(ElcStr, L" = ");
				_tcscat(ElcStr,LCchr(DLArray[nn].EGH,DLArray[nn].ELC,3));//起点断后里程

				b=PZtmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(BlcStr)-2)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(BlcStr)-2)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,BlcStr,b,texth,4,0);	

				b=PZtmp.a;
				pt[X]=ptb[X]+1.25*texth*sin(b); pt[Y]=ptb[Y]+1.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(ElcStr)-2)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(ElcStr)-2)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,ElcStr,b,texth,4,0);	

			}
			else
			{
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; ptb[Z]=0.0;
				//_tcscpy(chr, L" EP");if (Chinese_Engbz_type==2)
				_tcscpy(chr, L" ZD");
				if (KK==7) {
					lc=PZtmp.lc;   _tcscpy(ch,XLC(lc));
					//	_tcscpy(ch, L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
					_tcscat(chr, L" ");   _tcscat(chr,ch);
				};

				b=PZtmp.a;
				//pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
				//b=PZtmp.a+pi*0.5*zybz;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)-2)*texth*sin(b);
				pt[Y]=pt[Y]-(_tcslen(chr)-2)*texth*cos(b); }

				b=pi*0.5-b;if (b<0.0) b=2*pi+b;  if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,chr,b,texth,4,0);
				b=PZtmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makeline(pWd,ptb,pte,3);
				// ads_printf(L"\n i=%d,lc=%12.3f chr=%s",trow-1,lc, chr);
			}						
		}		
		P0tmp=PZtmp;
	}
}

double JD_CENTER::XLC1(double cml)
{
	double LCB = 0;  // LCB 断链的统一里程
	int iDL = 0;
	double dml = 0, tylc1;

	dml = cml;
	for (iDL = DLNum - 1; iDL >= 0; iDL--)
	{
		if (cml >= DLArray[iDL].TYLC)
		{
			//			GH=DLBTMP[iDL].EGH;
			dml = cml - DLArray[iDL].TYLC + DLArray[iDL].ELC;
			tylc1 = TYLC(dml);
			if (fabs(tylc1 - cml) < 0.001)
				return dml;
			else
				return -dml;
		}
	}
	return dml;
}
//  由断链数据表 求某统一里程的现场里程和冠号
double JD_CENTER::XLC( double cml,CString& GH)
{
	double LCB=0,tylc1;  // LCB 断链的统一里程
	int iDL=0;

	double dml = cml;
	GH = DLArray[0].EGH;
	for(iDL=DLNum-1;iDL>=0;iDL--)
	{
		if(cml>=DLArray[iDL].TYLC)
		{
			GH=DLArray[iDL].EGH;
			dml = cml-DLArray[iDL].TYLC+DLArray[iDL].ELC;
			tylc1 = TYLC(dml);
			if(fabs(tylc1-cml)<0.001)
				return dml;
			else
				return -dml;
		}
	}
	return dml;
	//
	//    double LCB=0, XCLC ;  // LCB 断链的统一里程
	//    int iDL=0,bz;
	//      //起点里程
	//    XCLC=TYLC;
	//    _tcscpy(GH,DLB[0].BGH);
	//  
	//	do {
	//	  bz=99;
	//	  //该断链点统一里程
	//	  if  (iDL==0) 
	//	  {
	//		  LCB=DLB[iDL].BLC+DLB[iDL].DL;  
	////		  acutPrintf(L"LCB=%lf\n",LCB);
	//	  }
	//	  else
	//	  {  LCB=LCB+DLB[iDL].BLC-(DLB[iDL-1].BLC+DLB[iDL-1].DL); 
	//	  }  
	//
	//	  if( fabs(TYLC-LCB)<0.001)
	//	  { 
	//		  XCLC=DLB[iDL].BLC+DLB[iDL].DL; 
	//	      _tcscpy(GH,DLB[iDL].EGH);
	//	      bz=99;	            
	//	  }
	//	  else if( (TYLC>LCB) &&(iDL<NDL))
	//	  {   XCLC=(TYLC-LCB)+DLB[iDL].BLC+DLB[iDL].DL; 
	//	      _tcscpy(GH,DLB[iDL].EGH);
	//	      iDL=iDL+1;
	//		  bz=-99;
	//	  }
	//	  else if (TYLC<=LCB)
	//	  {   XCLC=(TYLC-LCB)+DLB[iDL].BLC; 
	//	      _tcscpy(GH,DLB[iDL].BGH);
	//	      bz=99;
	//	  }	
	//	} while (bz<0);
	//
	//    CString str;
	//	str.Format(L"%0.3lf",XCLC);
	//	XCLC=_wtof(str);
	//    return XCLC;


}

CString JD_CENTER::XLC(double TYLC)
{
	CString GH;
	double xlc=fabs(XLC(TYLC,GH));
	return LCchr(GH,xlc,NLC);
}

Acad::ErrorStatus JD_CENTER::explode(AcDbVoidPtrArray& entitySet)
{
	AcDbVoidPtrArray entset;
	//ads_printf(L"in\n");
	if(IfCalXY&&JdCenNum>=2)
	{

		CalXYArray();
	}
	else
	{
		if(XYNum>0)
		{
			int k,j;
			double l,INDATA2[5];
			XYArray[0][4]=XYArray[1][6]=DLArray[0].ELC;
			for(int i=1;i<XYNum;i++)
			{
				for (k=0;k<5;k++) INDATA2[k]=XYArray[i][k];
				l=Get_INDATA_L(INDATA2,&j);
				XYArray[i+1][6]= XYArray[i][6]+l;
			}
		}
	}
	//ads_printf(L"2\n");
	if (XYNum>0) 
	{
		XYArray[0][4]=DLArray[0].ELC;
		LenOfLine = XYArray[XYNum][6] - XYArray[0][4];//线路长度 
		//	OutData();
		CString TGH;			
		double endXlc=fabs(XLC(XYArray[XYNum][6],TGH));	
		if(DLNum>0)
			setDLM(DLNum-1,endXlc,endXlc,DLArray[DLNum-1].BGH,DLArray[DLNum-1].EGH);

		Draw(NULL);
	}
	entitySet=entset;
	//ads_printf(L"out\n");
	return Acad::eOk;
}


Acad::ErrorStatus JD_CENTER::subGetOsnapPoints(
	AcDb::OsnapMode   osnapMode,
	int               gsSelectionMark,
	const AcGePoint3d&    pickPoint,
	const AcGePoint3d&    lastPoint,
	const AcGeMatrix3d&   viewXform,
	AcGePoint3dArray& snapPoints,

	AcDbIntArray&     geomIds) const 
{
	assertReadEnabled();
	Acad::ErrorStatus es=Acad::eOk;
	if (gsSelectionMark==0) return Acad::eOk;
	if (osnapMode!=AcDb::kOsModeEnd &&osnapMode!=AcDb::kOsModeNear
		&& osnapMode != AcDb::kOsModeMid
		&&osnapMode!=AcDb::kOsModePerp &&osnapMode!=AcDb::kOsModeCen
		&&osnapMode!=AcDb::kOsModeIns  )
	{
		return Acad::eOk;  
	}

	return Acad::eOk;
}


// Transform points and create a polyline out of them.
//
Acad::ErrorStatus  JD_CENTER::subGetTransformedCopy(
	const AcGeMatrix3d& mat,
	AcDbEntity*& ent) const
{
	assertReadEnabled();
	Acad::ErrorStatus es = Acad::eOk;
	return es;
}

Acad::ErrorStatus  JD_CENTER::subTranformBy(const AcGeMatrix3d &)
{
	AfxMessageBox(L"不能移动!");
	return Acad::eOk;
}

Acad::ErrorStatus JD_CENTER::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();

	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our ow
	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
	{
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}
	//////////////////////////////////////////////////////////////////////////
	Adesk::Int16 Version=20040720;
	Adesk::Int16 ver;
	pFiler->readItem(&ver);

	if(ver==Version)
	{
		int i=0;
		pFiler->readItem(&JdCenNum);
		JdCenArray.RemoveAll();
		JdCenter *JdCenObj=new JdCenter[JdCenNum];

		for(i=0;i<JdCenNum;i++)
		{
			pFiler->readItem(&JdCenObj[i].JDXZ);
			pFiler->readItem(&JdCenObj[i].N);
			pFiler->readItem(&JdCenObj[i].E);
			pFiler->readItem(&JdCenObj[i].JDN);
			pFiler->readItem(&JdCenObj[i].JDE);
			pFiler->readItem(&JdCenObj[i].CenN);
			pFiler->readItem(&JdCenObj[i].CenE);
			pFiler->readItem(&JdCenObj[i].R);
			pFiler->readItem(&JdCenObj[i].A1);
			pFiler->readItem(&JdCenObj[i].A2);
			pFiler->readItem(&JdCenObj[i].PreFwj);
			pFiler->readItem(&JdCenObj[i].AftFwj);
			pFiler->readItem(&JdCenObj[i].LeftOrRight);
			pFiler->readItem(&JdCenObj[i].IsOrNotHuiTou);
			pFiler->readItem(&JdCenObj[i].LinkAfter);
		}

		SetJdCenterArray(JdCenObj,JdCenNum);
		delete []JdCenObj;

		pFiler->readItem(&DLNum);

		DLArray.RemoveAll();
		ACHAR *ch=NULL;
		DLBZ DlObj;

		for(i=0;i<DLNum;i++)
		{
			pFiler->readItem(&ch);

			DlObj.BGH=ch;

			pFiler->readItem(&ch);
			DlObj.EGH=ch;

			pFiler->readItem(&DlObj.BLC);

			pFiler->readItem(&DlObj.ELC);

			DLArray.Add(DlObj);
		}

		GetDLTylc();
		pFiler->readItem(&ch);
		_tcscpy(LCGH,ch);

		pFiler->readItem(&ch);
		_tcscpy(JDGH,ch);
		pFiler->readItem(&JD0);
		pFiler->readItem(&SCALE);
		pFiler->readItem(&texth);
		pFiler->readItem(&DLBlength);
		pFiler->readItem(&NLC);
		pFiler->readItem(&DrawLeftOrRight);
		pFiler->readBool(&SHOWJD);
		pFiler->readItem(&FLDNo);
		pFiler->readItem(&PreJD);
		pFiler->readItem(&Ls1);
		pFiler->readItem(&ch);
		_tcscpy(RoadName,ch);
		pFiler->readItem(&ch);
		_tcscpy(mdbname,ch);
		pFiler->readBool(&IfCalXY);
		pFiler->readBool(&IfDelJZX);
		pFiler->readBool(&m_IsJYX);

		pFiler->readItem(&ch);
		m_RelRailName = ch;
		pFiler->readItem(&m_NCM);
		for (i=0; i<5*m_NCM; i++)
		{
			pFiler->readItem(&m_CM[i]);
		}
	}
	else
	{
		JdCenNum=0;
		XYNum=0;
		DLNum=0;
		AfxMessageBox(L"版本不正确!");
	}
	//////////////////////////////////////////////////////////////////////////

	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus JD_CENTER::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
	{
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	}
	//////////////////////////////////////////////////////////////////////////
	Adesk::Int16 Version=20040720;
	pFiler->writeItem(Version);	
	int i=0;
	pFiler->writeItem(JdCenNum);
	for(i=0;i<JdCenNum;i++)
	{
		JdCenter JdCenObj=JdCenArray[i];
		pFiler->writeItem(JdCenObj.JDXZ);
		pFiler->writeItem(JdCenObj.N);
		pFiler->writeItem(JdCenObj.E);
		pFiler->writeItem(JdCenObj.JDN);
		pFiler->writeItem(JdCenObj.JDE);
		pFiler->writeItem(JdCenObj.CenN);
		pFiler->writeItem(JdCenObj.CenE);
		pFiler->writeItem(JdCenObj.R);
		pFiler->writeItem(JdCenObj.A1);
		pFiler->writeItem(JdCenObj.A2);
		pFiler->writeItem(JdCenObj.PreFwj);
		pFiler->writeItem(JdCenObj.AftFwj);
		pFiler->writeItem(JdCenObj.LeftOrRight);
		pFiler->writeItem(JdCenObj.IsOrNotHuiTou);
		pFiler->writeItem(JdCenObj.LinkAfter);
	}
	pFiler->writeItem(DLNum);
	const ACHAR *ch=NULL;
	for(i=0;i<DLNum;i++)
	{
		DLBZ DlObj=DLArray[i];

		ch=_tcsdup(DlObj.BGH);
		pFiler->writeItem(ch);

		ch=_tcsdup(DlObj.EGH);
		pFiler->writeItem(ch);

		pFiler->writeItem(DlObj.BLC);

		pFiler->writeItem(DlObj.ELC);
	}

	ch=_tcsdup(LCGH);
	pFiler->writeItem(ch);

	ch=_tcsdup(JDGH);
	pFiler->writeItem(ch);

	pFiler->writeItem(JD0);

	pFiler->writeItem(SCALE);
	pFiler->writeItem(texth);
	pFiler->writeItem(DLBlength);
	pFiler->writeItem(NLC);
	pFiler->writeItem(DrawLeftOrRight);
	pFiler->writeBool(SHOWJD);
	pFiler->writeItem(FLDNo);
	pFiler->writeItem(PreJD);
	pFiler->writeItem(Ls1);
	ch=_tcsdup(RoadName);
	pFiler->writeItem(ch);
	ch=_tcsdup(mdbname);
	pFiler->writeItem(ch);
	pFiler->writeBoolean(IfCalXY);
	pFiler->writeBoolean(IfDelJZX);
	pFiler->writeBoolean(m_IsJYX);

	ch=_tcsdup(m_RelRailName);
	pFiler->writeItem(ch);
	pFiler->writeItem(m_NCM);
	for (i=0; i<5*m_NCM; i++)
	{
		pFiler->writeItem(m_CM[i]);
	}
	//////////////////////////////////////////////////////////////////////////
	return pFiler->filerStatus();
}



// Files data in from a DXF file.
//
Acad::ErrorStatus JD_CENTER::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	const char *chr=NULL;
	Acad::ErrorStatus es;
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }
	//	AfxMessageBox(L"不能存取!");	
	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus JD_CENTER::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dxfOutFields(pFiler);
	//	AfxMessageBox(L"不能存取!");	
	return pFiler->filerStatus();
}


Acad::ErrorStatus JD_CENTER::subGetGripPoints(AcGePoint3dArray& gripPoints,
											  AcDbIntArray& osnapModes,
											  AcDbIntArray& geomIds)const
{
	assertReadEnabled();
	///////////////////////////////////////////
	int i=0,j;
	AcGePoint3d Pt;
	for(i=0;i<JdCenNum;i++)
	{
		Pt.set(JdCenArray[i].JDE,JdCenArray[i].JDN,0.0);
		gripPoints.append(Pt);
	}//交点为夹点
	//-------------------------------------------------------------
	for(i=0;i<JdCenNum;i++)
	{
		Pt.set(JdCenArray[i].CenE,JdCenArray[i].CenN,0.0);
		gripPoints.append(Pt);
	}//交点为夹点

	xlpoint PB,PZ;
	double INDATA[5],l;
	for(i=1;i<JdCenNum-1;i++)
	{
		int XYNo=JdCenArray[i].CirXYNo;//设置圆曲线
		if(XYNo>0 && XYNo<XYNum)
		{
			for(j=0;j<5;j++)INDATA[j]=XYArray[XYNo][j];
			PB.x=XYArray[XYNo][8]; //N
			PB.y=XYArray[XYNo][9]; // E
			PB.a=XYArray[XYNo][5];
			l=XYArray[XYNo][2];
			xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // 曲线中点
			Pt.set(PZ.y,PZ.x,0.0);
			gripPoints.append(Pt);
		}
		else
		{
			Pt.set(0.0,0.0,0.0);
			gripPoints.append(Pt);
		}
	}
	/////////////////////////////////////////////
	return Acad::eOk;
}

Acad::ErrorStatus JD_CENTER::subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset)
{ 	
	//	AfxMessageBox(L"没有夹点!");	
	assertWriteEnabled();
	///////////////////////////////////////
	int i=0,k,Num;

	JdCenter *CurJC,*PreJC,*AftJC;
	double dx,dy,dd,fwjOffset;
	Num=indices.length();
	double PRE=BaseSettting::JiaDianLMD;//灵敏度
	dx=PRE*offset[X];
	dy=PRE*offset[Y];
	dd=xyddaa(0.0,0.0,dy,dx,&fwjOffset);
	for(i=0;i<Num;i++)
	{
		k=indices[i];
		if(k<JdCenNum)
		{
			CurJC=&JdCenArray[k];
			if(k>0)PreJC=&JdCenArray[k-1];
			if(k<JdCenNum-1)AftJC=&JdCenArray[k+1];

			int JDTDBZ=BaseSettting::JDTDFS;
			//				JDTDBZ=1;
			if(JDTDBZ==0)//任意拖动
			{
				CurJC->JDN+=dy;
				CurJC->JDE+=dx;
			}
			else if(JDTDBZ==1)//沿切线滑动
			{
				double DD1,DD01,DD2,DD02,fb,fb1,fb2,d1,d2;
				if (k<=JdCenNum-2)
				{ 
					DD02=xyddaa(CurJC->JDN,CurJC->JDE,AftJC->JDN,AftJC->JDE,&fb2);
					DD2=xyddaa(CurJC->JDN+dy,CurJC->JDE+dx,AftJC->JDN,AftJC->JDE,&fb);
					//  由直线段(大地坐标)[(XB,YB),(XE,YE)]的长度和方位角  返回:距离， *方位角 
					d2=DD02-DD2;
				} 
				else {  d2=0.0; fb2=0.0; }
				if (k>0)
				{
					DD01=xyddaa(PreJC->JDN,PreJC->JDE,CurJC->JDN,CurJC->JDE,&fb1);
					DD1=xyddaa(PreJC->JDN,PreJC->JDE,CurJC->JDN+dy,CurJC->JDE+dx,&fb);
					//  由直线段(大地坐标)[(XB,YB),(XE,YE)]的长度和方位角  返回:距离， *方位角 
					d1=DD1-DD01;
				}
				else {  d1=0.0; fb1=0.0; }
				dd=d1; fb=fb1;
				if (fabs(d1)<fabs(d2)) { dd=d2; fb=fb2;};
				//E
				CurJC->JDE+=dd*sin(fb);
				//N
				CurJC->JDN+=dd*cos(fb);
			}
			else if(JDTDBZ==2 || JDTDBZ==3 || JDTDBZ==4)
			{
				double OutN,OutE;
				OutN=CurJC->JDN+dy;
				OutE=CurJC->JDE+dx;

				if (JDTDBZ==2 || JDTDBZ==4) // 切前圆
				{
					if(k>1 && PreJC->JDXZ==IsJiaoDian && PreJC->LinkAfter!=Luan_Link)//不是起点
					{
						double Lo2,R1,p1,tR1,A2;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

						R1=PreJC->R;
						A2=PreJC->A2;
						if(A2>0.0)
						{
							Lo2=A2*A2/R1;
							double x=spiral_y(A2,Lo2);
							double t=Lo2/(2.0*R1);
							double dr=x+R1*sin(t+0.5*PI);			
							p1=dr-R1;
						}
						else p1=0.0;					
						//半径+内移量					
						tR1=R1+p1;
						double N1,E1,N0,E0;
						N1=JdCenArray[k-2].JDN;
						E1=JdCenArray[k-2].JDE;
						N0=PreJC->JDN;
						E0=PreJC->JDE;
						//求前圆圆心
						double CenN,CenE;
						CenN=PreJC->CenN;
						CenE=PreJC->CenE;
						double TN,TE;//切点
						//求切点
						int LRFlag=PreJC->LeftOrRight;
						GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
						BAS_DRAW_FUN::INTERS(&PreJC->N,&PreJC->E,N1,E1,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				if (JDTDBZ==3 || JDTDBZ==4)  // 切后圆
				{
					if(k<JdCenNum-2 && CurJC->LinkAfter!=Luan_Link)//不是终点
					{
						double Lo1,R2,p2,tR2,A1;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

						R2=AftJC->R;
						A1=AftJC->A1;
						if(A1>0.0)
						{
							Lo1=A1*A1/R2;
							double x=spiral_y(A1,Lo1);
							double t=Lo1/(2.0*R2);
							double dr=x+R2*sin(t+0.5*PI);			
							p2=dr-R2;
						}
						else p2=0.0;					
						//半径+内移量					
						tR2=R2+p2;
						double N0,E0,N2,E2;
						N0=JdCenArray[k+1].N;
						E0=JdCenArray[k+1].E;
						N2=JdCenArray[k+2].N;
						E2=JdCenArray[k+2].E;
						//求前圆圆心
						double CenN,CenE;
						CenN=AftJC->CenN;
						CenE=AftJC->CenE;
						//acutPrintf(L"CenE:%lf,%lf\n",CenE,CenN);
						double TN,TE;//切点
						//求切点
						int LRFlag=AftJC->LeftOrRight;
						GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
						BAS_DRAW_FUN::INTERS(&AftJC->N,&AftJC->E,N2,E2,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				//(JDTDBZ==4)// 切前、后两圆
				//当前交点新位置
				CurJC->JDN+=dy;
				CurJC->JDE+=dx;
			}

			if(CurJC->JDXZ==IsJiaoDian)//交点
			{
				CurJC->N=CurJC->JDN;
				CurJC->E=CurJC->JDE;
			}
			else//坐标性质为圆心,交点的变化应反算圆心
			{
				//将性质改为交点，代入calxyarray计算出新的圆心坐标,再改回来
				/*
				CurJC->JDXZ=IsJiaoDian;
				CurJC->N=CurJC->JDN;
				CurJC->E=CurJC->JDE;
				CalXYArray();
				CurJC->JDXZ=IsCenter;*/

				CurJC->CenN+=dy;
				CurJC->CenE+=dx;
				CurJC->N=CurJC->CenN;
				CurJC->E=CurJC->CenE;

			}

		}
		else if(k<2*JdCenNum)//圆心
		{
			int JdNo=k-JdCenNum;
			CurJC=&JdCenArray[JdNo];

			CurJC->CenN+=dy;
			CurJC->CenE+=dx;
			if(CurJC->JDXZ==IsCenter)//圆心
			{
				CurJC->N=CurJC->CenN;
				CurJC->E=CurJC->CenE;
			}
			else//坐标性质为交点,圆心的变化应反算交点
			{
				//将性质改为交点，代入calxyarray计算出新的圆心坐标,再改回来
				/*
				CurJC->JDXZ=IsCenter;
				CurJC->N=CurJC->CenN;
				CurJC->E=CurJC->CenE;
				CalXYArray();
				CurJC->JDXZ=IsJiaoDian;*/
				CurJC->JDN+=dy;
				CurJC->JDE+=dx;
				CurJC->N=CurJC->JDN;
				CurJC->E=CurJC->JDE;
				//			ads_printf(L"ht=%d\n",CurJC->IsOrNotHuiTou);				
			}
		}
		else
		{
			int JdNo=k-2*JdCenNum+1;
			double R;
			CurJC=&JdCenArray[JdNo];
			/*
			double FwjJdToCen;
			xyddaa(CurJC->JDN,CurJC->JDE,CurJC->CenN,CurJC->CenE,&FwjJdToCen);
			double t;
			xyddaa(0.0,0.0,dy,dx,&t);
			if(fabs(FwjJdToCen-t)>0.5*PI)CurJC->R+=dd;
			else CurJC->R-=dd;*/
			dd=sqrt(dx*dx+dy*dy);
			if(dd>0.001)
			{
				double xt,yt;
				int track,type ;
				track = 1;
				type = 5;
				struct resbuf *result ;
				result = ads_buildlist(RTSTR, L"",0);
				ads_grread(track,&type,result );
				xt = result->resval.rpoint[Y];
				yt = result->resval.rpoint[X];
				ads_relrb(result) ;
				double tmp;
				calptan(JdCenArray[JdNo-1].JDN,JdCenArray[JdNo-1].JDE,JdCenArray[JdNo].JDN,JdCenArray[JdNo].JDE,JdCenArray[JdNo+1].JDN,JdCenArray[JdNo+1].JDE,xt,yt,
					&tmp,&R,&tmp,&tmp,&tmp,&tmp,&tmp);
				if(BaseSettting::Rstep==0)//任意值
					CurJC->R = R;
				else
					CurJC->R = (int)(R/BaseSettting::Rstep+0.5)*BaseSettting::Rstep;


				if(CurJC->JDXZ==IsCenter)//圆心
				{
					R = sqrt((xt-CurJC->CenN)*(xt-CurJC->CenN) + (yt-CurJC->CenE)*(yt-CurJC->CenE));
					if(BaseSettting::Rstep==0)//任意值
						CurJC->R = R;
					else
						CurJC->R = (int)(R/BaseSettting::Rstep+0.5)*BaseSettting::Rstep;


					/*
					CurJC->JDXZ=IsJiaoDian;
					CurJC->N=CurJC->JDN;
					CurJC->E=CurJC->JDE;
					CalXYArray();
					CurJC->JDXZ=IsCenter;
					CurJC->N=CurJC->CenN;
					CurJC->E=CurJC->CenE;*/

				}
				/*
				R = get_R(R,DV);
				JDarray[k][4] = R;
				JDarray[k][3] = get_l0(R,DV);
				JDarray[k][5] = get_l0(R,DV);

				double fb,fe;
				double ddb=xyddaa(JDarray[k-1][1],JDarray[k-1][2],JDarray[k][1],JDarray[k][2],&fb);
				double dde=xyddaa(JDarray[k][1],JDarray[k][2],JDarray[k+1][1],JDarray[k+1][2],&fe);
				double aa=fe-fb;
				if (aa<-pi) aa=aa+2*pi;
				if  (aa>pi) aa=aa-2*pi;
				aa=fabs(aa);
				aa=dms_rad(aa);
				double BPMTL[10];
				getBPMTL(aa,JDarray[k][4],JDarray[k][3],JDarray[k][5],&BPMTL[0]);
				double T1=BPMTL[6];
				double T2=BPMTL[7];//L=BPMTL[8];

				ReadGuifanFile();
				double Lmin=mGuiFanCS.MinZhi;
				if(k==1)
				{
				if(T1>ddb)
				{
				JDarray[k-1][1]=JDarray[k][1]-(T1+Lmin)*cos(fb);
				JDarray[k-1][2]=JDarray[k][2]-(T1+Lmin)*sin(fb);
				}
				}
				if(k==NJD-2)
				{
				if(T2>dde)
				{
				JDarray[k+1][1]=JDarray[k][1]+(T2+Lmin)*cos(fe);
				JDarray[k+1][2]=JDarray[k][2]+(T2+Lmin)*sin(fe);
				}
				}*/

			}





		}

		/*		if(k<JdCenNum-2)
		{
		k=k+1;

		JD_CENTER *CurJdCen,*PreJdCen,*AftJdCen;

		CurJdCen=&JdCenArray[k];
		PreJdCen=&JdCenArray[k-1];
		AftJdCen=&JdCenArray[k+1];
		XYNo=CurJdCen->CirXYNo;
		if(XYNo<0 || XYNo >XYNum)continue;
		if(XYNo==1 || XYNo==XYNum-1)Rmax=999999999999999;
		else
		{
		PreDist=xyddaa(CurJdCen->N,CurJdCen->E,
		PreJdCen->N,PreJdCen->E,&tmpfwj);

		if(PreJdCen->JDXZ==IsJiaoDian)PreRmax=PreDist;
		else PreRmax=PreDist-PreJdCen->R;

		AftDist=xyddaa(CurJdCen->N,CurJdCen->E,
		AftJdCen->N,AftJdCen->E,&tmpfwj);

		if(AftJdCen->JDXZ==IsJiaoDian)AftRmax=AftDist;
		else PreRmax=AftDist-AftJdCen->R;	

		Rmax=PreRmax<AftRmax ? PreRmax:AftRmax;
		}

		for(j=0;j<5;j++)INDATA[j]=XYArray[XYNo][j];
		PB.x=XYArray[XYNo][8]; //N
		PB.y=XYArray[XYNo][9]; // E
		PB.a=XYArray[XYNo][5];
		l=XYArray[XYNo][2];
		xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // 某线元中点
		double fwjCenToPZ=PZ.a+XYArray[XYNo][4]*0.5*PI;
		tmpfwj=fwjCenToPZ-fwjOffset;
		double R=CurJdCen->R;
		if(CurJdCen->JDXZ==IsCenter)CurR=sqrt(fabs(dd*dd+R*R-2.0*dd*R*cos(tmpfwj)));
		else CurR=sqrt(fabs(dd*dd+R*R+2.0*dd*R*cos(tmpfwj)));

		CurJdCen->R=CurR;
		continue;//temp

		if(CurR<=Rmax&&CurR>0.5*R)
		{
		CurJdCen->R=CurR;
		if(CurJdCen->JDXZ==IsCenter)
		{
		if(PreJdCen->JDXZ==IsJiaoDian&&PreJdCen->R<0.0001)
		{
		int iXY=CurJdCen->CirXYNo;
		if(iXY<=1)
		{
		continue;
		}
		iXY--;
		if(int(XYArray[iXY][0]+0.001)!=XianYuan::LINE)iXY--;
		if(int(XYArray[iXY][0]+0.001)==XianYuan::LINE)
		{
		if(XYArray[iXY][1]>0.1)
		{
		ZYLink ZYObj(XYArray[iXY][8],XYArray[iXY][9],XYArray[iXY][5],CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->LeftOrRight);
		if(!ZYObj.ERRFlag)CurJdCen->A1=ZYObj.pSPIRAL.A;
		}
		}
		}
		if(AftJdCen->JDXZ==IsJiaoDian&&AftJdCen->R<0.0001)
		{
		int iXY=CurJdCen->CirXYNo;
		if(iXY>=XYNum-1)
		{
		continue;
		}

		iXY++;
		if(int(XYArray[iXY][0]+0.001)!=XianYuan::LINE)iXY++;
		if(int(XYArray[iXY][0]+0.001)==XianYuan::LINE)
		{
		if(XYArray[iXY][1]>0.1)
		{
		ZYLink ZYObj(XYArray[iXY][8],XYArray[iXY][9],XYArray[iXY][5]+PI,CurJdCen->N,CurJdCen->E,CurJdCen->R,-1*CurJdCen->LeftOrRight);
		if(!ZYObj.ERRFlag)CurJdCen->A2=ZYObj.pSPIRAL.A;
		}
		}
		}
		}
		}
		}*/

	}
	//	CalXYArray();
	//////////////////////////////////////
	return Acad::eOk;
}

void JD_CENTER::subList() const 
{
	assertReadEnabled();
	ads_printf(L"\n 类：\t%s",isA()->name());
	ads_printf(L"\nJdCenNum=%d\n",JdCenNum);
	for(int i=0;i<JdCenNum;i++)
	{
		ads_printf(L"%d %d %lf %lf %lf %lf %lf %d %d %d\n",
			i,JdCenArray[i].JDXZ,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,
			JdCenArray[i].R,JdCenArray[i].A2,JdCenArray[i].LeftOrRight,JdCenArray[i].LinkAfter,
			JdCenArray[i].CirXYNo);
	}
}

void JD_CENTER::Reverse()
{
	assertWriteEnabled();
	int i=0,j;
	JdCenter *JCArray=new JdCenter[JdCenNum];
	for(i=0;i<JdCenNum;i++)
	{
		j=JdCenNum-1-i;
		JCArray[i]=JdCenArray[j];		
	}
	for(i=0;i<JdCenNum;i++)
	{
		if(i<JdCenNum-1)JCArray[i].LinkAfter=JCArray[i+1].LinkAfter;
		double a=JCArray[i].A1;
		JCArray[i].A1=JCArray[i].A2;
		JCArray[i].A2=a;
		double fwj=JCArray[i].PreFwj;
		JCArray[i].PreFwj=JCArray[i].AftFwj;
		JCArray[i].AftFwj=fwj;
		JCArray[i].LeftOrRight*=(-1);
	}
	SetJdCenterArray(JCArray,JdCenNum);
	delete []JCArray;
}

//写出交点数据
void JD_CENTER::TranJD(CString fname)
{
	if(fname="")
	{
		assertWriteEnabled();
		JdCenter *CurJdCen;
		for(int i=1;i<JdCenNum-1;i++)
		{
			CurJdCen=&JdCenArray[i];
			if(CurJdCen->JDXZ==IsCenter)
			{
				CurJdCen->JDXZ=IsJiaoDian;
				CurJdCen->N=CurJdCen->JDN;
				CurJdCen->E=CurJdCen->JDE;

				//求当前交点的转角
				double tempaa=CurJdCen->PreFwj+PI;
				if(tempaa>2.0*PI)tempaa-=2.0*PI;
				double aa=CurJdCen->AftFwj-tempaa;
				if (aa<-pi) aa=aa+2*pi;
				if (aa>pi) aa=aa-2*pi;
				int LR;
				if(aa<0.0)LR=LEFT;
				else LR=RIGHT;

				if(LR*CurJdCen->LeftOrRight<0.0)CurJdCen->IsOrNotHuiTou=true;
				else CurJdCen->IsOrNotHuiTou=false;
			}
		}
	}
	else
	{
		FILE *fp=_wfopen(fname, L"w");
		fwprintf(fp, L"%d\n",JdCenNum);
		JdCenter *CurJDCen;
		for(int i=0;i<JdCenNum;i++)
		{
			CurJDCen=&JdCenArray[i];
			fwprintf(fp, L"%d %d %lf %lf %lf %lf %lf %d %d\n",i,IsJiaoDian,CurJDCen->JDN,CurJDCen->JDE,
				CurJDCen->A1,CurJDCen->R,CurJDCen->A2,CurJDCen->LeftOrRight,CurJDCen->LinkAfter);
		}
		fclose(fp);
	}	
}

void JD_CENTER::TranCenter()
{
	assertWriteEnabled();
	JdCenter *CurJdCen,*PreJdCen;
	for(int i=1;i<JdCenNum-1;i++)
	{
		CurJdCen=&JdCenArray[i];
		if(CurJdCen->JDXZ==IsJiaoDian)
		{
			CurJdCen->JDXZ=IsCenter;
			CurJdCen->N=CurJdCen->CenN;
			CurJdCen->E=CurJdCen->CenE;

			//acutPrintf(L"Center:%d: %lf,%lf\n",i,CurJdCen->CenE,CurJdCen->CenN);
			//求当前交点的转角
			double tempaa=CurJdCen->PreFwj+PI;
			if(tempaa>2.0*PI)tempaa-=2.0*PI;
			double aa=CurJdCen->AftFwj-tempaa;
			if (aa<-pi) aa=aa+2*pi;
			if (aa>pi) aa=aa-2*pi;
			int LR;
			if(aa<0.0)LR=LEFT;
			else LR=RIGHT;			

			if(CurJdCen->IsOrNotHuiTou)CurJdCen->LeftOrRight=LR*(-1);
			else CurJdCen->LeftOrRight=LR;

			PreJdCen=&JdCenArray[i-1];
			if(PreJdCen->LinkAfter==S_Link || PreJdCen->LinkAfter==C_Link)
			{
				if(PreJdCen->LeftOrRight*CurJdCen->LeftOrRight>0)PreJdCen->LinkAfter=C_Link;
				else PreJdCen->LinkAfter=S_Link;
			}
			if(PreJdCen->LinkAfter==S_Link)
			{
				double DistO1O2,tf;
				DistO1O2=xyddaa(PreJdCen->CenN,PreJdCen->CenE,CurJdCen->CenN,CurJdCen->CenE,&tf);
				double ER=PreJdCen->R+CurJdCen->R;//前后两半径之和
				if(DistO1O2<ER)
				{	//调整半径
					CurJdCen->R=DistO1O2-PreJdCen->R-0.000000001;
				}
			}
			if(PreJdCen->LinkAfter==Luan_Link)
			{
				/*
				double DistO1O2,tf;
				DistO1O2=xyddaa(PreJdCen->CenN,PreJdCen->CenE,CurJdCen->CenN,CurJdCen->CenE,&tf);
				double ER;//两圆半径之差
				ER=PreJdCen->R-CurJdCen->R;
				if(DistO1O2>fabs(ER))
				{
				}
				*/
			}	
		}
	}
}
//在尾部添加另一条线路
//传入另一条线路实体，本条线路上的一点，另一条线路上的一点
void JD_CENTER::AppendXL(JD_CENTER *OtherXL,double n1,double e1,double n2,double e2)
{
	//处理办法：第一步：先在线元数组中10号位置添加一个对应交点号/圆心号的信息
	//第二步：由线路上一点获取一个线元，找出对应的交点/圆心 号
	//第三步：分情况处理
	//（1） 直线 与 直线 相接
	//（2） 直线 与 圆   相接
	//（3） 圆   与 直线 相接
	//（4） 圆   与 圆   相接  S 型
	//（5） 圆   与 圆   相接  C 型
	//（6） 圆   与 圆   相接  卵型
	//     求出缓和曲线
	//第四步：把另一条线路实体的交点/圆心添加到本线路实体中:
	//（1）有可能在线路中部接线
	//（2）有可能线路反向

	if(this==OtherXL)
	{
		acutPrintf(L"所选为同一条线路!\n");
		return;
	}
	assertWriteEnabled();
	//start:
	double tylc1,tylc2;//在两点分别在两条线上的投影统一里程
	tylc1=PROJ_ML(n1,e1);
	tylc2=OtherXL->PROJ_ML(n2,e2);

	int XYNo1,XYNo2;//两点分别对应的序号
	XYNo1=GetXYNoFromTYLC(tylc1);
	XYNo2=OtherXL->GetXYNoFromTYLC(tylc2);

	int JDNo1,JDNo2;//两点分别对应的交点号
	JDNo1=XYJDNO[XYNo1];
	JDNo2=OtherXL->XYJDNO[XYNo2];

	JdCenter *JdCen1,*JdCen2;

	//获取相接的两个交点/圆心的指针
	JdCen1=&JdCenArray[JDNo1];
	JdCen2=&OtherXL->JdCenArray[JDNo2];

	//分情况处理
	if(JdCen1->JDXZ==IsCenter && JdCen2->JDXZ==IsCenter)
	{

		//两交点/圆心的半径及坐标/圆心距
		double R1,R2,N1,E1,N2,E2,DistO1O2,TmpFwj;
		R1=JdCen1->R;
		N1=JdCen1->CenN;
		E1=JdCen1->CenE;

		R2=JdCen2->R;
		N2=JdCen2->CenN;
		E2=JdCen2->CenE;

		//求圆心距
		DistO1O2=xyddaa(N1,E1,N2,E2,&TmpFwj);

		//acutPrintf(L"%lf,%lf,%lf,%lf,R1=%lf,R2=%lf,DistO1O2=%lf\n",E1,N1,E2,N2,R1,R2,DistO1O2);
		int LinkModel;//连接方式
		if(DistO1O2<fabs(R1-R2))LinkModel=Luan_Link;
		else if(DistO1O2>R1+R2)LinkModel=S_Link;
		else LinkModel=C_Link;

		if(LinkModel==Luan_Link)
		{
			JdCen1->LinkAfter=Luan_Link;
		}
		else if(LinkModel==S_Link)
		{
			//缺省1：1；
			JdCen1->LinkAfter=S_Link;
			JdCen1->A2=0.0;
			JdCen2->A1=0.0;
		}
		else// if(LinkModel==C_Link)
		{
			//缺省1：1；
			JdCen1->LinkAfter=C_Link;
			JdCen1->A2=0.0;
			JdCen2->A1=0.0;
		}

		//移除本线的后面交点/圆
		JdCenArray.RemoveAt(JDNo1+1,JdCenNum-(JDNo1+1));		
		for(int i=JDNo2;i<OtherXL->JdCenNum;i++)JdCenArray.Add(OtherXL->JdCenArray[i]);
		JdCenNum=JdCenArray.GetSize();
		CalXYArray();
	}
}

//由统一里程求所处的线元编号
int JD_CENTER::GetXYNoFromTYLC(double tylc)
{
	//如果里程大于线元起点里程，返回该线元号
	for(int i=XYNum-1;i>=0;i--)if(tylc>=XYArray[i][6])return i;
	return -1;//没有找到
}

//投影统一里程
double JD_CENTER::PROJ_ML(double n, double e)
{

	//    OutData();
	double prjml;
	prjml = BAS_DRAW_FUN::PROJ_ML(XYArray,XYNum,n,e,n,e);
	return prjml;
}

//生成直曲角"表
void JD_CENTER::CreateZQZJB()
{
	ZQZJB=new ZQZJTAB[JdCenNum];
	JdCenter *CurJdCen,*PreJdCen,*AftJdCen;
	xlpoint Pt;
	double tylcZH,tylcHY,tylcY0,tylcYH,tylcHZ;

	//	CalJiaoDian();
	for(int i=0;i<JdCenNum;i++)
	{
		CurJdCen=&JdCenArray[i];
		if(i>0)PreJdCen=&JdCenArray[i-1];
		if(i<JdCenNum-1)AftJdCen=&JdCenArray[i+1];

		//交点号
		ZQZJB[i].JDNO.Format(L"%s%.0lf",JDGH,CurJdCen->JdNo);

		//交点桩号
		ZQZJB[i].JDZH="";
		//交点坐标
		ZQZJB[i].JDN=CurJdCen->JDN;
		ZQZJB[i].JDE=CurJdCen->JDE;

		//与后一交点的交点距
		if(i<JdCenNum-1)ZQZJB[i].JDJJ=xyddaa(CurJdCen->JDN,CurJdCen->JDE,AftJdCen->JDN,AftJdCen->JDE,&ZQZJB[i].FWJ);
		else ZQZJB[i].JDJJ=0.0;
		//与后一交点的夹直线长
		if(i>=0 &&i<JdCenNum-1)
		{
			ZQZJB[i].JIAZX=ZQZJB[i].JDJJ-CurJdCen->T2-AftJdCen->T1;
			if(ZQZJB[i].JIAZX<0.001)
				ZQZJB[i].JIAZX=0.0;
		}
		else
		{
			ZQZJB[i].JIAZX=0.0;
		}
		//当前交点的转角
		if(i>0 && i<JdCenNum-1)
		{
			//			aa=CurJdCen->AftFwj+PI-CurJdCen->PreFwj;
			//			if(aa<-PI)aa+=2.0*PI;
			//			if(aa>PI)aa-=2.0*PI;
			ZQZJB[i].AFA=CurJdCen->afa;
		}
		else ZQZJB[i].AFA=0.0;


		if(i>0 && i<JdCenNum-1)
		{
			//切线长
			ZQZJB[i].T1=CurJdCen->T1;
			ZQZJB[i].T2=CurJdCen->T2;
			//半径

			if(XYArray[CurJdCen->CirXYNo-1][0]<4.1)//圆的前一线元为直线或完整缓线
				ZQZJB[i].R1 = VERY_MAX;
			else //圆的前一线元为不完整缓
				ZQZJB[i].R1 = XYArray[CurJdCen->CirXYNo-1][2];
			//			ZQZJB[i].R1=PreJdCen->R;

			ZQZJB[i].R2=CurJdCen->R;

			if(XYArray[CurJdCen->CirXYNo+1][0]<4.1)//圆的前一线元为直线或完整缓线
				ZQZJB[i].R3 = VERY_MAX;
			else //圆的前一线元为不完整缓
				ZQZJB[i].R3 = XYArray[CurJdCen->CirXYNo+1][3];
			//			ZQZJB[i].R3=AftJdCen->R;
			//缓和曲线
			ZQZJB[i].A1=CurJdCen->A1;
			ZQZJB[i].A2=CurJdCen->A2;
			//曲线长
			double R1,R2,A1,A2;
			//			if(PreJdCen->R>0.00001)R1=PreJdCen->R;
			//			else R1=VERY_MAX;
			//			if(CurJdCen->R>0.00001)R2=CurJdCen->R;
			//			else R2=VERY_MAX;
			R1 = ZQZJB[i].R1;
			R2 = ZQZJB[i].R2;

			A1=CurJdCen->A1;
			A2=CurJdCen->A2;
			ZQZJB[i].Ls1=fabs(A1*A1/R1-A1*A1/R2);
			ZQZJB[i].Lc=XYArray[CurJdCen->CirXYNo][2];

			//			if(AftJdCen->R>0.00001)R1=AftJdCen->R;
			//			else R1=VERY_MAX;
			//			if(CurJdCen->R>0.00001)R2=CurJdCen->R;
			//			else R2=VERY_MAX;
			R1 = ZQZJB[i].R3;
			R2 = ZQZJB[i].R2;

			ZQZJB[i].Ls2=fabs(A2*A2/R1-A2*A2/R2);
			ZQZJB[i].L=ZQZJB[i].Ls1+ZQZJB[i].Lc+ZQZJB[i].Ls2;

			//缓圆点的统一里程　＝　圆曲线线元的起点里程
			tylcHY=XYArray[CurJdCen->CirXYNo][6];
			//直缓圆点的统一里程　＝　圆曲线线元的起点里程　－　前缓长
			tylcZH=tylcHY-ZQZJB[i].Ls1;
			//圆曲线中点的里程　＝　圆曲线线元的起点里程　+　圆曲线长的一半
			tylcY0=tylcHY+0.5*ZQZJB[i].Lc;
			//圆缓点的统一里程　＝　圆曲线线元的起点里程　+　圆曲线长
			tylcYH=tylcHY+ZQZJB[i].Lc;
			//缓直点的统一里程　＝　缓圆点里程　＋　后缓长
			tylcHZ=tylcYH+ZQZJB[i].Ls2;

			ZQZJB[i].JDZH.Format(L"%s",GetXCLCStr(tylcZH+ZQZJB[i].T1));

			//第一缓和曲线起点桩号
			ZQZJB[i].ZHZH.Format(L"%s",GetXCLCStr(tylcZH));
			Pt.lc=tylcZH;
			xlpoint_pz(&Pt);
			//第一缓和曲线起点坐标
			ZQZJB[i].ZHN=Pt.x;
			ZQZJB[i].ZHE=Pt.y;


			//第一缓和曲线终点及圆曲线起点桩号
			ZQZJB[i].HYZH.Format(L"%s",GetXCLCStr(tylcHY));
			Pt.lc=tylcHY;
			xlpoint_pz(&Pt);
			//第一缓和曲线终点及圆曲线起点坐标
			ZQZJB[i].HYN=Pt.x;
			ZQZJB[i].HYE=Pt.y;

			//圆曲线中点桩号
			ZQZJB[i].YoZH.Format(L"%s",GetXCLCStr(tylcY0));
			Pt.lc=tylcY0;
			xlpoint_pz(&Pt);
			//圆曲线中点坐标
			ZQZJB[i].YoN=Pt.x;
			ZQZJB[i].YoE=Pt.y;			
			//外距
			double dx,dy;
			dx = Pt.x - CurJdCen->JDN;
			dy = Pt.y - CurJdCen->JDE;
			ZQZJB[i].OutDist=sqrt(dx*dx+dy*dy);
			//第二缓和曲线起点及圆曲线终点桩号
			ZQZJB[i].YHZH.Format(L"%s",GetXCLCStr(tylcYH));
			Pt.lc=tylcYH;
			xlpoint_pz(&Pt);
			//第二缓和曲线起点及圆曲线终点坐标
			ZQZJB[i].YHN=Pt.x;
			ZQZJB[i].YHE=Pt.y;

			//第二缓和曲线终点桩号
			ZQZJB[i].HZZH.Format(L"%s",GetXCLCStr(tylcHZ));
			Pt.lc=tylcHZ;
			xlpoint_pz(&Pt);
			//第二缓和曲线终点坐标
			ZQZJB[i].HZN=Pt.x;
			ZQZJB[i].HZE=Pt.y;
		}
		else
		{
			//切线长
			ZQZJB[i].T1=0.0;
			ZQZJB[i].T2=0.0;
			//半径
			ZQZJB[i].R1=0.0;
			ZQZJB[i].R2=0.0;
			ZQZJB[i].R3=0.0;
			//缓和曲线
			ZQZJB[i].A1=0.0;
			ZQZJB[i].A2=0.0;
			//曲线长
			ZQZJB[i].Ls1=0.0;
			ZQZJB[i].Lc=0.0;
			ZQZJB[i].Ls2=0.0;
			ZQZJB[i].L=0.0;
			//外距
			ZQZJB[i].OutDist=0.0;

			if(i==0)tylcZH=XYArray[1][6];
			if(i==JdCenNum-1)
			{
				int k=XYNum-1;
				//线元类型
				int j=(int)(XYArray[k][0]+0.0001);

				if(j==XianYuan::LINE)tylcZH=XYArray[k][6]+XYArray[k][1];//直线
				else if(j==XianYuan::ARC)tylcZH=XYArray[k][6]+XYArray[k][2];//弧
				else if(j==XianYuan::PreSPIRAL||j==XianYuan::AftSPIRAL)tylcZH=XYArray[k][6]
				+XYArray[k][1]*XYArray[k][1]/XYArray[k][2];//完整缓和曲线
				else tylcZH=XYArray[k][6]+fabs(XYArray[k][1]*XYArray[k][1]/XYArray[k][2]
				-XYArray[k][1]*XYArray[k][1]/XYArray[k][3]);//不完整缓和曲线
			}

			//第一缓和曲线起点桩号
			ZQZJB[i].ZHZH.Format(L"%s",GetXCLCStr(tylcZH));
			ZQZJB[i].JDZH=ZQZJB[i].ZHZH;
			Pt.lc=tylcZH;
			xlpoint_pz(&Pt);
			//第一缓和曲线起点坐标
			ZQZJB[i].ZHN=Pt.x;
			ZQZJB[i].ZHE=Pt.y;

			//第一缓和曲线终点及圆曲线起点桩号
			ZQZJB[i].HYZH=ZQZJB[i].ZHZH;
			//第一缓和曲线终点及圆曲线起点坐标
			ZQZJB[i].HYN=Pt.x;
			ZQZJB[i].HYE=Pt.y;

			//圆曲线中点桩号
			ZQZJB[i].YoZH=ZQZJB[i].ZHZH;
			//圆曲线中点坐标
			ZQZJB[i].YoN=Pt.x;
			ZQZJB[i].YoE=Pt.y;

			//第二缓和曲线起点及圆曲线终点桩号
			ZQZJB[i].YHZH=ZQZJB[i].ZHZH;				
			ZQZJB[i].YHN=Pt.x;
			ZQZJB[i].YHE=Pt.y;

			//第二缓和曲线终点桩号
			ZQZJB[i].HZZH=ZQZJB[i].ZHZH;
			//第二缓和曲线终点坐标
			ZQZJB[i].HZN=Pt.x;
			ZQZJB[i].HZE=Pt.y;				
		}				
		ZQZJB[i].Note="";
	}

}

CString JD_CENTER::GetXCLCStr(double tylc)
{
	CString GH;
	double xclc;
	xclc=fabs(XLC(tylc,GH));
	return LCchr(GH,xclc,2);
}

//  增加一个断链数据
void JD_CENTER::addDLM(int iDL,double BLC,double ELC,CString EGH  )
	//							 增中断链号，断前里程，断后里程
{
	if(fabs(ELC-BLC)<0.001)ELC=BLC;
	if (iDL>0 && iDL< DLNum )
	{
		if( (iDL<DLNum && BLC>DLArray[iDL-1].ELC && BLC< DLArray[iDL].BLC )
			|| iDL==DLNum )
		{
			assertWriteEnabled();
			DLBZ newDL;
			DLArray.InsertAt(iDL,newDL);
			DLNum+=1;
			DLArray[iDL].BGH=DLArray[iDL-1].EGH;
			DLArray[iDL].EGH=EGH;
			/*if(iDL<DLNum-1)*/DLArray[iDL+1].BGH=EGH;
			DLArray[iDL].BLC=BLC;
			DLArray[iDL].ELC= ELC;
			double DL=ELC-BLC;
			/*if(iDL<DLNum-1)*/DLArray[iDL+1].BLC += DL;
		}
	}
	GetDLTylc();
}

//  删除一个断链数据
void JD_CENTER::DelDLM(int iDL)
{
	double DL;
	if (iDL>0 && iDL< DLNum-1)
	{
		assertWriteEnabled();

		DL=DLArray[iDL].ELC-DLArray[iDL].BLC;
		DLArray.RemoveAt(iDL);
		DLNum-=1;
		if(iDL<DLNum)
		{
			DLArray[iDL].BGH=DLArray[iDL-1].EGH;
			DLArray[iDL].BLC-=DL;
		}
	}
	GetDLTylc();
}

int JD_CENTER::GetAfterDL(double tylc)
{
	GetDLTylc();
	//求某统一里程的后一个断链
	for(int iDL=DLNum-1;iDL>=0;iDL--)
	{		
		if(tylc>DLArray[iDL].TYLC)return iDL+1;
	}
}

void JD_CENTER::GetDLTylc()
{//计算每个断链处的统一里程
	CString FLDName;
	if(FLDNo>0)FLDName.Format(L"%.1lf",DLArray[0].ELC/1000);
	else FLDName="主线";
	double LCB=0;  // LCB 断链的统一里程
	int iDL=0;
	for(iDL=0;iDL<DLNum;iDL++)
	{
		//该断链点统一里程
		if(iDL==0)LCB=DLArray[iDL].ELC;
		else  LCB=LCB+DLArray[iDL].BLC-(DLArray[iDL-1].ELC);
		DLArray[iDL].TYLC=LCB;
		DLArray[iDL].FLDName=FLDName;
	}
}


void JD_CENTER::SetShowJD(BOOL IsShow)
{
	assertWriteEnabled();
	SHOWJD=IsShow;
}

void JD_CENTER::SetJDGH(CString str)
{
	_tcscpy(JDGH,str);
}

void JD_CENTER::SetLCGH(CString str)
{
	_tcscpy(LCGH,str);
}

Acad::ErrorStatus 
	JD_CENTER::getOffsetCurves(double            offsetDist, 
	AcDbVoidPtrArray& offsetCurves) const
{
	assertReadEnabled();	
	Acad::ErrorStatus es = Acad::eOk;
	//acutPrintf(L"OffsetOk!\n");
	AcGePoint3d sPt(0,0,0);
	AcGePoint3d ePt(100,410,0);
	AcDbLine *Line=new AcDbLine(sPt,ePt);
	offsetCurves.append(Line);	
	return es;
}

int JD_CENTER::GetPreJD(double N,double E)
{
	double Pttylc,JDtylc;
	Pttylc=PROJ_ML(N,E);
	for(int i=JdCenNum-1;i>=0;i--)
	{
		JdCenter *CurJC=&JdCenArray[i];
		JDtylc=PROJ_ML(CurJC->JDN,CurJC->JDE);
		if(Pttylc>=JDtylc)return i;
	}
}

void JD_CENTER::GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2)
{
	int i=GetPreJD(N,E);
	N1=JdCenArray[i].JDN;
	E1=JdCenArray[i].JDE;
	N2=JdCenArray[i+1].JDN;
	E2=JdCenArray[i+1].JDE;
}

double JD_CENTER::LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo)
	//给定统一里程，是否向前寻找附近的整百米标，要求在直线上,
{
	double gettylc=tylc1x;
	double lc0;//零桩
	int iXY=0;//搜索所处线元
	int XYClass;//线元类型
	double xlc;//一线上投影现场里程
	int i;
	for( i=XYNum-1;i>0;i--)
	{
		if(tylc1x>=XYArray[i][6])
		{
			iXY=i;
			break;
		}
	}
	if(i<=0)return -1.0;//plh06.03
	XYClass=XYArray[iXY][0];//取到线元类型
	if(IsForwardTo)//如果向前搜索
	{
		if(XYClass!=1 && iXY>1)iXY--;
		XYClass=XYArray[iXY][0];//取到线元类型
		if(XYClass!=1 && iXY>1)iXY--;
		XYClass=XYArray[iXY][0];//取到线元类型
		//此时线元必直线
		if(XYClass==1)
		{
			gettylc=XYArray[iXY+1][6];//一线直缓点里程
			if(tylc1x<gettylc)gettylc=tylc1x;//首次定位到直线上，
			//xlc=XLC1(gettylc,GH,NDL,DLB);
			CString LCGH;
			xlc=fabs(XLC(gettylc,LCGH));
			lc0=xlc-100*(int)(xlc/100+0.00001);
			if(lc0<XYArray[iXY+1][6]-XYArray[iXY][6]&&DistToNearDL(gettylc,true)>lc0)//调整后仍该线元,且调整前后位置之间没有断链
			{
				gettylc-=lc0;
			}
			else//取整十米
			{
				lc0=xlc-10*(int)(xlc/10+0.00001);
				gettylc-=lc0;
			}
		}
	}
	else
	{
		if(XYClass!=1 && iXY<XYNum-1)iXY++;
		XYClass=XYArray[iXY][0];//取到线元类型
		if(XYClass!=1 && iXY<XYNum-1)iXY++;
		XYClass=XYArray[iXY][0];//取到线元类型

		if(XYClass==1)
		{
			//此时线元必直线
			gettylc=XYArray[iXY][6];//直缓里程
			if(tylc1x>gettylc)gettylc=tylc1x;//首次定位到直线上，		
			//		xlc=XLC1(gettylc,GH,NDL,DLB);
			CString LCGH;
			xlc=fabs(XLC(gettylc,LCGH));
			lc0=xlc-100*(int)(xlc/100+0.00001);
			lc0=100.0-lc0;
			if(lc0<XYArray[iXY+1][6]-XYArray[iXY][6]&&fabs(DistToNearDL(gettylc,false))>lc0)//调整后仍该线元
			{
				gettylc+=lc0;
			}
			else//取整十米
			{
				lc0=xlc-10*(int)(xlc/10+0.00001);
				lc0=10.0-lc0;
				gettylc+=lc0;
			}		
		}
	}
	return gettylc;
}

double JD_CENTER::DistToNearDL(double tylc,bool IsForwardTo)//
	//给定统一里程，是否向前搜索，返加与附近断链处统一里程的差值，tylc>断链处lc...+
{
	double LCB=0.0;//断链处统一里程
	int iDL=0;
	double iXLlength=0.0;//某段长度
	for(iDL=1;iDL<DLNum;iDL++)
	{
		if(tylc<=LCB)break;
		iXLlength=DLArray[iDL].BLC-DLArray[iDL-1].ELC;//某段长度
		LCB+=iXLlength;//断链处统一里程
	}
	if(IsForwardTo)//向前搜索
	{
		LCB-=iXLlength;
	}
	else//向后搜索
	{
	}
	return tylc-LCB;	
}

bool JD_CENTER::CloneJC(JD_CENTER *pJC)
{
	if(pJC)
	{
		_tcscpy(pJC->LCGH,this->LCGH);
		_tcscpy(pJC->JDGH,this->JDGH);
		pJC->SCALE=this->SCALE;
		pJC->texth=this->texth;
		pJC->DLBlength=this->DLBlength;
		pJC->NLC=this->NLC;
		pJC->DrawLeftOrRight=this->DrawLeftOrRight;
		pJC->JD0=this->JD0;
		pJC->SHOWJD=this->SHOWJD;
		pJC->FLDNo=this->FLDNo;

		pJC->JdCenNum=this->JdCenNum;
		int i;
		for( i=0;i<this->JdCenNum;i++)
		{
			pJC->JdCenArray.Add(this->JdCenArray[i]);
		}
		pJC->DLNum=this->DLNum;
		pJC->DLArray.SetSize(pJC->DLNum);
		for(i=0;i<this->DLNum;i++)
		{
			pJC->DLArray[i].BGH=this->DLArray[i].BGH;
			pJC->DLArray[i].BLC=this->DLArray[i].BLC;
			pJC->DLArray[i].EGH=this->DLArray[i].EGH;
			pJC->DLArray[i].ELC=this->DLArray[i].ELC;
			pJC->DLArray[i].FLDNo=this->DLArray[i].FLDNo;
			pJC->DLArray[i].FLDName=this->DLArray[i].FLDName;
		}
		pJC->CalXYArray();
		return true;
	}
	return false;
}

/*===================================================================
函数名:CalTK
------------------------------------------------------------
函数功能:计算图框
------------------------------------------------------------
输入:	double		StartXCLC		起始里程
		double		EndXCLC			终止里程
		double		TKLen			每个图框支持的线路长度
		double		Frame_Height	图框高
		double		Frame_Width		图框宽
		STuKuang	*TK				图框结构体指针
		int			&TkNum			图框数量
		double		MoveCenterX		线路中心移动X
		double		MoveCenterY		线路中心移动Y
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：20190903编写注释
=====================================================================*/
void JD_CENTER::CalTK(double StartXCLC, double EndXCLC, double TKLen, double Frame_Height, double Frame_Width, STuKuang *TK, int &TkNum, double MoveCenterX, double MoveCenterY)
{
	double StartTYLC,EndTYLC;
	StartTYLC = TYLC(StartXCLC);
	EndTYLC = TYLC(EndXCLC);
	int iTK,i;
	double slc, elc, xlc, amt, amt1;
	CString LCGH;
	//计算图框起终点的里程
	bool IsBreak=false;
	iTK = 0;
	slc = StartTYLC;
	elc = slc + TKLen;
	if(elc > EndTYLC)
		goto L20;
	for (i = 0; i < DLNum; i++)
	{
		while((elc - DLArray[i].TYLC) < - 0.001)
		{			
			//终点取到整百米			
			xlc = fabs(XLC(elc, LCGH));
			amt = xlc - 100 * (int)(xlc / 100 + 0.00001);
			if (amt > 0.01)
				elc -= amt;
			STuKuang *CurTK = &TK[iTK];
			CurTK->StartTYLC = slc;
			CurTK->EndTYLC = elc;
			CurTK->StartXCLC = XLC(slc);
			CurTK->EndXCLC = XLC(elc);
			iTK++;			

			slc = elc;
			elc = slc + TKLen;
			if(elc > EndTYLC)
				goto L20;
		}

		xlc = fabs(XLC(elc, LCGH));
		amt = xlc - 100 * (int)(xlc / 100 + 0.00001);
		if(amt>0.01)
			xlc -= amt;
		if(xlc >= DLArray[i].ELC)//
			elc = elc - amt;
		else
		{
			amt1 = DLArray[i].BLC - 100 * (int)(DLArray[i].BLC / 100 + 0.00001);
			elc = elc - amt -amt1;
		}			
		STuKuang *CurTK = &TK[iTK];
		CurTK->StartTYLC = slc;
		CurTK->EndTYLC = elc;
		CurTK->StartXCLC = XLC(slc);
		CurTK->EndXCLC = XLC(elc);
		iTK++;			

		slc = elc;
		elc = slc + TKLen;
		if(elc > EndTYLC)
			break;
	}
L20:
	if (elc > EndTYLC&&fabs(elc - EndTYLC - TKLen) > 0.001)
	{
		STuKuang *CurTK = &TK[iTK];
		CurTK->StartTYLC = slc;
		CurTK->EndTYLC = EndTYLC;
		CurTK->StartXCLC = XLC(slc);
		CurTK->EndXCLC = XLC(EndTYLC);
		iTK++;			
	}
	TkNum=iTK;

	//计算图框中心点及四角点的坐标
	ads_point SPt,EPt;//图框起终边与线路交点
	double LtHmax,RtHmax;
	double LtXmax,LtYmax;
	double RtXmax,RtYmax;
	double SPACER = 10.0;
	xlpoint PZ;
	STuKuang *CurTK = NULL;
	ads_point pt;//弦上一点
	double Sfwj, Efwj, fwj;
	for (iTK = 0; iTK < TkNum; iTK++)
	{
		LtHmax = 0.0;
		RtHmax = 0.0;

		CurTK=&TK[iTK];

		PZ.lc = CurTK->StartTYLC;
		xlpoint_pz(&PZ);
		SPt[X] = PZ.y;
		SPt[Y] = PZ.x;
		Sfwj = PZ.a;
		CurTK->StX = SPt[X] + 5 * sin(Sfwj + PI);
		CurTK->StY = SPt[Y] + 5 * cos(Sfwj + PI);

		PZ.lc = CurTK->EndTYLC;
		xlpoint_pz(&PZ);
		EPt[X] = PZ.y;
		EPt[Y] = PZ.x;
		Efwj = PZ.a;
		CurTK->EdX = EPt[X] + 5 * sin(Efwj);
		CurTK->EdY = EPt[Y] + 5 * cos(Efwj);

		LtXmax = SPt[X];
		LtYmax = SPt[Y];
		RtXmax = EPt[X];
		RtYmax = EPt[Y];

		double lc0 = CurTK->StartTYLC + SPACER;

		while (lc0 < CurTK->EndTYLC)
		{
			PZ.lc = lc0;
			xlpoint_pz(&PZ);
			pt[X] = PZ.y;
			pt[Y] = PZ.x;

			double th = cal_h(SPt, EPt, pt);	//寻找该pt里程下起终点连线到该点的垂直距离，左负右正
			if(th < LtHmax)
			{
				LtHmax = th;
				LtXmax = pt[X];
				LtYmax = pt[Y];
			}
			if(th > RtHmax)
			{
				RtHmax = th;
				RtXmax = pt[X];
				RtYmax = pt[Y];
			}
			lc0 += SPACER;
		}

		//计算左右最大振幅的中点
		pt[X] = (LtXmax + RtXmax) / 2.0;
		pt[Y] = (LtYmax + RtYmax) / 2.0;
		//计算最大振幅中点沿起点-终点方向500m点
		double angle;
		xyddaa(SPt[Y], SPt[X], EPt[Y], EPt[X], &angle);
		ads_point tpt, pt1, pt2, pt3, pt4;
		tpt[X] = pt[X] + 500.0 * sin(angle);
		tpt[Y] = pt[Y] + 500.0 * cos(angle);

		//起点沿垂直于起点-终点方向1000m处点
		fwj = Sfwj - PI/2.0;
		if(fwj < 0.0)
			fwj = fwj + 2.0 * PI;
		pt1[X] = SPt[X] + 1000.0*sin(fwj);
		pt1[Y] = SPt[Y] + 1000.0*cos(fwj);
		//求交点
		INTERS(&pt3[X], &pt3[Y], pt[X], pt[Y], tpt[X], tpt[Y], SPt[X], SPt[Y], pt1[X], pt1[Y]);

		//终点沿垂直于起点-终点方向1000m处点
		fwj = Efwj - PI / 2.0;
		if(fwj < 0.0)
			fwj = fwj + 2.0 * PI;
		pt2[X] = EPt[X] + 1000.0*sin(fwj);
		pt2[Y] = EPt[Y] + 1000.0*cos(fwj);
		//求交点
		INTERS(&pt4[X], &pt4[Y], pt[X], pt[Y], tpt[X], tpt[Y], EPt[X], EPt[Y], pt2[X], pt2[Y]);

		//计算图框中心点
		CurTK->CenX = (pt3[X] + pt4[X]) / 2.0 - MoveCenterX;
		CurTK->CenY = (pt3[Y] + pt4[Y]) / 2.0 - MoveCenterY;

		//20190718	尝试修改图框	
		CurTK->LtUpX = CurTK->CenX - Frame_Width / 2.0;
		CurTK->LtUpY = CurTK->CenY + Frame_Height / 2.0;

		CurTK->LtBmX = CurTK->CenX - Frame_Width / 2.0;
		CurTK->LtBmY = CurTK->CenY - Frame_Height / 2.0;

		CurTK->RtUpX = CurTK->CenX + Frame_Width / 2.0;
		CurTK->RtUpY = CurTK->CenY + Frame_Height / 2.0;

		CurTK->RtBmX = CurTK->CenX + Frame_Width / 2.0;
		CurTK->RtBmY = CurTK->CenY - Frame_Height / 2.0;
		////四角点
		//fwj = angle - PI/2.0;
		//if(fwj < 0.0)
		//	fwj = fwj + 2.0 * PI;
		//tpt[X] = CurTK->CenX + LtWidth * sin(fwj);
		//tpt[Y] = CurTK->CenY + LtWidth * cos(fwj);

		//pt3[X] = tpt[X] + 1000.0 * sin(angle);
		//pt3[Y] = tpt[Y] + 1000.0 * cos(angle);
		//INTERS(&CurTK->LtUpX,&CurTK->LtUpY,tpt[X],tpt[Y],pt3[X],pt3[Y],
		//	SPt[X], SPt[Y], pt1[X], pt1[Y]);
		//INTERS(&CurTK->RtUpX,&CurTK->RtUpY,tpt[X],tpt[Y],pt3[X],pt3[Y],
		//	EPt[X], EPt[Y], pt2[X], pt2[Y]);

		//fwj = angle + PI/2.0;
		//if (fwj > 2.0*PI)
		//	fwj = fwj - 2.0 * PI;
		//tpt[X] = CurTK->CenX + RtWidth * sin(fwj);
		//tpt[Y] = CurTK->CenY + RtWidth * cos(fwj);
		//pt4[X] = tpt[X] + sin(angle);
		//pt4[Y] = tpt[Y] + cos(angle);

		//INTERS(&CurTK->LtBmX,&CurTK->LtBmY,tpt[X],tpt[Y],pt4[X],pt4[Y],
		//	SPt[X],SPt[Y],pt1[X],pt1[Y]);
		//INTERS(&CurTK->RtBmX,&CurTK->RtBmY,tpt[X],tpt[Y],pt4[X],pt4[Y],
		//	EPt[X],EPt[Y],pt2[X],pt2[Y]);	 

		////最后一幅图修正右角点坐标
		//if(iTK==TkNum-1)
		//{
		//	double dis = TKLen - (CurTK->EndTYLC-CurTK->StartTYLC);			
		//	if(dis>100.0)
		//	{          
		//		CurTK->RtUpX += dis*sin(angle);
		//		CurTK->RtUpY += dis*cos(angle);
		//		CurTK->RtBmX += dis*sin(angle);
		//		CurTK->RtBmY += dis*cos(angle);
		//		CurTK->CenX += 0.5*dis*sin(angle);
		//		CurTK->CenY += 0.5*dis*cos(angle);		
		//	}
		//}
	}
}

void JD_CENTER::SetTK(STuKuang& Frame, double Frame_Height, double Frame_Width, double CenterX, double CenterY)
{
	//计算图框中心点
	Frame.CenX = CenterX;
	Frame.CenY = CenterY;

	//20190718	尝试修改图框	
	Frame.LtUpX = Frame.CenX - Frame_Width / 2.0;
	Frame.LtUpY = Frame.CenY + Frame_Height / 2.0;

	Frame.LtBmX = Frame.CenX - Frame_Width / 2.0;
	Frame.LtBmY = Frame.CenY - Frame_Height / 2.0;

	Frame.RtUpX = Frame.CenX + Frame_Width / 2.0;
	Frame.RtUpY = Frame.CenY + Frame_Height / 2.0;

	Frame.RtBmX = Frame.CenX + Frame_Width / 2.0;
	Frame.RtBmY = Frame.CenY - Frame_Height / 2.0;
}

void JD_CENTER::TransFormTK(STuKuang *TK, int TkNum, double CenX, double CenY ,double scale)
{
	AcGeVector3d norm(0,0,1);
	norm.normalize();

	for (int iTK = 0; iTK < TkNum; iTK++)
	{
		STuKuang *CurTK = &TK[iTK];

		CurTK->newCenX = CenX;
		CurTK->newCenY = CenY;

		AcGeMatrix3d xformR, xformT, xformS;
		double jiao;
		xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);

		AcGeVector3d  moveBy(CurTK->newCenX - CurTK->CenX, CurTK->newCenY - CurTK->CenY, 0.0);
		AcGePoint3d PP(CurTK->CenX, CurTK->CenY, 0.0);
		xformR.setToRotation(-1.0*jiao, norm, PP);
		xformT.setToTranslation(moveBy);
		//		xformS.setToScaling(1.0/scale,AcGePoint3d(CenX,CenY,0.0));		
		//		if(scale>1.0)CurTK->xform=xformS*xformT*xformR;
		//		else CurTK->xform=xformT*xformR;
		CurTK->xform=xformT*xformR;

		AcGePoint3d StPt(CurTK->StX,CurTK->StY,0.0);
		StPt.transformBy(CurTK->xform);
		CurTK->newStX=StPt.x;
		CurTK->newStY=StPt.y;

		AcGePoint3d EdPt(CurTK->EdX,CurTK->EdY,0.0);
		EdPt.transformBy(CurTK->xform);
		CurTK->newEdX=EdPt.x;
		CurTK->newEdY=EdPt.y;

		AcGePoint3d LtUpPt(CurTK->LtUpX,CurTK->LtUpY,0.0);
		LtUpPt.transformBy(CurTK->xform);
		CurTK->newLtUpX=LtUpPt.x;
		CurTK->newLtUpY=LtUpPt.y;

		AcGePoint3d LtBmPt(CurTK->LtBmX,CurTK->LtBmY,0.0);
		LtBmPt.transformBy(CurTK->xform);
		CurTK->newLtBmX=LtBmPt.x;
		CurTK->newLtBmY=LtBmPt.y;

		AcGePoint3d RtUpPt(CurTK->RtUpX,CurTK->RtUpY,0.0);
		RtUpPt.transformBy(CurTK->xform);
		CurTK->newRtUpX=RtUpPt.x;
		CurTK->newRtUpY=RtUpPt.y;

		AcGePoint3d RtBmPt(CurTK->RtBmX,CurTK->RtBmY,0.0);
		RtBmPt.transformBy(CurTK->xform);
		CurTK->newRtBmX=RtBmPt.x;
		CurTK->newRtBmY=RtBmPt.y;
	}
}
void JD_CENTER::setRoadName(CString  rname)
{

	//	assertWriteEnabled();
	_tcscpy(RoadName , rname);
}
void JD_CENTER::setMdbName(CString  MdBname)
{
	//	assertWriteEnabled();
	_tcscpy(mdbname , MdBname);
}
/*


void JD_CENTER::TRAN_JD_CEN(JdCenter *PreJdCen,JdCenter *CurJdCen,JdCenter *AfterJdCen)
{

//为交点
if(CurJdCen->JDXZ==IsJiaoDian)
{
bool HasJiaZX=true;
//获取前、后JD/CENTER指针

if( (PreJdCen->LinkAfter==S_Link || PreJdCen->LinkAfter==C_Link)
&& CurJdCen->A1*CurJdCen->A2<0.1)
{
//当前交点的前切
CurJdCen->T1=PreJdCen->DistToAfterJD-PreJdCen->T2;
if(CurJdCen->T1<0.0001){Alert.Format(L"第%d个交点出错!",i);acutPrintf(L"%s\n",Alert);return;}

//保证前切
if(fabs(CurJdCen->A1)<0.0001 && fabs(CurJdCen->A2)<0.0001)
{
//调整半径
double ta=0.5*(PI-fabs(CurJdCen->afa));
if(ta<0.0001){Alert.Format(L"第%d个交点出错!",i);acutPrintf(L"%s\n",Alert);return;}
CurJdCen->R=CurJdCen->T1*tan(ta);
}
else if(fabs(CurJdCen->A1)<0.0001)
{
//调整A2
CurJdCen->A1=0.0;
ThreeJDCalThisAFromThatA_PreT_R CalObj;
CalObj.SetA1(0.0);
CalObj.SetAfa(fabs(CurJdCen->afa));
CalObj.SetPreT_R(CurJdCen->T1,CurJdCen->R);
double a2;
if(CalObj.GetA2(a2))CurJdCen->A2=a2;
}
else if(fabs(CurJdCen->A2)<0.0001)
{
//调整A1
CurJdCen->A2=0.0;
ThreeJDCalThisAFromThatA_PreT_R CalObj;
CalObj.SetA2(0.0);
CalObj.SetAfa(fabs(CurJdCen->afa));
CalObj.SetPreT_R(CurJdCen->T1,CurJdCen->R);
double a1;
if(CalObj.GetA1(a1))CurJdCen->A1=a1;
}
}			

if(CurJdCen->LinkAfter==S_Link || CurJdCen->LinkAfter==C_Link)
{
if(i>=2 && (PreJdCen->LinkAfter==S_Link
|| PreJdCen->LinkAfter==C_Link 
/ *|| PreJdCen->LinkAfter==Luan_Link* /))
{
if( CurJdCen->A1*CurJdCen->A2>0.1 )
{								
double tempT21,tempT12;
//	tempT12=PreJdCen->T2;

tempT12=PreJdCen->T2;

Norm3JD Norm3jdObj(CurJdCen->JDN,CurJdCen->JDE,AfterJdCen->JDN,
AfterJdCen->JDE,JdCenArray[i+2].JDN,JdCenArray[i+2].JDE,
AfterJdCen->R,AfterJdCen->A1,AfterJdCen->A2);
tempT21=Norm3jdObj.T1;
double t1,t2;

t1=PreJdCen->DistToAfterJD-tempT12;
t2=CurJdCen->DistToAfterJD-tempT21;

if(t1<0.0||t2<0.0){Alert.Format(L"第%d个交点出错!",i);acutPrintf(L"%s\n",Alert);return;}

ThreeJDCalA1A2FromT1T2R ThreeJdObj;
ThreeJdObj.SetPreJd(PreJdCen->JDN,PreJdCen->JDE);
ThreeJdObj.SetMidJd(CurJdCen->JDN,CurJdCen->JDE);
ThreeJdObj.SetAftJd(AfterJdCen->JDN,AfterJdCen->JDE);
ThreeJdObj.SetRadius(CurJdCen->R);
ThreeJdObj.SetT1T2(t1,t2);
ThreeJdObj.UpdateData();
if(!ThreeJdObj.ERRFlag)
{
CurJdCen->A1=ThreeJdObj.A1;
CurJdCen->A2=ThreeJdObj.A2;							
}
}
}
else
{	
if(i+2<=JdCenNum-1)
{
FourJDLocationC_S JDCSObj;
JDCSObj.SetFourJD(PreJdCen->JDN,PreJdCen->JDE,CurJdCen->JDN,CurJdCen->JDE,
AfterJdCen->N,AfterJdCen->E,JdCenArray[i+2].N,JdCenArray[i+2].E);					
JDCSObj.SetPreJDCS(CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
JDCSObj.SetAftJDCS(AfterJdCen->R,AfterJdCen->A1,AfterJdCen->A2);
JDCSObj.UpdateData();
if(JDCSObj.ERRFlag!=true)
{
CurJdCen->A2=JDCSObj.A12;
AfterJdCen->A1=JDCSObj.A21;
}
}
}				
}

//求当前交点的转角
aa=CurJdCen->afa;

if(PreJdCen->JDXZ==IsJiaoDian && PreJdCen->LinkAfter==Luan_Link)
{//交点法计算卵型曲线

double Arcl;
double R1,R2;
JDLuanLink JDLuanObj;
JDLuanObj.SetPreJD(PreJdCen->JDN,PreJdCen->JDE,PreJdCen->R);
JDLuanObj.SetCurJD(CurJdCen->JDN,CurJdCen->JDE,CurJdCen->R,CurJdCen->A1,CurJdCen->A2);
JDLuanObj.SetAftJD(AfterJdCen->JDN,AfterJdCen->JDE);
CurJdCen->T1=PreJdCen->DistToAfterJD-PreJdCen->T2;
JDLuanObj.SetPreT(CurJdCen->T1);
JDLuanObj.UpdateData();
if(JDLuanObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
CurJdCen->A1=JDLuanObj.PreA;
CurJdCen->A2=JDLuanObj.AftA;
CurJdCen->T2=JDLuanObj.AftT;
CurJdCen->CenN=JDLuanObj.CurCenN;
CurJdCen->CenE=JDLuanObj.CurCenE;
return;
}
//回头
if(CurJdCen->IsOrNotHuiTou)aa=2.0*PI-fabs(aa);
else aa=fabs(aa);
//求前切、后切、曲线长
R=CurJdCen->R;
A1=CurJdCen->A1;
A2=CurJdCen->A2;
Lo1=A1*A1/R;
Lo2=A2*A2/R;
getBPMTL(dms_rad(aa),R,Lo1,Lo2,&BPMTL[0]);
T1=BPMTL[6];//前切长
T2=BPMTL[7];//后切长
L=BPMTL[8];//曲线长
CurJdCen->T1=T1;
CurJdCen->T2=T2;
CurJdCen->L=L;
//求直缓点的坐标
Nzh=CurJdCen->N+T1*cos(CurJdCen->PreFwj);
Ezh=CurJdCen->E+T1*sin(CurJdCen->PreFwj);


//计算前一段夹直线长
l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
PEZ.x=Nzh+l*cos(CurJdCen->PreFwj);
PEZ.y=Ezh+l*sin(CurJdCen->PreFwj);
PEZ.a=CurJdCen->PreFwj+PI;
StdJiao(PEZ.a);
if(PreJdCen->LinkAfter==NotRelation && l>0.0)
{
INDATA[0]  = XianYuan::LINE;
INDATA[1]  = l;
INDATA[2]  = 0.0;
INDATA[3]  = 0.0;
INDATA[4]  = 0.0;

PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
else
{
//acutPrintf(L"第%d个交点与前面的夹直线不够!\n",i);
}
//前缓
l=Lo1;
if(l>0.0)
{
INDATA[0] = XianYuan::PreSPIRAL;
INDATA[1] = A1;
INDATA[2] = R;
INDATA[3] = 0.0;
INDATA[4] = CurJdCen->LeftOrRight;

PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//求出圆心坐标

double JiaoGQToCen;//缓和曲线与当前圆弧公切点到圆心的方位角
JiaoGQToCen=PEZ.a+CurJdCen->LeftOrRight*0.5*PI;
CurJdCen->CenN=PEZ.x+CurJdCen->R*cos(JiaoGQToCen);
CurJdCen->CenE=PEZ.y+CurJdCen->R*sin(JiaoGQToCen);
return;
//ARC
}
//为圆心
else if(CurJdCen->JDXZ==IsCenter)//求交点
{
CurJdCen->CenN=CurJdCen->N;
CurJdCen->CenE=CurJdCen->E;

int StartXYNo,EndXYNo;//该交点的起终线元号
StartXYNo=XYNum;
bool IsAllPreSpiral=true;
bool IsAllAftSpiral=true;
bool IsHasJiaZX=true;
//获取前、后JD/CENTER指针
PreJdCen=&JdCenArray[i-1];
AfterJdCen=&JdCenArray[i+1];
bool HasAfterSpiral=true;
//如果前面是交点 ,且不为卵型连接
if(PreJdCen->JDXZ==IsJiaoDian/ *&&PreJdCen->LinkAfter!=Luan_Link* /)
{
IsHasJiaZX=true;
//if(fabs(xyddaa(CurJdCen->N,CurJdCen->E,PreJdCen->N,PreJdCen->E,&tj)-CurJdCen->R)<0.001)
if(CurJdCen->A1<0.001)
{
//直圆公切
/ *
A1=0.0;
N1=PreJdCen->N;
E1=PreJdCen->E;
Nzh=N1;
Ezh=E1;
* /
A1=0.0;
double fwjCenToQD=PreJdCen->AftFwj-CurJdCen->LeftOrRight*0.5*PI;
N1=CurJdCen->CenN+CurJdCen->R*cos(fwjCenToQD);
E1=CurJdCen->CenE+CurJdCen->R*sin(fwjCenToQD);
Nzh=N1;
Ezh=E1;

Lo1=0.0;
CurJdCen->A1=0.0;
}
else
{
ZYLink ZYObj(PreJdCen->N,PreJdCen->E,PreJdCen->AftFwj,
CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->LeftOrRight );


if(ZYObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
A1=CurJdCen->A1;
N1=ZYObj.pSPIRAL.EndN;
E1=ZYObj.pSPIRAL.EndE;
Nzh=ZYObj.pSPIRAL.BaseN;
Ezh=ZYObj.pSPIRAL.BaseE;
Lo1=A1*A1/CurJdCen->R;
}
}
else//前面是圆心
{
IsHasJiaZX=false;
//由后接关系分情况处理
//一、S型
if(PreJdCen->LinkAfter==S_Link)
{
A1=CurJdCen->A1;
Lo1=A1*A1/CurJdCen->R;
}
//二、C型
if(PreJdCen->LinkAfter==C_Link)
{
A1=CurJdCen->A1;
Lo1=A1*A1/CurJdCen->R;					
}
//三、卵型
if(PreJdCen->LinkAfter==Luan_Link)
{
A1=CurJdCen->A1;
Lo1=fabs(A1*A1/CurJdCen->R-A1*A1/PreJdCen->R);
IsAllPreSpiral=false;			
}
/ *
Nzh=ZYObj.pSPIRAL.BaseN;
Ezh=ZYObj.pSPIRAL.BaseE;
* /
//四、有夹直线
if(PreJdCen->LinkAfter==NotRelation)
{
IsHasJiaZX=true;
YYQXLink YYQXObj;
YYQXObj.SetCir1(PreJdCen->N,PreJdCen->E,PreJdCen->R);
YYQXObj.SetCir2(CurJdCen->N,CurJdCen->E,CurJdCen->R);
YYQXObj.SetA(PreJdCen->A2,CurJdCen->A1);

if(PreJdCen->LeftOrRight<0 && CurJdCen->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
else if(PreJdCen->LeftOrRight>0 && CurJdCen->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
else if(PreJdCen->LeftOrRight<0 && CurJdCen->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
else YYQXObj.SetQXClass(S_S);

YYQXObj.CalQX();

if(YYQXObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}

A1=CurJdCen->A1;
if(CurJdCen->A1>0.0)
{
N1=YYQXObj.pSPIRAL2.EndN;//PtOnCirN2;
E1=YYQXObj.pSPIRAL2.EndE;//PtOnCirE2;
Nzh=YYQXObj.pSPIRAL2.StartN;//GQN2;
Ezh=YYQXObj.pSPIRAL2.StartE;//GQE2;
}
else
{
N1=YYQXObj.GQN2;
E1=YYQXObj.GQE2;
Nzh=YYQXObj.GQN2;
Ezh=YYQXObj.GQE2;
}
Lo1=A1*A1/CurJdCen->R;
}
}
//如果后一个为交点
if(AfterJdCen->JDXZ==IsJiaoDian)
{
//求后一条边的方位角
AfterJdCen->PreFwj=GetFwjByJd_Center_A_LR(AfterJdCen->N,AfterJdCen->E,
CurJdCen->N,CurJdCen->E,CurJdCen->R,CurJdCen->A2,
-1*CurJdCen->LeftOrRight);		

double tj;
if(fabs(xyddaa(CurJdCen->N,CurJdCen->E,AfterJdCen->N,AfterJdCen->E,&tj)-CurJdCen->R)<0.001)
{
A2=0.0;
N2=AfterJdCen->N;
E2=AfterJdCen->E;
Lo2=0.0;
CurJdCen->A2=0.0;
}
else
{
ZYLink ZYObj(AfterJdCen->N,AfterJdCen->E,AfterJdCen->PreFwj,
CurJdCen->N,CurJdCen->E,CurJdCen->R,-1*CurJdCen->LeftOrRight );

if(ZYObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
A2=CurJdCen->A2;
N2=ZYObj.pSPIRAL.EndN;
E2=ZYObj.pSPIRAL.EndE;
Lo2=A2*A2/CurJdCen->R;
}
}
else//如果后一个为圆心
{
//			double NHY,EHY;
//由后接关系分情况处理
//一、S型
if(CurJdCen->LinkAfter==S_Link)
{
YYSLink YYSObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,
CurJdCen->A2,AfterJdCen->A1,CurJdCen->LeftOrRight );

if(YYSObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到真实缓和曲线参数
CurJdCen->A2=YYSObj.A1;
AfterJdCen->A1=YYSObj.A2;

A2=YYSObj.A1;
N2=YYSObj.PtOnCir1N;
E2=YYSObj.PtOnCir1E;
Lo2=A2*A2/CurJdCen->R;
}
//二、C型
if(CurJdCen->LinkAfter==C_Link)
{
YYCLink YYCObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,
CurJdCen->A2,AfterJdCen->A1,CurJdCen->LeftOrRight );

if(YYCObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到真实缓和曲线参数
CurJdCen->A2=YYCObj.A1;
AfterJdCen->A1=YYCObj.A2;

A2=YYCObj.A1;
N2=YYCObj.pSPIRAL1.EndN;
E2=YYCObj.pSPIRAL1.EndE;
Lo2=A2*A2/CurJdCen->R;
}
//三、卵型
if(CurJdCen->LinkAfter==Luan_Link)
{
HasAfterSpiral=false;
IsAllAftSpiral=false;
YYLuanLink YYLuanObj( CurJdCen->N,CurJdCen->E,CurJdCen->R,
AfterJdCen->N,AfterJdCen->E,AfterJdCen->R,															
CurJdCen->LeftOrRight );

if(YYLuanObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到缓和曲线参数					
CurJdCen->A2=0.0;
AfterJdCen->A1=YYLuanObj.A;

A2=YYLuanObj.A;
if(CurJdCen->R>AfterJdCen->R)
{
N2=YYLuanObj.pSPIRAL.StartN;
E2=YYLuanObj.pSPIRAL.StartE;
}
else
{
N2=YYLuanObj.pSPIRAL.EndN;
E2=YYLuanObj.pSPIRAL.EndE;
}
Lo2=A2*A2/CurJdCen->R;
}
//四、有夹直线
if(CurJdCen->LinkAfter==NotRelation)
{
YYQXLink YYQXObj;
YYQXObj.SetCir1(CurJdCen->N,CurJdCen->E,CurJdCen->R);
YYQXObj.SetCir2(AfterJdCen->N,AfterJdCen->E,AfterJdCen->R);
YYQXObj.SetA(CurJdCen->A2,AfterJdCen->A1);

if(CurJdCen->LeftOrRight<0 && AfterJdCen->LeftOrRight<0)YYQXObj.SetQXClass(C_F);
else if(CurJdCen->LeftOrRight>0 && AfterJdCen->LeftOrRight>0)YYQXObj.SetQXClass(C_S);
else if(CurJdCen->LeftOrRight<0 && AfterJdCen->LeftOrRight>0)YYQXObj.SetQXClass(S_F);
else YYQXObj.SetQXClass(S_S);

YYQXObj.CalQX();

if(YYQXObj.ERRFlag)
{
Alert.Format(L"第%d个交点出错!",i);
acutPrintf(L"%s\n",Alert);
return;
}
//得到真实缓和曲线参数

A2=CurJdCen->A2;
if(CurJdCen->A2>0.0)
{
N2=YYQXObj.pSPIRAL1.EndN;//PtOnCirN1;
E2=YYQXObj.pSPIRAL1.EndE;//PtOnCirE1;
}
else
{
N2=YYQXObj.GQN1;
E2=YYQXObj.GQE1;
}
Lo2=A2*A2/CurJdCen->R;
}
}
EndXYNo=XYNum;//初值
PEZ.a=CurJdCen->PreFwj+PI;
//夹直线
//计算前一段夹直线长
if(IsHasJiaZX)
{
l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
if(l>0.0)
{
EndXYNo=XYNum;

XYArray[XYNum][0] = XianYuan::LINE;
XYArray[XYNum][1] = l;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
}
//前缓
l=Lo1;
if(l>0.0)
{
EndXYNo=XYNum;

if(IsAllPreSpiral)//完整前缓
{
XYArray[XYNum][0] = XianYuan::PreSPIRAL;
XYArray[XYNum][1] = A1;
XYArray[XYNum][2] = CurJdCen->R;
XYArray[XYNum][3] = 0.0;
}
else
{
if(PreJdCen->R>CurJdCen->R)
XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
XYArray[XYNum][1] = A1;
XYArray[XYNum][2] = PreJdCen->R;
XYArray[XYNum][3] = CurJdCen->R;
}
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//用于计算圆曲线长			
N1=PEZ.x;	E1=PEZ.y;
//ARC
l=GetArcLengthBy2PointOnCircleAndLR(CurJdCen->N,CurJdCen->E,CurJdCen->R,
N1,E1,N2,E2,CurJdCen->LeftOrRight);
if(l>0.0)
{
//判断是否回头
if(l>CurJdCen->R*PI)CurJdCen->IsOrNotHuiTou=true;
else CurJdCen->IsOrNotHuiTou=false;
EndXYNo=XYNum;
CurJdCen->CirXYNo=XYNum;
XYArray[XYNum][0] = XianYuan::ARC;
XYArray[XYNum][1] = CurJdCen->R;
XYArray[XYNum][2] = l;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//后缓
l=Lo2;
if(l>0.0&&HasAfterSpiral)
{
EndXYNo=XYNum;

if(IsAllAftSpiral)//完整后缓
{
XYArray[XYNum][0] = XianYuan::AftSPIRAL;
XYArray[XYNum][1] = A2;
XYArray[XYNum][2] = CurJdCen->R;
XYArray[XYNum][3] = 0.0;
}
else
{
if(CurJdCen->R>AfterJdCen->R)
XYArray[XYNum][0] = XianYuan::BigToSmallSPIRAL;
else XYArray[XYNum][0] = XianYuan::SmallToBigSPIRAL;
XYArray[XYNum][1] = A2;
XYArray[XYNum][2] = CurJdCen->R;
XYArray[XYNum][3] = AfterJdCen->R;
}
XYArray[XYNum][4] = CurJdCen->LeftOrRight;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应交点/圆心号
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//计算交点
CurJdCen->PreFwj=XYArray[StartXYNo][5]+PI;
CurJdCen->AftFwj=PEZ.a;
double n1,e1,n2,e2,n3,e3,n4,e4;//两切线

int i;
i=StartXYNo;
if(XYArray[i][0]<1.5)i++;
n1=XYArray[i][8];
e1=XYArray[i][9];

n2=n1+1000.0*cos(CurJdCen->PreFwj);
e2=e1+1000.0*sin(CurJdCen->PreFwj);

n3=PEZ.x;
e3=PEZ.y;

n4=n3+1000.0*cos(CurJdCen->AftFwj);
e4=e3+1000.0*sin(CurJdCen->AftFwj);

INTERS(&CurJdCen->JDN,&CurJdCen->JDE,n1,e1,n2,e2,n3,e3,n4,e4);

double tmpjiao;
CurJdCen->T1=xyddaa(n1,e1,CurJdCen->JDN,CurJdCen->JDE,&tmpjiao);
CurJdCen->T2=xyddaa(n3,e3,CurJdCen->JDN,CurJdCen->JDE,&tmpjiao);
CurJdCen->L=PEZ.lc-XYArray[StartXYNo][6];
}
/ *
CurJdCen->JdNo=JD0+iJD;
iJD++;
* /
}
CurJdCen=&JdCenArray[JdCenNum-1];
//夹直线
Nzh=JdCenArray[JdCenNum-1].N;//终点
Ezh=JdCenArray[JdCenNum-1].E;
//计算前一段夹直线长
l=sqrt( (Nzh-PEZ.x)*(Nzh-PEZ.x) + (Ezh-PEZ.y)*(Ezh-PEZ.y) );
if(l>0.0)
{
XYArray[XYNum][0] = XianYuan::LINE;
XYArray[XYNum][1] = l;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应最后一个交点
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
}
//最后一个线元为终点
XYArray[XYNum][0] = 0.0;
XYArray[XYNum][1] = 0.0;
XYArray[XYNum][2] = 0.0;
XYArray[XYNum][3] = 0.0;
XYArray[XYNum][4] = 0.0;
XYArray[XYNum][5] = PEZ.a ;
XYArray[XYNum][6] = PEZ.lc;
XYArray[XYNum][7] = PEZ.r ;
XYArray[XYNum][8] = PEZ.x ;
XYArray[XYNum][9] = PEZ.y ;
XYJDNO[XYNum]=i;//对应最后一个交点
//求该线元终点
for (k=0;k<5;k++) INDATA[k]=XYArray[XYNum][k];
//XYNum++;
PB0=PEZ;
xlzbxy(&PB0,&PEZ,l,INDATA,0);
GetDLTylc();

//交点
CurJdCen=&JdCenArray[JdCenNum-1];
CurJdCen->JDN=PEZ.x;
CurJdCen->JDE=PEZ.y;
CurJdCen->A1=0.0;
CurJdCen->A2=0.0;
CurJdCen->T1=0.0;
CurJdCen->T2=0.0;
//计算线路终点的统一里程
int j=(int)(XYArray[XYNum-1][0]+0.001);
if(j==XianYuan::LINE)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1];
else if(j==XianYuan::ARC)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][2];
else if(j==XianYuan::AftSPIRAL)XLLength=XYArray[XYNum-1][6]+XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/XYArray[XYNum-1][2];
else
{
//终点投影
XLLength=PROJ_ML(PEZ.x,PEZ.y);
}

if(XYNum>0) XYArray[0][4]=DLArray[0].ELC;

LenOfLine = XLLength - XYArray[0][4];//线路长度

CString TGH;
DLNum=DLArray.GetSize();
double endXlc=XLC(XLLength,TGH);
//	double endXlc=XLC(XYArray[XYNum-1][6],TGH);
//修正最后一个断链
setDLM(DLNum-1,endXlc,endXlc,DLArray[DLNum-1].BGH,DLArray[DLNum-1].EGH);
//
//	acutPrintf(L"%s ",DLArray[DLNum-1].BGH);
//	acutPrintf(L"%lf ",DLArray[DLNum-1].BLC);
//	acutPrintf(L"%s ",DLArray[DLNum-1].EGH);
//	acutPrintf(L"%lf ",DLArray[DLNum-1].ELC);
//	acutPrintf(L"\n");


if(JdCenNum>2)
{
if(JdCenArray[1].T1>JdCenArray[0].DistToAfterJD)
{
JdCenArray[0].JDN=XYArray[1][8];//JdCenArray[1].JDN+JdCenArray[1].T1*cos(JdCenArray[1].PreFwj);
JdCenArray[0].JDE=XYArray[1][9];//JdCenArray[1].JDE+JdCenArray[1].T1*sin(JdCenArray[1].PreFwj);
JdCenArray[0].N=JdCenArray[0].JDN;
JdCenArray[0].E=JdCenArray[0].JDE;
}

if(JdCenArray[JdCenNum-2].T2>JdCenArray[JdCenNum-2].DistToAfterJD)
{
JdCenArray[JdCenNum-1].JDN=JdCenArray[JdCenNum-2].JDN+JdCenArray[JdCenNum-2].T2*cos(JdCenArray[JdCenNum-2].AftFwj);
JdCenArray[JdCenNum-1].JDE=JdCenArray[JdCenNum-2].JDE+JdCenArray[JdCenNum-2].T2*sin(JdCenArray[JdCenNum-2].AftFwj);
JdCenArray[JdCenNum-1].N=JdCenArray[JdCenNum-1].JDN;
JdCenArray[JdCenNum-1].E=JdCenArray[JdCenNum-1].JDE;
}
}
//	CalJiaoDian();
//	OutData();
//	acutPrintf(L"\nOK!");
}
*/
//szf
int JD_CENTER::PM_DMX(double* dmx, double ZJ, double sml, double eml)
{

	FormLCB(ZDMJG, sml, eml);
	double startLC, endLC;
	startLC = pLCB[0].lc;
	endLC = pLCB[NLCB - 1].lc;
	xlpoint PZ;
	double pp[2], tmp;
	int NUM = 0;
	int i, j, k = 0;
	double lc = startLC;
	PZ.lc = lc;
	xlpoint_pz(&PZ);
	pp[0] = PZ.x + ZJ * cos(PZ.a + 0.5 * PI);
	pp[1] = PZ.y + ZJ * sin(PZ.a + 0.5 * PI);


	tmp=ptriDTM.interpolate(pp);

	dmx[NUM*2]=PZ.lc;
	dmx[NUM*2+1]=tmp;
	NUM++;

	ACHAR pszLabel[80];
	_tcscpy(pszLabel, L"纵断面地面线内插");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	int SUM = (int)((endLC-startLC)/ZDMJG+0.1);
	i = 0; j=1;
	while(XYArray[j][6]<=startLC)
		j++;
	while (i<NLCB)//QXB里程为现场里程；
	{	
		int Pos = (int)(NUM*100.0/SUM);
		acedSetStatusBarProgressMeterPos(Pos);
		if(pLCB[i].lc<XYArray[j][6]+0.0001)
		{
			lc=pLCB[i].lc;		
			i++;
		}	
		else if(j<=XYNum)
		{
			lc=XYArray[j][6];
			j++;
		}
		else
			break;

		PZ.lc=lc;
		xlpoint_pz(&PZ) ;
		pp[0] = PZ.x +ZJ*cos(PZ.a+0.5*PI);
		pp[1] = PZ.y +ZJ*sin(PZ.a+0.5*PI);
		tmp=ptriDTM.interpolate(pp);


		dmx[NUM*2]=PZ.lc;
		dmx[NUM*2+1]=tmp;
		//	acutPrintf(L"%lf %lf %lf\n",lc,pLCB[i].lc,tmp);//fabs(dmx[NUM*2]-dmx[(NUM-1)*2]));
		if(fabs(dmx[NUM*2]-dmx[NUM*2-2])>0.001)NUM++;			
	}
	acedSetStatusBarProgressMeterPos(100);    
	acedRestoreStatusBar();

	//acutPrintf(L"NUM=%d\n",NUM);
	return NUM/*NUM*/;
}

//szf
void  JD_CENTER::FormLCB(double dlc,double sml,double eml)  
	//  标注公里百米里程标  
{    
	double  BZlc,lc,LCB,DDlength,DL;
	int BZW,iDL;
	long int zlc;
	LCBdata plcb;

	pLCB.RemoveAll();
	NLCB=0;
	LCB=DLArray[0].ELC;
	DDlength = LCB;

	if(LCB<=sml)
	{
		plcb.lc=sml;
		plcb.lcflag = -1;
		_tcscpy(plcb.flag, L"QD");
		plcb.BTxz=0;
		pLCB.Add(plcb);
		NLCB++;		 
	}
	else if(LCB<eml+0.001)
	{
		plcb.lc=LCB;
		plcb.lcflag = -1;
		_tcscpy(plcb.flag, L"QD");
		plcb.BTxz=0;
		pLCB.Add(plcb);
		NLCB++;		 
	}

	// 逐个断链段标注
	for (iDL=0;iDL<DLNum;iDL++)
	{ 
		DL = DLArray[iDL].ELC-DLArray[iDL].BLC;
		LCB= DLArray[iDL].ELC;  //本段起点里程 
		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< DLNum-1)  // 不为起 终点
		{ // 绘出 iDL点
			lc=DDlength;

			if(lc>eml+0.001)
				break;
			else if(lc>sml-0.0001)
			{
				plcb.lc=lc;
				plcb.BTxz=0;
				if(fmod(DLArray[iDL].BLC+0.0001,1000)<0.01)//断链在现场里程整公里处
				{
					_tcscpy(plcb.flag, L"断链");
					plcb.lcflag=10;
				}
				else //szf 2005.5.9
				{
					_tcscpy(plcb.flag, L"断链");
					plcb.lcflag = -99;//断链在零桩
					if(fmod((DLArray[iDL].BLC-floor(DLArray[iDL].BLC/1000)*1000),100)<0.01) //断链在现场里程整百米处
						plcb.lcflag=floor((DLArray[iDL].BLC-floor(DLArray[iDL].BLC/1000)*1000)/100+0.01);
					else if(fmod((DLArray[iDL].ELC-floor(DLArray[iDL].ELC/1000)*1000),100)<0.01) //断链在现场里程整百米处
						plcb.lcflag=floor((DLArray[iDL].ELC-floor(DLArray[iDL].ELC/1000)*1000)/100+0.01);
				}
				pLCB.Add(plcb);
				NLCB++;		
			}
		};	  
		zlc=(long int)(LCB/dlc);
		BZlc=zlc*dlc;  // 实际里程 
		if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
		BZW=99;
		while(iDL<DLNum-1&&BZlc<DLArray[iDL+1].BLC)
		{
			// 统一里程
			lc=DDlength+BZlc-LCB; 
			if(lc>eml+0.001)
				break;
			else if(lc>sml-0.0010)
			{
				plcb.lc=lc;
				plcb.BTxz=0;
				plcb.lcflag = 10;
				if(fmod(BZlc+0.0001,1000)<0.01)//现场里程整公里处
					_tcscpy(plcb.flag, L"公里桩");
				//   pLCB[NLCB].lcflag=-1;
				//plcb.lcflag=10;//2003.12.26
				else
				{
					_tcscpy(plcb.flag, L"");
					plcb.lcflag=floor((BZlc-floor(BZlc/1000)*1000)/100+0.01);
				}
				pLCB.Add(plcb);
				NLCB++;		
			}
			BZlc=BZlc+dlc;		 
		} 
		if(iDL<DLNum-1)
		{
			DDlength=DDlength+DLArray[iDL+1].BLC-LCB;//断链处统一里程。
			lc=DDlength;
		}

	} // IDL
	if(lc>eml)
	{
		plcb.lc=eml;		
		plcb.BTxz=0;
		plcb.lcflag=-1;
		_tcscpy(plcb.flag, L"ZD");
		if(plcb.lc-pLCB[NLCB-1].lc > 0.001)
		{
			pLCB.Add(plcb);
			NLCB++;		
		}
	}
	else if(lc>sml-0.001) // 终点
	{
		plcb.lc=lc;		
		plcb.BTxz=0;
		plcb.lcflag=-1;
		_tcscpy(plcb.flag, L"ZD");
		pLCB.Add(plcb);
		NLCB++;		
	}
}

void  JD_CENTER::AddTZDtoLCB() //将曲线特征点加入里程表 
{
	int i=0,j=0,k;
	double lc;
	LCBdata plcb;
	ACHAR chr[10],ch[10],tch[10];

	for(i=1; i<XYNum; i++)
	{
		lc=XYArray[i][6];

		{
			//找到第一个大于lc的位置
			j=0;

			while(j<pLCB.GetSize() && lc>pLCB[j].lc-0.001)
				j++;

			if(j>0 && lc>pLCB[j-1].lc+0.001)
			{
				k=(int)(XYArray[i][0]+0.01);
				if (k>=3) {_tcscpy(tch, L" H"); };
				if (k==1) {_tcscpy(tch, L" Z"); };
				if (k==2) {_tcscpy(tch, L" Y"); };
				k=(int)(XYArray[i-1][0]+0.01);
				if (k>=3) {_tcscpy(ch, L" H"); };
				if (k==1) {_tcscpy(ch, L" Z"); };
				if (k==2) {_tcscpy(ch, L" Y"); };
				_tcscpy(chr,ch);_tcscat(chr,tch);
				if (_tcsncmp(chr, L" YY",3)==0) {  _tcscpy(chr, L" GQ");   };
				if (_tcsncmp(chr, L" C.C.",5)==0) {  _tcscpy(chr, L" P.C.C.");};
				if (_tcsncmp(chr, L" S.S.",5)==0&&(k==3||k==4) ) {  _tcscpy(chr, L" PRC");};
				if (_tcsncmp(chr, L" HH",3)==0&&(k==3||k==4))
				{
					_tcscpy(chr, L" GQ");
				};

				plcb.lc = lc;
				plcb.BTxz=0;
				plcb.lcflag = -1; 
				_tcscpy(plcb.flag,chr);
				pLCB.InsertAt(j,plcb);
			}
		}
	}
	NLCB = pLCB.GetSize();
}

void  JD_CENTER::AddBritoLCB(int NBTS,double BTS[][3]) //将桥隧加入里程表 
{
	double lc;
	LCBdata plcb;
	int j;

	if(NBTS<1)
		return;

	for(int i=0; i<NBTS; i++)
	{
		lc = BTS[i][0];

		j=0;
		while(j<pLCB.GetSize() && lc>pLCB[j].lc-0.001)
			j++;
		if(j<1)
			continue;

		if(lc>pLCB[j-1].lc+0.001 || pLCB[j-1].BTxz>=4)//考虑桥隧相连
		{
			plcb.lc = lc;
			plcb.lcflag = -1; 
			if(BTS[i][2]<0.1)//0-全桥
			{
				plcb.BTxz=-3;
				_tcscpy(plcb.flag, L"全桥起始桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				plcb.BTxz=-1;
				_tcscpy(plcb.flag, L"左桥起始桩");
			}
			else 
			{
				plcb.BTxz=-2;
				_tcscpy(plcb.flag, L"右桥起始桩");
			}
			pLCB.InsertAt(j,plcb);
		}
		else//修改前一断面的性质
		{
			pLCB[j-1].lc = BTS[i][0];
			if(BTS[i][2]<0.1)//0-全桥
			{
				pLCB[j-1].BTxz=-3;
				_tcscpy(pLCB[j-1].flag, L"全桥起始桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				pLCB[j-1].BTxz=-1;
				_tcscpy(pLCB[j-1].flag, L"左桥起始桩");
			}
			else 
			{
				pLCB[j-1].BTxz=-2;
				_tcscpy(pLCB[j-1].flag, L"右桥起始桩");
			}
		}
		////////////////////////////////////////////////////////////

		lc = BTS[i][1];
		j=0;
		while(j<pLCB.GetSize() && lc>pLCB[j].lc-0.001)
			j++;
		if(j<1)
			continue;
		if(lc>pLCB[j-1].lc+0.001)
		{
			plcb.lc = lc;
			if(BTS[i][2]<0.1)//0-全桥
			{
				plcb.BTxz=3;
				_tcscpy(plcb.flag, L"全桥终止桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				plcb.BTxz=1;
				_tcscpy(plcb.flag, L"左桥终止桩");
			}
			else 
			{
				plcb.BTxz=2;
				_tcscpy(plcb.flag, L"右桥终止桩");
			}
			pLCB.InsertAt(j,plcb);
		}
		else//修改前一断面的性质
		{
			pLCB[j-1].lc = BTS[i][1];
			if(BTS[i][2]<0.1)//0-全桥
			{
				pLCB[j-1].BTxz=3;
				_tcscpy(pLCB[j-1].flag, L"全桥终止桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				pLCB[j-1].BTxz=1;
				_tcscpy(pLCB[j-1].flag, L"左桥终止桩");
			}
			else 
			{
				pLCB[j-1].BTxz=2;
				_tcscpy(pLCB[j-1].flag, L"右桥终止桩");
			}
		}

	}
	NLCB = pLCB.GetSize();
}

void  JD_CENTER::AddTuntoLCB(int NBTS,double BTS[][3]) //将桥隧加入里程表 
{
	double lc;
	LCBdata plcb;
	int j;

	if(NBTS<1)
		return;

	for(int i=0; i<NBTS; i++)
	{
		lc = BTS[i][0];
		j=0;
		while(j<pLCB.GetSize() && lc>pLCB[j].lc-0.001)
			j++;
		if(j<1)
			continue;
		if(lc>pLCB[j-1].lc+0.001 || (pLCB[j-1].BTxz<=3 && pLCB[j-1].BTxz>=1))//考虑桥隧相连
		{
			plcb.lc = lc;
			plcb.lcflag = -1; 
			if(BTS[i][2]<0.1)//0-全桥
			{
				plcb.BTxz=-6;
				_tcscpy(plcb.flag, L"全隧起始桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				plcb.BTxz=-4;
				_tcscpy(plcb.flag, L"左隧起始桩");
			}
			else 
			{
				plcb.BTxz=-5;
				_tcscpy(plcb.flag, L"右隧起始桩");
			}
			pLCB.InsertAt(j,plcb);
		}
		else//修改前一断面的性质
		{
			pLCB[j-1].lc = BTS[i][0];
			if(BTS[i][2]<0.1)//0-全桥
			{
				pLCB[j-1].BTxz=-6;
				_tcscpy(pLCB[j-1].flag, L"全隧起始桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				pLCB[j-1].BTxz=-4;
				_tcscpy(pLCB[j-1].flag, L"左隧起始桩");
			}
			else 
			{
				pLCB[j-1].BTxz=-5;
				_tcscpy(pLCB[j-1].flag, L"右隧起始桩");
			}
		}
		////////////////////////////////////////////////////////////

		lc = BTS[i][1];
		j=0;
		while(j<pLCB.GetSize() && lc>pLCB[j].lc-0.001)
			j++;

		if(j<1)
			continue;
		if(lc>pLCB[j-1].lc+0.001 || (pLCB[j-1].BTxz<=-1 && pLCB[j-1].BTxz>=-3))
		{
			plcb.lc = lc;
			if(BTS[i][2]<0.1)//0-全桥
			{
				plcb.BTxz=6;
				_tcscpy(plcb.flag, L"全隧终止桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				plcb.BTxz=4;
				_tcscpy(plcb.flag, L"左隧终止桩");
			}
			else 
			{
				plcb.BTxz=5;
				_tcscpy(plcb.flag, L"右隧终止桩");
			}
			pLCB.InsertAt(j,plcb);
		}
		else//修改前一断面的性质
		{
			pLCB[j-1].lc = BTS[i][1];
			if(BTS[i][2]<0.1)//0-全桥
			{
				pLCB[j-1].BTxz=6;
				_tcscpy(pLCB[j-1].flag, L"全隧终止桩");
			}
			else if(BTS[i][2]<1.1)//1-左桥
			{
				pLCB[j-1].BTxz=4;
				_tcscpy(pLCB[j-1].flag, L"左隧终止桩");
			}
			else 
			{
				pLCB[j-1].BTxz=5;
				_tcscpy(pLCB[j-1].flag, L"右隧终止桩");
			}
		}

	}
	NLCB = pLCB.GetSize();
}


//add输入起始统一里程，起始偏距(左-右+)，终止里程，终止偏距,变宽方式(0-线性，1-抛物线)
void JD_CENTER::DrawKD(double sml,double sw,double eml,double ew,int mode,int icolor,double interval)
{
	AcGePoint2d mpt;
	AcGePoint2dArray  pArr;

	if(eml-sml<0.001)
		return;

	//	int n=(int)((eml-sml)/interval+1);
	//	pArr.setLogicalLength(n+1);

	double cml,wid,k;
	cml = sml;
	int i=0;
	while(cml<eml)
	{
		//求cml处的宽度
		k = (cml-sml)/(eml-sml);
		if(mode>0)
			k = 4*k*k*k - 3*k*k*k*k;
		wid =  sw + k*(ew-sw);//线性

		xlpoint PZtmp;
		PZtmp.lc=cml;  // 统一里程
		xlpoint_pz(&PZtmp);  
		mpt.y = PZtmp.x + wid * cos(PZtmp.a + 0.5*PI);
		mpt.x = PZtmp.y + wid * sin(PZtmp.a + 0.5*PI);
		pArr.append(mpt);						
		cml += interval;
		i++;
	}

	xlpoint PZtmp;
	PZtmp.lc=eml;  // 统一里程
	xlpoint_pz(&PZtmp);  
	mpt.y = PZtmp.x + wid * cos(PZtmp.a + 0.5*PI);
	mpt.x = PZtmp.y + wid * sin(PZtmp.a + 0.5*PI);
	pArr.append(mpt);					
	int NUM=pArr.length();
	AcDbPolyline *pPline=new AcDbPolyline(NUM);
	for(i=0;i<NUM;i++)
	{
		pPline->addVertexAt(i,pArr[i]);
		pPline->setWidthsAt(i,0.0,0.0);
	}

	AcDbObjectId plId;
	pPline->setColorIndex(icolor);	
	plId=AddEntityToDbs(pPline);

}

//////////////////////////////////////////////////////////////////////////
void JD_CENTER::GetQxTZDStr(int i,ACHAR chr[])
{
	ACHAR ch[20],tch[20];
	int j;

	_tcscpy(chr, L"");
	if(i==0)
		_tcscpy(chr, L"QD");
	else if(i==XYNum-1)
		_tcscpy(chr, L"ZD");
	else if (i>1 && i<=XYNum-1)
	{
		j=(int)(XYArray[i][0]+0.01);
		if (j>=3)
			_tcscpy(tch, L"H");
		else if (j==1) //直线
			_tcscpy(tch, L"Z");
		else if (j==2) 
			_tcscpy(tch, L"Y"); 

		j=(int)(XYArray[i-1][0]+0.01);
		if (j>=3) { _tcscpy(ch, L" S"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch, L" H"); };
		if (j==1) { _tcscpy(ch, L" T"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch, L" Z"); };
		if (j==2) { _tcscpy(ch, L" C"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch, L" Y"); };
		_tcscpy(chr,ch);_tcscat(chr,tch);


		if (_tcsncmp(chr, L" YY",3)==0) {  _tcscpy(chr, L" GQ");   };
		if (_tcsncmp(chr, L" C.C.",5)==0) {  _tcscpy(chr, L" P.C.C.");};
		if (_tcsncmp(chr, L" S.S.",5)==0&&(j==3||j==4) ) {  _tcscpy(chr, L" PRC");};
		if (_tcsncmp(chr, L" HH",3)==0&&(j==3||j==4))
		{
			//_tcscpy(chr, L" FGD");
			_tcscpy(chr, L" GQ");
		};
	}

}
//  由断链数据表 求某现场里程的统一里程
double JD_CENTER::TYLC(CString ckml)
{   
	double TYLCB=0,LCB, LCE,TLC,XLC;  // LCB 断链的统一里程
	int iDL=0,bz;
	//起点里程
	CString GH;
	split_ckml(ckml,&XLC,GH);
	TLC=XLC;
	bz=-99;
	for (iDL=0;iDL<DLNum;iDL++)   // 
	{
		//该断链点统一里程
		TYLCB=DLArray[iDL].TYLC;

		//该断链后链里程
		LCB=DLArray[iDL].ELC;

		//下一断链的前链里程
		if(iDL<DLNum-1)LCE=DLArray[iDL+1].BLC; 
		if( ( iDL<DLNum-1 && XLC>=LCB && XLC<=LCE ) || (iDL==DLNum-1 && XLC>=LCB) )  // 落在该段落内
		{   
			if( bz<0) {  TLC= TYLCB+XLC-LCB;  bz=1; }  // 第一次求到
			else 
			{ 
				bz=2;
				//				ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
				//					TLC,TYLCB+XLC-LCB);  
				if(_tcscmp(GH,DLArray[iDL].EGH)==0)TLC=TYLCB+XLC-LCB;
			}
		}
	}
	return TLC;     
}

void JD_CENTER::XYArrayToJD()//线元转交点
{
	//对线元数组循环
	//	CString fname;
	//	fname.Format(L"c:\\resB.%s" , RoadName);
	//	fpw = _wfopen(fname, L"w");

	//	fwprintf(fpw, L"%d\n", JdCenNum);

	//	for(int i=0; i<=XYNum; i++)
	//	{
	//		//	fwprintf(fpw, L"%d %lf %lf %lf %lf %lf\n",i,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,JdCenArray[i].R,JdCenArray[i].A2);
	//		ads_printf(L"1######### %d %d ",XYNum,i);
	//		for(int j=0;j<10;j++)
	//			ads_printf(L"%lf ",XYArray[i][j]);
	//		ads_printf(L"\n");
	//	}
	int i;
	//	fclose(fpw);
	if(XYNum<1) return;

	int CurJDXYNum;
	//交点圆心数组,首尾为起终点
	JdCenArray.RemoveAll();
	JdCenter jddata;
	//先付起点

	jddata.N = XYArray[0][1];
	jddata.E = XYArray[0][2];
	jddata.A1 = XYArray[0][4];
	jddata.A2=jddata.R=0.0;
	jddata.IsOrNotHuiTou=false;
	jddata.JDXZ = IsJiaoDian;
	jddata.LinkAfter = NotRelation;
	jddata.JDN = jddata.N;
	jddata.JDE = jddata.E;
	JdCenArray.Add(jddata);

	int cirxynum;
	CurJDXYNum = 0;
	cirxynum = 0;
	double jd[2];
	double spt[2],sfwj,ept[2],efwj;
	double XYlen,aa,prefwj;
	int QorH;//0-前,1-后
	for( i=1; i<XYNum; i++)
	{
		//		ads_printf(L"@@@@@@@%d\n",i);
		//		 for(int j=0;j<10;j++)
		//		 {
		//			ads_printf(L"%lf ",XYArray[i][j]);
		//		 }
		//		 ads_printf(L"\n");
		//取出第一个线元
		if(fabs(XYArray[i][0]-1.0)<0.001)//直线
			continue;
		else if(fabs(XYArray[i][0]-2.0)<0.001)//当前为圆
		{			
			//判别后接曲线性质以决定是否增加控制点 
			if(i<XYNum)
			{
				if(fabs(XYArray[i+1][0]-1.0)<0.001 || 
					fabs(XYArray[i+1][0])<0.001 || 
					fabs(XYArray[i+1][0]-2.0)<0.001 ||
					(XYArray[i+1][0]>2.1 && XYArray[i][4]*XYArray[i+1][4]<-0.001)||
					(XYArray[i+1][0]>4.1 && XYArray[i][4]*XYArray[i+1][4]>0.0)

					)//后接直线或者圆
				{
					//计算交点
					spt[0] = XYArray[i][8],spt[1] = XYArray[i][9];
					sfwj = XYArray[i][5];
					ept[0] = XYArray[i+1][8],ept[1] = XYArray[i+1][9];
					efwj = XYArray[i+1][5];
					CalXYJD(jd,spt,sfwj,ept,efwj);//计算线元交点                        
					jddata.N = jd[0];
					jddata.E = jd[1];
					jddata.A1 = 0.0;
					jddata.A2=0.0;
					jddata.R = XYArray[i][1]; 
					XYlen = XYArray[i+1][6] - XYArray[i][6];
					aa = XYlen / jddata.R;
					//判别是否回头
					if(aa>pi)
						jddata.IsOrNotHuiTou=true;
					else
						jddata.IsOrNotHuiTou=false;

					jddata.JDXZ = IsJiaoDian;
					if(jddata.IsOrNotHuiTou)//回头还要判别
					{
						//判别前向后向
						int njd;
						njd = JdCenArray.GetSize();
						QorH = 0;//前
						if(njd>0)
						{
							xyddaa(JdCenArray[njd-1].N,JdCenArray[njd-1].E,jddata.N,jddata.E,&prefwj);

							if(fabs(sfwj-prefwj)<0.01)
								QorH = 0;//前
							else
								QorH = 1;//后
						}
						if(QorH == 0)//前)
						{
							if(XYArray[i][4]<0)
								jddata.IsOrNotHuiTou = 1;
							else
								jddata.IsOrNotHuiTou = 2;
						}
						else
						{
							if(XYArray[i][4]<0)
								jddata.IsOrNotHuiTou = 3;
							else
								jddata.IsOrNotHuiTou = 4;

						}

					}
					jddata.LinkAfter = NotRelation;
					if(XYArray[i+1][0]>4.1)
					{
						jddata.LinkAfter = Luan_Link;
						jddata.IsOrNotHuiTou=false;
						jddata.JDXZ = IsCenter;
						jddata.N = spt[0] + jddata.R*cos(sfwj + XYArray[i][4]*0.5*pi);
						jddata.E = spt[1] + jddata.R*sin(sfwj + XYArray[i][4]*0.5*pi);
						jddata.LeftOrRight = XYArray[i][4];
					}

					if(JdCenArray.GetSize()>1 && JdCenArray[JdCenArray.GetSize()-1].LinkAfter==Luan_Link)
					{
						jddata.IsOrNotHuiTou=false;
						jddata.JDXZ = IsCenter;
						jddata.N = spt[0] + jddata.R*cos(sfwj + XYArray[i][4]*0.5*pi);
						jddata.E = spt[1] + jddata.R*sin(sfwj + XYArray[i][4]*0.5*pi);
						jddata.LeftOrRight = XYArray[i][4];
					}
					jddata.JDN = jddata.N;
					jddata.JDE = jddata.E;					
					JdCenArray.Add(jddata);
					//					ads_printf(L"1:size=%d\n",JdCenArray.GetSize());
				}
				else//后接完整缓和曲线 且同向
				{
					//计算交点
					double hhsfwj,hhzxj;
					spt[0] = XYArray[i][8],spt[1] = XYArray[i][9];
					sfwj = XYArray[i][5];
					if(i<XYNum-1)
					{
						ept[0] = XYArray[i+2][8],ept[1] = XYArray[i+2][9];
						efwj = XYArray[i+2][5];
						hhsfwj = XYArray[i+1][5];
					}
					else
					{
						ept[0] = XYArray[XYNum][8],ept[1] = XYArray[XYNum][9];
						efwj = XYArray[XYNum][5];
						hhsfwj = XYArray[XYNum-1][5];
					}
					CalXYJD(jd,spt,sfwj,ept,efwj);//计算线元交点                        
					jddata.N = jd[0];
					jddata.E = jd[1];
					jddata.A1 = 0.0;
					jddata.A2=XYArray[i+1][1];
					jddata.R = XYArray[i][1]; 
					//判别是否回头
					XYlen = XYArray[i+1][6] - XYArray[i][6];
					aa = XYlen / jddata.R;
					hhzxj = fabs(ZXJ(efwj,hhsfwj));
					aa += hhzxj;

					if(aa>pi)
						jddata.IsOrNotHuiTou=true;
					else
						jddata.IsOrNotHuiTou=false;

					jddata.JDXZ = IsJiaoDian;
					if(jddata.IsOrNotHuiTou)//回头还要判别
					{
						//判别前向后向
						int njd;
						njd = JdCenArray.GetSize();
						QorH = 0;//前
						if(njd>0)
						{
							xyddaa(JdCenArray[njd-1].N,JdCenArray[njd-1].E,jddata.N,jddata.E,&prefwj);

							if(fabs(sfwj-prefwj)<0.01)
								QorH = 0;//前
							else
								QorH = 1;//后
						}
						if(QorH == 0)//前)
						{
							if(XYArray[i][4]<0)
								jddata.IsOrNotHuiTou = 1;
							else
								jddata.IsOrNotHuiTou = 2;
						}
						else
						{
							if(XYArray[i][4]<0)
								jddata.IsOrNotHuiTou = 3;
							else
								jddata.IsOrNotHuiTou = 4;

						}

					}
					jddata.LinkAfter = NotRelation;

					if(JdCenArray.GetSize()>1 && JdCenArray[JdCenArray.GetSize()-1].LinkAfter==Luan_Link)
					{
						jddata.IsOrNotHuiTou=false;
						jddata.JDXZ = IsCenter;
						jddata.N = spt[0] + jddata.R*cos(sfwj + XYArray[i][4]*0.5*pi);
						jddata.E = spt[1] + jddata.R*sin(sfwj + XYArray[i][4]*0.5*pi);
						jddata.LeftOrRight = XYArray[i][4];
					}
					jddata.JDN = jddata.N;
					jddata.JDE = jddata.E;
					JdCenArray.Add(jddata);
					//					ads_printf(L"2:size=%d\n",JdCenArray.GetSize());
					i=i+1;
				}
			}
		}
		else//当前为缓和曲线 
		{
			if(i<XYNum)
			{
				if(fabs(XYArray[i+1][0]-1.0)<0.001 || 
					fabs(XYArray[i+1][0])<0.001 || 
					fabs(XYArray[i+1][0]-4.0)<0.001 || 
					XYArray[i][4]*XYArray[i+1][4]<-0.001)//后接直线或者反向
				{
					//计算交点
					spt[0] = XYArray[i][8],spt[1] = XYArray[i][9];
					sfwj = XYArray[i][5];
					ept[0] = XYArray[i+1][8],ept[1] = XYArray[i+1][9];
					efwj = XYArray[i+1][5];
					CalXYJD(jd,spt,sfwj,ept,efwj);//计算线元交点                        
					jddata.N = jd[0];
					jddata.E = jd[1];

					jddata.A1 = XYArray[i][1];
					jddata.A2=0.0;
					if(fabs(XYArray[i][0]-4.0)<0.01)
					{
						jddata.A2 = XYArray[i][1];
						jddata.A1=0.0;
					}
					if(XYArray[i][0]==3.0||XYArray[i][0]==4.0)
						jddata.R = XYArray[i][2]; 
					else if(XYArray[i][0]==5.0)
						jddata.R = XYArray[i][3]; 
					else if(XYArray[i][0]==6.0)
						jddata.R = XYArray[i][2]; 

					//判别是否回头（单缓）
					//	if(CalIfHuiTOU(sfwj,efwj,XYArray[i][4]))
					//    					jddata.IsOrNotHuiTou=true;
					//					else
					//                        jddata.IsOrNotHuiTou=false;

					jddata.JDXZ = IsJiaoDian;
					jddata.LinkAfter = NotRelation;
					if(JdCenArray.GetSize()>1 && JdCenArray[JdCenArray.GetSize()-1].LinkAfter==Luan_Link)
					{
						jddata.IsOrNotHuiTou=false;
						jddata.JDXZ = IsCenter;
						jddata.N = spt[0] + jddata.R*cos(sfwj + XYArray[i][4]*0.5*pi);
						jddata.E = spt[1] + jddata.R*sin(sfwj + XYArray[i][4]*0.5*pi);
						jddata.LeftOrRight = XYArray[i][4];
					}

					//JdCenArray.Add(jddata);
					jddata.JDN = jddata.N;
					jddata.JDE = jddata.E;
					JdCenArray.Add(jddata);
					//					ads_printf(L"3:size=%d\n",JdCenArray.GetSize());
				}
				else//同向
				{
					int add;
					//计算交点
					if(XYArray[i+1][0]>2.1)//后接同向缓和曲线
					{
						spt[0] = XYArray[i][8],spt[1] = XYArray[i][9];
						sfwj = XYArray[i][5];
						if(i<XYNum-1)
						{
							ept[0] = XYArray[i+2][8],ept[1] = XYArray[i+2][9];
							efwj = XYArray[i+2][5];
						}
						else
						{
							ept[0] = XYArray[XYNum][8],ept[1] = XYArray[XYNum][9];
							efwj = XYArray[XYNum][5];
						}
						CalXYJD(jd,spt,sfwj,ept,efwj);//计算线元交点                        
						jddata.N = jd[0];
						jddata.E = jd[1];
						jddata.A1 = XYArray[i][1];
						jddata.A2 = XYArray[i+1][1];
						jddata.R = XYArray[i][2]; 

						//判别是否回头(双缓)
						//	if(CalIfHuiTOU(sfwj,efwj,XYArray[i][4]))
						//							jddata.IsOrNotHuiTou=true;
						//						else
						//							jddata.IsOrNotHuiTou=false;

						jddata.JDXZ = IsJiaoDian;
						jddata.LinkAfter = NotRelation;
						if(JdCenArray.GetSize()>1 && JdCenArray[JdCenArray.GetSize()-1].LinkAfter==Luan_Link)
						{
							jddata.IsOrNotHuiTou=false;
							jddata.JDXZ = IsCenter;
							jddata.N = XYArray[i+1][8] + jddata.R*cos(XYArray[i+1][5] + XYArray[i][4]*0.5*pi);
							jddata.E = XYArray[i+1][9] + jddata.R*sin(XYArray[i+1][5] + XYArray[i][4]*0.5*pi);
							jddata.LeftOrRight = XYArray[i][4];
						}
						jddata.JDN = jddata.N;
						jddata.JDE = jddata.E;						
						JdCenArray.Add(jddata);
						//						ads_printf(L"4:size=%d\n",JdCenArray.GetSize());
						i=i+1;
					}
					else//后接同向圆曲线
					{
						spt[0] = XYArray[i][8],spt[1] = XYArray[i][9];
						sfwj = XYArray[i][5];
						double hhefwj,hhzxj,hhsfwj;

						if(i<XYNum-1 && XYArray[i+2][0]>2.1 && XYArray[i+2][0]<4.1 && XYArray[i+2][4]*XYArray[i+1][4]>0.0001)
							//缓圆缓组合
						{
							if(i<XYNum-2)
							{
								ept[0] = XYArray[i+3][8],ept[1] = XYArray[i+3][9];
								efwj = XYArray[i+3][5];
							}
							else
							{
								ept[0] = XYArray[XYNum][8],ept[1] = XYArray[XYNum][9];
								efwj = XYArray[XYNum][5];
							}
							CalXYJD(jd,spt,sfwj,ept,efwj);//计算线元交点                        
							jddata.N = jd[0];
							jddata.E = jd[1];
							jddata.A1 = XYArray[i][1];
							jddata.A2 = XYArray[i+2][1];
							jddata.R = XYArray[i+1][1]; 
							add=2;
							//判别是否回头
							hhefwj = XYArray[i+1][5];
							hhzxj = fabs(ZXJ(hhefwj,sfwj));

							XYlen = XYArray[i+2][6] - XYArray[i+1][6];
							aa = XYlen / jddata.R;
							aa += hhzxj;

							hhsfwj = XYArray[i+2][5];
							hhzxj = fabs(ZXJ(efwj,hhsfwj));
							aa += hhzxj;


							if(aa>pi)
								jddata.IsOrNotHuiTou=true;
							else
								jddata.IsOrNotHuiTou=false;

							jddata.JDXZ = IsJiaoDian;
							if(jddata.IsOrNotHuiTou)//回头还要判别
							{
								//判别前向后向
								int njd;
								njd = JdCenArray.GetSize();
								QorH = 0;//前
								if(njd>0)
								{
									xyddaa(JdCenArray[njd-1].N,JdCenArray[njd-1].E,jddata.N,jddata.E,&prefwj);

									if(fabs(sfwj-prefwj)<0.01)
										QorH = 0;//前
									else
										QorH = 1;//后
								}
								if(QorH == 0)//前)
								{
									if(XYArray[i][4]<0)
										jddata.IsOrNotHuiTou = 1;
									else
										jddata.IsOrNotHuiTou = 2;
								}
								else
								{
									if(XYArray[i][4]<0)
										jddata.IsOrNotHuiTou = 3;
									else
										jddata.IsOrNotHuiTou = 4;

								}

							}

							jddata.JDXZ = IsJiaoDian;
							jddata.LinkAfter = NotRelation;
							if(JdCenArray.GetSize()>1 && JdCenArray[JdCenArray.GetSize()-1].LinkAfter==Luan_Link)
							{
								jddata.IsOrNotHuiTou=false;
								jddata.JDXZ = IsCenter;
								jddata.N = XYArray[i+1][8] + XYArray[i+1][1]*cos(XYArray[i+1][5] + XYArray[i+1][4]*0.5*pi);
								jddata.E = XYArray[i+1][9] + XYArray[i+1][1]*sin(XYArray[i+1][5] + XYArray[i+1][4]*0.5*pi);
								jddata.LeftOrRight = XYArray[i+1][4];
							}

						}
						else//缓+圆
						{
							if(i<XYNum-1)
							{
								ept[0] = XYArray[i+2][8],ept[1] = XYArray[i+2][9];
								efwj = XYArray[i+2][5];
							}
							else
							{
								ept[0] = XYArray[XYNum][8],ept[1] = XYArray[XYNum][9];
								efwj = XYArray[XYNum][5];
							}
							CalXYJD(jd,spt,sfwj,ept,efwj);//计算线元交点                        
							jddata.N = jd[0];
							jddata.E = jd[1];
							jddata.A1 = XYArray[i][1];
							jddata.A2 = 0.0;
							jddata.R = XYArray[i+1][1]; 
							add=1;

							hhefwj = XYArray[i+1][5];
							hhzxj = fabs(ZXJ(hhefwj,sfwj));

							XYlen = XYArray[i+2][6] - XYArray[i+1][6];
							aa = XYlen / jddata.R;
							aa += hhzxj;

							if(aa>pi)
								jddata.IsOrNotHuiTou=true;
							else
								jddata.IsOrNotHuiTou=false;

							jddata.JDXZ = IsJiaoDian;
							if(jddata.IsOrNotHuiTou)//回头还要判别
							{
								//判别前向后向
								int njd;
								njd = JdCenArray.GetSize();
								QorH = 0;//前
								if(njd>0)
								{
									xyddaa(JdCenArray[njd-1].N,JdCenArray[njd-1].E,jddata.N,jddata.E,&prefwj);

									if(fabs(sfwj-prefwj)<0.01)
										QorH = 0;//前
									else
										QorH = 1;//后
								}
								if(QorH == 0)//前)
								{
									if(XYArray[i][4]<0)
										jddata.IsOrNotHuiTou = 1;
									else
										jddata.IsOrNotHuiTou = 2;
								}
								else
								{
									if(XYArray[i][4]<0)
										jddata.IsOrNotHuiTou = 3;
									else
										jddata.IsOrNotHuiTou = 4;

								}

							}							
							jddata.JDXZ = IsJiaoDian;
							jddata.LinkAfter = NotRelation;
							if(i<XYNum-1 && XYArray[i+2][0]>4.1)
							{
								jddata.LinkAfter = Luan_Link;
								jddata.IsOrNotHuiTou=false;
								jddata.JDXZ = IsCenter;
								jddata.N = XYArray[i+1][8] + XYArray[i+1][1]*cos(XYArray[i+1][5] + XYArray[i+1][4]*0.5*pi);
								jddata.E = XYArray[i+1][9] + XYArray[i+1][1]*sin(XYArray[i+1][5] + XYArray[i+1][4]*0.5*pi);
								jddata.LeftOrRight = XYArray[i+1][4];
							}
							if(JdCenArray.GetSize()>1 && JdCenArray[JdCenArray.GetSize()-1].LinkAfter==Luan_Link)
							{
								jddata.IsOrNotHuiTou=false;
								jddata.JDXZ = IsCenter;
								jddata.N = XYArray[i+1][8] + XYArray[i+1][1]*cos(XYArray[i+1][5] + XYArray[i+1][4]*0.5*pi);
								jddata.E = XYArray[i+1][9] + XYArray[i+1][1]*sin(XYArray[i+1][5] + XYArray[i+1][4]*0.5*pi);
								jddata.LeftOrRight = XYArray[i+1][4];
							}


						}

						//判别是否回头
						jddata.JDN = jddata.N;
						jddata.JDE = jddata.E;
						JdCenArray.Add(jddata);
						//		                ads_printf(L"5:size=%d\n",JdCenArray.GetSize());
						i = i+add;

					}
				}
			}
		}
	}//end for
	//付终点
	jddata.N = XYArray[XYNum][8];
	jddata.E = XYArray[XYNum][9];
	jddata.A1 = 0.0;
	jddata.A2=jddata.R=0.0;
	jddata.IsOrNotHuiTou=false;
	jddata.JDXZ = IsJiaoDian;
	jddata.LinkAfter = NotRelation;
	jddata.JDN = jddata.N;
	jddata.JDE = jddata.E;
	JdCenArray.Add(jddata);
	JdCenNum = JdCenArray.GetSize();
	//	ads_printf(L"jdn=%d\n",JdCenNum);
	//	FILE *fpw;	

	for(i=0; i<JdCenNum; i++)
	{    
		JdCenArray[i].JdNo=i;
		//		ads_printf(L"2: %d %lf %lf %lf %lf %lf\n",i,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,JdCenArray[i].R,JdCenArray[i].A2);
	}


	/*
	CString fname;
	fname.Format(L"c:\\res.%s" , RoadName);
	fpw = _wfopen(fname, L"w");
	fwprintf(fpw, L"%d\n", JdCenNum);
	for(i=0; i<JdCenNum; i++)
	{
	fwprintf(fpw, L"%d %lf %lf %lf %lf %lf\n",i,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,JdCenArray[i].R,JdCenArray[i].A2);
	ads_printf(L"2: %d %lf %lf %lf %lf %lf\n",i,JdCenArray[i].N,JdCenArray[i].E,JdCenArray[i].A1,JdCenArray[i].R,JdCenArray[i].A2);
	}
	fclose(fpw);*/

}

void JD_CENTER::CalXYJD(double jd[2],double Spt[2],double SFwj,double Ept[2],double EFwj)//计算线元交点
{
	double pt1[2],pt2[2];
	pt1[0] = Spt[0] + 1000000*cos(SFwj);
	pt1[1] = Spt[1] + 1000000*sin(SFwj);
	pt2[0] = Ept[0] + 1000000*cos(EFwj+PI);
	pt2[1] = Ept[1] + 1000000*sin(EFwj+PI);
	INTERS(&jd[0],&jd[1],Spt[0],Spt[1],pt1[0],pt1[1],Ept[0],Ept[1],pt2[0],pt2[1]);
}

int JD_CENTER::CalIfHuiTOU(double sfwj,double efwj,double zx)
{
	double zxj;
	zxj = ZXJ(efwj,sfwj);
	if(zx<0.0)//左转
	{
		if(zxj>0.0)
			zxj = -(2*PI - zxj);
	}
	else
	{
		if(zxj<0.0)
			zxj = 2*PI - zxj;
	}

	if(fabs(zxj)>PI)
		return 1;
	return 0;
}


void JD_CENTER::SetXYArray(double xyarr[3][10])
{
	//2005.4.25 hhp
	IfCalXY = FALSE;
	SHOWJD = FALSE;
	RouteItem item;
	XYNum = RIAarray.GetSize();
	if (XYNum == 0)
	{		
		item.v0 = xyarr[0][0];
		item.v1 = xyarr[0][1];
		item.v2 = xyarr[0][2];
		item.v3 = xyarr[0][3];
		item.v4 = xyarr[0][4];
		item.v5 = xyarr[0][5];
		item.v6 = xyarr[0][6];
		item.v7 = xyarr[0][7];
		item.v8 = xyarr[0][8];
		item.v9 = xyarr[0][9];
		RIAarray.Add(item);

		item.v0 = xyarr[1][0];
		item.v1 = xyarr[1][1];
		item.v2 = xyarr[1][2];
		item.v3 = xyarr[1][3];
		item.v4 = xyarr[1][4];
		item.v5 = xyarr[1][5];
		item.v6 = xyarr[1][6];
		item.v7 = xyarr[1][7];
		item.v8 = xyarr[1][8];
		item.v9 = xyarr[1][9];
		RIAarray.Add(item);

		item.v0 = xyarr[2][0];
		item.v1 = xyarr[2][1];
		item.v2 = xyarr[2][2];
		item.v3 = xyarr[2][3];
		item.v4 = xyarr[2][4];
		item.v5 = xyarr[2][5];
		item.v6 = xyarr[2][6];
		item.v7 = xyarr[2][7];
		item.v8 = xyarr[2][8];
		item.v9 = xyarr[2][9];
		RIAarray.Add(item);
		XYNum = RIAarray.GetSize();

		if(XYArray)
		{
			delete []XYArray;
			XYArray=NULL;
		}		
		XYArray = new double[XYNum][10];
		for(int i=0;i<XYNum;i++)
		{
			XYArray[i][0] = (RIAarray.GetAt(i)).v0;
			XYArray[i][1] = (RIAarray.GetAt(i)).v1;
			XYArray[i][2] = (RIAarray.GetAt(i)).v2;
			XYArray[i][3] = (RIAarray.GetAt(i)).v3;
			XYArray[i][4] = (RIAarray.GetAt(i)).v4;
			XYArray[i][6] = (RIAarray.GetAt(i)).v6;
			XYArray[i][5] = (RIAarray.GetAt(i)).v5;
			XYArray[i][7] = (RIAarray.GetAt(i)).v7;
			XYArray[i][8] = (RIAarray.GetAt(i)).v8;
			XYArray[i][9] = (RIAarray.GetAt(i)).v9;	
		}
	}
	//计算线路长;
	if (fabs(XYArray[1][0]-1) < DataPrecision)
	{
		XLLength = XYArray[1][1];
	}
	else if(fabs(XYArray[1][0]-2) < DataPrecision)
	{
		XLLength = XYArray[1][2];
	}
	else if ((fabs(XYArray[1][0]-3) < DataPrecision) ||
		(fabs(XYArray[1][0]-4) < DataPrecision))
	{
		XLLength = XYArray[1][1]*XYArray[1][1]/XYArray[1][2];  //A*A/R
	}
	else
	{
		XLLength = XYArray[1][1]*XYArray[1][1]*fabs(1/XYArray[1][2]-1/XYArray[1][3]);
	}
	//初始化断链;
	DLArray[1].TYLC = XLLength;
	DLArray[1].ELC = XLLength;
	DLArray[1].BLC = XLLength;

	DLArray[0].TYLC = 0;
	DLArray[0].ELC = 0;
	DLArray[0].BLC = 0;

	XYNum -= 1;
}

//2005.4.28 hhp
void JD_CENTER::AddXYArray(double xyarr[3][10])
{
	IfCalXY = FALSE;
	SHOWJD = FALSE;
	UpdateDataRIArray();
	//删去最后结束线元;
	XYNum = RIAarray.GetSize();
	RIAarray.RemoveAt(XYNum-1);
	//计算删除被接线元后的线路长;
	if (fabs(XYArray[XYNum-2][0]-1) < DataPrecision)
	{
		XLLength -= XYArray[XYNum-2][1];
	}
	else if(fabs(XYArray[XYNum-2][0]-2) < DataPrecision)
	{
		XLLength -= XYArray[XYNum-2][2];
	}
	else if ((fabs(XYArray[XYNum-2][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-2][0]-4) < DataPrecision))
	{
		XLLength -= XYArray[XYNum-2][1]*XYArray[XYNum-2][1]/XYArray[XYNum-2][2];  //A*A/R
	}
	else
	{
		XLLength -= XYArray[XYNum-2][1]*XYArray[XYNum-2][1]*
			fabs(1/XYArray[XYNum-2][2]-1/XYArray[XYNum-2][3]);
	}
	//删去被接线元;
	XYNum = RIAarray.GetSize();
	RIAarray.RemoveAt(XYNum-1);
	RouteItem item;
	item.v0 = xyarr[0][0];
	item.v1 = xyarr[0][1];
	item.v2 = xyarr[0][2];
	item.v3 = xyarr[0][3];
	item.v4 = xyarr[0][4];
	item.v5 = xyarr[0][5];
	item.v6 = xyarr[0][6];
	item.v7 = xyarr[0][7];
	item.v8 = xyarr[0][8];
	item.v9 = xyarr[0][9];
	RIAarray.Add(item);

	item.v0 = xyarr[1][0];
	item.v1 = xyarr[1][1];
	item.v2 = xyarr[1][2];
	item.v3 = xyarr[1][3];
	item.v4 = xyarr[1][4];
	item.v5 = xyarr[1][5];
	item.v6 = xyarr[1][6];
	item.v7 = xyarr[1][7];
	item.v8 = xyarr[1][8];
	item.v9 = xyarr[1][9];
	RIAarray.Add(item);

	item.v0 = xyarr[2][0];
	item.v1 = xyarr[2][1];
	item.v2 = xyarr[2][2];
	item.v3 = xyarr[2][3];
	item.v4 = xyarr[2][4];
	item.v5 = xyarr[2][5];
	item.v6 = xyarr[2][6];
	item.v7 = xyarr[2][7];
	item.v8 = xyarr[2][8];
	item.v9 = xyarr[2][9];
	RIAarray.Add(item);	
	if(XYArray)
	{
		delete []XYArray;
		XYArray=NULL;
	}
	XYNum = RIAarray.GetSize();	
	XYArray = new double[XYNum][10];

	for(int i=0;i<XYNum;i++)
	{
		XYArray[i][0] = (RIAarray.GetAt(i)).v0;
		XYArray[i][1] = (RIAarray.GetAt(i)).v1;
		XYArray[i][2] = (RIAarray.GetAt(i)).v2;
		XYArray[i][3] = (RIAarray.GetAt(i)).v3;
		XYArray[i][4] = (RIAarray.GetAt(i)).v4;
		XYArray[i][5] = (RIAarray.GetAt(i)).v5;
		XYArray[i][6] = (RIAarray.GetAt(i)).v6;
		XYArray[i][7] = (RIAarray.GetAt(i)).v7;
		XYArray[i][8] = (RIAarray.GetAt(i)).v8;
		XYArray[i][9] = (RIAarray.GetAt(i)).v9;
	}
	//计算添加两个线元后的线路长;
	if (fabs(XYArray[XYNum-3][0]-1) < DataPrecision)
	{
		XLLength += XYArray[XYNum-3][1];
	}
	else if(fabs(XYArray[XYNum-3][0]-2) < DataPrecision)
	{
		XLLength += XYArray[XYNum-3][2];
	}
	else if ((fabs(XYArray[XYNum-3][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-3][0]-4) < DataPrecision))
	{
		XLLength += XYArray[XYNum-3][1]*XYArray[XYNum-3][1]/XYArray[XYNum-3][2];  //A*A/R
	}
	else
	{
		XLLength += XYArray[XYNum-3][1]*XYArray[XYNum-3][1]*
			fabs(1/XYArray[XYNum-3][2]-1/XYArray[XYNum-3][3]);
	}
	if (fabs(XYArray[XYNum-2][0]-1) < DataPrecision)
	{
		XLLength += XYArray[XYNum-2][1];
	}
	else if(fabs(XYArray[XYNum-2][0]-2) < DataPrecision)
	{
		XLLength += XYArray[XYNum-2][2];
	}
	else if ((fabs(XYArray[XYNum-2][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-2][0]-4) < DataPrecision))
	{
		XLLength += XYArray[XYNum-2][1]*XYArray[XYNum-2][1]/XYArray[XYNum-2][2];  //A*A/R
	}
	else
	{
		XLLength += XYArray[XYNum-2][1]*XYArray[XYNum-2][1]*
			fabs(1/XYArray[XYNum-2][2]-1/XYArray[XYNum-2][3]);
	}
	//重置断链表;
	DLArray[1].TYLC = XLLength;
	DLArray[1].ELC = XLLength;
	DLArray[1].BLC = XLLength;

	XYNum -= 1;
}

void JD_CENTER::UpdateDataRIArray()
{
	//2005.4.30 hhp;
	//XYNum = RIAarray.GetSize()-1;
	RIAarray.RemoveAll();
	RouteItem item;
	for(int i=0;i<=XYNum;i++)
	{		
		item.v0 = XYArray[i][0];
		item.v1 = XYArray[i][1];
		item.v2 = XYArray[i][2];
		item.v3 = XYArray[i][3];
		item.v4 = XYArray[i][4];
		item.v5 = XYArray[i][5];
		item.v6 = XYArray[i][6];
		item.v7 = XYArray[i][7];
		item.v8 = XYArray[i][8];
		item.v9 = XYArray[i][9];
		RIAarray.Add(item);
	}
}

void JD_CENTER::ShowXY(AcGiWorldDraw* pWd)
{
	CString huhu;
	int i=0,j=0;
	xlpoint PB,PE;
	double INDATA[5];
	// ??????
	PB.x=XYArray[0][1]; //N
	PB.y=XYArray[0][2]; // E
	PB.a=XYArray[0][3];// FA
	PB.lc=XYArray[0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	XYNum = RINum;
	for(i=1;i<XYNum;i++)
	{
		for (j=0;j<5;j++) INDATA[j] = XYArray[i][j];
		PB.a=XYArray[i][5];
		PB.x=XYArray[i][8];
		PB.y=XYArray[i][9];
		if(pWd==NULL) GUXWXY(INDATA,&PB,&PE,1); //?????·????

		else
		{
			int icolor=-1;
			int XYClass=(int)(XYArray[i][0]+0.001);
			double Rmin,A;
			if(XYClass==3)//?°??
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][2];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			else if(XYClass==4)//?ó??
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][2];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			else if(XYClass==5)//5-???ê????R?ó->R??
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][3];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}
			else if(XYClass==6)// 6-???ê????R??->R?ó
			{
				A=XYArray[i][1];
				Rmin=XYArray[i][2];
				if(A<Rmin/3.0 || A>Rmin)icolor=1;
			}			
			G_WXY(pWd,INDATA,&PB,&PE,icolor); //?????·????
		}
	}
	BZLC(pWd,100.0,DrawLeftOrRight,1,1);//×??·???ò??-1?±???ò  
	BZTZD(pWd,0/*-1*DrawLeftOrRight*/,1);

	xlpoint  PZtmp; 
	AcGePoint3d  ptb,pte,Pt;
	AcGePoint3d MidPt,StartPt,EndPt,APt1,APt2;//???·×??ò
	double B;
	XYNum--;
	double xl_length=XYArray[XYNum][6]-XYArray[1][6];
	PZtmp.lc=XYArray[1][6]+0.5*xl_length;// ????????
	xlpoint_pz(&PZtmp);  
	B=PZtmp.a;
	pte[X]=PZtmp.y+xl_length*sin(B+pi*0.5)/4.0;
	pte[Y]=PZtmp.x+xl_length*cos(B+pi*0.5)/4.0;
	ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x;
	//////////////////////////////////////////////////////////////////////////
	//???·×??ò
	if(pWd)
	{
		MidPt.x=0.5*(ptb.x+pte.x);
		MidPt.y=0.5*(ptb.y+pte.y);
		MidPt.z=0.0;

		StartPt.x=MidPt.x-xl_length*sin(B)/6.0;
		StartPt.y=MidPt.y-xl_length*cos(B)/6.0;
		StartPt.z=0.0;

		EndPt.x=MidPt.x+xl_length*sin(B)/6.0;
		EndPt.y=MidPt.y+xl_length*cos(B)/6.0;
		EndPt.z=0.0;

		double ArrowJ=0.03*PI;
		double ArrowLen=xl_length/15.0;
		APt1.x=EndPt.x+ArrowLen*sin(B+PI+ArrowJ);
		APt1.y=EndPt.y+ArrowLen*cos(B+PI+ArrowJ);
		APt1.z=0.0;

		APt2.x=EndPt.x+ArrowLen*sin(B+PI-ArrowJ);
		APt2.y=EndPt.y+ArrowLen*cos(B+PI-ArrowJ);
		APt2.z=0.0;

		AcGePoint3d ArrowEndPt;
		ArrowEndPt.x=EndPt.x+0.6*ArrowLen*sin(B+PI);
		ArrowEndPt.y=EndPt.y+0.6*ArrowLen*cos(B+PI);
		ArrowEndPt.z=0.0;
		int icolor=1;
		if(FLDNo>0)icolor=0;
		G_makeline(pWd,StartPt,EndPt,icolor);
		G_makeline(pWd,APt1,EndPt,icolor);
		G_makeline(pWd,APt2,EndPt,icolor);		 
		G_makeline(pWd,APt1,ArrowEndPt,icolor);
		G_makeline(pWd,APt2,ArrowEndPt,icolor);
		//////////////////////////////////////////////////////////////////////////		
		G_makeline(pWd,ptb,pte,icolor);
		Pt[Y]=pte[Y]+xl_length*cos(B)/20.0;
		Pt[X]=pte[X]+xl_length*sin(B)/20.0;
		ptb[Y]=pte[Y]+xl_length*cos(B+pi)/20.0;
		ptb[X]=pte[X]+xl_length*sin(B+pi)/20.0;
		G_makeline(pWd,Pt,ptb,icolor);
		pte[Y]=Pt[Y]+xl_length*cos(B+pi*0.5)/20.0;
		pte[X]=Pt[X]+xl_length*sin(B+pi*0.5)/20.0;
		G_makeline(pWd,Pt,pte,icolor);
		Pt[Y]=ptb[Y]+xl_length*cos(B+pi*0.5)/20.0;
		Pt[X]=ptb[X]+xl_length*sin(B+pi*0.5)/20.0;
		G_makeline(pWd,ptb,Pt,icolor);
		pte[Y]=Pt[Y]+xl_length*cos(B)/10.0;
		pte[X]=Pt[X]+xl_length*sin(B)/10.0;
		G_makeline(pWd,Pt,pte,icolor);
	}
}

void JD_CENTER::ReplaceItem(double xyarr[3][10])
{	
	//除去倒数第二个有效线元长度;
	if (fabs(XYArray[XYNum-2][0]-1) < DataPrecision)
	{
		XLLength -= XYArray[XYNum-2][1];
	}
	else if(fabs(XYArray[XYNum-2][0]-2) < DataPrecision)
	{
		XLLength -= XYArray[XYNum-2][2];
	}
	else if ((fabs(XYArray[XYNum-2][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-2][0]-4) < DataPrecision))
	{
		XLLength -= XYArray[XYNum-2][1]*XYArray[XYNum-2][1]/XYArray[XYNum-2][2];  //A*A/R
	}
	else
	{
		XLLength -= XYArray[XYNum-2][1]*XYArray[XYNum-2][1]*
			fabs(1/XYArray[XYNum-2][2]-1/XYArray[XYNum-2][3]);
	}	
	//除去倒数第一个有效线元长度;
	if (fabs(XYArray[XYNum-1][0]-1) < DataPrecision)
	{
		XLLength -= XYArray[XYNum-1][1];
	}
	else if(fabs(XYArray[XYNum-1][0]-2) < DataPrecision)
	{
		XLLength -= XYArray[XYNum-1][2];
	}
	else if ((fabs(XYArray[XYNum-1][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-1][0]-4) < DataPrecision))
	{
		XLLength -= XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/XYArray[XYNum-1][2];  //A*A/R
	}
	else
	{
		XLLength -= XYArray[XYNum-1][1]*XYArray[XYNum-1][1]*
			fabs(1/XYArray[XYNum-1][2]-1/XYArray[XYNum-1][3]);
	}	
	for(int i=XYNum-2; i<=XYNum; i++)
	{
		XYArray[i][0] = xyarr[i+2-XYNum][0];
		XYArray[i][1] = xyarr[i+2-XYNum][1];
		XYArray[i][2] = xyarr[i+2-XYNum][2];
		XYArray[i][3] = xyarr[i+2-XYNum][3];
		XYArray[i][4] = xyarr[i+2-XYNum][4];
		XYArray[i][5] = xyarr[i+2-XYNum][5];
		XYArray[i][6] = xyarr[i+2-XYNum][6];
		XYArray[i][7] = xyarr[i+2-XYNum][7];
		XYArray[i][8] = xyarr[i+2-XYNum][8];
		XYArray[i][9] = xyarr[i+2-XYNum][9];
	}
	//计算线路长;
	//加上倒数第二个有效线元长度;
	if (fabs(XYArray[XYNum-2][0]-1) < DataPrecision)
	{
		XLLength += XYArray[XYNum-2][1];
	}
	else if(fabs(XYArray[XYNum-2][0]-2) < DataPrecision)
	{
		XLLength += XYArray[XYNum-2][2];
	}
	else if ((fabs(XYArray[XYNum-2][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-2][0]-4) < DataPrecision))
	{
		XLLength += XYArray[XYNum-2][1]*XYArray[XYNum-2][1]/XYArray[XYNum-2][2];  //A*A/R
	}
	else
	{
		XLLength += XYArray[XYNum-2][1]*XYArray[XYNum-2][1]*
			fabs(1/XYArray[XYNum-2][2]-1/XYArray[XYNum-2][3]);
	}
	//加上倒数第一个有效线元长度;
	if (fabs(XYArray[XYNum-1][0]-1) < DataPrecision)
	{
		XLLength += XYArray[XYNum-1][1];
	}
	else if(fabs(XYArray[XYNum-1][0]-2) < DataPrecision)
	{
		XLLength += XYArray[XYNum-1][2];
	}
	else if ((fabs(XYArray[XYNum-1][0]-3) < DataPrecision) ||
		(fabs(XYArray[XYNum-1][0]-4) < DataPrecision))
	{
		XLLength += XYArray[XYNum-1][1]*XYArray[XYNum-1][1]/XYArray[XYNum-1][2];  //A*A/R
	}
	else
	{
		XLLength += XYArray[XYNum-1][1]*XYArray[XYNum-1][1]*
			fabs(1/XYArray[XYNum-1][2]-1/XYArray[XYNum-1][3]);
	}
	//重置断链表;
	DLArray[1].TYLC = XLLength;
	DLArray[1].ELC = XLLength;
	DLArray[1].BLC = XLLength;
}
//计算直线与线路的交点
void JD_CENTER::CalJD_ZX_XL(double Spt[2],double fwj,double PtOnXL[2],double &cml)	
{
	int JDSum;
	double ResPt[2],ResPt1[2];
	Ptstr Apt;
	CArray<Ptstr,Ptstr> PtArr;


	ResPt[0]=0.0;
	ResPt[1]=0.0;
	ResPt1[0]=0.0;
	ResPt1[1]=0.0;

	PtArr.RemoveAll();



	//构建线元数据
	//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大

	//(R1>R2)/////////////////////////////////////////////////////////////////////////
	//1:			L		R    A         A              A                A        //
	//2:					L	 R         R              R1               R2       //
	//3:                                                  R2               R1       //
	//////////////////////////////////////////////////////////////////////////////////
	//4: 左右转
	//5: 起始切线方位角
	//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
	//7: 半径 R
	//8，9：线元起点坐标 N,E
	//最后一个线元为终点
	//0:0
	double Ept[2];
	Ept[0]=Spt[0]+1000.0*cos(fwj);
	Ept[1]=Spt[1]+1000.0*sin(fwj);

	double XY1[10];
	XY1[0]=1.0;
	XY1[2]=XY1[3]=XY1[4]=XY1[6]=0.0;
	XY1[1]=xyddaa(Spt[0],Spt[1],Ept[0],Ept[1],&XY1[5]);
	XY1[7]=1.0E50;
	XY1[8]=Spt[0],XY1[9]=Spt[1];

	JDSum=0;
	int XYSum = XYNum;
	int i;

	double res[3],res1[3];
	for( i=1; i<XYSum; i++)
	{			
		if(XYArray[i][0]<1.1)//直线
			JDSum = CalJD_ZX_ZX(XY1,XYArray[i],ResPt);
		else if(XYArray[i][0]<2.1)//圆
			JDSum = CalJD_ZX_ARC(XY1,XYArray[i],ResPt,ResPt1);
		else
		{
			JDSum = CalJD_ZX_Spiral(XY1,XYArray[i],res,res1);
			ResPt[0]=res[0],ResPt[1]=res[1];
			ResPt1[0]=res1[0],ResPt1[1]=res1[1];
		}
		if(JDSum>0)
		{
			if(JDSum==1)
			{
				Apt.pt[0] = ResPt[0],Apt.pt[1]=ResPt[1];
				PtArr.Add(Apt);
			}
			else if(JDSum==2)
			{
				double dis1,dis2;
				dis1 = sqrt((Spt[0]-ResPt[0])*(Spt[0]-ResPt[0]));
				dis2 = sqrt((Spt[0]-ResPt1[0])*(Spt[0]-ResPt1[0]));
				if(dis1 < dis2)
				{
					Apt.pt[0]=ResPt[0],Apt.pt[1]=ResPt[1];
					PtArr.Add(Apt);
				}
				else
				{
					Apt.pt[0]=ResPt1[0],Apt.pt[1]=ResPt1[1];
					PtArr.Add(Apt);
				}
			}
		}
	}
	if(PtArr.GetSize()>0)
	{
		double len,dis = 99999999999;

		PtOnXL[0]=PtArr[0].pt[0],PtOnXL[1]=PtArr[0].pt[1];
		for(i=0; i<PtArr.GetSize(); i++)
		{
			len = sqrt((PtArr[i].pt[0]-Spt[0])*(PtArr[i].pt[0]-Spt[0])+(PtArr[i].pt[1]-Spt[1])*(PtArr[i].pt[1]-Spt[1]));
			if(len<dis)
			{
				dis = len;
				PtOnXL[0]=PtArr[i].pt[0],PtOnXL[1]=PtArr[i].pt[1];
			}
		}
		cml = PROJ_ML(PtOnXL[0],PtOnXL[1]);
	}
	else 
	{
		cml = PROJ_ML(Spt[0],Spt[1]);
		xlpoint PZ;
		PZ.lc = cml;
		xlpoint_pz(&PZ);
		PtOnXL[0]=PZ.x,PtOnXL[1]=PZ.y;
	}		
	PtArr.RemoveAll();
}

void JD_CENTER::ReadJK()
{
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(mdbname, L"左加宽分段表",RoadName);
	ZjkNm = xlmdb.NJK;
	if(ZjkNm>0)
	{
		Zjk = new CgSZ[ZjkNm];
		for(int i=0;i<ZjkNm;i++)
		{
			Zjk[i] = xlmdb.CG[i];	
			Zjk[i].cml = TYLC(Zjk[i].xlc);
		}
	}

	xlmdb.Read_XLDbs(mdbname, L"右加宽分段表",RoadName);
	YjkNm = xlmdb.NJK;
	if(YjkNm>0)
	{
		Yjk = new CgSZ[YjkNm];
		for(int i=0;i<YjkNm;i++)
		{
			Yjk[i] = xlmdb.CG[i];	
			Yjk[i].cml = TYLC(Yjk[i].xlc);
		}
	}

}
// 按W3,W4解算鼻端圆心坐标
void JD_CENTER::CalNoseCen(JD_CENTER *pm2,double sml1,int QorH,double W3,double W4,							  
						   double &CmlonZX,double &CmlOnYX,double NosePt[2])
{
	xlpoint PZ, PZ2;    // 试算鼻端点在主线和右线上的投影点  
	double pt[2];       // 试算鼻端点
	double cml1, cml2;  // 包含NosePt的迭代区间
	double predis, dis = 0.0; // pt到pm2的距离
	double cmlonyx;

	cml1 = sml1;
	cml2 = sml1 + QorH*10.0;

	// 找到包含NosePt的迭代区间(cml1,cml2)
	while(cml2 < this->XLLength)
	{
		// 1.计算1线偏离点pt
		PZ.lc = cml2;
		this->xlpoint_pz(&PZ);
		int LorR = this->Judge2XLorR(PZ,pm2->XYArray,pm2->XYNum);
		pt[0] = PZ.x + W3 * cos(PZ.a + LorR*0.5*pi);
		pt[1] = PZ.y + W3 * sin(PZ.a + LorR*0.5*pi);

		// 2.计算pt到pm2的距离
		cmlonyx = pm2->PROJ_ML(pt[0],pt[1]);
		PZ2.lc = cmlonyx;
		pm2->xlpoint_pz(&PZ2);
		predis = dis;
		dis = sqrt((PZ2.x-pt[0])*(PZ2.x-pt[0])+(PZ2.y-pt[1])*(PZ2.y-pt[1]));
		if(dis > W4)
		{
			break; // 找到包含NosePt的迭代区间(cml1,cml2)
		}
		else
		{
			cml1 = cml2;
			cml2 += QorH*10.0;
		}
	}

	while(fabs(cml2-cml1)>0.001 && fabs(dis-W4)>0.0001)
	{
		// 1.计算1线偏离点pt
		PZ.lc = 0.5*(cml1+cml2);
		this->xlpoint_pz(&PZ);
		int LorR = this->Judge2XLorR(PZ,pm2->XYArray,pm2->XYNum);
		pt[0] = PZ.x + W3 * cos(PZ.a + LorR*0.5*pi);
		pt[1] = PZ.y + W3 * sin(PZ.a + LorR*0.5*pi);

		// 2.计算pt到pm2的距离
		cmlonyx = pm2->PROJ_ML(pt[0],pt[1]);
		PZ2.lc = cmlonyx;
		pm2->xlpoint_pz(&PZ2);
		dis = sqrt((PZ2.x-pt[0])*(PZ2.x-pt[0])+(PZ2.y-pt[1])*(PZ2.y-pt[1]));
		if(dis > W4)
		{
			cml2 = PZ.lc;
		}
		else
		{
			cml1 = PZ.lc;
		}
	}

	// 3.计算鼻端圆心坐标
	if(fabs(dis-W4) < 0.0001)
	{
		CmlonZX = 0.5*(cml1+cml2);
		PZ.lc = CmlonZX; 
		this->xlpoint_pz(&PZ);
		int LorR = this->Judge2XLorR(PZ,pm2->XYArray,pm2->XYNum);
		pt[0] = PZ.x + W3 * cos(PZ.a + LorR*0.5*pi);
		pt[1] = PZ.y + W3 * sin(PZ.a + LorR*0.5*pi);

		CmlOnYX = pm2->PROJ_ML(pt[0],pt[1]);
		NosePt[0] = pt[0];
		NosePt[1] = pt[1];
	}
	else
	{
		CmlOnYX = -1;	
		acutPrintf(L"没有找到鼻端!\n");
	}
}

void JD_CENTER::CalNoseCen(JD_CENTER *pm2,double sml1,int QorH,double C1,double C2,int YTinf,
						   double Rnose,
						   double &CmlonZX,double &CmlOnYX,double NosePt[2])
{
	//1.计算1线偏离点pt;
	double pt[2],pt2[2];
	xlpoint PZ,PZ2;
	double cml1,cml2,dis,cmlonyx,predis;
	double Offset1,Offset2;
	double half_fgdk,lydk,cxdk,yljk,tljk,tljhp;
	int bkmode,sjbgmode;

	ReadLMK("左路面宽分段表");
	ReadLMK("右路面宽分段表");
	ReadJK();

	pm2->ReadLMK("左路面宽分段表");
	pm2->ReadLMK("右路面宽分段表");
	pm2->ReadJK();

	cml1 = sml1;
	cml2 = sml1 + QorH*10.0;
	dis = 0.0;
	Offset2 = 100;
	//先搜索区间cml1,cml2
	while(cml2<this->XLLength&&dis<(Offset2+Rnose))
	{
		PZ.lc = cml2;

		this->xlpoint_pz(&PZ);

		int LorR = this->Judge2XLorR(PZ,pm2->XYArray,pm2->XYNum);


		if(LorR>0)//取右路面宽
		{
			if(NYLMK<1)
			{
				ads_printf(L"请先输入%s-%s右侧路面宽数据!\n",mdbname,RoadName);
				return;
			}
			CalLMK_JK(cml2,this->NYLMK,this->YLMK,this->YjkNm,this->Yjk,
				half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
			CalLMKbyFile(cml2,1,half_fgdk,lydk,cxdk,yljk,tljk);
		}
		else
		{
			if(NZLMK<1)
			{
				ads_printf(L"请先输入%s-%s左侧路面宽数据!\n",mdbname,RoadName);
				return;
			}
			CalLMK_JK(cml2,this->NZLMK,this->ZLMK,this->ZjkNm,this->Zjk,
				half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
			CalLMKbyFile(cml2,-1,half_fgdk,lydk,cxdk,yljk,tljk);
		}

		if(YTinf==0)//硬路肩
		{			
			Offset1 = half_fgdk+lydk+cxdk+yljk+C1;
		}
		else
			Offset1 = half_fgdk+lydk+cxdk+yljk+tljk;

		pt[0] = PZ.x + (Offset1+Rnose) * cos(PZ.a + LorR*0.5*pi);
		pt[1] = PZ.y + (Offset1+Rnose) * sin(PZ.a + LorR*0.5*pi);
		//2.计算pt到pm2的距离;
		cmlonyx = pm2->PROJ_ML(pt[0],pt[1]);

		if(LorR>0)//取左路面宽
		{
			if(pm2->NZLMK<1)
			{
				ads_printf(L"请先输入%s-%s左侧路面宽数据!\n",pm2->mdbname,pm2->RoadName);
				return;
			}
			CalLMK_JK(cmlonyx,pm2->NZLMK,pm2->ZLMK,pm2->ZjkNm,pm2->Zjk,
				half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
			CalLMKbyFile(cmlonyx,-1,half_fgdk,lydk,cxdk,yljk,tljk);
		}
		else
		{
			if(pm2->NZLMK<1)
			{
				ads_printf(L"请先输入%s-%s右侧路面宽数据!\n",pm2->mdbname,pm2->RoadName);
				return;
			}
			CalLMK_JK(cmlonyx,pm2->NYLMK,pm2->YLMK,pm2->YjkNm,pm2->Yjk,
				half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
			CalLMKbyFile(cmlonyx,1,half_fgdk,lydk,cxdk,yljk,tljk);
		}

		if(YTinf==0)//硬路肩
			Offset2 = half_fgdk+lydk+cxdk+yljk+C2;
		else
			Offset2 = half_fgdk+lydk+cxdk+yljk+tljk;

		PZ2.lc = cmlonyx;
		pm2->xlpoint_pz(&PZ2);

		pt2[0]=PZ2.x,pt2[1]=PZ2.y;
		predis = dis;
		dis = sqrt((pt2[0]-pt[0])*(pt2[0]-pt[0])+(pt2[1]-pt[1])*(pt2[1]-pt[1]));

		if(dis>Offset2+Rnose)
			break;
		else
		{
			cml1 = cml2;
			cml2 += QorH*10.0;
		}

	}


	CString GH;
	while(fabs(cml2-cml1)>0.001&&fabs(dis-Offset2-Rnose)>0.0001)
	{
		PZ.lc = 0.5*(cml1+cml2);
		this->xlpoint_pz(&PZ);
		int LorR = this->Judge2XLorR(PZ,pm2->XYArray,pm2->XYNum);

		if(LorR>0)//取右路面宽
			CalLMK_JK(PZ.lc,this->NYLMK,this->YLMK,this->YjkNm,this->Yjk,
			half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
		else
			CalLMK_JK(PZ.lc,this->NZLMK,this->ZLMK,this->ZjkNm,this->Zjk,
			half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
		CalLMKbyFile(PZ.lc,LorR,half_fgdk,lydk,cxdk,yljk,tljk);
		if(YTinf==0)//硬路肩	
			Offset1 = half_fgdk+lydk+cxdk+yljk+C1;
		else
			Offset1 = half_fgdk+lydk+cxdk+yljk+tljk;	

		pt[0] = PZ.x + (Offset1+Rnose) * cos(PZ.a + LorR*0.5*pi);
		pt[1] = PZ.y + (Offset1+Rnose) * sin(PZ.a + LorR*0.5*pi);
		//2.计算pt到pm2的距离;
		cmlonyx = pm2->PROJ_ML(pt[0],pt[1]);
		if(LorR>0)//取右路面宽
			CalLMK_JK(cmlonyx,pm2->NZLMK,pm2->ZLMK,pm2->ZjkNm,pm2->Zjk,
			half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
		else
			CalLMK_JK(cmlonyx,pm2->NYLMK,pm2->YLMK,pm2->YjkNm,pm2->Yjk,
			half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
		CalLMKbyFile(cmlonyx,LorR,half_fgdk,lydk,cxdk,yljk,tljk);
		if(YTinf==0)//硬路肩
			Offset2 = half_fgdk+lydk+cxdk+yljk+C2;
		else
			Offset2 = half_fgdk+lydk+cxdk+yljk+tljk;


		PZ2.lc = cmlonyx;
		pm2->xlpoint_pz(&PZ2);
		pt2[0]=PZ2.x,pt2[1]=PZ2.y;
		dis = sqrt((pt2[0]-pt[0])*(pt2[0]-pt[0])+(pt2[1]-pt[1])*(pt2[1]-pt[1]));
		if(dis>Offset2+Rnose)
			cml2 = PZ.lc;
		else
			cml1 = PZ.lc;

	}

	if(fabs(dis-Offset2-Rnose)<0.001)
	{
		CmlonZX = 0.5*(cml1+cml2);
		PZ.lc = CmlonZX; 
		this->xlpoint_pz(&PZ);
		int LorR = this->Judge2XLorR(PZ,pm2->XYArray,pm2->XYNum);
		pt[0] = PZ.x + (Offset1+Rnose) * cos(PZ.a + LorR*0.5*pi);
		pt[1] = PZ.y + (Offset1+Rnose) * sin(PZ.a + LorR*0.5*pi);
		//2.计算pt到pm2的距离;
		CmlOnYX = pm2->PROJ_ML(pt[0],pt[1]);

		NosePt[0] = pt[0];
		NosePt[1] = pt[1];
	}
	else
		CmlOnYX = -1;		
}
void JD_CENTER::CalCG(double projml,double& CG)
{
	CG=0;
	if(ZcgNm>0)
	{
		for (int i=0;i<ZcgNm-1;i++)
		{
			if (projml>Zcg[i].cml-0.1&&projml<Zcg[i+1].cml+0.1)
			{
				CG=Zcg[i].cg+(Zcg[i].cg-Zcg[i+1].cg)*(projml-Zcg[i].cml)/(Zcg[i+1].cml-projml);
				CG=fabs(CG);
			}
		}
	}
}
void JD_CENTER::ReadCG()
{
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(mdbname, L"左超高分段表",RoadName);
	ZcgNm = xlmdb.NCG;
	if(ZcgNm>0)
	{
		Zcg = new CgSZ[ZcgNm];
		for(int i=0;i<ZcgNm;i++)
		{
			Zcg[i] = xlmdb.CG[i];	
			Zcg[i].cml = TYLC(Zcg[i].xlc);
		}
	}

	xlmdb.Read_XLDbs(mdbname, L"右超高分段表",RoadName);
	YcgNm = xlmdb.NCG;
	if(YcgNm>0)
	{
		Ycg = new CgSZ[YcgNm];
		for(int i=0;i<YcgNm;i++)
		{
			Ycg[i] = xlmdb.CG[i];	
			Ycg[i].cml = TYLC(Ycg[i].xlc);
		}
	}
}
void JD_CENTER::CalPZXPT(double cml,double jd,double dis,double pt[2],double &prjml)
{
	//1计算直线
	double spt[2],ept[2],dis1,dis2,mdis,xpt[2],mpt[2];
	xlpoint PZ;
	double projml;


	PZ.lc = cml;
	xlpoint_pz(&PZ);
	spt[0]=PZ.x,spt[1]=PZ.y;
	ept[0] = spt[0] + 50.0*cos(PZ.a + jd*pi/180.0);
	ept[1] = spt[1] + 50.0*sin(PZ.a + jd*pi/180.0);
	//2取两点计算距中线距离
	dis1=0.0;
	projml=PROJ_ML(ept[0],ept[1]);
	PZ.lc = projml;
	xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	dis2 = sqrt((xpt[0]-ept[0])*(xpt[0]-ept[0])+(xpt[1]-ept[1])*(xpt[1]-ept[1]));
	//3二分叠代计算出pt
	mpt[0] = 0.5*(spt[0]+ept[0]);
	mpt[1] = 0.5*(spt[1]+ept[1]);
	projml=PROJ_ML(mpt[0],mpt[1]);
	PZ.lc = projml;
	xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	mdis = sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1]));

	while(fabs(mdis-dis)>0.0001)
	{
		if(mdis>dis)
			ept[0]=mpt[0],ept[1]=mpt[1];
		else
			spt[0]=mpt[0],spt[1]=mpt[1];
		mpt[0] = 0.5*(spt[0]+ept[0]);
		mpt[1] = 0.5*(spt[1]+ept[1]);
		projml=PROJ_ML(mpt[0],mpt[1]);
		PZ.lc = projml;
		xlpoint_pz(&PZ);
		xpt[0]=PZ.x,xpt[1]=PZ.y;
		mdis = sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1]));			
	}
	pt[0]=mpt[0],pt[1]=mpt[1];
	prjml = projml;
}

void JD_CENTER::CalLMKbyFile(double cml,int ZorY,double &half_fgdk,double &lydk,double &cxdk,double &yljk,double &tljk)
{
	int i,j;
	double ToZFD,ToLYD,ToCXD,ToYLJ,ToTlj;
	AcGePoint2d Apt;

	ToZFD = half_fgdk;
	ToLYD = half_fgdk+lydk;
	ToCXD = half_fgdk+lydk+cxdk;
	ToYLJ = ToCXD + yljk;
	ToTlj = ToYLJ + tljk;
	xlpoint PZ;
	PZ.lc = cml;
	xlpoint_pz(&PZ);
	double x1,y1,x2,y2,dis;
	x1 = PZ.x;
	y1 = PZ.y;
	x2 = x1 + 100*cos(PZ.a+ZorY*PI*0.5);
	y2 = y1 + 100*sin(PZ.a+ZorY*PI*0.5);

	for(i=0;i<BxNum;i++)
	{
		if(cml>pBXArr[i].bxsml&&cml<pBXArr[i].bxeml&&ZorY==pBXArr[i].LorR)
		{
			AcDbPolyline *pPline=new AcDbPolyline(pBXArr[i].ptnum);
			for(j =0;j<pBXArr[i].ptnum;j++)
			{
				Apt.x = pBXArr[i].ptarr[j*3];
				Apt.y = pBXArr[i].ptarr[j*3+1];
				pPline->addVertexAt(j,Apt);
				pPline->setWidthsAt(j,0.0,0.0);
			}

			AcDbLine *Line=new AcDbLine(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));		
			AcGePoint3dArray interPt;
			Line->intersectWith(pPline,AcDb::kOnBothOperands,interPt,0,0);
			if(interPt.length()>0)
			{
				double dx,dy;
				dx = interPt[0].x-x1, dy = interPt[0].y-y1;
				dis = sqrt(dx*dx+dy*dy);
				if(pBXArr[i].bxinf==0)
					ToZFD = dis;
				else if(pBXArr[i].bxinf==1)
					ToLYD = dis;
				else if(pBXArr[i].bxinf==2)
					ToCXD = dis;
				else if(pBXArr[i].bxinf==3)
					ToYLJ = dis;
				else
					ToTlj = dis;
			}
		}
	}

	half_fgdk = ToZFD;
	lydk = ToLYD - ToZFD;
	cxdk = ToCXD - ToLYD;
	yljk = ToYLJ - ToCXD;
	tljk = ToTlj - ToYLJ;
}

int JD_CENTER::GetInOrOut(int ZDStOrEd,JD_CENTER *pm2)
{
	double sml,eml,dx,dy,dis;

	if(ZDStOrEd==0)//匝道起端
	{
		sml = PROJ_ML(pm2->XYArray[1][8],pm2->XYArray[1][9]);
		dx = pm2->XYArray[2][8]-pm2->XYArray[1][8];
		dy = pm2->XYArray[2][9]-pm2->XYArray[1][9];
		dis = sqrt(dx*dx+dy*dy);
		if(dis>0.5)
			eml = PROJ_ML(pm2->XYArray[2][8],pm2->XYArray[2][9]);
		else
		{
			xlpoint PZ;
			PZ.lc = pm2->XYArray[1][6]+5;
			pm2->xlpoint_pz(&PZ);
			eml = PROJ_ML(PZ.x,PZ.y);
		}
		if(eml>sml)
			return 1;//在主线上向里程增加方向找边坡分离点
		else
			return -1;//在主线上向里程减小方向找边坡分离点
	}
	else//匝道终端
	{
		eml = PROJ_ML(pm2->XYArray[pm2->XYNum][8],pm2->XYArray[pm2->XYNum][9]);
		dx = pm2->XYArray[pm2->XYNum][8]-pm2->XYArray[pm2->XYNum-1][8];
		dy = pm2->XYArray[pm2->XYNum][9]-pm2->XYArray[pm2->XYNum-1][9];
		dis = sqrt(dx*dx+dy*dy);
		if(dis>0.5)
			sml = PROJ_ML(pm2->XYArray[pm2->XYNum-1][8],pm2->XYArray[pm2->XYNum-1][9]);
		else
		{
			xlpoint PZ;
			PZ.lc = pm2->XYArray[pm2->XYNum][6]-5;
			pm2->xlpoint_pz(&PZ);
			sml = PROJ_ML(PZ.x,PZ.y);
		}

		if(sml<eml)
			return -1;//在主线上向里程减小方向找边坡分离点
		else
			return 1;//在主线上向里程增加方向找边坡分离点
	}
}
//给定直线,线路平面,在直线上找到一点到线路距离等于给定宽度(求直线与平面中线的平行线交点)
//cml:里程 angle: 角度(弧度) 直线确定
//dis:宽度
//返回交点及在投影里程
bool JD_CENTER::CalPXXJD(double cml,double angle,double dis,double pt[2],double &prjml,double &newWD)
{
	//1计算直线
	double spt[2],ept[2],dis1,dis2,mdis,xpt[2],mpt[2];
	xlpoint PZ;
	double projml,bml,bdis;

	newWD = -1.0;
	PZ.lc = cml;
	xlpoint_pz(&PZ);
	spt[0]=PZ.x,spt[1]=PZ.y;

	ept[0] = spt[0] + 1.2*dis*cos(PZ.a + angle);
	ept[1] = spt[1] + 1.2*dis*sin(PZ.a + angle);

	//2取两点计算距中线距离
	dis1=0.0;
	projml=PROJ_ML(ept[0],ept[1]);
	PZ.lc = projml;
	xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	dis2 = sqrt((xpt[0]-ept[0])*(xpt[0]-ept[0])+(xpt[1]-ept[1])*(xpt[1]-ept[1]));
	//3二分叠代计算出pt
	mpt[0] = 0.5*(spt[0]+ept[0]);
	mpt[1] = 0.5*(spt[1]+ept[1]);
	projml=PROJ_ML(mpt[0],mpt[1]);
	PZ.lc = projml;
	xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	mdis = sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1]));
	bml = projml;
	bdis = mdis;

	while(fabs(mdis-dis)>0.0001)
	{
		if(mdis>dis)
			ept[0]=mpt[0],ept[1]=mpt[1];
		else
			spt[0]=mpt[0],spt[1]=mpt[1];
		mpt[0] = 0.5*(spt[0]+ept[0]);
		mpt[1] = 0.5*(spt[1]+ept[1]);
		bml = projml;
		projml=PROJ_ML(mpt[0],mpt[1]);
		PZ.lc = projml;
		xlpoint_pz(&PZ);
		xpt[0]=PZ.x,xpt[1]=PZ.y;
		bdis = mdis;
		mdis = sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1]));			

		if((bdis-mdis)*(bml-projml)<0.0)//不单调
			return false;

		if(fabs(bml-projml)<0.0001 || (fabs(mdis-bdis)<0.0001))
			break;

	}
	pt[0]=mpt[0],pt[1]=mpt[1];
	prjml = projml;

	PZ.lc = cml;
	xlpoint_pz(&PZ);
	spt[0]=PZ.x,spt[1]=PZ.y;
	newWD = sqrt((pt[0]-spt[0])*(pt[0]-spt[0])+(pt[1]-spt[1])*(pt[1]-spt[1]));

	return true;
}
//判别cml所在线元的转向
int JD_CENTER::FindWichZorY(double cml)
{
	int ZorY;//0-直线，-1-左转，1-右转
	ZorY = 0;//

	double l,dlc,lc;
	int i,j,k,bz=-99;
	double INDATA2[5],jslc;
	struct xlpoint PB0,PEZ,*PBI,*PEI;
	PB0.lc=XYArray[0][4];
	jslc=cml;
	if(jslc > XYArray[XYNum][6])
		jslc = XYArray[XYNum][6];
	PBI=&PB0;PEI=&PEZ; 
	i=0; bz=99;
	do
	{ 
		i=i+1; 
		if (i>=XYNum) bz=-99;
		if (bz>0)
		{
			// 第 i 线元
			for (k=0;k<5;k++) INDATA2[k]=XYArray[i][k];
			l=Get_INDATA_L(INDATA2,&j);
			if(i>=XYNum-1)
				l += 0.1;//最后一个线元加一个毫米

			PBI->lc = XYArray[i][6];
			// l 为该线元的长度 m

			if( ( jslc <= PBI->lc+l ) && (jslc>=PBI->lc) ) 
				// 计算点在 i 线元上
			{ 
				if(j==1)//直线
					ZorY = 0;
				else
				{
					if(XYArray[i][4]<0.0)
						ZorY = -1;
					else
						ZorY = 1;
				}

				k=999;
				bz=-99;
			};
		}  // bz>0
	}while (bz>0);

	if (k==999) 
		return ZorY; //有解 j 为 线元类型
	else
	{  	
		return (-9); // 无解
	};
	return ZorY;
}


int JD_CENTER::Adjust_L_JDLuan(double aa,double R,double RB,double Lo1,double RE,double Lo2,double T,int qorh)
{
	double T1,T2,L;
	double l11,l12;

	int num=0;
	l11=0.01,l12=2*R;
	for(;;)
	{
		num++;
		if(fabs(l11-l12)<PRECISION)
			return 0;
		double l10=0.5*(l11+l12);
		////求圆心//////////////////////////////////////////////////////////////////////
		double tempT;
		if(qorh==-1)//调整前lo
			get_T_Len(dms_rad(aa),R,RB,l10,RE,Lo2,tempT,T2,L);
		else
			get_T_Len(dms_rad(aa),R,RB,Lo1,RE,l10,T1,tempT,L);

		if(fabs(tempT-T)<PRECISION)return l10;
		else
		{
			if(tempT<T)
				l11 = l10;
			else 
				l12 = l10;
		}
		if(num >1000)
			return 0;
	}

}
//初步计算所有交点的切长
void JD_CENTER::Cal_CurJD_QXSE(int JDno,double& sprojml,double& eprojml)
{
	sprojml=eprojml=0;
	double PreN,PreE,CurN,CurE,AftN,AftE,ZHN,ZHE,HZN,HZE;
	double PreL,AftL,T1,T2;
	if (JDno<=0||JDno>=JdCenNum-1)
	{
		return;
	}
	PreN=JdCenArray[JDno-1].N;
	PreE=JdCenArray[JDno-1].E;
	CurN=JdCenArray[JDno].N;
	CurE=JdCenArray[JDno].E;
	AftN=JdCenArray[JDno+1].N;
	AftE=JdCenArray[JDno+1].E;
	T1=JdCenArray[JDno].T1;
	T2=JdCenArray[JDno].T2;
	PreL=sqrt((PreN-CurN)*(PreN-CurN)+(PreE-CurE)*(PreE-CurE));
	AftL=sqrt((AftN-CurN)*(AftN-CurN)+(AftE-CurE)*(AftE-CurE));
	ZHN=CurN+(CurN-PreN)*T1/PreL;
	ZHE=CurE+(CurE-PreE)*T1/PreL;
	HZN=CurN+(CurN-AftN)*T2/AftL;
	HZE=CurE+(CurE-AftE)*T2/AftL;
	sprojml=PROJ_ML(ZHN,ZHE);
	eprojml=PROJ_ML(HZN,HZE);
}
//计算所有交点的切线长，并根据连接类型调整缓长？
void JD_CENTER::Cal_AllJD_T()
{
	JdCenter *PreJdCen,*CurJdCen,*AfterJdCen;
	double aa,R,A1,A2,Lo1,Lo2,BPMTL[9],T1,T2,L;
	for(int i=1;i<JdCenNum-1;i++)
	{
		//获取当前JD/CENTER指针
		CurJdCen=&JdCenArray[i];		
		if(CurJdCen->JDXZ==IsJiaoDian)
		{
			PreJdCen=&JdCenArray[i-1];
			AfterJdCen=&JdCenArray[i+1];      
			aa=CurJdCen->afa;
			aa=fabs(aa);
			//求前切、后切、曲线长
			R=CurJdCen->R;
			A1=CurJdCen->A1;
			A2=CurJdCen->A2;
			Lo1=A1*A1/R;
			Lo2=A2*A2/R;
			getBPMTL(dms_rad(aa),R,Lo1,Lo2,&BPMTL[0]);
			T1=BPMTL[6];//前切长
			T2=BPMTL[7];//后切长

			double RB,RE;
			RB=1e50;
			RE=1e50;

			if(PreJdCen->LinkAfter==Luan_Link || CurJdCen->LinkAfter==Luan_Link)
			{
				if(PreJdCen->JDXZ==IsJiaoDian && PreJdCen->LinkAfter==Luan_Link && CurJdCen->A1>1e-6 )
				{
					RB = PreJdCen->R;
					//调整A1
					Lo1 = fabs((CurJdCen->A1*CurJdCen->A1)/RB - (CurJdCen->A1*CurJdCen->A1)/R);
				}
				else
					RB = 1e50;

				if(CurJdCen->JDXZ==IsJiaoDian && CurJdCen->LinkAfter==Luan_Link && CurJdCen->A2>1e-6 )
				{
					RE = AfterJdCen->R;
					Lo2 = fabs((CurJdCen->A2*CurJdCen->A2)/RE - (CurJdCen->A2*CurJdCen->A2)/R);
				}
				else
					RE = 1e50;

				get_T_Len(dms_rad(aa),R,RB,Lo1,RE,Lo2,T1,T2,L);
			}
			

			CurJdCen->T1=T1;
			CurJdCen->T2=T2;
		}
	}
}

void JD_CENTER::TrsBPDArraytoCM(int NBPD, double BPD_array[][4])
{
	double LC1,H1,LC2,H2,R;
	LC1= BPD_array[0][0];
	H1=  BPD_array[0][1];
	m_CM[0] =LC1,m_CM[1]=H1,m_CM[2]=0.0, m_CM[3] = 0.0, m_CM[4]=0.0;

	for(int i=1; i<NBPD; i++)
	{
		LC2= BPD_array[i][0];
		H2=BPD_array[i][1];    
		R =BPD_array[i][2];
		m_CM[i*5]=LC2,m_CM[i*5+1]=H2;
		m_CM[i*5+2]=(H2-H1)/(LC2-LC1);
		m_CM[i*5+3]=LC2-LC1;
		m_CM[i*5+4]=R;
		H1=H2,LC1=LC2;
	}
	m_CM[NBPD*5] =m_CM[NBPD*5+1]=m_CM[NBPD*5+2]=m_CM[NBPD*5+3]=m_CM[NBPD*5+4]=0.0;
	m_NCM = NBPD+1;
}


//计算cml处的高程,坡度
double JD_CENTER::rdes_level(double cml,double &slope)
	/* ml; continue mileage  */
{
	slope = -100.0;
	if (m_NCM==0)
	{
		slope = 0.0;
		return -100.0;
	}
	ACHAR       str[80];
	int   j;
	double     z=-1.0,t1,A,B,C,delta_i,z2,x,sml,eml;
	//--------------------------------------------------------
	sml = m_CM[0];//起始连续里程
	eml =  m_CM[(m_NCM-2)*5];

	str[0]='\0';
	if (cml<sml-0.001 || cml>eml+0.01) {
		//    ads_printf(L"%d %d ML=%0.2lf out of range(%0.3lf---%0.3lf)\n",NBPD,NCM,cml,sml,eml);
		//   ads_alert(str);
		return -100.0;
	}
	else if ( cml<m_CM[0]) 
	{
		z = m_CM[1]-m_CM[2]*(m_CM[0]-cml);
		slope = m_CM[2];
		//     z+= DLEVEL(0, cml,NCM,CM); 
	}
	else if (cml>m_CM[(m_NCM-2)*5+0] ) {
		z= m_CM[(m_NCM-2)*5+1]+m_CM[(m_NCM-1)*5+2]*(cml - m_CM[(m_NCM-2)*5+0]);
		slope = m_CM[(m_NCM-1)*5+2];
		//    z+=DLEVEL(NCM-1,cml,NCM,CM);
	}
	else{
		for(j=1; j<m_NCM-1; j++)
			if (m_CM[(j-1)*5+0]-0.0001<=cml&&cml<=m_CM[j*5+0]+0.0001)
			{
				slope = m_CM[j*5+2];
				if(m_CM[(j-1)*5+4]<0.0)//抛物线
				{
					t1 = -1.0*m_CM[(j-1)*5+4];
					if(cml < m_CM[(j-1)*5]+t1)//处于竖曲线范围内
					{
						z = m_CM[(j-1)*5+1]-m_CM[(j-1)*5+2]*t1;//竖曲线起点高程
						C = z;
						delta_i = m_CM[j*5+2]-m_CM[(j-1)*5+2];
						A = delta_i/4.0/t1;//竖曲线坡度变化率
						z2 = m_CM[(j-1)*5+1]+m_CM[j*5+2]*t1;//竖曲线终点高程
						B = ((z2-z)-t1*delta_i)/2.0/t1;
						x = cml - m_CM[(j-1)*5] + t1;
						z = A*x*x + B*x + C;
						break;
					}
				}
				if(m_CM[j*5+4]<0.0)//抛物线
				{
					t1 = -1.0*m_CM[j*5+4];
					if(cml > m_CM[j*5]-t1)//处于竖曲线范围内
					{
						z = m_CM[j*5+1]-m_CM[j*5+2]*t1;//竖曲线起点高程
						C = z;
						delta_i = m_CM[(j+1)*5+2]-m_CM[j*5+2];
						A = delta_i/4.0/t1;//竖曲线坡度变化率
						z2 = m_CM[j*5+1]+m_CM[(j+1)*5+2]*t1;//竖曲线终点高程
						B = ((z2-z)-t1*delta_i)/2.0/t1;
						x = cml - (m_CM[j*5] - t1);
						z = A*x*x + B*x + C;
						break;
					}
				}

				z = m_CM[(j-1)*5+1]+m_CM[j*5+2]*(cml-m_CM[(j-1)*5+0]);
				z+= DLEVEL(j,cml);
				break;

			}
	}
	return (z);
}

//由CM数组计算cml处的高程
double JD_CENTER::DLEVEL(int icm, double cml)
{
	if (m_NCM==0)
		return -100.0;

	double vr1, vr2, di1,di2, vt1,vt2, ds, ds1, dds,s1,s2;
	int kr;

	if (icm==0) 
	{
		vr1=1.0e8;
		di1=0.0;
		vr2 = m_CM[icm*5+4];
		di2 = (m_CM[(icm+1)*5+2] - m_CM[icm*5+2])*100.0;
		ds=m_CM[0]-cml;
	}
	else if (icm==m_NCM-1) {
		vr1 = m_CM[(icm-1)*5+4];
		di1 = (m_CM[icm*5+2] - m_CM[(icm-1)*5+2])*100.0;
		vr2=1.0e8;
		di2=0.0;
		ds=cml-m_CM[icm*5+0];
	}
	else 
	{
		vr1 = m_CM[(icm-1)*5+4];
		vr2 = m_CM[icm*5+4];
		di1 = (m_CM[icm*5+2] - m_CM[(icm-1)*5+2])*100.0;
		di2 = (m_CM[(icm+1)*5+2] - m_CM[icm*5+2])*100.0;
		ds = cml-m_CM[(icm-1)*5+0];
	}
	sign(s1, di1);
	sign(s2, di2);
	if(vr1>=0.0)
		vt1 = fabs(vr1*di1/200.0);
	else
		vt1 = fabs(vr1);

	if(vr2>=0.0)
		vt2 = fabs(vr2*di2/200.0);
	else
		vt2 = fabs(vr2);
	//      vt2 = fabs(vr2*di2/200.0);

	if (ds<vt1) {
		ds1 = vt1-ds;
		kr  = 1;
	}
	else if (ds>m_CM[icm*5+3]-vt2) {
		ds1 = ds-m_CM[icm*5+3]+vt2;
		kr  = 2;
	}
	else  {
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

void JD_CENTER::DrawInterSect(AcGiWorldDraw* pWd)
{
	double LY = 150;
	double LX = 50;
	int LorR = -1;
	int Color = 0;
	AcGePoint3d CenPt,BefPt,AftPt,CrsPt,MLPt;

	ACHAR MLStr[40];
	ACHAR CnsStr[40];
	_tcscpy(MLStr, L"");
	_tcscpy(CnsStr, L"");
	bool IsAngErr = false;
	bool IsDetaHErr = false;
	double NoteAng;

	NoteAng = FwjtoAngX(m_InterSectData.CurFwj - pi/2.0);

	CenPt[X] = m_InterSectData.interE;
	CenPt[Y] = m_InterSectData.InterN;

	CrsPt[X] = CenPt[X] + LY * sin(m_InterSectData.CurFwj + LorR*0.5*PI);
	CrsPt[Y] = CenPt[Y] + LY * cos(m_InterSectData.CurFwj + LorR*0.5*PI);

	BefPt[X] = CrsPt[X] - LX * sin(m_InterSectData.CurFwj);
	BefPt[Y] = CrsPt[Y] - LX * cos(m_InterSectData.CurFwj);

	AftPt[X] = CrsPt[X] + LX * sin(m_InterSectData.CurFwj);
	AftPt[Y] = CrsPt[Y] + LX * cos(m_InterSectData.CurFwj);

	G_makeline(pWd, CenPt, CrsPt, Color);
	G_makeline(pWd, BefPt, AftPt, Color);

	//道路交叉高程信息
	MLPt[X] = CenPt[X] + 5.0 * sin(m_InterSectData.CurFwj + LorR*0.5*PI);
	MLPt[Y] = CenPt[Y] + 5.0 * cos(m_InterSectData.CurFwj + LorR*0.5*PI);

	NoteAng = FwjtoAngX(m_InterSectData.CurFwj + LorR*0.5*PI);

	double cml = TYLC(m_InterSectData.CurDml);
	_stprintf(MLStr, L"%s  H=%.3lf", XLC(cml), m_InterSectData.CurDesH);
	//20191108修改	调整绘制位置，避免两行信息靠的过近
	MLPt[X] += texth / 2 * sin(m_InterSectData.CurFwj + PI);
	MLPt[Y] += texth / 2 * cos(m_InterSectData.CurFwj + PI);
	G_maketextNew(pWd, MLPt, MLStr, NoteAng, texth, Color);

	//铁路交叉高程信息
	_stprintf(MLStr, L"%s  H=%.3lf", m_InterSectData.InterCkml, m_InterSectData.InterDesH);
	//20191108修改	调整绘制位置，避免两行信息靠的过近
	MLPt[X] += texth * sin(m_InterSectData.CurFwj);
	MLPt[Y] += texth * cos(m_InterSectData.CurFwj);
	G_maketextNew(pWd, MLPt, MLStr, NoteAng, texth, Color, 3);

	//高差信息
	ACHAR Ang[20];
	dms(m_InterSectData.JiaJiao, 0, Ang);
	if ( fabs(m_InterSectData.CurDesH+100)<0.01)
	{
		_stprintf(CnsStr, L"未设置变坡点, α=%s",  Ang);
	}
	else
	{
		_stprintf(CnsStr, L"ΔH=%.3lf, α=%s", m_InterSectData.DetaH, Ang);
	}

	NoteAng = FwjtoAngX(m_InterSectData.CurFwj);
	//20191108新增，调整位置
	CrsPt[X] += texth / 2 * sin(m_InterSectData.CurFwj + LorR * 0.5 * PI);
	CrsPt[Y] += texth / 2 * cos(m_InterSectData.CurFwj + LorR * 0.5 * PI);

	G_maketextNew(pWd, CrsPt, CnsStr, NoteAng, texth, Color, 0);
}

Adesk::Boolean  JD_CENTER::G_maketextNew(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode)
{   

	AcGeVector3d normal(0.0,0.0,1.0), direction(cos(ang), sin(ang), 0.0);
	// ads_printf(L"\n             %10.3f %10.3f %10.4f ",Pt.x,Pt.y,dms_rad(ang));
	pWd->subEntityTraits().setColor(icolor);	
	//    pWd->geometry().text(Pt , normal, direction, texth,1.0, 0.0, ctext);
	AcGiTextStyle pTextStyle;
	//pTextStyle.setFont(L"宋体",TRUE,FALSE,0,0);
	pTextStyle.setFont(L"宋体",0,0,134,2);
	pTextStyle.setTextSize(texth);
	AcGePoint3d pt1;
	double len = _tcslen(ctext) * texth;
	if (mode == 0)//输入点为字中下点时
	{
		Pt.x = Pt.x - 0.5*len*cos(ang);
		Pt.y = Pt.y - 0.5*len*sin(ang);
	}
	else if (mode == 1)//输入点为字左下点时
	{
		Pt.x = Pt.x ;
		Pt.y = Pt.y ;
	}
	else if (mode == 2)//输入点为字右下点时
	{
		Pt.x = Pt.x - len*cos(ang);
		Pt.y = Pt.y - len*sin(ang);
	}
	else if (mode == 3)//输入点为字左上角点
	{
		Pt.x = Pt.x + texth * cos(ang-0.5*PI);
		Pt.y = Pt.y + texth * sin(ang-0.5*PI);
	}
	pWd->geometry().text(Pt,normal,direction,ctext,_tcslen(ctext),Adesk::kTrue,pTextStyle);
	return 1;
}
int JD_CENTER::getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE )
{   
	assertReadEnabled();

	if (JDi>=0 && JDi<=JdCenNum-1)  //  起~终点
	{  
		*NN=JdCenArray[JDi].N;
		*EE=JdCenArray[JDi].E;
		*L1i=JdCenArray[JDi].Ls1;
		*L2i=JdCenArray[JDi].Ls2;
		*Ri=JdCenArray[JDi].R;
	};	 
	return JdCenNum;
}

//20190706	绘制起终点、交点信息函数	为适配标准改路模板	ljx
void JD_CENTER::Draw_PI_Infor(double Scml, double Ecml, double texth, int Decimals, bool IsNoteRAL, bool IsGutter)
{
	//设置参数
	xlpoint  Rail_Pt_Start, Rail_Pt_Mid, Rail_Pt_End;	//线元上的起点、中点、终点	
	int Cir_Color = 3, Text_Color = 1, S_N_E_Color = 6;	//小圆颜色，文字颜色，起终点颜色	暂时按模板写，后续可设置参数自由修改
	ACHAR Layer_Name[256], Draw_Text[256];	//图层名称，暂时缺省设置，后续可设置参数自由修改	线元信息，用于传递绘制文字
	_tcscpy(Layer_Name, L"平面图-交点信息");
	AcGePoint3d tempAcGePt;	//临时3维点，用于传递坐标参数
	double Cir_Radius = 1.0;	//绘制的圆曲线半径
	double INDATA[5];	//本线元的信息
	double WB = 10 * texth;

	//逐个线元循环
	for (int i = 1; i <= XYNum; i++)
	{
		//-1.检测该线元是否在指定绘制范围内
		if (XYArray[i][6] < Scml - 0.01 || XYArray[i][6] > Ecml + 0.01)
			continue;

		//0.获取线元的信息
		Rail_Pt_Start.lc = XYArray[i][6];
		Rail_Pt_Start.x = XYArray[i][8];
		Rail_Pt_Start.y = XYArray[i][9];
		Rail_Pt_Start.a = XYArray[i][5];
		Rail_Pt_Start.r = XYArray[i][7];
		for (int k = 0; k < 5; k++)
			INDATA[k] = XYArray[i][k]; 
		int Cur_Geo_Element = (int)(XYArray[i][0] + 0.01);	//目前的线元种类

		//1.绘制线元起点处的小圆	绿色	半径为1
		tempAcGePt.set(Rail_Pt_Start.y, Rail_Pt_Start.x, 0);
		makecircle(tempAcGePt, Cir_Radius, Cir_Color, Layer_Name);

		//2.线元为圆曲线时，绘制半径信息（缓和曲线是否需要绘制？）	20191231修改	圆曲线半径信息在标注特征桩时标注
		//if (Cur_Geo_Element == 2 || Cur_Geo_Element == 3 || Cur_Geo_Element == 4)
		//{
		//	//①获取线元文字
		//	ACHAR tempACHAR[16];
		//	double Cur_Geo_Element_Length = 0.0;
		//	if (Cur_Geo_Element == 2)	//圆曲线
		//	{
		//		_tcscpy(Draw_Text, L"R-");
		//		Cur_Geo_Element_Length = INDATA[2];
		//		ads_rtos(XYArray[i][1], 2, Decimals, tempACHAR);	//获取指定小数位数的曲线半径

		//		//20190909新增	检测是否是整数
		//		if (fabs(XYArray[i][1] - (int)XYArray[i][1]) < 0.01)
		//			_stprintf(tempACHAR, L"%.0lf", XYArray[i][1]);
		//	}
		//	else
		//	{
		//		_tcscpy(Draw_Text, L"L-");
		//		Cur_Geo_Element_Length = XYArray[i][1] * XYArray[i][1] / XYArray[i][2];
		//		ads_rtos(Cur_Geo_Element_Length, 2, Decimals, tempACHAR);	//获取指定小数位数的缓和曲线长度

		//		//20190909新增	检测是否是整数
		//		if (fabs(Cur_Geo_Element_Length - (int)Cur_Geo_Element_Length) < 0.01)
		//			_stprintf(tempACHAR, L"%.0lf", Cur_Geo_Element_Length);
		//	}
		//	_tcscat(Draw_Text, tempACHAR);

		//	//②计算绘制位置及方位角
		//	xlzbxy(&Rail_Pt_Start, &Rail_Pt_Mid, 0.5 * Cur_Geo_Element_Length, INDATA, 0);	//计算线元中点坐标
		//	double Angle = Rail_Pt_Mid.a;
		//	int LorR = (int)(XYArray[i][4] + 0.01);		//左或者右
		//	if (LorR == 0 || LorR > 1 || LorR < -1)
		//		LorR = -1;
		//	tempAcGePt[X] = Rail_Pt_Mid.y + 1.25 * texth * sin(Angle + LorR * 0.5 * pi);
		//	tempAcGePt[Y] = Rail_Pt_Mid.x + 1.25 * texth * cos(Angle + LorR * 0.5 * pi);
		//	Angle = FwjtoAngX(Angle);
		//	//③绘制文字
		//	if(IsNoteRAL)
		//		maketext(tempAcGePt, Draw_Text, Angle, texth, Text_Color, 3, L"宋体", Layer_Name);
		//}

		//3.绘制起终点
		if (i == 1 || i == XYNum)
		{
			//计算连线信息
			double Angle = Rail_Pt_Start.a - pi * 0.5;
			AcGePoint3d Pt_Line_Start, Pt_Line_End;
			Pt_Line_Start.set(Rail_Pt_Start.y, Rail_Pt_Start.x, 0);
			Pt_Line_End.set(Rail_Pt_Start.y + WB * sin(Angle), Rail_Pt_Start.x + WB * cos(Angle), 0);
			//绘制连线
			makeline(Pt_Line_Start, Pt_Line_End, S_N_E_Color, 0.0, Layer_Name);

			//计算起点到终点的向量
			AcGeVector2d LineVec(Pt_Line_End.x - Pt_Line_Start.x, Pt_Line_End.y - Pt_Line_Start.y);
			double Text_Angle = LineVec.angle() + pi * 0.5;
			if (Text_Angle > 2 * pi)
				Text_Angle -= 2 * pi;

			//绘制“改路起点”、“改路终点”	20190819	修改名称
			//if (i == 1)
			//	_tcscpy(Draw_Text, L"改路起点");
			//else
			//	_tcscpy(Draw_Text, L"改路终点");
			if (!IsGutter)
			{
				if (i == 1)
					_tcscpy(Draw_Text, L"道路改移起点");
				else
					_tcscpy(Draw_Text, L"道路改移终点");
			}
			else
			{
				if (i == 1)
					_tcscpy(Draw_Text, L"改沟起点");
				else
					_tcscpy(Draw_Text, L"改沟终点");
			}


			tempAcGePt.x = (Pt_Line_Start.x + Pt_Line_End.x) / 2 + 1 * texth * cos(Text_Angle);
			tempAcGePt.y = (Pt_Line_Start.y + Pt_Line_End.y) / 2 + 1 * texth * sin(Text_Angle);
			maketext(tempAcGePt, Draw_Text, LineVec.angle(), texth, S_N_E_Color, 3, L"宋体", Layer_Name);

			//绘制里程
			CString tempCkml = XLC(Rail_Pt_Start.lc);
			_tcscpy(Draw_Text, tempCkml);
			tempAcGePt.x = (Pt_Line_Start.x + Pt_Line_End.x) / 2 - 1 * texth * cos(Text_Angle);
			tempAcGePt.y = (Pt_Line_Start.y + Pt_Line_End.y) / 2 - 1 * texth * sin(Text_Angle);
			maketext(tempAcGePt, Draw_Text, LineVec.angle(), texth, S_N_E_Color, 3, L"宋体", Layer_Name);
		}
	}
}

//封装【坐标】、【现场里程】、【投影里程】间的相互转换函数		20190715	ljx
//①【坐标】与【投影里程】相互转换
void JD_CENTER::Trs_ProjMl_To_NE(double ProjMl, double& N, double& E)
{
	xlpoint *PZ = new xlpoint();
	PZ->lc = ProjMl;
	xlpoint_pz(PZ);
	N = PZ->x;
	E = PZ->y;
}
double JD_CENTER::Trs_NE_To_ProjMl(double N, double E)
{
	return PROJ_ML(N, E);
}
//②【投影里程】与【现场里程】相互转换
double JD_CENTER::Trs_Ckml_To_ProjMl(CString Ckml)
{
	return TYLC(Ckml);
}
CString JD_CENTER::Trs_ProjMl_To_Ckml(double ProjMl)
{
	return XLC(ProjMl);
}
void JD_CENTER::Trs_ProjMl_To_Ckml(double ProjMl, ACHAR Ckml[])
{
	CString tempCkml = Trs_ProjMl_To_Ckml(ProjMl);
	_tcscpy(Ckml, tempCkml);
}
//③【坐标】与【现场里程】相互转换
void JD_CENTER::Trs_Ckml_To_NE(CString Ckml, double& N, double& E)
{
	double tempProjMl = Trs_Ckml_To_ProjMl(Ckml);
	Trs_ProjMl_To_NE(tempProjMl, N, E);
}
CString JD_CENTER::Trs_NE_To_Ckml(double N, double E)
{
	double tempProjMl = Trs_NE_To_ProjMl(N, E);
	return Trs_ProjMl_To_Ckml(tempProjMl);
}
void JD_CENTER::Trs_NE_To_Ckml(double N, double E, ACHAR Ckml[])
{
	double tempProjMl = Trs_NE_To_ProjMl(N, E);
	_tcscpy(Ckml, Trs_ProjMl_To_Ckml(tempProjMl));
}
//Dist为负时在线路左侧，Dist为正时在线路右侧
void JD_CENTER::Trs_CkmlPt_To_NE(CString Ckml, double Dist,double& N, double& E)
{
	double tempProjMl = Trs_Ckml_To_ProjMl(Ckml);
	Trs_ProjMl_To_NE(tempProjMl, N, E);
	//求解方位角
	AcGePoint3d Pt;
	Pt.set(E,N,0.0);
	xlpoint *PZ = new xlpoint();
	PZ->lc = tempProjMl;
	xlpoint_pz(PZ);
	//求解方位角对应的向量
	AcGeVector3d vec;
	vec = BAS_DRAW_FUN::calfwjvec(PZ->a);
	vec.normalize();
	//向右旋转90度
	AcGeVector3d VecR;
	VecR = 1 * vec.crossProduct(AcGeVector3d::kZAxis);
	VecR.normalize();
	AcGePoint3d Pt2;
	Pt2 = Pt + VecR * Dist;
	N = Pt2.y;
	E = Pt2.x;
}
