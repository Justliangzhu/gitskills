// DMX.h: interface for the DMX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMX_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_)
#define AFX_DMX_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define AppName  /*MSG1*/L"GUADS" 
#include "triDTM_road.h"
#include "JD_CENTER.h"
#include "dbmain.h"
#include "Struct.h"


class DMX_ROAD : //自定义地面线类
//	public AcDbEntity,
//	public BAS_DRAW_FUN
 
//	public triDTM
	public JD_CENTER
{
public:
	int get_axle_legth();
	int textStyle();
	int Boxw(AcGiWorldDraw* pWd,AcGePoint3d& UpLPt,AcGePoint3d& DownRPt, int icolor,AcDb::LineWeight lw,AcDbObjectId layerId);
	void draw_frame(AcGiWorldDraw* pWd);//画图框
	void DrawConnectSlope(AcGiWorldDraw* pWd);//绘接坡
	void Modify_DATA(int num,double array[]);
	AcDbObjectId CreateLayer(ACHAR *,ACHAR *);
	int Linew(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int color,AcDb::LineWeight lw,AcDbObjectId layerId);
	void draw_AXLE(AcGiWorldDraw* pWd);

	void Get_Frame_Infor(double& Left, double& Top, double& Right, double& Bottom);
	double MLToX(double cml);
	double LevelToY(double Level);
	void set_ori_scale();
	void get_ori_scale();
	AcGePoint2d Data_drawtoreal(AcGePoint2d pt);
	void Data_realtodraw();
	double CalDMGC(double cml);//输入里程,计算地面高程
	JD_CENTER *m_gtzx;
	ACRX_DECLARE_MEMBERS(DMX_ROAD);
	DMX_ROAD();
	virtual ~DMX_ROAD();
	DMX_ROAD(int num,double array[],int zdmno,int zxorbx);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	int mode_display;
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,AcDbIntArray& osnapModes,AcDbIntArray& geomIds)const;
    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset);
	virtual void subList() const;
	
    double NCheight(double cml);//input cml,output height using DMXys array;
	double HBVScale;
	DMXYS DMXys[10000];//暂时
	double ZMax,ZMin;//地面线图最小最大高程
	double x_ori,y_ori;//地面线图原点坐标
	double H_ori;//地面线原点处标高
	double HScale,VScale;//地面线图纵横编比例
	double startLC,endLC;
	int Zdmno;//纵断面号
	int ZxOrBx;//1 中线 0 边线
	double ZJ;
	int Color;//地面线颜色;
	std::vector<JPInfor>m_JPInforArray_DMX; //绘接坡信息
	std::vector<GCKZPt>m_LevelCtrlPtArray_DMX; //绘接坡信息

	Adesk::Int32 NUM;//地面线点数//dwgIN.dwgout.
	AcDbObjectId NetId,AxleId,FrameId;
};
int MAKE_ACDBOPENOBJECT_FUNCTION(DMX_ROAD);//?
#endif // !defined(AFX_DMX_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_)
