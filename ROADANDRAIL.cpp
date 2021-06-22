// ROADANDRAIL.cpp : Initialization functions
#include "StdAfx.h"

#include "StdArx.h"
#include "resource.h"
#include <afxdllx.h>
#include "gsmh.h"
#include "netdogapi.h"
#include "zdmsj.h"
#include "GTZDM.h"
#include "BRI.h"
#include "TUN.h"
#include "DMX_LCB.h"
#include "DMX_PQX.h"
#include "DeshBz.h"
#include "EarhBz.h"
#include "GTZDK.h"
#include "trim.h"
#include "MODIFY_GTZDM.h"
#include "XLdatabase.H"
#include "GTZX_JYX.h"
#include "DirdlgDlg.h"


#include "HdmText.h"	// 横断面类
#include "LTGOU.h"	// 路堤水沟类
#include "earthline.h"	// 地面线类
#include "designline.h"	// 地面线类
#include "JSGOU.h"
#include "HdmDes.h"
#include "SPIRAL.H"
#include "EXMANAGER.h"
#include "DockLocationTool.h"
#include "PMDIALOG.h"
#include "triDTM_road.h"  
#include "EditAllRoads.h"
#include "JdCenterInputDlg.h"
#include "accmd.h"
#include <vector>

JdCenterInputDlg *InputJdCenter=NULL;
HINSTANCE _hdllInstance =NULL ; 
DockLocationTool *DockLocationToolDlg=NULL;
DockLocationTool *PMenuBar=NULL;
BaseSettting BaseSetObj;


long PassWord1,PassWord2;
short int DogAddr,DogBytes;
unsigned long DogPassword, NewPassword,DogResult;
unsigned char DogCascade;
void far *DogData=NULL;
unsigned int Retcode;
int rnd1,rnd2;

HANDLE mapfileHandleP,MEMmapfileHandleP;
LPVOID FilePtrP ;
triDTM_road ptriDTM;
PMDIALOG* PmDlg = NULL;
CString Cworkdir,Pro;
ACHAR SysPath[1024];
extern  AcDbVoidPtrArray g_reactorList;


DWORD m_dwDogHandle;		// socket handle
DWORD	m_dwPassword=123456;
//#define LOADMENU


//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
// This command registers an ARX command.


void AddCommand(const ACHAR* cmdGroup, const ACHAR* cmdInt, const ACHAR* cmdLoc,
				const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal = -1);

// NOTE: DO NOT edit the following lines.
//bool CheckPassword();
void InitApplication();
void UnloadApplication();
extern bool CheckDogByDLL();
extern bool CheckByTime();

ACRX_DXF_DEFINE_MEMBERS(JD_CENTER, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,JD_CENTER, L"ROAD");
ACRX_DXF_DEFINE_MEMBERS(EXMANAGER, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,EXMANAGER, L"ROAD");
ACRX_DXF_DEFINE_MEMBERS(GTZDM_ROAD,AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  GTZDM_ROAD, L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(BRI_ROAD,AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0, BRI_ROAD, L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(TUN_ROAD,AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0, TUN_ROAD, L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(DMX_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  DMX_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(DMX_LCB_ROAD,AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  DMX_LCB_ROAD, L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(DMX_PQX_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  DMX_PQX_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(GTZDK_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  GTZDK_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(DeshBz_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  DeshBz_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(EarhBz_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  EarhBz_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(TRIM_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  TRIM_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(GTZX_JYX_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  GTZX_JYX_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(GTZX2_JYX_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  GTZX2_JYX_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(GTZX_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  GTZX_ROAD , L"GUADS");
ACRX_DXF_DEFINE_MEMBERS(GTZX2_ROAD, AcDbEntity,AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,0,  GTZX2_ROAD , L"GUADS");

void MODIFYDATA_ZDM();

ZDMSJ ZDM;

void createGTZDM_JDDATAFILE() 
{  
	ZDM.createGTZDM_JDDATAFILE(); 
}
void changeBPD()
{ 
	ZDM.changeBPD();
}
void createGTZDM_INPUT() 
{ 
	ZDM.createGTZDM_INPUT();
}
void zdmchangeDL() 
{
	ZDM.changeDL();
}
void zdmaddDL() 
{
	ZDM.addDL();
}
void zdmdelDL() 
{
	ZDM.delDL();
}
void add_BPD() 
{
	ZDM.add_BPD();
}
void del_BPD() 
{
	ZDM.del_BPD();
}
void SAVEPD() 
{
	ZDM.SAVEPD();
}
void LINKPDX()
{
	ZDM.LINK_PDLINE();
}
//拉坡设计
void CZMLPSJ()
{
	ZDM.CZmLPSJ();
}
void AutoSlopeData()
{

	ZDM.AutoSlopeData();
}

double gammln(double xx)
{
	int j;
	double temp;
	double cof[6],stp,half,one,fpf,x,tmp,ser;
	cof[1] = 76.18009173;
	cof[2] = -86.50532033;
	cof[3] = 24.01409822;
	cof[4] = -1.231739516;
	cof[5] = 0.00120858003;
	cof[6] = -0.00000536382;
	stp = 2.50662827465;
	half = 0.5;
	one = 1.0;
	fpf = 5.5;
	x = xx - one;
	tmp = x + fpf;
	tmp = (x + half) * log(tmp) - tmp;
	ser = one;
	for (j = 1;j<=6;j++)
	{
		x = x + one;
		ser = ser + cof[j] / x;
	}
	temp = tmp + log(stp * ser);
	return temp;
}
void gser(double& gamser, double& a, double& x, double& gln)
{
	int itmax,n;
	double ap,sum,del,eps;
	itmax = 100;
	eps = 0.0000003;
	gln = gammln(a);
	if (x <= 0.0)
	{
		if (x < 0.0)
		{
			//  cout<<" pause";
			exit(1);
		}
		gamser = 0.0;
		exit(1);
	}
	ap = a;
	sum = 1.0 / a;
	del = sum;
	for( n = 1;n<=itmax;n++)
	{
		ap = ap + 1.0;
		del = del * x / ap;
		sum = sum + del;
		if (fabs(del) < (fabs(sum) * eps))
		{//	goto loop;
			gamser = sum * exp(-x + a * log(x) - gln);
			break;
		}
	}
}

void gcf(double& gammcf, double& a, double& x, double& gln)
{
	int itmax,n;
	double eps,a0,a1,b0,b1,fac,an,ana,anf,gold,g;
	itmax = 100;
	eps = 0.0000003;
	gln = gammln(a);
	gold = 0.0;
	a0 = 1.0;
	a1 = x;
	b0 = 0.0;
	b1 = 1.0;
	fac = 1.0;
	for (n = 1;n<=itmax;n++)
	{
		an = n;
		ana = an - a;
		a0 = (a1 + a0 * ana) * fac;
		b0 = (b1 + b0 * ana) * fac;
		anf = an * fac;
		a1 = x * a0 + anf * a1;
		b1 = x * b0 + anf * b1;
		if (a1 != 0.0)
		{
			fac = 1.0 / a1;
			g = b1 * fac;
			if (fabs((g - gold) / g) < eps )
			{
				gammcf = exp(-x + a * log(x) - gln) * g;
				break;
			}
			gold = g;
		}
	}
}
double gammq(double a, double x)
{
	double temp,gamser,gammcf,gln;
	if (x < 0.0 || a <= 0.0)
	{
		// cout<< "pause";
		exit(1);
	}
	if (x < a + 1.0)
	{
		gser(gamser, a, x, gln);
		temp= 1.0 - gamser;
	}
	else
	{
		gcf(gammcf, a, x, gln);
		temp = gammcf;
	}
	return temp;   
}

void  fit( double x[], double y[], int ndata, double sig[], int mwt, double& a,double& b, double& siga, double& sigb, double& chi2, double& q )
{
	int i;
	double sigdat,t,sxoss,wt,ss,sx = 0.0;
	double sy = 0.0;
	double st2 = 0.0;
	b = 0.0;
	if (mwt != 0)
	{
		ss = 0.0;
		for (i = 1; i<=ndata; i++)
		{
			wt = 1.0 / (sig[i] * sig[i]);
			ss = ss + wt;
			sx = sx + x[i] * wt;
			sy = sy + y[i] * wt;
		}
	}
	else
	{
		for (i = 1; i<=ndata; i++)
		{
			sx = sx + x[i];
			sy = sy + y[i];
		}
		ss = ndata;
	}
	sxoss = sx / ss;
	if (mwt != 0)
	{
		for (i = 1; i<=ndata; i++)
		{
			t = (x[i] - sxoss) / sig[i];
			st2 = st2 + t * t;
			b = b + t * y[i] / sig[i];
		}
	}
	else
	{
		for (i = 1; i<=ndata; i++)
		{
			t = x[i] - sxoss;
			st2 = st2 + t * t;
			b = b + t * y[i];
		}
	}
	b = b / st2;
	a = (sy - sx * b) / ss;
	siga = sqrt((1.0 + sx * sx / (ss * st2)) / ss);
	sigb = sqrt(1.0 / st2);
	chi2 = 0.0;
	if (mwt == 0)
	{
		for (i = 1; i<=ndata; i++)
		{
			chi2 = chi2 + pow((y[i] - a - b * x[i]) , 2);
		}
		q = 1.0;
		sigdat = sqrt(chi2 / (ndata - 2));
		siga = siga * sigdat;
		sigb = sigb * sigdat;
	}
	else
	{
		for (i = 1; i<=ndata; i++)
		{
			chi2 = chi2 + pow(((y[i] - a - b * x[i]) / sig[i]) , 2);
		}
		q = gammq(0.5 * (ndata - 2), 0.5 * chi2);
	}
}


void pPFITLINE()
{
	struct Point2d
	{
		Point2d(){x=0.0;y=0.0;}
		double x,y;
	}pts[200];

	int num=0;
	ads_point pt;
	do
	{		
		if(RTNORM==acedGetPoint(NULL, L"\nSelect a point:",pt))
		{
			pts[num].x=pt[X];
			pts[num].y=pt[Y];
			num++;
		}
		else break;		
	}while(num<=200);

	if(num==2)
	{
		AcDbLine *Line=new AcDbLine(AcGePoint3d(pts[0].x,pts[0].y,0.0),AcGePoint3d(pts[1].x,pts[1].y,0.0));
		BAS_DRAW_FUN::AddEntityToDbs(Line);
	}
	else if(num>2)
	{
		//求最小、最大X
		double Xmin,Xmax;
		Xmin=pts[0].x;
		Xmax=pts[0].x;
		int i;
		for( i=0;i<num;i++)
		{
			if(Xmin>pts[i].x)Xmin=pts[i].x;
			if(Xmax<pts[i].x)Xmax=pts[i].x;
		}

		double ptx[201],pty[201],sig[201];
		for(i=0; i<=num; i++)
		{
			ptx[i+1] = pts[i].x;
			pty[i+1] = pts[i].y;
			sig[i+1] = 0.01;
		}

		double a,b,siga, sigb, chi2, q;
		fit(ptx, pty,num, sig, 0, a, b, siga, sigb, chi2, q);
		double Ymin,Ymax;
		Ymin=Xmin*b+a;
		Ymax=Xmax*b+a;
		AcDbLine *Line=new AcDbLine(AcGePoint3d(Xmin,Ymin,0.0),AcGePoint3d(Xmax,Ymax,0.0));
		BAS_DRAW_FUN::AddEntityToDbs(Line);
	}
}


//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
// Define the sole extension module object.
AC_IMPLEMENT_EXTENSION_MODULE(ROADANDRAILDLL);

// Now you can use the CAcModuleResourceOverride class in
// your application to switch to the correct resource instance.
// Please see the ObjectARX Documentation for more details

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point
extern "C"

	BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_hdllInstance = hInstance;
		// Extension DLL one time initialization
		ROADANDRAILDLL.AttachInstance(hInstance);
		InitAcUiDLL();
	} else if (dwReason == DLL_PROCESS_DETACH) {
		// Terminate the library before destructors are called
		ROADANDRAILDLL.DetachInstance();

	}
	return TRUE;    // ok
}




/////////////////////////////////////////////////////////////////////////////
// ObjectARX EntryPoint
extern "C" AcRx::AppRetCode 

	acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{

	CString fstr = acedGetAppName();
	switch (msg) 
	{	
	case AcRx::kInitAppMsg:
		{
			// Comment out the following line if your
			// application should be locked into memory
			bool NetDogInf;
			/*if(!CheckPassword())
			return AcRx::kRetError;*/
			NetDogInf = false;

			//////////////////////////////////////////////////
			ACHAR ACAD_PATH[256];
			ACHAR AcadPath[256];
			FILE *fpw;
			ACHAR fname[256];
			ACHAR str[280];

			_tcscpy(fname, L"\0");

			if (!fstr.IsEmpty())
			{
				_tcscpy(fname,fstr);
				fstr.MakeUpper();
				fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
				_tcscpy(SysPath,fstr);
			}
			////////////////////////////////////////////////////(zdf改)

			acrxDynamicLinker->unlockApplication(pkt);
			acrxDynamicLinker->registerAppMDIAware(pkt);

			InitApplication();

			//给CAD上方添加默认的项目名称 如  当前工程项目：C:\改移道路1 [方案号：方案1]
			FILE *fp;
			_stprintf(str,L"%s\\workdir",SysPath);
			int isHaveMdb = 0;//是否存在mdb文件
			CString	m_dirstr = L"";
			CString	m_ProNo = L"";
			CDirdlgDlg ob;
			if((fp=_wfopen(str,L"r"))!=NULL)
			{
				fwscanf(fp,L"%s",str);
				m_dirstr = str;
				fclose(fp);
				ob.InStr$toNull(m_dirstr);
				CString temp = m_dirstr + L"\\PROJECT";
				CFileFind Find;
				if (Find.FindFile(temp))
					isHaveMdb = 1;
				if ((fp = _wfopen(temp, L"r")) != NULL)
				{
					fwscanf(fp ,L"%s" , str);
					m_ProNo = str;
					fclose(fp);
				}
				ob.InStr$toNull(m_ProNo);
			}
			if (m_dirstr != L"" && m_ProNo != L"" && isHaveMdb == 1)
			{
				CString str;
				str = L"当前工程项目:" + m_dirstr + L"  [方案号: " + m_ProNo + L"]";
				//_stprintf(mes,L" %s 方案号: %s",,);
				acedGetAcadFrame()->SetWindowText(str);
			}
		}
		break;
	case AcRx::kUnloadAppMsg:

		UnloadApplication();
		break;
	}

	return AcRx::kRetOK;
}
////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////

// Init this application. Register your
// commands, reactors...

void InitApplication()
{
#ifdef DIGIROAD
	if(!CheckByTime() || !CheckDogByDLL())
	{
		return;
	}

	prmenurmenu();
#endif

#ifdef GY_ROAD_DULI_SYS
	if(!CheckByTime() || !CheckDogByDLL())
	{
		return;
	}
	prmenurmenu();
#endif
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_INIT
	//////////////////////////////////////////////////////////////////////////
	//改移道路
	AddCommand(L"PENGLIHUI", L"DeleteJZX", L"DeleteJZX", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, DeleteJZX);
	AddCommand(L"PENGLIHUI", L"EditAllRoadS", L"EditAllRoadS", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, EditAllRoadS);//编辑所有改移道路数据
	AddCommand(L"PENGLIHUI", L"OutAllRoadSTab", L"OutAllRoadSTab", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, OutAllRoadSTab);//输出所有改移道路表格
	AddCommand(L"PENGLIHUI", L"OutQXJKCGTab", L"OutQXJKCGTab", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, OutQXJKCGTab);//输出所有改移道路表格
	AddCommand(L"PENGLIHUI", L"ADDPMTK", L"ADDPMTK", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ADDPMTK);//新建图框
	AddCommand(L"PENGLIHUI", L"OPENPMTK", L"OPENPMTK", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, OPENPMTK);//导入图框
	AddCommand(L"PENGLIHUI", L"SAVEPMTK", L"SAVEPMTK", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SAVEPMTK);//保存图框
	AddCommand(L"PENGLIHUI", L"TRIMPMT", L"TRIMPMT", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, TRIMPMT);//自动成图
	//	
	//	//////////////////////////////////////////////////////////////////////////
	AddCommand(L"PENGLIHUI", L"NewMdfRoad", L"NewMdfRoad", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, NewMdfRoad);
	AddCommand(L"PENGLIHUI", L"SelMdfRoad", L"SelMdfRoad", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SelMdfRoad);
	AddCommand(L"PENGLIHUI", L"INJD", L"INJD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiinputjdcenter);
	AddCommand(L"PENGLIHUI", L"PZOOM", L"PZOOM", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiPZOOM);
	AddCommand(L"PENGLIHUI", L"DRAWQXYS_Road", L"DRAWQXYS_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiDRAWQXYS);
	AddCommand(L"PENGLIHUI", L"DRAWXYYSB_Road", L"DRAWXYYSB_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, DRAWXYYSB);	
	AddCommand(L"PENGLIHUI", L"JCDRAWPM_Road", L"JCDRAWPM_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, /*penglihuic*/penglihuiJCDRAWPM);
	AddCommand(L"PENGLIHUI", L"DRAWGRID_Road", L"DRAWGRID_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, DRAWGRID);
	AddCommand(L"PENGLIHUI", L"DRAWZDM_Road", L"DRAWZDM_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, DRAWZDM);	//绘纵面图	
	AddCommand(L"PENGLIHUI", L"AJUSTAXLE_Road", L"AJUSTAXLE_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, AJUSTAXLE);
	AddCommand(L"PENGLIHUI", L"ADDAXLE_Road", L"ADDAXLE_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ADDAXLE);
	AddCommand(L"PENGLIHUI", L"DELAXLE_Road", L"DELAXLE_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, DELAXLE);
	AddCommand(L"PENGLIHUI", L"OUTZDM_Road", L"OUTZDM_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, OUTZDM);
	AddCommand(L"PENGLIHUI", L"ZXYTOXL", L"ZXYTOXL", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiZXYTOXL);
	AddCommand(L"PENGLIHUI", L"SAVEPMDBS", L"SAVEPMDBS", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiSAVEPMDBS);
	AddCommand(L"PENGLIHUI", L"OPENPMDBSBYJD", L"OPENPMDBSBYJD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiOPENPMDBSBYJD);
	AddCommand(L"PENGLIHUI", L"JCADDDL", L"JCADDDL", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCADDDL);
	AddCommand(L"PENGLIHUI", L"JCADDJD", L"JCADDJD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCADDJD);
	AddCommand(L"PENGLIHUI", L"JCDELJD", L"JCDELJD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCDELJD);
	AddCommand(L"PENGLIHUI", L"JCEDITJD", L"JCEDITJD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCEDITJD);
	AddCommand(L"PENGLIHUI", L"JCDELDL", L"JCDELDL", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCDELDL);
	AddCommand(L"PENGLIHUI", L"JCDLB", L"JCDLB", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, JCZMDLB);
	AddCommand(L"PENGLIHUI", L"JCZMDLB_Road", L"JCZMDLB_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCDLB);
	AddCommand(L"PENGLIHUI", L"JCAUTOPM_Road", L"JCAUTOPM_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuiJCAUTOPM);
	AddCommand(L"PENGLIHUI", L"LMK", L"LMK", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SetLMK);
	AddCommand(L"PENGLIHUI", L"CALCGFD", L"CALCGFD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, CalCGFD);
	AddCommand(L"PENGLIHUI", L"CXCGFD", L"CXCGFD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, CXCGFD);
	AddCommand(L"PENGLIHUI", L"SetCGBZ", L"SetCGBZ", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SetCGBZ);
	AddCommand(L"PENGLIHUI", L"SetJKBZ", L"SetJKBZ", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SetJKBZ);
	AddCommand(L"RAILDESIGN", L"RAUTODES", L"RAUTODES", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, RAutoDesign);
	AddCommand(L"RAILDESIGN", L"ZDCT", L"ZDCT", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ZDCT);
	AddCommand(L"RAILDESIGN", L"GJCT", L"GJCT", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, GJCT);
	//	penglihuitest	
	AddCommand(L"PENGLIHUI", L"ROADTEST", L"ROADTEST", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, penglihuitest);
	////纵断面	
	//	//导入纵断面方案
	AddCommand(L"PENGLIHUI", L"INPUTZMFA_Road", L"INPUTZMFA_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pinputzmfainputzmfa);
	//	//指定平面方案
	AddCommand(L"PENGLIHUI", L"ZM_ZDPMFA_Road", L"ZM_ZDPMFA_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pzm_zdpmfazm_zdpmfa);
	//	//设置纵面标准
	AddCommand(L"PENGLIHUI", L"SetZMBZ_Road", L"SetZMBZ_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SetZMBZ);
	//	高程控制点
	AddCommand(L"RAILDESIGN", L"SETCTRLH", L"SETCTRLH", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SetLevelCtrl);
	//	接坡信息
	AddCommand(L"RAILDESIGN", L"JPInfor", L"JPInfor", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, SetJPInfor);
	//	//点绘地面线
	AddCommand(L"PENGLIHUI", L"DMX_Road", L"DMX_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ptestdmx);
	//	//采集地面线
	AddCommand(L"RAILDESIGN", L"PICKZDMDMX_Road", L"PICKZDMDMX_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pPickZdmDmxPickZdmDmx);
	//	//编辑地面点
	AddCommand(L"RAILDESIGN", L"EDITDMD_Road", L"EDITDMD_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, peditdmdeditdmd);
	//	AddCommand(L"RAILDESIGN", L"ZDJP", L"ZDJP", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ZDJP);
	//	//绘控制标高
	//	AddCommand(L"RAILDESIGN", L"KZBG", L"KZBG", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, DrawKZBG);
	//	//纵面初始化
	AddCommand(L"PENGLIHUI", L"INITZDM_Road", L"INITZDM_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pINITZDMINITZDM);
	AddCommand(L"PENGLIHUI", L"SAVEZDMPRJ_Road", L"SAVEZDMPRJ_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, psavezdmprjsavezdmprj);
	AddCommand(L"PENGLIHUI", L"CalBri_Road", L"CalBri_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pCalBriCalBri);
	AddCommand(L"PENGLIHUI", L"DelBri_Road", L"DelBri_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pdelbridelbri);
	AddCommand(L"PENGLIHUI", L"AddBri_Road", L"AddBri_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, paddBriaddBri);
	AddCommand(L"PENGLIHUI", L"EDITBri_Road", L"EDITBri_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pEditBriEditBri);
	AddCommand(L"PENGLIHUI", L"CalTun_Road", L"CalTun_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pCALTUNCALTUN);
	AddCommand(L"PENGLIHUI", L"DelTun_Road", L"DelTun_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pdeltundeltun);
	AddCommand(L"PENGLIHUI", L"AddTun_Road", L"AddTun_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, paddtunaddtun);
	AddCommand(L"PENGLIHUI", L"EDITTun_Road", L"EDITTun_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pEDITTunEDITTun);
	AddCommand(L"PENGLIHUI", L"MdbInTun_Road", L"MdbInTun_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pMdbInTunMdbInTun);
	AddCommand(L"PENGLIHUI", L"InputBri_Road", L"InputBri_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pInputBriInputBri);
	AddCommand(L"PENGLIHUI", L"MDYPD_Road", L"MDYPD_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pMdyPDMdyPD);
	//
	AddCommand(L"PENGLIHUI", L"ZDKDES_ROAD", L"ZDKDES_ROAD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, raildrawpmzttdrawpmztt);
	AddCommand(L"PENGLIHUI", L"SAVEZDK_ROAD", L"SAVEZDK_ROAD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, railsavezdksavezdk);
	AddCommand(L"PENGLIHUI", L"EDITZDK_ROAD", L"EDITZDK_ROAD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, raileditzdkeditzdk);
	AddCommand(L"PENGLIHUI", L"CHANGEZDK_ROAD", L"CHANGEZDK_ROAD", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, railchangezdkchangezdk);


	AddCommand(L"PENGLIHUI", L"DX3D", L"DX3D", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, pDX3D);


	//	//}}AFX_ARX_INIT
	////纵断面	
	acedRegCmds->addCommand(L"PENGLIHUI", L"AutoZDM_Road", L"AutoZDM_Road", ACRX_CMD_MODAL, &AutoSlopeData);//一键纵坡
	acedRegCmds->addCommand(L"PENGLIHUI", L"IDZDM_Road",  L"IDZDM_Road", ACRX_CMD_MODAL,  &createGTZDM_INPUT);//直线转坡度线
	acedRegCmds->addCommand(L"PENGLIHUI", L"CZMLPSJ_Road", L"CZMLPSJ_Road",   ACRX_CMD_MODAL,    &CZMLPSJ);	
	acedRegCmds->addCommand(L"PENGLIHUI", L"AddBPD_Road",     L"AddBPD_Road",     ACRX_CMD_MODAL,    &add_BPD);
	acedRegCmds->addCommand(L"PENGLIHUI", L"DELBPD_Road",     L"DELBPD_Road",     ACRX_CMD_MODAL,    &del_BPD);
	acedRegCmds->addCommand(L"PENGLIHUI", L"LINKPDX_Road",     L"LINKPDX_Road",    ACRX_CMD_MODAL,    &LINKPDX);
	acedRegCmds->addCommand(L"PENGLIHUI", L"ZPB_Road", L"ZPB_Road",ACRX_CMD_MODAL,&MODIFYDATA_ZDM);//纵坡表编辑数据

	acedRegCmds->addCommand(L"PENGLIHUI", L"HdmJBCS_Road", L"HdmJBCS_Road",ACRX_CMD_MODAL,&HdmJBCS2);//RIBBON横断面基本参数设置命令
	acedRegCmds->addCommand(L"PENGLIHUI", L"CALSTA_Road", L"CALSTA_Road",ACRX_CMD_MODAL,&CALSTA);//纵坡表编辑数据
	//提取横断面地面线
	acedRegCmds->addCommand(L"PENGLIHUI", L"GetDmx_Road", L"GetDmx_Road",ACRX_CMD_MODAL,&GetDmx);
	acedRegCmds->addCommand(L"RAILDESIGN", L"ModifyDMX", L"ModifyDMX", ACRX_CMD_MODAL, &ModifyDmx);//自动设计
	acedRegCmds->addCommand(L"PENGLIHUI", L"HdmAutoDes_Road", L"HdmAutoDes_Road",ACRX_CMD_MODAL,&HdmAutoDes);
	acedRegCmds->addCommand(L"PENGLIHUI", L"InHdm_Road", L"InHdm_Road",ACRX_CMD_MODAL,&INHDM);
	acedRegCmds->addCommand(L"PENGLIHUI", L"EDITHDMBP_Road", L"EDITHDMBP_Road",ACRX_CMD_MODAL,&EDITHDMBP);
	acedRegCmds->addCommand(L"PENGLIHUI", L"GUTINTER", L"GUTINTER", ACRX_CMD_MODAL, &GUTINTERACT);
	acedRegCmds->addCommand(L"PENGLIHUI", L"SAVEHDM_Road", L"SAVEHDM_Road",ACRX_CMD_MODAL,&SAVEHDM);
	acedRegCmds->addCommand(L"PENGLIHUI", L" DRAWHDMT_Road", L"DRAWHDMT_Road",ACRX_CMD_MODAL,&DRAWHDMT);
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTHDM_Road", L"OUTHDM_Road",ACRX_CMD_MODAL,&OUTHDM);
	acedRegCmds->addCommand(L"PENGLIHUI", L" AdjGouPD_Road", L"AdjGouPD_Road",ACRX_CMD_MODAL,&AdjustGouPD);
	//表格

	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTZZYDZB_Road", L"OUTZZYDZB_Road",ACRX_CMD_MODAL,&OutZZYDZB);
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTTSFJSB_Road", L"OUTTSFJSB_Road",ACRX_CMD_MODAL,&OutTSFJSB);
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTGCSLB_Road", L"OUTGCSLB_Road", ACRX_CMD_MODAL, &OutGCSLB);
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTDYT_Road", L"OUTDYT_Road", ACRX_CMD_MODAL, &OutDYT);

	//平面设计
	acedRegCmds->addCommand(L"PENGLIHUI", L" JCADDXL", L"JCADDXL",ACRX_CMD_MODAL,&penglihuiJCADDXL);//连续布线
	acedRegCmds->addCommand(L"PENGLIHUI", L" RPMLOC", L"RPMLOC",ACRX_CMD_MODAL,&ROADANDRAILRPMLOC);//离散布线
	acedRegCmds->addCommand(L"PENGLIHUI", L" JCREVERSE", L"JCREVERSE",ACRX_CMD_MODAL,&penglihuiJCREVERSE);//线路反向
	acedRegCmds->addCommand(L"PENGLIHUI", L" XLLINK", L"XLLINK",ACRX_CMD_MODAL,&penglihuiXLLINK);//线路连接
	acedRegCmds->addCommand(L"PENGLIHUI", L" OPENPMDBSBYXY", L"OPENPMDBSBYXY",ACRX_CMD_MODAL,&penglihuiOPENPMDBSBYXY);//导入方案
	acedRegCmds->addCommand(L"PENGLIHUI", L" INXY", L"INXY",ACRX_CMD_MODAL,&ROADANDRAILINXY);//录入查询
	acedRegCmds->addCommand(L"PENGLIHUI", L" ZJBSCD", L"ZJBSCD",ACRX_CMD_MODAL,&ZJBSCD);//直接式变速车道
	acedRegCmds->addCommand(L"PENGLIHUI", L" LOCATIONTOOL", L"LOCATIONTOOL",ACRX_CMD_MODAL,&penglihuiLOCATIONTOOL);//模式布线
	acedRegCmds->addCommand(L"PENGLIHUI", L" JMBX", L"JMBX",ACRX_CMD_MODAL,&JMBX);//积木布线
	acedRegCmds->addCommand(L"PENGLIHUI", L" BL", L"BL",ACRX_CMD_MODAL,&penglihuibl);//积木布线-直线
	acedRegCmds->addCommand(L"PENGLIHUI", L" BA", L"BA",ACRX_CMD_MODAL,&penglihuiBA);//积木布线-圆
	acedRegCmds->addCommand(L"PENGLIHUI", L" BS", L"BS",ACRX_CMD_MODAL,&penglihuibs);//积木布线-缓和曲线
	acedRegCmds->addCommand(L"PENGLIHUI", L" CXJ", L"CXJ",ACRX_CMD_MODAL,&penglihuiCXJ);//线元转交点
	acedRegCmds->addCommand(L"PENGLIHUI", L" Lmk", L"Lmk",ACRX_CMD_MODAL,&EditLMK);//路面宽度
	acedRegCmds->addCommand(L"PENGLIHUI", L" DrawLMX", L"DrawLMX",ACRX_CMD_MODAL,&DrawLmx);//绘路面线
	acedRegCmds->addCommand(L"PENGLIHUI", L" DrawPZX", L"DrawPZX",ACRX_CMD_MODAL,&DrawPZX);//绘偏置线
	acedRegCmds->addCommand(L"PENGLIHUI", L" GetLmk", L"GetLmk",ACRX_CMD_MODAL,&GetLMK);//提取路面宽
	acedRegCmds->addCommand(L"PENGLIHUI", L" EditLMK", L"EditLMK",ACRX_CMD_MODAL,&EditLMK);//提取路面宽
	acedRegCmds->addCommand(L"PENGLIHUI", L" CalBD", L"CalBD",ACRX_CMD_MODAL,&CalBD);//鼻端计算
	acedRegCmds->addCommand(L"PENGLIHUI", L" SetCGBZ", L"SetCGBZ",ACRX_CMD_MODAL,&SetCGBZ);//设置超高标准
	acedRegCmds->addCommand(L"PENGLIHUI", L" SetJKBZ", L"SetJKBZ",ACRX_CMD_MODAL,&SetJKBZ);//设置加宽标准
	acedRegCmds->addCommand(L"PENGLIHUI", L" JCPROJPOINT", L"JCPROJPOINT",ACRX_CMD_MODAL,&penglihuiJCPROJPOINT);//绘切垂线
	acedRegCmds->addCommand(L"PENGLIHUI", L" JCJDTDFS", L"JCJDTDFS",ACRX_CMD_MODAL,&penglihuiJCJDTDFS);//拖动设置
	acedRegCmds->addCommand(L"RAILDESIGN", L" RAUTODES", L"RAUTODES", ACRX_CMD_MODAL, &RAutoDesign);//自动设计
	acedRegCmds->addCommand(L"RAILDESIGN", L" ZDCT", L"ZDCT", ACRX_CMD_TRANSPARENT, &ZDCT);//2020322梁柱
	acedRegCmds->addCommand(L"RAILDESIGN", L" GJCT", L"GJCT", ACRX_CMD_MODAL, &GJCT);//

	//自动设计
	AddCommand(L"RAILDESIGN", L"EDITDMD_Road", L"EDITDMD_Road", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, peditdmdeditdmd);

	//纵断面
	acedRegCmds->addCommand(L"PENGLIHUI", L" INBXDMX", L"INBXDMX",ACRX_CMD_MODAL,&INBXDMX);//数据库导入
	acedRegCmds->addCommand(L"PENGLIHUI", L" BXDMX", L"BXDMX",ACRX_CMD_MODAL,&BxDmx);//内插地面线
	acedRegCmds->addCommand(L"PENGLIHUI", L" SAVEBXDMX", L"SAVEBXDMX",ACRX_CMD_MODAL,&SaveBXDmx);//存入数据库
	acedRegCmds->addCommand(L"PENGLIHUI", L" ZDJP", L"ZDJP",ACRX_CMD_MODAL,&ZDJP);//匝道接坡
	acedRegCmds->addCommand(L"PENGLIHUI", L" KZBG", L"KZBG",ACRX_CMD_MODAL,&DrawKZBG);//绘控制标高

	//横断面


	//图形输出
	acedRegCmds->addCommand(L"PENGLIHUI", L" DrawDBZX", L"DrawDBZX",ACRX_CMD_MODAL,&DrawDBZX);//绘中线
	acedRegCmds->addCommand(L"PENGLIHUI", L" DrawDBCIR", L"DrawDBCIR",ACRX_CMD_MODAL,&DrawDBCIR);//绘鼻端圆
	acedRegCmds->addCommand(L"PENGLIHUI", L" DrawDBHX", L"DrawDBHX",ACRX_CMD_MODAL,&DrawDBHX);//绘横线
	acedRegCmds->addCommand(L"PENGLIHUI", L" DrawDBBZ", L"DrawDBBZ",ACRX_CMD_MODAL,&DrawDBBZ);//端部标注
	acedRegCmds->addCommand(L"PENGLIHUI", L" XGDBBZ", L"XGDBBZ",ACRX_CMD_MODAL,&XGDBBZ);//修改端部

	//表格输出
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTZXQXZJ", L"OUTZXQXZJ",ACRX_CMD_MODAL,&OutZXQXZJ);//直线曲线转角表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTZZZB", L"OUTZZZB",ACRX_CMD_MODAL,&OutZZZB);//逐桩坐标表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTZPSQX", L"OUTZPSQX",ACRX_CMD_MODAL,&OutZPSQX);//纵坡竖曲线表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTMGLTSFB", L"OUTMGLTSFB",ACRX_CMD_MODAL,&OutMGLTSFB);//每公里土石方数量表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTLXTWB", L"OUTLXTWB",ACRX_CMD_MODAL,&OutLXTWB);//路线填挖数量表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTLJSJB", L"OUTLJSJB",ACRX_CMD_MODAL,&OutLJSJB);//路基设计表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OUTBGPSG", L"OUTBGPSG",ACRX_CMD_MODAL,&OutBGPSG);//边沟排水沟设计表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OutQBTB", L"OutQBTB",ACRX_CMD_MODAL,&OutQBTB);//清表数量表
	acedRegCmds->addCommand(L"PENGLIHUI", L" OutWYNB", L"OutWYNB",ACRX_CMD_MODAL,&OutWYNB);//挖淤数量表

	//三维可视
	acedRegCmds->addCommand(L"PENGLIHUI", L" LX3D", L"LX3D",ACRX_CMD_MODAL,&pLX3D);//CAD线路三维建模
	acedRegCmds->addCommand(L"PENGLIHUI", L" GenPers", L"GenPers",ACRX_CMD_MODAL,&GenPers);//挖淤数量表

	//大临
	acedRegCmds->addCommand(L"PENGLIHUI", L" INITLW", L"INITLW", ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, &pInitLw);//CAD线路三维建模
	acedRegCmds->addCommand(L"PENGLIHUI", L" SYSMLW", L"SYSMLW", ACRX_CMD_MODAL, &pSysmLw);//挖淤数量表
	acedRegCmds->addCommand(L"PENGLIHUI", L" ZTOPLW", L"ZTOPLW", ACRX_CMD_MODAL, &pZtopLw);//挖淤数量表

	//辅助工具
	acedRegCmds->addCommand(L"PENGLIHUI", L" PMLCTOZB", L"PMLCTOZB",ACRX_CMD_MODAL,&pPMLCTOZB);//里程转坐标
	acedRegCmds->addCommand(L"PENGLIHUI", L" PMZBTOLC", L"PMZBTOLC",ACRX_CMD_MODAL,&pPMZBTOLC);//坐标转里程
	acedRegCmds->addCommand(L"PENGLIHUI", L" CXLMBG", L"CXLMBG",ACRX_CMD_MODAL,&CXLMBG);//查询路面标高
	acedRegCmds->addCommand(L"PENGLIHUI", L" CXYDAREA", L"CXYDAREA",ACRX_CMD_MODAL,&CXYDAREA);//查询用地面积
	acedRegCmds->addCommand(L"PENGLIHUI", L" ProCheck", L"ProCheck",ACRX_CMD_MODAL,&CheckPro);//方案审查
	acedRegCmds->addCommand(L"PENGLIHUI", L" PmGuide", L"PmGuide",ACRX_CMD_MODAL,&PmGuide);//平面导航
	acedRegCmds->addCommand(L"PENGLIHUI", L" XLJD", L"XLJD",ACRX_CMD_MODAL,&ROADANDRAILXLJD);//线路求交
	acedRegCmds->addCommand(L"PENGLIHUI", L" DELAYER", L"DELAYER",ACRX_CMD_MODAL,&DELAYER);//删除一层
	acedRegCmds->addCommand(L"PENGLIHUI", L" RUBBER", L"RUBBER",ACRX_CMD_MODAL,&RUBBER);//橡皮擦
	acedRegCmds->addCommand(L"PENGLIHUI", L" VIZTOOL", L"VIZTOOL",ACRX_CMD_MODAL,&VIZTOOL);//工具条
	acedRegCmds->addCommand(L"PENGLIHUI", L" RIBBON", L"RIBBON",ACRX_CMD_MODAL,&RIBBON);//Ribbon
	//acedRegCmds->addCommand(L"PENGLIHUI", L" PMENUBAR", L"PMENUBAR", ACRX_CMD_MODAL, &penglihuiPMENUBAR);//经典菜单
	acedRegCmds->addCommand(L"PENGLIHUI", L" PMENUBAR", L"PMENUBAR", ACRX_CMD_MODAL, &prmenurmenu);//经典菜单
	

	//数据转换
	acedRegCmds->addCommand(L"PENGLIHUI", L" WDtoDR", L"WDtoDR",ACRX_CMD_MODAL,&WDtoDR);//橡皮擦
	acedRegCmds->addCommand(L"PENGLIHUI", L" DRtoWD", L"DRtoWD",ACRX_CMD_MODAL,&DRtoWD);//工具条
	acedRegCmds->addCommand(L"PENGLIHUI", L" WDdmxtoDR", L"WDdmxtoDR",ACRX_CMD_MODAL,&WDdmxtoDR);//橡皮擦
	acedRegCmds->addCommand(L"PENGLIHUI", L" DRtoWDdmx", L"DRtoWDdmx",ACRX_CMD_MODAL,&DRtoWDdmx);//工具条

	//建立数模
	acedRegCmds->addCommand(L"PENGLIHUI", L" TQSJ", L"TQSJ",ACRX_CMD_MODAL,&tiqushuju);//提取数据
	acedRegCmds->addCommand(L"PENGLIHUI", L" CLSJ", L"CLSJ",ACRX_CMD_MODAL,&chulishuju);//处理数据
	acedRegCmds->addCommand(L"PENGLIHUI", L" JLSM", L"JLSM",ACRX_CMD_MODAL,&jianlishumo);//提取数据
	acedRegCmds->addCommand(L"PENGLIHUI", L" XSSM", L"XSSM",ACRX_CMD_MODAL,&xianshishumo);//处理数据
	acedRegCmds->addCommand(L"PENGLIHUI", L" TCONA", L"TCONA", ACRX_CMD_MODAL, &TINConvertASC);//处理数据
	acedRegCmds->addCommand(L"PENGLIHUI", L" QHSM", L"QHSM", ACRX_CMD_MODAL, &Changeshumo);//提取数据

	//横断面参数
	acedRegCmds->addCommand(L"PENGLIHUI", L" HDMCS", L"HDMCS",ACRX_CMD_MODAL,&hengduanmiancanshu);//显示数模

	//车速检测
	acedRegCmds->addCommand(L"PENGLIHUI", L" JSCS", L"JSCS",ACRX_CMD_MODAL,&jisuanchesu);//显示数模
	acedRegCmds->addCommand(L"PENGLIHUI", L" CSCT", L"CSCT",ACRX_CMD_MODAL,&chesuchutu);//建立数模
	acedRegCmds->addCommand(L"PENGLIHUI", L" CSCB", L"CSCB",ACRX_CMD_MODAL,&chesuchubiao);//显示数模

	//实时交互漫游
	acedRegCmds->addCommand(L"PENGLIHUI", L" SSJHMY", L"SSJHMY",ACRX_CMD_MODAL,&shishijiaohumanyou);//显示数模

	//帮助

	// TODO: add your initialization functions


	

	BAS_DRAW_FUN::CreateLayer(L"公路平面设计专用层");
	/*acedGetAcadFrame()->SetWindowText (L"公路定线"); 
	acedGetAcadFrame()->UpdateWindow (); */

//#ifdef LOADMENU
//		/*penglihuiPMENUBAR();*/
//#endif
	





	srand((unsigned)time(NULL));
	PassWord1 = rand() + 998888;
	PassWord2 = rand() + 998888;

	ACHAR filename[256];
	_stprintf(filename, L"%s\\lib\\acad.dat", SysPath);
	FILE* fpw = NULL;
	if ((fpw = _wfopen(filename, L"wb")) != NULL)
	{
//#ifdef x64
		{
			fwrite(&PassWord1, sizeof(int), 1, fpw);
			fwrite(&PassWord2, sizeof(int), 1, fpw);
		}
//#else 
//		{
//			fwrite(&PassWord1, sizeof(long), 1, fpw);
//			fwrite(&PassWord2, sizeof(long), 1, fpw);
//		}
//#endif
		fclose(fpw);
	}
	//------------设置内存文件密码---------------------
//#ifdef x64
	MEMmapfileHandleP = CreateFileMapping((HANDLE)0xFFFFFFFFFFFFFFFF, NULL, PAGE_READWRITE, 0, 2048, L"MemMapP");
//#else 
//	MEMmapfileHandleP = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, L"MemMapP");
//#endif
	FilePtrP = MapViewOfFile(MEMmapfileHandleP, FILE_MAP_WRITE, 0, 0, 0);
	ACHAR tempstr[256];
	_stprintf(tempstr, L"%ld", PassWord2);
	_tcscpy((ACHAR *)FilePtrP, tempstr);


	acutPrintf(L"输入命令: \"PMENUBAR\"调出系统菜单!\n");




	JD_CENTER::rxInit();

	EXMANAGER::rxInit();

	DMX_ROAD::rxInit();////易漏掉

	GTZDM_ROAD::rxInit();
	BRI_ROAD::rxInit();
	TUN_ROAD::rxInit();
	DMX_LCB_ROAD::rxInit();

	DMX_PQX_ROAD::rxInit();////易漏掉
	GTZDK_ROAD::rxInit();
	HdmText_ROAD::rxInit();
	DesignLine_ROAD::rxInit();
	EarthLine_ROAD::rxInit();
	LTGOU_ROAD::rxInit();
	JSGOU_ROAD::rxInit();
	DeshBz_ROAD::rxInit();
	EarhBz_ROAD::rxInit();
	TRIM_ROAD::rxInit();
	GTZX_ROAD::rxInit();
	GTZX2_ROAD::rxInit();
	GTZX_JYX_ROAD::rxInit();
	GTZX2_JYX_ROAD::rxInit();

	acrxBuildClassHierarchy();	
	ads_regapp(AppName);

	//	darcysldesignsldesign();
	//	darcypddesignpddesign();
}

// Unload this application. Unregister all objects
// registered in InitApplication.

void UnloadApplication()
{
	// NOTE: DO NOT edit the following lines.


	AcDbEntity *pEnt;  
	int length;
	if (acdbHostApplicationServices()->workingDatabase())
	{
		// Remove all the jReactors added by this program
		length = g_reactorList.length();
		for (int i = 0; i < length; i++)
		{
			int rc;
			rc = acdbOpenObject(pEnt,
				((ReactorEntityPair*)g_reactorList[i])->entityId,
				AcDb::kForWrite, Adesk::kTrue);
			if(rc != Acad::eOk)
				continue;
			pEnt->removeReactor(
				((ReactorEntityPair*)g_reactorList[i])->pEntReactor);
			pEnt->close();
			delete ((ReactorEntityPair*)g_reactorList[i])->pEntReactor;
		}
	}

	//{{AFX_ARX_EXIT
	acedRegCmds->removeGroup(L"PENGLIHUI");
	acedRegCmds->removeGroup(L"RAILDESIGN");
	acedRegCmds->removeGroup(L"PENGLIHUIHUHU_CC_");
	//}}AFX_ARX_EXIT

	// TODO: clean up your application


	deleteAcRxClass(JD_CENTER::desc());
	deleteAcRxClass(EXMANAGER::desc());
	deleteAcRxClass(DMX_ROAD::desc());
	deleteAcRxClass(GTZDM_ROAD::desc());
	deleteAcRxClass(BRI_ROAD::desc());
	deleteAcRxClass(TUN_ROAD::desc());
	deleteAcRxClass(DMX_LCB_ROAD::desc());
	deleteAcRxClass(DMX_PQX_ROAD::desc());
	deleteAcRxClass(GTZDK_ROAD::desc());
	deleteAcRxClass(DeshBz_ROAD::desc());
	deleteAcRxClass(EarhBz_ROAD::desc());
	deleteAcRxClass(TRIM_ROAD::desc());
	deleteAcRxClass(GTZX_ROAD::desc());
	deleteAcRxClass(GTZX_JYX_ROAD::desc());
	deleteAcRxClass(GTZX2_JYX_ROAD::desc());
	deleteAcRxClass(GTZX2_ROAD::desc());
	if(InputJdCenter)delete InputJdCenter;
	if(DockLocationToolDlg)delete DockLocationToolDlg;
	if(PMenuBar)delete PMenuBar;

}

// This functions registers an ARX command.
// It can be used to read the localized command name
// from a string table stored in the resources.



void AddCommand(const ACHAR* cmdGroup, const ACHAR* cmdInt, const ACHAR* cmdLoc,
				const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal)
{

	ACHAR cmdLocRes[65];

	// If idLocal is not -1, it's treated as an ID for
	// a string stored in the resources.
	if (idLocal != -1) {

		// Load strings from the string table and register the command.
		::LoadString(_hdllInstance, idLocal, cmdLocRes, 64);
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLocRes, cmdFlags, cmdProc);

	} else
		// idLocal is -1, so the 'hard coded'
		// localized function name is used.
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLoc, cmdFlags, cmdProc);
}


//编辑坡度
void MODIFYDATA_ZDM()
{
	// TODO: Implement the command
	ads_name en;
	AcGePoint3d PT;
	AcDbObjectId eId;
	AcDbObject *pObj;
	XLDataBase DBS;
	int rc = ads_entsel(L"\n请选择坡度线: ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;

	pzLinep = GlobalZDMTK.GetPM(PT);
	int izdm = GlobalZDMTK.FindWichZdm(PT);
	if(!pzLinep)
	{
		ads_printf(L"请先选择平面方案!\n");
		return;
	}
	if(izdm<0)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
	}
	if (rc!=RTNORM || !pObj->isKindOf(GTZDM_ROAD::desc()))
	{             
		ads_printf(L"\n所选实体非坡度线实体,将为您新建坡度线实体!");

		int NPD=1;
		double PDarray[MAXBPDNUM][4];
		ACHAR GH[8];
		_tcscpy(GH, L"K");
		int i;
		for(i=0; i<MAXBPDNUM; i++)
			for(int j=0; j<4; j++)
				PDarray[i][j] = 0.0;
		PDarray[0][0] = pzLinep->DLArray[0].ELC;
		GTZDM_ROAD *zm;
		zm = new GTZDM_ROAD(NPD,PDarray,&GH[0],0,3);
		//断链表		

		BAS_DRAW_FUN basdraw;
		ZpId = basdraw.AddEntityToDbs(zm);//全局
		eId = ZpId;		
		GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].zdm =zm;
	}
	CAcModuleResourceOverride myResources;
	MODIFY_GTZDM *pMODIFY = NULL;
	pMODIFY = new MODIFY_GTZDM();
	pMODIFY->eId=eId;
	pMODIFY->Create(IDD_DIALOG_MODIFY_GTZDM,NULL);
	pMODIFY->ShowWindow(SW_SHOW);
	GlobalZDMTK.ZDMTK[izdm].zdm = pMODIFY->pzdmLine;
	/*if(pMODIFY.DoModal()==IDOK)
	GlobalZDMTK.ZDMTK[izdm].zdm = pMODIFY.pzdmLine;*/

	// 	acDocManager->sendStringToExecute(ZDMDOC, L"regen\n",false,true);	
}







//bool CheckPassword()
//{
//	ACHAR fname[1024];
//	FILE *fpr;
//	long PassWord1,PassWord2;
//	//	GetPrivateProfileString("RLDVS", L"RLDVS",NULL,SysPath,256, L"win.ini");
//	_tcscpy(fname,acedGetAppName());
//	CString fstr;
//	fstr = fname;
//	fstr.MakeUpper();
//	fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
//	_tcscpy(SysPath,fstr);
//
//	resbuf *pNewRb=acutBuildList(RTSTR,SysPath,0);  //不提示加载arx
//	acedSetVar(L"TRUSTEDPATHS",pNewRb);
//	acutRelRb(pNewRb);
//
//	_stprintf(fname, L"%s\\lib\\acad.dat",SysPath);
//	if((fpr = _wfopen(fname, L"rb")) == NULL)
//	{
//		ads_alert(L"请从主界面运行本程序!");
//		return false;
//	}
//	fread(&PassWord1,sizeof(long),1,fpr);
//	fread(&PassWord1,sizeof(long),1,fpr);
//	fclose(fpr);
//	HANDLE MEMmapfileHandleP;//mapfileHandleP,
//	MEMmapfileHandleP = OpenFileMapping(FILE_MAP_READ,FALSE, L"MemMapP");
//	LPVOID FilePtr = MapViewOfFile(MEMmapfileHandleP,FILE_MAP_READ,0,0,0);
//	if(FilePtr)
//	{
//		PassWord2 =  _wtol((ACHAR *)FilePtr);
//	}
//	else
//	{
//		ads_alert(L"请从RLDVS主界面运行本程序!");
//		return false;
//	}
//
//	if(PassWord1 != PassWord2)
//	{
//		ads_alert(L"请从主界面运行本程序!");
//		return false;
//	}
//	return true;
//}


