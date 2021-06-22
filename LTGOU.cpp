// LTGOU.cpp: implementation of the LTGOU class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LTGOU.h"
#include "EarthLine.h"
#include "JSGOU.h"
#include "HdmText.h"
#include "HdmDes.h"
#include "designline.h"

#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
	else if ((x) > 0)    \
	y =1;             \
	else                 \
	y = -1;           \
}
extern double Sfactor;
extern HdmDes HDM;

extern AcDbVoidPtrArray DesLineInfor;//设计线实体信息，ID
extern struct ent_Id_Des{
	AcDbObjectId entityId;
	double cml;
	double zory;
	int xlxz;
	int r_bh;
};
extern EarthLine_ROAD* my_l_earthline;
extern EarthLine_ROAD* my_r_earthline;
extern LTGOU_ROAD *    my_LTGOU;
extern JSGOU_ROAD *    my_JSGOU;
extern HdmText_ROAD *  my_Note;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ACRX_DXF_DEFINE_MEMBERS(LTGOU_ROAD, AcDbEntity,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0, LTGOU_ROAD,RailHdm
						);

//ACRX_DXF_DEFINE_MEMBERS(LTGOU, AcDbEntity, 1, LTGOU_ENTITY, ltgou_entity);
//MAKE_ACDBOPENOBJECT_FUNCTION(LTGOU);
LTGOU_ROAD::LTGOU_ROAD()
{

	m_LTGouPointNum = 0;
	m_GouBotPoint.x = 0.0,m_GouBotPoint.y = 0.0;
	m_OriGouBotPt = m_GouBotPoint;
	m_GouBotH=m_GouBotX=m_XtoPJ=0.0;
	m_PJDpt.x=m_PJDpt.y=0.0;
	//	HPDK=HPDI=GH=GW=SP1=SP2=PSG_JQHD=0.0;
	GouProperty=0;
	m_T2W=m_T1W=m_T2H=m_GH=m_GW=m_HH=m_SP1=m_SP2=m_SP3=m_PSG_GDR = 0.0;
	int i;
	for(i=0; i<18; i++)
	{
		m_LTGouPoints[i].x=0.0;
		m_LTGouPoints[i].y=0.0;
	}
	JqPtNum = 0;
	for(i=0; i<19; i++)
	{
		JqPt[i].x = 0.0;
		JqPt[i].y = 0.0;
	}
	ifMoveGripPoints = false;
	m_IfSetBotH = 0;
}

LTGOU_ROAD::~LTGOU_ROAD()
{

}
Adesk::Boolean LTGOU_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{

	//	assertReadEnabled();
	/*
	if(m_LTGouPointNum==0)//无水沟
	return Adesk::kFalse;*/
	int inf;
	inf=0;
	if((GouProperty==1 || GouProperty == 3)&&m_LTGouPointNum>0)
	{
		if(ifMoveGripPoints)
		{

			double des_cml;
			int des_zory;
			AcDbObjectId desId;
			int i;
			for( i=0;i<DesLineInfor.length();i++)
			{
				des_cml=((ent_Id_Des* )DesLineInfor[i])->cml;
				des_zory=((ent_Id_Des* )DesLineInfor[i])->zory;
				if(fabs(m_cml-des_cml)<0.1&&m_ZorY==des_zory) 
				{
					inf=1;
					break;//里程和边侧都要相同
				}
			}

			if(inf)
			{

				desId=((ent_Id_Des *)DesLineInfor[i])->entityId;
				DesignLine_ROAD * designline=NULL;

				designline=new DesignLine_ROAD;
				if(acdbOpenObject(designline,desId,AcDb::kForRead)==Acad::eOk)
				{
					//					designline->close();
					designline->CalBP();
					HDM.SDM_AutoDesign(designline,l_earthline,this,JSGou,r_earthline,Note);
					designline->close();
				}				
			}
			else
				ads_printf(L"找不到对应设计线实体!\n");

			ifMoveGripPoints=false;//夹点操作时在undo时只调用响应器和worlddraw，不调用MoveGripPoints，不重新计算边坡点
		}
		pWd->subEntityTraits().setColor(3);
		pWd->geometry().polyline(m_LTGouPointNum,m_LTGouPoints);
		AcGePoint3d lpt[2];
		//将坡脚点与水沟第一点连接

		lpt[0] = m_LTGouPoints[0];
		lpt[1] = m_PJDpt;
		if(lpt[0].y < lpt[1].y+0.01)
			pWd->geometry().polyline(2,lpt);

		//	做水沟的沟底标高显示
		AcGePoint3d origin;
		ACHAR Str[20];
		AcGePoint3d pt[4];
		if(m_LTGouPointNum>4)
		{
			if (m_LTGouPointNum==6)
			{
				m_GouBotPoint.x = 0.5*(m_LTGouPoints[2].x+m_LTGouPoints[3].x);
				m_GouBotPoint.y = 0.5*(m_LTGouPoints[2].y+m_LTGouPoints[3].y);
			}
			else if (m_LTGouPointNum==16)
			{
				m_GouBotPoint.x = m_LTGouPoints[8].x;
				m_GouBotPoint.y = m_LTGouPoints[8].y;
			}
			else if (m_LTGouPointNum>14)
			{
				m_GouBotPoint.x = m_LTGouPoints[8].x;
				m_GouBotPoint.y = m_LTGouPoints[8].y + PSG_JQHD*Sfactor;
			}
			else
			{
				m_GouBotPoint.x = 0.5*(m_LTGouPoints[2].x+m_LTGouPoints[3].x);
				m_GouBotPoint.y = 0.5*(m_LTGouPoints[2].y+m_LTGouPoints[3].y) + PSG_JQHD*Sfactor;
			}

			pt[0] = m_GouBotPoint;//
			pt[1].x = m_GouBotPoint.x - 0.2;
			pt[1].y = m_GouBotPoint.y + 0.3;
			pt[2].x = m_GouBotPoint.x + 0.2;
			pt[2].y = m_GouBotPoint.y + 0.3;
			pt[3] = pt[0];
			pWd->subEntityTraits().setColor(6);
			pWd->subEntityTraits().setFillType(kAcGiFillAlways);
			pWd->geometry().polygon(4,pt);

			origin.x = m_GouBotPoint.x;
			origin.y = m_GouBotPoint.y + 0.5;
			origin.z = m_GouBotPoint.z;
			AcGeVector3d direction(1.0, 0.0, 0.0);
			AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面

			acdbRToS(m_GouBotH,2,3,Str);
			pWd->subEntityTraits().setColor(2);
			pWd->geometry().text(origin, normal, direction, 1,
				0.8, 0.0, Str);
			//水沟浆切
			pWd->subEntityTraits().setColor(2);
			pWd->geometry().polyline(JqPtNum,JqPt);
		}

	}
	//坡脚墙
	/*	if(GouProperty==0&&pjqptnum>0)
	{
	pWd->subEntityTraits().setColor(2);
	pWd->geometry().polyline(pjqptnum,PjqPt);
	}*/


	return Adesk::kTrue;
}
void LTGOU_ROAD::subList() const
{
	assertReadEnabled();

	AcDbEntity::subList();

	ads_printf(L"\n\t\t自定义横断面路堤水沟实体");
	//	ads_printf(L"\n\t\t里程 = %s %s", m_ckml,m_ZorY);
	//	ads_printf(L"\n\t\t地面点数 = %d", m_EarthPointNum);
	//	ads_printf(L"\n\t\t水沟结构尺寸:");
	//	ads_printf(L"\n\t\t深=%0.2lfm 底宽=%0.2lfm 内侧边坡=1:%0.2lf 外侧边坡=1:%0.2lf", m_GH,m_GW,m_SP1,m_SP2);

	//	for(i=0; i<m_LTGouPointNum; i++)
	//	   ads_printf(L"\n\t\tx=%0.3lf y=%0.3lf", m_LTGouPoints[i].x,m_LTGouPoints[i].y);
}

Acad::ErrorStatus LTGOU_ROAD::subTransformBy(const AcGeMatrix3d& xform)
{
	int i;

	if(m_LTGouPointNum==0)
		return AcDbEntity::subTransformBy(xform);
	//	m_FillArea=0.0;

	m_OriGouBotPt = m_GouBotPoint; 

	assertWriteEnabled();	
	m_GouBotPoint.transformBy(xform);

	for(i=0; i<m_LTGouPointNum; i++)
		m_LTGouPoints[i].transformBy(xform);

	m_GouBotH = m_GouBotH + (m_GouBotPoint.y - m_OriGouBotPt.y)/Sfactor;
	m_GouBotX = m_GouBotX + (m_GouBotPoint.x - m_OriGouBotPt.x)/Sfactor;

	return AcDbEntity::subTransformBy(xform);
}

int LTGOU_ROAD::Cal_Yd(double x,double &Yd,int &ptnum)
{
	int i;

	i = 0;
	Yd = m_LTGouPoints[0].y;
	ptnum = 0;
	while(m_ZorY*x < m_ZorY*(m_LTGouPoints[i].x+0.001) && i<m_LTGouPointNum)
		i++;
	if(i==m_LTGouPointNum)
	{
		Yd = m_LTGouPoints[i-1].y;
		ptnum = i-1;
		return 0;//找不到
	}
	if(i==0)
		return -1;		
	ptnum = i;
	Yd = m_LTGouPoints[i-1].y + (x-m_LTGouPoints[i-1].x)*(m_LTGouPoints[i].y-m_LTGouPoints[i-1].y)/(m_LTGouPoints[i].x-m_LTGouPoints[i-1].x);
	return 1;//找到
}
Adesk::Boolean LTGOU_ROAD::Draw(int iColor)
{
	assertReadEnabled();
	if (m_LTGouPointNum==6 || m_LTGouPointNum==16)
		m_GouBotPoint.y = m_GouBotPoint.y;
	else
		m_GouBotPoint.y = m_GouBotPoint.y + PSG_JQHD*Sfactor;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"横断面图-路堤水沟");
	AcDbObjectId layerId = CreateLayer(LayerName);
	AcGePoint3dArray ptarray,ptarray1;
	int i;
	if((GouProperty==1 || GouProperty == 3)&&m_LTGouPointNum>0)		//20200430新增GouProperty==3状况
	{
		//if(m_LTGouPointNum==0)//无水沟
		//return Adesk::kFalse;
		for(i=0; i<m_LTGouPointNum; i++)
			ptarray.append(m_LTGouPoints[i]);
		make2dline(ptarray,iColor,0.5,0.5, layerId);

		ACHAR Str[20];
		_stprintf(Str,L"%.3lf",m_GouBotH);	//标高
		//字高1.5
		NoteBG(m_GouBotPoint, 1.5, Str, 6, 2, Sfactor, 2,LayerName);

		for(i=0; i<JqPtNum; i++)
			ptarray1.append(JqPt[i]);
		make2dline(ptarray1,iColor,0.5,0.5, layerId);
	}


	return Adesk::kTrue;
}

Acad::ErrorStatus  LTGOU_ROAD::subGetOsnapPoints(
	AcDb::OsnapMode osnapMode,
	int gsSelectionMark,
	const AcGePoint3d & pickPoint,
	const AcGePoint3d & lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray & snapPoints,
	AcDbIntArray& geomIds) const
{

	assertReadEnabled();
	Acad::ErrorStatus es=Acad::eOk;
	//	if(gsSelectionMark==0) return es;
	//	if(osnapMode!=AcDb::kOsModeEnd&&osnapMode!=AcDb::kOsModeNear
	//		&&osnapMode!=AcDb::kOsModeMid&&osnapMode!=AcDb::kOsModePerp
	//		&&osnapMode!=AcDb::kOsModeCen&&osnapMode!=AcDb::kOsModeIns) 
	//	{return es;}
	//	switch(osnapMode)
	//	{
	//	case AcDb::kOsModeEnd:
	//		{	
	//			for(int i=0;i<m_LTGouPointNum;i++)
	//				snapPoints.append(m_LTGouPoints[i]);
	//			break;
	//		}
	//	default:     return es;
	//	}
	return es;

}
Acad::ErrorStatus LTGOU_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											   AcDbIntArray& osnapModes,
											   AcDbIntArray& geomIds) const
{


	assertReadEnabled();//重载CAD函数用assertReadEnabled()
	int i;
	for(i=0; i<m_LTGouPointNum; i++)
		gripPoints.append(m_LTGouPoints[i]);
	gripPoints.append(m_GouBotPoint);
	return Acad::eOk;
}

Acad::ErrorStatus LTGOU_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,
												  const AcGeVector3d& offset)
{


	assertWriteEnabled();
	ifMoveGripPoints=true;

	int k,zory,inf,yinf;
	if(indices.length()==0||offset.isZeroLength())
		return Acad::eOk;
	sign(zory,m_ZorY);
	sign(inf,offset[X]);
	sign(yinf,offset[Y]);


	for(int i=0;i<indices.length();i++)
	{
		k=indices[i];
		if(k==0)
		{
			if(fabs(offset[Y])>0.1)//改变植树带高
				m_HH = m_HH+offset[Y]/Sfactor;
		}
		if(k==1)
		{
			if(fabs(offset[Y])>1)//改变植树带高
				m_HH = m_HH+offset[Y]/Sfactor;
			else//改变植树带宽
				m_T1W = m_T1W +  zory*inf*sqrt(offset[X]*offset[X]+offset[Y]*offset[Y])/Sfactor;
		}
		if(k==2 || k==3)
		{
			if(fabs(offset[Y])>1)//改变沟深
			{
				m_GH = m_GH + offset[Y]/Sfactor;
				if(m_GH < 0.6)
				{
					m_GH = 0.6;
					//改变植树带高
					m_HH = m_HH+offset[Y]/Sfactor;
				}
			}
			else
				m_T1W = m_T1W +  zory*inf*sqrt(offset[X]*offset[X]+offset[Y]*offset[Y])/Sfactor;
		}
		if(k==4)
		{
			if(fabs(offset[Y])>1)//改变土埂高
			{
				m_T2H = m_T2H + offset[Y]/Sfactor;
				if(m_T2H<0.0)
					m_T2H = 0.0;
			}
			else//改变植树带宽
				m_T1W = m_T1W +  zory*inf*sqrt(offset[X]*offset[X]+offset[Y]*offset[Y])/Sfactor;

		}
		if(k==5)
		{
			if(fabs(offset[Y])>1)//改变土埂高
			{
				m_T2H = m_T2H + offset[Y]/Sfactor;
				if(m_T2H<0.0)
					m_T2H = 0.0;
			}
			else//改变土埂宽
				m_T2W = m_T2W +  zory*inf*sqrt(offset[X]*offset[X]+offset[Y]*offset[Y])/Sfactor;
		}
		if(k==6)//改变土埂边坡
		{
			double x,y,bx,by;
			x = m_LTGouPoints[k].x + offset[X];
			y = m_LTGouPoints[k].y + offset[Y];
			bx = m_LTGouPoints[k-1].x ;
			by = m_LTGouPoints[k-1].y ;
			if(fabs(y-by)>0.001)
				m_SP3 = fabs(x-bx)/fabs(y-by);
			else
				m_SP3 = 1.0E6;

		}
	}

	return Acad::eOk;
}
//dwgin和dwgout读取循序要一致
Acad::ErrorStatus LTGOU_ROAD::dwgInFields(AcDbDwgFiler * filer)
{

	assertWriteEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgInFields(filer))!=Acad::eOk)
		return es;

	filer->readItem(&m_LTGouPointNum);
	int i;
	for( i=0;i<m_LTGouPointNum;i++)
		filer->readPoint3d(&m_LTGouPoints[i]);

	filer->readItem(&m_T2W);
	filer->readItem(&m_T1W);
	filer->readItem(&m_T2H);
	filer->readItem(&m_GH);
	filer->readItem(&m_GW);
	filer->readItem(&m_HH);
	filer->readItem(&m_SP1);
	filer->readItem(&m_SP2);
	filer->readItem(&m_SP3);
	filer->readItem(&PSG_JQHD);
	filer->readItem(&m_PSG_GDR);


	filer->readItem(&JqPtNum);
	for(i=0;i<JqPtNum;i++)
		filer->readPoint3d(&JqPt[i]);
	filer->readPoint3d(&m_GouBotPoint);
	filer->readPoint3d(&m_OriGouBotPt);
	filer->readItem(&m_GouBotH);//
	filer->readItem(&m_GouBotX);
	filer->readItem(&m_cml);
	filer->readInt16(&GouProperty);


	filer->readItem(&m_ZorY);
	filer->readInt16(&m_IfSetBotH);
	filer->readPoint3d(&m_PJDpt);
	//	filer->readItem(&m_FillORCut);

	//void *tpointer;
	//filer->readAddress(&tpointer);
	//l_earthline=(EarthLine_ROAD*)tpointer;
	//filer->readAddress(&tpointer);
	//r_earthline=(EarthLine_ROAD*)tpointer;
	//filer->readAddress(&tpointer);
	//JSGou=(JSGOU_ROAD *)tpointer;
	//filer->readAddress(&tpointer);
	//Note=(HdmText_ROAD *)tpointer;
	l_earthline=my_l_earthline;
	r_earthline=my_r_earthline;
	JSGou=my_JSGOU;
	Note=my_Note;

	return filer->filerStatus();
}

Acad::ErrorStatus LTGOU_ROAD::dwgOutFields(AcDbDwgFiler * filer) const
{

	assertReadEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgOutFields(filer))!=Acad::eOk)
		return es;


	filer->writeItem(m_LTGouPointNum);
	int i;
	for( i=0;i<m_LTGouPointNum;i++)
		filer->writePoint3d(m_LTGouPoints[i]);

	filer->writeItem(m_T2W);
	filer->writeItem(m_T1W);
	filer->writeItem(m_T2H);
	filer->writeItem(m_GH);
	filer->writeItem(m_GW);
	filer->writeItem(m_HH);
	filer->writeItem(m_SP1);
	filer->writeItem(m_SP2);
	filer->writeItem(m_SP3);
	filer->writeItem(PSG_JQHD);
	filer->writeItem(m_PSG_GDR);


	filer->writeItem(JqPtNum);
	for(i=0;i<JqPtNum;i++)
		filer->writePoint3d(JqPt[i]);

	filer->writePoint3d(m_GouBotPoint);
	filer->writePoint3d(m_OriGouBotPt);
	filer->writeItem(m_GouBotH);//
	filer->writeItem(m_GouBotX);
	filer->writeItem(m_cml);
	filer->writeInt16(GouProperty);

	filer->writeItem(m_ZorY);
	filer->writeInt16(m_IfSetBotH);
	filer->writePoint3d(m_PJDpt);
	//	filer->writeItem(m_FillORCut);

	//filer->writeAddress(l_earthline);
	//filer->writeAddress(r_earthline);
	//filer->writeAddress(JSGou);
	//filer->writeAddress(Note);	
	my_l_earthline=l_earthline;
	my_r_earthline=r_earthline;
	my_JSGOU=JSGou;
	my_Note=Note;


	return filer->filerStatus();

}

//设置侧沟参数
void LTGOU_ROAD::SetGutterPara(double T2W, double T1W, double T2H, double GH, double GW, double HH, double SP1, double SP2, double SP3, double JQHD, double GDR)
{
	m_T2W = T2W;
	m_T1W = T1W;
	m_T2H = T2H;
	m_GH = GH;
	m_GW = GW;
	m_HH = HH;
	m_SP1 = SP1;
	m_SP2 = SP2;
	m_SP3 = SP3;
	PSG_JQHD = JQHD;
	m_PSG_GDR = GDR;
}

//删除水沟	20190930新增
void LTGOU_ROAD::DeleteGutter()
{
	SetGutterPara(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	m_LTGouPointNum = 0;
	m_IfSetBotH = 0;
	m_GouBotH = 0.0;
	GouProperty = -1;
}
