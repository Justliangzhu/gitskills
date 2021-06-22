//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "StdArx.h"
#include "MyBar.h"
#include "DirdlgDlg.h"
//#include "FABXZBEXCEL.h"
//#include "AboutDlg.h"
//#include "Threedimensional.h"
//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  SYSTEMNAME L"VizRail"


//#define  SYSTEMNAME "NRDAS"
//#define  SYSTEMNAME "RLDVS"

extern void rdtm();
extern char PmOrZm;//标识当前命令为平面或者纵面
extern bool CheckDog();//单机版BOOL bWriteMem
extern bool CheckNetDog1();//网络版
//extern bool CheckDogByDLL();
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CMyBar, CAcUiDockControlBar)

	BEGIN_MESSAGE_MAP(CMyBar, CAcUiDockControlBar)
		//{{AFX_MSG_MAP(CMyBar)
		ON_WM_CREATE()
		//改移道路

		//////////////////////////////////////////////////////////////////////////
		//绑定菜单命令

		//项目管理
		ON_COMMAND(ID_MENUITEM_NEWXM_GY, OnMENUITEM_NEWXM_GY)//新建道路
		ON_COMMAND(ID_MENUITEM_BDXM_GY, OnMENUITEM_BDXM_GY)//选择道路
		ON_COMMAND(ID_MENUITEM_DLHZ_GY, OnMENUITEM_DLHZ_GY)//道路汇总表
		ON_COMMAND(ID_MENUITEM_DrawAllRoadsTab_GY, OnMENUITEM_DrawAllRoadsTab_GY)//输出道路汇总表

		//平面
		ON_COMMAND(ID_MENUITEM_INPUTDBSBYJD_GY, OnMENUITEMINPUTDBSBYJD_GY)
		ON_COMMAND(ID_MENUITEM_INPUTJDCEN_GY, OnMENUITEMINPUTJDCEN_GY)
		ON_COMMAND(ID_MENUITEM_ZXYTOXL_GY, OnMENUITEMZXYTOXL_GY)
		ON_COMMAND(ID_MENUITEM_DELJZX_GY, OnMENUITEMDELJZX_GY)
		ON_COMMAND(ID_MENUITEM_ADDJD_GY, OnMENUITEMADDJD_GY)
		ON_COMMAND(ID_MENUITEM_DELJD_GY, OnMENUITEMDELJD_GY)
		ON_COMMAND(ID_MENUITEM_EDITDL_GY, OnMENUITEMEDITDL_GY)
		ON_COMMAND(ID_MENUITEM_ADDDL_GY, OnMENUITEMADDDL_GY)
		ON_COMMAND(ID_MENUITEM_DELDL_GY, OnMENUITEMDELDL_GY)
		ON_COMMAND(ID_MENUITEM_LMKD_GY, OnMENUITEMLMKD_GY)
		ON_COMMAND(ID_MENUITEM_LIANXUDINGXIAN_GY, OnMENUITEMLIANXUDINGXIAN_GY)
		ON_COMMAND(ID_MENUITEM_LOCATIONTOOL2_GY, OnMENUITEMLOCATIONTOOL2_GY)
		ON_COMMAND(ID_MENUITEM_REVERSE_GY, OnMENUITEMREVERSE_GY)
		ON_COMMAND(ID_MENUITEM_LINKXL_GY, OnMENUITEMLINKXL_GY)
		ON_COMMAND(ID_MENUITEM_OUTPUTDBS_GY, OnMENUITEMOUTPUTDBS_GY)
		ON_COMMAND(ID_MENUITEM_RAUTODES_GY, OnMENUITEMRAUTODES_GY)
		

		//纵面
		ON_COMMAND(ID_MENUITEMsetZmCs_GY	 ,OnMENUITEMsetZmCs_GY)	//基本参;
		ON_COMMAND(ID_MENUITEMzmzdpmfa_GY	 ,OnMENUITEMzmzdpmfa_GY)	//选择平面
		ON_COMMAND(ID_MENUITEMsetSJBZ_GY	 ,OnMENUITEMsetSJBZ_GY)	//设计标准
		ON_COMMAND(ID_MENUITEMZDMFA_GY	 ,OnMENUITEMZDMFA_GY)	//导入纵面
		ON_COMMAND(ID_MENUITEMSETCTRLH_GY, OnMENUITEMSETCTRLH_GY)	//高程控制点
		ON_COMMAND(ID_MENUITEMSETJPInfor_GY, OnMENUITEMSETJPInfor_GY)	//接坡信息
		ON_COMMAND(ID_MENUITEM_ZMEDITDL_GY	 ,OnMENUITEM_ZMEDITDL_GY)	//编辑断链
		ON_COMMAND(ID_MENUITEM_PickDmx_GY 	 ,OnMENUITEM_PickDmx_GY) 	//屏幕采集地面线
		ON_COMMAND(ID_MENUITEM_DMX_GY	 ,OnMENUITEM_DMX_GY)	//数模内插地面线
		ON_COMMAND(ID_MENUITEMEditDMD_GY	 ,OnMENUITEMEditDMD_GY)	//列表编辑地面线
		ON_COMMAND(ID_MENUITEM_AutoSlopeData_GY, OnMENUITEM_AutoSlopeData_GY)	//一键纵坡
		ON_COMMAND(ID_MENUITEM_PDX_GY	 ,OnMENUITEM_PDX_GY)	//直线转坡度线
		ON_COMMAND(ID_MENUITEM_LPSJ_GY	 ,OnMENUITEM_LPSJ_GY)	//拉坡设计
		ON_COMMAND(ID_MENUITEM_MdyPd_GY	 ,OnMENUITEM_MdyPd_GY)	//调整坡度
		ON_COMMAND(ID_MENUITEM_ZPSJB_GY	 ,OnMENUITEM_ZPSJB_GY)	//"编辑坡度
		ON_COMMAND(ID_MENUITEM_addBpd_GY	 ,OnMENUITEM_addBpd_GY)	//增加变坡点
		ON_COMMAND(ID_MENUITEM_DelBpd_GY	 ,OnMENUITEM_DelBpd_GY)	//删除变坡点
		ON_COMMAND(ID_MENUITEM_LinkBD_GY	 ,OnMENUITEM_LinkBD_GY)	//坡段连接
		ON_COMMAND(ID_MENUITEM_CalBri_GY	 ,OnMENUITEM_CalBri_GY)	//自动设桥
		ON_COMMAND(ID_MENUITEM_InputBri_GY	 ,OnMENUITEM_InputBri_GY)	//导入桥梁 
		ON_COMMAND(ID_MENUITEM_DelBri_GY	 ,OnMENUITEM_DelBri_GY)	//删除桥梁
		ON_COMMAND(ID_MENUITEM_AddBri_GY	 ,OnMENUITEM_AddBri_GY)	//增加桥梁
		ON_COMMAND(ID_MENUITEM_EditBri_GY	 ,OnMENUITEM_EditBri_GY)	//列表编辑
		ON_COMMAND(ID_MENUITEM_CalTun_GY	 ,OnMENUITEM_CalTun_GY)	//自动设隧
		ON_COMMAND(ID_MENUITEM_InputTun_GY	 ,OnMENUITEM_InputTun_GY)	//导入隧道
		ON_COMMAND(ID_MENUITEM_Deltun_GY	 ,OnMENUITEM_Deltun_GY)	//删除隧道
		ON_COMMAND(ID_MENUITEM_addtun_GY	 ,OnMENUITEM_addtun_GY)	//增加隧道
		ON_COMMAND(ID_MENUITEM_EditTun_GY	 ,OnMENUITEM_EditTun_GY)	//列表编辑
		ON_COMMAND(ID_MENUITEMSAVEZDM_GY	 ,OnMENUITEMSAVEZDM_GY)	//保存方案


		//横断面
		ON_COMMAND(ID_MENUITEMsetHmCs_GY	 ,OnMENUITEMsetHmCs_GY)	//基本参数
		ON_COMMAND(ID_MENUITEM_CalSta_GY	 ,OnMENUITEM_CalSta_GY)	//桩号序列
		ON_COMMAND(ID_MENUITEM_GetDmx_GY	 ,OnMENUITEM_GetDmx_GY)	//提取地面线
		ON_COMMAND(ID_MENUITEM_ModifyDmx_GY		,OnMENUITEM_ModifyDmx_GY)	//修改地面线
		ON_COMMAND(ID_MENUITEM_HdmAutoDes_GY	,OnMENUITEM_HdmAutoDes_GY)	//自动设计
		ON_COMMAND(ID_MENUITEM_InputHdm_GY	 ,OnMENUITEM_InputHdm_GY)	//导入横断面
		ON_COMMAND(ID_MENUITEM_ChangBP_GY	 ,OnMENUITEM_ChangBP_GY)	//修改边坡
		ON_COMMAND(ID_MENUITEM_GUTINTERACT_GY, OnMENUITEM_GutInteract_GY)	//水沟交互
		ON_COMMAND(ID_MENUITEM_GOUPD_GY	 ,OnMENUITEM_GOUPD_GY)	//沟底纵坡
		ON_COMMAND(ID_MENUITEM_SAVEHDM_GY	 ,OnMENUITEM_SAVEHDM_GY)	//保存方案

		//图形输出
		ON_COMMAND(ID_MENUITEM_EDITZDK_GY	 ,OnMENUITEMEDITZDK_GY)	//保存方案
		ON_COMMAND(ID_MENUITEM_ZDKDES_GY	 ,OnMENUITEMZDKDES_GY)	//保存方案
		ON_COMMAND(ID_MENUITEM_DelYDZ_GY, OnMENUITEMDelYDZ_GY)
		ON_COMMAND(ID_MENUITEM_ADDYDZ_GY, OnMenuitemAddydz_GY)
		ON_COMMAND(ID_MENUITEM_CHANGEZDK_GY	 ,OnMENUITEMCHANGEZDK_GY)	//保存方案
		ON_COMMAND(ID_MENUITEM_SAVEZDK_GY	 ,OnMENUITEMSAVEZDK_GY)	//保存方案
		//平面图

		ON_COMMAND(ID_MENUITEM_DRAWPM_GY	 ,OnMENUITEM_DRAWPM_GY)	//绘平面图
		ON_COMMAND(ID_MENUITEM_DRAWGRID_GY	 ,OnMENUITEM_DRAWGRID_GY)//绘经纬网格
		ON_COMMAND(ID_MENUITEM_CBQXYS_GY	 ,OnMENUITEM_CBQXYS_GY)	//绘曲线要素
		ON_COMMAND(ID_MENUITEM_HXYYSB_GY	 ,OnMENUITEM_HXYYSB_GY)	//绘线元要素表
		ON_COMMAND(ID_MENUITEM_OUTPM_GY	 ,OnMENUITEM_OUTPM_GY)	//分幅输出

		//纵断面图
		ON_COMMAND(ID_MENUITEM_DRAWZDM_GY	 ,OnMENUITEM_DRAWZDM_GY)	//绘纵面图
		ON_COMMAND(ID_MENUITEM_AJUSTAXLE_GY	 ,OnMENUITEM_AJUSTAXLE_GY)	//调整断高
		ON_COMMAND(ID_MENUITEM_ADDAXLE_GY	 ,OnMENUITEM_ADDAXLE_GY)	//增加断轴
		ON_COMMAND(ID_MENUITEM_DELAXLE_GY	 ,OnMENUITEM_DELAXLE_GY)	//删除断轴
		ON_COMMAND(ID_MENUITEM_OUTZDM_GY	 ,OnMENUITEM_OUTZDM_GY)		//输出纵面图

		//平纵断面图
		ON_COMMAND(ID_MENUITEM_PMBZTK_ROAD, OnMENUITEM_PMBZTK_ROAD)	//新建图框
		ON_COMMAND(ID_MENUITEM_INTK__ROAD, OnMENUITEM_INTK__ROAD)	//导入图框
		ON_COMMAND(ID_MENUITEM_SAVETK__ROAD, OnMENUITEM_SAVETK__ROAD)	//保存图框
		ON_COMMAND(ID_MENUITEM_PMCutTu__ROAD, OnMENUITEM_PMCutTu__ROAD)	//自动成图

		ON_COMMAND(ID_MENUITEM_DRAWHDM_GY	 ,OnMENUITEM_DRAWHDM_GY)	//绘横断面图
		ON_COMMAND(ID_MENUITEM_OUTHDM_GY	 ,OnMENUITEM_OUTHDM_GY)	//输出横断面图

		//表格输出
		ON_COMMAND(ID_MENUITEM_ZZYDZBB_GY	 ,OnMENUITEM_ZZYDZBB_GY)	//逐桩用地坐标表
		ON_COMMAND(ID_MENUITEM_TSFJSB_GY, OnMENUITEM_TSFJSB_GY)	//土石方数量计算表
		ON_COMMAND(ID_MENUITEM_GCSLB_GY, OnMENUITEM_GCSLB_GY)	////工程数量表
		ON_COMMAND(ID_MENUITEM_DYT_GY, OnMENUITEM_DYT_GY)	////路基大样图

		//三维模型拼合计算
		ON_COMMAND(ID_MENUITEM_DigHole_GY	 ,OnMENUITEM_DigHole_GY)	//三维模型拼合计算
//#endif
		
//#ifdef DIGIROAD
	ON_COMMAND(ID_MENUITEM_INPUTDBSBYJD, OnMenuitemINPUTDBSBYJD)//从数据库读取
	ON_COMMAND(ID_MENUITEM_INPUTDBSBYXY, OnMenuitemINPUTDBSBYXY)//从数据库读取
	ON_COMMAND(ID_MENUITEM_OUTPUTDBS, OnMenuitemOUTPUTDBS)//保存到数据库
	ON_COMMAND(ID_MENUITEM_DATA, OnMenuitemDATA)//保存到数据库

	ON_COMMAND(ID_MENUITEM_LOCATIONTOOL1, OnMenuitemLocationtool1)
	ON_COMMAND(ID_MENUITEM_LOCATIONTOOL2, OnMenuitemLocationtool2)
	ON_COMMAND(ID_MENUITEM_ZJBSCDSJ,OnMenuitemZJBSCD)
	ON_COMMAND(ID_MENUITEM_INPUTJDCEN, OnMenuitemInPutJDCen)
	ON_COMMAND(ID_MENUITEM_INPUTXY, OnMenuitemInPutXY)
	ON_COMMAND(ID_MENUITEM_DRAWPM, OnMenuitemDrawPm)
	ON_COMMAND(ID_MENUITEM_DRAWGRID, OnMenuitemDrawGRID)
	ON_COMMAND(ID_MENUITEM_DRAWZDM, OnMenuitemDrawZdm)	
	ON_COMMAND(ID_MENUITEM_AJUSTAXLE, OnMenuitemAJUSTAXLE)
	ON_COMMAND(ID_MENUITEM_ADDAXLE, OnMenuitemAddAXLE)
	ON_COMMAND(ID_MENUITEM_DELAXLE, OnMenuitemDelAXLE)
	ON_COMMAND(ID_MENUITEM_OUTZDM, OnMenuitemOutZdm)	
	ON_COMMAND(ID_MENUITEM_REVERSE, OnMenuitemReverse)
	ON_COMMAND(ID_MENUITEM_TRANJD, OnMenuitemTranJD)
	ON_COMMAND(ID_MENUITEM_TRANCENTER, OnMenuitemTranCenter)
	ON_COMMAND(ID_MENUITEM_EDITJDCEN, OnMenuitemEditJD)
	ON_COMMAND(ID_MENUITEM_LINKXL, OnMenuitemLINKXL)
	ON_COMMAND(ID_MENUITEM_ZXYTOXL, OnMenuitemZXYTOXL)//主线元转线路
	ON_COMMAND(ID_MENUITEM_NEWXM, OnMenuitemNEWXM)//新建项目
	ON_COMMAND(ID_MENUITEM_BDXM, OnMenuitemBDXM)//打开项目
	ON_COMMAND(ID_MENUITEM_ZQZJB, OnMenuitemZQZJB)//直曲转角表
	ON_COMMAND(ID_MENUITEM_ZDQXB, OnMenuitemZDQXB)//匝道曲线表	
	ON_COMMAND(ID_MENUITEM_ZDZDZBB, OnMenuitemZDZDZBB)//匝道主点坐标表
	ON_COMMAND(ID_MENUITEM_ZDZZZBB, OnMenuitemZDZZZBB)//匝道主桩坐标表
	ON_COMMAND(ID_MENUITEM_ADDDL, OnMenuitemADDDL)//增加断链
	ON_COMMAND(ID_MENUITEM_DELDL, OnMenuitemDELDL)//删除断链
	ON_COMMAND(ID_MENUITEM_INPUTEXJDCEN, OnMenuitemINPUTEXJDCEN)//录入二线平面数据
	ON_COMMAND(ID_MENUITEM_QIECHUIXIAN, OnMenuitemQIECHUIXIAN)//切线垂线工具
	ON_COMMAND(ID_MENUITEM_LIANXUDINGXIAN, OnMenuitemLIANXUDINGXIAN)//连续定线
	ON_COMMAND(ID_MENUITEM_DELFLD, OnMenuitemDELFLD)//"删除分离段"	
	ON_COMMAND(ID_MENUITEM_EDITDL, OnMenuitemEDITDL)//编辑断链
	ON_COMMAND(ID_MENUITEM_ZMEDITDL, OnMenuitemZMEDITDL)//编辑断链
	ON_COMMAND(ID_MENUITEM_JDTDFS, OnMenuitemJDTDFS)//设置交点拖动方式
	ON_COMMAND(ID_MENUITEM_AUTOTK, OnMenuitemAUTOTK)//自动布图框
	ON_COMMAND(ID_MENUITEM_OUTPM, OnMenuitemOUTPM)//分幅输出平面图
	ON_COMMAND(ID_MENUITEM_CBQXYS, OnMenuitemDrawCBQXYS)//曲线要素图
	ON_COMMAND(ID_MENUITEM_HXYYSB, OnMenuitemDrawXYYSB)//线元要数图
	ON_COMMAND(ID_MENUITEM_DBDRAWZX,OnMenuitemDrawDBZX)//绘端部中线
	ON_COMMAND(ID_MENUITEM_DBDRAWBD, OnMenuitemDrawDBBDCIR)
	ON_COMMAND(ID_MENUITEM_DBDRAWHX, OnMenuitemDrawDBHX)
	ON_COMMAND(ID_MENUITEM_DBBZ, OnMenuitemDBBZ)
	ON_COMMAND(ID_MENUITEM_XGDB, OnMenuitemXGDB)

	ON_COMMAND(ID_MENUITEM_PMTSET, OnMenuitemPMTSET)//参数设置
	ON_COMMAND(ID_MENUITEM_LMKD, OnMenuitemLMKD)//add
	ON_COMMAND(ID_MENUITEM_DRAWLMX, OnMenuitemLMX)//绘路面线/add
	ON_COMMAND(ID_MENUITEM_DRAWPZX, OnMenuitemPZX)
	ON_COMMAND(ID_MENUITEM_GETLMKTOFILE, OnMenuitemGetLmkToFile)//add
	ON_COMMAND(ID_MENUITEM_EDITLMK, OnMenuitemEditLmk)//add
	ON_COMMAND(ID_MENUITEM_PPCD, OnMenuitemPPCD)//爬坡车道
	ON_COMMAND(ID_MENUITEM_CALBD,OnMenuitemCalBD)//计算鼻端
	ON_COMMAND(ID_MENUITEM_OUTHDM, OnMenuitemOutHdm)	
	ON_COMMAND(ID_MENUITEM_DRAWHDM, OnMenuitemDrawHdm)	
	ON_COMMAND(ID_MENUITEM_JMBX,OnMenuitemJMBX)
	ON_COMMAND(ID_MENUITEM_ZDCT, OnMenuitemZDCT)
	ON_COMMAND(ID_MENUITEM_DRAWGUTTER, OnMenuitemDRAWGUTTER)
			
	//建立数模
	ON_COMMAND(ID_MENUITEMtqsj, OnMENUITEMtqsj)
	ON_COMMAND(ID_MENUITEMdealdata, OnMENUITEMdealdata)
	ON_COMMAND(ID_MENUITEMcreateTin, OnMENUITEMcreateTin)
	ON_COMMAND(ID_MENUITEMChangeTin, OnMENUITEMChangeTin)
			
	ON_COMMAND(ID_MENUITEMdisplay, OnMENUITEMdisplay)
	ON_COMMAND(ID_MENUITEMTINConvertASC, OnMENUITEMTINConvertASC)

	//纵断面
	ON_COMMAND(ID_MENUITEMZDMFA, OnMenuitemzdmfa)//倒入纵断面方案
	ON_COMMAND(ID_MENUITEM_PDX, OnMenuitemPdx)//直线转纵断面设计线
	ON_COMMAND(ID_MENUITEMzmzdpmfa, OnMENUITEMzmzdpmfa)//指定平面方案
	ON_COMMAND(ID_MENUITEM_DMX, OnMenuitemDMX)//点绘地面线
	ON_COMMAND(ID_MENUITEM_BXDMX,OnMenuitemBXDMX)//点绘地面线
	ON_COMMAND(ID_MENUITEM_INBXDMX,OnMenuitemINBXDMX)
	ON_COMMAND(ID_MENUITEM_SAVEBXDMX,OnMenuitemSAVEBXDMX)//保存边线地面线
	ON_COMMAND(ID_MENUITEM_ZDJP, OnMenuitemZDJP)//楂道接坡
	ON_COMMAND(ID_MENUITEMKZBG, OnMenuitemKZBG)//绘控制标高
	ON_COMMAND(ID_MENUITEMsetZmCs, OnMENUITEMsetZmCs)//设定纵面参数
	ON_COMMAND(ID_MENUITEMsetSJBZ, OnMENUITEMsetZMSJBZ)//设定设计标准
	ON_COMMAND(ID_MENUITEM_LPSJ, OnMenuitemLpsj)//拉坡设计
	ON_COMMAND(ID_MENUITEM_MdyPd, OnMENUITEMMdyPd)//调整坡度
	ON_COMMAND(ID_MENUITEM_ZPSJB, OnMenuitemZpsjb)//编辑坡度
	ON_COMMAND(ID_MENUITEM_addBpd, OnMENUITEMaddBpd)
	ON_COMMAND(ID_MENUITEM_DelBpd, OnMENUITEMDelBpd)
	ON_COMMAND(ID_MENUITEM_LinkBD, OnMENUITEMLinkBD)//坡段连接
	ON_COMMAND(ID_MENUITEMSAVEZDM, OnMenuitemsavezdm)//保存纵面方案
	ON_COMMAND(ID_MENUITEM_CalBri, OnMENUITEMCalBri)//自动设桥
	ON_COMMAND(ID_MENUITEM_DelBri, OnMENUITEMDelBri)
	ON_COMMAND(ID_MENUITEM_AddBri, OnMENUITEMAddBri)
	ON_COMMAND(ID_MENUITEM_EditBri, OnMENUITEMEditBri)
	ON_COMMAND(ID_MENUITEM_InputBri, OnMENUITEMInputBri)
	ON_COMMAND(ID_MENUITEM_PickDmx, OnMENUITEMPickDmx)//屏幕采集地面线
	ON_COMMAND(ID_MENUITEMEditDMD, OnMENUITEMEditDMD)//编辑地面点

	ON_COMMAND(ID_MENUITEM_CalTun, OnMENUITEMCalTun)	
	ON_COMMAND(ID_MENUITEM_Deltun, OnMENUITEMDeltun)
	ON_COMMAND(ID_MENUITEM_addtun, OnMENUITEMaddtun)
	ON_COMMAND(ID_MENUITEM_EditTun, OnMENUITEMEditTun)
	ON_COMMAND(ID_MENUITEM_InputTun, OnMENUITEMInputTun)

	ON_COMMAND(ID_MENUITEM_SETCGBZ, OnMENUITEMSETCGBZ)//设置超高标准
	ON_COMMAND(ID_MENUITEM_SETJKBZ, OnMENUITEMSETJKBZ)//设置加宽标准	
	ON_COMMAND(ID_MENUITEM_CALCGFD, OnMENUITEMCALCGFD)//计算超高分段
	ON_COMMAND(ID_MENUITEM_CXCGFD, OnMENUITEMCXCGFD)//查询超高分段

	//横断面设计
	ON_COMMAND(ID_MENUITEMsetHmCs, OnMENUITEMsetHmCs)//设定横断面参数
	ON_COMMAND(ID_MENUITEM_CalSta, OnMENUITEMCalSta)//计算桩号序列
	ON_COMMAND(ID_MENUITEM_CalNose, OnMENUITEMCalNose)//计算桩号序列

	ON_COMMAND(ID_MENUITEM_GetDmx, OnMENUITEMGetDmx)//提取地面线
	ON_COMMAND(ID_MENUITEM_HdmAutoDes, OnMENUITEMHdmAutoDes)//横断面自动设计
	ON_COMMAND(ID_MENUITEM_InputHdm, OnMENUITEMInputHdm)//导入横断面
	ON_COMMAND(ID_MENUITEM_ChangBP, OnMENUITEMChangeBp)//导入横断面
	ON_COMMAND(ID_MENUITEM_SAVEHDM, OnMENUITEMSAVEHDM)//导入横断面
	ON_COMMAND(ID_MENUITEM_GOUPD, OnMENUITEMGOUPD)//导入横断面

	//用地设计	
	ON_COMMAND(ID_MENUITEM_InputYD, OnMENUITEMInputYD)
	ON_COMMAND(ID_MENUITEM_DelYDZ, OnMENUITEMDelYDZ)
	ON_COMMAND(ID_MENUITEM_ADDYDZ, OnMenuitemAddydz)
	ON_COMMAND(ID_MENUITEM_EDITYDDATA, OnMenuitemEdityddata)
	ON_COMMAND(ID_MENUITEM_CHANGEYDZ, OnMenuitemChangeydz)
	ON_COMMAND(ID_MENUITEM_SAVEYDDATA, OnMenuitemSaveyddata)
	//车速检测
	ON_COMMAND(ID_MENUITEMjscs, OnMENUITEMjscs)
	ON_COMMAND(ID_MENUITEMcsct, OnMENUITEMcsct)
	ON_COMMAND(ID_MENUITEMcscb, OnMENUITEMcscb)


	//表格输出
	ON_COMMAND(ID_MENUITEM_ZXQXZJB, OnMenuitemZXQXZJB)
	ON_COMMAND(ID_MENUITEM_ZZZBB, OnMenuitemZZZBB)
	ON_COMMAND(ID_MENUITEM_ZPSQXB, OnMenuitemZPSQXB)
	ON_COMMAND(ID_MENUITEM_ZZYDZBB, OnMenuitemZZYDZBB)
	ON_COMMAND(ID_MENUITEM_TSFJSB, OnMenuitemTSFJSB)
	ON_COMMAND(ID_MENUITEM_GCSLB, OnMenuitemGCSLB)
	ON_COMMAND(ID_MENUITEM_DYT, OnMenuitemDYT)
	ON_COMMAND(ID_MENUITEM_MGLTSFB, OnMenuitemMGLTSFB)
	ON_COMMAND(ID_MENUITEM_LXTWSJB, OnMenuitemLXTWSJB)
	ON_COMMAND(ID_MENUITEM_LJSJB, OnMenuitemLJSJB)
	ON_COMMAND(ID_MENUITEM_BGPSG, OnMenuitemBGPSG)
	ON_COMMAND(ID_MENUITEM_QBTTAB, OnMenuitemQBTTAB)
	ON_COMMAND(ID_MENUITEM_WYNTAB, OnMenuitemWYNTAB)

	//三维可视
	ON_COMMAND(ID_MENUITEMdx3d, OnMenuitemDX3D)
	ON_COMMAND(ID_MENUITEMlx3d, OnMenuitemLX3D)
	ON_COMMAND(ID_MENUITEM3DFrameBrowser,OnMenuitemView3D)
	ON_COMMAND(ID_MENUITEM3DPerspective,OnMenuitemPerspective)


	//大临工程
	ON_COMMAND(ID_MENUITEM_INITLW, OnMenuitemINITLW)
	ON_COMMAND(ID_MENUITEM_SYSMLW, OnMenuitemSYSMLW)
	ON_COMMAND(ID_MENUITEM_ZTOPLW, OnMenuitemZTOPLW)
	//辅助工具
	ON_COMMAND(ID_MENUITEM_PMGUIDE, OnMenuitemPmGuide)
	ON_COMMAND(ID_MENUITEM_DELAYER, OnMenuitemDELAYER)
	ON_COMMAND(ID_MENUITEM_RUBBER, OnMenuitemRUBBER)
	ON_COMMAND(ID_MENUITEM_LCZZB, OnMenuitemLczzb)
	ON_COMMAND(ID_MENUITEM_ZBZLC, OnMenuitemZbzlc)
	ON_COMMAND(ID_MENUITEM_CXLMBG, OnMenuitemCxlmbg)
	ON_COMMAND(ID_MENUITEM_CXYDAREA,OnMenuitemCxYdArea)
	ON_COMMAND(ID_MENUITEM_FACHECK,OnMenuitemProCheck)
	ON_COMMAND(ID_MENUITEM_XLJD, OnMenuitemXLJD)
	ON_COMMAND(ID_MENUITEM_TOOL, OnMenuitemTOOL)
	ON_COMMAND(ID_MENUITEM_RIBBON, OnMenuitemRIBBON)
	//数据转换
	ON_COMMAND(ID_MENUITEM_WDtoDR, OnMenuitemWDtoDR)
	ON_COMMAND(ID_MENUITEM_DRtoWD, OnMenuitemDRtoWD)
	ON_COMMAND(ID_MENUITEM_WDdmxtoDR, OnMenuitemWDdmxtoDR)
	ON_COMMAND(ID_MENUITEM_DRtoWDdmx, OnMenuitemDRtoWDdmx)

	//帮助
	ON_COMMAND(ID_MENUITEM_HELP, OnMenuitemHELP)
//#endif




		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	//-----------------------------------------------------------------------------
	// CMyBar *pInstance =new CMyBar ;
	// pInstance->Create (acedGetAcadFrame (), "My title bar") ;
	// pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
	// pInstance->RestoreControlBar () ;

	static CLSID clsCMyBar ={ 0xda523cb2, 0x9629, 0x42b2, { 0x93, 0x7c, 0xbd, 0x99, 0x32, 0x49, 0x94, 0x6e } } ;

//#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU| WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX)//zdf

	bool runexe(ACHAR fname[])
	{
		ACHAR cmdline[80];
		STARTUPINFO sb;
		PROCESS_INFORMATION pprocess;  


		_tcscpy(cmdline,L" ");
		//	_stprintf(cmdline,"%s%ld",cmdline,PassWord2);
		sb.cb = sizeof(sb);
		sb.wShowWindow = SW_SHOWNORMAL;
		sb.dwFlags = STARTF_USESHOWWINDOW;
		if(CreateProcess(fname,cmdline,NULL,NULL,FALSE,HIGH_PRIORITY_CLASS,NULL,NULL,&sb,
			&pprocess))
			return true;
		else
			return false;
	}
	//-----------------------------------------------------------------------------
	CMyBar::CMyBar() : CAcUiDockControlBar () {
		//{{AFX_DATA_INIT(CMyBar)
		//}}AFX_DATA_INIT
		m_bKeepFocus = true;
	}


	BOOL CMyBar::Create(CWnd*pParent, LPCTSTR lpszTitle) {
		CString strWndClass ;
		strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;

		CRect rect (0, 0, 100, 20) ;
		if ( !CAcUiDockControlBar::Create (
			strWndClass,
			lpszTitle,
			WS_VISIBLE |WS_CHILD |  WS_CLIPCHILDREN ,
			rect,
			pParent, 0
			)
			)
			return (FALSE) ;
		/*SetBkColor(GetDC()->m_hDC,RGB(0,255,255));*/
		SetToolID (&clsCMyBar) ;

		//----- TODO: Add your code here

		return (TRUE) ;
	}

	int CMyBar::OnCreate (LPCREATESTRUCT lpCreateStruct) {


		if ( CAcUiDockControlBar::OnCreate (lpCreateStruct) == -1 )
			return (-1) ;

		m_wndMenuBar.SetBackColor(BLACK_BRUSH);//(zdf)
		//m_wndMenuBar.SetBackColor(WHITE_BRUSH);//(zdf)
		if (!m_wndMenuBar.Create(WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | CCS_NODIVIDER | CCS_NORESIZE /*|CBRS_SIZE_FIXED*/, 
			CRect(0,0,1,1), this , IDR_MENU1))  //IDC_MENUBAR

			//if (!m_wndMenuBar.Create(WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT |CCS_TOP , 
			//	CRect(0,0,1,1), this , IDC_MENUBAR))

			//    if (!m_wndMenuBar.CreateEx(TBSTYLE_FLAT ,TBSTYLE_FLAT | WS_CHILD | WS_VISIBLE | TBSTYLE_LIST | CCS_NODIVIDER| CCS_NORESIZE|CBRS_SIZE_FIXED, 
			//CRect(0,0,1,1), this , IDC_MENUBAR))

			return -1;
		LPARAM		plparam = 2;
	    AddCustomMenuItems(plparam);

		m_wndMenuBar.LoadMenu(CreateMenuXP());
		/*
		if (!m_wndTree.Create(WS_CHILD | WS_VISIBLE |
		TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,
		CRect(0,0,1,1), this, IDC_TREE))
		return -1;

		m_wndTree.ModifyStyleEx(0, WS_EX_STATICEDGE);
		*/

		return (0) ;
	}

	void CMyBar::SizeChanged (CRect *lpRect, BOOL bFloating, int flags) {
		//----- TODO: Add your code here
		lpRect->InflateRect(-6, -6);
		m_wndMenuBar.SetWindowPos(NULL,lpRect->left,lpRect->top,lpRect->Width(),20,SWP_SHOWWINDOW);
		// 	m_wndTree.SetWindowPos(NULL,lpRect->left,lpRect->top+20,lpRect->Width(),lpRect->Height()-20,SWP_SHOWWINDOW);
	}

	bool CMyBar::CanFrameworkTakeFocus()
	{
		if (GetKeepFocus())
		{
			return false;
		}

		return CAcUiDockControlBar::CanFrameworkTakeFocus();
	}

	void CMyBar::OnContextMenu(CWnd* pWnd, CPoint point) 
	{
		// TODO: Add your message handler code here
		//You can comment out the next line and then write some code to create a XPMenu
		CAcUiDockControlBar::OnContextMenu(pWnd, point);
	}



	///////////////////////////////////
	BOOL CMyBar::AddCustomMenuItems(LPARAM hMenu)
	{
		HMENU hPopMenu;
		hPopMenu = (HMENU)(hMenu & 0xffff);
		CMenu* pop_menu = CMenu::FromHandle(hPopMenu);

		// 去掉弹出菜单上默认的
		pop_menu->RemoveMenu(ID_ADUI_HIDEBAR, MF_BYPOSITION);
		pop_menu->RemoveMenu(ID_ADUI_ALLOWDOCK, MF_BYCOMMAND);
		return true;
	}
	//////////////////////////////////

	CMenuXP* CMyBar::CreateMenuXP()
	{
		CMenuXP	*pMenu = new CMenuXP;

		//You can unrecomment the next two line to load a normal menu
		// XPMenu
		pMenu->CreatePopupMenu();

		CMenuXP	*pPopup,*pPopup1,*pPopup2,*pPopup3;

#ifdef DIGIROAD
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"项目管理"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_NEWXM,L"新建项目"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BDXM,L"打开项目"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DATA,L"数据管理"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DLHZ_GY,L"数据管理"));

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(24,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"建立数模")); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMtqsj,L"提取数据"));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdealdata,L"处理数据"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcreateTin,L"建立数模"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdisplay,L"显示数模"));	


		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"平面设计"));
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYJD,L"导入方案"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTJDCEN,L"录入查询"));
		//		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN,L"查询数据"));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());	
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL,L"实体转换"));		  
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LIANXUDINGXIAN,L"连续布线"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL2,L"离散布线"));	
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_REVERSE,L"线路反向"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LINKXL,L"线路连接"));		  					  
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"交点模式 "));  

		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYXY,L"导入方案"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTXY,L"录入查询"));
		//		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN,L"查询数据"));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());		
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZJBSCDSJ,L"直接式变速车道"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL1,L"模式布线"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_JMBX,L"积木布线"));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL,L"实体转换"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TRANJD,L"线元转交点"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"线元模式 "));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());		

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITDL,L"编辑断链"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDDL,L"增加断链"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELDL,L"删除断链"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());	
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPUTDBS,L"保存方案"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());			

		//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TRANCENTER,L"转成圆心数据"));		

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LMKD,L"路面宽度"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWLMX,L"绘路面线"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPZX,L"绘偏置线"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GETLMKTOFILE,L"提取路面宽"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITLMK,L"编辑路面宽"));
		//  pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PPCD,L"爬坡车道"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CALBD,L"鼻端计算"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SETCGBZ,L"设置超高标准 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SETJKBZ,L"设置加宽标准 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CALCGFD,L"计算超高加宽分段 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXCGFD,L"查询超高加宽分段 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"计算超高加宽 "));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_QIECHUIXIAN,L"绘切垂线"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_JDTDFS,L"拖动设置"));//设置交点拖动方式
		//szf
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"纵断面设计"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetZmCs,L"基本参数 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMzmzdpmfa,L"选择平面 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetSJBZ,L"设计标准 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMZDMFA,L"导入纵面 "));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZMEDITDL,L"编辑断链"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PickDmx,L"屏幕采集 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DMX,L"数模内插 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD,L"列表编辑 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"地面线 "));

		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INBXDMX,L"数据库导入 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BXDMX,L"内插地面线 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD,L"列表编辑 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEBXDMX,L"存入数据库 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"边线地面线 "));

		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDJP,L"匝道接坡 "));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMKZBG,L"绘控制标高 "));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PDX,L"直线转坡度线 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LPSJ,L"拉坡设计 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_MdyPd,L"调整坡度 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZPSJB,L"编辑坡度 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addBpd,L"增加变坡点 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBpd,L"删除变坡点 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LinkBD,L"坡段连接 "));
		//	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZMcheck,L"违规检测"));

		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"坡度设计"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalBri,L"自动设桥 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputBri,L"导入桥梁 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBri,L"删除桥梁 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AddBri,L"增加桥梁 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditBri,L"列表编辑 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"桥梁设置"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalTun,L"自动设隧 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputTun,L"导入隧道 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_Deltun,L"删除隧道 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addtun,L"增加隧道 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditTun,L"列表编辑 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"隧道设置"));  
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMSAVEZDM,L"保存方案"));

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"横断面设计"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetHmCs,L"基本参数 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalSta,L"桩号序列 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GetDmx,L"提取地面线 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HdmAutoDes,L"自动设计 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputHdm,L"导入横断面 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ChangBP,L"修改边坡 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GOUPD,L"沟底纵坡 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEHDM,L"保存方案 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"用地设计"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputYD,L"导入用地数据 "));
		/*
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelYDZ,L"删除用地桩 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDYDZ,L"增加用地桩 "));*/

		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITYDDATA,L"编辑用地数据 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CHANGEYDZ,L"修改用地桩 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEYDDATA,L"保存用地数据 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//szf
		/*
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"  平  面  出  图  "));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"平面出图"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"选择线路"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"绘线位"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"标注里程及断链"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"标注主点里程"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"绘交点连线"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"自动布图框"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"分幅输出平面图"));	
		*/

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(24,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"车速检测")); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMjscs,L"计算车速"));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcsct,L"车速出图"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcscb,L"车速出表"));


		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"图形输出"));
		//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMTSET,L"参数设置"));
		//	pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"绘平面图"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWGRID,L"绘经纬网格"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CBQXYS,L"绘曲线要素"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HXYYSB,L"绘线元要素表"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPM,L"分幅输出"));	
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"平面图"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBDRAWZX,L"绘中线"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBDRAWBD,L"绘鼻端圆"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBDRAWHX,L"绘横线"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBBZ,L"端部标注"));   
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_XGDB,L"修改端部"));    
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"端部数据图"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWZDM,L"绘纵面图"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AJUSTAXLE,L"调整断高"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDAXLE,L"增加断轴"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAXLE,L"删除断轴"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTZDM,L"输出纵面图"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(2,L"纵断面图"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWHDM,L"绘横断面图"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTHDM,L"输出横断面图"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(3,L"横断面图"));   
		//	pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"表格输出 "));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXQXZJB,L"直线曲线转角表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZZZBB,L"逐桩坐标表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZPSQXB,L"纵坡竖曲线表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZZYDZBB,L"逐桩用地坐标表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TSFJSB,L"土石方数量计算表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GCSLB,L"工程数量表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DYT,L"路基大样图"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_MGLTSFB,L"每公里土石方数量表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LXTWSJB,L"路线填挖数据表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LJSJB,L"路基设计表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BGPSG,L"边沟排水沟设计表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_QBTTAB,L"清表数量表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_WYNTAB,L"挖淤数量表"));
		//   pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(4,L"表格输出"));   
		//	pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"三维可视 ")); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdx3d,L"三维模型拼合计算 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMlx3d,L"CAD线路三维建模 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM3DFrameBrowser,L"实时交互漫游 "));	
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM3DPerspective,L"生成透视图 "));	
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"辅助工具"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LCZZB,L"里程转坐标 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZBZLC,L"坐标转里程 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXLMBG,L"查询路面标高 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXYDAREA,L"查询用地面积 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_FACHECK,L"方案审查 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMGUIDE,L"平面导航 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_XLJD,L"线路求交 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAYER,L"删除一层 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RUBBER,L"橡皮擦 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TOOL,L"工具条 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RIBBON,L"打开Ribbon菜单 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());


		////纬地和DigiRoad数据之间的转换
		//pPopup=new CMenuXP;
		//pPopup->CreatePopupMenu();
		//pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		//pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"数据转换 ")); 
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_WDtoDR,L"交点文件转DigiRoad "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		////pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRtoWD,L"DigiRoad转交点文件"));
		////pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_WDdmxtoDR,L"地面线文件转DigiRoad "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		////pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRtoWDdmx,L"DigiRoad转地面线文件"));
		////pPopup->AppendODMenu(0, new CMenuXPSeparator());


		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HELP,L"搜索..."));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"帮助"));
#endif


		///////////////////改移道路嵌入系统///////////////////////////////////////////////////
#ifdef GY_Road_SYS
		//pPopup3=new CMenuXP;
		//pPopup3->CreatePopupMenu();
		////pPopup3->AddSideBar(new CMenuXPSideBar(24, SYSTEMNAME));
		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1,L"项目管理"));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_NEWXM_GY, L"新建道路"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BDXM_GY, L"选择道路"));
		////pPopup->AppendODMenu(0, new CMenuXPSeparator());
		////pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DATA_GY, "数据管理"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DLHZ_GY, L"编辑道路汇总表"));
		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		////	pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1,L"平面设计"));

		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYJD_GY, L"导入方案"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTJDCEN_GY, L"录入查询"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL_GY, L"实体转换"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELJZX_GY, L"删除夹直线"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDJD_GY, L"增加交点"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELJD_GY, L"删除交点"));

		////		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN, "查询数据"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());	
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITDL_GY, L"编辑断链"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDDL_GY, L"增加断链"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELDL_GY, L"删除断链"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());	
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPUTDBS_GY, L"保存方案"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LMKD_GY, L"路面宽度"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LIANXUDINGXIAN_GY, L"设置超高标准"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL2_GY, L"设置加宽标准"));	
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_REVERSE_GY, L"计算超高加宽分段"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LINKXL_GY, L"查询超高加宽分段"));		  					  
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"计算超高加宽 "));
		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		////pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1,L"纵断面设计"));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetZmCs_GY, L"基本参数 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetSJBZ_GY, L"设计标准 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMZDMFA_GY, L"导入纵面 "));	
		//pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZMEDITDL_GY, L"编辑断链"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMzmzdpmfa_GY, L"选择平面 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PickDmx_GY, L"屏幕采集 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DMX_GY, L"数模内插 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD_GY, L"列表编辑 "));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"地面线 "));

		//pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INBXDMX_GY, L"数据库导入 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BXDMX_GY, L"内插地面线 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD_GY, L"列表编辑 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEBXDMX_GY, L"存入数据库 "));
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PDX_GY, L"直线转坡度线 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LPSJ_GY, L"拉坡设计 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_MdyPd_GY, L"单坡度编辑 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZPSJB_GY, L"编辑坡度 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addBpd_GY, L"增加变坡点 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBpd_GY, L"删除变坡点 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LinkBD_GY, L"坡段连接 "));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"坡度设计"));   
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalBri_GY, L"自动设桥 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputBri_GY, L"导入桥梁 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBri_GY, L"删除桥梁 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AddBri_GY, L"增加桥梁 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditBri_GY, L"列表编辑 "));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"桥梁设置"));   
		//pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalTun_GY, L"自动设隧 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputTun_GY, L"导入隧道 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_Deltun_GY, L"删除隧道 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addtun_GY, L"增加隧道 "));
		//pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditTun_GY, L"列表编辑 "));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"隧道设置"));  
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMSAVEZDM_GY, L"保存方案"));

		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		////pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1,L"横断面设计"));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetHmCs_GY, L"基本参数 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalSta_GY, L"桩号序列 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GetDmx_GY, L"提取地面线 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HdmAutoDes_GY, L"自动设计 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputHdm_GY, L"导入横断面 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ChangBP_GY, L"修改边坡 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		////pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GOUPD_GY, "沟底纵坡 "));
		////pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEHDM_GY, L"保存方案 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());

		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		////pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1,L"用地设计"));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDKDES_GY, L"导入用地数据 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//// 	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelYDZ_GY, "删除用地桩 "));
		//// 	pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//// 	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDYDZ_GY, "增加用地桩 "));
		//// 	pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITZDK_GY, L"编辑用地数据 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CHANGEZDK_GY, L"修改用地数据 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEZDK_GY, L"保存用地数据 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());


		////pPopup = new CMenuXP;
		////pPopup3->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_EDITZDK_GY, "编辑占地宽 "));
		////pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//////pMenu->AppendODPopup(0, pPopup, new CMenuXPText(4, "编辑占地宽 "));




		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		////pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1,L"图形输出"));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM_GY, L"绘平面图"));
		////pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWGRID_GY, "绘经纬网格"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CBQXYS_GY, L"绘曲线要素"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HXYYSB_GY, L"绘线元要素表"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPM_GY, L"分幅输出"));	
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"平面图"));   
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());

		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWZDM_GY, L"绘纵面图"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AJUSTAXLE_GY, L"调整断高"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDAXLE_GY, L"增加断轴"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAXLE_GY, L"删除断轴"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTZDM_GY, L"输出纵面图"));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(2, L"纵断面图"));   
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());

		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMBZTK_ROAD, L"新建图框 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INTK__ROAD, L"导入图框 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVETK__ROAD, L"保存图框 "));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMCutTu__ROAD, L"自动成图 "));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"平纵面图"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());

		//pPopup2 = new CMenuXP;
		//pPopup2->CreatePopupMenu();
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWHDM_GY, L"绘横断面图"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTHDM_GY, L"输出横断面图"));
		//pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(3, L"横断面图"));   
		////	pPopup->AppendODMenu(0, new CMenuXPSeparator());

		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		//pPopup3->AppendODPopup(0, pPopup, new CMenuXPText(1, L"表格输出 "));
		//pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZZYDZBB_GY, L"逐桩用地坐标表"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TSFJSB_GY, L"土石方数量计算表"));
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DrawAllRoadsTab_GY, L"输出道路汇总表"));
		//pPopup3->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_DigHole_GY, L"三维模型拼合计算 "));
		////pPopup3->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_Rail3D_GY, "实时交互式漫游 "));
		////pPopup3->AppendODMenu(0, new CMenuXPSeparator());
		////pPopup3->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_ToolBar_GY, L"工具条 "));
		//pMenu->AppendODPopup(0, pPopup3, new CMenuXPText(7, L"改移道路")); 
#endif

		////////////////////////////改移道路独立系统///////////////////////////////////////////////
#ifdef GY_ROAD_DULI_SYS
		/*pMenu=new CMenuXP;
		pMenu->CreatePopupMenu();*/
		//pMenu->AddSideBar(new CMenuXPSideBar(24, SYSTEMNAME));
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"项目管理"));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_NEWXM_GY, L"新建道路"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BDXM_GY, L"选择道路"));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DATA_GY, "数据管理"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DLHZ_GY, L"编辑道路汇总表"));

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(24,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"建立数模")); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMtqsj,L"提取数据"));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdealdata,L"处理数据"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcreateTin,L"建立数模"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMChangeTin, L"切换数模"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdisplay,L"显示数模"));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMTINConvertASC, L"TIN转ASC"));


		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		//	pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"平面设计"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYJD_GY, L"导入方案"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTJDCEN_GY, L"列表编辑"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL_GY, L"转设计线"));

		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELJZX_GY, L"删除夹直线"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDJD_GY, L"增加交点"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELJD_GY, L"删除交点"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_REVERSE, L"线路反向"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"平面工具"));


		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYXY, L"导入方案"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTXY, L"录入查询"));
		//		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN,L"查询数据"));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		/*pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZJBSCDSJ, L"直接式变速车道"));*/
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL1, L"模式布线"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_JMBX, L"积木布线"));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL, L"实体转换"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TRANJD, L"线元转交点"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"线元模式 "));


		//		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN, "查询数据"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITDL_GY, L"编辑断链"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDDL_GY, L"增加断链"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELDL_GY, L"删除断链"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"道路断链"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPUTDBS_GY, L"保存方案"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LMKD_GY, L"路面宽度"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LIANXUDINGXIAN_GY, L"设置超高标准"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL2_GY, L"设置加宽标准"));	
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_REVERSE_GY, L"计算超高加宽分段"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LINKXL_GY, L"查询超高加宽分段"));		  					  
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"计算超高加宽 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RAUTODES_GY, L"自动设计成图"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		//pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"纵断面设计"));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetZmCs_GY, L"基本参数 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetSJBZ_GY, L"设计标准 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMZDMFA_GY, L"导入纵面 "));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMSETCTRLH_GY, L"高程控制点 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMSETJPInfor_GY, L"接坡信息 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		/*pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZMEDITDL_GY, L"编辑断链"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());*/
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMzmzdpmfa_GY, L"选择平面 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PickDmx_GY, L"屏幕采集 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DMX_GY, L"数模内插 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD_GY, L"列表编辑 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"地面线 "));

		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INBXDMX_GY, L"数据库导入 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BXDMX_GY, L"内插地面线 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD_GY, L"列表编辑 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEBXDMX_GY, L"存入数据库 "));
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AutoSlopeData_GY, L"一键纵坡 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PDX_GY, L"直线转坡度线 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LPSJ_GY, L"拉坡设计 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_MdyPd_GY, L"单坡度编辑 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZPSJB_GY, L"编辑坡度 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addBpd_GY, L"增加变坡点 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBpd_GY, L"删除变坡点 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LinkBD_GY, L"坡段连接 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"坡度设计"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalBri_GY, L"自动设桥 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputBri_GY, L"导入桥梁 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBri_GY, L"删除桥梁 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AddBri_GY, L"增加桥梁 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditBri_GY, L"列表编辑 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"桥梁设置"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalTun_GY, L"自动设隧 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputTun_GY, L"导入隧道 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_Deltun_GY, L"删除隧道 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addtun_GY, L"增加隧道 "));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditTun_GY, L"列表编辑 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"隧道设置"));  
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMSAVEZDM_GY, L"保存方案"));

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		//pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"横断面设计"));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetHmCs_GY, L"基本参数 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalSta_GY, L"桩号序列 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GetDmx_GY, L"提取地面线 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ModifyDmx_GY, L"修改地面线 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HdmAutoDes_GY, L"自动设计 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputHdm_GY, L"导入横断面 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ChangBP_GY, L"修改边坡 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GUTINTERACT_GY, L"水沟交互 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GOUPD_GY, "沟底纵坡 "));
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEHDM_GY, L"保存方案 "));





		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		//pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"用地设计"));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDKDES_GY, L"导入用地数据 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		// 	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelYDZ_GY, "删除用地桩 "));
		// 	pPopup->AppendODMenu(0, new CMenuXPSeparator());
		// 	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDYDZ_GY, "增加用地桩 "));
		// 	pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITZDK_GY, L"编辑用地数据 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CHANGEZDK_GY, L"修改用地数据 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEZDK_GY, L"保存用地数据 "));


		//pPopup = new CMenuXP;
		//pMenu->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_EDITZDK_GY, "编辑占地宽 "));
		//pMenu->AppendODMenu(0, new CMenuXPSeparator());
		////pMenu->AppendODPopup(0, pPopup, new CMenuXPText(4, "编辑占地宽 "));




		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		//pPopup->AddSideBar(new CMenuXPSideBar(20, SYSTEMNAME));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"图形输出"));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM_GY, L"绘平面图"));
		//pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWGRID_GY, "绘经纬网格"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CBQXYS_GY, L"绘曲线要素"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HXYYSB_GY, L"绘线元要素表"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPM_GY, L"分幅输出"));	
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"平面图"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWZDM_GY, L"绘纵面图"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AJUSTAXLE_GY, L"调整断高"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDAXLE_GY, L"增加断轴"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAXLE_GY, L"删除断轴"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTZDM_GY, L"输出纵面图"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(2, L"纵断面图"));   
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMBZTK_ROAD, L"新建图框 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INTK__ROAD, L"导入图框 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVETK__ROAD, L"保存图框 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMCutTu__ROAD, L"自动成图 "));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1, L"平纵面图"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWHDM_GY, L"绘横断面图"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTHDM_GY, L"输出横断面图"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(3, L"横断面图")); 

		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDCT, L"一键式自动出图"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWGUTTER, L"一键式改沟出图"));
		
		//	pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1, L"表格输出 "));
		pMenu->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZZYDZBB_GY, L"逐桩用地坐标表"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TSFJSB_GY, L"土石方数量计算表"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GCSLB_GY, L"工程数量表"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DYT_GY, L"路基大样图"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DrawAllRoadsTab_GY, L"输出道路汇总表"));

		//pPopup = new CMenuXP;
		//pPopup->CreatePopupMenu();
		//pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1, L"三维计算 "));
		//pPopup->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_DigHole_GY, L"三维模型拼合计算 "));
		//pMenu->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_Rail3D_GY, "实时交互式漫游 "));
		//pMenu->AppendODMenu(0, new CMenuXPSeparator());
		//pMenu->AppendODMenu(0,new CMenuXPText(ID_MENUITEM_ToolBar_GY, L"工具条 "));
		//新三维
		/*pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1, L"三维计算 "));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdx3d, L"三维模型拼合计算 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMlx3d, L"CAD线路三维建模 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM3DFrameBrowser, L"实时交互漫游 "));*/
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1, L"大临路网规划 "));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INITLW, L"路网规划"));
		/*pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SYSMLW, L"系统路网"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZTOPLW, L"专题路网优化"));*/

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"辅助工具"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LCZZB,L"里程转坐标 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZBZLC,L"坐标转里程 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXLMBG,L"查询路面标高 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXYDAREA,L"查询用地面积 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_FACHECK,L"方案审查 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMGUIDE,L"平面导航 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_XLJD,L"线路求交 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAYER,L"删除一层 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RUBBER,L"橡皮擦 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RIBBON, L"打开Ribbon "));
		//20190722	暂时关闭	工具条用不了，里面命令是错的
		//pPopup->AppendODMenu(0, new CMenuXPSeparator());
		//pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TOOL,L"工具条 "));

		/*pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HELP,L"用户手册"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"帮助"));*/
#endif






		return pMenu;	
	}


	inline bool CMyBar::GetKeepFocus() const
	{
		return m_bKeepFocus;
	}


	inline void CMyBar::SetKeepFocus(const bool newVal)
	{
		m_bKeepFocus = newVal;
	}



	void CMyBar::GetFloatingMinSize(long *MinWide,long *MinHeight)
	{
		*MinWide = 900;
		*MinHeight = 30;
	}

	//bool CMyBar::LoadArx(ACHAR ArxFname[256],ACHAR cmd[80])
	//{
	//	/*if (!CheckDogByDLL())
	//		return false;*/

	//	ACHAR cmdstr[80];
	//	acedGetAcadDwgView()->SetFocus();
	//	ACHAR fname[256];                                                                                                                      
	//	_stprintf(fname , L"%s\\%s",SysPath,ArxFname);
	//	//ads_alert(fname);
	//	acDocManager->lockDocument(acDocManager->curDocument());//pxb 2005.06.13
	//	acedArxLoad(fname);
	//	acDocManager->activateDocument(acDocManager->curDocument());
	//	_stprintf(cmdstr , L"(command \"%s\")\n",cmd);
	//	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
	//	acDocManager->unlockDocument(acDocManager->curDocument());//pxb 2005.06.13
	//	return 1;
	//}



	

	/////////////////////////////////////////////////////////////////////////////
	// CGC3DApp commands
	bool CMyBar::CheckDate()
	{
		//--------检测是否超过期限,从2004.2.12开始20天---------
		//检测日期是否过期
		int Year,Mon,Day,CurYear,CurMon,CurDay,StartDays,EndDays,SpanDays;
		Year=2004;
		Mon=2;
		Day=8;

		//时间检测   
		SYSTEMTIME lpSystemTime;
		GetSystemTime(&lpSystemTime);   // address of system time structure
		CurYear = lpSystemTime.wYear;
		CurMon = lpSystemTime.wMonth;
		CurDay = lpSystemTime.wDay;

		StartDays = (Mon-1)*30+Day;
		EndDays = (CurMon-1)*30+CurDay;
		SpanDays = EndDays-StartDays;

		// _stprintf(str,"CurYear=%d Month=%d Day=%d Year=%d day=%d mon=%d sdday=%d",CurYear,CurMon,CurDay,Year,Day,Mon,SpanDays);
		//  AfxMessageBox(str);

		if(Year!=CurYear || SpanDays<0 || SpanDays>20)
			// if(SpanDays<0 || SpanDays>200)
		{
			//	 DestroySys();
			//	 AfxMessageBox("本系统暂不提供三维浏览功能, 若想继续使用，请与软件开发单位联系: 0731-2655164 蒲浩");
			return 0;
		}
		return true;


	}
	

	//////////////////////////////////////////////////////////////////////////
//#ifdef GY_Road_SYS
	//菜单实现
	void CMyBar::OnMENUITEM_NEWXM_GY()//新建道路
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"NewMdfRoad\")\n");
		_tcscpy(cmdstr, L"NewMdfRoad\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_BDXM_GY()//选择道路
	{
		//LoadArx(L"ROADDesign.arx",L"SelMdfRoad");
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SelMdfRoad\")\n");
		_tcscpy(cmdstr, L"SelMdfRoad\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_DLHZ_GY()//道路汇总表
	{
		//LoadArx(L"ROADDesign.arx","EditAllroads");
		ACHAR cmdstr[80];
		// TODO: Add your command handler code here
		//_tcscpy(cmdstr, L"(command \"EditAllroads\")\n");
		_tcscpy(cmdstr, L"EditAllroads\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEM_DrawAllRoadsTab_GY()//输出改移道路汇总表
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OutAllRoadsTab\")\n");
		_tcscpy(cmdstr, L"OutAllRoadsTab\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMINPUTDBSBYJD_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OPENPMDBSBYJD\")\n");
		_tcscpy(cmdstr, L"OPENPMDBSBYJD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMINPUTJDCEN_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"INJD\")\n");
		_tcscpy(cmdstr, L"INJD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMZXYTOXL_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ZXYTOXL\")\n");
		_tcscpy(cmdstr, L"ZXYTOXL\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMADDJD_GY()//增加交点
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCADDJD\")\n");
		_tcscpy(cmdstr, L"JCADDJD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMDELJD_GY()//删除交点
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCDELJD\")\n");
		_tcscpy(cmdstr, L"JCDELJD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMDELJZX_GY()//删除夹直线
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DeleteJZX\")\n");
		_tcscpy(cmdstr, L"DeleteJZX\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMDLFX_GY()//道路反向
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCDELJD\")\n");
		_tcscpy(cmdstr, L"JCREVERSE\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}

	void CMyBar::OnMENUITEMEDITDL_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCDLB\")\n");
		_tcscpy(cmdstr, L"JCDLB\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMADDDL_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCADDDL\")\n");
		_tcscpy(cmdstr, L"JCADDDL\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMDELDL_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCDELDL\")\n");
		_tcscpy(cmdstr, L"JCDELDL\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMLMKD_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"Lmk\")\n");
		_tcscpy(cmdstr, L"Lmk\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMLIANXUDINGXIAN_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SetCGBZ\")\n");
		_tcscpy(cmdstr, L"SetCGBZ\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMLOCATIONTOOL2_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SetJKBZ\")\n");
		_tcscpy(cmdstr, L"SetJKBZ\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMREVERSE_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CALCGFD\")\n");
		_tcscpy(cmdstr, L"CALCGFD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMLINKXL_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CXCGFD\")\n");
		_tcscpy(cmdstr, L"CXCGFD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMOUTPUTDBS_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SAVEPMDBS\")\n");
		_tcscpy(cmdstr, L"SAVEPMDBS\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMRAUTODES_GY()
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SAVEPMDBS\")\n");
		_tcscpy(cmdstr, L"RAUTODES\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}
	

	//纵面	
	void CMyBar::OnMENUITEMsetZmCs_GY()	//基本参;
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"InitZdm_Road\")\n");
		_tcscpy(cmdstr, L"InitZdm_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMzmzdpmfa_GY()	//选择平面
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ZM_ZDPMFA_Road\")\n");
		_tcscpy(cmdstr, L"ZM_ZDPMFA_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMsetSJBZ_GY()	//设计标准
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SetZMBZ_Road\")\n");
		_tcscpy(cmdstr, L"SetZMBZ_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMZDMFA_GY()//导入纵面
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"INPUTZMFA_Road\")\n");
		_tcscpy(cmdstr, L"INPUTZMFA_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMSETCTRLH_GY()//高程控制点
	{
		ACHAR cmdstr[80];
		_tcscpy(cmdstr, L"SETCTRLH\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}

	void CMyBar::OnMENUITEMSETJPInfor_GY()//接坡信息
	{
		ACHAR cmdstr[80];
		_tcscpy(cmdstr, L"JPInfor\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}

	void CMyBar::OnMENUITEM_ZMEDITDL_GY()	//编辑断链
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCZMDLB_Road\")\n");
		_tcscpy(cmdstr, L"JCZMDLB_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_PickDmx_GY()	//屏幕采集地面线
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"PickZdmDmx_Road\")\n");
		_tcscpy(cmdstr, L"PickZdmDmx_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEM_DMX_GY()	//数模内插地面线
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DMX_Road\")\n");
		_tcscpy(cmdstr, L"DMX_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMEditDMD_GY()	//列表编辑地面线
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"EDITDMD_Road\")\n");
		_tcscpy(cmdstr, L"EDITDMD_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEM_AutoSlopeData_GY()	//一键纵坡
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"IDZDM_Road\")\n");
		_tcscpy(cmdstr, L"AutoZDM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}

	void CMyBar::OnMENUITEM_PDX_GY()	//直线转坡度线
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"IDZDM_Road\")\n");
		_tcscpy(cmdstr, L"IDZDM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_LPSJ_GY()	//拉坡设计
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CZMLPSJ_Road\")\n");
		_tcscpy(cmdstr, L"CZMLPSJ_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_MdyPd_GY()	//调整坡度
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"MDYPD_Road\")\n");
		_tcscpy(cmdstr, L"MDYPD_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_ZPSJB_GY()	//"编辑坡度
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ZPB_Road\")\n");
		_tcscpy(cmdstr, L"ZPB_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_addBpd_GY()	//增加变坡点
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ADDBPD_Road\")\n");
		_tcscpy(cmdstr, L"ADDBPD_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_DelBpd_GY()	//删除变坡点
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DELBPD_Road\")\n");
		_tcscpy(cmdstr, L"DELBPD_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_LinkBD_GY()	//坡段连接
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"LINKPDX_Road\")\n");
		_tcscpy(cmdstr, L"LINKPDX_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}


	void CMyBar::OnMENUITEM_CalBri_GY()	//自动设桥
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CalBri_Road\")\n");
		_tcscpy(cmdstr, L"CalBri_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_InputBri_GY()	//导入桥梁 
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"InputBri_Road\")\n");
		_tcscpy(cmdstr, L"InputBri_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_DelBri_GY()	//删除桥梁
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DelBri_Road\")\n");
		_tcscpy(cmdstr, L"DelBri_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_AddBri_GY()	//增加桥梁
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"AddBri_Road\")\n");
		_tcscpy(cmdstr, L"AddBri_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_EditBri_GY()	//列表编辑
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"EDITBri_Road\")\n");
		_tcscpy(cmdstr, L"EDITBri_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_CalTun_GY()	//自动设隧
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CalTun_Road\")\n");
		_tcscpy(cmdstr, L"CalTun_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_InputTun_GY()	//导入隧道
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"MdbInTun_Road\")\n");
		_tcscpy(cmdstr, L"MdbInTun_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_Deltun_GY()	//删除隧道
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DelTun_Road\")\n");
		_tcscpy(cmdstr, L"DelTun_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_addtun_GY()	//增加隧道
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"AddTun_Road\")\n");
		_tcscpy(cmdstr, L"AddTun_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_EditTun_GY()	//列表编辑
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"EDITTun_Road\")\n");
		_tcscpy(cmdstr, L"EDITTun_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	void CMyBar::OnMENUITEMSAVEZDM_GY()	//保存方案
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SAVEZDMPRJ_Road\")\n");
		_tcscpy(cmdstr, L"SAVEZDMPRJ_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}


	//横断面	
	void CMyBar::OnMENUITEMsetHmCs_GY()	//基本参数???
	{
		//AfxMessageBox("hh");
		ACHAR fname[256];
		_stprintf(fname , L"%s\\Roadhdmdata.exe",SysPath);
		//  runexe(fname);    
		//WinExec((LPCSTR)fname,1);
		ShellExecute(NULL,L"open",fname,NULL,NULL,SW_SHOWNORMAL);
	}
	void CMyBar::OnMENUITEM_CalSta_GY()	//桩号序列
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CALSTA_Road\")\n");
		_tcscpy(cmdstr, L"CALSTA_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_GetDmx_GY()	//提取地面线
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"GetDmx_Road\")\n");
		_tcscpy(cmdstr, L"GetDmx_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_ModifyDmx_GY()	//修改地面线
	{
		ACHAR cmdstr[80];
		_tcscpy(cmdstr, L"ModifyDMX\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}
	void CMyBar::OnMENUITEM_HdmAutoDes_GY()	//自动设计
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"HdmAutoDes_Road\")\n");
		_tcscpy(cmdstr, L"HdmAutoDes_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_InputHdm_GY()	//导入横断面
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"InHdm_Road\")\n");
		_tcscpy(cmdstr, L"InHdm_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_ChangBP_GY()	//修改边坡
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"EDITHDMBP_Road\")\n");
		_tcscpy(cmdstr, L"EDITHDMBP_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_GutInteract_GY()	//水沟交互
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"EDITHDMBP_Road\")\n");
		_tcscpy(cmdstr, L"GUTINTER\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}
	void CMyBar::OnMENUITEM_GOUPD_GY()	//沟底纵坡
	{
		//LoadArx(L"ROADDesign.arx","AdjGouPD_Road");
	}
	void CMyBar::OnMENUITEM_SAVEHDM_GY()	//保存方案
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SAVEHDM_Road\")\n");
		_tcscpy(cmdstr, L"SAVEHDM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEMEDITZDK_GY()	//编辑占地宽
	{

		// TODO: Add your command handler code here
		// TODO: Add your command handler code here
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"EDITZDK_ROAD\")\n");
		_tcscpy(cmdstr, L"EDITZDK_ROAD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);

	}
	void CMyBar::OnMENUITEMZDKDES_GY()	//编辑占地宽
	{

		// TODO: Add your command handler code here
		// TODO: Add your command handler code here
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ZDKDES_ROAD\")\n");
		_tcscpy(cmdstr, L"ZDKDES_ROAD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);

	}
	void CMyBar::OnMENUITEMDelYDZ_GY() //删除用地桩
	{
		// TODO: Add your command handler code here
		acedGetAcadDwgView()->SetFocus();
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DELZDK_ROAD\")\n");
		_tcscpy(cmdstr, L"DELZDK_ROAD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
		//acDocManager->sendStringToExecute(acDocManager->curDocument(),"DELZDK_ROAD\n",0);	
	}

	void CMyBar::OnMenuitemAddydz_GY() //添加用地桩
	{
		// TODO: Add your command handler code here
		acedGetAcadDwgView()->SetFocus();
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ADDZDK_ROAD\")\n");
		_tcscpy(cmdstr, L"ADDZDK_ROAD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
		//acDocManager->sendStringToExecute(acDocManager->curDocument(),"ADDZDK_ROAD\n",0);
	}
	void CMyBar::OnMENUITEMCHANGEZDK_GY()	//编辑占地宽
	{
		// TODO: Add your command handler code here
		// TODO: Add your command handler code here
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"CHANGEZDK_ROAD\")\n");
		_tcscpy(cmdstr, L"CHANGEZDK_ROAD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);

	}
	void CMyBar::OnMENUITEMSAVEZDK_GY()	//编辑占地宽
	{

		// TODO: Add your command handler code here
		// TODO: Add your command handler code here
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SAVEZDK_ROAD\")\n");
		_tcscpy(cmdstr, L"SAVEZDK_ROAD\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);

	}
	//图形输出	
	//平面图	
	void CMyBar::OnMENUITEM_DRAWPM_GY()	//绘平面图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCDRAWPM_Road\")\n");
		_tcscpy(cmdstr, L"JCDRAWPM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_DRAWGRID_GY()//绘经纬网格
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DRAWGRID_Road\")\n");
		_tcscpy(cmdstr, L"DRAWGRID_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_CBQXYS_GY()	//绘曲线要素
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DRAWQXYS_Road\")\n");
		_tcscpy(cmdstr, L"DRAWQXYS_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_HXYYSB_GY()	//绘线元要素表
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DRAWXYYSB_Road\")\n");
		_tcscpy(cmdstr, L"DRAWXYYSB_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_OUTPM_GY()	//分幅输出
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"JCAUTOPM_Road\")\n");
		_tcscpy(cmdstr, L"JCAUTOPM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	//纵断面图	
	void CMyBar::OnMENUITEM_DRAWZDM_GY()	//绘纵面图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DRAWZDM_Road\")\n");
		_tcscpy(cmdstr, L"DRAWZDM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_AJUSTAXLE_GY()	//调整断高
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"AJUSTAXLE_Road\")\n");
		_tcscpy(cmdstr, L"AJUSTAXLE_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_ADDAXLE_GY()	//增加断轴
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ADDAXLE_Road\")\n");
		_tcscpy(cmdstr, L"ADDAXLE_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_DELAXLE_GY()	//删除断轴
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DELAXLE_Road\")\n");
		_tcscpy(cmdstr, L"DELAXLE_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_OUTZDM_GY()	//输出纵面图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OUTZDM_Road\")\n");
		_tcscpy(cmdstr, L"OUTZDM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	//平纵断面图
	void CMyBar::OnMENUITEM_PMBZTK_ROAD()	//新建图框
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"ADDPMTK\")\n");
		_tcscpy(cmdstr, L"ADDPMTK\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_INTK__ROAD()	//导入图框
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OPENPMTK\")\n");
		_tcscpy(cmdstr, L"OPENPMTK\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_SAVETK__ROAD()	//保存图框
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"SAVEPMTK\")\n");
		_tcscpy(cmdstr, L"SAVEPMTK\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_PMCutTu__ROAD()//自动成图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"TRIMPMT\")\n");
		_tcscpy(cmdstr, L"TRIMPMT\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	//横断面图
	void CMyBar::OnMENUITEM_DRAWHDM_GY()	//绘横断面图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DRAWHDMT_Road\")\n");
		_tcscpy(cmdstr, L"DRAWHDMT_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_OUTHDM_GY()	//输出横断面图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OUTHDM_Road\")\n");
		_tcscpy(cmdstr, L"OUTHDM_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}

	//表格输出	
	void CMyBar::OnMENUITEM_ZZYDZBB_GY()	//逐桩用地坐标表
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OUTZZYDZB_Road\")\n");
		_tcscpy(cmdstr, L"OUTZZYDZB_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_TSFJSB_GY()	//土石方数量计算表
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OUTTSFJSB_Road\")\n");
		_tcscpy(cmdstr, L"OUTTSFJSB_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
	void CMyBar::OnMENUITEM_GCSLB_GY()	////工程数量表
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OUTGCSLB_Road\")\n");
		_tcscpy(cmdstr, L"OUTGCSLB_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}
	void CMyBar::OnMENUITEM_DYT_GY()	////路基大样图
	{
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"OUTDYT_Road\")\n");
		_tcscpy(cmdstr, L"OUTDYT_Road\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
	}

	void CMyBar::OnMENUITEM_DigHole_GY()	//三维模型拼合计算
	{		
		ACHAR cmdstr[80];
		//_tcscpy(cmdstr, L"(command \"DX3D\")\n");
		_tcscpy(cmdstr, L"DX3D\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	}
//#endif

	


	void CMyBar::OnMenuitemzdmfa() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	
	//_tcscpy(cmdstr,L"(command \"INPUTZMFA_Road\")\n");
	_tcscpy(cmdstr, L"INPUTZMFA_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);		 			
}

void CMyBar::OnMenuitemPdx()//直线转纵断面设计线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"IDZDM_Road\")\n");
	_tcscpy(cmdstr, L"IDZDM_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMENUITEMsetZMSJBZ()//设置纵面标准
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"SetZMBZ_Road\")\n");
	_tcscpy(cmdstr, L"SetZMBZ_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMzmzdpmfa() //指定平面方案
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	
	//_tcscpy(cmdstr,L"(command \"ZM_ZDPMFA_Road\")\n");
	_tcscpy(cmdstr, L"ZM_ZDPMFA_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMENUITEMPickDmx() //采集地面数据
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"PickZdmDmx_Road\")\n");
	_tcscpy(cmdstr, L"PickZdmDmx_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMEditDMD() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	
	acDocManager->setCurDocument(acDocManager->curDocument(),AcAp::kNone,Adesk::kTrue);
	//_tcscpy(cmdstr,L"(command \"EDITDMD_Road\")\n");
	_tcscpy(cmdstr, L"EDITDMD_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
		 
}

void CMyBar::OnMenuitemZDJP() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	
	acDocManager->setCurDocument(acDocManager->curDocument(),AcAp::kNone,Adesk::kTrue);
	//_tcscpy(cmdstr,L"(command \"ZDJP\")\n");
	_tcscpy(cmdstr, L"ZDJP\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);		 
}

void CMyBar::OnMenuitemDMX() //点绘地面线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"DMX_Road\")\n");	
	_tcscpy(cmdstr, L"DMX_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}
//
void CMyBar::OnMenuitemINBXDMX()
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"INBXDMX\")\n");	
	_tcscpy(cmdstr, L"INBXDMX\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void CMyBar::OnMenuitemBXDMX() //点绘边线地面线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"BXDMX\")\n");	
	_tcscpy(cmdstr, L"BXDMX\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMenuitemSAVEBXDMX() //点绘边线地面线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"SAVEBXDMX\")\n");	
	_tcscpy(cmdstr, L"SAVEBXDMX\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMenuitemKZBG()
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"KZBG\")\n");	
	_tcscpy(cmdstr, L"KZBG\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMsetZmCs() //纵断面初始化
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"InitZdm_Road\")\n");
	_tcscpy(cmdstr, L"InitZdm_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMenuitemLpsj() //拉坡设计
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"CZMLPSJ_Road\")\n");
	_tcscpy(cmdstr, L"CZMLPSJ_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMMdyPd() //调整坡度
{
	// TODO: Add your command handler code here		
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"MDYPD_Road\")\n");
	_tcscpy(cmdstr, L"MDYPD_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void CMyBar::OnMenuitemZpsjb() //纵坡数据表
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"ZPB_Road\")\n");
	_tcscpy(cmdstr, L"ZPB_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMaddBpd() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"ADDBPD_Road\")\n");
	_tcscpy(cmdstr, L"ADDBPD_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}

void CMyBar::OnMENUITEMDelBpd() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"DELBPD_Road\")\n");
	_tcscpy(cmdstr, L"DELBPD_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}
void CMyBar::OnMENUITEMLinkBD() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"LINKPDX_Road\")\n");
	_tcscpy(cmdstr, L"LINKPDX_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMenuitemsavezdm() //保存纵面方案
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"SAVEZDMPRJ_Road\")\n");
	_tcscpy(cmdstr, L"SAVEZDMPRJ_Road\n");
    acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);		 		
}

void CMyBar::OnMENUITEMCalBri() //自动设桥
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"CalBri_Road\")\n");
	_tcscpy(cmdstr, L"CalBri_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}
void CMyBar::OnMENUITEMDelBri() //删除桥梁
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"DelBri_Road\")\n");
	_tcscpy(cmdstr, L"DelBri_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMAddBri() //增加桥梁
{
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"AddBri_Road\")\n");
	_tcscpy(cmdstr, L"AddBri_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMCalTun() //自动设隧
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"CalTun_Road\")\n");
	_tcscpy(cmdstr, L"CalTun_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}
void CMyBar::OnMENUITEMDeltun() //删除隧道
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"DelTun_Road\")\n");
	_tcscpy(cmdstr, L"DelTun_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMENUITEMaddtun() //增加隧道
{
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"AddTun_Road\")\n");
	_tcscpy(cmdstr, L"AddTun_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void CMyBar::OnMENUITEMEditBri() //编辑桥
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"EDITBri_Road\")\n");
	_tcscpy(cmdstr, L"EDITBri_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMENUITEMEditTun() //编辑隧
{
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"EDITTun_Road\")\n");
	_tcscpy(cmdstr, L"EDITTun_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMENUITEMInputTun() //导入隧道(数据库)
{
    // TODO: Add your command handler code here
    ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"MdbInTun_Road\")\n");
	_tcscpy(cmdstr, L"MdbInTun_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMENUITEMInputBri() //导入桥梁
{
	// TODO: Add your command handler code here		
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"InputBri_Road\")\n");
	_tcscpy(cmdstr, L"InputBri_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void CMyBar::OnMenuitemLMKD()//定义路面宽度
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"Lmk\")\n");
	_tcscpy(cmdstr, L"Lmk\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}	

void CMyBar::OnMenuitemGetLmkToFile()//重实体中提取边线坐标到文件中
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"GetLmk\")\n");
	_tcscpy(cmdstr, L"GetLmk\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMenuitemEditLmk()//重实体中提取边线坐标到文件中
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"EditLmk\")\n");
	_tcscpy(cmdstr, L"EditLmk\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMenuitemLMX()//绘路面线
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"DrawLMX\")\n");
	_tcscpy(cmdstr, L"DrawLMX\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}

void CMyBar::OnMenuitemPZX()//绘路面线
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"DrawPZX\")\n");
	_tcscpy(cmdstr, L"DrawPZX\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}	

void CMyBar::OnMenuitemCalBD()//绘路面线
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"CalBD\")\n");
	_tcscpy(cmdstr, L"CalBD\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}	

void CMyBar::OnMenuitemADDDL()//增加断链
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCADDDL\n",0);		
}
void CMyBar::OnMenuitemDELDL()//删除断链
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDELDL\n",0);		
}

void CMyBar::OnMenuitemLocationtool1() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"LOCATIONTOOL\n",0);		
}

void CMyBar::OnMenuitemLocationtool2() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"RPMLOC\n",0);		
}

void CMyBar::OnMenuitemZJBSCD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZJBSCD\n",0);
}

void CMyBar::OnMenuitemInPutJDCen()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"INJD\n",0);		
}

void CMyBar::OnMenuitemInPutXY()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"INXY\n",0);		
}

void CMyBar::OnMenuitemDrawPm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDRAWPM_Road\n",0);		
}

void CMyBar::OnMenuitemDrawGRID()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRAWGRID_Road\n",0);		
}

void CMyBar::OnMenuitemDrawZdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRAWZDM_Road\n",0);		
}

void CMyBar::OnMenuitemDrawHdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRAWHDMT_Road\n",0);		
}

void CMyBar::OnMenuitemAJUSTAXLE()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"AJUSTAXLE_Road\n",0);		
}
void CMyBar::OnMenuitemAddAXLE()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ADDAXLE_Road\n",0);		
}

void CMyBar::OnMenuitemDelAXLE()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DELAXLE_Road\n",0);		
}

void CMyBar::OnMenuitemOutZdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZDM_Road\n",0);		
}

void CMyBar::OnMenuitemOutHdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTHDM_Road\n",0);		
}
void CMyBar::OnMenuitemZDCT()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZDCT\n",0);		
}

void CMyBar::OnMenuitemDRAWGUTTER()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"GJCT\n", 0);
}

void CMyBar::OnMenuitemReverse()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCREVERSE\n",0);		
}

void CMyBar::OnMenuitemTranJD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CXJ\n",0);		
}

void CMyBar::OnMenuitemTranCenter()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"TRANCENTER\n",0);		
}

void CMyBar::OnMenuitemEditJD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"EDITJDCEN\n",0);		
}

void CMyBar::OnMenuitemLINKXL()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XLLINK\n",0);		
}

void CMyBar::OnMenuitemZXYTOXL()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZXYTOXL\n",0);
}

void CMyBar::OnMenuitemNEWXM()//新建项目
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"NewMdfRoad\n",0);
}

void CMyBar::OnMenuitemBDXM()
{
	// TODO: Add your command handler code here	
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SelMdfRoad\n",0);
}

void CMyBar::OnMenuitemINPUTDBSBYJD()//从数据库读取
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OPENPMDBSBYJD\n",0);	
}

void CMyBar::OnMenuitemINPUTDBSBYXY()//从数据库读取
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OPENPMDBSBYXY\n",0);	
}

void CMyBar::OnMenuitemOUTPUTDBS()//保存到数据库
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SAVEPMDBS\n",0);	
}

void CMyBar::OnMenuitemZQZJB()//直曲转角表
{
	acutPrintf(L"直曲转角表!\n");	
}

void CMyBar::OnMenuitemZDQXB()//匝道曲线表	
{
	acutPrintf(L"匝道曲线表!\n");	
}

void CMyBar::OnMenuitemZDZDZBB()//匝道主点坐标表
{
	acutPrintf(L"匝道主点坐标表!\n");
}

void CMyBar::OnMenuitemZDZZZBB()//匝道主桩坐标表
{
	acutPrintf(L"匝道主桩坐标表!\n");
}

void CMyBar::OnMenuitemINPUTEXJDCEN()//录入二线平面数据
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCEXSJ\n",0);	
}

void CMyBar::OnMenuitemQIECHUIXIAN()//切线垂线工具
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCPROJPOINT\n",0);	
}

void CMyBar:: OnMenuitemLIANXUDINGXIAN()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCADDXL\n",0);	
}

void CMyBar::OnMenuitemDELFLD()//删除分离段
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDELFLD\n",0);	
}

void CMyBar::OnMenuitemEDITDL()//编辑断链
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDLB\n",0);	
}

void CMyBar::OnMenuitemZMEDITDL()//编辑断链
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCZMDLB_Road\n",0);	
}
void CMyBar::OnMenuitemJDTDFS()//设置交点拖动方式
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCJDTDFS\n",0);	
}

void CMyBar::OnMenuitemAUTOTK()//自动布图框
{
}

void CMyBar::OnMenuitemOUTPM()//分幅输出平面图	
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCAUTOPM_Road\n",0);		
}

void CMyBar::OnMenuitemDrawCBQXYS()
{
 /*
   SetDrawQXYS *qxdlg=NULL;
    qxdlg = new SetDrawQXYS(acedGetAcadFrame());
    qxdlg->Create (IDD_DIALOGQXYS, acedGetAcadFrame());
 		  //  move the window over so we can see the control lines
    qxdlg->ShowWindow(SW_SHOW);*/
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	//_tcscpy(cmdstr,L"(command \"DRAWQXYS_Road\")\n");
	_tcscpy(cmdstr, L"DRAWQXYS_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);  

}
void CMyBar::OnMenuitemDrawXYYSB()
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"DRAWXYYSB_Road\")\n");
	_tcscpy(cmdstr, L"DRAWXYYSB_Road\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0); 
}

void CMyBar::OnMenuitemPMTSET()//平面图参数设置
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCPMDRAWSET\n",0);		
}

void CMyBar::OnMENUITEMtqsj() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"TQSJ\n",0);			
}

void CMyBar::OnMENUITEMdealdata() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CLSJ\n",0);	
}

void CMyBar::OnMENUITEMcreateTin() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JLSM\n",0);	
}
void CMyBar::OnMENUITEMChangeTin()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"QHSM\n", 0);
}

void CMyBar::OnMENUITEMdisplay() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XSSM\n",0);	
}

void CMyBar::OnMENUITEMTINConvertASC()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"TCONA\n", 0);
}

bool CMyBar::LoadArx(ACHAR ArxFname[256],ACHAR cmd[80])
{
	ACHAR cmdstr[80];
	
	//if(!CheckDog())
	//    	return false;
	
	//    if(!CheckNetDog())
	//     return false;
	acedGetAcadDwgView()->SetFocus();
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\%s",SysPath,ArxFname);

	acedArxLoad(fname);
	acDocManager->activateDocument(acDocManager->curDocument());
	_stprintf(cmdstr ,L"(command \"%s\")\n",cmd);
	//_stprintf(cmdstr, L"(command \"%s\")\n", cmd);
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
	
	return 1;
	
}

void CMyBar::OnMENUITEMSETCGBZ() //设置超高标准
{
	// TODO: Add your command handler code here
	/*SetCGBZDlg dlg;
	dlg.DoModal();*/
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SetCGBZ\n",0);	
}

void CMyBar::OnMENUITEMSETJKBZ() //设置加宽标准
{
	// TODO: Add your command handler code here
	/*CSetJKNorm dlg;
	dlg.DoModal();*/
	
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SetJKBZ\n",0);		
}

void CMyBar::OnMENUITEMCALCGFD() //计算超高分段
{
	// TODO: Add your command handler code here

	acedGetAcadDwgView()->SetFocus();

	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CALCGFD\n",0);				

}

void CMyBar::OnMENUITEMCXCGFD() //查询超高分段
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CXCGFD\n",0);				
	
}

void CMyBar::OnMENUITEMsetHmCs() //横断面初始化
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"HDMCS\n",0);		


}

void CMyBar::OnMENUITEMCalSta() //计算桩号序列
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CALSTA_Road\n",0);				
}

void CMyBar::OnMENUITEMAjustSta()//调整桩号
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"AjustSta\n",0);
}

void CMyBar::OnMENUITEMGetDmx() //横断面地面线
{
	// TODO: Add your command handler code here
	if(ptriDTM.DTMHasRead==1)
	    ptriDTM.ReleaseDtm();//释放Dtm
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"GetDmx_Road\n",0);
}

void CMyBar::OnMENUITEMHdmAutoDes() //横断面自动设计
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"HdmAutoDes_Road\n",0);
}

void CMyBar::OnMENUITEMInputHdm() //导入横断面
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"InHdm_Road\n",0);
}

void CMyBar:: OnMENUITEMChangeBp() //修改边坡
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"EDITHDMBP_Road\n",0);
}
void CMyBar:: OnMENUITEMSAVEHDM() //修改边坡
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SAVEHDM_Road\n",0);
}
 


void CMyBar::OnMENUITEMInputYD() //导入用地数据
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZDKDES\n",0);
}

void CMyBar::OnMENUITEMDelYDZ() //删除用地桩
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DELZDK\n",0);	
}

void CMyBar::OnMenuitemAddydz() //添加用地桩
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ADDZDK\n",0);
}

void CMyBar::OnMenuitemEdityddata() //编辑用地数据
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"EDITZDK\n",0);
}

void CMyBar::OnMenuitemChangeydz() //修改用地桩
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CHANGEZDK\n",0);
}

void CMyBar::OnMenuitemSaveyddata() //保存用地数据
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SAVEZDK\n",0);
}
	
void CMyBar::OnMenuitemZXQXZJB()
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZXQXZJ\n",0);
}
void CMyBar::OnMenuitemZZZBB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZZZB\n",0);
}
void CMyBar::OnMenuitemZPSQXB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZPSQX\n",0);
}
void CMyBar::OnMenuitemZZYDZBB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZZYDZB_Road\n",0);
}
void CMyBar::OnMenuitemTSFJSB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTTSFJSB_Road\n",0);
}
void CMyBar::OnMenuitemGCSLB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"OUTGCSLB_Road\n", 0);
}
void CMyBar::OnMenuitemDYT()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"OUTDYT_Road\n", 0);
}
void CMyBar::OnMenuitemMGLTSFB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTMGLTSFB\n",0);
}
void CMyBar::OnMenuitemLXTWSJB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTLXTWB\n",0);
}
void CMyBar::OnMenuitemLJSJB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTLJSJB\n",0);
}

void CMyBar::OnMenuitemBGPSG()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTBGPSG\n",0);
}

void CMyBar::OnMenuitemDATA()
{
	LoadArx(L"projmdb.arx",L"tree");	
}
 
void CMyBar::OnMENUITEMGOUPD()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"AdjGouPD_Road\n",0);
}

void CMyBar::OnMenuitemDELAYER()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DELAYER\n",0);
}

void CMyBar::OnMenuitemRUBBER()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"RUBBER\n",0);
}

void CMyBar::OnMenuitemTOOL()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"VIZTOOL\n",0);
}

void CMyBar::OnMenuitemRIBBON()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"RIBBON\n",0);
}

void CMyBar::OnMenuitemWDtoDR()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"WDtoDR\n",0);
}

void CMyBar::OnMenuitemDRtoWD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRtoWD\n",0);
}

void CMyBar::OnMenuitemWDdmxtoDR()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"WDdmxtoDR\n",0);
}

void CMyBar::OnMenuitemDRtoWDdmx()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRtoWDdmx\n",0);
}

void CMyBar::OnMenuitemDX3D()
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"DX3D\")\n");
	_tcscpy(cmdstr, L"DX3D\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void CMyBar::OnMenuitemLX3D()
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"LX3D\")\n");
	_tcscpy(cmdstr, L"LX3D\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void CMyBar::OnMenuitemView3D()//实时交互漫游
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"SSJHMY\")\n");
	_tcscpy(cmdstr, L"SSJHMY\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);


}

void CMyBar::OnMenuitemINITLW()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"INITLW\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
}
void CMyBar::OnMenuitemSYSMLW()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"SYSMLW\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
}
void CMyBar::OnMenuitemZTOPLW()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"ZTOPLW\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
}

void CMyBar::OnMenuitemPmGuide()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"PmGuide\n",0);	
}

void CMyBar::OnMenuitemLczzb() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"PMLCTOZB\")\n");
	_tcscpy(cmdstr, L"PMLCTOZB\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);					
}
void CMyBar::OnMenuitemZbzlc()
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"PMZBTOLC\")\n");
	_tcscpy(cmdstr, L"PMZBTOLC\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMenuitemCxlmbg()
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"CXLMBG\")\n");
	_tcscpy(cmdstr, L"CXLMBG\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void CMyBar::OnMenuitemCxYdArea()
{
	ACHAR cmdstr[80];
	//_tcscpy(cmdstr,L"(command \"CXYDAREA\")\n");
	_tcscpy(cmdstr, L"CXYDAREA\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void CMyBar::OnMenuitemProCheck()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ProCheck\n",0);	
}
void CMyBar::OnMenuitemJMBX() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JMBX\n",0);
}

void CMyBar::OnMenuitemXLJD() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XLJD\n",0);
}	

void CMyBar::OnMENUITEMCalNose() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CalNose\n",0);
}	

void CMyBar::OnMenuitemDrawDBZX()//绘端部中线
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBZX\n",0);
}
void CMyBar::OnMenuitemDrawDBBDCIR()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBCIR\n",0);
}
void CMyBar::OnMenuitemDrawDBHX()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBHX\n",0);
}
void CMyBar::OnMenuitemDBBZ()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBBZ\n",0);
}

void CMyBar::OnMenuitemXGDB()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XGDBBZ\n",0);
}

void CMyBar::OnMenuitemPerspective()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"GenPers\n",0);
}

void CMyBar::OnMenuitemPPCD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"PPCD\n",0);
}


void CMyBar::OnMenuitemHELP()
{

	ACHAR hlpname[256];
	_stprintf(hlpname,L"%s\\DGRoadHelp.chm",SysPath);
	acedHelp(hlpname, NULL, 0);

}

void CMyBar::OnMenuitemQBTTAB()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OutQBTB\n",0);
}
void CMyBar::OnMenuitemWYNTAB()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OutWYNB\n",0);
}
//计算车速
void CMyBar::OnMENUITEMjscs()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JSCS\n",0);
}

//车速出图
void CMyBar::OnMENUITEMcsct()
{	
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CSCT\n",0);

}
//车速出表
void CMyBar::OnMENUITEMcscb()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CSCB\n",0);


}
