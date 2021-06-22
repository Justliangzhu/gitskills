// StaDataSheet.cpp : implementation file
//

#include "stdafx.h"
#include "StaDataSheet.h"
#include "XLDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StaDataSheet

IMPLEMENT_DYNAMIC(StaDataSheet, CPropertySheet)

	StaDataSheet::StaDataSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

StaDataSheet::StaDataSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	StaDlgAry=NULL;
	xlnum = 0;
	road = NULL;

	INIT();
}

StaDataSheet::~StaDataSheet()
{
}


BEGIN_MESSAGE_MAP(StaDataSheet, CPropertySheet)
	//{{AFX_MSG_MAP(StaDataSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StaDataSheet message handlers

BOOL StaDataSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();


	return bResult;
}
BOOL StaDataSheet::INIT() 
{
	ReadWorkdir();
	CString title;
	title = L"当前方案名: " + Pro;
	//SetWindowText(title);

	//CString  HdmFname;

	mdbFname = Cworkdir+L"\\data\\"+Pro+L".mdb";
	//读道路总数
	XLDataBase mdb;
	mdb.ReadAllRoadName(mdbFname);//读取线路数据库中断链表的数据

	xlnum = mdb.m_RoadSum;
	if(xlnum<1)
	{
		AfxMessageBox(L"当前道路总数为0，请先指定当前方案名称和道路名!");
		return false;
	}

	if(road) {delete []road;road=NULL;}
	if(xlnum>0)
	{
		if(road) delete[]road; road=NULL;
		road=new RoadSTAdata[xlnum];
		if(StaDlgAry) delete[]StaDlgAry; StaDlgAry=NULL;
		StaDlgAry = new CCalSta[xlnum];
	}
	//初始化
	int i;
	for(i=0; i<50; i++)
		SmlOnZx[i]=EmlOnZx[i]=NoseSmlonZX[i]=NoseSmlonFld[i]=NoseEmlonZX[i]=NoseEmlonFld[i]=-1;

	//读数据库
	for(i=0; i<xlnum; i++)
	{   	   		   	   		   
		road[i].roadname=mdb.Road[i].RoadName;//左绕行段线路性质为-1
		//从数据库中读取

		mdb.Read_XLDbs(mdbFname,L"桩号序列",road[i].roadname);

		road[i].NSTA = mdb.NSTA;

		if(road[i].lcb.GetSize()>0)
			road[i].lcb.RemoveAll();
		int j;
		for(j=0; j<road[i].NSTA; j++)
			road[i].lcb.Add(mdb.STA[j]);

		if(mdb.STA) delete []mdb.STA;
		mdb.STA = NULL;
		mdb.NSTA = 0;

		// 创建每一道路平面对象
		//		   mdb.Read_XLDbs(mdbFname,"控制点表",road[i].roadname);
		mdb.Read_XLDbs(mdbFname,L"线元表",road[i].roadname);					
		mdb.Read_XLDbs(mdbFname,L"断链表",road[i].roadname);

		road[i].pm = NULL;
		if(mdb.XYNum>0)
		{
			road[i].pm=new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
			//			 road[i].pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
			if(mdb.NDL>0)
				road[i].pm->setDLB(mdb.DLArray,mdb.NDL);
			_tcscpy(road[i].pm->mdbname , mdbFname);
			_tcscpy(road[i].pm->RoadName , road[i].roadname);
		}

		StaDlgAry[i].pm = road[i].pm;
		StaDlgAry[i].NSTA = road[i].NSTA;
		StaDlgAry[i].roadname = road[i].roadname;

		for(j=0; j<road[i].NSTA; j++)
			StaDlgAry[i].lcb.Add(road[i].lcb[j]);
		StaDlgAry[i].m_psp.pszTitle=road[i].roadname;

		mdb.Read_XLDbs(mdbFname,L"大中桥",road[i].roadname);			   	   		   
		mdb.Read_XLDbs(mdbFname,L"隧道",road[i].roadname);
		StaDlgAry[i].NBRI = mdb.BriNum;
		for(j=0; j<StaDlgAry[i].NBRI; j++)
		{
			StaDlgAry[i].BriAry[j][0] = StaDlgAry[i].pm->TYLC(mdb.Bri[j].StDml);
			StaDlgAry[i].BriAry[j][1] = StaDlgAry[i].pm->TYLC(mdb.Bri[j].EdDml);
			if(mdb.Bri[j].XZ=="左幅")
				StaDlgAry[i].BriAry[j][2] = 1.0;
			else if(mdb.Bri[j].XZ=="右幅")
				StaDlgAry[i].BriAry[j][2] = 2.0;
			else
				StaDlgAry[i].BriAry[j][2] = 0.0;
		}

		StaDlgAry[i].NTUN = mdb.TunNum;
		for(j=0; j<StaDlgAry[i].NTUN; j++)
		{
			StaDlgAry[i].TunAry[j][0] = StaDlgAry[i].pm->TYLC(mdb.Tun[j].StDml);
			StaDlgAry[i].TunAry[j][1] = StaDlgAry[i].pm->TYLC(mdb.Tun[j].EdDml);
			if(mdb.Tun[j].XZ=="左幅")
				StaDlgAry[i].TunAry[j][2] = 1.0;
			else if(mdb.Tun[j].XZ=="右幅")
				StaDlgAry[i].TunAry[j][2] = 2.0;
			else
				StaDlgAry[i].TunAry[j][2] = 0.0;
		}	 	
		AddPage(&StaDlgAry[i]);		   
	}
	CalTZSTA();//计算关联特征桩

}

//检测 cml 在psta的桩号序列中是否存在  不存在  插入在psta 的桩号 设置对应关系 
void StaDataSheet::AddLcToArr(CCalSta &psta, double &cml,double gldml,CString glRoadName)
{
	int i;
	LCBdata ALc;
	int inf=0;


	for(i=0;i<psta.lcb.GetSize();i++)
	{
		if(cml<psta.lcb[i].lc-0.001)
		{
			inf=1;
			break;
		}
	}
	//  ads_printf(L"cml=%lf %lf\n",cml,psta.lcb[i].lc);
	if(inf)
	{
		if(fabs(psta.lcb[i-1].lc-cml)<0.5)//cml在psta 的桩号存在 
		{
			psta.lcb[i-1].gldmlc = gldml;
			psta.lcb[i-1].glRoadName = glRoadName;
			cml = psta.lcb[i-1].lc;
			//		 ads_printf(L"******%s glcml=%0.3lf curcml=%0.3lf\n",glRoadName,glcml,cml);
		} 
		else if(fabs(psta.lcb[i].lc-cml)<0.5)//cml在psta 的桩号存在 
		{
			psta.lcb[i].gldmlc = gldml;
			psta.lcb[i].glRoadName = glRoadName;
			cml = psta.lcb[i].lc;
			//		 ads_printf(L"******%s glcml=%0.3lf curcml=%0.3lf\n",glRoadName,glcml,cml);
		}
		else//cml在psta 的桩号不存在
		{
			ALc.lc = cml;
			ALc.gldmlc = gldml;
			ALc.glRoadName = glRoadName;
			psta.lcb.InsertAt(i,ALc);
			//		ads_printf(L"!!!!!!%d %s glcml=%0.3lf curcml=%0.3lf\n",i-1,glRoadName,glcml,cml);
		}
	}
	psta.NSTA=psta.lcb.GetSize();
}
void StaDataSheet::Adjust() 
{
	// TODO: Add your control notification handler code here
	//1.计算各分离段起终点在主线上的投影里程,确定调整范围sml-eml
	xlpoint PZtmp;
	CString mes;
	double cmlOnFLD,spt[2],ept[2];
	double NosePt[2];
	int i;
	for(i=1; i<xlnum; i++)
	{
		//计算分离段起终点在主线上的投影里程
		double cmlOnZx;
		if(road[i].pm && road[i].pm->XYNum>0)
		{
			//	ads_printf(L"i=%d sml=%lf eml=%lf %lf\n",i,NoseSmlonZX[i],NoseEmlonZX[i],EmlOnZx[i]);
			//设置主线与分离段关联
			int j;
			for(j=0;j<StaDlgAry[0].NSTA;j++)
			{
				PZtmp.lc = StaDlgAry[0].lcb[j].lc;
				if(PZtmp.lc>NoseSmlonZX[i]-0.1 && PZtmp.lc<NoseEmlonZX[i]+0.1)
				{
					StaDlgAry[i].pm->xlpoint_pz(&PZtmp);
					double xjj,projml,respt[2];
					CalXjj(PZtmp.lc,StaDlgAry[0].pm->XYArray,StaDlgAry[0].pm->XYNum,
						StaDlgAry[i].pm->XYArray,StaDlgAry[i].pm->XYNum,xjj,projml,respt);
					if(fabs(xjj)<80.0)
					{
						if(xjj>0)//主线在左侧
						{
							respt[0] = PZtmp.x + 0.5*fabs(xjj)*cos(PZtmp.a+0.5*PI);
							respt[1] = PZtmp.y + 0.5*fabs(xjj)*sin(PZtmp.a+0.5*PI);
						}
						else
						{
							respt[0] = PZtmp.x + 0.5*fabs(xjj)*cos(PZtmp.a-0.5*PI);
							respt[1] = PZtmp.y + 0.5*fabs(xjj)*sin(PZtmp.a-0.5*PI);
						}
						cmlOnFLD = StaDlgAry[i].pm->PROJ_ML(respt[0],respt[1]);
						double dml;
						CString GH;
						AddLcToArr(StaDlgAry[i],cmlOnFLD,StaDlgAry[0].pm->XLC(PZtmp.lc,GH),road[0].roadname);				
						dml = StaDlgAry[i].pm->XLC(cmlOnFLD,GH);
						StaDlgAry[0].lcb[j].gldmlc=dml;
						StaDlgAry[0].lcb[j].glRoadName = road[i].roadname;						
					}
				}
			}

			for(j=0;j<StaDlgAry[i].NSTA;j++)
			{
				PZtmp.lc = StaDlgAry[i].lcb[j].lc;
				if(PZtmp.lc>NoseSmlonFld[i]-0.1 && PZtmp.lc<NoseEmlonFld[i]+0.1)
				{
					StaDlgAry[i].pm->xlpoint_pz(&PZtmp);
					double xjj,projml,respt[2];
					CalXjj(PZtmp.lc,StaDlgAry[i].pm->XYArray,StaDlgAry[i].pm->XYNum,
						StaDlgAry[0].pm->XYArray,StaDlgAry[0].pm->XYNum,xjj,projml,respt);
					if(fabs(xjj)<80.0)
					{
						if(xjj<0)//主线在左侧
						{
							respt[0] = PZtmp.x + 0.5*fabs(xjj)*cos(PZtmp.a-0.5*PI);
							respt[1] = PZtmp.y + 0.5*fabs(xjj)*sin(PZtmp.a-0.5*PI);
						}
						else
						{
							respt[0] = PZtmp.x + 0.5*fabs(xjj)*cos(PZtmp.a+0.5*PI);
							respt[1] = PZtmp.y + 0.5*fabs(xjj)*sin(PZtmp.a+0.5*PI);
						}
						cmlOnZx = StaDlgAry[0].pm->PROJ_ML(respt[0],respt[1]);
						CString GH;
						double dml;
						dml = StaDlgAry[i].pm->XLC(StaDlgAry[i].lcb[j].lc,GH);
						AddLcToArr(StaDlgAry[0],cmlOnZx,dml,road[i].roadname);				
						StaDlgAry[i].lcb[j].gldmlc=StaDlgAry[0].pm->XLC(cmlOnZx,GH);
						StaDlgAry[i].lcb[j].glRoadName = road[0].roadname;

					}
				}

			}

		}
	}
	for(i=1; i<xlnum; i++)
	{		
		for(int j=0;j<StaDlgAry[0].NSTA;j++)
		{
			if(StaDlgAry[0].lcb[j].lc>NoseSmlonZX[i]-0.1&&StaDlgAry[0].lcb[j].lc<NoseEmlonZX[i]+0.1)
			{
				PZtmp.lc = StaDlgAry[0].lcb[j].lc;
				StaDlgAry[0].pm->xlpoint_pz(&PZtmp);
				double xjj,projml,respt[2];
				CalXjj(PZtmp.lc,StaDlgAry[0].pm->XYArray,StaDlgAry[0].pm->XYNum,
					StaDlgAry[i].pm->XYArray,StaDlgAry[i].pm->XYNum,xjj,projml,respt);
				if(fabs(xjj)<80.0)
				{
					if(xjj>0)//主线在左侧
					{
						respt[0] = PZtmp.x + 0.5*fabs(xjj)*cos(PZtmp.a+0.5*PI);
						respt[1] = PZtmp.y + 0.5*fabs(xjj)*sin(PZtmp.a+0.5*PI);
					}
					else
					{
						respt[0] = PZtmp.x + 0.5*fabs(xjj)*cos(PZtmp.a-0.5*PI);
						respt[1] = PZtmp.y + 0.5*fabs(xjj)*sin(PZtmp.a-0.5*PI);
					}
					cmlOnFLD = StaDlgAry[i].pm->PROJ_ML(respt[0],respt[1]);
					CString GH;

					double dml;
					dml = StaDlgAry[0].pm->XLC(StaDlgAry[0].lcb[j].lc,GH);
					AddLcToArr(StaDlgAry[i],cmlOnFLD,dml,road[0].roadname);				
					StaDlgAry[0].lcb[j].gldmlc=StaDlgAry[i].pm->XLC(cmlOnFLD,GH);
					StaDlgAry[0].lcb[j].glRoadName = road[i].roadname;
				}
			}
		}
	}



	ads_alert(L"调整完毕!");
}

//计算关联特征桩
void StaDataSheet::CalTZSTA() 
{
	double spt[2],ept[2],NosePt[2];
	CString GH;

	StaDlgAry[0].NGLSTA=0;
	for(int i=1; i<xlnum; i++)
	{
		//计算分离段起终点在主线上的投影里程
		double cmlOnZx;
		StaDlgAry[i].NGLSTA = 0;
		if(road[i].pm && road[i].pm->XYNum>0)
		{
			spt[0] = road[i].pm->XYArray[1][8];
			spt[1] = road[i].pm->XYArray[1][9];		
			SmlOnZx[i] = road[0].pm->PROJ_ML(spt[0],spt[1]);
			ept[0] = road[i].pm->XYArray[road[i].pm->XYNum][8];
			ept[1] = road[i].pm->XYArray[road[i].pm->XYNum][9];		
			EmlOnZx[i] = road[0].pm->PROJ_ML(ept[0],ept[1]);
			StaDlgAry[0].NGLSTA++;
			StaDlgAry[0].GLSTA[i-1][0] = SmlOnZx[i];
			StaDlgAry[0].GLSTA[i-1][1] = EmlOnZx[i];
			StaDlgAry[0].GLSTA[i-1][2] = 0.0;


			/*
			road[0].pm->CalNoseCen(road[i].pm,SmlOnZx[i],1,0,0,0,0.6,
			NoseSmlonZX[i],NoseSmlonFld[i],NosePt);
			road[0].pm->CalNoseCen(road[i].pm,EmlOnZx[i],-1,0,0,0,0.6,
			NoseEmlonZX[i],NoseEmlonFld[i],NosePt);
			*/

			/*
			GLTZSTAdata glsta;
			glsta.FLDSmlOnZX = road[0].pm->XLC(SmlOnZx[i],GH); 
			glsta.FLDEmlOnZX = road[0].pm->XLC(EmlOnZx[i],GH);
			glsta.NoseSmlOnZX = road[0].pm->XLC(NoseSmlonZX[i],GH);
			glsta.NoseEmlOnZX = road[0].pm->XLC(NoseEmlonZX[i],GH);
			glsta.NoseSmlOnFLD = road[i].pm->XLC(NoseSmlonFld[i],GH);
			glsta.NoseEmlOnFLD = road[i].pm->XLC(NoseEmlonFld[i],GH);
			glsta.FLDname = road[i].roadname;
			GLTZlcb.Add(glsta);*/

		}
	}
}
