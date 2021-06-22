//
// StdArx.h : include file for ObjectARX/DBX include files
// this file is only included once by your stdafx.h

#if !defined(AFX_STDARX_H__AC323765_A022_469D_85AF_79B9820E1B48__INCLUDED_)
#define AFX_STDARX_H__AC323765_A022_469D_85AF_79B9820E1B48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_ARX_INC_SELECTED
#include "acgi.h"             // graphical definitions
#include "geassign.h"         // AcGe classes
#include "dbents.h"           // text
//}}AFX_ARX_INC_SELECTED

//{{AFX_ARX_INC_OTHERS
//}}AFX_ARX_INC_OTHERS
#include "AdskDMgr.h"         // Utility class for document data
#include "docdata.h"          // Your document specific data

#include "dbsymtb.h" // 2005.11.01 ldf
#include "dbents.h"  // 2005.11.01 ldf
#include "imgdef.h"  // 2005.10.31 ldf
#include "imgent.h"  // 2005.11.01 ldf
#include "acedads.h" // 2005.11.01 ldf
#include "ARXBaseDraw.h" // 2005.11.01 ldf

// Declare it as an extern here so that it
// becomes available in all modules
//extern AsdkDataManager<CDocData> DocVars;
//{{AFX_ARX_FUNC
void NewMdfRoad();
void SelMdfRoad();
void SelMdfRoad();
void penglihuiYYLLINK();
void penglihuiYYCLINK();
void penglihuiYYSLINK();
void penglihuiLOCATIONTOOL();
void penglihuiPMENUBAR();
void penglihuitest();
void penglihuiJCDRAWPM();
void DRAWGRID();
void DRAWZDM();
void AJUSTAXLE();
void ADDAXLE();
void DELAXLE();
void OUTZDM();
void penglihuiTRANJD();
void penglihuiINPUTJD();
void penglihuiADDZXY();
void penglihuiSAVEPMDBS();
void penglihuiOPENPMDBSBYJD();
void penglihuiOPENPMDBSBYXY();
void penglihuiJCADDDL();
void penglihuiJCADDJD();
void penglihuiJCDELJD();
void penglihuiJCEDITJD();
void penglihuiJCDELDL();
void penglihuiJCEXSJ();
void penglihuiJCPROJPOINT();
void penglihuiJCADDXL();
void penglihuiJCDELFLD();
void penglihuiJCDLB();
void JCZMDLB();
void penglihuiJCJDTDFS();
void penglihuiJCAUTOPM();
void penglihuiJCAUTOTK();
void penglihuiJCPMDRAWSET();
void penglihuiJCBIAODING();
void penglihuihuhu_cc_gg();
void SetLMK();
void GetLMK();
void EditLMK();
void DrawLmx();
void SetCGBZ();
void SetJKBZ();
void CalBD();
void CalCGFD();
void CXCGFD();
void ZJBSCD();
void DrawPZX();
void PPCD();
void RAutoDesign();
void ZDCT();
void GJCT();

void ZDJP();
void pinputzmfainputzmfa();//倒入纵面方案
void pzm_zdpmfazm_zdpmfa();//纵面指定平面方案
void SetLevelCtrl();	//设置高程控制点
void SetJPInfor();	//设置接坡信息
void SetZMBZ();//设置纵断面标准
void ptestdmx();//点绘地面线
void BxDmx();
void INBXDMX();
void SaveBXDmx();//保存边线地面线
void peditdmdeditdmd();
void pPickZdmDmxPickZdmDmx();//采集地面线
void pINITZDMINITZDM();//纵面初始化
void psavezdmprjsavezdmprj();//保存纵面方案
void pCalBriCalBri();//自动设桥
void pdelbridelbri();
void paddBriaddBri();
void pEditBriEditBri();
void pCALTUNCALTUN();
void pdeltundeltun();
void paddtunaddtun();
void pEDITTunEDITTun();
void pMdbInTunMdbInTun();
void pInputBriInputBri();
void pMdyPDMdyPD();
void raildrawpmzttdrawpmztt();
void raildelzdkdelzdk();
void railaddzdkaddzdk();
void railsavezdksavezdk();
void raileditzdkeditzdk();
void railchangezdkchangezdk();
//}}AFX_ARX_FUNC
void penglihuiTRANCENTER();
void penglihuiZXYTOXL();
void pPMLOC();
void penglihuiXLLINK();
void penglihuiEDITJDCEN();
void penglihuiinputjdcenter();
void penglihuiinputxy();
void penglihuiPZOOM();
void penglihuiDRAWQXYS();
void DRAWXYYSB();
void penglihuiZYLINK();
void penglihuiJCREVERSE();
void penglihuiGQCIR();
void CALSTA();

void GetDmx();
void ModifyDmx();
void HdmAutoDes();
void INHDM();
void EDITHDMBP();
void GUTINTERACT();
void SAVEHDM();
void DRAWHDMT();
void OUTHDM();
void AdjustGouPD();//ph

void OutZZZB();
void OutZXQXZJ();
void OutZPSQX();
void OutZZYDZB();
void OutLJSJB();
void OutBGPSG();
void OutTSFJSB();
void OutGCSLB();
void OutDYT();
void OutMGLTSFB();
void OutLXTWB();
void OutQBTB();
void OutWYNB();

void penglihuibl();
void penglihuiBA();
void penglihuibs();
void penglihuiCXJ();
void penglihuic();

void AjustSta();
void pDX3D();
void pLX3D();
void GenPers();

//大临
void pInitLw();
void pSysmLw();
void pZtopLw();

//辅助工具
void DELAYER();
void RUBBER();
void pPMLCTOZB();
void pPMZBTOLC();
void CXLMBG();
void CXYDAREA();
void CalXYJD();
void VIZTOOL();
void RIBBON();
//建立数模
void tiqushuju();
void chulishuju();
void jianlishumo();
void Changeshumo();
void xianshishumo();
void TINConvertASC();

//横断面
void hengduanmiancanshu();

//车速检测
void jisuanchesu();
void chesuchutu();
void chesuchubiao();

//实时交互漫游
void shishijiaohumanyou();


//数据转换
void WDtoDR();
void DRtoWD();
void WDdmxtoDR();
void DRtoWDdmx();

void CheckPro();
void CalNose();

void prmenurmenu();

void ROADANDRAILRPMLOC(void);
void ROADANDRAILINXY(void);
void ROADANDRAILXLJD(void);;


//工具条

void darcychecka();
void  darcydarcyxmgl();
void darcydarcyjlsm();  
void darcydarcyjlsmout();
void darcydarcyxmglout();

void darcysldesignsldesign();//平面设计中的单线设计
void darcysldesignfagl();//平面设计中的方案管理
void darcydldesigndldesign();//二线设计
void darcydldes();//断链设计

void darcyythyth();//一体化
void darcypddesignpddesign();//坡度设计
void darcybridgedesignbridgedes();//桥梁设计
void darcysddesignsddesign();//隧道设计
void  darcyzdmdeszdmdes(); //纵断面设计	
void  darcyhdmdes(); //横断面设计	
void  darcyyddes(); //用地设计

void darcyplanplan();        //平面图
void darcyverticalsectionver_section();//纵断面

//
void DarcySLDESIGNDXSJOUT();
void DarcyFAGLOUT();
void DarcyDLDESIGNDLSJOUT();
void DarcyPDDESIGNPDSJOUT();





void DarcyBRIDGEDESIGNQLSJOUT();
void DarcySDDESIGNSDDESIGNOUT();
void DarcyPLANPMTOUT();
void DarcyZDMDESZDMTOUT();
void DarcyYTHDLSZOUT();
void darcyverticalsectionsingle_culvert();

void DarcyDLDESIGNPMYTHOUT();
void DarcyDLDESIGNZMYTHSJOUT();

void DarcyHDMDESOUT();
void DarcyYDOUT();
void CreatePrj();
void OpenPrj();
void InMdb();
void ppickdatdapickdatda();
void pdealdatadealdata();
void pcreatetincreatetin();
void pdiaplaydtmdiaplaydtm();
void pSetCgBz();//设置超高标准
void pSetJkBz();//设置加宽标准
void ZmSJBZ();

void HdmJBCS();
void HdmJBCS2();//Ribbon菜单的横断面基本参数
void DrawKZBG();
void JMBX();
void DrawDBZX();
void DrawDBCIR();
void DrawDBHX();
void DrawDBBZ();
void XGDBBZ();
void PmGuide();

void darcychecka(void);
void floatXMGL(void);
void floatJLSM(void);
void floatPMSJ(void);
void floatJDMS(void);
void floatXYMS(void);
void floatZDMSJ(void);
void floatPDSJ(void);
void floatQLSZ(void);
void floatSDSZ(void);
void floatHDMSJ(void);
void floatYDSJ(void);
void floatPMT(void);
void floatDBSJT(void);
void floatZHDMT(void);
void floatBGSC(void);

//////////////////////////////////////////////////////////////////////////
//改移道路
void EditAllRoadS();//编辑所有道路信息
void OutAllRoadSTab();//输出所有道路信息表格
void OutQXJKCGTab();//输出曲线超过加宽表
void ADDPMTK();//增加图框
void OPENPMTK();//导入图框
void SAVEPMTK();//保存图框
void TRIMPMT();//自动成图
void DRAWZDMForPZT();//在平纵图中绘纵断面
void EDITZDK_Road();//编辑占地宽
void DeleteJZX();//删除夹直线
// TODO: Here you can add your own includes / declarations
#endif 
