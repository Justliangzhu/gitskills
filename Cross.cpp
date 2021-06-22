// CROSS.cpp
#include "stdafx.h"
#include "CROSS.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016
#include <math.h>

extern double Sfactor;
extern AcDbVoidPtrArray g_reactorList;
//struct ReactorEntityPair {
//    AcDbObjectId entityId;
//    AcDbEntityReactor *pEntReactor;
//};
AcDbVoidPtrArray DesLineInfor;//设计线实体信息，ID
struct ent_Id_Des{
	AcDbObjectId entityId;
	double cml;
	double zory;
	CString roadname;

};
extern ReactorEntityPair *pPair=NULL;
ent_Id_Des * pDes=NULL;
CROSS::CROSS()
{
	// Initialize the rect Frame Thickness
	// and the internal Window Framing thickness
	m_OriPoint.x = 0.0;
	m_OriPoint.y = 0.0;
	Ymin = 0.0;


	LeftEarthLinePart=NULL;RightEarthLinePart=NULL;
	LeftDesignLinePart=NULL;RightDesignLinePart=NULL;
	Y_LeftDesignLinePart=NULL;Y_RightDesignLinePart=NULL;
	LeftLTGouPart=NULL;RightLTGouPart=NULL;
	Y_LeftLTGouPart=NULL;Y_RightLTGouPart=NULL;
	LeftJSGouPart=RightJSGouPart=NULL;
	Y_LeftJSGouPart=Y_RightJSGouPart=NULL;
	Note=NULL;

	LDesReactor=RDesReactor=Y_RDesReactor=NULL;
	xjj=0.0;

	LearthId=RearthId=LgouId=RgouId=LJsgId=RJsgId=LdesignId=RdesignId=NoteId=NULL;
	Y_LgouId=Y_RgouId=Y_LJsgId=Y_RJsgId=Y_LdesignId=Y_RdesignId=NULL;

	IsLinkHdm = false;	
	BzTWInf = 0;

	IfSetDQ = 0;

	m_IsModifyGutter = false;
	m_GutterPath = L"";
	m_GutterThick = 0.0;

	m_IsDrawCrossEarthPile = false;

	m_NetCenterY = 0.0;
}

CROSS::~CROSS()
{
	// Does nothing
}

void CROSS::DrawCrossCenLine(AcDbObjectId layerId,double NoteTextH)
{

	//	AcDbObjectId LId,RId;
	AcDbDatabase *pCurDb;
	pCurDb = acdbHostApplicationServices()->workingDatabase();
	//	pCurDb->setLtscale(0.3);
	char str[80];
	AcGePoint2d Spt,Ept; 
	double x,y;

	AcDbPolyline  *pline=NULL,*pline1=NULL,*pline2=NULL;
	AcDbObjectId  ObjId;
	double Zdmh,Zdesh,Ydmh,Ydesh;
	bool ifzx;

	ifzx = false;
	//先标左线
	x=m_OriPoint.x;

	if(LeftEarthLinePart && LeftEarthLinePart->m_EarthPointNum>0)
		Zdmh =  LeftEarthLinePart->m_EarthPoints[0].y;
	else
		Zdmh = m_OriPoint.y;

	if(LeftDesignLinePart && LeftDesignLinePart->m_DesignPointNum>0)
		Zdesh = LeftDesignLinePart->m_DesignPoints[0].y;
	else
	{
		if(LeftEarthLinePart)
			Zdesh = Zdmh;
		else
			Zdesh =m_OriPoint.y;
	}
	Ymin = Zdesh < Zdmh ? Zdesh : Zdmh;
	Ymax = Zdesh > Zdmh ? Zdesh : Zdmh;

	if(RightEarthLinePart && RightEarthLinePart->m_EarthPointNum>0)
		Ydmh =  RightEarthLinePart->m_EarthPoints[0].y;
	else
		Ydmh = m_OriPoint.y;
	if(RightDesignLinePart&&RightDesignLinePart->m_DesignPointNum>0)
		Ydesh = RightDesignLinePart->m_DesignPoints[0].y;
	else
	{
		if(RightEarthLinePart)
			Ydesh = Ydmh;
		else
			Ydesh =m_OriPoint.y;
	}

	Ymin = Ydesh < Ymin ? Ydesh : Ymin;
	Ymin = Ydmh < Ymin ? Ydmh : Ymin;
	Ymax = Ydesh > Ymax ? Ydesh : Ymax;
	Ymax = Ydmh > Ymax ? Ydmh : Ymax;

	//	if(ifzx)
	//	{
	Spt.x=Ept.x=x;
	Spt.y = Ymax + 1.0*Sfactor;
	Ept.y = Ymin - 1.0*Sfactor;
	pline = new AcDbPolyline;
	pline->setLinetype(L"DASHDOTX2");//画一线
	pline->addVertexAt(0,Spt);
	pline->addVertexAt(1,Ept);
	pline->setLayer(layerId);
	ObjId=AddEntityToDbs(pline) ;
	pline->close();	


	if (fabs(OffsetByXLZX)>0.001)
	{
		Spt.x=Ept.x=x - OffsetByXLZX * Sfactor ;
		Spt.y = Ymax + 1.0*Sfactor;
		Ept.y = Ymin - 1.0*Sfactor;
		pline1 = new AcDbPolyline;
		pline1->setLinetype(L"DASHDOTX2");//画一线
		pline1->addVertexAt(0,Spt);
		pline1->addVertexAt(1,Ept);
		pline1->setLayer(layerId);
		ObjId=AddEntityToDbs(pline1) ;
		pline1->close();	
	}
	//	}
	//	if(xjj>-0.0001)//主线为左线
	//	{		
	if(Note)
	{			
		Note->mpt.x = Spt.x;
		if(BzTWInf==0)
			Note->mpt.y = Ept.y- 3.0*Sfactor;
		else
			Note->mpt.y = Ept.y- 10.0*Sfactor;
	}
	//	}

	//标右线
	if(DorS==2)
	{
		//	if(Y_LeftDesignLinePart->m_DesignPointNum>0 || Y_RightDesignLinePart->m_DesignPointNum>0)
		//	{
		//		if(ifzx)//有左线
		x=Spt.x + fabs(xjj)*Sfactor;
		//			else
		//			{
		//				if(Y_LeftDesignLinePart->m_DesignPointNum>0)
		//				   x= Y_LeftDesignLinePart->m_DesignPoints[0].x;
		//				else
		//					x= Y_RightDesignLinePart->m_DesignPoints[0].x;
		//			}
		Zdmh = m_OriPoint.y;
		if(LeftEarthLinePart && LeftEarthLinePart->m_EarthPointNum>0)
			Zdmh =  LeftEarthLinePart->m_EarthPoints[0].y;
		if(Y_LeftDesignLinePart && Y_LeftDesignLinePart->m_DesignPointNum>0)
			Zdesh = Y_LeftDesignLinePart->m_DesignPoints[0].y;		        		       	
		else
			Zdesh = Zdmh;		        

		Ymin = Zdesh < Zdmh ? Zdesh : Zdmh;
		Ymax = Zdesh > Zdmh ? Zdesh : Zdmh;

		if(RightEarthLinePart && RightEarthLinePart->m_EarthPointNum>0)
			Ydmh =  RightEarthLinePart->m_EarthPoints[0].y;
		if(Y_RightDesignLinePart && Y_RightDesignLinePart->m_DesignPointNum>0)
			Ydesh = Y_RightDesignLinePart->m_DesignPoints[0].y;
		else
			Ydesh = Ydmh;

		Ymin = Ydesh < Ymin ? Ydesh : Ymin;
		Ymin = Ydmh < Ymin ? Ydmh : Ymin;
		Ymax = Ydesh > Ymax ? Ydesh : Ymax;
		Ymax = Ydmh > Ymax ? Ydmh : Ymax;
		//				  Spt.x = Ept.x = x;
		//				  Spt.y = Ymax + 2.0*Sfactor;
		//				  Ept.y = Ymin - 2.0*Sfactor;
		//				  pline = new AcDbPolyline;
		//				  pline->setLinetype(L"DASHDOTX2");//画一线
		//				  pline->addVertexAt(0,Spt);
		//				  pline->addVertexAt(1,Ept);
		//				  pline->setLayer(layerId);
		//				  ObjId=AddEntityToDbs(pline) ;
		//				  pline->close();

		//				 if(xjj<-0.00001)//主线为右线
		//				 {
		//					 if(Note)
		//					 {
		//					   Note->mpt.x = Spt.x;//标注点(主线)
		//					   Note->mpt.y = Ept.y- 3.0*Sfactor;
		//					 }
		//					 
		//				 }

	}
	// NoteDist(spt,ept,1.0,str,6,2,Sfactor,2)	;

	//20190819	标注里程
	AcGePoint3d Pt3d(Ept.x, Ept.y - 1 * Sfactor, 0.0);
	maketext(Pt3d, m_ckml, 0, NoteTextH/*0.4 * Sfactor*/, GREEN, 2, L"宋体", L"横断面图-中线");
}
AcDbObjectId CROSS::AddEntityToModal(AcDbObjectId layerId,AcDbEntity *entity)
{
	AcDbObjectId objId;
	entity->setLayer(layerId);
	objId=AddEntityToDbs(entity);
	return objId;
}

void CROSS::Draw()
{
	AcDbEntity *pEnt;
	AcDbObjectId layerId;
	layerId=CreateLayer(L"Hdm");//如果层已存在，则只返回层Id，没有则建立新层

	DrawCrossCenLine(layerId);

	if(LeftEarthLinePart)
		LearthId=AddEntityToModal(layerId,LeftEarthLinePart);

	if(RightEarthLinePart)
		RearthId=AddEntityToModal(layerId,RightEarthLinePart);

	if(LeftLTGouPart)
		LgouId=AddEntityToModal(layerId,LeftLTGouPart);

	if(RightLTGouPart)
		RgouId=AddEntityToModal(layerId,RightLTGouPart);

	if(LeftJSGouPart)
		LJsgId=AddEntityToModal(layerId,LeftJSGouPart);

	if(RightJSGouPart)
		RJsgId=AddEntityToModal(layerId,RightJSGouPart);

	if(Note)
		NoteId=AddEntityToModal(layerId,Note);

	/////////////////////////////////////////////////设计线部分
	//左侧
	if(LeftDesignLinePart)
	{
		if(IsLinkHdm)
		{
			LeftDesignLinePart->DesignLineColor = 7;
			LeftDesignLinePart->lmhdcolor = 7;
		}

		LdesignId=AddEntityToModal(layerId,LeftDesignLinePart);
		LeftDesignLinePart->l_earthline=LeftEarthLinePart;//用于夹点操作
		LeftDesignLinePart->r_earthline=RightEarthLinePart;
		LeftDesignLinePart->LTGou=LeftLTGouPart;
		LeftDesignLinePart->JSGou=LeftJSGouPart;
		LeftDesignLinePart->Note=Note;
		LeftDesignLinePart->gouId=LgouId;

		pDes=new ent_Id_Des;//记录设计线的ID.一段里程交互时，可以根据断面里程找寻断面的id
		pDes->entityId=LdesignId;
		pDes->cml=LeftDesignLinePart->cml;
		pDes->zory=LeftDesignLinePart->m_ZorY;
		pDes->roadname = LeftDesignLinePart->RoadName;
		//	pDes->xlxz=LeftDesignLinePart->xlxz;
		//	pDes->r_bh=LeftDesignLinePart->r_bh;

		DesLineInfor.append(pDes);

		if(IfSetDQ==-1 || IfSetDQ==2)
			if (LeftDesignLinePart->m_DesignPointNum>0)
				DrawCrossDQLine(LeftDesignLinePart->m_DesignPoints[LeftDesignLinePart->m_DesignPointNum-1]);

	}

	//右侧
	if(RightDesignLinePart)
	{
		if(IsLinkHdm)
		{
			RightDesignLinePart->DesignLineColor = 7;
			RightDesignLinePart->lmhdcolor = 7;
		}

		RdesignId=AddEntityToModal(layerId,RightDesignLinePart);
		RightDesignLinePart->l_earthline=LeftEarthLinePart;
		RightDesignLinePart->r_earthline=RightEarthLinePart;
		RightDesignLinePart->LTGou=RightLTGouPart;
		RightDesignLinePart->JSGou=RightJSGouPart;
		RightDesignLinePart->Note=Note;
		RightDesignLinePart->gouId=RgouId;

		pDes=new ent_Id_Des;
		pDes->entityId=RdesignId;
		pDes->cml=RightDesignLinePart->cml;
		pDes->zory=RightDesignLinePart->m_ZorY;
		pDes->roadname =RightDesignLinePart->RoadName;
		//	pDes->xlxz=RightDesignLinePart->xlxz;
		//	pDes->r_bh=RightDesignLinePart->r_bh;
		DesLineInfor.append(pDes);

		if(IfSetDQ==1 || IfSetDQ==2)
			if (RightDesignLinePart->m_DesignPointNum>0)
				DrawCrossDQLine(RightDesignLinePart->m_DesignPoints[RightDesignLinePart->m_DesignPointNum-1]);
	}

	//////////////////////////////////设计线部分
	////////////////////////水沟部分
	if(LeftLTGouPart)
	{
		LeftLTGouPart->l_earthline=LeftEarthLinePart;//用于夹点操作
		LeftLTGouPart->r_earthline=RightEarthLinePart;
		LeftLTGouPart->JSGou=LeftJSGouPart;
		LeftLTGouPart->Note=Note;

	}

	if(RightLTGouPart)
	{
		RightLTGouPart->l_earthline=LeftEarthLinePart;
		RightLTGouPart->r_earthline=RightEarthLinePart;
		RightLTGouPart->JSGou=RightJSGouPart;
		RightLTGouPart->Note=Note;
	}


	if(DorS==2)
	{
		if(Y_LeftLTGouPart)
		{
			Y_LgouId=AddEntityToModal(layerId,Y_LeftLTGouPart);            
		}

		if(Y_RightLTGouPart)
			Y_RgouId=AddEntityToModal(layerId,Y_RightLTGouPart);

		if(Y_LeftJSGouPart)
			Y_LJsgId=AddEntityToModal(layerId,Y_LeftJSGouPart);

		if(Y_RightJSGouPart)
			Y_RJsgId=AddEntityToModal(layerId,Y_RightJSGouPart);

		//////////////////////////////////////设计线部分
		if(Y_LeftDesignLinePart)
		{
			Y_LdesignId=AddEntityToModal(layerId,Y_LeftDesignLinePart);
			Y_LeftDesignLinePart->l_earthline=LeftEarthLinePart;
			Y_LeftDesignLinePart->r_earthline=RightEarthLinePart;
			Y_LeftDesignLinePart->LTGou=Y_LeftLTGouPart;
			Y_LeftDesignLinePart->JSGou=Y_LeftJSGouPart;
			Y_LeftDesignLinePart->Note=Note;

			pDes=new ent_Id_Des;
			pDes->entityId=Y_LdesignId;
			pDes->cml=Y_LeftDesignLinePart->cml;
			pDes->zory=Y_LeftDesignLinePart->m_ZorY;
			//	pDes->xlxz=Y_LeftDesignLinePart->xlxz;
			//	pDes->r_bh=Y_LeftDesignLinePart->r_bh;
			DesLineInfor.append(pDes);
		}

		if(Y_RightDesignLinePart)
		{
			Y_RdesignId=AddEntityToModal(layerId,Y_RightDesignLinePart);
			Y_RightDesignLinePart->l_earthline=LeftEarthLinePart;
			Y_RightDesignLinePart->r_earthline=RightEarthLinePart;
			Y_RightDesignLinePart->LTGou=Y_RightLTGouPart;
			Y_RightDesignLinePart->JSGou=Y_RightJSGouPart;
			Y_RightDesignLinePart->Note=Note;

			pDes=new ent_Id_Des;
			pDes->entityId=Y_RdesignId;
			pDes->cml=Y_RightDesignLinePart->cml;
			pDes->zory=Y_RightDesignLinePart->m_ZorY;
			//	pDes->xlxz=Y_RightDesignLinePart->xlxz;
			//	pDes->r_bh=Y_RightDesignLinePart->r_bh;
			DesLineInfor.append(pDes);
		}

		//////////////////////////////////////设计线部分
		////////////////////////水沟部分
		if(Y_LeftLTGouPart)
		{
			Y_LeftLTGouPart->l_earthline=LeftEarthLinePart;
			Y_LeftLTGouPart->r_earthline=RightEarthLinePart;
			Y_LeftLTGouPart->JSGou=Y_LeftJSGouPart;
			Y_LeftLTGouPart->Note=Note;
		}

		if(Y_RightLTGouPart)
		{
			Y_RightLTGouPart->l_earthline=LeftEarthLinePart;
			Y_RightLTGouPart->r_earthline=RightEarthLinePart;
			Y_RightLTGouPart->JSGou=Y_RightJSGouPart;
			Y_RightLTGouPart->Note=Note;

		}

	}
	//单线在左右设计线加响应器
	//双线是左线左部分设计线、右线右部分设计线加响应器
	if(DorS==1)
	{
		//单路面的左侧
		if(LdesignId && acdbOpenObject(pEnt,LdesignId,AcDb::kForRead)==Acad::eOk)//左线左侧
		{
			LeftDesignLinePart=DesignLine_ROAD::cast(pEnt);
			//LeftDesignLinePart->assertWriteEnabled();
			LDesReactor=new DesLineReact(LearthId,RearthId,LgouId,LJsgId,NoteId);
			LeftDesignLinePart->addReactor(LDesReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=LdesignId;
			pPair->pEntReactor=LDesReactor;
			g_reactorList.append(pPair);

		}

		if(LgouId && acdbOpenObject(pEnt,LgouId,AcDb::kForRead)==Acad::eOk)
		{
			LeftLTGouPart=LTGOU_ROAD::cast(pEnt);
			//传指针
			LLTGouReactor=new LTGouReactor(LearthId,RearthId,LeftDesignLinePart,LJsgId,NoteId);
			LeftLTGouPart->addReactor(LLTGouReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=LgouId;
			pPair->pEntReactor=LLTGouReactor;
			g_reactorList.append(pPair);
		}

		//20190927新增	为天沟增加响应器
		if (LJsgId && acdbOpenObject(pEnt, LJsgId, AcDb::kForRead) == Acad::eOk)
		{
			LeftJSGouPart = JSGOU_ROAD::cast(pEnt);
			//传指针
			LJSGouReactor = new JSGouReactor(LearthId, RearthId, LeftDesignLinePart, LgouId, NoteId);
			LeftJSGouPart->addReactor(LJSGouReactor);

			pEnt->close();
			pPair = new ReactorEntityPair;
			pPair->entityId = LJsgId;
			pPair->pEntReactor = LJSGouReactor;
			g_reactorList.append(pPair);
		}
		

		//单路面的右侧
		if(RdesignId && acdbOpenObject(pEnt,RdesignId,AcDb::kForRead)==Acad::eOk)//左线右侧
		{
			RightDesignLinePart=DesignLine_ROAD::cast(pEnt);
			//RightDesignLinePart->assertWriteEnabled();
			RDesReactor=new DesLineReact(LearthId,RearthId,RgouId,RJsgId,NoteId);
			RightDesignLinePart->addReactor(RDesReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=RdesignId;
			pPair->pEntReactor=RDesReactor;
			g_reactorList.append(pPair);
		}

		if(RgouId && acdbOpenObject(pEnt,RgouId,AcDb::kForRead)==Acad::eOk)
		{
			RightLTGouPart=LTGOU_ROAD::cast(pEnt);
			//传指针
			RLTGouReactor=new LTGouReactor(LearthId,RearthId,RightDesignLinePart,RJsgId,NoteId);
			RightLTGouPart->addReactor(RLTGouReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=RgouId;
			pPair->pEntReactor=RLTGouReactor;
			g_reactorList.append(pPair);
		}

		//20190927新增	为天沟增加响应器
		if (RJsgId && acdbOpenObject(pEnt, RJsgId, AcDb::kForRead) == Acad::eOk)
		{
			RightJSGouPart = JSGOU_ROAD::cast(pEnt);
			//传指针
			RJSGouReactor = new JSGouReactor(LearthId, RearthId, RightDesignLinePart, RgouId, NoteId);
			RightJSGouPart->addReactor(RJSGouReactor);

			pEnt->close();
			pPair = new ReactorEntityPair;
			pPair->entityId = RJsgId;
			pPair->pEntReactor = RJSGouReactor;
			g_reactorList.append(pPair);
		}

	}
	if(DorS==2)
	{
		//双路面的左线左侧

		if(LdesignId && acdbOpenObject(pEnt,LdesignId,AcDb::kForRead)==Acad::eOk)//左线左侧
		{
			LeftDesignLinePart=DesignLine_ROAD::cast(pEnt);
			//LeftDesignLinePart->assertWriteEnabled();
			LDesReactor=new DesLineReact(LearthId,RearthId,LgouId,LJsgId,NoteId);
			LeftDesignLinePart->addReactor(LDesReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=LdesignId;
			pPair->pEntReactor=LDesReactor;
			g_reactorList.append(pPair);

		}

		if(LgouId && acdbOpenObject(pEnt,LgouId,AcDb::kForRead)==Acad::eOk)
		{
			LeftLTGouPart=LTGOU_ROAD::cast(pEnt);
			//传指针
			LLTGouReactor=new LTGouReactor(LearthId,RearthId,LeftDesignLinePart,LJsgId,NoteId);
			LeftLTGouPart->addReactor(LLTGouReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=LgouId;
			pPair->pEntReactor=LLTGouReactor;
			g_reactorList.append(pPair);
		}

		//双路面的右线右侧
		if(Y_RdesignId && acdbOpenObject(pEnt,Y_RdesignId,AcDb::kForRead)==Acad::eOk)//右线右侧
		{
			Y_RightDesignLinePart=DesignLine_ROAD::cast(pEnt);
			//	Y_RightDesignLinePart->assertWriteEnabled();
			Y_RDesReactor=new DesLineReact(LearthId,RearthId,Y_RgouId,Y_RJsgId,NoteId);
			Y_RightDesignLinePart->addReactor(Y_RDesReactor);

			pEnt->close();

			pPair=new ReactorEntityPair;
			pPair->entityId=Y_RdesignId;
			pPair->pEntReactor=Y_RDesReactor;
			g_reactorList.append(pPair);
		}

		if(Y_RgouId && acdbOpenObject(pEnt,Y_RgouId,AcDb::kForRead)==Acad::eOk)
		{
			Y_RightLTGouPart=LTGOU_ROAD::cast(pEnt);
			//传指针
			Y_RLTGouReactor=new LTGouReactor(LearthId,RearthId,Y_RightDesignLinePart,RJsgId,NoteId);
			Y_RightLTGouPart->addReactor(Y_RLTGouReactor);

			pEnt->close();
			pPair=new ReactorEntityPair;
			pPair->entityId=Y_RgouId;
			pPair->pEntReactor=Y_RLTGouReactor;
			g_reactorList.append(pPair);
		}

	}
	return;
} 

//
//
//void CROSS::Draw()
//{
//	AcDbEntity *pEnt;
//	AcDbObjectId layerId;
//	layerId=CreateLayer(L"Hdm");//如果层已存在，则只返回层Id，没有则建立新层
//	DrawCrossCenLine(layerId);
//	LdesignId=AddEntityToModal(layerId,LeftDesignLinePart);
//	LeftDesignLinePart->l_earthline=LeftEarthLinePart;//用于夹点操作
//	LeftDesignLinePart->r_earthline=RightEarthLinePart;
//	LeftDesignLinePart->LTGou=LeftLTGouPart;
//	LeftDesignLinePart->JSGou=LeftJSGouPart;
//	LeftDesignLinePart->Note=Note;
//
//
//	pDes=new ent_Id_Des;
//	pDes->entityId=LdesignId;
//	pDes->cml=LeftDesignLinePart->cml;
//	pDes->zory=LeftDesignLinePart->m_ZorY;
//	pDes->roadname = LeftDesignLinePart->RoadName;
//////	pDes->xlxz=LeftDesignLinePart->xlxz;
////////	pDes->r_bh=LeftDesignLinePart->r_bh;
//    DesLineInfor.append(pDes);
//
////ads_printf(L"%lf %d %d\n",m_cml,RightDesignLinePart->m_DesignPointNum,DorS);
//	RdesignId=AddEntityToModal(layerId,RightDesignLinePart);
//	RightDesignLinePart->l_earthline=LeftEarthLinePart;
//	RightDesignLinePart->r_earthline=RightEarthLinePart;
//	RightDesignLinePart->LTGou=RightLTGouPart;
//	RightDesignLinePart->JSGou=RightJSGouPart;
//	RightDesignLinePart->Note=Note;
//	
//	pDes=new ent_Id_Des;
//	pDes->entityId=RdesignId;
//	pDes->cml=RightDesignLinePart->cml;
//	pDes->zory=RightDesignLinePart->m_ZorY;
//	pDes->roadname = RightDesignLinePart->RoadName;
//	DesLineInfor.append(pDes);
//	
//	LearthId=AddEntityToModal(layerId,LeftEarthLinePart);
//	RearthId=AddEntityToModal(layerId,RightEarthLinePart);
////
//	LgouId=AddEntityToModal(layerId,LeftLTGouPart);
//	RgouId=AddEntityToModal(layerId,RightLTGouPart);
////
//	LJsgId=AddEntityToModal(layerId,LeftJSGouPart);
//	RJsgId=AddEntityToModal(layerId,RightJSGouPart);
////
//	NoteId=AddEntityToModal(layerId,Note);
//
//	//ads_printf(L"\ncml=%lf,Id=%d",LeftDesignLinePart->cml,LgouId);
//	
//	if(DorS==2)
//	{
//		Y_LdesignId=AddEntityToModal(layerId,Y_LeftDesignLinePart);
//		pDes=new ent_Id_Des;
//		pDes->entityId=Y_LdesignId;
//		pDes->cml=Y_LeftDesignLinePart->cml;
//		pDes->zory=Y_LeftDesignLinePart->m_ZorY;
//	//	pDes->xlxz=Y_LeftDesignLinePart->xlxz;
//	//	pDes->r_bh=Y_LeftDesignLinePart->r_bh;
//		pDes->roadname = Y_LeftDesignLinePart->RoadName;
//		DesLineInfor.append(pDes);
//		
//		Y_RdesignId=AddEntityToModal(layerId,Y_RightDesignLinePart);
//		pDes=new ent_Id_Des;
//		pDes->entityId=Y_RdesignId;
//		pDes->cml=Y_RightDesignLinePart->cml;
//		pDes->zory=Y_RightDesignLinePart->m_ZorY;
//		//pDes->xlxz=Y_RightDesignLinePart->xlxz;
//		//pDes->r_bh=Y_RightDesignLinePart->r_bh;
//		pDes->roadname = Y_RightDesignLinePart->RoadName;
//		DesLineInfor.append(pDes);
//						
//		Y_LgouId=AddEntityToModal(layerId,Y_LeftLTGouPart);
//		Y_RgouId=AddEntityToModal(layerId,Y_RightLTGouPart);
//		Y_LJsgId=AddEntityToModal(layerId,Y_LeftJSGouPart);
//		Y_RJsgId=AddEntityToModal(layerId,Y_RightJSGouPart);
//	}
//	//单线在左右设计线加响应器
//	//双线是左线左部分设计线、右线右部分设计线加响应器
//
//	
//	if(acdbOpenObject(pEnt,LdesignId,AcDb::kForWrite)==Acad::eOk)//左线左侧
//		{
//			LeftDesignLinePart=DesignLine::cast(pEnt);
//			//LeftDesignLinePart->assertWriteEnabled();
//			LDesReactor=new DesLineReact(LearthId,RearthId,LgouId,LJsgId,NoteId);
//			LeftDesignLinePart->addReactor(LDesReactor);
//
//			pEnt->close();
//			pPair=new ReactorEntityPair;
//			pPair->entityId=LdesignId;
//			pPair->pEntReactor=LDesReactor;
//			g_reactorList.append(pPair);
//		}
//		
//			
//	
//		if(DorS==1)
//			{
//				if(acdbOpenObject(pEnt,RdesignId,AcDb::kForWrite)==Acad::eOk)//左线右侧
//				{
//					RightDesignLinePart=DesignLine::cast(pEnt);
//					//RightDesignLinePart->assertWriteEnabled();
//					RDesReactor=new DesLineReact(LearthId,RearthId,RgouId,RJsgId,NoteId);
//					RightDesignLinePart->addReactor(RDesReactor);
//	
//					pEnt->close();
//					pPair=new ReactorEntityPair;
//					pPair->entityId=RdesignId;
//					pPair->pEntReactor=RDesReactor;
//					g_reactorList.append(pPair);
//				}
//			}
//			if(DorS==2)
//			{
//				if(acdbOpenObject(pEnt,Y_RdesignId,AcDb::kForWrite)==Acad::eOk)//右线右侧
//				{
//					Y_RightDesignLinePart=DesignLine::cast(pEnt);
//				//	Y_RightDesignLinePart->assertWriteEnabled();
//					Y_RDesReactor=new DesLineReact(LearthId,RearthId,Y_RgouId,Y_RJsgId,NoteId);
//					Y_RightDesignLinePart->addReactor(Y_RDesReactor);
//	
//					pEnt->close();
//					
//					pPair=new ReactorEntityPair;
//					pPair->entityId=Y_RdesignId;
//					pPair->pEntReactor=Y_RDesReactor;
//					g_reactorList.append(pPair);
//				}
//			
//			}
//
//	
//} 
//标注距离
void CROSS::NoteDist(AcGePoint3d Spt,AcGePoint3d Ept,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,AcDbObjectId LayerId)
{
	//绘线
	//makeline(Spt,Ept,iColor);
	AcDbLine *pLine	= new AcDbLine(Spt,Ept);
	pLine->setColorIndex(iColor) ;
	pLine->setLayer(LayerId);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	//绘箭头
	AcGePoint3dArray ptarray;
	AcGePoint3d pt;
	pt.x = Spt.x + (Ept.x-Spt.x)/15.0;
	pt.y = Spt.y + (Ept.y-Spt.y)/15.0;
	pt.z =0.0;
	ptarray.append(pt);
	ptarray.append(Spt);
	make2dline(ptarray,iColor,0.05*sfactor,0.0,LayerId);
	ptarray.removeSubArray(0,1);//删除
	pt.x = Ept.x - (Ept.x-Spt.x)/15.0;
	pt.y = Ept.y - (Ept.y-Spt.y)/15.0;
	pt.z =0.0;
	ptarray.append(pt);
	ptarray.append(Ept);
	make2dline(ptarray,iColor,0.05*sfactor,0.0,LayerId);
	//注字
	AcGePoint3d origin;
	origin.x = 0.5*(Spt.x+Ept.x);
	origin.y = 0.5*(Spt.y+Ept.y) + 0.25*sfactor;
	origin.z =0.0;
	H_maketext(origin,Str,0,texth,itextColor,mode,L"宋体",LayerId);	
}

AcDbObjectId CROSS::H_maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,AcDbObjectId layerId)
{

	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	if(pTextStyleTbl->getAt(textStyle,pTextStyleTblId)!= Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(textStyle);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(textStyle,0,0,134,2);
		pTextStyleTblRcd->setXScale(0.8);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(textStyle,pTextStyleTblId);
	}	
	pTextStyleTbl->close();	
	if (mode==1) // 左对齐
	{      pText->setHorizontalMode(AcDb::kTextLeft); }
	else if (mode==2) // // 中对齐
	{      pText->setHorizontalMode(AcDb::kTextCenter); }
	else // // 右对齐
	{      pText->setHorizontalMode(AcDb::kTextRight); };
	pText->setColorIndex(icolor);
	if (mode==1) {  pText->setPosition(pos);	}
	else {   pText->setAlignmentPoint(pos);     };
	//  ads_printf(L"\n   m        Pt=%10.3f   %10.3f   %10.4f %s",pos[X],pos[Y],dms_rad(ang),ctext);  
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setHeight(texth);
	pText->setTextString(ctext);
	pText->setLayer(layerId);
	TId=AddEntityToDbs(pText);
	return TId;     
}

void CROSS::OutDraw(double TabTextH,double NoteTextH)
{
	AcDbEntity *pEnt;
	AcDbObjectId layerId;
	layerId=CreateLayer(L"横断面图-中线");//如果层已存在，则只返回层Id，没有则建立新层

	DrawCrossCenLine(layerId, NoteTextH);

	//绘设计线
	if(LeftDesignLinePart)
	{
		LeftDesignLinePart->m_IsModifyGutter = m_IsModifyGutter;
		LeftDesignLinePart->m_GutterThick = m_GutterThick;
		if(!IsLinkHdm)
			LeftDesignLinePart->Draw(NoteTextH);
		else
		{
			LeftDesignLinePart->lmhdcolor=LeftDesignLinePart->DesignLineColor=7;
			LeftDesignLinePart->Draw(NoteTextH);
		}
		if(IfSetDQ==-1 || IfSetDQ==2)
			DrawCrossDQLine(LeftDesignLinePart->m_DesignPoints[LeftDesignLinePart->m_DesignPointNum-1]);

		DrawGutterBlock(LeftDesignLinePart->m_DesignPoints[0]);
	}
	if(RightDesignLinePart) 
	{
		RightDesignLinePart->m_IsModifyGutter = m_IsModifyGutter;
		RightDesignLinePart->m_GutterThick = m_GutterThick;
		if(!IsLinkHdm)
			RightDesignLinePart->Draw(NoteTextH);
		else
		{
			RightDesignLinePart->lmhdcolor=RightDesignLinePart->DesignLineColor=7;
			RightDesignLinePart->Draw(NoteTextH);
		}

		if(IfSetDQ==1 || IfSetDQ==2)
			DrawCrossDQLine(RightDesignLinePart->m_DesignPoints[RightDesignLinePart->m_DesignPointNum-1]);
	}

	////绘地面线
	if(LeftEarthLinePart) 
	{
		//	if(!IsLinkHdm || xjj<-0.001) 
		LeftEarthLinePart->Draw(7,NoteTextH);
	}
	if(RightEarthLinePart) 
	{
		//   if(!IsLinkHdm || xjj>-0.001) 
		RightEarthLinePart->Draw(7, NoteTextH);
	}
	//绘路堤水沟
	if(LeftLTGouPart) 
	{
		if(!IsLinkHdm)
			LeftLTGouPart->Draw(3);
		else
			LeftLTGouPart->Draw(7);
	}
	if(RightLTGouPart)
	{
		if(!IsLinkHdm)
			RightLTGouPart->Draw(3);
		else
			RightLTGouPart->Draw(7);
	}

	//绘天沟
	if(LeftJSGouPart)
	{
		if(!IsLinkHdm)
			LeftJSGouPart->Draw(4);
		else
			LeftJSGouPart->Draw(7);
	}

	if(RightJSGouPart)
	{
		if(!IsLinkHdm)
			RightJSGouPart->Draw(4);
		else
			RightJSGouPart->Draw(7);
	}


	if(DorS==2)
	{

		if(Y_LeftDesignLinePart) Y_LeftDesignLinePart->Draw();

		if(Y_RightDesignLinePart) Y_RightDesignLinePart->Draw();

		if(Y_LeftLTGouPart) Y_LeftLTGouPart->Draw(3);

		if(Y_RightLTGouPart) Y_RightLTGouPart->Draw(3);
		//
		if(Y_LeftJSGouPart) Y_LeftJSGouPart->Draw(4);

		if(Y_RightJSGouPart) Y_RightJSGouPart->Draw(4);

	}
	//绘制主线里程

	//绘制占地宽信息
	if(m_IsDrawCrossEarthPile)
		DrawZDK();

	if(!IsLinkHdm)
	{
		//wr_BG();
		//wr_TW();
		if (!m_IsModifyGutter)		//绘制改沟图时，横断面不标注信息
			DrawCrossInfor(TabTextH);
		else
			DrawCrossInfor_Gutter();	

		if(Note && !m_IsModifyGutter)
			Note->OutDrawBP(NoteTextH);
	}

} 

/*------------------------------------------------------------*/
//写标高
void CROSS::wr_BG()
{
	ads_real x,y;
	AcGePoint3d Upt[10],Bpt[10],Mpt[10],UMpt,Tpt;
	ACHAR str[30];
	int i;

	x = m_OriPoint.x ;	//20190805	增加50标注
	y = Ymin;
	// ads_printf(L"x=%lf y=%lf\n",x,y);	
	UMpt.x = x , UMpt.y = y - 10;
	Upt[0].x=x-48,Upt[0].y=UMpt.y;//顶点
	Bpt[0].x=x-48,Bpt[0].y=UMpt.y-10;//底点
	Mpt[0].x=x-48,Mpt[0].y=UMpt.y-5;//底点
	for(i=1;i<=8;i++)
	{
		Upt[i].x=Upt[0].x+i*12;
		Mpt[i].x=Mpt[0].x+i*12;
		Bpt[i].x=Bpt[0].x+i*12;
		Upt[i].y=UMpt.y;
		Mpt[i].y=UMpt.y-5;
		Bpt[i].y=UMpt.y-10;
	}
	//绘表
	double texth;
	int itextColor;
	texth = 2.5;
	itextColor = 2;
	makeline1(Upt[0],Upt[8],1);
	makeline1(Mpt[0],Mpt[8],1);
	makeline1(Bpt[0],Bpt[8],1);
	for(i=0; i<9; i++)
		makeline1(Upt[i],Bpt[i],1);	   
	//注字 
	Tpt.x = 0.5*(Upt[0].x+Upt[1].x);
	Tpt.y = 0.5*(Upt[0].y+Mpt[0].y);
	_tcscpy(str,L"B1");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	Tpt.x = 0.5*(Upt[1].x+Upt[2].x);
	_tcscpy(str,L"B2");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.x = 0.5*(Upt[2].x+Upt[3].x);
	_tcscpy(str,L"B3");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.x = 0.5*(Upt[3].x+Upt[4].x);
	_tcscpy(str,L"ZPH");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.x = 0.5*(Upt[4].x+Upt[5].x);
	_tcscpy(str,L"YPH");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.x = 0.5*(Upt[5].x+Upt[6].x);
	_tcscpy(str,L"A3");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.x = 0.5*(Upt[6].x+Upt[7].x);
	_tcscpy(str,L"A2");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.x = 0.5*(Upt[7].x+Upt[8].x);
	_tcscpy(str,L"A1");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	//------------------------------------------------------------------------
	Tpt.x = 0.5*(Mpt[0].x+Bpt[1].x);
	Tpt.y = 0.5*(Mpt[0].y+Bpt[0].y);

	if(LeftDesignLinePart)
	{
		_stprintf(str,L"%0.2lf",LeftDesignLinePart->RHF[9]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	

		Tpt.x = 0.5*(Mpt[1].x+Bpt[2].x);
		_stprintf(str,L"%0.2lf",LeftDesignLinePart->RHF[7]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	

		Tpt.x = 0.5*(Upt[2].x+Upt[3].x);
		_stprintf(str,L"%0.2lf",LeftDesignLinePart->RHF[5]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	

		Tpt.x = 0.5*(Upt[3].x+Upt[4].x);
		_stprintf(str,L"%0.2lf",LeftDesignLinePart->RHF[3]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}

	if(RightDesignLinePart)
	{
		Tpt.x = 0.5*(Upt[4].x+Upt[5].x);
		_stprintf(str,L"%0.2lf",RightDesignLinePart->RHF[3]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);

		Tpt.x = 0.5*(Upt[5].x+Upt[6].x);
		_stprintf(str,L"%0.2lf",RightDesignLinePart->RHF[5]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	

		Tpt.x = 0.5*(Upt[6].x+Upt[7].x);
		_stprintf(str,L"%0.2lf",RightDesignLinePart->RHF[7]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	

		Tpt.x = 0.5*(Upt[7].x+Upt[8].x);
		_stprintf(str,L"%0.2lf",RightDesignLinePart->RHF[9]);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	}


}

/*------------------------------------------------------------*/
void CROSS::wr_TW()
{
	ads_real y;
	AcGePoint3d Lpt[9],Rpt[9],Mpt[9],Tpt;
	ACHAR str[30];
	int i;

	// y = Ymin;
	//	y = m_OriPoint.y;
	//   y = Ymax;
	y = Ymin+20;
	Lpt[0].x=DX + 50,Lpt[0].y=y;//顶点		20190805修改
	Rpt[0].x=Lpt[0].x+33,Rpt[0].y=y;//底点
	Mpt[0].x=Lpt[0].x+6,Mpt[0].y=y;//底点
	for(i=1;i<=8;i++)
	{
		Lpt[i].x=Lpt[0].x;
		Lpt[i].y=Lpt[0].y+i*5;
		Mpt[i].x=Mpt[0].x;
		Mpt[i].y=Mpt[0].y+i*5;
		Rpt[i].x=Rpt[0].x;
		Rpt[i].y=Rpt[0].y+i*5;      
	}
	//绘表
	makeline1(Lpt[0],Lpt[8],1);
	makeline1(Mpt[0],Mpt[8],1);
	makeline1(Rpt[0],Rpt[8],1);
	for(i=0; i<9; i++)
		makeline1(Lpt[i],Rpt[i],1);

	//注字 
	double texth;
	int itextColor;
	texth = 2.5;
	itextColor = 2;
	Tpt.x = 0.5*(Lpt[0].x+Mpt[0].x);
	Tpt.y = 0.5*(Lpt[0].y+Lpt[1].y);
	_tcscpy(str,L"hR");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Lpt[1].y+Lpt[2].y);
	_tcscpy(str,L"hL");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Lpt[2].y+Lpt[3].y);
	_tcscpy(str,L"BR");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Lpt[3].y+Lpt[4].y);
	_tcscpy(str,L"BL");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Lpt[4].y+Lpt[5].y);
	_tcscpy(str,L"At");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Lpt[5].y+Lpt[6].y);
	_tcscpy(str,L"Aw");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Lpt[6].y+Lpt[7].y);
	if(m_ZTWH>0.0)
		_tcscpy(str,L"T");
	else
		_tcscpy(str,L"W");
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	//--------------------------------------填数
	Tpt.x = 0.5*(Mpt[0].x+Rpt[0].x);
	Tpt.y = 0.5*(Mpt[7].y+Mpt[8].y);
	_tcscpy(str,m_ckml);
	maketext(Tpt,str,0,texth,itextColor,3,NULL);	

	Tpt.y = 0.5*(Mpt[6].y+Mpt[7].y);
	if(fabs(m_ZTWH-m_YTWH)<0.001)
	{
		_stprintf(str,L"%0.2lf",m_ZTWH);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}
	else
	{
		_stprintf(str,L"%0.2lf(%0.2lf)",m_ZTWH,m_YTWH);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}

	Tpt.y = 0.5*(Mpt[5].y+Mpt[6].y);
	if(fabs(m_CutArea)>0.001)
	{
		_stprintf(str,L"%0.2lf",m_CutArea);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}

	Tpt.y = 0.5*(Mpt[4].y+Mpt[5].y);
	if(fabs(m_FillArea)>0.001)
	{
		_stprintf(str,L"%0.2lf",m_FillArea);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
#endif // VERSION_CAD2010

	Tpt.y = 0.5*(Mpt[3].y+Mpt[4].y);
	if(LeftDesignLinePart && LeftDesignLinePart->m_DesignPointNum>0)
	{
		_stprintf(str,L"%0.2lf",LeftDesignLinePart->BPLen);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}

	Tpt.y = 0.5*(Mpt[2].y+Mpt[3].y);
	if(RightDesignLinePart && RightDesignLinePart->m_DesignPointNum>0)
	{
		_stprintf(str,L"%0.2lf",RightDesignLinePart->BPLen);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}

	Tpt.y = 0.5*(Mpt[1].y+Mpt[2].y);
	if(LeftDesignLinePart && LeftDesignLinePart->m_DesignPointNum>0)
	{
		_stprintf(str,L"%0.2lf",LeftDesignLinePart->BPH);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}


	Tpt.y = 0.5*(Mpt[0].y+Mpt[1].y);
	if(RightDesignLinePart && RightDesignLinePart->m_DesignPointNum>0)
	{
		_stprintf(str,L"%0.2lf",RightDesignLinePart->BPH);
		maketext(Tpt,str,0,texth,itextColor,3,NULL);	
	}
}


void CROSS::DrawCrossDQLine(AcGePoint3d pjpt)
{

	AcDbDatabase *pCurDb;
	pCurDb = acdbHostApplicationServices()->workingDatabase();
	AcGePoint2d Spt,Ept; 
	double x,y;
	CreateLayer(L"横断面图-中线");

	AcDbPolyline  *pline=NULL;
	AcDbObjectId  ObjId;

	Spt.x=Ept.x=pjpt.x;
	Spt.y = Ymax + 1.0*Sfactor;
	Ept.y = Ymin - 1.0*Sfactor;
	pline = new AcDbPolyline;
	pline->setLinetype(L"DASHDOTX2");//画一线
	pline->addVertexAt(0,Spt);
	pline->addVertexAt(1,Ept);
	pline->setLayer(L"横断面图-挡墙");
	ObjId=AddEntityToDbs(pline) ;
	pline->close();		
}

void CROSS::DrawCrossInfor(double TextH)
{
	ads_real y;
	AcGePoint3d Lpt[9], Rpt[9], Mpt[9], Tpt(0.0, 0.0, 0.0);
	ACHAR str[30];
	int i;

	y = m_NetCenterY - 30.0 * 0.1 * m_Scale;
	AcGePoint3d PtOri(DX - 50 * 0.1 * m_Scale, y, 0.0), PtRight(DX - 50 * 0.1 * m_Scale + 100 * 0.1 * m_Scale, y, 0.0), tempPt1, tempPt2;

	ACHAR LayerName[256];
	_tcscpy(LayerName,L"横断面图-横断面信息");
	//画框
	//画横线
	for (int i = 0; i < 7; i++)
	{
		tempPt1.set(PtOri.x, PtOri.y + i * 10 * 0.1 * m_Scale, 0.0);
		tempPt2.set(PtRight.x, PtRight.y + i * 10 * 0.1 * m_Scale, 0.0);
		makeline(tempPt1, tempPt2, 3, 0, LayerName);
	}

	//画竖线
	tempPt1 = PtOri;
	tempPt2.set(tempPt1.x, tempPt1.y + 60.0 * 0.1 * m_Scale, 0.0);
	//makeline1(tempPt1, tempPt2, 3);
	makeline(tempPt1, tempPt2, 3, 0, LayerName);

	tempPt1.set(PtOri.x + 20.0 * 0.1 * m_Scale, PtOri.y, 0.0);
	tempPt2.set(PtOri.x + 20.0 * 0.1 * m_Scale, PtOri.y + 40.0 * 0.1 * m_Scale, 0.0);
	//makeline1(tempPt1, tempPt2, 3);
	makeline(tempPt1, tempPt2, 3, 0, LayerName);

	tempPt1.set(PtOri.x + 60.0 * 0.1 * m_Scale, PtOri.y, 0.0);
	tempPt2.set(PtOri.x + 60.0 * 0.1 * m_Scale, PtOri.y + 50.0 * 0.1 * m_Scale, 0.0);
	//makeline1(tempPt1, tempPt2, 3);
	makeline(tempPt1, tempPt2, 3, 0, LayerName);

	tempPt1.set(PtOri.x + 100.0 * 0.1 * m_Scale, PtOri.y, 0.0);
	tempPt2.set(PtOri.x + 100.0 * 0.1 * m_Scale, PtOri.y + 60.0 * 0.1 * m_Scale, 0.0);
	//makeline1(tempPt1, tempPt2, 3);
	makeline(tempPt1, tempPt2, 3, 0, LayerName);

	//注字 
	double texth;
	int itextColor;
	texth = TextH * 0.1 * m_Scale;
	itextColor = 3;
	Tpt.x = PtOri.x + 17.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 52.5 * 0.1 * m_Scale;
	_tcscpy(str, L"桩  号：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 17.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 42.5 * 0.1 * m_Scale;
	_tcscpy(str, L"填：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 42.5 * 0.1 * m_Scale;
	_tcscpy(str, L"挖：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 10 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 32.5 * 0.1 * m_Scale;
	_tcscpy(str, L"路基宽");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 27.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 32.5 * 0.1 * m_Scale;
	_tcscpy(str, L"左：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 32.5 * 0.1 * m_Scale;
	_tcscpy(str, L"右：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 10 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 22.5 * 0.1 * m_Scale;
	_tcscpy(str, L"超  高");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 27.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 22.5 * 0.1 * m_Scale;
	_tcscpy(str, L"左：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 22.5 * 0.1 * m_Scale;
	_tcscpy(str, L"右：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 10 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 12.5 * 0.1 * m_Scale;
	_tcscpy(str, L"边  坡");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 27.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 12.5 * 0.1 * m_Scale;
	_tcscpy(str, L"左：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 12.5 * 0.1 * m_Scale;
	_tcscpy(str, L"右：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 10 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 2.5 * 0.1 * m_Scale;
	_tcscpy(str, L"面  积");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 27.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 2.5 * 0.1 * m_Scale;
	_tcscpy(str, L"填：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 2.5 * 0.1 * m_Scale;
	_tcscpy(str, L"挖：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	for (double Height = 22.5 * 0.1 * m_Scale; Height < 50 * 0.1 * m_Scale; Height += 10.0 * 0.1 * m_Scale)
	{
		for (double Width = 55.0 * 0.1 * m_Scale; Width < 100 * 0.1 * m_Scale; Width += 40.0 * 0.1 * m_Scale)
		{
			Tpt.x = PtOri.x + Width;
			Tpt.y = PtOri.y + Height;
			_tcscpy(str, L"M");
			maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
		}
	}

	for (double Width = 55.0 * 0.1 * m_Scale; Width < 100 * 0.1 * m_Scale; Width += 40.0 * 0.1 * m_Scale)
	{
		Tpt.x = PtOri.x + Width;
		Tpt.y = PtOri.y + 2.5 * 0.1 * m_Scale;
		_tcscpy(str, L"M²");
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}

	//--------------------------------------填数
	//里程
	Tpt.x = PtOri.x + 60.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 52.5 * 0.1 * m_Scale;
	_tcscpy(str, m_ckml);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//填挖高信息
	if(m_ZTWH > 0.0)
		Tpt.x = PtOri.x + 45.0 * 0.1 * m_Scale;
	else
		Tpt.x = PtOri.x + 85.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 42.5 * 0.1 * m_Scale;
	_stprintf(str, L"%0.2lf", fabs(m_ZTWH));
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//左右路基宽
	Tpt.x = PtOri.x + 45.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 32.5 * 0.1 * m_Scale;
	double LeftWidth = fabs(LeftDesignLinePart->RHF[8] - LeftDesignLinePart->RHF[0]);
	_stprintf(str, L"%0.2lf", LeftWidth);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 85.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 32.5 * 0.1 * m_Scale;
	double RightWidth = fabs(RightDesignLinePart->RHF[8] - RightDesignLinePart->RHF[0]);
	_stprintf(str, L"%0.2lf", RightWidth);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//左右超高
	Tpt.x = PtOri.x + 45.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 22.5 * 0.1 * m_Scale;
	_stprintf(str, L"%0.2lf", LeftWidth * m_SuphRate_L / 100.0);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 85.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 22.5 * 0.1 * m_Scale;
	_stprintf(str, L"%0.2lf", RightWidth * m_SuphRate_R / 100.0);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//左右边坡
	Tpt.x = PtOri.x + 45.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 12.5 * 0.1 * m_Scale;
	if (LeftDesignLinePart->m_FillORCut < 0.1)
		CalSlopeRateACHAR(LeftDesignLinePart->m_TM[0], str);
	else
		CalSlopeRateACHAR(LeftDesignLinePart->m_WM[0], str);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 85.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 12.5 * 0.1 * m_Scale;
	if (RightDesignLinePart->m_FillORCut < 0.1)
		CalSlopeRateACHAR(RightDesignLinePart->m_TM[0], str);
	else
		CalSlopeRateACHAR(RightDesignLinePart->m_WM[0], str);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//面积
	Tpt.x = PtOri.x + 40.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 2.5 * 0.1 * m_Scale;
	if (fabs(m_FillArea) > 0.001)
	{
		_stprintf(str, L"%0.2lf", m_FillArea);
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}

	Tpt.x = PtOri.x + 80.0 * 0.1 * m_Scale;
	Tpt.y = PtOri.y + 2.5 * 0.1 * m_Scale;
	if (fabs(m_CutArea) > 0.001)
	{
		_stprintf(str, L"%0.2lf", m_CutArea);
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}

}

void CROSS::CalSlopeRateACHAR(double m, ACHAR Str[])
{
	if (fabs(m - Round(m + 0.001)) < 0.1)
	{
		_stprintf(Str, L"1:%d\n", (int)(m + 0.01));
	}
	else if (fabs(m * 10 -Round((m + 0.001) * 10)) < 0.1)
	{
		//	  ads_printf(L"%lf \n",m);
		_stprintf(Str, L"1:%2.1f\n", (float)m);
	}
	else
	{
		_stprintf(Str, L"1:%3.2f\n", (float)m);
	}
}

double CROSS::Round(double Value)
{
	return (Value > 0.0) ? floor(Value + 0.5) : ceil(Value - 0.5);
}

void CROSS::DrawGutterBlock(AcGePoint3d OriPt)
{
	CFileFind fd;
	if (fd.FindFile(m_GutterPath))
	{
		ACHAR str[256];
		_stprintf(str, L"%s", m_GutterPath);
		AcDbVoidPtrArray entitySet;
		AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(str, m_GutterBlockName);
		if (pBlockId > 0)
		{
			AcDbBlockReference* pBkRr = new AcDbBlockReference(OriPt, pBlockId);
			pBkRr->setScaleFactors(AcGeScale3d(1, 1, 1));
			CreateLayer(L"横断面图-改沟图块");
			pBkRr->setLayer(L"横断面图-改沟图块");
			pBkRr->setColorIndex(0);
			BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
		}
	}
}

/*===================================================================
函数名:DrawZDK
------------------------------------------------------------
函数功能:绘制占地宽信息
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	
------------------------------------------------------------
编写日期:20190823
------------------------------------------------------------
备注：ljx
=====================================================================*/
void CROSS::DrawZDK()
{
	//设置参数
	AcGePoint3d tempPt(0.0, 0.0, 0.0);
	AcGePoint3dArray PtArray3D;
	ACHAR ZDKText[256], LayerName[256];
	_tcscpy(LayerName, L"横断面图-占地宽");

	//绘制左侧
	//根据左侧占地宽寻找对应地面点坐标
	int tempNum = 0;
	double EarX = m_OriPoint.x - m_LeftZdk * m_Scale;
	double EarY = 0.0;
	if (LeftEarthLinePart->Cal_Yd(EarX, EarY, tempNum) == 1)	//找不到就不画了，免得画到零点更加怪异
	{
		//绘制框
		PtArray3D.removeAll();
		PtArray3D.append(tempPt.set(EarX, EarY - 0.25 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX - 0.25 * m_Scale, EarY - 0.25 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX - 0.25 * m_Scale, EarY + 2 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX, EarY + 2 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX, EarY - 0.25 * m_Scale, 0.0));
		makepolyline(PtArray3D, true, GREEN, AcDb::kLnWtByLayer, LayerName);

		//绘制框上直线
		PtArray3D.removeAll();
		PtArray3D.append(tempPt.set(EarX, EarY + 2.25 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX, EarY + 4.25 * m_Scale, 0.0));
		makepolyline(PtArray3D, true, GREEN, AcDb::kLnWtByLayer, LayerName);

		//绘制文字
		_stprintf(ZDKText, L"%.2lfm", m_LeftZdk);
		maketext(tempPt.set(EarX - 0.2 * m_Scale, EarY + 3.25 * m_Scale, 0.0), ZDKText, pi / 2.0, 0.4 * m_Scale, GREEN, 2, LayerName);
	}

	//绘制右侧
	//根据右侧占地宽寻找对应地面点坐标
	EarX = m_OriPoint.x + m_RightZdk * m_Scale;
	EarY = 0.0;
	if (RightEarthLinePart->Cal_Yd(EarX, EarY, tempNum) == 1)	//找不到就不画了，免得画到零点更加怪异
	{
		//绘制框
		PtArray3D.removeAll();
		PtArray3D.append(tempPt.set(EarX, EarY - 0.25 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX + 0.25 * m_Scale, EarY - 0.25 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX + 0.25 * m_Scale, EarY + 2 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX, EarY + 2 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX, EarY - 0.25 * m_Scale, 0.0));
		makepolyline(PtArray3D, true, GREEN, AcDb::kLnWtByLayer, LayerName);

		//绘制框上直线
		PtArray3D.removeAll();
		PtArray3D.append(tempPt.set(EarX, EarY + 2.25 * m_Scale, 0.0));
		PtArray3D.append(tempPt.set(EarX, EarY + 4.25 * m_Scale, 0.0));
		makepolyline(PtArray3D, true, GREEN, AcDb::kLnWtByLayer, LayerName);

		//绘制文字
		_stprintf(ZDKText, L"%.2lfm", m_RightZdk);
		maketext(tempPt.set(EarX - 0.2 * m_Scale, EarY + 3.25 * m_Scale, 0.0), ZDKText, pi / 2.0, 0.4 * m_Scale, GREEN, 2, LayerName);
	}
}

void CROSS::DrawCrossInfor_Gutter()
{
	ads_real y;
	AcGePoint3d Lpt[9], Rpt[9], Mpt[9], Tpt(0.0, 0.0, 0.0);
	ACHAR str[30];
	int i;

	y = m_NetCenterY - 15.0;
	AcGePoint3d PtOri(DX - 50, y, 0.0), PtRight(DX - 50 + 100, y, 0.0), tempPt1, tempPt2;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"横断面图-横断面信息");

	//画框
	//画横线
	for (int i = 0; i < 4; i++)
	{
		tempPt1.set(PtOri.x, PtOri.y + i * 10, 0.0);
		tempPt2.set(PtRight.x, PtRight.y + i * 10, 0.0);
		makeline(tempPt1, tempPt2, GREEN, 0.0, LayerName);
	}

	//画竖线
	tempPt1 = PtOri;
	tempPt2.set(tempPt1.x, tempPt1.y + 30.0, 0.0);
	makeline(tempPt1, tempPt2, GREEN, 0.0, LayerName);

	tempPt1.set(PtOri.x + 20.0, PtOri.y, 0.0);
	tempPt2.set(PtOri.x + 20.0, PtOri.y + 10.0, 0.0);
	makeline(tempPt1, tempPt2, GREEN, 0.0, LayerName);

	tempPt1.set(PtOri.x + 60.0, PtOri.y, 0.0);
	tempPt2.set(PtOri.x + 60.0, PtOri.y + 20.0, 0.0);
	makeline(tempPt1, tempPt2, GREEN, 0.0, LayerName);

	tempPt1.set(PtOri.x + 100.0, PtOri.y, 0.0);
	tempPt2.set(PtOri.x + 100.0, PtOri.y + 30.0, 0.0);
	makeline(tempPt1, tempPt2, GREEN, 0.0, LayerName);

	//注字 
	double texth;
	int itextColor;
	texth = 5.0;
	itextColor = 3;
	Tpt.x = PtOri.x + 17.5;
	Tpt.y = PtOri.y + 22.5;
	_tcscpy(str, L"桩  号：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 17.5;
	Tpt.y = PtOri.y + 12.5;
	_tcscpy(str, L"填：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5;
	Tpt.y = PtOri.y + 12.5;
	_tcscpy(str, L"挖：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 10;
	Tpt.y = PtOri.y + 2.5;
	_tcscpy(str, L"面  积");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 27.5;
	Tpt.y = PtOri.y + 2.5;
	_tcscpy(str, L"填：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	Tpt.x = PtOri.x + 67.5;
	Tpt.y = PtOri.y + 2.5;
	_tcscpy(str, L"挖：");
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	for (double Width = 55.0; Width < 100; Width += 40.0)
	{
		Tpt.x = PtOri.x + Width;
		Tpt.y = PtOri.y + 12.5;
		_tcscpy(str, L"M");
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}

	for (double Width = 55.0; Width < 100; Width += 40.0)
	{
		Tpt.x = PtOri.x + Width;
		Tpt.y = PtOri.y + 2.5;
		_tcscpy(str, L"M²");
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}

	//--------------------------------------填数
	//里程
	Tpt.x = PtOri.x + 60.0;
	Tpt.y = PtOri.y + 22.5;
	_tcscpy(str, m_ckml);
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//填挖高信息
	if (m_ZTWH > 0.0)
		Tpt.x = PtOri.x + 45.0;
	else
		Tpt.x = PtOri.x + 85.0;
	Tpt.y = PtOri.y + 12.5;
	_stprintf(str, L"%0.2lf", fabs(m_ZTWH));
	maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);

	//面积
	Tpt.x = PtOri.x + 40.0;
	Tpt.y = PtOri.y + 2.5;
	if (fabs(m_FillArea) > 0.001)
	{
		_stprintf(str, L"%0.2lf", m_FillArea);
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}

	Tpt.x = PtOri.x + 80.0;
	Tpt.y = PtOri.y + 2.5;
	if (fabs(m_CutArea) > 0.001)
	{
		_stprintf(str, L"%0.2lf", m_CutArea);
		maketext(Tpt, str, 0, texth, itextColor, 2, L"宋体", LayerName);
	}
}






































