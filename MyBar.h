//-----------------------------------------------------------------------------
#if !defined(ARX__MYBAR_H__20021214_182510)
#define ARX__MYBAR_H__20021214_182510

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"

#include "menubar/MenuXP.h"
#include "menubar/MenuBarXP.h"

#define ISLOCATION 0
#define ISMENU    1

//#define  GY_Road_SYS//改移道路嵌入系统
//#define GY_ROAD_DULI_SYS//改移道路独立系统
//#define DIGIROAD//digiroad系统


//-----------------------------------------------------------------------------
class CMyBar : public CAcUiDockControlBar {
	DECLARE_DYNAMIC(CMyBar)

public:
	void AssignImages();
	
	CTreeCtrl m_wndTree;
	CImageList m_ImgIcons;
	CMenuBarXP	m_wndMenuBar;

	CMenuXP* CreateMenuXP(void);
	
	CMyBar () ;
	bool GetKeepFocus() const;
	void SetKeepFocus(const bool newVal);
    bool  LoadArx(ACHAR ArxFname[256],ACHAR cmd[80]);
    bool CheckDate();
	//{{AFX_VIRTUAL(CMyBar)
	public:
	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyBar)
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	//////////////////////////////////////////////////////////////////////////
//#ifdef GY_Road_SYS
	//菜单声明
	//项目管理
	afx_msg void OnMENUITEM_NEWXM_GY();//新建道路
	afx_msg void OnMENUITEM_BDXM_GY();//选择道路
	afx_msg void OnMENUITEM_DLHZ_GY();//编辑道路汇总表
	afx_msg void OnMENUITEM_DrawAllRoadsTab_GY();//输出改移道路汇总表

	//平面
	afx_msg void OnMENUITEMINPUTDBSBYJD_GY();
	afx_msg void OnMENUITEMINPUTJDCEN_GY();
	afx_msg void OnMENUITEMZXYTOXL_GY();
	afx_msg void OnMENUITEMDELJZX_GY();
	afx_msg void OnMENUITEMADDJD_GY();
	afx_msg void OnMENUITEMDELJD_GY();
	afx_msg void OnMENUITEMEDITDL_GY();
	afx_msg void OnMENUITEMADDDL_GY();
	afx_msg void OnMENUITEMDELDL_GY();
	afx_msg void OnMENUITEMLMKD_GY();
	afx_msg void OnMENUITEMLIANXUDINGXIAN_GY();
	afx_msg void OnMENUITEMLOCATIONTOOL2_GY();
	afx_msg void OnMENUITEMREVERSE_GY();
	afx_msg void OnMENUITEMLINKXL_GY();
	afx_msg void OnMENUITEMOUTPUTDBS_GY();
	afx_msg void OnMENUITEMRAUTODES_GY();
	afx_msg void OnMENUITEMDLFX_GY();//道路反向
	//纵面	
	afx_msg void OnMENUITEMsetZmCs_GY();	//基本参数;
	afx_msg void OnMENUITEMzmzdpmfa_GY();	//选择平面
	afx_msg void OnMENUITEMsetSJBZ_GY();	//设计标准
	afx_msg void OnMENUITEMZDMFA_GY();	//导入纵面
	afx_msg void OnMENUITEMSETCTRLH_GY();	//高程控制点
	afx_msg void OnMENUITEMSETJPInfor_GY();	//接坡信息
	afx_msg void OnMENUITEM_ZMEDITDL_GY();	//编辑断链
	afx_msg void OnMENUITEM_PickDmx_GY();	//屏幕采集地面线
	afx_msg void OnMENUITEM_DMX_GY();	//数模内插地面线
	afx_msg void OnMENUITEMEditDMD_GY();	//列表编辑地面线
	afx_msg void OnMENUITEM_AutoSlopeData_GY();	//一键纵坡
	afx_msg void OnMENUITEM_PDX_GY();	//直线转坡度线
	afx_msg void OnMENUITEM_LPSJ_GY();	//拉坡设计
	afx_msg void OnMENUITEM_MdyPd_GY();	//调整坡度
	afx_msg void OnMENUITEM_ZPSJB_GY();	//"编辑坡度
	afx_msg void OnMENUITEM_addBpd_GY();	//增加变坡点
	afx_msg void OnMENUITEM_DelBpd_GY();	//删除变坡点
	afx_msg void OnMENUITEM_LinkBD_GY();	//坡段连接
	afx_msg void OnMENUITEM_CalBri_GY();	//自动设桥
	afx_msg void OnMENUITEM_InputBri_GY();	//导入桥梁 
	afx_msg void OnMENUITEM_DelBri_GY();	//删除桥梁
	afx_msg void OnMENUITEM_AddBri_GY();	//增加桥梁
	afx_msg void OnMENUITEM_EditBri_GY();	//列表编辑
	afx_msg void OnMENUITEM_CalTun_GY();	//自动设隧
	afx_msg void OnMENUITEM_InputTun_GY();	//导入隧道
	afx_msg void OnMENUITEM_Deltun_GY();	//删除隧道
	afx_msg void OnMENUITEM_addtun_GY();	//增加隧道
	afx_msg void OnMENUITEM_EditTun_GY();	//列表编辑
	afx_msg void OnMENUITEMSAVEZDM_GY();	//保存方案


	//横断面	
	afx_msg void OnMENUITEMsetHmCs_GY();	//基本参数
	afx_msg void OnMENUITEM_CalSta_GY();	//桩号序列
	afx_msg void OnMENUITEM_GetDmx_GY();	//提取地面线
	afx_msg void OnMENUITEM_ModifyDmx_GY();	//提取地面线
	afx_msg void OnMENUITEM_HdmAutoDes_GY();	//自动设计
	afx_msg void OnMENUITEM_InputHdm_GY();	//导入横断面
	afx_msg void OnMENUITEM_ChangBP_GY();	//修改边坡
	afx_msg void OnMENUITEM_GutInteract_GY();	//水沟交互
	afx_msg void OnMENUITEM_GOUPD_GY();	//沟底纵坡
	afx_msg void OnMENUITEM_SAVEHDM_GY();	//保存方案

	afx_msg void OnMENUITEMEDITZDK_GY();
	afx_msg void OnMENUITEMZDKDES_GY();
	afx_msg void OnMENUITEMDelYDZ_GY();
	afx_msg void OnMenuitemAddydz_GY();
	afx_msg void OnMENUITEMCHANGEZDK_GY();
	afx_msg void OnMENUITEMSAVEZDK_GY();


	//图形输出	
	//平面图	

	afx_msg void OnMENUITEM_DRAWPM_GY();	//绘平面图
	afx_msg void OnMENUITEM_DRAWGRID_GY();	//绘经纬网格
	afx_msg void OnMENUITEM_CBQXYS_GY();	//绘曲线要素
	afx_msg void OnMENUITEM_HXYYSB_GY();	//绘线元要素表
	afx_msg void OnMENUITEM_OUTPM_GY();	//分幅输出

	//纵断面图	
	afx_msg void OnMENUITEM_DRAWZDM_GY();	//绘纵面图
	afx_msg void OnMENUITEM_AJUSTAXLE_GY();	//调整断高
	afx_msg void OnMENUITEM_ADDAXLE_GY();	//增加断轴
	afx_msg void OnMENUITEM_DELAXLE_GY();	//删除断轴
	afx_msg void OnMENUITEM_OUTZDM_GY();	//输出纵面图

	//平纵面图
	afx_msg void OnMENUITEM_PMBZTK_ROAD();	//新建图框
	afx_msg void OnMENUITEM_INTK__ROAD();	//导入图框
	afx_msg void OnMENUITEM_SAVETK__ROAD();	//保存图框
	afx_msg void OnMENUITEM_PMCutTu__ROAD();//自动成图


	afx_msg void OnMENUITEM_DRAWHDM_GY();	//绘横断面图
	afx_msg void OnMENUITEM_OUTHDM_GY();	//输出横断面图


	//表格输出	

	afx_msg void OnMENUITEM_ZZYDZBB_GY();	//逐桩用地坐标表
	afx_msg void OnMENUITEM_TSFJSB_GY();	//土石方数量计算表
	afx_msg void OnMENUITEM_GCSLB_GY();	////工程数量表
	afx_msg void OnMENUITEM_DYT_GY();//路基大样图
	

	afx_msg void OnMENUITEM_DigHole_GY();	//三维模型拼合计算
//#endif
	
//#ifdef DIGIROAD
	afx_msg void OnMenuitemINPUTDBSBYJD();//从数据库读取
	afx_msg void OnMenuitemINPUTDBSBYXY();//从数据库读取
	afx_msg void OnMenuitemOUTPUTDBS();//保存到数据库		
	afx_msg void OnMenuitemLocationtool1();
	afx_msg void OnMenuitemLocationtool2();
	afx_msg void OnMenuitemZJBSCD();
	afx_msg void OnMenuitemInPutJDCen();
	afx_msg void OnMenuitemInPutXY();
	afx_msg void OnMenuitemDrawPm();
	afx_msg void OnMenuitemDrawGRID();
	afx_msg void OnMenuitemDrawZdm();
	afx_msg void OnMenuitemAJUSTAXLE();
	afx_msg void OnMenuitemAddAXLE();
	afx_msg void OnMenuitemDelAXLE();
	afx_msg void OnMenuitemOutZdm();
	afx_msg void OnMenuitemReverse();
	afx_msg void OnMenuitemTranJD();
	afx_msg void OnMenuitemTranCenter();
	afx_msg void OnMenuitemEditJD();
	afx_msg void OnMenuitemLINKXL();
	afx_msg void OnMenuitemZXYTOXL();//主线元转线路
	afx_msg void OnMenuitemNEWXM();//新建项目
	afx_msg void OnMenuitemBDXM();//标定项目
	afx_msg void OnMenuitemZQZJB();//直曲转角表
	afx_msg void OnMenuitemZDQXB();//匝道曲线表	
	afx_msg void OnMenuitemZDZDZBB();//匝道主点坐标表
	afx_msg void OnMenuitemZDZZZBB();//匝道主桩坐标表
	afx_msg void OnMenuitemADDDL();//增加断链
	afx_msg void OnMenuitemDELDL();//删除断链
	afx_msg void OnMenuitemINPUTEXJDCEN();//录入二线平面数据
	afx_msg void OnMenuitemQIECHUIXIAN();//切线垂线工具
	afx_msg void OnMenuitemLIANXUDINGXIAN();//连续定线
	afx_msg void OnMenuitemDELFLD();//删除分离段
	afx_msg void OnMenuitemEDITDL();//编辑断链
	afx_msg void OnMenuitemZMEDITDL();//编辑断链
	afx_msg void OnMenuitemJDTDFS();//设置交点拖动方式
	afx_msg void OnMenuitemAUTOTK();//自动布图框
	afx_msg void OnMenuitemOUTPM();//分幅输出平面图	
	afx_msg void OnMenuitemDrawCBQXYS();
	afx_msg void OnMenuitemDrawXYYSB();
	afx_msg void OnMenuitemPMTSET();//参数设置

	afx_msg void OnMenuitemDrawDBZX();//绘端部中线
	afx_msg void OnMenuitemDrawDBBDCIR();
	afx_msg void OnMenuitemDrawDBHX();
	afx_msg void OnMenuitemDBBZ();
	afx_msg void OnMenuitemXGDB();
	//建立数模
	afx_msg void OnMENUITEMtqsj();
	afx_msg void OnMENUITEMdealdata();
	afx_msg void OnMENUITEMcreateTin();
	afx_msg void OnMENUITEMChangeTin();
	
	afx_msg void OnMENUITEMdisplay();
	afx_msg void OnMENUITEMTINConvertASC();
	
	//纵断面设计
	afx_msg void OnMenuitemzdmfa();
	afx_msg void OnMenuitemPdx();//直线转纵断面设计线
	afx_msg void OnMENUITEMzmzdpmfa();//指定平面方案
	afx_msg void OnMenuitemDMX();//点绘地面线
	afx_msg void OnMenuitemBXDMX();//点绘地面线
	afx_msg void OnMenuitemSAVEBXDMX();
	afx_msg void OnMenuitemINBXDMX();//导入边线地面线
	afx_msg void OnMenuitemZDJP();
	afx_msg void OnMenuitemKZBG();
	afx_msg void OnMENUITEMsetZmCs();//纵断面初始化
	afx_msg void OnMenuitemLpsj();//拉坡设计
	afx_msg void OnMENUITEMMdyPd();//调整坡度
	afx_msg void OnMenuitemZpsjb();//纵坡表编辑坡度
	afx_msg void OnMENUITEMaddBpd();
	afx_msg void OnMENUITEMDelBpd();
	afx_msg void OnMENUITEMLinkBD();//坡段连接
	afx_msg void OnMenuitemsavezdm();//保存纵面方案
	afx_msg void OnMENUITEMCalBri();//自动设桥
	afx_msg void OnMENUITEMDelBri();//删除桥梁
	afx_msg void OnMENUITEMAddBri();//增加桥梁
	afx_msg void OnMENUITEMPickDmx();//屏幕采集地面线
	afx_msg void OnMENUITEMEditDMD();//编辑地面点
	afx_msg void OnMENUITEMEditBri();
	afx_msg void OnMENUITEMCalTun();
	afx_msg void OnMENUITEMDeltun();
	afx_msg void OnMENUITEMaddtun();
	afx_msg void OnMENUITEMEditTun();
	afx_msg void OnMENUITEMInputTun();
	afx_msg void OnMENUITEMInputBri();
	afx_msg void OnMENUITEMsetZMSJBZ();
	afx_msg void OnMenuitemLMKD();//定义路面宽度
	afx_msg void OnMenuitemLMX();//定义路面宽度
	afx_msg void OnMenuitemPZX();
	afx_msg void OnMenuitemGetLmkToFile();
	afx_msg void OnMenuitemEditLmk();
	afx_msg void OnMenuitemCalBD();//计算鼻端

	afx_msg void OnMENUITEMSETCGBZ();//设置超高标准
	afx_msg void OnMENUITEMSETJKBZ();//设置加宽标准
	afx_msg void OnMENUITEMCALCGFD();//计算超高分段
	afx_msg void OnMENUITEMCXCGFD();//查询超高分段

	afx_msg void OnMENUITEMsetHmCs();
	afx_msg void OnMENUITEMCalSta(); //计算桩号序列
	afx_msg void OnMENUITEMCXSta(); //计算桩号序列
	afx_msg void OnMENUITEMGetDmx();//计算桩号序列
	afx_msg void OnMENUITEMHdmAutoDes(); //横断面地面线
	afx_msg void OnMENUITEMInputHdm();
	afx_msg void OnMENUITEMChangeBp();
	afx_msg void OnMENUITEMSAVEHDM(); //修改边坡
	//用地设计	
	afx_msg void OnMENUITEMInputYD();
	afx_msg void OnMENUITEMDelYDZ();
	afx_msg void OnMenuitemAddydz();
	afx_msg void OnMenuitemEdityddata();
	afx_msg void OnMenuitemChangeydz();
	afx_msg void OnMenuitemSaveyddata();

	afx_msg void OnMenuitemOutHdm();
    afx_msg void OnMenuitemZDCT();
	afx_msg void OnMenuitemDRAWGUTTER();
	afx_msg void OnMenuitemDrawHdm();

	afx_msg void OnMenuitemZXQXZJB();
	afx_msg void OnMenuitemZZZBB();
	afx_msg void OnMenuitemZPSQXB();
	afx_msg void OnMenuitemZZYDZBB();
	afx_msg void OnMenuitemTSFJSB();
	afx_msg void OnMenuitemGCSLB();
	afx_msg void OnMenuitemDYT();
	afx_msg void OnMenuitemMGLTSFB();
	afx_msg void OnMenuitemLXTWSJB();
	afx_msg void OnMenuitemLJSJB();
	afx_msg void OnMenuitemBGPSG();
	afx_msg void OnMenuitemDATA();
	afx_msg void OnMENUITEMGOUPD();

	afx_msg void OnMenuitemINITLW();
	afx_msg void OnMenuitemSYSMLW();
	afx_msg void OnMenuitemZTOPLW();

	afx_msg void OnMenuitemDELAYER();
	afx_msg void OnMenuitemPmGuide();
	afx_msg void OnMenuitemRUBBER();
	afx_msg void OnMenuitemLczzb();
	afx_msg void OnMenuitemZbzlc();
	afx_msg void OnMenuitemCxlmbg();
	afx_msg void OnMenuitemCxYdArea();
	afx_msg void OnMenuitemProCheck();
	afx_msg void OnMenuitemJMBX();
	afx_msg void OnMenuitemXLJD(); 
	afx_msg void OnMenuitemTOOL();
	afx_msg void OnMenuitemRIBBON();

	afx_msg void OnMenuitemWDtoDR();
	afx_msg void OnMenuitemDRtoWD();
	afx_msg void OnMenuitemWDdmxtoDR();
	afx_msg void OnMenuitemDRtoWDdmx();

	afx_msg void OnMenuitemDX3D();
	afx_msg void OnMenuitemLX3D();
	afx_msg void OnMenuitemView3D();
	afx_msg void OnMenuitemPerspective();

	afx_msg void OnMENUITEMAjustSta();
	afx_msg void OnMENUITEMCalNose(); 
	afx_msg void OnMenuitemPPCD();
	afx_msg void OnMenuitemHELP();
	afx_msg void OnMenuitemQBTTAB();
	afx_msg void OnMenuitemWYNTAB();


	afx_msg void OnMENUITEMjscs();
	afx_msg void OnMENUITEMcsct();
	afx_msg void OnMENUITEMcscb();
//#endif


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual bool CanFrameworkTakeFocus();
	virtual BOOL AddCustomMenuItems(LPARAM hMenu);
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags);
	virtual void GetFloatingMinSize(long *MinWide,long *MinHeight);
	
private:
	bool m_bKeepFocus;
} ;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //----- !defined(ARX__MYBAR_H__20021214_182510)
