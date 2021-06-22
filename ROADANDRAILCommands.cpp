//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "JdCenterInputDlg.h"
#include "BAS_DRAW_FUN.h"
#include "ArcGQ.h"
ads_point ZOOM_P1,ZOOM_P2,ZOOM_P3,ZOOM_P4;
AcDbCircle* pCircle = NULL;//平面交点追踪
AcDbCircle* pCircle1 = NULL;//用地界桩号点追踪
AcDbCircle* pCircle2 = NULL;//用地界桩号点追踪
extern JdCenterInputDlg *InputJdCenter;
#include "DockLocationTool.h"
extern DockLocationTool *DockLocationToolDlg;
extern DockLocationTool *PMenuBar;
#include "JDLuanLink.h"
#include "XLDataBase.h"
#include "CWorkDir.h"
#include "resource.h"
#include "EXMANAGER.h"
#include "BCCreateXLEntity.h"
// This is command 'INPUTJDCENTER'
#include "TrimTuSettingDialog.h"
#include "InRoadNameDlg.h"
#include "PMDIALOG.h"
#include "DMX_LCB.h"
#include "DMX_PQX.h"
#include "ReWriteFILE.h"
#include "GTZDMReactor.h"
#include "DMXReactor.h"
#include "ZdmOriDataDlg.h"
#include "BRIDlg.h"
#include "TUNDlg1.h"
#include "SetRoadName.h"
#include "PickDmxDockBar.h"
#include "EditDMXDlg.h"
#include "LMSheetDlg.h"
#include "PPCDSheet.h"
#include "hooseLMX.h"
#include "SetCGBZDlg.h"
#include "SetCGJSCS.h"
#include "calcg.h"
#include "SuphFDSheet.h"
#include "DrawPmDlg.h"
#include "DrawPM.h"
#include "DrawZDMDlg.h"
#include "StaDataSheet.h"
#include "SetDrawQXYS.h"
#include "ZDMHT.h"
#include "CWorkDir.h"
#include "ZDMHT1.h"
#include "GTZDK.h"
#include "LCZZBDlg.h"
#include "CheckToolBar.h"
#include "StaDataSheet.h"
#include "HdmDmx.h"
#include "HdmDes.h"
#include "HDMTdlg.h"
#include "OutTAB.h"	
#include "PMGUIDE.h"
#include "CheckToolBar.h"
#include "TRIMPMT.h"
#include "PmPointer.h"
#include   <direct.h> 
extern PMDIALOG* PmDlg;
PickDmxDockBar *PickDmxBar=NULL;
CCheckToolBar *ToolBardlg = NULL;
//纵断面
#include "DMDHQFS.h"
#include "ZMZDPMFA.h"
#include "editbp.h"
#include "InputZDM.h"
#include "GouPDsheet.h"
#include "DrawJWGRID.h"
#include "DRAWXYTZD.h"
#include "ZBTOLC.h"
#include "DUANBUHX.h"
#include "CXLMBG.h"
#include "CalYDArea.h"
#include "NCDMXCS.h"
#include "DeshBz.h"
#include "EarhBz.h"
#include "DrawRoadsDlg.h"
//#include "EditAllRoads.h"
#include "TRIM.h"
#include "TRIMDIAG.h"
#include "TRIMDIALOG.h"
//#include "RailPmPoint.h"
#include "DirdlgDlg.h"
#include "ZDMStandardDlg.h"
#include "SetJKNorm.h"
#include "SetCGBZDlg.h"
#include "EditZDKDlg.h"
#include "EditAllRoads.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016
#include "RoadDesignWorkingDatabase.h"
#include "RoadAutoDesign.h"
#include "RoadAutoDesignDlg.h"
#include "CAutoDrawGutterDlg.h"
#include "COpDrawingSheet.h"
#include "CSetLevelCtrlDlg.h"
#include "CGutInteractDlg.h"
#include "MyBar.h"
#include "DataTrans.h"
#include "DMXDataTrans.h"
#include "DrawGCSLBandDYT.h"
#include "DrawDYTdlg.h"
#include "CJPInfor.h"
#include "COutpRoadCollTabDlg.h"
#include "CCrossEarthInputDlg.h"
#include "SelTINDlg.h"
#include "Draw3D.h"
#include "DLInintLwDlg.h"
int isCT = 0;//1表示只自动出图，不设计 0表示先自动设计，后出图

struct egpt_node{
	struct egpt_node *prior;
	ads_point pt;
	struct egpt_node *next;
};
extern AcDbVoidPtrArray DesLineInfor;//设计线实体信息，ID
BOOL pzld;//平纵联动
AcDbObjectId ZxId;
AcDbVoidPtrArray g_reactorList;

RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库
vector<AcDbObjectId> XianLuArray_ZDCTZY;//线路数组，自动出图专用
CString DaoRuPingMianFangAnMdbMingCheng;//导入平面方案对应的数据库名称，自动出图专用

//////////////////////////////////////////////////////////////////

const double START_TIME = 201810130000.0;
const double END_TIME = 202206200000.0;


#ifdef DIGIROAD
//	END_TIME=210008150000.0
#endif

#define CHECK_TIME
//#define CHECK_DOG

bool CheckByTime();
bool CheckDogByDLL();


////////////////////////////////////////////////////////////////////

AcDbObjectId  ZDKID[100];
int IDsum=0;
GTZDK_ROAD ZDK;
int Ribbon = 0;
#include "JMXYDLG.h"
CJMXYDLG *XyDlg;
void LanXun(ads_point p1, ads_point p2,ads_point p3, ads_point p4);

void NewMdfRoad()//新建改移道路	新建道路
{
	// TODO: Add your command handler code here
	//ads_alert(L"当前软件是试用版本，请购买完整软件继续使用此功能！");
#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
	if(!CheckByTime())
		return;
#endif
	//return;
	CDirdlgDlg  dlg;
	dlg.DirMode = 0;
	dlg.DoModal();
}

void SelMdfRoad()//选择改移道路	选择道路
{
	// TODO: Add your command handler code here	
#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
	if(!CheckByTime())
		return;
#endif
	CDirdlgDlg  dlg;
	dlg.DirMode=1;
	dlg.DoModal();
}


//列表编辑（平面）
void penglihuiinputjdcenter()//录入平面数据
{
	//设置参数
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;

	//如果用户空选或者按esc
	if (RTNORM != acedEntSel(L"\n请选择线路:", en, pt))
	{
		if (PmDlg)
		{
			delete PmDlg;
			PmDlg = NULL;
		}

		PmDlg = new PMDIALOG();

		PmDlg->Create(IDD_DIALOG_PM,acedGetAcadFrame());
	}
	else	//正常选择了线路实体
	{
		acdbGetObjectId(eId,en);	
		if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
			return;
		pEnt->close();
		if(pEnt->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER *pline = JD_CENTER::cast(pEnt);
			if(pline->JdCenNum>1)
			{
				if (PmDlg)
				{
					delete PmDlg;
					PmDlg = NULL;
				}
				PmDlg = new PMDIALOG();
				PmDlg->JdCenObj = JD_CENTER::cast(pEnt);
				PmDlg->JdCenId = eId;
				PmDlg->Create(IDD_DIALOG_PM, acedGetAcadFrame());
			}
		}
		else
			ads_printf(L"选择的非线路实体!\n");
	}	
}

//修改线路实体

//查询实体数据
void penglihuiEDITJDCEN()
{	
	/*
	if(InputJdCenter)
	{
	delete InputJdCenter;
	InputJdCenter=NULL;
	}
	*/	
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;
	CString fname="C:\\JD.txt";

	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();
	if(pEnt->isKindOf(JD_CENTER::desc()))
	{
		JD_CENTER *pline = JD_CENTER::cast(pEnt);
		if(pline->JdCenNum>1)
		{
			/*   if(PmDlg)
			{
			delete PmDlg;
			PmDlg=NULL;
			}
			PmDlg=new PMDIALOG;*/
			PmDlg->JdCenObj=JD_CENTER::cast(pEnt);
			PmDlg->JdCenId = eId;
			PmDlg->Create(IDD_DIALOG_PM, acedGetAcadFrame());
		}
		else
		{
			if(XyDlg)
			{
				delete XyDlg;
				XyDlg=NULL;
			}
			XyDlg=new CJMXYDLG(acedGetAcadFrame(),eId);
			XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
			XyDlg->ShowWindow(SW_SHOW);
		}
		/*
		AfxMessageBox(L"查询一线实体!");
		InputJdCenter=new JdCenterInputDlg;
		InputJdCenter->JdCenObj=JD_CENTER::cast(pEnt);
		InputJdCenter->JdCenId=eId;
		InputJdCenter->Create(IDD_DIALOG_JDCENTERINPUT,acedGetAcadFrame());	
		*/
	}
	else
	{
		if(XyDlg)
		{
			delete XyDlg;
			XyDlg=NULL;
		}
		XyDlg=new CJMXYDLG(acedGetAcadFrame(),eId);
		XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
		XyDlg->ShowWindow(SW_SHOW);
	}
}

void penglihuiinputxy()//录入线元数据
{
	/*
	if(XyDlg)
	{
	delete XyDlg;
	XyDlg=NULL;
	}
	XyDlg=new CJMXYDLG(acedGetAcadFrame(),0);
	XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
	XyDlg->ShowWindow(SW_SHOW);*/
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;
	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt))
	{
		if(XyDlg)
		{
			delete XyDlg;
			XyDlg=NULL;
		}
		XyDlg=new CJMXYDLG(acedGetAcadFrame(),0);
		XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
		XyDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		acdbGetObjectId(eId,en);
		if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
			return;
		pEnt->close();
		if(pEnt->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER *pline = JD_CENTER::cast(pEnt);

			if(XyDlg)
			{
				delete XyDlg;
				XyDlg=NULL;
			}
			XyDlg=new CJMXYDLG(acedGetAcadFrame(),eId);
			XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
			XyDlg->ShowWindow(SW_SHOW);	
		}
		else
		{
			if(XyDlg)
			{
				delete XyDlg;
				XyDlg=NULL;
			}
			XyDlg=new CJMXYDLG(acedGetAcadFrame(),eId);
			XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
			XyDlg->ShowWindow(SW_SHOW);
		}
	}

}
// This is command 'PZOOM'
void penglihuiPZOOM()
{
	// TODO: Implement the command
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2010
	
}

// This is command 'DRAWQXYS_Road'	绘曲线要素
void penglihuiDRAWQXYS()
{
	// TODO: Implement the command
	SetDrawQXYS qxdlg;
	qxdlg.DoModal();
}
//This is command 绘线元要素
void DRAWXYYSB()
{
	DRAWXYTZD  xydlg;
	xydlg.DoModal();
}
// This is command 'ZYLINK'	直圆连接？
void penglihuiZYLINK()	
{
	// TODO: Implement the command
	ads_name en,en1;
	ads_point pt,pt1;
	AcDbObjectId eId,eId1;
	AcDbEntity * pEnt;
	AcDbEntity * pEnt1;
	if(RTNORM!=acedEntSel(L"选择直线:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);
	/*acdbOpenObject(pEnt,eId,AcDb::kForRead);*/
	if(acdbOpenObject(pEnt, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pEnt->close();
	if(!pEnt->isKindOf(AcDbLine::desc())){acutPrintf(L"所选实体不是直线!\n");return;}	
	if(RTNORM!=acedEntSel(L"\n选择圆或圆弧:",en1,pt1)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId1,en1);
	/*acdbOpenObject(pEnt1,eId1,AcDb::kForRead);*/
	if(acdbOpenObject(pEnt1, eId1, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pEnt1->close();	

	AcDbLine *Line=NULL;
	AcDbCircle *Cir=NULL;
	AcDbArc *Arc=NULL;
	double N1,E1,N2,E2;
	double CenN,CenE,R;
	int LRFlag;
	if(pEnt->isKindOf(AcDbLine::desc()))
	{
		pEnt->close();
		Line=AcDbLine::cast(pEnt);
		N1=Line->startPoint().y;
		E1=Line->startPoint().x;
		N2=Line->endPoint().y;
		E2=Line->endPoint().x;

		if(pEnt1->isKindOf(AcDbCircle::desc()))
		{
			/*acdbOpenObject(Cir,eId1,AcDb::kForRead);*/
			if(acdbOpenObject(Cir, eId1, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			Cir->close();

			CenN=Cir->center().y;
			CenE=Cir->center().x;
			R=Cir->radius();
			ACHAR kw[8];
			acedInitGet(RSG_NONULL,L"F S f s");
			acedGetKword(L"\n请选择连接方向--(S)顺时针 / (F)返时针:",kw);
			if(acedGetInput(kw)==RTNORM)
			{
				if(_tcscmp(kw,L"F")==0||_tcscmp(kw,L"f")==0)LRFlag=-1;
				else if(_tcscmp(kw,L"S")==0||_tcscmp(kw,L"s")==0)LRFlag=1;
			}
			ZYLink ob(N1,E1,N2,E2,CenN,CenE,R,LRFlag);
			ob.pSPIRAL.DRAW();
		}
		/*
		else if(pEnt1->isKindOf(AcDbArc::desc()))
		{
		acdbOpenObject(Arc,eId1,AcDb::kForWrite);
		Arc->close();

		ZYLink ob(Line,Arc);
		}
		else acutPrintf(L"所选实体不是圆或圆弧！\n");*/

	}	
}

// This is command 'YYLLINK'
void penglihuiYYLLINK()
{
	// TODO: Implement the command
	ads_name en,en1;
	ads_point pt,pt1;
	AcDbObjectId eId,eId1;
	AcDbEntity * pEnt;
	AcDbEntity * pEnt1;
	AcGePoint3d Cent1,Cent2;
	AcDbCircle *Cir1,*Cir2;

	double CenN1,CenE1,CenN2,CenE2,R1,R2;
	int LRFlag;

	if(RTNORM!=acedEntSel(L"\n请选择圆/圆弧:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(RTNORM!=acedEntSel(L"\n请选择另一个圆/圆弧:",en1,pt1)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId1,en1);
	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	if (acdbOpenObject(pEnt1,eId1,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();		pEnt1->close();

	if(pEnt->isKindOf(AcDbCircle::desc())&&pEnt1->isKindOf(AcDbCircle::desc()))
	{
		ACHAR kw[8];
		acedInitGet(RSG_NONULL,L"F S f s");
		acedGetKword(L"\n请选择连接方向--(S)顺时针 / (F)返时针:",kw);
		if(acedGetInput(kw)==RTNORM)
		{
			if(_tcscmp(kw,L"F")==0||_tcscmp(kw,L"f")==0)LRFlag=-1;
			else if(_tcscmp(kw,L"S")==0||_tcscmp(kw,L"s")==0)LRFlag=1;
		}
		Cir1=AcDbCircle::cast(pEnt);
		Cir2=AcDbCircle::cast(pEnt1);
		CenN1=Cir1->center().y;
		CenE1=Cir1->center().x;
		R1=Cir1->radius();
		CenN2=Cir2->center().y;
		CenE2=Cir2->center().x;
		R2=Cir2->radius();
		YYLuanLink ob(CenN1,CenE1,R1,CenN2,CenE2,R2,LRFlag);
		ob.pSPIRAL.DRAW();
	}
}

// This is command 'YYCLINK'
void penglihuiYYCLINK()
{
	// TODO: Implement the command
	ads_name en,en1;
	ads_point pt,pt1;
	AcDbObjectId eId,eId1;
	AcDbEntity * pEnt;
	AcDbEntity * pEnt1;
	AcGePoint3d Cent1,Cent2;
	AcDbCircle *Cir1,*Cir2;

	double CenN1,CenE1,CenN2,CenE2,R1,R2;
	int LRFlag;

	if(RTNORM!=acedEntSel(L"\n请选择圆/圆弧:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(RTNORM!=acedEntSel(L"\n请选择另一个圆/圆弧:",en1,pt1)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId1,en1);
	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	if (acdbOpenObject(pEnt1,eId1,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();		pEnt1->close();

	if(pEnt->isKindOf(AcDbCircle::desc())&&pEnt1->isKindOf(AcDbCircle::desc()))
	{
		ACHAR kw[8];
		acedInitGet(RSG_NONULL,L"F S f s");
		acedGetKword(L"\n请选择连接方向--(S)顺时针 / (F)返时针:",kw);
		if(acedGetInput(kw)==RTNORM)
		{
			if(_tcscmp(kw,L"F")==0||_tcscmp(kw,L"f")==0)LRFlag=-1;
			else if(_tcscmp(kw,L"S")==0||_tcscmp(kw,L"s")==0)LRFlag=1;
		}
		Cir1=AcDbCircle::cast(pEnt);
		Cir2=AcDbCircle::cast(pEnt1);
		CenN1=Cir1->center().y;
		CenE1=Cir1->center().x;
		R1=Cir1->radius();
		CenN2=Cir2->center().y;
		CenE2=Cir2->center().x;
		R2=Cir2->radius();
		YYCLink ob(CenN1,CenE1,R1,CenN2,CenE2,R2,1,1,LRFlag);
		ob.pSPIRAL1.DRAW();
		ob.pSPIRAL2.DRAW();
	}
}

// This is command 'YYSLINK'
void penglihuiYYSLINK()
{
	// TODO: Implement the command
	ads_name en,en1;
	ads_point pt,pt1;
	AcDbObjectId eId,eId1;
	AcDbEntity * pEnt;
	AcDbEntity * pEnt1;
	AcGePoint3d Cent1,Cent2;
	AcDbCircle *Cir1,*Cir2;

	double CenN1,CenE1,CenN2,CenE2,R1,R2;
	int LRFlag;

	if(RTNORM!=acedEntSel(L"\n请选择圆/圆弧:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(RTNORM!=acedEntSel(L"\n请选择另一个圆/圆弧:",en1,pt1)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId1,en1);
	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	if (acdbOpenObject(pEnt1,eId1,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();		pEnt1->close();

	if(pEnt->isKindOf(AcDbCircle::desc())&&pEnt1->isKindOf(AcDbCircle::desc()))
	{
		ACHAR kw[8];
		acedInitGet(RSG_NONULL,L"F S f s");
		acedGetKword(L"\n请选择连接方向--(S)顺时针 / (F)返时针:",kw);
		if(acedGetInput(kw)==RTNORM)
		{
			if(_tcscmp(kw,L"F")==0||_tcscmp(kw,L"f")==0)LRFlag=-1;
			else if(_tcscmp(kw,L"S")==0||_tcscmp(kw,L"s")==0)LRFlag=1;
		}
		Cir1=AcDbCircle::cast(pEnt);
		Cir2=AcDbCircle::cast(pEnt1);
		CenN1=Cir1->center().y;
		CenE1=Cir1->center().x;
		R1=Cir1->radius();
		CenN2=Cir2->center().y;
		CenE2=Cir2->center().x;
		R2=Cir2->radius();

		/*
		YYQXLink YYQXObj;
		YYQXObj.SetCir1(CenN1,CenE1,R1);
		YYQXObj.SetCir2(CenN2,CenE2,R2);
		YYQXObj.SetA(0,0);

		YYQXObj.SetQXClass(C_F);
		YYQXObj.SetQXClass(C_S);
		YYQXObj.SetQXClass(S_F);
		YYQXObj.SetQXClass(S_S);

		YYQXObj.CalQX();

		AcDbLine *Line=new AcDbLine(AcGePoint3d(YYQXObj.GQE1,YYQXObj.GQN1,0.0),
		AcGePoint3d(YYQXObj.GQE2,YYQXObj.GQN2,0.0));
		BAS_DRAW_FUN::AddEntityToDbs(Line);
		*/


		YYSLink ob(CenN1,CenE1,R1,CenN2,CenE2,R2,0,80,LRFlag);
		ob.pSPIRAL1.DRAW();
		ob.pSPIRAL2.DRAW();
	}
}

bool IsFirstLocdlg1=true;

#include "DTMODEDLG.h"
DTMODEDLG *DTBXDlg=NULL;
// This is command 'LOCATIONTOOL'
void penglihuiLOCATIONTOOL()
{
	// TODO: Implement the command	
	/*
	if(!IsFirstLocdlg1)
	{
	Locdlg1.ShowWindow(TRUE);
	}
	else
	{
	IsFirstLocdlg1=false;
	Locdlg1.Create(IDD_DIALOG_LOCALION1,acedGetAcadFrame());
	Locdlg1.ShowWindow(SW_SHOW);
	}	*/

	ads_point PT,PT1,PT2;
	AcDbObjectId PId=0,HId=0,CId=0,IDarr[3];
	AcDbObject *pEnt1,*pEnt2;
	ads_name en;

	int rc;
	int Counter=0;
	rc = ads_entsel(L"\n选取起始控制单元(主线、直线、圆或圆弧)(不选) : ", en, PT);
	if(rc==RTCAN)
		return;
	if (rc == RTNORM) 
		acdbGetObjectId(PId, en);

	rc = ads_entsel(L"\n选取终止控制单元(主线、直线、圆或圆弧)(不选) : ", en, PT1);
	if(rc==RTCAN)
		return;
	if (rc == RTNORM) 
		acdbGetObjectId(HId, en);	

	rc = ads_entsel(L"\n选取连接单元(圆或圆弧): ", en, PT2);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
	{
		ads_printf(L"必须选择连接圆!\n");
		return;
	}
	if (rc == RTNORM) 
		acdbGetObjectId(CId, en);

	if(PId!=0)
	{
		IDarr[0]=PId;
		Counter++;
	}
	if(HId!=0)
	{
		IDarr[2]=HId;
		Counter++;
	}
	if(CId!=0)
	{
		IDarr[1]=CId;
		Counter++;
	}
	else
	{
		ads_printf(L"连接圆不能为空!\n");
		return;
	}
	if(Counter<2)
	{
		ads_printf(L"至少应有两个单元!\n");
		return;
	}

	if(Counter==2)
	{
		if(IDarr[0])
		{
			/*acdbOpenObject(pEnt1,IDarr[0],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt1, IDarr[0], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pEnt1->close();
		}
		else
		{
			/*acdbOpenObject(pEnt1,IDarr[2],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt1,IDarr[2], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pEnt1->close();
		}

		/*acdbOpenObject(pEnt2,IDarr[1],AcDb::kForRead);*/
		if(acdbOpenObject(pEnt2,IDarr[1], AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pEnt2->close();		
		if((pEnt1->isKindOf(AcDb2dPolyline::desc())
			|| pEnt1->isKindOf(AcDbPolyline::desc())
			|| pEnt1->isKindOf(AcDbLine::desc()))&&
			(pEnt2->isKindOf(AcDb2dPolyline::desc())
			|| pEnt2->isKindOf(AcDbPolyline::desc())
			|| pEnt2->isKindOf(AcDbLine::desc())))
		{
			ads_printf(L"所选两个线元均为直线,须指定连接线元!\n");
			return;
		}

	} 

	if(DTBXDlg)
	{
		delete DTBXDlg;
		DTBXDlg = NULL;
	}
	DTBXDlg =  new DTMODEDLG;
	//zl传入拾取点
	AcGePoint3d Point1, Point2, Point3;
	Point1.x = PT[X];
	Point1.y = PT[Y];
	Point1.z = 0.00;
	Point2.x = PT1[X];
	Point2.y = PT1[Y];
	Point2.z = 0.00;
	Point3.x = PT2[X];
	Point3.y = PT2[Y];
	Point3.z = 0.00;

	DTBXDlg->PT = Point1;//第一个线元
	DTBXDlg->PT1 = Point2;//第san 个线元
	DTBXDlg->PT2 = Point3;//第er 个线元
	//
	DTBXDlg->IDarr[0]=PId;//yi
	DTBXDlg->IDarr[1]=CId;//二
	DTBXDlg->IDarr[2]=HId;//san

	DTBXDlg->Create(IDD_DIALOG_DTMODE,acedGetAcadFrame());

	DTBXDlg->ShowWindow(SW_SHOW);

}
//#include "menubar\\MenuBar.h"
////CMenuBar *menubar=NULL;
//CMenuBarXP	*m_wndMenuBar;

// This is command 'PMENUBAR'
//#include "AcDialogBar.h"
//CAcDialogBar *pToolMenuBar=NULL;

void penglihuiPMENUBAR()
{
	// TODO: Implement the command

	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);

	if (DockLocationToolDlg == NULL)
	{
		DockLocationToolDlg =new DockLocationTool;
		DockLocationToolDlg->Create(acedGetAcadFrame(), L"");
		DockLocationToolDlg->SetBorders(0,0,0,0);
		DockLocationToolDlg->EnableDocking(CBRS_ALIGN_TOP);
		DockLocationToolDlg->AllowDockingMenuItemState();
		DockLocationToolDlg->RestoreControlBar();
		DockLocationToolDlg->SetAllowDockingMenuItemState(true);
	}
	else
		acedGetAcadFrame()->ShowControlBar(PMenuBar, TRUE, FALSE);	


	//if(PMenuBar!=NULL)
	//{
	//	delete PMenuBar;
	//	PMenuBar=NULL;
	//}
	//PMenuBar=new DockLocationTool;
	//PMenuBar->m_MODEL=ISMENU;
	//PMenuBar->Create(acedGetAcadFrame(),L"菜单");
	//PMenuBar->SetBorders(0,0,0,0);
	//PMenuBar->EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	//acedGetAcadFrame()->DockControlBar(PMenuBar,AFX_IDW_DOCKBAR_TOP,CRect(0,0,200,25));//,CRect(0,0,75,75)*/
}
//线路反向
void penglihuiJCREVERSE()	
{
	// TODO: Implement the command
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;


	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(acdbOpenObject(pEnt,eId,AcDb::kForRead)!=Acad::eOk)
		return;

	if(pEnt->isKindOf(JD_CENTER::desc()))
	{
		pEnt->close();	
		/*acdbOpenObject(pJC,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		JD_CENTER *pJC=JD_CENTER::cast(pEnt);
		pJC->Reverse();
		pJC->close();
	}
}

void penglihuiGQCIR()
{
	ArcGQ pGQ;
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;
	double N,E,Rmain;
	AcGePoint3d pk(0,0,0),temp;
	int type;
	struct resbuf result;
	if(RTNORM!=acedEntSel(L"\n请选择圆/弧:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();	
	if(pEnt->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pCir=AcDbCircle::cast(pEnt);
		/*acdbOpenObject(pCir,eId,AcDb::kForRead);*/
		if(acdbOpenObject(pCir,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		N=pCir->center().y;
		E=pCir->center().x;
		Rmain=pCir->radius();
		pCir->close();
		pGQ.SetMainArc(pCir);

		//////////////////////////////
		if(RTNORM!=acedEntSel(L"\n请选择圆/弧:",en,pt)){acutPrintf(L"选择失败!");return;}
		acdbGetObjectId(eId,en);

		if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
			return;
		pEnt->close();	
		if(pEnt->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle *pCiri=AcDbCircle::cast(pEnt);
			pGQ.AddGQArc(pCiri);
		}

		//////////////////////////////
		for(;;)
		{		
			if(ads_grread(1 , &type , &result)!=RTNORM)
			{ 
				break;
			}
			if(type==2)
			{
				short KEYCODE=result.resval.rint;
				//acutPrintf(L"%d",KEYCODE);
				if(KEYCODE==32)
				{
				}
				else if(KEYCODE==13)
				{
					break;
				}
				else {}
			}
			else if(type==25)
			{

			}
			else if(type == 3)break;
			else if(type == 5)
			{
				pk.x=result.resval.rpoint[X];
				pk.y=result.resval.rpoint[Y];
				pk.z=result.resval.rpoint[Z];
				/*acdbOpenObject(pCir,eId,AcDb::kForWrite);*/
				if(acdbOpenObject(pCir,eId,AcDb::kForWrite)!=Acad::eOk)	
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				temp=pCir->center();
				pCir->setCenter(pk);
				pCir->close();
				//pGQ.SetMainArc(pCir);
				if(pGQ.IsGQ())
				{
				}
				else
				{
					/*acdbOpenObject(pCir,eId,AcDb::kForWrite);*/
					if(acdbOpenObject(pCir,eId,AcDb::kForWrite)!=Acad::eOk)	
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					//pCir->setColorIndex(0);
					pCir->setCenter(temp);
					pCir->close();
				}
			}
		}
	}	
}// This is command 'TEST'

#include "SpiralY.h"
SpiralX SobX;
SpiralY SobY;
void Psin()
{
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	AcGePoint2dArray PtAy;
	double kk=0;
	double buchang=1.0;
	double A=1000;
	for(double a=0.1;a<3*A;a+=buchang)
	{
		AcGePoint2d Pt;
		Pt[X]=a;
		Pt[Y]=BAS_DRAW_FUN::spiral_x(A,a);
		PtAy.append(Pt);
	}		
	BAS_DRAW_FUN::makepolyline(PtAy,0,1.0);

	AcDbLine *Line1=new AcDbLine(AcGePoint3d(sqrt(PI)*A,0.0,0.0),AcGePoint3d(sqrt(PI)*A,5*A,0.0));
	BAS_DRAW_FUN::AddEntityToDbs(Line1);

	AcDbLine *Line2=new AcDbLine(AcGePoint3d(sqrt(2.0*PI)*A,0.0,0.0),AcGePoint3d(sqrt(2.0*PI)*A,5*A,0.0));
	BAS_DRAW_FUN::AddEntityToDbs(Line2);

	AcDbLine *Line3=new AcDbLine(AcGePoint3d(sqrt(4.0*PI)*A,0.0,0.0),AcGePoint3d(sqrt(4.0*PI)*A,5*A,0.0));
	BAS_DRAW_FUN::AddEntityToDbs(Line3);

	acDocManager->unlockDocument(acDocManager->curDocument());

}

void Pcos()
{
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"pline", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"pline", RTNONE);
#endif // VERSION_CAD2010

	for(double a=0;a<500;a+=2)
	{
		ads_point Pt;
		Pt[X]=SobX.SpiralGetX(100,a);
		Pt[Y]=SobY.SpiralGetY(100,a);

		//Pt[X]=a;
		//Pt[Y]=cos(a);
#ifdef VERSION_CAD2016
		acedCommandS(RTPOINT, Pt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTPOINT, Pt, RTNONE);
#endif // VERSION_CAD2010
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010
}
//测试命令	
void penglihuitest()
{
	//PmPointer::DrawRailPro();

	//CCrossEarthInputDlg* InputDlg = NULL;
	//InputDlg = new CCrossEarthInputDlg;
	//InputDlg->Create(IDD_DIALOG_CROSSEARTHINPUT);
	//InputDlg->ShowWindow(SW_SHOW);
	PmPointer CreatePM;
	CreatePM.DrawRailPro();
}

// This is command 'DRAWZDM'
CArray<CZDMHT, CZDMHT>DrawZdm;
int NZdmT=0;
void DRAWZDMForPZT(CString DbsName, double CorX, double CorY, double PmScale,double& XLen)//平纵断面图中绘纵断面
{
	XLen = 0.0;
	CString XZ;
	double m_Hscale = 2000.0 / PmScale;
	double m_Zscale = 200.0 / PmScale;
	double Scml,Ecml,xscale,yscale,xinori,yinori,cml;
	//ZDMHT zmhtdlg("纵面绘图");
	int i,drawmode;
	//if(zmhtdlg.DoModal()!=IDOK)
	//	return;

	CZDMHT zdmdraw;
	zdmdraw.NTKHt = 255.0 * PmScale;
	if(DrawZdm.GetSize()>0)
		DrawZdm.RemoveAll();

	NZdmT=0;
	int NZCG,NYCG,NZJK,NYJK,NZLMK,NYLMK,NBXDMX=0;
	CgSZ *pZcg=NULL,*pYcg=NULL,*pZjk=NULL,*pYjk=NULL;
	LMKdata *pZlmk=NULL,*pYlmk=NULL;
	XLDataBase *pdbs=NULL;
	pdbs = new  XLDataBase;

	//////////////////////////////////////////////////////////////////////////
	//创建平面指针

	JD_CENTER* pm = NULL;
	pdbs->Read_XLDbs(DbsName,L"控制点表",L"主线");		
	pdbs->Read_XLDbs(DbsName,L"断链表",L"主线");
	if(pdbs->NJdCen>0)
	{
		pm=new JD_CENTER();//主线对象
		pm->SetJdCenterArray(pdbs->JdCenArray,pdbs->NJdCen);
		if(pdbs->NDL>0)
			pm->setDLB(pdbs->DLArray,pdbs->NDL);
		_tcscpy(pm->mdbname,DbsName);
		_tcscpy(pm->RoadName,L"主线");
	}
	else
	{
		pdbs->Read_XLDbs(DbsName,L"线元表",L"主线");	
		if(pdbs->XYNum>0)
		{
			pm=new JD_CENTER(pdbs->XYArray,pdbs->XYNum);//主线对象
			if(pdbs->NDL>0)
				pm->setDLB(pdbs->DLArray,pdbs->NDL);
			_tcscpy(pm->mdbname,DbsName);
			_tcscpy(pm->RoadName,L"主线");
		}
		else
		{
			AfxMessageBox(DbsName + "无法创建平面!");
			return;
		}
	}
	if (pzLinep)
	{
		delete pzLinep;
		pzLinep = NULL;
	}
	pzLinep = pm;
	XLen = pzLinep->LenOfLine;
	//////////////////////////////////////////////////////////////////////////

	pdbs->Read_XLDbs(pm->mdbname,L"纵断链表",pm->RoadName);
	pdbs->Read_XLDbs(pm->mdbname,L"坡度表",pm->RoadName,L"全幅");
	BAS_DRAW_FUN ob;
	if(pdbs)
	{
		for(i=0; i<pdbs->NPD; i++)
			pdbs->PDB[i].cml = ob.TYLC1(pdbs->PDB[i].ml,pdbs->NZDL,pdbs->ZDLArray);
	}

	pdbs->Read_XLDbs(pm->mdbname,L"大中桥",pm->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pm->mdbname,L"隧道",pm->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pm->mdbname,L"纵地面线表",pm->RoadName);
	pdbs->Read_XLDbs(pm->mdbname,L"小桥涵",pm->RoadName);
	pdbs->Read_XLDbs(pm->mdbname,L"左超高分段表",pm->RoadName);

	NZCG = pdbs->NCG;
	pZcg = new CgSZ[NZCG];
	for(i=0;i<NZCG;i++)
	{
		pZcg[i]=pdbs->CG[i];
		pZcg[i].cml = pm->TYLC(pZcg[i].xlc);
	}
	pdbs->Read_XLDbs(pm->mdbname,L"右超高分段表",pm->RoadName);
	NYCG = pdbs->NCG;
	pYcg = new CgSZ[NYCG];
	for(i=0;i<NYCG;i++)
	{
		pYcg[i]=pdbs->CG[i];
		pYcg[i].cml = pm->TYLC(pYcg[i].xlc);
	}

	pdbs->Read_XLDbs(pm->mdbname,L"左加宽分段表",pm->RoadName);
	NZJK = pdbs->NCG;
	pZjk = new CgSZ[NZJK];
	for(i=0;i<NZJK;i++)
	{
		pZjk[i]=pdbs->CG[i];
		pZjk[i].cml = pm->TYLC(pZjk[i].xlc);
	}
	pdbs->Read_XLDbs(pm->mdbname,L"右加宽分段表",pm->RoadName);
	NYJK = pdbs->NCG;
	pYjk = new CgSZ[NYJK];
	for(i=0;i<NYJK;i++)
	{
		pYjk[i]=pdbs->CG[i];
		pYjk[i].cml = pm->TYLC(pYjk[i].xlc);
	}

	pdbs->Read_XLDbs(pm->mdbname,L"左路面宽分段表",pm->RoadName);
	NZLMK = pdbs->NLMK;
	pZlmk = new LMKdata[NZLMK];
	for(i=0;i<NZLMK;i++)
	{
		pZlmk[i]=pdbs->lmk[i];
		pZlmk[i].cml = pm->TYLC(pZlmk[i].xlc);
	}
	pdbs->Read_XLDbs(pm->mdbname,L"右路面宽分段表",pm->RoadName);
	NYLMK = pdbs->NLMK;
	pYlmk = new LMKdata[NYLMK];
	for(i=0;i<NYLMK;i++)
	{
		pYlmk[i]=pdbs->lmk[i];
		pYlmk[i].cml = pm->TYLC(pYlmk[i].xlc);
	}

	CString TKName;//图框名
	CString DZGKName;//地质概况图名
	xscale = m_Hscale/1000;
	yscale = m_Zscale/1000;
	xinori = CorX;
	yinori = CorY;
	drawmode = 1;//施工图
	Scml = pm->DLArray[0].ELC;
	Ecml = pm->XLLength;
	pm->FormLCB(100,Scml,Ecml);
	i=0;
	int CurTno,j;
	CurTno = 0;

	while(Scml < Ecml-0.01)
	{
		zdmdraw.m_pm = pm;
		zdmdraw.CurTNo =CurTno+i;
		zdmdraw.m_TKName = TKName;
		zdmdraw.m_dzgkname = DZGKName;
		zdmdraw.m_bzym = FALSE;
		zdmdraw.pDBS = pdbs;
		zdmdraw.pZDBS = NULL;
		zdmdraw.pYDBS = NULL;
		zdmdraw.NZCG = NZCG;
		zdmdraw.NYCG = NYCG;
		zdmdraw.ZCG = pZcg;
		zdmdraw.YCG = pYcg;
		zdmdraw.NZJK = NZJK;
		zdmdraw.NYJK = NYJK;
		zdmdraw.ZJK = pZjk;
		zdmdraw.YJK = pYjk;
		zdmdraw.NZLMK = NZLMK;
		zdmdraw.NYLMK = NYLMK;
		zdmdraw.ZLMK = pZlmk;
		zdmdraw.YLMK = pYlmk;
		zdmdraw.X_scale = xscale;
		zdmdraw.Y_scale = yscale;
		zdmdraw.X_inori = xinori+i*500;
		zdmdraw.Y_inori = yinori;
		zdmdraw.DrawMode = drawmode; 
		zdmdraw.infwgx = TRUE;
		zdmdraw.m_Scml = Scml;
		zdmdraw.NBxDmx = NBXDMX;
		if(drawmode)//施工
		{
			if(i==0)//第一幅
			{
				cml = Ecml;
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = true;
			}
			else
			{
				//  cml = Scml+xscale*370;
				cml = Scml+xscale*350;
				if(cml >Ecml)
					cml = Ecml;
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = false;
			}
		}

		zdmdraw.WXPnum=9;
		zdmdraw.WayorRamp = 0;//公路
		zdmdraw.InfLmPt[0]= false;
		zdmdraw.InfLmPt[1]= false;
		zdmdraw.InfLmPt[2]= false;
		zdmdraw.InfLmPt[3]= false;
		zdmdraw.InfLmPt[4]= false;
		zdmdraw.InfLmPt[5]= false;
		zdmdraw.InfLmPt[6]= false;
		zdmdraw.InfLmPt[7]= false;
		zdmdraw.InfLmPt[8]= false;
		zdmdraw.LmPtName[0] = "P1";
		zdmdraw.LmPtName[1] = "P2";
		zdmdraw.LmPtName[2] = "P3";
		zdmdraw.LmPtName[3] = "P4";
		zdmdraw.LmPtName[4] = "P5";
		zdmdraw.LmPtName[5] = "P6";
		zdmdraw.LmPtName[6] = "P7";
		zdmdraw.LmPtName[7] = "P8";


		zdmdraw.wxp[8].wxpinf = 8;//竖曲线
		zdmdraw.wxp[8].Lht = 10.0 * PmScale;
		zdmdraw.wxp[8].texth = 2.5 * PmScale;

		zdmdraw.wxp[7].wxpinf = 0;//地质概况
		zdmdraw.wxp[7].Lht = 10.0 * PmScale;
		zdmdraw.wxp[7].texth = 2.5 * PmScale;

		zdmdraw.wxp[6].wxpinf = 7;//填挖高
		zdmdraw.wxp[6].Lht = 10.0 * PmScale;
		zdmdraw.wxp[6].texth = 2.5 * PmScale;

		zdmdraw.wxp[5].wxpinf = 1;//设计标高
		zdmdraw.wxp[5].Lht = 10.0 * PmScale;
		zdmdraw.wxp[5].texth = 2.5 * PmScale;

		zdmdraw.wxp[4].wxpinf = 3;//坡度、坡长
		zdmdraw.wxp[4].Lht = 10.0 * PmScale;
		zdmdraw.wxp[4].texth = 2.25 * PmScale;

		zdmdraw.wxp[3].wxpinf = 2;//地面高程
		zdmdraw.wxp[3].Lht = 10.0 * PmScale;
		zdmdraw.wxp[3].texth = 2.5 * PmScale;

		zdmdraw.wxp[2].wxpinf = 4;//里程桩号
		zdmdraw.wxp[2].Lht = 15.0 * PmScale;
		zdmdraw.wxp[2].texth = 2.5 * PmScale;

		zdmdraw.wxp[1].wxpinf = 5;//直线及平曲线
		zdmdraw.wxp[1].Lht = 15.0 * PmScale;
		zdmdraw.wxp[1].texth = 2.5 * PmScale;

		zdmdraw.wxp[0].wxpinf = 6;//超高
		zdmdraw.wxp[0].Lht = 20.0 * PmScale;
		zdmdraw.wxp[0].texth = 2.5 * PmScale;

		zdmdraw.DrawZDM();

		Scml = zdmdraw.m_Ecml;
		DrawZdm.Add(zdmdraw);
		i++;
	}

	NZdmT = i;

	if (pZlmk) delete []pZlmk; pZlmk = NULL;
	if (pYlmk) delete []pYlmk; pYlmk = NULL;
	if(pZcg) delete []pZcg; pZcg = NULL;
	if(pYcg) delete []pYcg; pYcg = NULL;
	if (pZjk) delete []pZjk; pZjk = NULL;
	if (pYjk) delete []pYjk; pYjk = NULL;
	//if (pdbs) delete pdbs; pdbs = NULL;
}

//绘纵断面	绘纵面图
void DRAWZDM()
{
	// TODO: Implement the command 
	CString XZ;
	double Scml, Ecml, xscale, yscale, xinori, yinori, cml;
	ZDMHT zmhtdlg(L"纵面绘图");
	int i,drawmode;
	if (zmhtdlg.DoModal() != IDOK)
		return;

	CZDMHT zdmdraw;
	if (DrawZdm.GetSize() > 0)
		DrawZdm.RemoveAll();

	double Left = 0.0, Bottom = 0.0, Right = 0.0, Top = 0.0;	//20190718添加	用以纵断面出图后自动缩放
	NZdmT = 0;
	int NZCG, NYCG, NZJK, NYJK, NZLMK, NYLMK, NBXDMX = 0;
	CgSZ *pZcg = NULL, *pYcg = NULL, *pZjk = NULL, *pYjk = NULL;
	LMKdata *pZlmk = NULL, *pYlmk = NULL;
	XLDataBase *pdbs = NULL, *pzdbs = NULL, *pydbs = NULL;
	XLDataBase *pBxDBS[10];//最多10个边线地面线
	pdbs = new  XLDataBase;
	pzLinep = NULL;
	pzLinep = zmhtdlg.zinidlg.pm;
	if (pzLinep == NULL)
	{
		return;
	}

	if(zmhtdlg.zinidlg.zdmbx.m_drawbx)//绘制边线地面线
	{	
		for(int i=1; i<zmhtdlg.zinidlg.zdmbx.m_grid.GetRowCount(); i++)
		{
			int flag;
			zmhtdlg.zinidlg.zdmbx.m_grid.RowIsEpt(i,zmhtdlg.zinidlg.zdmbx.m_grid.GetColumnCount(),flag);			  
			if(flag)//不空
			{
				zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].ZJ = _wtof(zmhtdlg.zinidlg.zdmbx.m_grid.GetItemText(i, 1));
				zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].sdml = _wtof(zmhtdlg.zinidlg.zdmbx.m_grid.GetItemText(i, 2));
				zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].edml = _wtof(zmhtdlg.zinidlg.zdmbx.m_grid.GetItemText(i, 3));
				zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].Color = _wtoi(zmhtdlg.zinidlg.zdmbx.m_grid.GetItemText(i, 4));
				pBxDBS[NBXDMX]=new  XLDataBase;
				pBxDBS[NBXDMX]->Scml = pzLinep->TYLC(zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].sdml);
				pBxDBS[NBXDMX]->Ecml = pzLinep->TYLC(zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].edml);
				pBxDBS[NBXDMX]->Color = zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].Color;
				XZ.Format(L"%0.3lf",zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].ZJ);
				pBxDBS[NBXDMX]->Read_XLDbs(pzLinep->mdbname,L"边线地面线表",pzLinep->RoadName,XZ);	
				pBxDBS[NBXDMX]->ZJ = zmhtdlg.zinidlg.zdmbx.DmxCsArr[NBXDMX].ZJ;
				NBXDMX++;
				if(NBXDMX==10)
					break;
			}
			else
				break;
		}//统计非空行数
	}
	pdbs->Read_XLDbs(pzLinep->mdbname,L"纵断链表",pzLinep->RoadName);

	//读取坡度表数据
	if(zmhtdlg.zinidlg.ZPInf==1)//全幅
		pdbs->Read_XLDbs(pzLinep->mdbname,L"坡度表",pzLinep->RoadName,L"全幅");
	else if(zmhtdlg.zinidlg.ZPInf==2)//左幅
		pdbs->Read_XLDbs(pzLinep->mdbname,L"坡度表",pzLinep->RoadName,L"左幅");
	else if(zmhtdlg.zinidlg.ZPInf==3)//右幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"右幅");
	else//全部
	{
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"全幅");
		pdbs->ReadAllXZ(pzLinep->mdbname,pzLinep->RoadName);			
		if(pdbs->RoadZ=="左幅")
		{
			pzdbs = new  XLDataBase;
			pzdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"左幅");
		}
		if(pdbs->RoadY=="右幅")
		{
			pydbs = new  XLDataBase;
			pydbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"右幅");
		}
	}
	BAS_DRAW_FUN ob;
	if(pdbs)
	{
		for (i = 0; i < pdbs->NPD; i++)
			pdbs->PDB[i].cml = ob.TYLC1(pdbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}
	if(pzdbs)
	{
		for (i = 0; i < pzdbs->NPD; i++)
			pzdbs->PDB[i].cml = ob.TYLC1(pzdbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}
	if(pydbs)
	{
		for (i = 0; i < pydbs->NPD; i++)
			pydbs->PDB[i].cml = ob.TYLC1(pydbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}

	//读取地面线数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"纵地面线表", pzLinep->RoadName);

	////读取地面线数据
	//pdbs->Read_XLDbs(pzLinep->mdbname, L"高程控制点表", pzLinep->RoadName);
	//
	//zdmdraw.m_LevelCtrlPtArray = pdbs->m_LevelCtrlPtArray;
	//int size = zdmdraw.m_LevelCtrlPtArray.size();
	//读取桥隧数据
	pdbs->Read_XLDbs(pzLinep->mdbname,L"大中桥",pzLinep->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pzLinep->mdbname,L"隧道",pzLinep->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pzLinep->mdbname,L"小桥涵",pzLinep->RoadName);

	//读取超高数据
	pdbs->Read_XLDbs(pzLinep->mdbname,L"左超高分段表",pzLinep->RoadName);
	NZCG = pdbs->NCG;
	pZcg = new CgSZ[NZCG];
	for (i = 0; i < NZCG; i++)
	{
		pZcg[i] = pdbs->CG[i];
		pZcg[i].cml = pzLinep->TYLC(pZcg[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname,L"右超高分段表",pzLinep->RoadName);
	NYCG = pdbs->NCG;
	pYcg = new CgSZ[NYCG];
	for(i=0;i<NYCG;i++)
	{
		pYcg[i]=pdbs->CG[i];
		pYcg[i].cml = pzLinep->TYLC(pYcg[i].xlc);
	}

	//读取超高数据
	pdbs->Read_XLDbs(pzLinep->mdbname,L"左加宽分段表",pzLinep->RoadName);
	NZJK = pdbs->NCG;
	pZjk = new CgSZ[NZJK];
	for(i=0;i<NZJK;i++)
	{
		pZjk[i]=pdbs->CG[i];
		pZjk[i].cml = pzLinep->TYLC(pZjk[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname,L"右加宽分段表",pzLinep->RoadName);
	NYJK = pdbs->NCG;
	pYjk = new CgSZ[NYJK];
	for(i=0;i<NYJK;i++)
	{
		pYjk[i]=pdbs->CG[i];
		pYjk[i].cml = pzLinep->TYLC(pYjk[i].xlc);
	}

	//读取路面宽数据
	pdbs->Read_XLDbs(pzLinep->mdbname,L"左路面宽分段表",pzLinep->RoadName);
	NZLMK = pdbs->NLMK;
	pZlmk = new LMKdata[NZLMK];
	for(i=0;i<NZLMK;i++)
	{
		pZlmk[i]=pdbs->lmk[i];
		pZlmk[i].cml = pzLinep->TYLC(pZlmk[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname,L"右路面宽分段表",pzLinep->RoadName);
	NYLMK = pdbs->NLMK;
	pYlmk = new LMKdata[NYLMK];
	for(i=0;i<NYLMK;i++)
	{
		pYlmk[i]=pdbs->lmk[i];
		pYlmk[i].cml = pzLinep->TYLC(pYlmk[i].xlc);
	}

	//设置绘图参数
	xscale = _wtof(zmhtdlg.zinidlg.m_Hscale)/1000;		//x轴比例尺
	yscale = _wtof(zmhtdlg.zinidlg.m_Zscale)/1000;		//y轴比例尺
	xinori = _wtof(zmhtdlg.zinidlg.m_CorX);				//绘图原点X
	yinori = _wtof(zmhtdlg.zinidlg.m_CorY);				//绘图原点Y
	drawmode = zmhtdlg.zinidlg.DrawMode;				//绘图模式
	Scml = zmhtdlg.zinidlg.pm->TYLC(zmhtdlg.zinidlg.m_StDml);	//起点里程
	Ecml = zmhtdlg.zinidlg.pm->TYLC(zmhtdlg.zinidlg.m_EdDml);	//终点里程
	pzLinep->FormLCB(100,Scml,Ecml);	//按100m间距生成里程表
	i=0;
	int CurTno,j;
	CurTno =_wtoi(zmhtdlg.zinidlg.m_SPage);

	while(Scml < Ecml-0.01)
	{
		//传递绘图数据
		zdmdraw.m_pm = zmhtdlg.zinidlg.pm;
		zdmdraw.CurTNo =CurTno+i;
		zdmdraw.m_TKName = zmhtdlg.zinidlg.m_TKName;
		zdmdraw.m_dzgkname = zmhtdlg.zinidlg.m_dzgkname;
		zdmdraw.m_bzym = zmhtdlg.zinidlg.m_bzym;
		zdmdraw.pDBS = pdbs;
		zdmdraw.pZDBS = pzdbs;
		zdmdraw.pYDBS = pydbs;
		zdmdraw.NZCG = NZCG;
		zdmdraw.NYCG = NYCG;
		zdmdraw.ZCG = pZcg;
		zdmdraw.YCG = pYcg;
		zdmdraw.NZJK = NZJK;
		zdmdraw.NYJK = NYJK;
		zdmdraw.ZJK = pZjk;
		zdmdraw.YJK = pYjk;
		zdmdraw.NZLMK = NZLMK;
		zdmdraw.NYLMK = NYLMK;
		zdmdraw.ZLMK = pZlmk;
		zdmdraw.YLMK = pYlmk;
		zdmdraw.X_scale = xscale;
		zdmdraw.Y_scale = yscale;
		zdmdraw.X_inori = xinori + i * 1000;
		zdmdraw.Y_inori = yinori;
		zdmdraw.DrawMode = drawmode; 
		zdmdraw.infwgx = zmhtdlg.zinidlg.m_infwgx;
		zdmdraw.isdrawctrh = zmhtdlg.zinidlg.m_IsDrawCtrH;
		zdmdraw.m_Scml = Scml;
		zdmdraw.NBxDmx = NBXDMX;
		zdmdraw.m_VDrawPara.IsDrawFeaturePile = zmhtdlg.zinidlg.m_IsDrawFeaturePile;
		//传递边界地面线数据
		for (j = 0; j < NBXDMX; j++)
			zdmdraw.BxDBS[j] = pBxDBS[j];
		//根据图幅设定传递相应参数
		if(drawmode)//施工
		{
			if(i==0)//第一幅
			{
				if(zmhtdlg.zinidlg.FForZF==0)//分幅输出
				{
					//  cml = Scml+xscale*345;
					cml = Scml + xscale * 700;
					if(cml >Ecml)
						cml = Ecml;
				}
				else//整幅输出
				{
					cml = Ecml;
					//zdmdraw.NTKHt = (pdbs->Hmax-pdbs->Hmin)/yscale;
					//zdmdraw.WTKHt = zdmdraw.NTKHt+20;
					//ads_printf(L"yscale=%lf %lf %lf hmax=%lf hmin=%lf\n",yscale,zdmdraw.NTKHt,zdmdraw.WTKHt,pdbs->Hmax,pdbs->Hmin);
					//
				}
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = true;
			}
			else
			{
				//  cml = Scml+xscale*370;
				cml = Scml+xscale*700;
				if(cml >Ecml)
					cml = Ecml;
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = false;
			}
		}
		else//初步设计
		{
			if(zmhtdlg.zinidlg.FForZF==0)//分幅输出
			{
				cml = Scml+xscale*700;
				if(cml >Ecml)
					cml = Ecml;
			}
			else//整幅输出
			{
				cml = Ecml;
				//zdmdraw.NTKHt = (pdbs->Hmax-pdbs->Hmin)/yscale;
				//zdmdraw.WTKHt = zdmdraw.NTKHt+20;

			}

			zdmdraw.m_Ecml = cml;
		}
		zdmdraw.WXPnum = zmhtdlg.zinidlg.wxplm.wxpnum;
		zdmdraw.WayorRamp = zmhtdlg.zinidlg.wxplm.WayorRamp;
		zdmdraw.InfLmPt[0] = zmhtdlg.zinidlg.wxplm.m_infP1;
		zdmdraw.InfLmPt[1] = zmhtdlg.zinidlg.wxplm.m_infP2;
		zdmdraw.InfLmPt[2] = zmhtdlg.zinidlg.wxplm.m_infP3;
		zdmdraw.InfLmPt[3] = zmhtdlg.zinidlg.wxplm.m_infP4;
		zdmdraw.InfLmPt[4] = zmhtdlg.zinidlg.wxplm.m_infP5;
		zdmdraw.InfLmPt[5] = zmhtdlg.zinidlg.wxplm.m_infP6;
		zdmdraw.InfLmPt[6] = zmhtdlg.zinidlg.wxplm.m_infP7;
		zdmdraw.InfLmPt[7] = zmhtdlg.zinidlg.wxplm.m_infP8;
		zdmdraw.LmPtName[0] = zmhtdlg.zinidlg.wxplm.m_p1name;
		zdmdraw.LmPtName[1] = zmhtdlg.zinidlg.wxplm.m_p2name;
		zdmdraw.LmPtName[2] = zmhtdlg.zinidlg.wxplm.m_p3name;
		zdmdraw.LmPtName[3] = zmhtdlg.zinidlg.wxplm.m_p4name;
		zdmdraw.LmPtName[4] = zmhtdlg.zinidlg.wxplm.m_p5name;
		zdmdraw.LmPtName[5] = zmhtdlg.zinidlg.wxplm.m_p6name;
		zdmdraw.LmPtName[6] = zmhtdlg.zinidlg.wxplm.m_p7name;
		zdmdraw.LmPtName[7] = zmhtdlg.zinidlg.wxplm.m_p8name;
		//传递五线谱参数
		for(int j=0;j<zdmdraw.WXPnum;j++)
		{
			zdmdraw.wxp[zdmdraw.WXPnum-1-j].Lht=zmhtdlg.zinidlg.wxplm.wxp[j].Lht;
			zdmdraw.wxp[zdmdraw.WXPnum-1-j].texth=zmhtdlg.zinidlg.wxplm.wxp[j].texth;
			zdmdraw.wxp[zdmdraw.WXPnum-1-j].wxpinf=zmhtdlg.zinidlg.wxplm.wxp[j].wxpinf;
		}

		zdmdraw.DrawZDM();

		//20190718	自动缩放，设定为第一幅图，周围扩大100
		if (zdmdraw.IsFirst)
		{
			Left = zdmdraw.X_inori - 100.0;
			Bottom = zdmdraw.Y_inori - 100.0;
			Right = zdmdraw.X_inori + zdmdraw.m_Frame_Center_X * 2 + 100.0;
			Top = zdmdraw.Y_inori + zdmdraw.m_Frame_Center_Y * 2 + 100.0;
		}

		Scml = zdmdraw.m_Ecml;
		DrawZdm.Add(zdmdraw);
		i++;
	}

	NZdmT = i;

	if(pZcg) delete []pZcg;
	if(pYcg) delete []pYcg;

	BAS_DRAW_FUN::Zoom(Left, Bottom, Right, Top);
}

// This is command 'AJUSTAXLE_Road'	调整断高
void AJUSTAXLE()
{
	int i,rc;
	ads_point PT;
	double cml;

	rc=ads_getpoint(NULL,L"\n请选择断轴位置:",PT); 
	if(rc==RTCAN)
		return;
	for (i = 0; i < NZdmT; i++)
	{
		cml = DrawZdm[i].sxtocml(PT[X]);
		if (cml >= DrawZdm[i].m_Scml - 0.1&&cml < DrawZdm[i].m_Ecml)
			DrawZdm[i].AjustAxle(PT);
		
	}
}
// This is command 'ADDAXLE_Road'
void ADDAXLE()
{
	int i,rc;
	ads_point PT;
	double cml;

	rc=ads_getpoint(NULL,L"\n请选择断轴位置:",PT); 
	if(rc==RTCAN)
		return;
	for(i=0;i<NZdmT;i++)
	{
		cml = DrawZdm[i].sxtocml(PT[X]);
		if(cml>DrawZdm[i].m_Scml&&cml<DrawZdm[i].m_Ecml);
		DrawZdm[i].AddAxle(PT);
	}
}

// This is command 'DELAXLE_Road'
void DELAXLE()
{
	int i,rc;
	ads_point PT;
	double cml;

	rc=ads_getpoint(NULL,L"\n请选择断轴位置:",PT); 
	if(rc==RTCAN)
		return;
	for(i=0;i<NZdmT;i++)
	{
		cml = DrawZdm[i].sxtocml(PT[X]);
		if(cml>DrawZdm[i].m_Scml&&cml<DrawZdm[i].m_Ecml);
		DrawZdm[i].DelAxle(PT);
	}
}

// This is command 'ZJBSCD' 直接式变速车道设计
#include "DialogSetDirectRamp.h"
void ZJBSCD()
{
	// 定义直接式变速车道设计对话框对象
	CDialogSetDirectRamp * m_pDlgSetDRamp = NULL;
	// 创建并显示对话框对象
	if (m_pDlgSetDRamp) 
	{
		delete m_pDlgSetDRamp;
		m_pDlgSetDRamp = NULL;
	}
	m_pDlgSetDRamp = new CDialogSetDirectRamp;		
	m_pDlgSetDRamp->Create(IDD_DIALOGDIRECTRAMP);
	m_pDlgSetDRamp->ShowWindow(SW_SHOW);
}
// This is command 'OUTZDM'	输出纵面图
void OUTZDM()
{
	int i;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"zoom", RTSTR, L"EXTENT", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"zoom", RTSTR, L"EXTENT", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010

	if (NZdmT>0)
	{
		for (i = 0; i < NZdmT; i++)
			DrawZdm[i].WriteBlock();
	}
	else
	{
		AfxMessageBox(L"无纵断面设计");
	}
	
}

// This is command 'JCDRAWPM_Road'	
void DRAWGRID()
{
	// TODO: Implement the command
	DrawJWGRID  dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	dlg.DRAWGRID();
}

// This is command 'JCDRAWPM'	绘平面图	改移道路	201907
void penglihuiJCDRAWPM()
{
	// TODO: Implement the command
   CDrawPmDlg dlg;
   JD_CENTER *pmtmp = NULL;
   CString ZDKfilename;
   int RoadSum,i;
  
   if(dlg.DoModal()!=IDOK)
	   return;
  
   BAS_DRAW_FUN::xlpoint PZ;
#ifdef VERSION_CAD2016
   acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
   acedCommandS(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
   acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
   ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
   ads_command(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
   ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2010

   pzLinep = dlg.pm;
   PZ.lc = dlg.pm->TYLC(dlg.m_QD);
   pzLinep->xlpoint_pz(&PZ);
   ZOOM_P1[Y]=PZ.x;ZOOM_P1[X]=PZ.y;
   PZ.lc = dlg.pm->TYLC(dlg.m_ZD);
   pzLinep->xlpoint_pz(&PZ);
   ZOOM_P2[Y]=PZ.x;ZOOM_P2[X]=PZ.y;
#ifdef VERSION_CAD2016
   acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
   ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2010
   
   AcGePoint3d Pt;
   Pt.x = dlg.pm->XYArray[1][9]; Pt.y = dlg.pm->XYArray[1][8];
   pzLinep->maketext(Pt,L"",0,0);
   pmtmp = pzLinep;
   if(dlg.m_DrawAll==1)//绘制单条线路
	   RoadSum = 1;
   else //绘制所有线路
	   RoadSum = dlg.DBS.m_RoadSum;

   if(dlg.m_DelEntInf)
   {
	   CString RoadName;
	   for(i=0;i<RoadSum;i++)
	   {
		   if(RoadSum==1)//绘制单条线路
			   RoadName = dlg.pm->RoadName;
		   else
			   RoadName = dlg.DBS.Road[i].RoadName;
		   dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"控制点表",RoadName);		   			   
		   dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"断链表",RoadName);
		   if(dlg.DBS.NJdCen>0)
		   {
//			   if(dlg.pm)
//				   delete dlg.pm;
			   dlg.pm=new JD_CENTER();//主线对象
			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
			   if(dlg.DBS.NDL>0)
				   dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
			   _tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
			   _tcscpy(dlg.pm->RoadName,RoadName);
		   }
		   else
		   {
			  dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"线元表",RoadName);	
			  if(dlg.DBS.XYNum>0)
			  {
//				  if(dlg.pm)
//					  delete dlg.pm;

				  dlg.pm=new JD_CENTER(dlg.DBS.XYArray,dlg.DBS.XYNum);//主线对象
				  //			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
				  if(dlg.DBS.NDL>0)
					  dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
				  _tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
				  _tcscpy(dlg.pm->RoadName,RoadName);
			  }
		   }
		   GTZDK_ROAD *DrawZDK=new GTZDK_ROAD;
		   if(RoadSum==1)
		   {
			   DrawZDK->Scml = dlg.pm->TYLC(dlg.m_QD);
			   DrawZDK->Ecml = dlg.pm->TYLC(dlg.m_ZD);
		   }
		   else
		   {
			   DrawZDK->Scml = dlg.pm->DLArray[0].ELC;
			   DrawZDK->Ecml = dlg.pm->XLLength;
		   }
		   DrawZDK->DFACTOR = _wtof(dlg.m_SCALE)/1000;
		   FILE *fpr;
		   DrawZDK->pLine = dlg.pm;	   
		   ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + dlg.pm->RoadName;
		   if((fpr=_wfopen(ZDKfilename,L"r"))==NULL)
		   {
			   ads_printf(L"没有%s占地宽文件!\n",ZDKfilename);
			   continue;
		   }		
		   DrawZDK->ReadZDKFile(fpr);
		   fclose(fpr);
		   DrawZDK->DelEntInYD();
	   }
   }
   
   CDrawPM *drawpm;
   for (i = 0; i < RoadSum; i++)
   {
	   //读取线路信息	
	   if (dlg.m_DrawAll == 1)		//绘制单条线路
	   {
		   dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"控制点表",dlg.pm->RoadName);		   			   
		   dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"断链表",dlg.pm->RoadName);
		   CString rname = dlg.pm->RoadName;
		   if(dlg.DBS.NJdCen>0)
		   {
			   dlg.pm=new JD_CENTER();//主线对象
			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
			   if(dlg.DBS.NDL>0)
				   dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
			   _tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
			   _tcscpy(dlg.pm->RoadName,rname);
		   }
		   
	       drawpm = new CDrawPM(dlg.pm,dlg.m_QD,dlg.m_ZD);
	   }
	   else
	   {		   
		   dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"控制点表",dlg.DBS.Road[i].RoadName);		   			   
		   dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"断链表",dlg.DBS.Road[i].RoadName);
		   if(dlg.DBS.NJdCen>0)
		   {
//			   if(dlg.pm)
//				   delete dlg.pm;
			   dlg.pm=new JD_CENTER();//主线对象
			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
			   if(dlg.DBS.NDL>0)
				   dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
			   _tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
			   _tcscpy(dlg.pm->RoadName,dlg.DBS.Road[i].RoadName);
		   }
		   else
		   {
              dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"线元表",dlg.DBS.Road[i].RoadName);
			  if(dlg.DBS.XYNum>0)
			  {
//				  if(dlg.pm)
//					  delete dlg.pm;
				  dlg.pm=new JD_CENTER(dlg.DBS.XYArray,dlg.DBS.XYNum);//主线对象
				  //			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
				  if(dlg.DBS.NDL>0)
					  dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
				  _tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
				  _tcscpy(dlg.pm->RoadName,dlg.DBS.Road[i].RoadName);
			  }
		   }
		   dlg.m_QD = dlg.pm->DLArray[0].ELC;
		   dlg.m_ZD = dlg.pm->DLArray[dlg.pm->DLNum-1].BLC;	
		   drawpm= new CDrawPM(dlg.pm,dlg.m_QD,dlg.m_ZD);
	   }

	   //设置绘图参数
	   drawpm->DFACTOR = _wtof(dlg.m_SCALE)/1000;
	   drawpm->texth = dlg.m_WHT;
	   drawpm->m_DrawPara.TextHPara.EarthLineTextH = drawpm->m_DrawPara.TextHPara.IntersectionInforTextH =
		   drawpm->m_DrawPara.TextHPara.MileTextH = drawpm->m_DrawPara.TextHPara.PileTextH = dlg.m_WHT * drawpm->DFACTOR;
	   drawpm->DFACTOR =  drawpm->DFACTOR*( drawpm->texth/3.0);
	   drawpm->drawfldzdk = dlg.m_drawfldzdk;
	   if(dlg.m_XSW=="一位")
		   drawpm->XSW = 1;
	   else if(dlg.m_XSW=="两位")
		   drawpm->XSW = 2;
	   else
		   drawpm->XSW = 3;

	   if (dlg.m_LCBZ == "二十米标")
		   drawpm->LcSpacer = 20;
	   else if(dlg.m_LCBZ =="百米标")
		   drawpm->LcSpacer = 100;
	   else if(dlg.m_LCBZ =="五百米标")
		   drawpm->LcSpacer = 500;
	   else
		   drawpm->LcSpacer = 1000;
	   drawpm->ZdkMode = dlg.m_ZDKMode;
	   drawpm->ZdkZlen = dlg.m_ZcLen;
	   drawpm->ZdkYlen = dlg.m_YcLen;
	   drawpm->BzRAL = dlg.m_BZRAL;
	   drawpm->ZxWid = _wtof(dlg.m_ZxWid);
	   drawpm->YdxWid = _wtof(dlg.m_YDXWid);

	   //绘制线路中线
	   //drawpm->DrawXLZX();
	   drawpm->Draw_Center_Line();

	   if(dlg.DrawMode==0)//线位
	   {
		   drawpm->DrawJDLine();	//绘制交点连线
		   drawpm->BZLC(0);			//标注里程
		   //drawpm->BZTZD(0);		//标注特征桩
		   drawpm->Draw_PI_Infor();	//绘制交点信息	20190706
		   if(dlg.m_DrawDZQ)	//绘制大中桥
		   {
               drawpm->DrawDZQ();
			  // ads_printf(L"OK!\n");
		   }
		   if(dlg.m_DrawTun)	//绘制隧道
		   	   drawpm->DrawTun();
		   
		   if(dlg.m_DrawBM)		//绘制水准点
			   drawpm->DrawBM();
		   if(dlg.m_DrawDX)		//绘制导线
			   drawpm->DrawDX();
		   if(dlg.m_DrawCulvet)	//绘制小桥涵
		   {
			    drawpm->ReadHdmResultFile();
		  	    drawpm->DrawCulvet();
		   }

	   }
	   else if(dlg.DrawMode==1)//总体
	   {

		   drawpm->BZLC(1);
		   drawpm->BZTZD(1);

		   drawpm->DrawZTT();

		   drawpm->ZdkMode=4;
		   drawpm->YdxWid = 0.0;
		   drawpm->DrawZDK();//绘用地线

		   if(dlg.m_DrawBM)
			   drawpm->DrawBM();
		   if(dlg.m_DrawDX)
			   drawpm->DrawDX();

		   if(dlg.m_DrawCulvet)
		  	    drawpm->DrawCulvet();
		   if(dlg.m_DrawDZQ)
		  	   drawpm->DrawDZQ();
		   if(dlg.m_DrawTun)
		  	   drawpm->DrawTun();
	   }
	   else//用地
	   {		   
		   drawpm->BZLC(1);
		   drawpm->BZTZD(1);
		   drawpm->DrawZDK();
		   if(dlg.m_DrawDZQ)
			   drawpm->DrawDZQ();
		   if(dlg.m_DrawTun)
			   drawpm->DrawTun();
		   if(dlg.m_DrawBM)
			   drawpm->DrawBM();
		   if(dlg.m_DrawDX)
			   drawpm->DrawDX();
		   if(dlg.m_DrawCulvet)
		  	    drawpm->DrawCulvet();
	   }
	   if(drawpm) 
	   { 
		   delete drawpm;
		   drawpm = NULL;
	   }
   }
}


void penglihuic()
{
	// TODO: Implement the command

	CDrawRoadsDlg RoadsDlg;
	if (RoadsDlg.DoModal()==IDOK)
	{
		for (int iRoad=0; iRoad<RoadsDlg.m_SelRoadArray.GetSize(); iRoad++)
		{
			CString MdbName = Cworkdir + L"\\DATA\\" + RoadsDlg.m_SelRoadArray[iRoad] + L".mdb";
			CString pmcsFName = Cworkdir + L"\\DATA\\" + RoadsDlg.m_SelRoadArray[iRoad] + L".pmcs";
			CString MBpmcsFName = Cworkdir + L"\\DATA\\模板.pmcs";
			CDrawPmDlg dlg;
			CString ZDKfilename;
			int RoadSum,i;
			dlg.m_Dbs = MdbName;
			if (!dlg.ReadCSFileForDraw())
				return;

			BAS_DRAW_FUN::xlpoint PZ;
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

			PZ.lc = dlg.pm->TYLC(dlg.m_QD);
			dlg.pm->xlpoint_pz(&PZ);
			ZOOM_P1[Y]=PZ.x;ZOOM_P1[X]=PZ.y;
			PZ.lc = dlg.pm->TYLC(dlg.m_ZD);
			dlg.pm->xlpoint_pz(&PZ);
			ZOOM_P2[Y]=PZ.x;ZOOM_P2[X]=PZ.y;
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, RTNONE);
#endif // VERSION_CAD2010
			
			AcGePoint3d Pt;
			Pt.x = dlg.pm->XYArray[1][9]; Pt.y = dlg.pm->XYArray[1][8];
			dlg.pm->maketext(Pt,L"",0,0);
			if(dlg.m_DrawAll==1)//绘制单条线路
				RoadSum = 1;
			else //绘制所有线路
				RoadSum = dlg.DBS.m_RoadSum;

			if(dlg.m_DelEntInf)
			{
				CString RoadName;
				for(i=0;i<RoadSum;i++)
				{
					if(RoadSum==1)//绘制单条线路
						RoadName = dlg.pm->RoadName;
					else
						RoadName = dlg.DBS.Road[i].RoadName;
					dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"控制点表",RoadName);		   			   
					dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"断链表",RoadName);
					if(dlg.DBS.NJdCen>0)
					{
						if(dlg.pm)
							delete dlg.pm;
						dlg.pm=new JD_CENTER();//主线对象
						dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
						if(dlg.DBS.NDL>0)
							dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
						_tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
						_tcscpy(dlg.pm->RoadName,RoadName);
					}
					else
					{
						dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"线元表",RoadName);	
						if(dlg.DBS.XYNum>0)
						{
							if(dlg.pm)
								delete dlg.pm;

							dlg.pm=new JD_CENTER(dlg.DBS.XYArray,dlg.DBS.XYNum);//主线对象
							//			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
							if(dlg.DBS.NDL>0)
								dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
							_tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
							_tcscpy(dlg.pm->RoadName,RoadName);
						}
					}
					if (dlg.m_ZDKMode!=3 && dlg.DrawMode!=0)
					{
						GTZDK_ROAD *DrawZDK=new GTZDK_ROAD;
						if(RoadSum==1)
						{
							DrawZDK->Scml = dlg.pm->TYLC(dlg.m_QD);
							DrawZDK->Ecml = dlg.pm->TYLC(dlg.m_ZD);
						}
						else
						{
							DrawZDK->Scml = dlg.pm->DLArray[0].ELC;
							DrawZDK->Ecml = dlg.pm->XLLength;
						}
						DrawZDK->DFACTOR = _wtof(dlg.m_SCALE)/1000;
						FILE *fpr;
						DrawZDK->pLine = dlg.pm;	   
						ZDKfilename =Cworkdir + L"\\data\\" + L"hdmZDK." + Pro + L"." + dlg.pm->RoadName;
						if((fpr=_wfopen(ZDKfilename,L"r"))==NULL)
						{
							ads_printf(L"没有%s占地宽文件!\n",ZDKfilename);
							continue;
						}		
						DrawZDK->ReadZDKFile(fpr);
						fclose(fpr);
						if (dlg.m_DelEntInf)
							DrawZDK->DelEntInYD();
					}

				}
			}

			CDrawPM *drawpm;
			for(i=0;i<RoadSum;i++)
			{
				if(dlg.m_DrawAll==1)//绘制单条线路
				{
					dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"控制点表",dlg.pm->RoadName);		   			   
					dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"断链表",dlg.pm->RoadName);
					CString rname = dlg.pm->RoadName;
					if(dlg.DBS.NJdCen>0)
					{
						if (dlg.pm)
							delete dlg.pm;
						dlg.pm=new JD_CENTER();//主线对象
						dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
						if(dlg.DBS.NDL>0)
							dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
						_tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
						_tcscpy(dlg.pm->RoadName,rname);
					}

					drawpm= new CDrawPM(dlg.pm,dlg.m_QD,dlg.m_ZD);
				}
				else
				{		   
					dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"控制点表",dlg.DBS.Road[i].RoadName);		   			   
					dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"断链表",dlg.DBS.Road[i].RoadName);
					if(dlg.DBS.NJdCen>0)
					{
						if(dlg.pm)
							delete dlg.pm;
						dlg.pm=new JD_CENTER();//主线对象
						dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
						if(dlg.DBS.NDL>0)
							dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
						_tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
						_tcscpy(dlg.pm->RoadName,dlg.DBS.Road[i].RoadName);
					}
					else
					{
						dlg.DBS.Read_XLDbs(dlg.m_Dbs,L"线元表",dlg.DBS.Road[i].RoadName);
						if(dlg.DBS.XYNum>0)
						{
							if(dlg.pm)
								delete dlg.pm;
							dlg.pm=new JD_CENTER(dlg.DBS.XYArray,dlg.DBS.XYNum);//主线对象
							//			   dlg.pm->SetJdCenterArray(dlg.DBS.JdCenArray,dlg.DBS.NJdCen);
							if(dlg.DBS.NDL>0)
								dlg.pm->setDLB(dlg.DBS.DLArray,dlg.DBS.NDL);
							_tcscpy(dlg.pm->mdbname,dlg.m_Dbs);
							_tcscpy(dlg.pm->RoadName,dlg.DBS.Road[i].RoadName);
						}
					}
					dlg.m_QD = dlg.pm->DLArray[0].ELC;
					dlg.m_ZD = dlg.pm->DLArray[dlg.pm->DLNum-1].BLC;	
					drawpm= new CDrawPM(dlg.pm,dlg.m_QD,dlg.m_ZD);
				}
				drawpm->DFACTOR = _wtof(dlg.m_SCALE)/1000;
				drawpm->texth = dlg.m_WHT;
				drawpm->DFACTOR =  drawpm->DFACTOR*( drawpm->texth/3.0);
				drawpm->drawfldzdk = dlg.m_drawfldzdk;
				if(dlg.m_XSW==L"一位")
					drawpm->XSW = 1;
				else if(dlg.m_XSW==L"两位")
					drawpm->XSW = 2;
				else
					drawpm->XSW = 3;
				if (dlg.m_LCBZ == L"二十米标")
					drawpm->LcSpacer = 20;
				else if(dlg.m_LCBZ ==L"百米标")
					drawpm->LcSpacer = 100;
				else if(dlg.m_LCBZ ==L"五百米标") 
					drawpm->LcSpacer = 500;
				else if(dlg.m_LCBZ ==L"公里标")
					drawpm->LcSpacer = 1000;
				else
					drawpm->LcSpacer = 1e10;
				drawpm->ZdkMode = dlg.m_ZDKMode;
				drawpm->ZdkZlen = dlg.m_ZcLen;
				drawpm->ZdkYlen = dlg.m_YcLen;
				drawpm->BzRAL = dlg.m_BZRAL;
				drawpm->ZxWid = _wtof(dlg.m_ZxWid);
				drawpm->YdxWid = _wtof(dlg.m_YDXWid);
				drawpm->DrawXLZX();

				if(dlg.DrawMode==0)//线位
				{

					if (dlg.m_DrawJD)
						drawpm->DrawJDLine();

					drawpm->DrawSeML();
					if (dlg.m_LCBZ != L"不标注")
					{
						drawpm->BZLC(0);
						drawpm->BZTZD(0);
					}
					if(dlg.m_DrawDZQ)
					{
						drawpm->DrawDZQ();
					}
					if(dlg.m_DrawTun)
						drawpm->DrawTun();

					if(dlg.m_DrawBM)
						drawpm->DrawBM();
					if(dlg.m_DrawDX)
						drawpm->DrawDX();
					if(dlg.m_DrawCulvet)
					{
						//drawpm->ReadHdmResultFile();
						drawpm->DrawCulvet();
					}

				}
				else if(dlg.DrawMode==1)//总体
				{
					drawpm->DrawSeML();
					if (dlg.m_LCBZ != L"不标注")
					{
						drawpm->BZLC(1);
						drawpm->BZTZD(1);
					}

					drawpm->DrawZTT();

					drawpm->ZdkMode=4;
					drawpm->YdxWid = 0.0;
					drawpm->DrawZDK();//绘用地线

					if(dlg.m_DrawBM)
						drawpm->DrawBM();
					if(dlg.m_DrawDX)
						drawpm->DrawDX();

					if(dlg.m_DrawCulvet)
						drawpm->DrawCulvet();
					if(dlg.m_DrawDZQ)
						drawpm->DrawDZQ();
					if(dlg.m_DrawTun)
						drawpm->DrawTun();
				}
				else//用地
				{
					drawpm->DrawSeML();
					if (dlg.m_LCBZ != L"不标注")
					{
						drawpm->BZLC(1);
						drawpm->BZTZD(1);
					}
					drawpm->DrawZDK();
					if(dlg.m_DrawDZQ)
						drawpm->DrawDZQ();
					if(dlg.m_DrawTun)
						drawpm->DrawTun();
					if(dlg.m_DrawBM)
						drawpm->DrawBM();
					if(dlg.m_DrawDX)
						drawpm->DrawDX();
					if(dlg.m_DrawCulvet)
						drawpm->DrawCulvet();
				}
				if(drawpm) 
				{ 
					delete drawpm;
					drawpm = NULL;
				}
			}
		}
	}
	return;
}

// This is command 'TRANJD'
void penglihuiTRANJD()
{
	// TODO: Implement the command
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;
	CString fname="C:\\JD.txt";

	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();
	if(pEnt->isKindOf(JD_CENTER::desc()))
	{
		JD_CENTER *pJC=JD_CENTER::cast(pEnt);
		/*acdbOpenObject(pJC,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pJC,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pJC->TranJD(/*fname*/);
		pJC->close();
	}
}

// This is command 'TRANCENTER'
void penglihuiTRANCENTER()
{
	// TODO: Implement the command
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;
	CString fname="C:\\JD.txt";

	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	if(acdbOpenObject(pEnt,eId,AcDb::kForWrite)!=Acad::eOk)
		return;
	pEnt->close();
	if(pEnt->isKindOf(JD_CENTER::desc()))
	{
		JD_CENTER *pJC=JD_CENTER::cast(pEnt);
		/*acdbOpenObject(pJC,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pJC,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pJC->TranCenter(/*fname*/);
		pJC->close();
	}

	/*
	int jdnum,i;
	CString fname="C:\\JD.txt";
	FILE *fp=_wfopen(fname,L"r");
	if(fp!=NULL)
	{
	fwscanf(fp,L"%d",&jdnum);
	BAS_DRAW_FUN::JdCenter *JdAy=new BAS_DRAW_FUN::JdCenter[jdnum];
	BAS_DRAW_FUN::JdCenter *CurJDCen;
	int temint;
	for(i=0;i<jdnum;i++)
	{
	CurJDCen=&JdAy[i];
	fwscanf(fp,L"%d%d%lf%lf%lf%lf%lf%d%d",&temint,&CurJDCen->JDXZ,&CurJDCen->N,&CurJDCen->E,
	&CurJDCen->A1,&CurJDCen->R,&CurJDCen->A2,&CurJDCen->LeftOrRight,&CurJDCen->LinkAfter);

	acutPrintf(L"%d,%d,%lf,%lf,%lf,%lf,%lf,%d,%d\n",temint,CurJDCen->JDXZ,CurJDCen->N,CurJDCen->E,
	CurJDCen->A1,CurJDCen->R,CurJDCen->A2,CurJDCen->LeftOrRight,CurJDCen->LinkAfter);
	}
	JD_CENTER *pJC=new JD_CENTER;
	pJC->SetJdCenterArray(JdAy,jdnum);
	BAS_DRAW_FUN::AddEntityToDbs(pJC);
	delete []JdAy;
	}
	fclose(fp);*/

}

#include "XLLinkDlg.h"
XLLinkDlg XLLinkDlgObj;
bool IsFirstXLLinkDlg=true;
void penglihuiXLLINK()
{
	if(!IsFirstXLLinkDlg)
	{
		XLLinkDlgObj.ShowWindow(TRUE);
	}
	else
	{
		IsFirstXLLinkDlg=false;
		XLLinkDlgObj.Create(IDD_DIALOG_XLLINK,acedGetAcadFrame());
		XLLinkDlgObj.ShowWindow(SW_SHOW);
	}		
	/*
	ads_name en1,en2;
	ads_point pt1,pt2;
	AcDbObjectId eId1,eId2;
	AcDbEntity *pEnt1,*pEnt2;


	if(RTNORM!=acedEntSel(L"\n请选择第一条线路:",en1,pt1)
	|| RTNORM!=acedEntSel(L"\n请选择第二条线路:",en2,pt2))
	{acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId1,en1);
	acdbGetObjectId(eId2,en2);

	acdbOpenObject(pEnt1,eId1,AcDb::kForWrite);
	pEnt1->close();
	acdbOpenObject(pEnt2,eId2,AcDb::kForWrite);
	pEnt2->close();
	if(pEnt1->isKindOf(JD_CENTER::desc())&&pEnt2->isKindOf(JD_CENTER::desc()))
	{
	JD_CENTER *pJC1=JD_CENTER::cast(pEnt1);
	JD_CENTER *pJC2=JD_CENTER::cast(pEnt2);

	acdbOpenObject(pJC1,eId1,AcDb::kForWrite);
	pJC1->AppendXL(pJC2,pt1[Y],pt1[X],pt2[Y],pt2[X]);		
	pJC1->close();
	acdbOpenObject(pJC2,eId2,AcDb::kForWrite);
	pJC2->erase();
	pJC2->close();
	}*/

}// This is command 'ZXYTOXL'
//转动态设计线	转设计线
void penglihuiZXYTOXL()
{

	//  if(!ReadWorkdir())return ;
	//	double DV=mGuiFanCS.SuDuMuBiao;

	//  L1=get_l0(R,DV);
	//	L2=get_l0(R,DV);
#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
	if(!CheckByTime())
		return;
#endif
	int NLC=3,NH=3,texth=8;
	int rc;
	ads_name en;
	AcDbObjectId eId,GTZXId;
	AcGePoint3d PT,location;
	JD_CENTER *pJC=NULL;
	BAS_DRAW_FUN ob;

	int i=0,j=0,trow;
	//   double JD_array[MAXJDNUM][6];

	//   for (i=0;i<MAXJDNUM;i++)for (j=0;j<6;j++)JD_array[i][j]=0;
	//选择实体
	rc = ads_entsel(L"\n请选择POLYLINE/LINE/ARC: ", en, asDblArray(PT));
	if (rc != RTNORM)
	{  
		ads_printf(L"\nError during object selection"); 
		return;   
	}

	acdbGetObjectId(eId, en);  

	//20190725	不再直接形成线元表的方式转设计线
	CArray<BAS_DRAW_FUN::XYarray,BAS_DRAW_FUN::XYarray>pXYArr;
	pXYArr.RemoveAll();
	ob.FormXYarray(eId,pXYArr);
	if(pXYArr.GetSize()<1)
	{
		AcDbObject *pObj;
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		////ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());	//20190714	修改不提示，用户不需要这个
		if( pObj->isKindOf(AcDb2dPolyline::desc())
			|| pObj->isKindOf(AcDbPolyline::desc())
			|| pObj->isKindOf(AcDbLine::desc())
			|| pObj->isKindOf(AcDbArc::desc()) 
			)
		{   
			if (pObj->isKindOf(AcDb2dPolyline::desc())) 
			{  
				pObj->close();
				AcDb2dPolyline *pPline;

				AcDb2dVertex *pVertex;
				AcGePoint3d location;
				AcDbObjectId vertexObjId;

				if(acdbOpenObject(pPline,eId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				AcDbObjectIterator *pVertIter= pPline->vertexIterator();

				//pPline->

				pPline->close(); 

				for ( i= 0; !pVertIter->done(); i++, pVertIter->step())
				{
				}
				trow=i;	

				if(trow <=0)
					return;

				pJC=new JD_CENTER;
				BAS_DRAW_FUN::JdCenter *JCArray=new BAS_DRAW_FUN::JdCenter[trow];

				//20190724	修改读取速度等级位置，用于计算初始圆曲线半径和缓和曲线长度
				int DV;
				CString grade;
				pJC->ReadRoadDV(DV, grade);
				pJC->DV = DV;
				pJC->Grade = grade;

				for ( i= 0; !pVertIter->done(); i++, pVertIter->step())
				{  //寻找 的某顶点
					vertexObjId = pVertIter->objectId();
					if(acdbOpenObject(pVertex,vertexObjId, AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					location = pVertex->position();

					JCArray[i].JDXZ = IsJiaoDian;
					JCArray[i].LinkAfter = NotRelation;
					JCArray[i].IsOrNotHuiTou = false;
					JCArray[i].N = location[Y];
					JCArray[i].E = location[X];
					JCArray[i].R = 0.0;
					JCArray[i].A1 = 0.0;
					JCArray[i].A2 = 0.0;
					if (i > 0 && i < trow - 1)
					{
						JCArray[i].R = pJC->Cal_Road_MinR(pJC->DV);	
						//pJC->GetLoMin(pJC->DV, JCArray[i].Ls1);
						//JCArray[i].Ls2 = JCArray[i].Ls1;
						//JCArray[i].A1 = JCArray[i].A2 = sqrt(JCArray[i].Ls1 * JCArray[i].R);
					}
						

				};		
				pPline->close();     
				pJC->SetJdCenterArray(JCArray,trow);
				pJC->setRoadName("主线");



				GTZXId = BAS_DRAW_FUN::AddEntityToDbs(pJC);

				delete []JCArray;				
			}
			//  ployline
			else if (pObj->isKindOf(AcDbPolyline::desc())) 
			{  
				pObj->close();
				AcDbPolyline *pPline1;

				AcGePoint3d location;
				AcDbObjectId ObjId;
				int numverts;

				if(acdbOpenObject(pPline1,eId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pPline1 = AcDbPolyline::cast(pObj);
				numverts = pPline1->numVerts();

				if(numverts <=0)
				{
					pPline1->close();
					return;
				}

				pJC=new JD_CENTER;
				BAS_DRAW_FUN::JdCenter *JCArray=new BAS_DRAW_FUN::JdCenter[numverts];

				int DV;
				CString grade;
				pJC->ReadRoadDV(DV, grade);
				pJC->DV = DV;
				pJC->Grade = grade;

				for ( i= 0; i<numverts; i++)
				{  //寻找的某顶点

					pPline1->getPointAt( i, location);
					JCArray[i].JDXZ = IsJiaoDian;
					JCArray[i].LinkAfter = NotRelation;
					JCArray[i].IsOrNotHuiTou = false;
					JCArray[i].N = location[Y];
					JCArray[i].E = location[X];
					JCArray[i].R=0.0;
					JCArray[i].A1 = 0.0;
					JCArray[i].A2 = 0.0;
					if (i > 0 && i < numverts - 1)
					{
						JCArray[i].R = 5.0;
						//JCArray[i].R = pJC->Cal_Road_MinR(pJC->DV);
						//pJC->GetLoMin(pJC->DV, JCArray[i].Ls1);
						//JCArray[i].Ls2 = JCArray[i].Ls1;
						//JCArray[i].A1 = JCArray[i].A2 = sqrt(JCArray[i].Ls1 * JCArray[i].R);
					}
				}

				pPline1->close();

				pJC->SetJdCenterArray(JCArray,numverts);
				pJC->setRoadName("主线");

				GTZXId = BAS_DRAW_FUN::AddEntityToDbs(pJC);

				delete []JCArray;		

			}
			else if  (pObj->isKindOf(AcDbLine::desc())) 
			{     
				pObj->close();

				AcDbLine *LINE=AcDbLine::cast(pObj);
				AcGePoint3d sPt,ePt;
				if(acdbOpenObject(LINE,eId,AcDb::kForWrite)==Acad::eOk)
				{
					sPt=LINE->startPoint();
					ePt=LINE->endPoint();
					LINE->erase();
					LINE->close();
				}

				pJC=new JD_CENTER;
				BAS_DRAW_FUN::JdCenter *JCArray=new BAS_DRAW_FUN::JdCenter[2];
				int DV;
				CString grade;
				pJC->ReadRoadDV(DV, grade);
				pJC->DV = DV;
				pJC->Grade = grade;


				int i=0;
				JCArray[i].JDXZ=IsJiaoDian;
				JCArray[i].LinkAfter=NotRelation;
				JCArray[i].IsOrNotHuiTou=false;
				JCArray[i].N=sPt.y;
				JCArray[i].E=sPt.x;
				JCArray[i].R=0.0;
				JCArray[i].A1=0.0;
				JCArray[i].A2=0.0;

				i=1;
				JCArray[i].JDXZ=IsJiaoDian;
				JCArray[i].LinkAfter=NotRelation;
				JCArray[i].IsOrNotHuiTou=false;
				JCArray[i].N=ePt.y;
				JCArray[i].E=ePt.x;
				JCArray[i].R=0.0;
				JCArray[i].A1=0.0;
				JCArray[i].A2=0.0;

				pJC->SetJdCenterArray(JCArray,2);
				pJC->setRoadName("主线");



				GTZXId = BAS_DRAW_FUN::AddEntityToDbs(pJC);
				delete []JCArray;
			}
			else if(pObj->isKindOf(AcDbArc::desc()))//plh07.17
			{
				pObj->close();

				AcDbArc *ARC=AcDbArc::cast(pObj);
				if(acdbOpenObject(ARC,eId,AcDb::kForWrite)!=Acad::eOk)
					return;

				double j1,j2,x0,y0,R;
				j1=ARC->startAngle();
				j2=ARC->endAngle();
				x0=ARC->center().x;
				y0=ARC->center().y;
				R=ARC->radius();
				ARC->erase();
				ARC->close();

				BCCreateXLEntity BCCreateObj;
				BCCreateObj.SetARC(y0,x0,R,j1,j2);
				BCCreateObj.UpdateData();

				pJC = BCCreateObj.pGtzx;

				GTZXId = BCCreateObj.eId;

			}
		}
		else
		{
			pObj->close();
			ads_printf(L"\nSelected entity is not a Polyline!");
			return;
		} 	
	}
	else
	{
		pJC=new JD_CENTER;
		pJC->XYNum = pXYArr.GetSize()-1;
		if(pJC->XYNum<2)
			return;
		if(pJC->XYArray)
			delete []pJC->XYArray;
		pJC->XYArray=new double[pJC->XYNum+1][10];

		//       FILE *fpw;
		// 		fpw = _wfopen(L"D:\\xydata.txt",L"w");
		//		fwprintf(fpw,L"%d\n",pJC->XYNum);
		for(int i=0;i<=pJC->XYNum;i++)
		{ 
			//			 ads_printf(L"i=%d ",i);
			for(int j=0;j<10;j++)   
			{
				pJC->XYArray[i][j]=pXYArr[i].xyarr[j];
				//				ads_printf(L"%0.3lf ",pJC->XYArray[i][j]);
			}
			//             ads_printf(L"\n");
		}
		//  		 fclose(fpw);


		//	     pJC->XYArrayToJD();
		pJC->DLArray.SetSize(2);
		pJC->setDLM(0,pJC->XYArray[1][6],pJC->XYArray[1][6],L"K",L"K");
		pJC->setDLM(1,pJC->XYArray[pJC->XYNum][6],pJC->XYArray[pJC->XYNum][6],L"K",L"K");
		//	 ads_printf(L"%lf %lf\n",pJC->XYArray[1][6],pJC->XYArray[pJC->XYNum][6]);
		pJC->DLNum=2;
		pJC->IfCalXY = false;
		pJC->SHOWJD = false;
		int DV;
		CString grade;
		pJC->ReadRoadDV(DV,grade);
		pJC->DV=DV;
		pJC->Grade=grade;

		BAS_DRAW_FUN::AddEntityToDbs(pJC);
	}


}

// This is command 'SAVEPMDBS'
//保存方案（平面）
void penglihuiSAVEPMDBS()
{
	// TODO: Implement the command
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbObject * pEnt;


	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt))
	{
		acutPrintf(L"选择失败!");
		return;
	}
	else
	{
		acdbGetObjectId(eId,en);

		if (acdbOpenObject(pEnt,eId,AcDb::kForRead)!=Acad::eOk)
			return;
		pEnt->close();
		pEnt->close();
		if (pEnt->isKindOf(JD_CENTER::desc()))
		{
			InRoadNameDlg dlg(eId);

			dlg.DoModal();		
			/*
			//生成直曲转角表		
			pJC->CreateZQZJB();
			DBS.NJdCen=pJC->JdCenNum;
			DBS.ZQZJB=new BAS_DRAW_FUN::ZQZJTAB[DBS.NJdCen];
			for(i=0;i<DBS.NJdCen;i++)
			{
			DBS.ZQZJB[i]=pJC->ZQZJB[i];
			}*/
		}
		else
			ads_printf(L"选择的非线路实体!\n");


	}

}
//导入方案（平面）
void penglihuiOPENPMDBSBYJD()
{
	// TODO: Implement the command

#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
	if (!CheckByTime())
		return;
#endif
	//线路数组，自动出图专用
	XianLuArray_ZDCTZY.clear();

	bool DRAWRAIL=FALSE;
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	
	GTZX_ROAD* pSJJX = NULL;
	GTZX2_ROAD* pSJEX = NULL;
	GTZX_JYX_ROAD* pJYJX = NULL;
	GTZX2_JYX_ROAD* pJYEX = NULL;
	Path = Pro + L".mdb";
	CFileDialog FDlg(TRUE, L".mdb", Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT | OFN_LONGNAMES | OFN_EXPLORER;
	FDlg.m_ofn.lpstrFilter = L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	bool IsInterSect = false;
	if (FDlg.DoModal() == IDOK)
	{
		POSITION Pos = FDlg.GetStartPosition();
		while (Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos);

			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名

			CString MdfName = Path;
			CString ProName = DBS.GetProByMdbName(Path);
			DaoRuPingMianFangAnMdbMingCheng = ProName;//导入方案对应的数据库名称，自动出图专用


			Path.MakeLower();
			MdfName = Path;
			ProName = DBS.GetProByMdbName(Path);

			MdfName.Replace(ProName + L".mdb", L"改移道路.mdf");
			for (int i = 0; i < DBS.m_RoadSum; i++)
			{
				AcDbObjectId XianLuID = NULL;
				JD_CENTER* pJC = NULL;
				AcDbObjectId TieluID = NULL;

				pJC = g_RoadWorkingDatabase.CreateHorizontalRoad(Path, DBS.Road[i].RoadName, true);
				if(!pJC)
					continue;

				double PD_array[MAXBPDNUM][4];
				int res = DBS.Read_XLDbs(Path, L"坡度表", L"主线", L"全幅");
				int res1 = DBS.Read_XLDbs(Path, L"纵断链表", L"主线");
				if (res && res1)//读坡度
				{
					int NBPD = DBS.NPD;
					for (int i = 0; i < NBPD; i++)
					{
						PD_array[i][0] = pJC->TYLC1(DBS.PDB[i].ml, DBS.NZDL, DBS.ZDLArray);
						PD_array[i][1] = DBS.PDB[i].Level;
						PD_array[i][2] = DBS.PDB[i].Rshu;
						PD_array[i][3] = 0.0;
					}

					if (DBS.PDB)
					{
						if (DBS.PDB) delete[]DBS.PDB;
						DBS.PDB = NULL;
					}
					if (NBPD > 0)
					{
						pJC->TrsBPDArraytoCM(NBPD, PD_array);
					}
				}

				DBS.ModifyRoadSum = 0;
				DBS.Read_XLDbs(MdfName, L"改移道路表", ProName);
				if (DBS.ModifyRoadSum > 0)
				{
					pJC->m_RelRailName = DBS.pModifyRoad[0].RailName;

					IsInterSect = PmPointer::CalIntersectPt(pJC);
				}

				acedGetAcadDwgView()->SetFocus();

				pJC->setLayer(L"公路平面设计专用层");
				XianLuID = BAS_DRAW_FUN::AddEntityToDbs(pJC);
				if (XianLuID != NULL)
					XianLuArray_ZDCTZY.push_back(XianLuID);


			}
			DBS.Release();
		}
	}
}
void penglihuiOPENPMDBSBYXY()
{
	// TODO: Implement the command
	//XLDataBase DBS;
	//CWorkDir WorkDir;
	//CString Path;

	//Path = Pro+".mdb";
	//CFileDialog FDlg(TRUE,L".mdb",Path);
	//FDlg.m_ofn.lpstrDefExt = L"mdb";
	//FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	////支持多个数据库输入
	//FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_LONGNAMES;
	//FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
	//if(FDlg.DoModal()==IDOK)
	//{
	//	POSITION Pos=FDlg.GetStartPosition();
	//	while(Pos)
	//	{
	//		//依次打开每一个数据库
	//		Path = FDlg.GetNextPathName(Pos); 			
	//		DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名

	//		for(int i=0; i<DBS.m_RoadSum; i++)
	//		{	
	//			JD_CENTER *pJC=NULL;
	//			DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);			
	//			DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);	
	//			if(DBS.XYNum>0)
	//			{
	//				pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
	//				pJC->setRoadName(DBS.Road[i].RoadName);
	//				pJC->setMdbName(Path);
	//				if(DBS.NDL>0)
	//				{
	//					pJC->setDLB(DBS.DLArray,DBS.NDL);
	//				}
	//				pJC->setLayer(L"公路平面设计专用层");
	//				BAS_DRAW_FUN::AddEntityToDbs(pJC);
	//			}
	//		}
	//	}
	//	DBS.Release();
	//}		

	//2019.10.10
	//线路数组，自动出图专用
	XianLuArray_ZDCTZY.clear();

#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
	if (!CheckByTime())
		return;
#endif
	bool DRAWRAIL=FALSE;
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	
	GTZX_ROAD* pSJJX = NULL;
	GTZX2_ROAD* pSJEX = NULL;
	GTZX_JYX_ROAD* pJYJX = NULL;
	GTZX2_JYX_ROAD* pJYEX = NULL;
	Path = Pro + L".mdb";
	CFileDialog FDlg(TRUE, L".mdb", Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT | OFN_LONGNAMES | OFN_EXPLORER;
	FDlg.m_ofn.lpstrFilter = L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	bool IsInterSect = false;
	if (FDlg.DoModal() == IDOK)
	{
		POSITION Pos = FDlg.GetStartPosition();
		while (Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos);

			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名

			CString MdfName = Path;
			CString ProName = DBS.GetProByMdbName(Path);
			DaoRuPingMianFangAnMdbMingCheng = ProName;//导入方案对应的数据库名称，自动出图专用


			Path.MakeLower();
			MdfName = Path;
			ProName = DBS.GetProByMdbName(Path);

			MdfName.Replace(ProName + L".mdb", L"改移道路.mdf");
			for (int i = 0; i < DBS.m_RoadSum; i++)
			{
				AcDbObjectId XianLuID = NULL;
				JD_CENTER* pJC = NULL;
				AcDbObjectId TieluID = NULL;

				DBS.Read_XLDbs(Path, L"控制点表", DBS.Road[i].RoadName);
				DBS.Read_XLDbs(Path, L"断链表", DBS.Road[i].RoadName);
				DBS.Read_XLDbs(Path, L"线元表", DBS.Road[i].RoadName);
				//线元模式开始2019.10.11
				//线元模式结束
				if (DBS.NJdCen > 0)
				{
					//形成线路实体
					pJC = new JD_CENTER;

					pJC->SetJdCenterArray(DBS.JdCenArray, DBS.NJdCen);
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);

					if (DBS.NDL > 0)
					{
						pJC->setDLB(DBS.DLArray, DBS.NDL);
					}
					DBS.Read_XLDbs(Path, L"线路属性");
					pJC->DV = DBS.RoadXZ.DV;

					//////////////////////////////////////////////////////////////////////////
					double PD_array[MAXBPDNUM][4];
					int res = DBS.Read_XLDbs(Path, L"坡度表", L"主线", L"全幅");
					int res1 = DBS.Read_XLDbs(Path, L"纵断链表", L"主线");
					if (res && res1)//读坡度
					{
						int NBPD = DBS.NPD;
						for (int i = 0; i < NBPD; i++)
						{
							PD_array[i][0] = pJC->TYLC1(DBS.PDB[i].ml, DBS.NZDL, DBS.ZDLArray);
							PD_array[i][1] = DBS.PDB[i].Level;
							PD_array[i][2] = DBS.PDB[i].Rshu;
							PD_array[i][3] = 0.0;
						}

						if (DBS.PDB)
						{
							if (DBS.PDB) delete[]DBS.PDB;
							DBS.PDB = NULL;
						}
						if (NBPD > 0)
						{
							pJC->TrsBPDArraytoCM(NBPD, PD_array);
						}
					}

					DBS.ModifyRoadSum = 0;
					DBS.Read_XLDbs(MdfName,L"改移道路表", ProName);
					if (DBS.ModifyRoadSum > 0)
					{
						pJC->m_RelRailName = DBS.pModifyRoad[0].RailName;
						CString RailName = pJC->m_RelRailName;
						if (!pJC->m_RelRailName.IsEmpty())
						{
							
							//CString JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
							//CString ProStr = RailName.Mid(0, RailName.GetLength() - 2);
							//CString RailMdbName = MdfName;
							//RailMdbName.Replace(L"\\改移道路.mdf", L"\\"+ ProStr + ".mdb");

							BAS_DRAW_FUN BasFun;
							CString JXorEX, ProStr, RailMdbName;
							if (RailName.ReverseFind('\\') == -1)
							{
								JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
								ProStr = RailName.Mid(0, RailName.GetLength() - 2);
								RailMdbName = MdfName;
								RailMdbName.Replace(L"\\改移道路.mdf", L"\\" + ProStr + ".mdb");
							}
							else
							{
								RailName = BasFun.GetProByMdbName(RailName);
								JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
								ProStr = RailName.Mid(0, RailName.GetLength() - 2);
								RailMdbName = pJC->m_RelRailName;
								RailMdbName.Replace(RailName, ProStr);
							}

							//AfxMessageBox(L"\\改移道路\\DATA\\改移道路.mdf");
							//RailMdbName = RailMdbName + L"\\DATA\\" + ProStr + L".mdb";	//铁路方案路径名
							/*RailMdbName.Replace(L"\\改移道路\\data\\改移道路.mdf", L"\\DATA\\" + ProStr + L".mdb");*/
							
							
							bool isSJX = PmPointer::SJXorJYX(RailMdbName, 6, false);
							//RailMdbName+="\\DATA\\" + ProStr + ".mdb";
							if (JXorEX == L"基线")
							{
								if (isSJX)
								{
									/*pSJJX = new GTZX_ROAD;*/
									pSJJX = PmPointer::CreateSJJX(RailMdbName);

									IsInterSect = PmPointer::CalIntersectPt(pJC, pSJJX);
									
								}
								else
								{
									pJYJX = PmPointer::CreateGTSJJXPointer(RailMdbName);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pJYJX);
									
								}
							}
							else if (JXorEX == L"左线")
							{
								if (isSJX)
								{
									pSJEX = PmPointer::CreateSJEX(RailMdbName, -1);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pSJEX);
									
								}

								else
								{
									pJYEX = PmPointer::CreateGTSJEXPointer(RailMdbName);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pJYEX);
									
								}
							}
							else if (JXorEX == L"右线")
							{
								if (isSJX)
								{
									pSJEX = PmPointer::CreateSJEX(RailMdbName, 1);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pSJEX);
									
								}
								else
								{
									pJYEX = PmPointer::CreateGTSJEXPointer(RailMdbName);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pJYEX);
									
								}
							}
							else
								AfxMessageBox(L"ProName对应铁路线路名设置有误");
						}
					}
					//////////////////////////////////////////////////////////////////////////

					acedGetAcadDwgView()->SetFocus();
					//ACHAR choice[10];
					//if (i==0)
					//{
					//	ads_initget(RSG_OTHER, L"Y y N n");
					//	if (ads_getkword(L"\n是否绘制铁路线路: 是(Y)，或[否(N)] <Y>:  ", choice) == RTNONE)
					//		_tcscpy(choice, L"Y");
					//	if (_tcscmp(choice, L"Y") == 0 || _tcscmp(choice, L"y") == 0)//结束
					//	{
					//		DRAWRAIL = TRUE;
					//	}
					//}
					//if (DRAWRAIL== TRUE)
					//{
					//	if (pSJJX)
					//	{
					//		double XMin = pSJJX->JDarray[0][2], XMax = 0.00, YMin = pSJJX->JDarray[0][1], YMax = 0.00;
					//		for (int i = 0; i < pSJJX->NJD; i++)
					//		{
					//			if (XMin > pSJJX->JDarray[i][2])
					//				XMin = pSJJX->JDarray[i][2];

					//			if (XMax < pSJJX->JDarray[i][2])
					//				XMax = pSJJX->JDarray[i][2];

					//			if (YMin > pSJJX->JDarray[i][1])
					//				YMin = pSJJX->JDarray[i][1];

					//			if (YMax < pSJJX->JDarray[i][1])
					//				YMax = pSJJX->JDarray[i][1];
					//		}
					//		

					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);    
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJJX);
					//	}

					//	if (pSJEX)
					//	{
					//		double XMin = pSJEX->JDarray[0][2], XMax = 0.00, YMin = pSJEX->JDarray[0][1], YMax = 0.00;
					//		for (int i = 0; i < pSJJX->NJD; i++)
					//		{
					//			if (XMin > pSJEX->JDarray[i][2])
					//				XMin = pSJEX->JDarray[i][2];

					//			if (XMax < pSJEX->JDarray[i][2])
					//				XMax = pSJEX->JDarray[i][2];

					//			if (YMin > pSJEX->JDarray[i][1])
					//				YMin = pSJEX->JDarray[i][1];

					//			if (YMax < pSJEX->JDarray[i][1])
					//				YMax = pSJEX->JDarray[i][1];
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJEX);
					//	}

					//	if (pJYJX)
					//	{
					//		double XMin = pJYJX->JDarray[0].E, XMax = 0.00, YMin = pJYJX->JDarray[0].N, YMax = 0.00;
					//		for (int i = 0; i < pJYJX->NJD; i++)
					//		{
					//			if (XMin > pJYJX->JDarray[i].E)
					//				XMin = pJYJX->JDarray[i].E;

					//			if (XMax < pJYJX->JDarray[i].E)
					//				XMax = pJYJX->JDarray[i].E;

					//			if (YMin > pJYJX->JDarray[i].N)
					//				YMin = pJYJX->JDarray[i].N;

					//			if (YMax < pJYJX->JDarray[i].N)
					//				YMax = pJYJX->JDarray[i].N;
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pJYJX);
					//	}

					//	if (pSJEX)
					//	{
					//		double XMin = pSJEX->JDarray[0][2], XMax = 0.00, YMin = pSJEX->JDarray[0][1], YMax = 0.00;
					//		for (int i = 0; i < pSJEX->NJD; i++)
					//		{
					//			if (XMin > pSJEX->JDarray[i][2])
					//				XMin = pSJEX->JDarray[i][2];

					//			if (XMax < pSJEX->JDarray[i][2])
					//				XMax = pSJEX->JDarray[i][2];

					//			if (YMin > pSJEX->JDarray[i][1])
					//				YMin = pSJEX->JDarray[i][1];

					//			if (YMax < pSJEX->JDarray[i][1])
					//				YMax = pSJEX->JDarray[i][1];
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJEX);
					//	}

					//	

					//}
					
				
					pJC->setLayer(L"公路平面设计专用层");
					XianLuID = BAS_DRAW_FUN::AddEntityToDbs(pJC);
					if (XianLuID != NULL)
						XianLuArray_ZDCTZY.push_back(XianLuID);
				}
				//线元模式
				else 
				{
					pJC = new JD_CENTER;
					pJC->XYNum = DBS.XYNum;
					if (pJC->XYNum < 2)
						return;
					if (pJC->XYArray)
						delete[]pJC->XYArray;
					pJC->XYArray = new double[pJC->XYNum + 1][10];
					for (int i = 0; i <= pJC->XYNum; i++)
					{
						for (int j = 0; j < 10; j++)
						{
							pJC->XYArray[i][j] = DBS.XYArray[i][j];
						}

					}
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);
					//	     pJC->XYArrayToJD();
					if (DBS.NDL > 0)
					{
						pJC->setDLB(DBS.DLArray, DBS.NDL);
					}
					/*pJC->DLArray.SetSize(2);
					pJC->setDLM(0, pJC->XYArray[1][6], pJC->XYArray[1][6], L"K", L"K");
					pJC->setDLM(1, pJC->XYArray[pJC->XYNum][6], pJC->XYArray[pJC->XYNum][6],*//* L"K", L"K");*/
					pJC->DLNum = DBS.NDL;
					pJC->IfCalXY = false;
					pJC->SHOWJD = false;
					int DV;
					CString grade;
					pJC->ReadRoadDV(DV, grade);
					pJC->DV = DV;
					pJC->Grade = grade;


					//////////////////////////////////////////////////////////////////////////////
					double PD_array[MAXBPDNUM][4];
					int res = DBS.Read_XLDbs(Path, L"坡度表", L"主线", L"全幅");
					int res1 = DBS.Read_XLDbs(Path, L"纵断链表", L"主线");
					if (res && res1)//读坡度
					{
						int NBPD = DBS.NPD;
						for (int i = 0; i < NBPD; i++)
						{
							PD_array[i][0] = pJC->TYLC1(DBS.PDB[i].ml, DBS.NZDL, DBS.ZDLArray);
							PD_array[i][1] = DBS.PDB[i].Level;
							PD_array[i][2] = DBS.PDB[i].Rshu;
							PD_array[i][3] = 0.0;
						}

						if (DBS.PDB)
						{
							if (DBS.PDB) delete[]DBS.PDB;
							DBS.PDB = NULL;
						}
						if (NBPD > 0)
						{
							pJC->TrsBPDArraytoCM(NBPD, PD_array);
						}
					}

					DBS.ModifyRoadSum = 0;
					DBS.Read_XLDbs(MdfName, L"改移道路表", ProName);
					if (DBS.ModifyRoadSum > 0)
					{
						pJC->m_RelRailName = DBS.pModifyRoad[0].RailName;
						CString RailName = pJC->m_RelRailName;
						if (!pJC->m_RelRailName.IsEmpty())
						{

							CString JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
							CString ProStr = RailName.Mid(0, RailName.GetLength() - 2);
							CString RailMdbName = MdfName;
							RailMdbName.Replace(L"\\改移道路.mdf", L"\\" + ProStr + ".mdb");
							//AfxMessageBox(L"\\改移道路\\DATA\\改移道路.mdf");
							//RailMdbName = RailMdbName + L"\\DATA\\" + ProStr + L".mdb";	//铁路方案路径名
							/*RailMdbName.Replace(L"\\改移道路\\data\\改移道路.mdf", L"\\DATA\\" + ProStr + L".mdb");*/


							bool isSJX = PmPointer::SJXorJYX(RailMdbName, 6, false);
							//RailMdbName+="\\DATA\\" + ProStr + ".mdb";
							if (JXorEX == L"基线")
							{
								if (isSJX)
								{
									/*pSJJX = new GTZX_ROAD;*/
									pSJJX = PmPointer::CreateSJJX(RailMdbName);

									IsInterSect = PmPointer::CalIntersectPt(pJC, pSJJX);

								}
								else
								{
									pJYJX = PmPointer::CreateGTSJJXPointer(RailMdbName);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pJYJX);

								}
							}
							else if (JXorEX == L"左线")
							{
								if (isSJX)
								{
									pSJEX = PmPointer::CreateSJEX(RailMdbName, -1);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pSJEX);

								}

								else
								{
									pJYEX = PmPointer::CreateGTSJEXPointer(RailMdbName);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pJYEX);

								}
							}
							else if (JXorEX == L"右线")
							{
								if (isSJX)
								{
									pSJEX = PmPointer::CreateSJEX(RailMdbName, 1);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pSJEX);

								}
								else
								{
									pJYEX = PmPointer::CreateGTSJEXPointer(RailMdbName);
									IsInterSect = PmPointer::CalIntersectPt(pJC, pJYEX);

								}
							}
							else
								AfxMessageBox(L"ProName对应铁路线路名设置有误");
						}
					}
					//////////////////////////////////////////////////////////////////////////

					acedGetAcadDwgView()->SetFocus();
					//if (i==0)
					//{
					//	ACHAR choice[10];
					//	ads_initget(RSG_OTHER, L"Y y N n");
					//	if (ads_getkword(L"\n是否绘制铁路线路: 是(Y)，或[否(N)] <Y>:  ", choice) == RTNONE)
					//		_tcscpy(choice, L"Y");
					//	if (_tcscmp(choice, L"Y") == 0 || _tcscmp(choice, L"y") == 0)//结束
					//	{
					//		DRAWRAIL = TRUE;
					//	}
					//}
					//if (DRAWRAIL == TRUE)
					//{
					//	if (pSJJX)
					//	{
					//		double XMin = pSJJX->JDarray[0][2], XMax = 0.00, YMin = pSJJX->JDarray[0][1], YMax = 0.00;
					//		for (int i = 0; i < pSJJX->NJD; i++)
					//		{
					//			if (XMin > pSJJX->JDarray[i][2])
					//				XMin = pSJJX->JDarray[i][2];

					//			if (XMax < pSJJX->JDarray[i][2])
					//				XMax = pSJJX->JDarray[i][2];

					//			if (YMin > pSJJX->JDarray[i][1])
					//				YMin = pSJJX->JDarray[i][1];

					//			if (YMax < pSJJX->JDarray[i][1])
					//				YMax = pSJJX->JDarray[i][1];
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJJX);
					//	}

					//	if (pSJEX)
					//	{
					//		double XMin = pSJEX->JDarray[0][2], XMax = 0.00, YMin = pSJEX->JDarray[0][1], YMax = 0.00;
					//		for (int i = 0; i < pSJJX->NJD; i++)
					//		{
					//			if (XMin > pSJEX->JDarray[i][2])
					//				XMin = pSJEX->JDarray[i][2];

					//			if (XMax < pSJEX->JDarray[i][2])
					//				XMax = pSJEX->JDarray[i][2];

					//			if (YMin > pSJEX->JDarray[i][1])
					//				YMin = pSJEX->JDarray[i][1];

					//			if (YMax < pSJEX->JDarray[i][1])
					//				YMax = pSJEX->JDarray[i][1];
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJEX);
					//	}

					//	if (pJYJX)
					//	{
					//		double XMin = pJYJX->JDarray[0].E, XMax = 0.00, YMin = pJYJX->JDarray[0].N, YMax = 0.00;
					//		for (int i = 0; i < pJYJX->NJD; i++)
					//		{
					//			if (XMin > pJYJX->JDarray[i].E)
					//				XMin = pJYJX->JDarray[i].E;

					//			if (XMax < pJYJX->JDarray[i].E)
					//				XMax = pJYJX->JDarray[i].E;

					//			if (YMin > pJYJX->JDarray[i].N)
					//				YMin = pJYJX->JDarray[i].N;

					//			if (YMax < pJYJX->JDarray[i].N)
					//				YMax = pJYJX->JDarray[i].N;
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pJYJX);
					//	}

					//	if (pSJEX)
					//	{
					//		double XMin = pSJEX->JDarray[0][2], XMax = 0.00, YMin = pSJEX->JDarray[0][1], YMax = 0.00;
					//		for (int i = 0; i < pSJEX->NJD; i++)
					//		{
					//			if (XMin > pSJEX->JDarray[i][2])
					//				XMin = pSJEX->JDarray[i][2];

					//			if (XMax < pSJEX->JDarray[i][2])
					//				XMax = pSJEX->JDarray[i][2];

					//			if (YMin > pSJEX->JDarray[i][1])
					//				YMin = pSJEX->JDarray[i][1];

					//			if (YMax < pSJEX->JDarray[i][1])
					//				YMax = pSJEX->JDarray[i][1];
					//		}


					//		///*pSJJX = NULL;*/

					//		BAS_DRAW_FUN_JYX MZOOM;
					//		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
					//		TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJEX);
					//	}



					//}


					pJC->setLayer(L"公路平面设计专用层");
					XianLuID = BAS_DRAW_FUN::AddEntityToDbs(pJC);
					if (XianLuID != NULL)
						XianLuArray_ZDCTZY.push_back(XianLuID);
					BAS_DRAW_FUN::AddEntityToDbs(pJC);
				}

				/*else
				{
				DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);
				if(DBS.XYNum>0)
				{
				pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
										pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				pJC->setRoadName(DBS.Road[i].RoadName);
				pJC->setMdbName(Path);
				if(DBS.NDL>0)
				{
				pJC->setDLB(DBS.DLArray,DBS.NDL);
				}
				pJC->setLayer(L"公路平面设计专用层");
				BAS_DRAW_FUN::AddEntityToDbs(pJC);
				}
				}*/
			}
			DBS.Release();
		}
	}

}
// This is command 'JCADDDL'
//增加断链
void penglihuiJCADDDL()
{
	// TODO: Implement the command
	int rc;
	ads_point PT;
	ads_name en;
	ACHAR choice[5];
	ACHAR mes[256];
	int iDL = 0;
	CString GH,ckml,Eckml,Bckml;

	rc = ads_entsel(L"\n请选择平面设计线实体 ", en, PT);
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	acdbOpenObject(pObj, eId, AcDb::kForRead);
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
	pObj->close();
	if(pObj->isKindOf(JD_CENTER::desc())) 
	{  

		JD_CENTER *pzLine=JD_CENTER::cast(pObj);

		double TYLC = pzLine->PROJ_ML(PT[Y],PT[X]);
		double BLC = pzLine->XLC(TYLC,GH);
		ckml=pzLine->LCchr(GH,BLC,3);


		ads_initget(RSG_OTHER,L"P p M m");

		rc = ads_getkword(L"\n请输入断前里程: 采点(P) / 里程 (M): <P>  ", choice);
		if (rc == RTNONE)
			_tcscpy(choice ,L"P");
		if (rc == RTCAN)
			return;

		if(_tcscmp(choice ,L"P")==0 || _tcscmp(choice ,L"p")==0)//结束
		{
			ads_getpoint(NULL,L"\n请在平面设计线实体上采集断前里程: ",PT);  

			TYLC = pzLine->PROJ_ML(PT[Y],PT[X]);
			BLC = pzLine->XLC(TYLC,GH);
		}
		else 
		{
			ads_initget(1+2+4,NULL);
			_stprintf(mes,L"\n请输入断前现场里程 = <%s>    ",ckml);
			ACHAR tmp[256];
			acedGetString(0,mes,tmp);
			Bckml=tmp;
			if(Bckml=="")Bckml=ckml;
			pzLine->split_ckml(Bckml, &BLC, GH);
			//			  TYLC=pzLine->TYLC(Bckml);
			TYLC=pzLine->TYLC(BLC);
		}

		ckml=pzLine->LCchr(GH,BLC,3);

		double Exclc;

		ads_initget(1+2+4,NULL);

		_stprintf(mes,L"\n请输入断后现场里程 = <%s>    ",ckml);
		ACHAR tmp[256];
		if(RTNORM==acedGetString(0,mes,tmp))
		{
			Eckml=tmp;
			if(Eckml=="")Eckml=ckml;
			pzLine->split_ckml(Eckml, &Exclc, GH);
			/*acdbOpenObject(pObj, eId, AcDb::kForWrite);*/
			if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pzLine=JD_CENTER::cast(pObj);
			pzLine->addDLM(pzLine->GetAfterDL(TYLC),BLC,Exclc,GH);
			pzLine->close();  // Finished with the pline header.     
		}         
	}
	else
	{
		acutPrintf(L"所选不为线路实体!\n");
	}
}

//增加交点
void penglihuiJCADDJD()
{
	AcDbObjectId NewAddOneJDGTZXeId;
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	BAS_DRAW_FUN OB;
	int JDi;
	ACHAR JDNumTemp[20];
	CArray<BAS_DRAW_FUN::JdCenter,BAS_DRAW_FUN::JdCenter>TempJDarray; 


	rc = ads_entsel(L"选取一个线路: \n", en, asDblArray(PT));

	if (rc != RTNORM) 
	{    
		ads_printf(L"Error during object selection\n");  
		return;   
	} 

	acdbGetObjectId(NewAddOneJDGTZXeId, en);

	int iRXD = -1;
	AcDbObject *pObj;
	if(acdbOpenObject(pObj, NewAddOneJDGTZXeId, AcDb::kForRead)==Acad::eOk)
	{

		JD_CENTER  *pzLine = NULL;
		if(pObj->isKindOf(JD_CENTER::desc())) 
		{  
			pObj->close(); 
			pzLine=JD_CENTER::cast(pObj);

		}
		else
		{
			ads_printf(L"请选择线路实体！\n");
			return;
		}

		TempJDarray.RemoveAll();
		int i;
		for( i=0;i<pzLine->JdCenArray.GetSize();i++)
		{
			TempJDarray.Add(pzLine->JdCenArray[i]);
		}

		int  NJD,n,bz=-99;
		double R,L1,L2,NN1,EE1,NN2,EE2,dtemp,dd,ss;

		NJD=pzLine->getJD_RL(0,&R,&L1,&L2,&NN1,&EE1);
		JDi=NJD;
		i=1;
		do 
		{	
			dd=sqrt((PT[X]-EE1)*(PT[X]-EE1)+(PT[Y]-NN1)*(PT[Y]-NN1));

			n=pzLine->getJD_RL(i,&R,&L1,&L2,&NN2,&EE2);

			dtemp=sqrt((PT[X]-EE2)*(PT[X]-EE2)+(PT[Y]-NN2)*(PT[Y]-NN2));

			ss=sqrt((NN1-NN2)*(NN1-NN2)+(EE1-EE2)*(EE1-EE2));

			//	dd/dtemp;  //选取点到（起点）某顶点的距离(平方）
			if (fabs(ss-dd-dtemp)<0.5)  
			{ 
				JDi=i;
				bz=99;
			} 
			else 
			{ 
				NN1=NN2;
				EE1=EE2; 
			};

			i=i+1;
			if (i>=NJD)bz=99;
		}while (bz<0); 

		//橡皮线
		//橡皮筋效果
		struct resbuf *result ;
		int track,type ;
		ads_point cpt,pt;
		result = ads_buildlist(RTSTR,L"",0);//创建结果缓冲区链表
		track = 1;
		type = 5;
		ads_point spt,ept;

		//前一点
		spt[X]= TempJDarray[JDi-1].E;
		spt[Y]= TempJDarray[JDi-1].N;

		//后一个点
		ept[X]=TempJDarray[JDi].E;
		ept[Y]=TempJDarray[JDi].N;

		//当前点
		cpt[X]=PT[X];
		cpt[Y]=PT[Y];

		//ads_grdraw(spt ,cpt,-1,0);//在当前端口画一矢量
		//ads_grdraw(ept ,cpt,-1,0);
		ads_point OriSpt,OriEpt;
		OriSpt[X] = spt[X];
		OriSpt[Y] = spt[Y];
		OriEpt[X] = ept[X];
		OriEpt[Y] = ept[Y];

		//////////////////////////////////////////////////////////////////////////
		double Lo1,Lo2,RQ,RH,pQ,tRQ,pH,tRH,A1;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径
		double CenNQ,CenEQ,CenNH,CenEH;
		double N1,E1,N0,E0,N2,E2;
		double TN,TE;//切点
		//int JDTDBZ=BaseSettting::JDTDFS = 0;
		int JDTDBZ=BaseSettting::JDTDFS;
		if (JDi>1 && (JDTDBZ==2 || JDTDBZ==4))//前切圆
		{
			Lo1 = TempJDarray[JDi-1].Ls1;
			Lo2 = TempJDarray[JDi-1].Ls2;
			RQ = TempJDarray[JDi-1].R;
			//求前圆圆心
			OB.GetCenter3JD(TempJDarray[JDi-2].N,TempJDarray[JDi-2].E,TempJDarray[JDi-1].N,TempJDarray[JDi-1].E,TempJDarray[JDi].N,TempJDarray[JDi].E,RQ,Lo1,Lo2,CenNQ,CenEQ);
			if(Lo1>0.0)
			{
				A1=sqrt(RQ*Lo1);
				double x=OB.spiral_y(A1,Lo1);
				double t=Lo1/(2.0*RQ);
				double dr=x+RQ*sin(t+0.5*PI);			
				pQ=dr-RQ;
			}
			else pQ=0.0;					
			//半径+内移量					
			tRQ=RQ+pQ;
		}

		if (JDi<pzLine->JdCenNum-1 && (JDTDBZ==3 || JDTDBZ==4))//后切圆
		{
			Lo1 = TempJDarray[JDi].Ls1;
			Lo2 = TempJDarray[JDi].Ls2;
			RH = TempJDarray[JDi].R;
			//求前圆圆心
			OB.GetCenter3JD(TempJDarray[JDi-1].N,TempJDarray[JDi-1].E,TempJDarray[JDi].N,TempJDarray[JDi].E,TempJDarray[JDi+1].N,TempJDarray[JDi+1].E,RH,Lo1,Lo2,CenNH,CenEH);
			if(Lo1>0.0)
			{
				A1=sqrt(RH*Lo1);
				double x=OB.spiral_y(A1,Lo1);
				double t=Lo1/(2.0*RH);
				double dr=x+RH*sin(t+0.5*PI);			
				pH=dr-RH;
			}
			else pH=0.0;					
			//半径+内移量	
			tRH=RH+pH;
		}
		//////////////////////////////////////////////////////////////////////////

		while ( type != 3  )//鼠标选择了一点后退出循环
		{
			if(ads_grread(track,&type,result )==RTCAN)//读取输入设备,RTCAN即按下了ESC键
				return;                               //track=1:以拖曳方式
			pt[X]=result->resval.rpoint[X];           //type=5:与track=1对应
			pt[Y]=result->resval.rpoint[Y];		      //result存当前瞄准器坐标

			if(pt[X]!=cpt[X] || pt[Y]!=cpt[Y])//鼠标离开了原来的点
			{
				//////////////////////////////////////////////////////////////////////////
				//如果有前后切圆拖动要求，需要计算spt,ept
				if (JDTDBZ==2 || JDTDBZ==3 || JDTDBZ==4)
				{
					if (JDi>1 && (JDTDBZ==2 || JDTDBZ==4))//前切圆
					{
						//判断点是否在圆内部
						double DistToCen = 0.0;
						DistToCen = sqrt((CenNQ-pt[Y])*(CenNQ-pt[Y]) + (CenEQ-pt[X])*(CenEQ-pt[X]));
						if (DistToCen>tRQ)
						{
							N1=TempJDarray[JDi-2].N;
							E1=TempJDarray[JDi-2].E;
							N0=spt[Y];
							E0=spt[X];
							N2=cpt[Y];
							E2=cpt[X];
							//求切点
							int LRFlag=OB.GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
							OB.GetQD(CenNQ,CenEQ,tRQ,pt[Y],pt[X],LRFlag,TN,TE);
							OB.INTERS(&spt[Y],&spt[X],N1,E1,N0,E0,TN,TE,pt[Y],pt[X]);
						}
						else
						{
							ads_printf(L"交点在前一交点圆曲线内，无法沿前切圆拖动！\n");
							pt[Y] = cpt[Y];
							pt[X] = cpt[X];
						}
					}
					if (JDi<pzLine->JdCenNum-1 && (JDTDBZ==3 || JDTDBZ==4))//后切圆
					{
						double DistToCen = 0.0;
						DistToCen = sqrt((CenNH-pt[Y])*(CenNH-pt[Y]) + (CenEH-pt[X])*(CenEH-pt[X]));
						if (DistToCen>tRH)
						{
							N1=cpt[Y];
							E1=cpt[X];
							N0=ept[Y];
							E0=ept[X];
							N2=TempJDarray[JDi+1].N;
							E2=TempJDarray[JDi+1].E;
							//求切点
							int LRFlag=OB.GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
							OB.GetQD(CenNH,CenEH,tRH,pt[Y],pt[X],-1*LRFlag,TN,TE);
							OB.INTERS(&ept[Y],&ept[X],N2,E2,N0,E0,TN,TE,pt[Y],pt[X]);
						}
						else
						{
							ads_printf(L"交点在后交点圆曲线内，无法沿后切圆拖动！\n");
							pt[Y] = cpt[Y];
							pt[X] = cpt[X];
						}
					}		
				}
				ads_grdraw(OriSpt ,cpt,-1,0);
				ads_grdraw(OriEpt ,cpt,-1,0);
				ads_grdraw(spt ,pt,-1,0);
				ads_grdraw(ept ,pt,-1,0);
			}
			cpt[X] = pt[X] , cpt[Y]=pt[Y];
			OriSpt[X] = spt[X];
			OriSpt[Y] = spt[Y];
			OriEpt[X] = ept[X];
			OriEpt[Y] = ept[Y];
		}
		PT1[X]=pt[X], PT1[Y]=pt[Y];

		ACHAR mes[256],choice[30];
		// //		_stprintf(mes,L"请输入交点号 = <推荐交点号: %s-1>",TempJDarray[JDi].JDNum);
		// 
		// 		ads_initget(RSG_OTHER,L"Y y N n");
		// 		rc = ads_getkword(mes, choice);
		// 		if (rc==RTCAN)
		// 		{
		// 			return;
		// 		}
		// 		else if(rc==RTNONE)
		// 		{
		// //			_stprintf(JDNumTemp,L"%s-1",TempJDarray[JDi].JDNum);
		// 		}
		// 		else
		// 		{
		// 			_tcscpy( JDNumTemp , choice);
		// 		}

		ads_initget(RSG_OTHER,L"Y y N n");
		// 		rc = ads_getkword(L"\n是否后续交点号顺次加一: 是(Y) / 否(N): <N>  ", choice);
		// 		if (rc==RTCAN)
		// 			return;
		// 		if(rc==RTNONE)
		// 			_tcscpy(choice ,L"N");

		if (JDi>0 && JDi<NJD)
		{
			//修改后续交点号
			// 输入交点号

			//	_stprintf(mes,L"\n  输入交点号",JDNumTemp);
			ads_printf(L"\n\n 请输入 JDi=%d 该交点的半径R和缓长",JDi);

			pzLine->RtjMin = 0;
			_stprintf(mes,L"\n  输入半径 R = ");
			ads_initget(2+4 ,NULL);
			rc = ads_getreal(mes,&R);
			if (rc==RTCAN)
				return;
			if(rc==RTNONE)
				R = pzLine->RtjMin;
			double l0;
			l0 = 0.0;
			_stprintf(mes,L"\n  输入缓长 Lo = ");		     
			ads_initget(4 ,NULL);
			rc = ads_getreal(mes,&L1);
			if (rc==RTCAN)
				return;
			if(rc==RTNONE)
				L1 = l0;
			L2 = L1;	

			//增加交点数组
			// 			TempJDarray[JDi-1].N = spt[Y];
			// 			TempJDarray[JDi-1].E = spt[X];
			// 			TempJDarray[JDi].N = ept[Y];
			// 			TempJDarray[JDi].E = ept[X];
			BAS_DRAW_FUN::JdCenter OneJDTemp;
			//_tcscpy( OneJDTemp.JDNum , JDNumTemp);
			OneJDTemp.R = R;
			OneJDTemp.Ls1 = L1;
			OneJDTemp.Ls2 = L2;
			OneJDTemp.A1 = sqrt(R*L1);
			OneJDTemp.A2 = sqrt(R*L2);
			OneJDTemp.N = PT1[Y];
			OneJDTemp.E = PT1[X];
			OneJDTemp.JDN = PT1[Y];
			OneJDTemp.JDE = PT1[X];
			OneJDTemp.CenN = PT1[Y];
			OneJDTemp.CenE = PT1[X];
			OneJDTemp.JDXZ = 0;
			OneJDTemp.IsOrNotHuiTou=0;
			OneJDTemp.LinkAfter = NotRelation;
			TempJDarray.InsertAt(JDi,OneJDTemp,1);

		}
	}
	TempJDarray[0].Ls1=TempJDarray[0].Ls2=0;
	TempJDarray[TempJDarray.GetSize()-1].Ls1=TempJDarray[TempJDarray.GetSize()-1].Ls2=0;
	BAS_DRAW_FUN::JdCenter *JCArray=new BAS_DRAW_FUN::JdCenter[TempJDarray.GetSize()];
	if(acdbOpenObject(pObj, NewAddOneJDGTZXeId, AcDb::kForWrite)==Acad::eOk)
	{
		if(pObj->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER  *pzLinep = JD_CENTER::cast(pObj);

			pzLinep->assertWriteEnabled();
			pzLinep->JdCenArray.RemoveAll();


			for(int i=0;i<TempJDarray.GetSize();i++)
			{
				JCArray[i]=TempJDarray[i];
			}
			pzLinep->SetJdCenterArray(JCArray,TempJDarray.GetSize());
			//pzLinep->JdCenNum = TempJDarray.GetSize();

			//pzLinep->CalXYArray();

			pzLinep->close();

			return;
		}
	}
}
void penglihuiJCEDITJD()
{
}
//删除交点
void penglihuiJCDELJD()
{
	AcDbObjectId NewAddOneJDGTZXeId;
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	BAS_DRAW_FUN OB;
	int JDi=0;
	ACHAR JDNumTemp[20];
	CArray<BAS_DRAW_FUN::JdCenter,BAS_DRAW_FUN::JdCenter>TempJDarray; 


	rc = ads_entsel(L"选取一个线路: \n", en, asDblArray(PT));

	if (rc != RTNORM) 
	{    
		ads_printf(L"Error during object selection\n");  
		return;   
	} 

	acdbGetObjectId(NewAddOneJDGTZXeId, en);

	int iRXD = -1;
	AcDbObject *pObj;
	if(acdbOpenObject(pObj, NewAddOneJDGTZXeId, AcDb::kForRead)==Acad::eOk)
	{

		JD_CENTER  *pzLine = NULL;
		if(pObj->isKindOf(JD_CENTER::desc())) 
		{  
			pObj->close(); 
			pzLine=JD_CENTER::cast(pObj);
			if (pzLine->JdCenNum<3)
			{
				AfxMessageBox(L"交点数目小于3个，不能删除！");
				return;
			}
		}
		else
		{
			ads_printf(L"请选择线路实体！\n");
			return;
		}
		int  NJD,n,bz=-99;
		double R,L1,L2,NN1,EE1,NN2,EE2,dtemp,dd,ss;

		NJD=pzLine->getJD_RL(0,&R,&L1,&L2,&NN1,&EE1);
		JDi=NJD;
		int i=1;
		double dis1,dis2;
		i=pzLine->GetPreJD(PT[Y],PT[X]);
		dis1=sqrt((PT[X]-pzLine->JdCenArray[i].E)*(PT[X]-pzLine->JdCenArray[i].E)+(PT[Y]-pzLine->JdCenArray[i].N)*(PT[Y]-pzLine->JdCenArray[i].N));
		dis2=sqrt((PT[X]-pzLine->JdCenArray[i+1].E)*(PT[X]-pzLine->JdCenArray[i+1].E)+(PT[Y]-pzLine->JdCenArray[i+1].N)*(PT[Y]-pzLine->JdCenArray[i+1].N));
		if(dis1<dis2)
			JDi=i;
		else
			JDi=i+1;
		//do 
		//{	
		//	dd=sqrt((PT[X]-EE1)*(PT[X]-EE1)+(PT[Y]-NN1)*(PT[Y]-NN1));

		//	n=pzLine->getJD_RL(i,&R,&L1,&L2,&NN2,&EE2);

		//	dtemp=sqrt((PT[X]-EE2)*(PT[X]-EE2)+(PT[Y]-NN2)*(PT[Y]-NN2));

		//	ss=sqrt((NN1-NN2)*(NN1-NN2)+(EE1-EE2)*(EE1-EE2));

		//	//	dd/dtemp;  //选取点到（起点）某顶点的距离(平方）
		//	if (fabs(ss-dd-dtemp)<0.5)  
		//	{ 
		//		JDi=i;
		//		bz=99;
		//	} 
		//	else 
		//	{ 
		//		NN1=NN2;
		//		EE1=EE2; 
		//	};

		//	i=i+1;
		//	if (i>=NJD)bz=99;
		//}while (bz<0); 
		TempJDarray.RemoveAll();
		for(int i=0;i<pzLine->JdCenArray.GetSize();i++)
		{
			if (i!=JDi)
			{
				TempJDarray.Add(pzLine->JdCenArray[i]);
			}
		}

	}
	TempJDarray[0].Ls1=TempJDarray[0].Ls2=0;
	TempJDarray[TempJDarray.GetSize()-1].Ls1=TempJDarray[TempJDarray.GetSize()-1].Ls2=0;
	BAS_DRAW_FUN::JdCenter *JCArray=new BAS_DRAW_FUN::JdCenter[TempJDarray.GetSize()];
	if(acdbOpenObject(pObj, NewAddOneJDGTZXeId, AcDb::kForWrite)==Acad::eOk)
	{
		if(pObj->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER  *pzLinep = JD_CENTER::cast(pObj);

			pzLinep->assertWriteEnabled();
			pzLinep->JdCenArray.RemoveAll();


			for(int i=0;i<TempJDarray.GetSize();i++)
			{
				JCArray[i]=TempJDarray[i];
			}
			pzLinep->SetJdCenterArray(JCArray,TempJDarray.GetSize());
			//pzLinep->JdCenNum = TempJDarray.GetSize();

			//pzLinep->CalXYArray();

			pzLinep->close();

			return;
		}
	}

}
// This is command 'JCDELDL'
//删除断链
void penglihuiJCDELDL()
{
	// TODO: Implement the command
	int rc;
	ads_point PT;
	ads_name en;

	rc = ads_entsel(L"\n请在图上待删除断链处采点 : ", en, PT);
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();		
	if(pObj->isKindOf(JD_CENTER::desc())) 
	{ 
		JD_CENTER  *pzLine=JD_CENTER::cast(pObj);
		int iDL=-1;		
		double TYLC = pzLine->PROJ_ML(PT[Y],PT[X]);
		double MinL = 99999999999;
		for(int i=1; i<pzLine->DLNum; i++)
		{
			double deltaL = fabs(pzLine->DLArray[i].TYLC - TYLC);
			if(deltaL<MinL)
			{
				MinL = deltaL; 
				iDL = i;
			}
		}		
		/*acdbOpenObject(pObj, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pzLine=JD_CENTER::cast(pObj);
		if(iDL<pzLine->DLNum && iDL>0)pzLine->DelDLM(iDL);
		pzLine->close();  // Finished with the pline header.     		
	}
	else
	{
		acutPrintf(L"所选不为线路实体!\n");
	}
}


#include "LocationDlg.h"

LocationDlg Locdlg;
bool IsFirstLocdlg=true;
//屏幕定线
void pPMLOC()
{
	if(!IsFirstLocdlg)
	{
		Locdlg.ShowWindow(TRUE);
	}
	else
	{
		IsFirstLocdlg=false;
		Locdlg.Create(IDD_DIALOG_LOCATION2,acedGetAcadFrame());
		Locdlg.ShowWindow(SW_SHOW);
	}	
}

/*
#include "EXSJDlg.h"
EXSJDlg Exsjdlg;
bool IsFirstExsjdlg=true;*/


// This is command 'JCEXSJ'//分离段设计
void penglihuiJCEXSJ()
{
	// TODO: Implement the command
	AcDbObjectIdArray IdArr;
	BAS_DRAW_FUN::GetJD_CENTEREntity(IdArr);
	int num=IdArr.length();
	EXMANAGER *pEXMan=new EXMANAGER;	
	for(int i=0;i<num;i++)
	{
		JD_CENTER *pJC;	
		if(Acad::eOk==acdbOpenObject(pJC,IdArr[i],AcDb::kForWrite))
		{
			if(pJC->FLDNo>0)
			{
				pEXMan->FLDArray.Add(pJC);
				pEXMan->FLDNum++;
				pJC->erase();
			}
		}
		pJC->close();
	}
	acutPrintf(L"\n共有%d个分离段!\n",pEXMan->FLDNum);
	if(pEXMan->FLDNum>0)
	{
		//pEXMan->SortFLDArray();
		pEXMan->setLayer(L"公路平面设计专用层");
		BAS_DRAW_FUN::AddEntityToDbs(pEXMan);
	}
	else
	{
		delete pEXMan;
	}

	/*
	if(!IsFirstExsjdlg)
	{
	Exsjdlg.ShowWindow(TRUE);
	}
	else
	{
	IsFirstExsjdlg=false;
	Exsjdlg.Create(IDD_DIALOG_EXSJ,acedGetAcadFrame());
	Exsjdlg.ShowWindow(SW_SHOW);
	}	*/

}

// This is command 'JCPROJPOINT'
void penglihuiJCPROJPOINT()
{
	// TODO: Implement the command
	int rc;
	ads_point PT;
	ads_name en;
	rc = ads_entsel(L"\n请选择主线: ", en, PT);
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	ACHAR choice[256];
	_tcscpy(choice,L"N");	
	double xlc;
	CString GH;
	AcDbObjectId eId,lId,QXlId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();		
	if(pObj->isKindOf(JD_CENTER::desc())) 
	{ 
		JD_CENTER  *pJC=JD_CENTER::cast(pObj);	
		BAS_DRAW_FUN::xlpoint PZ;
		PZ.lc=pJC->PROJ_ML(PT[Y],PT[X]);
		pJC->xlpoint_pz(&PZ);
		AcGePoint3d sPt,ePt;
		AcGePoint3d QXsPt,QXePt;
		sPt.set(PT[X],PT[Y],0.0);
		ePt.set(PZ.y,PZ.x,0.0);
		QXsPt.x=ePt.x+500.0*sin(PZ.a);
		QXsPt.y=ePt.y+500.0*cos(PZ.a);
		QXePt.x=ePt.x-500.0*sin(PZ.a);
		QXePt.y=ePt.y-500.0*cos(PZ.a);
		AcDbLine *Line=new AcDbLine(sPt,ePt);
		lId=BAS_DRAW_FUN::AddEntityToDbs(Line);
		AcDbLine *QXLine=new AcDbLine(QXsPt,QXePt);
		QXlId=BAS_DRAW_FUN::AddEntityToDbs(QXLine);

		AcGePoint3d pk;
		int type;
		struct resbuf result;
		short KEYCODE;
		acutPrintf(L"/////////S键>>设置!/////////\n");
		for(;;)
		{
			if(ads_grread(1 , &type , &result)!=RTNORM)
			{ 
				break;
			}
			else 
			{
				KEYCODE=result.resval.rint;
				//		if(KEYCODE>=0)acutPrintf(L"%d\n",KEYCODE);

				if(KEYCODE==83 || KEYCODE==115)//S
				{
					ads_initget(RSG_OTHER,L"N n H h T t S s");
					if(ads_getkword(L"\n任意点(N)/整百米(H)/整十米(T)/曲线特征点(S): <N>", choice)==RTNONE)
						_tcscpy(choice ,L"N");	 
				}

				if(KEYCODE==13 || KEYCODE==32)//回车、空格
				{
					break;
				}
				else{}
			}

			if(type == 3)break;
			else if(type==25)//右键
			{
				break;
			}
			else if(type == 5)
			{
				pk.x=result.resval.rpoint[X];
				pk.y=result.resval.rpoint[Y];
				pk.z=result.resval.rpoint[Z];

				PZ.lc=pJC->PROJ_ML(pk.y,pk.x);
				xlc=pJC->XLC(PZ.lc,GH);

				if(_tcscmp(choice,L"H")==0 || _tcscmp(choice,L"h")==0)
				{
				}
				if(_tcscmp(choice,L"T")==0 || _tcscmp(choice,L"t")==0)
				{
				}
				if(_tcscmp(choice,L"S")==0 || _tcscmp(choice,L"s")==0)
				{
				}
				pJC->xlpoint_pz(&PZ);
				acutPrintf(L"里程: %lf, 角度: %lf\n",xlc,PZ.a);
				sPt.set(pk[X],pk[Y],0.0);
				ePt.set(PZ.y,PZ.x,0.0);
				QXsPt.x=ePt.x+500.0*sin(PZ.a);
				QXsPt.y=ePt.y+500.0*cos(PZ.a);
				QXePt.x=ePt.x-500.0*sin(PZ.a);
				QXePt.y=ePt.y-500.0*cos(PZ.a);

				if(Acad::eOk==acdbOpenObject(Line,lId,AcDb::kForWrite))
				{
					Line->setStartPoint(sPt);
					Line->setEndPoint(ePt);
					Line->close();
				}
				if(Acad::eOk==acdbOpenObject(QXLine,QXlId,AcDb::kForWrite))
				{
					QXLine->setStartPoint(QXsPt);
					QXLine->setEndPoint(QXePt);
					QXLine->close();
				}
			}
		}			
	}
	else
	{
		acutPrintf(L"所选不为主线路实体!\n");
	}
}
#include "xlsj.h"
// This is command 'JCADDXL'

XLSJ xlsjDlg;
void penglihuiJCADDXL()
{
	// TODO: Implement the command
	xlsjDlg.Create(IDD_addJD,acedGetAcadFrame());
	xlsjDlg.ShowWindow(SW_SHOW);
}

// This is command 'JCDELFLD'
void penglihuiJCDELFLD()
{
	// TODO: Implement the command
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;

	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);

	/*acdbOpenObject(pEnt,eId,AcDb::kForRead);*/
	if(acdbOpenObject(pEnt,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pEnt->close();
	if(pEnt->isKindOf(EXMANAGER::desc()))
	{
		EXMANAGER *pEX=EXMANAGER::cast(pEnt);
		/*acdbOpenObject(pEX,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pEX,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pEX->DeliFLD(pEX->GetiFLDFromNE(pt[Y],pt[X]));
		pEX->close();
	}
}
#include "DLBdlg.h"
// This is command 'JCDLB'
//编辑断链
void penglihuiJCDLB()
{
	// TODO: Implement the command
	DLBdlg DLBObj;
	DLBObj.DoModal();
}

// This is command 'JCDLB_Road'
void JCZMDLB()
{
	// TODO: Implement the command
	DLBdlg DLBObj;
	DLBObj.DoModal();
}

#include "JDTDSETTINGDLG.h"
// This is command 'JCJDTDFS'
void penglihuiJCJDTDFS()
{
	// TODO: Implement the command
	JDTDSETTINGDLG JDTDDlg;
	JDTDDlg.DoModal();
}
#include "MyLayout.h"
// This is command 'JCAUTOPM'	分幅输出
#include "PmCutDlg.h"
void penglihuiJCAUTOPM()
{
	// TODO: Implement the command
	//关闭捕捉对象
	BAS_DRAW_FUN::Set_OSMODE(0);
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
	acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);	//命令行提示关
	ads_command(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);	//十字架模式关
	ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);	//设置坐标系
#endif // VERSION_CAD2010

	//检测平面
	if (pzLinep == NULL)
	{
		ads_alert(L"请先绘制平面图，再切图!");
		return;
	}

	//读取平面模板
	CWorkDir WordDir;
	FILE *fp=NULL;
	fp=_wfopen(WordDir.PmDrawSetFile,L"r");
	if (fp == NULL)
	{
		CString PmDrawSetFile = WordDir.DataPath + "模板.PmDrawSet";
		fp = _wfopen(PmDrawSetFile,L"r");
	}
	if(fp)
	{	
		CString m_XLName,m_StartLc,m_EndLc,m_JDGH;
		int m_NLC,m_Scale,StartNo;
		double m_Texth,LWide,RWide;
		ACHAR ch[256];
		fwscanf(fp,L"%s",ch);m_XLName=ch;
		fwscanf(fp,L"%s",ch);m_StartLc=ch;
		fwscanf(fp,L"%s",ch);m_EndLc=ch;
		fwscanf(fp,L"%d",&m_Scale);
		fwscanf(fp,L"%d",&m_NLC);	
		fwscanf(fp,L"%lf",&m_Texth);		
		fclose(fp);

		CTrimTuSettingDialog TrimDlg;

		double K = m_Scale / 1000.0;
		TrimDlg.m_Length *= K;
		if (TrimDlg.DoModal() != IDOK)
			return;
		if(WordDir.ReadOk)
		{			
			pzLinep = TrimDlg.pm;
			m_StartLc = TrimDlg.m_Sdml;
			m_EndLc = TrimDlg.m_Edml;			
			StartNo = TrimDlg.m_StartNo;
			double TKLen = TrimDlg.m_Length;

			double sml, eml;
			sml = pzLinep->TYLC(m_StartLc);	//起始里程
			eml = pzLinep->TYLC(m_EndLc);	//终止里程

			int TukuangNum;
			TukuangNum = int(fabs(eml - sml) / TKLen + 50);
			STuKuang *TK = new STuKuang[TukuangNum];
			int TkNum;
			pzLinep->CalTK(_wtof(m_StartLc), _wtof(m_EndLc), TKLen, TrimDlg.m_Frame_Height, TrimDlg.m_Frame_Width, TK, TkNum);

			if(TrimDlg.m_TrimFlag==0)//打断方式
			{
				ads_point OriPt,Lfup,Lfbt,Rtup,Rtbt;
				AcGePoint3d spt,ept;
				ads_point Hpt,Tpt;

				PmCutDlg pmdlg;
				pmdlg.pm = pzLinep;
				if(pmdlg.DoModal()!=IDOK)
					return;

				if(TrimDlg.m_OutName=="")
					TrimDlg.m_OutName = "平面图";

				OriPt[X] = _wtof(pmdlg.m_Xori);
				OriPt[Y] = _wtof(pmdlg.m_Yori);
				pzLinep->TransFormTK(TK, TkNum, OriPt[X] + 780 / 2.0, OriPt[Y] + 554.0 / 2.0, K);
				ACHAR dwgPath[80], bkname[256], filename[256];
				_tcscpy(dwgPath, TrimDlg.m_TuKuangFile);
				AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"平面图框");
				_tcscpy(filename,WordDir.m_ZBZFile);
				AcDbObjectId pBlockId1 = BAS_DRAW_FUN::CreateBlockFromDwgFile(filename, L"指北针");

				CString pszLabel = "正在分幅平面图";
				acedSetStatusBarProgressMeter(pszLabel, 0, 100);

				for (int iTK = 0; iTK < TkNum; iTK++)
				{
					int pos = (int)iTK*100.0 / TkNum;
					acedSetStatusBarProgressMeterPos(pos);											
					STuKuang *CurTK = &TK[iTK];

					//ads_printf(L"共%d幅图,正在输出第%d幅图(%s~%s)到%s%d.dwg文件中\n",TkNum,iTK+1,CurTK->StartXCLC,CurTK->EndXCLC,TrimDlg.m_OutName,StartNo);
					//double dX = iTK*(1.5 * 840 * K);
					double dX = iTK * 1000.0;
					CurTK->newCenX += dX;

					AcGeMatrix3d xformT;
					xformT.setToTranslation(AcGeVector3d(dX, 0.0, 0.0));

					AcGePoint3d p1,p2,p3,p4;
					p1[X] = CurTK->LtUpX;
					p1[Y] = CurTK->LtUpY;

					p2[X] = CurTK->RtUpX;
					p2[Y] = CurTK->RtUpY;

					p3[X] = CurTK->RtBmX;
					p3[Y] = CurTK->RtBmY;

					p4[X] = CurTK->LtBmX;
					p4[Y] = CurTK->LtBmY;

					ads_point pt1,pt2,pt3,pt4;
					ads_point bkpt1,bkpt2,bkpt3,bkpt4;
					bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
					bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
					bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
					bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;

					int result;
					BAS_DRAW_FUN::CalFrame(-2.0*m_Texth, p1, p2, p3, p4, pt1, pt2, pt3, pt4);

					
					result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4);
					ads_printf(L"第%d幅图切图完成.\n", iTK + 1);

					if(result > 0)
					{
						BAS_DRAW_FUN::CalFrame(0.01, p1, p2, p3, p4, bkpt1, bkpt2, bkpt3, bkpt4);
						result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4, result);
					}

					//把框内的所有实体复制转移到指定
					BAS_DRAW_FUN::AutoEntTransform(CurTK->xform, xformT, pt1, pt2, pt3, pt4);

					//pt1[X] = OriPt[X] + 
					//暂时不绘制
					////左边竖线
					//spt.x = CurTK->newLtBmX + dX; spt.y = CurTK->newLtBmY;
					//ept.x = CurTK->newLtUpX + dX; ept.y = CurTK->newLtUpY;
					//BAS_DRAW_FUN::makeline(spt, ept, 4, 0);
					////起始里程文字
					//Hpt[X] = spt.x, Hpt[Y] = spt.y;
					//Tpt[X] = ept.x, Tpt[Y] = ept.y;
					//double ang = ads_angle(Hpt, Tpt);
					//spt.x = CurTK->newStX + dX; spt.y = CurTK->newStY;
					//_tcscpy(ch, CurTK->StartXCLC);
					//BAS_DRAW_FUN::maketext(spt, ch, ang, m_Texth*K, 4, 3);
					////右边竖线
					//spt.x = CurTK->newRtBmX + dX; spt.y = CurTK->newRtBmY;
					//ept.x = CurTK->newRtUpX + dX; ept.y = CurTK->newRtUpY;
					//BAS_DRAW_FUN::makeline(spt, ept, 4, 0);
					////终止里程文字
					//Hpt[X] = spt.x, Hpt[Y] = spt.y;
					//Tpt[X] = ept.x, Tpt[Y] = ept.y;
					//ang = ads_angle(Hpt, Tpt);
					//spt.x = CurTK->newEdX + dX; spt.y = CurTK->newEdY;
					//_tcscpy(ch, CurTK->EndXCLC);
					//BAS_DRAW_FUN::maketext(spt, ch, ang, m_Texth*K, 4, 3);

					if(TrimDlg.m_TKPos == 1)//顶
					{
						CurTK->inspt[X] = CurTK->newCenX;
						CurTK->inspt[Y] = CurTK->newCenY - (0.5 * 554 - 15);
					}
					else if(TrimDlg.m_TKPos == 2)//中
					{
						CurTK->inspt[X] = CurTK->newCenX;
						CurTK->inspt[Y] = CurTK->newCenY;			
					}
					else //底
					{
						CurTK->inspt[X] = CurTK->newCenX;
						CurTK->inspt[Y] = CurTK->newCenY + (0.5 * 554 - 15);
					}
					//绘右上角图标
					Hpt[X] = CurTK->inspt[X]; Hpt[Y] = CurTK->inspt[Y];

					//20190718	这些暂时不绘制
					////绘制里程文字信息
					//ads_point Stpt, Edpt;
					//ads_polar(Hpt, 0, 160 * K, Tpt);
					//ads_polar(Tpt, PI / 2.0, 135 * K, Stpt);
					//_stprintf(ch,L"%s-%s",CurTK->StartXCLC,CurTK->EndXCLC);
					//AcGePoint3d tempPt(Stpt[X], Stpt[Y], 0.0);
					//BAS_DRAW_FUN::maketext(tempPt,ch,0.0, 4.0 * K);

					////绘制图号文字信息
					//ads_polar(Hpt, 0, 145 * K, Tpt);
					//ads_polar(Tpt, PI / 2.0, 128 * K, Stpt);
					//_stprintf(ch, L"%d", StartNo);
					//tempPt.set(Stpt[X], Stpt[Y], 0.0);
					//BAS_DRAW_FUN::maketext(tempPt, ch, 0.0, 4.0 * K);

					////绘制图总数文字信息
					//ads_polar(Hpt, 0, 175 * K, Tpt);
					//ads_polar(Tpt, PI / 2.0, 128 * K, Stpt);
					//_stprintf(ch, L"%s", TrimDlg.m_SumT);
					//tempPt.set(Stpt[X], Stpt[Y], 0.0);
					//BAS_DRAW_FUN::maketext(tempPt, ch, 0.0, 4.0 * K);
					
					//////////////////////////////////////////////////////////////////////////

					//Lfup[X] = CurTK->newCenX - 189.9*K; Lfup[Y] = CurTK->newCenY + 128.0*K;
					//Lfbt[X] = CurTK->newCenX - 189.9*K; Lfbt[Y] = CurTK->newCenY - 128.0*K;
					//Rtup[X] = CurTK->newCenX + 189.9*K; Rtup[Y] = CurTK->newCenY + 128.0*K;
					//Rtbt[X] = CurTK->newCenX + 189.9*K; Rtbt[Y] = CurTK->newCenY - 128.0*K;
					//BAS_DRAW_FUN::AutoEntBK1(Lfbt, Lfup);
					//BAS_DRAW_FUN::AutoEntBK1(Rtup, Rtbt);

					//Lfup[X] = CurTK->newCenX-192*K; Lfup[Y] = CurTK->newCenY+130*K;
					//Lfbt[X] = CurTK->newCenX-192*K; Lfbt[Y] = CurTK->newCenY-130*K;
					//Rtup[X] = CurTK->newCenX+192*K; Rtup[Y] = CurTK->newCenY+130*K;
					//Rtbt[X] = CurTK->newCenX+192*K; Rtbt[Y] = CurTK->newCenY-130*K;
					//BAS_DRAW_FUN::DelBoxObject(Lfup,Lfbt,Rtbt,Rtup);

					//插入图框
					AcGePoint3d InsertPt(CurTK->inspt[X],CurTK->inspt[Y],0.0);
					if(pBlockId>0)
					{
						AcDbBlockReference *pBkRr = new AcDbBlockReference(InsertPt,pBlockId);
						//pBkRr->setScaleFactors(AcGeScale3d(K,K,1));
						BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
					}

					//插入指北针	20190718	指北针暂时不绘制
					//if (pBlockId1 > 0)
					//{
					//	InsertPt.x = 0.5*(CurTK->newLtUpX + CurTK->newRtUpX) + dX;
					//	InsertPt.y = 0.5*(CurTK->newLtUpY + CurTK->newRtUpY);
					//	AcDbBlockReference *pBkRr1 = new AcDbBlockReference(InsertPt, pBlockId1);
					//	double jiao;
					//	BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);
					//	pBkRr1->setScaleFactors(AcGeScale3d(1, 1, 1));
					//	pBkRr1->setRotation(-jiao);
					//	BAS_DRAW_FUN::AddEntityToDbs(pBkRr1);
					//}

					StartNo++;
				}
				acedSetStatusBarProgressMeterPos(100); 
			}			
			else
			{
				pzLinep->TransFormTK(TK,TkNum,411.48/2.0,284.90/2.0,K);

				for(int iTK=0;iTK<TkNum;iTK++)
				{
					STuKuang *CurTK=&TK[iTK];						
					//////////////////////////////////////////////////////////////////////////					
					/*
					AcDbLine *L1=new AcDbLine(AcGePoint3d(CurTK->LtUpX,CurTK->LtUpY,0.0),AcGePoint3d(CurTK->RtUpX,CurTK->RtUpY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L1);
					AcDbLine *L2=new AcDbLine(AcGePoint3d(CurTK->LtUpX,CurTK->LtUpY,0.0),AcGePoint3d(CurTK->LtBmX,CurTK->LtBmY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L2);
					AcDbLine *L3=new AcDbLine(AcGePoint3d(CurTK->RtBmX,CurTK->RtBmY,0.0),AcGePoint3d(CurTK->RtUpX,CurTK->RtUpY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L3);
					AcDbLine *L4=new AcDbLine(AcGePoint3d(CurTK->RtBmX,CurTK->RtBmY,0.0),AcGePoint3d(CurTK->LtBmX,CurTK->LtBmY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L4);

					AcDbLine *L1=new AcDbLine(AcGePoint3d(CurTK->newLtUpX,CurTK->newLtUpY,0.0),AcGePoint3d(CurTK->newRtUpX,CurTK->newRtUpY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L1);
					AcDbLine *L2=new AcDbLine(AcGePoint3d(CurTK->newLtUpX,CurTK->newLtUpY,0.0),AcGePoint3d(CurTK->newLtBmX,CurTK->newLtBmY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L2);
					AcDbLine *L3=new AcDbLine(AcGePoint3d(CurTK->newRtBmX,CurTK->newRtBmY,0.0),AcGePoint3d(CurTK->newRtUpX,CurTK->newRtUpY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L3);
					AcDbLine *L4=new AcDbLine(AcGePoint3d(CurTK->newRtBmX,CurTK->newRtBmY,0.0),AcGePoint3d(CurTK->newLtBmX,CurTK->newLtBmY,0.0));
					BAS_DRAW_FUN::AddEntityToDbs(L4);
					*/
					//////////////////////////////////////////////////////////////////////////
					ACHAR LayoutName[256];
					_stprintf(LayoutName,L"%s～%s",CurTK->StartXCLC,CurTK->EndXCLC);
					CMyLayout::CreateLayout(LayoutName);
					/*
					AcGePoint3dArray PtAy;
					PtAy.append(AcGePoint3d(CurTK->newLtUpX,CurTK->newLtUpY,0.0));
					PtAy.append(AcGePoint3d(CurTK->newRtUpX,CurTK->newRtUpY,0.0));
					PtAy.append(AcGePoint3d(CurTK->newRtBmX,CurTK->newRtBmY,0.0));
					PtAy.append(AcGePoint3d(CurTK->newLtBmX,CurTK->newLtBmY,0.0));
					CMyLayout::CreateViewInLayout(LayoutName,PtAy);
					*/
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
					acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
					ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010

					ads_point p1,p2,p3,p4,z1,z2,ori,orix,oriy;
					double xmin,xmax,ymin,ymax;
					p1[X]=CurTK->newLtUpX;
					p1[Y]=CurTK->newLtUpY;

					p2[X]=CurTK->newRtUpX;
					p2[Y]=CurTK->newRtUpY;

					p3[X]=CurTK->newRtBmX;
					p3[Y]=CurTK->newRtBmY;

					p4[X]=CurTK->newLtBmX;
					p4[Y]=CurTK->newLtBmY;

					double jiao;
					BAS_DRAW_FUN::xyddaa(CurTK->LtBmX,CurTK->LtBmY,CurTK->RtBmX,CurTK->RtBmY,&jiao);
					jiao+=0.5*PI;
					ori[X]=CurTK->LtBmX;
					ori[Y]=CurTK->LtBmY;

					orix[X]=CurTK->RtBmX;
					orix[Y]=CurTK->RtBmY;

					oriy[X]=ori[X]+10*cos(jiao);
					oriy[Y]=ori[Y]+10*sin(jiao);

					//						BAS_DRAW_FUN::getMinMaxXY(p1,p2,p3,p4,xmin,xmax,ymin,ymax);
					xmin=0.0;
					xmax=(p3[X]-p4[X]);//*K;
					ymin=0.0;
					ymax=(p2[Y]-p3[Y]);//*K;

					if(p1[X]<p4[X])xmin-=(p4[X]-p1[X]);//*K;
					if(p2[X]>p3[X])xmax+=(p2[X]-p3[X]);//*K;					

					z1[X]=xmin;
					z1[Y]=ymin;
					z2[X]=xmax;
					z2[Y]=ymax;

					//						AcGePoint3dArray PtAy;
					//						PtAy.append(AcGePoint3d(p1[X],p1[Y],0.0));
					//						PtAy.append(AcGePoint3d(p2[X],p2[Y],0.0));
					//						PtAy.append(AcGePoint3d(p3[X],p3[Y],0.0));
					//						PtAy.append(AcGePoint3d(p4[X],p4[Y],0.0));
					//						CMyLayout::CreateViewInLayout(LayoutName,PtAy);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
					acedCommandS(RTSTR, L"MSPACE", RTNONE);
					acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

					acedCommandS(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);
					acedCommandS(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);

					acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
					acedCommandS(RTSTR, L"PSPACE", RTNONE);
					acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
					acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
					acedCommandS(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
					acedCommandS(RTNONE);
#endif // VERSION_CAD2016
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
					_tcscpy(dwgPath,TrimDlg.m_TuKuangFile);
					AcDbObjectId pBlockId;
					pBlockId=BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath,L"");
					if(pBlockId>0)
					{
						AcGePoint3d InsertPt(411.48/2.0,284.90/2.0,0.0);
						AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt,pBlockId);
						pBkRr->setScaleFactors(AcGeScale3d(K,K,1));
						BAS_DRAW_FUN::AddEntityToDbs(pBkRr,0,1);
					}
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR,L"-PLOT",RTSTR,L"Y",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",
						RTSTR,L"E",RTSTR,L"F",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",
						RTSTR,L"",RTSTR,L"Y", RTSTR,L"N",RTNONE);	
					acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR,L"-PLOT",RTSTR,L"Y",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",
						RTSTR,L"E",RTSTR,L"F",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",RTSTR,L"",
						RTSTR,L"",RTSTR,L"Y", RTSTR,L"N",RTNONE);	
					ads_command(RTNONE);
#endif // VERSION_CAD2010

				}
			}
			if(TK)delete []TK;
		}	
	}
	else
	{
		AfxMessageBox(L"请先设置出图模板");
	}
}


// This is command 'JCAUTOTK'
void penglihuiJCAUTOTK()
{
	// TODO: Implement the command
}

#include "PmDrawDialog.h"
// This is command 'JCPMDRAWSET'
void penglihuiJCPMDRAWSET()
{
	// TODO: Implement the command
	CPmDrawDialog PmSetDlg;
	PmSetDlg.DoModal();
}

// This is command 'JCBIAODING'
void penglihuiJCBIAODING()
{
	// TODO: Implement the command
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{		
		Path=WorkDir.DbsPathName;
		CFileDialog FDlg(TRUE,L".mdb",Path);
		FDlg.m_ofn.lpstrDefExt = L"mdb";
		FDlg.m_ofn.Flags = OFN_CREATEPROMPT;
		FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
		FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
		if(FDlg.DoModal()==IDOK)
		{
			CString MdbName=FDlg.GetFileName();
			int m=MdbName.Find(L".mdb");
			WorkDir.WriteProjName(MdbName.Left(m));
			CString PathName=FDlg.GetPathName();
			int n=PathName.Find(L"\\DATA");
			WorkDir.WriteWorkdir(PathName.Left(n));
		}
	}
}

// This is command 'GG'
void penglihuihuhu_cc_gg()
{
	// TODO: Implement the command
	BAS_DRAW_FUN n;
	n.groups();
}


// This is command 'ZM_ZDPMFA_Road'	选择平面（纵断面地面线）
void pzm_zdpmfazm_zdpmfa()//纵断面指定平面方案
{
	// TODO: Implement the command
	// TODO: Implement the command
#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
	if(!CheckByTime())
		return;
#endif
	ZMZDPMFA  dlg;	
	if(dlg.DoModal()==IDOK)
	{
		pzLinep = dlg.pzLine;
		pzld = dlg.m_pzld;//平纵联动
		ZxId = dlg.GTZXId;//平面实体ID			
	}
	else
	{
		pzld = false;
		return;
	}
}

// This is command 'SetZMBZ_Road'
void SetZMBZ()
{
	ZDMStandardDlg dlg;
	dlg.DoModal();
}

//点绘边线地面线
void BxDmx()
{
	double zdmjg;
	int i;
	int num;
	double *dmx;
	BAS_DRAW_FUN basdraw;

	CNCDMXCS dmxcsdlg(0,0);
	if(dmxcsdlg.DoModal()!=IDOK)
		return;

	zdmjg = _wtof(dmxcsdlg.m_ncspacer);
	int izdm = dmxcsdlg.izdm;
	if(izdm<0)
		return;
	if(dmxcsdlg.m_ifdtm==0 && !ptriDTM.DTMHasRead)	
		ptriDTM.Read_Dtm();

	GlobalZDMTK.ZDMTK[izdm].pm->ZDMJG = zdmjg;
	if(dmxcsdlg.NDMX>0)
	{
		if(dmxcsdlg.m_ifdtm==0)//数模内插边线地面线
		{
			DMX_ROAD *bxDmx;	
			double sml,eml;
			for(i=0;i<dmxcsdlg.NDMX;i++)
			{
				if(fabs(dmxcsdlg.DmxCsArr[i].ZJ)>0.1)//为边线
				{
					sml = GlobalZDMTK.ZDMTK[izdm].pm->TYLC(dmxcsdlg.DmxCsArr[i].sdml);
					eml = GlobalZDMTK.ZDMTK[izdm].pm->TYLC(dmxcsdlg.DmxCsArr[i].edml);
					if(eml > sml && sml>-0.1)
					{
						num=(int)((eml-sml)/(zdmjg-1))*2;						
						dmx=new double [num*2];
						num=GlobalZDMTK.ZDMTK[izdm].pm->PM_DMX(dmx,dmxcsdlg.DmxCsArr[i].ZJ,sml,eml);
						bxDmx=new DMX_ROAD(num,dmx,izdm,0);
						bxDmx->ZJ=dmxcsdlg.DmxCsArr[i].ZJ;
						bxDmx->startLC = GlobalZDMTK.ZDMTK[izdm].pm->DLArray[0].ELC;
						if(dmx) 
						{ 
							delete []dmx;
							dmx = NULL;
						}
						bxDmx->Color = dmxcsdlg.DmxCsArr[i].Color;
						basdraw.AddEntityToDbs(bxDmx);
					}
				}
			}
		}
		else//实测内插边线地面线
		{
			DMX_ROAD *bxDmx;	
			double sml,eml,*newdmx;
			HdmDes hdm;
			for(i=0;i<dmxcsdlg.NDMX;i++)
			{
				if(fabs(dmxcsdlg.DmxCsArr[i].ZJ)>0.1)//为边线
				{
					sml = GlobalZDMTK.ZDMTK[izdm].pm->TYLC(dmxcsdlg.DmxCsArr[i].sdml);
					eml = GlobalZDMTK.ZDMTK[izdm].pm->TYLC(dmxcsdlg.DmxCsArr[i].edml);
					if(eml > sml && sml>-0.1)
					{
						num=(int)((eml-sml)/(zdmjg-1))*2;	
						if(num>0)
							dmx=new double [2*num*2];
						else
							return;
						num = hdm.SetBXDmx(GlobalZDMTK.ZDMTK[izdm].pm->mdbname,GlobalZDMTK.ZDMTK[izdm].pm->RoadName,dmx,dmxcsdlg.DmxCsArr[i].ZJ,sml,eml,GlobalZDMTK.ZDMTK[izdm].pm);
						if(num>0)
							newdmx = new double [num*2];
						else
							return;
						for(int j=0; j<num*2; j++)
							newdmx[j] = dmx[j];
						if(dmx) 
						{ 
							delete []dmx;
							dmx = NULL;
						}
						bxDmx=new DMX_ROAD(num,newdmx,izdm,0);
						bxDmx->ZJ=dmxcsdlg.DmxCsArr[i].ZJ;
						bxDmx->startLC = GlobalZDMTK.ZDMTK[izdm].pm->DLArray[0].ELC;
						if(newdmx) 
						{ 
							delete []newdmx;
							newdmx = NULL;
						}
						bxDmx->Color = dmxcsdlg.DmxCsArr[i].Color;
						basdraw.AddEntityToDbs(bxDmx);
					}
				}
			}



		}
	}

}

void INBXDMX()
{
	AcGePoint3d PT;
	ads_point resPt;
	int izdm;

	if(acedGetPoint(NULL,L"请在纵面图内拾取一点!\n",resPt)==RTNORM)
	{
		PT.x = resPt[X];
		PT.y = resPt[Y];
		izdm = GlobalZDMTK.FindWichZdm(PT);
		if(izdm>=0)
		{				
			if(GlobalZDMTK.ZDMTK[izdm].pm)
			{
				pzLinep = GlobalZDMTK.ZDMTK[izdm].pm;
				XLDataBase DBS;
				BAS_DRAW_FUN basdraw;
				DMX_ROAD *bxDmx;
				double *dmx;
				int i;
				DBS.Read_XLDbs(GlobalZDMTK.ZDMTK[izdm].mdbname,L"边线地面线表",GlobalZDMTK.ZDMTK[izdm].pm->RoadName);				
				CNCDMXCS dmxcsdlg(0,0);
				dmxcsdlg.NDMX = DBS.NDMX;
				for(i=0;i<DBS.NDMX;i++)
				{
					dmxcsdlg.DmxCsArr[i].ZJ = DBS.DmxCsArr[i].ZJ;
					dmxcsdlg.DmxCsArr[i].sdml = DBS.DmxCsArr[i].sdml;
					dmxcsdlg.DmxCsArr[i].edml = DBS.DmxCsArr[i].edml;
					dmxcsdlg.DmxCsArr[i].Color = DBS.DmxCsArr[i].Color;
				}
				if(dmxcsdlg.DoModal()!=IDOK)
					return;
				CString XZ;
				for(i=0;i<dmxcsdlg.NDMX;i++)
				{
					XZ.Format(L"%0.3lf",dmxcsdlg.DmxCsArr[i].ZJ);
					DBS.Read_XLDbs(GlobalZDMTK.ZDMTK[izdm].mdbname,L"边线地面线表",GlobalZDMTK.ZDMTK[izdm].pm->RoadName,XZ);					
					dmx=new double [DBS.NTW*2];
					for(int j=0;j<DBS.NTW;j++)
					{
						dmx[j*2] = DBS.TWB[j].cml;
						dmx[j*2+1] = DBS.TWB[j].DMGC;
					}
					bxDmx=new DMX_ROAD(DBS.NTW,dmx,izdm,0);
					bxDmx->ZJ=dmxcsdlg.DmxCsArr[i].ZJ;
					bxDmx->startLC = GlobalZDMTK.ZDMTK[izdm].pm->DLArray[0].ELC;
					if(dmx) 
					{ 
						delete []dmx;
						dmx = NULL;
					}
					bxDmx->Color = dmxcsdlg.DmxCsArr[i].Color;
					basdraw.AddEntityToDbs(bxDmx);
				}
			}	
		}
	}   
}
//保存边线地面线
void SaveBXDmx()
{
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;	
	DMX_ROAD  *pdmx;

	rc = ads_entsel(L"\n选择要保存的地面线实体: ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{
		ads_printf(L"\n实体选择错误!"); 
		return;   
	}

	int izdm;
	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;
	acdbGetObjectId(eId, en);	
	AcDbObject *pObj;

	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(DMX_ROAD::desc())) //是DMX实体,取数据
	{
		pdmx = DMX_ROAD::cast(pObj);
		double ml;	
		XLDataBase DBS;

		DBS.NTW = pdmx->NUM; 
		if(DBS.TWB) {delete []DBS.TWB; DBS.TWB=NULL;}
		if(DBS.NTW>0)
		{
			ads_printf(L"\n正在写边线地面线表...");
			DBS.TWB = new XLDataBase::TWTAB[DBS.NTW];
			CString ZJ,str;
			ZJ.Format(L"%0.3lf",pdmx->ZJ);
			for(int i=0; i<pdmx->NUM;i++)
			{
				ml = GlobalZDMTK.ZDMTK[izdm].pm->XLC1(pdmx->DMXys[i].lc);
				str.Format(L"%0.2lf",ml);
				DBS.TWB[i].ml = _wtof(str);
				str.Format(L"%0.2lf",pdmx->DMXys[i].level);
				DBS.TWB[i].DMGC = _wtof(str);

			}
			DBS.Write_XLDbs(GlobalZDMTK.ZDMTK[izdm].mdbname,L"边线地面线表",GlobalZDMTK.ZDMTK[izdm].pm->RoadName,ZJ);
		}
	}
	else
	{
		acutPrintf(L"所选实体不是地面线实体!\n");
		return;
	}	
}
// This is command 'DMX'	数模内插
void ptestdmx()//内插地面线
{
	// TODO: Implement the command
	int NCinf,i;
	
	if (pzLinep == NULL)
	{
		ads_alert(L"请选定当前项目并选择平面方案!");
		return;
	}
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	CString MdbName = Cworkdir + L"\\data\\" + Pro + L".mdb";
	pzLinep = g_RoadWorkingDatabase.CreateHorizontalRoad(MdbName, pzLinep->RoadName);
	BAS_DRAW_FUN basdraw;
	AcGePoint3d PT,PT1,PT2;
	CString DmxFname;

	if(pzld)
		NCinf=1;
	else
	{
		DMDHQFS  dlg;
		if(dlg.DoModal()==IDOK)
			NCinf=dlg.NCinf;
		else
			return;
		if(!NCinf)
			DmxFname=pzLinep->mdbname;
	}

	int IsDtmExisting=0;
	//double DMPJGC;
	if(NCinf&& !ptriDTM.DTMHasRead)
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

	AcGePoint3d pkPt;
	acedInitGet(NULL, NULL);
	if(acedGetPoint(NULL,L"\n请在屏幕上指定绘图原点:",asDblArray(pkPt))==RTCAN)
		return;
	pORI_SCALE.X_ORI = pkPt.x + 30;
	pORI_SCALE.Y_ORI = pkPt.y + pORI_SCALE.PQXHIGH + pORI_SCALE.LCBHigh + 20;

	GlobalZDMTK.NZDM++;//个数增加
	GlobalZDMTK.CurZDM = GlobalZDMTK.NZDM-1;
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].pm = pzLinep;
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].mdbname=pzLinep->mdbname;
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].RXDname = pzLinep->RoadName;
	int num;
	double *dmx, zdmjg;
	DMX_ROAD *pDmx;

	if(NCinf)//数模内插地面线
	{
		int zdmjg1;

		ads_initget(2+4,NULL);
		if(ads_getreal(L"\n  请输入纵断面地面线内插间隔<10米> ",&zdmjg)==RTNONE)
			zdmjg = 10;

		if((int)(pzLinep->LenOfLine/(zdmjg-1))+pzLinep->XYNum > 30000)//超过数组界限
		{
			zdmjg1=(int)(pzLinep->LenOfLine/(30000-pzLinep->XYNum));
			ads_printf(L"内插间隔%lf会引起数组越界，修改内插间隔为%d米\n",zdmjg,zdmjg1);
			zdmjg = zdmjg1;
		}

		num = (int)(pzLinep->LenOfLine / (zdmjg - 1)) * 2;

		if(num<1)
			return;
		//dmx = new double[num * 2];
		dmx = new double[num * 6];	//存在越界情况，先暂时这么处理
		pzLinep->ZDMJG = zdmjg;		

		//ads_printf(L"%0.2lf\n",pzLinep->DLArray[0].ELC);
		//ads_printf(L"%0.2lf\n", pzLinep->DLArray[0].ELC + pzLinep->LenOfLine);
		num = pzLinep->PM_DMX(dmx, 0, pzLinep->DLArray[0].ELC, pzLinep->DLArray[0].ELC + pzLinep->LenOfLine);	//这个函数用来生成地面线
		pDmx = new DMX_ROAD(num, dmx, GlobalZDMTK.CurZDM, 1);

		if(dmx) delete []dmx;		
		//统一里程转现场里程
		CString  gh; 
		double xlc;
		for(i=0; i<pDmx->NUM;i++)
		{
			xlc = pzLinep->XLC(pDmx->DMXys[i].lc, gh);
			_tcscpy(pDmx->DMXys[i].ckml, L"\0");
			_tcscpy(pDmx->DMXys[i].ckml, pzLinep->LCchr(gh, xlc, 2));
		}
	}
	else//从填挖表读取
	{
		CAdoConnection m_adoConnection;
		CAdoRecordSet Report_input;

		ACHAR mes[256];
		AfxOleInit();
		try                 
		{	
			// 打开本地Access库
#ifdef X64
			_stprintf(mes, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", DmxFname);
#else
			_stprintf(mes, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", DmxFname);
#endif // X64
			
			CString strConnect = mes;
			//strConnect+="数据库.mdb";
			m_adoConnection.Disconnect();
			if (!m_adoConnection.Connect(strConnect))
			{
				AfxMessageBox(L"连接数据库失败!");			
				return ;
			}	
		}
		catch(_com_error e)
		{		
			AfxMessageBox(L"数据库连接失败，确认Pm.mdb是否在当前路径下!");		
		} 	
		CString m_strSQL;
		CString TabFname;			

		TabFname = "纵地面线表";
		m_strSQL.Format(L"select * from %s where 道路名='%s' order by 序号",TabFname,pzLinep->RoadName);

		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"数据库没有打开或已经关闭!");
			return ;
		}
		Report_input.SetAdoConnection(&m_adoConnection);
		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			ads_printf(L"没有纵地面线表记录\n");			
			Report_input.Close();
			m_adoConnection.Disconnect();
			return ;
		}

		CString GhK,rcd; 
		double dml;

		num = Report_input.GetRecordCount();
		ads_printf(L"地面点数：%d\n",num);
		if(num>0)
			dmx=new double [num*2];
		else
			return;

		CString str;
		Report_input.MoveFirst();
		int i=0;
		while(!Report_input.IsEOF())
		{	
			Report_input.GetValueString(rcd,L"加桩里程数");//加桩里程	
			dml=_wtof(rcd);
			//			pzLinep->dmltockml(GhK,dml,ckml);
			dmx[i*2]=pzLinep->TYLC(dml);  
			Report_input.GetValueString(str,L"地面高程");//地面高程	
			dmx[i*2+1] = _wtof(str);
			i++;
			Report_input.MoveNext();
		}
		Report_input.Close();
		m_adoConnection.Disconnect();
		pDmx=new DMX_ROAD(num,dmx,GlobalZDMTK.CurZDM,1);
		if(dmx) delete []dmx;
	}

	double DesH = 0.0;
	g_RoadWorkingDatabase.GetJPData(GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].mdbname, GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].RXDname, pDmx->m_JPInforArray_DMX); //传入接坡信息
	g_RoadWorkingDatabase.GetLevelCtrlData(GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].mdbname, GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].RXDname, pDmx->m_LevelCtrlPtArray_DMX);	//传入控制高程数组
	if (pDmx->m_LevelCtrlPtArray_DMX.size() > 0)
	{
		//考虑交叉点
		for (int i = 0; i < pDmx->m_LevelCtrlPtArray_DMX.size(); i++)
		{
			if (pDmx->m_LevelCtrlPtArray_DMX[i].Note.Find(L"交叉") >= 0)
			{
				if (pDmx->m_LevelCtrlPtArray_DMX[i].Style == 1)
				{
					DesH = pDmx->m_LevelCtrlPtArray_DMX[i].DesHSX;

					pDmx->ZMin = pDmx->ZMin < DesH ? pDmx->ZMin : (DesH - 3);
					pDmx->ZMax = pDmx->ZMax > DesH ? pDmx->ZMax : (DesH + 3);
				}
				else if (pDmx->m_LevelCtrlPtArray_DMX[i].Style == 2)
				{
					DesH = pDmx->m_LevelCtrlPtArray_DMX[i].DesHXX;
					pDmx->ZMin = pDmx->ZMin < DesH ? pDmx->ZMin : (DesH - 3);
					pDmx->ZMax = pDmx->ZMax > DesH ? pDmx->ZMax : (DesH + 3);
				}
			}
		}
	}
	pDmx->set_ori_scale();
	pDmx->get_axle_legth();
	AcDbObjectId  LDMXId;
	LDMXId=basdraw.AddEntityToDbs(pDmx);

	//平曲线
	DMX_PQX_ROAD *pPqx=new DMX_PQX_ROAD();
	pPqx->m_Frame_Index = GlobalZDMTK.CurZDM;
	//		if(pqx)delete []pqx;
	AcDbObjectId  LPQXId;
	LPQXId=basdraw.AddEntityToDbs(pPqx);

	//里程表
	DMX_LCB_ROAD *pDmx_Lcb=new DMX_LCB_ROAD();
	AcDbObjectId  LLCBId;
	LLCBId=basdraw.AddEntityToDbs(pDmx_Lcb);
	pDmx_Lcb->close();

	//地面线标高
	EarhBz_ROAD *pEarhBz=new EarhBz_ROAD();
	pEarhBz->m_Frame_Index = GlobalZDMTK.CurZDM;
	AcDbObjectId  EarhBzId;
	EarhBzId=basdraw.AddEntityToDbs(pEarhBz);

	//加响应器
	AcDbObject *pObj;
	DMXReactor *pDMXReactor = new DMXReactor(EarhBzId);
	if(acdbOpenObject(pObj,LDMXId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	DMX_ROAD *pdmx;
	pdmx = DMX_ROAD::cast(pObj);
	pdmx->addReactor(pDMXReactor);
	pdmx->close();		
	ReactorEntityPair *pPair = new ReactorEntityPair;
	pPair->entityId=LDMXId;
	pPair->pEntReactor=pDMXReactor;
	g_reactorList.append(pPair);
	/*	if(pzld && NCinf && prePair==NULL)//平纵联动,并且内插方式且未加响应器
	{
	acdbOpenObject(pzLinep, ZxId, AcDb::kForRead);			
	GTZXReactor *pGTZXReactor= new GTZXReactor(LDMXId,LPQXId,LLCBId);
	pzLinep->addReactor(pGTZXReactor);
	pzLinep->close();
	ReactorEntityPair *pPair = new ReactorEntityPair;
	pPair->entityId=ZxId;
	pPair->pEntReactor=pGTZXReactor;
	prePair=pPair;
	g_reactorList.append(pPair);
	}
	else if(prePair!=NULL)//非平纵联动，且已加响应器 删除
	{
	acdbOpenObject(pzLinep,prePair->entityId,AcDb::kForRead, Adesk::kTrue);
	pzLinep->removeReactor(prePair->pEntReactor);
	pzLinep->close();
	prePair=NULL;
	}*/

	if(GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].RXDname!=L"主线")//是分离段
	{
		int NBPD;
		double PD_array[MAXBPDNUM][2],PDB[2][4];
		ACHAR ckml[180],GH[10],chrh[30];
		XLDataBase xlmdb;
		CString mdbname;

		mdbname = GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].mdbname;
		JD_CENTER *pm;
		// 创建每一道路平面对象
		xlmdb.Read_XLDbs(mdbname,L"控制点表",L"主线");		
		xlmdb.Read_XLDbs(mdbname,L"断链表",L"主线");
		pm = NULL;
		if(xlmdb.NJdCen>0)
		{
			pm=(JD_CENTER *)new JD_CENTER;//主线对象
			pm->SetJdCenterArray(xlmdb.JdCenArray,xlmdb.NJdCen);
			if(xlmdb.NDL>0)
				pm->setDLB(xlmdb.DLArray,xlmdb.NDL);
		}
		else return;

		xlmdb.Read_XLDbs(mdbname,L"坡度表",L"主线");
		NBPD = xlmdb.NPD;
		if(NBPD < 2)
			return;
		int deshxsw;

		for(i=0;i<NBPD;i++)
		{
			PD_array[i][0] = pm->TYLC(xlmdb.PDB[i].ml);
			PD_array[i][1] = xlmdb.PDB[i].Level;	//主线坡度			
		} 

		deshxsw = 3;//设计标高小数位数
		if(xlmdb.PDB) 
		{
			deshxsw = (int)(xlmdb.PDB[0].Rshu + 0.1);
			pORI_SCALE.DesHXSW = deshxsw;
			delete[]xlmdb.PDB;
			xlmdb.PDB = NULL;
		}

		double cml, Sht, Spd, Eht, Epd, Len;
		AcGePoint3d spt, ept;

		cml=pm->PROJ_ML(pzLinep->XYArray[0][1],pzLinep->XYArray[0][2]);//分离线起点在主线统一里程
		int j;
		for(j=0;j<NBPD;j++)
		{
			if(cml<PD_array[j][0])
				break;
		}
		Len = cml - PD_array[j-1][0];
		Sht = PD_array[j - 1][1] + (PD_array[j][1] - PD_array[j - 1][1])*(cml - PD_array[j - 1][0]) / (PD_array[j][0] - PD_array[j - 1][0]);
		Spd = (PD_array[j][1] - PD_array[j - 1][1])*100.0 / (PD_array[j][0] - PD_array[j - 1][0]);
		cml = pzLinep->DLArray[0].ELC;//分离线起点里程
		PDB[0][0] = (cml - pzLinep->DLArray[0].ELC) / pORI_SCALE.HBVSCALE + pORI_SCALE.X_ORI;
		PDB[0][1] = Sht - pORI_SCALE.ZMIN + pORI_SCALE.Y_ORI;
		double Dis;
		Dis = 400;
		if(pzLinep->LenOfLine<=400)
			Dis = 0.5*Dis;
		PDB[1][0] = (cml + Dis - pzLinep->DLArray[0].ELC) / pORI_SCALE.HBVSCALE + pORI_SCALE.X_ORI;
		PDB[1][1] = Sht + Dis*Spd / 100.0 - pORI_SCALE.ZMIN + pORI_SCALE.Y_ORI;
		_tcscpy(GH,pzLinep->DLArray[0].EGH);
		GTZDM_ROAD  *pzLine = new GTZDM_ROAD(2, PDB, GH, 1, deshxsw);
		pzLine->setINIT(3,3,1,5,100);
		xlmdb.AddEntityToDbs(pzLine);
		spt.x=PDB[0][0],spt.y=PDB[0][1];
		ept.x = (cml - 200.0 - pzLinep->DLArray[0].ELC) / pORI_SCALE.HBVSCALE + pORI_SCALE.X_ORI;
		ept.y = Sht - 200.0*Spd / 100.0 - pORI_SCALE.ZMIN + pORI_SCALE.Y_ORI;
		xlmdb.makeline(spt,ept,2,0);

		double fb, de,texth=4.0;
		de=xlmdb.xyddaa(PD_array[j-1][1],PD_array[j-1][0],PD_array[j][1],PD_array[j][0],&fb);
		fb= 0.5*pi-fb;
		// 坡度  
		ads_rtos(Spd,2,1,chrh);
		_tcscat(chrh,L"(o/o)");		
		ept.x = spt.x - 200.0*0.5 / pORI_SCALE.HBVSCALE - texth;
		ept.y = spt.y + 0.8*texth;
		xlmdb.maketext(ept, chrh, fb, texth, 0, 2);
		//  坡长
		if (fabs(Len-(int)(Len))<0.001)  { ads_rtos(Len,2,0,chrh);}
		else ads_rtos(Len,2,3,chrh);
		ept.x = spt.x - 200.0 * 0.5 / pORI_SCALE.HBVSCALE - 2 * texth;
		ept.y = spt.y - 2.0 * texth;
		xlmdb.maketext(ept, chrh, fb, texth, 0, 2);

		//		pzLinep->dmltockml(pzLinep->DLB[pzLinep->NDL-1].EGH,pzLinep->DLB[pzLinep->NDL-1].BLC+pzLinep->DLB[pzLinep->NDL-1].DL,ckml);//终点主线里程
		//		cml=xlmdb.TYLC1(ckml,xlmdb.NDL,xlmdb.DLB);//算主线统一里程
		cml=pm->PROJ_ML(pzLinep->XYArray[pzLinep->XYNum][8],pzLinep->XYArray[pzLinep->XYNum][9]);//分离线终点在主线统一里程
		for(j=0;j<NBPD;j++)
		{
			if(cml<=PD_array[j][0])
				break;
		}
		Len = PD_array[j][0] - cml;
		Eht=PD_array[j-1][1]+(PD_array[j][1]-PD_array[j-1][1])*(cml-PD_array[j-1][0])/(PD_array[j][0]-PD_array[j-1][0]);
		Epd=(PD_array[j][1]-PD_array[j-1][1])*100.0/(PD_array[j][0]-PD_array[j-1][0]);
		cml=pzLinep->DLArray[pzLinep->DLNum-1].TYLC;
		PDB[0][0]=(cml-Dis-pzLinep->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+pORI_SCALE.X_ORI;
		PDB[0][1]=Eht-Epd*Dis/100.0-pORI_SCALE.ZMIN+pORI_SCALE.Y_ORI; 
		PDB[1][0]=(cml-pzLinep->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+pORI_SCALE.X_ORI;
		PDB[1][1]=Eht-pORI_SCALE.ZMIN+pORI_SCALE.Y_ORI; 		
		_tcscpy(GH,pzLinep->DLArray[pzLinep->DLNum-1].BGH);
		pzLine= new GTZDM_ROAD(2,PDB,GH,1,deshxsw);
		pzLine->setINIT(3,3,1,5,100);
		xlmdb.AddEntityToDbs(pzLine);
		spt.x=PDB[1][0],spt.y=PDB[1][1]-20;
		ept.x=(cml+200.0-pzLinep->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+pORI_SCALE.X_ORI;
		ept.y=Eht+200.0*Epd/100.0-pORI_SCALE.ZMIN+pORI_SCALE.Y_ORI-20; 		
		xlmdb.makeline(spt,ept,2,0);

		de=xlmdb.xyddaa(PD_array[j-1][1],PD_array[j-1][0],PD_array[j][1],PD_array[j][0],&fb);
		fb= 0.5*pi-fb;
		// 坡度  
		ads_rtos(Epd,2,1,chrh);
		_tcscat(chrh,L"(o/o)");		
		ept.x=spt.x + 200.0*0.5/pORI_SCALE.HBVSCALE+texth;
		ept.y=spt.y + 0.8*texth;
		xlmdb.maketext(ept,chrh,fb,texth,0,2);     
		//  坡长
		if (fabs(Len-(int)(Len))<0.001)  {  ads_rtos(Len,2,0,chrh);}
		else ads_rtos(Len,2,3,chrh);
		ept.x=spt.x + 200.0*0.5/pORI_SCALE.HBVSCALE+texth;
		ept.y=spt.y - 2.0*texth;
		xlmdb.maketext(ept,chrh,fb,texth,0,2);     		
	}
}

// This is command 'INITZDM_Road'	基本参数（纵断面）
void pINITZDMINITZDM()
{
	// TODO: Implement the command
	ZdmOriDataDlg dlg;
	dlg.DoModal();//设置绘图参数 		   	
}

#include "SAVEZDMDLG.h"
// This is command 'SAVEZDMPRJ_Road'	保存方案（纵断面）
void psavezdmprjsavezdmprj()	
{
	// TODO: Implement the command
	int i,trow=0;
	AcDbObjectId eId;
	CString str;

	DMX_ROAD  *pdmx=NULL;
	GTZDM_ROAD *pzdm=NULL;	
	AcGePoint3d EarPT,DesPT;

	CString RXDname;

	pzdm = NULL; 
	/*
	rc = ads_entsel(L"\n选择一个纵断面坡度线 : ", en, asDblArray(DesPT));
	if(rc==RTCAN)
	return;
	if (rc == RTNORM) 
	{
	acdbGetObjectId(eId, en);
	acdbOpenObject(pObj, eId, AcDb::kForRead);
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM::desc())) //是GTZDM实体,取数据
	{		
	pObj->close();
	acdbOpenObject(pzdm, eId, AcDb::kForRead);
	pzdm->close();
	}
	else
	{
	pObj->close();
	acutPrintf(L"所选实体不是纵断面坡度线,未保存纵坡!\n");
	//			 return;
	}
	}
	else
	{
	acutPrintf(L"选择纵断面坡度线实体错误,未保存纵坡!\n");
	//		 return;
	}*/

	SAVEZDMDLG dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	int izdm;
	//szf	 GlobalZDMTK.IsRXDOrDX(DesPT,DXorRXD,rxdno,RXDname);
	/*
	pzLinep = GlobalZDMTK.GetPM(DesPT);
	izdm = GlobalZDMTK.FindWichZdm(DesPT);
	if(izdm<0 || izdm >=GlobalZDMTK.NZDM)
	return;*/
	izdm = _wtoi(dlg.m_ZDMTKNO);
	pzLinep = GlobalZDMTK.ZDMTK[izdm].pm;
	CString curprj,mdbname;
 	curprj =dlg.m_Dbs; 
	curprj.MakeUpper();
	mdbname = pzLinep->mdbname;
	mdbname.MakeUpper();
	if(mdbname!=curprj) 
	{
		CFileFind Findlg;
		if(Findlg.FindFile(curprj)!=0)//已经有该文件 并且文件名与实体数据库名不一致
		{
			CReWriteFILE Redlg;
			Redlg.MES=curprj+"文件已存在,并且与所选实体记录的文件名不一致,是否覆盖?";
			if(Redlg.DoModal()!=IDOK)
				return;
		}		
	}

	//ads_printf(L"\n正在写交点表...");
	//从纵断面坡度实体的响应器得到桥隧指针
	//	GlobalZDMTK.ZDMTK[izdm].pbri=NULL;
	//	GlobalZDMTK.ZDMTK[izdm].ptun=NULL;
	/*
	AcDbVoidPtrArray *ZDMReactorArr=NULL;
	if(pzdm)
	{
	ZDMReactorArr=pzdm->reactors();
	AcDbEntity *pEnt;
	int num=0;
	if(ZDMReactorArr!=NULL)
	num = ZDMReactorArr->length();
	for(i=0;i<num;i++)
	{
	GTZDMReactor *pReactor=NULL;
	pReactor = (GTZDMReactor *)ZDMReactorArr->at(i);

	if(pReactor)
	{
	if(pReactor->m_BorT=='B')
	{		
	if(pReactor->m_ObjId)
	{
	//读Bri数据；
	int es = acdbOpenObject(pEnt, pReactor->m_ObjId, AcDb::kForRead);//打开关联的实体BRIorTun修改数据
	if(es == Acad::eOk)
	{
	GlobalZDMTK.ZDMTK[izdm].pbri = BRI::cast(pEnt);
	GlobalZDMTK.ZDMTK[izdm].pbri->close();//?wor
	}										
	}
	}
	else if(pReactor->m_BorT=='T')
	{
	if(pReactor->m_ObjId)
	{
	//读Bri数据；					
	int es = acdbOpenObject(pEnt, pReactor->m_ObjId, AcDb::kForRead);//打开关联的实体BRIorTun修改数据
	if(es == Acad::eOk)
	{
	GlobalZDMTK.ZDMTK[izdm].ptun = TUN::cast(pEnt);
	GlobalZDMTK.ZDMTK[izdm].ptun->close();//?wor						
	}										
	}
	}
	}
	}
	}*/

	//写曲线表
	//考虑平纵联动，还要写平面
	//写入数据库	

	XLDataBase DBS;
	/*
	if(dlg.m_infpm&&pzLinep)
	{
	ads_printf(L"\n正在写平面方案...");
	DBS.NJdCen=pzLinep->JdCenNum;
	if(DBS.NJdCen>0)
	DBS.JdCenArray=new BAS_DRAW_FUN::JdCenter[DBS.NJdCen];
	for(i=0;i<DBS.NJdCen;i++)
	DBS.JdCenArray[i]=pzLinep->JdCenArray[i];

	DBS.NDL=pzLinep->DLNum;
	DBS.DLArray=new BAS_DRAW_FUN::DLBZ[DBS.NDL];
	for(i=0;i<DBS.NDL;i++)
	DBS.DLArray[i]=pzLinep->DLArray[i];
	DBS.XYNum = pzLinep->XYNum;
	DBS.XYArray = new double[DBS.XYNum+1][10];
	int j;
	for(i=0;i<=DBS.XYNum;i++)
	{
	for(j=0;j<10;j++)
	DBS.XYArray[i][j]=pzLinep->XYArray[i][j];
	}
	if(curprj != "")
	{
	DBS.Write_XLDbs(curprj,L"道路名",dlg.m_RoadName);
	DBS.Write_XLDbs(curprj,L"控制点表",dlg.m_RoadName);
	DBS.Write_XLDbs(curprj,L"线元表",dlg.m_RoadName);
	DBS.Write_XLDbs(curprj,L"断链表",dlg.m_RoadName);	
	}
	}*/

	double LL,II,HH,LC;
	pzdm = GlobalZDMTK.ZDMTK[izdm].zdm;
	if(pzdm)
	{
		DBS.NZDL=pzdm->DLNum;
		DBS.ZDLArray = new BAS_DRAW_FUN::DLBZ[DBS.NZDL];
		//	acutPrintf(L"NDL=%d\n ",DBS.NDL);
		for (i = 0; i < DBS.NZDL; i++)
			DBS.ZDLArray[i] = pzdm->DLArray[i];
		DBS.Write_XLDbs(curprj, L"纵断链表", dlg.m_RoadName);
	}
	if (dlg.m_infpd&&pzdm)
	{
		//写坡度表
		ads_printf(L"\n正在写坡度表...");
		DBS.NPD = pzdm->NBPD;
		if (DBS.PDB) 
			delete[]DBS.PDB;
		if (DBS.NPD > 0)
		{
			DBS.PDB = new XLDataBase::PDTAB[DBS.NPD];
			for (i = 0; i < DBS.NPD; i++)
			{
				double xlc;
				CString  GH;	
				if (i < DBS.NPD - 1)
				{
					LL = (pzdm->BPD_array[i + 1][0] - pzdm->BPD_array[i][0]) * pzdm->HSCALE;
					if (fabs(LL) < 0.001)
						LL = 100.0;
					II = (pzdm->BPD_array[i + 1][1] - pzdm->BPD_array[i][1]) * pzdm->VSCALE * 100.0 / LL;
				}
				else
					LL = II = 0.0;

				//坡长
				str.Format(L"%0.2lf", LL);
				DBS.PDB[i].length = _wtof(str);

				//坡度
				if(II>0.00000000001)
					DBS.PDB[i].degree = ((int)(II*10.0 + 0.5)) / 10.0;
				else if(II<-0.00000000001)
					DBS.PDB[i].degree = -1.0*((int)(fabs(II)*10.0 + 0.5)) / 10.0;
				else
					DBS.PDB[i].degree = 0.0;          
				//标高
				//LC=(pzdm->BPD_array[i][0]-pzdm->X0)*pzdm->HBVSCALE+pzdm->K0;
				LC = pzdm->Trs_Coor_System(pzdm->BPD_array[i][0], MILE, TO_SLOPE);
				xlc = pzLinep->XLC1(LC);
				//			DBS.PDB[i].GH = GH;
				str.Format(L"%0.3lf",xlc);
				DBS.PDB[i].ml = _wtof(str);

				//HH=pzdm->BPD_array[i][1]-pzdm->Y0+pzdm->H0;    // 设计高程
				HH = pzdm->Trs_Coor_System(pzdm->BPD_array[i][1], LEVEL, TO_SLOPE);
				if (pzdm->DesHXSW == 2)
					str.Format(L"%0.2lf", HH);
				else if (pzdm->DesHXSW == 3)
					str.Format(L"%0.3lf", HH);
				else if (pzdm->DesHXSW == 1)
					str.Format(L"%0.1lf", HH);
				else
					str.Format(L"%0.0lf", HH);

				DBS.PDB[i].Level = _wtof(str);
				DBS.PDB[i].Rshu = pzdm->BPD_array[i][2];
				//竖曲线长度
				double deltaI;//坡度代数差
				if(i>0 && i<DBS.NPD-1)
					deltaI = fabs(DBS.PDB[i].degree - DBS.PDB[i-1].degree);
				else
					deltaI = 0.0;

				DBS.PDB[i].RLen = 2 * (DBS.PDB[i].Rshu / 200.0)*deltaI;
				DBS.PDB[i].Notes = " ";
			}
			DBS.PDB[0].Rshu = pzdm->DesHXSW;
			DBS.PDB[0].RLen = pzdm->Xsw;
			DBS.PDB[0].Notes="半径:标高小数位数;长度:坡度小数位数";
			DBS.Write_XLDbs(curprj,L"坡度表",dlg.m_RoadName,dlg.m_ZPXZ);
		}
		/*		if(DXorRXD==1)//单线
		{
		xlmdb.WR_XLDbs(curprj,L"坡度表");
		}
		else
		{ 
		double cml,ZxLevel,Zxii;
		ZxLevel=0;
		pzLinep->dmltockml(pzLinep->DLB[0].BGH,pzLinep->DLB[0].BLC,ckml);//起点主线里程
		cml=xlmdb.TYLC1(ckml,xlmdb.NDL,xlmdb.DLB);//算主线统一里程
		for(int j=0;j<NBPD-1;j++)
		{
		if(cml<=PD_array[j][0])
		break;
		}
		ZxLevel=(PD_array[j-1][1]+(PD_array[j][1]-PD_array[j-1][1])*(cml-PD_array[j-1][0])/(PD_array[j][0]-PD_array[j-1][0]));
		if(fabs(ZxLevel-xlmdb.PDB[0].Level)>0.04)//起点高程不匹配
		ads_printf(L"绕行段起点高程%0.2lf与一线接线点高程%0.2lf不匹配!\n",xlmdb.PDB[0].Level,ZxLevel);
		Zxii=(PD_array[j][1]-PD_array[j-1][1])*1000.0/(PD_array[j][0]-PD_array[j-1][0]);
		if(Zxii>0.00000000001)
		Zxii = ((int)(Zxii*10.0+0.5))/10.0;
		else if(Zxii<-0.00000000001)
		Zxii = -1.0*((int)(fabs(Zxii)*10.0+0.5))/10.0;
		else
		Zxii = 0.0;          
		if(fabs(Zxii-xlmdb.PDB[0].degree)>0.15)//起始坡度不匹配
		ads_printf(L"绕行段起始坡度%0.2lf与一线接线处坡度%0.2lf不匹配!\n",xlmdb.PDB[0].degree,Zxii);

		ZxLevel=0;
		pzLinep->dmltockml(pzLinep->DLB[pzLinep->NDL-1].EGH,pzLinep->DLB[pzLinep->NDL-1].BLC+pzLinep->DLB[pzLinep->NDL-1].DL,ckml);//终点里程
		cml=xlmdb.TYLC1(ckml,xlmdb.NDL,xlmdb.DLB);//算主线统一里程
		for(j=0;j<NBPD-1;j++)
		{
		if(cml<=PD_array[j][0])
		break;
		}
		ZxLevel=(PD_array[j-1][1]+(PD_array[j][1]-PD_array[j-1][1])*(cml-PD_array[j-1][0])/(PD_array[j][0]-PD_array[j-1][0]));
		if(fabs(ZxLevel-xlmdb.PDB[xlmdb.NPD-1].Level)>0.04)//终点高程不匹配
		ads_printf(L"绕行段终点高程%0.2lf与一线接线点高程%0.2lf不匹配!\n",xlmdb.PDB[xlmdb.NPD-1].Level,ZxLevel);
		Zxii=(PD_array[j][1]-PD_array[j-1][1])*1000.0/(PD_array[j][0]-PD_array[j-1][0]);
		if(Zxii>0.00000000001)
		Zxii = ((int)(Zxii*10.0+0.5))/10.0;
		else if(Zxii<-0.00000000001)
		Zxii = -1.0*((int)(fabs(Zxii)*10.0+0.5))/10.0;
		else
		Zxii = 0.0;          
		if(fabs(Zxii-xlmdb.PDB[xlmdb.NPD-2].degree)>0.15)//终止坡度不匹配
		ads_printf(L"绕行段终止坡度%0.2lf与一线接线处坡度%0.2lf不匹配!\n",xlmdb.PDB[xlmdb.NPD-2].degree,Zxii);
		}*/
	}

	if(dlg.m_infbri&&GlobalZDMTK.ZDMTK[izdm].pbri)
	{
		ads_printf(L"\n正在写大中桥表...");
		CString  GH;
		DBS.BriNum = GlobalZDMTK.ZDMTK[izdm].pbri->NBRI;  
		if(DBS.Bri) {delete []DBS.Bri,DBS.Bri=NULL;}
		if(DBS.BriNum>0)
		{
			DBS.Bri = new BRITAB[DBS.BriNum];
			for(i=0; i< DBS.BriNum;i++)
			{
				DBS.Bri[i].BriName=GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].BriName;
				DBS.Bri[i].StDml=GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].sdml;//pzLinep->XLC(GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].sml,GH);
				//			   DBS.Bri[i].BGH = GH;
				DBS.Bri[i].CenDml=GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].cdml;//pzLinep->XLC(GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].cml,GH);
				//			   DBS.Bri[i].CGH = GH;
				DBS.Bri[i].EdDml=GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].edml;//pzLinep->XLC(GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].eml,GH);
				//			   DBS.Bri[i].EGH = GH;
				DBS.Bri[i].KKSY = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].KKSY;
				DBS.Bri[i].SWHt = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].SWHt;
				DBS.Bri[i].XZ = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].XZ;
				DBS.Bri[i].HeadAng = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].HeadAng;
				DBS.Bri[i].TailAng = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].TailAng;
				DBS.Bri[i].ZZDK = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].ZZDK;
				DBS.Bri[i].YZDK = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].YZDK;
				DBS.Bri[i].BriLen = GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].eml-GlobalZDMTK.ZDMTK[izdm].pbri->Bri[i].sml;
			}
		}
		DBS.Write_XLDbs(curprj,L"大中桥",dlg.m_RoadName/*,dlg.m_ZPXZ*/);
		ads_printf(L"桥梁数据已写入%s—大中桥表!\n",curprj);
	}

	if(dlg.m_inftun&&GlobalZDMTK.ZDMTK[izdm].ptun!=NULL)
	{
		ads_printf(L"\n正在写隧道表...");
		CString  GH;
		DBS.TunNum = GlobalZDMTK.ZDMTK[izdm].ptun->NTun;  
		if(DBS.Tun) {delete []DBS.Tun,DBS.Tun=NULL;}
		if(DBS.TunNum>0)
		{
			DBS.Tun = new TUNTAB[DBS.TunNum];
			for(i=0; i<DBS.TunNum;i++)
			{
				DBS.Tun[i].TunName=GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].TunName ;
				DBS.Tun[i].StDml=GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].sdml;//pzLinep->XLC(GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].sml,GH);
				//			   DBS.Tun[i].BGH = GH;			   
				DBS.Tun[i].EdDml=GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].edml;//pzLinep->XLC(GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].eml,GH);
				DBS.Tun[i].XZ=GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].XZ;
				//			   DBS.Tun[i].EGH = GH;
				DBS.Tun[i].flag = "1";//0-表示明洞，1-表示隧道
				DBS.Tun[i].ZZDK = GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].ZZDK;
				DBS.Tun[i].YZDK = GlobalZDMTK.ZDMTK[izdm].ptun->Tun[i].YZDK;
			}
		}
		DBS.Write_XLDbs(curprj,L"隧道",dlg.m_RoadName/*,dlg.m_ZPXZ*/);
		ads_printf(L"隧道数据已写入%s—隧道表!\n",curprj);
	}

	//写纵地面线
	pdmx=GlobalZDMTK.ZDMTK[izdm].dmx;
	if(dlg.m_infdmx&&pdmx)
	{
		int NCM;
		double CM[5*MAXBPDNUM];
		CString  GH;
		if(pzdm)
			pzdm->PDdataToCM(NCM,CM);
		double ml;	   
		DBS.NTW = pdmx->NUM; 
		if(DBS.TWB) {delete []DBS.TWB; DBS.TWB=NULL;}
		if(DBS.NTW>0)
		{
			ads_printf(L"\n正在写纵地面线表...");
			DBS.TWB = new XLDataBase::TWTAB[DBS.NTW];
			for(i=0; i<pdmx->NUM;i++)
			{
				ml = pzLinep->XLC1(pdmx->DMXys[i].lc);
				str.Format(L"%0.2lf",ml);
				DBS.TWB[i].ml = _wtof(str);
				str.Format(L"%0.2lf",pdmx->DMXys[i].level);
				DBS.TWB[i].DMGC = _wtof(str);

			}
			DBS.Write_XLDbs(curprj,L"纵地面线表",dlg.m_RoadName);
		}
	}
	ads_printf(L"\nOK!");
	AfxMessageBox(L"保存纵断面方案成功！");
}


// This is command 'CalBri_Road'
void pCalBriCalBri()
{
	// TODO: Implement the command
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;

	DMX_ROAD  *pdmx;
	GTZDM_ROAD *pzdm;	
	double Th,MinLen,STH;
	int izdm=0;

	ads_initget(2+4,NULL);

	rc = ads_getreal(L"\n  请输入桥线分界填高<8米> ",&STH);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		STH = 8;
	rc = ads_getreal(L"\n  请输入设桥最小填高<15米> ",&Th);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		Th = 15;	
	rc = ads_getreal(L"\n  请输入自动设桥的最小桥长<20米> ",&MinLen);
	if(rc==RTNONE)
		MinLen = 20;

	rc = ads_entsel(L"\nSelect a  地面线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   }
	JD_CENTER *pz;
	pz = GlobalZDMTK.GetPM(PT);
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"dmx:无法找到平面方案!\n");



	acdbGetObjectId(eId, en);

	AcDbObject *pObj;

	if(acdbOpenObject(pObj, eId, AcDb::kForRead)==Acad::eOk)
		pObj->close();
	else
		return;

	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(DMX_ROAD::desc())) //是DMX实体,取数据
	{
		pdmx = DMX_ROAD::cast(pObj);
		//acdbOpenObject(pdmx, eId, AcDb::kForRead);
	}
	else
	{
		acutPrintf(L"所选实体不是地面线实体!\n");
		return;
	}

	rc = ads_entsel(L"\nSelect a  纵断面设计线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   
	}
	acdbGetObjectId(eId, en);
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)==Acad::eOk)
		pObj->close();
	else
		return;

	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) //是ZDM实体,取数据
	{
		pzdm = GTZDM_ROAD::cast(pObj);
		ZpId = eId;//全局变量
	}
	else
	{
		acutPrintf(L"所选实体不是纵断面设计线实体!\n");
		return;
	}
	//计算填挖高
	int NBri=0;
	struct Bridata
	{
		double sml,eml,cml;
		ACHAR SKml[80],EKml[80],CKml[80];
		int flag;//0 特大桥, 1大桥, 2中桥, 3小桥
	} Bri[500];//最多500座
	double BriMl[500][2];
	double sml,eml,len,xlc,Btwh,LenFoot,cml;
	CString  GH;
	int i,FootNum;
	rc = ads_getreal(L"\n  请输入搜索步长<10米> ",&LenFoot);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		LenFoot = 10;
	if(LenFoot > 0.001)
		i=floor((pzdm->CM[(pzdm->NCM-2)*5+0]-pzdm->CM[0])/LenFoot)+2;
	else
		return;

	struct DMXYS//地面线要素
	{
		double level,lc;//坐标及里程（真实坐标）
	}*FootXys=NULL;	 
	FootXys = new DMXYS[i];
	FootNum=0;
	cml = pzdm->CM[0];
	while(cml < pzdm->CM[(pzdm->NCM-2)*5+0]+0.001)
	{
		FootXys[FootNum].lc = cml;
		FootXys[FootNum].level = pdmx->NCheight(cml);
		cml+=LenFoot;
		FootNum++;
	}

	for(i=0; i<FootNum; i++)
	{
		double cml =  FootXys[i].lc;
		double earH = FootXys[i].level;
		double desh = pzdm->rdes_level(cml);
		double twh = desh -earH;
		if(twh >= Th)//可能设桥
		{
			//找起点
			if(i<1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh=twh;
				while(n>0 && Stwh>STH)//找终点
				{
					Btwh = Stwh;
					n--;
					cml = FootXys[n].lc;
					earH =  FootXys[n].level;
					desh =  pzdm->rdes_level(cml);
					Stwh = desh - earH;                    
				}
				if(Stwh < STH)
					sml = cml + (FootXys[n+1].lc-cml)*(STH-Stwh)/(Btwh-Stwh);
				else
					sml = cml;
			}


			while(i<FootNum-1 && twh>=STH)//找终点
			{
				Btwh = twh;
				i++;
				cml = FootXys[i].lc;
				earH =  FootXys[i].level;
				desh =  pzdm->rdes_level(cml);
				twh = desh - earH;                    
			}
			if(twh < STH)
				eml = cml - (cml - FootXys[i-1].lc)*(STH-Btwh)/(twh-Btwh);
			else
				eml = FootXys[i].lc;
			len = eml - sml;
			if(len>MinLen)//桥长至少10m
			{
				Bri[NBri].sml = sml;
				Bri[NBri].eml = eml;
				Bri[NBri].cml = 0.5*(eml+sml);
				xlc = pzLinep->XLC(sml,GH);
				_tcscpy(Bri[NBri].SKml,pzLinep->LCchr(GH,xlc,3));
				xlc = pzLinep->XLC(eml,GH);
				_tcscpy(Bri[NBri].EKml,pzLinep->LCchr(GH,xlc,3));
				xlc = pzLinep->XLC(Bri[NBri].cml,GH);
				_tcscpy(Bri[NBri].CKml,pzLinep->LCchr(GH,xlc,3));

				BriMl[NBri][0] = sml , BriMl[NBri][1] = eml ;
				NBri++;				 
			}
		}
		Btwh = twh;
	}


	for(i=0; i<NBri; i++)
		ads_printf(L"\n%d 中心里程:%s %s %s %0.2lf",i,Bri[i].CKml,Bri[i].SKml,Bri[i].EKml,Bri[i].eml-Bri[i].sml);

	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;
	GlobalZDMTK.ZDMTK[izdm].zdm = pzdm;
	BRI_ROAD *pBri;
	pBri=new BRI_ROAD(NBri,BriMl,pzdm,1,izdm);
	GlobalZDMTK.ZDMTK[izdm].pbri=pBri;
	BAS_DRAW_FUN basdraw;

	AcDbObjectId BriId=basdraw.AddEntityToDbs(pBri);	
	//
	//
	////加响应器
	GTZDMReactor *pGTZDMReactor = new GTZDMReactor(BriId,'B');
	if(acdbOpenObject(pObj, ZpId, AcDb::kForRead)==Acad::eOk)
	{
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		Zm->addReactor(pGTZDMReactor);
		Zm->close();

		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId=ZpId;
		pPair->pEntReactor=pGTZDMReactor;
		g_reactorList.append(pPair);
	}


	if(FootXys) 
	{
		delete []FootXys;	
		FootXys = NULL;
	}

}

// This is command 'DelBri_Road'
void pdelbridelbri()//删除桥梁
{
	int rc;
	AcGePoint2d PT;
	AcGePoint3d PT1;
	ads_point pt;
	ads_name en;
	AcDbObjectId eId;
	// TODO: Implement the command
	rc = ads_entsel(L"\n请选择要删除的桥梁: ", en, pt);
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   }
	PT1.set(pt[0],pt[1],pt[2]);
	JD_CENTER *pz;
	pz = GlobalZDMTK.GetPM(PT1);
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"delbri:无法找到平面方案!\n");


	acdbGetObjectId(eId, en);
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
	BRI_ROAD *pBri;
	if (pObj->isKindOf(BRI_ROAD::desc())) //是BRI实体,取数据
		pBri = BRI_ROAD::cast(pObj);
	else
	{
		acutPrintf(L"所选实体不是桥梁实体!\n");
		pObj->close();
		return;
	}
	PT.set(pt[X],pt[Y]);
	pBri->DelBri(PT);
	pObj->close();
}

// This is command 'AddBri_Road'
void paddBriaddBri()//增加桥梁
{
	// TODO: Implement the command
	ACHAR choice[5];
	int rc,NBri;
	BRI_ROAD *pBri;
	AcDbObject *pObj;
	AcGePoint2d SPT,EPT,PT;
	AcGePoint3d PT1;
	double BriMl[500][2],sml,eml,len;
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	GTZDM_ROAD *pzdm;	

	ads_initget(RSG_OTHER,L"A a N n");
	rc = ads_getkword(L"\n增加桥梁 (A) / 新建桥梁 (N): <A>  ", choice);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		_tcscpy(choice ,L"A");

	if(_tcscmp(choice ,L"A")==0 || _tcscmp(choice ,L"a")==0)//在既有桥梁数组中增加一座桥梁
	{
		rc = ads_entsel(L"\n请选择既有桥梁: ", en, pt);
		if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
		return;   }
		PT1.set(pt[0],pt[1],pt[2]);
		JD_CENTER *pz;
		pz = GlobalZDMTK.GetPM(PT1);
		if(pz)
			pzLinep = pz;
		else
			ads_printf(L"addbri:无法找到平面方案!\n");

		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
		if (pObj->isKindOf(BRI_ROAD::desc())) //是BRI实体,取数据
			pBri = BRI_ROAD::cast(pObj);
		else
		{
			acutPrintf(L"所选实体不是桥梁实体!\n");
			pObj->close();
			return;
		}
		ads_point pt;
		rc = ads_getpoint(NULL,L"\n请拾取桥梁起点里程...",pt); 
		if(rc==RTCAN)
		{
			pObj->close();
			return;
		}
		SPT.set(pt[X],pt[Y]);
		rc = ads_getpoint(NULL,L"\n请拾取桥梁终点里程...",pt); 
		if(rc==RTCAN)
		{
			pObj->close();
			return;
		}
		EPT.set(pt[X],pt[Y]);
		pBri->AddBri(SPT,EPT);
		pObj->close();
	}
	else//原桥梁个数为0,新建一座桥梁,
	{
		rc = ads_entsel(L"\nSelect a  纵断面设计线 : ", en, pt);
		if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
		return;   
		}

		PT1.set(pt[0],pt[1],pt[2]);
		PT1.set(pt[0],pt[1],pt[2]);
		JD_CENTER *pz;
		pz = GlobalZDMTK.GetPM(PT1);
		if(pz)
			pzLinep = pz;
		else
			ads_printf(L"addbri:无法找到平面方案!\n");
		int izdm;
		izdm = GlobalZDMTK.FindWichZdm(PT1);
		if(izdm<0)
			return;
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();

		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(GTZDM_ROAD::desc())) //是ZDM实体,取数据
		{
			pzdm = GTZDM_ROAD::cast(pObj);
			ZpId = eId;//全局变量
		}
		else
		{
			acutPrintf(L"所选实体不是纵断面设计线实体!\n");
			return;
		}
		//
		rc = ads_getpoint(NULL,L"\n请拾取桥梁起点里程...",pt); 
		if(rc==RTCAN)
			return;

		SPT.set(pt[X],pt[Y]);
		SPT = pzdm->Data_drawtoreal(SPT);
		sml = SPT.x; 

		rc =ads_getpoint(NULL,L"\n请拾取桥梁终点里程...",pt); 
		if(rc==RTCAN)
			return;
		EPT.set(pt[X],pt[Y]);
		EPT = pzdm->Data_drawtoreal(EPT);
		eml = EPT.x; 

		len = eml - sml;
		NBri = 0;
		if(len>0.0)//桥长至少10m
		{
			BriMl[NBri][0] = sml , BriMl[NBri][1] = eml ;
			NBri++;
		}


		BRI_ROAD *pBri;

		pBri=new BRI_ROAD(NBri,BriMl,pzdm,1,izdm);
		GlobalZDMTK.ZDMTK[izdm].pbri=pBri;
		BAS_DRAW_FUN basdraw;
		AcDbObjectId BriId=basdraw.AddEntityToDbs(pBri);	
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(BriId,'B');
		/*acdbOpenObject(pObj, ZpId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,ZpId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		Zm->addReactor(pGTZDMReactor);
		Zm->close();

		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId=ZpId;
		pPair->pEntReactor=pGTZDMReactor;
		g_reactorList.append(pPair);
	}	
}

// This is command 'CalTun_Road'
void pCALTUNCALTUN()
{
	// TODO: Implement the command
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;

	DMX_ROAD  *pdmx;
	GTZDM_ROAD *pzdm;	
	double Th,MinLen,STH;

	ads_initget(2+4,NULL);

	rc = ads_getreal(L"\n  请输入线隧分界挖深<12米> ",&STH);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		STH = 12;
	rc = ads_getreal(L"\n  请输入设置隧道最小挖深<30米> ",&Th);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		Th = 30;
	rc = ads_getreal(L"\n  请输入自动设隧的最小长度<20米> ",&MinLen);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		MinLen = 20;


	Th = -1.0*Th;
	STH = -1.0*STH;
	rc = ads_entsel(L"\nSelect a  地面线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   }
	JD_CENTER *pz;
	pz = GlobalZDMTK.GetPM(PT);
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"dmx:无法找到平面方案!\n");

	int izdm;
	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;

	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(DMX_ROAD::desc())) //是DMX实体,取数据
	{
		pdmx = DMX_ROAD::cast(pObj);
		//acdbOpenObject(pdmx, eId, AcDb::kForRead);
	}
	else
	{
		acutPrintf(L"所选实体不是地面线实体!\n");
		return;
	}

	rc = ads_entsel(L"\nSelect a  纵断面设计线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   
	}
	acdbGetObjectId(eId, en);
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) //是ZDM实体,取数据
	{
		pzdm = GTZDM_ROAD::cast(pObj);
		ZpId = eId;//全局变量
	}
	else
	{
		acutPrintf(L"所选实体不是纵断面设计线实体!\n");
		return;
	}
	//计算填挖高
	int NTun=0;
	struct Tundata
	{
		double sml,eml,cml;
		ACHAR SKml[80],EKml[80],CKml[80];		 
	} Tun[500];//最多500座
	double TunMl[500][2];
	double sml,eml,len,xlc,Btwh,LenFoot,cml;
	CString GH;
	int i,FootNum;

	rc = ads_getreal(L"\n  请输入搜索步长<10米> ",&LenFoot);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		LenFoot = 10;
	i=floor((pzdm->CM[(pzdm->NCM-2)*5+0]-pzdm->CM[0])/LenFoot)+2;
	struct DMXYS//地面线要素
	{
		double level,lc;//坐标及里程（真实坐标）
	}*FootXys=NULL;	 
	FootXys = new DMXYS[i];
	FootNum=0;
	cml = pzdm->CM[0];
	while(cml < pzdm->CM[(pzdm->NCM-2)*5+0]+0.001)
	{
		FootXys[FootNum].lc = cml;
		FootXys[FootNum].level = pdmx->NCheight(cml);
		cml+=LenFoot;
		FootNum++;
	}
	for(i=0; i<FootNum; i++)
	{
		double cml =  FootXys[i].lc;
		double earH = FootXys[i].level;
		double desh = pzdm->rdes_level(cml);
		double twh = desh -earH;
		if(twh <= Th)//需要设置隧道
		{
			//找起点
			if(i<1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh=twh;
				while(n>0 && Stwh<STH)//找终点
				{
					Btwh = Stwh;
					n--;
					cml = FootXys[n].lc;
					earH =  FootXys[n].level;
					desh =  pzdm->rdes_level(cml);
					Stwh = desh - earH;                    
				}
				if(Stwh > STH)
					sml = cml + (FootXys[n+1].lc-cml)*(STH-Stwh)/(Btwh-Stwh);
				else
					sml = cml;
			}

			while(i<FootNum-1 && twh<=STH)//找终点
			{
				Btwh = twh;
				i++;
				cml = FootXys[i].lc;
				earH =  FootXys[i].level;
				desh =  pzdm->rdes_level(cml);
				twh = desh - earH;                    
			}
			if(twh > Th && i<FootNum-1)
				eml =  FootXys[i-1].lc + (cml - FootXys[i-1].lc)*(STH-Btwh)/(twh-Btwh);
			else
				eml = FootXys[i].lc;
			len = eml - sml;
			if(len>MinLen)//隧道长至少20m
			{
				Tun[NTun].sml = sml;
				Tun[NTun].eml = eml;
				Tun[NTun].cml = 0.5*(eml+sml);
				xlc = pzLinep->XLC(sml,GH);
				_tcscpy(Tun[NTun].SKml,pzLinep->LCchr(GH,xlc,3));
				xlc = pzLinep->XLC(eml,GH);
				_tcscpy(Tun[NTun].EKml,pzLinep->LCchr(GH,xlc,3));
				xlc = pzLinep->XLC(Tun[NTun].cml,GH);
				_tcscpy(Tun[NTun].CKml,pzLinep->LCchr(GH,xlc,3));

				TunMl[NTun][0] = sml , TunMl[NTun][1] = eml ;
				NTun++;
			}
		}
		Btwh = twh;

	}
	for(i=0; i<NTun; i++)
		ads_printf(L"\n%d 中心里程:%s %s %s %0.2lf",i,Tun[i].CKml,Tun[i].SKml,Tun[i].EKml,Tun[i].eml-Tun[i].sml);

	TUN_ROAD *pTun;
	pTun=new TUN_ROAD(NTun,TunMl,pzdm,1,izdm);
	GlobalZDMTK.ZDMTK[izdm].ptun=pTun;
	BAS_DRAW_FUN basdraw;
	AcDbObjectId TunId = basdraw.AddEntityToDbs(pTun);	
	//加响应器
	GTZDMReactor *pGTZDMReactor = new GTZDMReactor(TunId,'T');
	/* acdbOpenObject(pObj, ZpId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,ZpId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	GTZDM_ROAD *Zm;
	Zm = GTZDM_ROAD::cast(pObj);
	Zm->addReactor(pGTZDMReactor);
	Zm->close();

	ReactorEntityPair *pPair = new ReactorEntityPair;
	pPair->entityId=ZpId;
	pPair->pEntReactor=pGTZDMReactor;
	g_reactorList.append(pPair);

}

// This is command 'DelTun_Road'
void pdeltundeltun()
{
	// TODO: Implement the command
	int rc;
	AcGePoint2d PT;
	AcGePoint3d PT1;
	ads_point pt;
	ads_name en;
	AcDbObjectId eId;
	// TODO: Implement the command
	rc = ads_entsel(L"\n请选择要删除的隧道: ", en, pt);
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   }
	PT1.set(pt[0],pt[1],pt[2]);
	JD_CENTER *pz;
	pz = GlobalZDMTK.GetPM(PT1);
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"deltun:无法找到平面方案!\n");

	acdbGetObjectId(eId, en);
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
	TUN_ROAD *pTun;
	if (pObj->isKindOf(TUN_ROAD::desc())) //是TUN实体,取数据
		pTun = TUN_ROAD::cast(pObj);
	else
	{
		acutPrintf(L"所选实体不是隧道实体!\n");
		pObj->close();
		return;
	}
	PT.set(pt[X],pt[Y]);
	pTun->DelTun(PT);
	pObj->close();
}

// This is command 'AddTun_Road'
void paddtunaddtun()
{
	// TODO: Implement the command
	ACHAR choice[5];
	int rc,NTun;
	TUN_ROAD *pTun;
	AcDbObject *pObj;
	AcGePoint2d SPT,EPT,PT;
	AcGePoint3d PT1;
	double TunMl[500][2],sml,eml,len;
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	GTZDM_ROAD *pzdm;	

	ads_initget(RSG_OTHER,L"A a N n");
	rc = ads_getkword(L"\n增加隧道 (A) / 新建隧道 (N): <A>  ", choice);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		_tcscpy(choice ,L"A");

	if(_tcscmp(choice ,L"A")==0 || _tcscmp(choice ,L"a")==0)//在既有桥梁数组中增加一座桥梁
	{
		rc = ads_entsel(L"\n请选择既有隧道: ", en, pt);
		if(rc==RTCAN)
			return;
		PT1.set(pt[0],pt[1],pt[2]);
		JD_CENTER *pz;
		pz = GlobalZDMTK.GetPM(PT1);
		if(pz)
			pzLinep = pz;
		else
			ads_printf(L"addbri:无法找到平面方案!\n");

		if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
		return;   }
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
		if (pObj->isKindOf(TUN_ROAD::desc())) //是Tun实体,取数据
			pTun = TUN_ROAD::cast(pObj);
		else
		{
			acutPrintf(L"所选实体不是隧道实体!\n");
			pObj->close();
			return;
		}
		ads_point pt;
		rc = ads_getpoint(NULL,L"\n请拾取隧道起点里程...",pt); 
		if(rc==RTCAN)
		{
			pObj->close();
			return;
		}
		SPT.set(pt[X],pt[Y]);
		rc = ads_getpoint(NULL,L"\n请拾取隧道终点里程...",pt); 
		if(rc==RTCAN)
		{
			pObj->close();
			return;
		}
		EPT.set(pt[X],pt[Y]);
		pTun->AddTun(SPT,EPT);
		pObj->close();
	}
	else//原桥梁个数为0,新建一座桥梁,
	{
		rc = ads_entsel(L"\nSelect a  纵断面设计线 : ", en, pt);
		if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
		return;   
		}
		PT1.set(pt[0],pt[1],pt[2]);
		JD_CENTER *pz;
		pz = GlobalZDMTK.GetPM(PT1);
		if(pz)
			pzLinep = pz;
		else
			ads_printf(L"addtun:无法找到平面方案!\n");
		int izdm;
		izdm = GlobalZDMTK.FindWichZdm(PT1);
		if(izdm<0)
			return;

		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();

		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(GTZDM_ROAD::desc())) //是ZDM实体,取数据
		{
			pzdm = GTZDM_ROAD::cast(pObj);
			ZpId = eId;//全局变量
		}
		else
		{
			acutPrintf(L"所选实体不是纵断面设计线实体!\n");
			return;
		}
		//
		rc = ads_getpoint(NULL,L"\n请拾取隧道起点里程...",pt); 
		if(rc==RTCAN)
			return;
		SPT.set(pt[X],pt[Y]);
		SPT = pzdm->Data_drawtoreal(SPT);
		sml = SPT.x; 

		rc = ads_getpoint(NULL,L"\n请拾取隧道终点里程...",pt); 
		if(rc==RTCAN)
			return;
		EPT.set(pt[X],pt[Y]);
		EPT = pzdm->Data_drawtoreal(EPT);
		eml = EPT.x; 

		len = eml - sml;
		NTun = 0;
		if(len>0.0)//桥长至少10m
		{
			TunMl[NTun][0] = sml , TunMl[NTun][1] = eml ;
			NTun++;
		}
		TUN_ROAD *pTun;
		pTun=new TUN_ROAD(NTun,TunMl,pzdm,1,izdm);
		GlobalZDMTK.ZDMTK[izdm].ptun=pTun;
		BAS_DRAW_FUN basdraw;
		AcDbObjectId TunId=basdraw.AddEntityToDbs(pTun);	
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(TunId,'T');
		/*acdbOpenObject(pObj, ZpId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,ZpId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		Zm->addReactor(pGTZDMReactor);
		Zm->close();

		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId=ZpId;
		pPair->pEntReactor=pGTZDMReactor;
		g_reactorList.append(pPair);

	}	
}

// This is command 'EDITBri_Road'
void pEditBriEditBri()//编辑桥
{
	// TODO: Implement the command
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	AcDbObject *pObj;
	//TODO: Implement the command
	rc = ads_entsel(L"\n请选择要编辑的桥梁或纵坡设计线实体: ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM) 
	{
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
	}

	JD_CENTER *pz;
	pz = GlobalZDMTK.GetPM(PT);
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"editbri:无法找到平面方案!\n");

	int izdm;
	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;	
	GTZDM_ROAD *pzdm;
	pzdm = GlobalZDMTK.GetZDM(PT);
	if(!pzdm)
		ads_printf(L"zm:无法找到纵面方案!\n");

	if(rc != RTNORM || !pObj->isKindOf(BRI_ROAD::desc()))//新建
	{
		ads_printf(L"\n所选实体不是桥梁实体，系统将为您新建桥梁实体!\n");
		int NBri=0;
		double BriMl[1][2];
		BriMl[0][0]=BriMl[0][1]=0.0;
		if(acdbOpenObject(pObj, ZpId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"请先指定纵坡方案!\n");
			return ;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		BRI_ROAD *pBri;
		pBri=new BRI_ROAD(NBri,BriMl,Zm,1,izdm);
		GlobalZDMTK.ZDMTK[izdm].pbri=pBri;
		BAS_DRAW_FUN basdraw;
		eId=basdraw.AddEntityToDbs(pBri);	
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(eId,'B');
		Zm->addReactor(pGTZDMReactor);
		Zm->close();

		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId=ZpId;
		pPair->pEntReactor=pGTZDMReactor;
		g_reactorList.append(pPair);
	}

	BriDlg dlg;
	dlg.m_eId = eId;
	dlg.zdm = pzdm;
	if(dlg.DoModal()==IDOK)
		GlobalZDMTK.ZDMTK[izdm].pbri = dlg.pBri;
}

// This is command 'EDITTun_Road'
void pEDITTunEDITTun()
{
	// TODO: Implement the command
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	AcDbObject *pObj;
	// TODO: Implement the command
	rc = ads_entsel(L"\n请选择要编辑的隧道或纵坡设计线实体: ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM) 
	{
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
	}	  
	JD_CENTER *pz;
	pz = GlobalZDMTK.GetPM(PT);
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"edittun:无法找到平面方案!\n");
	int izdm;
	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;
	GTZDM_ROAD *pzdm;
	pzdm = GlobalZDMTK.GetZDM(PT);
	if(!pzdm)
		ads_printf(L"zm:无法找到纵面方案!\n");


	if(rc != RTNORM || !pObj->isKindOf(TUN_ROAD::desc()))//新建
	{
		ads_printf(L"\n所选实体不是隧道实体，系统将为您新建隧道实体!\n");

		int NTun=0;
		double TunMl[1][2];
		TunMl[0][0]=TunMl[0][1]=0.0;
		if(acdbOpenObject(pObj, ZpId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"请先指定纵坡方案!\n");
			return ;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		TUN_ROAD *pTun;
		pTun=new TUN_ROAD(NTun,TunMl,Zm,1,izdm);
		GlobalZDMTK.ZDMTK[izdm].ptun=pTun;
		BAS_DRAW_FUN basdraw;
		eId=basdraw.AddEntityToDbs(pTun);	
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(eId,'T');
		Zm->addReactor(pGTZDMReactor);
		Zm->close();

		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId=ZpId;
		pPair->pEntReactor=pGTZDMReactor;
		g_reactorList.append(pPair);

	}

	TunDlg dlg;
	dlg.m_eId = eId;
	dlg.zdm = pzdm;

	if(dlg.DoModal()==IDOK)
		GlobalZDMTK.ZDMTK[izdm].ptun = dlg.pTun;
}

// This is command 'INPUTZMFA_Road'//单线	导入纵面
void pinputzmfainputzmfa()//从线路方案数据库中导入纵断面
{
	// TODO: Implement the command
	CString workdir;

	CInputZDM dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	//读取平面数据
	XLDataBase DBS;
	pzLinep = g_RoadWorkingDatabase.CreateHorizontalRoad(dlg.m_MdbName, dlg.m_RoadName);
	if (!pzLinep)
	{
		AfxMessageBox(L"未检测到平面方案，无法导入纵面！");
		return;
	}
	//if(DBS.Read_XLDbs(dlg.m_MdbName,L"线元表",dlg.m_RoadName)==0)
	//{
	//	AfxMessageBox(L"未检测到平面方案，无法导入纵面！");
	//	return;
	//}
	//DBS.Read_XLDbs(dlg.m_MdbName,L"纵断链表",dlg.m_RoadName);
	//if(DBS.XYNum>0)
	//{
	//	pzLinep=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
	//	if(DBS.NZDL>0)
	//		pzLinep->setDLB(DBS.ZDLArray,DBS.NZDL);
	//	_tcscpy(pzLinep->mdbname,dlg.m_MdbName);
	//	_tcscpy(pzLinep->RoadName,dlg.m_RoadName);
	//}

	//创建图框
	if(dlg.DrawMode==0)
	{
		GlobalZDMTK.NZDM++;//个数增加
		GlobalZDMTK.CurZDM = GlobalZDMTK.NZDM-1;
	}
	else 
		GlobalZDMTK.CurZDM = _wtoi(dlg.m_ZtfNo);
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].pm = pzLinep;
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].mdbname = dlg.m_MdbName;
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].RXDname = dlg.m_RoadName;

	//坡度表  
	int res,NBPD,i;
	int deshxsw,pdxsw;
	double PD_array[MAXBPDNUM][4],*dmx=NULL;
	NBPD = 0;
	deshxsw = 3;
	pdxsw=3;
	double Zmin=0,Zmax=300;
	if (dlg.DrawMode == 0 || dlg.m_PdxInf)
	{
		res = DBS.Read_XLDbs(dlg.m_MdbName, L"坡度表", pzLinep->RoadName, dlg.m_ZPXZ);
		if(res)//读坡度
		{			
			NBPD = DBS.NPD;
			if(NBPD > 0)
			{
				Zmin = 999999, Zmax = -999999;
			}
			for (i = 0; i < NBPD; i++)
			{
				PD_array[i][0] = pzLinep->TYLC(DBS.PDB[i].ml);
				PD_array[i][1] = DBS.PDB[i].Level;
				PD_array[i][2] = DBS.PDB[i].Rshu;			
				PD_array[i][3] = 0.0;
				Zmin = Zmin < PD_array[i][1] ? Zmin : PD_array[i][1] ;
				Zmax = Zmax > PD_array[i][1] ? Zmax : PD_array[i][1] ;
			} 
			if(DBS.PDB) 
			{
				deshxsw = (int)(PD_array[0][2]+0.1);
				pdxsw = int(DBS.PDB[0].RLen+0.001);
				pORI_SCALE.DesHXSW = deshxsw;
				pORI_SCALE.XSW = pdxsw;
				deshxsw = 3;
				pdxsw = 3;
				pORI_SCALE.DesHXSW = deshxsw;
				pORI_SCALE.XSW = pdxsw;
				delete[]DBS.PDB;
				DBS.PDB = NULL;
			}
		}
		else
		{
			ads_printf(L"\n读坡度表失败!");
			//	return;
		}
	}

	pORI_SCALE.X_ORI = dlg.m_OriX + 30;
	pORI_SCALE.Y_ORI = dlg.m_OriY + pORI_SCALE.PQXHIGH + pORI_SCALE.LCBHigh + 20;


	//地面线表
	BAS_DRAW_FUN basdraw;
	DMX_ROAD *pDmx=NULL;
	int num=0;
	double DesH = 0.0;

	AcDbObjectId  LDMXId=NULL;
	if(dlg.DrawMode==0||dlg.m_DmxInf)
	{
		res = DBS.Read_XLDbs(dlg.m_MdbName,L"纵地面线表",pzLinep->RoadName);
		if(res)//读地面线
		{
			num = DBS.NTW;
			if(num > 0)
			{
				dmx=new double [num*2];
				for(i=0;i<num;i++)
				{
					dmx[i*2] = pzLinep->TYLC(DBS.TWB[i].ml);
					dmx[i*2+1] = DBS.TWB[i].DMGC;

				} 
			}
			else
			{
				dmx=new double [2];
				dmx[0]=Zmin;
				dmx[1]=Zmax;
			}
			pDmx = new DMX_ROAD(num, dmx, GlobalZDMTK.CurZDM, 1);
			g_RoadWorkingDatabase.GetJPData(dlg.m_MdbName, dlg.m_RoadName, pDmx->m_JPInforArray_DMX); //传入接坡信息
			g_RoadWorkingDatabase.GetLevelCtrlData(dlg.m_MdbName, dlg.m_RoadName, pDmx->m_LevelCtrlPtArray_DMX);	//传入控制高程数组
			if (pDmx->m_LevelCtrlPtArray_DMX.size() > 0)
			{
				//考虑交叉点
				for (int i = 0; i < pDmx->m_LevelCtrlPtArray_DMX.size(); i++)
				{
					if (pDmx->m_LevelCtrlPtArray_DMX[i].Note.Find(L"交叉") >= 0)
					{
						if (pDmx->m_LevelCtrlPtArray_DMX[i].Style == 1)
						{
							DesH = pDmx->m_LevelCtrlPtArray_DMX[i].DesHSX;

							pDmx->ZMin = pDmx->ZMin < DesH ? pDmx->ZMin : (DesH - 3);
							pDmx->ZMax = pDmx->ZMax > DesH ? pDmx->ZMax : (DesH + 3);
						}
						else if (pDmx->m_LevelCtrlPtArray_DMX[i].Style == 2)
						{
							DesH = pDmx->m_LevelCtrlPtArray_DMX[i].DesHXX;
							pDmx->ZMin = pDmx->ZMin < DesH ? pDmx->ZMin : (DesH - 3);
							pDmx->ZMax = pDmx->ZMax > DesH ? pDmx->ZMax : (DesH + 3);
						}
					}
				}
			}
			pDmx->set_ori_scale();
			if(dmx) 
			{
				delete []dmx; 
				dmx=NULL;
			}
			if(DBS.TWB) 
			{
				delete []DBS.TWB;
				DBS.TWB=NULL;
			}
			double high = pDmx->get_axle_legth();
			LDMXId = basdraw.AddEntityToDbs(pDmx);
		}
		else
		{
			ads_printf(L"\n读纵地面线表失败!");
		}

		//曲线表	五线谱【线路平面】栏
		DMX_PQX_ROAD *pPqx = new DMX_PQX_ROAD();
		pPqx->m_Frame_Index = GlobalZDMTK.CurZDM;
		AcDbObjectId  LPQXId;
		LPQXId = basdraw.AddEntityToDbs(pPqx);

		//里程表	五线谱【里程桩号】栏（顶部）
		DMX_LCB_ROAD *pDmx_Lcb=new DMX_LCB_ROAD();
		AcDbObjectId  LLCBId;
		LLCBId=basdraw.AddEntityToDbs(pDmx_Lcb);
		pDmx_Lcb->close();
	}

	//纵坡	
	GTZDM_ROAD  *pzdmLine=NULL;
	AcDbObjectId  LIdzdm,TunId,ZdmPdId=NULL;
	ACHAR GH[8];
	_tcscpy(GH,L"K");
	if(NBPD>0)
	{
		if(dlg.DrawMode==0||dlg.m_PdxInf)
		{
			pzdmLine= new GTZDM_ROAD(NBPD,PD_array,&GH[0],0,deshxsw);
			//pzdmLine->setINIT(3,3,1,4,10); 
			//断链表	
			GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].zdm = pzdmLine;//记录纵断面			

			pzdmLine->DesHXSW = deshxsw;
			pzdmLine->Xsw = pdxsw;
			pzdmLine->Property = dlg.m_ZPXZ;
			g_RoadWorkingDatabase.GetLevelCtrlData(dlg.m_MdbName, dlg.m_RoadName, pzdmLine->m_LevelCtrlPtArray);	//传入控制高程数组
			g_RoadWorkingDatabase.GetJPData(dlg.m_MdbName, dlg.m_RoadName, pzdmLine->m_JPInforArray_slope); //传入接坡信息
			ZdmPdId=ZpId=pzdmLine->AddEntityToDbs(pzdmLine);
			pzdmLine->m_Frame_Index = GlobalZDMTK.CurZDM;
		}
	}

	//隧道表
	if(dlg.DrawMode==0|| dlg.m_TunInf)
	{
		res = DBS.Read_XLDbs(dlg.m_MdbName,L"隧道",pzLinep->RoadName/*,dlg.m_ZPXZ*/);
		if(res)//读隧道
		{
			num = DBS.TunNum;
			if(num>0)
			{
				TUN_ROAD *pTun;
				pTun=new TUN_ROAD(num,DBS.Tun,pzdmLine,0,GlobalZDMTK.CurZDM);
				GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].ptun=pTun;
				if(DBS.Tun) {delete []DBS.Tun;DBS.Tun=NULL;};
				BAS_DRAW_FUN basdraw;
				TunId=basdraw.AddEntityToDbs(pTun);	
			}
			if(num>0&&ZdmPdId)
			{
				//加响应器
				GTZDMReactor *pGTZDMReactor = new GTZDMReactor(TunId,'T');
				AcDbObject *pObj;
				if(acdbOpenObject(pObj,ZdmPdId,AcDb::kForWrite)!=Acad::eOk)	
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				GTZDM_ROAD *Zm;
				Zm = GTZDM_ROAD::cast(pObj);
				Zm->assertWriteEnabled();
				Zm->addReactor(pGTZDMReactor);
				Zm->close();
				ReactorEntityPair *pPair = new ReactorEntityPair;
				pPair->entityId = ZdmPdId;
				pPair->pEntReactor = pGTZDMReactor;
				g_reactorList.append(pPair);
			}
		}		
	}

	if (dlg.DrawMode == 0 || dlg.m_BriInf)
	{
		//桥梁表
		res = DBS.Read_XLDbs(dlg.m_MdbName, L"大中桥", pzLinep->RoadName/*,dlg.m_ZPXZ*/);
		if(res)//读大中桥
		{
			num = DBS.BriNum;
			AcDbObjectId BriId;
			if(num>0)
			{
				BRI_ROAD *pBri;
				pBri = new BRI_ROAD(num, DBS.Bri, pzdmLine, 0, GlobalZDMTK.CurZDM);
				GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].pbri = pBri;
				if (DBS.Bri) { delete[]DBS.Bri; DBS.Bri = NULL; };
				BAS_DRAW_FUN basdraw;
				BriId = basdraw.AddEntityToDbs(pBri);
			}
			if(num>0&&ZdmPdId)
			{
				//加响应器
				GTZDMReactor *pGTZDMReactor = new GTZDMReactor(BriId,'B');
				AcDbObject *pObj;
				if(acdbOpenObject(pObj,ZdmPdId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				GTZDM_ROAD *Zm;
				Zm = GTZDM_ROAD::cast(pObj);
				Zm->addReactor(pGTZDMReactor);
				Zm->close();

				ReactorEntityPair *pPair = new ReactorEntityPair;
				pPair->entityId = ZdmPdId;
				pPair->pEntReactor = pGTZDMReactor;
				g_reactorList.append(pPair);
			}
		}
	}

	//设计标高标注
	if(ZdmPdId)
	{
		DeshBz_ROAD *pDeshBz = new DeshBz_ROAD();
		pDeshBz->pzdm = pzdmLine;
		pDeshBz->m_Frame_Index = GlobalZDMTK.CurZDM;
		AcDbObjectId  DeshBzId;
		DeshBzId=basdraw.AddEntityToDbs(pDeshBz);
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(DeshBzId,'D');
		AcDbObject *pObj;
		if(acdbOpenObject(pObj,ZdmPdId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		Zm->addReactor(pGTZDMReactor);
		Zm->close();		
		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId = ZdmPdId;
		pPair->pEntReactor = pGTZDMReactor;
		g_reactorList.append(pPair);
	}

	//地面线标注
	if(LDMXId)
	{
		EarhBz_ROAD *pEarhBz = new EarhBz_ROAD();
		pEarhBz->m_Frame_Index = GlobalZDMTK.CurZDM;	//20190712添加	不再采取图框坐标范围定位图框号
		AcDbObjectId  EarhBzId;
		EarhBzId=basdraw.AddEntityToDbs(pEarhBz);
		//加响应器
		AcDbObject *pObj;
		DMXReactor *pDMXReactor = new DMXReactor(EarhBzId);
		if(acdbOpenObject(pObj,LDMXId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		DMX_ROAD *pdmx;
		pdmx = DMX_ROAD::cast(pObj);
		pdmx->addReactor(pDMXReactor);
		pdmx->close();		
		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId = LDMXId;
		pPair->pEntReactor = pDMXReactor;
		g_reactorList.append(pPair);
	}

	if (pDmx)
	{
		double Left = 0.0, Top = 0.0, Right = 0.0, Bottom = 0.0;
		pDmx->Get_Frame_Infor(Left, Top, Right, Bottom);
		BAS_DRAW_FUN::Zoom(Left, Top, Right, Bottom);
	}
}

//高程控制点
void SetLevelCtrl()
{
	CSetLevelCtrlDlg* SelLevelDlg = NULL;
	SelLevelDlg = new CSetLevelCtrlDlg;
	SelLevelDlg->Create(IDD_DIALOG_SetLevelCtrlData);
	SelLevelDlg->ShowWindow(SW_SHOW);
}

//接坡信息录入
void SetJPInfor()
{
	CJPInfor* JPInforDlg = NULL;
	JPInforDlg = new CJPInfor;
	JPInforDlg->Create(IDD_DIALOG_JPInfor);
	JPInforDlg->ShowWindow(SW_SHOW);
}

// This is command 'MdbInTun_Road'
void pMdbInTunMdbInTun()//
{
	// TODO: Implement the command
	int rc;
	AcDbObject *pObj;
	AcGePoint2d SPT,EPT;
	AcGePoint3d PT;
	ads_name en;  
	AcDbObjectId eId;
	GTZDM_ROAD *pzdm;	

	// TODO: Implement the command
	rc = ads_entsel(L"\nSelect a  纵断面设计线 : ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
		ads_printf(L"\nError during object selection"); 
		return;   
	}
	acdbGetObjectId(eId, en);
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();

	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
	if(pObj->isKindOf(GTZDM_ROAD::desc())) //是ZDM实体,取数据
	{
		pzdm = GTZDM_ROAD::cast(pObj);
		ZpId = eId;//全局变量
	}
	else
	{
		acutPrintf(L"所选实体不是纵断面设计线实体!\n");
		return;
	}

	int izdm;
	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;
	pzLinep=GlobalZDMTK.GetPM(PT);

	CString CurMdbFname;
	CurMdbFname = pzLinep->mdbname;
	XLDataBase xlmdb;
	if(xlmdb.Read_XLDbs(CurMdbFname,L"隧道",pzLinep->RoadName))//读大中桥
	{
		int num = xlmdb.TunNum;
		if(num>0)
		{
			TUN_ROAD *pTun;
			pTun=new TUN_ROAD(num,xlmdb.Tun,pzdm,0,izdm);
			GlobalZDMTK.ZDMTK[izdm].ptun=pTun;
			if(xlmdb.Tun)
			{
				delete []xlmdb.Tun;
				xlmdb.Tun =NULL;
			}
			BAS_DRAW_FUN basdraw;
			AcDbObjectId TunId=basdraw.AddEntityToDbs(pTun);	
			//加响应器
			GTZDMReactor *pGTZDMReactor = new GTZDMReactor(TunId,'T');
			AcDbObject *pObj;
			if(acdbOpenObject(pObj, ZpId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"请先指定纵坡方案!\n");
				pObj->close();
				return ;
			}            
			GTZDM_ROAD *Zm;
			Zm = GTZDM_ROAD::cast(pObj);
			Zm->addReactor(pGTZDMReactor);
			Zm->close();

			ReactorEntityPair *pPair = new ReactorEntityPair;
			pPair->entityId=ZpId;
			pPair->pEntReactor=pGTZDMReactor;
			g_reactorList.append(pPair);
		}
	}
}

// This is command 'INPUTBRI'
void pInputBriInputBri()
{
	// TODO: Implement the command
	int rc;
	AcDbObject *pObj;
	AcGePoint3d SPT,EPT,PT;
	ads_name en;
	AcDbObjectId eId;
	GTZDM_ROAD *pzdm;	

	// TODO: Implement the command
	rc = ads_entsel(L"\n选择纵断面设计线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   
	}
	acdbGetObjectId(eId,en);
	/*acdbOpenObject(pObj,eId,AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();

	int izdm;
	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;
	pzLinep=GlobalZDMTK.GetPM(PT);	
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) //是ZDM实体,取数据
	{
		pzdm = GTZDM_ROAD::cast(pObj);
		ZpId = eId;//全局变量
	}
	else
	{	
		acutPrintf(L"所选实体不是纵断面设计线实体!\n");
		return;
	}
	//读取当前方案名称;	 
	/*
	CString CurMdbFname;
	PmtFnameDlg dlg;//文件方式
	dlg.m_PmFname = GlobalZDMTK.ZDMTK[izdm].mdbname;
	//	dlg.m_PmFname = pzdm->p
	if(dlg.DoModal()!=IDOK)
	return;
	CurMdbFname = dlg.m_PmFname;*/

	CString CurMdbFname;
	CurMdbFname = pzLinep->mdbname;
	XLDataBase xlmdb;
	int res;

	res = xlmdb.Read_XLDbs(CurMdbFname,L"大中桥",pzLinep->RoadName);//读大中桥
	if(res)
	{		
		int num = xlmdb.BriNum;
		if(num>0)
		{
			BRI_ROAD *pBri;
			pBri=new BRI_ROAD(num,xlmdb.Bri,pzdm,0,izdm);
			GlobalZDMTK.ZDMTK[izdm].pbri=pBri;
			if(xlmdb.Bri) 
			{
				delete []xlmdb.Bri;
				xlmdb.Bri=NULL;
			}

			BAS_DRAW_FUN basdraw;
			AcDbObjectId BriId=basdraw.AddEntityToDbs(pBri);	
			//加响应器

			GTZDMReactor *pGTZDMReactor = new GTZDMReactor(BriId,'B');
			AcDbObject *pObj;
			if(acdbOpenObject(pObj, ZpId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"请先指定纵坡方案!\n");
				pObj->close();
				return ;
			}
			GTZDM_ROAD *Zm;
			Zm = GTZDM_ROAD::cast(pObj);
			Zm->addReactor(pGTZDMReactor);
			Zm->close();
			ReactorEntityPair *pPair = new ReactorEntityPair;
			pPair->entityId=ZpId;
			pPair->pEntReactor=pGTZDMReactor;
			g_reactorList.append(pPair);
		}
	}
}

// This is command 'MDYPD_Road'
void pMdyPDMdyPD()//调整坡度值
{

	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	GTZDM_ROAD *pzdm;
	AcDbObject *pObj;

	// TODO: Implement the command 
	int rc = ads_entsel(L"\n请选择要修改的坡度线: ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   
	}
	pzLinep=GlobalZDMTK.GetPM(PT);
	acdbGetObjectId(eId, en);
	/* acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();
	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
		pzdm =GTZDM_ROAD::cast(pObj);  	   
	else
	{
		acutPrintf(L"所选实体不是纵断面设计线实体!\n");
		return;
	}
	AcGePoint2d Drawpt,RealPt;
	Drawpt.x = PT.x;
	Drawpt.y = PT.y;

	RealPt = pzdm->Data_drawtoreal(Drawpt);

	double cml;
	cml = RealPt.x;
	pzdm->ScreenToReal();
	//找到要修改的坡度
	int i;
	for( i=0; i<pzdm->NBPD; i++)
	{
		if(cml<pzdm->PDarray[i][0])
			break;
	}
	int ipd;
	double PD_value;
	ipd = i-1;

	if(ipd<0 || ipd>pzdm->NBPD-1)//2005.3.9
	{
		ads_printf(L"无法找到对应坡段!\n");
		return ;
	}

	ACHAR mes[80];
	ads_initget(128,NULL);
	double pd;
	if(ipd<pzdm->NBPD-1)//2005.3.9
		pd = pzdm->BPD_array[ipd+1][3]; 
	//(pzdm->PDarray[ipd+1][1] - pzdm->PDarray[ipd][1])*1000.0/(pzdm->PDarray[ipd+1][0] - pzdm->PDarray[ipd][0]);
	else//2005.3.9
		pd = pzdm->BPD_array[ipd][3];	 

	int inf;
	inf=1;
	if(pd<0.0)
		inf=-1;
	double Xsw;
	Xsw = pzdm->Xsw;

	if(Xsw==1)//小数位取一位
		pd = inf*((int)(fabs(pd)*10+0.5))/10.0;//坡度
	else if(Xsw==2)//小数位取两位
		pd = inf*((int)(fabs(pd)*100+0.5))/100.0;//坡度
	else if(Xsw==3)//小数位取三位
		pd = inf*((int)(fabs(pd)*1000+0.5))/1000.0;//坡度
	else//小数位取零位);//坡度
		pd = inf*(int)(fabs(pd)+0.5);

	_stprintf(mes,L"\n  请输入新的坡度值 <%0.2lf> o/o: ",pd);
	int rt = ads_getreal(mes,&PD_value);
	if(rt==RTCAN)
		return;
	if(rt==RTNONE)
	{
		if(ipd>0)
			PD_value = pd;
		else
			PD_value = 0.0;
	}

	inf=1;
	if(PD_value<0.0)
		inf=-1;

	if(Xsw==1)//小数位取一位
		PD_value = inf*((int)(fabs(PD_value)*10+0.5))/10.0;//坡度
	else if(Xsw==2)//小数位取两位
		PD_value=inf*((int)(fabs(PD_value)*100+0.5))/100.0;//坡度
	else if(Xsw==3)//小数位取三位
		PD_value=inf*((int)(fabs(PD_value)*1000+0.5))/1000.0;//坡度
	else//小数位取零位);//坡度
		PD_value=inf*(int)(fabs(PD_value)+0.5);

	//     ads_printf(L"%lf\n",PD_value);
	ACHAR choice[5];
	ads_initget(RSG_OTHER,L"Y y N n C c");
	rt = ads_getkword(L"高程顺延/不顺延/ 修改坡长 :  Yes(Y) / No (N) / Change(C): <Y>  ", choice);
	if(rt==RTNONE)
		_tcscpy(choice ,L"Y");	 
	if(rt==RTCAN)
		return;

	int es=acdbOpenObject(pzdm, eId, AcDb::kForWrite);//获取对象指针，有READ
	if(es != Acad::eOk)return;

	int mode;
	if(_tcscmp(choice ,L"Y")==0 || _tcscmp(choice ,L"y")==0)//高程顺延
		mode=0;
	else if(_tcscmp(choice ,L"N")==0 || _tcscmp(choice ,L"n")==0)//高程不顺延边坡点里程不变
		mode=1;
	else//高程顺延,边坡点里程改变,前后坡度不变
		mode=2;

	pzdm->AdjustPD(ipd,PD_value,mode);
	pzdm->close();
}

// This is command 'PICKZDMDMX_Road'	屏幕采集
void pPickZdmDmxPickZdmDmx()//采集纵断面地面线
{
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	AcDbObject *pObj = NULL;

	rc = ads_entsel(L"\nSelect a  平面线路实体 : ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);

		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{		
			pObj->close();
			if(PickDmxBar!=NULL)
			{
				PickDmxBar->DestroyWindow();
				delete PickDmxBar; 
				PickDmxBar=NULL;
			}
			// TODO: Implement the command
			PickDmxBar = new PickDmxDockBar;			
			PickDmxBar->m_pm = JD_CENTER::cast(pObj);

		}
	}
	if(rc != RTNORM ||
		(!pObj->isKindOf(JD_CENTER::desc())))
	{
		if (pObj)
			pObj->close(); 
		ads_printf(L"所选实体不是平面实体,系统从当前方案数据库读取平面信息!\n");
		// TODO: Implement the command
		CString workdir,curpm;
		ACHAR str[180];
		FILE *fpr;
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		workdir=Cworkdir;
		curpm=workdir+L"\\project";
		if((fpr = _wfopen(curpm,L"r"))==NULL)   
			curpm="";
		else
		{
			fwscanf(fpr,L"%s",str);
			curpm=str;
			fclose(fpr);
		}

		//20191106	修改读取
		int len, i;
		len = curpm.GetLength();
		i = curpm.Find('$');
		while (i < len && i >= 0)
		{
			curpm.SetAt(i, ' ');
			i = curpm.Find('$');
		}

		curpm = workdir + L"\\data\\" + curpm + L".mdb";
		int num;

		XLDataBase DBS;
		//		DBS.Read_XLDbs(curpm,L"控制点表",L"主线");
		DBS.Read_XLDbs(curpm,L"线元表",L"主线");				
		DBS.Read_XLDbs(curpm,L"断链表",L"主线");
		if(DBS.XYNum>0)
		{
			pzLinep=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			//			pzLinep->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0)
				pzLinep->setDLB(DBS.DLArray,DBS.NDL);
			pzLinep->setRoadName(L"主线");
			pzLinep->setMdbName(curpm);		
		}
		PickDmxBar = new PickDmxDockBar;
		PickDmxBar->m_pm = pzLinep;
	}	
	// now display it
	PickDmxBar->Create(acedGetAcadFrame (),L"采集地面线");
	PickDmxBar->EnableDocking (CBRS_ALIGN_RIGHT|CBRS_ALIGN_LEFT);
	PickDmxBar->SetWindowText (L"地面线数据");

	PickDmxBar->RestoreControlBar();
	acedGetAcadFrame()->ShowControlBar (PickDmxBar, TRUE, TRUE);
}

void ZDJP()
{
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	AcDbObject *pObj;
	JD_CENTER *zdpm,*zxpm;

	rc = ads_entsel(L"\n选择匝道平面线路实体 : ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
			zdpm = JD_CENTER::cast(pObj);
		else
		{
			ads_printf(L"选择实体非平面实体!\n");
			return;
		}
	}
	else
		return;

	rc = ads_entsel(L"\n选择主线平面线路实体 : ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;

	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
			zxpm = JD_CENTER::cast(pObj);
		else
		{
			ads_printf(L"选择实体非平面实体!\n");
			return;
		}
	}
	else
		return;
	//
	ACHAR choice[5];
	HdmDes hdm;
	double NoseDmlONZX,NoseDmlONZD,NoseCml,sml;
	//    if(!hdm.FindWichHP(zxpm->RoadName,zdpm->RoadName,0,NoseDmlONZX,NoseDmlONZD);
	//    ads_initget(RSG_OTHER,L"T t F f");
	//    rc = ads_getkword(L"\n匝道出入口处与主线路面横坡: 同向坡(T) / 反向坡 (F): <T>  ", choice);
	//	if(rc==RTNONE)
	//		_tcscpy(choice,L"T");
	//	if(rc==RTCAN)
	//		return;
	CString str;
	str = choice;
	str.MakeUpper();
	hdm.ReadBDFile();
	BAS_DRAW_FUN::xlpoint PZ;
	double xzx,yzx,xzd,yzd,Lmhp,Ylmhp,Hht,Qht,dis;
	XLDataBase xlmdb,zdxlmdb;
	OutTAB tab;
	GTZDM_ROAD *zdm,*Zzdm,*Yzdm;
	int i;
	AcGePoint3d spt,ept,Apt;
	double cml,cml1,cml2,NosePt[2];
	CString RoadName = zdpm->RoadName;

	int LorR;
	AcGePoint3dArray PtArr;
	double RNose=0,Rtmp;
	double ptx[20],pty[20],sig[20];
	//  if(str=="T")//同向坡(T)
	//	 {
	int TorF;
	CString xl1,xl2;
	xl1.Format(L"%s,%s",zxpm->mdbname,zxpm->RoadName);
	xl1.MakeUpper();
	xl2.Format(L"%s,%s",zdpm->mdbname,zdpm->RoadName);
	xl2.MakeUpper();

	if(hdm.FindWichBDDml(xl1,xl2,0,NoseDmlONZX,NoseDmlONZD))
	{
		hdm.FindWichHP(xl1,xl2,0,TorF);
		if(TorF==1)//同向坡
		{
			sml = zxpm->TYLC(NoseDmlONZX);
			PZ.lc = sml;
			zxpm->xlpoint_pz(&PZ);
			LorR = zxpm->Judge2XLorR(PZ,zdpm->XYArray,zdpm->XYNum);
			if(LorR<0)
			{
				Zzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"左幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"左超高分段表",zxpm->RoadName);
				if(zdm==NULL&&Zzdm==NULL)
				{
					ads_alert(L"请先录入主线纵断面数据!\n");
					return;
				}
			}
			else
			{
				Yzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"右幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"右超高分段表",zxpm->RoadName);
				if(Yzdm==NULL&&zdm==NULL)
				{
					ads_alert(L"请先录入主线纵断面数据!\n");
					return;
				}
			}
			if(xlmdb.NCG<1)
			{
				ads_alert(L"请先计算主线超高加宽!\n");
				return;
			}		
			if(NoseDmlONZD>0.1)
			{
				sml = zdpm->TYLC(NoseDmlONZD);
				NoseCml = sml;
				PZ.lc = sml;
				zdpm->xlpoint_pz(&PZ);
				xzd = PZ.x,yzd = PZ.y;
				sml = zxpm->PROJ_ML(PZ.x,PZ.y);
				PZ.lc = sml;
				zxpm->xlpoint_pz(&PZ);
				xzx = PZ.x,yzx = PZ.y;
				dis = sqrt((xzd-xzx)*(xzd-xzx)+(yzd-yzx)*(yzd-yzx));

				hdm.CalLMHP(sml,xlmdb.NCG, xlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Hht = hdm.CalDesH(sml,Zzdm,zdm)+dis*Lmhp*0.01;
				else
					Hht = hdm.CalDesH(sml,Yzdm,zdm)+dis*Lmhp*0.01;
				//           Hht = zdm->rdes_level(sml)+dis*Lmhp*0.01;
				ads_printf(L"%0.3lf %0.3lf %0.3lf\n",NoseDmlONZD,dis,Lmhp);
				sml = NoseCml-5;
				PZ.lc = sml;
				zdpm->xlpoint_pz(&PZ);
				xzd = PZ.x,yzd = PZ.y;
				sml = zxpm->PROJ_ML(PZ.x,PZ.y);
				PZ.lc = sml;
				zxpm->xlpoint_pz(&PZ);
				xzx = PZ.x,yzx = PZ.y;
				dis = sqrt((xzd-xzx)*(xzd-xzx)+(yzd-yzx)*(yzd-yzx));

				hdm.CalLMHP(sml,xlmdb.NCG, xlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Qht = hdm.CalDesH(sml,Zzdm,zdm)+dis*Lmhp*0.01;
				else
					Qht = hdm.CalDesH(sml,Yzdm,zdm)+dis*Lmhp*0.01;

				//Qht = zdm->rdes_level(sml)+dis*Lmhp*0.01;

				for(i=0;i<GlobalZDMTK.NZDM;i++)
				{
					//				ads_printf(L"%0.3lf %0.3lf %0.3lf\n",Qht,Hht,GlobalZDMTK.ZDMTK[i].Y0);
					if(GlobalZDMTK.ZDMTK[i].RXDname == RoadName)
					{
						spt.x=(NoseCml-5-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;
						spt.y=(Qht-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0);
						ept.x=(NoseCml-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;
						ept.y=(Hht-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0);
						xlmdb.makeline(spt,ept,0,0);
					}
				}			
			}
		}
		else//前反向坡
		{
			ads_printf(L"\n正在自动计算拱顶线...");
			//1 求路拱线
			hdm.FindWichNose(xl1,xl2,0,RNose);		
			if(RNose < 0.001)
			{
				ads_printf(L"先计算鼻端里程!\n");
				return;
			}
			Rtmp = 0;

			cml =  zxpm->PROJ_ML(zdpm->JdCenArray[0].N,zdpm->JdCenArray[0].E);

			zxpm->CalNoseCen(zdpm,cml,1,0,0,0,RNose,cml1,cml2,NosePt);

			PZ.lc = cml1;
			zxpm->xlpoint_pz(&PZ);
			LorR = zxpm->Judge2XLorR(PZ,zdpm->XYArray,zdpm->XYNum);
			if(LorR<0)
			{
				Zzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"左幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"左超高分段表",zxpm->RoadName);

				zdxlmdb.Read_XLDbs(zdpm->mdbname,L"右超高分段表",zdpm->RoadName);
				if(Zzdm==NULL&&zdm==NULL)
				{
					ads_alert(L"请先录入主线纵断面数据!\n");
					return;
				}
			}
			else
			{
				Yzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"右幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"右超高分段表",zxpm->RoadName);
				zdxlmdb.Read_XLDbs(zdpm->mdbname,L"左超高分段表",zdpm->RoadName);
				if(Yzdm==NULL&&zdm==NULL)
				{
					ads_alert(L"请先录入主线纵断面数据!\n");
					return;
				}
			}
			if(xlmdb.NCG<1)
			{
				ads_alert(L"请先计算主线超高加宽!\n");
				return;
			}

			if(zdxlmdb.NCG<1)
			{
				ads_alert(L"请先计算匝道超高加宽!\n");
				return;
			}
			ads_printf(L"OK\n");

			ads_printf(L"正在计算控制点标高...");
			Rtmp = 0.001;
			while(Rtmp <= RNose+0.001)
			{
				zxpm->CalNoseCen(zdpm,cml,1,0,0,0,Rtmp,cml1,cml2,NosePt);
				//计算主线标高
				PZ.lc = cml1;
				zxpm->xlpoint_pz(&PZ);
				dis=sqrt((NosePt[0]-PZ.x)*(NosePt[0]-PZ.x)+(NosePt[1]-PZ.y)*(NosePt[1]-PZ.y));

				hdm.CalLMHP(cml1,xlmdb.NCG, xlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Qht = hdm.CalDesH(cml1,Zzdm,zdm)+dis*Lmhp*0.01;
				else
					Qht = hdm.CalDesH(cml1,Yzdm,zdm)+dis*Lmhp*0.01;

				PZ.lc = cml2;
				zdpm->xlpoint_pz(&PZ);
				dis=sqrt((NosePt[0]-PZ.x)*(NosePt[0]-PZ.x)+(NosePt[1]-PZ.y)*(NosePt[1]-PZ.y));
				hdm.CalLMHP(cml2,zdxlmdb.NCG, zdxlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Qht = Qht-dis*Lmhp*0.01;
				else
					Qht = Qht-dis*Lmhp*0.01;
				Apt.x = cml2;
				Apt.y = Qht;
				PtArr.append(Apt);
				Rtmp +=0.1*RNose;		
			}

			int j;

			for(i=0;i<GlobalZDMTK.NZDM;i++)
			{
				if(GlobalZDMTK.ZDMTK[i].RXDname == RoadName)
				{
					spt.x = (PtArr[0].x-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;
					spt.y = PtArr[0].y-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0;
					ptx[1] = spt.x,pty[1] = spt.y;
					sig[1] = 0.01;

					for(j=1;j<PtArr.length();j++)
					{
						ept.x =(PtArr[j].x-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;;
						ept.y = PtArr[j].y-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0;
						ptx[j+1] = ept.x,pty[j+1] = ept.y;
						sig[j+1] = 0.01;  
						xlmdb.makeline(spt,ept,1,0);
						spt = ept;
					}
					if(j>0)
					{					   
						double mwt=0,a,b,siga, sigb, chi2, q;
						zxpm->fit(ptx, pty, j, sig, mwt, a, b, siga, sigb, chi2, q);					   
						spt.x = ptx[1] , spt.y = b*spt.x+a;
						ept.x = ptx[j] , ept.y = pty[j];
						//					   ads_printf(L"q=%lf %lf %lf sx=%lf %lf ex=%lf %lf\n",q,a,b,spt.x,spt.y,ept.x,ept.y);
						xlmdb.makeline(spt,ept,0,0);				
					}

				}
			}
			//4 直线拟合


			//1 求路拱线
			PtArr.removeSubArray(0,PtArr.length()-1);

		}
	}
	//判别左右侧
	//判断二线位于左侧还是右侧	
	if(hdm.FindWichBDDml(xl1,xl2,1,NoseDmlONZX,NoseDmlONZD))
	{
		hdm.FindWichHP(xl1,xl2,1,TorF);
		if(TorF==1)
		{
			PZ.lc = sml;
			zxpm->xlpoint_pz(&PZ);
			LorR = zxpm->Judge2XLorR(PZ,zdpm->XYArray,zdpm->XYNum);
			if(LorR<0)
			{
				Zzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"左幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"左超高分段表",zxpm->RoadName);

			}
			else
			{
				Yzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"右幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"右超高分段表",zxpm->RoadName);
			}
			if(NoseDmlONZD>0.1)
			{
				sml = zdpm->TYLC(NoseDmlONZD);
				NoseCml = sml;
				PZ.lc = sml;
				zdpm->xlpoint_pz(&PZ);
				xzd = PZ.x,yzd = PZ.y;
				sml = zxpm->PROJ_ML(PZ.x,PZ.y);
				PZ.lc = sml;
				zxpm->xlpoint_pz(&PZ);
				xzx = PZ.x,yzx = PZ.y;
				dis = sqrt((xzd-xzx)*(xzd-xzx)+(yzd-yzx)*(yzd-yzx));

				hdm.CalLMHP(sml,xlmdb.NCG, xlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Hht = hdm.CalDesH(sml,Zzdm,zdm)+dis*Lmhp*0.01;
				else
					Hht = hdm.CalDesH(sml,Yzdm,zdm)+dis*Lmhp*0.01;
				//           Hht = zdm->rdes_level(sml)+dis*Lmhp*0.01;

				sml = NoseCml+5;
				PZ.lc = sml;
				zdpm->xlpoint_pz(&PZ);
				xzd = PZ.x,yzd = PZ.y;
				sml = zxpm->PROJ_ML(PZ.x,PZ.y);
				PZ.lc = sml;
				zxpm->xlpoint_pz(&PZ);
				xzx = PZ.x,yzx = PZ.y;
				dis = sqrt((xzd-xzx)*(xzd-xzx)+(yzd-yzx)*(yzd-yzx));

				hdm.CalLMHP(sml,xlmdb.NCG, xlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Qht = hdm.CalDesH(sml,Zzdm,zdm)+dis*Lmhp*0.01;
				else
					Qht = hdm.CalDesH(sml,Yzdm,zdm)+dis*Lmhp*0.01;
				//           Qht = zdm->rdes_level(sml)+dis*Lmhp*0.01;

				for(i=0;i<GlobalZDMTK.NZDM;i++)
				{
					if(GlobalZDMTK.ZDMTK[i].RXDname == RoadName)
					{
						spt.x=(NoseCml+5-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;
						spt.y=(Qht-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0);
						ept.x=(NoseCml-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;
						ept.y=(Hht-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0);
						xlmdb.makeline(ept,spt,0,0);
					}
				}
			}
		}
		else//反向坡
		{
			hdm.FindWichNose(xl1,xl2,1,RNose);		
			//ads_printf(L"直线拟合...");
			Rtmp = 0;
			cml =  zxpm->PROJ_ML(zdpm->XYArray[zdpm->XYNum][8],zdpm->XYArray[zdpm->XYNum][9]);

			zxpm->CalNoseCen(zdpm,cml,-1,0,0,0,RNose,cml1,cml2,NosePt);

			PZ.lc = cml1;
			zxpm->xlpoint_pz(&PZ);
			LorR = zxpm->Judge2XLorR(PZ,zdpm->XYArray,zdpm->XYNum);
			if(LorR<0)
			{
				Zzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"左幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"左超高分段表",zxpm->RoadName);
				zdxlmdb.Read_XLDbs(zdpm->mdbname,L"右超高分段表",zdpm->RoadName);
			}
			else
			{
				Yzdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"右幅",zxpm);
				zdm = tab.ReadZm(zxpm->mdbname,zdpm->RoadName,L"全幅",zxpm);
				xlmdb.Read_XLDbs(zxpm->mdbname,L"右超高分段表",zxpm->RoadName);
				zdxlmdb.Read_XLDbs(zdpm->mdbname,L"左超高分段表",zdpm->RoadName);
			}	
			Rtmp = 0.001;
			while(Rtmp <= RNose+0.001)
			{
				zxpm->CalNoseCen(zdpm,cml,-1,0,0,0,Rtmp,cml1,cml2,NosePt);
				//计算主线标高
				PZ.lc = cml1;
				zxpm->xlpoint_pz(&PZ);
				dis=sqrt((NosePt[0]-PZ.x)*(NosePt[0]-PZ.x)+(NosePt[1]-PZ.y)*(NosePt[1]-PZ.y));

				hdm.CalLMHP(cml1,xlmdb.NCG, xlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Qht = hdm.CalDesH(cml1,Zzdm,zdm)+dis*Lmhp*0.01;
				else
					Qht = hdm.CalDesH(cml1,Yzdm,zdm)+dis*Lmhp*0.01;

				PZ.lc = cml2;
				zdpm->xlpoint_pz(&PZ);
				dis=sqrt((NosePt[0]-PZ.x)*(NosePt[0]-PZ.x)+(NosePt[1]-PZ.y)*(NosePt[1]-PZ.y));
				hdm.CalLMHP(cml2,zdxlmdb.NCG, zdxlmdb.CG,Lmhp,Ylmhp);
				if(LorR<0)
					Qht = Qht-dis*Lmhp*0.01;
				else
					Qht = Qht-dis*Lmhp*0.01;
				Apt.x = cml2;
				Apt.y = Qht;
				//			  ads_printf(L"x=%lf y=%lf\n",Apt.x,Apt.y);
				PtArr.append(Apt);
				Rtmp +=0.1*RNose;		
			}

			for(i=0;i<GlobalZDMTK.NZDM;i++)
			{
				if(GlobalZDMTK.ZDMTK[i].RXDname == RoadName)
				{
					spt.x = (PtArr[0].x-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;
					spt.y = PtArr[0].y-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0;
					ptx[1] = spt.x,pty[1] = spt.y;
					sig[1] = 0.01;

					int j;
					for(j=1;j<PtArr.length();j++)
					{
						ept.x =(PtArr[j].x-zdpm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE+GlobalZDMTK.ZDMTK[i].X0;;
						ept.y = PtArr[j].y-GlobalZDMTK.ZDMTK[i].H0+GlobalZDMTK.ZDMTK[i].Y0;
						ptx[j+1] = ept.x,pty[j+1] = ept.y;
						sig[j+1] = 0.01;  
						xlmdb.makeline(spt,ept,1,0);
						spt = ept;
					}
					if(j>0)
					{					   
						double mwt=0,a,b,siga, sigb, chi2, q;
						zxpm->fit(ptx, pty, j, sig, mwt, a, b, siga, sigb, chi2, q);					   
						spt.x = ptx[1] , spt.y = b*spt.x+a;
						ept.x = ptx[j] , ept.y = pty[j];
						//					   ads_printf(L"q=%lf %lf %lf sx=%lf %lf ex=%lf %lf\n",q,a,b,spt.x,spt.y,ept.x,ept.y);
						xlmdb.makeline(ept,spt,0,0);				
					}

				}
			}
		}
		//4 直线拟合
		ads_printf(L"OK\n");

	}
}

//自动设计成图
void AutoDesignAndDraw()
{

}

// This is command 'EDITDMD_Road'	列表编辑（地面线）
void peditdmdeditdmd()
{
	// TODO: Implement the command
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;

	rc = ads_entsel(L"\nSelect a  地面线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); 
	return;   }

	acdbGetObjectId(eId, en);    
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(DMX_ROAD::desc())) //是DMX实体,取数据
	{
		pObj->close();
		DMX_ROAD* pEarthLine = DMX_ROAD::cast(pObj);
		EditDMXDlg dlg;
		
		int izdm = pEarthLine->Zdmno;
		if (izdm < 0 || izdm >= GlobalZDMTK.NZDM)
			return;
		pzLinep = GlobalZDMTK.ZDMTK[izdm].pm;

		//pzLinep = GlobalZDMTK.GetPM(PT);
		if (!pzLinep)
			return;
		//int izdm = GlobalZDMTK.FindWichZdm(PT);
		//if(izdm < 0)
		//	return;
		dlg.m_eId = eId;
		if(dlg.DoModal()==IDOK && dlg.pdmx->ZxOrBx==1)
			GlobalZDMTK.ZDMTK[izdm].dmx = dlg.pdmx;
	}
	else
	{
		pObj->close();
		acutPrintf(L"所选实体不是地面线实体!\n");
	}
}

//路面宽度
void SetLMK()
{

	//选择平面实体
	ZMZDPMFA dlg;
	if(dlg.DoModal()==IDOK)
	{
		LMSheetDlg *LMJMDlg=NULL;
		CString str;
		str.Format(L"请设置%s方案 %s 的路面宽数据",dlg.pzLine->mdbname , dlg.pzLine->RoadName );
		LMJMDlg = new LMSheetDlg(dlg.pzLine,str);
		if(LMJMDlg->DoModal()==IDOK)
		{
			//写数据库
			XLDataBase xlmdb;
			xlmdb.NLMK = LMJMDlg->zlmk.NLMK;
			if(xlmdb.NLMK>0)
			{
				xlmdb.lmk = new LMKdata[xlmdb.NLMK];
				for(int i=0; i<xlmdb.NLMK; i++)
					xlmdb.lmk[i] = LMJMDlg->zlmk.LMK[i];
				xlmdb.Write_XLDbs(dlg.pzLine->mdbname,L"左路面宽分段表",dlg.pzLine->RoadName);					
				ads_printf(L"\n左幅路面宽数据已成功保存在%s中!\n",dlg.pzLine->mdbname);
			}
			xlmdb.NLMK = LMJMDlg->ylmk.NLMK;
			if(xlmdb.NLMK>0)
			{
				if(xlmdb.lmk) delete []xlmdb.lmk;
				xlmdb.lmk = NULL;
				xlmdb.lmk = new LMKdata[xlmdb.NLMK];
				for(int i=0; i<xlmdb.NLMK; i++)
					xlmdb.lmk[i] = LMJMDlg->ylmk.LMK[i];
				xlmdb.Write_XLDbs(dlg.pzLine->mdbname,L"右路面宽分段表",dlg.pzLine->RoadName);					
				ads_printf(L"右幅路面宽数据已成功保存在%s中!\n",dlg.pzLine->mdbname);
			}
#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
			//////////////////////////////////////////////////////////////////////////
			//将路面宽度写入汇总表
			CString MdfRoadNmae = dlg.pzLine->GetProByMdbName(dlg.pzLine->mdbname);
			CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
			xlmdb.Read_XLDbs(MdfName,L"改移道路表", MdfRoadNmae);
			if (xlmdb.ModifyRoadSum<1)
			{
				xlmdb.ModifyRoadSum = 1;
				xlmdb.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[xlmdb.ModifyRoadSum];
				xlmdb.IniMdfRoadData(MdfRoadNmae, xlmdb.pModifyRoad[0]);
			}
			else if (xlmdb.ModifyRoadSum>1)
			{
				AfxMessageBox(MdfRoadNmae + "在改移道路表中有多条记录!");
				return;
			}

			xlmdb.pModifyRoad[0].LmWide = LMJMDlg->zlmk.LMK[0].fgdk + LMJMDlg->zlmk.LMK[0].lydk + LMJMDlg->zlmk.LMK[0].cxdk
				+ LMJMDlg->ylmk.LMK[0].fgdk + LMJMDlg->ylmk.LMK[0].lydk + LMJMDlg->ylmk.LMK[0].cxdk;

			xlmdb.pModifyRoad[0].LjWide = LMJMDlg->zlmk.LMK[0].fgdk + LMJMDlg->zlmk.LMK[0].lydk + LMJMDlg->zlmk.LMK[0].cxdk + LMJMDlg->zlmk.LMK[0].yljk + LMJMDlg->zlmk.LMK[0].tljk
				+ LMJMDlg->ylmk.LMK[0].fgdk + LMJMDlg->ylmk.LMK[0].lydk + LMJMDlg->ylmk.LMK[0].cxdk + LMJMDlg->ylmk.LMK[0].yljk + LMJMDlg->ylmk.LMK[0].tljk;

			xlmdb.pModifyRoad[0].LmArea = xlmdb.pModifyRoad[0].LmWide * xlmdb.pModifyRoad[0].RoadLen;

			xlmdb.Write_XLDbs(MdfName,L"改移道路表",MdfRoadNmae);
#endif
			
		}
		delete LMJMDlg;
	}
}




void PPCD()//爬坡车道
{
	//	CAcModuleResourceOverride resourceOverride;	
	//选择平面实体
	ZMZDPMFA dlg;
	if(dlg.DoModal()==IDOK)
	{
		CPPCDSheet *PPCDJMDlg=NULL;
		CString str;
		str.Format(L"请设置%s方案 %s 的爬坡车道数据",dlg.pzLine->mdbname , dlg.pzLine->RoadName );
		PPCDJMDlg = new CPPCDSheet(dlg.pzLine,str);
		if(PPCDJMDlg->DoModal()==IDOK)
		{
			//写数据库
			XLDataBase xlmdb;
			xlmdb.NPPCD = PPCDJMDlg->Zppcd.NPPCD;
			if(xlmdb.NPPCD>0)
			{
				xlmdb.ppcd = new BAS_DRAW_FUN::PPCDdata[xlmdb.NPPCD];
				for(int i=0; i<xlmdb.NPPCD; i++)
					xlmdb.ppcd[i] = PPCDJMDlg->Zppcd.PPCD[i];
				xlmdb.Write_XLDbs(dlg.pzLine->mdbname,L"左爬坡车道分段表",dlg.pzLine->RoadName);					
				ads_printf(L"\n左幅爬坡车道数据已成功保存在%s中!\n",dlg.pzLine->mdbname);
			}
			xlmdb.NPPCD = PPCDJMDlg->Yppcd.NPPCD;
			if(xlmdb.NPPCD>0)
			{
				if(xlmdb.ppcd) delete []xlmdb.ppcd;
				xlmdb.ppcd = NULL;
				xlmdb.ppcd = new BAS_DRAW_FUN::PPCDdata[xlmdb.NPPCD];
				for(int i=0; i<xlmdb.NPPCD; i++)
					xlmdb.ppcd[i] = PPCDJMDlg->Yppcd.PPCD[i];
				xlmdb.Write_XLDbs(dlg.pzLine->mdbname,L"右爬坡车道分段表",dlg.pzLine->RoadName);					
				ads_printf(L"右幅爬坡车道数据已成功保存在%s中!\n",dlg.pzLine->mdbname);
			}						
		}
		delete PPCDJMDlg;
	}

}

#include "BXLMK.h"
#include "SELBXKD.h"
//从polyline 实体中获取坐标 写入BXLMKD文件中
void GetLMK()
{
	//	CAcModuleResourceOverride resourceOverride;	
	//选择平面实体	
	ZMZDPMFA dlg;
	if(dlg.DoModal()==IDOK)
	{
		ads_name en;
		AcDbObjectId eId;
		AcGePoint3d PT;

		int rc = ads_entsel(L"\n请选择边线: ", en, asDblArray(PT));
		if (rc != RTNORM)
		{  
			ads_printf(L"\n实体选择错误!"); 
			return;   
		}		
		acdbGetObjectId(eId, en); 

		AcDbObject* pCurObj;
		Acad::ErrorStatus res;

		res = acdbOpenObject(pCurObj, eId, AcDb::kForRead);	
		pCurObj->close();
		if(res==Acad::eOk)
		{
			if(pCurObj->isKindOf(AcDbPolyline::desc()))				
			{  
				BAS_DRAW_FUN::BXLMKD OneBx;
				AcDbPolyline *pPolyline;
				AcGePoint3d pt;
				pPolyline=AcDbPolyline::cast(pCurObj);				
				int VertCount = pPolyline->numVerts();
				if(VertCount>0)
				{
					double sml,eml,dis,sx,sy,ex,ey;
					pPolyline->getPointAt(0,pt);
					sx = pt.y, sy=pt.x;
					sml = dlg.pzLine->PROJ_ML(sx, sy);
					pPolyline->getPointAt(VertCount-1,pt);
					ex = pt.y, ey=pt.x;
					eml = dlg.pzLine->PROJ_ML(ex, ey);
					sml = dlg.pzLine->XLC1(sml);
					eml = dlg.pzLine->XLC1(eml);

					CBXLMK lmkdlg;
					lmkdlg.m_bxsdml.Format(L"%0.3lf",sml);
					lmkdlg.m_bxedml.Format(L"%0.3lf",eml);
					//                    ads_printf(L"%lf %lf %lf %lf\n",OneBx.ptarr[0], OneBx.ptarr[1],OneBx.ptarr[(VertCount-1)*2], OneBx.ptarr[(VertCount-1)*2+1]);
					if(lmkdlg.DoModal()==IDOK)
					{
						sml = _wtof(lmkdlg.m_bxsdml);
						eml = _wtof(lmkdlg.m_bxedml);
						OneBx.bxsml = dlg.pzLine->TYLC(sml);
						OneBx.bxeml = dlg.pzLine->TYLC(eml);
						OneBx.bxinf = lmkdlg.bxinf;
						CArray<BAS_DRAW_FUN::Point,BAS_DRAW_FUN::Point> ptarr;
						ptarr.RemoveAll();
						BAS_DRAW_FUN::Point Apt;
						int i;
						for(i=0; i<VertCount;i++)
						{
							pPolyline->getPointAt(i,pt);	
							Apt.x=pt.y;
							Apt.y=pt.x;
							Apt.z=dlg.pzLine->PROJ_ML(Apt.x, Apt.y);
							if(Apt.z>OneBx.bxsml-0.001&&Apt.z<OneBx.bxeml+0.001)
								ptarr.Add(Apt);
						}
						OneBx.ptnum = ptarr.GetSize();
						OneBx.ptarr = new double[OneBx.ptnum*3];
						for(i=0;i<OneBx.ptnum;i++)
						{
							OneBx.ptarr[i*3]=ptarr[i].x;
							OneBx.ptarr[i*3+1]=ptarr[i].y;
							OneBx.ptarr[i*3+2]=ptarr[i].z;
						}

						OneBx.LorR = dlg.pzLine->JudgePTLorR(OneBx.ptarr[0],OneBx.ptarr[1],dis,dlg.pzLine->XYArray,dlg.pzLine->XYNum);						
						FILE *fpr,*fpw;
						CString mdbname=dlg.pzLine->mdbname;
						mdbname.MakeUpper();
						int pos,len,j,bxnum,k;
						pos = mdbname.Find(L"DATA");
						len = mdbname.GetLength();
						mdbname=mdbname.Right(len-pos-5);
						CString fname;
						BAS_DRAW_FUN ob;
						ob.ReadWorkdir();
						fname =Cworkdir + "\\DATA\\" + "BXLMKD." + mdbname + dlg.pzLine->RoadName;
						if((fpr=_wfopen(fname,L"r"))==NULL)
						{	
							fpw = _wfopen(fname,L"w");
							fwprintf(fpw,L"%d\n",1);
							fwprintf(fpw,L"%0.3lf %0.3lf %d %d %d\n",OneBx.bxsml,OneBx.bxeml,OneBx.LorR,OneBx.bxinf,OneBx.ptnum);
							for(j=0;j<OneBx.ptnum;j++)
								fwprintf(fpw,L"%0.3lf %0.3lf %0.3lf ",OneBx.ptarr[j*3],OneBx.ptarr[j*3+1],OneBx.ptarr[j*3+2]);
							fwprintf(fpw,L"\n");
							fclose(fpw);
						}
						else
						{
							BAS_DRAW_FUN::BXLMKD bxtmp;
							CArray<BAS_DRAW_FUN::BXLMKD,BAS_DRAW_FUN::BXLMKD>pBXArr;
							fwscanf(fpr,L"%d",&bxnum);
							for(k=0;k<bxnum;k++)
							{
								fwscanf(fpr,L"%lf%lf%d%d%d",&bxtmp.bxsml,&bxtmp.bxeml,&bxtmp.LorR,&bxtmp.bxinf,&bxtmp.ptnum);
								bxtmp.ptarr = new double[bxtmp.ptnum*3];
								for(j=0;j<bxtmp.ptnum;j++)
									fwscanf(fpr,L"%lf%lf%lf",&bxtmp.ptarr[j*3],&bxtmp.ptarr[j*3+1],&bxtmp.ptarr[j*3+2]);
								pBXArr.Add(bxtmp);
							}
							fclose(fpr); 
							bool hasoverlap=false;
							for(k=0;k<bxnum;k++)
							{
								if(pBXArr[k].LorR==OneBx.LorR&&pBXArr[k].bxinf==OneBx.bxinf)//当前输入的OneBx与已有的同侧同性质
								{
									if((pBXArr[k].bxsml>OneBx.bxsml&&pBXArr[k].bxsml<OneBx.bxeml)||//输入的OneBx与pBXArr[k]
										(pBXArr[k].bxeml>OneBx.bxsml&&pBXArr[k].bxeml<OneBx.bxeml)||//有重叠部分
										(OneBx.bxsml>pBXArr[k].bxsml&&OneBx.bxsml<pBXArr[k].bxeml)||
										(OneBx.bxeml>pBXArr[k].bxsml&&OneBx.bxeml<pBXArr[k].bxeml))
									{
										hasoverlap=true;
										CSELBXKD selbxdlg;
										CString side,bxname;
										if(OneBx.LorR==-1)
											side = "左侧";
										else
											side = "右侧";
										bxname = "硬路肩边线";
										if(OneBx.bxinf==0)
											bxname = "中央分隔带边线";
										else if(OneBx.bxinf==1)
											bxname = "路缘带边线";
										else if(OneBx.bxinf==2)
											bxname = "车行道边线";
										else if(OneBx.bxinf==3)
											bxname = "硬路肩边线";
										else if(OneBx.bxinf==4)
											bxname = "土路肩边线";
										selbxdlg.m_inf=side+bxname;
										selbxdlg.m_newinf.Format(L"%0.3lf～%0.3lf",dlg.pzLine->XLC1(OneBx.bxsml),dlg.pzLine->XLC1(OneBx.bxeml));
										selbxdlg.m_yyinf.Format(L"%0.3lf～%0.3lf",dlg.pzLine->XLC1(pBXArr[k].bxsml),dlg.pzLine->XLC1(pBXArr[k].bxeml));
										if(selbxdlg.DoModal()==IDOK)
										{
											BAS_DRAW_FUN::BXLMKD newbx;
											ob.MergeBX(pBXArr[k],OneBx,selbxdlg.selinf,newbx);
											pBXArr.RemoveAt(k);
											pBXArr.Add(newbx);
											break;
										}
										else
										{
											if(OneBx.ptarr)
											{
												delete []OneBx.ptarr;
												OneBx.ptarr=NULL;
											}
											for(j=0;j<bxnum;j++)
											{
												if(pBXArr[j].ptarr)
												{
													delete []pBXArr[j].ptarr;
													pBXArr[j].ptarr=NULL;
												}
											}
											return;
										}
									}
								}
							}
							if(hasoverlap==false)
								pBXArr.Add(OneBx);

							fpw = _wfopen(fname,L"w");
							fwprintf(fpw,L"%d\n",pBXArr.GetSize());
							for(k=0;k<pBXArr.GetSize();k++)
							{	
								fwprintf(fpw,L"%0.3lf %0.3lf %d %d %d\n",pBXArr[k].bxsml,pBXArr[k].bxeml,pBXArr[k].LorR,pBXArr[k].bxinf,pBXArr[k].ptnum);
								for(j=0;j<pBXArr[k].ptnum;j++)
									fwprintf(fpw,L"%0.3lf %0.3lf %0.3lf ",pBXArr[k].ptarr[j*3],pBXArr[k].ptarr[j*3+1],pBXArr[k].ptarr[j*3+2]);
								fwprintf(fpw,L"\n");
							}
							fclose(fpw);
							for(j=0;j<pBXArr.GetSize();j++)
							{
								if(pBXArr[j].ptarr)
								{
									delete []pBXArr[j].ptarr;
									pBXArr[j].ptarr=NULL;
								}
							}
						}

					}
					if(OneBx.ptarr)
					{
						delete []OneBx.ptarr;
						OneBx.ptarr=NULL;
					}

				}
			}
			else
				ads_printf(L"\n实体选择错误!"); 
		} 
	}
}

#include "EDITLMK.h"
//路面宽度
CEDITLMK *plmkdlg=NULL;
void EditLMK()
{
	//选择平面实体	
	ZMZDPMFA dlg;
	if(dlg.DoModal()==IDOK)
	{
		FILE *fpr,*fpw;
		CString mdbname=dlg.pzLine->mdbname;
		mdbname.MakeUpper();
		int pos,len,j,bxnum,k;
		pos = mdbname.Find(L"DATA");
		len = mdbname.GetLength();
		mdbname=mdbname.Right(len-pos-5);
		CString fname;
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		fname =Cworkdir + "\\DATA\\" + "BXLMKD." + mdbname + dlg.pzLine->RoadName;
		if((fpr=_wfopen(fname,L"r"))!=NULL)
		{	
			BAS_DRAW_FUN::BXLMKD bxtmp;
			AcGePoint2dArray PtAy;
			AcGePoint2d Pt;
			double tmp;
			if(plmkdlg)
			{
				delete plmkdlg;
				plmkdlg = NULL;
			}
			plmkdlg =  new CEDITLMK;
			plmkdlg->pm = dlg.pzLine;
			fwscanf(fpr,L"%d\n",&bxnum);
			plmkdlg->BxNum = bxnum;
			for(k=0;k<bxnum;k++)
			{	
				fwscanf(fpr,L"%lf%lf%d%d%d",&bxtmp.bxsml,&bxtmp.bxeml,&bxtmp.LorR,&bxtmp.bxinf,&bxtmp.ptnum);
				PtAy.removeSubArray(0,PtAy.length()-1);
				for(j=0;j<bxtmp.ptnum;j++)
				{
					fwscanf(fpr,L"%lf%lf%lf",&Pt.y,&Pt.x,&tmp);
					//					if(Pt.y>0.1&&Pt.x>0.1)
					PtAy.append(Pt);
				}
				bxtmp.eID=BAS_DRAW_FUN::makepolyline(PtAy,0,0.0);
				bxtmp.ptarr = NULL;
				bxtmp.ptnum = 0;
				plmkdlg->pBXArr.Add(bxtmp);
			}			
			fclose(fpr);

			plmkdlg->Create(IDD_DIALOG_EDITLMK,acedGetAcadFrame());			
			plmkdlg->ShowWindow(SW_SHOW);			
		}
		else
			ads_printf(L"所选方案没有提取过路面宽!\n");
	}
}

#include "BDCALDlg.h"
void CalBD()
{
	BDCALDlg *BdDlg;
	BdDlg = new BDCALDlg;
	BdDlg->Create(IDD_DIALOG_CALBD,acedGetAcadFrame());
	BdDlg->ShowWindow(SW_SHOW);
}

ChooseLMX *Lmxdlg=NULL;
void DrawLmx()
{
	//选择平面线路实体
	if(Lmxdlg!=NULL){delete Lmxdlg;Lmxdlg=NULL;}
	Lmxdlg=new ChooseLMX(acedGetAcadFrame());		
	Lmxdlg->Create(IDD_DIALOG_CheckLmx,acedGetAcadFrame());
	Lmxdlg->ShowWindow(SW_SHOW);
	//
	//	ChooseLMX  dlg;
	//	if(dlg.DoModal()!=IDOK)
	//	  return;
	//绘路面线
}

// This is command 'DBX' 绘制线路偏置边线 2005.07.12 ldf
#include "DrawBianXianDlg.h"
CDrawBianXianDlg* pDrawBianXianDlg = NULL;
void DrawPZX()
{
	// TODO: Implement the command
	if(pDrawBianXianDlg) 
	{
		delete pDrawBianXianDlg;
		pDrawBianXianDlg = NULL;
	}
	pDrawBianXianDlg = new CDrawBianXianDlg;		
	pDrawBianXianDlg->Create(IDD_DRAW_BX);
	pDrawBianXianDlg->ShowWindow(SW_SHOW);
}

void SetCGBZ() //设置超高标准
{
	// TODO: Add your command handler code here
	SetCGBZDlg dlg;
	dlg.DoModal();
}

void SetJKBZ() //设置加宽标准
{
	// TODO: Add your command handler code here
	CSetJKNorm dlg;
	dlg.DoModal();
}

//计算超高加宽分段
void CalCGFD()
{
	XLDataBase zxmdb;
	XLDataBase xlmdb;
	JD_CENTER *zxpm;
	ZMZDPMFA dlg1;
	AcGePoint3dArray ptarr,ptarr1;

	if(dlg1.DoModal()==IDOK)
	{
		//1.选择平面方案
		JD_CENTER *pm=NULL;
		pm = dlg1.pzLine;
		if(pm==NULL)
		{
			ads_alert(L"请先指定平面方案!");
			return;
		}
		if(pm->mdbname==L""||pm->mdbname==L"未命名")
		{
			ads_alert(L"请先保存道路方案，否则超高数据无法保存!");
			return;
		}
		if(pm->DV<1)
		{
			ads_alert(L"请先设定设计车速，否则超高数据无法保存!");
			return;
		}

		if(pm->RoadName==L""||pm->RoadName==L"未命名")
		{
			ads_alert(L"请先命名道路方案，否则超高数据无法保存!");
			return;
		}

		//4.显示对话框，输入超高渐变率
		SetCGJSCS cgdlg(pm);	
		if(cgdlg.DoModal()!=IDOK)
			return;
		//5.逐段线元计算超高分段
		calcg ccalcg(pm, _wtof(cgdlg.m_ZxCgJbl), _wtof(cgdlg.m_HxCgJbl), cgdlg.CgLenMode, cgdlg.CgMode);
		if(ccalcg.indata()!=1)	//读取超高和加宽数据
			return ;
		ccalcg.Cal_Suph();


		CString str;
		str.Format(L"计算%s方案 %s 的超高加宽分段数据",pm->mdbname , pm->RoadName );
		SuphFDSheet dlg(str);
		dlg.ZCG.mdbname = pm->mdbname;
		dlg.ZCG.roadname = pm->RoadName;
		dlg.YCG.mdbname = pm->mdbname;
		dlg.YCG.roadname = pm->RoadName;

		//传递左侧超高数据
		dlg.ZCG.NCG = ccalcg.ZcgNm;
		int i;
		for(i=0;i<ccalcg.ZcgNm;i++)
		{ 
			ccalcg.Zcg[i].xlc = pm->XLC1(ccalcg.Zcg[i].cml);
			dlg.ZCG.CG[i] = ccalcg.Zcg[i];
		}
		//传递右侧超高数据
		dlg.YCG.NCG = ccalcg.YcgNm;	
		for(i=0;i<ccalcg.YcgNm;i++)
		{
			ccalcg.Ycg[i].xlc = pm->XLC1(ccalcg.Ycg[i].cml);
			dlg.YCG.CG[i] = ccalcg.Ycg[i];
		}

		xlmdb.Read_XLDbs(pm->mdbname,L"线路属性",pm->RoadName);
		HdmDes hdm;
		if(xlmdb.RoadXZ.Grade=="匝道")
		{
			//取鼻端同坡或反坡信息
			hdm.ReadBDFile();				   
			if(hdm.NBIDUAN>0)//考虑休正
			{
				CString xl2;
				xl2.Format(L"%s,%s",pm->mdbname,pm->RoadName);
				int TorF;
				CString ZXmdbname,ZXRoadname;
				double ZXdml,ZDdml;
				AcGePoint3d spt, ept, pt;
				double flddml1,flddml2;
				if(hdm.FindWichLinkRoad(xl2,0,TorF,ZXmdbname,ZXRoadname,ZXdml,ZDdml,flddml1,flddml2))					  
				{
					//				   zxmdb.Read_XLDbs(ZXmdbname,L"控制点表",ZXRoadname);
					zxmdb.Read_XLDbs(ZXmdbname,L"线元表",ZXRoadname);						   
					zxmdb.Read_XLDbs(ZXmdbname,L"断链表",ZXRoadname);
					if(zxmdb.XYNum>0)
					{
						zxpm=new JD_CENTER(zxmdb.XYArray,zxmdb.XYNum);//主线对象
						//					   zxpm->SetJdCenterArray(zxmdb.JdCenArray,zxmdb.NJdCen);
						if(zxmdb.NDL>0)
							zxpm->setDLB(zxmdb.DLArray,zxmdb.NDL);

						_tcscpy(zxpm->mdbname,ZXmdbname);
						_tcscpy(zxpm->RoadName,ZXRoadname);
						if(TorF==1)//同坡					     							   
							ccalcg.AdjustCGByZX(zxpm,pm,ZXdml,ZDdml,dlg.ZCG.NCG,dlg.ZCG.CG,dlg.YCG.NCG,dlg.YCG.CG);						   			   
						else//反坡
							ccalcg.AdjustCGByZX_FX(zxpm,pm,ZXdml,ZDdml,_wtof(cgdlg.m_flblen),dlg.ZCG.NCG,dlg.ZCG.CG,dlg.YCG.NCG,dlg.YCG.CG);						   			   
					}
					else
					{
						ads_printf(L"请先录入主线平面数据!\n");
					}
				}
				else
				{
					ads_printf(L"请先计算%s分流鼻里程!\n",xl2);
				}

				if(hdm.FindWichLinkRoad(xl2,1,TorF,ZXmdbname,ZXRoadname,ZXdml,ZDdml,flddml1,flddml2))					  
				{
					//				   zxmdb.Read_XLDbs(ZXmdbname,L"控制点表",ZXRoadname);
					zxmdb.Read_XLDbs(ZXmdbname,L"线元表",ZXRoadname);						   
					zxmdb.Read_XLDbs(ZXmdbname,L"断链表",ZXRoadname);
					if(zxmdb.XYNum>0)
					{
						zxpm=new JD_CENTER(zxmdb.XYArray,zxmdb.XYNum);//主线对象
						//					   zxpm->SetJdCenterArray(zxmdb.JdCenArray,zxmdb.NJdCen);
						if(zxmdb.NDL>0)
							zxpm->setDLB(zxmdb.DLArray,zxmdb.NDL);

						_tcscpy(zxpm->mdbname,ZXmdbname);
						_tcscpy(zxpm->RoadName,ZXRoadname);
						if(TorF==1)//同坡					     							   
							ccalcg.AdjustCGByZX1(zxpm,pm,ZXdml,ZDdml,dlg.ZCG.NCG,dlg.ZCG.CG,dlg.YCG.NCG,dlg.YCG.CG);						   			   
						else
							ccalcg.AdjustCGByZX_FX1(zxpm,pm,ZXdml,ZDdml,_wtof(cgdlg.m_hlblen),dlg.ZCG.NCG,dlg.ZCG.CG,dlg.YCG.NCG,dlg.YCG.CG);						   			   
					}
					else
					{
						ads_printf(L"请先录入主线平面数据!\n");
					}
				}
				else
				{
					ads_printf(L"请先计算%s分流鼻里程!\n",xl2);
				}
			}
			else
			{
				ads_printf(L"请先计算鼻端里程!\n");
			}
		}//调整完毕

		//传递左侧加宽数据
		dlg.ZJK.NCG = ccalcg.ZjkNm;
		for(i=0;i<ccalcg.ZjkNm;i++)
		{ 
			ccalcg.Zjk[i].xlc = pm->XLC1(ccalcg.Zjk[i].cml);
			dlg.ZJK.CG[i] = ccalcg.Zjk[i];
		}
		//传递右侧加宽数据
		dlg.YJK.NCG = ccalcg.YjkNm;
		for(i=0;i<ccalcg.YjkNm;i++)
		{
			ccalcg.Yjk[i].xlc = pm->XLC1(ccalcg.Yjk[i].cml);
			dlg.YJK.CG[i] = ccalcg.Yjk[i];
		}

		//展示计算结果
		if(dlg.DoModal()==IDOK)
		{
			//存储左超高数据
			xlmdb.NCG = dlg.ZCG.NCG;
			int i;
			for(i=0;i<dlg.ZCG.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.ZCG.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.ZCG.CG[i].cg;

			}
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"左超高分段表",pm->RoadName);
				ads_printf(L"\n左幅超高分段数据已成功保存在%s中!\n",pm->mdbname);
			}

			//存储右超高数据
			xlmdb.NCG = dlg.YCG.NCG;
			for(i=0;i<dlg.YCG.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.YCG.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.YCG.CG[i].cg;

			}			
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"右超高分段表",pm->RoadName);
				ads_printf(L"\n右幅超高分段数已成功保存在%s中!\n",pm->mdbname);
			}

			//存储左加宽数据
			xlmdb.NCG = dlg.ZJK.NCG;
			for(i=0;i<dlg.ZJK.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.ZJK.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.ZJK.CG[i].cg;
			}
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"左加宽分段表",pm->RoadName);
				ads_printf(L"\n左幅加宽分段数据已成功保存在%s中!\n",pm->mdbname);
			}

			//存储右加宽数据
			xlmdb.NCG = dlg.YJK.NCG;
			for(i=0;i<dlg.YJK.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.YJK.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.YJK.CG[i].cg;
			}
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"右加宽分段表",pm->RoadName);
				ads_printf(L"\n右幅加宽分段数据已成功保存在%s中!\n",pm->mdbname);
			}
		}
	}

}

//查询超高分段
void CXCGFD()
{
	ZMZDPMFA dlg1;
	if(dlg1.DoModal()==IDOK)
	{
		//1.选择平面方案
		JD_CENTER *pm;
		pm = dlg1.pzLine;

		if(pm->mdbname==L""||pm->mdbname==L"未命名"||
			pm->DV<1||pm->RoadName==L""||pm->RoadName==L"未命名")
		{
			ads_alert(L"请先命名道路方案!");
			return;
		}
		XLDataBase xlmdb;

		SuphFDSheet dlg(L"超高加宽分段数据");

		if(xlmdb.Read_XLDbs(pm->mdbname,L"左超高分段表",pm->RoadName)==0)
		{
			ads_printf(L"请先设置超高标准");
			return;
		}
		dlg.ZCG.NCG = xlmdb.NCG;
		int i;
		for( i=0;i<xlmdb.NCG;i++)
			dlg.ZCG.CG[i] = xlmdb.CG[i];

		if( xlmdb.Read_XLDbs(pm->mdbname,L"右超高分段表",pm->RoadName)==0)
		{
			ads_printf(L"请先设置超高标准");
			return;
		}
		dlg.YCG.NCG = xlmdb.NCG;
		for(i=0;i<xlmdb.NCG;i++)
			dlg.YCG.CG[i] = xlmdb.CG[i];

		if( xlmdb.Read_XLDbs(pm->mdbname,L"左加宽分段表",pm->RoadName)==0)
		{
			ads_printf(L"请先设置加宽标准");
			return;
		}
		dlg.ZJK.NCG = xlmdb.NCG;
		for( i=0;i<xlmdb.NCG;i++)
			dlg.ZJK.CG[i] = xlmdb.CG[i];

		if(xlmdb.Read_XLDbs(pm->mdbname,L"右加宽分段表",pm->RoadName)==0)
		{
			ads_printf(L"请先设置加宽标准");
			return;
		}
		dlg.YJK.NCG = xlmdb.NCG;
		for(i=0;i<xlmdb.NCG;i++)
			dlg.YJK.CG[i] = xlmdb.CG[i];
		if(dlg.DoModal()==IDOK)
		{
			XLDataBase xlmdb;
			xlmdb.NCG = dlg.ZCG.NCG;
			int i;
			for(i=0;i<dlg.ZCG.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.ZCG.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.ZCG.CG[i].cg;
			}

			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"左超高分段表",pm->RoadName);
				ads_printf(L"\n左幅超高分段数据已成功保存在%s中!\n",pm->mdbname);
			}

			xlmdb.NCG = dlg.YCG.NCG;
			for(i=0;i<dlg.YCG.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.YCG.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.YCG.CG[i].cg;
			}
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"右超高分段表",pm->RoadName);
				ads_printf(L"\n右幅路面宽数据已成功保存在%s中!\n",pm->mdbname);
			}

			xlmdb.NCG = dlg.ZJK.NCG;
			for(i=0;i<dlg.ZJK.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.ZJK.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.ZJK.CG[i].cg;
			}
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"左加宽分段表",pm->RoadName);
				ads_printf(L"\n左幅加宽分段数据已成功保存在%s中!\n",pm->mdbname);
			}

			xlmdb.NCG = dlg.YJK.NCG;
			for(i=0;i<dlg.YJK.NCG;i++)
			{ 
				xlmdb.CG[i].xlc = dlg.YJK.CG[i].xlc;
				xlmdb.CG[i].cg =  dlg.YJK.CG[i].cg;
			}
			if(xlmdb.NCG>0)
			{
				xlmdb.Write_XLDbs(pm->mdbname,L"右加宽分段表",pm->RoadName);
				ads_printf(L"\n右幅加宽分段数据已成功保存在%s中!\n",pm->mdbname);
			}
		}

	}

}
//自动设计
void RAutoDesign()
{
	//AfxMessageBox(L"此功能暂未开放！");
	//return;
	isCT = 0;
	RoadAutoDesignDlg AutoDes;
	AutoDes.DoModal();
}
void ZDCT()
{
	BAS_DRAW_FUN Bas_Fun;
	Bas_Fun.ReadWorkdir();
	//20200322梁柱关闭
	/*if (Pro != DaoRuPingMianFangAnMdbMingCheng)
	{
		AfxMessageBox(L"未导入平面方案或导入的平面方案与【项目管理】中选择的道路方案不对应，请核查！");
		return;
	}*/
	isCT = 1;
	RoadAutoDesignDlg AutoDes;
	AutoDes.DoModal();
	isCT = 0;
}

//出改沟图
void GJCT()
{
	BAS_DRAW_FUN Bas_Fun;
	Bas_Fun.ReadWorkdir();
	if (Pro != DaoRuPingMianFangAnMdbMingCheng)
	{
		AfxMessageBox(L"未导入平面方案或导入的平面方案与【项目管理】中选择的道路方案不对应，请核查！");
		return;
	}
	isCT = 1;
	CAutoDrawGutterDlg AutoDes;
	AutoDes.DoModal();
	isCT = 0;
}

//计算桩号序列
void CALSTA()
{
	StaDataSheet dlg(L"计算桩号序列");
	if(dlg.DoModal()==IDOK)
	{
		//写数据库
		XLDataBase xlmdb;
		for(int i=0; i<dlg.xlnum; i++)
		{
			xlmdb.NSTA = dlg.StaDlgAry[i].NSTA;
			if(xlmdb.STA) delete[]xlmdb.STA;xlmdb.STA=NULL;
			if(xlmdb.NSTA>0)
			{
				xlmdb.STA = new LCBdata[xlmdb.NSTA];
				for(int j=0; j<xlmdb.NSTA; j++)
					xlmdb.STA[j] = dlg.StaDlgAry[i].lcb[j];
				//写数据库
				xlmdb.Write_XLDbs(dlg.mdbFname,L"桩号序列",dlg.road[i].roadname); 
				ads_printf(L"\n%s的桩号序列已存入%s中!\n",
					dlg.road[i].roadname,dlg.mdbFname);
			}

		}

	}
}
//提取地面线
void GetDmx()
{
	//选择方式

	HdmDmx hdm;

	hdm.CalDmx();

}

//修改地面线
void ModifyDmx()
{
	CCrossEarthInputDlg* InputDlg = NULL;
	InputDlg = new CCrossEarthInputDlg;
	InputDlg->Create(IDD_DIALOG_CROSSEARTHINPUT);
	InputDlg->ShowWindow(SW_SHOW);
}

//刷新土石方和用地数据
void RefreshTSFAndYDArea()
{
	OutTAB outTab;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	CString MdbName = Cworkdir + "\\DATA\\" + Pro + ".mdb";
	XLDataBase DBS;
	double FillVolSum,CutVolSum;
	if (pzLinep==NULL)
	{
		DBS.Read_XLDbs(MdbName,L"线元表",L"主线");		
		DBS.Read_XLDbs(MdbName,L"断链表",L"主线");
		if(DBS.XYNum>0)
		{
			pzLinep=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				pzLinep->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pzLinep->mdbname,MdbName);
			_tcscpy(pzLinep->RoadName,L"主线");
		}
		if (pzLinep==NULL)
		{
			return;
		}
	}
	outTab.CalAllTSF(MdbName,L"主线", pzLinep->DLArray[0].ELC, pzLinep->DLArray[pzLinep->DLArray.GetSize()-1].BLC, FillVolSum, CutVolSum);

	DBS.YDLXSum = 0;
	DBS.Read_XLDbs(MdbName,L"用地类型表",L"主线");
	if (DBS.YDLXSum==0)
	{
		DBS.YDLXSum = 1;
		DBS.YDLXB = new YongDiLXB[1];
		DBS.YDLXB[0].dml = pzLinep->DLArray[pzLinep->DLArray.GetSize()-1].BLC;
		DBS.YDLXB[0].YDLX = "耕地";
	}

	double szydk,syydk,ezydk,eyydk,sml,eml;
	int i,sno=-1,eno=-1,iroad,ii;
	double zxsml = pzLinep->DLArray[0].ELC;
	double zxeml = pzLinep->XLLength;

	CString ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + "主线";
	FILE* fpr = NULL;
	int NZDK;
	ZDKDAT* pZDK = NULL;
	double dml;
	if((fpr=_wfopen(ZDKfilename ,L"r"))!=NULL)
	{
		fwscanf(fpr,L"%d",& NZDK);
		if( NZDK>0)
		{
			pZDK = new ZDKDAT[ NZDK];
			for(int j=0;j< NZDK;j++)
				fwscanf(fpr ,L"%lf%lf%d%lf%lf",& pZDK[j].cml,&dml,& pZDK[j].IsInsertHdm,& pZDK[j].LZDK,& pZDK[j].RZDK);
		}
		else
		{
			fclose(fpr);
			return;
		}
		fclose(fpr);
	}
	else
	{
		AfxMessageBox(L"无法打开占地宽文件!请先进行横断面或用地设计!");
		return;
	}

	CString LandAreaStr;//用地面积
	double YdAll = 0.0;
	zxsml = pzLinep->DLArray[0].ELC;
	for (int iYD=0; iYD<DBS.YDLXSum; iYD++)
	{
		zxeml = pzLinep->TYLC(DBS.YDLXB[iYD].dml);

		//if(zxsml< pZDK[0].cml-0.01||zxeml> pZDK[ NZDK-1].cml+0.01)
		//{
		//	ads_alert(L"里程出界!");
		//	return;
		//}
		if(zxsml < pZDK[0].cml-0.01)
		{
			ads_printf(L"统计用地的起点里程小于用地桩起点里程\n");
			zxsml = pZDK[0].cml;
		}
		if (zxeml > pZDK[ NZDK-1].cml+0.01)
		{
			ads_printf(L"统计用地的终点里程大于用地桩终点里程\n");
			zxeml = pZDK[ NZDK-1].cml;
		}
		double SmlZx,EmlZx;
		SmlZx = zxsml, EmlZx = zxeml;

		double zarea=0.0,yarea=0.0;

		sml = zxsml;
		eml = zxeml;
		sno=-1,eno=-1;
		for(i=0;i<NZDK;i++)
		{
			if(pZDK[i].cml>=sml-0.01)
			{
				sno = i;
				if(i < NZDK && i>0)
				{
					szydk =  pZDK[i-1].LZDK + (sml-pZDK[i-1].cml)*
						(pZDK[i].LZDK - pZDK[i-1].LZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
					syydk =  pZDK[i-1].RZDK + (sml-pZDK[i-1].cml)*
						(pZDK[i].RZDK - pZDK[i-1].RZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
				}
				else if(i==0)
				{
					szydk =  pZDK[0].LZDK;
					syydk =  pZDK[0].RZDK;
				}
				break;
			}
		}

		for(i=0;i<NZDK;i++)
		{
			if(pZDK[i].cml>=eml-0.01)
			{
				eno = i;
				if(i < NZDK && i>0)
				{
					ezydk =  pZDK[i-1].LZDK + (eml - pZDK[i-1].cml)*
						(pZDK[i].LZDK - pZDK[i-1].LZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
					eyydk =  pZDK[i-1].RZDK + (eml - pZDK[i-1].cml)*
						(pZDK[i].RZDK - pZDK[i-1].RZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
				}
				break;
			}
		}

		if(sno>=0 && eno>=0)
		{	
			for(i=sno;i<eno-1;i++)
			{
				zarea +=0.5*(pZDK[i].LZDK+pZDK[i+1].LZDK)*
					(pZDK[i+1].cml-pZDK[i].cml);
				yarea +=0.5*(pZDK[i].RZDK+pZDK[i+1].RZDK)*
					(pZDK[i+1].cml-pZDK[i].cml);
			}
			if(eno>sno)
			{
				zarea += 0.5*(szydk+pZDK[sno].LZDK)*(pZDK[sno].cml-sml);
				zarea += 0.5*(ezydk+pZDK[eno-1].LZDK)*(eml-pZDK[eno-1].cml);
				yarea += 0.5*(syydk+pZDK[sno].RZDK)*(pZDK[sno].cml-sml);
				yarea += 0.5*(eyydk+pZDK[eno-1].RZDK)*(eml-pZDK[eno-1].cml);
			}
			else
			{
				zarea += 0.5*(szydk+ezydk)*(eml-sml);
				yarea += 0.5*(syydk+eyydk)*(eml-sml);
			}

		}

		if (zarea+yarea>1.0)
		{
			CString Temp;
			Temp.Format(L"%.1lf", (zarea + yarea) * 3.0 / 2000.0);
			LandAreaStr += (Temp + "(" + DBS.YDLXB[iYD].YDLX + ")");
		}

		YdAll += (zarea + yarea) * 3.0 / 2000.0;

		zxsml = zxeml;

	}

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	//////////////////////////////////////////////////////////////////////////
	//将线路属性写入改移道路表
	CString MdfRoadNmae = Pro;
	CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
	DBS.Read_XLDbs(MdfName,L"改移道路表", MdfRoadNmae);
	if (DBS.ModifyRoadSum<1)
	{
		DBS.ModifyRoadSum = 1;
		DBS.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[DBS.ModifyRoadSum];
		DBS.IniMdfRoadData(MdfRoadNmae, DBS.pModifyRoad[0]);
	}
	else if (DBS.ModifyRoadSum>1)
	{
		AfxMessageBox(Pro + "在改移道路表中有多条记录!");
		return;
	}

	//20191106新增	添加道路厚度和种类数据记录
	std::vector<RoadLayerData> RoadLayerArray_L, RoadLayerArray_R;
	g_RoadWorkingDatabase.GetRoadLayer(MdbName, L"主线", LEFT, RoadLayerArray_L);
	g_RoadWorkingDatabase.GetRoadLayer(MdbName, L"主线", RIGHT, RoadLayerArray_R);
	map<CString, double> RoadLayerMap;
	for (int i = 0; i < RoadLayerArray_L.size(); i++)
		RoadLayerMap[RoadLayerArray_L[i].LayerName] = RoadLayerArray_L[i].LayerThickness;
	for (int i = 0; i < RoadLayerArray_R.size(); i++)
		RoadLayerMap[RoadLayerArray_R[i].LayerName] = RoadLayerArray_R[i].LayerThickness;

	DBS.pModifyRoad[0].LmType.Empty();
	DBS.pModifyRoad[0].LmThick = 0;
	map<CString, double>::iterator map_it = RoadLayerMap.begin();
	while (map_it != RoadLayerMap.end())
	{
		DBS.pModifyRoad[0].LmType = DBS.pModifyRoad[0].LmType + map_it->first + L"/";
		DBS.pModifyRoad[0].LmThick = DBS.pModifyRoad[0].LmThick + map_it->second;
		++map_it;
	}
	if (!DBS.pModifyRoad[0].LmType.IsEmpty())
		DBS.pModifyRoad[0].LmType = DBS.pModifyRoad[0].LmType.Left(DBS.pModifyRoad[0].LmType.GetLength() - 1);
	DBS.pModifyRoad[0].LmThick *= 100;	//从m转换为cm

	DBS.pModifyRoad[0].FillVol = FillVolSum;
	DBS.pModifyRoad[0].CutVol = CutVolSum;
	DBS.pModifyRoad[0].LandArea = YdAll;
	DBS.pModifyRoad[0].LandAreaStr = LandAreaStr;
	DBS.Write_XLDbs(MdfName,L"改移道路表",MdfRoadNmae);
#endif
}

//刷新用地数据
void RefreshYDArea()
{
	OutTAB outTab;
	CString MdbName = Cworkdir + "\\DATA\\" + Pro + ".mdb";
	XLDataBase DBS;
	if (pzLinep==NULL)
	{
		DBS.Read_XLDbs(MdbName,L"线元表",L"主线");		
		DBS.Read_XLDbs(MdbName,L"断链表",L"主线");
		if(DBS.XYNum>0)
		{
			pzLinep=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				pzLinep->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pzLinep->mdbname,MdbName);
			_tcscpy(pzLinep->RoadName,L"主线");
		}
	}

	DBS.YDLXSum = 0;
	DBS.Read_XLDbs(MdbName,L"用地类型表",L"主线");
	if (DBS.YDLXSum==0)
	{
		DBS.YDLXSum = 1;
		DBS.YDLXB = new YongDiLXB[1];
		DBS.YDLXB[0].dml = pzLinep->DLArray[pzLinep->DLArray.GetSize()-1].BLC;
		DBS.YDLXB[0].YDLX = "耕地";
	}

	double szydk,syydk,ezydk,eyydk,sml,eml;
	int i,sno=-1,eno=-1,iroad,ii;
	double zxsml = pzLinep->DLArray[0].ELC;
	double zxeml = pzLinep->XLLength;

	CString ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + "主线";
	FILE* fpr = NULL;
	int NZDK;
	ZDKDAT* pZDK = NULL;
	double dml;
	if((fpr=_wfopen(ZDKfilename ,L"r"))!=NULL)
	{
		fwscanf(fpr,L"%d",& NZDK);
		if( NZDK>0)
		{
			pZDK = new ZDKDAT[ NZDK];
			for(int j=0;j< NZDK;j++)
				fwscanf(fpr ,L"%lf%lf%d%lf%lf",& pZDK[j].cml,&dml,& pZDK[j].IsInsertHdm,& pZDK[j].LZDK,& pZDK[j].RZDK);
		}
		fclose(fpr);
	}
	else
	{
		AfxMessageBox(L"无法打开占地宽文件!请先进行横断面或用地设计!");
		return;
	}

	if (NZDK<=0)
	{
		return;
	}
	CString LandAreaStr;//用地面积
	double YdAll = 0.0;
	zxsml = pzLinep->DLArray[0].ELC;
	for (int iYD=0; iYD<DBS.YDLXSum; iYD++)
	{
		zxeml = pzLinep->TYLC(DBS.YDLXB[iYD].dml);

		if(zxsml < pZDK[0].cml-0.01)
		{
			ads_printf(L"统计用地的起点里程小于用地桩起点里程\n");
			zxsml = pZDK[0].cml;
		}
		if (zxeml > pZDK[ NZDK-1].cml+0.01)
		{
			ads_printf(L"统计用地的终点里程大于用地桩终点里程\n");
			zxeml = pZDK[ NZDK-1].cml;
		}
		double SmlZx,EmlZx;
		SmlZx = zxsml, EmlZx = zxeml;

		double zarea=0.0,yarea=0.0;

		sml = zxsml;
		eml = zxeml;
		sno=-1,eno=-1;
		for(i=0;i<NZDK;i++)
		{
			if(pZDK[i].cml>=sml-0.01)
			{
				sno = i;
				if(i < NZDK && i>0)
				{
					szydk =  pZDK[i-1].LZDK + (sml-pZDK[i-1].cml)*
						(pZDK[i].LZDK - pZDK[i-1].LZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
					syydk =  pZDK[i-1].RZDK + (sml-pZDK[i-1].cml)*
						(pZDK[i].RZDK - pZDK[i-1].RZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
				}
				else if(i==0)
				{
					szydk =  pZDK[0].LZDK;
					syydk =  pZDK[0].RZDK;
				}
				break;
			}
		}

		for(i=0;i<NZDK;i++)
		{
			if(pZDK[i].cml>=eml-0.01)
			{
				eno = i;
				if(i < NZDK && i>0)
				{
					ezydk =  pZDK[i-1].LZDK + (eml - pZDK[i-1].cml)*
						(pZDK[i].LZDK - pZDK[i-1].LZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
					eyydk =  pZDK[i-1].RZDK + (eml - pZDK[i-1].cml)*
						(pZDK[i].RZDK - pZDK[i-1].RZDK)/
						(pZDK[i].cml - pZDK[i-1].cml);
				}
				break;
			}
		}

		if(sno>=0 && eno>=0)
		{	
			for(i=sno;i<eno-1;i++)
			{
				zarea +=0.5*(pZDK[i].LZDK+pZDK[i+1].LZDK)*
					(pZDK[i+1].cml-pZDK[i].cml);
				yarea +=0.5*(pZDK[i].RZDK+pZDK[i+1].RZDK)*
					(pZDK[i+1].cml-pZDK[i].cml);
			}
			if(eno>sno)
			{
				zarea += 0.5*(szydk+pZDK[sno].LZDK)*(pZDK[sno].cml-sml);
				zarea += 0.5*(ezydk+pZDK[eno-1].LZDK)*(eml-pZDK[eno-1].cml);
				yarea += 0.5*(syydk+pZDK[sno].RZDK)*(pZDK[sno].cml-sml);
				yarea += 0.5*(eyydk+pZDK[eno-1].RZDK)*(eml-pZDK[eno-1].cml);
			}
			else
			{
				zarea += 0.5*(szydk+ezydk)*(eml-sml);
				yarea += 0.5*(syydk+eyydk)*(eml-sml);
			}

		}

		if (zarea+yarea>1.0)
		{
			CString Temp;
			Temp.Format(L"%.1lf", (zarea + yarea) * 3.0 / 2000.0);
			LandAreaStr += (Temp + "(" + DBS.YDLXB[iYD].YDLX + ")");
		}

		YdAll += (zarea + yarea) * 3.0 / 2000.0;

		zxsml = zxeml;

	}
#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	//////////////////////////////////////////////////////////////////////////
	//将线路属性写入改移道路表
	CString MdfRoadNmae = Pro;
	CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
	DBS.Read_XLDbs(MdfName,L"改移道路表", MdfRoadNmae);
	if (DBS.ModifyRoadSum<1)
	{
		DBS.ModifyRoadSum = 1;
		DBS.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[DBS.ModifyRoadSum];
		DBS.IniMdfRoadData(MdfRoadNmae, DBS.pModifyRoad[0]);
	}
	else if (DBS.ModifyRoadSum>1)
	{
		AfxMessageBox(Pro + "在改移道路表中有多条记录!");
		return;
	}

	DBS.pModifyRoad[0].LandArea = YdAll;
	DBS.pModifyRoad[0].LandAreaStr = LandAreaStr;
	DBS.Write_XLDbs(MdfName,L"改移道路表",MdfRoadNmae);
#endif
}


HdmDes HDM;
//横断面自动设计
void HdmAutoDes()
{
	DesLineInfor.removeAll();
	HDM.DesignHdm();
	RefreshTSFAndYDArea();
}
//导入横断面
void INHDM()
{
	DesLineInfor.removeAll();
	HDM.ShowCross();
}
//修改横断面边坡	修改边坡
void EDITHDMBP()
{
	ads_name ent;
	AcDbEntity *pEnt;
	DesignLine_ROAD * desline;
	AcDbObjectId DesId;
	ads_point pt;
	int i;
	bool ifcur,ifpart,ifall;
	double nBPJS;
	double TM[30],TH[30],TPTW[30];//边坡参数，传到界面中做初值
	CEditBP EditBP;
	ads_entsel(L"\n请选择要修改的边坡...",ent,pt);
	acdbGetObjectId(DesId,ent);
	if(acdbOpenObject(pEnt,DesId,AcDb::kForRead)!=Acad::eOk)
		return;
	pEnt->close();		
	//pEnt->assertWriteEnabled();//加响应器修改实体时，不可少
	if(pEnt->isKindOf(DesignLine_ROAD::desc()))
	{
		desline=DesignLine_ROAD::cast(pEnt);
		desline->CalBP();
		if(desline->m_FillORCut<0.01)
		{
			nBPJS=desline->m_nLTBPJS;
			for(i=0; i<desline->m_nLTBPJS; i++)
			{
				TM[i] = desline->m_TM[i];
				TH[i] = desline->m_TH[i];
				TPTW[i]=desline->m_TPTW[i];
			}
		}

		else
		{
			nBPJS=desline->m_nLZBPJS;
			for(i=0; i<desline->m_nLZBPJS; i++)
			{
				TM[i] = desline->m_WM[i];
				TH[i] = desline->m_PH[i];
				TPTW[i]=desline->m_ZPTW[i];
			}
		}

		EditBP.BPJS = nBPJS;
		for(i=0; i<EditBP.BPJS; i++)
		{
			EditBP.m_M[i]=TM[i];
			EditBP.m_H[i]=TH[i];
			EditBP.m_PTW[i]=TPTW[i];
		}
		//SelectDM(desline->cml);//得到所选断面的序号
		if(EditBP.DoModal()==IDOK)
		{
			int selectmode;
			ifcur=EditBP.ifCur;
			ifpart=EditBP.ifPart;
			ifall=EditBP.ifAll;
			if(ifcur) selectmode=0;
			else if(ifpart) selectmode=1;
			else selectmode=2;
			ads_printf(L"\nEditBP.m_M=%f\n",EditBP.m_M[0]);
			HDM.ChangeBp(selectmode, DesId, EditBP.BPJS, EditBP.m_M, EditBP.m_H, EditBP.m_PTW);
			desline->close();
		}
		else
		{
			desline->close();
			return;
		}
		/*	ads_initget(128,L"Y y N n");
		if((ads_getkword(L"\n修改是否有效? Yes/No<Y>:_",choice))==RTNONE)
		_tcscpy(choice,L"Y");
		if(_tcscmp(choice,L"n")==0||_tcscmp(choice,L"N")==0)
		{
		ChangeBp(ifcur,ifpart,ifall,DesId,nLTBPJS,nLZBPJS,nBPJS,TM,TH,TPTW);
		}

		if(_tcscmp(choice,L"y")==0||_tcscmp(choice,L"Y")==0)//更新文件
		{
		int xlxz=desline->xlxz;
		int r_bh=desline->r_bh;
		if(xlxz==0&&r_bh==-1)//一线
		hdmdes.WriteYXAllFile();
		if(xlxz==-1)//左线
		hdmdes.WriteRXDAllFile();
		if(xlxz==1)//右线
		hdmdes.WriteRXDAllFile();


		//	if(IfSave3DData)//是否保存三维数据
		//	{
		hdmdes.CalBjpt();
		if(hdmdes.BJPT) delete[] hdmdes.BJPT;
		hdmdes.BJPT = NULL;
		hdmdes.Cal_LX3DPT();//生成左线和右线线路三维坐标
		//计算桥墩中心
		hdmdes.CalBriBaseXY(); 
		ads_printf(L"3D OK!\n");
		//	}
		}*/
	}	

}

//水沟交互
void GUTINTERACT()
{
	CGutInteractDlg* GutInactDlg = NULL;
	GutInactDlg = new CGutInteractDlg;
	GutInactDlg->Create(IDD_DIALOG_GUTINTERACT);
	GutInactDlg->ShowWindow(SW_SHOW);
}

//保存方案（横断面）
void SAVEHDM()
{
	HDM.WriteFile();
	RefreshTSFAndYDArea();
	AfxMessageBox(L"保存横断面方案成功！");
}
//绘横断面图
void DRAWHDMT()
{
	//读文件
	HDM.DRAWHDMT();
}
//输出横断面图
void OUTHDM()
{
	//读文件
	HDM.OutDwgFile();
}
//表格输出
//1.逐桩坐标表
void OutZZZB()
{
	OutTAB tab;
	tab.OUT_ZZZB();
}

void OutZXQXZJ()
{
	OutTAB tab;
	tab.OUT_ZXQXZJ();
}

void OutZPSQX()
{
	OutTAB tab;
	tab.OUT_ZPSQX();
}

void OutZZYDZB()
{
	OutTAB tab;
	//tab.OUT_ZZYDZB();
	tab.OUT_ZZYDZBMJ();
}

void OutLJSJB()
{
	OutTAB tab;
	tab.OUT_LJSJB();
}
void OutBGPSG()
{
	OutTAB tab;
	tab.OUT_BGPSGSJB();
}
//土石方数量计算表
void OutTSFJSB()
{
	OutTAB tab;
	tab.OUT_TSFJSB();
}
//工程数量表
void OutGCSLB()
{
	HDM.DrawGCSL();
}
//路基大样图
void OutDYT()
{
	CDrawDYTdlg pdlg;
	pdlg.DoModal();
}
void OutMGLTSFB()
{
	OutTAB tab;
	tab.OUT_MGLTSFB();
}
void OutLXTWB()
{
	OutTAB tab;
	tab.OUT_LXTWB();
}

void OutQBTB()//清表土表
{
	OutTAB tab;
	tab.OUT_QBTB();
}
void OutWYNB()//挖淤泥表
{
	OutTAB tab;
	tab.OUT_WYNB();
}

void DELAYER()
{
	/*
	FILE *fpr,*fpw;
	int num,i;
	ads_point pt;
	BAS_DRAW_FUN ob;
	AcGePoint3d rpt;

	char ch[20],gc[20];
	fpr = _wfopen(L"C:\\data\\dgxpt.dat",L"r");
	//	fpw = _wfopen(L"C:\\data\\dgxpt1.dat",L"w");
	fwscanf(fpr,L"%d",&num);
	for(i=0;i<num;i++)
	{
	fwscanf(fpr,L"%s%lf%lf%lf",ch,&pt[0],&pt[1],&pt[2]);
	/ *
	_stprintf(ch,L"%d",i+146);
	if(i+146>177)
	_stprintf(ch,L"%d",i+147);   
	fwprintf(fpw,L"%s,,%0.3lf,%0.3lf,%0.3lf\n",ch,pt[0],pt[1],pt[2]);* /
	//		fwprintf(fpw,L"%d,,%0.3lf,%0.3lf,%0.3lf\n",i+1,pt[0],pt[1],pt[2]);
	//		_stprintf(ch,L"%0.2lf",pt[2]);
	acedCommandS(RTSTR,L"donut",RTREAL,0.0,RTREAL,0.25,RT3DPOINT,pt,RTSTR,L"",0);
	rpt.x = pt[0];
	rpt.y = pt[1];
	rpt.z = pt[2];
	ob.makecircle(rpt,0.3,2,L"点");

	rpt.x = pt[0]+2.2;
	rpt.y = pt[1];
	_stprintf(gc,L"%0.2lf",pt[2]);
	ob.maketext(rpt,gc,0.0,1.1,0,3,L"宋体",L"特征点高程");
	/ *
	_stprintf(ch,L"%d",i+146);
	if(i+146>177)
	_stprintf(ch,L"%d",i+147);* /
	rpt.x = pt[0]-1.5;
	rpt.y = pt[1];
	ob.maketext(rpt,ch,0.0,1.1,1,3,L"宋体",L"点号");

	}
	fclose(fpr);
	//	fclose(fpw);

	return;*/

	ACHAR   lname[80];	
	if(ads_getstring(1,L"\n输入要删除的层名:_", lname)==RTNORM)
	{
		BAS_DRAW_FUN tool;
		tool.dlayer(lname);   
	}
}

// This is command 'VIZTOOL'
void VIZTOOL()
{
	//	AcGePoint3d PT,PT1,PT2;
	//	AcDbObject *pEnt1,*pEnt2;
	//	ads_name en;
	//	ads_point spt,ept,pts,pte;
	//	int rc;
	//	int Counter;
	//    BAS_DRAW_FUN ob;	
	//	
	//	rc = ads_entsel(L"\n选取起始点 : ", en, asDblArray(PT));
	//	if(rc==RTCAN)
	//		return;
	//	if (rc == RTNORM)
	//	{
	//		ob.GetCircle(en,spt);
	////		ads_printf(L"sx=%lf sy=%lf sz=%lf\n",spt[X],spt[Y],spt[Z]);
	//	}
	//	
	//	rc = ads_entsel(L"\n选取终止点 : ", en, asDblArray(PT));
	//	if(rc==RTCAN)
	//		return;
	//	if (rc == RTNORM) 
	//	{
	//		ob.GetCircle(en,ept);
	////		ads_printf(L"ex=%lf ey=%lf ez=%lf\n",ept[X],ept[Y],ept[Z]);
	//	}
	//    
	//	if(spt[Z]<ept[Z])
	//	{
	//		pts[X] = spt[X];
	//		pts[Y] = spt[Y];
	//		pts[Z] = spt[Z];
	//		pte[X] = ept[X];
	//		pte[Y] = ept[Y];
	//		pte[Z] = ept[Z];
	//	}
	//	else
	//	{
	//		pts[X] = ept[X];
	//		pts[Y] = ept[Y];
	//		pts[Z] = ept[Z];
	//		pte[X] = spt[X];
	//		pte[Y] = spt[Y];
	//		pte[Z] = spt[Z];
	//	}
	//	
	//	double sz;
	//	sz = floor(pts[Z])+1;
	//	ads_point ncpt;
	//	while(sz<pte[Z])
	//	{
	//		ncpt[X] = pts[X] + (sz-pts[Z])*(pte[X]-pts[X])/(pte[Z]-pts[Z]);
	//		ncpt[Y] = pts[Y] + (sz-pts[Z])*(pte[Y]-pts[Y])/(pte[Z]-pts[Z]);
	//		ncpt[Z] = sz;
	//		acedCommandS(RTSTR,L"donut",RTREAL,0.0,RTREAL,0.01,RT3DPOINT,ncpt,RTSTR,L"",0);
	//		sz+=1.0;
	//	}	
	//	return;
	//

	if(ToolBardlg!=NULL)
	{
		ToolBardlg->SetActiveWindow();
		ToolBardlg->ShowWindow(SW_SHOW);
		return ;
	}
	ToolBardlg = new CCheckToolBar();
	ToolBardlg->Create(IDD_CheckToolBar,acedGetAcadFrame());
	ToolBardlg->ShowWindow(SW_SHOW);

}

void RIBBON()
{
	if (Ribbon == 0)
	{
		CString fname;
		fname.Format(L"%s\\RoadRibbonMenu.dll", SysPath);
		Ribbon = 1;

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"_ribbon", RTNONE);
		acedCommandS(RTSTR, L"_netload", RTSTR, fname, RTNONE);
		acedCommandS(RTSTR, L"_RoadRIBBON", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"_ribbon", RTNONE);
		ads_command(RTSTR, L"_netload", RTSTR, fname, RTNONE);
		ads_command(RTSTR, L"_ROADRIBBON", RTNONE);


#endif // VERSION_CAD2010
	}
}



void WDtoDR()
{
	DataTrans dlg;
	dlg.DoModal();
}

void DRtoWD()
{

}

void WDdmxtoDR()
{
	DMXDataTrans dlg;
	dlg.DoModal();
}

void DRtoWDdmx()
{

}


// This is command 'RUBBER'
void RUBBER()
{
	/*
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	rc = ads_entsel(L"\nSelect a  设计线 : ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
	ads_printf(L"\nError during object selection"); 
	return;   
	}
	acdbGetObjectId(eId, en);
	BAS_DRAW_FUN ob;
	CArray<BAS_DRAW_FUN::XYarray,BAS_DRAW_FUN::XYarray>pXYArr;
	ob.FormXYarray(eId,pXYArr);
	return;*/


	struct egpt_node *newptr,*tailptr,*tailp,*frontptr;  
	struct resbuf result;
	ads_real dist,angle;
	ads_point pt,tpt;
	int type;   

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

	frontptr = (struct egpt_node *)calloc(1,sizeof(struct egpt_node));
	frontptr->next = NULL;
	tailptr = frontptr;

L10:
	type = 0;
	newptr = (struct egpt_node *)calloc(1,sizeof(struct egpt_node));
	newptr->next = NULL;
	newptr->prior = tailptr;
	tailptr->next = newptr;
	tailptr = newptr;

	ads_printf(L"\n输入多边形第一点:");
	for(;;)
	{
		if(ads_grread(1 , &type , &result)!=RTNORM)
		{
			if(newptr) free(newptr);
			if(frontptr) free(frontptr);
			return ;
		}
		if(type == 3)
			break;
		if(type == 12)
		{
			if(newptr) free(newptr);
			if(frontptr) free(frontptr);
			return ;
		}
		if(type != 5)//||(type != 3 ))
		{ 
			ads_printf(L"\n无效的输入!");
			ads_printf(L"\n输入多边形第一点:");
		}
	}
	newptr->pt[X] = result.resval.rpoint[X]; 
	newptr->pt[Y] = result.resval.rpoint[Y];
	pt[X] = result.resval.rpoint[X]; 
	pt[Y] = result.resval.rpoint[Y];

	for(;;)
	{
L20:
		ads_printf(L"\nUndo/<下一点>:");
		for(;;)
		{
			if(ads_grread(1,&type,&result) !=RTNORM)
			{
				newptr = frontptr;
				while(newptr)
				{
					tailptr = newptr->next;
					free(newptr);
					newptr = tailptr;
				}
				return ; 
			}

			if(type == 2)
			{
				if(result.resval.rint==117 || result.resval.rint==85)
				{
					if(tailptr->prior == frontptr)
					{
						ads_grdraw(tailptr->pt,result.resval.rpoint,-1,0);					
						newptr=tailptr;
						tailptr = tailptr->prior;
						tailptr->next = NULL;
						free(newptr);
						goto L10;
					}
					else
					{
						ads_grdraw(frontptr->next->pt,pt,-1,0);
						ads_grdraw(tailptr->pt,pt,-1,0);	
						ads_grdraw(tailptr->prior->pt,tailptr->pt,-1,0);
						newptr=tailptr;
						tailptr = tailptr->prior;
						tailptr->next = NULL;
						free(newptr);
						if(tailptr->prior == frontptr)
						{
							ads_grdraw(tailptr->pt,pt,-1,0);	
						}
						else
						{
							ads_grdraw(frontptr->next->pt,pt,-1,0);
							ads_grdraw(tailptr->pt,pt,-1,0);	
						}
						goto L20;
					}
				}
			}
			if(type == 3)
				break;
			if(type == 25)
			{
				goto L30;
			}
			if(type != 5)//||(type != 3 ))
			{ 
				//  ads_printf(L"\n无效的输入!");
				ads_printf(L"\nUndo/<下一点>:");
			}

			if(BAS_DRAW_FUN::caldistance(pt,result.resval.rpoint)>0.01)
			{
				if(tailptr->prior == frontptr)
				{
					ads_grdraw(frontptr->next->pt,pt,-1,0);
					ads_grdraw(frontptr->next->pt,result.resval.rpoint,-1,0);
				}
				else
				{
					ads_grdraw(frontptr->next->pt,pt,-1,0);
					ads_grdraw(frontptr->next->pt,result.resval.rpoint,-1,0);
					ads_grdraw(tailptr->pt,pt,-1,0);
					ads_grdraw(tailptr->pt,result.resval.rpoint,-1,0);
				}
			}
			pt[X] = result.resval.rpoint[X]; 
			pt[Y] = result.resval.rpoint[Y];
		}

		if(tailptr->prior == frontptr)
		{
			ads_grdraw(frontptr->next->pt,result.resval.rpoint,-1,0);
		}
		newptr = (struct egpt_node *)calloc(1,sizeof(struct egpt_node));
		newptr->next = NULL;
		newptr->prior = tailptr;
		tailptr->next = newptr;
		tailptr = newptr;
		newptr->pt[X] = result.resval.rpoint[X]; 
		newptr->pt[Y] = result.resval.rpoint[Y];
	}

L30:
	if(fabs(pt[X]-tailptr->pt[X]) > 0.01 || fabs(pt[Y]-tailptr->pt[Y]) >0.01)  
	{
		ads_grdraw(frontptr->next->pt,pt,-1,0);
		ads_grdraw(tailptr->pt,pt,-1,0);
		ads_grdraw(frontptr->next->pt,tailptr->pt,-1,0);
	}

	ads_getpoint(NULL ,L"\n请在区域内采一点,L" , pt);
	newptr=frontptr->next;
	if(!newptr) 
	{
		free(frontptr);
		return;
	}
	tailp = newptr->next;
	int count =0;
	int flag=-1;
	ads_point PrePt, NextPt;
	while(tailp)
	{
		count =0;
		flag=-1;
		while(count <5&&flag<0)
		{
			angle = ads_angle(pt,newptr->pt);
			dist = sqrt((pt[X]-newptr->pt[X])*(pt[X]-newptr->pt[X])+(pt[Y]-newptr->pt[Y])*(pt[Y]-newptr->pt[Y]));
			dist = dist-0.1*count;
			ads_polar(pt,angle,dist,PrePt);
			angle = ads_angle(pt,tailp->pt);
			dist = sqrt((pt[X]-tailp->pt[X])*(pt[X]-tailp->pt[X])+(pt[Y]-tailp->pt[Y])*(pt[Y]-tailp->pt[Y]));
			dist = dist-0.1*count;
			ads_polar(pt,angle,dist,NextPt);
			flag = BAS_DRAW_FUN::AutoEntBK1(PrePt,NextPt);
			count++;
		}
		newptr = tailp;
		tailp = newptr->next;
	}
	count =0;
	flag=-1;
	while(count <2&&flag<0)
	{
		angle = ads_angle(pt,tailptr->pt);
		dist = sqrt((pt[X]-tailptr->pt[X])*(pt[X]-tailptr->pt[X])+(pt[Y]-tailptr->pt[Y])*(pt[Y]-tailptr->pt[Y]));
		dist = dist-0.1*count;
		ads_polar(pt,angle,dist,PrePt);
		angle = ads_angle(pt,frontptr->next->pt);
		dist = sqrt((pt[X]-frontptr->next->pt[X])*(pt[X]-frontptr->next->pt[X])+(pt[Y]-frontptr->next->pt[Y])*(pt[Y]-frontptr->next->pt[Y]));
		dist = dist-0.1*count;
		ads_polar(pt,angle,dist,NextPt);
		flag = BAS_DRAW_FUN::AutoEntBK1(PrePt,NextPt);	   
		count++;
	}

	newptr = frontptr->next;  
	while(newptr)
	{
		angle = ads_angle(pt,newptr->pt);
		dist = sqrt((pt[X]-newptr->pt[X])*(pt[X]-newptr->pt[X])+(pt[Y]-newptr->pt[Y])*(pt[Y]-newptr->pt[Y]));
		dist = dist+1.0;
		ads_polar(pt,angle,dist,tpt);
		newptr->pt[X] = tpt[X]; newptr->pt[Y] = tpt[Y];
		newptr = newptr->next;
	}


	newptr = frontptr->next;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, newptr->pt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, newptr->pt, 0);
#endif // VERSION_CAD2010

	newptr = newptr->next;
	while(newptr)
	{
#ifdef VERSION_CAD2016
		acedCommandS(RTPOINT, newptr->pt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTPOINT, newptr->pt, 0);
#endif // VERSION_CAD2010

		newptr = newptr->next;
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010


	newptr = frontptr;
	while(newptr)
	{
		tailptr = newptr->next;
		free(newptr);
		newptr = tailptr;
	}  
	return ; 
}
// This is command 'ZDKDES'
void raildrawpmzttdrawpmztt()
{
	// TODO: Implement the command	
	int RoadSum,i;
	XLDataBase DBS;
	CString mdbname,ZDKfilename;
	JD_CENTER *pm;
	IDsum = 0;

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

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	mdbname.Format(L"%s\\data\\%s.mdb",Cworkdir,Pro);
	DBS.ReadAllRoadName(mdbname);
	RoadSum = DBS.m_RoadSum;
	CDrawPM *drawpm;
	for(i=0;i<RoadSum;i++)
	{
		//		DBS.Read_XLDbs(mdbname,L"控制点表",DBS.Road[i].RoadName);
		DBS.Read_XLDbs(mdbname,L"线元表",DBS.Road[i].RoadName);				
		DBS.Read_XLDbs(mdbname,L"断链表",DBS.Road[i].RoadName);
		if(DBS.XYNum>0)
		{
			pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			//			pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0)
				pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pm->mdbname,mdbname);
			_tcscpy(pm->RoadName,DBS.Road[i].RoadName);
		}

		drawpm= new CDrawPM(pm,pm->DLArray[0].ELC,pm->DLArray[pm->DLNum-1].BLC);
		pzLinep = pm;
		if(i==0)
		{
			ZOOM_P1[Y]=pm->XYArray[1][8];ZOOM_P1[X]=pm->XYArray[1][9];
			ZOOM_P2[Y]=pm->XYArray[pm->XYNum-1][8];ZOOM_P2[X]=pm->XYArray[pm->XYNum-1][9];
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2010

			AcGePoint3d Pt;
			Pt.x = pm->XYArray[1][9]; Pt.y = pm->XYArray[1][8];
			pm->maketext(Pt,L"",0,0);
		}
		drawpm->DFACTOR = 2;
		drawpm->texth = 3;
		drawpm->XSW = 2;
		drawpm->LcSpacer = 100;	
		drawpm->DrawXLZX();
		drawpm->BZLC(1);
		drawpm->BZTZD(1);
		drawpm->DrawDZQ();
		GTZDK_ROAD *DrawZDK=new GTZDK_ROAD;
		DrawZDK->Scml = pm->DLArray[0].ELC;
		DrawZDK->Ecml = pm->XLLength;		
		FILE *fpr;
		DrawZDK->pLine = pm;

		ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + pm->RoadName;
		if((fpr=_wfopen(ZDKfilename,L"r"))==NULL)
		{
			ads_printf(L"没有%s占地宽文件!\n",ZDKfilename);
			return;
		}		
		DrawZDK->ReadZDKFile(fpr);
		fclose(fpr);
		ZDKID[i]=DrawZDK->AddEntityToDbs(DrawZDK);
		IDsum++;
	}
}

// This is command 'DELZDK'
void raildelzdkdelzdk()
{
	// TODO: Implement the command	
	ZDK.DelZDK();
}

// This is command 'ADDZDK'
void railaddzdkaddzdk()
{
	// TODO: Implement the command
	ZDK.AddZDK();
}

// This is command 'CHANGEZDK'
void railchangezdkchangezdk()
{
	// TODO: Implement the command
	ZDK.ChangeZDK();
}

// This is command 'SAVEZDK'
void railsavezdksavezdk()
{
	// TODO: Implement the command
	FILE *fpw;
	GTZDK_ROAD *ppZDK=NULL;
	int i;
	CString ZdkFname,GH;

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	for(i=0;i<IDsum;i++)
	{
		if(acdbOpenObject(ppZDK, ZDKID[i], AcDb::kForRead)!=Acad::eOk)
			return;
		ppZDK->close();
		if(ppZDK!=NULL)
		{
			ZdkFname = Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + ppZDK->pLine->RoadName;
			fpw=_wfopen(ZdkFname,L"w");
			fwprintf(fpw,L"%d\n",ppZDK->NZDK);
			for(i=0;i<ppZDK->NZDK;i++)
				fwprintf(fpw,L"%0.3lf %0.3lf 0 %0.3lf %0.3lf\n",ppZDK->pZDK[i].cml,ppZDK->pLine->XLC1(ppZDK->pZDK[i].cml),ppZDK->pZDK[i].LZDK,ppZDK->pZDK[i].RZDK);
			fclose(fpw);
			ads_printf(L"%s占地宽数据已存入文件\n",ZdkFname);
		}
	}
}

#include "ZDKSHEET.h"
CZDKSHEET *ZDKINPUTDlg;
// This is command 'EDITZDK'
void raileditzdkeditzdk()
{
	// TODO: Implement the command	
	ads_name en;
	AcGePoint3d PT;
	AcDbObjectId eId;
	AcDbObject *pObj=NULL;
	GTZDK_ROAD *pzx;

	int rc = ads_entsel(L"\n请选择占地线实体 : ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if(rc==RTNORM)
	{
		acdbGetObjectId(eId, en);     
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();  
		if (pObj->isKindOf(GTZDK_ROAD::desc())) //是GTZDX实体,取数据
		{		
			//		pzx = GTZDK::cast(pObj);
			/*acdbOpenObject(pzx, eId, AcDb::kForRead);*/	
			if(acdbOpenObject(pzx,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pzx->close();

			if(ZDKINPUTDlg!=NULL){delete ZDKINPUTDlg;ZDKINPUTDlg=NULL;}
			ZDKINPUTDlg=new CZDKSHEET(pzx,eId,L"编辑用地宽",acedGetAcadFrame());		
			ZDKINPUTDlg->Create(acedGetAcadFrame(),WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE);

		}
	}
}

//编辑用地数据
void EDITZDK_Road()
{
	//AfxMessageBox(L"1");
	CEditZDKDlg pEditZDKDlg ;
	if(pEditZDKDlg.DoModal()==IDOK)
	{
		RefreshYDArea();
	}
	//pEditZDKDlg->Create(IDD_DIALOG_EDITZDK, acedGetAcadFrame());
	//pEditZDKDlg->ShowWindow(SW_SHOW);
}
//删除夹直线
void DeleteJZX()
{
	//AfxMessageBox(L"1");
	
	ads_name en;
	ads_point pt;//新位置
	AcDbObject* pObj = NULL;
	AcDbObjectId eId;
	if(RTNORM!=acedEntSel(L"\n选择线路:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);
	//acdbOpenObject(pObj,eId,AcDb::kForRead);
	if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)
		return;

	pObj->close();
	int bJDno;
	int eJDno;
	if(pObj->isKindOf(JD_CENTER::desc()))
	{
		/*acdbOpenObject(pObj,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		JD_CENTER *pline = JD_CENTER::cast(pObj);
		pline->assertWriteEnabled();
		bJDno=pline->GetPreJD(pt[1],pt[0]);
		CString str;
		
		wchar_t Chose[10];
		int	retun_Retval = acedGetString(0, L"是否均匀分配夹直线前后缓长[是（Y）/否（N）](Y):", Chose);
		if (retun_Retval == RTNORM && !_tcscmp(Chose, L""))
		{
			
		}
		else
		{
			if (!_tcscmp(Chose, L"Y") || !_tcscmp(Chose, L"y"))
			{

			}
			else if (!_tcscmp(Chose, L"N") || !_tcscmp(Chose, L"n"))
			{
				pline->IfDelJZX = TRUE;
				pline->PreJD = bJDno;
				pline->Ls1 = pline->JdCenArray[bJDno].Ls2;//夹直线长
			}
		}
		
		if (bJDno>=pline->JdCenNum-1)
		{
			return;
		}
		eJDno=bJDno+1;
		if (pline->JdCenArray[bJDno].afa*pline->JdCenArray[eJDno].afa<0)
		{
			pline->JdCenArray[bJDno].LinkAfter=S_Link;
			//pline->JdCenArray[eJDno].LinkAfter=S_Link;
		}
		else
		{
			pline->JdCenArray[bJDno].LinkAfter=C_Link;
			//pline->JdCenArray[eJDno].LinkAfter=C_Link;
		}
		pline->close();
	}
	else
		ads_printf(L"选择的非线路实体!\n");
	//pObj->close();

	//pEditZDKDlg->Create(IDD_DIALOG_EDITZDK, acedGetAcadFrame());
	//pEditZDKDlg->ShowWindow(SW_SHOW);
}

GouPDSheet *GouPDdlg;	
//调整水沟坡度
void AdjustGouPD()
{
	if(GouPDdlg!=NULL){delete GouPDdlg;GouPDdlg=NULL;}
	GouPDdlg=new GouPDSheet(L"排水沟底纵坡数据",acedGetAcadFrame());		
	GouPDdlg->Create(acedGetAcadFrame(),WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE);


	//    GouPDSheet dlg("排水沟底纵坡数据");	
	//	if(dlg.DoModal()==IDOK)
	//	{
	//		//写数据库
	//		HdmDataBase xlmdb;
	//		for(int i=0; i<dlg.xlnum*2; i++)
	//		{
	//			if(dlg.GouPDDlgAry[i].ZorY==-1)
	//			{
	//		        xlmdb.NZGouPD = dlg.GouPDDlgAry[i].NGouPD;
	//			    if(xlmdb.ZGouPD) delete[]xlmdb.ZGouPD;xlmdb.ZGouPD=NULL;
	//			    if(xlmdb.NZGouPD>0)
	//				{
	//			       xlmdb.ZGouPD = new  BAS_DRAW_FUN::GouPDdata[xlmdb.NZGouPD];
	//			       for(int j=0; j<xlmdb.NZGouPD; j++)
	//                        xlmdb.ZGouPD[j] = dlg.GouPDDlgAry[i].GouPD[j];
	//			   //写数据库
	//                   xlmdb.WriteGouPDB(dlg.mdbFname,dlg.road[i].roadname,L"左沟底坡"); 
	//			    ads_printf(L"\n%s的左沟底坡已存入%s中!\n",dlg.road[i].roadname,dlg.mdbFname);
	//				}
	//			}
	//			else
	//			{
	//			    xlmdb.NYGouPD = dlg.GouPDDlgAry[i].NGouPD;
	//			    if(xlmdb.YGouPD) delete[]xlmdb.YGouPD;xlmdb.YGouPD=NULL;
	//			    if(xlmdb.NYGouPD>0)
	//				{
	//			       xlmdb.YGouPD = new  BAS_DRAW_FUN::GouPDdata[xlmdb.NYGouPD];
	//			       for(int j=0; j<xlmdb.NYGouPD; j++)
	//                        xlmdb.YGouPD[j] = dlg.GouPDDlgAry[i].GouPD[j];
	//			   //写数据库
	//                   xlmdb.WriteGouPDB(dlg.mdbFname,dlg.road[i].roadname,L"右沟底坡"); 
	//			       ads_printf(L"\n%s的右沟底坡已存入%s中!\n",dlg.road[i].roadname,dlg.mdbFname);
	//				}
	//			}
	//		}
	//	}
}
// This is command 'BL'
// This is command 'BL'
#include "SetLineDlg1.h"
CSetLineDlg * m_pdlgsl = NULL;
void penglihuibl()//定直线
{
	ads_name EntityName;
	ads_point BasePoint;
	AcDbObjectId EntityId =0;
	if (ads_entsel(L"\n选择实体:",EntityName,BasePoint) != RTNORM) 
	{
		acutPrintf(L"没有选中实体!");
		return;
	}	
	if(acdbGetObjectId(EntityId,EntityName) != Acad::eOk)
	{
		acutPrintf(L"获取实体对象ID失败!");
		return;
	}	
	if (m_pdlgsl) 
	{
		delete m_pdlgsl;
		m_pdlgsl = NULL;
	}
	m_pdlgsl = new CSetLineDlg(EntityId,acedGetAcadFrame());
	//ZL2019.11.4
	AcGePoint3d Point;
	Point.x = BasePoint[X];
	Point.y = BasePoint[Y];
	Point.z = 0.00;
	m_pdlgsl->point = Point;//第
	m_pdlgsl->Create(IDD_DIALOGSETLINE);
	m_pdlgsl->ShowWindow(SW_SHOW);	
}
#include "SetArcOrCircle.h"
// This is command 'BA'
CSetArcOrCircle * m_pdlgsa = NULL;
void penglihuiBA()
{
	ads_name EntityName;
	ads_point BasePoint;
	AcDbObjectId EntityId =0;
	if (ads_entsel(L"\n选择实体:",EntityName,BasePoint) != RTNORM) 
	{
		acutPrintf(L"没有选中实体!");
		return;
	}	
	if(acdbGetObjectId(EntityId,EntityName) != Acad::eOk)
	{
		acutPrintf(L"获取实体对象ID失败!");
		return;
	}
	if (m_pdlgsa) 
	{
		delete m_pdlgsa;
		m_pdlgsa = NULL;
	}
	m_pdlgsa = new CSetArcOrCircle(EntityId);	
	//ZL2019.11.4
	AcGePoint3d Point;
	Point.x = BasePoint[X];
	Point.y = BasePoint[Y];
	Point.z= 0.00;
	m_pdlgsa->PT = Point;//第一个线元
	m_pdlgsa->Create(IDD_DIALOGSETARC);
	m_pdlgsa->ShowWindow(SW_SHOW);			
}

// This is command 'BS'
#include "SetSprialdlg.h"
SetSprialdlg * m_pdlgss = NULL;
void penglihuibs()
{
	ads_name EntityName;
	ads_point BasePoint;
	AcDbObjectId EntityId =0;
	if (ads_entsel(L"\n选择实体:",EntityName,BasePoint) != RTNORM) 
	{
		acutPrintf(L"没有选中实体!");
		return;
	}	
	if(acdbGetObjectId(EntityId,EntityName) != Acad::eOk)
	{
		acutPrintf(L"获取实体对象ID失败!");
		return;
	}
	if (m_pdlgsa) 
	{
		delete m_pdlgss;
		m_pdlgss = NULL;
	}
	m_pdlgss = new SetSprialdlg(EntityId);		
	//ZL2019.11.4
	AcGePoint3d Point;
	Point.x = BasePoint[X];
	Point.y = BasePoint[Y];
	Point.z = 0.00;
	m_pdlgss->point = Point;//第
	m_pdlgss->Create(IDD_DLGSETSPRIAL);
	m_pdlgss->ShowWindow(SW_SHOW);
}

// This is command 'CXJ'//线元转交点
void penglihuiCXJ()
{
	// TODO: Implement the command
	ads_name EntityName;
	ads_point BasePoint;
	AcDbObjectId EntityId =0;
	if (ads_entsel(L"\n选择实体:",EntityName,BasePoint) != RTNORM) 
	{
		acutPrintf(L"没有选中实体!");
		return;
	}	
	if(acdbGetObjectId(EntityId,EntityName) != Acad::eOk)
	{
		acutPrintf(L"获取实体对象ID失败!");
		return;
	}
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	AcDbEntity * pEntity = NULL;
	//acdbOpenObject(pEntity,EntityId,AcDb::kForWrite);
	if(acdbOpenObject(pEntity,EntityId,AcDb::kForWrite)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER")==0)
	{
		JD_CENTER * pJC = NULL;
		pJC = JD_CENTER::cast(pEntity);
		pJC->assertWriteEnabled();	
		pJC->XYArrayToJD();
		pJC->IfCalXY = false;		
		pJC->SHOWJD = true;
		pJC->close();			
	}
	else 
	{
		AfxMessageBox(L"所选不是线路实体或没有选中实体!");
		pEntity->close();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
}

//调整桩号序列
void AjustSta()
{
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;

	Path = Pro+".mdb";
	CFileDialog FDlg(TRUE,L".mdb",Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_LONGNAMES;
	FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
	if(FDlg.DoModal()==IDOK)
	{
		POSITION Pos=FDlg.GetStartPosition();
		while(Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos); 			
			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名			
			for(int i=0; i<DBS.m_RoadSum; i++)
			{
				JD_CENTER *pJC=NULL;
				//				DBS.Read_XLDbs(Path,L"控制点表",DBS.Road[i].RoadName);
				DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);						
				DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);			
				if(DBS.XYNum>0)
				{
					pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
					//					pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);
					if(DBS.NDL>0)
						pJC->setDLB(DBS.DLArray,DBS.NDL);

					DBS.Read_XLDbs(Path,L"桩号序列",DBS.Road[i].RoadName);
					pJC->NLCB = DBS.NSTA;

					pJC->pLCB.RemoveAll();					
					for(int j=0; j<pJC->NLCB; j++)
						pJC->pLCB.Add(DBS.STA[j]);

					if(DBS.STA) delete []DBS.STA;
					DBS.STA = NULL;
					DBS.NSTA = 0;					
				}				
			}
			DBS.Release();
		}
	}
}

#include "FastTin.h"
//拼合计算
void pDX3D()
{
	// TODO: Implement the command

	CFastTin Tindlg;
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;

	Path = Pro+".mdb";
	CFileDialog FDlg(TRUE,L".mdb",Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_LONGNAMES;
	FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
	if(FDlg.DoModal()==IDOK)
	{
		POSITION Pos=FDlg.GetStartPosition();
		while(Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos); 			
			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名

			for(int i=0; i<DBS.m_RoadSum; i++)
			{
				JD_CENTER *pJC=NULL;

				//				DBS.Read_XLDbs(Path,L"控制点表",DBS.Road[i].RoadName);
				DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);	

				DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);	

				if(DBS.XYNum>0)
				{

					pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
					//					pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);

					if(DBS.NDL>0)
						pJC->setDLB(DBS.DLArray,DBS.NDL);
					CDrawPM *drawpm = new CDrawPM(pJC,pJC->DLArray[0].ELC,pJC->DLArray[pJC->DLNum-1].BLC);
					drawpm->FormHole();			
					Tindlg.RoadHole[Tindlg.NRoad]=drawpm;
					Tindlg.NRoad++;
					//					break;
				}	

			}

			DBS.Release();
		}

		Tindlg.FastCreateTin();

	}

}



void pInitLw()
{
	/*DLInintLwDlg *pInit = NULL;
	pInit = new DLInintLwDlg;
	pInit->Create(IDD_DIALOG_DLINI, acedGetAcadFrame());
	pInit->ShowWindow(SW_SHOWNORMAL);*/

	DLInintLwDlg dlg;
	dlg.DoModal();

}
void pSysmLw()
{
	AfxMessageBox(L"pSysmLw");
}
void pZtopLw()
{
	AfxMessageBox(L"pZtopLw");
}
extern int Read_Dtm();
extern void ReleaseDtm();//释放Dtm
//CAD中绘制3D模型
void pLX3D()
{
	// TODO: Implement the command
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;

	Path = Pro+".mdb";
	CFileDialog FDlg(TRUE,L".mdb",Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_LONGNAMES;
	FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
	if(FDlg.DoModal()==IDOK)
	{
		ads_printf(L"正在读取地形模型，请稍候\n");
		CDraw3D draw3dlg;
		if(!Read_Dtm())//读dtm
			return ;
		draw3dlg.draw_Terrain();
		ReleaseDtm();	

		POSITION Pos=FDlg.GetStartPosition();
		while(Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos); 			
			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名			
			for(int i=0; i<DBS.m_RoadSum; i++)
			{
				JD_CENTER *pJC=NULL;
				//				DBS.Read_XLDbs(Path,L"控制点表",DBS.Road[i].RoadName);
				DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);						
				DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);			
				if(DBS.XYNum>0)
				{
					pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
					//					pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);
					if(DBS.NDL>0)
						pJC->setDLB(DBS.DLArray,DBS.NDL);
					CDraw3D *draw3d = new CDraw3D();
					draw3d->pm = pJC;
					draw3d->ReadLMK("左路面宽分段表");
					draw3d->ReadLMK("右路面宽分段表");
					draw3d->Create3DModel();           
					//					break;
				}				
			}
			DBS.Release();
		}
	}
}

//里程转坐标
void PmGuide()
{
	PMGUIDE *pPmguide;
	pPmguide = new PMGUIDE;
	pPmguide->Create(IDD_DIALOG_Guide,acedGetAcadFrame());
	pPmguide->ShowWindow(SW_SHOW);
}

//里程转坐标
void pPMLCTOZB()
{
	// TODO: Implement the command	
	CLCZZBDlg  PmLcToZbDlb;
	//CLcToZbDialog
	PmLcToZbDlb.DoModal();
}

//坐标转里程
void pPMZBTOLC()
{
	// TODO: Implement the command

	ZBTOLC *PmZbToLcDlg;
	PmZbToLcDlg = new ZBTOLC;
	PmZbToLcDlg->Create(IDD_DIALOG_ZBTOLC,acedGetAcadFrame());
	PmZbToLcDlg->ShowWindow(SW_SHOW);
}

//查询路面标高
void CXLMBG()
{
	// TODO: Implement the command

	CCXLMBG *Pcxlmbg;
	Pcxlmbg = new CCXLMBG;
	Pcxlmbg->Create(IDD_DIALOG_CXLMBG,acedGetAcadFrame());
	Pcxlmbg->ShowWindow(SW_SHOW);
}

//查询路面标高
void CXYDAREA()
{
	// TODO: Implement the command	
	CCalYDArea *PcxYd;
	PcxYd = new CCalYDArea;
	PcxYd->Create(IDD_DIALOG_YDAREA,acedGetAcadFrame());
	PcxYd->ShowWindow(SW_SHOW);
}

// This is command 'CXJ'
void CreatePrj()
{
	if(PMenuBar)
		PMenuBar->OnMenuitemNEWXM();

}
void OpenPrj()
{
	if(PMenuBar)
		PMenuBar->OnMenuitemBDXM();
}

void InMdb()
{
	if(PMenuBar)
		PMenuBar->OnMenuitemDATA();
}
//数模
// This is command 'PICKDATDA'
void ppickdatdapickdatda()//采集数据
{
	// TODO: Implement the command
	if(PMenuBar)
		PMenuBar->OnMENUITEMtqsj();  
}

// This is command 'DEALDATA'
void pdealdatadealdata()//处理数据
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMdealdata();  
}

// This is command 'CREATETIN'
void pcreatetincreatetin()//建立数模
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMcreateTin();  
}

// This is command 'DIAPLAYDTM'
void pdiaplaydtmdiaplaydtm()//显示数模
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMdisplay();  
}

void pSetCgBz()//设置超高标准
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMSETCGBZ();  
}

void pSetJkBz()//设置加宽标准
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMSETJKBZ();  
}
// This is command '纵面设计标准'
void ZmSJBZ()
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMsetZMSJBZ();// This is command 'CXJ'
}

// This is command '横断面基本参数'
void HdmJBCS()
{
	if(PMenuBar)
		PMenuBar->OnMENUITEMsetHmCs();// This is command 'CXJ'

}

// This is command '横断面基本参数'，为ribbon菜单而设置
void HdmJBCS2()  
{
	// TODO: Add your  handler code here
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\Roadhdmdata.exe",SysPath);
	ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);
}


#include "ZDMKZBG.h"
void DrawKZBG()
{
	ZDMKZBG *KzbgDlg;
	KzbgDlg=new ZDMKZBG;
	KzbgDlg->Create(IDD_DIALOG_XZBG,acedGetAcadFrame());
	KzbgDlg->ShowWindow(SW_SHOW);
	KzbgDlg->SetFocus();
}

#include "JMBXdlg.h"
JMBXDlg *jmbxdlg=NULL;
void JMBX()
{
	if (jmbxdlg) 
	{
		delete jmbxdlg;
		jmbxdlg = NULL;
	}
	jmbxdlg = new JMBXDlg;
	jmbxdlg->Create(IDD_DIALOG_JMBX);
	jmbxdlg->ShowWindow(SW_SHOW);	

}

//计算线元交点
void CalXYJD()
{
	int NLC=3,NH=3,texth=8;
	int rc;
	ads_name en;
	AcDbObject *pObj;
	AcDbObjectId eId,GTZXId;
	AcGePoint3d pt,location;
	JD_CENTER *pJC1=NULL,*pJC2=NULL;
	BAS_DRAW_FUN ob;

	rc = ads_entsel(L"\n选择第一个平面线路实体 : ", en, asDblArray(pt));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{		
			pObj->close();
			pJC1 = JD_CENTER::cast(pObj);
		}
		else
		{
			ads_printf(L"不是平面线路实体!\n");
			return;
		}
	}
	else
		return;

	rc = ads_entsel(L"\n选择第二个平面线路实体 : ", en, asDblArray(pt));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{		
			pObj->close();
			pJC2 = JD_CENTER::cast(pObj);
		}
		else
		{
			ads_printf(L"不是平面线路实体!\n");
			return;
		}
	}
	else
		return;

	AcGePoint2dArray JDpt;
	int JDNUM;
	double ResPt[3],ResPt1[3];
	AcGePoint2d respt;
	if(JDpt.length()>0)
		JDpt.removeSubArray(0,JDpt.length()-1);
	if(pJC1 && pJC2)
	{
		//计算线元交点
		if(pJC1->XYNum>0)
		{
			for(int i=1; i<pJC1->XYNum; i++)
			{
				for(int j=1; j<pJC2->XYNum; j++)
				{
					JDNUM = ob.CalJD_XY1_XY2(pJC1->XYArray[i],pJC2->XYArray[j],ResPt,ResPt1);	
					//					if(JDNUM>0)
					//ads_printf(L"JDNUm=%d sml1=%lf xy1=%lf sml2=%lf xy2=%lf %lf %lf\n",
					//JDNUM,pJC1->XYArray[i][6],pJC1->XYArray[i][0],pJC2->XYArray[j][6],pJC2->XYArray[j][0]
					//,ResPt[0],ResPt[1]);
					if(JDNUM>0)
					{
						respt.set(ResPt[0],ResPt[1]);
						JDpt.append(respt);						
						//ads_printf(L"1:x=%lf y=%lf\n",ResPt[0],ResPt[1]);
					}
					if(JDNUM==2)
					{
						respt.set(ResPt1[0],ResPt1[1]);
						JDpt.append(respt);	
						//ads_printf(L"2:x=%lf y=%lf\n",ResPt1[0],ResPt1[1]);					   
					}

				}
			}

		}

	}

	if(JDpt.length()>0)
	{
		for(int i=0; i<JDpt.length(); i++)
		{
			//		ads_printf(L"%d %lf %lf\n",i,JDpt[i].x,JDpt[i].y);

			ads_point cpt;
			cpt[0]=JDpt[i].y,cpt[1]=JDpt[i].x;
			//	   acedCommandS(RTSTR,L"Circle",RTPOINT,cpt,RTREAL,2.0,RTNONE);
			//计算在线路1上的投影里程
			double cml1,cml2,fwj1,fwj2,zj;
			CString xlc1,xlc2;
			cml1 = pJC1->PROJ_ML(JDpt[i].x,JDpt[i].y);
			xlc1 = pJC1->XLC(cml1);
			BAS_DRAW_FUN::xlpoint PZ;
			PZ.lc = cml1;
			pJC1->xlpoint_pz(&PZ);
			fwj1 = PZ.a;

			cml2 = pJC2->PROJ_ML(JDpt[i].x,JDpt[i].y);
			xlc2 = pJC2->XLC(cml2);
			PZ.lc = cml2;
			pJC2->xlpoint_pz(&PZ);
			fwj2 = PZ.a;
			zj = pJC1->ZXJ(fwj2,fwj1);
			//绘图
			AcGePoint2d pt,pt1,pt2;
			pt.x=JDpt[i].y,pt.y=JDpt[i].x;
			pt1.x = JDpt[i].y + 100 * sin(fwj1+0.5*zj);
			pt1.y = JDpt[i].x + 100 * cos(fwj1+0.5*zj);
			if(pt1.x>pt.x)
				pt2.x = pt1.x + 100;
			else
				pt2.x = pt1.x - 100;
			pt2.y = pt1.y;

			AcDbPolyline *pPline=new AcDbPolyline(3);
			pPline->addVertexAt(0,pt);
			pPline->addVertexAt(1,pt1);
			pPline->addVertexAt(2,pt2);
			AcDbObjectId plId;
			pPline->setColorIndex(5);
			plId=pJC1->AddEntityToDbs(pPline);

			//标注
			AcGePoint3d mpt;
			ACHAR text[80];
			mpt.x = 0.5*(pt1.x+pt2.x);
			mpt.y = 0.5*(pt1.y+pt2.y)+6;
			_tcscpy(text,xlc1);
			pJC1->maketext(mpt,text,0.0,8);

			mpt.y = 0.5*(pt1.y+pt2.y)-6;
			_tcscpy(text,xlc2);
			pJC1->maketext(mpt,text,0.0,8);

			mpt.y -= 10;	  
			_stprintf(text,L"夹角%0.5lf",fabs(pJC1->dms_rad(zj)));
			pJC1->maketext(mpt,text,0.0,8);
		}
	}   
}

void DrawDBZX()
{
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	CDrawPM  drawpmzx;
	Path = Pro+".mdb";
	CFileDialog FDlg(TRUE,L".mdb",Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_LONGNAMES;
	FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
	if(FDlg.DoModal()==IDOK)
	{
		POSITION Pos=FDlg.GetStartPosition();
		while(Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos); 			
			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名

			for(int i=0; i<DBS.m_RoadSum; i++)
			{
				JD_CENTER *pJC=NULL;
				//					DBS.Read_XLDbs(Path,L"控制点表",DBS.Road[i].RoadName);
				DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);							
				DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);			
				if(DBS.XYNum>0)
				{
					pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
					//						pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);
					if(DBS.NDL>0)
					{
						pJC->setDLB(DBS.DLArray,DBS.NDL);
					}
					drawpmzx.pm = pJC;
					drawpmzx.Scml = pJC->DLArray[0].ELC;
					drawpmzx.Ecml = pJC->XLLength;
					drawpmzx.DrawXLZX();
				}
			}
			DBS.Release();
		}				
	}
}
#include "DrawBDCIR.h"
DrawBDCIR *bdcirdlg=NULL;
void DrawDBCIR()
{
	bdcirdlg=new DrawBDCIR(acedGetAcadFrame());		
	bdcirdlg->Create(IDD_DIALOG_BDYHZ,acedGetAcadFrame());
	bdcirdlg->ShowWindow(SW_SHOW);
	return;
}

DUANBUHX *dbdlg=NULL;	
void DrawDBHX()
{
	dbdlg=new DUANBUHX(acedGetAcadFrame());		
	dbdlg->Create(IDD_DIALOG_LJDBHX,acedGetAcadFrame());
	dbdlg->ShowWindow(SW_SHOW);
	return;
}

#include "DBBZ.h"
DBBZ *dbbzdlg=NULL;
void DrawDBBZ()
{
	dbbzdlg=new DBBZ(acedGetAcadFrame());		
	dbbzdlg->Create(IDD_DIALOG_DBBZ,acedGetAcadFrame());
	dbbzdlg->ShowWindow(SW_SHOW);
	return;
}

//修改端部标注
#include "DELDBBZ.h"

CDELDBBZ *pDelDbbzdlg=NULL;
void XGDBBZ()
{
	ACHAR kw[8];
	int mode=0;

	acedInitGet(RSG_OTHER,L"T t C c D d");
	int rt=acedGetKword(L"\nT-删文字/C-刷新数据/D-删横线 <T>: ",kw);
	if(rt==RTNONE)
		_tcscpy(kw,L"T");

	if(_tcscmp(kw,L"T")==0 || _tcscmp(kw,L"t")==0)
	{
		pDelDbbzdlg=new CDELDBBZ(acedGetAcadFrame());		
		pDelDbbzdlg->Create(IDD_DIALOG_DELDBBZ,acedGetAcadFrame());
		pDelDbbzdlg->ShowWindow(SW_SHOW);
		return;
	}
	/*
	else if(_tcscmp(kw,L"S")==0 || _tcscmp(kw,L"s")==0)
	{
	mode = 1;
	}*/
	else if(_tcscmp(kw,L"C")==0 || _tcscmp(kw,L"c")==0)
	{
		int rc;
		AcGePoint3d PT;
		ads_name en;
		int i,xsw;
		double cml,tmp;
		CString mdbname,roadname,hxbh;

		mdbname=roadname="";
		rc = ads_entsel(L"\n选择要修改标注的端部横线 : ", en, asDblArray(PT));
		if (rc == RTNORM) 	   
		{   	
			AcDbObjectId eId;
			AcDbObjectIdArray IdArr;
			acdbGetObjectId(eId, en);
			BAS_DRAW_FUN::GetDBXdata(eId,mdbname,roadname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);			
			if(mdbname!=""&&roadname!="")
			{
				AcArray<CString> layerArrayHX;
				layerArrayHX.append("端部横线");
				AcDbObjectIdArray HxIdArr;
				AcDbPolyline *pent;
				AcDbObject *pobj;
				BAS_DRAW_FUN::GetDBHXEntity(HxIdArr,layerArrayHX,mdbname,roadname,hxbh);
				for(i=0;i<HxIdArr.length();i++)
				{
					//acdbOpenObject(pobj,HxIdArr[i],AcDb::kForRead);
					if(acdbOpenObject(pobj,HxIdArr[i], AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pobj->close();				
					pent = AcDbPolyline::cast(pobj); 
					pent->highlight();
				}			     

				AcArray<CString> layerArray;
				layerArray.append("路肩高");
				layerArray.append("设计高");
				layerArray.append("路面横坡");
				layerArray.append("路肩横坡");

				double curx,cury,prex,prey;
				AcDbEntity *pEnt;
				BAS_DRAW_FUN::GetDBHXEntity(IdArr,layerArray,mdbname,roadname,hxbh);
				CString layername;
				JD_CENTER *m_pm;
				XLDataBase DBS;
				DBS.Read_XLDbs(mdbname,L"线元表",roadname);
				DBS.Read_XLDbs(mdbname,L"断链表",roadname);
				if(DBS.XYNum>0)
				{
					m_pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
					//			m_pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					if(DBS.NDL>0)
						m_pm->setDLB(DBS.DLArray,DBS.NDL);
					_tcscpy(m_pm->mdbname,mdbname);
					_tcscpy(m_pm->RoadName,roadname);     
				}
				if(m_pm)
				{
					CDrawPM drawpm;
					drawpm.pm = m_pm;
					drawpm.Scml = m_pm->XYArray[1][6];
					drawpm.Ecml = m_pm->XYArray[m_pm->XYNum][6];
					drawpm.ReadLMK("左路面宽分段表");
					drawpm.ReadLMK("右路面宽分段表");
					drawpm.DrawBGDATA();
					double prjml,value,hp,ht1,ht2,hp1;
					int xsw;
					CString text;
					int pos,len;
					ACHAR bztext[180],str[80];
					for(i=0;i<IdArr.length();i++)
					{
						BAS_DRAW_FUN::GetDBXdata(IdArr[i],mdbname,roadname,cml,hxbh,curx,cury,prex,prey,xsw);
						/*acdbOpenObject(pEnt,IdArr[i],AcDb::kForRead);*/
						if(acdbOpenObject(pEnt,IdArr[i], AcDb::kForRead)!=Acad::eOk)
						{
							ads_printf(L"打开实体失败！\n");
							return;
						}
						pEnt->close();
						if(pEnt->isKindOf(AcDbText::desc()))
						{
							layername = pEnt->layer();
							AcDbText *pText;
							/*
							text = pText->textString();	
							pos = text.Find(L".");
							len = text.GetLength();
							text=text.Right(len-pos-1);
							pos = text.Find(L"%");
							ads_printf(L"%s\n",text);
							if(pos!=-1)
							text=text.Left(pos);
							xsw = text.GetLength();*/

							if(layername=="路肩高")
							{
								drawpm.CalTLMHt(prey,prex,cury,curx,prjml,value,hp);
								ads_rtos(value,2,xsw,bztext);																	
							}
							else if(layername=="设计高")
							{
								drawpm.CalYLMHt(cury,curx,prjml,value,hp);								
								ads_rtos(value,2,xsw,bztext);																
							}
							else if(layername=="路面横坡")
							{
								drawpm.CalYLMHt(prey,prex,prjml,ht1,hp);
								drawpm.CalYLMHt(cury,curx,prjml,ht2,hp);
								value = sqrt((prex-curx)*(prex-curx)+(prey-cury)*(prey-cury));					
								hp1 = (ht2-ht1)*100/value;								
								ads_rtos(fabs(hp1),2,xsw,str);								
								_stprintf(bztext,L"%s%s",str,L"%");								
							}
							else//路肩横坡
							{
								drawpm.CalYLMHt(prey,prex,prjml,ht1,hp);
								drawpm.CalTLMHt(prey,prex,cury,curx,prjml,ht2,hp);
								value = sqrt((prex-curx)*(prex-curx)+(prey-cury)*(prey-cury));
								hp1 = (ht2-ht1)*100/value;
								ads_rtos(fabs(hp1),2,xsw,str);	
								_stprintf(bztext,L"%s%s",str,L"%");								
							}

							if(acdbOpenObject(pText,IdArr[i],AcDb::kForWrite)==Acad::eOk)
							{
								pText->setTextString(bztext);
								pText->close();								   
							}
						}
					}
				}
			}
		}

	}
	else//删横线
	{
		int rc,xsw;
		AcGePoint3d PT;
		ads_name en;
		int i;
		AcDbObject *pobj;
		double cml,tmp;
		AcDbPolyline *pent;	
		CString mdbname,roadname,hxbh;

		rc = ads_entsel(L"\n选择一条横线 : ", en, asDblArray(PT));
		if (rc == RTNORM) 	   
		{   	
			AcDbObjectId eId;
			AcDbObjectIdArray IdArr;
			acdbGetObjectId(eId, en);
			//acdbOpenObject(pobj,eId,AcDb::kForRead);
			if(acdbOpenObject(pobj,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pobj->close();
			if(pobj->isKindOf(AcDbPolyline::desc()))
			{
				BAS_DRAW_FUN::GetDBXdata(eId,mdbname,roadname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);				
				AcArray<CString> layerArray;
				layerArray.append("端部横线");
				BAS_DRAW_FUN::GetDBHXEntity(IdArr,layerArray,mdbname,roadname,hxbh);
				AcDbEntity *pEnt;
				for(i=0;i<IdArr.length();i++)
				{
					if(acdbOpenObject(pEnt,IdArr[i],AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
					{
						pEnt->erase();
						pEnt->close();
					}
				}
			}
		}			  		
	}
}

//方案审查
void CheckPro()
{
	// TODO: Implement the command
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	double DV;
	CString ErrMes;
	CStringArray ErrMesArr;
	ACHAR str[80],dvstr[80];


	Path = Pro+".mdb";
	CFileDialog FDlg(TRUE,L".mdb",Path);
	FDlg.m_ofn.lpstrDefExt = L"mdb";
	FDlg.m_ofn.lpstrInitialDir = WorkDir.DataPath;
	//支持多个数据库输入
	FDlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_LONGNAMES;
	FDlg.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";		
	if(FDlg.DoModal()==IDOK)
	{
		POSITION Pos=FDlg.GetStartPosition();
		while(Pos)
		{
			//依次打开每一个数据库
			Path = FDlg.GetNextPathName(Pos); 			
			DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名
			if(DBS.Read_XLDbs(Path,L"线路属性"))//读取线路数据库中的数据
				DV = DBS.RoadXZ.DV;
			else
			{
				ads_printf(L"%s方案尚未输入设计车速!\n",Path);
				DV = 100.0;
			}

			DBS.Read_XLDbs(Path,L"超高计算标准");
			int NSUPH=0;
			double SuphData[20][2];
			NSUPH = DBS.NuphData-1;
			int j;
			for( j=0; j<NSUPH; j++)
			{
				SuphData[j][0] = DBS.SuphData[j][0];
				SuphData[j][1] = DBS.SuphData[j][1];
			}

			for(int i=0; i<DBS.m_RoadSum; i++)
			{	
				//平面
				JD_CENTER *pJC=NULL;
				DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);			
				DBS.Read_XLDbs(Path,L"线元表",DBS.Road[i].RoadName);	
				if(DBS.XYNum>0)
				{
					pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);
					pJC->setRoadName(DBS.Road[i].RoadName);
					pJC->setMdbName(Path);
					if(DBS.NDL>0)
					{
						pJC->setDLB(DBS.DLArray,DBS.NDL);
					}
					//检测平面
					for(int ixy=1; ixy<DBS.XYNum; ixy++)
					{
						//检查夹直线
						if(pJC->XYArray[ixy][0]<1.1)//直线
						{
							if((pJC->XYArray[ixy-1][4] * pJC->XYArray[ixy+1][4]) > 0.001)//同向曲线
								if(pJC->XYArray[ixy][1] < 6*DV && pJC->XYArray[ixy][1]>0.1)
								{
									CString lc;
									lc = pJC->XLC(pJC->XYArray[ixy][6]+0.5*pJC->XYArray[ixy][1]);
									ACHAR str[80],dvstr[80];
									ads_rtos(pJC->XYArray[ixy][1],2,3,str);
									ads_rtos(6*DV,2,3,dvstr);
									ErrMes.Format(L"错误：%s-%s 里程 %s 处 夹直线长%s 小于%s(6倍车速)",Path,DBS.Road[i].RoadName,lc,str,dvstr);
									ErrMesArr.Add(ErrMes);
								}
								else 	if((pJC->XYArray[ixy-1][4] * pJC->XYArray[ixy+1][4]) < -0.001)//反向曲线
									if( pJC->XYArray[ixy][1] < 2*DV && pJC->XYArray[ixy][1]>0.1)
									{
										CString lc;
										lc = pJC->XLC(pJC->XYArray[ixy][6]+0.5*pJC->XYArray[ixy][1]);
										ACHAR str[80],dvstr[80];
										ads_rtos(pJC->XYArray[ixy][1],2,3,str);
										ads_rtos(2*DV,2,3,dvstr);
										ErrMes.Format(L"错误：%s-%s 里程 %s 处 夹直线长%s 小于%s(2倍车速)",Path,DBS.Road[i].RoadName,lc,str,dvstr);
										ErrMesArr.Add(ErrMes);
									}
						}
						else if(pJC->XYArray[ixy][0]>2.1)//缓和曲线
						{
							//前缓
							double A,R,L;
							A = pJC->XYArray[ixy][1];
							if(pJC->XYArray[ixy][0]<4.1)
							{
								R = pJC->XYArray[ixy][2];
								L = A*A/R;
							}
							else
							{
								R = pJC->XYArray[ixy][2] < pJC->XYArray[ixy][3] ? pJC->XYArray[ixy][2] : pJC->XYArray[ixy][3];
								L = fabs(A*A/pJC->XYArray[ixy][2]-A*A/pJC->XYArray[ixy][3]);
							}

							if(A>R || A<R/3.0)  
							{
								CString lc;
								lc = pJC->XLC(pJC->XYArray[ixy][6]+0.5*pJC->XYArray[ixy][2]);

								ads_rtos(A,2,3,str);
								ads_rtos(R,2,3,dvstr);
								ErrMes.Format(L"错误：%s-%s 里程 %s 处 缓和曲线A值%s 不满足R/3～R(R:%s)",Path,DBS.Road[i].RoadName,lc,str,dvstr);
								ErrMesArr.Add(ErrMes);
							}
							//最小回旋线长度限制
							double Lmin;
							BAS_DRAW_FUN ob;
							ob.GetLoMin(DV,Lmin);
							if(L<Lmin)
							{
								CString lc;
								lc = pJC->XLC(pJC->XYArray[ixy][6]+0.5*L);
								ads_rtos(L,2,3,str);
								ads_rtos(Lmin,2,3,dvstr);
								ErrMes.Format(L"错误：%s-%s 里程 %s 处 缓和曲线长度%s 大于最小缓和曲线长%s",Path,DBS.Road[i].RoadName,lc,str,dvstr);
								ErrMesArr.Add(ErrMes);

							}

						}
						else if(pJC->XYArray[ixy][0]<2.1 && NSUPH>0)
						{
							double R = pJC->XYArray[ixy][1];
							double L = pJC->XYArray[ixy][2];
							double Rmin,Rmax;
							Rmin = SuphData[0][0] < SuphData[NSUPH-1][0] ? SuphData[0][0] : SuphData[NSUPH-1][0];
							Rmax = SuphData[0][0] > SuphData[NSUPH-1][0] ? SuphData[0][0] : SuphData[NSUPH-1][0];
							if(R<Rmin-0.001 || R>Rmax+0.001)
							{
								CString lc;
								lc = pJC->XLC(pJC->XYArray[ixy][6]+0.5*L);
								ACHAR str[80],dvstr[80],dvstr1[80];
								ads_rtos(R,2,3,str);
								ads_rtos(Rmin,2,3,dvstr);
								ads_rtos(Rmax,2,3,dvstr1);
								ErrMes.Format(L"错误：%s-%s 里程 %s 处 圆曲线半径%s 不满足Rmin(%s)～Rmax(%s)",Path,DBS.Road[i].RoadName,lc,str,dvstr,dvstr1);
								ErrMesArr.Add(ErrMes);
							}

							double zj,fwj1,fwj2,pzj,hzj,allzj;
							fwj1 = pJC->XYArray[ixy][5];
							fwj2 = pJC->XYArray[ixy+1][5];
							zj = pJC->ZXJ(fwj2,fwj1) * 180 / pi;
							if(fabs(zj)<7&&ixy<pJC->XYNum-1)//圆曲线转角小于7度
							{
								fwj1 = pJC->XYArray[ixy-1][5];
								fwj2 = pJC->XYArray[ixy][5];
								pzj = pJC->ZXJ(fwj2,fwj1) * 180 / pi;

								fwj1 = pJC->XYArray[ixy+1][5];
								fwj2 = pJC->XYArray[ixy+2][5];
								hzj = pJC->ZXJ(fwj2,fwj1) * 180 / pi;
								allzj=fabs(zj);
								if(pJC->XYArray[ixy][4]*pJC->XYArray[ixy-1][4]>0.0)
									allzj+=fabs(pzj);
								if(pJC->XYArray[ixy][4]*pJC->XYArray[ixy+1][4]>0.0)
									allzj+=fabs(hzj);


								if(allzj<7)
								{									
									BAS_DRAW_FUN ob;
									double Lmin;
									ob.GetCirLenMin(DV,Lmin);
									if(L<Lmin)
									{
										CString lc;
										lc = pJC->XLC(pJC->XYArray[ixy][6]+0.5*L);
										ads_rtos(L,2,3,str);
										ads_rtos(Lmin,2,3,dvstr);
										ErrMes.Format(L"错误：%s-%s 里程 %s 处 转角小于7度,圆曲线长度%s 小于最小圆曲线长%s",Path,DBS.Road[i].RoadName,lc,str,dvstr);
										ErrMesArr.Add(ErrMes);

									}
								}

							}
						}

					}

				}
				//平面检测end

				//开始检测纵面
				OutTAB  tab;
				GTZDM_ROAD *AllZM,*ZZM,*YZM;
				AllZM=ZZM=YZM=NULL;		
				if(!pJC)
					continue;

				AllZM = tab.ReadZm(Path,DBS.Road[i].RoadName,L"全幅",pJC);

				ZZM = tab.ReadZm(Path,DBS.Road[i].RoadName,L"左幅",pJC);
				YZM = tab.ReadZm(Path,DBS.Road[i].RoadName,L"右幅",pJC);			
				CStringArray err;
				if(AllZM)//检查纵面
				{
					AllZM->checkzm(Path,DBS.Road[i].RoadName,L"全幅",err);		
					if(err.GetSize()>0)
						ErrMesArr.Append(err);						
				}
				if(ZZM)//检查纵面
				{
					ZZM->checkzm(Path,DBS.Road[i].RoadName,L"左幅",err);
					if(err.GetSize()>0)
						ErrMesArr.Append(err);					
				}
				if(YZM)//检查纵面
				{
					YZM->checkzm(Path,DBS.Road[i].RoadName,L"右幅",err);
					if(err.GetSize()>0)
						ErrMesArr.Append(err);					
				}

				//开始检测桥梁
				//隧道内的纵坡应大于0.3并小于3%，但短于100m的隧道不受此限
				//隧道洞口内侧不小于3秒设计速度行程长度与洞口外侧不小于3秒设计速度行程长度范围内的平、纵线形应一致。
				int ntun = 0;
				double sml,eml;
				int res = DBS.Read_XLDbs(Path,L"隧道",DBS.Road[i].RoadName,L"全幅");
				if(res)//读隧道
				{
					BAS_DRAW_FUN ob;
					for(j=0;j<DBS.TunNum;j++)
					{
						sml = pJC->TYLC(DBS.Tun[j].StDml);
						if(!ob.checkIfSamePm(sml,DV,pJC->XYArray,pJC->XYNum))
						{
							ErrMes.Format(L"错误：%s-%s %s-%s全幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的平面线形不一致",
								Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
							ErrMesArr.Add(ErrMes);
						}
						if(AllZM)
						{
							if(!ob.checkIfSameZm(sml,DV,AllZM->CM,AllZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-全幅纵坡 %s-%s全幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(ZZM)
						{
							if(!ob.checkIfSameZm(sml,DV,ZZM->CM,ZZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-左幅纵坡 %s-%s全幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(YZM)
						{
							if(!ob.checkIfSameZm(sml,DV,YZM->CM,YZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-右幅纵坡 %s-%s全幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}

						eml = pJC->TYLC(DBS.Tun[j].EdDml);
						if(!ob.checkIfSamePm(eml,DV,pJC->XYArray,pJC->XYNum))
						{
							ErrMes.Format(L"错误：%s-%s %s-%s全幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的平面线形不一致",
								Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
							ErrMesArr.Add(ErrMes);
						}
						if(AllZM)
						{
							if(!ob.checkIfSameZm(eml,DV,AllZM->CM,AllZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-全幅纵坡 %s-%s全幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(ZZM)
						{
							if(!ob.checkIfSameZm(eml,DV,ZZM->CM,ZZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-左幅纵坡 %s-%s全幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(YZM)
						{
							if(!ob.checkIfSameZm(eml,DV,YZM->CM,YZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-右幅纵坡 %s-%s全幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}

					}
				}

				res = DBS.Read_XLDbs(Path,L"隧道",DBS.Road[i].RoadName,L"左幅");
				if(res)//读隧道
				{
					BAS_DRAW_FUN ob;
					for(j=0;j<DBS.TunNum;j++)
					{
						sml = pJC->TYLC(DBS.Tun[j].StDml);
						if(!ob.checkIfSamePm(sml,DV,pJC->XYArray,pJC->XYNum))
						{
							ErrMes.Format(L"错误：%s-%s %s-%s左幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的平面线形不一致",
								Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
							ErrMesArr.Add(ErrMes);
						}

						if(AllZM)
						{
							if(!ob.checkIfSameZm(sml,DV,AllZM->CM,AllZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-全幅纵坡 %s-%s左幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(ZZM)
						{
							if(!ob.checkIfSameZm(sml,DV,ZZM->CM,ZZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-左幅纵坡 %s-%s左幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}

						eml = pJC->TYLC(DBS.Tun[j].EdDml);
						if(!ob.checkIfSamePm(eml,DV,pJC->XYArray,pJC->XYNum))
						{
							ErrMes.Format(L"错误：%s-%s %s-%s左幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的平面线形不一致",
								Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
							ErrMesArr.Add(ErrMes);
						}
						if(AllZM)
						{
							if(!ob.checkIfSameZm(eml,DV,AllZM->CM,AllZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-全幅纵坡 %s-%s左幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(ZZM)
						{
							if(!ob.checkIfSameZm(eml,DV,ZZM->CM,ZZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-左幅纵坡 %s-%s左幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}


					}
				}

				res = DBS.Read_XLDbs(Path,L"隧道",DBS.Road[i].RoadName,L"右幅");
				if(res)//读隧道
				{
					BAS_DRAW_FUN ob;
					for(j=0;j<DBS.TunNum;j++)
					{
						sml = pJC->TYLC(DBS.Tun[j].StDml);
						if(!ob.checkIfSamePm(sml,DV,pJC->XYArray,pJC->XYNum))
						{
							ErrMes.Format(L"错误：%s-%s %s-%s右幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的平面线形不一致",
								Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
							ErrMesArr.Add(ErrMes);
						}

						if(AllZM)
						{
							if(!ob.checkIfSameZm(sml,DV,AllZM->CM,AllZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-全幅纵坡 %s-%s右幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(YZM)
						{
							if(!ob.checkIfSameZm(sml,DV,ZZM->CM,ZZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-右幅纵坡 %s-%s右幅隧道 进口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}

						eml = pJC->TYLC(DBS.Tun[j].EdDml);
						if(!ob.checkIfSamePm(eml,DV,pJC->XYArray,pJC->XYNum))
						{
							ErrMes.Format(L"错误：%s-%s %s-%s右幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的平面线形不一致",
								Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
							ErrMesArr.Add(ErrMes);
						}

						if(AllZM)
						{
							if(!ob.checkIfSameZm(eml,DV,AllZM->CM,AllZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-全幅纵坡 %s-%s右幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}
						if(YZM)
						{
							if(!ob.checkIfSameZm(eml,DV,ZZM->CM,ZZM->NCM))
							{
								ErrMes.Format(L"错误：%s-%s-右幅纵坡 %s-%s右幅隧道 出口处,内侧3秒设计速度行程长度与洞口外侧3秒设计速度行程长度范围内的纵面线形不一致",
									Path,DBS.Road[i].RoadName,DBS.Tun[j].SKml,DBS.Tun[j].EKml);
								ErrMesArr.Add(ErrMes);
							}

						}

					}
				}
				if(AllZM) delete AllZM;
				if(ZZM) delete ZZM;
				if(YZM) delete YZM;
				if(pJC) delete pJC;

			}
		}
		DBS.Release();
	}	
	if(ErrMesArr.GetSize()>0)//有错误
	{
		FILE *fpw=NULL;
		CString ErrFname;
		ErrFname = Cworkdir + "\\data\\errmes.txt";

		fpw = _wfopen(ErrFname,L"w");
		if(fpw)
		{
			fwprintf(fpw,L"%d\n",ErrMesArr.GetSize());
			for(int i=0; i<ErrMesArr.GetSize(); i++)
				fwprintf(fpw,L"%d %s\n",i, ErrMesArr[i]);
			fclose(fpw);
			CString fname;
			fname = L"notepad.exe " + ErrFname;
			//WinExec((LPCSTR)fname,1);
			ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);
			ads_printf(L"注意：错误信息已保存在%s文件中!\n",ErrFname);
		}

	}

}
//计算鼻端桩号
void CalNose()
{	
}

void GenPers()
{
	// TODO: Implement the command
	// 关闭对象捕捉
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"OSMODE", RTSHORT, 0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"OSMODE", RTSHORT, 0, RTNONE);
#endif // VERSION_CAD2010

	CString strISMFileName;
	//	if(GetAutoCADPath(strISMFileName))
	strISMFileName = L"acismui.arx";
	acedArxLoad(strISMFileName); 

	// 读取透视图信息文件
	FILE    *pfr;
	ACHAR    fname[256];
	ACHAR    cTKFileName[256]; // 图框文件名
	ads_point pt;             // 图框起点位置
	CString strAppPath;
	int     nPerspectiveFile = 0; // 透视图文件个数
	CStringArray strPerspectiveFileNames; // 透视图文件名数组

	//strAppPath = CARXBaseDraw::GetAppPath();
	BAS_DRAW_FUN ob;
	XLDataBase DBS;
	ob.ReadWorkdir();
	strAppPath=Cworkdir;

	_stprintf(fname,L"%s\\3d\\Perspective.Info",strAppPath);

	if(pfr = _wfopen(fname,L"r"))
	{
		pt[0] = 0.0 + 190;
		pt[1] = 0.0 + 138.5;
		fwscanf(pfr,L"%s", cTKFileName);
		fwscanf(pfr,L"%d", &nPerspectiveFile);
		for(int i=0; i<nPerspectiveFile; i++)
		{
			// 读透视图文件名
			ACHAR cPerspectiveFileName[256];
			fwscanf(pfr,L"%s", cPerspectiveFileName);

			pt[0] = pt[0] + 190 ;


			// 插入图框
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-insert", RTSTR, cTKFileName, RTPOINT, pt,
				RTSTR, L"", RTSTR, L"", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-insert", RTSTR, cTKFileName, RTPOINT, pt,
				RTSTR, L"", RTSTR, L"", RTSTR, L"", RTNONE);
			ads_command(RTSTR, L"OSMODE", RTSHORT, 0, RTNONE);
#endif // VERSION_CAD2010



			// 插入透视图
			Acad::ErrorStatus ErrorStatus;
			AcDbObjectId ImageEntityId;	
			CString strImageName, strImagePath;

			strImageName.Format(L"Image%d",i);
			ErrorStatus = CARXBaseDraw::DrawImage(ImageEntityId, strImageName, cPerspectiveFileName);
			if(ErrorStatus != Acad::eOk)
			{
				ads_printf(L"Error: DrawImage failed.\n"); 
				return;
			}

			// 修改图像
			AcGePoint3d origin(pt[0]-190, pt[1]-138.5, 0.0);
			ErrorStatus = CARXBaseDraw::ModifyImage(ImageEntityId, origin, 380, 263);
			if (ErrorStatus != Acad::eOk)
			{
				ads_printf(L"Error: Modify image failed.\n"); 
			}

			pt[0] += 500.0;
		}
	}
	else
	{
		AfxMessageBox(L"打开透视图信息文件Perspective.Info失败!");
		return;
	}

	// 查看全部
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"ZOOM", RTSTR, L"ALL", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"ZOOM", RTSTR, L"ALL", RTNONE);
#endif // VERSION_CAD2010

}

bool LoadArx(ACHAR ArxFname[256],ACHAR cmd[80])
{
	ACHAR cmdstr[80];

	acedGetAcadDwgView()->SetFocus();
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\%s",SysPath,ArxFname);
	acedArxLoad(fname);
	acDocManager->activateDocument(acDocManager->curDocument());
	_stprintf(cmdstr ,L"(command \"%s\")\n",cmd);
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0,false,false);

	return 1;
}

//编辑所有改移道路数据	编辑道路汇总表
void EditAllRoadS()
{
	CEditAllRoads EditAllRoads; 
	EditAllRoads.m_grid.TableName="线路相对关系表";
	EditAllRoads.DoModal(); 
}


void OutQXJKCGTab()
{
	ACHAR DWGPath[256];
	double TextH = 0.5 ;
	AcGePoint3d InsertPt;
	ads_point InsPt;
	AcGePoint3d NotePt;
	int PageSum;
	int PageIndex;
	ads_point PT1;
	acedGetPoint(NULL,_T("\n请拾取基点"),PT1);
	InsertPt.x = PT1[0];
	InsertPt.y = PT1[1];
	InsertPt.z = PT1[2];
	_stprintf(DWGPath,L"%s\\dwg\\曲线超过加宽表.dwg",SysPath);

	double RowHeigh = 6.7382;
	double ColWide[11];
	ColWide[0] = 12.634;
	ColWide[1] = 12.634;
	ColWide[2] = 12.634;
	ColWide[3] = 16.845;
	ColWide[4] = 16.845;
	ColWide[5] = 16.845;
	ColWide[6] = 16.845;
	ColWide[7] = 10.528;
	ColWide[8] = 10.528;
	ColWide[9] = 10.528;
	ColWide[10] = 10.528;

	XLDataBase DBS;
	DBS.ReadWorkdir();
	CString m_mdbname = Cworkdir+"\\DATA\\";
	m_mdbname = m_mdbname + Pro+".mdb";	
	DBS.Read_XLDbs(m_mdbname,L"控制点表");	
	//	mdb.Read_XLDbs(PrjMdb,L"线元表",RoadName);		
	DBS.Read_XLDbs(m_mdbname,L"断链表");
	JD_CENTER* pm1;
	if(DBS.NJdCen>0)
	{
		pm1=(JD_CENTER *)new JD_CENTER();//主线对象
		pm1->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		pm1->JD0 = DBS.JdCenArray[0].A2;
		if(DBS.NDL>0)
			pm1->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm1->mdbname,m_mdbname);
		_tcscpy(pm1->RoadName,L"主线");
		pm1->CreateZQZJB();
	}
	if (pm1)
	{
		if (pm1->JdCenNum>2)
		{
			PageSum = (int)(pm1->JdCenNum/10.0) + 1;
			int iRoad = 0;
			for (int PageIndex=0; PageIndex<PageSum; PageIndex++)
			{

				InsPt[X] = InsertPt.x;
				InsPt[Y] = InsertPt.y;
				InsPt[Z] = 0.0;

#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, 1.0, RTPOINT, InsPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, 1.0, RTPOINT, InsPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010

				NotePt.x = InsertPt.x + 130;
				NotePt.y = InsertPt.y +92;
				ACHAR PageInf[20];
				_stprintf(PageInf,L"第 %d 页  共 %d 页", PageIndex+1, PageSum);
				DBS.maketext(NotePt, PageInf, 0.0, 2.5);

				for (int iRow=0; iRow<10; iRow++)
				{
					if (iRow+PageIndex*10>pm1->JdCenNum-3)
					{
						continue;
					}
					TextH = 2.5;
					NotePt.x = InsertPt.x + 6.32;
					NotePt.y = InsertPt.y + 64 - iRow * RowHeigh;

					ACHAR TextArray[19][40];
					ACHAR Ckml[20], GH[8];
					double dml;

					for (int iCol=0; iCol<11; iCol++)
						_tcscpy(TextArray[iCol],L"\0");
					_stprintf(TextArray[0],L"JD%d", iRow+PageIndex*10+2);
					_stprintf(TextArray[1],L"%.2lf", pm1->JdCenArray[iRow+PageIndex*10+1].R);
					double TempProjml ,ZHProjml,HZProjml,CG;
					TempProjml=pm1->PROJ_ML(pm1->JdCenArray[iRow+PageIndex*10+1].N,pm1->JdCenArray[iRow+PageIndex*10+1].E);
					pm1->Cal_CurJD_QXSE(iRow+PageIndex*10+1,ZHProjml,HZProjml);
					_stprintf(TextArray[2],L"%.2lf", ZHProjml-HZProjml);
					_tcscpy(TextArray[3],pm1->GetXCLCStr(ZHProjml));
					_tcscpy(TextArray[4],pm1->GetXCLCStr(HZProjml));
					pm1->ReadCG();
					pm1->CalCG((ZHProjml+HZProjml)/2,CG);
					_stprintf(TextArray[5],L"%.0lf", CG);
					_stprintf(TextArray[6],L"%.2lf", pm1->JdCenArray[iRow+PageIndex*10+1].Ls1);
					pm1->ReadLMK("左路面宽分段表");
					pm1->ReadLMK("右路面宽分段表");
					pm1->ReadJK();
					double Yalf_fgdk,Ylydk,Ycxdk,Yyljk,Ytljk,Ytljhp;
					double Lalf_fgdk,Llydk,Lcxdk,Lyljk,Ltljk,Ltljhp;
					int Ybkmode,Ysjbgmode,Lbkmode,Lsjbgmode;
					pm1->CalLMK_JK((ZHProjml+HZProjml)/2,pm1->NZLMK,pm1->ZLMK,pm1->ZjkNm,pm1->Zjk,
						Lalf_fgdk,Llydk,Lcxdk,Lyljk,Ltljk,Ltljhp,Lbkmode,Lsjbgmode);
					pm1->CalLMK_JK((ZHProjml+HZProjml)/2,pm1->NYLMK,pm1->YLMK,pm1->YjkNm,pm1->Yjk,
						Yalf_fgdk,Ylydk,Ycxdk,Yyljk,Ytljk,Ytljhp,Ybkmode,Ysjbgmode);
					_stprintf(TextArray[7],L"%.2lf",Lalf_fgdk+Llydk+Lcxdk+Lyljk+Ltljk);
					_stprintf(TextArray[8],L"%.2lf",Yalf_fgdk+Ylydk+Ycxdk+Yyljk+Ytljk);
					_stprintf(TextArray[9],L"%.2lf",Lalf_fgdk+Llydk+Lcxdk+Lyljk);
					_stprintf(TextArray[10],L"%.2lf",Yalf_fgdk+Ylydk+Ycxdk+Yyljk);
					DBS.maketext(NotePt, TextArray[0], 0.0, TextH);
					for (int iCol=1; iCol<11; iCol++)
					{
						NotePt.x += 0.5 * (ColWide[iCol-1] + ColWide[iCol]);
						if (_tcslen(TextArray[iCol]) > 0)
							DBS.maketext(NotePt, TextArray[iCol], 0.0, TextH);
					}
					//////////////////////////////////////////////////////////////////////////
				}
				InsertPt.x += 485.0;
			}
		}
	}
	else
	{
		AfxMessageBox(L"该线路不存在！");
		return;
	}
}
//输出所有改移道路表格	输出道路汇总表
void OutAllRoadSTab()
{
	COutpRoadCollTabDlg OutDlg;
	OutDlg.DoModal();
	return;

	//20191104	修改为对话框模式，增加输出到excel的功能，下面绘制代码留作参考，目前不使用

	ACHAR DWGPath[256];
	double TextH = 3.5 ;
	AcGePoint3d InsertPt;
	ads_point InsPt;
	AcGePoint3d NotePt;
	int PageSum;
	int PageIndex;
	ads_point PT1;
	acedGetPoint(NULL,_T("\n请拾取基点"),PT1);
	InsertPt.x = PT1[0];
	InsertPt.y = PT1[1];
	InsertPt.z = PT1[2];
	_stprintf(DWGPath,L"%s\\dwg\\改移道路汇总表.dwg",SysPath);

	double RowHeigh = 29.0;
	double ColWide[19];
	ColWide[0] = 14.0;
	ColWide[1] = 25.5;
	ColWide[2] = 14.0;
	ColWide[3] = 13.0;
	ColWide[4] = 14.5;
	ColWide[5] = 11.0;
	ColWide[6] = 47.0;
	ColWide[7] = 12.5;
	ColWide[8] = 12.0;
	ColWide[9] = 12.0;
	ColWide[10] = 20.0;
	ColWide[11] = 20.0;
	ColWide[12] = 20.0;
	ColWide[13] = 15.0;
	ColWide[14] = 15.0;
	ColWide[15] = 15.0;
	ColWide[16] = 15.0;
	ColWide[17] = 57.0;
	ColWide[18] = 32.5;

	XLDataBase DBS;
	DBS.ReadWorkdir();
	CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
	if (DBS.Read_XLDbs(MdfName,L"改移道路表"))
	{
		if (DBS.ModifyRoadSum>0)
		{
			PageSum = (int)(DBS.ModifyRoadSum / 8.0) + 1;
			int iRoad = 0;
			for (int PageIndex=0; PageIndex<PageSum; PageIndex++)
			{

				InsPt[X] = InsertPt.x;
				InsPt[Y] = InsertPt.y;
				InsPt[Z] = 0.0;

#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, 1.0, RTPOINT, InsPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, 1.0, RTPOINT, InsPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010

				NotePt.x = InsertPt.x + 402.0;
				NotePt.y = InsertPt.y + 282.0;
				ACHAR PageInf[20];
				_stprintf(PageInf,L"第 %d 页  共 %d 页", PageIndex+1, PageSum);
				DBS.maketext(NotePt, PageInf, 0.0, 4.0);

				for (int iRow=0; iRow<8; iRow++)
				{
					TextH = 3.5;

					NotePt.x = InsertPt.x + 57.0;
					NotePt.y = InsertPt.y + 234.5 - iRow * RowHeigh;

					ACHAR TextArray[19][40];
					ACHAR Ckml[20], GH[8];
					double dml;

					for (int iCol=0; iCol<19; iCol++)
						_tcscpy(TextArray[iCol],L"\0");

					if (!DBS.pModifyRoad[iRoad].SCkml.IsEmpty())
					{
						_tcscpy(Ckml, DBS.pModifyRoad[iRoad].SCkml);
						DBS.split_ckml(Ckml, &dml, GH);
						_tcscpy(TextArray[0], DBS.LCchr(GH, dml, 1, 1));
					}

					if (!DBS.pModifyRoad[iRoad].RailCkml.IsEmpty())
					{
						_tcscpy(Ckml, DBS.pModifyRoad[iRoad].RailCkml);
						DBS.split_ckml(Ckml, &dml, GH);
						_tcscpy(TextArray[1], DBS.LCchr(GH, dml, 1, 1));
					}

					if (!DBS.pModifyRoad[iRoad].ECkml.IsEmpty())
					{
						_tcscpy(Ckml, DBS.pModifyRoad[iRoad].ECkml);
						DBS.split_ckml(Ckml, &dml, GH);
						_tcscpy(TextArray[2], DBS.LCchr(GH, dml, 1, 1));
					}

					if (DBS.pModifyRoad[iRoad].RoadLen>1.0)
						_stprintf(TextArray[3],L"%.0lf", DBS.pModifyRoad[iRoad].RoadLen);

					if (DBS.pModifyRoad[iRoad].InterAng>1.0)
						_stprintf(TextArray[4],L"%.4lf", DBS.pModifyRoad[iRoad].InterAng);

					if (!DBS.pModifyRoad[iRoad].UpDown.IsEmpty())
						_tcscpy(TextArray[5], DBS.pModifyRoad[iRoad].UpDown);

					if (!DBS.pModifyRoad[iRoad].KKSY.IsEmpty())
						_tcscpy(TextArray[6], DBS.pModifyRoad[iRoad].KKSY);

					if (DBS.pModifyRoad[iRoad].LjWide>1.0)
						ads_rtos(DBS.pModifyRoad[iRoad].LjWide, 2, 1, TextArray[8]);

					if (DBS.pModifyRoad[iRoad].LmWide>1.0)
						ads_rtos(DBS.pModifyRoad[iRoad].LmWide, 2, 1, TextArray[9]);

					if (DBS.pModifyRoad[iRoad].LmThick>1.0)
						_stprintf(TextArray[10],L"%.0lf", DBS.pModifyRoad[iRoad].LmThick);

					if (!DBS.pModifyRoad[iRoad].LmType.IsEmpty())
						_tcscpy(TextArray[11], DBS.pModifyRoad[iRoad].LmType);

					if (DBS.pModifyRoad[iRoad].LmArea>1.0)
						_stprintf(TextArray[12],L"%.0lf", DBS.pModifyRoad[iRoad].LmArea);

					if (DBS.pModifyRoad[iRoad].FillVol>1.0)
						_stprintf(TextArray[13],L"%.0lf", DBS.pModifyRoad[iRoad].FillVol);

					if (DBS.pModifyRoad[iRoad].CutVol>1.0)
						_stprintf(TextArray[14],L"%.0lf", DBS.pModifyRoad[iRoad].CutVol);

					if (DBS.pModifyRoad[iRoad].MasonryVol>1.0)
						_stprintf(TextArray[15],L"%.0lf", DBS.pModifyRoad[iRoad].MasonryVol);

					if (!DBS.pModifyRoad[iRoad].Note.IsEmpty())
						_tcscpy(TextArray[18], DBS.pModifyRoad[iRoad].Note);


					if (_tcslen(TextArray[0])>0)
						DBS.maketext(NotePt, TextArray[0], 0.0, TextH);

					for (int iCol=1; iCol<19; iCol++)
					{
						NotePt.x += 0.5 * (ColWide[iCol-1] + ColWide[iCol]);
						if (_tcslen(TextArray[iCol]) > 0)
							DBS.maketext(NotePt, TextArray[iCol], 0.0, TextH);
					}

					//多行文字写公路等级
					if (!DBS.pModifyRoad[iRoad].RandType.IsEmpty())
					{
						_tcscpy(TextArray[7], DBS.pModifyRoad[iRoad].RandType);
						NotePt.x = InsertPt.x + 195.25;
						DBS.MakeMText(NotePt, TextArray[7], 0.0, TextH, TextH);
					}

					//多行文字写用地
					if (!DBS.pModifyRoad[iRoad].LandAreaStr.IsEmpty())
					{
						int SLandTypeIndex = -1;
						int ELandTypeIndex = -1;
						int LandNumIndex = 0;
						CString TempStr = DBS.pModifyRoad[iRoad].LandAreaStr;
						CStringArray NoteTextArray;
						SLandTypeIndex = TempStr.FindOneOf(L"(");
						ELandTypeIndex = TempStr.FindOneOf(L")");
						while (SLandTypeIndex>-1 && ELandTypeIndex>SLandTypeIndex)
						{
							NoteTextArray.Add( TempStr.Mid(LandNumIndex, SLandTypeIndex-LandNumIndex) );
							NoteTextArray.Add( TempStr.Mid(SLandTypeIndex+1, ELandTypeIndex-SLandTypeIndex-1) );
							TempStr = TempStr.Mid(ELandTypeIndex+1, TempStr.GetLength()-ELandTypeIndex);
							SLandTypeIndex = TempStr.FindOneOf(L"(");
							ELandTypeIndex = TempStr.FindOneOf(L")");
						}
						if (NoteTextArray.GetSize()==0)
						{
							ads_printf(L"%s\n",L"第%张表第%d行中的用地数据格式有误！", PageIndex+1, iRow+1);
							NoteTextArray.Add(DBS.pModifyRoad[iRoad].LandAreaStr);
						}
						NotePt.x = InsertPt.x + 338.0;
						int TypeSum = (int)((NoteTextArray.GetSize()+0.5) / 2.0);
						NotePt.y += (1.2*(TypeSum-1)*TextH + 0.5*1.2*TextH);
						for (int i=0; i<TypeSum; i++)
						{
							_tcscpy(TextArray[16], NoteTextArray[2*i]);
							DBS.maketext(NotePt, TextArray[16], 0.0, TextH);
							NotePt.y -= 1.2*TextH;
							_tcscpy(TextArray[16], NoteTextArray[2*i+1]);
							DBS.maketext(NotePt, TextArray[16], 0.0, TextH);
							NotePt.y -= 1.2*TextH;
						}
					}

					//绘制纵断面示意图
					//////////////////////////////////////////////////////////////////////////
					CString RoadName = DBS.pModifyRoad[iRoad].RoadName;
					CString MdbName = Cworkdir + "\\DATA\\" + RoadName + ".mdb";
					double PD_array[MAXBPDNUM][4];
					int res = DBS.Read_XLDbs(MdbName,L"坡度表",L"主线",L"全幅");
					int res1 = DBS.Read_XLDbs(MdbName,L"纵断链表",L"主线");
					if(res&&res1)//读坡度
					{
						int NBPD = DBS.NPD;
						double MaxLever = -1e9;
						double MinLever = 1e9;
						for(int i=0;i<NBPD;i++)
						{
							PD_array[i][0] = DBS.TYLC1(DBS.PDB[i].ml,DBS.NZDL,DBS.ZDLArray);
							PD_array[i][1] = DBS.PDB[i].Level;
							PD_array[i][2] = DBS.PDB[i].Rshu;
							PD_array[i][3] = 0.0;
							if (MaxLever < PD_array[i][1])
								MaxLever = PD_array[i][1];
							if (MinLever > PD_array[i][1])
								MinLever = PD_array[i][1];
						} 

						if(DBS.PDB) 
						{
							if(DBS.PDB) delete[]DBS.PDB;
							DBS.PDB = NULL;
						}

						TextH = 3.0;
						double XScale,YScale;
						double MlNoteLen = 15.0;
						AcGePoint3d ZDMCenPt,SPt,EPt;
						ZDMCenPt.x = InsertPt.x + 345.5;
						ZDMCenPt.y = InsertPt.y + 220.0 - iRow * RowHeigh;

						XScale = (ColWide[17] - 8.0) / (PD_array[NBPD-1][0] - PD_array[0][0]);
						YScale = (RowHeigh - 8.0 - MlNoteLen - TextH) / (MaxLever - MinLever);

						SPt.x = ZDMCenPt.x + 4.0;
						SPt.y = ZDMCenPt.y + 4.0 + (PD_array[0][1] - MinLever) * YScale;
						NotePt.x = SPt.x;
						NotePt.y = SPt.y + MlNoteLen;
						DBS.makeline(SPt, NotePt, 0, 0.0);//绘起点里程标注线
						dml = DBS.XLC1(PD_array[0][0], GH, DBS.NZDL, DBS.ZDLArray);
						_tcscpy(Ckml, DBS.LCchr(GH, dml, 3, 1));
						NotePt.y = SPt.y + TextH;
						NotePt.x += 1.2 * TextH;
						DBS.maketext(NotePt, Ckml, 0.5*pi, TextH, 0, 1);//标注起点里程

						double RoadCml = DBS.TYLC1(DBS.pModifyRoad[iRoad].RoadDml, DBS.NZDL, DBS.ZDLArray);

						for (int i=1; i<NBPD; i++)
						{
							EPt.x = ZDMCenPt.x + 4.0 + (PD_array[i][0] - PD_array[0][0]) * XScale;
							EPt.y = ZDMCenPt.y + 4.0 + (PD_array[i][1] - MinLever) * YScale;
							DBS.makeline(SPt, EPt, 0, 0.35);//绘坡度线
							NotePt.x = EPt.x;
							NotePt.y = EPt.y + MlNoteLen;
							DBS.makeline(EPt, NotePt, 0, 0.0);//绘里程标注线
							dml = DBS.XLC1(PD_array[i][0], GH, DBS.NZDL, DBS.ZDLArray);
							_tcscpy(Ckml, DBS.LCchr(GH, dml, 3, 1));
							NotePt.y = EPt.y + TextH;
							NotePt.x -= 0.2 * TextH;
							DBS.maketext(NotePt, Ckml, 0.5*pi, TextH, 0, 1);//标注里程

							NotePt.x = 0.5 * (SPt.x + EPt.x);
							NotePt.y = 0.5 * (SPt.y + EPt.y);
							ads_point Pt1,Pt2;
							Pt1[X] = SPt.x;
							Pt1[Y] = SPt.y;
							Pt1[Z] = 0.0;
							Pt2[X] = EPt.x;
							Pt2[Y] = EPt.y;
							Pt2[Z] = 0.0;
							double Ang = ads_angle(Pt1, Pt2);
							double PDLen = PD_array[i][0]-PD_array[i-1][0];
							double Degree = (PD_array[i][1]-PD_array[i-1][1]) / (PD_array[i][0]-PD_array[i-1][0]) * 100.0;
							ads_rtos(PDLen, 2, 3, Ckml);
							NotePt.y += 0.6 * TextH;
							DBS.maketext(NotePt, Ckml, Ang, TextH);//标注坡长
							ads_rtos(Degree, 2, 1, Ckml);
							_tcscat(Ckml,L"%");
							NotePt.y -= 1.6 * TextH;
							DBS.maketext(NotePt, Ckml, Ang, TextH);//标注坡度

							if (RoadCml>PD_array[i-1][0] && RoadCml<PD_array[i][0])
							{
								AcGePoint3d RailSPt,RailEPt;
								RailSPt.x = SPt.x + (RoadCml - PD_array[i-1][0]) * XScale;
								RailSPt.y = SPt.y + (EPt.y-SPt.y)/(EPt.x-SPt.x) * (RoadCml - PD_array[i-1][0]) * XScale;
								RailEPt.x = RailSPt.x;
								RailEPt.y = RailSPt.y + MlNoteLen;
								NotePt.x = RailSPt.x - 0.6 * TextH;
								NotePt.y = RailEPt.y;
								DBS.makeline(RailSPt, RailEPt, 0, 0.0);
								DBS.maketext(NotePt,L"铁路中心", 0.5*pi, TextH);//标注铁路中心
							}

							SPt = EPt;
						}


					}
					//////////////////////////////////////////////////////////////////////////


					iRoad++;
					if (iRoad == DBS.ModifyRoadSum)
						return;
				}
				InsertPt.x += 485.0;
			}
		}
	}
	else
	{
		AfxMessageBox(L"改移道路汇总表中无数据!");
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
//切图
#define TRIMVERSION 20040220
int TKNUM;
//#include "dbmain.h"
BAS_DRAW_FUN ob;
int flag_k;
ACHAR NumTK[20];//"共%d幅图"
ACHAR iTK[20];//"当前处理第%d幅"
ACHAR iCounter[20];//"重复切割%d次"
int AutoEntBK(ads_point p0, ads_point p1,ads_point p2, ads_point p3,int ITK);
int AutoEntBK1(ads_point p0, ads_point p1);
void CalFrame(double dist,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,ads_point &p1,ads_point &p2,ads_point &p3,ads_point &p4);
void TrimCommand(ads_point p0,ads_point p1,ads_point p2,ads_point p3,//外边界//用Trim命令切割
				 ads_point p4,ads_point p5,ads_point p6,ads_point p7,//内边界
				 ads_point p8,ads_point p9,ads_point p10,ads_point p11);//切割线
bool FSelectEntitys(ads_point p1, ads_point p2,ads_point p3, ads_point p4);
void MoveAndRoate(ads_point FromPt,ads_point ToPt,double jiao,ads_point p1, ads_point p2,ads_point p3, ads_point p4);

void REGENENTITY(AcDbEntity* pEnt,AcDbObjectId eId)//强制重生成实体
{
	/*
	acdbOpenObject(pEnt,eId,AcDb::kForWrite);
	pEnt->recordGraphicsModified();
	pEnt->close();
	*/
	pEnt->draw();
}
//
//void CalFrame(ads_point &p1,ads_point &p2,ads_point &p3,ads_point &p4);
void AutoEntTransform(AcGeMatrix3d xformr,AcGeMatrix3d xformt,ads_point p1, ads_point p2,ads_point p3, ads_point p4);

// This is command 'PMADDTK'
void ADDPMTK()
{
	//	acdbAcisRestoreEntityFromDwgFiler();
	// TODO: Implement the command
	double high=297;//图框高
	double dH=10;//内外框间距
	double ddtk=40;//外图框间距
	double TUSCALE;
	TRIMDIALOG pTrimDialog;
	pTrimDialog.DoModal();
	TUSCALE=pTrimDialog.m_Scale;
	if(!pTrimDialog.IsOk)return;
	high=pTrimDialog.m_H;
	dH=pTrimDialog.m_dH;
	ddtk=pTrimDialog.m_dTK;
	acutPrintf(L"high=%lf,dH=%lf\n",high,dH);
	TRIM_ROAD *pTrim=NULL;

	AcDbObjectId eId;
	pTrim=new TRIM_ROAD(high,dH,ddtk,pTrimDialog.m_dSpace,TUSCALE);
	eId=ob.AddEntityToDbs(pTrim);
	int rc=RTNORM;
	int counter=0;
	int type;
	struct resbuf result;
	ads_point pk;
	acedGetPoint(NULL,L"\n点取第一个点或 [Undo]:",pk);//plhadd
	AcGePoint3d PT(pk[X],pk[Y],pk[Z]);
	PT.z=0;
	AcDbObject* pObj = NULL;
	/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pTrim = TRIM_ROAD::cast(pObj);
	pTrim->AddFrame(PT);
	pTrim->close();
	int flag=0;
	AcGePoint3d PT0;
	do
	{
		counter++;
		double jiao=ob.angleX(PT0,PT);
		PT.x=PT.x+0.1*cos(jiao);PT.y=PT.y+0.1*sin(jiao);
		PT0=PT;
		/*	acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pTrim = TRIM_ROAD::cast(pObj);
		pTrim->AddFrame(PT);
		pTrim->close();		
		for(;;)
		{			
			if(acedGrRead(1 , &type , &result)!=RTNORM)
			{ 
				//				acutPrintf(L"!=rtnorm\n");
				/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pTrim = TRIM_ROAD::cast(pObj);
				pTrim->DelEndFrame();
				pTrim->close();				
				return;
			}
			if(type==2)
			{
				short KEYCODE=result.resval.rint;
				//	acutPrintf(L"%d",KEYCODE);
				if(KEYCODE==117||KEYCODE==85)
				{
					/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pTrim = TRIM_ROAD::cast(pObj);
					pTrim->DelEndFrame();
					pTrim->close();				
				}
				else if(KEYCODE==13||KEYCODE==32)
				{
					/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pTrim = TRIM_ROAD::cast(pObj);
					pTrim->DelEndFrame();
					pTrim->close();				
					return;
				}
				else {}
			}
			else if(type == 3)break;
			else if((type!=5) && (type!=12))
			{ 
				//ads_printf(L"\n必须用鼠标! ");
				/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pTrim = TRIM_ROAD::cast(pObj);
				pTrim->DelEndFrame();
				pTrim->close();

				return;
			}
			else if(type != 5)break;
			else if(type == 5)
			{
				PT.x=result.resval.rpoint[X];
				PT.y=result.resval.rpoint[Y];
				PT.z=0;
				double jiao2=ob.angleX(PT0,PT)+Pai;
				if(jiao2>2*Pai)jiao2-=2*Pai;//保证两相信框不重合
				if(PT0.distanceTo(PT)>0.1&&jiao2!=jiao)
				{
					/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pTrim = TRIM_ROAD::cast(pObj);
					pTrim->SetEndPoint(PT);
					pTrim->close();				
				}
			}
			//			else break;
		}//for(;;)
		acutPrintf(L"\n点取下一点或 [Undo]:");//plhadd
	}while(counter<200);
	/*acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,eId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pTrim = TRIM_ROAD::cast(pObj);
	pTrim->DelEndFrame();
	pTrim->close();
	//	delete pTrim;	
	acutPrintf(L"\n操作完成!\n");
}
//AcDbSpatialFilter
void MoveAndRoate(ads_point FromPt,ads_point ToPt,double jiao,ads_point p1, ads_point p2,ads_point p3, ads_point p4)
{
	ads_name ssname;
	struct resbuf *pointlist;
	long n;
	AcDbObjectId eId;
	pointlist = ads_buildlist(RTPOINT,p1,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,0);
	acedSSGet(L"WP",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		//ads_printf(L"Number of entity is %ld\n",n);
		if(n > 0)
		{
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"MOVE", RTPICKS, ssname, RTPOINT, FromPt, RTPOINT, ToPt, RTNONE);
			acedCommandS(RTSTR, L"ROTATE", RTPICKS, ssname, RTPOINT, ToPt, RTANG, jiao, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"MOVE", RTPICKS, ssname, RTPOINT, FromPt, RTPOINT, ToPt, RTNONE);
			ads_command(RTSTR, L"ROTATE", RTPICKS, ssname, RTPOINT, ToPt, RTANG, jiao, RTNONE);
#endif // VERSION_CAD2010
			
		}
		ads_relrb(pointlist);
		ads_ssfree(ssname); 
		//	acedRestoreStatusBar();
	}
}

int AutoEntBK(ads_point p0, ads_point p1,ads_point p2, ads_point p3,int ITK=0)
{
	acedSetStatusBarProgressMeterPos(0);		
	BAS_DRAW_FUN::AutoEntBK1(p0,p1);
	acedSetStatusBarProgressMeterPos(36);
	BAS_DRAW_FUN::AutoEntBK1(p1,p2);
	acedSetStatusBarProgressMeterPos(50);
	BAS_DRAW_FUN::AutoEntBK1(p2,p3);
	acedSetStatusBarProgressMeterPos(86);
	if(ITK==0)BAS_DRAW_FUN::AutoEntBK1(p3,p0);
	acedSetStatusBarProgressMeterPos(100);
	return 0;
}

int AutoEntBK1(ads_point p0, ads_point p1)
{
	AcGePoint3dArray points;
	AcDbVoidPtrArray curveSegments;

	ads_point breakPt;
	int flag=1;//1:OK  -1:not finished,有线没剪断
	ads_name ssname,ename,last;
	struct resbuf *pointlist;
	long n;
	AcGePoint3d Pt0,Pt1;
	AcGePoint3d tempP0,tempP1;
	Pt0.set(p0[X],p0[Y],0);
	Pt1.set(p1[X],p1[Y],0);
	tempP0.set(p0[X],p0[Y],0);
	tempP1.set(p1[X],p1[Y],0);
	AcDbLine *pLine=new AcDbLine(Pt0,Pt1);
	AcDb2dPolyline* Polyline2d = NULL;
	AcDb3dPolyline* Polyline3d = NULL;
	//	AcDbPolyline* Poly;
	AcDb::Poly2dType Poly2dTyp;
	AcDb::Poly3dType Poly3dTyp;

	//	AcDbObjectId Lid=ob.AddEntityToDbs(pLine);
	//AcDbLine pLine1(Pt0,Pt1);

	AcDbObjectId eId;
	AcDbEntity *pEnt;
	AcDbCurve *pCurve;
	Acad::ErrorStatus es;
	AcGePlane plane(AcGePoint3d(0,0,0),AcGeVector3d(0,0,1));

	pointlist = ads_buildlist(RTPOINT,p0,RTPOINT,p1,0);
	//  ads_grdraw(p0,p1,-1,0);
	acedSSGet(L"F",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		//ads_printf(L"Number of entity is %ld\n",n);
		ACHAR pszLabel[80];
		//_stprintf(pszLabel,L"切割中:");
		_tcscpy(pszLabel,NumTK);
		_tcscat(pszLabel,iTK);
		int Max=n;
		double Cur=0;
		int Pos=0;
		//	if(flag_k<2)acedSetStatusBarProgressMeter(pszLabel,0,1000);
		es=Acad::eOk;
		while(n > 0)
		{
			n = n - 1;			
			ads_ssname(ssname,n,ename);
			acdbGetObjectId(eId,ename); 
			if(Acad::eOk!=acdbOpenObject(pEnt,eId,AcDb::kForRead)){/*acutPrintf(L"eId=%ld,n=%d  ",eId,n);*/acutPrintf(L"open error\n");continue;}
			pEnt->close();
			if(pEnt->isKindOf(TRIM_ROAD::desc())
				||pEnt->isKindOf(AcDbGroup::desc())
				||pEnt->isKindOf(AcDbBlockReference::desc())){/*acutPrintf(L"-1")*//*acutPrintf(L"block\n");*/continue;}
			if(points.length()>0)points.removeSubArray(0,points.length()-1);		   	   
			es=pEnt->intersectWith(pLine,AcDb::kOnBothOperands,plane,points);
			if(pEnt->isKindOf(AcDbCurve::desc())&&Acad::eOk==es&&points.length()>0)
			{
				es=acdbOpenObject(pCurve,eId,AcDb::kForWrite);
				pEnt->cast(pCurve);
				pEnt->close();//

				if(pCurve->isKindOf(AcDb3dPolyline::desc()))
				{
					//acdbOpenObject(Polyline3d,eId,AcDb::kForRead);
					if(acdbOpenObject(Polyline3d,eId, AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return 0;
					}
					Poly3dTyp=Polyline3d->polyType();
					Polyline3d->close();

					if(AcDb::k3dSimplePoly!=Poly3dTyp)
					{
						Polyline3d->close();										
						pLine->intersectWith(pEnt,AcDb::kOnBothOperands,plane,points);
						//	acutPrintf(L"length=%d\n",points.length());

						if(points.length()>0)
						{
							breakPt[X]=points[0].x;
							breakPt[Y]=points[0].y;	
#ifdef VERSION_CAD2016
							acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, breakPt, RTPOINT, breakPt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
							ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, breakPt, RTPOINT, breakPt, RTNONE);
#endif // VERSION_CAD2010
							
							for(int pp=1;pp<points.length();pp++)
							{
								breakPt[X]=points[pp].x;
								breakPt[Y]=points[pp].y;
								ads_entlast(last);	
#ifdef VERSION_CAD2016
								acedCommandS(RTSTR, L"break", RTENAME, last, RTPOINT, breakPt, RTPOINT, breakPt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
								ads_command(RTSTR, L"break", RTENAME, last, RTPOINT, breakPt, RTPOINT, breakPt, 0);
#endif // VERSION_CAD2010
												

							}
						}			
					}
				}
				else if(pCurve->isKindOf(AcDb2dPolyline::desc()))
				{
					//acdbOpenObject(Polyline2d,eId,AcDb::kForRead);
					if(acdbOpenObject(Polyline2d,eId, AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return 0;
					}
					Poly2dTyp=Polyline2d->polyType();	
					Polyline2d->close();

					if(AcDb::k2dSimplePoly==Poly2dTyp)
					{
						ads_point p1,  p2, p3,  p4;
						//						pLine->intersectWith(pEnt,AcDb::kOnBothOperands,plane,points);
						//						acutPrintf(L"length=%d\n",points.length());

						//						if(points.length()>0)
						{
							breakPt[X]=points[0].x;
							breakPt[Y]=points[0].y;	
							p1[X]=points[0].x-1;
							p1[Y]=points[0].y+1;
							p2[X]=points[0].x-1;
							p2[Y]=points[0].y-1;
							p3[X]=points[0].x+1;
							p3[Y]=points[0].y-1;
							p4[X]=points[0].x+1;
							p4[Y]=points[0].y+1;
#ifdef VERSION_CAD2016
							acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, breakPt, RTPOINT, breakPt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
							ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, breakPt, RTPOINT, breakPt, RTNONE);
#endif // VERSION_CAD2010
							LanXun(p1,p2,p3,p4);

							for(int pp=1;pp<points.length();pp++)
							{
								breakPt[X]=points[pp].x;
								breakPt[Y]=points[pp].y;
								p1[X]=points[pp].x-1;
								p1[Y]=points[pp].y+1;
								p2[X]=points[pp].x-1;
								p2[Y]=points[pp].y-1;
								p3[X]=points[pp].x+1;
								p3[Y]=points[pp].y-1;
								p4[X]=points[pp].x+1;
								p4[Y]=points[pp].y+1;
								ads_entlast(last);		
#ifdef VERSION_CAD2016
								acedCommandS(RTSTR, L"break", RTENAME, last, RTPOINT, breakPt, RTPOINT, breakPt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
								ads_command(RTSTR, L"break", RTENAME, last, RTPOINT, breakPt, RTPOINT, breakPt, 0);
#endif // VERSION_CAD2010
								
								LanXun(p1,p2,p3,p4);


							}
						}			
					}
					//Polyline2d->setLinetypeGenerationOn();
				}
				/*
				else if(pCurve->isKindOf(AcDbPolyline::desc()))
				{	
				acutPrintf(L"1");
				/ *

				acdbOpenObject(pCurve,eId,AcDb::kForWrite);
				((AcDbPolyline*)pCurve)->convertTo(Polyline2d);
				Polyline2d->close();
				acedAlert(L"2");
				* /

				}
				*/
				else
				{
					es=pCurve->getSplitCurves(points,curveSegments);
					if(es==Acad::eOk)
					{
						/*acdbOpenObject(pCurve,eId,AcDb::kForWrite);*/
						if(acdbOpenObject(pCurve,eId,AcDb::kForWrite)!=Acad::eOk)	
						{
							ads_printf(L"打开实体失败！\n");
							return 0;
						}
						pCurve->erase();
						pCurve->close();
						AcDbEntity *iEnt;
						for(int j=0;j<curveSegments.length();j++)
						{
							iEnt=(AcDbEntity *)curveSegments[j];
							ob.AddEntityToDbs(iEnt,0);						
							//							iEnt->draw();
						}
					}
				}
				if(curveSegments.length()>0)curveSegments.removeSubArray(0,curveSegments.length()-1);
			}
			if(es!=Acad::eOk)flag=-1;

		}//while()
		ads_relrb(pointlist);
		ads_ssfree(ssname);   
		//	acedRestoreStatusBar();		
	}	
	if(points.length()>0)points.removeSubArray(0,points.length()-1);		   	   

	delete pLine;	pLine=NULL;
	ads_point Pt;
	Pt[X]=0.0;Pt[Y]=0.0;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);//更新视图	
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);//更新视图	
#endif // VERSION_CAD2010
	

	return flag;
}
//-----------------------------------------------------------------------------
// This is command 'PMOUTDRAW, by  [2003-10-30], , 
void TRIMPMT()
{
	//	CTRIMPMT  mytrimpmt;
	//	mytrimpmt.DoModal();
	////CEditAllRoads  Dlg;
	////Dlg.DoModal();
#ifdef OARXWIZDEBUG
	acutPrintf ("\nOARXWIZDEBUG - plhPPTR() called.");
#endif//OARXWIZDEBUG
	// TODO: Implement the command
	double ORI_X,ORI_Y,ORI_H;//外框左下点X、Y及高
	double OUTER_RB_X,OUTER_RB_Y;
	double TUSCALE;
	ads_name en;
	ads_point result;//新位置
	AcDbObject* pObj = NULL;
	AcDbObjectId eId;
	AcDbEntity *pEnt;
	int n,i;
	ads_point *LPt=NULL;
	ads_point *RPt=NULL;
	ads_point p1,p2,p3,p4,p5,pt,p7,p8,p9;
	ads_point z1,z2;
	z1[Z]=z2[Z]=p1[Z]=p2[Z]=p3[Z]=p4[Z]=p5[Z]=pt[Z]=p7[Z]=p8[Z]=p9[Z]=0.0;
	z1[X]=z2[X]=p1[X]=p2[X]=p3[X]=p4[X]=p5[X]=pt[X]=p7[X]=p8[X]=p9[X]=0.0;
	z1[Y]=z2[Y]=p1[Y]=p2[Y]=p3[Y]=p4[Y]=p5[Y]=pt[Y]=p7[Y]=p8[Y]=p9[Y]=0.0;
	TRIM_ROAD *pTrim=NULL;
	AcGeMatrix3d xformR,xformT;
	AcGeVector3d norm(0,0,1);
	norm.normalize();

	if(RTNORM!=acedEntSel(L"\n选择图框:",en,pt)){acutPrintf(L"选择失败!");return;}
	acdbGetObjectId(eId,en);
	//acdbOpenObject(pObj,eId,AcDb::kForRead);
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();
	if(pObj->isKindOf(TRIM_ROAD::desc()))
	{
		/*acdbOpenObject(pObj,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pTrim = TRIM_ROAD::cast(pObj);
		n=pTrim->NMidPt;
		TKNUM=n-1;
		pTrim->CreateLC();
		pTrim->close();
		pObj->close();
		ORI_X=0.0;
		ORI_Y=0.0;

		TRIMDIAG DIAG;
		DIAG.m_TRIMFLAG=0;
		if(DIAG.DoModal()!=IDOK)return;
		AcGePoint3d OriPt(0,0,0);
		//ads_printf(L"DIAG.m_TRIMFLAG=%d\n",DIAG.m_TRIMFLAG);
		if(DIAG.m_TRIMFLAG==0)
		{
			if(RTNORM==acedGetPoint(NULL,L"\n指定位置:\n",result))
			{
				ORI_X=result[X];
				ORI_Y=result[Y];
				OriPt.x=result[X];
				OriPt.y=result[Y];
			}
			else return;
		}
		/*acdbOpenObject(pObj,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pTrim = TRIM_ROAD::cast(pObj);
		pTrim->CreateResBox(OriPt);	
		AcGePoint3d OriPt1(0,-1.0*pTrim->dSPACE,0);
		ORI_X=ORI_X-1.5*pTrim->dH;
		ORI_Y=ORI_Y-pTrim->dH-pTrim->dSPACE;
		ORI_H=pTrim->High_Outer;
		TUSCALE=pTrim->TUSCALE;
		pTrim->CreateResBox1(OriPt1);	
		pTrim->close();
		pObj->close();

		LPt=new ads_point[n];
		RPt=new ads_point[n];
		for(i=0;i<n;i++)
		{
			LPt[i][X]=pTrim->LPt[i].x;
			LPt[i][Y]=pTrim->LPt[i].y;
			LPt[i][Z]=0.0;
			RPt[i][X]=pTrim->RPt[i].x;
			RPt[i][Y]=pTrim->RPt[i].y;
			RPt[i][Z]=0.0;
		}	
		double jiao41,jiao32,jiao;//左边，右边，下边与X轴夹角
		double djiao1,djiao2;//
		double xmin,xmax,ymin,ymax;
		z1[X]=pTrim->boxallOUT.LDPt.x;	z1[Y]=pTrim->boxallOUT.LDPt.y;
		z2[X]=pTrim->boxallOUT.RTPt.x;	z2[Y]=pTrim->boxallOUT.RTPt.y;

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
		acedCommandS(RTSTR, L"-OSNAP", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
		ads_command(RTSTR, L"-OSNAP", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2010

		if(DIAG.m_TRIMFLAG>0)
		{

#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"LAYOUT", RTSTR, L"NEW", RTSTR, L"TRIMTU", RTNONE);
			acedCommandS(RTSTR, L"LAYOUT", RTSTR, L"SET", RTSTR, L"TRIMTU", RTNONE);

			acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
			acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
			acedCommandS(RTSTR, L"LIMITS", RTPOINT, z1, RTPOINT, z2, RTNONE);
			acedCommandS(RTSTR, L"ZOOM", RTPOINT, z1, RTPOINT, z2, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"LAYOUT", RTSTR, L"NEW", RTSTR, L"TRIMTU", RTNONE);
			ads_command(RTSTR, L"LAYOUT", RTSTR, L"SET", RTSTR, L"TRIMTU", RTNONE);

			ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
			ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
			ads_command(RTSTR, L"LIMITS", RTPOINT, z1, RTPOINT, z2, RTNONE);
			ads_command(RTSTR, L"ZOOM", RTPOINT, z1, RTPOINT, z2, RTNONE);
#endif // VERSION_CAD2010

		}


		double XLen = 0.0;
		double XCors,YCors;
		if (DIAG.m_IsDrawZDM)
		{

			XCors = pTrim->resBOX1[pTrim->NMidPt-1].RDPt.x + 50.0*TUSCALE;
			YCors = pTrim->resBOX1[pTrim->NMidPt-1].RDPt.y + 10.0*TUSCALE;
			DRAWZDMForPZT(DIAG.m_MdbName, XCors, YCors, TUSCALE, XLen);
			pTrim->boxallIN.RTPt.x += XLen/2.0*TUSCALE + 350.0*TUSCALE;
			pTrim->boxallIN.RDPt.x += XLen/2.0*TUSCALE + 350.0*TUSCALE;
			pTrim->boxallOUT.RTPt.x += XLen/2.0*TUSCALE + 350.0*TUSCALE;
			pTrim->boxallOUT.RDPt.x += XLen/2.0*TUSCALE + 350.0*TUSCALE;

		}

		//p7[X]=XCors;
		//p7[Y]=YCors;
		//p8[X]=pTrim->boxallIN.RTPt.x;
		//p8[Y]=pTrim->boxallIN.RTPt.y;
		//p9[X]=XCors-2;
		//p9[Y]=YCors-2;
		//acedCommandS(RTSTR,L"scale",RTSTR,L"w",RTPOINT,p9,RTPOINT,p8,RTSTR,L"",RTPOINT,p7,RTREAL,TUSCALE,RTSTR,L" ",RTNONE);

		//ads_printf(L"%d %f %f %f\n",t,p9[Y],p8[X],p8[Y]);
		//if (fabs(TUSCALE-1.0)<0.001)
		//{
		//	acedCommandS(RTSTR,L"scale",RTSTR,L"w",RTPOINT,p9,RTPOINT,p8,RTSTR,L"",RTPOINT,p7,RTREAL,1.0,RTSTR,L" ",RTNONE);
		//}		
		//if (fabs(TUSCALE-2.0)<0.001)
		//{
		//	acedCommandS(RTSTR,L"scale",RTSTR,L"w",RTPOINT,p9,RTPOINT,p8,RTSTR,L"",RTPOINT,p7,RTREAL,2.0,RTNONE);
		//}



		p1[X]=pTrim->boxallOUT.LDPt.x;
		p1[Y]=pTrim->boxallOUT.LDPt.y;
		p2[X]=pTrim->boxallOUT.LTPt.x;
		p2[Y]=pTrim->boxallOUT.LTPt.y;
		p3[X]=pTrim->boxallOUT.RTPt.x;
		p3[Y]=pTrim->boxallOUT.RTPt.y;
		p4[X]=pTrim->boxallOUT.RDPt.x;
		p4[Y]=pTrim->boxallOUT.RDPt.y;


		OUTER_RB_X=pTrim->boxallOUT.RDPt.x;
		OUTER_RB_Y=pTrim->boxallOUT.RDPt.y;


		BAS_DRAW_FUN::SetCurLayer(L"0");
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR,L"pline",RTPOINT,p1,RTSTR,L"w",RTREAL,0.0,RTREAL,0.0,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,RTSTR,L"CLOSE",RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR,L"pline",RTPOINT,p1,RTSTR,L"w",RTREAL,0.0,RTREAL,0.0,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,RTSTR,L"CLOSE",RTNONE);
#endif // VERSION_CAD2010
		

		p1[X]=pTrim->boxallIN.LDPt.x;
		p1[Y]=pTrim->boxallIN.LDPt.y;
		p2[X]=pTrim->boxallIN.LTPt.x;
		p2[Y]=pTrim->boxallIN.LTPt.y;
		p3[X]=pTrim->boxallIN.RTPt.x;
		p3[Y]=pTrim->boxallIN.RTPt.y;
		p4[X]=pTrim->boxallIN.RDPt.x;
		p4[Y]=pTrim->boxallIN.RDPt.y;

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 1.0*TUSCALE, RTREAL, 1.0*TUSCALE, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);

		acedCommandS(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"ACAD_ISO08W100", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 1.0*TUSCALE, RTREAL, 1.0*TUSCALE, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);

		ads_command(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"ACAD_ISO08W100", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010


		for(i=1;i<n-1;i++)
		{
			p1[X]=0.5*(pTrim->resBOX1[i].RDPt.x+pTrim->resBOX1[i+1].LDPt.x);
			p1[Y]=pTrim->resBOX1[i].RDPt.y;
			p2[X]=0.5*(pTrim->resBOX1[i].RTPt.x+pTrim->resBOX1[i+1].LTPt.x);
			p2[Y]=pTrim->resBOX1[i].RTPt.y;
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR,L"pline",RTPOINT,p1,RTSTR,L"w",RTREAL,0.0,RTREAL,0.0,RTPOINT,p2,RTSTR,L"",RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010
		}

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"ByLayer", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"ByLayer", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010
		

		for(i=1;i<n;i++)
		{
			p1[X]=pTrim->resBOX1[i].LDPt.x;
			p1[Y]=pTrim->resBOX1[i].LDPt.y;
			p2[X]=pTrim->resBOX1[i].LTPt.x;
			p2[Y]=pTrim->resBOX1[i].LTPt.y;
			p3[X]=pTrim->resBOX1[i].RTPt.x;
			p3[Y]=pTrim->resBOX1[i].RTPt.y;
			p4[X]=pTrim->resBOX1[i].RDPt.x;
			p4[Y]=pTrim->resBOX1[i].RDPt.y;
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010
			
		}

		int si,ei;
		si=int(_wtof(DIAG.m_START)+0.01);
		ei=int(_wtof(DIAG.m_END)+0.01);
		//FILE *fpr;
		//fpr=_wfopen(L"c:\\syspath.txt",L"r");
		if(DIAG.m_TRIMFLAG>0)
		{
			//if(fpr!=NULL)
			//{
			//	fwscanf(fpr,L"%s",SysPath);
			//	fclose(fpr);
			//	char lineFname[256];
			//	_stprintf(lineFname,L"%s\\lib\\rail.lin",SysPath);
			//	acdbHostApplicationServices()->workingDatabase()->
			//		loadLineTypeFile(L"cc",lineFname);//
			//}

			ACHAR lineFname[256];
			_stprintf(lineFname,L"%s\\lib\\rail.lin",SysPath);
			acdbHostApplicationServices()->workingDatabase()->
				loadLineTypeFile(L"cc",lineFname);

#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"cc", RTSTR, L"", RTNONE);
			acedCommandS(RTSTR, L"-layer", RTSTR, L"new", RTSTR, L"TRIMBIANKUANG", RTSTR, L"PLOT", RTSTR, L"n", RTSTR, L"TRIMBIANKUANG", RTSTR, L"", RTNONE);
			acedCommandS(RTSTR, L"-layer", RTSTR, L"MAKE", RTSTR, L"TRIMBIANKUANG", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"cc", RTSTR, L"", RTNONE);
			ads_command(RTSTR, L"-layer", RTSTR, L"new", RTSTR, L"TRIMBIANKUANG", RTSTR, L"PLOT", RTSTR, L"n", RTSTR, L"TRIMBIANKUANG", RTSTR, L"", RTNONE);
			ads_command(RTSTR, L"-layer", RTSTR, L"MAKE", RTSTR, L"TRIMBIANKUANG", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010

		}
		else
		{
			acedSetStatusBarProgressMeter(L"正在切图!",0,100);
		}
		for(i=si;i<=ei;i++)
		{
			acutPrintf(L"\n正在输出第%d幅(共%d幅图),请稍候......",i,n-1);

			if(DIAG.m_TRIMFLAG>0)
			{				
				p1[X]=pTrim->resBOX[i].LTPt.x;
				p1[Y]=pTrim->resBOX[i].LTPt.y;

				p2[X]=pTrim->resBOX[i].RTPt.x;
				p2[Y]=pTrim->resBOX[i].RTPt.y;

				p3[X]=pTrim->resBOX[i].RDPt.x;
				p3[Y]=pTrim->resBOX[i].RDPt.y;

				p4[X]=pTrim->resBOX[i].LDPt.x;
				p4[Y]=pTrim->resBOX[i].LDPt.y;	

				jiao41=ads_angle(p4,p1);
				jiao32=ads_angle(p3,p2);
				jiao=pTrim->jiao[i]+0.5*pi;
				djiao1=jiao41-jiao;
				if(djiao1<0.0)djiao1+=2*pi;
				djiao2=jiao32-jiao;
				if(djiao2<0.0)djiao2+=2*pi;
				djiao1-=0.5*pi;
				djiao2-=0.5*pi;
				p5[X]=RPt[i-1][X]+10.0*cos(jiao);
				p5[Y]=RPt[i-1][Y]+10.0*sin(jiao);

				xmin=0.0;	xmax=p3[X]-p4[X];//pTrim->resBOX[i].RDPt.distanceTo(pTrim->resBOX[i].LDPt);
				ymin=0.0;	ymax=pTrim->High;

				if(p1[X]<p4[X])xmin-=p4[X]-p1[X];//pTrim->High*tan(djiao1);
				if(p2[X]>p3[X])xmax+=p2[X]-p3[X];//pTrim->High*tan(djiao2);

				z1[X]=xmin;	z1[Y]=ymin;
				z2[X]=xmax;	z2[Y]=ymax;

#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
				acedCommandS(RTSTR, L"MSPACE", RTNONE);
				acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
				acedCommandS(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, RPt[i - 1], RTPOINT, RPt[i], RTPOINT, p5, RTNONE);
				acedCommandS(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);
				acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
				acedCommandS(RTSTR, L"PSPACE", RTNONE);
				acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
				acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
				ads_command(RTSTR, L"MSPACE", RTNONE);
				ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
				ads_command(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, RPt[i - 1], RTPOINT, RPt[i], RTPOINT, p5, RTNONE);
				ads_command(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);
				ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
				ads_command(RTSTR, L"PSPACE", RTNONE);
				ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
				ads_command(RTNONE);
#endif // VERSION_CAD2010

				acutPrintf(L"OK!\n");
			}
			else
			{
				ads_point p0,p1,p2,p3,basePt;
				p0[X]=pTrim->LPt[i-1].x;
				p0[Y]=pTrim->LPt[i-1].y;
				p0[Z]=0;
				p1[X]=pTrim->LPt[i].x;
				p1[Y]=pTrim->LPt[i].y;
				p1[Z]=0;

				p2[X]=pTrim->RPt[i].x;
				p2[Y]=pTrim->RPt[i].y;
				p2[Z]=0;
				p3[X]=pTrim->RPt[i-1].x;
				p3[Y]=pTrim->RPt[i-1].y;
				p3[Z]=0;

				AutoEntBK(p0,p1,p2,p3);

				/*acdbOpenObject(pObj,eId,AcDb::kForRead);*/
				if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pTrim = TRIM_ROAD::cast(pObj);
				AcGePoint3d PP=pTrim->MidPt[i-1];
				basePt[X]=0.5*(pTrim->MidPt[i-1].x+pTrim->MidPt[i].x);
				basePt[Y]=0.5*(pTrim->MidPt[i-1].y+pTrim->MidPt[i].y);
				pTrim->close();

				double jiao=pTrim->angleX(pTrim->LPt[i-1],pTrim->LPt[i]);
				result[X] = pTrim->resBOX[i].Cen.x,result[Y] = pTrim->resBOX[i].Cen.y;
				AcGeVector3d  moveBy(result[X]-basePt[X],result[Y]-basePt[Y],0);
				PP.set(basePt[X],basePt[Y],0);
				xformR.setToRotation(-1.0*jiao,norm,PP);
				xformT.setToTranslation(moveBy);
				ads_point pt1,pt2,pt3,pt4;
				CalFrame(-3.0,pTrim->LPt[i-1],pTrim->LPt[i],pTrim->RPt[i],pTrim->RPt[i-1],pt1,pt2,pt3,pt4);

				flag_k=1;
				p0[X]=pTrim->LPt[i-1].x;
				p0[Y]=pTrim->LPt[i-1].y;
				p1[X]=pTrim->LPt[i].x;
				p1[Y]=pTrim->LPt[i].y;
				p2[X]=pTrim->RPt[i].x;
				p2[Y]=pTrim->RPt[i].y;
				p3[X]=pTrim->RPt[i-1].x;
				p3[Y]=pTrim->RPt[i-1].y;

				/*
				pt1[X]=p0[X];
				pt1[Y]=p0[Y];
				pt2[X]=p1[X];
				pt2[Y]=p1[Y];
				pt3[X]=p2[X];
				pt3[Y]=p2[Y];
				pt4[X]=p3[X];
				pt4[Y]=p3[Y];
				*/
				AutoEntBK(p0,p1,p2,p3);
				AutoEntTransform(xformR,xformT,pt1,pt2,pt3,pt4);
				for(int k=2;k<20;k++)
				{
					flag_k=k;//是否显示进度条。<2显示
					CalFrame((k-1),pTrim->LPt[i-1],pTrim->LPt[i],pTrim->RPt[i],pTrim->RPt[i-1],p0,p1,p2,p3);
					if(AutoEntBK(p0,p1,p2,p3)>0)k=20;
					AutoEntTransform(xformR,xformT,pt1,pt2,pt3,pt4);
				}
			}
		}
		if(DIAG.m_TRIMFLAG>0)
		{
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR,L"-VPORTS",RTSTR,L"LOCK",RTSTR,L"ON",RTSTR,L"ALL",RTSTR,L"",RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR,L"-VPORTS",RTSTR,L"LOCK",RTSTR,L"ON",RTSTR,L"ALL",RTSTR,L"",RTNONE);
#endif // VERSION_CAD2010
		}


		if (DIAG.m_IsDrawZDM)//出平纵图时不画封面,需要插入
		{
			ACHAR DWGPath[256];
			ads_point InsertPt;

			InsertPt[X] = pTrim->resBOX1[pTrim->NMidPt-1].RTPt.x + 10.0*TUSCALE;
			InsertPt[Y] = pTrim->boxallIN.RTPt.y - 100.0*TUSCALE;
			InsertPt[Z] = 0.0;
			_stprintf(DWGPath,L"%s\\dwg\\公路主要技术标准.dwg",SysPath);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, InsertPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, InsertPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010
			




			InsertPt[X] = pTrim->boxallIN.RTPt.x - 120.0*TUSCALE;
			InsertPt[Y] = pTrim->boxallIN.RTPt.y - 140.0*TUSCALE;
			_stprintf(DWGPath,L"%s\\dwg\\公路工程数量表.dwg",SysPath);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, InsertPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, InsertPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010
			


			InsertPt[X] = pTrim->boxallIN.RDPt.x - 180.0*TUSCALE;
			InsertPt[Y] = pTrim->boxallIN.RDPt.y;
			_stprintf(DWGPath,L"%s\\dwg\\公路设计信息.dwg",SysPath);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, InsertPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, InsertPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010
			


			return;
		}

		//////////////////////////////////////////////////////////////////////////
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"CONTINUOUS", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"-linetype", RTSTR, L"SET", RTSTR, L"CONTINUOUS", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010
		
		//////////////////////////////////////////////////////////////////////////

		double dd=TUSCALE*10.0;
		double ddLR=TUSCALE*15.0;

		///////////////图例

		//外
		p1[X]=ORI_X;
		p1[Y]=ORI_Y;
		p2[X]=ORI_X-TUSCALE*210.0;
		p2[Y]=ORI_Y;
		p3[X]=ORI_X-TUSCALE*210.0;
		p3[Y]=ORI_Y+ORI_H;
		p4[X]=ORI_X;
		p4[Y]=ORI_Y+ORI_H;

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010
		

		//内
		p1[X]-=ddLR;
		p1[Y]+=dd;
		p2[X]+=ddLR;
		p2[Y]=p1[Y];
		p3[X]=p2[X];
		p3[Y]-=dd;;
		p4[X]=p1[X];
		p4[Y]=p3[Y];

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR,L"pline",RTPOINT,p1,RTSTR,L"w",RTREAL,1.0*TUSCALE,RTREAL,1.0*TUSCALE,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,RTSTR,L"CLOSE",RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 1.0*TUSCALE, RTREAL, 1.0*TUSCALE, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010


		//封面//////////////////////////////////////////////////////////////////////////

		//外
		p1[X]=ORI_X-TUSCALE*210.0;
		p1[Y]=ORI_Y;
		p2[X]=ORI_X-TUSCALE*210.0-TUSCALE*210.0;
		p2[Y]=ORI_Y;
		p3[X]=ORI_X-TUSCALE*210.0-TUSCALE*210.0;
		p3[Y]=ORI_Y+ORI_H;
		p4[X]=ORI_X-TUSCALE*210.0;
		p4[Y]=ORI_Y+ORI_H;

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR,L"pline",RTPOINT,p1,RTSTR,L"w",RTREAL,0.0,RTREAL,0.0,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,RTSTR,L"CLOSE",RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010
		
		//内
		p1[X]-=ddLR;
		p1[Y]+=dd;
		p2[X]+=ddLR;
		p2[Y]=p1[Y];
		p3[X]=p2[X];
		p3[Y]-=dd;
		p4[X]=p1[X];
		p4[Y]=p3[Y];	
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR,L"pline",RTPOINT,p1,RTSTR,L"w",RTREAL,1.0*TUSCALE,RTREAL,1.0*TUSCALE,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,RTSTR,L"CLOSE",RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 1.0*TUSCALE, RTREAL, 1.0*TUSCALE, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010
		
		////封底
		//外
		p1[X]=OUTER_RB_X;
		p1[Y]=OUTER_RB_Y;
		p2[X]=OUTER_RB_X+TUSCALE*210.0;
		p2[Y]=ORI_Y;
		p3[X]=OUTER_RB_X+TUSCALE*210.0;
		p3[Y]=OUTER_RB_Y+ORI_H;
		p4[X]=OUTER_RB_X;
		p4[Y]=ORI_Y+ORI_H;

#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 0.0, RTREAL, 0.0, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010
		
		//内
		p1[X]+=ddLR;
		p1[Y]+=dd;
		p2[X]-=ddLR;
		p2[Y]=p1[Y];
		p3[X]=p2[X];
		p3[Y]-=dd;
		p4[X]=p1[X];
		p4[Y]=p3[Y];	
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 1.0*TUSCALE, RTREAL, 1.0*TUSCALE, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"pline", RTPOINT, p1, RTSTR, L"w", RTREAL, 1.0*TUSCALE, RTREAL, 1.0*TUSCALE, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
#endif // VERSION_CAD2010
		
		//插入图块
		//fpr=_wfopen(L"c:\\syspath.txt",L"r");
		//if(fpr!=NULL)
		//{
		//	fwscanf(fpr,L"%s",SysPath);
		//	fclose(fpr);
		//	char DWGPath[256];
		//	_stprintf(DWGPath,L"%s\\dwg\\fd.dwg",SysPath);//2:_stprintf(DWGPath,L"%s\\dwg\\1015",SysPath);
		//	acedCommandS(RTSTR,L"-insert",RTSTR,DWGPath,RTSTR,L"s",RTREAL,TUSCALE,RTPOINT,p1,RTREAL,0.0,RTNONE);
		//}


		ACHAR DWGPath[256];
		_stprintf(DWGPath,L"%s\\dwg\\fd.dwg",SysPath);//2:_stprintf(DWGPath,L"%s\\dwg\\1015",SysPath);
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, p1, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, TUSCALE, RTPOINT, p1, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010
		


		p1[X]=p2[X]-12.5*TUSCALE;
		p1[Y]=p2[Y]+25.0*TUSCALE;
		ACHAR scale[20];
		_stprintf(scale,L"1:%ld",long(TUSCALE*1000+0.001));
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"text", RTSTR, L"j", RTSTR, L"c", RTPOINT, p1, RTREAL, 3.5*TUSCALE, RTREAL, 0.0, RTSTR, scale, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"text", RTSTR, L"j", RTSTR, L"c", RTPOINT, p1, RTREAL, 3.5*TUSCALE, RTREAL, 0.0, RTSTR, scale, RTNONE);
#endif // VERSION_CAD2010

	}
	//if(LPt){delete LPt;	LPt=NULL;}
	//if(RPt){delete RPt;	RPt=NULL;}
	acedRestoreStatusBar();
}

void CalFrame(double dist,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,ads_point &p1,ads_point &p2,ads_point &p3,ads_point &p4)
{
	//用四点作一POLYLINE，OFFSET后得到另一条POLYLINE。
	AcGePoint3dArray vertices;
	vertices.append(P1);
	vertices.append(P2);
	vertices.append(P3);
	vertices.append(P4);
	AcDbVoidPtrArray offsetCurves;

	AcDbObjectId eId,eId1;
	AcDb2dPolyline *Poly=new AcDb2dPolyline(AcDb::k2dSimplePoly,vertices,0.0,Adesk::kTrue);
	eId=ob.AddEntityToDbs(Poly);

	Acad::ErrorStatus es;
	es=acdbOpenObject(Poly,eId,AcDb::kForWrite);
	if(es!=Acad::eOk)return;
	Poly->getOffsetCurves(dist,offsetCurves);
	//	acutPrintf(L"dist=%lf\n",dist);
	Poly->erase();
	Poly->close();
	AcDb2dPolyline *offsetPoly=(AcDb2dPolyline*)offsetCurves[0];
	eId1=ob.AddEntityToDbs(offsetPoly);
	AcDbObjectIterator *pVertIter=offsetPoly->vertexIterator();
	AcDb2dVertex *pVertex;
	AcGePoint3d location;
	AcDbObjectId VertexObjId;
	AcGePoint3dArray ver;
	for(int vertexNumber=0;!pVertIter->done();vertexNumber++,pVertIter->step())
	{
		VertexObjId=pVertIter->objectId();
		/*acdbOpenObject(pVertex,VertexObjId,AcDb::kForRead);*/
		if(acdbOpenObject(pVertex, VertexObjId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		location=pVertex->position();
		ver.append(location);
		pVertex->close();								
	}
	delete pVertIter;
	p1[X]=ver[0].x;p1[Y]=ver[0].y;
	p2[X]=ver[1].x;p2[Y]=ver[1].y;
	p3[X]=ver[2].x;p3[Y]=ver[2].y;
	p4[X]=ver[3].x;p4[Y]=ver[3].y;
	ver.removeSubArray(0,ver.length()-1);	
	//acdbOpenObject(offsetPoly,eId1,AcDb::kForWrite);
	if(acdbOpenObject(offsetPoly,eId1,AcDb::kForWrite)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	offsetPoly->erase();
	offsetPoly->close();
	vertices.removeSubArray(0,vertices.length()-1);

}//-----------------------------------------------------------------------------

void AutoEntTransform(AcGeMatrix3d xformr,AcGeMatrix3d xformt,ads_point p1, ads_point p2,ads_point p3, ads_point p4)
{
	AcGeMatrix3d xform=xformt*xformr;
	ads_name ssname,ename;
	struct resbuf *pointlist;
	long n;
	AcDbObjectId eId;
	AcDbEntity *pEnt;
	pointlist = ads_buildlist(RTPOINT,p1,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,0);
	acedSSGet(L"WP",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		//ads_printf(L"Number of entity is %ld\n",n);
		int Max=n;
		double Cur=0;
		int Pos=0;
		AcDb2dPolyline* Polyline2d;
		while(n > 0)
		{
			Cur=(Max-n)*100.0/Max;
			Pos=(int)Cur;
			acedSetStatusBarProgressMeterPos(Pos);

			n = n - 1;
			ads_ssname(ssname,n,ename);
			acdbGetObjectId(eId,ename); 
			if(Acad::eOk!=acdbOpenObject(pEnt,eId,AcDb::kForWrite))continue;
			if(pEnt->isKindOf(AcDb2dPolyline::desc()))
			{
				Polyline2d=AcDb2dPolyline::cast(pEnt);
				//acdbOpenObject(Polyline2d,eId,AcDb::kForWrite);
				Polyline2d->setLinetypeGenerationOn();
			}
			pEnt->transformBy(xform);
			pEnt->close();
			//			pEnt->draw();
		}
		ads_relrb(pointlist);
		ads_ssfree(ssname); 
		//	acedRestoreStatusBar();
	}
	ads_point Pt;
	Pt[X]=0.0;Pt[Y]=0.0;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);
#endif // VERSION_CAD2010
		
}

void LanXun(ads_point p1, ads_point p2,ads_point p3, ads_point p4)
{
	//AcGeMatrix3d xform=xformt*xformr;
	ads_name ssname,ename;
	struct resbuf *pointlist;
	long n;
	AcDbObjectId eId;
	AcDbEntity *pEnt;
	pointlist = ads_buildlist(RTPOINT,p1,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,0);
	acedSSGet(L"CP",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		//ads_printf(L"Number of entity is %ld\n",n);
		int Max=n;
		double Cur=0;
		int Pos=0;
		AcDb2dPolyline* Polyline2d;
		while(n > 0)
		{
			Cur=(Max-n)*100.0/Max;
			Pos=(int)Cur;
			acedSetStatusBarProgressMeterPos(Pos);

			n = n - 1;
			ads_ssname(ssname,n,ename);
			acdbGetObjectId(eId,ename); 
			if(Acad::eOk!=acdbOpenObject(pEnt,eId,AcDb::kForWrite))continue;
			if(pEnt->isKindOf(AcDb2dPolyline::desc()))
			{
				Polyline2d=AcDb2dPolyline::cast(pEnt);
				//acdbOpenObject(Polyline2d,eId,AcDb::kForWrite);
				Polyline2d->setLinetypeGenerationOn();
			}
			//	pEnt->transformBy(xform);
			pEnt->close();
			//			pEnt->draw();
		}
		ads_relrb(pointlist);
		ads_ssfree(ssname); 
		//	acedRestoreStatusBar();
	}
	// 	ads_point Pt;
	// 	Pt[X]=0.0;Pt[Y]=0.0;
	// 	acedCommandS(RTSTR,L"PAN",RTPOINT,Pt,RTPOINT,Pt,RTNONE);	
}

bool FSelectEntitys(ads_point p1, ads_point p2,ads_point p3, ads_point p4)
{
	bool IsEmptyCP;//框内是否已空
	ads_name ssname;
	struct resbuf *pointlist;
	long n;
	pointlist = ads_buildlist(RTPOINT,p1,RTPOINT,p2,RTPOINT,p3,RTPOINT,p4,0);
	acedSSGet(L"CP",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		if(n>0)IsEmptyCP=false;
		else IsEmptyCP=true;
		ads_relrb(pointlist);
		ads_ssfree(ssname); 
	}
	else IsEmptyCP=true;

	return IsEmptyCP;
}

// This is command 'SAVEPMTK, by  [2003-11-5], , 
void SAVEPMTK()
{
#ifdef OARXWIZDEBUG
	acutPrintf (L"\nOARXWIZDEBUG - plhPSAVEPMTK() called.");
#endif // OARXWIZDEBUG
	// TODO: Implement the command
	struct resbuf result;
	AcDbObjectId eId;
	TRIM_ROAD *pTrim;
	int rc=RTNORM;
	ACHAR fname[80];
	FILE *fpc;
	ads_name en;
	ads_point pt;
	AcDbEntity * pEnt;
	AcDbObject* pObj = NULL;
	if(RTNORM!=acedEntSel(L"请选择图框:",en,pt)){acutPrintf(L"选择图框失败!\n");return;}
	acdbGetObjectId(eId,en);
	/*acdbOpenObject(pObj,eId,AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();

	if(!pObj->isKindOf(TRIM_ROAD::desc())){acutPrintf(L"所选不是图框!\n");return;}
	/*acdbOpenObject(pObj,eId,AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pTrim = TRIM_ROAD::cast(pObj);
	int NUM=pTrim->NMidPt;//边框数==图框数+1
	double TKSCALE=pTrim->TUSCALE;
	double OuterHigh=pTrim->High_Outer;//外框高
	double dFrame=pTrim->dH;//边框间距
	double dTK=pTrim->dTK;//
	AcGePoint3d *MidPt=new AcGePoint3d[NUM];
	int i;
	for( i=0;i<NUM;i++)
	{
		MidPt[i]=pTrim->MidPt[i];
	}
	pTrim->close();

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256];
	if(ob.ReadWorkdir())
	{
		_tcscpy(DefaultPath,Cworkdir);
		_tcscat(DefaultPath,L"\\DATA\\");
		_tcscat(DefaultPath,Pro);
		_tcscat(DefaultPath,L".tk");
	}
	else _tcscpy(DefaultPath,L"");
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////
	long version=TRIMVERSION;
	if ((ads_getfiled( L"请输入图框文件名 *.tk ",DefaultPath,L"tk",1+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	//	ads_printf(L"%s",result.resval.rstring);
	_tcscpy(fname,result.resval.rstring);   
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w+"))==NULL){return;}
	else
	{
		fwprintf(fpc,L"%ld\n",version);
		fwprintf(fpc,L"%d\n",NUM);
		fwprintf(fpc,L"%lf\n",TKSCALE);
		fwprintf(fpc,L"%lf\n",OuterHigh);
		fwprintf(fpc,L"%lf\n",dFrame);
		fwprintf(fpc,L"%lf\n",dTK);
		fwprintf(fpc,L"%lf\n",pTrim->dSPACE);
		for (i=0;i<NUM;i++)fwprintf(fpc,L"%lf %lf\n",MidPt[i].x,MidPt[i].y);
		//		fwprintf(fpc,L"%lf\n",dFrame);
		fclose(fpc);
	}

	if(MidPt!=NULL)delete []MidPt;

}

//-----------------------------------------------------------------------------
// This is command 'OPENPMTK, by  [2003-11-5], , 
void OPENPMTK()
{
#ifdef OARXWIZDEBUG
	acutPrintf ("\nOARXWIZDEBUG - plhPOPENPMTK() called.");
#endif // OARXWIZDEBUG

	// TODO: Implement the command
	int NUM;//边框数==图框数+1
	double TKSCALE;
	double OuterHigh;//外框高
	double dFrame;//边框间距
	double dTK;//两折图斜线的空白间距
	double dSpace;
	TRIM_ROAD *pTrim=NULL;

	struct resbuf result;
	AcDbObjectId eId;
	AcDbObject* pObj = NULL;
	int rc=RTNORM;
	ACHAR fname[80];
	FILE *fpc;

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256];
	if(ob.ReadWorkdir())
	{
		_tcscpy(DefaultPath,Cworkdir);
		_tcscat(DefaultPath,L"\\DATA\\");
		_tcscat(DefaultPath,Pro);
		_tcscat(DefaultPath,L".tk");
	}
	else _tcscpy(DefaultPath,L"");
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////
	long version;
	if ( (ads_getfiled( L"请输入图框文件名 *.tk ",DefaultPath,L"tk",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring);   
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"r"))==NULL){return;}
	else
	{
		fwscanf(fpc,L"%ld\n",&version);
		if(version!=TRIMVERSION)
		{
			AfxMessageBox(L"版本不正确!");
			fclose(fpc);
			return;
		}
		fwscanf(fpc,L"%d\n",&NUM);
		fwscanf(fpc,L"%lf\n",&TKSCALE);
		fwscanf(fpc,L"%lf\n",&OuterHigh);
		fwscanf(fpc,L"%lf\n",&dFrame);
		fwscanf(fpc,L"%lf\n",&dTK);
		fwscanf(fpc,L"%lf\n",&dSpace);
		pTrim=new TRIM_ROAD(OuterHigh,dFrame,dTK,dSpace,TKSCALE);
		eId=ob.AddEntityToDbs(pTrim);
		double x,y;		
		/*acdbOpenObject(pObj,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pTrim = TRIM_ROAD::cast(pObj);
		for(int i=0;i<NUM;i++)
		{
			fwscanf(fpc,L"%lf %lf\n",&x,&y);
			pTrim->AddFrame(AcGePoint3d(x,y,0));
		}
		pTrim->close();
		fclose(fpc);		
	}
}


int AutoEntBK2(ads_point p0, ads_point p1)
{
	ads_name ssname,ename;
	struct resbuf *pointlist;
	long n;
	AcGePoint3d Pt0,Pt1;
	Pt0.set(p0[X],p0[Y],0);
	Pt1.set(p1[X],p1[Y],0);
	AcDbLine pLine(Pt0,Pt1);

	AcDbObjectId eId;
	AcDbEntity *pEnt;
	AcDbCurve *pCurve;

	pointlist = ads_buildlist(RTPOINT,p0,RTPOINT,p1,0);
	//  ads_grdraw(p0,p1,-1,0);
	acedSSGet(L"F",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		ads_printf(L"Number of entity is %ld\n",n);
		while(n > 0)
		{
			n = n - 1;
			ads_ssname(ssname,n,ename);
			acdbGetObjectId(eId,ename); 
			if(Acad::eOk!=acdbOpenObject(pEnt,eId,AcDb::kForWrite)){continue;}
			pEnt->close();
			if(pEnt->isKindOf(TRIM_ROAD::desc())){/*acutPrintf(L"-1")*/;continue;}
			AcGePoint3dArray points;

			AcGePlane plane(AcGePoint3d(0,0,0),AcGeVector3d(0,0,1));
			if(pEnt->isKindOf(AcDbCurve::desc())
				&&Acad::eOk==pEnt->intersectWith(&pLine,AcDb::kOnBothOperands,plane,points)
				&&points.length()>0)
			{
				if(Acad::eOk==acdbOpenObject(pCurve,eId,AcDb::kForRead))
				{
					pCurve->close();
					AcDbVoidPtrArray entitySet;
					AcDbVoidPtrArray curveSegments;
					if(pCurve->isKindOf(AcDbPolyline::desc())
						||pCurve->isKindOf(AcDb2dPolyline::desc())
						||pCurve->isKindOf(AcDb3dPolyline::desc()))
					{
						int flag23d=0;//2:2d,3:3d
						AcDbPolyline *poly=NULL;
						AcDb2dPolyline *poly2d=NULL;
						AcDb3dPolyline *poly3d=NULL;
						if(pCurve->isKindOf(AcDb2dPolyline::desc())){flag23d=2;pCurve->cast(poly2d);}
						else if(pCurve->isKindOf(AcDb3dPolyline::desc())){flag23d=3;pCurve->cast(poly3d);}
						else {flag23d=0;pCurve->cast(poly);}
						int num=0;
						if(flag23d==0) num=poly->numVerts();
						AcGePoint3dArray PtAY;
						AcDb::Poly3dType type3d;
						AcDb::Poly2dType type2d;						
						if(flag23d==0)for(int i=0;i<num;i++)
						{
							poly->getPointAt(i,PtAY[i]);
						}
						else if(flag23d==2)
						{
							acdbOpenObject(poly2d,eId,AcDb::kForRead);
							AcDbObjectIterator *pVertIter=poly2d->vertexIterator();
							poly2d->close();
							AcDb2dVertex *pVertex;
							AcGePoint3d location;
							AcDbObjectId vertexObjId;
							int vertexNumber;
							for(vertexNumber=0;!pVertIter->done();
								vertexNumber++,pVertIter->step())
							{
								vertexObjId=pVertIter->objectId();
								/*acdbOpenObject(pVertex,vertexObjId,AcDb::kForRead);*/
								if(acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead)!=Acad::eOk)
								{
									ads_printf(L"打开实体失败！\n");
									return 0;
								}
								location=pVertex->position();
								pVertex->close();
								PtAY.append(location);								
							}
							num=vertexNumber-1;
							delete pVertIter;
						}
						else
						{
							/*acdbOpenObject(poly3d,eId,AcDb::kForRead);*/
							if(acdbOpenObject(poly3d, eId, AcDb::kForRead)!=Acad::eOk)
							{
								ads_printf(L"打开实体失败！\n");
								return 0;
							}
							AcDbObjectIterator *pVertIter=poly3d->vertexIterator();
							poly3d->close();
							AcDb2dVertex *pVertex;
							AcGePoint3d location;
							AcDbObjectId vertexObjId;
							int vertexNumber;
							for( vertexNumber=0;!pVertIter->done();
								vertexNumber++,pVertIter->step())
							{
								vertexObjId=pVertIter->objectId();
								/*acdbOpenObject(pVertex,vertexObjId,AcDb::kForRead);*/
								if(acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead)!=Acad::eOk)
								{
									ads_printf(L"打开实体失败！\n");
									return 0;
								}
								location=pVertex->position();
								pVertex->close();
								PtAY.append(location);
							}
							num=vertexNumber-1;
							delete pVertIter;
						}
						if(flag23d==3)
						{
							poly->cast(poly3d);
							type3d=poly3d->polyType();
							if(type3d==AcDb::k3dSimplePoly)type2d=AcDb::k2dSimplePoly;
							else if(type3d==AcDb::k3dQuadSplinePoly)type2d=AcDb::k2dQuadSplinePoly;
							else type2d=AcDb::k2dCubicSplinePoly;
							flag23d=3;
						}
						if(flag23d==2)
						{
							poly->cast(poly2d);
							type2d=poly2d->polyType();
							flag23d=2;
						}
						/*acdbOpenObject(poly,eId,AcDb::kForWrite);*/
						if(acdbOpenObject(poly,eId,AcDb::kForWrite)!=Acad::eOk)	
						{
							ads_printf(L"打开实体失败！\n");
							return 0;
						}
						poly->erase();
						poly->close();
						if(poly==NULL){delete poly;poly=NULL;}
						if(poly2d==NULL){delete poly2d;poly2d=NULL;}
						if(poly3d==NULL){delete poly3d;poly3d=NULL;}
						poly2d=new AcDb2dPolyline(type2d,PtAY);
						PtAY.removeSubArray(0,num-1);
						/*acdbOpenObject(poly2d,eId,AcDb::kForWrite);*/
						if(acdbOpenObject(poly2d,eId,AcDb::kForWrite)!=Acad::eOk)	
						{
							ads_printf(L"打开实体失败！\n");
							return 0;
						}
						poly2d->close();
						poly2d->draw();
						poly2d->intersectWith(&pLine,AcDb::kOnBothOperands,plane,points);
						int l=points.length();
						if(Acad::eOk==poly2d->getSplitCurves(points,curveSegments))
						{
							/*acdbOpenObject(poly2d,eId,AcDb::kForWrite);*/
							if(acdbOpenObject(poly2d,eId,AcDb::kForWrite)!=Acad::eOk)	
							{
								ads_printf(L"打开实体失败！\n");
								return 0;
							}
							poly2d->erase();
							poly2d->close();			   
							for(int j=0;j<curveSegments.length();j++)
							{
								AcDb2dPolyline *iEnt=(AcDb2dPolyline *)curveSegments[j];
								if(flag23d==2||flag23d==3)iEnt->convertToPolyType(type2d);
								ob.AddEntityToDbs(iEnt,0);						
								iEnt->draw();
							}
						}
						/*
						else
						{
						acdbOpenObject(poly2d,eId,AcDb::kForWrite);
						poly2d->close();
						AcDbVoidPtrArray entitySet;
						poly2d->explode(entitySet);
						int len=entitySet.length();
						for(int k=0;k<len;k++)
						{
						(entitySet[k])->intersectWith(&pLine,AcDb::kOnBothOperands,plane,points);
						}

						}					*/

					}
					else					
					{
						if(Acad::eOk==pCurve->getSplitCurves(points,curveSegments))
						{
							/*acdbOpenObject(pCurve,eId,AcDb::kForWrite);*/
							if(acdbOpenObject(pCurve,eId,AcDb::kForWrite)!=Acad::eOk)	
							{
								ads_printf(L"打开实体失败！\n");
								return 0;
							}
							pCurve->erase();
							pCurve->close();			   
							for(int j=0;j<curveSegments.length();j++)
							{
								AcDbEntity *iEnt=(AcDbEntity *)curveSegments[j];
								ob.AddEntityToDbs(iEnt,0);						
								iEnt->draw();
							}
						}
					}
				}
				points.removeSubArray(0,points.length()-1);		   	   
			} 
			else{/*acutPrintf(L"NULL\n");*/}
		}
		ads_relrb(pointlist);
		ads_ssfree(ssname);   
	}	
	return 1;
}//-----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

// This is command 'RMENU'
CMyBar* g_pMyBar = NULL;
void prmenurmenu()
{
	if (!CheckDogByDLL())
	{
		//ads_alert(L"未检测到软件锁！");
		return;
	}
	#if (defined GY_ROAD_DULI_SYS)||(defined DIGIROAD)
		if(!CheckByTime())
			return;
	#endif

	struct resbuf res;
	res.restype = RTSHORT;
	res.resval.rint = 1;
	acedSetVar(L"COLORTHEME",&res);

	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);

	if (g_pMyBar == NULL)
	{
		g_pMyBar =new CMyBar;
		//g_pMyBar->m_MODEL = ISMENU;
		g_pMyBar->Create(acedGetAcadFrame(), L"");
		g_pMyBar->SetBorders(0,0,0,0);
		g_pMyBar->EnableDocking(CBRS_ALIGN_TOP);
		//acedGetAcadFrame()->EnableDocking(CBRS_ALIGN_TOP);
		g_pMyBar->AllowDockingMenuItemState();
		CRect rect(0, 0, 100, 40);
		g_pMyBar->DockControlBar(AFX_IDW_DOCKBAR_TOP, rect);
		//g_pMyBar->RestoreControlBar(AFX_IDW_DOCKBAR_TOP, &rect);
		g_pMyBar->SetAllowDockingMenuItemState(true);

		//g_pMyBar->AdjustVertDockedBarHeight(600);
	}
	else 
		acedGetAcadFrame()->ShowControlBar(g_pMyBar, TRUE, FALSE);	
}

///////////////////////////从2012版移植过来////////////////////////////////////

//离散布线
void ROADANDRAILRPMLOC(void)
{
	// Add your code for command ROADANDRAIL.TEST here
	if(!IsFirstLocdlg)
	{
		Locdlg.ShowWindow(TRUE);
	}
	else
	{
		IsFirstLocdlg=false;
		Locdlg.Create(IDD_DIALOG_LOCATION2,acedGetAcadFrame());
		Locdlg.ShowWindow(SW_SHOW);
	}	
}

void ROADANDRAILINXY(void)
{
	// Add your code for command ROADANDRAIL.INXY here
	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity * pEnt;
	if(RTNORM!=acedEntSel(L"\n请选择线路:",en,pt))
	{
		if(XyDlg)
		{
			delete XyDlg;
			XyDlg=NULL;
		}
		XyDlg=new CJMXYDLG(acedGetAcadFrame(),0);
		XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
		XyDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		acdbGetObjectId(eId,en);
		acdbOpenObject(pEnt,eId,AcDb::kForWrite);
		pEnt->close();
		if(pEnt->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER *pline = JD_CENTER::cast(pEnt);

			if(XyDlg)
			{
				delete XyDlg;
				XyDlg=NULL;
			}
			XyDlg=new CJMXYDLG(acedGetAcadFrame(),eId);
			XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
			XyDlg->ShowWindow(SW_SHOW);	
		}
		else
		{
			if(XyDlg)
			{
				delete XyDlg;
				XyDlg=NULL;
			}
			XyDlg=new CJMXYDLG(acedGetAcadFrame(),eId);
			XyDlg->Create(IDD_DIALOG_JMBXINPUT,acedGetAcadFrame());	
			XyDlg->ShowWindow(SW_SHOW);
		}
	}

}

//计算线元交点
void ROADANDRAILXLJD(void)
{
	// Add your code for command ROADANDRAIL.TEST here
	int NLC=3,NH=3,texth=8;
	int rc;
	ads_name en;
	AcDbObject *pObj;
	AcDbObjectId eId,GTZXId;
	AcGePoint3d pt,location;
	JD_CENTER *pJC1=NULL,*pJC2=NULL;
	BAS_DRAW_FUN ob;

	rc = ads_entsel(L"\n选择第一个平面线路实体 : ", en, asDblArray(pt));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		acdbOpenObject(pObj, eId, AcDb::kForRead);
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{		
			pObj->close();
			pJC1 = JD_CENTER::cast(pObj);
		}
		else
		{
			ads_printf(L"不是平面线路实体!\n");
			return;
		}
	}
	else
		return;

	rc = ads_entsel(L"\n选择第二个平面线路实体 : ", en, asDblArray(pt));
	if(rc==RTCAN)
		return;
	if (rc == RTNORM)
	{
		acdbGetObjectId(eId, en);
		acdbOpenObject(pObj, eId, AcDb::kForRead);
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{		
			pObj->close();
			pJC2 = JD_CENTER::cast(pObj);
		}
		else
		{
			ads_printf(L"不是平面线路实体!\n");
			return;
		}
	}
	else
		return;

	AcGePoint2dArray JDpt;
	int JDNUM;
	double ResPt[3],ResPt1[3];
	AcGePoint2d respt;
	if(JDpt.length()>0)
		JDpt.removeSubArray(0,JDpt.length()-1);
	if(pJC1 && pJC2)
	{
		//计算线元交点
		if(pJC1->XYNum>0)
		{
			for(int i=1; i<pJC1->XYNum; i++)
			{
				for(int j=1; j<pJC2->XYNum; j++)
				{
					JDNUM = ob.CalJD_XY1_XY2(pJC1->XYArray[i],pJC2->XYArray[j],ResPt,ResPt1);	
					//					if(JDNUM>0)
					//ads_printf(L"JDNUm=%d sml1=%lf xy1=%lf sml2=%lf xy2=%lf %lf %lf\n",
					//JDNUM,pJC1->XYArray[i][6],pJC1->XYArray[i][0],pJC2->XYArray[j][6],pJC2->XYArray[j][0]
					//,ResPt[0],ResPt[1]);
					if(JDNUM>0)
					{
						respt.set(ResPt[0],ResPt[1]);
						JDpt.append(respt);						
						//ads_printf(L"1:x=%lf y=%lf\n",ResPt[0],ResPt[1]);
					}
					if(JDNUM==2)
					{
						respt.set(ResPt1[0],ResPt1[1]);
						JDpt.append(respt);	
						//ads_printf(L"2:x=%lf y=%lf\n",ResPt1[0],ResPt1[1]);					   
					}

				}
			}

		}

	}

	if(JDpt.length()>0)
	{
		for(int i=0; i<JDpt.length(); i++)
		{
			//		ads_printf(L"%d %lf %lf\n",i,JDpt[i].x,JDpt[i].y);

			ads_point cpt;
			cpt[0]=JDpt[i].y,cpt[1]=JDpt[i].x;
			//	   ads_command(RTSTR,"Circle",RTPOINT,cpt,RTREAL,2.0,RTNONE);
			//计算在线路1上的投影里程
			double cml1,cml2,fwj1,fwj2,zj;
			CString xlc1,xlc2;
			cml1 = pJC1->PROJ_ML(JDpt[i].x,JDpt[i].y);
			xlc1 = pJC1->XLC(cml1);
			BAS_DRAW_FUN::xlpoint PZ;
			PZ.lc = cml1;
			pJC1->xlpoint_pz(&PZ);
			fwj1 = PZ.a;

			cml2 = pJC2->PROJ_ML(JDpt[i].x,JDpt[i].y);
			xlc2 = pJC2->XLC(cml2);
			PZ.lc = cml2;
			pJC2->xlpoint_pz(&PZ);
			fwj2 = PZ.a;
			zj = pJC1->ZXJ(fwj2,fwj1);
			//绘图
			AcGePoint2d pt,pt1,pt2;
			pt.x=JDpt[i].y,pt.y=JDpt[i].x;
			pt1.x = JDpt[i].y + 100 * sin(fwj1+0.5*zj);
			pt1.y = JDpt[i].x + 100 * cos(fwj1+0.5*zj);
			if(pt1.x>pt.x)
				pt2.x = pt1.x + 100;
			else
				pt2.x = pt1.x - 100;
			pt2.y = pt1.y;

			AcDbPolyline *pPline=new AcDbPolyline(3);
			pPline->addVertexAt(0,pt);
			pPline->addVertexAt(1,pt1);
			pPline->addVertexAt(2,pt2);
			AcDbObjectId plId;
			pPline->setColorIndex(5);
			plId=pJC1->AddEntityToDbs(pPline);

			//标注
			AcGePoint3d mpt;
			ACHAR text[80];
			mpt.x = 0.5*(pt1.x+pt2.x);
			mpt.y = 0.5*(pt1.y+pt2.y)+6;
			_tcscpy(text,xlc1);
			pJC1->maketext(mpt,text,0.0,8);

			mpt.y = 0.5*(pt1.y+pt2.y)-6;
			_tcscpy(text,xlc2);
			pJC1->maketext(mpt,text,0.0,8);

			mpt.y -= 10;	  
			_stprintf(text,L"夹角%0.5lf",fabs(pJC1->dms_rad(zj)));
			pJC1->maketext(mpt,text,0.0,8);
		}
	}   

}

bool CheckByTime()//单机版
{
#ifndef CHECK_TIME
	return true;
#endif // !CHECK_TIME

	FILE *fpw;
	ACHAR fname[1024];
	DWORD dwStatus;
	ACHAR Message[100];
	ACHAR str[280];

	//时间检测   

	SYSTEMTIME lpSystemTime;
	GetSystemTime(&lpSystemTime);   // address of system time structure

	_tcscpy(fname, L"\0");
	CString fstr = acedGetAppName();
	if (!fstr.IsEmpty())
	{
		_tcscpy(fname,fstr);
		fstr.MakeUpper();
		fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
		_tcscpy(SysPath,fstr);
	}

	_stprintf(fname, L"%s\\lib\\tm.shx", SysPath);
	double PerTimeNum = 0;
	double CurTimeNum = lpSystemTime.wYear * 1e8 + lpSystemTime.wMonth * 1e6 + lpSystemTime.wDay * 1e4 + lpSystemTime.wHour * 100 + lpSystemTime.wMinute;
	FILE* pFile = NULL;

	//////////////////////////////////////////////////////////////////////
	//生成tm文件
	//pFile = _wfopen(fname, L"wb");
	//if (pFile)
	//{
	//	fwrite(&CurTimeNum, sizeof(double), 1, pFile);
	//	fclose(pFile); 
	//}
	//return true;
	//////////////////////////////////////////////////////////////////////   

	pFile = _wfopen(fname, L"rb");
	if (pFile)
	{
		fread(&PerTimeNum, sizeof(double), 1, pFile);
		/*ads_printf("####Per=%lf,Cur=%lf\n",PerTimeNum, CurTimeNum);*/ 
		if (PerTimeNum>CurTimeNum || PerTimeNum<START_TIME || PerTimeNum>END_TIME || CurTimeNum<START_TIME)
		{
			AfxMessageBox(L"系统时间有误!");
			fclose(pFile);
			return false;
		}
		fclose(pFile);
		pFile = _wfopen(fname, L"wb");
		if (pFile)
		{
			//fwprintf(pFile, L"%ld", CurTimeNum);
			fwrite(&CurTimeNum, sizeof(double), 1, pFile);
			fclose(pFile);
		}
	}
	else
	{
		AfxMessageBox(L"系统文件丢失!");
		return false;
	}

	//if (CurTimeNum>END_TIME)
	//{
	//	AfxMessageBox(L"本系统试用已到期, 请与 13973119632 中南大学蒲浩联系!");
	//	return false;
	//}
	//FILE *pfw;
	//pfw = fopen("c:\\tmp.scr", "w");
	//if(!pfw)
	//{
	//	AfxMessageBox(L"请以管理员身份运行本程序或CAD（操作：鼠标左键选中执行程序，右键选择管理员身份运行)！");
	//	return false;
	//}
	//fclose(pfw);
	return true;
}

//通过动态链接库检测软件狗
bool CheckDogByDLL()
{
#ifndef CHECK_DOG
	return true;
#endif

	ACHAR fname[MAX_PATH];
	_tcscpy(fname, L"\0");
	CString fstr = acedGetAppName();
	if (!fstr.IsEmpty())
	{
		_tcscpy(fname,fstr);
		fstr.MakeUpper();
		fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
		_tcscpy(SysPath,fstr);
	}
	resbuf *pNewRb=acutBuildList(RTSTR,SysPath,0);
	acedSetVar(L"TRUSTEDPATHS",pNewRb);
	acutRelRb(pNewRb);


	typedef int (*lpFun)();//定义函数类型指针，动态调用是需要
	HINSTANCE hDll;//Dll句柄
	lpFun MyFun = NULL;//函数指针
	_stprintf(fname, L"%s\\lib\\cs.cdg", SysPath);
	hDll = LoadLibrary(fname);//加载DLL
	if (hDll!=NULL)//加载成功
	{
		MyFun = (lpFun)GetProcAddress(hDll,"CheckDog");//将DLL中的Add函数赋值给MyFun
		if (MyFun!=NULL)//DLL中的函数获取成功
		{
			int Result = 0;
			Result = MyFun();//调用DLL中的CheckDog
			ads_printf(L"@@@@@@@@ %d\n",Result);
			if (Result==818818)
				return true;
		}
		else
			AfxMessageBox(L"加载DLL失败！");
	}
	else
		AfxMessageBox(L"系统文件丢失，无法启动！");
	FreeLibrary(hDll);//释放DLL
	return false;
}

void tiqushuju()
{
	LoadArx(L"dtmoutdata.arx",L"outdata_l");
}

void chulishuju()
{
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\dldata.exe",SysPath);
	ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);
}

void jianlishumo()
{
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\CreateTin.exe",SysPath);
	//	runexe(fname);
	/*WinExec((LPCSTR)fname,1);*/
	ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);
}
void Changeshumo()
{
	CSelTINDlg dlg;
	dlg.DoModal();
}

void xianshishumo()
{
	if(ptriDTM.DTMHasRead==1)
		ptriDTM.ReleaseDtm();//释放Dtm

	LoadArx(L"3d.arx",L"draw");
}

void TINConvertASC()
{
	if (!CheckDogByDLL())
		return;

	double CellSize = 30.0;
	int RT = acedGetReal(L"\n请输入格网密度<30.0> :", &CellSize);
	if (RT != RTNORM && RT != RTNONE)
		return;

	double NoUseH = -100.0;
	RT = acedGetReal(L"\n请输入默认高程值<-100.0> :", &NoUseH);
	if (RT != RTNORM && RT != RTNONE)
		return;

	triDTM_road* ptriDTM_p = NULL;
	if (ptriDTM_p == NULL)
		ptriDTM_p = new triDTM_road();

	bool IsDtmExisting = true;
	if (!ptriDTM_p->DTMHasRead)
		IsDtmExisting = ptriDTM_p->Read_Dtm();

	if (IsDtmExisting)
	{
		int RowSum, ColSum;
		double Pt[2];
		double Heigh = -100.0;
		ColSum = (ptriDTM_p->RightUpPt[1] - ptriDTM_p->LeftLowPt[1]) / CellSize + 1;
		RowSum = (ptriDTM_p->RightUpPt[0] - ptriDTM_p->LeftLowPt[0]) / CellSize + 1;

		CString Mes;
		Mes.Format(L"系统将生成%d×%d的格网数模，生成格网数模可能需要较长时间，是否继续？", RowSum, ColSum);
		if (MessageBox(NULL, Mes, L"是否继续？", MB_YESNO) == IDNO)
			return;

		FILE* fpr = NULL;
		ACHAR fname[512];

		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		ACHAR szPath[MAX_PATH];
		ACHAR TempCurTinName[80] = L"";
		CString CurTinName;
		_stprintf(szPath, L"%s\\PMAP\\CurTIN", Cworkdir);
		FILE* pFile = _wfopen(szPath, L"r");
		if (pFile)
		{
			fwscanf(pFile, L"%s", TempCurTinName);
			CurTinName = TempCurTinName;
			fclose(pFile);
		}
		else
			CurTinName.Empty();

		ads_printf(L"正在导入数模数据文件,请稍候...\n");
		if (CurTinName.IsEmpty())
			_stprintf(fname, L"%s\\PMAP\\%s.asc", Cworkdir, Pro);
		else
			//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);   
			_stprintf(fname, L"%s\\PMAP\\%s.asc.%s", Cworkdir, Pro, CurTinName); //zy-测试


		//_stprintf(fname, L"%s\\PMAP\\%s.asc", Cworkdir, Pro);
		//AfxMessageBox(fname);
		if ((fpr = _wfopen(fname, L"w")) == NULL)
		{
			CString Mes;
			Mes.Format(L"无法创建%s文件", fname);
			AfxMessageBox(Mes);
			return;
		}
		fwprintf(fpr, L"ncols\t%d\n", ColSum);
		fwprintf(fpr, L"nrows\t%d\n", RowSum);
		fwprintf(fpr, L"xllcorner\t%lf\n", ptriDTM_p->LeftLowPt[1]);
		fwprintf(fpr, L"yllcorner\t%lf\n", ptriDTM_p->LeftLowPt[0]);
		fwprintf(fpr, L"cellsize\t%lf\n", CellSize);
		fwprintf(fpr, L"nodata_value\t%lf\n", NoUseH);

		acedSetStatusBarProgressMeter(L"正在生成格网数模数据", 0, 100);
		int Pos = 0;
		for (int i = RowSum; i > -1; i--)
		{
			bool IsFindStart = false;//找到该行上具有高程的起始列号
			double PreH = NoUseH;
			for (int j = 0; j < ColSum; j++)
			{
				Pt[1] = ptriDTM_p->LeftLowPt[1] + j * CellSize;
				Pt[0] = ptriDTM_p->LeftLowPt[0] + i * CellSize;
				Heigh = ptriDTM_p->interpolate(Pt);
				if (IsFindStart)
				{
					if (Heigh < -99.0)
						Heigh = PreH;
					fwprintf(fpr, L"%lf ", Heigh);
					PreH = Heigh;
				}
				else
				{
					if (Heigh > -99.0)
					{
						IsFindStart = true;
						for (int k = 0; k <= j; k++)
							fwprintf(fpr, L"%lf ", Heigh);
					}
					else
						continue;
				}
			}
			if (!IsFindStart)
			{
				for (int k = 0; k < ColSum; k++)
					fwprintf(fpr, L"%lf ", NoUseH);
			}
			Pos = 100.0 * (RowSum - i) / RowSum;
			acedSetStatusBarProgressMeterPos(Pos);
			fwprintf(fpr, L"\n");
		}
		fclose(fpr);
		acedRestoreStatusBar();

		ACHAR mes[512];
		_stprintf(mes, L"格网数模数据计算完成，数据保存到  %s", fname);
		AfxMessageBox(mes);
	}
}

void hengduanmiancanshu()
{
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\Roadhdmdata.exe",SysPath);
	//  runexe(fname);    
	//WinExec((LPCSTR)fname,1); 
	ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);
}

void jisuanchesu()
{
	LoadArx(L"csjy.arx",L"CalSpeedDlg");
}

void chesuchutu()
{
	LoadArx(L"csjy.arx",L"OutSpeedPic");	
}

void chesuchubiao()
{
	LoadArx(L"csjy.arx",L"SpeedExcelDlg");	
}

//实时交互漫游
void shishijiaohumanyou()
{
	//ACHAR fname[256];
	//_stprintf(fname ,L"%s\\T3DView.exe",SysPath);
	//ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);

	//检测方案是否存在平面
	//CString MdbName = BAS_DRAW_FUN::GetCurProMdbPath();
	//JD_CENTER* pLine = g_RoadWorkingDatabase.CreateHorizontalRoad(MdbName);
	//if (!pLine || (pLine && pLine->JdCenArray.GetSize() == 0))	//如果平面线形不存在或者存在但是交点数量为0，那么视为平面线形不存在
	//{
	//	AfxMessageBox(L"本方案尚未设计平面线形，无法生成三维模型！");
	//	return;
	//}

	////写出move文件
	//CString movefile = Cworkdir + "\\move";
	//FILE* fp1;
	//fp1 = _wfopen(movefile, L"w");
	//if (fp1 != NULL)
	//{
	//	fprintf(fp1, "%f,%f", -pLine->JdCenArray[0].E, -pLine->JdCenArray[0].N);
	//	fclose(fp1);
	//}
	//else
	//{
	//	AfxMessageBox(L"写入文件失败！请检查存在工程目录的磁盘是否已满");
	//	return;
	//}

	ACHAR fname[256];
	_stprintf(fname, L"%s\\3d\\Rail3D.exe", SysPath);
	//_stprintf(fname, L"%s\\Rail3D.exe", SysPath);
	ShellExecute(NULL, L"open", fname, NULL, NULL, SW_SHOWNORMAL);
}

