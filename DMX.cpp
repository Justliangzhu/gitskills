// DMX.cpp: implementation of the DMX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "resource.h"
#include "DMX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
AcGiWorldDraw* pWd;
extern JD_CENTER  *pzLinep;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern triDTM *ptriDTM;
DMX_ROAD::DMX_ROAD(int num,double array[],int zdmno,int zxorbx)//array[0]--lc,array[1]--level
{
	//	acedAlert(L"DMX");
	Zdmno = zdmno;//纵断面图编号
	NUM=num;
	ZxOrBx = zxorbx;

	//将地面线数据传入到类成员中
	if (NUM > 0)
	{
		ZMax = -99999999;
		ZMin = 99999999;
		CString  gh;
		double xlc;
		for (int i = 0; i < NUM; i++)
		{
			DMXys[i].lc = array[2 * i];
			DMXys[i].level = array[2 * i + 1];

			xlc = pzLinep->XLC(DMXys[i].lc, gh);
			_tcscpy(DMXys[i].ckml, L"\0");
			_tcscpy(DMXys[i].ckml, pzLinep->LCchr(gh, xlc, 2));
			if(DMXys[i].level > -80)
			{
				ZMin = ZMin < DMXys[i].level ? ZMin : DMXys[i].level ;
				ZMax = ZMax > DMXys[i].level ? ZMax : DMXys[i].level ;
			}
		}
	}
	else
	{
		ZMin = array[0];
		ZMax = array[1];
	}
	//	startLC=DMXys[0].lc;
	//	endLC=DMXys[NUM-1].lc; //2005.1.11 解决地面线的第一个点不等于线路起点里程时，纵断面设计地面线总绘在起点处
	if(ZxOrBx)//中线
	{

		startLC = pzLinep->DLArray[0].BLC + pzLinep->DLArray[0].DL;
		//	endLC = pzLinep->XLLength;		
		endLC = pzLinep->TYLC(pzLinep->DLArray[pzLinep->DLNum - 1].BLC);
		//ads_printf(L"@@@@@@@@@@@@@@@ lc=%lf %lf\n",startLC,endLC);			
		pORI_SCALE.STARTLC = startLC;//起始，终止里程
		pORI_SCALE.ENDLC = endLC;
		int tmp  = (int)floor(ZMin/10.0);////////////////p///
		ZMin = tmp * 10.0;
		tmp = (int)floor(ZMax / 10.0) + 1;
		ZMax = tmp * 10.0;
		if (ZMax - ZMin < 20)
		{
			ZMax = ZMax + 10;
			ZMin = ZMin - 10;
		}

		set_ori_scale();
	}
	else//边线
		get_ori_scale();

	//	acutPrintf(L"\nzmin=%lf,zmax=%lf",ZMin,ZMax);
	//	if (Zmax-Zmin<140.0) Zmax = Zmin+140.0;
	//create layer
	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"DOT2",L"acadiso.lin");//
	NetId = CreateLayer(L"NET",L"Dot2");
	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
	AxleId = CreateLayer(L"AXLE",L"CONTINUOUS");
	//acdbHostApplicationServices()->workingDatabase()->
	//	loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
	FrameId = CreateLayer(L"FRAME",L"CONTINUOUS");



	//create textstyle
	textStyle();

}
DMX_ROAD::DMX_ROAD()
{
	m_gtzx=NULL;
	m_JPInforArray_DMX.clear();
	m_LevelCtrlPtArray_DMX.clear();
}

DMX_ROAD::~DMX_ROAD()
{
	//delete []DMXys;	//这个地方会引起CAD关闭时莫名崩溃，先暂时隐藏观察是否会存在什么问题	20190712
}
Adesk::Boolean  DMX_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{   
	//acedAlert(L"DMX::worldDraw");
	AcGePoint3d Pt;

	//double DesH = 0.0;
	//if (m_LevelCtrlPtArray_DMX.size() > 0)
	//{
	//	//考虑交叉点
	//	for (int i = 0; i < m_LevelCtrlPtArray_DMX.size(); i++)
	//	{
	//		if (m_LevelCtrlPtArray_DMX[i].Note.Find(L"交叉") >= 0)
	//		{
	//			if (m_LevelCtrlPtArray_DMX[i].Style == 1)
	//			{
	//				DesH = m_LevelCtrlPtArray_DMX[i].DesHSX;

	//				ZMin = ZMin < DesH ? ZMin : (DesH - 5);
	//				ZMax = ZMax > DesH ? ZMax : (DesH + 5);
	//			}
	//			else if (m_LevelCtrlPtArray_DMX[i].Style == 2)
	//			{
	//				DesH = m_LevelCtrlPtArray_DMX[i].DesHXX;
	//				ZMin = ZMin < DesH ? ZMin : (DesH - 5);
	//				ZMax = ZMax > DesH ? ZMax : (DesH + 5);
	//			}
	//		}
	//	}
	//}
	//if (ZxOrBx)//中线
	//	set_ori_scale();
	//else//边线
	//	get_ori_scale();

	pzLinep = GlobalZDMTK.ZDMTK[Zdmno].pm;	
	Data_realtodraw();//转化为图上坐标

	AcGePoint3d verts[2];
	AcGeVector3d  normal(0.0,0.0,1.0);

	int j=1;
	AcDbObjectId DmxId = CreateLayer(L"DMX",L"CONTINUOUS");
	if(ZxOrBx)
	{
		pWd->subEntityTraits().setColor(3);//夹直线
		for (int i = 0; i < NUM-1; i++) 
		{
			verts[0].x=DMXys[i].sx; verts[0].y=DMXys[i].sy;verts[0].z=0.0;
			verts[1].x=DMXys[i+1].sx; verts[1].y=DMXys[i+1].sy;verts[1].z=0.0;
			if (pWd!= NULL) 
			{
				pWd->subEntityTraits().setLayer(DmxId);//先设层
				pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);//先设层
				if(j<pzLinep->XYNum)
				{
					if(DMXys[i+1].lc<pzLinep->XYArray[j][6])
					{
						if(pzLinep->XYArray[j-1][0]<1.2)
							pWd->subEntityTraits().setColor(3);//夹直线
						else if(pzLinep->XYArray[j-1][0]<2.2)//circle
							pWd->subEntityTraits().setColor(2);
						else
							pWd->subEntityTraits().setColor(1);//sipral
					}
					else
						j++;

				}
				else//确定终点线元
				{
					if(pzLinep->XYArray[j-1][0]<1.2)
						pWd->subEntityTraits().setColor(3);//夹直线
					else if(pzLinep->XYArray[j-1][0]<2.2)//circle
						pWd->subEntityTraits().setColor(2);
					else
						pWd->subEntityTraits().setColor(1);//sipral
				}
				if(DMXys[i].level>-99 && DMXys[i+1].level>-99)//必须在数模内
					pWd->geometry().polyline(2,verts);
			} 
			else
			{
				assert(Adesk::kFalse);//?	
			}
		}
		draw_AXLE(pWd);
		draw_frame(pWd);
	}
	else
	{
		pWd->subEntityTraits().setColor(Color);
		for (int i = 0; i < NUM-1; i++) 
		{
			verts[0].x=DMXys[i].sx; verts[0].y=DMXys[i].sy;verts[0].z=0.0;
			verts[1].x=DMXys[i+1].sx; verts[1].y=DMXys[i+1].sy;verts[1].z=0.0;
			if(DMXys[i].level>-99 && DMXys[i+1].level>-99)//必须在数模内
				pWd->geometry().polyline(2,verts);
		}
	}

	//绘前后接坡,根据勾选框判断是否绘接坡
 	if (m_JPInforArray_DMX.size()>0&&m_JPInforArray_DMX[0].Is_JP == 1)
		DrawConnectSlope(pWd);

	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

Acad::ErrorStatus DMX_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	//	acedAlert(L"dwgin1");
	assertWriteEnabled();
	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our owner as a hard
	// pointer ID so now we need to read it in to keep things
	// in sync.
	//
	if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}	
	/////////////////add///////////////
	pFiler->readItem( &Zdmno);
	pFiler->readItem( &NUM);
	pFiler->readItem( &startLC);
	pFiler->readItem( &endLC);
	pFiler->readItem( &H_ori);
	pFiler->readItem( &HBVScale);
	pFiler->readItem( &ZMax);
	pFiler->readItem( &ZMin);
	pFiler->readItem( &x_ori);
	pFiler->readItem( &y_ori);
	pFiler->readItem(&HScale);	//20190709	改用横纵轴双参数绘制
	pFiler->readItem(&VScale);
	for (int i=0;i<(int)(NUM);i++)
	{  
		pFiler->readItem( &DMXys[i].lc);
		pFiler->readItem( &DMXys[i].level);
	}

	ACHAR *tempchar = NULL;
	int JPNum = 0, GCKZDNum = 0;

	pFiler->readItem(&JPNum);
	m_JPInforArray_DMX.clear();
	m_JPInforArray_DMX.resize(JPNum);
	for (int i =0;i <JPNum;i++)
	{
		pFiler->readItem(&m_JPInforArray_DMX[i].Aft_H);
		pFiler->readItem(&m_JPInforArray_DMX[i].Aft_PC);
		pFiler->readItem(&m_JPInforArray_DMX[i].Aft_PD);
		pFiler->readItem(&m_JPInforArray_DMX[i].Bef_H);
		pFiler->readItem(&m_JPInforArray_DMX[i].Bef_PC);
		pFiler->readItem(&m_JPInforArray_DMX[i].Bef_PD);
		pFiler->readItem(&m_JPInforArray_DMX[i].Stcml);
		pFiler->readItem(&m_JPInforArray_DMX[i].Edcml);
		pFiler->readItem(&m_JPInforArray_DMX[i].Is_JP);
		pFiler->readItem(&m_JPInforArray_DMX[i].Is_JP_Aft);
		pFiler->readItem(&m_JPInforArray_DMX[i].Is_JP_Bef);
	}

	pFiler->readItem(&GCKZDNum);
	m_LevelCtrlPtArray_DMX.clear();
	m_LevelCtrlPtArray_DMX.resize(GCKZDNum);
	for (int i =0;i<GCKZDNum;i++)
	{
		pFiler->readItem(&m_LevelCtrlPtArray_DMX[i].Cml);
		pFiler->readItem(&m_LevelCtrlPtArray_DMX[i].XCLc);
		pFiler->readItem(&m_LevelCtrlPtArray_DMX[i].clearrence);
		pFiler->readItem(&m_LevelCtrlPtArray_DMX[i].DesHSX);
		pFiler->readItem(&m_LevelCtrlPtArray_DMX[i].DesHXX);
		pFiler->readItem(&m_LevelCtrlPtArray_DMX[i].Style);
	}

	//////////////////////////////
	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus DMX_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	/////////////////add///////////////
	pFiler->writeItem( Zdmno);
	pFiler->writeItem( NUM);
	pFiler->writeItem( startLC);
	pFiler->writeItem( endLC);
	pFiler->writeItem( H_ori);
	pFiler->writeItem( HBVScale);
	pFiler->writeItem( ZMax);
	pFiler->writeItem( ZMin);
	pFiler->writeItem( x_ori);
	pFiler->writeItem( y_ori);	
	pFiler->writeItem(HScale);	//20190709	改用横纵轴双参数绘制
	pFiler->writeItem(VScale);
	//	acutPrintf(L"\nwrite.dwgout NUM=%d",NUM);
	for (int i=0;i<(int)(NUM);i++)
	{
		pFiler->writeItem( DMXys[i].lc);
		pFiler->writeItem( DMXys[i].level);
	}


	int JPNum , GCKZDNum;
	JPNum = m_JPInforArray_DMX.size();
	GCKZDNum = m_LevelCtrlPtArray_DMX.size();

	pFiler->writeInt32(JPNum);
	for (int i = 0; i < JPNum; i++)
	{
		pFiler->writeItem(m_JPInforArray_DMX[i].Aft_H);
		pFiler->writeItem(m_JPInforArray_DMX[i].Aft_PC);
		pFiler->writeItem(m_JPInforArray_DMX[i].Aft_PD);
		pFiler->writeItem(m_JPInforArray_DMX[i].Bef_H);
		pFiler->writeItem(m_JPInforArray_DMX[i].Bef_PC);
		pFiler->writeItem(m_JPInforArray_DMX[i].Bef_PD);
		pFiler->writeItem(m_JPInforArray_DMX[i].Stcml);
		pFiler->writeItem(m_JPInforArray_DMX[i].Edcml);
		pFiler->writeItem(m_JPInforArray_DMX[i].Is_JP);
		pFiler->writeItem(m_JPInforArray_DMX[i].Is_JP_Aft);
		pFiler->writeItem(m_JPInforArray_DMX[i].Is_JP_Bef);
	}

	pFiler->writeItem(GCKZDNum);
	for (int i = 0; i < GCKZDNum; i++)
	{
		pFiler->writeItem(m_LevelCtrlPtArray_DMX[i].Cml);
		pFiler->writeItem(m_LevelCtrlPtArray_DMX[i].XCLc);
		pFiler->writeItem(m_LevelCtrlPtArray_DMX[i].clearrence);
		pFiler->writeItem(m_LevelCtrlPtArray_DMX[i].DesHSX);
		pFiler->writeItem(m_LevelCtrlPtArray_DMX[i].DesHXX);
		pFiler->writeItem(m_LevelCtrlPtArray_DMX[i].Style);
	}
	///////////////////////////////
	return pFiler->filerStatus();
}

// Files data in from a DXF file.
//
Acad::ErrorStatus DMX_ROAD::dxfInFields(AcDbDxfFiler* pFiler)//open
{
	//    acedAlert(L"dxfin1");
	assertWriteEnabled();
	Acad::ErrorStatus es;
	struct resbuf inbuf;//结果缓冲区
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"DMX")) {
		ads_printf(L"\n error 10010 类型有错 \n");
		return Acad::eBadDxfSequence;
	}
	/////////////////add///////////////

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	Zdmno= inbuf.resval.rint;	}
		else		{		}
	}

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	NUM= inbuf.resval.rint;	}
		else		{		}
	}

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal )
		{	startLC= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+1 )
		{	endLC= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+2 )
		{	H_ori= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+3 )
		{	HBVScale= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+5 )
		{	ZMax= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+6)
		{	ZMin= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+6)
		{	x_ori= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+6 )
		{	y_ori= inbuf.resval.rreal;	}
		else		{		}
	}

	//	acutPrintf(L"\ndxfin NUM=%d",NUM);
	for (int i=0;i<(int)(NUM);i++)
	{  
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+6)
			{
				DMXys[i].lc= inbuf.resval.rreal;
			}
			else		{		}
		}
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+7)
			{
				DMXys[i].level= inbuf.resval.rreal;
			}
			else		{		}
		}

	} 
	//	acedAlert(L"dxfin2");
	return pFiler->filerStatus();
}

// Files data out to a DXF file.
Acad::ErrorStatus DMX_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "DMX");///////////"classname  DMX"//add///////////
	pFiler->writeItem(AcDb::kDxfInt32,Zdmno);
	pFiler->writeItem(AcDb::kDxfInt32,NUM);
	pFiler->writeItem( AcDb::kDxfReal,startLC);
	pFiler->writeItem( AcDb::kDxfReal+1,endLC);
	pFiler->writeItem( AcDb::kDxfReal+2,H_ori);
	pFiler->writeItem( AcDb::kDxfReal+3,HBVScale);
	pFiler->writeItem( AcDb::kDxfReal+5,ZMax);
	pFiler->writeItem( AcDb::kDxfReal+6,ZMin);
	pFiler->writeItem( AcDb::kDxfReal+6,x_ori);
	pFiler->writeItem( AcDb::kDxfReal+6,y_ori);	

	//cutPrintf("\ndxfout NUM=%d",NUM);
	for (int i=0;i<(int)(NUM);i++)
	{
		pFiler->writeItem(AcDb::kDxfReal+6,DMXys[i].lc);
		pFiler->writeItem(AcDb::kDxfReal+7,DMXys[i].level);
	}
	return pFiler->filerStatus();
}
//地面线实际坐标转化为图坐标
void DMX_ROAD::Data_realtodraw()
{
	int Max=NUM-1;
	for(;Max>=0;Max--)
	{
		//DMXys[Max].sx=x_ori+(DMXys[Max].lc-startLC)/HBVScale;
		//DMXys[Max].sy=y_ori+(DMXys[Max].level-H_ori);
		//20190709	修改采用双轴比例尺
		DMXys[Max].sx = x_ori + (DMXys[Max].lc - startLC) / HScale;
		DMXys[Max].sy = y_ori + (DMXys[Max].level - H_ori) / VScale;
	}
}
double DMX_ROAD::NCheight(double cml)//input cml,output height using DMXys array;
{
	int i;
	double ht;
	for(i=0; i<NUM; i++)
	{
		if(cml <= DMXys[i].lc)
		{
			if(i==0)
				ht=DMXys[0].level;
			else
				ht = DMXys[i-1].level + (cml-DMXys[i-1].lc)*(DMXys[i].level-DMXys[i-1].level)/(DMXys[i].lc-DMXys[i-1].lc);
			return ht;
		}
	}
}
//纵断面图上地面线一点转化为实际坐标
AcGePoint2d DMX_ROAD::Data_drawtoreal(AcGePoint2d pt)
{
	AcGePoint2d pk;
	//pk.x = (pt.x - x_ori)*HBVScale + startLC;
	//pk.y = (pt.y - y_ori) + H_ori;
	//20190709	修改采用双轴比例尺
	pk.x = (pt.x - x_ori) * HScale + startLC;
	pk.y = (pt.y - y_ori) * VScale + H_ori;
	return pk;
}

//实际坐标转屏幕坐标
double DMX_ROAD::MLToX(double cml)
{
	double X = x_ori + (cml - startLC) / HScale;
	return X;
}

//实际坐标转屏幕坐标
double DMX_ROAD::LevelToY(double Level)
{
	double Y = y_ori + (Level - H_ori) / VScale;
	return Y;
}


void DMX_ROAD::subList() const 
{
	if(ZxOrBx)
		ads_printf(L"中线地面线\n");
	else
	{
		if(ZJ>0)
			ads_printf(L"右侧%0.1lf米边线地面线\n",ZJ);
		else
			ads_printf(L"左侧%0.1lf米边线地面线\n",fabs(ZJ));
	}
}

void DMX_ROAD::set_ori_scale()
{
	x_ori=pORI_SCALE.X_ORI;
	y_ori=pORI_SCALE.Y_ORI;//地面线图原点坐标
	H_ori=ZMin;//地面线原点处标高
	HBVScale=pORI_SCALE.HBVSCALE;//地面线图纵横编比例
	HScale = pORI_SCALE.HSCALE;	//20190709	修改采用双轴比例尺
	VScale = pORI_SCALE.VSCALE;
	pORI_SCALE.STARTLC=startLC;
	pORI_SCALE.ENDLC=endLC;
	pORI_SCALE.ZMIN=ZMin;
	pORI_SCALE.ZMTH = (ZMax - ZMin) / VScale;	//20190709	修改采用双轴比例尺
}

void DMX_ROAD::get_ori_scale()
{
	x_ori=GlobalZDMTK.ZDMTK[Zdmno].X0;
	y_ori=GlobalZDMTK.ZDMTK[Zdmno].Y0;//地面线图原点坐标
	H_ori=ZMin=GlobalZDMTK.ZDMTK[Zdmno].H0;//地面线原点处标高
	HBVScale=pORI_SCALE.HBVSCALE;//地面线图纵横编比例
}

void DMX_ROAD::draw_AXLE(AcGiWorldDraw* pWd)
{
	int color_net=9;
	int     j;
	int     begin, end;
	double  sx1,sy1, sx2,sy2,lev;
	ads_point pt;
	ACHAR str[20];

	begin = (int)ZMin; end = (int)ZMax;
	sx1 = sx2 = x_ori;
	sy1 = y_ori;
	sy2 = y_ori + (end - begin) / VScale ;
	AcGePoint3d startPt, endPt, startPt_axle, endPt_axle;
	startPt_axle.x = startPt.x = sx1;
	endPt_axle.x = endPt.x = sx2;
	startPt_axle.y = startPt.y = sy1;
	endPt_axle.y = endPt.y = sy2;
	sx2 = sx1 - 1.5;
	pt[0] = sx2;
	//竖直网格线
	for (j = begin; j <= end; j += 2)
	{
		sy1 = y_ori + (j - begin) / VScale;	//20190709	修改采用双轴比例尺
		sy2 = sy1;
		pt[1] = sy1 - 2;

		startPt.x = sx1;
		startPt.y = sy1;
		endPt.x = sx1 + (endLC - startLC) / HScale;	//20190709	修改采用双轴比例尺
		endPt.y = sy2;
		Linew (pWd,startPt,endPt,color_net,AcDb::kLnWt000,NetId);///net_H	
		startPt.x = sx2;
		startPt.y = sy1;
		endPt.x = sx1;
		endPt.y = sy2;
		Linew (pWd,startPt,endPt,3,AcDb::kLnWt000,AxleId);	

		lev = j;
		_stprintf(str,L"%0.0lf", lev);
		AcGePoint3d tPt(pt[0],pt[1],0.0);
		G_maketext(pWd, tPt, str, 0, pORI_SCALE.PDTextH, 7, 3);
	}
	//水平网格线
	pzLinep->FormLCB(100.0,startLC,endLC);
	for (int zhuang = 1; zhuang < pzLinep->NLCB; zhuang++)
	{
		startPt.x = x_ori + (pzLinep->pLCB[zhuang].lc - startLC) / HScale;	//20190709	修改采用双轴比例尺
		startPt.y = y_ori;
		endPt.x = startPt.x;
		endPt.y = startPt.y + (end - begin) / VScale;	//20190709	修改采用双轴比例尺
		if (pzLinep->pLCB[zhuang].lcflag == 10)
			Linew(pWd, startPt, endPt, 10, AcDb::kLnWt000, NetId);//net_V公里
		else if (pzLinep->pLCB[zhuang].lcflag < 0)
			Linew(pWd, startPt, endPt, 2, AcDb::kLnWt000, NetId);//断链
		else 
			Linew(pWd, startPt, endPt, 0, AcDb::kLnWt000, NetId);//net_V
	}	

	startPt.x = x_ori + (endLC - startLC) / HScale;	//20190709	修改采用双轴比例尺
	startPt.y = y_ori;
	endPt.x = startPt.x;
	endPt.y = startPt.y + (end - begin) / VScale;
	Linew (pWd,startPt,endPt,color_net,AcDb::kLnWt000,NetId);//net_V->endlc
	Linew (pWd,startPt_axle,endPt_axle,3,AcDb::kLnWt030,AxleId);	
}

int DMX_ROAD::Linew(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw,AcDbObjectId layerId)
{

	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLayer(layerId);
	//	pWd->subEntityTraits().setLineType(layerId);
	pWd->subEntityTraits().setLineWeight(lw);	
	pWd->geometry().polyline(2,pArr);
	return  1;	
}
Acad::ErrorStatus DMX_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset)
{
	//20190713添加	地面线拖动功能
	for (int j = 0; j < indices.length(); j++)
	{ 
		int Cur_Index = indices[j];

		if (Cur_Index >= 0 && Cur_Index < NUM)
		{
			DMXys[Cur_Index].lc += offset[X] * HScale;
			DMXys[Cur_Index].level += offset[Y] * VScale;
		}
	}

	return Acad::eOk;
}


int DMX_ROAD::Boxw(AcGiWorldDraw* pWd,AcGePoint3d& UpLPt,AcGePoint3d& DownRPt, int icolor,AcDb::LineWeight lw,AcDbObjectId layerId)
{
	AcGePoint3d* pArr=new AcGePoint3d[4];
	pArr[0].set(UpLPt.x, UpLPt.y, UpLPt.z);
	pArr[1].set(DownRPt.x, UpLPt.y, UpLPt.z);
	pArr[2].set(DownRPt.x, DownRPt.y, UpLPt.z);
	pArr[3].set(UpLPt.x, DownRPt.y, UpLPt.z);
	pArr[4].set(UpLPt.x, UpLPt.y, UpLPt.z);


	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLayer(layerId);
	//	pWd->subEntityTraits().setLineType(layerId);
	pWd->subEntityTraits().setLineWeight(lw);	
	pWd->geometry().polyline(5,pArr);
	return  0;	
}

AcDbObjectId DMX_ROAD::CreateLayer(ACHAR *LayerName, ACHAR *LineType)
{
	AcDbLayerTable *pLayerTable;
	//	AcDbObjectId LayerTabRecordId;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForRead);
	AcDbObjectId LayerTabRecordId;
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) == Acad::eOk)
	{
		pLayerTable->close();
		return LayerTabRecordId;
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	AcDbLayerTableRecord *pLayerTableRecord=new AcDbLayerTableRecord;
	pLayerTableRecord->setName(LayerName);
	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTble,AcDb::kForRead);
	AcDbObjectId ltypeObjId;
	pLinetypeTble->getAt(LineType,ltypeObjId,Adesk::kTrue);
	//	acutPrintf(L"\nId=%d",ltypeObjId);
	pLayerTableRecord->setLinetypeObjectId(ltypeObjId);
	pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	pLinetypeTble->close();
	return LayerTabRecordId;
}

Acad::ErrorStatus DMX_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											 AcDbIntArray& osnapModes,
											 AcDbIntArray& geomIds)const
{   AcGePoint3d Pt;
int i;
assertReadEnabled();
// 将地面点加入夹点集
// ads_printf(L"\n NJD=%d",NJD);
for (i = 0; i < NUM; i++) 
{  Pt.set(DMXys[i].sx,DMXys[i].sy,0.0);
gripPoints.append(Pt);
}
return Acad::eOk;
}

void DMX_ROAD::Modify_DATA(int num, double array[])
{
	assertWriteEnabled();
	NUM=num;
	ZMax=-99999999;
	ZMin=9999999999;
	for(int i=0;i<NUM;i++)
	{
		DMXys[i].lc=array[2*i];
		DMXys[i].level=array[2*i+1];
		if(DMXys[i].level<-99)//没内插
			continue;
		ZMin = ZMin < DMXys[i].level ? ZMin : DMXys[i].level ;
		ZMax = ZMax > DMXys[i].level ? ZMax : DMXys[i].level ;
	}
	startLC=DMXys[0].lc;
	endLC=DMXys[NUM-1].lc;
	int tmp  = (int)floor(ZMin/10.0);////////////////p///
	ZMin = tmp*10.0;
	tmp  = (int)floor(ZMax/10.0)+1;
	ZMax = tmp*10.0;

	if(ZMax-ZMin<20)
	{
		ZMax = ZMax+50;
		ZMin = ZMin-50;
	}


	set_ori_scale();
}

void DMX_ROAD::draw_frame(AcGiWorldDraw* pWd)
{
	AcGePoint3d Pt0,Pt1,Pt2,Pt3;
	Pt0.x = x_ori - 30;//upleft
	Pt0.y = y_ori + (ZMax - ZMin) / VScale + pORI_SCALE.LCBHigh + 20;	//20190709	修改采用双轴比例尺
	Pt2.x = x_ori + (endLC - startLC) / HScale + 20;//downright
	Pt2.y = y_ori - pORI_SCALE.PQXHIGH - pORI_SCALE.DESHIGH - pORI_SCALE.EARHIGH;
	Pt1.x = Pt0.x;//downleft
	Pt1.y = Pt2.y;
	Pt3.x = Pt2.x;//upright
	Pt3.y = Pt0.y;
	Linew(pWd, Pt0, Pt1, 3, AcDb::kLnWt030, FrameId);
	Linew(pWd, Pt1, Pt2, 3, AcDb::kLnWt030, FrameId);
	Linew(pWd, Pt2, Pt3, 3, AcDb::kLnWt030, FrameId);
	Linew(pWd, Pt3, Pt0, 3, AcDb::kLnWt030, FrameId);

	//-----------------------------	
	GlobalZDMTK.ZDMTK[Zdmno].lpt.set(Pt1.x, Pt1.y);//记录图幅范围
	GlobalZDMTK.ZDMTK[Zdmno].rpt.set(Pt3.x, Pt3.y);
	GlobalZDMTK.ZDMTK[Zdmno].dmx = this;
	GlobalZDMTK.ZDMTK[Zdmno].X0 = x_ori;
	GlobalZDMTK.ZDMTK[Zdmno].Y0 = y_ori;
	GlobalZDMTK.ZDMTK[Zdmno].H0 = H_ori;

	AcGePoint3d TextPt;

	ACHAR mes[256];

	_stprintf(mes, L"%s方案 %s纵断面 ", GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].mdbname, GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].RXDname);

	TextPt.x = 0.5 * (Pt0.x + Pt3.x);
	TextPt.y = Pt3.y + 10.0;

	G_maketext(pWd,TextPt,mes,0,10,7,1);//字高10
}

//绘接坡
void DMX_ROAD::DrawConnectSlope(AcGiWorldDraw* pWd)
{
	//画引出坡度线
	AcGePoint2dArray ptarr;
	AcGePoint2d NewPt;
	AcGePoint3d textpoint;
	ACHAR str[20],chrh[30];
	int DLnum = pzLinep->DLArray.GetSize();
	//-------绘起始接坡-------------
	ptarr.setLogicalLength(2); //设置数组的逻辑长度
	m_JPInforArray_DMX[0].Stcml = pzLinep->DLArray[0].BLC;
	m_JPInforArray_DMX[0].Edcml = pzLinep->DLArray[DLnum-1].ELC;
	ptarr[0] = AcGePoint2d(MLToX(m_JPInforArray_DMX[0].Stcml), LevelToY(m_JPInforArray_DMX[0].Bef_H));

	NewPt.x = ptarr[0].x - 20.0 / HScale;
	NewPt.y = ptarr[0].y - 20.0 *m_JPInforArray_DMX[0].Bef_PD*0.01 / VScale;

	ptarr[1] = ptarr[0];
	ptarr[0] = NewPt;
	makepolyline(ptarr, 0, AcDb::kLnWt000);

	if (m_JPInforArray_DMX[0].Bef_PD > 0)
	{
		//标引出坡度
		textpoint.set((ptarr[0].x + ptarr[1].x) / 2.0 - 5.0, (ptarr[0].y + ptarr[1].y) / 2.0, 0.0);
		ads_rtos(m_JPInforArray_DMX[0].Bef_PD, 2, 3, chrh);
		_tcscat(chrh, L"%");
		//_stprintf(str, _T("%.3lf"), chrh);
		maketext(textpoint, chrh, m_JPInforArray_DMX[0].Bef_PD*0.01, 3, 0, 2);

		//标引出坡度插旗  //坡长
		_stprintf(str, _T("%.3lf"), m_JPInforArray_DMX[0].Bef_PC);
		textpoint.set((ptarr[0].x + ptarr[1].x) / 2.0 + 4.0, (ptarr[0].y + ptarr[1].y) / 2.0 - 3.5, 0.0);
		maketext(textpoint, str, m_JPInforArray_DMX[0].Bef_PD*0.01, 3, 0, 2);

		ptarr[0] = AcGePoint2d(ptarr[1].x, ptarr[1].y + 15);
		makepolyline(ptarr, 0, AcDb::kLnWt000);
	}
	else
	{
		//标引出坡度
		textpoint.set((ptarr[0].x + ptarr[1].x) / 2.0 + 4.5, (ptarr[0].y + ptarr[1].y) / 2.0, 0.0);
		ads_rtos(m_JPInforArray_DMX[0].Bef_PD, 2, 3, chrh);
		_tcscat(chrh, L"%");
		//_stprintf(str, _T("%.3lf"), chrh);
		maketext(textpoint, chrh, m_JPInforArray_DMX[0].Bef_PD*0.01, 3, 0, 2);

		//标引出坡度插旗  //坡长
		_stprintf(str, _T("%.3lf"), m_JPInforArray_DMX[0].Bef_PC);
		textpoint.set((ptarr[0].x + ptarr[1].x) / 2.0 - 4.0, (ptarr[0].y + ptarr[1].y) / 2.0 - 3.5, 0.0);
		maketext(textpoint, str, m_JPInforArray_DMX[0].Bef_PD*0.01, 3, 0, 2);

		ptarr[0] = AcGePoint2d(ptarr[1].x, ptarr[1].y + 15);
		makepolyline(ptarr, 0, AcDb::kLnWt000);
	}


	//-------绘终止接坡-------------
	AcGePoint2dArray ptarr_1;
	ptarr_1.setLogicalLength(2); //设置数组的逻辑长度
	ptarr_1[0] = AcGePoint2d(MLToX(m_JPInforArray_DMX[0].Edcml), LevelToY(m_JPInforArray_DMX[0].Aft_H));

	NewPt.x = ptarr_1[0].x + 20.0 / HScale;
	NewPt.y = ptarr_1[0].y + 20.0*m_JPInforArray_DMX[0].Aft_PD*0.01 / VScale;

	ptarr_1[1] = NewPt;
	makepolyline(ptarr_1, 0, AcDb::kLnWt000);

	if (m_JPInforArray_DMX[0].Aft_PD < 0)
	{
		//标接入坡度
		textpoint.set((ptarr_1[0].x + ptarr_1[1].x) / 2.0 + 5.0, (ptarr_1[0].y + ptarr_1[1].y) / 2.0, 0.0);
		ads_rtos(m_JPInforArray_DMX[0].Aft_PD, 2, 3, chrh);
		_tcscat(chrh, L"%");
		//_stprintf(str, _T("%.3lf %"), m_JPInforArray_DMX[0].Aft_PD);
		maketext(textpoint, chrh, m_JPInforArray_DMX[0].Aft_PD*0.01, 3, 0, 2);

		//标接入坡度插旗
		_stprintf(str, _T("%.03f"), m_JPInforArray_DMX[0].Aft_PC);
		textpoint.set((ptarr_1[0].x + ptarr_1[1].x) / 2.0 - 4.0, (ptarr_1[0].y + ptarr_1[1].y) / 2.0 - 3.5, 0.0);
		maketext(textpoint, str, m_JPInforArray_DMX[0].Aft_PD*0.01, 3, 0, 2);	 // 坡长

		ptarr_1[0] = AcGePoint2d(ptarr_1[1].x, ptarr_1[1].y + 15);
		makepolyline(ptarr_1, 0, AcDb::kLnWt000);
	}
	else
	{
		//标接入坡度
		textpoint.set((ptarr_1[0].x + ptarr_1[1].x) / 2.0 - 3.5, (ptarr_1[0].y + ptarr_1[1].y) / 2.0, 0.0);
		ads_rtos(m_JPInforArray_DMX[0].Aft_PD, 2, 3, chrh);
		_tcscat(chrh, L"%");
		//_stprintf(str, _T("%.3lf %"), m_JPInforArray_DMX[0].Aft_PD);
		maketext(textpoint, chrh, m_JPInforArray_DMX[0].Aft_PD*0.01, 3, 0, 2);

		//标接入坡度插旗
		_stprintf(str, _T("%.03f"), m_JPInforArray_DMX[0].Aft_PC);
		textpoint.set((ptarr_1[0].x + ptarr_1[1].x) / 2.0 + 5.0, (ptarr_1[0].y + ptarr_1[1].y) / 2.0 - 3.5, 0.0);
		maketext(textpoint, str, m_JPInforArray_DMX[0].Aft_PD*0.01, 3, 0, 2);	 // 坡长

		ptarr_1[0] = AcGePoint2d(ptarr_1[1].x, ptarr_1[1].y + 15);
		makepolyline(ptarr_1, 0, AcDb::kLnWt000);
	}


}


int DMX_ROAD::textStyle()
{
	AcDbTextStyleTable *pTextStyleTbl;

	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	if(pTextStyleTbl->getAt(L"宋体",pTextStyleTblId) == Acad::eOk)
	{
		pTextStyleTbl->close();
		return 0;
	}

	AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
	pTextStyleTblRcd->setName(L"宋体");
	pTextStyleTblRcd->setFileName(L"宋体");
	pTextStyleTblRcd->setFont(L"宋体",0,0,134,2);
	pTextStyleTblRcd->setXScale(0.8);
	pTextStyleTbl->add(pTextStyleTblRcd);
	pTextStyleTblRcd->close();
	pTextStyleTbl->close();	
	return 0;
}

int DMX_ROAD::get_axle_legth()
{
	return (int)(ZMax-ZMin);
}

double DMX_ROAD::CalDMGC(double cml)
{
	double DMGC;

	if(cml < DMXys[0].lc-0.001 || cml > DMXys[NUM-1].lc+0.001)
		return -100.0;//里程越界
	int i=0;
	while(i<NUM && DMXys[i].lc<cml)
		i++;
	if(i>0)
	{
		if(fabs(DMXys[i].lc-DMXys[i-1].lc)>0.01)
			DMGC = DMXys[i-1].level + (cml-DMXys[i-1].lc)*(DMXys[i].level-DMXys[i-1].level)/(DMXys[i].lc-DMXys[i-1].lc);
		else
			DMGC = DMXys[i-1].level;
	}
	else
		DMGC = DMXys[0].level;

	return DMGC;		
}
//纵断面获取图框范围信息，用于导入纵断面后的自动缩放
void DMX_ROAD::Get_Frame_Infor(double& Left, double& Top, double& Right, double& Bottom)
{
	Left = x_ori - 30;
	Top = y_ori + (ZMax - ZMin) / VScale + pORI_SCALE.LCBHigh + 50;	//上面还行字，多加30
	Right = x_ori + (endLC - startLC) / HScale + 20;
	Bottom = y_ori - pORI_SCALE.PQXHIGH - pORI_SCALE.DESHIGH - pORI_SCALE.EARHIGH;
}