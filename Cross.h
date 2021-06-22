// CROSS.h
// by Puhao
// "Programming AutoCAD with ObjectARX"

#include "DesignLine.h"
#include "LTGou.h"
#include "Hdmtext.h"
#include "earthline.h"
#include "BAS_DRAW_FUN.h"
#include "JSGou.h"
#include "DesLineReact.h"
#include "LTGouReactor.h"
#include "JSGouReactor.h"	//20190927新增

class CROSS : public BAS_DRAW_FUN//管理类
{
	public:
    	CROSS();	// Constructor
		~CROSS();	// Destructor
		void FreeRoom();

	    void DrawCrossCenLine(AcDbObjectId layerId,double NoteTextH=4.0);
		void Draw();
		void NoteDist(AcGePoint3d Spt,AcGePoint3d Ept,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,AcDbObjectId LayerId);
		AcDbObjectId AddEntityToModal(AcDbObjectId layerId,AcDbEntity *entity);
	  	AcDbObjectId H_maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,AcDbObjectId layerId);
		void OutDraw(double TabTextH=5.0,double NoteTextH=4.0);
	public:
		int BzTWInf;//-1 标注填挖下移
		ACHAR  m_ckml[256];
		ACHAR  m_FLDckml[256];
		double m_cml;
		double m_ZTWH,m_YTWH,m_YZTWH,m_YYTWH;//填挖高
		double m_FillArea;//填方面积
		double m_CutArea;//挖方面积
		double m_LeftZdk;//左侧占地宽
		double m_RightZdk;//右侧占地宽
		double m_BTinf;
		double m_YBTinf;
		double m_EarH;
		double Ymin,Ymax;
		double xjj;//线间距
		double offset,Yoffset;

		//20190822新增	改沟参数
		bool m_IsModifyGutter;		//是否是改沟模式
		CString m_GutterPath;		//改沟图块路径
		double m_GutterThick;		//水沟沟底厚

		//20190829新增	是否绘制占地桩
		bool m_IsDrawCrossEarthPile;

		//20190903新增	网格中心坐标
		double m_NetCenterY;

		//20190910新增	水沟图块名称
		ACHAR m_GutterBlockName[256];
	
		int DorS;//单路面或双路面
		int IfSetDQ;
		AcGePoint3d m_OriPoint;//绘图原点
		double m_Scale;	//比例尺
		double DX;
		//地面点
		EarthLine_ROAD   *LeftEarthLinePart,*RightEarthLinePart;
		//设计点
		DesignLine_ROAD  *LeftDesignLinePart,*RightDesignLinePart;
		DesignLine_ROAD  *Y_LeftDesignLinePart,*Y_RightDesignLinePart;
		//水沟
		LTGOU_ROAD       *LeftLTGouPart,*RightLTGouPart;
		LTGOU_ROAD       *Y_LeftLTGouPart,*Y_RightLTGouPart;
		JSGOU_ROAD       *LeftJSGouPart,*RightJSGouPart;
		JSGOU_ROAD       *Y_LeftJSGouPart,*Y_RightJSGouPart;

		//注字
		HdmText_ROAD     *Note;
	   AcDbObjectId LearthId,RearthId,LgouId,RgouId,LJsgId,RJsgId,LdesignId,RdesignId,NoteId;
	   AcDbObjectId Y_LgouId,Y_RgouId,Y_LJsgId,Y_RJsgId,Y_LdesignId,Y_RdesignId;
	   
	   DesLineReact *LDesReactor;
	   DesLineReact *RDesReactor;
	   DesLineReact * Y_RDesReactor;

	   LTGouReactor *LLTGouReactor;
	   LTGouReactor *RLTGouReactor;
	   LTGouReactor *Y_RLTGouReactor;

	   //20190927新增	截水沟反应器
	   JSGouReactor* LJSGouReactor;
	   JSGouReactor* RJSGouReactor;
	   JSGouReactor* Y_RJSGouReactor;

	   bool IsLinkHdm;

	   void wr_BG();
	   void wr_TW();
	   void DrawCrossDQLine(AcGePoint3d pjpt);

	   void DrawCrossInfor(double TextH=5.0);	//绘制横断面信息	20190815新增
	   void CalSlopeRateACHAR(double m, ACHAR Str[]);
	   double Round(double Value);

	   void DrawGutterBlock(AcGePoint3d OriPt);

	   void DrawZDK();	//绘制占地宽信息	20190823新增

	   void DrawCrossInfor_Gutter();	//绘制改沟横断面信息	20190829

	   double OffsetByXLZX;//路基中线相对于线路中线的偏移量
	   double m_EarHOnLJZX;//路基中线地面标高

	   double m_SuphRate_L, m_SuphRate_R;
};
