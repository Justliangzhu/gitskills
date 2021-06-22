// DialogSetDirectRamp.h : header file
// 直接式变速车道设计对话框
// 1.三角段长度按计算出来的结果取值,不允许更改
// 2.变速车道总长度要大于规范值
// 3.三角段与渐变段同义,故有三角渐变段的说法
// 4.变速车道的线形与主线一致(参见规范说明)
// 5.在变速车道设计时如后面没有选择后接圆,则变速车道长度满足规范及渐变率满足要求即可
// 6.三角段的终占是由从主线渐变出一个匝道宽位置确定
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGSETDIRECTRAMP_H__BD905B50_44BB_4F2F_ACA9_A90EB1A9D676__INCLUDED_)
#define AFX_DIALOGSETDIRECTRAMP_H__BD905B50_44BB_4F2F_ACA9_A90EB1A9D676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BlockComputerFun.h"
#include "JD_CENTER.H"
#include "math.h"
#include "ZYLink.h"
#include "YYLuanLink.h"
#include "YYSLink.h"
#include "LLTuLink.h"

/////////////////////////////////////////////////////////////////////////////
// 直接式变速车道设计对话框

class CDialogSetDirectRamp : public CDialog, public BlockComputerFun
{
// Construction
public:
	CDialogSetDirectRamp(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSetDirectRamp)
	enum { IDD = IDD_DIALOGDIRECTRAMP };
	CString	m_EditChangedata;   // 渐变率
	CString	m_EditRampTriMiles; // 渐变段(连接段)起点桩号
	CString	m_EditConnectLen;   // 连接段长度
	CString	m_EditDRampSMiles;  // 变速车道起点桩号
	CString	m_EditDRampEMiles;  // 变速车道终点桩号
	CString	m_EditDRampLen;     // 变速车道长
	CString	m_EditDRampERad;    // 连接圆半径
	CString	m_AorL;             // 回旋线参数类型
	int		m_HLinkModel;       // 连接圆与终止单元的连接类型
	double	m_dA1L1;            // A1 or L1
	double	m_dA2L2;            // A2 or L2
	double	m_dA3L3;            // A3 or L3
	double	m_dA4L4;            // A4 or L4
	CString	m_strW1234;         // W1,W2,W3,W4
	CString	m_fldR;             // 分流点曲率半径
	CString	m_fldA;             // 分流点A值
	double	m_dOffsetW;         // 终止单元偏宽
	//}}AFX_DATA

	double W1, W2, W3, W4;    // 变速车道控制点参数
	double A1, A2, A3, A4;    // 缓和曲线参数
	
	double MainVect,RampVect;   // 主线、匝道的设计车速
	JD_CENTER*   pGottnRamp;    // 主线实体指针
	AcDbObjectId SelEntId;      // 所选主线的ID
	double ValidItemType;       // 主线中被选中处的线元
	double ProTriMiles;	        // 渐变段起点对应的主线里程
	double m_dTriLen;           // 渐变段长度
	double DRampTriX,DRampTriY,TriSOrient,TriRad;  // 渐变段起点坐标、方位角及半径
	double TriMainX,TriMainY,TriMainOA,TriMainRad; // 渐变段对应的主线点坐标、方位角及半径
	
	double ProDRampSMiles,ProDRampEMiles;         // 变速车道起终点对应的线路里程
	AcGePoint3d DRampSDot,DRampEDot,OrignDot;     // 变速车道起终点坐标及缓曲线的起点坐标
	double DRampSOrient,DRampEOrient,DROriOrient; // 变速车道起终点及缓曲线的起点的方位角
	double DRampSMainX,DRampSMainY,DRampSMainOA,DRampSMainRad; // 变速车道起终点对应的主线点坐标、方位角及半径
	double SCirX, SCirY;         // 变速车道起点处的圆心坐标
	double DRampDef;             // 变速车道的偏向
	double DRampSRad, DRampERad; // 变速车道起终点处的半径
	
	AcDbObjectId TriRampId;      // 渐变段的图形ID
	AcDbObjectId ConnectLineID;  // 连接段(不包括渐变段)为直线时的图形ID
	AcDbObjectId ConnectCirID;   // 连接段(不包括渐变段)为曲线时的图形ID

	AcDbObjectId SelCircleId;                 // 连接圆ID
	double ProDRampCirMiles;	              // 连接圆的圆心对应的主线里程
	bool   m_bCirSelAgain;                    // 连接圆是否重新拾取
	double DRampCirX, DRampCirY, DRampCirRad; // 连接圆的圆心坐标及半径	

	AcDbObjectId StartElementId;              // 起始单元ID
	AcDbObjectId EndElementId;                // 终止单元ID
	AcDbObjectId OffsetElementId;             // 偏置单元ID
	
	double ChangeValue; // 渐变率变量(偏转角)

	AcDbObjectId SprialID1,SprialID2;             // 两缓和曲线ID	
	AcGePoint3d  FirstSprialEnd, SecondSprialEnd; // 两缓曲线的终点坐标
	double       FirstSprialEOA, SecondSprialEOA; // 两缓曲线的终点方位角
	double       FirstSprialLen, SecondSprialLen; // 两缓和曲线长
	
	double LCLindSide; // 圆圆连接的偏向(逆时针:-1 顺时针:1) 
	double CCLineDef;  // 连接圆圆心在渐变段起点切线的侧向
	double m_RampPosLeftOrRight;  // 变速车道位于主线的侧向(连接圆圆心相对于主线前进方向) 左:-1 右:1
	double m_RampAddOrSub;        // 变速车道的性质(加速:1 减速:-1)
	double flag;                  // 变速车道与主线是否同向
	
	double m_dNoseCmlOnZX, m_dNoseCmlOnYX; // 鼻端在主线和右线上的投影里程
	double m_NosePt[2];                    // 鼻端圆心点
	AcDbObjectId W1Id, W2Id, W3Id, W4Id;   // 参数W1,W2,W3,W4对应标识直线ID
	
	double RouteItemArrayOld[10][10]; // 匝道线元(用于acutGrDraw)
	double RouteItemArray[10][10]; // 匝道线元
	JD_CENTER* RampJD_CENTER;      // 匝道实体指针
	AcDbObjectId RampJD_CENTERID;  // 匝道实体ID
	AcDbObjectId XYId[11];         // 匝道各线元ID
	// 
	int ItemNum;    
	int m_iXYMax;   // 最大线元号
	int m_iSelElem; // 选择单元数量
	bool   m_bFitConnectLen; // 是否指定连接段长
	double ConnectLen; // 连接段长(不包括渐变段)
	double SLength, SLength1, SLength2; // 缓和曲线段长

	int m_LinkModel;  // 变速车道起点为曲线时,变速车道与连接圆的连接方式

	// 
	double MainValueA;
	// 
	AcDbObjectId AddCirId;

	AcGePoint3d PickDotOld, PickDotNew; // 鼠标拾取点
	ads_point   PickDotEnd;   // 终止单元拾取基点
	bool   m_bJD_CENTER;      // 终止单元是否为JD_CENTER
	bool   m_bConnectDirect;  // 是否直接连接终止单元
	bool   m_bDragConnectCir; // 是否在拖动连接单元
	int    m_iConnectCirXY;   // 连接圆线元编号
	double m_dEndElemR;       // 终止单元半径
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSetDirectRamp)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// 根据界面上所得信息计算渐变段长及变速车道参数
	void ComputerDRampParameters();
	// 计算缓曲线上到指定直线相距为D的点
	void GetEndPointOfDRamp();
	// 获得圆心在渐变段起点切线的侧向及圆圆的连接偏向(顺时针还是逆时针)
	bool GetCenterSideAndLinkDef();
	// 圆弧上两点及圆心坐标求此两点间的圆弧长度
	double GetArcLength(double CenX,double CenY,double ArcX1,double ArcY1,
	double ArcX2,double ArcY2,double ArcRad);
		
	void AutoModel();
	
protected:
	bool GetParameter();          // 获得界面参数并计算设置相关数据 2005.07.21 ldf
	bool GetW1234();              // 获得控制点参数W1,W2,W3,W4
	void PutA1A2();               // 由统一的A1A2值计算界面输出参数
	void GetA1A2();               // 由界面输入参数获得统一的A1A2值
	bool SelectMainElement();     // 选取主线单元
	bool SelectConnectElement();  // 选取连接单元(圆或圆弧)
	bool SelectEndElement();      // 选取终止单元(线路实体、直线、圆、圆弧)
	bool GetMainElement();        // 实时获得主线单元
	bool GetConnectCirCoordRad(); // 实时获得连接圆圆心坐标和半径
	bool SetConnectCirRad(double dRadius);         // 设置连接圆半径	
	bool SetConnectCirCoord(AcGePoint3d centerPt); // 设置连接圆圆心坐标
	bool DragConnectCir();        // 动态拖动连接圆
	bool GetOffsetElement();      // 实时获得终止偏置单元
	bool CheckSprial();           // 检查缓和曲线的合理性
	bool CreateRampJD_CENTER(double XYArrayTmp[][10],int iXYNum);   // 创建变速车道线路实体
	void Draw();                  // 绘制设计线路  2005.07.22 ldf
	// Generated message map functions
	//{{AFX_MSG(CDialogSetDirectRamp)
	afx_msg LONG OnAcadKeepFocus(UINT,LONG);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtontoknowmeaning();	
	afx_msg void OnButtongetendrad();
	afx_msg void OnButtonupdatedramp();
	afx_msg void OnCheck1();
	afx_msg void OnSelchangeAl();
	afx_msg void OnSelendokAl();
	afx_msg void OnButtonImply();
	afx_msg void OnCHECKConnectDirect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETDIRECTRAMP_H__BD905B50_44BB_4F2F_ACA9_A90EB1A9D676__INCLUDED_)
