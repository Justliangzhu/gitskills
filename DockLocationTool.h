//-----------------------------------------------------------------------------
#if !defined(ARX__DOCKLOCATIONTOOL_H__20040520_131808)
#define ARX__DOCKLOCATIONTOOL_H__20040520_131808

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"
#include "menubar\\MenuXP.h"
#include "menubar\\MenuBarXP.h"
#define ISLOCATION 0
#define ISMENU 1
//可停靠菜单条
//-----------------------------------------------------------------------------
class DockLocationTool : public CAcUiDockControlBar {
	DECLARE_DYNAMIC(DockLocationTool)

public:
	int m_MODEL;
	CMenuBarXP	m_wndMenuBar;
	
	DockLocationTool () ;
//	LocationDialog LnDlg;
	bool LoadArx(ACHAR ArxFname[256],ACHAR cmd[80]);
	//{{AFX_VIRTUAL(DockLocationTool)
	public:
	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL
	static CMenuXP* CreateMenuXP();
	bool GetKeepFocus() const;
	void SetKeepFocus(const bool newVal);
	BOOL CreateControlBar(LPCREATESTRUCT lpCreateStruct);
//protected:
	//{{AFX_MSG(DockLocationTool)
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
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
	afx_msg void OnMENUITEMdisplay();
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
	afx_msg void OnMenuitemZDCT();
	afx_msg void OnMenuitemXLJD(); 
	afx_msg void OnMenuitemTOOL();
	afx_msg void OnMenuitemRIBBON();

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
	
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual bool CanFrameworkTakeFocus();
	virtual BOOL AddCustomMenuItems(LPARAM hMenu);
	virtual void GetFloatingMinSize(long *MinWide,long *MinHeight);
	
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags);

private:
	bool m_bKeepFocus;
	
} ;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //----- !defined(ARX__DOCKLOCATIONTOOL_H__20040520_131808)
