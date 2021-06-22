#if !defined(AFX_DLINITLWDLG_H__B8B5B192_35E3_4270_A8C8_E50490D59799__INCLUDED_)
#define AFX_DLINITLWDLG_H__B8B5B192_35E3_4270_A8C8_E50490D59799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LMSJ.h : header file
//
#include "BAS_DRAW_FUN.h"
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "resource.h"
#include <omp.h>
#define NUMOFARC 50000

// DLInintLwDlg 对话框
struct DEMData
{
	double Cml;//里程
	double EarthH;//地面标高
	double DetaH;//填挖高
	double DesH;//设计标高
	double Area;//填挖面积

	int row, col;//行列号
	double MaxbridgeH;//最大桥高
	double maxTunlen;//最大隧道长度
	float MinBriH;//设桥最小填高
	float DividBriH;//线桥分界填高
	float MinBriL;//最小桥长
	float MinTunH;//设隧最小挖深
	float DividTunH;//线隧分界挖深
	float MinTunL;//最小隧长
	float CutUnitCost;//挖方每方单价
	float CutBPXS;//挖方边坡系数
	float CutBPHeight;
	float CutPTWide;
	float FillUnitCost;//填方每方单价
	float FillBPXS;//填方边坡系数
	float FillBPHeight;
	float FillPTWide;
	float DQUnitCost;//挡墙每方单价
	float DQBPXS;//填方边坡系数
	float BriUnitCost;//桥梁每方单价
	float CZBriHeight;//桥梁参照桥高
	float QTCost;//桥头费用
	float TunlUnitCost;//隧道每延米单价
	float CZTunlLen;//参照隧长
	float DMCost;//洞门费用
	float ZZCost;//征地费用
	float YDJKLen;//用地加宽长度

	float ZYLDCost;//征林地费用
	float LDJKLen;//用地加宽长度
	float ZYCDCost;//征草地费用
	float CDJKLen;//用地加宽长度
	float ZYGDCost;//征耕地费用
	float GDJKLen;//用地加宽长度
	float ZYZJDCost;//征宅地费用
	float ZJDJKLen;//用地加宽长度


	float XXCost; //线形费用

	bool ISKY;//是否跨越
	float UpDetaH;//高差上限
	float DownDetaH;//高差下限
	short KYType;//跨越类型
//xjx 
	float slope;//fwj
	float aspect;

	int ID_XXW;
	int QJ_XXW;

	int DebrisFlowID;
	int LandslideID;
	int CollapseID;
	double g_MaxLen_HighBri;
};
struct NodeData
{
	AcGePoint3d PtNode;
	int IdNum;
	int Row, Col;
	vector<int>ConnectIds;
	AcGePoint3dArray ConnectPts;
	vector<double>Values;
	bool isOnExistRoad;
	double relief;
	double roughness;
	bool inForbid;
	double V_colum;
};
typedef struct
{
	vector<CString>vexs; //顶点表
	vector<vector<double>>arcs;//邻接矩阵 
	//vector<vector<bool>>isOnRoad;//邻接矩阵 
	int vexnum, arcnum; //图当前点数和边数
	AcGePoint3dArray LocationPts;
}AMGraph;
class DLInintLwDlg : public CDialog
{
	DECLARE_DYNAMIC(DLInintLwDlg)

public:
	
	DLInintLwDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DLInintLwDlg();
	AcGePoint2dArray AdjustPoints(AcGePoint2dArray ForPlinePts);
	void ShortestPath_DIJ(AMGraph &G, int v0,int& vend,bool isOnEndRoad=true);
	bool Cal_Cost_a2b(AcGePoint3d PT1, AcGePoint3d PT2, DEMData DMXCS, double & Cost);
	bool FormDMXTW(AcGePoint3d PT1, AcGePoint3d PT2, int & NCRS, DEMData DMXCS, vector<DEMData>& DMXCSArray);
	bool AutoSetBRI(DEMData DMXCS, vector<DEMData>& DMXCSArray, vector<BRITAB_DN>& BridgeArray);
	bool AutoSetTUN(DEMData DMXCS, vector<DEMData>& DMXCSArray, vector<TUNTAB_DN>& TunArray);
	double CalCost(vector<DEMData>& DMXCSArray, vector<BRITAB_DN>& BridgeArray, vector<TUNTAB_DN>& TunArray, DEMData DMXCS, double & Cost);
	void INITyhcs();
	bool IsOnExistRoad(AcGePoint2d CanPt, vector<AcGePoint2dArray> Verts, AcGePoint2d &ResPt);
	AcGePoint3dArray FormAbdMask(double length,int precision, AcGePoint3d Pt);
	AcGePoint3dArray FormAbdMask(double length, double step, int index);
	void Sort(int flag);
// 对话框数据
	double CostMainLine;
	double singleCost;

	//vector<double>CostsofDropEarth;
	vector<vector<double>>CostsofHouse;
	enum { IDD = IDD_DIALOG_DLINI};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DEMData DMXCS;
	vector<int>IndexWorkhouse;
	vector<int>IndexDropEarth;
	vector<int>IndexThrdhouse;
	vector<double>BXDis2Road;
	vector<double>BXDis2YL;
	vector<double>THDis2Road;
	vector<AcGePoint2dArray> Verts;
	CGridCtrl m_grid;
	GV_ITEM Item;
	BAS_DRAW_FUN ob;
	AcGePoint3dArray GDPts,YLPts;
	vector<CString>strTypeGDs,GDNames;
	vector<NodeData>NodeDatas;
	vector<NodeData>RoadNodeDatas;
	NodeData tempData;
	AMGraph amg,Thdamg,Recamg;
	AcDbObjectIdArray ExistRoadIds;
	AcDbObjectIdArray ForbidIds;
	vector<int>FearIds,ConnectIds;
	vector<vector<int>>ComPIds;
	int Row, Col;
	afx_msg void OnBnClickedButtonPickgd();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDladdrow();
	afx_msg void OnBnClickedButtonDldet();
	afx_msg void OnBnClickedButtonCrtgrd();
	// //网格边长
	double m_Size;
	double MaxValue;
	double CFCoofe;
	vector<int>Indexs, PassRoad;
	AcGePoint2dArray ForPlinePts,temppts;
	SYSTEMTIME lpSystemTime;
	double CurTimeNum;
	int m_NumSelect, m_NumSelectThrd;
	AcDbObjectIdArray TreenIds;
	bool S[NUMOFARC]; //记录从源点V0到终点Vi是否已经确定为最短路径，确定了记true，否则记false
	int Path[NUMOFARC]; //记录从源点V0到终点Vi的当前最短路径上终点Vi的直接前驱顶点序号，若V0到Vi之间有边前驱为V0否则为-1 
	vector<vector<int>>Paths;
	vector<int>path;
	double D[NUMOFARC]; //记录源点到终点之间最短路径的长度，存在记V0到Vi的边的权值，否则记为MAX 
	vector<double>slgCost;
	vector<vector<double>>DCost;
	afx_msg void OnBnClickedButtonCrtmesh();
	afx_msg void OnBnClickedButtonExist();
	afx_msg void OnBnClickedButtonCandipool();
	double m_TZLen;
	CGridCtrl m_Grid;
	afx_msg void OnBnClickedButtonDladdrowbx();
	afx_msg void OnBnClickedButtonDldetbx();
	CGridCtrl m_YLgrid;
	afx_msg void OnBnClickedButtonPickyl();
	afx_msg void OnBnClickedButtonDladdrowyl();
	afx_msg void OnBnClickedButtonDldetyl();
	afx_msg void OnBnClickedButtonRedrawgd();
	afx_msg void OnBnClickedButtonRewrawyl();
	int m_JM;
	afx_msg void OnBnClickedButtonDropearth();
	afx_msg void OnBnClickedButtonWorkhouse();
	afx_msg void OnBnClickedButtonTuba();
	double m_XMBLen;
	int m_JMXMB;
	double m_Volum;
	double m_MaxPD;
	double m_OKPD;
};
#endif