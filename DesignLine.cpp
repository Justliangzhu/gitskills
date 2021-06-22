// DesignLine.cpp: implementation of the DesignLine class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DesignLine.h"
#include "HdmDes.h"
#include "geplin3d.h" 
// #include "MdyLMK.h"
// #include "LMHPdlg.h"
// #include "LMBG.h"
//#include "oridata.h"
//#include "HdmBPdlg.h"
extern double Sfactor;
extern HdmDes HDM;
// extern struct OriData::LDBPdata GlobalBPdata;	                   

#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
	else if ((x) > 0)    \
	y =1;             \
	else                 \
	y = -1;           \
}

ACRX_DXF_DEFINE_MEMBERS(DesignLine_ROAD, AcDbEntity,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0,AcDbProxyEntity::kNoOperation,
						DESIGNLINE,RailHdm);


EarthLine_ROAD* my_l_earthline;
EarthLine_ROAD* my_r_earthline;
LTGOU_ROAD *    my_LTGOU;
JSGOU_ROAD *    my_JSGOU;
HdmText_ROAD *  my_Note;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DesignLine_ROAD::DesignLine_ROAD()
{
	DesignLineColor = 3;
	lmhdcolor = 6;
	m_DesignPointNum = 0;
	m_DesignPoints = NULL;
	int i;
	for(i=0; i<30; i++)
		m_TM[i]=m_TH[i]=m_TPTW[i]=m_TPTI[i]=m_WM[i]=m_PH[i]=m_ZPTW[i]=m_ZPTI[i]=0.0;
	m_nLTBPJS=m_nLZBPJS=0;
	for(i=0;i<29;i++)
		JqPt[i].x=0.0;JqPt[i].y=0.0;
	JqPtNum=0;
	LnewYd=RnewYd=0.0;

	m_ZorY=1;
	//double DorS;
	//xlxz,r_bh;//线路性质，饶行段编号
	BTinf=0.0;
	cml=0.0;
	m_Xcdh=m_Yljh=m_Tljh=0.0;
	m_FillORCut=0.0;
	for(i=0; i<10; i++)
		RHF[i]=0.0;
	for(i=0; i<3; i++)
		LMHP[i]=0.0;
	GK=GS=GP1=GP2=CG_JQHD=CG_GDR=0.0;//边沟信息
	BPLen=BPH=0.0;//边坡长与高
	ifMoveGripPoints=false;
	l_earthline=NULL;
	r_earthline=NULL;
	LTGou=NULL;
	JSGou=NULL;
	Note=NULL;
	for(i=0; i<4; i++)
		m_ArcBpR[i]=0.0;
	gouId=NULL;
	m_GouPropty = -1;

	m_IsModifyGutter = false;
	m_GutterType = -1;
}

DesignLine_ROAD::~DesignLine_ROAD()
{
}

Adesk::Boolean DesignLine_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{

	AcGePoint3d HDpt[8];
	assertReadEnabled();
	//	
	if(ifMoveGripPoints)//夹点操作时，不断计算边坡点
	{
		HDM.SDM_AutoDesign(this,l_earthline,LTGou,JSGou,r_earthline,Note);
		ifMoveGripPoints=false;//夹点操作时在undo时只调用响应器和worlddraw，不调用MoveGripPoints，不重新计算边坡点
	}
	//	for(int i=0; i<m_DesignPointNum; i++)
	//		ads_printf(L"i=%d %lf %lf %lf\n",i,cml,m_DesignPoints[i].x,m_DesignPoints[i].y);
	//绘设计线
	pWd->subEntityTraits().setColor(DesignLineColor);

	//绘设计线
	int desptnum;
	AcGePoint3d despt[500];

	desptnum = m_DesignPointNum;
	for(int k=0; k<desptnum; k++)
		despt[k] = m_DesignPoints[k];

	//pWd->geometry().polyline(m_DesignPointNum,m_DesignPoints);
	bool ifsetarcbp1,ifsetarcbp2,ifsetarcbp3,ifsetarcbp4;
	ifsetarcbp1=ifsetarcbp2=ifsetarcbp3=ifsetarcbp4=false;
	//ads_printf(L"@@@@@ %lf %lf %lf %lf %lf %d\n",cml,m_ArcBpR[0],m_ArcBpR[1],m_ArcBpR[2],m_ArcBpR[3],m_DesignPointNum);
	if(m_ArcBpR[0]>0.001 && m_FillORCut<0.001)//土路肩设弧形边坡 填方
	{
		//修正设计点
		AcGePoint3d arcpt[31];
		if(DesArcBp(m_ArcBpR[0],4,arcpt,'T'))
		{
			despt[4] = arcpt[0];
			int i;
			for( i=desptnum-1; i>=5; i--)
				despt[i+31] = despt[i];
			for(i=0; i<=30; i++)
				despt[5+i] = arcpt[i];
			desptnum = desptnum+31;
		}

	}
	if(m_ArcBpR[2]>0.001 && m_FillORCut>0.001)//路堑平台设弧形边坡 挖方
	{
		//修正设计点
		AcGePoint3d arcpt[31];
		if(DesArcBp(m_ArcBpR[2],8,arcpt,'W'))
		{
			despt[8] = arcpt[0];
			int i;
			for( i=desptnum-1; i>=9; i--)
				despt[i+31] = despt[i];
			for(i=0; i<=30; i++)
				despt[9+i] = arcpt[i];
			desptnum = desptnum+31;
		}
	}
	if(m_ArcBpR[3]>0.001 && m_FillORCut>0.001 || (LTGou->m_LTGouPointNum<1 && m_ArcBpR[1]>0.001 && m_FillORCut<0.001))//路堑坡顶处设置弧形边坡 挖方
	{
		//修正设计点
		AcGePoint3d arcpt[31];
		char torw;
		if(m_FillORCut>0.001)
			torw='W';
		else
			torw='T';

		if(DesTopArcBp(m_ArcBpR[3],arcpt,torw))
		{
			despt[desptnum-1] = arcpt[0];
			for(int i=0; i<=30; i++)
				despt[desptnum+i] = arcpt[i];
			desptnum = desptnum+31;
		}
	}

	if(LTGou->m_LTGouPointNum>1 && m_ArcBpR[1]>0.001 && m_FillORCut<0.001)//路堤坡脚处设置弧形边坡 有水沟
	{

		//修正设计点
		AcGePoint3d arcpt[31];
		if(DesArcTFBpWithLTgou(m_ArcBpR[2],m_DesignPointNum-1,arcpt,'W'))
		{
			despt[desptnum-1] = arcpt[0];
			for(int i=0; i<=30; i++)
				despt[desptnum+i] = arcpt[i];
			desptnum = desptnum+31;
			LTGou->m_LTGouPoints[0]=arcpt[30];
		}
	}

	if((fabs(m_FillORCut-1.0)<0.001 || fabs(m_FillORCut-3.0)<0.001) && m_DesignPointNum>7)//挖方或路肩沟
	{
		if (CG_GDR > 0.001)//如果有弧形水沟，则内插10个弧形点
		{
			//1计算出沟底弧形点
			double spt[2],ept[2],pt[20];
			if (m_ZorY>0)
			{
				spt[0]=despt[5].x;
				spt[1]=despt[5].y;
				ept[0]=despt[6].x;
				ept[1]=despt[6].y;
			}

			else
			{
				spt[0]=despt[6].x;
				spt[1]=despt[6].y;
				ept[0]=despt[5].x;
				ept[1]=despt[5].y;
			}

			GetArcPt(spt,ept,CG_GDR*Sfactor,10,pt);
			//2修正设计点
			int npt;
			double TmpDesPt[500];
			npt=0;
			int i;
			for(i=0; i<=5; i++)
			{
				TmpDesPt[npt*2] = despt[i].x;
				TmpDesPt[npt*2+1] = despt[i].y;
				npt++;
			}

			if (m_ZorY>0)
			{
				for(i=0; i<10; i++)
				{
					TmpDesPt[npt*2] = pt[i*2];
					TmpDesPt[npt*2+1] = pt[i*2+1];
					npt++;
				}
			}

			else
			{
				for(i=9; i>-1; i--)
				{
					TmpDesPt[npt*2] = pt[i*2];
					TmpDesPt[npt*2+1] = pt[i*2+1];
					npt++;
				}
			}

			for(i=6; i<desptnum; i++)
			{
				TmpDesPt[npt*2] = despt[i].x;
				TmpDesPt[npt*2+1] = despt[i].y;
				npt++;
			}


			desptnum = npt;
			for(i=0; i<desptnum; i++)
			{
				despt[i].x = TmpDesPt[i*2];
				despt[i].y = TmpDesPt[i*2+1];
			}
		}

		if (fabs(CG_JQHD)>0.001)
		{
			CGhasJQ(m_ZorY,despt,CG_JQHD,CG_GDR*Sfactor,JqPtNum,JqPt);//计算浆砌点
			pWd->subEntityTraits().setColor(DesignLineColor);
			pWd->geometry().polyline(JqPtNum,JqPt);
		}

	}
	pWd->geometry().polyline(desptnum,despt);

	std::vector<AcGePoint3d> watchpt;
	for (int i = 0; i < m_DesignPointNum; i++)
		watchpt.push_back(m_DesignPoints[i]);

	if(m_DesignPointNum>=4)
	{
		HDpt[0] = m_DesignPoints[1];
		HDpt[1] = m_DesignPoints[1];
		HDpt[1].y = HDpt[1].y - m_Xcdh*Sfactor;
		HDpt[2] = m_DesignPoints[2];
		HDpt[2].y = HDpt[2].y - m_Xcdh*Sfactor;
		HDpt[3] = m_DesignPoints[2];
		HDpt[3].y = HDpt[3].y - m_Yljh*Sfactor;
		HDpt[4] = m_DesignPoints[3];
		HDpt[4].y = HDpt[4].y - m_Yljh*Sfactor;
		HDpt[5] = m_DesignPoints[3];
		HDpt[5].y = HDpt[5].y - m_Tljh*Sfactor;
		HDpt[6] = m_DesignPoints[4];
		HDpt[6].y = HDpt[6].y - m_Tljh*Sfactor;	
		HDpt[7] = m_DesignPoints[4];

		pWd->subEntityTraits().setColor(lmhdcolor);
		if(m_Tljh>0.0001)
			pWd->geometry().polyline(8,HDpt);
		else
			pWd->geometry().polyline(6,HDpt);

		if(RHF[8]>0.001)
		{
			BZDesh(pWd,m_DesignPoints[1],RHF[3]);
			BZDesh(pWd,m_DesignPoints[2],RHF[5]);
			BZDesh(pWd,m_DesignPoints[3],RHF[7]);
			BZDesh(pWd,m_DesignPoints[4],RHF[9]);
		}
		else
			BZDesh(pWd,m_DesignPoints[4],RHF[9]);

	}


	CalBP();

	return Adesk::kTrue;
}
void DesignLine_ROAD::Draw(double NoteTextH)
{
	AcGePoint3dArray ptarray,ptarray1,ptarray2;
	int m;
	int iColor;
	assertReadEnabled();
	//绘设计线
	if(m_DesignPointNum<1)
		return;
	//iColor = 3;
	//绘设计线
	int desptnum;
	AcGePoint3d despt[500];

	desptnum = m_DesignPointNum;
	for(int k=0; k<desptnum; k++)
		despt[k] = m_DesignPoints[k];

	//pWd->geometry().polyline(m_DesignPointNum,m_DesignPoints);
	bool ifsetarcbp1,ifsetarcbp2,ifsetarcbp3,ifsetarcbp4;
	ifsetarcbp1=ifsetarcbp2=ifsetarcbp3=ifsetarcbp4=false;
	//ads_printf(L"@@@@@ %lf %lf %lf %lf %lf %d\n",cml,m_ArcBpR[0],m_ArcBpR[1],m_ArcBpR[2],m_ArcBpR[3],m_DesignPointNum);
	if(m_ArcBpR[0]>0.001 && m_FillORCut<0.001)//土路肩设弧形边坡 填方
	{
		//修正设计点
		AcGePoint3d arcpt[31];
		if(DesArcBp(m_ArcBpR[0],4,arcpt,'T'))
		{
			despt[4] = arcpt[0];
			int i;
			for( i=desptnum-1; i>=5; i--)
				despt[i+31] = despt[i];
			for(i=0; i<=30; i++)
				despt[5+i] = arcpt[i];
			desptnum = desptnum+31;
		}

	}
	if(m_ArcBpR[2]>0.001 && m_FillORCut>0.001)//路堑平台设弧形边坡 挖方
	{
		//修正设计点
		AcGePoint3d arcpt[31];
		if(DesArcBp(m_ArcBpR[2],8,arcpt,'W'))
		{
			despt[8] = arcpt[0];
			int i;
			for( i=desptnum-1; i>=9; i--)
				despt[i+31] = despt[i];
			for(i=0; i<=30; i++)
				despt[9+i] = arcpt[i];
			desptnum = desptnum+31;
		}
	}
	if(m_ArcBpR[3]>0.001 && m_FillORCut>0.001 || (LTGou->m_LTGouPointNum<1 && m_ArcBpR[1]>0.001 && m_FillORCut<0.001))//路堑坡顶处设置弧形边坡 挖方
	{
		//修正设计点
		AcGePoint3d arcpt[31];
		char torw;
		if(m_FillORCut>0.001)
			torw='W';
		else
			torw='T';

		if(DesTopArcBp(m_ArcBpR[3],arcpt,torw))
		{
			despt[desptnum-1] = arcpt[0];
			for(int i=0; i<=30; i++)
				despt[desptnum+i] = arcpt[i];
			desptnum = desptnum+31;
		}
	}

	if(LTGou->m_LTGouPointNum>1 && m_ArcBpR[1]>0.001 && m_FillORCut<0.001)//路堤坡脚处设置弧形边坡 有水沟
	{

		//修正设计点
		AcGePoint3d arcpt[31];
		if(DesArcTFBpWithLTgou(m_ArcBpR[2],m_DesignPointNum-1,arcpt,'W'))
		{
			despt[desptnum-1] = arcpt[0];
			for(int i=0; i<=30; i++)
				despt[desptnum+i] = arcpt[i];
			desptnum = desptnum+31;
			LTGou->m_LTGouPoints[0]=arcpt[30];
			LTGOU_ROAD *ltgou = NULL;
			if (gouId && acdbOpenObject(ltgou,gouId,AcDb::kForWrite)==Acad::eOk) 
			{
				ltgou->assertWriteEnabled();
				ltgou->m_LTGouPoints[0]=arcpt[30];
				ltgou->close();
			}
		}
	}

	if((fabs(m_FillORCut-1.0)<0.001 || fabs(m_FillORCut-3.0)<0.001) && m_DesignPointNum>7)//挖方或路肩沟
	{
		if (CG_GDR > 0.001)//如果有弧形水沟，则内插10个弧形点
		{
			//1计算出沟底弧形点
			double spt[2],ept[2],pt[20];
			if (m_ZorY>0)
			{
				spt[0]=despt[5].x;
				spt[1]=despt[5].y;
				ept[0]=despt[6].x;
				ept[1]=despt[6].y;
			}

			else
			{
				spt[0]=despt[6].x;
				spt[1]=despt[6].y;
				ept[0]=despt[5].x;
				ept[1]=despt[5].y;
			}

			GetArcPt(spt,ept,CG_GDR*Sfactor,10,pt);
			//2修正设计点
			int npt;
			double TmpDesPt[500];
			npt=0;
			int i;
			for( i=0; i<=5; i++)
			{
				TmpDesPt[npt*2] = despt[i].x;
				TmpDesPt[npt*2+1] = despt[i].y;
				npt++;
			}

			if (m_ZorY>0)
			{
				for(i=0; i<10; i++)
				{
					TmpDesPt[npt*2] = pt[i*2];
					TmpDesPt[npt*2+1] = pt[i*2+1];
					npt++;
				}
			}

			else
			{
				for(i=9; i>-1; i--)
				{
					TmpDesPt[npt*2] = pt[i*2];
					TmpDesPt[npt*2+1] = pt[i*2+1];
					npt++;
				}
			}

			for(i=6; i<desptnum; i++)
			{
				TmpDesPt[npt*2] = despt[i].x;
				TmpDesPt[npt*2+1] = despt[i].y;
				npt++;
			}


			desptnum = npt;
			for(i=0; i<desptnum; i++)
			{
				despt[i].x = TmpDesPt[i*2];
				despt[i].y = TmpDesPt[i*2+1];
			}
		}

		if (fabs(CG_JQHD)>0.001)
		{
			CGhasJQ(m_ZorY,despt,CG_JQHD,CG_GDR*Sfactor,JqPtNum,JqPt);//计算浆砌点			
			//画侧沟的浆切
			for(m=0;m<JqPtNum;m++)
				ptarray1.append(JqPt[m]);
			make2dline(ptarray1, DesignLineColor, 0.0, 0.0, L"横断面图-设计线");
		}
	}

	for(int i=0; i<desptnum; i++)
		ptarray.append(despt[i]);
	make2dline(ptarray, DesignLineColor, 0.5 * 0.1 * Sfactor, 0.5 * 0.1 * Sfactor, L"横断面图-设计线");


	//标注设计高
	if (!m_IsModifyGutter)
	{
		ACHAR Str[256];
		_stprintf(Str, L"%0.3lf", RHF[9]);//路肩设计高
		NoteBGWithLine(m_DesignPoints[4], NoteTextH/*0.4 * m_sfactor*/, Str, GREEN, YELLOW, Sfactor, 2, L"横断面图-标高");
		if (m_ZorY < 0)
		{
			_stprintf(Str, L"%0.3lf", RHF[1]);//路中心设计高
			NoteBGWithLine(m_DesignPoints[0], NoteTextH/*0.4 * m_sfactor*/, Str, GREEN, YELLOW, Sfactor, 2, L"横断面图-标高");
		}

		//标注横坡
		NoteRoadCrossSlope(NoteTextH);
	}
	else
	{
		ACHAR Str[256];
		AcGePoint3d tempPt = m_DesignPoints[0];
		tempPt.y += m_GutterThick / 100.0 * m_sfactor;
		if (m_ZorY < 0)
		{
			_stprintf(Str, L"%0.3lf", RHF[1] + m_GutterThick / 100.0);//路中心设计高
			NoteBG(tempPt, NoteTextH/*0.4 * m_sfactor*/, Str, MAGENTA, YELLOW, Sfactor, 2, L"横断面图-标高");
		}
	}

	//if (m_ZorY == -1 || m_ZorY == -2)
	//	//NoteBG(m_DesignPoints[4], 0.4 * m_sfactor, Str, 6, 2, Sfactor, 2);
	//	
	//else
	//	//NoteBG(m_DesignPoints[4], 0.4 * m_sfactor, Str, 6, 2, Sfactor, 2);
	//	NoteBGWithLine(m_DesignPoints[4], 0.4 * m_sfactor, Str, 6, 2, Sfactor, 2);



	//画路面厚度
	ptarray.removeSubArray(0,ptarray.length()-1);
	AcGePoint3d HDpt[8];
	if(m_DesignPointNum>=4)
	{
		HDpt[0] = m_DesignPoints[1];
		HDpt[1] = m_DesignPoints[1];
		HDpt[1].y = HDpt[1].y - m_Xcdh*Sfactor;
		HDpt[2] = m_DesignPoints[2];
		HDpt[2].y = HDpt[2].y - m_Xcdh*Sfactor;
		HDpt[3] = m_DesignPoints[2];
		HDpt[3].y = HDpt[3].y - m_Yljh*Sfactor;
		HDpt[4] = m_DesignPoints[3];
		HDpt[4].y = HDpt[4].y - m_Yljh*Sfactor;
		HDpt[5] = m_DesignPoints[3];
		HDpt[5].y = HDpt[5].y - m_Tljh*Sfactor;
		HDpt[6] = m_DesignPoints[4];
		HDpt[6].y = HDpt[6].y - m_Tljh*Sfactor;	
		HDpt[7] = m_DesignPoints[4];

		for(int i=0; i<8; i++)
			ptarray.append(HDpt[i]);
		//	iColor = 6;
		make2dline(ptarray,lmhdcolor,0.0,0.0, L"横断面图-路面厚度");

	}


}

void DesignLine_ROAD::subList() const
{
	assertReadEnabled();

	AcDbEntity::list();

	ads_printf(L"\n\t\t自定义横断面设计线实体");
	ads_printf(L"\n\t\t里程 = %0.3lf %d %s",cml,m_ZorY,RoadName);

	// 	ads_printf(L"\n\t\t设计点数 = %d", m_DesignPointNum);
	// 	for(int i=0; i<m_DesignPointNum; i++)
	// 	   ads_printf(L"\n\t\tx=%0.3lf y=%0.3lf", m_DesignPoints[i].x,m_DesignPoints[i].y);



}

int DesignLine_ROAD::Cal_Yd(double x,double &Yd,int &ptnum,AcGePoint3dArray &despts)
{
	int i,ptsum;
	AcGePoint3d pt;

	ptsum = despts.length();
	i = 0;
	Yd = despts.at(0).y;
	ptnum = 0;
	if(m_ZorY<0)//左侧
	{
		while(x < despts.at(i).x-0.001 && i<ptsum)
			i++;
	}
	else
	{
		while(x > despts.at(i).x+0.001 && i<ptsum)
			i++;
	}
	if(i==ptsum)
	{
		Yd = despts.at(i-1).y;
		ptnum = i-1;
		return 0;//找不到
	}
	if(i==0)
		return -1;		
	ptnum = i;
	Yd = despts.at(i-1).y + (x-despts.at(i-1).x)*(despts.at(i).y-despts.at(i-1).y)/(despts.at(i).x-despts.at(i-1).x);

	return 1;//找到
}
Acad::ErrorStatus  DesignLine_ROAD::subTranformBy(const AcGeMatrix3d &)
{
	return Acad::eOk;
}
Acad::ErrorStatus DesignLine_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
													AcDbIntArray& osnapModes,
													AcDbIntArray& geomIds) const
{
	assertReadEnabled();

	int i;

	for(i=0; i<m_DesignPointNum; i++)
		gripPoints.append(m_DesignPoints[i]);

	return Acad::eOk;
}
Acad::ErrorStatus  DesignLine_ROAD::subGetTransformedCopy(	const AcGeMatrix3d& mat,
														  AcDbEntity*& ent) const
{
	assertReadEnabled();
	Acad::ErrorStatus es = Acad::eOk;
	return es;
}
Acad::ErrorStatus DesignLine_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,
													   const AcGeVector3d& offset)
{
	assertWriteEnabled();
	ifMoveGripPoints=true;
	//ifJD=true;
	if(indices.length()==0||offset.isZeroLength())
		return Acad::eOk;
	int i,k,inf,zory,m;
	double MM,PTI,rhf;

	CalBP();
	sign(inf,offset[X]);//得到offset[X]的符号,向右为正，向左为负
	sign(zory,m_ZorY);
	rhf=RHF[3];
	for(int j=0;j<indices.length();j++)//indices 热点
	{
		k=indices[j];//0.1.===m_despointsnum-1,
		//沿路面延长线方向移动	
		//		if(k<5)//k=4.3.2
		//		{
		//			if(fabs(fabs(offset[Y])-fabs(offset[X])*0.04)<5)
		//			{
		//				RHF[k*2]=fabs(RHF[(k-1)*2]+zory*inf*sqrt(offset[X]*offset[X]+offset[Y]*offset[Y])/Sfactor);
		//				RHF[k*2+1]=rhf-(RHF[(k-1)*2]*0.04)/Sfactor;
		//			}
		//		}
		if((k>=5&&m_FillORCut<0.1)||(k>=8&&m_FillORCut>0.99))//沿坡度方向移动
		{
			if(k>=5&&m_FillORCut<0.1)
			{
				if(k==2||k==3||k==4) k==4;//路面有三个点重
				for(m=1;m<31;m++)
					if(k-3<=2*m) break;//激活点处于哪个边坡段
				MM=m_TM[m-1];
			}
			if(k>=8&&m_FillORCut>0.99)
			{
				for(m=1;m<31;m++)
					if(k-7<=2*m) break;//激活点处于哪个边坡段
				MM=m_WM[m-1];
			}
			if(fabs(fabs(offset[X])-fabs(offset[Y])*MM)<10)//沿坡度方向移动
			{
				//ads_printf(L"1:%d m=%d %lf %lf %lf offsetx=%lf oy=%lf\n",m_nLZBPJS,m-1,m_PH[m-1],m_PH[m],m_ZPTW[m-1],offset[X],offset[Y]);	
				if(k<m_DesignPointNum)//考虑6级边坡
				{

					if(m_FillORCut<0.1) 
					{
						if(fmod((k-3),2.0)!=0.0)//9.11.13向上真加，向下减少
							m_TH[m-1]=fabs(m_TH[m-1]+offset[Y]/Sfactor);
						else//5.7.9向上减少，向下真加
							m_TH[m-1]=fabs(m_TH[m-1]-offset[Y]/Sfactor);
					}
					else 
					{
						if(fmod((k-7),2.0)!=0.0)//8.10.12
							m_PH[m-1]=fabs(m_PH[m-1]-offset[Y]/Sfactor);
						else
							m_PH[m-1]=fabs(m_PH[m-1]+offset[Y]/Sfactor);
					}
				}

			}
			else//
			{
				if(k>=5&&m_FillORCut<0.1)
				{
					for(m=1;m<31;m++)
						if(k-4<=2*m) break;//激活点处于哪个边坡段
					PTI=m_TPTI[m-1];
				}
				if(k>=7&&fabs(m_FillORCut-1)<0.001)
				{
					for(m=1;m<31;m++)
						if(k-6<=2*m) break;//激活点处于哪个边坡段
					PTI=m_ZPTI[m-1];
				}

				if(fabs(fabs(offset[Y])-fabs(offset[X])*PTI)<5)//沿平台方向移动
				{
					if(k<m_DesignPointNum)//考虑6级边坡
					{
						if(m_FillORCut<0.1) 
						{
							if(fmod((k-4),2.0)!=0.0)//5.6.7向左真加，向右减少
								m_TPTW[m-1]=fabs(m_TPTW[m-1]-zory*inf*sqrt(offset[Y]*offset[Y]+offset[X]*offset[X])/Sfactor);
							else//6.8.10向左减少，向右真加
								m_TPTW[m-1]=fabs(m_TPTW[m-1]+zory*inf*sqrt(offset[Y]*offset[Y]+offset[X]*offset[X])/Sfactor);
						}
						else 
						{
							if(fmod((k-6),2.0)!=0.0)//5.7.9向左真加，向右减少
							{
								m_ZPTW[m-1]=fabs(m_ZPTW[m-1]-zory*inf*sqrt(offset[Y]*offset[Y]+offset[X]*offset[X])/Sfactor);
							}
							else//6.8.10向左减少，向右真加
								m_ZPTW[m-1]=fabs(m_ZPTW[m-1]+zory*inf*sqrt(offset[Y]*offset[Y]+offset[X]*offset[X])/Sfactor);
						}
					}
				}
			}
		}
	}
	//ads_printf(L"3: m=%d %lf %lf offsetx=%lf oy=%lf\n",m,m_PH[m-1],m_ZPTW[m-1],offset[X],offset[Y]);	
	return Acad::eOk;
}

int DesignLine_ROAD::Set_LM(int mode)
{
	assertWriteEnabled();   
	/*
	if(mode==1)//修改路面宽
	{
	double fgdk,xcdk,yljk,tljk;
	fgdk = RHF[0];
	xcdk = RHF[2] - RHF[0];
	yljk = RHF[4] - RHF[2];
	tljk = RHF[6] - RHF[4];
	MdyLMK lmkdlg(NULL,fgdk,xcdk,yljk,tljk);

	if(lmkdlg.DoModal()==IDOK)
	{
	RHF[0] = lmkdlg.m_fgdk;
	RHF[2] = RHF[0] + lmkdlg.m_xcdk;
	RHF[3] = RHF[1] + lmkdlg.m_xcdk*LMHP[0];
	RHF[4] = RHF[2] + lmkdlg.m_yljk;
	RHF[5] = RHF[3] + lmkdlg.m_yljk*LMHP[1];
	RHF[6] = RHF[4] +	lmkdlg.m_tljk;		 
	RHF[7] = RHF[5] + lmkdlg.m_tljk*LMHP[2];
	return 1;
	}
	}
	else if(mode==2)//修改路面宽
	{
	double xcdhp,yljhp,tljhp;
	char str[30];

	_stprintf(str,"%0.2lf",LMHP[0]*100.0);
	xcdhp = _wtof(str);
	_stprintf(str,"%0.2lf",LMHP[1]*100.0);
	yljhp = _wtof(str);
	_stprintf(str,"%0.2lf",LMHP[2]*100.0);
	tljhp = _wtof(str);
	LMHPDlg lmhpdlg(NULL,xcdhp,yljhp,tljhp);

	if(lmhpdlg.DoModal()==IDOK)
	{
	LMHP[0] = lmhpdlg.m_xcdhp*0.01;
	LMHP[1] = lmhpdlg.m_yljhp*0.01;
	LMHP[2] = lmhpdlg.m_tljhp*0.01;

	RHF[3] = RHF[1] + (RHF[2]-RHF[0])*LMHP[0];
	RHF[5] = RHF[3] + (RHF[4]-RHF[2])*LMHP[1];
	RHF[7] = RHF[5] + (RHF[6]-RHF[4])*LMHP[2];
	return 1;
	}
	}
	else if(mode==3)//修改路面标高
	{
	double fgdh,xcdh,yljh,tljh;

	fgdh = RHF[1];
	xcdh = RHF[3];
	yljh = RHF[5];
	tljh = RHF[7];

	LMBG lmbg(NULL,fgdh,xcdh,yljh,tljh);

	if(lmbg.DoModal()==IDOK)
	{
	RHF[1] = lmbg.m_fgdh;
	RHF[3] = lmbg.m_xcdh;
	RHF[5] = lmbg.m_yljh;
	RHF[7] = lmbg.m_tljh;
	return 1;
	}
	}*/

	return 0;
}

int DesignLine_ROAD::Set_BP(int mode)
{

	/*
	assertWriteEnabled();   
	CHdmBPdlg bpxsdlg;
	if(bpxsdlg.DoModal()==IDOK)
	{
	if(m_FillORCut==0)//填0-填方 1=挖方
	{
	for(i=0; i<m_nLTBPJS; i++)
	{
	m_TM[i] = bpxsdlg.bpxs[i];
	m_TH[i] = bpxsdlg.bph[i];
	m_TPTW[i] = bpxsdlg.ptw[i];
	m_TPTI[i] = bpxsdlg.pti[i];
	ads_printf(L"%lf %lf %lf %lf\n",m_TM[i],m_TH[i],m_TPTW[i],m_TPTI[i]);
	}
	}
	else
	{
	for(i=0; i<m_nLZBPJS; i++)
	{
	m_WM[i] = bpxsdlg.bpxs[i];
	m_PH[i] = bpxsdlg.bph[i];
	m_ZPTW[i] = bpxsdlg.ptw[i];
	m_ZPTI[i] = bpxsdlg.pti[i];
	ads_printf(L"%lf %lf %lf %lf\n",m_WM[i],m_PH[i],m_ZPTW[i],m_ZPTI[i]);
	}
	}
	return 1;
	}*/

	return 0;
}

void DesignLine_ROAD::SetBPStr(double FillORCut, int ptnum, AcGePoint3d DesPoints[],double bp[30],AcGePoint3dArray &SlopPt,int ZorY)
{
	int i,sno;
	CalBP();
	if(SlopPt.length()>0)
	{
		for( i=0; i<SlopPt.length(); i++)
			SlopPt.removeAt(i);
	}

	SlopPt.setLogicalLength(0);
	if(FillORCut < 0.1)//填方
	{
		for(i=0; i<m_nLTBPJS; i++)
			bp[i] = m_TM[i];

		if(FillORCut>-0.99 && ptnum>5)  
		{
			sno = 4;
			while(sno+1 < ptnum) 
			{
				SlopPt.append(DesPoints[sno]);
				SlopPt.append(DesPoints[sno+1]);
				sno=sno+2;
			}
		}
	}
	else
	{
		for(i=0; i<m_nLZBPJS; i++)
			bp[i] = m_WM[i];


		if(fabs(FillORCut-1.0)<0.001 &&  ptnum>9) 
		{

			sno = 8;
			while(sno+1 < ptnum) 
			{
				SlopPt.append(DesPoints[sno]);
				SlopPt.append(DesPoints[sno+1]);
				sno=sno+2;
			}
		}
	}

}

void DesignLine_ROAD::CalBP()
{
	int i;

	if(m_FillORCut<0.01 && m_DesignPointNum>4)
	{
		m_nLTBPJS = 0;
		i=5;
		while(i<m_DesignPointNum)
		{
			if(fabs(m_DesignPoints[i][Y] - m_DesignPoints[i-1][Y])>0.001)
				m_TM[m_nLTBPJS] = fabs(m_DesignPoints[i][X] - m_DesignPoints[i-1][X]) / fabs(m_DesignPoints[i-1][Y] - m_DesignPoints[i][Y]);
			else
				m_TM[m_nLTBPJS] = 1.0E6;

			m_TH[m_nLTBPJS] = fabs(m_DesignPoints[i-1][Y] - m_DesignPoints[i][Y])/Sfactor;			  
			if(i<m_DesignPointNum-1)
			{
				m_TPTW[m_nLTBPJS] = fabs(m_DesignPoints[i][X] - m_DesignPoints[i+1][X])/Sfactor;
				if (m_TPTW[m_nLTBPJS]>0.001)
					m_TPTI[m_nLTBPJS] = 100.0 * fabs(m_DesignPoints[i][Y] - m_DesignPoints[i+1][Y]) / fabs(m_DesignPoints[i][X] - m_DesignPoints[i+1][X]);
				else
					m_TPTI[m_nLTBPJS] = 0.0;

			}
			else
			{
				m_TPTW[m_nLTBPJS] = 0.0;
				m_TH[m_nLTBPJS] = 100.0;
				m_TPTI[m_nLTBPJS] = 0.0;
			}
			//			m_TPTI[m_nLTBPJS] = 0.0;
			i=i+2;
			m_nLTBPJS++;
		}
	}
	else if(m_FillORCut>0.01 && m_DesignPointNum>8)
	{
		m_nLZBPJS = 0;
		i=9;

		while(i<m_DesignPointNum)
		{
			if(fabs(m_DesignPoints[i][Y] - m_DesignPoints[i-1][Y])>0.001)
				m_WM[m_nLZBPJS] = fabs(m_DesignPoints[i][X] - m_DesignPoints[i-1][X]) / fabs(m_DesignPoints[i-1][Y] - m_DesignPoints[i][Y]);
			else
				m_WM[m_nLZBPJS] = 1.0E6;

			if(i<m_DesignPointNum-1)
			{
				m_ZPTW[m_nLZBPJS] = fabs(m_DesignPoints[i-1][X] - m_DesignPoints[i-2][X])/Sfactor;
				m_PH[m_nLZBPJS] = fabs(m_DesignPoints[i-1][Y] - m_DesignPoints[i][Y])/Sfactor;
				if (m_ZPTW[m_nLZBPJS]>0.001)
					m_ZPTI[m_nLZBPJS] = 100.0 * fabs(m_DesignPoints[i-1][Y] - m_DesignPoints[i-2][Y]) / fabs(m_DesignPoints[i-1][X] - m_DesignPoints[i-2][X]);
				else
					m_ZPTI[m_nLZBPJS] = 0.0;
			}
			else
			{
				m_ZPTW[m_nLZBPJS] = fabs(m_DesignPoints[i-1][X] - m_DesignPoints[i-2][X])/Sfactor;
				m_PH[m_nLZBPJS] = 100.0;
				m_ZPTI[m_nLZBPJS] = 0.0;
			}
			//	ads_printf(L"zptw=%lf\n",m_ZPTW[m_nLZBPJS]);
			//			m_ZPTI[m_nLZBPJS] = 0.0;
			i=i+2;
			m_nLZBPJS++;
		}
	}

}

void DesignLine_ROAD::BZDesh(AcGiWorldDraw* pWd,AcGePoint3d notept,double desh)
{
	AcGePoint3d pt[4];
	AcGePoint3d origin;
	ACHAR Str[20];

	pt[0] = notept;
	pt[1].x = pt[0].x - 0.1*Sfactor;
	pt[1].y = pt[0].y + 0.15*Sfactor;
	pt[2].x = pt[0].x + 0.1*Sfactor;
	pt[2].y = pt[0].y + 0.15*Sfactor;
	pt[3] = pt[0];
	pWd->subEntityTraits().setColor(6);
	pWd->subEntityTraits().setFillType(kAcGiFillAlways);
	pWd->geometry().polygon(4,pt);

	origin.x = pt[0].x;
	origin.y = pt[0].y + 0.25*Sfactor;
	origin.z = pt[0].z;
	AcGeVector3d direction(1.0,0.0,0.0);
	AcGeVector3d normal(0.0,0.0,1.0);
	AcGiTextStyle pTextStyle;
	//	if(m_ZorY==1)
	//	{
	origin.x = pt[0].x;// - 1*Sfactor;
	origin.y = pt[0].y + 0.25*Sfactor;
	origin.z = pt[0].z;
	//	}

	pTextStyle.setTextSize(0.5);//设置字宽

	acdbRToS(desh,2,3,Str);
	//设置文本（路肩设计高）
	pWd->subEntityTraits().setColor(2);
	pWd->geometry().text(origin,normal,direction,Str,_tcslen(Str),Adesk::kTrue,pTextStyle);//字高、字宽、旋转角度
}

Acad::ErrorStatus DesignLine_ROAD::dwgInFields(AcDbDwgFiler * filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgInFields(filer))!=Acad::eOk)
		return es;
	filer->readItem(&m_DesignPointNum);

	if(m_DesignPoints) {delete []m_DesignPoints;m_DesignPoints=NULL;}
	if(m_DesignPointNum>0)
		m_DesignPoints=new AcGePoint3d[m_DesignPointNum];
	int i;
	for( i=0;i<m_DesignPointNum;i++)
		filer->readPoint3d(&m_DesignPoints[i]);
	for(i=0;i<10;i++)
		filer->readItem(&RHF[i]);

	filer->readItem(&m_nLTBPJS);
	filer->readItem(&m_nLZBPJS);

	for(i=0; i<30; i++)
	{
		filer->readItem(&m_TM[i]);
		filer->readItem(&m_TH[i]);
		filer->readItem(&m_TPTW[i]);
		filer->readItem(&m_TPTI[i]);
		filer->readItem(&m_WM[i]);
		filer->readItem(&m_PH[i]);
		filer->readItem(&m_ZPTW[i]);
		filer->readItem(&m_ZPTI[i]);
	}

	filer->readItem(&JqPtNum);
	for(i=0;i<JqPtNum;i++)
		filer->readPoint3d(&JqPt[i]);

	//	filer->readItem(&xlxz);
	//	filer->readItem(&r_bh);
	filer->readItem(&hdmno);
	filer->readItem(&cml);
	filer->readItem(&m_ZorY);
	filer->readItem(&m_FillORCut);
	filer->readItem(&LnewYd);
	filer->readItem(&RnewYd);
	filer->readItem(&m_ZJ);
	filer->readItem(&m_XW);
	filer->readItem(&m_WW[0]);
	filer->readItem(&m_WW[1]);


	//void *tpointer;
	//filer->readAddress(&tpointer);
	//l_earthline=(EarthLine_ROAD*)tpointer;
	//filer->readAddress(&tpointer);
	//r_earthline=(EarthLine_ROAD*)tpointer;
	//filer->readAddress(&tpointer);
	//LTGou=(LTGOU_ROAD *)tpointer;
	//filer->readAddress(&tpointer);
	//JSGou=(JSGOU_ROAD *)tpointer;
	//filer->readAddress(&tpointer);
	//Note=(HdmText_ROAD *)tpointer;
	l_earthline=my_l_earthline;
	r_earthline=my_r_earthline;
	LTGou=my_LTGOU;
	JSGou=my_JSGOU;
	Note=my_Note;

	ACHAR *ch;
	filer->readItem(&ch);
	_tcscpy(RoadName,ch);
	filer->readItem(&DesignLineColor);
	filer->readItem(&lmhdcolor);

	for(i=0;i<4;i++)
		filer->readItem(&m_ArcBpR[i]);
	filer->readItem(&m_sfactor);
	return filer->filerStatus();

}

Acad::ErrorStatus DesignLine_ROAD::dwgOutFields(AcDbDwgFiler * filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if((es=AcDbEntity::dwgOutFields(filer))!=Acad::eOk)
		return es;
	filer->writeItem(m_DesignPointNum);
	int i;
	for( i=0;i<m_DesignPointNum;i++)
		filer->writePoint3d(m_DesignPoints[i]);
	for(i=0;i<10;i++)
		filer->writeItem(RHF[i]);
	filer->writeItem(m_nLTBPJS);
	filer->writeItem(m_nLZBPJS);
	for(i=0; i<30; i++)
	{
		filer->writeItem(m_TM[i]);
		filer->writeItem(m_TH[i]);
		filer->writeItem(m_TPTW[i]);
		filer->writeItem(m_TPTI[i]);
		filer->writeItem(m_WM[i]);
		filer->writeItem(m_PH[i]);
		filer->writeItem(m_ZPTW[i]);
		filer->writeItem(m_ZPTI[i]);
	}
	filer->writeItem(JqPtNum);
	for(i=0;i<JqPtNum;i++)
		filer->writePoint3d(JqPt[i]);

	//	filer->writeItem(xlxz);
	//	filer->writeItem(r_bh);

	filer->writeItem(hdmno);
	filer->writeItem(cml);
	filer->writeItem(m_ZorY);
	filer->writeItem(m_FillORCut);
	filer->writeItem(LnewYd);
	filer->writeItem(RnewYd);
	filer->writeItem(m_ZJ);
	filer->writeItem(m_XW);
	filer->writeItem(m_WW[0]);
	filer->writeItem(m_WW[1]);

	//  	filer->writeAddress(l_earthline);
	//  	filer->writeAddress(r_earthline);
	//  	filer->writeAddress(LTGou);
	//  	filer->writeAddress(JSGou);
	//  	filer->writeAddress(Note);	
	my_l_earthline=l_earthline;
	my_r_earthline=r_earthline;
	my_LTGOU=LTGou;
	my_JSGOU=JSGou;
	my_Note=Note;


	const ACHAR *ch=NULL;
	ch=_tcsdup(RoadName);
	filer->writeItem(ch);
	filer->writeItem(DesignLineColor);
	filer->writeItem(lmhdcolor);

	for(i=0;i<4;i++)
		filer->writeItem(m_ArcBpR[i]);

	filer->writeItem(m_sfactor);

	return filer->filerStatus();
}

Acad::ErrorStatus  DesignLine_ROAD::subGetOsnapPoints(
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
	if(osnapMode!=AcDb::kOsModeEnd&&osnapMode!=AcDb::kOsModeNear
		&&osnapMode!=AcDb::kOsModeMid&&osnapMode!=AcDb::kOsModePerp
		&&osnapMode!=AcDb::kOsModeCen&&osnapMode!=AcDb::kOsModeIns) 
	{return es;}
	switch(osnapMode)
	{
	case AcDb::kOsModeEnd:
		{	
			for(int i=0;i<m_DesignPointNum;i++)
				snapPoints.append(m_DesignPoints[i]);
			break;
		}
	default:     return es;
	}
	return es;
}

//输入半径，角点坐标，返回弧形边坡点
bool DesignLine_ROAD::DesArcBp(double R,int JiaoDianNum,AcGePoint3d arcpt[31],char TorW)			 
{
	//判别边坡斜长是否超过5m
	bool IfSetArc1,IfSetArc2;
	double bpc;
	IfSetArc1 = false;

	R = R*m_sfactor;

	if(m_DesignPointNum>10)//至少两级边坡
		IfSetArc1 = true;
	else if(m_DesignPointNum>0)//一级边坡
	{
		//边坡高
		bpc = m_DesignPoints[m_DesignPointNum-1].distanceTo(m_DesignPoints[m_DesignPointNum-2]);
		if(bpc>=R)
			IfSetArc1 = true;
	}
	//地面横坡是否向上
	//	double dmhp=GetDmhp(DesPtList[(DesPtSum-1)*2],EarPtList[(EarPtSum-1)*2],EarPtList,EarPtSum);
	if(IfSetArc1)
	{
		//以坡顶点为圆心，R为半径，求圆与地面线交点
		double Xo,Yo,jd[2];
		Xo =  m_DesignPoints[JiaoDianNum].x;
		Yo =  m_DesignPoints[JiaoDianNum].y;//圆心
		//计算ARC圆心
		double fwj1,fwj2,zj,ArcR,ArcSpt[2],ArcCen[2],ArcEpt[2];
		if(m_DesignPoints[JiaoDianNum].distanceTo(m_DesignPoints[JiaoDianNum-1])>0.001)
			xyddaa(m_DesignPoints[JiaoDianNum-1].y,m_DesignPoints[JiaoDianNum-1].x,Yo,Xo,&fwj1);
		else
			if(m_ZorY>0)
				fwj1 = 0.5*pi;
			else
				fwj1 = 1.5*pi;

		if(m_DesignPoints[JiaoDianNum].distanceTo(m_DesignPoints[JiaoDianNum+1])>0.001)
			xyddaa(Yo,Xo,m_DesignPoints[JiaoDianNum+1].y,m_DesignPoints[JiaoDianNum+1].x,&fwj2);
		else
			return false;

		//zj = fabs(fwj2-fwj1);
		zj = fabs(ZXJ(fwj2,fwj1));
		ArcR = fabs(R*tan(0.5*zj));

		bpc = m_DesignPoints[JiaoDianNum].distanceTo(m_DesignPoints[JiaoDianNum-1]);
		if(bpc>0.001)
		{
			ArcSpt[0] = m_DesignPoints[JiaoDianNum].x -ArcR * (m_DesignPoints[JiaoDianNum].x-m_DesignPoints[JiaoDianNum-1].x)/bpc;
			ArcSpt[1] = m_DesignPoints[JiaoDianNum].y -ArcR * (m_DesignPoints[JiaoDianNum].y-m_DesignPoints[JiaoDianNum-1].y)/bpc;
		}
		else
		{
			ArcSpt[0] = m_DesignPoints[JiaoDianNum].x - m_ZorY*ArcR;
			ArcSpt[1] = m_DesignPoints[JiaoDianNum].y ;
		}

		bpc = m_DesignPoints[JiaoDianNum].distanceTo(m_DesignPoints[JiaoDianNum+1]);
		if(bpc>0.001)
		{
			ArcEpt[0] = m_DesignPoints[JiaoDianNum].x + ArcR * (m_DesignPoints[JiaoDianNum+1].x-m_DesignPoints[JiaoDianNum].x)/bpc;
			ArcEpt[1] = m_DesignPoints[JiaoDianNum].y + ArcR * (m_DesignPoints[JiaoDianNum+1].y-m_DesignPoints[JiaoDianNum].y)/bpc;
		}
		else
			return false;
		if(TorW=='T')
		{
			ArcCen[0] = ArcSpt[1] + R*cos(fwj1+m_ZorY*0.5*pi);
			ArcCen[1] = ArcSpt[0] + R*sin(fwj1+m_ZorY*0.5*pi);
		}
		else
		{
			ArcCen[0] = ArcSpt[1] + R*cos(fwj1-m_ZorY*0.5*pi);
			ArcCen[1] = ArcSpt[0] + R*sin(fwj1-m_ZorY*0.5*pi);
		}

		arcpt[0].x = ArcSpt[0];
		arcpt[0].y = ArcSpt[1];

		xyddaa(ArcCen[0],ArcCen[1],ArcSpt[1],ArcSpt[0],&fwj1);
		xyddaa(ArcCen[0],ArcCen[1],ArcEpt[1],ArcEpt[0],&fwj2);

		zj = ZXJ(fwj2,fwj1);

		//xyddaa(ArcCen[1],ArcCen[0],ArcSpt[1],ArcSpt[0],&fwj1);
		int i;
		for(i=0; i<30; i++)
		{
			arcpt[i].x = ArcCen[1] + R*sin(fwj1+i*zj/30.0);
			arcpt[i].y = ArcCen[0] + R*cos(fwj1+i*zj/30.0);

		}
		arcpt[i].x = ArcEpt[0];
		arcpt[i].y = ArcEpt[1];

	}
	//计算ARC与地面线段交点
	return IfSetArc1;
}

//无水沟，坡度线与地面线相交设计弧形边坡
int DesignLine_ROAD::DesTopArcBp(double R,AcGePoint3d arcpt[31],char TorW)			 
{
	//判别边坡斜长是否超过5m
	bool IfSetArc1,IfSetArc2;
	double bpc;
	IfSetArc1 = false;

	R = R*m_sfactor;

	if(m_DesignPointNum>10)//至少两级边坡
		IfSetArc1 = true;
	else//一级边坡
	{
		//边坡高
		bpc = m_DesignPoints[m_DesignPointNum-1].distanceTo(m_DesignPoints[m_DesignPointNum-2]);
		if(bpc>=R)
			IfSetArc1 = true;
	}
	//地面横坡是否向上
	//	double dmhp=GetDmhp(DesPtList[(DesPtSum-1)*2],EarPtList[(EarPtSum-1)*2],EarPtList,EarPtSum);

	if(IfSetArc1)
	{
		//以坡顶点为圆心，R为半径，求圆与地面线交点
		double Xo,Yo,jd[2];
		Xo =  m_DesignPoints[(m_DesignPointNum-1)].x;
		Yo =  m_DesignPoints[(m_DesignPointNum-1)].y;//圆心
		bpc = m_DesignPoints[m_DesignPointNum-1].distanceTo(m_DesignPoints[m_DesignPointNum-2]);
		if(bpc<0.001)
			return false;
		int inf = 0;
		inf = CalDmxJD(Xo,Yo,R,jd);

		//ARC圆心

		if(inf)//找到交点
		{
			//计算ARC圆心
			double fwj1,fwj2,zj,ArcR,ArcSpt[2],ArcCen[2],ArcEpt[2];
			xyddaa( m_DesignPoints[(m_DesignPointNum-2)].y, m_DesignPoints[(m_DesignPointNum-2)].x,Yo,Xo,&fwj1);
			xyddaa(Yo,Xo,jd[1],jd[0],&fwj2);
			//zj = fabs(fwj2-fwj1);
			zj = fabs(ZXJ(fwj2,fwj1));
			ArcR = fabs(R/tan(0.5*zj));

			ArcSpt[1] = m_DesignPoints[(m_DesignPointNum-1)].x - R * ( m_DesignPoints[(m_DesignPointNum-1)].x-m_DesignPoints[(m_DesignPointNum-2)].x)/bpc;
			ArcSpt[0] = m_DesignPoints[(m_DesignPointNum-1)].y - R * ( m_DesignPoints[(m_DesignPointNum-1)].y-m_DesignPoints[(m_DesignPointNum-2)].y)/bpc;
			ArcEpt[0] = jd[1];
			ArcEpt[1] = jd[0];

			if(TorW=='W')
			{
				ArcCen[0] = ArcSpt[0] + ArcR*cos(fwj1+m_ZorY*0.5*pi);
				ArcCen[1] = ArcSpt[1] + ArcR*sin(fwj1+m_ZorY*0.5*pi);
			}
			else
			{
				ArcCen[0] = ArcSpt[0] + ArcR*cos(fwj1-m_ZorY*0.5*pi);
				ArcCen[1] = ArcSpt[1] + ArcR*sin(fwj1-m_ZorY*0.5*pi);
			}

			arcpt[0].x = ArcSpt[1];
			arcpt[0].y = ArcSpt[0];
			//	zj =  fabs(pi - zj);

			xyddaa(ArcCen[0],ArcCen[1],ArcSpt[0],ArcSpt[1],&fwj1);
			xyddaa(ArcCen[0],ArcCen[1],ArcEpt[0],ArcEpt[1],&fwj2);

			zj = ZXJ(fwj2,fwj1);


			//	xyddaa(ArcCen[0],ArcCen[1],ArcSpt[0],ArcSpt[1],&fwj1);
			int i;
			for( i=0; i<30; i++)
			{
				arcpt[i].x = ArcCen[1] + ArcR*sin(fwj1+i*zj/30.0);
				arcpt[i].y = ArcCen[0] + ArcR*cos(fwj1+i*zj/30.0);
			}
			arcpt[i].x = ArcEpt[1];
			arcpt[i].y = ArcEpt[0];

		}
		else
			return false;
	}
	//计算ARC与地面线段交点
	return 	IfSetArc1;
}


int DesignLine_ROAD::CalDmxJD(double Xo,double Yo,double R,double jd[2])
{
	double x1,y1,x2,y2;
	double Spt[2],Ept[2];
	double xmin,ymin,xmax,ymax;

	EarthLine_ROAD *earthline;
	if(m_ZorY<0)
		earthline = l_earthline;
	else
		earthline = r_earthline;

	AcGeCircArc3d    circle(AcGePoint3d(Xo,Yo,0.0),AcGeVector3d(0,0,1),R);
	int num;
	AcGePoint3d p1,p2;

	AcGePoint3dArray ptarr;
	for(int i=0; i<earthline->m_EarthPointNum-1; i++)
	{
		if(earthline->m_EarthPoints[i].distanceTo(earthline->m_EarthPoints[i+1])<0.001)
			continue;

		AcGeLine3d line(earthline->m_EarthPoints[i],earthline->m_EarthPoints[i+1]);

		Spt[0] = earthline->m_EarthPoints[i].x,Spt[1] = earthline->m_EarthPoints[i].y;
		Ept[0] = earthline->m_EarthPoints[i+1].x,Ept[1] = earthline->m_EarthPoints[i+1].y;

		xmin = Spt[0] < Ept[0] ? Spt[0] : Ept[0];
		ymin = Spt[1] < Ept[1] ? Spt[1] : Ept[1];
		xmax = Spt[0] > Ept[0] ? Spt[0] : Ept[0];
		ymax = Spt[1] > Ept[1] ? Spt[1] : Ept[1];
		circle.intersectWith(line,num,p1,p2);
		if(num>0)
		{
			if(p1.x>xmin-0.001 && p1.x<xmax+0.001 && p1.y>ymin-0.001 && p1.y<ymax+0.001)
			{
				if(m_ZorY>0 && p1.x>Xo || m_ZorY<0 && p1.x<Xo)
				{
					jd[0]=p1.x,jd[1]=p1.y;
					return 1;
				}
			}

			if(num>1)
			{
				if(p2.x>xmin-0.001 && p2.x<xmax+0.001 && p2.y>ymin-0.001 && p2.y<ymax+0.001)
				{
					if(m_ZorY>0 && p2.x>Xo || m_ZorY<0 && p2.x<Xo)
					{
						jd[0]=p2.x,jd[1]=p2.y;
						return 1;
					}
				}
			}

		}
	}

	return 0;
}	


//输入半径，角点坐标，返回弧形边坡点
bool DesignLine_ROAD::DesArcTFBpWithLTgou(double R,int JiaoDianNum,AcGePoint3d arcpt[31],char TorW)			 
{
	//判别边坡斜长是否超过5m
	bool IfSetArc1,IfSetArc2;
	double bpc;
	IfSetArc1 = false;

	R = R*m_sfactor;

	if(m_DesignPointNum>10)//至少两级边坡
		IfSetArc1 = true;
	else if(m_DesignPointNum>0)//一级边坡
	{
		//边坡高
		bpc = m_DesignPoints[m_DesignPointNum-1].distanceTo(m_DesignPoints[m_DesignPointNum-2]);
		if(bpc>=R)
			IfSetArc1 = true;
	}
	//地面横坡是否向上
	//	double dmhp=GetDmhp(DesPtList[(DesPtSum-1)*2],EarPtList[(EarPtSum-1)*2],EarPtList,EarPtSum);
	if(IfSetArc1)
	{
		//以坡顶点为圆心，R为半径，求圆与路堤沟线交点
		double Xo,Yo,jd[2];
		Xo =  m_DesignPoints[JiaoDianNum].x;
		Yo =  m_DesignPoints[JiaoDianNum].y;//圆心
		//计算ARC圆心
		double fwj1,fwj2,zj,ArcR,ArcSpt[2],ArcCen[2],ArcEpt[2];
		if(m_DesignPoints[JiaoDianNum].distanceTo(m_DesignPoints[JiaoDianNum-1])>0.001)
			xyddaa(m_DesignPoints[JiaoDianNum-1].y,m_DesignPoints[JiaoDianNum-1].x,Yo,Xo,&fwj1);
		else
			if(m_ZorY>0)
				fwj1 = 0.5*pi;
			else
				fwj1 = 1.5*pi;

		AcGePoint3d 	ltgoupt;
		if(LTGou->m_LTGouPointNum<2)
			return false;
		ltgoupt = LTGou->m_LTGouPoints[1];
		if(m_DesignPoints[JiaoDianNum].distanceTo(ltgoupt)>0.001)
			xyddaa(Yo,Xo,ltgoupt.y,ltgoupt.x,&fwj2);
		else
			return false;

		//zj = fabs(fwj2-fwj1);
		zj = fabs(ZXJ(fwj2,fwj1));
		ArcR = fabs(R*tan(0.5*zj));

		bpc = m_DesignPoints[JiaoDianNum].distanceTo(m_DesignPoints[JiaoDianNum-1]);
		if(bpc>0.001)
		{
			ArcSpt[0] = m_DesignPoints[JiaoDianNum].x -ArcR * (m_DesignPoints[JiaoDianNum].x-m_DesignPoints[JiaoDianNum-1].x)/bpc;
			ArcSpt[1] = m_DesignPoints[JiaoDianNum].y -ArcR * (m_DesignPoints[JiaoDianNum].y-m_DesignPoints[JiaoDianNum-1].y)/bpc;
		}
		else
		{
			ArcSpt[0] = m_DesignPoints[JiaoDianNum].x - m_ZorY*ArcR;
			ArcSpt[1] = m_DesignPoints[JiaoDianNum].y ;
		}

		bpc = m_DesignPoints[JiaoDianNum].distanceTo(ltgoupt);
		if(bpc>0.001)
		{
			ArcEpt[0] = m_DesignPoints[JiaoDianNum].x + ArcR * (ltgoupt.x-m_DesignPoints[JiaoDianNum].x)/bpc;
			ArcEpt[1] = m_DesignPoints[JiaoDianNum].y + ArcR * (ltgoupt.y-m_DesignPoints[JiaoDianNum].y)/bpc;
		}
		else
			return false;
		if(TorW=='T')
		{
			ArcCen[0] = ArcSpt[1] + R*cos(fwj1+m_ZorY*0.5*pi);
			ArcCen[1] = ArcSpt[0] + R*sin(fwj1+m_ZorY*0.5*pi);
		}
		else
		{
			ArcCen[0] = ArcSpt[1] + R*cos(fwj1-m_ZorY*0.5*pi);
			ArcCen[1] = ArcSpt[0] + R*sin(fwj1-m_ZorY*0.5*pi);
		}

		arcpt[0].x = ArcSpt[0];
		arcpt[0].y = ArcSpt[1];

		xyddaa(ArcCen[0],ArcCen[1],ArcSpt[1],ArcSpt[0],&fwj1);
		xyddaa(ArcCen[0],ArcCen[1],ArcEpt[1],ArcEpt[0],&fwj2);

		zj = ZXJ(fwj2,fwj1);

		//xyddaa(ArcCen[1],ArcCen[0],ArcSpt[1],ArcSpt[0],&fwj1);
		int i;
		for(i=0; i<30; i++)
		{
			arcpt[i].x = ArcCen[1] + R*sin(fwj1+i*zj/30.0);
			arcpt[i].y = ArcCen[0] + R*cos(fwj1+i*zj/30.0);

		}
		arcpt[i].x = ArcEpt[0];
		arcpt[i].y = ArcEpt[1];

	}
	//计算ARC与地面线段交点
	return IfSetArc1;
}


//////////////////////////////////////////////////////////////////////////
void DesignLine_ROAD::CGhasJQ(int zory,AcGePoint3d m_DesignPoints[],double CG_JQHD,double CG_GDR,int &JqPtSum,AcGePoint3d JqPtArray[])
{
	// HdmDes.cpp: implementation of the HdmDes class.
	AcGePoint3d TrueGouPtArray[14];
	int TrueGouPtSum;
	if (CG_GDR<0.001)
	{
		for (int Index=0; Index<4; Index++)
		{
			TrueGouPtArray[Index] = m_DesignPoints[Index+4];
		}
		TrueGouPtSum = 4;
	}
	else//插入沟底十个弧形点
	{
		for (int Index=0; Index<14; Index++)
		{
			TrueGouPtArray[Index] = m_DesignPoints[Index+4];
		}
		TrueGouPtSum = 14;
	}

	cal_JQpt(zory,-1,TrueGouPtSum,TrueGouPtArray,CG_JQHD,JqPtSum,JqPtArray);
}
//////////////////////////////////////////////////////////////////////////

void DesignLine_ROAD::cal_JQpt(int zory,int LQGorLDG,int GouPtSum,AcGePoint3d GouPtArray[],double JQHD,int &JqPtSum,AcGePoint3d JqPtArray[])
{
	// HdmDes.cpp: implementation of the HdmDes class.
	JqPtSum = 0;
	if (fabs(JQHD)<0.001)
		return;
	double SP1,SP2,GS;
	double TopJQPt1[2],TopJQPt2[2];
	GS = (GouPtArray[0].y - GouPtArray[1].y) / Sfactor - JQHD;

	if(GouPtSum==4)
	{
		if (fabs(GouPtArray[1].y - GouPtArray[0].y)<0.001)
			SP1 = 0.0;
		else
			SP1 = fabs((GouPtArray[1].x - GouPtArray[0].x) / (GouPtArray[1].y - GouPtArray[0].y));
		if (fabs(GouPtArray[3].y - GouPtArray[2].y)<0.001)
			SP2 = 0.0;
		else
			SP2 = fabs((GouPtArray[3].x - GouPtArray[2].x) / (GouPtArray[3].y - GouPtArray[2].y));

		//计算沟顶的两个浆砌点
		TopJQPt1[0] = GouPtArray[3].x - zory*LQGorLDG*JQHD*sqrt(1+SP2*SP2)*Sfactor;
		TopJQPt1[1] = GouPtArray[3].y;
		TopJQPt2[0] = GouPtArray[0].x + zory*LQGorLDG*JQHD*sqrt(1+SP1*SP1)*Sfactor;
		TopJQPt2[1] = GouPtArray[0].y;

		//将水沟设计点偏移一个浆砌厚度
		AcGePoint2d tmpPt;
		AcDbPolyline* pGouPline = new AcDbPolyline();//水沟点多段线
		AcDbPolyline* pJQPline = new AcDbPolyline();//浆砌点多段线
		int Index;
		for (Index=0; Index<4; Index++)
		{
			tmpPt.x = GouPtArray[Index].x;
			tmpPt.y = GouPtArray[Index].y;
			pGouPline->addVertexAt(Index,tmpPt);
		}

		//20190926	修改当无法获取曲线时确保程序不崩溃
		for (Index = 0; Index < 4; Index++)
		{
			JqPtArray[Index] = GouPtArray[Index];
		}

		AcDbVoidPtrArray offsetCurves;
		int ErrorS = pGouPline->getOffsetCurves(-1.0*zory*LQGorLDG*JQHD*Sfactor,offsetCurves);
		if (offsetCurves == NULL)
		{
			return;
		}
		else
		{
			pJQPline = (AcDbPolyline*)(offsetCurves[0]);

			for (Index = 4; Index < 8; Index++)
			{
				pJQPline->getPointAt(7 - Index, JqPtArray[Index]);
			}
			JqPtArray[4].x = TopJQPt1[0];
			JqPtArray[4].y = TopJQPt1[1];
			JqPtArray[7].x = TopJQPt2[0];
			JqPtArray[7].y = TopJQPt2[1];
			JqPtArray[8] = JqPtArray[0];
			JqPtSum = 9;
		}

	}

	if(GouPtSum==14)
	{
		if (fabs(GouPtArray[1].y - GouPtArray[0].y)<0.001)
			SP1 = 0.0;
		else
			SP1 = fabs((GouPtArray[1].x - GouPtArray[0].x) / (GouPtArray[1].y - GouPtArray[0].y));
		if (fabs(GouPtArray[13].y - GouPtArray[12].y)<0.001)
			SP2 = 0.0;
		else
			SP2 = fabs((GouPtArray[13].x - GouPtArray[12].x) / (GouPtArray[13].y - GouPtArray[12].y));

		//计算沟顶的两个浆砌点
		TopJQPt1[0] = GouPtArray[13].x - zory*LQGorLDG*JQHD*sqrt(1+SP2*SP2)*Sfactor;
		TopJQPt1[1] = GouPtArray[13].y;
		TopJQPt2[0] = GouPtArray[0].x + zory*LQGorLDG*JQHD*sqrt(1+SP1*SP1)*Sfactor;
		TopJQPt2[1] = GouPtArray[0].y;

		//将水沟设计点偏移一个浆砌厚度
		AcGePoint2d tmpPt;
		AcDbPolyline* pGouPline = new AcDbPolyline();//水沟点多段线
		AcDbPolyline* pJQPline = new AcDbPolyline();//浆砌点多段线
		int Index;
		for (Index=0; Index<14; Index++)
		{
			tmpPt.x = GouPtArray[Index].x;
			tmpPt.y = GouPtArray[Index].y;
			pGouPline->addVertexAt(Index,tmpPt);
		}

		AcDbVoidPtrArray offsetCurves;
		pGouPline->getOffsetCurves(-1.0*zory*LQGorLDG*JQHD*Sfactor,offsetCurves);
		pJQPline = (AcDbPolyline*)(offsetCurves[0]);
		int NumVectors = pJQPline->numVerts();

		for (Index=0; Index<14; Index++)
		{
			JqPtArray[Index] = GouPtArray[Index];
		}

		for (Index=14; Index<14+NumVectors; Index++)
		{
			pJQPline->getPointAt(13+NumVectors-Index,JqPtArray[Index]);
		}
		JqPtArray[14].x = TopJQPt1[0];
		JqPtArray[14].y = TopJQPt1[1];
		JqPtArray[27].x = TopJQPt2[0];
		JqPtArray[27].y = TopJQPt2[1];
		for (Index=13+NumVectors; Index<27; Index++)
		{
			JqPtArray[Index] = JqPtArray[27];
		}
		JqPtArray[28] = JqPtArray[0];
		JqPtSum=29;
	}

}

//检测侧沟是否存在
void DesignLine_ROAD::CheckLateralGutterExist()
{
	bool IsLateralGutterExist = false;
	double FirstX = m_DesignPoints[4].x, FirstY = m_DesignPoints[4].y, SecondX = m_DesignPoints[5].x, SecondY = m_DesignPoints[5].y;
	if (fabs(FirstX - SecondX) < 0.01 && fabs(FirstY - SecondY) < 0.01)
		IsLateralGutterExist = false;
	else
		IsLateralGutterExist = true;
}

//设置侧沟参数
void DesignLine_ROAD::SetLateralGutterPara(double ParaGK, double ParaGS, double ParaGP1, double ParaGP2, double ParaCG_JQHD, double ParaCG_GDR)
{
	GK = ParaGK;
	GS = ParaGS;
	GP1 = ParaGP1;
	GP2 = ParaGP2;
	CG_JQHD = ParaCG_JQHD;
	CG_GDR = ParaCG_GDR;
}

void DesignLine_ROAD::SetLDGPara(double T2W, double T1W, double T2H, double GH, double GW, double HH, double SP1, double SP2, double SP3, double JQHD, double GDR)
{
	m_LDG_T2W = T2W;
	m_LDG_T1W = T1W;
	m_LDG_T2H = T2H;
	m_LDG_GH = GH;
	m_LDG_GW = GW;
	m_LDG_HH = HH;
	m_LDG_SP1 = SP1;
	m_LDG_SP2 = SP2;
	m_LDG_SP3 = SP3;
	m_LDG_JQHD = JQHD;
	m_LDG_GDR = GDR;
}

void DesignLine_ROAD::SetTGPara(double BJ, double GW, double GS, double SP1, double SP2, double JQHD, double GDR)
{
	m_TG_BJ = BJ;
	m_TG_GW = GW;
	m_TG_GS = GS;
	m_TG_SP1 = SP1;
	m_TG_SP2 = SP2;
	m_TG_JQHD = JQHD;
	m_TG_GDR = GDR;
}

void DesignLine_ROAD::NoteRoadCrossSlope(double NoteTextH)
{
	//计算横坡
	double SlopeRate = fabs((RHF[1] - RHF[7]) / (RHF[0] - RHF[6]));
	CString tempCStr;
	tempCStr.Format(L"%.2lf%%", SlopeRate * 100);
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"横断面图-横坡");

	//绘制线条
	AcGePoint3d StartPt(0.0, 0.0, 0.0) , EndPt(0.0, 0.0, 0.0), MidPt(0.0, 0.0, 0.0);
	AcGeVector3d LineVec = m_DesignPoints[4] - m_DesignPoints[0];
	LineVec.normalize();	//归一化
	MidPt.set((m_DesignPoints[0].x + m_DesignPoints[4].x) / 2.0, (m_DesignPoints[0].y + m_DesignPoints[4].y) / 2.0 + 0.2 * Sfactor, 0.0);
	StartPt = MidPt - 1.6 / 2.0 * Sfactor * LineVec;
	EndPt = MidPt + 1.6 / 2.0 * Sfactor * LineVec;
	makeline(StartPt, EndPt, GREEN, 0.0, LayerName);

	//绘制箭头
	AcGeVector3d ArrowVec;
	if (RHF[1] > RHF[7])
	{
		ArrowVec = m_DesignPoints[4] - m_DesignPoints[0];
		StartPt = EndPt;
	}
	else
	{
		ArrowVec = m_DesignPoints[0] - m_DesignPoints[4];
		StartPt = StartPt;
	}
		
	if (m_ZorY < 0)
	{
		if(RHF[1] > RHF[7])
			ArrowVec.rotateBy(PI / 6 + PI, AcGeVector3d::kZAxis);
		else
			ArrowVec.rotateBy(-1 * PI / 6 - PI, AcGeVector3d::kZAxis);
	}
	else
	{
		if (RHF[1] > RHF[7])
			ArrowVec.rotateBy(-1 * PI / 6 - PI, AcGeVector3d::kZAxis);
		else
			ArrowVec.rotateBy(PI / 6 + PI, AcGeVector3d::kZAxis);
	}
		

	ArrowVec.normalize();
	EndPt = StartPt + ArrowVec * 0.3 * Sfactor;
	makeline(StartPt, EndPt, GREEN, 0.0, LayerName);

	//标注文字
	AcGeVector2d LineVec2D(LineVec.x, LineVec.y);
	MidPt.y += 0.2 * Sfactor;
	double Angle = 0;
	if (m_ZorY > 0)
		Angle = LineVec2D.angle() - pi;
	else
		Angle = LineVec2D.angle();
	ACHAR Str[256];
	_tcscpy(Str, tempCStr);
	maketext(MidPt, Str, 0, NoteTextH/*0.4 * Sfactor*/, GREEN, 2, L"宋体", LayerName);
}
//////////////////////////////////////////////////////////////////////////













