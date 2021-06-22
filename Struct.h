#pragma once
#include <vector>
//路面宽参数
struct LMKdata
{
	double cml;
	double xlc;//"分段终里程"
	double fgdk;//分隔带半宽
	double fgdhp;//分隔带横坡
	double lydk;//路缘带宽
	double cxdk;//车行道宽
	double lmhp;//路面横坡
	double yljk;//硬路肩宽
	double yljhp;//硬路肩横坡
	double tljk;//土路肩宽
	double tljhp;//土路肩横坡
	int bkmode;//变宽方式,0-线性，1-抛物线
	int sjbgmode;//设计标高位置，0-分隔带边缘,1-路肩
	double OffsetByXLZX;//路基中线相对于线路中线的偏移量（左负右正）
};

//超高参数
struct CgSZ
{
	double cml;
	double xlc;
	double cg;
};

//地面线要素
struct DMXYS
{
	ACHAR  ckml[20];//现场里程
	double dml;
	double level, lc;//坐标及里程（真实坐标）
	double sx, sy;//坐标及里程（图上坐标）
	DMXYS()
	{
		_tcscpy(ckml, L"");
		dml = 0.0;
		level = 0.0;
		lc = 0.0;
		sx = sy = 0.0;
	}

};

//变坡点表
struct PDTAB_ROAD
{
	CString GH;//变坡点冠号
	double  ml;//变坡点里程
	double cml;
	double  Level;//设计高程
	double Rshu, RLen;//竖曲线半径,长度
	double degree, length;//坡度，坡长
	CString Notes;//附注
	ACHAR CKml[80];

	PDTAB_ROAD()
	{
		GH = L"";
		ml = 0.0;
		cml = 0.0;
		Level = 0.0;
		Rshu = 0.0;
		RLen = 0.0;
		degree = 0.0;
		length = 0.0;
		Notes = L"";
		_tcscpy(CKml, L"");
	}
};

//高程控制点表
struct GCKZPt 
{
	
	CString RoadName;
	ACHAR GH[16];
	double XCLc;//现场里程
	double Cml;//统一里程
	double clearrence;
	double DesHSX;//高程上限
	double DesHXX;//高程下限
	int Style;//控制点性质（4-必经点，1-只有上限点，2-只有下限点，3-即有上限点又有下限点）
	CString Note;//备注“可以输入也可以不输入，有时想知道该控制点的名称，是某某路控制

	GCKZPt()
	{
		_tcscpy(GH, L"K");
		XCLc = 0.0;
		Cml = 0.0;
		DesHSX = 0.0;
		DesHXX = 0.0;
		Style = 0;
		Note = L"";
		clearrence = 0.0;
	}
};

//信封区
struct EnvPt
{
	double Mileage;
	double Lever;
};
//分段
struct CRSPT
{
	double Cml;
	float Earh;
	float Style;//性质
};

//填方分类
struct LDTLdata
{
	double dml;
	int type;
};

//用地汇总分段表
struct YongDiFDB
{
	double dml;//里程数
	CString BZ;
};

//挖方分类
struct WFFLdata
{
	double dml;
	double sj[6];
};

//关联断面路肩拉直
struct GLDMLZdata
{
	double Sdml;
	double Edml;
	double dX;
	double dH;
};

//关联断面路堑边坡推平
struct GLDMLQBPTPdata
{
	double Sdml;
	double Edml;
	double dX;
};

//关联断面线间沟
struct GLDMXJGdata
{
	double Sdml;
	double Edml;
	double GouWide;
	double GouHigh;
	double NBP;
	double WBP;
	double JQHD;
	double GDR;
};

//清表土厚度
struct hthddata
{
	double dml;
	double hthd;
	int xz;
};

//水沟坡度数据	沟底坡
struct GouPDdata
{
	double Sdml, SH;
	double Edml, EH;
};

//超填宽度表
struct ctkddata
{
	double Sdml, Edml;
	double Sctkd, Ectkd;
};

//弧形边坡
struct arcbpata
{
	double Sdml, Edml;
	double SR, ER;
	int ArcLocation;//0-路堤土路肩，1-路堤坡脚，2-路堑坡顶，3-路堑坡脚
};

//挡墙
struct dqdata
{
	double sdml, edml;
	double szj, ezj;
};

//低填路堤填平数据	低填路堤自动填平
struct LDTPData
{
	double sdml, edml;
};

//低挖路堑推平数据	低挖路堑自动推平
struct LQTPData
{
	double sdml, edml;
};

//土路肩加宽数据	低填路堤自动加宽土路肩
struct TLJJKData
{
	double sdml, edml;
}; 

//路面厚度
struct LMHdata
{
	double dml;
	double cxdh, yljh, tljh;
};

//地面线内插宽
struct crswiddata
{
	double dml;
	double wid;
};

//横断面限制宽度
struct dmdata
{
	double dml;
	double xk;
};

//用地加宽
struct pwddata
{
	double dml;
	double ltjk, lqjk;
};

//路堤边坡
struct LDBPdata
{
	double dml;
	int BPJS;//边坡级数
	double bpdata[20 * 4];
};

//路堤水沟	排水沟
struct LDSGdata
{
	double sdml, edml;
	int WallorGou;//加一列水沟和坡脚墙的选择（0-坡脚墙，1-水沟）
	double jh;//浆切厚度
	double S_DSYK, E_DSYK;//档水堰宽
	double S_DSYH, E_DSYH;//档水堰厚
	double S_HPDK, E_HPDK;//护坡道宽
	double S_HPDH, E_HPDH;//护坡道厚
	double S_GS, E_GS;//沟深
	double S_GK, E_GK;//沟宽
	double nbp;//内侧边坡
	double wbp;//外侧边坡		
	double DSYP;//挡水堰外侧边坡
	double LDSG_GDR;//路堤沟底半径
};

//路堑水沟	侧沟
struct LQSGdata
{
	double dml;
	double Jh;//浆切厚度
	double gw;//沟宽
	double gs;//沟深（沟深是基床表层厚度+0.2）
	double nbp;//内侧边坡
	double wbp;//外侧边坡      
	double GDR;
};

//天沟
struct TGdata
{
	double sdml;
	double edml;
	double sbj;//起始边距
	double sgw;//起始沟宽
	double sgs;//起始沟深
	double ebj;//终止边距
	double egw;//终止沟宽
	double egs;//终止沟深
	double nbp;//内侧边坡
	double wbp;//外侧边坡
	double jqhd;//浆切厚度
	double TG_GDR;//沟底半径
};

//用地类型表
struct YongDiLXB
{
	double dml;//里程数
	CString YDLX;
};

//路面分层数据	20191016新增
struct RoadLayerData
{
	CString LayerName;	//层名
	double LayerThickness;	//层厚
	RoadLayerData()
	{
		LayerName = L"";
		LayerThickness = 0.0;
	}
};

//里程标表
struct LCBdata
{
	int  lcflag;//里程类型，-1（公里标）,1-9百米标，0-断链标
	ACHAR flag[10];
	int BTxz;//桥隧性质 0-非桥非隧 -1-左桥头,-2-右桥头,-3-全桥头 -4-左隧头,-5-右隧头,-6-全隧头
	// 1-左桥尾, 2-右桥尾, 3-全桥尾  4-左隧尾,5-右隧尾,-6-全隧尾
	double lc;//统一里程

	double gldmlc;//关联断面统一里程
	double gldml;//关联断面现场里程
	CString glRoadName;//关联的道路名
	///////////////////////////////////////////////////////////////////////////////////
};

//隧道表
struct TUNTAB
{
	CString TunName;//桥名 
	CString BGH;//冠号
	CString EGH;//冠号
	CString flag;//插旗方式
	double StDml;//起点里程
	double EdDml;//终点里程
	CString XZ;//隧性质
	double ZZDK, YZDK;
	ACHAR SKml[80], EKml[80];
	double TunCost;
};
struct TUNTAB_DN//隧道表
{
	ACHAR NameOfWay[50];//线路名	例如：既有基线、设计基线等
	ACHAR RxdName[100];//绕行段名	例如：绕行段103.1等

	int iRXD;//绕行段编号。＜0表示并行或单线
	ACHAR TUNName[50];//隧道名 
	ACHAR BGH[16];//冠号
	ACHAR EGH[16];//冠号
	int Flag;//插旗方式 0-明洞、1-一般隧道 //ACHAR Flag[50]

	double StDml;//起点里程 
	double EdDml;//终点里程
	double StH;
	double EdH;
	bool GJZT;//改建状态：0-既有、1-新建
	int NumOfLine; //线数

	double BCml;//在DbsToExcelDlg中作投影里程
	double ECml;
	double LeftXJ;
	double RightXJ;
	double ConstructionXJ;
	double z;
	//以下不是必要的，只是为了用模板函数，需要和桥梁一样有中心里程
	//double CenH;
	double CenDml;
	double CCml;
	ACHAR CGH[16];
	CString GJXZ;
	ACHAR SKml[80], EKml[80];
	double TunCost;
	TUNTAB_DN()
	{
		_tcscpy(NameOfWay, L"");
		_tcscpy(RxdName, L"");
		iRXD = 0;
		_tcscpy(TUNName, L"");
		_tcscpy(BGH, L"");
		_tcscpy(EGH, L"");
		_tcscpy(CGH, L"");
		CenDml = 0.0;
		BCml = 0.0;
		ECml = 0.0;
		StDml = 0.0;
		CCml = 0.0;
		StH = 0.0;
		EdDml = 0.0;
		EdH = 0.0;
		GJXZ = L"";
		LeftXJ = 2.440;
		RightXJ = 2.440;
		ConstructionXJ = 10.00;
	}
};


//大中桥表
struct BRITAB
{
	CString BriName;//桥名 
	CString BGH, EGH, CGH;//冠号
	double CenDml;//中心里程
	double StDml;//起点里程
	double EdDml;//终点里程
	CString KKSY;//孔跨式样
	CString SWHt;//一百年水位高程
	CString XZ;//桥性质
	double HeadAng;//头交角
	double TailAng;//尾交角
	double BriLen;
	double ZZDK, YZDK;
	double Scml, Ecml;
	ACHAR SKml[80], EKml[80], CKml[80];
	double BriCost;
};

struct BRITAB_DN//大中桥表
{
	ACHAR NameOfWay[50];	//线路名	例如：既有基线、设计基线等
	ACHAR RxdName[100];		//绕行段名 例如：绕行段103.1等

	INT16 iRXD;//绕行段编号。＜0表示并行或单线
	ACHAR BriName[50];//桥名 
	ACHAR BGH[16];//冠号
	ACHAR EGH[16];
	ACHAR CGH[16];
	double CenDml;//中心里程
	double CenH;//中心高程（new）
	double StDml;//起点里程
	double StH;//起点高程(new)
	double EdDml;//终点里程
	double EdH;//终点高程(new)
	double LeftXJ;
	double RightXJ;
	double ConstructionXJ;
	ACHAR KKSY[50];//孔跨式样
	int LineNum;//线数  //单线1双线2 ?818
	ACHAR SWHt[50];//一百年水位高程 //818 记得初值赋空
	double BriLen;//桥长
	double BriCost;
	bool GJZT;//改建状态 0-既有 1-新建 818  CString GJZT
	double BriHigh;//桥高
	double AveBriHigh;//平均墩高

	double BCml;//统一里程
	double CCml;
	double ECml;
	double TWg;
	//ACHAR SKml[80],EKml[80],CKml[80];//818 以后这些都隐掉
	CString GJXZ;//新增改建性质
	double BridgeWidth;	//2019.3.20添加梁宽信息
	ACHAR SKml[80], EKml[80], CKml[80];
	BRITAB_DN()	//2019.3.20添加桥梁结构体初始化
	{
		_tcscpy(NameOfWay, L"");
		_tcscpy(RxdName, L"");
		iRXD = 0;
		_tcscpy(BriName, L"");
		_tcscpy(BGH, L"");
		_tcscpy(EGH, L"");
		_tcscpy(CGH, L"");
		GJXZ = L"";
		CenDml = 0.0;
		CenH = 0.0;
		StDml = 0.0;
		StH = 0.0;
		EdDml = 0.0;
		EdH = 0.0;
		LeftXJ = 2.440;
		RightXJ = 2.440;
		ConstructionXJ = 10.00;
		_tcscpy(KKSY, L"");
		LineNum = 0;
		_tcscpy(SWHt, L"");
		BriLen = 0.0;
		GJZT = false;
		BriHigh = 0.0;
		AveBriHigh = 0.0;
		BCml = 0.0;
		CCml = 0.0;
		ECml = 0.0;
		BridgeWidth = 8.0;	//先默认设置为8.0
		TWg = 0.00;
	}
};

struct HDMDMXdata_AutoDesign//里程标表
{
	int  lcflag;//里程类型，-1（公里标）,1-9百米标，0-断链标
	ACHAR flag[10];
	int BTxz;//桥隧性质 0-非桥非隧 -1-左桥头,-2-右桥头,-3-全桥头 -4-左隧头,-5-右隧头,-6-全隧头
							 // 1-左桥尾, 2-右桥尾, 3-全桥尾  4-左隧尾,5-右隧尾,-6-全隧尾
	double lc;//统一里程
///////////////////////////////////////////////////////////////////////////////////
	double EarH;//中桩地面高
	int ZEarPtNum, YEarPtNum;
	std::vector<AcGePoint3d> ZEarPtAry, YEarPtAry;
};

//纵断面设计标准
struct ProfileDesignStd
{
	CString MAXPD; 			//最大坡度
	CString YBPCMin;		//一般坡长最小值
	CString JXPCMin;		//极限坡长最小值
	CString T_YBRshuMin;	//凸形竖曲线	一般最小值
	CString T_JXRShu;		//凸形竖曲线	极限最小值
	CString A_YBRshuMin;	//凹形竖曲线	一般最小值
	CString A_JXRShu;		//凹形竖曲线	极限最小值
	CString YBLRshuMin;		//一般竖曲线长度
	CString JXLRshuMin;		//极限竖曲线长度
};

//出图方式
enum OutputDrawingMode
{
	Break = 0,		//打断方式出图
	Layout = 1		//布局方式出图
};

//总体绘图参数
struct DrawOverallPara
{
	//出图类别
	bool IsDrawHoriOvalDrawing;		//平面总体图
	bool IsDrawHoriRoad;			//平面图
	bool IsDrawHoriEarth;			//用地图
	bool IsDrawProfile;				//纵面图
	bool IsDrawCross;				//横断面图
	bool IsDrawCurveInfo;			//曲线要素表
	bool IsDrawRoadbed;				//路基大样图

	//出图方式
	OutputDrawingMode OutMode;

	//页码，图名
	bool IsNotePage;
	bool IsNoteDrawingName;
	int StartPageNum;
	int TotalPageNum;
	DrawOverallPara()
	{
		IsDrawHoriOvalDrawing = true;
		IsDrawHoriRoad = true;
		IsDrawHoriEarth = false;
		IsDrawProfile = true;
		IsDrawCross = true;
		IsDrawCurveInfo = false;
		IsDrawRoadbed = true;

		OutMode = Break;

		IsNotePage = true;
		IsNoteDrawingName = true;
		StartPageNum = 1;
		TotalPageNum = 0;
	}
};

//平面绘图中心指定模式
enum HDrawCenterAssignMode
{
	ByMoveDistance = 0,		//通过指定移动距离确定绘图中心
	ByCoordinate = 1		//通过坐标确定绘图中心
};

//平面绘图字高设置
struct DrawHorizontalTextHeight
{
	double MileTextH;
	double IntersectionInforTextH;
	double EarthLineTextH;
	double PileTextH;
	DrawHorizontalTextHeight()
	{
		MileTextH = 3.0;
		IntersectionInforTextH = 3.0;
		EarthLineTextH = 3.0;
		PileTextH = 3.0;
	}
};

//平面绘图参数
struct DrawHorizontalPara
{
	int DrawMode;//绘图方式 0  线位图 1 平面总体图 2 用地图
	int DrawAll;//是否绘所有线路0-是 1-非
	int ZDKMode;//绘制占地图方式
	CString RoadName;//道路名
	double QD;//起点里程
	double ZD;//终点里程
	CString SCALE;//比例
	CString XSW;//小数位数
	double WHT;//字高
	CString LCBZ;	//里程标注
	double ZcLen;//左侧用地标注线长
	double YcLen;//右侧用地标注线长
	CString ZxWid;//线路中线宽度
	CString YDXWid;//用地桩线宽度
	bool DrawDZQ;	//绘大中桥
	bool DrawTun;	//绘隧道
	bool DrawCulvet;	//绘小桥涵
	bool DrawBM;	//绘水准点
	bool DrawDX;	//绘导线
	bool DelEntInf;	//删除界内实体
	bool BZRAL;		//标注半径缓长
	bool DrawJD;	//标注交点
	bool drawfldzdk;	//分离段占地宽
	double FrameLength;	//图框长	20190902新增
	double FrameHeight;	//图框宽	20190902新增
	double TopSpace;	//顶部留白区域	20190902新增
	double BottomSpace;	//底部留白区域	20190902新增
	double MoveCenterY;	//移动线路高度	20190904新增
	double MoveCenterX;	//移动线路宽度	20190904新增
	double OneMapRoadLen;	//一个图幅的道路长度	20190905新增
	HDrawCenterAssignMode CenterMode;		//线路中心指定模式	20190909新增
	double CenterX;		//绘图中心X坐标（仅当线路中心指定模式为通过坐标确定绘图中心时生效）	20190909新增
	double CenterY;		//绘图中心Y坐标（仅当线路中心指定模式为通过坐标确定绘图中心时生效）	20190909新增
	ACHAR FrameBlockName[256];	//图框名称	20190910新增
	bool IsBaseMapGray;
	int Is_buju;
	DrawHorizontalTextHeight TextHPara;	//字高绘制参数	20191231新增

	//初始化
	DrawHorizontalPara()
	{
		DrawMode = 0;//绘图方式 0  线位图 1 平面总体图 2 用地图
		DrawAll = 0;//是否绘所有线路0-是 1-非
		ZDKMode = 0;//绘制占地图方式
		RoadName = L"主线";//道路名
		QD = 0.0;//起点里程
		ZD = 0.0;//终点里程
		SCALE = L"2000";//比例
		XSW = L"三位";//小数位数
		WHT = 3.0;//字高
		LCBZ = L"百米标";	//里程标注
		ZcLen = 20.0;//左侧用地标注线长
		YcLen = 20.0;//右侧用地标注线长
		ZxWid = L"1.4";//线路中线宽度
		YDXWid = L"1.0";//用地桩线宽度
		DrawDZQ = true;	//绘大中桥
		DrawTun = true;	//绘隧道
		DrawCulvet = true;	//绘小桥涵
		DrawBM = false;	//绘水准点
		DrawDX = false;	//绘导线
		DelEntInf = false;	//删除界内实体
		BZRAL = true;		//标注半径缓长
		DrawJD = true;	//标注交点
		drawfldzdk = false;	//分离段占地宽
		FrameLength = 780.0;	//图框长	20190902新增
		FrameHeight = 554.0;	//图框高	20190902新增
		TopSpace = 0.0;	//顶部留白区域	20190902新增
		BottomSpace = 0.0;	//底部留白区域	20190902新增
		MoveCenterY = 0.0;	//移动线路高度	20190904新增
		MoveCenterX = 0.0;	//移动线路宽度	20190904新增
		OneMapRoadLen = 700.0;	//一个图幅的道路长度	20190905新增
		CenterMode = ByMoveDistance;	//线路中心指定模式	20190909新增
		CenterX = 0.0;	//绘图中心X坐标（仅当线路中心指定模式为通过坐标确定绘图中心时生效）	20190909新增
		CenterY = 0.0;	//绘图中心Y坐标（仅当线路中心指定模式为通过坐标确定绘图中心时生效）	20190909新增
		_tcscpy(FrameBlockName, L"图框");	//图框名称	20190910新增
		IsBaseMapGray = false;
		Is_buju = 0;
	}
};

//五线谱参数
struct WXP {
	int wxpinf;//表示五线谱项7：超高；6：平面；5：里程桩号
	//4：设计坡度；3,变坡点设计高程 2：设计高程；1: 线间距 0：地质概况
	double bottomht;//栏底距离内图框底高度
	double Lht;//栏高
	double texth;//该栏目注字高度
	CString text;

	WXP()
	{
		wxpinf = 0;
		bottomht = 0.0;
		Lht = 0.0;
		texth = 0.0;
		text = L"";
	}
};


//纵断面绘图参数
struct DrawProfilePara
{
	int DrawMode;//0 初步 1 施工
	int FForZF;//0 分幅 1整幅
	int ZPInf;//0 全部 1 全幅 2 左幅 3 右幅
	double	Hscale;		//横向比例
	double	Zscale;		//纵向比例
	double	EdDml;		//终止里程
	double	StDml;		//起始里程
	int	SPage;		//起始页码
	BOOL	bzym;			//是否标注页码
	BOOL	infwgx;			//是否绘制网格线
	BOOL    ifDrawCtrH;		//是否绘制控制高
	CString	dzgkname;		//地质概况文件路径
	bool IsGutterMode;		//是否是改沟模式
	bool IsDrawFeaturePile; //是否绘制五线谱的特征桩
	bool IsDrawCurveInf;	//是否绘制竖曲线要素
	std::vector<WXP> WXPArray;	//五线谱数组
	ACHAR FrameBlockName[256];	//图框名称	20190910新增
	int Is_buju_ZDM; //纵断面布局出图
	double YbreakTextH;
	double DefTexhH;
	DrawProfilePara()
	{
		DrawMode = 1;
		FForZF = 0;
		ZPInf = 0;
		Hscale = 1000;
		Zscale = 100;
		EdDml = 0;
		StDml = 0;
		SPage = 0;
		bzym = FALSE;
		infwgx = TRUE;
		ifDrawCtrH = TRUE;
		dzgkname = L"";
		IsGutterMode = false;
		IsDrawFeaturePile = false;
		IsDrawCurveInf = true;
		WXPArray.clear();
		Is_buju_ZDM = 0;
		YbreakTextH = 5.0;
		DefTexhH = 5.0;
		_tcscpy(FrameBlockName, L"图框");	//图框名称	20190910新增
	}
};

//横断面绘图参数
struct DrawCrossPara
{
	double Scale;	//比例尺
	bool IsDrawGrid;	//是否绘制网格
	bool IsDrawEarthPile;	//是否绘制用地桩
	double EarthWidL;	//左侧地面线宽
	double EarthWidR;	//右侧地面线宽
	ACHAR FrameBlockName[256];	//图框名称
	double TabTextH;
	double BZTextH;

	DrawCrossPara()
	{
		Scale = 200.0;
		IsDrawGrid = true;
		IsDrawEarthPile = false;
		EarthWidL = 5.0;
		EarthWidR = 5.0;
		_tcscpy(FrameBlockName, L"图框");	//图框名称
		TabTextH = 5.0;
		BZTextH = 4.0;
	}
};

//设置桩号性质	20190911新增
struct CrossType
{
	double Cml;
	int Type;		//0-正常（一般不用） 1-桥头	2-桥尾	3-隧头	4-隧尾	5-涵洞头	6-涵洞尾

	CrossType()
	{
		Cml = 0.0;
		Type = 0;
	}
};

//接坡信息
struct JPInfor 
{
	double Bef_H; //前接坡高程
	double Bef_PD; //前接坡坡度
	double Bef_PC; //前接坡坡长
	double Aft_H;	//后接坡高程
	double Aft_PD;	 //后接坡坡度
	double Aft_PC;	 //后接坡坡长
	CString RoadName; //线路名
	CString RailwayName;
	ACHAR stKml[80];
	ACHAR edKml[80];
	CString GH;
	double Stcml; //起始里程
	double Edcml; //终止里程
	int Is_JP; //0->不约束;1->约束
	//根据坡长判断前后端是否需要接坡约束
	int Is_JP_Bef; // 0->前不约束 1->约束
	int Is_JP_Aft; //0->后不约束 1->约束
	JPInfor()
	{
		Bef_H = 0.0;
		Bef_PD = 0.0;
		Bef_PC = 0.0;
		Aft_H = 0.0;
		Aft_PD = 0.0;
		Aft_PC = 0.0;
		Stcml = 0.0;
		Edcml = 0.0;
		RoadName = L"";
		RailwayName = L"";
		_tcscpy(stKml, L"");
		_tcscpy(edKml, L"");
		GH = L"";
		Is_JP = 0;
		Is_JP_Bef = 0;
		Is_JP_Aft = 0;
	}
};