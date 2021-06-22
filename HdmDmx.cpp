// HdmDmx.cpp: implementation of the HdmDmx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HdmDmx.h"
#include "GetHdmDmx.h"
#include "triDTM_road.h"
#include "XLDataBase.h"
//#include <iostream.h>
#include "Shlwapi.h "
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern triDTM_road ptriDTM;
//struct EdgeNode {
//					double pt[3];
//					struct EdgeNode *next;
//				}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HdmDmx::HdmDmx()
{
	mdbFname="";
	RoadName="";
	road=NULL;
}

HdmDmx::~HdmDmx()
{

}

void HdmDmx::CalDmx()
{
	//计算横断面地面线
	GetHdmDmx dlg;
	int IsDtmExisting;
	XLDataBase mdb;
	HdmDataBase hdmmdb;

	if(dlg.DoModal()==IDOK)
	{
		//读工作目录
		ReadWorkdir();
		//CString  HdmFname;
		mdbFname = Cworkdir+"\\data\\"+Pro+".mdb";
		//读道路总数

		mdb.ReadAllRoadName(mdbFname);//读取线路数据库中断链表的数据

		if(mdb.m_RoadSum<1)
		{
			AfxMessageBox(L"当前道路总数为0，请先指定当前方案名称和道路名!");
			return ;
		}

		if(road) {delete []road;road=NULL;}
		if(mdb.m_RoadSum>0)
		{
			if(road) delete[]road; road=NULL;
			road=new RoadDmxdata[mdb.m_RoadSum];
		}
		for(int i=0; i<mdb.m_RoadSum; i++)
		{

			road[i].pm = NULL;
			road[i].NSTA =0;
			road[i].hdmdmx = NULL;

			road[i].roadname=mdb.Road[i].RoadName;//左绕行段线路性质为-1
			//				//读平面
			//				// 创建每一道路平面对象
			//				mdb.Read_XLDbs(mdbFname,L"控制点表",road[i].roadname);
			mdb.Read_XLDbs(mdbFname,L"线元表",road[i].roadname);						
			mdb.Read_XLDbs(mdbFname,L"断链表",road[i].roadname);

			road[i].pm = NULL;
			if(mdb.XYNum>0)
			{
				road[i].pm=new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
				//					road[i].pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
				if(mdb.NDL>0)
					road[i].pm->setDLB(mdb.DLArray,mdb.NDL);
			}
			else
				return;

			//从数据库中读取横断面内插宽

			hdmmdb.Read_HdmDbs(mdbFname,road[i].roadname,L"左地面线内插宽");
			hdmmdb.Read_HdmDbs(mdbFname,road[i].roadname,L"右地面线内插宽");
			double LLd,RRd;
			double xl[200],yl[200],zl[200];
			double xr[200],yr[200],zr[200];

			if(dlg.mode==0)//数模内插
			{
				mdb.Read_XLDbs(mdbFname,L"桩号序列",road[i].roadname);
				road[i].NSTA = mdb.NSTA;
				if(road[i].NSTA<1)
				{
					ads_alert(L"请先计算桩号序列!");
					return;
				}

				//读数模

				if (ptriDTM.DTMHasRead)
				{
					IsDtmExisting = 1;
				}
				else
				{
					if(ptriDTM.Read_Dtm())
						IsDtmExisting=1;
					else
					{
						IsDtmExisting=0;//数模不存在    
						ads_initget(2+4,NULL);
						if(ads_getreal(L"\n  请输入地面高程的平均值<100> ",&ptriDTM.DefaultH)==RTNONE)
							ptriDTM.DefaultH = 100;
					}
				}
				//	}

				EdgeNode *CrsDmxHead,*Tmp;
				int LPnum,RPnum,j;
				if(road[i].hdmdmx) delete []road[i].hdmdmx;

				road[i].hdmdmx = NULL;

				road[i].hdmdmx = (HDMDMXdata *)new HDMDMXdata[road[i].NSTA];

				ACHAR pszLabel[256];
				_stprintf(pszLabel,L"%s横断面地面线内插",road[i].roadname);
				acedSetStatusBarProgressMeter(pszLabel,0,100);

				for(j=0; j<road[i].NSTA; j++)
				{
					CString GH;
					road[i].hdmdmx[j].lc = road[i].pm->XLC(mdb.STA[j].lc,GH);

					double cml;
					int k;
					cml = mdb.STA[j].lc;//统一里程

					//计算左右地面点
					LLd = FindIdmxk(road[i].pm,road[i].hdmdmx[j].lc,hdmmdb.NZCRS,hdmmdb.ZCRSWID);
					RRd = FindIdmxk(road[i].pm,road[i].hdmdmx[j].lc,hdmmdb.NYCRS,hdmmdb.YCRSWID);

					xlpoint PZ;
					PZ.lc=cml;
					road[i].pm->xlpoint_pz(&PZ);
					double pt1[3],pt2[3],cita,x,y,yyt,Yd;
					Yd = 0.0;//初始值
					pt1[0]=PZ.x;pt1[1]=PZ.y;pt1[2]=0.0;
					cita=PZ.a;
					x=PZ.x;
					y=PZ.y;
					yyt=cita-0.5*pi;
					pt2[0]=x+LLd*cos(yyt);
					pt2[1]=y+LLd*sin(yyt);
					pt2[2]=0.0;

					if(ptriDTM.DTMHasRead)
						CrsDmxHead = ptriDTM.FindInterPt(pt1, pt2,LPnum);
					else
						LPnum = 0;

					if(LPnum>0)//左侧地面线有点
					{
						k=0;
						while(CrsDmxHead)
						{
							xl[k] =  CrsDmxHead->pt[0] ;
							yl[k] =  CrsDmxHead->pt[1] ;
							zl[k] =  CrsDmxHead->pt[2] ;

							Tmp = CrsDmxHead;
							CrsDmxHead = CrsDmxHead->next; 
							free(Tmp);
							k++;
						}

						if(k>0)
						{				  
							Yd = zl[0];
							if(Yd<0.1&&k>1)
							{
								Yd = zl[1];
							}
						}        

						for(k=1; k<LPnum; k++)
						{
							xl[k] = sqrt((xl[k]-xl[0])*(xl[k]-xl[0])+(yl[k]-yl[0])*(yl[k]-yl[0])); 

							if(zl[k]<-100.0)//没数模 按中桩
								zl[k] = Yd;
							yl[k] = zl[k]-Yd;	

						}	
						xl[0] = 0.0;
						yl[0] = 0.0;
					}//至少有一个点
					else//一个点也没有
					{
						LPnum = 2;
						Yd = 0.0;
						xl[0] = 0.0;
						yl[0] = 0.0;

						xl[1] = LLd;
						yl[1] = 0.0;
					}

					yyt = cita + 0.5*pi;
					pt2[0] = x + RRd*cos(yyt);
					pt2[1] = y + RRd*sin(yyt);
					pt2[2] = 0.0;
					CrsDmxHead = ptriDTM.FindInterPt(pt1, pt2,RPnum);

					//计算横断面左侧地面点坐标
					if(RPnum >0)
					{
						k=0;
						while(CrsDmxHead)
						{
							xr[k] =  CrsDmxHead->pt[0] ;
							yr[k] =  CrsDmxHead->pt[1] ;
							zr[k] =  CrsDmxHead->pt[2] ;
							Tmp = CrsDmxHead;
							CrsDmxHead = CrsDmxHead->next; 
							free(Tmp);
							k++;
						}
						for(k=1; k<RPnum; k++)
						{
							xr[k] = sqrt((xr[k]-xr[0])*(xr[k]-xr[0])+(yr[k]-yr[0])*(yr[k]-yr[0])); 

							if(zr[k]<-100.0)//没数模 按中桩
								zr[k] = Yd;
							yr[k] = zr[k]-Yd;					          				      
						}
						xr[0] = 0.0;
						yr[0] = 0.0;
					}
					else
					{
						RPnum = 2;
						Yd = 0.0;
						xr[0] = 0.0;
						yr[0] = 0.0;

						xr[1] = RRd;
						yr[1] = 0.0;
					}

					//地面线付值
					Point pt;
					road[i].hdmdmx[j].EarH = Yd;
					road[i].hdmdmx[j].ZEarPtNum = LPnum;
					road[i].hdmdmx[j].ZEarPtAry.RemoveAll();
					for(k=0; k<LPnum; k++)
					{
						pt.x = xl[k],pt.y=yl[k],pt.z=0.0;
						road[i].hdmdmx[j].ZEarPtAry.Add(pt);
					}
					road[i].hdmdmx[j].YEarPtNum = RPnum;
					road[i].hdmdmx[j].YEarPtAry.RemoveAll();
					for(k=0; k<RPnum; k++)
					{
						pt.x = xr[k],pt.y=yr[k],pt.z=0.0;
						road[i].hdmdmx[j].YEarPtAry.Add(pt);
					}
					int Pos = (int)(j*100.0/road[i].NSTA);
					acedSetStatusBarProgressMeterPos(Pos);
				}
				if(mdb.STA) delete []mdb.STA;
				mdb.STA = NULL;
				acedSetStatusBarProgressMeterPos(100);    
				acedRestoreStatusBar();
				if(ptriDTM.DTMHasRead)
					ptriDTM.ReleaseDtm();
				WriteHdmDmxFile(i);	
				///写hdkzdk文件		
				CString pathstr1 = Cworkdir + L"\\data\\" + road[i].roadname;
				if(!PathIsDirectory(pathstr1))					  
					::_wmkdir(pathstr1);
				WriteZdkHdkFile(i);
			}	//内插横断面地面线
			else if(dlg.mode==1)//外业实测(ZDK,HDK文件)
			{

				GetHdmDmxFromZDKandHDK(road[i].pm,mdbFname,road[i].roadname);

			}
			else if(dlg.mode==2)//虚拟横坡
			{
				double ZHP,YHP;
				ZHP = dlg.m_ZHP;
				YHP = dlg.m_YHP;
				//1.读桩号序列
				mdb.Read_XLDbs(mdbFname,L"桩号序列",road[i].roadname);
				road[i].NSTA = mdb.NSTA;
				//2.计算中心地面标高
				mdb.Read_XLDbs(mdbFname,L"纵地面线表",road[i].roadname);
				if(mdb.NTW<1)
				{
					ads_alert(L"无法找到中桩地面标高,请先输入纵断面地面线!");
					return;
				}
				if(road[i].NSTA<1)
				{
					ads_alert(L"请先计算桩号序列!");
					return;
				}

				if(road[i].hdmdmx) delete []road[i].hdmdmx;
				road[i].hdmdmx = NULL;
				road[i].hdmdmx = (HDMDMXdata *)new HDMDMXdata[road[i].NSTA];

				ACHAR pszLabel[256];
				_stprintf(pszLabel,L"%s横断面地面线",road[i].roadname);
				acedSetStatusBarProgressMeter(pszLabel,0,100);

				for(int j=0; j<road[i].NSTA; j++)
				{
					CString GH;
					road[i].hdmdmx[j].lc = road[i].pm->XLC(mdb.STA[j].lc,GH);
					double cml;
					int k;
					cml = mdb.STA[j].lc;//统一里程

					double tylc,Btylc,Yd,BYd,EarH;
					for(k=0; k<mdb.NTW; k++)
					{
						tylc = road[i].pm->TYLC(mdb.TWB[k].ml);
						Yd = mdb.TWB[k].DMGC;
						if(cml < tylc+0.001)
							break;
						Btylc = tylc;
						BYd = Yd;
					}

					if(k>0)
					{
						if(tylc-Btylc>0.0001)
							EarH = BYd + (cml-Btylc)/(tylc-Btylc)*(Yd-BYd);
						else
							EarH = Yd;
					}
					else
						EarH = Yd;
					LLd = FindIdmxk(road[i].pm,road[i].hdmdmx[j].lc,hdmmdb.NZCRS,hdmmdb.ZCRSWID);
					RRd = FindIdmxk(road[i].pm,road[i].hdmdmx[j].lc,hdmmdb.NYCRS,hdmmdb.YCRSWID);

					//3.计算左地面线
					Point pt;
					road[i].hdmdmx[j].EarH = EarH;
					road[i].hdmdmx[j].ZEarPtNum = 2;
					road[i].hdmdmx[j].ZEarPtAry.RemoveAll();
					pt.x = 0.0,pt.y=0.0,pt.z=0.0;
					road[i].hdmdmx[j].ZEarPtAry.Add(pt);
					pt.x = LLd,pt.y=LLd*ZHP*0.01,pt.z=0.0;
					road[i].hdmdmx[j].ZEarPtAry.Add(pt);

					road[i].hdmdmx[j].YEarPtNum = 2;
					road[i].hdmdmx[j].YEarPtAry.RemoveAll();
					pt.x = 0.0,pt.y=0.0,pt.z=0.0;
					road[i].hdmdmx[j].YEarPtAry.Add(pt);
					pt.x = LLd,pt.y=RRd*YHP*0.01,pt.z=0.0;
					road[i].hdmdmx[j].YEarPtAry.Add(pt);

					int Pos = (int)(j*100.0/road[i].NSTA);
					acedSetStatusBarProgressMeterPos(Pos);

				}
				acedSetStatusBarProgressMeterPos(100);    
				acedRestoreStatusBar();

				if(mdb.STA) delete []mdb.STA;
				mdb.STA = NULL;
				WriteHdmDmxFile(i);
			}
			else if(dlg.mode==3)//中桩实测+DTM内插相对高差
			{
				GetHdmDmxFromZDKandDTM(road[i].pm,mdbFname,road[i].roadname);
			}
		}
	}//end dialog

}

double HdmDmx::FindIdmxk(JD_CENTER *pm,double dml,int ndmxk,crswiddata dmxk[])//地面线宽
{
	int i;
	double cml,ml;

	cml = pm->TYLC(dml);
	for(i=0; i<ndmxk; i++)
	{
		ml = pm->TYLC(dmxk[i].dml);//分段终里程
		if(cml<ml || fabs(ml-cml)<0.001)
			//所给里程处于第I个分段内
				return dmxk[i].wid;
	}

	return dmxk[i-1].wid;
}

void HdmDmx::WriteHdmDmxFile(int i)
{
	FILE *fpw;
	CString DmxFname;

	DmxFname = Cworkdir + "\\data\\" + "hdmdmx." + Pro + "." + road[i].roadname;
	fpw = _wfopen(DmxFname,L"w");
	if(fpw)
	{
		fwprintf(fpw,L"%d\n",road[i].NSTA);
		int j;
		for(j=0; j<road[i].NSTA; j++)
		{
			fwprintf(fpw,L"%0.3lf %0.3lf %d %d\n",road[i].hdmdmx[j].lc,road[i].hdmdmx[j].EarH,road[i].hdmdmx[j].ZEarPtNum,road[i].hdmdmx[j].YEarPtNum);
			int k;
			for(k=0; k<road[i].hdmdmx[j].ZEarPtNum; k++)
				fwprintf(fpw,L"%0.3lf %0.3lf ",road[i].hdmdmx[j].ZEarPtAry[k].x,road[i].hdmdmx[j].ZEarPtAry[k].y);
			fwprintf(fpw,L"\n");
			for(k=0; k<road[i].hdmdmx[j].YEarPtNum; k++)
				fwprintf(fpw,L"%0.3lf %0.3lf ",road[i].hdmdmx[j].YEarPtAry[k].x,road[i].hdmdmx[j].YEarPtAry[k].y);
			fwprintf(fpw,L"\n");
		}
		fclose(fpw);
		char mes[256];
		ads_printf(L"%s横断面地面线已保存在%s中!\n",road[i].roadname,DmxFname);
		//ads_alert(mes);
	}



}

void HdmDmx::WriteZdkHdkFile(int i)
{
	FILE *fpw,*fpwhdk;
	CString GH,fname,fnamehdk,mes;
	double dml;
	int iKM1,iKM2,crsnum,itmp,k;

	if(road[i].NSTA>0)
	{
		//  dml = road[i].pm->XLC(road[i].hdmdmx[0].lc,GH);
		dml = road[i].hdmdmx[0].lc;
		iKM1 = (int)((dml+0.001)/1000.0);


		fname.Format(L"%s\\data\\%s\\ZDK%d.TXT" , Cworkdir,road[i].roadname,iKM1);
		if((fpw = _wfopen(fname ,L"w"))==NULL)
		{
			mes.Format(L"无法打开%s文件!",fname);
			AfxMessageBox(mes);
			return ;
		}
		fnamehdk.Format(L"%s\\data\\%s\\HDK%d.TXT" , Cworkdir,road[i].roadname,iKM1);
		if((fpwhdk = _wfopen(fnamehdk ,L"w"))==NULL)
		{
			mes.Format(L"无法打开%s文件!",fnamehdk);
			AfxMessageBox(mes);
			return ;
		}

		crsnum = 0;
		itmp = 1000;

		fwprintf(fpw ,L"\"%s%d\" %3d\n" , GH,iKM1,itmp);
		fwprintf(fpwhdk ,L"\"%s%d\" %3d\n" , GH,iKM1,itmp);

		for(int j=0; j<road[i].NSTA; j++)
		{
			//  dml = road[i].pm->XLC(road[i].hdmdmx[j].lc,GH);
			dml = road[i].hdmdmx[j].lc;

			iKM2 = (int)((dml+0.001)/1000.0);
			if(iKM2-iKM1<0.5)
			{
				fwprintf(fpw ,L"%12.3lf %12.3lf\n" , dml , road[i].hdmdmx[j].EarH);
				fwprintf(fpwhdk,L"%0.3lf 0 %d\n",dml,road[i].hdmdmx[j].ZEarPtNum+road[i].hdmdmx[j].YEarPtNum-1);				  
				for(k=road[i].hdmdmx[j].ZEarPtNum-1; k>0; k--)
					fwprintf(fpwhdk,L"%6.3lf %6.3lf ",-road[i].hdmdmx[j].ZEarPtAry[k].x,road[i].hdmdmx[j].ZEarPtAry[k].y);				  

				fwprintf(fpwhdk ,L"\n");
				fwprintf(fpwhdk ,L"0 0 ");
				for(k=1; k<road[i].hdmdmx[j].YEarPtNum; k++)	
					fwprintf(fpwhdk,L"%6.3lf %6.3lf ",road[i].hdmdmx[j].YEarPtAry[k].x,road[i].hdmdmx[j].YEarPtAry[k].y);

				fwprintf(fpwhdk,L"\n");
				crsnum++;
			}
			else
			{
				fwprintf(fpw ,L"%12.3lf %12.3lf\n" , dml , road[i].hdmdmx[j].EarH);

				fwprintf(fpwhdk,L"%0.3lf 0 %d\n",dml,road[i].hdmdmx[j].ZEarPtNum+road[i].hdmdmx[j].YEarPtNum-1);				  
				for(k=road[i].hdmdmx[j].ZEarPtNum-1; k>0; k--)
					fwprintf(fpwhdk,L"%6.3lf %6.3lf ",-road[i].hdmdmx[j].ZEarPtAry[k].x,road[i].hdmdmx[j].ZEarPtAry[k].y);				  				   
				fwprintf(fpwhdk ,L"\n");
				fwprintf(fpwhdk ,L"0 0 ");
				for(k=1; k<road[i].hdmdmx[j].YEarPtNum; k++)	
					fwprintf(fpwhdk,L"%6.3lf %6.3lf ",road[i].hdmdmx[j].YEarPtAry[k].x,road[i].hdmdmx[j].YEarPtAry[k].y);				  
				fwprintf(fpwhdk,L"\n");

				crsnum++;
				rewind(fpw);
				fwprintf(fpw ,L"\"%s%d\" %3d\n" , GH,iKM1,crsnum);				
				rewind(fpwhdk);
				fwprintf(fpwhdk ,L"\"%s%d\" %3d\n" , GH,iKM1,crsnum);
				fclose(fpw);
				fclose(fpwhdk);
				crsnum=0;
				iKM1 = iKM2;
				fname.Format(L"%s\\data\\%s\\ZDK%d.TXT" , Cworkdir,road[i].roadname,iKM1);
				if((fpw = _wfopen(fname ,L"w"))==NULL)
				{
					mes.Format(L"无法打开%s文件!",fname);
					AfxMessageBox(mes);
					return ;
				}
				fwprintf(fpw ,L"\"%s%d\" %3d\n" , GH,iKM1,itmp);		   
				fwprintf(fpw ,L"%12.3lf %12.3lf\n" , dml , road[i].hdmdmx[j].EarH);

				fnamehdk.Format(L"%s\\data\\%s\\HDK%d.TXT" , Cworkdir,road[i].roadname,iKM1);
				if((fpwhdk = _wfopen(fnamehdk ,L"w"))==NULL)
				{
					mes.Format(L"无法打开%s文件!",fnamehdk);
					AfxMessageBox(mes);
					return ;
				}
				fwprintf(fpwhdk ,L"\"%s%d\" %3d\n" , GH,iKM1,itmp);		   
				fwprintf(fpwhdk,L"%0.3lf 0 %d\n",dml,road[i].hdmdmx[j].ZEarPtNum+road[i].hdmdmx[j].YEarPtNum-1);				  
				for(k=road[i].hdmdmx[j].ZEarPtNum-1; k>0; k--)
					fwprintf(fpwhdk,L"%6.3lf %6.3lf ",-road[i].hdmdmx[j].ZEarPtAry[k].x,road[i].hdmdmx[j].ZEarPtAry[k].y);				  				   
				fwprintf(fpwhdk ,L"\n");
				fwprintf(fpwhdk ,L"0 0 ");
				for(k=1; k<road[i].hdmdmx[j].YEarPtNum; k++)	
					fwprintf(fpwhdk,L"%6.3lf %6.3lf ",road[i].hdmdmx[j].YEarPtAry[k].x,road[i].hdmdmx[j].YEarPtAry[k].y);				  
				fwprintf(fpwhdk,L"\n");
				crsnum++;
			}
		}	
	}
	rewind(fpw);
	fwprintf(fpw ,L"\"%s%d\" %3d\n" , GH,iKM1,crsnum);
	fclose(fpw);   

	rewind(fpwhdk);
	fwprintf(fpwhdk ,L"\"%s%d\" %3d\n" , GH,iKM1,crsnum);
	fclose(fpwhdk);   
	/*

	fwprintf(fpw,L"%0.3lf %0.3lf %d %d\n",road[i].hdmdmx[j].lc,road[i].hdmdmx[j].EarH,road[i].hdmdmx[j].ZEarPtNum,road[i].hdmdmx[j].YEarPtNum);
	for(int k=0; k<road[i].hdmdmx[j].ZEarPtNum; k++)
	fwprintf(fpw,L"%0.3lf %0.3lf ",road[i].hdmdmx[j].ZEarPtAry[k].x,road[i].hdmdmx[j].ZEarPtAry[k].y);
	fwprintf(fpw,L"\n");
	for(k=0; k<road[i].hdmdmx[j].YEarPtNum; k++)
	fwprintf(fpw,L"%0.3lf %0.3lf ",road[i].hdmdmx[j].YEarPtAry[k].x,road[i].hdmdmx[j].YEarPtAry[k].y);
	fwprintf(fpw,L"\n");
	}
	fclose(fpw);*/

}
/*--------------------------------------------------------------
计算路基设计表数据、
读入:
1.A文件
2.lmap\pms文件
3.QZML文件
4.分段文件
写出：
1.生成TAB\LJSJB文件
2.生成CROSS\CRS_LM文件
3.生成CROSS\CRS_EAR文件
-------------------------------------------------------------*/
void HdmDmx::GetHdmDmxFromZDKandHDK(JD_CENTER *pm,CString mdbname,CString RoadName)
{
	ACHAR fname[256],mes[256],ck[50],ckml[80];
	FILE *fpr,*fpr1,*fpw,*fpw1,*fpw2,*fpw3;
	int iKml,i,j,sum,num,k,len;
	ACHAR mlstr[300],mlstr1[80],ml[80];
	double dml,bml,Bh,cml,Hdkdml;
	double WB3,WB2,WB1,WA1,WA2,WA3;
	double deltaZH[2],deltaYH[2],deltaMH;
	double dtmp,x,y,Bjx,ik;
	int ptsum,inf,lnum,rnum,itmp,Snum,Enum,ijd,iwid;
	double RHFL[10] , RHFR[10];
	double ZBGmode,YBGmode;
	double NSB,NSE;
	double sml,eml;
	int NCrs;
	//  GSZHDlg   dlg;
	int fsum;
	struct filearray{
		ACHAR zdk[280];
		ACHAR hdk[280];
	}  Fname[200];
	ACHAR zdk[280];
	ACHAR hdk[280];
	int ifile;
	struct Hdmdmxdata
	{
		double cml,dml;
		double EarH;
		int NumL,NumR;
		double *Learpt,*Rearpt;
	}*CrsEar;
	int SKML,EKML;
	SKML = (int)((pm->DLArray[0].ELC+0.0001)/ 1000.0);
	EKML = (int)((pm->DLArray[pm->DLNum-1].BLC+0.0001)/ 1000.0);
	sml = pm->DLArray[0].ELC;
	eml = pm->DLArray[pm->DLNum-1].BLC;
	fsum = EKML - SKML + 1;//文件总数
	sum=0;
	for(i=0; i<fsum; i++)
	{
		_stprintf(Fname[i].zdk,L"%s\\data\\%s\\ZDK%d.txt",Cworkdir,RoadName,SKML+i);
		_stprintf(Fname[i].hdk,L"%s\\data\\%s\\HDK%d.txt",Cworkdir,RoadName,SKML+i);			    	 
	}         
	NCrs = 0;
	//1.计算横断面总数
	//处理第一个文件
	if((fpr=_wfopen(Fname[0].zdk,L"r"))!=NULL)
	{
		fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
		Snum = 0;
		for(j=0;j<sum;j++)
		{
			fwscanf(fpr ,L"%lf%lf" ,&dml,&Bh);
			if(dml < sml-0.1)
				Snum++;
			else
				break;
		}
		NCrs = sum - Snum;
		ads_printf(L"ncrs=%d sum=%d Sum=%d sml=%lf %s\n",NCrs,sum,Snum,sml,Fname[0].zdk);
		fclose(fpr);			
	}
	else
	{
		ads_printf(L"错误: 无法找到%s文件!\n",Fname[0].zdk);
		// AfxMessageBox(mes);
	}

	for(i=1;i<fsum-1; i++)
	{
		if((fpr=_wfopen(Fname[i].zdk,L"r"))!=NULL)
		{
			fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
			fclose(fpr);
			NCrs += sum;
		}
	}

	if((fpr=_wfopen(Fname[fsum-1].zdk,L"r"))!=NULL)
	{
		fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
		Enum = 0;
		for(j=0;j<sum;j++)
		{
			fwscanf(fpr ,L"%lf%lf" ,&dml,&Bh);
			if(dml < eml+0.1)
				Enum++;
			else
				break;
		}
		fclose(fpr);
		NCrs += Enum;
	}
	else
	{
		ads_printf(L"无法找到%s文件!\n",Fname[fsum-1].zdk);
		// AfxMessageBox(mes);
	}
	if(NCrs<1)
	{
		ads_alert(L"桩数为0,请将外业测量数据存入HDK,ZDK文件!");
		return;
	}


	int NBT;
	double BriTunAry[500][2];
	double Learpt[400],Rearpt[400];
	ReadBriTunData(pm,mdbname,RoadName,NBT,BriTunAry);


	CrsEar = new Hdmdmxdata[NCrs+2*NBT];

	int count=0;
	for(ifile=0;ifile<fsum; ifile++)
	{
		if((fpr=_wfopen(Fname[ifile].zdk,L"r"))==NULL)
		{
			ads_printf(L"无法找到%s文件!\n",Fname[ifile].zdk);
			// AfxMessageBox(mes);
			continue;
		}

		if((fpr1=_wfopen(Fname[ifile].hdk,L"r"))==NULL)
		{
			ads_printf(L"无法找到%s文件!\n",Fname[ifile].hdk);
			//AfxMessageBox(mes);
			continue;
		}

		fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
		fwscanf(fpr1 ,L"%s%d" ,mlstr1,&itmp);

		CString GH;
		len=F_num(mlstr,'K');
		for(i=1,k=0; i<len;k++, i++)
			ck[k] = mlstr[i];
		ck[k] = '\0';
		GH = ck;

		if(ifile == 0)//第一个文件空读Snum个
		{
			for(j=0;j<Snum;j++)
			{
				fwscanf(fpr ,L"%lf%lf" ,&dtmp,&dtmp);
				//读横断面地面点数据
				fwscanf(fpr1 ,L"%lf%d%d" ,&dtmp,&itmp,&ptsum);
				for(i=0; i<ptsum; i++)
					fwscanf(fpr1,L"%lf%lf ",&x,&y);
			}
			sum = sum - Snum;
		}

		if(ifile == fsum-1)//最后一个文件只读Enum个
			sum = Enum;
		//	sum = (int)(_wtof(nstr));
		for(j=0; j<sum;j++)
		{
			fwscanf(fpr ,L"%lf%lf" ,&dml,&Bh);
			//读横断面地面点数据
			fwscanf(fpr1 ,L"%lf%d%d" ,&Hdkdml,&inf,&ptsum);			
			ads_printf(L"处理ZDK中 %lf和 HDK中 %lf\n",dml,Hdkdml);

			if(fabs(dml-Hdkdml)>0.2)//ZDK与HDK文件未对应
			{
				_stprintf(mes,L"请检查%s与%s文件,发现ZDK文件中里程%0.2lf与HDK文件中里程%0.2lf不对应!",
					Fname[ifile].zdk,Fname[ifile].hdk,dml,Hdkdml);
				AfxMessageBox(mes);
				return;
			}
			//			}
			//			else
			//			{
			//				ptsum = 4;
			//				ads_printf(L"处理ZDK中 %lf \n",dml);
			//			}

			//AfxMessageBox(mes);
			cml=pm->TYLC(dml);

			//	if(dlg.m_type==1)
			//	{
			if(num>0 && fabs(cml-bml)<0.1)
			{
				for(i=0; i<ptsum; i++)
					fwscanf(fpr1,L"%lf%lf",&x,&y);
				continue;
			}
			//	}

			lnum=0;
			rnum=0;

			//if(dlg.m_type==1)
			//{
			for(i=0; i<ptsum; i++)
			{
				fwscanf(fpr1,L"%lf%lf ",&x,&y);
				if(x < -0.0001)
				{
					Learpt[lnum*2] = fabs(x);
					Learpt[lnum*2+1] = y;
					lnum++;					   
				}
				else
				{
					Rearpt[rnum*2] = x;
					Rearpt[rnum*2+1] = y;
					rnum++;
				}
			}
			CrsEar[count].cml=cml;
			CrsEar[count].EarH=Bh;
			CrsEar[count].NumL=lnum;
			CrsEar[count].NumR=rnum;
			CrsEar[count].Learpt=NULL;
			if(lnum>0)
				CrsEar[count].Learpt = new double[lnum*2];
			CrsEar[count].Rearpt=NULL;
			if(rnum>0)
				CrsEar[count].Rearpt = new double[rnum*2];
			for(i=0; i<lnum*2; i++)
				CrsEar[count].Learpt[i] = Learpt[i];
			for(i=0; i<rnum*2; i++)
				CrsEar[count].Rearpt[i] = Rearpt[i];
			count++;
		}
	}
	//将桥隧里程插入
	NCrs = count;

	for(i=0; i<NBT; i++)
	{
		for(k=0; k<2; k++)
		{
			cml = BriTunAry[i][k];

			j=0;
			while(j<NCrs && cml>CrsEar[j].cml)
				j++;
			if(j==NCrs) j--;
			if(j>0 && fabs(cml-CrsEar[j].cml)>0.001 && fabs(cml-CrsEar[j-1].cml)>0.001)
			{

				for(int n=NCrs; n>j; n--)
					CrsEar[n] = CrsEar[n-1];
				if(cml- CrsEar[j-1].cml < CrsEar[j].cml - cml)//取前面
					CrsEar[j] = CrsEar[j-1];
				CrsEar[j].cml = cml;
				NCrs++;
			}			
		}		
	}

	XLDataBase DBS;
	DBS.NTW = NCrs; 
	if(DBS.TWB) {delete []DBS.TWB; DBS.TWB=NULL;}
	if(DBS.NTW>0)
		DBS.TWB = new XLDataBase::TWTAB[DBS.NTW];

	DBS.NSTA = NCrs;
	if(DBS.STA) delete[]DBS.STA;DBS.STA=NULL;
	if(DBS.NSTA>0)
		DBS.STA = new  LCBdata[DBS.NSTA];

	CString DmxFname;
	DmxFname = Cworkdir + "\\data\\" + "hdmdmx." + Pro + "." + RoadName;
	fpw = _wfopen(DmxFname,L"w");
	if(fpw==NULL)
		return;
	fwprintf(fpw,L"%d\n",NCrs);	  

	for(i=0; i<NCrs; i++)
	{
		//2.写纵断面地面线表
		CString GH;
		DBS.TWB[i].ml = pm->XLC(CrsEar[i].cml,GH);			
		DBS.TWB[i].DMGC = CrsEar[i].EarH;
		//2.写桩号序列
		DBS.STA[i].lc = CrsEar[i].cml;
		DBS.STA[i].gldmlc = -1;//暂时
		DBS.STA[i].glRoadName="";


		///////////////////////////////////////////////////
		fwprintf(fpw,L"%0.3lf %0.3lf %d %d\n",DBS.TWB[i].ml,CrsEar[i].EarH,CrsEar[i].NumL+1,CrsEar[i].NumR);

		fwprintf(fpw,L"0.0 0.0 ");
		if(CrsEar[i].NumL>0)
		{
			for(k=CrsEar[i].NumL-1; k>=0; k--)
				fwprintf(fpw,L"%0.3lf %0.3lf ",CrsEar[i].Learpt[k*2],CrsEar[i].Learpt[k*2+1]);
		}
		fwprintf(fpw,L"\n");
		for(k=0; k<CrsEar[i].NumR; k++)
			fwprintf(fpw,L"%0.3lf %0.3lf ",CrsEar[i].Rearpt[k*2],CrsEar[i].Rearpt[k*2+1]);
		fwprintf(fpw,L"\n");
		//	if(CrsEar[i].Learpt) delete[]CrsEar[i].Learpt;
		//	if(CrsEar[i].Rearpt) delete[]CrsEar[i].Rearpt;

	}
	ads_printf(L"\nOK!");
	fclose(fpw);

	_stprintf(mes,L"%s横断面地面线已保存在%s中!",RoadName,DmxFname);
	ads_alert(mes);
	//4.写纵断面地面线文件
	DBS.Write_XLDbs(mdbname,L"纵地面线表",RoadName);
	if(DBS.TWB) delete []DBS.TWB;

	//3.写桩号序列
	DBS.Write_XLDbs(mdbname,L"桩号序列",RoadName); 
	if(DBS.STA) delete []DBS.STA;

	if(CrsEar) delete[]CrsEar;
	//	 AfxMessageBox(L"路基设计表计算完毕!");	
}

void HdmDmx::ReadBriTunData(JD_CENTER *pm,CString mdbFname,CString RoadName,int &NBT,double BriTunAry[500][2])
{
	XLDataBase mdb;
	int NBRI,NTUN;
	mdb.Read_XLDbs(mdbFname,L"大中桥",RoadName);		
	mdb.Read_XLDbs(mdbFname,L"隧道",RoadName);

	NBRI = mdb.BriNum;
	int j;
	for( j=0; j<NBRI; j++)
	{
		BriTunAry[j][0] = pm->TYLC(mdb.Bri[j].StDml);
		BriTunAry[j][1] = pm->TYLC(mdb.Bri[j].EdDml);
	}

	NTUN = mdb.TunNum;
	for(j=0; j<NTUN; j++)
	{
		BriTunAry[NBRI+j][0] = pm->TYLC(mdb.Tun[j].StDml);
		BriTunAry[NBRI+j][1] = pm->TYLC(mdb.Tun[j].EdDml);
	}

	NBT = NBRI + NTUN;
}

void HdmDmx::GetHdmDmxFromZDKandDTM(JD_CENTER *pm,CString mdbname,CString RoadName)
{

	HdmDataBase hdmmdb;
	ACHAR fname[256],mes[256],ck[50],ckml[80];
	FILE *fpr,*fpr1,*fpw,*fpw1,*fpw2,*fpw3;
	int iKml,i,j,sum,num,k,len;
	ACHAR mlstr[300],mlstr1[80],ml[80];
	double dml,bml,Bh,cml,Hdkdml;
	double WB3,WB2,WB1,WA1,WA2,WA3;
	double deltaZH[2],deltaYH[2],deltaMH;
	double dtmp,x,y,Bjx,ik;
	int ptsum,inf,lnum,rnum,itmp,Snum,Enum,ijd,iwid;
	double NSB,NSE;
	double sml,eml;
	int NCrs;
	//  GSZHDlg   dlg;
	int fsum;
	struct filearray{
		ACHAR zdk[280];
	}  Fname[200];
	ACHAR zdk[280];
	ACHAR hdk[280];
	int ifile;
	struct Hdmdmxdata
	{
		double cml,dml;
		double EarH;
		int NumL,NumR;
		double Learpt[400],Rearpt[400];
	}*CrsEar;
	int SKML,EKML;
	SKML = (int)((pm->DLArray[0].ELC+0.0001)/ 1000.0);
	EKML = (int)((pm->DLArray[pm->DLNum-1].BLC+0.0001)/ 1000.0);
	sml = pm->DLArray[0].ELC;
	eml = pm->DLArray[pm->DLNum-1].BLC;
	fsum = EKML - SKML + 1;//文件总数
	for(i=0; i<fsum; i++)
		_stprintf(Fname[i].zdk,L"%s\\data\\%s\\ZDK%d.txt",Cworkdir,RoadName,SKML+i);
	NCrs = 0;
	//1.计算横断面总数
	//处理第一个文件
	if((fpr=_wfopen(Fname[0].zdk,L"r"))!=NULL)
	{
		fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
		Snum = 0;
		for(j=0;j<sum;j++)
		{
			fwscanf(fpr ,L"%lf%lf" ,&dml,&Bh);
			if(dml < sml-0.1)
				Snum++;
			else
				break;
		}
		NCrs = sum - Snum;
		fclose(fpr);			
	}
	else
	{
		ads_printf(L"错误: 无法找到%s文件!\n",Fname[0].zdk);
		// AfxMessageBox(mes);
	}

	for(i=1;i<fsum-1; i++)
	{
		if((fpr=_wfopen(Fname[i].zdk,L"r"))!=NULL)
		{
			fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
			fclose(fpr);
			NCrs += sum;
		}
	}

	if((fpr=_wfopen(Fname[fsum-1].zdk,L"r"))!=NULL)
	{
		fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
		Enum = 0;
		for(j=0;j<sum;j++)
		{
			fwscanf(fpr ,L"%lf%lf" ,&dml,&Bh);
			if(dml < eml+0.1)
				Enum++;
			else
				break;
		}
		fclose(fpr);
		NCrs += Enum;
	}
	else
	{
		ads_printf(L"无法找到%s文件!\n",Fname[fsum-1].zdk);
		// AfxMessageBox(mes);
	}
	if(NCrs<1)
	{
		ads_alert(L"桩数为0,请将外业测量数据存入ZDK文件!");
		return;
	}

	int NBT;
	double BriTunAry[500][2];
	double LLd,RRd;
	double xl[200],yl[200],zl[200];
	double xr[200],yr[200],zr[200];

	ReadBriTunData(pm,mdbname,RoadName,NBT,BriTunAry);


	CrsEar = new Hdmdmxdata[NCrs+2*NBT];
	int count=0;
	for(ifile=0;ifile<fsum; ifile++)
	{
		if((fpr=_wfopen(Fname[ifile].zdk,L"r"))==NULL)
		{
			ads_printf(L"无法找到%s文件!\n",Fname[ifile].zdk);
			// AfxMessageBox(mes);
			continue;
		}


		fwscanf(fpr ,L"%s%d" ,mlstr,&sum);
		CString GH;
		len=F_num(mlstr,'K');
		for(i=1,k=0; i<len;k++, i++)
			ck[k] = mlstr[i];
		ck[k] = '\0';
		GH = ck;

		if(ifile == 0)//第一个文件空读Snum个
		{
			for(j=0;j<Snum;j++)
				fwscanf(fpr ,L"%lf%lf" ,&dtmp,&dtmp);
			sum = sum - Snum;
		}

		if(ifile == fsum-1)//最后一个文件只读Enum个
			sum = Enum;
		//	sum = (int)(_wtof(nstr));
		for(j=0; j<sum;j++)
		{
			fwscanf(fpr ,L"%lf%lf" ,&dml,&Bh);
			//读横断面地面点数据
			ads_printf(L"处理ZDK中 %lf\n",dml);
			cml=pm->TYLC(dml);

			CrsEar[count].cml=cml;
			CrsEar[count].EarH=Bh;
			count++;
		}
	}
	//将桥隧里程插入
	NCrs = count;
	for(i=0; i<NBT; i++)
	{
		for(k=0; k<2; k++)
		{
			cml = BriTunAry[i][k];
			j=0;
			while(j<NCrs && cml>CrsEar[j].cml)
				j++;
			if(j>0 && fabs(cml-CrsEar[j].cml)>0.001 && fabs(cml-CrsEar[j-1].cml)>0.001)
			{
				for(int n=NCrs; n>j; n--)
					CrsEar[n] = CrsEar[n-1];
				if(cml- CrsEar[j-1].cml < CrsEar[j].cml - cml)//取前面
					CrsEar[j] = CrsEar[j-1];
				CrsEar[j].cml = cml;
				NCrs++;
			}			
		}		
	}

	XLDataBase DBS;
	DBS.NTW = NCrs; 
	if(DBS.TWB) {delete []DBS.TWB; DBS.TWB=NULL;}
	if(DBS.NTW>0)
		DBS.TWB = new XLDataBase::TWTAB[DBS.NTW];

	DBS.NSTA = NCrs;
	if(DBS.STA) delete[]DBS.STA;DBS.STA=NULL;
	if(DBS.NSTA>0)
		DBS.STA = new  LCBdata[DBS.NSTA];


	//读数模
	int IsDtmExisting=0;


	if(ptriDTM.Read_Dtm())
		IsDtmExisting=1;
	else
	{
		IsDtmExisting=0;//数模不存在    
		ads_initget(2+4,NULL);
		if(ads_getreal(L"\n  请输入地面高程的平均值<100> ",&ptriDTM.DefaultH)==RTNONE)
			ptriDTM.DefaultH = 100;
	}


	EdgeNode *CrsDmxHead,*Tmp;
	int LPnum,RPnum;

	CString DmxFname;
	DmxFname = Cworkdir + "\\data\\" + "hdmdmx." + Pro + "." + RoadName;
	fpw = _wfopen(DmxFname,L"w");
	if(fpw==NULL)
		return;
	fwprintf(fpw,L"%d\n",NCrs);	  

	ACHAR pszLabel[256];
	_stprintf(pszLabel,L"%s横断面地面线内插",road[i].roadname);
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	for(i=0; i<NCrs; i++)
	{
		//2.写纵断面地面线表
		CString GH;
		double dml;
		dml = pm->XLC(CrsEar[i].cml,GH);
		DBS.TWB[i].ml = dml;			
		DBS.TWB[i].DMGC = CrsEar[i].EarH;			
		//2.写桩号序列
		DBS.STA[i].lc = CrsEar[i].cml;
		DBS.STA[i].gldmlc=-1;
		DBS.STA[i].glRoadName="";
		//3.数模内插相对高差
		int k;
		cml = CrsEar[i].cml;//统一里程
		//计算左右地面点
		LLd = FindIdmxk(pm,dml,hdmmdb.NZCRS,hdmmdb.ZCRSWID);
		RRd = FindIdmxk(pm,dml,hdmmdb.NYCRS,hdmmdb.YCRSWID);

		xlpoint PZ;
		PZ.lc=cml;
		pm->xlpoint_pz(&PZ);
		double pt1[3],pt2[3],cita,x,y,yyt,Yd;
		Yd = 0.0;//初始值
		pt1[0]=PZ.x;pt1[1]=PZ.y;pt1[2]=0.0;
		cita=PZ.a;
		x=PZ.x;
		y=PZ.y;
		yyt=cita-0.5*pi;
		pt2[0]=x+LLd*cos(yyt);
		pt2[1]=y+LLd*sin(yyt);
		pt2[2]=0.0;

		if(ptriDTM.DTMHasRead)
			CrsDmxHead = ptriDTM.FindInterPt(pt1, pt2,LPnum);
		else
			LPnum = 0;

		if(LPnum>0)//左侧地面线有点
		{
			k=0;
			while(CrsDmxHead)
			{
				xl[k] =  CrsDmxHead->pt[0] ;
				yl[k] =  CrsDmxHead->pt[1] ;
				zl[k] =  CrsDmxHead->pt[2] ;

				Tmp = CrsDmxHead;
				CrsDmxHead = CrsDmxHead->next; 
				free(Tmp);
				k++;
			}

			if(k>0)
			{				  
				Yd = zl[0];
				if(Yd<0.1&&k>1)
				{
					Yd = zl[1];
				}
			}        

			for(k=1; k<LPnum; k++)
			{
				xl[k] = sqrt((xl[k]-xl[0])*(xl[k]-xl[0])+(yl[k]-yl[0])*(yl[k]-yl[0])); 

				if(zl[k]<-100.0)//没数模 按中桩
					zl[k] = Yd;
				yl[k] = zl[k]-Yd;	

			}	
			xl[0] = 0.0;
			yl[0] = 0.0;
		}//至少有一个点
		else//一个点也没有
		{
			LPnum = 2;
			Yd = 0.0;
			xl[0] = 0.0;
			yl[0] = 0.0;

			xl[1] = LLd;
			yl[1] = 0.0;
		}

		yyt = cita + 0.5*pi;
		pt2[0] = x + RRd*cos(yyt);
		pt2[1] = y + RRd*sin(yyt);
		pt2[2] = 0.0;

		CrsDmxHead = ptriDTM.FindInterPt(pt1, pt2,RPnum);

		//计算横断面左侧地面点坐标
		if(RPnum >0)
		{
			k=0;
			while(CrsDmxHead)
			{
				xr[k] =  CrsDmxHead->pt[0] ;
				yr[k] =  CrsDmxHead->pt[1] ;
				zr[k] =  CrsDmxHead->pt[2] ;
				Tmp = CrsDmxHead;
				CrsDmxHead = CrsDmxHead->next; 
				free(Tmp);
				k++;
			}
			for(k=1; k<RPnum; k++)
			{
				xr[k] = sqrt((xr[k]-xr[0])*(xr[k]-xr[0])+(yr[k]-yr[0])*(yr[k]-yr[0])); 

				if(zr[k]<-100.0)//没数模 按中桩
					zr[k] = Yd;
				yr[k] = zr[k]-Yd;					          				      
			}
			xr[0] = 0.0;
			yr[0] = 0.0;
		}
		else
		{
			RPnum = 2;
			Yd = 0.0;
			xr[0] = 0.0;
			yr[0] = 0.0;

			xr[1] = RRd;
			yr[1] = 0.0;
		}

		//地面线付值							
		CrsEar[i].NumL = LPnum;
		//	CrsEar[i].Learpt = new double[LPnum*2];
		for(k=0; k<LPnum; k++)
		{
			CrsEar[i].Learpt[k*2] = xl[k];
			CrsEar[i].Learpt[k*2+1]=yl[k];			
		}
		CrsEar[i].NumR = RPnum;
		//	CrsEar[i].Rearpt = new double[RPnum*2];
		for(k=0; k<RPnum; k++)
		{
			CrsEar[i].Rearpt[k*2] = xr[k];
			CrsEar[i].Rearpt[k*2+1]=yr[k];			
		}
		int Pos = (int)(j*100.0/road[i].NSTA);
		acedSetStatusBarProgressMeterPos(Pos);

		fwprintf(fpw,L"%0.3lf %0.3lf %d %d\n",DBS.TWB[i].ml,CrsEar[i].EarH,CrsEar[i].NumL,CrsEar[i].NumR);
		for(k=0; k<CrsEar[i].NumL; k++)
			fwprintf(fpw,L"%0.3lf %0.3lf ",CrsEar[i].Learpt[k*2],CrsEar[i].Learpt[k*2+1]);
		fwprintf(fpw,L"\n");
		for(k=0; k<CrsEar[i].NumR; k++)
			fwprintf(fpw,L"%0.3lf %0.3lf ",CrsEar[i].Rearpt[k*2],CrsEar[i].Rearpt[k*2+1]);
		fwprintf(fpw,L"\n");
		//	if(CrsEar[i].Learpt) delete[]CrsEar[i].Learpt;
		//	if(CrsEar[i].Rearpt) delete[]CrsEar[i].Rearpt;

	}
	ads_printf(L"\nOK!");
	fclose(fpw);
	acedSetStatusBarProgressMeterPos(100);    
	acedRestoreStatusBar();
	if(ptriDTM.DTMHasRead)
		ptriDTM.ReleaseDtm();
	_stprintf(mes,L"%s横断面地面线已保存在%s中!",RoadName,DmxFname);
	ads_printf(L"%s\n",mes);
	//4.写纵断面地面线文件

	DBS.Write_XLDbs(mdbname,L"纵地面线表",RoadName);
	if(DBS.TWB) delete []DBS.TWB;

	//3.写桩号序列
	DBS.Write_XLDbs(mdbname,L"桩号序列",RoadName); 
	if(DBS.STA) delete []DBS.STA;

	if(CrsEar) delete[]CrsEar;


}






























































