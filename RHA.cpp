//-----------------------------------------------------------------------------
//----- RHA.cpp : Implementation of RHA
//-----------------------------------------------------------------------------
#pragma once
#include "StdAfx.h"
#include "RHA.h"
#include "CreatPM.h"
#include "XLDataBase.h"
#include "LISTERR.h"
#include "PMSJ.h"
#include "triDTM.h"
#include "GetAllVertData.h"
#include "JDMoveTypeDlg.h"
#include "ShejiBiaoZhun.h"

#include "string"
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;

LISTERR *ERRdlg=NULL;

int g_JDMoveType=0;
extern vector<CDocManage> g_DocManageAry; 

extern CString Cworkdir,Pro;
extern triDTM ptriDTM;//DTM

extern ACHAR SysPath[256];

extern GuiFanCanShu mJYGuiFanCS;

extern GuiFanCanShu mGJGuiFanCS;


//-----------------------------------------------------------------------------
Adesk::UInt32 RHA::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	RHA, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, RHA,
PINGMIANAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)




//-----------------------------------------------------------------------------
RHA::RHA () : AcDbEntity () {

	IsMoveDone=false;
}

RHA::~RHA () {

	//AcDbObjectId id1, id2;
	//id1=id();

	////在全局数组中删除
	//if(XLShuXing == JYJX || XLShuXing == JY2X  
	//	|| XLShuXing == SJJX || XLShuXing == SJ2X)
	//{
	//	Int16 xlnum=g_DocManageAry[m_iDoc].XLAry.size();//818 在关闭文档时，g_DocManageAry为空 m_iDoc=0 会挂，不应该在内部清除外部变量
	//	for (int i=0; i<xlnum; i++)
	//	{
	//		id2=g_DocManageAry[m_iDoc].XLAry[i]->id();//nan

	//		if(g_DocManageAry[m_iDoc].XLAry[i]->m_mdbName == m_mdbName
	//			&& g_DocManageAry[m_iDoc].XLAry[i]->m_XLName == m_XLName
	//			&& g_DocManageAry[m_iDoc].XLAry[i]->id() == id())//818 判断id是否保险
	//			g_DocManageAry[m_iDoc].XLAry.erase(g_DocManageAry[m_iDoc].XLAry.begin()+i);
	//	}
	//}

}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus RHA::dwgOutFields (AcDbDwgFiler *pFiler) const 
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (RHA::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	ACHAR *chr=NULL;

	pFiler->writeItem(m_iDoc);
	pFiler->writeItem(m_mdbName);
	pFiler->writeItem(XLShuXing);
	pFiler->writeItem(m_XLName);

	pFiler->writeItem(draw_zybz); 
	pFiler->writeItem(JD0); 	
	pFiler->writeItem(texth);

	//颜色参数
	pFiler->writeItem(GTZXJDToJDColor);
	pFiler->writeItem(XWArcColor);
	pFiler->writeItem(HuanHequxianColor);
	pFiler->writeItem(JiaZhiXianColorZuo);
	pFiler->writeItem(JiaZhiXianColorYou);
	pFiler->writeItem(mdbnameBoxColor);
	pFiler->writeItem(mdbnameTextColor);
	pFiler->writeItem(QXYSColor);
	pFiler->writeItem(LineWeightValue);


	//夹点拖动
	/*pFiler->writeItem(MoveJDIndx);
	pFiler->writeBool(IsMove);
	pFiler->writeBool(IsMoveDone);*/

	int NJD=JDarray.size();

	pFiler->writeItem(NJD);
	for(int i=0;i<NJD;i++)
	{
		pFiler->writeItem(JDarray[i].XLName);
		pFiler->writeItem(JDarray[i].JDXZ);
		pFiler->writeItem(JDarray[i].BLName);
		pFiler->writeDouble(JDarray[i].N);
		pFiler->writeDouble(JDarray[i].E);
		pFiler->writeDouble(JDarray[i].l1);
		pFiler->writeDouble(JDarray[i].R);
		pFiler->writeDouble(JDarray[i].l2);
		chr=_tcsdup(JDarray[i].BLJDName);  
		pFiler->writeItem(chr);
		pFiler->writeDouble(JDarray[i].XJJ);
		pFiler->writeDouble(JDarray[i].QXJK);
		pFiler->writeDouble(JDarray[i].NYDL);
		chr=_tcsdup(JDarray[i].ZHLC);  
		pFiler->writeItem(chr);
		pFiler->writeBool(JDarray[i].IsXJJFirst);
		chr=_tcsdup(JDarray[i].JDName);  
		pFiler->writeItem(chr);
		chr=_tcsdup(JDarray[i].Draw_JDName);  
		pFiler->writeItem(chr);
	}

	int NDL=DLarray.size();
	pFiler->writeItem(NDL);
	for(int i=0;i<DLarray.size();i++)
	{
		chr=_tcsdup(DLarray[i].XLName); 
		pFiler->writeItem(chr);
		pFiler->writeItem(DLarray[i].Indx);
		chr=_tcsdup(DLarray[i].BGH);  
		pFiler->writeItem(chr);
		pFiler->writeDouble(DLarray[i].BLC);
		chr=_tcsdup(DLarray[i].EGH);  
		pFiler->writeItem(chr);
		pFiler->writeDouble(DLarray[i].ELC);
		pFiler->writeDouble(DLarray[i].TYLC);
		chr=_tcsdup(DLarray[i].RXDName); 
		pFiler->writeItem(chr);
		chr=_tcsdup(DLarray[i].BLName);  
		pFiler->writeItem(chr);
	}

	if (!(XLShuXing == LSX 
		|| XLShuXing == SGBX))
	{
		int NBZLC=BZLCarray.size();
		pFiler->writeItem(NBZLC);
		for (int j=0;j<NBZLC;j++)
		{   
			chr=_tcsdup(BZLCarray[j].XLName);
			pFiler->writeItem(chr);
			chr=_tcsdup(BZLCarray[j].GJDNum);  
			pFiler->writeItem(chr);
			chr=_tcsdup(BZLCarray[j].CKml);
			pFiler->writeItem(chr);
			pFiler->writeItem(BZLCarray[j].X);
			pFiler->writeItem(BZLCarray[j].Y);
			pFiler->writeBool(BZLCarray[j].isAutoAdd);
		};
	}

	//改建段数组
	int gjdNum=GJDInforArray.size();
	pFiler->writeItem(gjdNum);

	for (int j=0;j<gjdNum;j++)
	{
		chr=_tcsdup(GJDInforArray[j].XLName);
		pFiler->writeItem(chr);
		chr=_tcsdup(GJDInforArray[j].gjdname);
		pFiler->writeItem(chr);
		chr=_tcsdup(GJDInforArray[j].BXLName);
		pFiler->writeItem(chr);
		chr=_tcsdup(GJDInforArray[j].EXLName);
		pFiler->writeItem(chr);
		chr=_tcsdup(GJDInforArray[j].BLC);
		pFiler->writeItem(chr);
		chr=_tcsdup(GJDInforArray[j].ELC);
		pFiler->writeItem(chr);

		pFiler->writeItem(GJDInforArray[j].GJDType);
		pFiler->writeItem(GJDInforArray[j].spt[0]);
		pFiler->writeItem(GJDInforArray[j].spt[1]);
		pFiler->writeItem(GJDInforArray[j].ept[0]);
		pFiler->writeItem(GJDInforArray[j].ept[1]);

		pFiler->writeItem(GJDInforArray[j].GJDSml);
		pFiler->writeItem(GJDInforArray[j].GJDEml);
	}


	//纵面绕行段数组
	if (XLShuXing == SJ2X
		|| XLShuXing == JY2X)
	{
		int zmRxdNum=FZZDMAry.size();
		pFiler->writeItem(zmRxdNum);

		for (int j=0; j<zmRxdNum; j++)
		{
			chr=_tcsdup(FZZDMAry[j].XLName);
			pFiler->writeItem(chr);
			chr=_tcsdup(FZZDMAry[j].DLName);  
			pFiler->writeItem(chr);
			chr=_tcsdup(FZZDMAry[j].BLName);
			pFiler->writeItem(chr);
			chr=_tcsdup(FZZDMAry[j].StCKml);
			pFiler->writeItem(chr);
			chr=_tcsdup(FZZDMAry[j].EdCKml);
			pFiler->writeItem(chr);

			pFiler->writeItem(FZZDMAry[j].spt[0]);
			pFiler->writeItem(FZZDMAry[j].spt[1]);
			pFiler->writeItem(FZZDMAry[j].ept[0]);
			pFiler->writeItem(FZZDMAry[j].ept[1]);

			pFiler->writeItem(FZZDMAry[j].BL_BCml);
			pFiler->writeItem(FZZDMAry[j].BL_ECml);
		}
	}

	//////规范////////////
	pFiler->writeItem(DV);
	pFiler->writeItem(T0min);
	pFiler->writeItem(Lrmin);
	pFiler->writeItem(RtjMin);
	pFiler->writeItem(RtjMax);

	short rNum=0;
	rNum=m_Rs.size();
	pFiler->writeItem(rNum);
	for (int j=0;j<rNum;j++)
	{
		pFiler->writeDouble(m_Rs[j]);
		pFiler->writeDouble(m_l0s[j]);
	}

	int nBPD=m_BPDArray.size();
	pFiler->writeItem(&nBPD);
	for (int j=0;j<nBPD;j++)
	{
		pFiler->writeDouble(m_BPDArray[j].Cml);
		pFiler->writeDouble(m_BPDArray[j].degree);
		chr=_tcsdup(m_BPDArray[j].GH);
		pFiler->writeItem(chr);
		pFiler->writeDouble(m_BPDArray[j].length);
		pFiler->writeDouble(m_BPDArray[j].Level);
		pFiler->writeDouble(m_BPDArray[j].ml);
		chr=_tcsdup(m_BPDArray[j].Notes);
		pFiler->writeItem(chr);
		pFiler->writeDouble(m_BPDArray[j].RLen);
		pFiler->writeDouble(m_BPDArray[j].Rshu);
	}

	
	
	free(chr);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus RHA::dwgInFields (AcDbDwgFiler *pFiler) 
{
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > RHA::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < RHA::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	ACHAR *chr=NULL;

	pFiler->readItem(&m_iDoc); 
	pFiler->readItem(&chr);
	m_mdbName=chr;
	pFiler->readItem(&chr);
	XLShuXing=chr;
	pFiler->readItem(&chr);
	m_XLName.Format(L"%s",chr);

	pFiler->readItem(&draw_zybz); 
	pFiler->readItem(&JD0); 	
	pFiler->readItem(&texth);

	//颜色参数
	pFiler->readItem(&GTZXJDToJDColor);
	pFiler->readItem(&XWArcColor);
	pFiler->readItem(&HuanHequxianColor);
	pFiler->readItem(&JiaZhiXianColorZuo);
	pFiler->readItem(&JiaZhiXianColorYou);
	pFiler->readItem(&mdbnameBoxColor);
	pFiler->readItem(&mdbnameTextColor);
	pFiler->readItem(&QXYSColor);
	pFiler->readItem((int *)&LineWeightValue);

	//夹点拖动
	/*pFiler->readItem(&MoveJDIndx);   //818 这几个变量如果归档会发生拖不动交点的情况
	pFiler->readBool(&IsMove);
	pFiler->readBool(&IsMoveDone);*/

	int NJD;
	pFiler->readItem(&NJD);
    JDarray.clear();
	JDarray.resize(NJD);

	for(int i=0;i<NJD;i++)
	{
		pFiler->readItem(&chr);
		_tcscpy(JDarray[i].XLName, chr);	
		pFiler->readItem(&JDarray[i].JDXZ);
		pFiler->readItem(&chr);
		_tcscpy(JDarray[i].BLName, chr);
		//pFiler->readItem(&JDarray[i].JDIndx);
		pFiler->readDouble(&JDarray[i].N);
		pFiler->readDouble(&JDarray[i].E);
		pFiler->readDouble(&JDarray[i].l1);
		pFiler->readDouble(&JDarray[i].R);
		pFiler->readDouble(&JDarray[i].l2);
		pFiler->readItem(&chr);
		_tcscpy(JDarray[i].BLJDName, chr);
		pFiler->readDouble(&JDarray[i].XJJ);
		pFiler->readDouble(&JDarray[i].QXJK);
		pFiler->readDouble(&JDarray[i].NYDL);
		pFiler->readItem(&chr);
		_tcscpy(JDarray[i].ZHLC, chr);		
		pFiler->readBool(&JDarray[i].IsXJJFirst);
		pFiler->readItem(&chr);
		_tcscpy( JDarray[i].JDName, chr);
		pFiler->readItem(&chr);
		_tcscpy( JDarray[i].Draw_JDName,chr);
	}

	int NDL;
	pFiler->readItem(&NDL);
    DLarray.clear();
	DLarray.resize(NDL);
	for(int i=0;i<NDL;i++)
	{
		pFiler->readString(&chr);
		_tcscpy(DLarray[i].XLName, chr);
		pFiler->readItem(&DLarray[i].Indx);
		pFiler->readString(&chr);
		_tcscpy(DLarray[i].BGH, chr);
		pFiler->readDouble(&DLarray[i].BLC);
		pFiler->readString(&chr);
		_tcscpy(DLarray[i].EGH, chr);
		pFiler->readDouble(&DLarray[i].ELC);
		pFiler->readDouble(&DLarray[i].TYLC);
		pFiler->readString(&chr);
		_tcscpy(DLarray[i].RXDName, chr);
		pFiler->readString(&chr);
		_tcscpy(DLarray[i].BLName, chr);
	}

	if (!(XLShuXing == LSX 
		|| XLShuXing == SGBX))
	{
		int NBZLC;
		pFiler->readItem(&NBZLC);
		BZLCarray.clear();
		BZLCarray.resize(NBZLC);

		for (int j=0;j<NBZLC;j++)
		{
			pFiler->readString(&chr);
			_tcscpy(BZLCarray[j].XLName, chr);
			pFiler->readString(&chr);
			_tcscpy(BZLCarray[j].GJDNum, chr);
			pFiler->readString(&chr);
			_tcscpy(BZLCarray[j].CKml,chr);
			pFiler->readItem(&BZLCarray[j].X);
			pFiler->readItem(&BZLCarray[j].Y);
			pFiler->readBool(&BZLCarray[j].isAutoAdd);
		}
	}

	//改建段数组
	int gjdNum;
	pFiler->readItem(&gjdNum);
	GJDInforArray.clear();
	GJDInforArray.resize(gjdNum);
	for (int j=0;j<gjdNum;j++)
	{
		pFiler->readString(&chr);
		_tcscpy(GJDInforArray[j].XLName, chr);
		pFiler->readString(&chr);
		_tcscpy(GJDInforArray[j].gjdname, chr);
		pFiler->readString(&chr);
		_tcscpy(GJDInforArray[j].BXLName, chr);
		pFiler->readString(&chr);
		_tcscpy(GJDInforArray[j].EXLName, chr);
		pFiler->readString(&chr);
		_tcscpy(GJDInforArray[j].BLC, chr);
		pFiler->readString(&chr);
		_tcscpy(GJDInforArray[j].ELC, chr);
		pFiler->readItem(&GJDInforArray[j].GJDType);

		pFiler->readItem(&GJDInforArray[j].spt[0]);
		pFiler->readItem(&GJDInforArray[j].spt[1]);
		pFiler->readItem(&GJDInforArray[j].ept[0]);
		pFiler->readItem(&GJDInforArray[j].ept[1]);

		pFiler->readItem(&GJDInforArray[j].GJDSml);
		pFiler->readItem(&GJDInforArray[j].GJDEml);
	}


	//纵面绕行段数组
	if (XLShuXing == SJ2X
		|| XLShuXing == JY2X)
	{
		int zmRxdNum;
		pFiler->readItem(&zmRxdNum);
		FZZDMAry.clear();
		FZZDMAry.resize(zmRxdNum);

		for (int j=0;j<zmRxdNum;j++)
		{
			pFiler->readString(&chr);
			_tcscpy(FZZDMAry[j].XLName, chr);
			pFiler->readString(&chr);
			_tcscpy(FZZDMAry[j].DLName, chr);
			pFiler->readString(&chr);
			_tcscpy(FZZDMAry[j].BLName, chr);
			pFiler->readString(&chr);
			_tcscpy(FZZDMAry[j].StCKml, chr);
			pFiler->readString(&chr);
			_tcscpy(FZZDMAry[j].EdCKml, chr);
			pFiler->readItem(&FZZDMAry[j].spt[0]);
			pFiler->readItem(&FZZDMAry[j].spt[1]);
			pFiler->readItem(&FZZDMAry[j].ept[0]);
			pFiler->readItem(&FZZDMAry[j].ept[1]);

			pFiler->readItem(&FZZDMAry[j].BL_BCml);
			pFiler->readItem(&FZZDMAry[j].BL_ECml);
		}
	}

	//////规范////////////
	pFiler->readItem(&DV);
	pFiler->readItem(&T0min);
	pFiler->readItem(&Lrmin);
	pFiler->readItem(&RtjMin);
	pFiler->readItem(&RtjMax);

	short rNum=0;
	m_Rs.clear();
	m_l0s.clear();

	pFiler->readItem(&rNum);
	m_Rs.resize(rNum);
	m_l0s.resize(rNum);
	for (int j=0;j<rNum;j++)
	{
		pFiler->readDouble(&m_Rs[j]);
		pFiler->readDouble(&m_l0s[j]);
	}

	int nBPD=0;
	pFiler->readItem(&nBPD);
	m_BPDArray.clear();
	m_BPDArray.resize(nBPD);
	for (int j=0;j<nBPD;j++)
	{
		pFiler->readDouble(&m_BPDArray[j].Cml);
		pFiler->readDouble(&m_BPDArray[j].degree);
		pFiler->readString(&chr);
		_tcscpy(m_BPDArray[j].GH, chr);
		pFiler->readDouble(&m_BPDArray[j].length);
		pFiler->readDouble(&m_BPDArray[j].Level);
		pFiler->readDouble(&m_BPDArray[j].ml);
		pFiler->readString(&chr);
		_tcscpy(m_BPDArray[j].Notes, chr);
		pFiler->readDouble(&m_BPDArray[j].RLen);
		pFiler->readDouble(&m_BPDArray[j].Rshu);
	}


	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean RHA::subWorldDraw (AcGiWorldDraw *mode)
{
	assertReadEnabled () ;

	if (IsMove) 
	{//如果拖动夹点
		
		if (IsMoveDone)
		{//夹点拖动结束
			
			if (MoveJDIndx>-1 && MoveJDIndx<JDarray.size())
			{
				int iRxd=0;
				iRxd=GetiRxdByProjml((QXB[MoveJDIndx].KZH+QXB[MoveJDIndx].KHZ)/2.0);

				//更新计算里程

				if(iRxd>-1)
				{//拖绕行段交点

					UpdateLC(iRxd);
				}
				else
				{
					//从前一交点往前找最近的一个断链
					int sdl=0;
					if(MoveJDIndx>0)
						MoveJDIndx--;
					sdl=GetPreRealDL(QXB[MoveJDIndx].KZH);

					//从这个断链开始，重算之后的所有断链的投影里程
					CalDLprojml(sdl);
				}
			}
			else
			{
				//自动解算
				UpdateAllData();
				ads_printf(L"提示：系统将自动解算里程\n");
			}

			//计算标志里程
			CalBZLC();
			SaveData();
		
		}
		else
		{
			//夹点拖动中
			getpxy_DATA(MoveJDIndx);
		}
	}
	else
	{
		//如果线路绘图
		if (XLShuXing == JY2X 
			|| XLShuXing == SJJX 
			|| XLShuXing == SJ2X)
			ModifyExJDXY();
		
		getpxy_DATA();

		//计算标志里程
		CalBZLC();
		SaveData();
	}
	

	//////nan
	//CString str;
	//for (int i=0;i<DLarray.size();i++)
	//{
	//	str.Format(L"\n%s断链 编号%d\n 前%s%lf", m_XLName, DLarray[i].Indx, DLarray[i].BGH, DLarray[i].BLC);
	//	ads_printf(str);
	//	str.Format(L" 后%s%lf",DLarray[i].EGH, DLarray[i].ELC);
	//	ads_printf(str);
	//}


	////nan
	//CString str;
	//for (int i=0;i<BZLCarray.size();i++)
	//{
	//	str.Format(L"\n%s标志 %s K-%lf %d\n ", m_XLName, BZLCarray[i].CKml,BZLCarray[i].K, BZLCarray[i].isAutoAdd);
	//	ads_printf(str);
	//	str.Format(L"投影里程%lf 连续里程%lf ",BZLCarray[i].xyToprojml, BZLCarray[i].kmlTocml  );
	//	ads_printf(str);
	//}

	//ads_printf(L"\n/////////////\n");


	//818 计算内业断链

	//一、几何

	// 绘线位
	DrawXL(mode);

	//绘交点连线
	DrawJDLineAndCir(mode);

	//二、里程

	if (!IsMove || (IsMove && IsMoveDone))//导入或拖动结束点鼠标时绘制里程标注 
	{
		//形成贯通断链表  818 以后不在subworlddraw中生成贯通断链表，会不会影响改建段里程的计算？？
		if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		{
			//删除并行段断链
			EarseDLofBLonBXD();
			//形成贯通断链表
			Create2XGTDLB();
		}

		//标注交点
		MarkJD(mode,JDarray,QXB,JDGH);

		//标注特征点
		MarkTZD(mode);

		//标注绕行段里程和断链
		MarkRXDLC(mode);

		//标注线间距
		MarkXJJ(mode);

		//标注标志里程
		if (XLShuXing != SJJX && XLShuXing != SJ2X)
			MarkBiaoZhiLC(mode, 100, /*draw_zybz*/1, 2);//818 所有的标注标左标右如何判断？

		//标注并行段断链
		for(int i=1;i<DLarray.size();i++)
		{
			if (DLarray[i].Indx == -2)
				MarkSglDL(mode, i, 1);
		}

		//标注坡度  
		if(m_BPDArray.size()>0)
		{
			//变坡点数组 转 CM 数组
			TrsBPDArraytoCM();
			BZCM(mode);//标注变坡点
		}
	}

	
	
	IsMove=false;
	IsMoveDone=true;

	return (AcDbEntity::subWorldDraw (mode)) ;
}

void RHA::TrsBPDArraytoCM()
{
	int NBPD = m_BPDArray.size();
	if (NBPD==0)
		return;
	ACHAR GH[10],Ckml[20];
	vector<vector<double>> tmpBPD_array;

	tmpBPD_array.clear();
	tmpBPD_array.resize(NBPD);
	for (int i=0; i<NBPD; i++)
		tmpBPD_array[i].resize(5);


	for (int i=0; i<NBPD; i++)
	{
		_tcscpy(GH, m_BPDArray[i].GH);
		_tcscpy(Ckml, LCchr(GH, m_BPDArray[i].ml, 3));
		TrsCkmltoCml(Ckml, m_BPDArray[i].Cml);//818
		tmpBPD_array[i][0] = m_BPDArray[i].Cml;//连续里程  
		tmpBPD_array[i][1] = m_BPDArray[i].Level;	//主线坡度			
		tmpBPD_array[i][2] = m_BPDArray[i].Rshu;//竖曲线半径
		tmpBPD_array[i][3] = 0;
		tmpBPD_array[i][4] = 0;
	}

	m_CM.clear();
	m_CM.resize((NBPD+1)*5);

	double LC1,H1,LC2,H2,R;
	LC1= tmpBPD_array[0][0];
	H1=  tmpBPD_array[0][1];  
	m_CM[0] =LC1,m_CM[1]=H1,m_CM[2]=0.0, m_CM[3] = 0.0, m_CM[4]=0.0;

	for(int i=1; i<NBPD; i++)
	{
		LC2= tmpBPD_array[i][0]; 
		H2=tmpBPD_array[i][1];    
		R =tmpBPD_array[i][2];
		m_CM[i*5]=LC2,m_CM[i*5+1]=H2;    
		m_CM[i*5+2]=(H2-H1)/(LC2-LC1);
		m_CM[i*5+3]=LC2-LC1;
		m_CM[i*5+4]=R;
		H1=H2,LC1=LC2;
	}
	m_CM[NBPD*5] =m_CM[NBPD*5+1]=m_CM[NBPD*5+2]=m_CM[NBPD*5+3]=m_CM[NBPD*5+4]=0.0;
}


bool RHA::BZCM(AcGiWorldDraw* pWd)
{
	xlpoint pt;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt,InterPt2,BefSlopePt,AftSlopePt,MLPt,BefSlopePt1;
	ACHAR Ckml[20];
	ACHAR BefLen[20];
	ACHAR AftLen[20];
	ACHAR BefSlope[20];
	ACHAR AftSlope[20];
	ACHAR Lever[20];
	int LorR = -1;
	double YL = 50;
	double XL = 50;
	int Color = 0;
	double CkmlAng;
	double LeverAng;
	double BefSlopeAng;
	double AftSlopeAng;
	bool isBefSlopeUp=false;
	bool isAftSlopeUp=false;
	int LeverXSnum;
	int SlopeXSnum;
	if (m_BPDArray.size()>0)
	{
		LeverXSnum=m_BPDArray[0].Rshu;
		SlopeXSnum=_wtoi(m_BPDArray[0].Notes);
	} 
	else
	{
		LeverXSnum=2;
		SlopeXSnum=2;
	}

	int m_NCM=m_BPDArray.size()+1;
	for (int i=0; i< m_NCM-1;i++)
	{
		isBefSlopeUp=false;
		isAftSlopeUp=false;
		TrsCmltoPZ(m_CM[i*5], pt);
		TrsCmltoCkml(m_CM[i*5], Ckml, /*m_Xsw*/3);//m_Xsw 818 取小数位数
		if (LeverXSnum==0)
		{
			_stprintf(Lever,L"%.0lf",m_CM[i*5+1]);
		}
		else if (LeverXSnum==1)
		{
			_stprintf(Lever,L"%.1lf",m_CM[i*5+1]);
		}
		else if (LeverXSnum==2)
		{
			_stprintf(Lever,L"%.2lf",m_CM[i*5+1]);
		}
		else if (LeverXSnum==3)
		{
			_stprintf(Lever,L"%.3lf",m_CM[i*5+1]);
		}
		else if (LeverXSnum==4)
		{
			_stprintf(Lever,L"%.4lf",m_CM[i*5+1]);
		}
		else 
		{
			_stprintf(Lever,L"%.2lf",m_CM[i*5+1]);
		}

		if (SlopeXSnum==0)
		{
			_stprintf(BefLen,L"%.0lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.0lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.0lf‰",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.0lf‰",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==1)
		{
			_stprintf(BefLen,L"%.1lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.1lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.1lf‰",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.1lf‰",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==2)
		{
			_stprintf(BefLen,L"%.2lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.2lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.2lf‰",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.2lf‰",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==3)
		{
			_stprintf(BefLen,L"%.3lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.3lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.3lf‰",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.3lf‰",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==4)
		{
			_stprintf(BefLen,L"%.4lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.4lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.4lf‰",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.4lf‰",1000*m_CM[(i+1)*5+2]);
		}
		else 
		{
			_stprintf(BefLen,L"%.2lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.2lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.2lf‰",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.2lf‰",1000*m_CM[(i+1)*5+2]);
		}

		if (m_CM[i*5+2]*1000.0>0.001)
		{
			isBefSlopeUp=true;
		}
		if (m_CM[(i+1)*5+2]*1000.0>0.001)
		{
			isAftSlopeUp=true;
		}
		cenPT[X]=pt.y;
		cenPT[Y]=pt.x;
		InterPt[X] = cenPT[X] + 0.5*YL * sin(pt.a + LorR*0.5*PI);
		InterPt[Y] = cenPT[Y] + 0.5*YL * cos(pt.a + LorR*0.5*PI);
		InterPt2[X]=cenPT[X] + YL * sin(pt.a + LorR*0.5*PI);
		InterPt2[Y]=cenPT[Y] + YL * cos(pt.a + LorR*0.5*PI);
		BefPt[X] = InterPt[X] - XL * sin(pt.a);
		BefPt[Y] = InterPt[Y] - XL * cos(pt.a);
		AftPt[X] = InterPt[X] + XL * sin(pt.a);
		AftPt[Y] = InterPt[Y] + XL * cos(pt.a);
		if (isBefSlopeUp)
		{
			BefSlopePt[X] = BefPt[X] + 0.5*YL * sin(pt.a - LorR*0.5*PI);
			BefSlopePt[Y] = BefPt[Y] + 0.5*YL * cos(pt.a - LorR*0.5*PI);
		}
		else
		{
			BefSlopePt[X] = BefPt[X] - 0.5*YL * sin(pt.a - LorR*0.5*PI);
			BefSlopePt[Y] = BefPt[Y] - 0.5*YL * cos(pt.a - LorR*0.5*PI);
		}
		if (isAftSlopeUp)
		{
			AftSlopePt[X] = AftPt[X] - 0.5*YL * sin(pt.a - LorR*0.5*PI);
			AftSlopePt[Y] = AftPt[Y] - 0.5*YL * cos(pt.a - LorR*0.5*PI);
		} 
		else
		{
			AftSlopePt[X] = AftPt[X] + 0.5*YL * sin(pt.a - LorR*0.5*PI);
			AftSlopePt[Y] = AftPt[Y] + 0.5*YL * cos(pt.a - LorR*0.5*PI);
		}
		MLPt[X] = cenPT[X] + 5 * sin(pt.a + LorR*0.5*PI);
		MLPt[Y] = cenPT[Y] + 5 * cos(pt.a + LorR*0.5*PI);
		G_makeline(pWd, cenPT, InterPt2, Color);
		G_makeline(pWd, InterPt, BefPt, Color);
		G_makeline(pWd, InterPt, AftPt, Color);
		if (i>0)
			G_makeline(pWd, InterPt, BefSlopePt, Color);
		if (i<m_NCM-2)
			G_makeline(pWd, InterPt, AftSlopePt, Color);


		ads_point Spt,Ept;

		if (LorR == -1)
		{
			Spt[0] = BefSlopePt[X];
			Spt[1] = BefSlopePt[Y];
			Ept[0] = InterPt[X];
			Ept[1] = InterPt[Y];
			BefSlopeAng = ads_angle(Spt,Ept);
			Spt[0] = InterPt[X];
			Spt[1] = InterPt[Y];
			Ept[0] = AftSlopePt[X];
			Ept[1] = AftSlopePt[Y];
			AftSlopeAng = ads_angle(Spt,Ept);

			CkmlAng = FwjtoAngX(pt.a -0.5*PI);
			G_maketext(pWd, MLPt, Ckml, CkmlAng, texth, Color, 1);

			LeverAng = FwjtoAngX(pt.a);
			G_maketext(pWd, InterPt2, Lever, LeverAng, texth, Color, 0);

			if (i>0)
			{
				G_maketext(pWd, BefSlopePt, BefSlope, BefSlopeAng, texth, Color, 1);
				G_maketext(pWd, BefSlopePt, BefLen, BefSlopeAng, texth, Color, 3);
			}
			if (i<m_NCM-2)
			{
				G_maketext(pWd, AftSlopePt, AftSlope, AftSlopeAng, texth, Color, 2);
				G_maketext(pWd, AftSlopePt, AftLen, AftSlopeAng, texth, Color, 3);
			}
		}
		else
		{
			Spt[0] = BefSlopePt[X];
			Spt[1] = BefSlopePt[Y];
			Ept[0] = InterPt[X];
			Ept[1] = InterPt[Y];
			BefSlopeAng = ads_angle(Ept,Spt);
			Spt[0] = InterPt[X];
			Spt[1] = InterPt[Y];
			Ept[0] = AftSlopePt[X];
			Ept[1] = AftSlopePt[Y];
			AftSlopeAng = ads_angle(Ept,Spt);

			CkmlAng = FwjtoAngX(pt.a -0.5*PI);
			G_maketext(pWd, MLPt, Ckml, CkmlAng, texth, Color, 2);

			LeverAng = FwjtoAngX(pt.a + PI);
			G_maketext(pWd, InterPt2, Lever, LeverAng, texth, Color, 0);

			if (i>0)
			{
				G_maketext(pWd, BefSlopePt, BefSlope, BefSlopeAng, texth, Color, 2);
				G_maketext(pWd, BefSlopePt, BefLen, BefSlopeAng, texth, Color, 3);
			}
			if (i<m_NCM-2)
			{
				G_maketext(pWd, AftSlopePt, AftSlope, AftSlopeAng, texth, Color, 1);
				G_maketext(pWd, AftSlopePt, AftLen, AftSlopeAng, texth, Color, 3);
			}

		}

		BZSHUQX(pWd,i,LorR);
	}
	return true;
}

//标注竖曲线
void RHA::BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR)
{
	xlpoint pt;
	double YL = 10;
	double XL = 5;
	double LC;
	int Color = 1;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt;
	AcGePoint3d  TBefPt,TAftPt;
	if (num==0 || num+1==m_BPDArray.size())
	{
		return;
	}
	if (m_BPDArray[num].RLen<0.001)
	{
		return;
	}
	LC=m_CM[num*5]-m_BPDArray[num].RLen*0.5;
	if ((num == 0) && (LC < /*StartCml*/0.0))//起点 818
	{
		LC=m_CM[num*5];

	}
	TrsCmltoPZ(LC, pt);
	BefPt[X] = pt.y;
	BefPt[Y] = pt.x;
	InterPt[X] = BefPt[X] + YL * sin(pt.a + LorR*0.5*PI);
	InterPt[Y] = BefPt[Y] + YL * cos(pt.a + LorR*0.5*PI);
	TBefPt[X] = InterPt[X] - XL * sin(pt.a);
	TBefPt[Y] = InterPt[Y] - XL * cos(pt.a);
	TAftPt[X] = InterPt[X] + XL * sin(pt.a);
	TAftPt[Y] = InterPt[Y] + XL * cos(pt.a);
	G_makeline(pWd, BefPt, InterPt, Color);
	G_makeline(pWd, InterPt, TBefPt, Color);
	G_makeline(pWd, InterPt, TAftPt, Color);
	LC=m_CM[num*5]+m_BPDArray[num].RLen*0.5;

	int m_NCM=m_BPDArray.size()+1;
	if ( (num == m_NCM-2) /*&& (LC> EndCml)*/)//818
	{
		LC=m_CM[num*5];
	}
	TrsCmltoNE(LC, pt.x, pt.y);
	AftPt[X]=pt.y;
	AftPt[Y]=pt.x;
	InterPt[X] = AftPt[X] + YL * sin(pt.a + LorR*0.5*PI);
	InterPt[Y] = AftPt[Y] + YL * cos(pt.a + LorR*0.5*PI);
	TBefPt[X] = InterPt[X] - XL * sin(pt.a);
	TBefPt[Y] = InterPt[Y] - XL * cos(pt.a);
	TAftPt[X] = InterPt[X] + XL * sin(pt.a);
	TAftPt[Y] = InterPt[Y] + XL * cos(pt.a);
	G_makeline(pWd, AftPt, InterPt, Color);
	G_makeline(pWd, InterPt, TBefPt, Color);
	G_makeline(pWd, InterPt, TAftPt, Color);
}






Adesk::UInt32 RHA::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

	//- Osnap points protocol
Acad::ErrorStatus RHA::subGetOsnapPoints(
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,	
	const AcGePoint3d &pickPoint,	
	const AcGePoint3d &lastPoint,	
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds,
	const AcGeMatrix3d& insertionMat) const 
{
	assertReadEnabled();
	Acad::ErrorStatus es=Acad::eOk;
	// if (gsSelectionMark==0) return Acad::eOk;
	if (osnapMode!=AcDb::kOsModeEnd &&osnapMode!=AcDb::kOsModeNear
		&& osnapMode!=AcDb::kOsModeMid
		&& osnapMode!=AcDb::kOsModePerp && osnapMode!=AcDb::kOsModeCen
		&& osnapMode!=AcDb::kOsModeIns) 
	{ return Acad::eOk;  };

	int i,j,k;
	double l=0.0,INDATA[5] ;
	xlpoint  PB,PE,PZ; 
	AcGePoint3d Pt;
	xlpoint PZtmp;
	int NJD=JDarray.size();
	int DLNum=DLarray.size();;

	AcGePoint3d  ptb,pt,pte,p1;
	ACHAR GH[16], GH1[16];
	int iDL, BZW;
	double dlc=100;
	double cml, LCB, BZlc, B;
	double NoteLen=1.5*texth;//标注线长度
	long int zlc;
	double DDlength=0;
	int rxdNum;
	int sdl, edl;
	int iRXD;
	ACHAR ckml[50];


	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd :
		// 交点
		for ( i = 0; i < NJD; i++) 
		{  
			Pt.set(JDarray[i].E,JDarray[i].N,0.0);
			snapPoints.append(Pt);
		};

		//  圆弧的终点
		PB.x=array_pxy[1]; //N
		PB.y=array_pxy[2]; // E
		PB.a=  rad(array_pxy[3]);// FA
		PB.lc=array_pxy[4];
		PB.r=1e40;
		PE.x=PE.y =PE.lc=PE.a=0.0;
		for (i =1; i < NJD; i++) 
		{    
			for (k =0; k<4; k++) 	
			{
				for (j=0; j<5; j++) 
					INDATA[j]=array_pxy[i*20+k*5+j]; 

				l=  Get_INDATA_L(INDATA,&j);
				xlzbxy(PB,PE,l,INDATA,0);// 某线元终点
				Pt[X]=PE.y;Pt[Y]=PE.x; Pt[Z]=0.0;
				snapPoints.append(Pt);
				/*
				xlzbxy(PB,&PZ,l*0.5,INDATA,0); // 某线元中点
				Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
				if (j==2) // 圆弧 中点
				{ snapPoints.append(Pt);	};
				*/
				PB=PE;  
			};
		};
		
	
		
		//断链点  818 暂时隐去
		for(int i=0;i<DLNum;i++)
		{
			PZtmp.lc=DLarray[i].TYLC;  // 统一里程
			k=xlpoint_pz(array,tarray,PZtmp);  //这里用到array 818 会挂掉
			if (k>0 && k<=6)  //有解
			{
				B=PZtmp.a;
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
				snapPoints.append(ptb);
				ptb[X]=PZtmp.y+27*texth*sin(B+pi*0.5);
				ptb[Y]=PZtmp.x+27*texth*cos(B+pi*0.5);
				snapPoints.append(ptb);
			}; 

		}


		//百米标点
		ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
		for(iRXD=0;;iRXD++)
		{
			for(int i=0;i<DLNum;i++)
			{
				sdl=-1;
				edl=-1;

				if(DLarray[i].Indx==iRXD)
				{
					sdl=i;
					while(i<DLNum && DLarray[i].Indx==iRXD)//plh05.26
					{
						edl=i;			
						i++;
					}
					break;
				}
			}
			
			if (sdl>-1 && edl>-1)
			{
				DDlength=DLarray[sdl].TYLC;

				for (iDL=sdl;iDL<edl;iDL++)
				{ 
					LCB= DLarray[iDL].ELC;  //本段起点里程 
					_tcscpy(GH,DLarray[iDL].EGH);
					_tcscpy(GH1,DLarray[iDL].BGH);

					zlc=(long int)(LCB/dlc); //818
					BZlc=zlc*dlc;  // 实际里程 
					if ((BZlc-LCB)<0.0001) 
						BZlc=BZlc+dlc;

					if ( BZlc>DLarray[iDL+1].BLC+0.0001)
					{
						DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//断链处统一里程。
						continue;
					}

					_tcscpy(GH,DLarray[iDL].EGH);  //冠号

					do 
					{
						// 连续里程
						cml=DDlength+BZlc-LCB; 
						// 投影里程
						//PZtmp.lc=CalcmlTonewcml(cml); 

						PZtmp.lc=cml;
						if(BZLCarray.size()<1 || XLShuXing == SJJX || XLShuXing == SJ2X)
							PZtmp.lc=cml;
						else
						{
							for(int i=1;i<BZLCarray.size();i++)
							{
								if (fabs(cml-BZLCarray[i].kmlTocml)<0.1)
								{
									PZtmp.lc= BZLCarray[i].xyToprojml;
								}
								if(cml>BZLCarray[i-1].kmlTocml-0.00001 && cml<BZLCarray[i].kmlTocml+0.00001)
								{
									PZtmp.lc = BZLCarray[i-1].xyToprojml + BZLCarray[i].K * (cml - BZLCarray[i-1].kmlTocml);
								}
							}

						}

						//添加捕捉点		 
						k=xlpoint_pz(array,tarray,PZtmp);   
						BZW=k;    //  K<0  无解
						if (k>0 && k<=6)  //有解
						{
							B=PZtmp.a;
							ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
							snapPoints.append(ptb);
							ptb[X]=PZtmp.y+NoteLen*sin(B+pi*0.5);
							ptb[Y]=PZtmp.x+NoteLen*cos(B+pi*0.5);
							snapPoints.append(ptb);//818 snapPoints 需要remove，防止越界
						};          
						BZlc=BZlc+dlc;
						if ( BZlc>DLarray[iDL+1].BLC) 
							BZW=-99;		 		 

					}while (BZW>0);

					DDlength=  DDlength+DLarray[iDL+1].BLC-LCB;//断链处统一里程。
				} // IDL

			}
			else
				break;
		}

		////标志里程
		//if(XLShuXing != SJJX && XLShuXing != SJ2X)
		//{
		//	for (i =0; i < BZLCarray.GetSize(); i++) 
		//	{  
		//		if (BZLCarray[i].isAutoAdd == false)
		//		{
		//			PZtmp.lc=BZLCarray[i].xyToprojml;
		//			k=xlpoint_pz(array,tarray,PZtmp);   
		//			if (k>0 && k<=6)  //有解
		//			{
		//				B=PZtmp.a;
		//				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
		//				snapPoints.append(ptb);
		//				ptb[X]=PZtmp.y+NoteLen*sin(B+pi*0.5);
		//				ptb[Y]=PZtmp.x+NoteLen*cos(B+pi*0.5);
		//				snapPoints.append(ptb);
		//			};  

		//		}
		//	}

		//}

		break;

	case  AcDb::kOsModeMid:

		PB.x=array_pxy[1]; //N
		PB.y=array_pxy[2]; // E
		PB.a=  rad(array_pxy[3]);// FA
		PB.lc=array_pxy[4];
		PB.r=1e40;
		PE.x=PE.y =PE.lc=PE.a=0.0;
		for (i =1; i < NJD; i++) 
		{    
			for (k =0; k <4; k++) 	
			{
				for (j=0; j <5; j++) INDATA[j]=array_pxy[i*20+k*5+j]; 
				l=  Get_INDATA_L(INDATA,&j);
				xlzbxy(PB,PZ,l*0.5,INDATA,0); // 某线元中点
				Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
				if (j==2) // 圆弧 中点
				{ 
					snapPoints.append(Pt);	
				};
				xlzbxy(PB,PE,l,INDATA,0);// 某线元终点
				PB=PE;  
			};
		}
		
		break;
	case AcDb::kOsModeNear:
		/*PZtmp.lc=PROJ_ML(array, tarray, pickPoint.y,pickPoint.x,pickPoint.y,pickPoint.x);
		xlpoint_pz(array,tarray,PZtmp);
		Pt.x =  PZtmp.y;
		Pt.y =  PZtmp.x;
		Pt.z =  0.0;
		snapPoints.append(Pt);  */  
		break;

	default :       
		return Acad::eOk;
	}

	return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus RHA::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
	) const {
		assertReadEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new getGripPoints() method below (which is the default implementation)

		AcGePoint3d Pt;
		xlpoint  PB, PE, PZ;
		double l=0.0,INDATA[5];
		int i=0, j=0, k=0;


		//一、交点作为夹点
		int NJD=JDarray.size();
		for (i=0; i<NJD; i++) 
		{
			Pt.set(JDarray[i].E, JDarray[i].N, 0.0);
			gripPoints.append(Pt);
		}

		//二、将各交点对应的圆弧中点加入夹点集。序号（NJD-1）+（NJD-1）+1——（NJD-1)+（NJD-1）+NJD-2
		PB.x=array_pxy[1]; //N
		PB.y=array_pxy[2]; // E
		PB.a=rad(array_pxy[3]);// FA
		PB.lc=array_pxy[4];
		PB.r=1e40;
		PE.x=PE.y=PE.lc=PE.a=0.0;
		for (i=1; i<NJD-1; i++) 
		{
			for (k =0; k<4; k++) 	
			{
				for (j=0; j <5; j++) 
					INDATA[j]=array_pxy[i*20+k*5+j]; 

				l=Get_INDATA_L(INDATA,&j);
				xlzbxy(PB,PE,l,INDATA,0);// 某线元终点
				xlzbxy(PB,PZ,l*0.5,INDATA,0); // 某线元中点

				if (j == 2) //圆弧中点
				{
					Pt[X]=PZ.y; Pt[Y]=PZ.x; Pt[Z]=0.0;
					gripPoints.append(Pt);
				}
				PB=PE;  
			}
		}


		//for(int i=0; i<GJDNotePtAry.GetSize(); i++)
		//	gripPoints.append(GJDNotePtAry[i]);

		//gripPoints.append(m_XLNameNotePt);

		return Acad::eOk;
}


//功能：根据用户设置的交点拖动方式和该交点的属性，得到该交点的实际拖动方式 
//输入： jdMoveType        用户设置的交点拖动方式
//       iJD               交点下标
//返回：返回交点拖动方式；如果不能拖动，返回-100
//算法：
int  RHA::GetJDMoveType(int jdMoveType, int iJD) const 
{
	int realMoveType=-100;
	int NJD=JDarray.size();

	if (JDarray[iJD].JDXZ == BXPX)
		ads_printf(L"平行交点不能拖动\n");
	else if (JDarray[iJD].JDXZ == S_Start || JDarray[iJD].JDXZ == S_End )
		ads_printf(L"S弯交点不能拖动\n");
	else if (JDarray[iJD].JDXZ == RX_One)
		//绕行中
		realMoveType=jdMoveType;
	else if(JDarray[iJD].JDXZ == RX_Start)
	{//绕行始，默认按前切线拖动

		if(iJD>0 && jdMoveType != HQX && jdMoveType != QYHQ)
		{//没有选择按后切线拖 

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == QQY)
				jdMoveType=QQX;//前切线
			else if(jdMoveType == QHQY || jdMoveType == HQY)
				jdMoveType=QQHY;//前切后圆

			if (iJD<NJD-1 && JDarray[iJD+1].JDXZ != RX_One && JDarray[iJD+1].JDXZ != RX_End)
			{//单点绕行

				//该单点绕行没有记录后一直线边平行信息
				//，那么该交点可以拖动
				if(IsJdBLInfEmpty(JDarray[iJD]))
					realMoveType=jdMoveType;
			}
			else
				realMoveType=jdMoveType;
		}
		else if (iJD == 0)//起点绕行
			realMoveType=jdMoveType;
		
	}
	else if (JDarray[iJD].JDXZ == RX_End)
	{//绕行止，默认按后切线拖动

		if(iJD<NJD-1 && jdMoveType != QQX && jdMoveType != QQHY)
		{//没有选择按前切线拖

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == HQY)
				jdMoveType=HQX;//后切线
			else if(jdMoveType == QHQY || jdMoveType == QQY)
				jdMoveType=QYHQ;//前圆后切

			realMoveType=jdMoveType;
		}
		else if (iJD == NJD-1)//终点绕行
			realMoveType=jdMoveType;
	}
	else if (JDarray[iJD].JDXZ == BX_NotPX)
	{
		//818 起终点不平行的情况怎么考虑？

		if(iJD>0 && JDarray[iJD-1].JDXZ != BX_NotPX
			&& iJD<NJD-1 && JDarray[iJD+1].JDXZ != BX_NotPX)
		{//单点并行不平行

			realMoveType=-100;
		}
		else if (iJD>0 && JDarray[iJD-1].JDXZ != BX_NotPX
			&& jdMoveType != HQX && jdMoveType != QYHQ)
		{//前一交点不是并行不平行，并且没有选择按后切线拖

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == QQY)
				jdMoveType=QQX;//前切线
			else if(jdMoveType == QHQY || jdMoveType == HQY)
				jdMoveType=QQHY;//前切后圆

			realMoveType=jdMoveType;
		}
		else if (iJD<NJD-1 && JDarray[iJD+1].JDXZ != BX_NotPX
			&& jdMoveType != QQX && jdMoveType != QQHY)
		{//后一交点不是并行不平行，并且没有选择按前切线拖

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == HQY)
				jdMoveType=HQX;//后切线
			else if(jdMoveType == QHQY || jdMoveType == QQY)
				jdMoveType=QYHQ;//前圆后切

			realMoveType=jdMoveType;
		}
		else
			//前后都有并行不平行交点
			realMoveType=jdMoveType;
	}

	return realMoveType;
}

Acad::ErrorStatus RHA::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset)
{
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	IsMove=true;
	MoveJDIndx=-1;


	int jdMoveType=0;
	jdMoveType=g_JDMoveType;


	//g_JDMoveType 交点拖动方式: =1为沿切线拖动  =2 沿前切圆滑动 3 沿后切圆滑动 
	//=4 沿前后切圆滑动 =5 前切后圆 =6 前圆后切 =7 沿前切线拖动 =8 沿后切线拖动 其他整数 任意拖动
	
	//GetDLTYLC();   ls 未考虑
	double dd=0, d1=0, d2=0, DD01=0, DD02=0, DD1=0, DD2=0;//距离
	double fb=0, fb1=0, fb2=0;//方位角
	double dx=0, dy=0;
	CString  mes;
	int k=0, j=0;
	int NJD=JDarray.size();


	dx=offset[X];//偏移量
	dy=offset[Y];	
	for ( j=0; j<indices.length(); j++)
	{	
		k=indices[j];       
	// 一、拖动交点（包括起点和终点），交点序号 0~NJD-1		
		if (k>-1 && k<NJD) 
		{
			MoveJDIndx=k;
			////nan
			//CString mes;
			//mes.Format(L"前：拖动方式 %d\n", jdMoveType);
			//ads_printf(mes);

			//根据用户设置的交点拖动方式和该交点的属性，得到该交点的实际拖动方式
			jdMoveType=GetJDMoveType(jdMoveType, k);
			if (jdMoveType<0)
				return Acad::eOk;

			////nan
			//mes.Format(L"后：拖动方式 %d\n", jdMoveType);
			//ads_printf(mes);

			

			if (jdMoveType == ZJQX)  
			{//交点沿最近切线拖动

				//由直线段(大地坐标)[(XB,YB),(XE,YE)]的长度和方位角  返回:距离， *方位角
				if(k>0)
				{
					DD01=xyddaa(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N,JDarray[k].E, &fb1);
					DD1=xyddaa(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N+dy,JDarray[k].E+dx, &fb);
					d1=DD1-DD01;
				}

				if (k<NJD-1)
				{
					DD02 =xyddaa(JDarray[k].N, JDarray[k].E, JDarray[k+1].N, JDarray[k+1].E, &fb2);
					DD2=xyddaa(JDarray[k].N+dy, JDarray[k].E+dx, JDarray[k+1].N, JDarray[k+1].E, &fb);
					d2=DD02-DD2;
				}

				dd=d1; fb=fb1;
				if (fabs(d1)<fabs(d2)) 
				{ //沿后切线拖动
					dd=d2; 
					fb=fb2;
				};
				//如果是起点，由后一个交点计算起点坐标；如果是终点，由前一个交点计算

				JDarray[k].E=JDarray[k].E+dd*sin(fb);
				JDarray[k].N=JDarray[k].N+dd*cos(fb);
			}
			else if (jdMoveType == QQX && k>0)
			{
				//沿前切线拖动
				DD01=xyddaa(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N,JDarray[k].E, &fb1);
				DD1=xyddaa(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N+dy,JDarray[k].E+dx, &fb);
				dd=DD1-DD01;

				JDarray[k].E=JDarray[k].E+dd*sin(fb1);
				JDarray[k].N=JDarray[k].N+dd*cos(fb1);
			}
			else if (jdMoveType == HQX && k<NJD-1)
			{
				//沿后切线拖动
				DD02 =xyddaa(JDarray[k].N, JDarray[k].E, JDarray[k+1].N, JDarray[k+1].E, &fb2);
				DD2=xyddaa(JDarray[k].N+dy, JDarray[k].E+dx, JDarray[k+1].N, JDarray[k+1].E, &fb);
				dd=DD02-DD2;

				JDarray[k].E=JDarray[k].E+dd*sin(fb2);
				JDarray[k].N=JDarray[k].N+dd*cos(fb2);
			}
			else if(jdMoveType >= 3 && jdMoveType <= 7)
			{
				//1.12 交点沿圆弧拖动
				double OutN,OutE;
				OutN=JDarray[k].N+offset[Y];
				OutE=JDarray[k].E+offset[X];
				// 切前圆
				if (jdMoveType == QQY || jdMoveType == QHQY || jdMoveType == QYHQ)  
				{

					if(k>1)//不是起点
					{
						double l1,l2,R1,p1,tR1,A2;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

						l1=JDarray[k-1].l1;
						l2=JDarray[k-1].l2;
						R1=JDarray[k-1].R;
						if(l2>0.0)
						{
							A2=sqrt(R1*l2);
							double x=spiral_y(A2,l2);
							double t=l2/(2.0*R1);
							double dr=x+R1*sin(t+m_dHalfPI);			
							p1=dr-R1;
						}
						else p1=0.0;					

						tR1=R1+p1;//半径+内移量	
						//求前圆圆心
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k-2].N;
						E1=JDarray[k-2].E;
						N0=JDarray[k-1].N;
						E0=JDarray[k-1].E;
						N2=JDarray[k].N;
						E2=JDarray[k].E;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,l1,l2,CenN,CenE);
						double TN,TE;//切点
						//求切点
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
						INTERS(&JDarray[k-1].N,&JDarray[k-1].E,N1,E1,N0,E0,TN,TE,OutN,OutE);
					}
					else
						ads_printf(L"固定前圆模式，不能更改此交点坐标\n");
				}

				// 切后圆
				if (jdMoveType == HQY || jdMoveType == QHQY || jdMoveType == QQHY)  
				{
					if(k<NJD-2)//不是终点
					{
						double l1,l2,R2,p2,tR2,A1;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

						l1=JDarray[k+1].l1;
						l2=JDarray[k+1].l2;
						R2=JDarray[k+1].R;
						if(l1>0.0)
						{
							A1=sqrt(R2*l1);
							double x=spiral_y(A1,l1);
							double t=l1/(2.0*R2);
							double dr=x+R2*sin(t+m_dHalfPI);			
							p2=dr-R2;
						}
						else p2=0.0;					
						//半径+内移量					
						tR2=R2+p2;
						//求前圆圆心
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k].N;
						E1=JDarray[k].E;
						N0=JDarray[k+1].N;
						E0=JDarray[k+1].E;
						N2=JDarray[k+2].N;
						E2=JDarray[k+2].E;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,l1,l2,CenN,CenE);
						double TN,TE;//切点
						//求切点
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
						INTERS(&JDarray[k+1].N,&JDarray[k+1].E,N2,E2,N0,E0,TN,TE,OutN,OutE);
					}
					else
						ads_printf(L"固定后圆模式，不能更改此交点坐标\n");
				}
				//(JDMoveStyle == 4)// 切前、后两圆
				//当前交点新位置
				if (jdMoveType == QQHY)
				{
					//前切后圆
					if(k>0 && k<NJD-2)
					{
						INTERS(&JDarray[k].N, &JDarray[k].E
							, OutN, OutE, JDarray[k+1].N, JDarray[k+1].E
							,JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N, JDarray[k].E);							
					}
					else
						ads_printf(L"错误：拖动点不满足“前切后圆”的方式\n");

				}
				else if (jdMoveType == QYHQ)
				{
					//前圆后切
					if(k>0 && k<NJD-1)
					{
						INTERS(&JDarray[k].N, &JDarray[k].E
							, JDarray[k-1].N, JDarray[k-1].E, OutN, OutE 
							,JDarray[k].N, JDarray[k].E, JDarray[k+1].N, JDarray[k+1].E);	
					}
					else
						ads_printf(L"错误：拖动起终点不应选择“前圆后切”的方式\n");

				}

			}
			else   //任意拖动
			{  
				JDarray[k].E=JDarray[k].E+offset[X];//x
				JDarray[k].N=JDarray[k].N+offset[Y];//y
			}

				
		    
		}
		//////////////////818 临时，注意：以后添加代码需考虑起终点拖动的情况!!!!!!!!!!!
		else if(k >= NJD && k <= 2*NJD-3)//圆弧中点(起终点无圆弧)
		{
			k=indices[j]-(NJD-1); // 该圆弧对应的交点号
			MoveJDIndx=k;

			dd=sqrt(offset[X]*offset[X]+offset[Y]*offset[Y]);
			//圆弧半径改变
			if(dd>0.001)
			{
				double R;
				double xt,yt;
				double l0;
				double tmp;

				int track,type;
				track = 1;
				type = 5;
				struct resbuf *result ;
				result = ads_buildlist(RTSTR,L"",0);
				ads_grread(track,&type,result );
				xt = result->resval.rpoint[Y];
				yt = result->resval.rpoint[X];
				ads_relrb(result) ;
				
				calptan(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N, JDarray[k].E, JDarray[k+1].N, JDarray[k+1].E
					, xt, yt, &tmp, &R, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp);
				
				//查询技术标准中距n_o_dR最近的圆曲线半径及其缓长
				l0=SearchProximalRadius(R);

				mes.Format(L"半径%0.2f，缓长%0.2f\n", R, l0);
				ads_printf(mes);
				JDarray[k].R = R;
				JDarray[k].l1=JDarray[k].l2=l0;

				//R = get_R(R,DV,!isGJD);   818 临时 暂时没考虑设计参数
				//JDarray[k].l1 = get_l0(R,DV,!isGJD);
				//JDarray[k].l2 = get_l0(R,DV,!isGJD);
			}


			
			
				
		}
			
	}
	return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}

//查询技术标准中距n_o_dR最近的圆曲线半径及其缓长
//返回圆曲线半径n_o_dR和缓长
//拖动圆曲线中点时，函数中的技术标准为全局变量
int RHA::SearchProximalRadius(double &n_o_fR)
{
	if(m_Rs.size() > 1)//对象有技术标准参数时
	{
		if(n_o_fR >= m_Rs[0])
		{
			n_o_fR = m_Rs[0];
			return m_l0s[0];
		}
		short i=0;
		for(i = 1; i < m_Rs.size(); i++)
		{
			if(n_o_fR > m_Rs[i])
			{
				if(n_o_fR > (m_Rs[i-1] + m_Rs[i])/2)
				{
					n_o_fR = m_Rs[i-1];
					return m_l0s[i-1];
				}
				else
				{
					n_o_fR = m_Rs[i];
					return m_l0s[i];
				}
			}
		}
		i--;
		n_o_fR = m_Rs[i];
		return m_l0s[i];
	}

	return 0;
}


RHA::RHA(int NJD, JDarrayStruct jd2[] , CString pGH, CString JDName, CString  xlName, CString mdbName, CString xlShuXing)
{
	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();

	//线路名 线路属性
	m_XLName=xlName;
	if (xlShuXing == L"")
		XLShuXing=GetXLAtrri(xlName);
	else
		XLShuXing=xlShuXing;

	
	if (mdbName == L"")
		m_mdbName=GetCurProjName();
	else
		m_mdbName=mdbName;

	//当前文档号
	CCreatPM::CurDocIndx(m_iDoc);

	JDarray.clear();
	QXB.clear();
	GJDInforArray.clear();

	//交点赋值
	JDarray.resize(NJD);
	_tcscpy(JDGH, JDName);
	for(int i=0; i<NJD; i++)
		JDarray[i]=jd2[i];

	if (XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
		JDarray[0].JDXZ=RX_Start;
		for(int i=1; i<NJD-1; i++)
			JDarray[i].JDXZ=RX_One;
		JDarray[NJD-1].JDXZ=RX_End;
	}

	//修正交点坐标
	ModifyExJDXY();

	//计算线元
	IsMove=false;
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);

	//设置默认断链
	DLarray.clear();
	setDLM(0, 0, 0, L"K", L"K", 0.0, 0);
	setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH, L"K", L"K", QXB[NJD-1].KZH, 0);//线路转换 曲线表为空 array 也为空818

	//设置标志里程
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		BZLCarray.clear();
	else
		GenBZLCbyDL();
	
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	InitJBCS();
}

RHA::RHA(CArray<JDarrayStruct,JDarrayStruct> &jd2, CString pGH, CString JDName, CString xlName, CString mdbName, CString xlShuXing, BOOL isSetDrawParam)//818 最后一个参数目前只是对既有线恢复中的临时线为false
{
	if (xlName.IsEmpty() == true)
	{
		ads_printf(L"\n线路名为空");//nan
	}

	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();

	m_XLName=xlName;
	
	if (xlShuXing.IsEmpty() == TRUE)
		XLShuXing=GetXLAtrri(xlName);
	else
		XLShuXing=xlShuXing;


	if (mdbName == L"")
		m_mdbName=GetCurProjName();
	else
		m_mdbName=mdbName;

	//当前文档号
	CCreatPM::CurDocIndx(m_iDoc);

	JDarray.clear();
	QXB.clear();
	GJDInforArray.clear();
	int NJD=jd2.GetSize();
	JDarray.resize(NJD);

	_tcscpy(JDGH, JDName);
	for(int i=0; i<NJD; i++)
		JDarray[i]=jd2[i];

	if (XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
		JDarray[0].JDXZ=RX_Start;
		for(int i=1; i<NJD-1; i++)
			JDarray[i].JDXZ=RX_One;
		JDarray[NJD-1].JDXZ=RX_End;
	}

	//修正交点坐标
	ModifyExJDXY();

	//计算线元
	IsMove=false;
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);
	
	//设置断链
	DLarray.clear();
	setDLM(0, 0, 0,  L"K", L"K", 0.0, 0);
	setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH,  L"K", L"K", QXB[NJD-1].KZH, 0);//818 冠号赋值 pGH??

	//设置标志里程
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		BZLCarray.clear();
	else
		GenBZLCbyDL();

	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	if(isSetDrawParam)
		InitJBCS();
}

RHA::RHA(const vector<JDarrayStruct> &jd2, CString pGH , CString JDName, CString xlName, CString mdbName, CString xlShuXing)
{
	
	if (xlName.IsEmpty() == true)
	{
		ads_printf(L"\n线路名为空");//nan
	}

	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();

	m_XLName=xlName;

	if (xlShuXing.IsEmpty() == TRUE)
		XLShuXing=GetXLAtrri(xlName);
	else
		XLShuXing=xlShuXing;


	if (mdbName == L"")
		m_mdbName=GetCurProjName();
	else
		m_mdbName=mdbName;

	//当前文档号
	CCreatPM::CurDocIndx(m_iDoc);

	JDarray.clear();
	QXB.clear();
	GJDInforArray.clear();

	int NJD=jd2.size();
	JDarray.resize(NJD);

	_tcscpy(JDGH, JDName);
	for(int i=0; i<NJD; i++)
		JDarray[i]=jd2[i];

	if (XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
		JDarray[0].JDXZ=RX_Start;
		for(int i=1; i<NJD-1; i++)
			JDarray[i].JDXZ=RX_One;
		JDarray[NJD-1].JDXZ=RX_End;
	}

	//修正交点
	ModifyExJDXY();

	//计算线元
	IsMove=false;
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);

	//设置断链
	DLarray.clear();
	setDLM(0, 0, 0,  L"K", L"K", 0.0, 0);
	setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH,  L"K", L"K", QXB[NJD-1].KZH, 0);//818 冠号赋值 pGH??

	//设置标志里程
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		BZLCarray.clear();
	else
		GenBZLCbyDL();

	//生成贯通断链表
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	InitJBCS();
	
}


/*===================================================================
函数名:构造函数
------------------------------------------------------------
函数功能:线路转换 或 初始化二线
------------------------------------------------------------
输入:
xlName              目标线路名
shuXing             目标线路属性
pSrcRHA             源线路
xjj                 目标线路与源线路之间的线间距，左- 右+
------------------------------------------------------------
输出: 新的平面线路 RHA
------------------------------------------------------------
算法描述:
1 对于设计线和既有线之间的转换和初始化，如果在时态上源线路在前目标线路在后
，那么目标线路完全参照原线路，反之则需要重新解算目标线路的参照关系
------------------------------------------------------------
编写: 李帅 2016.7.24  修改 2017.6.1
------------------------------------------------------------
备注：
=====================================================================*/
RHA::RHA(CString xlName, CString shuXing, const RHA *pSrcRHA, double xjj)
{
	if (pSrcRHA->JDarray.size()<2)
	{
		ads_printf(L"源线路的交点数小于2！\n");
		return;
	}

	if ((shuXing == LSX || shuXing == SGBX) && shuXing == pSrcRHA->XLShuXing)
	{
		ads_printf(L"暂时不支持临时线转临时线，施工便线转施工便线！\n");
		return;
	}

	xlName.MakeUpper();
	xlName.Trim();

	m_XLName=xlName;
	XLShuXing=shuXing;
	m_mdbName=pSrcRHA->m_mdbName;
	IsMove=false;

	//当前文档号
	CCreatPM::CurDocIndx(m_iDoc);

	JDarray.clear();
	QXB.clear();
	GJDInforArray.clear();
	int NJD=pSrcRHA->JDarray.size();
	JDarray.resize(NJD);

	if( fabs( xjj)< 0.001 )
		_tcscpy( JDGH , L"JD" );
	else if( xjj > 0.001 )
		_tcscpy( JDGH , L"YJD" );
	else 
		_tcscpy( JDGH , L"ZJD" );

	
	CString str;
	for(int i=0; i<NJD; i++)
	{
		JDarray[i].JDXZ=RX_One;
		_tcscpy(JDarray[i].XLName, m_XLName);
		_tcscpy(JDarray[i].BLName, L"");
		_tcscpy(JDarray[i].BLJDName, L"");
		_tcscpy(JDarray[i].Draw_JDName, L"");
		_tcscpy(JDarray[i].ZHLC, L"");
		JDarray[i].XJJ=0.0;
		JDarray[i].N=0.0;
		JDarray[i].E=0.0;
		JDarray[i].R=pSrcRHA->JDarray[i].R;
		JDarray[i].l1=pSrcRHA->JDarray[i].l1;
		JDarray[i].l2=pSrcRHA->JDarray[i].l2;

		str = pSrcRHA->JDarray[i].JDName;
		str.Replace(pSrcRHA->JDGH, L"");
		_stprintf( JDarray[i].JDName , L"%s%s" ,JDGH ,str);
	}

	JDarray[0].JDXZ=RX_Start;
	JDarray[JDarray.size()-1].JDXZ=RX_End;
	 

	//按线间距偏移后求得交点坐标，即JDarray的N/E
	GetJDXYwhenMoveXLbyXJJ(pSrcRHA, xjj);

	if (shuXing != SGBX && shuXing != LSX
		&& pSrcRHA->XLShuXing != SGBX && pSrcRHA->XLShuXing != LSX)
	{//对于设计线和既有线之间的转换和初始化

		//判断时态先后
		int valu;
		valu=CmpShuXing(pSrcRHA->XLShuXing, shuXing);

		if (valu<=0)//时态上，目标线路和源线路相同或目标线路在后
		{//目标线路可以完全参照源线路，填写参照信息
			for(int i=0; i<NJD; i++)
			{
				JDarray[i].JDXZ=BXPX;
				_tcscpy(JDarray[i].BLName, pSrcRHA->m_XLName);
				_tcscpy(JDarray[i].BLJDName, pSrcRHA->JDarray[i].JDName);
				JDarray[i].XJJ=xjj;
			}

			//同心圆的半径计算
			//if (fabs(xjj)>0.0001)
			//{
			//	for(int i=1; i+1<NJD; i++)
			//	{
			//		if (pSrcRHA->QXB[i].LorR*xjj>0)//内侧
			//			JDarray[i].R -= fabs(xjj);
			//		else						   //外侧
			//			JDarray[i].R += fabs(xjj);
			//	}
			//}
			//同心圆的半径计算
			if (fabs(xjj)>0.0001)
			{
				for(int i=1; i+1<NJD; i++)
				{
					CalOneBXJDdata(i, pSrcRHA);
				}
			}
			

			//计算线元
			getpxy_DATA();

			//生成断链
			GenDL();

			//生成标志里程
			if(XLShuXing != SJJX && XLShuXing != SJ2X)
				GenBZLCbyDL(false);
			else
				BZLCarray.clear();
		}
		else
		{
			//自动计算参照关系，赋值交点断链信息
			UpdateAllData();
		}
	}
	else 
	{
		//保存源线路pSrcRHA的断链等数据
		//，这样在更新数据时会尽可能保留断链等数据
		if (fabs(xjj)<0.0001)
		{
			OldJDarray.clear();
			OldDLarray.clear();
			OldBZLCarray.clear();

			int NDL=pSrcRHA->DLarray.size();
			OldJDarray.resize(NJD);
			OldDLarray.resize(NDL);

			for (int i=0;i<NJD;i++)
				OldJDarray[i]=pSrcRHA->JDarray[i];

			for (int i=0;i<NDL;i++)
				OldDLarray[i]=pSrcRHA->DLarray[i];

			for (int i=0;i<pSrcRHA->BZLCarray.size();i++)
			{
				if (pSrcRHA->BZLCarray[i].isAutoAdd == false)
					OldBZLCarray.push_back(pSrcRHA->BZLCarray[i]);
			}
		}
		
		//更新数据，自动计算参照关系，赋值交点断链信息
		UpdateAllData();
	}

	//GetT0min_Lrmin(); 818 两函数
	//RTolCheckUp( IsGaiJian );
	
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	InitJBCS();
}


/*函数作用：由交点数据(JDarray)生成曲线表(QXB)、线元数组(array)、线路长(xl_length)
算法：1 形成临时线元数组array_pxy
	  2 由临时线元数组形成线元数组array
*/
Adesk::Boolean  RHA::getpxy_DATA(int moveJDix)
{   // 导线法由导线数据文件求线位 L.SC.S.L.S.C.S.L...  得到线元文件

	double fb, fe, aa;//f-方位角, aa转向角，b-begin，e-end
	double db,de/*,lcb,lce*/;//d-距离，lc-里程
	double T0=0.0;
	ACHAR zy[2], GH[20];//左右，冠号
	double BPMTL[10], T1, T2, L;//曲线要素
	//double DDlength;
	int i=0 ,j=0, k=0;
	int color=2,iq=2,c;
	int JdNum=0, localJdNum=0;//交点数,四倍交点数
	int effectSJD, effectEJD;//影响线元计算的起终交点
	bool isCalAll=false;//是否计算所有交点

	if (!IsMove || (IsMove && IsMoveDone))
		isCalAll=true;

	for (i=0;i<10;i++) 
		BPMTL[i]=0.0;

	//给线元数组array[][10]、array_pxy[][4][5] 开空间、赋值
	
	JdNum=JDarray.size();

	effectSJD=0;
	effectEJD=JdNum-1;
	if (!isCalAll && moveJDix>-1)
	{
		//只计算array_pxy的几个线元
		effectSJD=moveJDix-2;
		effectEJD=moveJDix+2;

		if (effectSJD<0)
			effectSJD=0;

		if(effectEJD > JdNum-1)
			effectEJD=JdNum-1;

		localJdNum=effectEJD-effectSJD+1;
	}
	else
	{
		//全部计算
		array.clear();

		array.resize(JdNum*4);
		for (i=0;i<JdNum*4;i++)
		{
			array[i].resize(10);
			for (int j=0;j<10;j++)
				array[i][j]=0.0;
		}
		//if (array)
		//{
		//	if (m_oldNJD>0)
		//	{
		//		for (i=0;i<m_oldNJD*4;i++)//oriJdNum为前一个图形的交点数
		//		{
		//			delete[] array[i];//818 撤销有问题，会挂掉
		//		}
		//		delete[] array;
		//	}
		//	else
		//		ads_printf(L"m_oldNJD没赋值！\n");
		//	array=NULL;
		//}

		/*array=new double*[JdNum*4];
		for (i=0;i<JdNum*4;i++)
		{
			array[i]=new double[10];
			for (j=0;j<10;j++)
				array[i][j]=0;
		}*/

		//m_oldNJD=JdNum;

		localJdNum=JdNum;
	}
		
	array_pxy.clear();
	array_pxy.resize(20*localJdNum);

	for (i=0;i<array_pxy.size();i++)
		array_pxy[i]=0.0;
		
	//计算回头曲线
	CalHuitouQX();

	//起点赋值（起终点半径应为0）	

	//起点
	//前一段交点连线的长度
	db=xyddaa(JDarray[effectSJD].N,JDarray[effectSJD].E,JDarray[effectSJD+1].N,JDarray[effectSJD+1].E,&fb);//前一段交点连线的长度
	T0=0.0;//前一段交点后切线长

	array_pxy[0]=10.0;// 起点属性
	array_pxy[1]=JDarray[effectSJD].N; //坐标
	array_pxy[2]=JDarray[effectSJD].E; //坐标
	array_pxy[3]=dms_rad(fb);  //方位角 FA
	array_pxy[4]=0.0/*JDarray[effectSJD].l1*/; //里程 LC


	if (isCalAll)
	{
		QXB.clear();
		QXB.resize(JdNum);//818 QXB resize()开空间失败 
		QXB[0].l1 = JDarray[0].l1;
		QXB[0].R = JDarray[0].R;
		QXB[0].l2 = JDarray[0].l2;
		_tcscpy(QXB[0].JDName , JDarray[0].JDName);
		QXB[0].IsGJ = JDarray[0].IsGJ;
		QXB[0].KZH=0.0;  // 起点里程
		QXB[0].KHY=QXB[0].KZH;  
		QXB[0].KYH=QXB[0].KZH;
		QXB[0].KHZ=QXB[0].KZH;
		QXB[0].a = QXB[0].T1 = QXB[0].T2 = QXB[0].L ;
		QXB[0].LorR = 0;

	}

	if (JdNum==2) // 仅有两个交点,即一条直线时
	{  
		array_pxy[1*20]=1.0; //
		array_pxy[1*20+1]=db;  
	};

	//DDlength=QXB[0].KZH;// 起点里程

	bool PreLinK,AftLink;
	ACHAR jdname[50];
	double RB, RE, R;
	double Lo1, Lo2;
	int qorh;
	double tt;
	double cirLen, lineLen, rotAng;//
	int num;
	
	num=0;
	for (i=effectSJD+1; i<effectEJD;i++)
	{  
		num++;

		de=xyddaa(JDarray[i].N,JDarray[i].E,JDarray[i+1].N,JDarray[i+1].E,&fe);
		aa = ZXJ(fe,fb);//转角

		if (aa>0) 
		{  c=1; _tcscpy(zy,L"y"); }  //右偏
		else
		{ c=-1;_tcscpy(zy,L"z"); } //左偏

		aa=fabs(aa);
		aa=dms_rad(aa);

		//计算切线长
		getBPMTL(aa,JDarray[i].R,JDarray[i].l1,JDarray[i].l2,&BPMTL[0]); 
		T1=BPMTL[6];
		T2=BPMTL[7];
		L=BPMTL[8];  //曲线长

		PreLinK=FALSE,AftLink=FALSE;
		_tcscpy(jdname,JDarray[i].JDName);
		_tcscpy(jdname,JDarray[i-1].JDName);
		_tcscpy(jdname,JDarray[i+1].JDName);
		if(_tcscmp(JDarray[i].JDName,JDarray[i-1].JDName)==0)
			PreLinK=TRUE;
		if(_tcscmp(JDarray[i].JDName,JDarray[i+1].JDName)==0)
			AftLink=TRUE;
		RB=1e50,RE=1e50,R=0.0;

		Lo1=JDarray[i].l1;
		Lo2=JDarray[i].l2;
		qorh=0;
		tt=0.0;

		if(PreLinK || AftLink)
		{
			if( PreLinK)
			{
				if (JDarray[i].l1>1e-6)
				{
					RB = fabs(JDarray[i-1].R);
					qorh=-1;
					tt = db-T0;
				}
				else
					RB = 1e50;
				R= fabs(JDarray[i].R);
				get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);

			}

			if(AftLink  )
			{
				if (JDarray[i].l2>1e-6)
				{
					RE = fabs(JDarray[i+1].R);
					qorh=1;

					double at1,at2;
					CalJDT1T2(i+1,at1,at2,JDarray);
					tt = de-at1;
				}
				else
					RE = 1e50;
				R= fabs(JDarray[i].R);
				get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
				double TempT1,TempT2,Tempfe,Tempaa,TempL;
				xyddaa(JDarray[i+1].N,JDarray[i+1].E,JDarray[i+2].N,JDarray[i+2].E,&Tempfe);
				Tempaa = ZXJ(fe,Tempfe);
				get_T_Len(dms_rad(Tempaa),JDarray[i+1].R,JDarray[i+1].R,0,1e50,JDarray[i+1].l2,TempT1,TempT2,TempL);

				double A1=0.0,A2=0.0;
				if(fabs(TempT1+T2-de)>PRECISION)//需要调整缓长
				{
					double lo=Adjust_L_JDLuan(aa,R,RB,Lo1,RE,Lo2,tt,qorh);

					if(lo > 0.0)//有解
					{
						Lo2 = lo;
						get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
						A2=sqrt(RE*R/fabs(RE-R)*Lo2);
						JDarray[i].l2 = Lo2;

					}
				}

			}
		}

		//考虑最后一段缓和曲线为不完整缓和曲线的情况
		if(i==JdNum-2)
		{
			if(JDarray[i+1].R>0.001)
			{
				RE = fabs(JDarray[i+1].R);
				Lo2 = JDarray[i].l2;

				get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
			}
		}

		if ( L<(JDarray[i].l1+JDarray[i].l2))
		{
			ads_printf(L"交点%s号曲线 前后缓和曲线重叠：缓长L1+L2=%10.3f>大于曲线长L=%10.3f，缓长调整为0\n",JDarray[i].JDName,JDarray[i].l1+JDarray[i].l2,L);
			//	return 0;
			//曲线穿袖子，自动调整缓长为0再计算。 ls 2016
			JDarray[i].l1=0.0;
			JDarray[i].l2=0.0;
			getBPMTL(aa,JDarray[i].R,JDarray[i].l1,JDarray[i].l2,&BPMTL[0]); 
			T1=BPMTL[6];
			T2=BPMTL[7];
			L=BPMTL[8];  //曲线长

		}
		else if((L-JDarray[i].l1-JDarray[i].l2)<20.0 && fabs(JDarray[i].R)>0.001)
		{ 
			//ads_printf(L"\n %d号曲线的(R=%8.1f)圆曲线长L=%10.3f 不足20m ",i,JDarray[i].R,L-JDarray[i].l1-JDarray[i].l2);
			// return 0;
		};

		rotAng=aa;	
		//  line
		lineLen=db-T1-T0;
		array_pxy[num*20]=1.0;
		array_pxy[num*20+1]=lineLen;

		if( lineLen<20.0) 
		{  
			if( lineLen<0) 
				ads_printf(L" %s超过前一曲线HZ/YZ不合法\n",JDarray[i].JDName);
		};

		//A
		if(RB>1.0e6)
		{
			aa=sqrt(JDarray[i].l1*fabs(JDarray[i].R));
			if ( aa>0.000000001) 
			{ 
				array_pxy[num*20+1*5]=3.0; 
				array_pxy[num*20+1*5+1]=aa; //A
				array_pxy[num*20+1*5+2]=fabs(JDarray[i].R);  //R
				array_pxy[num*20+1*5+3]=0.0;
				array_pxy[num*20+1*5+4]=c;  //z y pian
			}

		}
		else 
		{

			double Re=0.0;
			if(fabs(fabs(JDarray[i].R)-RB)>1.0e-6)
				Re = (fabs(JDarray[i].R)*RB)/fabs(fabs(JDarray[i].R)-RB);
			else Re=0.0;
			aa=sqrt(JDarray[i].l1*Re);
			if ( aa>0.000000001) 
			{
				if(fabs(JDarray[i].R)>RB)
				{
					array_pxy[num*20+1*5]=6.0;
					array_pxy[num*20+1*5+2]=RB;  //R
					array_pxy[num*20+1*5+3]=fabs(JDarray[i].R); 
				}
				else 
				{
					array_pxy[num*20+1*5]=5.0;
					array_pxy[num*20+1*5+2]=fabs(JDarray[i].R);  //R
					array_pxy[num*20+1*5+3]=RB; 
				}  
				array_pxy[num*20+1*5+2]=RB;  //R
				array_pxy[num*20+1*5+3]=fabs(JDarray[i].R); 
				array_pxy[num*20+1*5+1]=aa; //A

				array_pxy[num*20+1*5+4]=c;  //z y pian
			};

		}
		

		//  circle

		//aa 园曲线长
		aa=L-JDarray[i].l1-JDarray[i].l2;
		cirLen=aa;

		if ( aa>0.000000001) 
		{ 
			array_pxy[num*20+2*5]=2.0;  //性质
			array_pxy[num*20+2*5+1]=fabs(JDarray[i].R); //R
			array_pxy[num*20+2*5+2]=aa; //yuan  chang 
			array_pxy[num*20+2*5+3]=0.0;
			array_pxy[num*20+2*5+4]=c;
		}
		else
		{
			array_pxy[num*20+2*5]=2.0;  //性质
			array_pxy[num*20+2*5+1]=fabs(JDarray[i].R); //R
			array_pxy[num*20+2*5+2]=aa; //yuan  chang 
			array_pxy[num*20+2*5+3]=fe ;

			array_pxy[num*20+2*5+4]=c;

			//三赋数值

		}
		

		//2-spiral

		if(RE>1.0e6)
		{
			aa=sqrt(JDarray[i].l2*fabs(JDarray[i].R));
			if ( aa>0.000000001)
			{
				//
				array_pxy[num*20+3*5]=4.0; 
				array_pxy[num*20+3*5+1]=aa;
				array_pxy[num*20+3*5+2]=fabs(JDarray[i].R);
				array_pxy[num*20+3*5+3]=0.0;
				array_pxy[num*20+3*5+4]=c;
			}; 

		}
		else
		{
			double Re=0.0;
			if(fabs(fabs(JDarray[i].R)-RE)>1.0e-6)
				Re = (fabs(JDarray[i].R)*RE)/fabs(fabs(JDarray[i].R)-RE);
			else 
				Re=0.0;
			aa=sqrt(JDarray[i].l2*Re);
			if ( aa>0.000000001) 
			{
				if(fabs(JDarray[i].R)>RE)
				{
					array_pxy[num*20+3*5]=5.0;
					array_pxy[num*20+3*5+2]=fabs(JDarray[i].R);  //R
					array_pxy[num*20+3*5+3]=RE;
				}
				else
				{
					array_pxy[num*20+3*5]=6.0;
					array_pxy[num*20+3*5+2]=RE;  //R
					array_pxy[num*20+3*5+3]=fabs(JDarray[i].R);
				}  
				array_pxy[num*20+3*5+1]=aa; //A
				array_pxy[num*20+3*5+2]=fabs(JDarray[i].R);  //R
				array_pxy[num*20+3*5+3]=RE;			 
				array_pxy[num*20+3*5+4]=c;  //z y pian
			};
		}

		if (isCalAll)
		{
			//曲线要素 角度为度.分秒
			QXB[i].a=rotAng;
			QXB[i].T1=T1;
			QXB[i].T2=T2;
			QXB[i].L=L;
			QXB[i].LorR = c;

			//ZH (zy)
			QXB[i].KZH=QXB[i-1].KHZ+lineLen;

			// HY
			//HY里程
			QXB[i].KHY=QXB[i].KZH + JDarray[i].l1;
			// YH
			QXB[i].KYH = QXB[i].KHY + cirLen;
			// HZ
			QXB[i].KHZ = QXB[i].KYH + JDarray[i].l2;
			QXB[i].l1 = JDarray[i].l1;
			QXB[i].R = JDarray[i].R;
			QXB[i].l2 = JDarray[i].l2;
			_tcscpy(QXB[i].JDName , JDarray[i].JDName);
			QXB[i].IsGJ = JDarray[i].IsGJ;
		}
	
		db=de;
		fb=fe;
		T0=T2;

	};


	if(effectEJD == localJdNum-1)
	{
		//  line
		db=db-T0 ;  //终点长度
		if ( db>0.0000000001) 
		{
			array_pxy[(localJdNum-1)*20]=1.0;	 
			array_pxy[(localJdNum-1)*20+1]=db;
		};

	

	}

	// 终点里程
	if (isCalAll)
	{
		QXB[JdNum-1].l1 = JDarray[JdNum-1].l1;
		QXB[JdNum-1].R = JDarray[JdNum-1].R;
		QXB[JdNum-1].l2 = JDarray[JdNum-1].l2;
		_tcscpy(QXB[JdNum-1].JDName, JDarray[JdNum-1].JDName);

		QXB[JdNum-1].IsGJ = JDarray[JdNum-1].IsGJ;

		QXB[JdNum-1].KZH = QXB[JdNum-2].KHZ + db;  
		//xl_length = QXB[JdNum-1].KZH - QXB[0].KZH;    
		QXB[JdNum-1].KHY=QXB[JdNum-1].KZH;
		QXB[JdNum-1].KYH=QXB[JdNum-1].KZH;
		QXB[JdNum-1].KHZ=QXB[JdNum-1].KZH;
		QXB[JdNum-1].a=QXB[JdNum-1].T1 = QXB[JdNum-1].T2 = QXB[JdNum-1].L=QXB[JdNum-1].LorR = 0.0;
	}




	if (!IsMove || (IsMove && IsMoveDone))
	{
		
		//  起点
		double INDATA[5],l,L0;
		xlpoint PB0,PEZ;
		for (j=0;j<5;j++)
			array[0][j]=array_pxy[j]; 
		tarray = 1;


		//生成2维 线元数组 10
		for( i=1;i<JdNum-1;i++) 
		{ 
			for (k=0;k<4;k++) 
			{		   
				for (j=0;j<5;j++) 
					INDATA[j]=array_pxy[i*20+k*5+j];

				j=(int)(INDATA[0]+0.01);
				l=0.0;
				//各个线元长度
				if (j==1) l=INDATA[1];//直线
				if (j==2) l=INDATA[2]; //
				if (j==3||j==4)   
				{
					if(fabs(INDATA[2])>0.000001)  //R
						l=INDATA[1]*INDATA[1]/INDATA[2];  //l
					else
						l=0.0;

				}
				if (j==5)
				{
					if(fabs(INDATA[2]*INDATA[3])>0.000001)
						l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
					else
						l=0.0;

				}
				if (j==6)
				{
					if(fabs(INDATA[2]*INDATA[3])>0.000001)
						l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);
					else
						l=0.0;
				}
				//暂时不要

				// 求L0=？
				L0=l;  //线元长度 
				if (j==2) L0=INDATA[1]; //R
				//		   ads_printf(L"l=%lf\n",l);


				if (L0>0.001)
				{
					for (int kk=0;kk<5;kk++) 
						array[tarray][kk]=array_pxy[i*20+k*5+kk]; 

					if(j==1)//直线
					{
						db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
						array[tarray][5]=fb ;
						array[tarray][6]=QXB[i-1].KHZ; //HZ  线元起点里程
						array[tarray][7]=1e40;  //R
						array[tarray][8]=JDarray[i-1].N+QXB[i-1].T2*cos(fb);  //起点坐标
						array[tarray][9]=JDarray[i-1].E+QXB[i-1].T2*sin(fb);
					}


					// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array[trow][0],array[trow][1],array[trow][2],array[trow][4]);
					tarray=tarray+1;  
				}

			};
		};
		//  终点 线元

		for (j=0;j<5;j++)
			array[tarray][j]=array_pxy[(JdNum-1)*20+j]; 	
		if(JdNum>2)
		{
			i=JdNum-1;  //终点线元  
			db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
			array[tarray][5]=fb;   
			array[tarray][6]=QXB[i-1].KHZ;  //zhong dian licheng  HZ
			array[tarray][7]=1e40;
			array[tarray][8]=JDarray[i-1].N+QXB[i-1].T2*cos(fb);
			array[tarray][9]=JDarray[i-1].E+QXB[i-1].T2*sin(fb);
		}	

		/*
		//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大

		//(R1>R2)/////////////////////////////////////////////////////////////////////////
		//1:			L		R    A         A              A                A        //
		//2:					L	 R         R              R1               R2       //
		//3:                                                  R2               R1       //
		//////////////////////////////////////////////////////////////////////////////////
		//4: 左右转
		//5: 起始切线方位角
		//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
		//7: 半径 R
		//8，9：线元起点坐标 N,E
		//最后一个线元为终点
		*/

		tarray=tarray+1;
		i = 0;
		PB0.lc=array[0][4];  // 起点
		PB0.x =array[0][1];
		PB0.y =array[0][2];   
		PB0.a =rad(array[0][3]);
		PB0.r =1e40;
		array[1][5] = PB0.a ;
		array[1][6] = PB0.lc;
		array[1][7] = PB0.r ;
		array[1][8] = PB0.x ;
		array[1][9] = PB0.y ;
		for (i = 1;i<tarray;i++)
		{ 
			// 第 i 线元
			for (k=0;k<5;k++) INDATA[k]=array[i][k];

			j=(int)(array[i+1][0]+0.0001);  //** i+1
			if(j==1)//下一单元是直线   
			{
				PB0.a=array[i+1][5];
				PB0.lc=array[i+1][6];
				PB0.r=array[i+1][7];
				PB0.x=array[i+1][8];
				PB0.y=array[i+1][9];
			}
			else
			{
				l=Get_INDATA_L(INDATA,&j);  // 计算线元长度
				xlzbxy(PB0,PEZ,l,INDATA,0);   // 计算坐标   //***********//

				array[i+1][5] = PEZ.a ;  //线元起点方位角

				//圆+线路长度为0
				//	if( fabs(array_pxy[i][k][0]-2)<0.00001 && l<0.00000001 ) array[i+1][5] = INDATA[3];



				array[i+1][6] = PEZ.lc;  // 求线元 终点坐标
				array[i+1][7] = PEZ.r ;//线元起点的半径,直圆\圆直为无穷大
				array[i+1][8] = PEZ.x ;
				array[i+1][9] = PEZ.y ;
				PB0=PEZ;   //坐标
			}
		};

		xl_length = QXB[JDarray.size()-1].KZH - QXB[0].KZH; 



		//形成夹直线数组 ls 2016.7.30
		//818 是否施工便线和临时线不需计算夹直线，需考虑临时线转设计线

		if(XLShuXing != LSX && XLShuXing != SGBX)
		{
			lsJZXarray.RemoveAll();
			lsJZXarray.SetSize(QXB.size()-1);
			for (int j=0; j<QXB.size()-1;j++)
			{
				PB0.lc=QXB[j].KHZ;
				xlpoint_pz(array, tarray, PB0);
				lsJZXarray[j].SPt.x=PB0.x;
				lsJZXarray[j].SPt.y=PB0.y;
				PB0.lc=QXB[j+1].KZH;
				xlpoint_pz(array, tarray, PB0);
				lsJZXarray[j].EPt.x=PB0.x;
				lsJZXarray[j].EPt.y=PB0.y;
			}

		}
		
	}
	

	return 1;
}

void RHA::setDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH,double TYLC, Int16 RXDIndx, ACHAR *RXDName, ACHAR *BLName)
{ 
	if (DLarray.size()<iDL)
	{
		ads_printf(L"插入位置有误，添加断链失败\n");
		return;
	}

	DLTab tmpDL;
	_tcscpy(tmpDL.XLName, m_XLName);
	_tcscpy(tmpDL.BGH,BGH);	
	_tcscpy(tmpDL.EGH,EGH);
	tmpDL.BLC=BLC;        	
	tmpDL.ELC=ELC;
	tmpDL.TYLC=TYLC;

	if (BLName == NULL)
		_tcscpy(tmpDL.BLName, L"");
	else
		_tcscpy(tmpDL.BLName, BLName);
	if (RXDName == NULL)
		_tcscpy(tmpDL.RXDName, L"");
	else
		_tcscpy(tmpDL.RXDName, RXDName);
	tmpDL.Indx=RXDIndx;

	if (DLarray.size()>iDL)
		DLarray.insert(DLarray.begin()+iDL, tmpDL);
	else if (DLarray.size() == iDL)
		DLarray.push_back(tmpDL);	
}

//void RHA::AddDL(double BLC,double ELC, ACHAR *BGH,ACHAR *EGH,double TYLC, Int16 RXDIndx, ACHAR *RXDName, ACHAR *BLName)
//{   
//	DLTab DLtmp;
//	_tcscpy(DLtmp.XLName, m_XLName);
//	_tcscpy(DLtmp.BGH,BGH);	
//	_tcscpy(DLtmp.EGH,EGH);
//	DLtmp.BLC=BLC;        	
//	DLtmp.ELC=ELC;
//	DLtmp.TYLC=TYLC;
//
//	if (BLName == NULL)
//		_tcscpy(DLtmp.BLName, L"");
//	else
//		_tcscpy(DLtmp.BLName, BLName);
//	if (RXDName == NULL)
//		_tcscpy(DLtmp.RXDName, L"");
//	else
//		_tcscpy(DLtmp.RXDName, RXDName);
//	DLtmp.Indx=RXDIndx;
//	
//	DLarray.push_back(DLtmp);
//}
//

/*===================================================================
函数名:TrsCkmltoProjml
------------------------------------------------------------
函数功能:现场里程转投影里程（几何）
------------------------------------------------------------
输入:
ckml                   现场里程
projml                 投影里程
------------------------------------------------------------
输出:计算出连续里程返回true，反之返回false
------------------------------------------------------------
算法描述:遍历所有的线路断链表，判断该里程在哪条线路（一线）的绕行段上，如果在，
则根据断链计算该点在一线上的连续里程，并由投影关系计算二线上的投影里程
------------------------------------------------------------
编写: 李帅 2016.4.8  
------------------------------------------------------------
备注：1 调用该函数前，应该根据断链点坐标判断断链是否存在，更新所有断链数据
2 注意：调用该函数前需要确保线路都生成了  818
=====================================================================*/
bool RHA::TrsCkmltoProjml(const ACHAR ckml[], double& projml) const
{
	double cml;
	Int16 result=0;//结果标记 =0：没找到，=1：找到

	projml=0.0;
	if(XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
	//一线
		//统一里程
		cml=TYLC1(ckml, DLarray);
		projml=CalcmlTonewcml(cml);
		result=1;
	}
	else
	{
	//二线
		double XLC;//现场里程
		ACHAR str[16];
		CString GH, bfEGH;//该现场里程的冠号, 该里程的前一个断链断后冠号
		int dlNum; //断链数
		double dlCml;//断链的连续里程
		double cml1x;//在一线上的投影里程
		Int16 mark=0;//标记
		CString mes;


		split_ckml(ckml, &XLC, str);
		GH=str;
		GH.MakeUpper();


		//遍历所有可能的线路断链表（可能的解释，例如：既有二线不可能参照设计基线）
		Int16 xlnum=g_DocManageAry[m_iDoc].XLAry.size();
		RHA *pRHA=NULL;

		for (int i=0; i<xlnum; i++)//
		{
			pRHA=g_DocManageAry[m_iDoc].XLAry[i];//pRHA分别取JYJX JY2X SJ2X

			//比较两条线的线路属性，本线路的时态必须不在pRHA之前，才能在pRHA的绕行段找里程
			if (CmpShuXing(pRHA->XLShuXing, XLShuXing)>0)
				continue;

			if(pRHA->m_mdbName != m_mdbName	|| pRHA->DLarray.size() == 0)//还没有断链
				continue;

			dlNum=pRHA->DLarray.size();

			for (int j=0; j<dlNum-1; j++)
			{
				bfEGH=pRHA->DLarray[j].EGH;
				bfEGH.MakeUpper();
				//2 在相邻断链点之间的段落内
				if (XLC>pRHA->DLarray[j].ELC-0.001 
					&& XLC<pRHA->DLarray[j+1].BLC+0.001 
					&& GH == bfEGH)
				{
					//3 判断是否在绕行段内
					if (pRHA->DLarray[j].Indx>-1 
						&& pRHA->DLarray[j].Indx 
						== pRHA->DLarray[j+1].Indx) 
					{
						mark++;//每在一个断链中存在，mark加一，超过一，便存在于两个以上的断链
						if (mark >1)
						{
							mes.Format(L"错误：现场里程%s存在于多个不同的绕行段上\n", ckml);
							ads_printf(mes);
							//return false;
						}

						//前一断链在一线上的统一里程
						dlCml=TYLC1(pRHA->DLarray[j].BGH, pRHA->DLarray[j].BLC, pRHA->DLarray);

						//4 计算投影里程（垂直于一线）
						if(pRHA->m_XLName == m_XLName)
						{//不参照其它线
							
							if (XLShuXing == JYJX || XLShuXing == JY2X)
							{//既有线
								cml1x = dlCml + XLC - DLarray[j].ELC;//该点的统一里程
								projml=CalcmlTonewcml(cml1x);//投影里程
							}
							else//设计线
								projml=DLarray[j].TYLC + XLC - DLarray[j].ELC;

							result++;
						}
						else
						{//参照其它线

							//计算在一线上的投影里程
							double projml1x=0.0;
							if (pRHA->XLShuXing == JYJX 
								|| pRHA->XLShuXing == JY2X)
							{//既有线
								cml1x = dlCml + XLC - pRHA->DLarray[j].ELC;
								projml1x=pRHA->CalcmlTonewcml(cml1x);
							}
							else//设计线
								projml1x=pRHA->DLarray[j].TYLC + XLC 
								- pRHA->DLarray[j].ELC;

							//一线反投影到二线，计算二线上的投影里程
							if(CalXJJ(projml1x, pRHA->array
								, pRHA->tarray, array
								, tarray, projml, true))
								result++;
						}

					}

				}
			}		
		}
	}

	if (result>0)
		return true;
	else
		return false;
}

//现场里程转投影里程
bool RHA::TrsCkmltoProjml(const ACHAR gh[], double dml, double& projml) const
{
	ACHAR ckml[50];

	_tcscpy(ckml, LCchr(gh, dml, 6));
	return TrsCkmltoProjml(ckml, projml);
}

/*===================================================================
函数名:TrsCkmltoCml
------------------------------------------------------------
函数功能:现场里程转连续里程
------------------------------------------------------------
输入:
ckml              现场里程
cml               连续里程
------------------------------------------------------------
输出:计算出连续里程返回true，反之返回false
------------------------------------------------------------
算法描述:1 一线: 由断链表求出统一里程
         2 二线: 遍历所有的线路断链表，判断该里程在哪条线路（一线）的绕行段上，如果在，
           就根据断链计算该点在一线上的连续里程，并由投影关系计算二线上的连续里程
------------------------------------------------------------
编写: 李帅 2016.7.19  
------------------------------------------------------------
备注：调用该函数前，应该根据断链点坐标判断断链是否存在，更新所有断链数据
=====================================================================*/
void RHA::TrsCkmltoCml(ACHAR ckml[], double &cml) const 
{
	cml=TYLC1(ckml, DLarray);//统一改为如此 818

	//Int16 result=0;//结果标记 =0：没找到，=1：找到

	//if(XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	//{
	//	//一线
	//	//统一里程
	//	cml=TYLC1(ckml, DLarray);//统一改为如此 818 
	//	result=1;
	//}
	//else
	//{
	//	//二线
	//	ACHAR str[16];
	//	CString GH, bfEGH;//该现场里程的冠号, 该里程的前一个断链断后冠号
	//	int DLNum; //断链数
	//	double cml1x;//在一线上的连续里程
	//	Int16 mark=0;//标记
	//	double  XLC=0;//现场里程

	//	split_ckml(ckml, &XLC, str);
	//	GH=str;
	//	GH.MakeUpper();
	//	
	//	//1 遍历所有的线路断链表
	//	Int16 xlnum=g_DocManageAry[m_iDoc].XLAry.size();
	//	for (int i=0; i<xlnum/*g_XLNameAry.GetSize()*/; i++)
	//	{
	//		if(g_DocManageAry[m_iDoc].XLAry[i]->m_mdbName != m_mdbName
	//			|| g_DocManageAry[m_iDoc].XLAry[i]->DLarray.size() == 0)//还没有断链
	//			continue;

	//		DLNum=g_DocManageAry[m_iDoc].XLAry[i]->DLarray.size();

	//		for (int j=0; j<DLNum-1; j++)
	//		{
	//			bfEGH=g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].EGH;
	//			bfEGH.MakeUpper();
	//			//2 在相邻断链点之间的段落内    818 判断g_DocManageAry[m_iDoc].XLAry[i]的数据库是否是同一数据库
	//			if (XLC>g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].ELC-0.001 && XLC<g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j+1].BLC+0.001 
	//				&& GH == bfEGH)
	//			{
	//				//3 判断是否在绕行段内。
	//				if (g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].Indx>-1 
	//					&& g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].Indx == g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j+1].Indx) /*GetRXDIndx(j, g_DocManageAry[m_iDoc].XLAry[i]->DLarray) >= 0*/
	//				{
	//					//在绕行段
	//					mark++;
	//					if (mark >1)
	//					{
	//						//ads_printf(L"错误：该现场里程存在与两个不同的绕行段上\n"); 818
	//						return false;
	//					}

	//					cml1x = g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].TYLC + XLC - g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].ELC;
	//					//4 计算投影里程（垂直于一线）
	//					if(g_DocManageAry[m_iDoc].XLAry[i]->m_XLName == m_XLName)//不参照其它线
	//					{
	//						cml=cml1x;
	//						result++;
	//					}
	//					else//参照其它线
	//					{
	//						double projml1x;//在一线上的投影里程
	//						double projml;//在二线上的投影里程
	//						projml1x=g_DocManageAry[m_iDoc].XLAry[i]->CalcmlTonewcml(cml1x);
	//						if(CalXJJ(projml1x, g_DocManageAry[m_iDoc].XLAry[i]->array, g_DocManageAry[m_iDoc].XLAry[i]->tarray, array
	//							, tarray, projml, true))
	//						{
	//							cml=CalnewcmlTocml(projml);
	//							result++;
	//						}
	//					}

	//				}

	//			}
	//		}		
	//	}

	//}



	//if (result>0)
	//	return true;
	//else
	//	return false;
}


void RHA::TrsCkmltoCml(const ACHAR gh[], const double xlc, double &cml) const 
{
	cml=TYLC1(gh, xlc, DLarray);
}










/*===================================================================
函数名:TrsProjmltoCkml
------------------------------------------------------------
函数功能:投影里程转现场里程
------------------------------------------------------------
输入:
projml            连续里程
ckml              现场里程
nLC               现场里程保留的小数位数
------------------------------------------------------------
输出:计算出现场里程返回true，反之返回false
------------------------------------------------------------
算法描述:由二线的连续里程计算出该点是否参照其它线路。如果不参照，
在二线上计算其现场里程；如果参照，求出在参照线（一线）的
投影里程，计算该投影里程在基线的现场里程。
         求参照线时（函数GetBLIndx），找最原始的参照线，eg:设计二线该里程段参照
设计基线，设计基线参照既有基线，最终找到既有基线。
------------------------------------------------------------
编写: 李帅 2016.4.12  
------------------------------------------------------------
备注：调用该函数前，应该根据断链点坐标判断断链是否存在，更新所有断链数据
=====================================================================*/
bool RHA::TrsProjmltoCkml(double projml, ACHAR ckml[], int nLC) const
{
	double projml1x;//在参照线上的投影里程
	double cml=0;//在该线上的连续里程
	double cml1x=0;//在参照线上的连续里程
	int dlNum;
	ACHAR GH[16];
	double XCLC=0.0;//现场里程
	double dlCml=0.0;//断链处统一里程


	_tcscpy(ckml, L"");

	//一、一线
	if(XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
		dlNum=DLarray.size();
		for (int j=0; j<dlNum-1; j++)
		{
			//2、在相邻断链点之间的段落内
			if (projml > DLarray[j].TYLC-0.001 
				&& projml < DLarray[j+1].TYLC+0.001)
			{

				//818 暂时认为临时线和施工便线也可以增加标志里程
				_tcscpy(GH, DLarray[j].EGH);
				cml=CalnewcmlTocml(projml);//统一里程
				XCLC=XLC1(cml, GH, DLarray);//现场里程
				
				_tcscpy(ckml, LCchr(GH, XCLC, nLC));
				return true;

			}
		}

	}
	else
	{
	//二、二线
		//1、获取最原始的参照线指针及在参照线上的投影里程
		//注意：得到的projml1x一定在pRHA_BL的绕行段上

		RHA *pRHA_BL=NULL;
		pRHA_BL=GetBasLine(projml, projml1x);//最原始的参照线

		if (pRHA_BL)
		{
			//求出在一线上的投影里程 projml1x
			dlNum=pRHA_BL->DLarray.size();
			for (int j=0; j<dlNum-1; j++)
			{
				//2、在相邻断链点之间的段落内
				if (projml1x > pRHA_BL->DLarray[j].TYLC-0.001 
					&& projml1x < pRHA_BL->DLarray[j+1].TYLC+0.001)
				{
					//冠号赋值
					_tcscpy(GH, pRHA_BL->DLarray[j].EGH);	

					if (pRHA_BL->XLShuXing == JYJX
						|| pRHA_BL->XLShuXing == JY2X)
					{//既有线

						//投影里程转统一里程
						cml1x=pRHA_BL->CalnewcmlTocml(projml1x);

						//计算现场里程数值
						XCLC=pRHA_BL->XLC1(cml1x, GH, pRHA_BL->DLarray);
					}
					else
					{//设计线

						XCLC = pRHA_BL->DLarray[j].ELC + projml1x
							- pRHA_BL->DLarray[j].TYLC;
					}

					_tcscpy(ckml, LCchr(GH, XCLC, nLC));
					return true;

				}
			}

		}
		else
			return false;
	}
}

//函数功能:投影里程转现场里程
bool RHA::TrsProjmltoCkml(double projml, int NLC, ACHAR GH[], double &XCLC) const
{
	ACHAR ckml[MaxCkmlStrNum];

	_tcscpy(GH, L"");
	XCLC=0.0;

	if (TrsProjmltoCkml(projml, ckml, NLC))
	{
		split_ckml(ckml, XCLC, GH);
		return true;
	}
	else
		return false;
}



/*===================================================================
函数名:GetBasLine    
------------------------------------------------------------
函数功能:给投影里程求该点最原始的参照线，返回参照线指针
------------------------------------------------------------
输入:
	   projml2x   该点的投影里程
	   blix       该点的参照线在数组g_DocManageAry[m_iDoc].XLAry的下标
------------------------------------------------------------
输出:  projml1x   该点在参照线上的投影里程
------------------------------------------------------------
返回值:  参照线指针，如果没找到返回空
------------------------------------------------------------
算法:  如果该点在绕行段，参照线就是本身；在并行段就找它的参照线，如果它的参照线
       还参照了别的线，就继续找下去,找到最原始的参照线
------------------------------------------------------------
编写: 李帅 2016.4.9  
=====================================================================*/
RHA* RHA::GetBasLine(double projml2x, double &projml1x) const
{
	int dlNum=0;
	ACHAR blName[50];
	Int16 mark=0;
	RHA *pRHA=NULL;
	RHA *pRHA_BL=NULL;
	CString mes;

	//求本线路在全局数组g_DocManageAry的指针
	pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, m_XLName);

	if (pRHA == NULL)
	{
		mes.Format(L"错误：里程计算时发现[%s]不在全局数组中\n", m_XLName);
		ads_printf(mes);//818 删除线路后在撤销时会
		return pRHA;
	}

	do 
	{
		dlNum=pRHA->DLarray.size();
		if (dlNum<2)
		{
			ads_printf(L"断链数小于2\n");
			return NULL;
		}

		mark=0;//标志
		_tcscpy(blName,L"");

		int sdl=0, edl=0;
		for(int i=0; i<dlNum-1; i++)
		{
			sdl=i;
			edl=-1;
			for(int j=sdl+1; j<dlNum; j++)
			{
				if(pRHA->DLarray[j].Indx != -3)//跳过从参照线上复制的断链
				{
					edl=j;
					break;
				}
			}

			if (edl == -1)
			{
				ads_printf(L"提示：断链编号设置错误\n");
				return NULL;
			}
			
			if(projml2x > pRHA->DLarray[sdl].TYLC-0.001 
				&& projml2x < pRHA->DLarray[edl].TYLC+0.001)//818 落在此段落，考虑计算误差？
			{
				if (pRHA->DLarray[sdl].Indx>-1 
					&& pRHA->DLarray[sdl].Indx == pRHA->DLarray[edl].Indx)
				{//在绕行段内  

					projml1x=projml2x;
					return pRHA;
				}
				else                    
				{//在并行段内

					//获得参照线名
					_tcscpy(blName, L"");
					if(pRHA->DLarray[sdl].Indx != -3)
					{//绕行段断链 或 并行段分段断链，直接复制参照线名
						_tcscpy(blName, pRHA->DLarray[sdl].BLName);
					}
					

					if (_tcscmp(blName, L"") == 0)
					{
						ads_printf(L"错误：某投影里程的参照线名为空\n");				
						return NULL;
					}

					//参照线指针
					pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);

					if (pRHA_BL)     
					{ //计算投影里程

						if(CalXJJ(projml2x, pRHA->array, pRHA->tarray
							, pRHA_BL->array, pRHA_BL->tarray, projml2x, false))
						{
							pRHA=pRHA_BL;//替换参照线
							mark=1;
							break;
						}
						else
						{
							ads_printf(L"一线在二线投影几何里程未找到\n");				
							return NULL;
						}

					}
					else               
					{
						ads_printf(L"某连续里程的参照线实体未找到\n");				
						return NULL;
					}
				}
			}

			i=edl-1;
		}

		if (mark == 0)
			return NULL;//投影里程不在断链之间

	} while (1);

}


/*===================================================================
函数名:TrsProjmltoCkml
------------------------------------------------------------
函数功能:投影里程转坐标
------------------------------------------------------------
输入:
Projml            投影里程
PZ                线路点（里程，坐标）
=====================================================================*/
bool RHA::TrsProjmltoNE(double Projml, xlpoint& PZ) const
{
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, PZ);
	return true;
}

void RHA::TrsProjmltoNE(double Projml, double &N,  double &E) const
{
	xlpoint PZ;

	N=E=0.0;
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, PZ);
	N=PZ.x;E=PZ.y;
}


/*===================================================================
函数名:TrsCmltoNE
------------------------------------------------------------
函数功能:统一里程转坐标
------------------------------------------------------------
输入:
Cml               统一里程
输出:
N、E              该里程的坐标
------------------------------------------------------------
编写：李帅 2016.12.16
=====================================================================*/
void RHA::TrsCmltoNE(double cml, double& N, double &E) const
{
	xlpoint PZ;

	PZ.lc=TrsCmltoProjml(cml);
	xlpoint_pz(array, tarray, PZ);
	N=PZ.x;E=PZ.y;
}

void RHA::TrsCmltoPZ(double cml, xlpoint& PZ)
{
	PZ.lc=TrsCmltoProjml(cml);
	xlpoint_pz(array, tarray, PZ);
}

/*===================================================================
函数名:TrsCkmltoNE
------------------------------------------------------------
函数功能:现场里程转坐标
------------------------------------------------------------
输入:
Ckml              现场里程
输出:
N                 纬距
E                 经距
=====================================================================*/
void RHA::TrsCkmltoNE(ACHAR Ckml[], double &N, double &E)
{
	//double projml;
	xlpoint PZ;

	TrsCkmltoProjml(Ckml, PZ.lc);
	xlpoint_pz(array, tarray, PZ);
	N=PZ.x;
	E=PZ.y;
}

void RHA::TrsCkmltoPZ(ACHAR Ckml[], xlpoint& PZ) const
{
	TrsCkmltoProjml(Ckml, PZ.lc);
	xlpoint_pz(array, tarray, PZ);
}

void RHA::TrsCkmltoPZ(ACHAR gh[], double dml, xlpoint& PZ) const
{
	TrsCkmltoProjml(gh, dml, PZ.lc);
	xlpoint_pz(array, tarray, PZ);
}





void RHA::TrsNEtoCml(double N, double E, double& cml)
{
	ACHAR ckml[MaxCkmlStrNum];
	double projml;

	projml= PROJ_ML(array, tarray, N, E, N, E);
	TrsProjmltoCkml(projml, ckml, 6);
	TrsCkmltoCml(ckml, cml);
}

/*===================================================================
函数名:LocatHmOnLine   
------------------------------------------------------------------------
函数功能:定位参照线直线段[sml, eml]上的百米标
------------------------------------------------------------------------
输入:
sProjml                直线段起点投影里程
eProjml                直线段终点投影里程
directn            方向，由终点向前取整 =true，由起点向后取整 =false
CKml[]             取整的现场里程（带冠号），输出
------------------------------------------------------------------------
输出: 取整成功则输出参照线上的现场里程CKml并返回在参照线上的投影里程tylc，否则返回-100
------------------------------------------------------------------------
备注: 1 此为一线的类函数，由一线实体调用
      2 由于存在两个连续绕行段的情况，要求：绕行止、单点绕行的绕行始需要记录其对应参照线和对应参照的参照交点。
	    一般的绕行始由其前一个交点的参照交点推算该处的参照线交点。
	  3 要求找到的百米标在一线（参照线）的直线段上
------------------------------------------------------------------------
编写: 李帅 2016.6.17  
=====================================================================*/
double RHA::LocatHmOnLine(double sProjml, double eProjml, bool directn, ACHAR CKml[]) const
{	
	ACHAR GH[16]=L"";
	double dml=0, dmlcpy=0;
	double projmlcpy=0;
	double intdml=0;
	double Intdist=0;
	double DLdist=0; 
	int DLIx=0;//断链编号
	double tylc=0;
	double projml=0;
	double dlCml=0;

	_tcscpy(CKml, L"");

	//1 获取现场里程值（double）
	if(sProjml+0.1 > eProjml)
		return -100;

	if (directn)
		projml=eProjml;
	else
		projml=sProjml;

	TrsProjmltoCkml(projml, CKml, 1);
	split_ckml(CKml, dml, GH);

	for (Int16 size=100; size >= 1; size=size/10)//取整百米里程标，整百米取不到取十米、一米
	{
		dmlcpy=dml;
		projmlcpy=projml;

		do 
		{
	//2 取整的现场里程
			if (directn)
			{
				intdml=floor(dmlcpy/size)*size;//向前取整的现场里程
				DLIx=FindNearDL(projmlcpy, DLarray, true);
			}
			else
			{
				intdml=ceil(dmlcpy/size)*size;//向后取整
				DLIx=FindNearDL(projmlcpy, DLarray, false);
			}

			if(DLIx<0)
				return -100;

			Intdist=fabs(dmlcpy-intdml);//取整后里程与取整前里程的距离
			DLdist=fabs(projmlcpy - DLarray[DLIx].TYLC);//前或后一个断链点到projmlcpy的距离
		
	 //3 考虑取整的现场里程是否跨越断链
			if (Intdist < DLdist+0.001)//没有跨断链	 //818 Intdist是cml之差，DLdist是projml,应当修改	
			{
				if (directn)
					_tcscpy(GH, DLarray[DLIx].EGH);
				else
					_tcscpy(GH, DLarray[DLIx].BGH);

				_tcscpy(CKml, LCchr(GH, intdml, 3));
	 //4 判断是否在参照线的夹直线上
				TrsCkmltoProjml(CKml, tylc);
				if(tylc>sProjml-0.001 && tylc<eProjml+0.001)//返回百米标
					return tylc;
				else
					break;//接着寻找整十米标、整米标
			}
			else//跨断链
			{
				projmlcpy=DLarray[DLIx].TYLC;

				if(projmlcpy>sProjml-0.001 && projmlcpy<eProjml+0.001)// ls 16.9.23 如果跨断链就取断链
				{
					if(directn)
						_tcscpy(CKml, LCchr(DLarray[DLIx].EGH, DLarray[DLIx].ELC, 3));
					else
						_tcscpy(CKml, LCchr(DLarray[DLIx].BGH, DLarray[DLIx].BLC, 3));

					return projmlcpy;
				}

				if(directn)
					dmlcpy=DLarray[DLIx].BLC;
				else
					dmlcpy=DLarray[DLIx].ELC; 

				

			}

		} while (DLIx >= 0);

	}

	return -100;
}


//给定统一里程，是否向前寻找附近的整百米标，要求在直线上 //现场里程
double RHA::LocateHundredMeterOnLine(double tylc1x, bool IsForwardTo, ACHAR ckml[]) const
{
	double gettylc=tylc1x;
	double sml, eml;
	int iXY=0;//搜索所处线元
	int XYClass;//线元类型
	ACHAR GH[20];//里程冠号
	double xlc;//一线上投影现场里程
	xlpoint TmpPZ;//
	int i;

	TrsProjmltoCkml(tylc1x, 3, GH, xlc);
	if (fmod(xlc+0.0001,100.0)<0.001)
	{
		//已经在百米标，无需再定位
		_tcscpy(ckml, LCchr(GH, xlc, 3));
		return tylc1x;
	}
	

	//判断该里程是否在断链点上
	int iDL=-1;
	iDL=JudgePtIsOnDL(tylc1x);
	if(iDL >= 0)
	{
		if(IsForwardTo)
			_tcscpy(ckml, LCchr(DLarray[iDL].EGH, DLarray[iDL].ELC));
		else
			_tcscpy(ckml, LCchr(DLarray[iDL].BGH, DLarray[iDL].BLC));

		return tylc1x;

	}
	//if (DistToNearDL(tylc1x, DLarray, true)<1.0//818 前后有断链，如何定位百米标
	//	|| DistToNearDL(tylc1x,  DLarray, false)<1.0)//找到断链截止 818 距离前后断链小于1？
	//	return tylc1x;

	for( i=tarray;i>0;i--)
	{
		if(tylc1x>=array[i][6])
		{
			iXY=i;
			break;
		}
	}
	if(i<=0)
		return -1.0;//plh06.03

	XYClass=array[iXY][0];//取到线元类型
	if(IsForwardTo)//如果向前搜索
	{
		for(; iXY>-1; iXY--)
		{
			XYClass=array[iXY][0];

			if (XYClass == 1)
				break;
		}
		
		//此时线元必直线
		eml=array[iXY+1][6];//一线直缓点里程
		if(tylc1x<eml)
			eml=tylc1x;

		gettylc=LocatHmOnLine(array[iXY][6], eml, true, ckml);
	}
	else
	{
		for(;iXY<tarray;iXY++)
		{
			XYClass=array[iXY][0];

			if (XYClass == 1)
				break;
		}

		//此时线元必直线
		sml=array[iXY][6];//缓直里程
		if(tylc1x>sml)
			sml=tylc1x;

		gettylc=LocatHmOnLine(sml, array[iXY+1][6], false, ckml);
	}

	return gettylc;
}




/*===================================================================
函数名:ModifyJDXZ
------------------------------------------------------------
函数功能:修改交点性质
------------------------------------------------------------
算法:1、全线绕行，将交点性质全改为基线交点
     2、绕行终交点性质若为绕行中，修改为RX_End
------------------------------------------------------------
编写: 李帅 2016.4.22  
=====================================================================*/
//void RHA::ModifyJDXZ()
//{
//	int NJD=JDarray.size();
//
//	//全线绕行
//	if (JDarray[0].JDXZ == RX_Start && JDarray[NJD-1].JDXZ == RX_End
//		&& GetRXDNum(JDarray) == 1)//修改为RX_End
//	{
//		for (int i=0; i<NJD; i++)
//			JDarray[i].JDXZ=BLJD;
//	}
//
//}


/*===================================================================
函数名:GetRXDNum
------------------------------------------------------------
函数功能:获得线路的绕行段数
------------------------------------------------------------
输入： tmpJDarray   交点数组
------------------------------------------------------------
输出： RXDNum   线路的绕行段数；如果没有
------------------------------------------------------------
算法: 找出交点属性为RX_Start的交点个数，即为绕行段数；如果没有RX_Start点作绕行段的开始，则返回-99
------------------------------------------------------------
编写: 李帅 2016.5.17  
=====================================================================*/
int RHA::GetRXDNum(vector<JDarrayStruct> &jdAry)           
{
	int RXDNum=0;//绕行段数目
	int JDNum=jdAry.size();
	int SJD;//绕行起始交点号

	for(int i=0; i<JDNum; i++)
	{
		/*if (i<JDNum-1)
		{*/
			if(jdAry[i].JDXZ == RX_End || jdAry[i].JDXZ==RX_One)
			{
				ads_printf(L"绕行段应有绕行始！请核查数据！\n");
				return -99;
			}

		/*}*/
		if(jdAry[i].JDXZ == RX_Start)
		{
			SJD=i;
			do 
			{
				i++;
			} while (i<JDNum && jdAry[i].JDXZ == RX_One);//818 last jd

			if (i == JDNum)
			{
				if (i-1>SJD)//缺少绕行止
				{
					jdAry[i-1].JDXZ=RX_End;
					ads_printf(L"绕行段应有绕行止!\n");
				}
			}
			else
			{
				if (jdAry[i].JDXZ == RX_End)//1 有绕行止
				{
				}
				else if (i>SJD+1)//2 缺少绕行止
				{
					jdAry[i-1].JDXZ=RX_End;
					ads_printf(L"绕行段应有绕行止!\n");
				}
				else
				{
					//3 单点绕行
					i--;
				}
				
			}
				
			

			RXDNum++;			
		}


	}
	return RXDNum;
	
}


/*===================================================================
函数名:GetRXDNum
------------------------------------------------------------
函数功能:获得线路的绕行段数
------------------------------------------------------------
输入： tmpJDarray   交点数组
------------------------------------------------------------
输出： RXDNum   线路的绕行段数；如果没有
------------------------------------------------------------
算法: 找出交点属性为RX_Start的交点个数，即为绕行段数；如果没有RX_Start点作绕行段的开始，则返回-99
------------------------------------------------------------
编写: 李帅 2016.5.17  
=====================================================================*/
int RHA::GetRXDNum() const           
{
	int RXDNum=0;//绕行段数目
	int JDNum=JDarray.size();
	int SJD;//绕行起始交点号

	for(int i=0; i<JDNum; i++)
	{
		if(JDarray[i].JDXZ == RX_End || JDarray[i].JDXZ == RX_One)
		{
			ads_printf(L"绕行段应有绕行始！请核查数据！\n");
			return -99;
		}

		if(JDarray[i].JDXZ == RX_Start)
		{
			SJD=i;
			do 
			{
				i++;
			} while (i<JDNum && JDarray[i].JDXZ == RX_One);//818 last jd

			if (i == JDNum)
			{
				if (i-1>SJD)//缺少绕行止
				{
					//JDarray[i-1].JDXZ=RX_End;
					ads_printf(L"绕行段应有绕行止!\n");
				}
			}
			else
			{
				if (JDarray[i].JDXZ == RX_End)//1 有绕行止
				{
				}
				else if (i>SJD+1)//2 缺少绕行止
				{
					//JDarray[i-1].JDXZ=RX_End;
					ads_printf(L"绕行段应有绕行止!\n");
				}
				else
				{
					//3 单点绕行
					i--;
				}

			}

			RXDNum++;			
		}


	}
	return RXDNum;
}


/*===================================================================
函数名:GetRxdSEJD    
------------------------------------------------------------
函数功能:得到指定绕行段的起终交点下标
------------------------------------------------------------
输入:
iRXD              该绕行段的编号
SJDIx       绕行段起点交点下标
EJDIx         绕行段终点交点下标
------------------------------------------------------------
输出:  找到起终交点返回true，否则返回false
------------------------------------------------------------
算法:  1、寻找RX_Start和RX_End为一个段落
       2、单点绕行就是一个RX_Start
       3、RX_One作为绕行段的终点时修正为RX_End
------------------------------------------------------------
编写: 李帅 2016.7.30  
=====================================================================*/
bool RHA::GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx, vector<JDarrayStruct> &jdAry)
{
	SJDIx = -1;
	EJDIx = -1;
	int RXDNum=0;//绕行段数目
	int NJD=jdAry.size();

	for(int i=0; i<NJD; i++)
	{
		if(jdAry[i].JDXZ == RX_Start)//绕行始
		{
			SJDIx=i;
			do
			{
				if(i<NJD-1)
					i++;
				else
					break;

			}while(i<NJD-1 && (jdAry[i].JDXZ == RX_End 
				   || jdAry[i].JDXZ == RX_One));//818 有问题 越界？

			 //818 未考虑复杂情况的交点属性,修改交点属性
			if (i == NJD-1 && (jdAry[i].JDXZ == RX_End || jdAry[i].JDXZ == RX_One))
				i=i;
			else
				i--;
			 
			if(i == SJDIx)//单点绕行
				EJDIx=SJDIx;
			else
				EJDIx=i;
			
			if(iRXD == RXDNum)//是否为该绕行段
				return true;
			RXDNum++;

		}
	}

	return false;	
}

//确定所有绕行段起终交点号数组
//李伟添加
bool RHA::GetAllRxdSEJD(vector<JDarrayStruct> &jdAry, vector<int>& SJDArray, vector<int>& EJDArray)
{
	int SJDIx = -1;
	int EJDIx = -1;
	int RXDNum=0;//绕行段数目
	int NJD=jdAry.size();

	for(int i=0; i<NJD; i++)
	{
		if(jdAry[i].JDXZ == RX_Start)//绕行始
		{
			SJDIx=i;
			do
			{
				if(i<NJD-1)
					i++;
				else
					break;

			}while(i<NJD-1 && (jdAry[i].JDXZ == RX_End 
				|| jdAry[i].JDXZ == RX_One));//818 有问题 越界？

			//818 未考虑复杂情况的交点属性,修改交点属性
			if (i == NJD-1 && (jdAry[i].JDXZ == RX_End || jdAry[i].JDXZ == RX_One))
				i=i;
			else
				i--;

			if(i == SJDIx)//单点绕行
				EJDIx=SJDIx;
			else
				EJDIx=i;

			SJDArray.push_back(SJDIx);
			EJDArray.push_back(EJDIx);
			RXDNum++;

		}
	}
	if (RXDNum > 0)
		return true;
	else
		return false;
}



//得到指定绕行段的起终交点下标
bool RHA::GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx)
{
	return GetRxdSEJD(iRXD, SJDIx, EJDIx, JDarray);
}
/*===================================================================
函数名:GenRXBX_DL   
------------------------------------------------------------
函数功能:生成绕行段起点或终点断链、并行段前后参照不同线路而产生的断链
------------------------------------------------------------
输入:
iRXD         绕行段编号0/1/2/3 或 并行段标识-2
jdix         交点下标
dl           断链（输出）
dirctn       断链类型：true——绕行始，false——绕行止
------------------------------------------------------------
输出:  生成断链返回true，否则false
------------------------------------------------------------
算法:  绕行起始点在参照线的夹直线上
------------------------------------------------------------
备注：1 要求绕行起始点对应的参照线交点下标不需要保证正确，由前后交点的参照线交点号推出。
      2 要求传入的交点数组中，每个交点都记录其与后一个交点形成的直线，对应参照直线边的前
        一个交点的下标
	  3 求并行段断链时，jdix为两交点参照不同线路输入的后一交点下标
	  4 自动生成断链时，先生成并行段断链再生成绕行断链，便于检查交点属性是否应改为绕行
------------------------------------------------------------
编写: 李帅 2016.6.22 
=====================================================================*/
bool RHA::GenRXDL(int iRXD, int jdix, DLTab &dl, bool dirctn)
{
	//考虑起终点绕行的情况 2016.7.29
	if(jdix == 0 && JDarray[jdix].JDXZ == RX_Start 
		&& GetRXDNum(JDarray)>1)
	{
		//起点绕行
		dl.Indx = iRXD;
		dl.BLC = 0.0;
		dl.ELC = dl.BLC;
		dl.TYLC = 0.0;
		_tcscpy(dl.BGH, L"");// 818 冠号唯一性问题，以及冠号的默认值
		_tcscpy(dl.EGH, L"CK");
		_tcscpy(dl.BLName, L"");
		_stprintf(dl.RXDName, L"绕行段%0.1lf", dl.ELC/1000.0);
		return true;
	}
	else if(jdix == JDarray.size()-1 && JDarray[jdix].JDXZ == RX_End 
		&& GetRXDNum(JDarray)>1)
	{
		//终点绕行
		_tcscpy(dl.BLName, L"");
		dl.BLC = dl.ELC+QXB[JDarray.size()-1].KZH-dl.TYLC;//dl.TYLC 为绕行始的投影里程 ls ？？未考虑绕行段中间断链 
		dl.ELC = dl.BLC;
		dl.TYLC = QXB[JDarray.size()-1].KZH;
		_tcscpy(dl.BGH, dl.EGH);
		_tcscpy(dl.EGH, L"");
		return true;
	}


	//中间绕行段断链
	ACHAR ckml[50];
	ACHAR blName[50];//参照线名,前一条线参照线
	RHA *pRHA_BL=NULL;
	double projml1x, projml2x;//在参照线上的投影里程
	ACHAR mes[100];

	//一、定位绕行起终点、并行段分界点
	if (JDarray[jdix].JDXZ == RX_Start && dirctn)
	{
		/*1 绕行始*/  
		int TmpJDIndx;
		double KZH, KZHProjTo1x;
		double KZH1x, KHZ1x; 

		//1.1 求绕行始的参照线。前一个交点属性为绕行始/止、并行、并行不平行时，以其参照线为起点参照线
		if (jdix>0 && (JDarray[jdix-1].JDXZ == BXPX 
			|| JDarray[jdix-1].JDXZ == BX_NotPX
			|| JDarray[jdix-1].JDXZ == RX_End 
			|| JDarray[jdix-1].JDXZ == RX_Start))                                  
			//818 在考虑参照线名称时，由前后关系推很容易出错，写一个关系处理函数。考虑两个单点绕行连起来的情况
			_tcscpy(blName, JDarray[jdix-1].BLName);
		else
			_tcscpy(blName, JDarray[jdix].BLName);

		//获得参照线指针
		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);
		if (pRHA_BL == NULL)
			return false;

		//1.2 确定绕行始百米标查找范围(KHZ1x, KZH1x)，保证找到的百米标在直线区间内，在参照线的直线上
		KZH=QXB[jdix].KZH;//绕行始的ZH点的二线投影里程
		//求KZH投影到参照线上的投影里程KZHProjTo1x
		if (!CalXJJ(KZH, array, tarray, pRHA_BL->array, pRHA_BL->tarray, KZHProjTo1x, false))
			return false;

		if(jdix>0)
		{
			GetJdno1FromJdstr(JDarray[jdix-1].BLJDName, pRHA_BL->JDarray, TmpJDIndx);//818 注意判断这个函数 TmpJDIndx可能 =-1
			if (TmpJDIndx<0)
			{
				_stprintf(mes, L"没有找到交点%s记录的参照线交点%s", JDarray[jdix-1].JDName, JDarray[jdix-1].BLJDName);
				return false;
			}
		}
		else
		{
			TmpJDIndx=0;ads_printf(L"未找到参照线上的交点！\n");
		}
		//TmpJDIndx= JDarray[jdix-1].BLJDName;
		KZH1x=pRHA_BL->QXB[TmpJDIndx+1].KZH;//参照线上，夹直线后一个交点的ZH点投影里程
		KZH1x=min(KZHProjTo1x, KZH1x);

		KHZ1x= pRHA_BL->QXB[TmpJDIndx].KHZ;//参照线上，夹直线前一个交点的HZ点投影里程

		//1.3 定位百米标，返回在参照线上的投影里程ZHProjml
		projml1x=pRHA_BL->LocatHmOnLine(KHZ1x, KZH1x, true, ckml);
		//1.4 考虑该绕行始断链在前一绕行止断链之前的情况
		if(iRXD>0)
		{
			CalXJJ(projml1x, pRHA_BL->array, pRHA_BL->tarray
				, array, tarray, projml2x, false);

			if (projml2x-0.1<DLarray[DLarray.size()-1].TYLC)//818 不一定是DLarray.size()-1，需要通过projml2x去找，暂时隐藏
			{
				//前一绕行止和后一绕行始重叠 818 暂时隐藏
				//double preDLprojml1x=0;//前一断链在参照线上的投影里程
				//CalXJJ(DLarray[DLarray.size()-1].TYLC, array, tarray
				//	, pRHA_BL->array, pRHA_BL->tarray, preDLprojml1x, false);

				//if(preDLprojml1x+0.1<KZH1x)
				//{
				//	//重新定位百米标
				//	projml1x=pRHA_BL->LocatHmOnLine(preDLprojml1x+0.1, KZH1x, true, ckml);
				//}
				//else
				//	ads_printf(L"前一绕行止和后一绕行始重叠，找不到合适的断链位置\n");
			}
		}
	}
	else if(JDarray[jdix].JDXZ == RX_End 
		    || (JDarray[jdix].JDXZ == RX_Start && dirctn == false))
	{
		/*2 绕行止*/
		int TmpJDIndx;
		double KHZ, KHZProjTo1x;
		double KZH1x, KHZ1x;

		//2.1 求绕行止的参照线
		if (jdix<JDarray.size()-1 && (JDarray[jdix+1].JDXZ == BXPX || JDarray[jdix+1].JDXZ == BX_NotPX))
			_tcscpy(blName, JDarray[jdix+1].BLName);
		else
			_tcscpy(blName, JDarray[jdix].BLName);

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);//参照线指针
		if (pRHA_BL == NULL)
			return false;

		//2.2 确定绕行止百米标定位范围
		KHZ=QXB[jdix].KHZ;//HZ点的二线投影里程
		//一线投影里程KHZProjTo1x
		if (!CalXJJ(KHZ, array, tarray, pRHA_BL->array, pRHA_BL->tarray, KHZProjTo1x, false))
			return false;

		if (jdix<JDarray.size()-1 && JDarray[jdix+1].JDXZ != RX_Start)
		{
			//计算用jdix+1记录的参照线交点推算绕行止对应的参照线交点
			GetJdno1FromJdstr(JDarray[jdix+1].BLJDName, pRHA_BL->JDarray, TmpJDIndx);
			//TmpJDIndx = JDarray[jdix+1].BLJDName;
			KHZ1x=pRHA_BL->QXB[TmpJDIndx-1].KHZ;//参照线上，夹直线前一个交点的HZ点投影里程	
			KHZ1x=max(KHZProjTo1x, KHZ1x);

			KZH1x= pRHA_BL->QXB[TmpJDIndx].KZH;//参照线上，夹直线后一个交点的ZH点投影里程
		}
		else
		{
			//计算用jdix记录的参照线交点
			int iBLJD;
			GetJdno1FromJdstr(JDarray[jdix].BLJDName, pRHA_BL->JDarray, iBLJD);
			KHZ1x=pRHA_BL->QXB[iBLJD].KHZ;
			KHZ1x=max(KHZProjTo1x, KHZ1x);

			KZH1x= pRHA_BL->QXB[iBLJD+1].KZH;
		}

		//2.3 定位百米标
		projml1x=pRHA_BL->LocatHmOnLine(KHZ1x, KZH1x, false, ckml);
		
	}
	else
		return false;

	if (projml1x<0)
		return false;
	

	//二、生成断链
	Int16 LorR;//左右
	double dml;//CKZH、CKHZ的里程值（去冠号）
	ACHAR GH[16];//CKZH、CKHZ的冠号
	double xjj=0;
	double resPt[2];
	
	dl.Indx=iRXD;
	_tcscpy(dl.XLName, m_XLName);
	_tcscpy(dl.BLName, blName);

	CalXJJ(projml1x, pRHA_BL->array, pRHA_BL->tarray
		   , array, tarray, xjj, projml2x, resPt, false);

	split_ckml(ckml, dml, GH);
	if (JDarray[jdix].JDXZ == RX_Start && dirctn)
	{
		//绕行始
		if (xjj>0)
			LorR=1;
		else
			LorR=-1;
		
		dl.BLC = dml;
		dl.ELC = dml;
		dl.TYLC = projml2x;
		_tcscpy(dl.BGH, GH);
		_tcscpy(dl.EGH, CreatRXD_SPtEGH(blName, LorR, GH));
		_stprintf(dl.RXDName, L"绕行段%0.1lf", dl.ELC/1000.0);

	}
	else if ((JDarray[jdix].JDXZ == RX_End) 
		     || (JDarray[jdix].JDXZ == RX_Start && dirctn == false)) 
	{
		//绕行止
		dl.BLC = dl.ELC+projml2x-dl.TYLC;//dl.TYLC 为绕行始的投影里程 ls ？？未考虑绕行段中间断链 
		dl.ELC = dml;
		dl.TYLC = projml2x;
		_tcscpy(dl.BGH, dl.EGH);//？？未考虑绕行段中间断链引起的冠号改变
		_tcscpy(dl.EGH, GH);
	}

	/*xlpoint pz;
	pz.lc=dl.TYLC;
	xlpoint_pz(array, tarray, pz);
	dl.N=pz.x;
	dl.E=pz.y;*/
	//nan
	/*AcGePoint3d pt;
	pt.x=pz.y;
	pt.y=pz.x;
	makecircle(pt, 10, 2);*/

	return true;
}

/*===================================================================
函数名:GenBXDL   
------------------------------------------------------------
函数功能:生成并行段前后参照不同线路而产生的断链
------------------------------------------------------------
输入:
jdix         交点下标
dl           断链（输出）
------------------------------------------------------------
输出:  生成断链返回true，否则false
------------------------------------------------------------
算法:  绕行起始点在参照线的夹直线上
------------------------------------------------------------
备注：1 要求绕行起始点对应的参照线交点下标不需要保证正确，由前后交点的参照线交点号推出。
2 要求传入的交点数组中，每个交点都记录其与后一个交点形成的直线，对应参照直线边的前
一个交点的下标
3 求并行段断链时，jdix为两交点参照不同线路输入的后一交点下标
4 自动生成断链时，先生成并行段断链再生成绕行断链，便于检查交点属性是否应改为绕行
------------------------------------------------------------
编写: 李帅 2016.6.22 修改 2017.6.25
=====================================================================*/
bool RHA::GenBXDL(int jdix, DLTab &dl)
{
	ACHAR ckml[50];
	ACHAR blName[50];//参照线名,前一条线参照线
	RHA *pRHA_BL=NULL;
	RHA *pRHA_PreBL=NULL;//前一条线参照线
	RHA *pRHA_NexBL=NULL;//后一条线参照线
	double projml1x, projml2x;//在参照线上的投影里程
	bool isPrevBLPrior=true;//=true 前一参照线优先（如：前后参照线分别是既有基线、设计基线）；=false 后者优先

	if(jdix>0 && JDarray[jdix].JDXZ == BXPX && JDarray[jdix-1].JDXZ == BXPX)//818 BX_NOTPX???考虑
	{
		/*并行段分界点*/
		_tcscpy(blName, JDarray[jdix].BLName);

		//获取前后参照线指针
		pRHA_PreBL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[jdix-1].BLName);

		pRHA_NexBL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[jdix].BLName);

		//3.1 找出该线路和前后两条参照线的夹直线在该线路投影重叠区间【sml, eml】
		int prevjdix, nexjdix;
		double sml=0, eml=0, sml1=0, eml1=0, sml2=0, eml2=0;

		sml=QXB[jdix-1].KHZ;
		eml=QXB[jdix].KZH;

		//寻找该线路前后交点对应的参照线交点下标
		if (!(GetJdno1FromJdstr(JDarray[jdix-1].BLJDName, pRHA_PreBL->JDarray, prevjdix)
			&& GetJdno1FromJdstr(JDarray[jdix].BLJDName, pRHA_NexBL->JDarray, nexjdix)))
			return false;


		//前参照线，如果有绕行段断链
		if (pRHA_PreBL->JDarray[prevjdix].JDXZ == RX_End || (pRHA_PreBL->JDarray[prevjdix].JDXZ == RX_Start 
			&& pRHA_PreBL->JDarray[prevjdix+1].JDXZ != RX_One && pRHA_PreBL->JDarray[prevjdix+1].JDXZ != RX_End))
		{//前一参照线交点是绕行止或者单点绕行

			//寻找绕行止断链
			int edl=FindNearDL(pRHA_PreBL->QXB[ prevjdix ].KHZ, pRHA_PreBL->DLarray, false);

			if (edl>0 && pRHA_PreBL->DLarray[edl].Indx >= 0
				&& ((edl+1 <  pRHA_PreBL->DLarray.size() 
				&& pRHA_PreBL->DLarray[edl].Indx != pRHA_PreBL->DLarray[edl+1].Indx)
				||(edl+1 == pRHA_PreBL->DLarray.size())))
			{//有绕行止断链

				//取前一参照线前一交点的HZ点到绕行止断链的这个范围
				//，并反投影回二线，得到二线上范围(sml1, eml1)
				if(!(CalXJJ(pRHA_PreBL->QXB[ prevjdix ].KHZ, pRHA_PreBL->array
					, pRHA_PreBL->tarray, array, tarray, sml1)
					&& CalXJJ(pRHA_PreBL->DLarray[edl].TYLC, pRHA_PreBL->array
					, pRHA_PreBL->tarray, array, tarray, eml1)))
					return false;
			}//818 else

		}
		else if(!(CalXJJ(pRHA_PreBL->QXB[ prevjdix ].KHZ, pRHA_PreBL->array
			, pRHA_PreBL->tarray, array, tarray, sml1)
			&& CalXJJ(pRHA_PreBL->QXB[ prevjdix+1 ].KZH, pRHA_PreBL->array
			, pRHA_PreBL->tarray, array, tarray, eml1))) 		
			//没有绕行段断链，取前一参照线的夹直线段范围，并反投影回二线，得到二线上范围(sml1, eml1)
			return false;

		//后参照线，如果有绕行段断链
		if (pRHA_NexBL->JDarray[nexjdix].JDXZ == RX_Start)
		{//后一参照交点是绕行始

			//寻找绕行始断链
			int sdl=FindNearDL(pRHA_NexBL->QXB[ nexjdix ].KZH, pRHA_NexBL->DLarray, true);

			if (pRHA_NexBL->DLarray[sdl].Indx >= 0
				&& ((sdl>0 && pRHA_NexBL->DLarray[sdl].Indx != pRHA_NexBL->DLarray[sdl-1].Indx)
				||(sdl == 0)))
			{//有绕行始断链

				if(!(CalXJJ(pRHA_NexBL->DLarray[sdl].TYLC, pRHA_NexBL->array
					, pRHA_NexBL->tarray, array, tarray, sml2)
					&& CalXJJ(pRHA_NexBL->QXB[ nexjdix ].KZH, pRHA_NexBL->array
					, pRHA_NexBL->tarray, array, tarray, eml2)))
					return false;
			}

		}
		else if(!(CalXJJ(pRHA_NexBL->QXB[ nexjdix-1 ].KHZ, pRHA_NexBL->array
			, pRHA_NexBL->tarray, array, tarray, sml2)
			&& CalXJJ(pRHA_NexBL->QXB[ nexjdix ].KZH, pRHA_NexBL->array
			, pRHA_NexBL->tarray, array, tarray, eml2)))//没有绕行段断链
			return false;

		sml=max(sml, sml1);
		sml=max(sml, sml2);
		eml=min(eml, eml1);
		eml=min(eml, eml2);

		//比较两条参照线的优先级
		isPrevBLPrior=CCreatPM::CmpXLPriority(JDarray[jdix-1].BLName, JDarray[jdix].BLName);
		if (sml+0.1<eml)
		{
			//double projml=0.0;
			if(isPrevBLPrior)
			{
				//前参照线优先
				CalXJJ(sml, array, tarray, pRHA_PreBL->array, pRHA_PreBL->tarray, sml1);
				CalXJJ(eml, array, tarray, pRHA_PreBL->array, pRHA_PreBL->tarray, eml1);
				projml1x=pRHA_PreBL->LocatHmOnLine(sml1, eml1, true, ckml);
				//blix=prevblix;
				pRHA_BL=pRHA_PreBL;
			}
			else
			{
				//后参照线优先
				CalXJJ(sml, array, tarray, pRHA_NexBL->array, pRHA_NexBL->tarray, sml2);
				CalXJJ(eml, array, tarray, pRHA_NexBL->array, pRHA_NexBL->tarray, eml2);
				projml1x=pRHA_NexBL->LocatHmOnLine(sml2, eml2, false, ckml);
				//blix=nexblix;
				pRHA_BL=pRHA_NexBL;
			}

		}
		else
		{
			//nan
			CString mes;
			mes.Format(L"%s中的%s的属性应为绕行始，而不是并行！", m_XLName, JDarray[jdix].JDName);
			//三线没有重叠区域，修改交点属性：两个交点中一个设置为单点绕行（绕行始）
			//818 可能有问题，投影到曲线上或者交点信息赋值不对
			if (isPrevBLPrior)
			{
				JDarray[jdix].JDXZ=RX_Start;
			}
			else
			{
				JDarray[jdix-1].JDXZ=RX_Start;
				if (jdix>1)
					_tcscpy(JDarray[jdix-1].BLName, JDarray[jdix].BLName);
				JDarray[jdix-1].XJJ=0;
				_tcscpy(JDarray[jdix-1].BLJDName, L"");//绕行始的参照线交点可以由前个交点推				
			}

			return false;
		}

	}


	//二、生成断链
	Int16 LorR;//左右
	double dml;//CKZH、CKHZ的里程值（去冠号）
	ACHAR GH[16];//CKZH、CKHZ的冠号
	double xjj=0;
	double resPt[2];

	dl.Indx=-2;
	_tcscpy(dl.XLName, m_XLName);
	_tcscpy(dl.BLName, blName);

	CalXJJ(projml1x, pRHA_BL->array, pRHA_BL->tarray
		, array, tarray, xjj, projml2x, resPt, false);

	split_ckml(ckml, dml, GH);

	 //818 要求并行段断链，往两条线投影都是绕行段，可能会有问题。
	//并行
	Int16 prevdlix=0, nexdlix=0;
	if (isPrevBLPrior)//前一参照线优先
	{
		dl.BLC = dml;
		_tcscpy(dl.BGH, GH);

		CalXJJ(projml2x, array, tarray,  pRHA_NexBL->array, pRHA_NexBL->tarray, projml1x);

		nexdlix=pRHA_NexBL->JudgePtIsOnDL(projml1x);//判断是否在断链上
		if (nexdlix >= 0)
		{
			dl.ELC = pRHA_NexBL->DLarray[nexdlix].ELC;//在断链上，取断链点断后
			_tcscpy(dl.EGH, pRHA_NexBL->DLarray[nexdlix].EGH);
		}
		else
		{
			pRHA_NexBL->TrsProjmltoCkml(projml1x, ckml, 3);
			split_ckml(ckml, dml, GH);
			dl.ELC = dml;
			_tcscpy(dl.EGH, GH);
		}

	}
	else//后一参照线优先
	{
		dl.ELC = dml;
		_tcscpy(dl.EGH, GH);

		CalXJJ(projml2x, array, tarray, pRHA_PreBL->array, pRHA_PreBL->tarray, projml1x);

		prevdlix=pRHA_PreBL->JudgePtIsOnDL(projml1x);
		if (prevdlix >= 0)//在断链上，取断链点断前
		{
			dl.BLC =pRHA_PreBL->DLarray[prevdlix].BLC;
			_tcscpy(dl.BGH,pRHA_PreBL->DLarray[prevdlix].BGH);
		}
		else
		{
			pRHA_PreBL->TrsProjmltoCkml(projml1x, ckml, 3);
			split_ckml(ckml, dml, GH);
			dl.BLC = dml;
			_tcscpy(dl.BGH, GH);
		}
	}

	dl.TYLC = projml2x;
	_tcscpy(dl.RXDName, L"");
}


/*===================================================================
函数名: IsOnDL   
函数功能: 判断投影里程是否位于断链点上。
输入: Projml     投影里程
输出: 在断链上，返回断链下标；不在，返回-1。
备注: 需要有断链的投影里程
编写: 李帅 2016.6.18 
=====================================================================*/
Int16 RHA::JudgePtIsOnDL(double projml) const
{
	for(int i=0;i<DLarray.size();i++)
	{
		if (fabs(projml-DLarray[i].TYLC)<0.01)
			return i;
	}

	return -1;
}


Int16 RHA::JudgePtIsOnDL(ACHAR ckml[]) const
{
	double cml=0.0;
	double dlCml=0.0;
	
	TrsCkmltoCml(ckml, cml);

	for(int i=0;i<DLarray.size();i++)
	{
		TrsCkmltoCml(DLarray[i].EGH, DLarray[i].ELC, dlCml);

		if (fabs(dlCml-cml)<0.01)
			return i;
	}

	return -1;
}
/*===================================================================
函数功能: 根据起终投影里程生成局部断链数组
输入:
sProjml     局部的起点投影里程
eProjml     局部的终点投影里程
输出: 
dlAry            局部断链数组
备注: 需要有断链的投影里程
编写: 李帅 2016.6.18 
=====================================================================*/
void RHA::CreateLocalDLarray(double sProjml, double eProjml, vector<DLTab> &dlAry) const
{
	//计算绕行段起终几何里程
	DLTab tmpDL;
	int iDL=-1;

	_tcscpy(tmpDL.XLName, m_XLName);
	_tcscpy(tmpDL.RXDName, L"");
	_tcscpy(tmpDL.BLName, L"");
	tmpDL.Indx=0;

	dlAry=DLarray;

	//筛选在起终里程范围内的断链
	for(int i=0; i<dlAry.size(); i++)
	{
		if (dlAry[i].TYLC+0.001<sProjml)
		{
			dlAry.erase(dlAry.begin()+i);i--;
			continue;
		}

		if (dlAry[i].TYLC>eProjml+0.001)
		{
			dlAry.erase(dlAry.begin()+i);i--;
			continue;
		}
	}

	//增加起点断链
	iDL=JudgePtIsOnDL(sProjml);
	if(iDL >= 0)
		dlAry.insert(dlAry.begin(), DLarray[iDL]);
	else
	{
		TrsProjmltoCkml(sProjml, 3, tmpDL.BGH, tmpDL.BLC);
		tmpDL.ELC=tmpDL.BLC;
		_tcscpy(tmpDL.EGH, tmpDL.BGH);
		tmpDL.TYLC=sProjml;

		//编号和参照线
		iDL=GetPreDL(tmpDL.TYLC);
		if (iDL>=0)
		{
			tmpDL.Indx=DLarray[iDL].Indx;
			_tcscpy(tmpDL.BLName, DLarray[iDL].BLName);
		}
		dlAry.insert(dlAry.begin(), tmpDL);
	}

	//增加终点断链
	iDL=JudgePtIsOnDL(eProjml);
	if(iDL >= 0)
		dlAry.push_back(DLarray[iDL]);
	else
	{
		TrsProjmltoCkml(eProjml, 3, tmpDL.BGH, tmpDL.BLC);

		tmpDL.ELC=tmpDL.BLC;
		_tcscpy(tmpDL.EGH, tmpDL.BGH);
		tmpDL.TYLC=eProjml;
		//编号和参照线
		iDL=GetPreDL(tmpDL.TYLC);
		if (iDL>=0)
		{
			tmpDL.Indx=DLarray[iDL].Indx;
			_tcscpy(tmpDL.BLName, DLarray[iDL].BLName);
		}
		dlAry.push_back(tmpDL);
	}

	//删除重复断链
	for(int i=1; i<dlAry.size(); i++)
	{
		if (fabs(dlAry[i].TYLC-dlAry[i-1].TYLC)<0.1)
		{
			dlAry.erase(dlAry.begin()+i);
			i--;
		}
	}
}



/*===================================================================
函数名:CreatRXDSPtEGH   
------------------------------------------------------------
函数功能:生成绕行段起点断后冠号
------------------------------------------------------------
输入:
blName            参照线路名
LorR              二线在一线的左右侧
GH1x              参照线的冠号
------------------------------------------------------------
输出: 绕行段起点断链断后冠号GH
------------------------------------------------------------
算法:  
------------------------------------------------------------
编写: 李帅 2016.4.15 
=====================================================================*/
ACHAR* RHA::CreatRXD_SPtEGH(CString blName, int LorR, ACHAR *GH1x)
{
	ACHAR GH[16];

	if (m_XLName == L"设计基线")
	{
		_tcscpy(GH, L"A");
		_tcscat(GH, GH1x);
		return &GH[0];
	}

	//未考虑重合的情况
	if(LorR == -1)
		_tcscpy(GH, L"Z");
	else
		_tcscpy(GH, L"Y");


	if (m_XLName == L"既有二线")
	{
		if (GetJYXNum(m_mdbName) > 2)
			_tcscat(GH, L"1");
	}
	else if (m_XLName == L"既有三线")
		_tcscat(GH, L"2");
	else if (m_XLName == L"既有四线")
		_tcscat(GH, L"3");
	else if (m_XLName == L"设计二线")
	{
		if (GetSJXNum(m_mdbName) > 2)
			_tcscat(GH, L"1");
	}
	else if (m_XLName == L"设计三线")
		_tcscat(GH, L"2");
	else if (m_XLName == L"设计四线")
		_tcscat(GH, L"3");
	else
		return L"";

	if (_tcsstr(GH1x, L"A")==NULL && m_XLName.Find(L"设计")>=0)//设计线在既有线冠号前加 A
		_tcscat(GH, L"A");

	_tcscat(GH, GH1x);

	return &GH[0];
}

/*===================================================================
函数名:GenDL   
------------------------------------------------------------
函数功能:自动生成断链
------------------------------------------------------------
输出:  找到起终交点返回true，否则返回false
------------------------------------------------------------
算法:  
------------------------------------------------------------
编写: 李帅 2016.4.19  
=====================================================================*/
void RHA::GenDL()
{
	ACHAR CKml[50];
	ACHAR GH[16], GH2[16];
	double dml;
	double projml1x;
	int blix;
	RHA *pRHA_BL=NULL;

	DLarray.clear();//818 LS 未考虑断链的保留
	int NJD=JDarray.size();	
	//一 基线断链
	if(JDarray[0].JDXZ == RX_Start
		&& JDarray[NJD-1].JDXZ == RX_End
		&& GetRXDNum(JDarray) == 1)/*JDarray[0].JDXZ == BLJD*/
	{
		setDLM(0, JDarray[0].l1, JDarray[0].l1, L"K", L"K", 0.0, 0);
		setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH, L"K", L"K", QXB[NJD-1].KZH, 0);
		return;
	}

	//二 并行段断链(GenRXBX_DL时可能会改变交点属性)
	//前后交点都并行参照不同的线路时，在并行段添加断链。
	DLTab tmpDL;
	vector<DLTab> tmpBXDLarry;//并行段断链数组
	tmpBXDLarry.clear();

	for (int i=1; i<NJD; i++)
	{
		if(JDarray[i-1].JDXZ == BXPX && JDarray[i].JDXZ == BXPX 
			&& _tcscmp(JDarray[i-1].BLName, JDarray[i].BLName) != 0)
		{
			if (!GenBXDL(i, tmpDL))
			{
				//nan
				ads_printf(L"提示：并行段产生断链错误\n");
				continue;
			}

			tmpBXDLarry.push_back(tmpDL);
		}
	}

	//三 起点并行断链
	if(JDarray[0].JDXZ == BXPX || JDarray[0].JDXZ == BX_NotPX)//起点并行
	{
		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[0].BLName);
		if(pRHA_BL)
		{
			CalXJJ(0.0, array, tarray, pRHA_BL->array, pRHA_BL->tarray, projml1x, false);
			pRHA_BL->TrsProjmltoCkml(projml1x, CKml, 6);
			split_ckml(CKml, dml, GH);
			setDLM(0, dml, dml,  GH, GH, 0.0, -1, 0, JDarray[0].BLName);
		}
		else
			ads_printf(L"错误：自动产生断链时参照线未找到\n");

	}
	

	//四 绕行段断链
	int RXDNum=GetRXDNum(JDarray);
	int SJDIndx, EJDIndx;
	DLTab tmpSDL, tmpEDL;
	for (int i=0; i<RXDNum; i++)//考虑了起终点绕行的情况
	{
		//获取绕行段的起终交点下标
		GetRxdSEJD(i, SJDIndx, EJDIndx, JDarray);//818 没考虑单点绕行（不可能是起终点）的情况、
		
		if(!GenRXDL(i, SJDIndx, tmpSDL, true))//绕行始断链
		{
			RXDNum--;
			ads_printf(L"提示：生成绕行段起点断链错误\n");//nan
			continue;
		}

		tmpEDL=tmpSDL;
		if (!GenRXDL(i, EJDIndx, tmpEDL, false))//绕行止断链
		{
			RXDNum--;
			ads_printf(L"提示：生成绕行段终点断链错误\n");//nan
			continue;
		}
		
		DLarray.push_back(tmpSDL);
		DLarray.push_back(tmpEDL);
	}
	
	//五 终点并行断链
	if (JDarray[NJD-1].JDXZ == BXPX || JDarray[NJD-1].JDXZ == BX_NotPX)
	{
		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[NJD-1].BLName);
		if(pRHA_BL)
		{
			CalXJJ(QXB[NJD-1].KZH, array, tarray, pRHA_BL->array, pRHA_BL->tarray, projml1x, false);
			pRHA_BL->TrsProjmltoCkml(projml1x, CKml, 6);
			split_ckml(CKml, dml, GH);
			setDLM(DLarray.size(), dml, dml,  GH, GH, QXB[NJD-1].KZH, -1, 0,JDarray[NJD-1].BLName);
		}
		else
			ads_printf(L"错误：自动产生断链时参照线未找到\n");
		//AddDL(dml, dml,  GH, GH2, QXB[NJD-1].KZH, -1, 0,JDarray[NJD-1].BLName);
	}

	//六 插入并行段断链
	int dlix;
	for (int i=0; i<tmpBXDLarry.size(); i++)
	{
		dlix=FindNearDL(tmpBXDLarry[i].TYLC, DLarray, false);
		if (dlix>-1)
			DLarray.insert(DLarray.begin()+dlix, tmpBXDLarry[i]);
	}

}

//功能：自动产生某个绕行段的起终断链
//输入：绕行段下标
//输出：绕行起终断链
//返回值：如果断链生成正确，返回true
//编写：ls 17.8.26
bool RHA::GeniRxdDL(int indx, DLTab rxdDL[2])
{
	_tcscpy(rxdDL[0].XLName, m_XLName);
	_tcscpy(rxdDL[1].XLName, m_XLName);

	//获取绕行段的起终交点下标
	int SJDIndx, EJDIndx;
	if (!GetRxdSEJD(indx, SJDIndx, EJDIndx, JDarray))
		return false;
	
	if(!GenRXDL(indx, SJDIndx, rxdDL[0], true))//绕行始断链
	{
		ads_printf(L"提示：生成绕行段起点断链错误\n");
		return false;
	}

	rxdDL[1]=rxdDL[0];
	if (!GenRXDL(indx, EJDIndx, rxdDL[1], false))//绕行止断链
	{
		ads_printf(L"提示：生成绕行段终点断链错误\n");
		return false;
	}

	return true;

}

//标注绕行段的里程和断链
void RHA::MarkRXDLC(AcGiWorldDraw* pWd)
{
	int RxdNum=GetRXDNum(JDarray);//绕行段数目

	//标注绕行段
	int sdl=0,edl=0;
	for(int i=0;i<RxdNum;i++)
	{
		//获取绕行段起终断链号
		GetiRXDDLinfo(i,sdl,edl,DLarray);
		if( sdl==-1 || edl==-1 ) 
			return ;

		//标注单个绕行段的里程、断链
		MarkiRXDLC(pWd,DrawParam.m_dLC,1,sdl,edl);
	}
	

}

//标注绕行段的里程和断链
void RHA::MarkRXDLC() const
{
	int RxdNum=GetRXDNum();//绕行段数目

	//标注绕行段
	int sdl=0,edl=0;
	Int16 zy=1;

	zy=DrawParam.OtherParam.MarkAry[2].LorR;
	for(int i=0;i<RxdNum;i++)
	{
		//获取绕行段起终断链号
		GetiRXDDLinfo(i,sdl,edl,DLarray);
		if( sdl==-1 || edl==-1 ) 
			return ;

		//标注单个绕行段的里程、断链
		MarkiRXDLC(DrawParam.m_dLC,/*1*/zy,sdl,edl);
	}

	//如果起终点参照别的线路，则标注起终点断链
	if (DLarray.size()>0)
	{
		if(DLarray[0].Indx<0)
			MarkSglDL(0, zy);
		if (DLarray[DLarray.size()-1].Indx<0)
			MarkSglDL(DLarray.size()-1, zy);
	}
	


}



//  标注单个绕行段（公里百米里程标）  
//zy=1,-1  标在右边，左边 //二线断链表//断链数
void RHA::MarkiRXDLC(AcGiWorldDraw* pWd, double dLC, int zy, int sdl, int edl/*DLTab *iDLB,int ndl*/ )  
{
	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,lc,b,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[100],chr[100],ch[100];
	ACHAR ckml[50];
	xlpoint  PZtmp;
	double cml;//连续里程
	double BZlc, DDlength;//标注里程数值dml、断链点的连续里程（非投影里程）

	_tcscpy(GH, DLarray[sdl].EGH);
	WB=1.5*texth;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// 断链处统一里程
	_tcscpy(ckml, LCchr(DLarray[sdl].EGH, DLarray[sdl].ELC, DrawParam.NLC));
	TrsCkmltoCml(ckml,DDlength);

	DrawParam.NLC=3;//临时 818 ls		

	for (iDL=sdl;iDL<edl;iDL++)//最后一个断链为终点
	{
		//标注绕行段中间断链
		if(iDL>sdl)
			MarkSglDL(pWd, iDL, zy);

		DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//断链值=断后里程-断前里程

		//下面开始标注里程
		zlc=(long int)(DLarray[iDL].ELC/dLC);
		BZlc=zlc*dLC;  // 实际里程 
		if((BZlc-DLarray[iDL].ELC)<0.0001)
			BZlc=BZlc+dLC;
		if ( BZlc>DLarray[iDL+1].BLC+0.0001)
		{
			DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//断链处统一里程。
			continue;
		}

		_tcscpy(GH,DLarray[iDL].EGH);  //冠号
		do   		
		{
			// 连续里程
			cml=DDlength+BZlc-DLarray[iDL].ELC; 
			// 投影里程
			//PZtmp.lc=CalcmlTonewcml(cml);
			//PZtmp.lc=TrsCmltoProjml(cml);//818 太慢了
			if(XLShuXing == SJJX || XLShuXing == SJ2X )//设计线
				PZtmp.lc=BZlc-DLarray[iDL].ELC+DLarray[iDL].TYLC;
			else//既有线、临时线、施工便线
				PZtmp.lc=CalcmlTonewcml(cml);

			k=xlpoint_pz(array, tarray, PZtmp);
			
			BZW=k;    //  K<0  无解
			if (k>0 && k<=6)  //有解//线元类型
			{
				B=PZtmp.a;
				pt[X]=PZtmp.y+WB*sin(B+pi*0.5*zy);
				pt[Y]=PZtmp.x+WB*cos(B+pi*0.5*zy);
				pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zy);
				pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zy);
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
				G_makeline(pWd,ptb,pt,QXYSColor);
				pt=pte;
				b=B+pi*0.5;
				b=pi*0.5-b; if (zy==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;
				if (b>2*pi) b=b-2*pi;

				ads_rtos(BZlc,2,0,chr);  
				ii=_tcslen(chr);if(ii>20) ii=19;
				if(dLC>=100.0&&ii>=2)
				{
					chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // 公里标
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH);
						_tcscat(ch,chr);
					}
					else if(ii>3 && chr[ii-3]!='0')    // 百米标
					{ 
						ch[0]=chr[ii-3];
						ch[1]='\0';
					}		    
				}
				else
				{
					_tcscpy(ch,GH);
					if ( ii<=2)
					{
						_tcscpy(ch,L"+");
						ch[1]='0';
						ch[2]=chr[ii-2];
						ch[3]=chr[ii-1];
						ch[4]='\0';
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp,L"+");
						chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2];
						chrtmp[3]=chr[ii-1];
						chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
						{
							chr[ii-3]='\0';
							_tcscpy(ch,GH);
							_tcscat(ch,chr);
							_tcscat(ch,chrtmp); 
						}
						else if(ii>3&&chr[ii-3]!='0')
						{
							_tcscpy(ch,chrtmp);
						}
					}
				}
				
				pte[X]=PZtmp.y+(_tcslen(ch)*1*texth+WB)*sin(B+pi*0.5*zy)+0.5*texth*sin(B);
				pte[Y]=PZtmp.x+(_tcslen(ch)*1*texth+WB)*cos(B+pi*0.5*zy)+0.5*texth*cos(B);
				b+=pi;

				G_maketext(pWd,pte,ch,b,texth,QXYSColor,1);
			};          
			BZlc=BZlc+dLC;
			if ( BZlc>DLarray[iDL+1].BLC) BZW=-99;		 
		}while (BZW>0);

		DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//断链处统一里程。

	}

	//标注绕行段起点
	int RxdNo=0;
	RxdNo=DLarray[sdl].Indx;
	
	TrsProjmltoNE(DLarray[sdl].TYLC,PZtmp);

	B=PZtmp.a;
	pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zy);
	pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zy);
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x; 

	G_makeline(pWd,ptb,pt,QXYSColor);


	p1[X]=PZtmp.y-0.8*texth*sin(B);
	p1[Y]=PZtmp.x-0.8*texth*cos(B);

	pte[X]=p1[X]+2.4*texth*sin(B);
	pte[Y]=p1[Y]+2.4*texth*cos(B);
	p1[X]+=WB*18*sin(B+pi*0.5*zy);
	p1[Y]+=WB*18*cos(B+pi*0.5*zy);
	pte[X]+=WB*18*sin(B+pi*0.5*zy);
	pte[Y]+=WB*18*cos(B+pi*0.5*zy);

	b=B+pi*0.5;
	b=pi*0.5-b;
	if(zy==-1)b=b+pi;
	if(b<0.0)b=2*pi+b;
	if(b>2*pi) b=b-2*pi;		         

	_tcscpy(GH,DLarray[sdl].EGH);
	_tcscpy(GH1,DLarray[sdl].BGH);

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH1,DLarray[sdl].BLC,DrawParam.NLC));  // 断前里程
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);
	G_maketext(pWd,p1,chr,b+pi,texth,QXYSColor,1);  

	ACHAR RXFlag[80];
	//plh04.11.26
	_stprintf(RXFlag,L"%s起点 ",DLarray[sdl].RXDName);


	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH,DLarray[sdl].ELC,DrawParam.NLC));     // 断后里程
	_tcscpy(chr,L" = ");
	_tcscat(chr,RXFlag);
	_tcscat(chr,ch);   // e.f chr=L"ZH K10+123.45"   

	DL=DLarray[sdl].ELC-DLarray[sdl].BLC;
	if (fabs(DL)>0.001)
	{ 
		if(DL>0.0)
		{
			_tcscat(chr, L" 短链 "); 
			ads_rtos( DL,2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else if( DL<0.0)
		{
			_tcscat(chr, L" 长链 "); 
			ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else
		{/*断链值正好为0*/}
	}
	G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);


	//标注绕行段终点
	iDL=edl;
	RxdNo=DLarray[edl].Indx;
	DL=DLarray[edl].ELC-DLarray[edl].BLC;//断链值=断后里程-断前里程
	_tcscpy(GH,DLarray[edl].EGH);
	_tcscpy(GH1,DLarray[edl].BGH);
	
	TrsProjmltoNE(DLarray[edl].TYLC,PZtmp);

	B=PZtmp.a;
	pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zy);
	pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zy);
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x; 
	G_makeline(pWd,ptb,pt,QXYSColor);


	p1[X]=PZtmp.y-0.8*texth*sin(B);
	p1[Y]=PZtmp.x-0.8*texth*cos(B);

	pte[X]=p1[X]+2.4*texth*sin(B);
	pte[Y]=p1[Y]+2.4*texth*cos(B);
	p1[X]+=WB*18*sin(B+pi*0.5*zy);
	p1[Y]+=WB*18*cos(B+pi*0.5*zy);
	pte[X]+=WB*18*sin(B+pi*0.5*zy);
	pte[Y]+=WB*18*cos(B+pi*0.5*zy);

	b=B+pi*0.5;
	b=pi*0.5-b;
	if(zy==-1)b=b+pi;
	if(b<0.0)b=2*pi+b;
	if(b>2*pi) b=b-2*pi;		         
	_stprintf(RXFlag,L"%s终点 ",DLarray[edl].RXDName);


	_tcscpy(ch,RXFlag);
	_tcscat(chr,L" ");
	_tcscat(ch,LCchr(GH1,DLarray[edl].BLC,DrawParam.NLC));  // 断前里程
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);  
	G_maketext(pWd,p1,chr,b+pi,texth,QXYSColor,1);  

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH,DLarray[edl].ELC,DrawParam.NLC));     // 断后里程
	_tcscpy(chr,L" = ");
	_tcscat(chr,ch);// e.f chr=L"ZH K10+123.45"   


	if (fabs( DL)>0.001)
	{ 
		if(DL>0.0)
		{
			_tcscat(chr, L" 短链 "); 
			ads_rtos( DL,2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else if( DL<0.0)
		{
			_tcscat(chr, L" 长链 "); 
			ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else
		{/*断链值正好为0*/}
	}


	G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);
}





//  标注单个绕行段（公里百米里程标）  
//zy=1,-1  标在右边，左边 //二线断链表//断链数
void RHA::MarkiRXDLC(double dLC,int zy, int sdl, int edl )  const
{
	double offset = 0.0;//暂为0 818
	ACHAR layname[256];

	_tcscpy(layname, DrawParam.LiCheng);

	/*if (m_XLName == L"既有基线")
		_stprintf(layname,L"%s",DrawParam.LiCheng);*/
	/*else if (m_XLName == L"施工便线")
		_stprintf(layname,L"%s",BXLiCheng);
	else if (m_XLName == L"贯通设计基线")
		_stprintf(layname,L"%s",GJLiCheng);
	else if (m_XLName == L"贯通设计Ⅱ线")
	{
		if (SCALE>9.9)
			offset = m_ExOffset;
		if (HasLeft)
			_stprintf(layname,L"%s",GJLeftLiCheng);
		else if (HasRight)
			_stprintf(layname,L"%s",GJRightLiCheng);
		else 
			return;
	}
	else
		return;*/

	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,lc,b,DL,ang;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[100],chr[100],ch[100];
	ACHAR ckml[50];
	xlpoint  PZtmp;
	double cml;//连续里程
	double BZlc, DDlength;//标注里程数值dml、断链点的连续里程（非投影里程）
	double dist=0.0;

	double textH=DrawParam.OtherParam.MarkAry[2].TextHeight;

	_tcscpy(GH, DLarray[sdl].EGH);
	WB=1.5*textH;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// 断链处统一里程
	//LCchr(DLarray[sdl].EGH, DLarray[sdl].ELC, NLC, ckml);
	_tcscpy(ckml, LCchr(DLarray[sdl].EGH, DLarray[sdl].ELC, DrawParam.NLC));
	TrsCkmltoCml(ckml,DDlength);


	for (iDL=sdl;iDL<edl;iDL++)//最后一个断链为终点
	{
		//标注绕行段断链(包括绕行始)
		if (fabs(DLarray[iDL].ELC-DLarray[iDL].BLC)+0.001
				>DrawParam.m_NoteMinDLLen)
			MarkSglDL(iDL, zy);

		DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//断链值=断后里程-断前里程

		//下面开始标注里程
		zlc=(long int)(DLarray[iDL].ELC/dLC);
		BZlc=zlc*dLC;  // 实际里程 
		if((BZlc-DLarray[iDL].ELC)<0.0001)
			BZlc=BZlc+dLC;
		if ( BZlc>DLarray[iDL+1].BLC+0.0001)
		{
			DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//断链处统一里程。
			continue;
		}

		_tcscpy(GH,DLarray[iDL].EGH);  //冠号
		do   		
		{
			// 连续里程
			cml=DDlength+BZlc-DLarray[iDL].ELC; 
			// 投影里程
			//PZtmp.lc=CalcmlTonewcml(cml); 
			PZtmp.lc=TrsCmltoProjml(cml);  
			k=xlpoint_pz(array, tarray, PZtmp);

			BZW=k;    //  K<0  无解
			if (k>0 && k<=6)  //有解//线元类型
			{
				B=PZtmp.a;
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 

				pt[X]=PZtmp.y+WB*sin(B+pi*0.5*zy);
				pt[Y]=PZtmp.x+WB*cos(B+pi*0.5*zy);
				//G_makeline(pWd,ptb,pt,QXYSColor);
				makeline2(ptb,pt,-1,AcDb::kLnWtByLayer,layname);

				if (DrawParam.m_TextDir == 1)
					b=-B;
				else
					b=pi-B;

				FormFwj(b);

				ads_rtos(BZlc,2,0,chr);  
				ii=_tcslen(chr);if(ii>20) ii=19;
				if(dLC>=100.0&&ii>=2)
				{
					chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // 公里标
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH);
						_tcscat(ch,chr);
					}
					else if(ii>3 && chr[ii-3]!='0')    // 百米标
					{ 
						ch[0]=chr[ii-3];
						ch[1]='\0';
					}		    
				}
				else
				{
					_tcscpy(ch,GH);
					if ( ii<=2)
					{
						_tcscpy(ch,L"+");
						ch[1]='0';
						ch[2]=chr[ii-2];
						ch[3]=chr[ii-1];
						ch[4]='\0';
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp,L"+");
						chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2];
						chrtmp[3]=chr[ii-1];
						chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
						{
							chr[ii-3]='\0';
							_tcscpy(ch,GH);
							_tcscat(ch,chr);
							_tcscat(ch,chrtmp); 
						}
						else if(ii>3&&chr[ii-3]!='0')
						{
							_tcscpy(ch,chrtmp);
						}
					}
				}

				dist=2*WB+0.4*_tcslen(ch)*textH;//字符串中心到线路中心的距离

				pte[X]=PZtmp.y+(dist+offset)*sin(B+pi*0.5*zy)/*+0.5*textH*sin(B)*/;
				pte[Y]=PZtmp.x+(dist+offset)*cos(B+pi*0.5*zy)/*+0.5*textH*cos(B)*/;
				

				//G_maketext(pWd,pte,ch,b,textH,QXYSColor,1);
				maketext(pte,ch,b,textH,-1,3,L"宋体",layname);     

			};          
			BZlc=BZlc+dLC;
			if ( BZlc>DLarray[iDL+1].BLC) BZW=-99;		 
		}while (BZW>0);

		DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//断链处统一里程。

	}

	////标注绕行止断链
	//MarkSglDL(edl, zy);


	//标注绕行段起点
	//ACHAR RXFlag[80];

	//TrsProjmltoNE(DLarray[sdl].TYLC,PZtmp);
	//_stprintf(RXFlag,L"%s起点 ",DLarray[sdl].RXDName);
	//_tcscpy(ch,L"\0");
	//_tcscpy(ch,LCchr(DLarray[sdl].BGH,DLarray[sdl].BLC,DrawParam.NLC));     
	//_tcscat(chr,RXFlag);
	//_tcscat(chr,ch);   // e.f chr=L"ZH K10+123.45"   

	//B=PZtmp.a;
	//ptb[X]=PZtmp.y;
	//ptb[Y]=PZtmp.x;
	//pte[X]=PZtmp.y+(_tcslen(chr)*1.2*textH)*sin(B-pi*0.5*zy);
	//pte[Y]=PZtmp.x+(_tcslen(chr)*1.2*textH)*cos(B-pi*0.5*zy);
	//makeline2(ptb,pte,-1,AcDb::kLnWtByLayer,layname);
	//
	//b=B+pi*0.5;
	//b=pi*0.5-b;
	//if(zy==-1)b=b+pi;
	//if(b<0.0)b=2*pi+b;
	//if(b>2*pi) b=b-2*pi;		         
	//if(type<=1)b=b-pi*0.5;

	//pt[X]=0.5*(ptb[X]+pte[X])+0.75*textH*sin(b);
	//pt[Y]=0.5*(ptb[Y]+pte[Y])+0.75*textH*cos(b);


	//if(zy>0)
	//	ang=PZtmp.a+pi;
	//else 
	//	ang=PZtmp.a;
	//pt[X]=0.5*(ptb[X]+pte[X])+0.75*textH*sin(ang);
	//pt[Y]=0.5*(ptb[Y]+pte[Y])+0.75*textH*cos(ang);

	//maketext(pt,chr,b+pi,textH,-1,3,L"宋体",layname);


	////标注绕行段终点
	////标注绕行止断链
	MarkSglDL(edl, zy);
	////标注绕行止
	//TrsProjmltoNE(DLarray[edl].TYLC,PZtmp);
	//_stprintf(RXFlag,L"%s终点 ",DLarray[edl].RXDName);
	//_tcscpy(ch,RXFlag);
	//_tcscat(chr,L" ");
	//_tcscat(ch,LCchr(DLarray[edl].EGH,DLarray[edl].ELC,DrawParam.NLC));  // 断前里程
	//_tcscpy(chr,L"  ");
	//_tcscat(chr,ch);

	//B=PZtmp.a;
	//ptb[X]=PZtmp.y;
	//ptb[Y]=PZtmp.x;
	//pte[X]=PZtmp.y+(_tcslen(chr)*1.2*textH)*sin(B-pi*0.5*zy);
	//pte[Y]=PZtmp.x+(_tcslen(chr)*1.2*textH)*cos(B-pi*0.5*zy);
	//makeline2(ptb,pte,-1,AcDb::kLnWtByLayer,layname);

	//b=B+pi*0.5;
	//b=pi*0.5-b;
	//if(zy==-1)b=b+pi;
	//if(b<0.0)b=2*pi+b;
	//if(b>2*pi) b=b-2*pi;		         
	//if(type<=1)b=b-pi*0.5;

	//if(zy>0)
	//	ang=PZtmp.a+pi;
	//else 
	//	ang=PZtmp.a;
	//pt[X]=0.5*(ptb[X]+pte[X])+0.75*textH*sin(ang);
	//pt[Y]=0.5*(ptb[Y]+pte[Y])+0.75*textH*cos(ang);


	////G_maketext(pWd,p1,chr,b+pi,textH,QXYSColor,1);  
	//maketext(pt,chr,b+pi,textH,-1,3,L"宋体",layname);

	//_tcscpy(ch,L"\0");
	//_tcscpy(ch,LCchr(GH,DLarray[edl].ELC,NLC));     // 断后里程
	//_tcscpy(chr,L" = ");
	//_tcscat(chr,ch);// e.f chr=L"ZH K10+123.45"   


	//if (fabs( DL)>0.001)
	//{ 
	//	if(DL>0.0)
	//	{
	//		_tcscat(chr, L" 短链 "); 
	//		ads_rtos( DL,2,NLC,ch);
	//		_tcscat(chr, ch); 
	//		_tcscat(chr, L" m");
	//	}
	//	else if( DL<0.0)
	//	{
	//		_tcscat(chr, L" 长链 "); 
	//		ads_rtos(fabs(DL),2,NLC,ch);
	//		_tcscat(chr, ch); 
	//		_tcscat(chr, L" m");
	//	}
	//	else
	//	{/*断链值正好为0*/}
	//}


	////G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);
	//maketext(pte,chr,b+pi,texth,-1,3,L"宋体",layname);

	



}




/*===================================================================
函数名:MarkSglDL
函数功能:标注单个断链
输入: pWd        对象 
      iDL        断链下标
	  zy         =1,标在右边;=-1，左边
	  type       <=1,逆时针旋转90度
编写: 李帅 2016.6.20
备注：
=====================================================================*/
void RHA::MarkSglDL(AcGiWorldDraw* pWd, int iDL, int zy)  
{
	if(iDL<0 || iDL>DLarray.size()-1)
		return;

	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,lc,b,DL;
	int   k;
	ACHAR GH[8],GH1[8],chr[100],ch[100];
	ACHAR ckml[50];
	xlpoint  PZtmp;
	WB=1.5*texth;
	

	_tcscpy(ckml, DLarray[iDL].EGH);
	_stprintf(ch, L"%lf",DLarray[iDL].ELC);
	_tcscat(ckml, ch);

	DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//断链值=断后里程-断前里程
	_tcscpy(GH, DLarray[iDL].EGH);
	_tcscpy(GH1, DLarray[iDL].BGH);

	// 绘断链点
	TrsCkmltoProjml(ckml, PZtmp.lc);//投影里程 818 设计二线 idl==6 有时算不出
	k=xlpoint_pz(array, tarray, PZtmp); 

	if (k>0 && k<=6)  //有解
	{
		B=PZtmp.a;
		pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zy);
		pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zy);
		ptb[X]=PZtmp.y;
		ptb[Y]=PZtmp.x; 
		G_makeline(pWd,ptb,pt,QXYSColor);


		p1[Y] = PZtmp.x - 1.2*texth*cos(B);
		p1[X] = PZtmp.y - 1.2*texth*sin(B);

		pte[Y] = PZtmp.x + 1.2*texth*cos(B);
		pte[X] = PZtmp.y + 1.2*texth*sin(B);
		p1[X]+=WB*18*sin(B+pi*0.5*zy);
		p1[Y]+=WB*18*cos(B+pi*0.5*zy);
		pte[X]+=WB*18*sin(B+pi*0.5*zy);
		pte[Y]+=WB*18*cos(B+pi*0.5*zy);


		b=B+pi*0.5;
		b=pi*0.5-b;
		if(zy==-1)b=b+pi;
		if(b<0.0)b=2*pi+b;
		if(b>2*pi) b=b-2*pi;		         

		_tcscpy(ch,L"\0");
		_tcscpy(ch,LCchr(GH1,DLarray[iDL].BLC,DrawParam.NLC));  // 断前里程
		_tcscpy(chr,L"  ");
		_tcscat(chr,ch);  
		b+=pi;
		G_maketext(pWd,p1,chr,b,texth,QXYSColor,1);  

		_tcscpy(ch,L"\0");
		_tcscpy(ch,LCchr(GH,DLarray[iDL].ELC,DrawParam.NLC));     // 断后里程
		_tcscpy(chr,L" =");
		_tcscat(chr,ch);   // e.f chr=L"ZH K10+123.45"   

		if (fabs( DL)>0.001)
		{ 
			if(DL>0.0)
			{
				_tcscat(chr, L" 短链 "); 
				ads_rtos( DL,2,DrawParam.NLC,ch);
				_tcscat(chr, ch); 
				_tcscat(chr, L" m");
			}
			else if( DL<0.0)
			{
				_tcscat(chr, L" 长链 "); 
				ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
				_tcscat(chr, ch); 
				_tcscat(chr, L" m");
			}
			else
			{
				/*断链值正好为0*/
			}
		}
		b+=pi;
		G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);		   
	};

}



/*===================================================================
函数名:MarkSglDL
函数功能:标注单个断链
输入: 
iDL        断链下标
zy         =1,标在右边;=-1，左边
type       <=1,逆时针旋转90度
编写: 李帅 2016.6.20
备注：
=====================================================================*/
void RHA::MarkSglDL(int iDL, int zy)  const
{
	if(iDL<0 || iDL>DLarray.size()-1)
		return;

	AcGePoint3d  pt,ptb,pte;
	double  B , b;
	double DL;
	int   k;
	ACHAR GH[8],GH1[8];
	ACHAR chr1[100], chr2[100], ch[100];
	ACHAR ckml[MaxCkmlStrNum], hmCkml[MaxCkmlStrNum];//该断链的现场里程，该断链前一个百米标的现场里程
	ACHAR layname[256];
	xlpoint  PZ, hmPZ, tmpPZ;//该断链点，和其前一个整百米点
	AcGePoint3d LT,LB,RT,RB;//断链标的顶框四顶点
	double FrameH; 
	double len;//断链“门”字型标注的高度
	double offset=0.0;//818
	int iRxd=-1;
	double txtHgt;
	bool isAllLineRX;
	Int16 horizonMode;//对齐方式， =-1 右对齐 =1 左对齐

	isAllLineRX=IsAllLineRX();
	txtHgt=DrawParam.OtherParam.MarkAry[2].TextHeight;
	_tcscpy(layname, DrawParam.LiCheng);
	horizonMode=zy*DrawParam.m_TextDir;

	//_tcscpy(ckml, DLarray[iDL].EGH);
	//_stprintf(ch, L"%lf",DLarray[iDL].ELC);
	_tcscpy(ckml, LCchr(DLarray[iDL].EGH, DLarray[iDL].ELC, 3));

	DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//断链值=断后里程-断前里程
	_tcscpy(GH, DLarray[iDL].EGH);
	_tcscpy(GH1, DLarray[iDL].BGH);

	// 绘断链点
	TrsCkmltoProjml(ckml, PZ.lc);//投影里程
	k=xlpoint_pz(array, tarray, PZ); 

	if (k>0 && k<=6)  //有解
	{
		_tcscpy(ch,L"\0");
		_tcscpy(ch,LCchr(GH1,DLarray[iDL].BLC,DrawParam.NLC));  // 断前里程

		_tcscpy(chr1, L"  ");

		if(iDL == 0 && (isAllLineRX || DLarray[iDL].Indx<0))
		{
			//QD
			_tcscpy(chr1, L"  QD ");
		}
		else if(iDL == DLarray.size()-1 && (isAllLineRX || DLarray[iDL].Indx<0))
		{
			//ZD
			_tcscpy(chr1, L"  ZD ");
		}
		else if(DLarray[iDL].Indx >= 0)
		{
			if ( iDL == DLarray.size()-1 || (iDL<DLarray.size()-1  
				&& DLarray[iDL+1].Indx != DLarray[iDL].Indx))
			{
				int k;
				for (k=iDL-1; k>-1; k--)
				{
					if(DLarray[k].Indx !=  DLarray[iDL].Indx 
						|| (k==0 && DLarray[k].Indx >= 0))
					{
						_stprintf(chr1, L"  %0.1f绕行改建终点 ", DLarray[k+1].BLC/1000.0);
						break;
					}
				}

				if (k == -1)
					ads_printf(L"出平面图时未找到绕行始断链！\n");//nan
			}
			else if (iDL == 0 
				|| (iDL>0 &&  DLarray[iDL-1].Indx != DLarray[iDL].Indx))
				_stprintf(chr1, L"  %0.1f绕行改建起点 ", DLarray[iDL].BLC);
			
		}
	
		_tcscat(chr1,ch); 

		 // 断后里程
		if((isAllLineRX || DLarray[iDL].Indx<0)
			&& (iDL == 0 || iDL == DLarray.size()-1)
			&& fabs(DL)<0.001 
			&& _tcscmp(DLarray[iDL].BGH, DLarray[iDL].EGH) ==0)
		{
			_tcscpy(chr2,L"");
		}
		else
		{
			_tcscpy(ch,L"\0");
			_tcscpy(ch,LCchr(GH,DLarray[iDL].ELC,DrawParam.NLC));    
			_tcscpy(chr2,L" =");
			_tcscat(chr2,ch);   // e.f chr=L"ZH K10+123.45"   

			if (fabs( DL)>0.001)
			{ 
				if(DL>0.0)
				{
					_tcscat(chr2, L" 短链 "); 
					ads_rtos( DL,2,DrawParam.NLC,ch);
					_tcscat(chr2, ch); 
					_tcscat(chr2, L" m");
				}
				else if( DL<0.0)
				{
					_tcscat(chr2, L" 长链 "); 
					ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
					_tcscat(chr2, ch); 
					_tcscat(chr2, L" m");
				}
				else
				{
					/*断链值正好为0*/
				}
			}
		}

		FrameH=2.0*txtHgt*DrawParam.SCALE;
		len=0.7*txtHgt*max(_tcslen(chr1), _tcslen(chr2))+2.4*FrameH;

		B=PZ.a;		  
		pt[X]=PZ.y+(len+offset)*sin(B+pi*0.5*zy);
		pt[Y]=PZ.x+(len+offset)*cos(B+pi*0.5*zy);
		ptb[X]=PZ.y+offset*sin(B+pi*0.5*zy);
		ptb[Y]=PZ.x+offset*cos(B+pi*0.5*zy);
		makeline2(ptb,pt,-1,AcDb::kLnWtByLayer,layname);

		LT=pt;
		LB[X]=PZ.y+(len-FrameH+offset)*sin(B+pi*0.5*zy);
		LB[Y]=PZ.x+(len-FrameH+offset)*cos(B+pi*0.5*zy);
		LB[Z]=0.0;

		if (DrawParam.m_TextDir == 1)
			b=-B;
		else
			b=pi-B;
		
		FormFwj(b);

		//绘断前
		//if (horizonMode == -1)//右对齐
		//{
		//对齐点
			pt[X]=PZ.y+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B+pi*0.5*zy) ;
			pt[Y]=PZ.x+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B+pi*0.5*zy) ;
		//}
		//else//左对齐
		//{
		//	pt[X]=pt[X]+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B-pi*0.5*zy) ;
		//	pt[Y]=pt[Y]+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B-pi*0.5*zy) ;
		//}
		ptb=pt;
		//考虑字头方向、对齐方式，使得字符与标注线间距为0.4个字高（txtHgt）
		pt[X]=pt[X]+(0.5*DrawParam.m_TextDir+0.9)*txtHgt*sin(B+pi) ;
		pt[Y]=pt[Y]+(0.5*DrawParam.m_TextDir+0.9)*txtHgt*cos(B+pi) ;

		ptb[X]=ptb[X]+(-0.5*DrawParam.m_TextDir+0.9)*txtHgt*sin(B) ;
		ptb[Y]=ptb[Y]+(-0.5*DrawParam.m_TextDir+0.9)*txtHgt*cos(B) ;

		
		//pt[X]=/*PZ.y*/pt[X]+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B+pi*0.5*zy) ;
		//pt[Y]=/*PZ.x*/pt[Y]+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B+pi*0.5*zy) ;
		///*pt[X]=PZ.y+1.0*txtHgt*sin(B+pi) ;
		//pt[Y]=PZ.x+1.0*txtHgt*cos(B+pi) ;*/

		//pt[X]=pt[X]+0.75*txtHgt*sin(B+horizonMode*pi) ;
		//pt[Y]=pt[Y]+0.75*txtHgt*cos(B+horizonMode*pi) ;

		if (horizonMode == 1)
			maketext(pt,chr1,b,txtHgt,-1,1,L"宋体",layname); //左对齐 
		else
			maketext(pt,chr1,b,txtHgt,-1,4,L"宋体",layname);//右对齐

		//绘断后
		//tmpPZ.lc=PZ.lc+1.2*txtHgt;////Lw2007-11-05
		//xlpoint_pz(array, tarray, &tmpPZ);
		//pt[X]=tmpPZ.y+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B+pi*0.5*zy) ;
		//pt[Y]=tmpPZ.x+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B+pi*0.5*zy) ;
		///*pt[X]=PZ.y+1.0*txtHgt*sin(tmpPZ.a) ;
		//pt[Y]=PZ.x+1.0*txtHgt*cos(tmpPZ.a) ;*/
		//pt[X]=PZ.y+0.75*txtHgt*sin(B+horizonMode*pi) ;
		//pt[Y]=PZ.x+0.75*txtHgt*cos(B+horizonMode*pi) ;

		if (horizonMode == 1)
			maketext(ptb,chr2,b,txtHgt,-1,1,L"宋体",layname);  
		else
			maketext(ptb,chr2,b,txtHgt,-1,4,L"宋体",layname);


		//前一整百米,可以在曲线上
		//如果断链值为0或者是起终断链或者是并行断链，不需要找前一整百米画门字形
		if(fabs(DL)<0.001 || iDL == 0 || DLarray[iDL].Indx<0 /*|| (DLarray[iDL].Indx >= 0 && (iDL == 0 
			|| (iDL>0 && DLarray[iDL-1].Indx != DLarray[iDL].Indx)))*/)
			return;

		

		//如果该点确实是整百米就不需要找前一整百米，画门字形
		double hmBLC;
		double remainder=fmod(DLarray[iDL].BLC, 100.0);//和整百米的距离（连续里程）
		if(fmod(DLarray[iDL].BLC, 100.0) == 0.0)
			return;

		double dlDist=DLarray[iDL].TYLC-DLarray[iDL-1].TYLC;
		remainder=remainder*dlDist/(DLarray[iDL].BLC-DLarray[iDL-1].ELC);//和整百米的距离（投影里程）
		if(remainder+0.01<dlDist)
		{
			hmBLC=floor(DLarray[iDL].BLC/100.0)*100;
			_tcscpy(hmCkml, LCchr(DLarray[iDL].BGH, hmBLC, 3));
			TrsCkmltoProjml(hmCkml, hmPZ.lc);
		}
		else
		{
			_tcscpy(hmCkml, LCchr(DLarray[iDL-1].BGH, DLarray[iDL-1].BLC, 3));
			hmPZ.lc=DLarray[iDL-1].TYLC;//取不到百米标，就取前一断链
		}

		iRxd=GetiRxdByProjml(hmPZ.lc);
		if(iRxd >= 0 && iRxd == DLarray[iDL].Indx)//在同一绕行段上
		{
			k=xlpoint_pz(array, tarray, hmPZ); 
			if (k>0 && k<=6)  //前一整百米有解
			{
				AcGePoint3d cenFrm;
				double cml1=0.0, cml2=0.0, dCml=0.0;
				ACHAR str[100];

				pt[X]=hmPZ.y+(len+offset)*sin(B+pi*0.5*zy);
				pt[Y]=hmPZ.x+(len+offset)*cos(B+pi*0.5*zy);
				ptb[X]=hmPZ.y+offset*sin(B+pi*0.5*zy);
				ptb[Y]=hmPZ.x+offset*cos(B+pi*0.5*zy);
				makeline2(ptb,pt,-1,AcDb::kLnWtByLayer,layname);

				RT=pt;	
				RB[X]=hmPZ.y+(len-FrameH+offset)*sin(B+pi*0.5*zy);
				RB[Y]=hmPZ.x+(len-FrameH+offset)*cos(B+pi*0.5*zy);
				
				//绘制断链点到整百米点沿线路方向的两条线
				makeline(LT,RT,-1,AcDb::kLnWtByLayer,layname);//“门”最上面的横线
				makeline(RB,LB,-1,AcDb::kLnWtByLayer,layname);//“门”的第二道横线

				//dCml=CalnewcmlTocml(PZ.lc)-CalnewcmlTocml(hmPZ.lc);//818 dCml -9*10^61 会越界
				TrsCkmltoCml(ckml, cml1);
				TrsCkmltoCml(hmCkml, cml2);
				dCml=cml1-cml2;
				_stprintf(str,L"%.2lf",dCml);
				
				cenFrm.x=0.25*(LT.x+LB.x+RT.x+RB.x);
				cenFrm.y=0.25*(LT.y+LB.y+RT.y+RB.y);
				cenFrm.z=0;
				
				b=B;
				b=pi*0.5-b;
				FormFwj(b);
				maketext(cenFrm,str,b,txtHgt,-1,3,L"宋体",layname);  
			}

		}

	
	};

}



//注意：NDL在调用该函数前需更新
bool RHA::GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno, const vector<DLTab> &dlAry) const 
{
	sDLno=-1;
	eDLno=-1;

	int DLNum=dlAry.size();
	for(int i=0;i<DLNum;i++)
	{
		if(dlAry[i].Indx==iRXD)
		{

			sDLno=i;
			while(i<DLNum && dlAry[i].Indx==iRXD)//plh05.26
			{
				eDLno=i;			
				i++;
			}
			break;
		}
	}

	if (sDLno>-1 && eDLno>-1 && sDLno<eDLno)
		return true;
	else
	{
		ads_printf(L"绕行段断链设置错误！\n");
		return false;
	}
	
}

//注意：NDL在调用该函数前需更新
bool RHA::GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno) const 
{
	sDLno=-1;
	eDLno=-1;

	int DLNum=DLarray.size();
	for(int i=0;i<DLNum;i++)
	{
		if(DLarray[i].Indx == iRXD)
		{

			sDLno=i;
			while(i<DLNum && DLarray[i].Indx==iRXD)//plh05.26
			{
				eDLno=i;			
				i++;
			}
			break;
		}
	}

	if (sDLno>-1 && eDLno>-1 && sDLno<eDLno)
		return true;
	else
	{
		ads_printf(L"绕行段断链设置错误！\n");
		return false;
	}
}

/*===================================================================
函数名:IsAllLineRX
函数功能:是否全线绕行
返回:    如果全线绕行返回true，否则返回false
编写: 李帅 2016.8.27
=====================================================================*/
bool RHA::IsAllLineRX() const
{
	if( GetRXDNum(/*JDarray*/) == 1
		&& JDarray[0].JDXZ == RX_Start 
		&& JDarray[JDarray.size()-1].JDXZ == RX_End)
		return true;
	else
		return false;
}

Adesk::Boolean  RHA::G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLineWeight(LineWeightValue);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}

//标注特征点
Adesk::Boolean RHA::MarkTZD(AcGiWorldDraw* pWd, int zybz)  //  标注里程
// zybz==0   按曲线偏向标注   zybz=1,-1  标在右边，左边
{    
	AcGePoint3d Pt;
	AcGePoint3d  pte, ptb;
	int k;
	double B, b;
	xlpoint  PZtmp; 
	ACHAR str[80];
	double cml=0.0, lc=0.0;
	ACHAR kml[80];
	double  WB;
	//标志里程
	WB=1.5*texth;

	for(int i=1;i<JDarray.size()-1;i++)
	{
		//zybz =1;
		for(int j=0;j<4;j++)
		{
			if(j == 0)
				cml=QXB[i].KZH;
			else if(j == 1)
				cml=QXB[i].KHY;
			else if(j == 2)
				cml=QXB[i].KYH;
			else
				cml=QXB[i].KHZ;

			PZtmp.lc = cml;
			k=xlpoint_pz(array,tarray,PZtmp); 
			TrsProjmltoCkml(cml,kml,3);

			if(j == 0)
				_tcscpy(QXB[i].CKZH, kml);
			else if(j == 1)
				_tcscpy(QXB[i].CKHY, kml);
			else if(j == 2)
				_tcscpy(QXB[i].CKYH, kml);
			else
				_tcscpy(QXB[i].CKHZ, kml);

			B=PZtmp.a;//fwj
			//点起点
			Pt[Y]=PZtmp.x; 
			Pt[X]=PZtmp.y;
			Pt[Z]=0.0;

			//字起点
			ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
			ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);

			ptb[Y] = ptb[Y] - 0.25*texth*cos(B);
			ptb[X] = ptb[X] - 0.25*texth*sin(B);

			if( JDarray[i].R > 1.0e-5 )
			{
				if( JDarray[i].l1 > 1.0e-5 )
				{
					if(j==0) _stprintf(str,L"ZH %s",kml);
					if(j==1) _stprintf(str,L"HY %s",kml);
				}
				else
				{
					if(j==0) continue ;
					if(j==1) _stprintf(str,L"ZY %s",kml);
				}

				if( JDarray[i].l2 > 1.0e-5 )
				{
					if(j==2) _stprintf(str,L"YH %s",kml);
					if(j==3) _stprintf(str,L"HZ %s",kml);
				}
				else
				{
					if(j==2) _stprintf(str,L"YZ %s",kml);
					if(j==3) continue ;
				}

			}
			else continue ;

			b=0.0;//旋转角度
			if(zybz>0)
			{
				b =pi/2.0-( B +pi*0.5*zybz) + pi ;
				if (b<0.0) b=2*pi+b;		 
				if (b>2*pi) b=b-2*pi;
			}
			else 
			{
				b =pi/2.0-( B +pi*0.5*zybz) ;
				if (b<0.0) b=2*pi+b;		 
				if (b>2*pi) b=b-2*pi;
			}

			//点终点
			pte[X]=PZtmp.y+(_tcslen(str)*1.2*texth)*sin(B+pi*0.5*zybz);
			pte[Y]=PZtmp.x+(_tcslen(str)*1.2*texth)*cos(B+pi*0.5*zybz);

			G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);//线
			if(zybz>0)
				G_maketext(pWd,pte,str,b,1.5*texth,QXYSColor,1);//文字
			else 
				G_maketext(pWd,ptb,str,b,1.5*texth,QXYSColor,1);

		}
	}

	return TRUE;
}


//标注特征点
Adesk::Boolean RHA::MarkTZD() const  //  标注里程
// zybz==0   按曲线偏向标注   zybz=1,-1  标在右边，左边
{    
	AcGePoint3d pt;
	AcGePoint3d  pte, ptb;
	int k;
	double B, b;
	xlpoint  PZtmp; 
	double projml=0.0, lc=0.0;
	ACHAR layerName[256];
	ACHAR str[80], kml[80];
	bool isJYX=false;
	double y, x, fwj;

	double txtHgt=DrawParam.OtherParam.MarkAry[1].TextHeight;
	int zybz=DrawParam.OtherParam.MarkAry[1].LorR;
	int textDir=DrawParam.m_TextDir;

	if(XLShuXing == JYJX || XLShuXing == JY2X)
		isJYX=true;

	_tcscpy(layerName, DrawParam.TeZhengDian); 
	//zybz=0;//818 曲线内侧？ 
	for(int i=1;i<JDarray.size()-1;i++)
	{
		//if (zybz==0)//曲线内侧
		//	zybz=QXB[i].LorR;

		//zybz =1;
		for(int j=0;j<4;j++)
		{
			if(j == 0)
				projml=QXB[i].KZH;
			else if(j == 1)
				projml=QXB[i].KHY;
			else if(j == 2)
				projml=QXB[i].KYH;
			else
				projml=QXB[i].KHZ;

			PZtmp.lc = projml;
			k=xlpoint_pz(array,tarray,PZtmp); 
			TrsProjmltoCkml(projml,kml,3);

		/*	if(j == 0)
				_tcscpy(QXB[i].CKZH, kml);
			else if(j == 1)
				_tcscpy(QXB[i].CKHY, kml);
			else if(j == 2)
				_tcscpy(QXB[i].CKYH, kml);
			else
				_tcscpy(QXB[i].CKHZ, kml);*/

			B=PZtmp.a;//fwj
			//点起点
			ptb[Y]=PZtmp.x; 
			ptb[X]=PZtmp.y;
			ptb[Z]=0.0;

			if( JDarray[i].R > 1.0e-5 )
			{
				if( JDarray[i].l1 > 1.0e-5 )
				{
					if(j==0) _stprintf(str,L"ZH %s",kml);
					if(j==1) _stprintf(str,L"HY %s",kml);
				}
				else
				{
					if(j==0) continue ;
					if(j==1) _stprintf(str,L"ZY %s",kml);
				}

				if( JDarray[i].l2 > 1.0e-5 )
				{
					if(j==2) _stprintf(str,L"YH %s",kml);
					if(j==3) _stprintf(str,L"HZ %s",kml);
				}
				else
				{
					if(j==2) _stprintf(str,L"YZ %s",kml);
					if(j==3) continue ;
				}

			}
			else continue ;

			

			//点终点
			pte[X]=PZtmp.y+(_tcslen(str)*0.8*txtHgt)*sin(B+pi*0.5*zybz);
			pte[Y]=PZtmp.x+(_tcslen(str)*0.8*txtHgt)*cos(B+pi*0.5*zybz);

			//G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
			makeline2(ptb,pte,-1,AcDb::kLnWtByLayer, layerName);//线

			//绘制废弃段
			if (isJYX && DrawParam.m_IsDrawFQD && !IsPtOnLYD(PZtmp.lc))
			{
				y = 0.5 * (ptb[Y] + pte[Y]);
				x = 0.5 * (ptb[X] + pte[X]);
				xyddaa(ptb[Y],ptb[X],pte[Y],pte[X],&fwj);
				DrawOneFQDBZ(y,x,fwj,0.2*_tcslen(str)*txtHgt,MAINLINE);
			}

			//if(zybz>0)
			//	G_maketext(pWd,pte,str,b,1.5*txtHgt,QXYSColor,1);//文字
			//else 
			//	G_maketext(pWd,ptb,str,b,1.5*txtHgt,QXYSColor,1);

			//if(zybz>0)
			//	b=B+pi;
			//else 
			//	b=B;

			pt[X]=0.5*(ptb[X]+pte[X])+0.75*txtHgt*sin((textDir-1)/2.0*pi+B);
			pt[Y]=0.5*(ptb[Y]+pte[Y])+0.75*txtHgt*cos((textDir-1)/2.0*pi+B);

			if (DrawParam.m_TextDir == 1)//旋转角度
				b=-B;
			else
				b=pi-B;

			FormFwj(b);

			/*if(zybz>0)
			{
				b =pi/2.0-( B +pi*0.5*zybz) + pi ;
				if (b<0.0) b=2*pi+b;		 
				if (b>2*pi) b=b-2*pi;
			}
			else 
			{
				b =pi/2.0-( B +pi*0.5*zybz) ;
				if (b<0.0) b=2*pi+b;		 
				if (b>2*pi) b=b-2*pi;
			}*/

			//文字
			maketext(pt,str,b,txtHgt,-1,3,L"宋体",layerName);

		
		}
	}

	return TRUE;
}


//绘直线（设置线宽）
Adesk::Boolean  RHA::G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLineWeight(lw);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}

void RHA::checkJDNum()
{
	CString JDnum1,JDnum2;
	for (int i=1;i<JDarray.size()-1;i++)
	{
		JDnum1=JDarray[i].JDName;
		JDnum2=JDarray[i-1].JDName;
		JDnum1.Trim();
		JDnum2.Trim();
		if (JDnum1==JDnum2)
			ads_printf(L"警告%s与前一交点交点号相同，默认会按复曲线处理！\n",JDnum1);
	}
}



/*===================================================================
函数名:SaveData   
------------------------------------------------------------
函数功能:在线路修改前，保存交点/断链/标志里程数据
------------------------------------------------------------
输出: 
------------------------------------------------------------
编写: 李帅 2016.7.16 
=====================================================================*/
void RHA::SaveData()
{
	//计算线元
	getpxy_DATA();
	//计算DL坐标
	CalDL_NE();

	OldJDarray.clear();
	OldDLarray.clear();
	OldBZLCarray.clear();

	int NJD=JDarray.size();
	int NDL=DLarray.size();
	OldJDarray.resize(NJD);
	OldDLarray.resize(NDL);

	for (int i=0;i<NJD;i++)
		OldJDarray[i]=JDarray[i];

	for (int i=0;i<NDL;i++)
		OldDLarray[i]=DLarray[i];

	for (int i=0;i<BZLCarray.size();i++)
	{
		if (BZLCarray[i].isAutoAdd == false)
			OldBZLCarray.push_back(BZLCarray[i]);
	}

}
/*===================================================================
函数名:UpdateData   
------------------------------------------------------------
函数功能:解算交点更新二线交点和断链数据
------------------------------------------------------------
输入:
sjd            在sjd后insrtBxJdNum个交点的性质都为并行或并行不平行
insrtBxJdNum   插入的并行交点数( 818 考虑多个并行不平行段和S弯)
			  【前两个参数只在插入临时线时用】

OldJDarray     原始交点
OldDLarray     原始断链
oldBZLCarray   原始标志里程
------------------------------------------------------------
输出: 
JDarray        交点
DLarray        断链
BZLCarray      标志里程
------------------------------------------------------------
备注: 
------------------------------------------------------------
编写: 李帅 2016.7.6 
=====================================================================*/
void RHA::UpdateAllData(/*int sJD, int insrtBxJdNum*/)
{
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		//计算二线参照关系，形成二线交点数据表
		CalExReference();
		
		//修正交点坐标
		ModifyExJDXY();
	}
	else if(!IsMove)
	{//不是拖动交点

		//全线绕行赋交点性质(交点拖动时不需要)
		for (int i=0; i<JDarray.size(); i++)
		{
			JDarray[i].JDXZ = RX_One;
			_tcscpy(JDarray[i].BLName, L"");
			_tcscpy(JDarray[i].BLJDName, L"");
			_tcscpy(JDarray[i].Draw_JDName, L"");
			_tcscpy(JDarray[i].ZHLC, L"");
			JDarray[i].XJJ=0.0;
		}
		JDarray[0].JDXZ=RX_Start;
		JDarray[JDarray.size()-1].JDXZ=RX_End;
	}
	
	//计算线元
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);
	
	//nan

	/*ofstream outfile;
	outfile.open("d:\\array.txt");
	outfile << "********************array********************" << endl;
	outfile << left<<setw(10) << "0" << setw(10) << "1" << setw(10) << "2"
		<< setw(10)<<"3"<< setw(10) << "4" <<setw(10) << "5" << setw(10) << "6" 
		<< setw(10) << "7"<<setw(10) << "8" << setw(10) << "9"<<endl;
	for (int i = 0;i<tarray;i++)
	{ 
		outfile << left<<setw(10) << array[i][0] << setw(10) << array[i][1] << setw(10) << array[i][2]
		<< setw(10)<< array[i][3]<< setw(10) << array[i][4] <<setw(10) << array[i][5] << setw(10) << array[i][6] 
		<< setw(10) << array[i][7]<<setw(10) << array[i][8] << setw(10) << array[i][9]<<endl;
	}

	outfile << "\n" << endl;
	outfile.close();*/

	//自动生成断链
	GenDL();

	//GenBZLCbyDL(true);
	//CalBZLC();//818 是否计算？

	//B 考虑保存断链
	CmpNewDLwithOld(); 
	if(XLShuXing != SJJX && XLShuXing != SJ2X)
		GenBZLCbyDL(false);
	else
		BZLCarray.clear();


	//if (XLShuXing == JYJX || XLShuXing == JY2X)
	//{
	//	//CmpNewBZLCwithOld();
	//	CalBZLC();//818 是否计算？
	//}

}

/*===================================================================
函数名:    UpdateRxdDL   
函数功能:  根据线位的变化重新计算绕行段断链（尽可能保留原有断链的现场里程）
输入:
iRXD            绕行段编号（全线绕行为0）
输出:         
算法:      
备注: 
编写:      李帅 2017.6.20 
=====================================================================*/
void RHA::UpdateRxdDL(int iRXD)
{
	int jdNum=JDarray.size();
	if (jdNum<2)
	{
		ads_printf(L"错误：交点数小于2\n");
		return;
	}

	ACHAR  rxdName[50];
	_tcscpy(rxdName, L"");

	if (XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX
		|| (GetRXDNum() == 1 && JDarray[0].JDXZ == RX_Start && JDarray[jdNum-1].JDXZ == RX_End))
		//818 二线之前也是全线绕行的判断需要吗？
	{//全线绕行
		int dlNum=0;
		double xlLen=0.0;
		double eptELC=0.0;
		ACHAR eptEGH[16];


		xlLen=QXB[jdNum-1].KHZ;//线路长
		dlNum=DLarray.size();

		if(dlNum<2)
			return;

		//终点断后
		_tcscpy(eptEGH, DLarray[dlNum-1].EGH);
		eptELC=DLarray[dlNum-1].ELC;


		if (DLarray[dlNum-1].TYLC+0.001>xlLen)
		{//线路长度变小，断链的几何里程大于线路长度的都删除
			for (int i=dlNum-2; i>0; i--)
			{
				if (DLarray[i].TYLC+0.001>xlLen)
					DLarray.erase(DLarray.begin()+i);
			}
		}
		dlNum=DLarray.size();


		//修改终点断链
		_tcscpy(DLarray[dlNum-1].BGH, DLarray[dlNum-2].EGH);//断前冠号		
		DLarray[dlNum-1].TYLC = xlLen;//投影里程
		DLarray[dlNum-1].BLC = DLarray[dlNum-2].ELC + DLarray[dlNum-1].TYLC - DLarray[dlNum-2].TYLC;//断前里程
		_tcscpy(DLarray[dlNum-1].EGH, eptEGH);//断后冠号
		DLarray[dlNum-1].ELC=eptELC;//断后里程

	}
	else if (iRXD >= 0)
	{//二线绕行段
		int sjd, ejd;//绕行始、止交点下标
		int sdl, edl;
		vector<DLTab> rxdDLarray;


		//起点绕行 818

		//终点绕行

		//确定起终断链点，检查原有的起终点断链是否在直线上
		//，如果不在重新设置断链。注意：默认不改变前后参照线
		bool isUseOldSdl=false;
		bool isUseOldEdl=false;
		DLTab sptDL, eptDL;
		int dlNum=0;
		double oldRxdLen=0.0;
		double newRxdLen=0.0;
		double len=0.0;


		eptDL.TYLC=sptDL.TYLC=0.0;

		//绕行起始交点
		GetRxdSEJD(iRXD, sjd, ejd, JDarray);

		//绕行起始断链
		GetiRXDDLinfo(iRXD, sdl, edl, DLarray);

		oldRxdLen=DLarray[edl].TYLC-DLarray[sdl].TYLC;//原有绕行段长度

		//取出绕行段断链
		rxdDLarray.assign(DLarray.begin()+sdl, DLarray.begin()+edl+1);//818 +1? of course
		dlNum=rxdDLarray.size();

		isUseOldSdl=CheckRxdDL(iRXD, true);
		if (isUseOldSdl)
			sptDL=DLarray[sdl];
		else
		{
			//自动产生断链
			GenRXDL(iRXD, sjd, sptDL, true);
			
			////复制之前的绕行段起点断后冠号 
			_tcscpy(sptDL.EGH, DLarray[sdl].EGH);
			//sptDL.ELC=DLarray[sdl].ELC;   818 起点绕行时？
		}


		//绕行终止断链
		isUseOldEdl=CheckRxdDL(iRXD, false);
		if (isUseOldEdl)
			eptDL=DLarray[edl];
		else
			//自动产生断链
			GenRXDL(iRXD, ejd, eptDL, false);

		//删除超过绕行段长度的绕行段中间断链
		//，并计算保留的断链的几何里程
		newRxdLen=eptDL.TYLC-sptDL.TYLC;
		len=0.0;
		for(int i=1; i<dlNum-1; i++)
		{
			len += rxdDLarray[i].BLC-rxdDLarray[i-1].ELC;
			if(len+0.001>newRxdLen)
			{//该断链超过绕行终点断链，删除
				rxdDLarray.erase(rxdDLarray.begin()+i);
				i--;
				dlNum--;
			}
			else
				rxdDLarray[i].TYLC = sptDL.TYLC+len;
		}

		//修改起终点断链
		rxdDLarray[0]=sptDL;
		rxdDLarray[dlNum-1]=eptDL;

		//修改终点断链的断前里程和冠号
		_tcscpy(rxdDLarray[dlNum-1].BGH, rxdDLarray[dlNum-2].EGH);

		rxdDLarray[dlNum-1].BLC=rxdDLarray[dlNum-2].ELC+eptDL.TYLC-rxdDLarray[dlNum-2].TYLC;

		//删除原来的绕行段断链
		DLarray.erase(DLarray.begin()+sdl, DLarray.begin()+edl+1);

		//插入新的断链
		_stprintf(rxdName, L"%0.1lf绕行段", rxdDLarray[0].ELC/1000.0);

		for (int k=dlNum-1;k>-1;k--)
		{
			rxdDLarray[k].Indx=iRXD;//绕行段编号
			_tcscpy(rxdDLarray[k].RXDName, rxdName);//绕行段名
			DLarray.insert(DLarray.begin()+sdl, rxdDLarray[k]);
		}
	}
	
}

/*===================================================================
函数名:    UpdateRxdBZLC   
函数功能:  更新绕行段标志里程
输入:
iRXD            绕行段编号（全线绕行为0）
bzlcArray       原来用户设置的标志里程
输出:         
算法:      根据线位的变化检查老的标志里程是否保留，并给新的断链上默认标志里程    
备注:      调用该函数前，需要更新断链
编写:      李帅 2017.6.20 
=====================================================================*/
void RHA::UpdateRxdBZLC(int iRXD, 	vector<BZLCStructTable> &bzlcArray)
{
	if (XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	int sdl, edl;
	vector<DLTab> rxdDLarray;
	ACHAR  rxdName[50];

	//绕行起始断链
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);

	//获取绕行段断链
	rxdDLarray.assign(DLarray.begin()+sdl, DLarray.begin()+edl+1);

	_tcscpy(rxdName, DLarray[sdl].RXDName);//绕行段名


	//检查标志里程是否保留 //818 本段函数和DLEDITDLG::SAVEDL() RHA::CmpNewBZLCwithOld()有相同
	CString str;
	ACHAR gh[16], ckml[50];
	double xclc=0.0;
	double dist=0.0;
	bool isDel=true;//是否删除标志里程
	int iDL=-1;
	double dml=0;
	ACHAR  dlgh[20];


	for (int i=0; i<bzlcArray.size(); i++)
	{
		isDel=true;

		//如果标志里程不在线路上，那么删除
		//计算到线路的距离和投影里程
		dist=DistToXL(array, tarray, bzlcArray[i].X, bzlcArray[i].Y, bzlcArray[i].xyToprojml);

		if (dist<0.01)
		{//在线路上

			iDL=FindNearDL(bzlcArray[i].xyToprojml, rxdDLarray, true);
			if(iDL>-1)
			{
				split_ckml(bzlcArray[i].CKml, dml, gh);
				_tcsupr(gh);
				_tcscpy(dlgh, rxdDLarray[iDL].EGH);

				//如果标志里程和前后断链的冠号、里程不匹配，那么删除
				if(_tcscmp(gh, dlgh) == 0
					&& dml-0.0001>rxdDLarray[iDL].ELC 
					&& dml+0.0001<rxdDLarray[iDL+1].BLC)
				{
					//冠号与前一断链一致、 里程在断链区间
					isDel=false;
				}
				else
				{
					str.Format(L"标志里程%s和前后断链的冠号、里程不匹配，自动删除\n", bzlcArray[i].CKml);
					ads_printf(str);
				}
			}
			else
			{
				str.Format(L"标志里程%s没找到前一个断链，自动删除\n", bzlcArray[i].CKml);
				ads_printf(str);
			}

		}
		else
		{
			str.Format(L"标志里程%s不在线路上，自动删除\n", bzlcArray[i].CKml);
			ads_printf(str);
		}

		if(isDel)
		{//删除标志里程
			bzlcArray.erase(bzlcArray.begin()+i);
			i--;
		}
	}

	
	//插入新断链产生的标志里程
	BZLCStructTable tmpBZLC;
	xlpoint PZ;

	_tcscpy(tmpBZLC.XLName, m_XLName);
	tmpBZLC.isAutoAdd=true;
	tmpBZLC.K=1.0;
	_tcscpy(tmpBZLC.GJDNum, rxdName);
	_tcscpy(tmpBZLC.XLName, m_XLName);

	for(int i=0;i<rxdDLarray.size();i++) 
	{
		tmpBZLC.xyToprojml=rxdDLarray[i].TYLC;
		PZ.lc=rxdDLarray[i].TYLC;
		xlpoint_pz(array, tarray, PZ);
		tmpBZLC.X=PZ.x;
		tmpBZLC.Y=PZ.y;
		_tcscpy(tmpBZLC.CKml, LCchr(rxdDLarray[i].BGH, rxdDLarray[i].BLC, 3)); 
		tmpBZLC.kmlTocml=TYLC1(tmpBZLC.CKml, rxdDLarray);//该函数只适用于绕行段

		BZLCarray.push_back(tmpBZLC);
	}

	//插入原有的标志里程
	if (bzlcArray.size()>0)
	{
		for (int i=0;i<bzlcArray.size();i++)
		{
			_tcscpy(bzlcArray[i].GJDNum, rxdDLarray[0].RXDName);
			BZLCarray.push_back(bzlcArray[i]);
		}
	}

		
	
}

/*===================================================================
函数名:    UpdateSglRxdLC   
函数功能:  更新某绕行段的断链和标志里程
输入:
iRXD            绕行段编号（全线绕行为0）
isCalXYarray    是否计算线元
输出:         
算法:      
备注:      注意本函数没有计算这个绕行段之后的其它断链的投影里程
			，所以调用本函数之后，需要调用函数 CalDLprojml（）
编写:      李帅 2017.6.30 
=====================================================================*/
void RHA::UpdateSglRxdLC(int iRXD, bool isCalXYarray)
{
	//计算线元
	if (isCalXYarray)
		getpxy_DATA();

	//处理标志里程数据
	vector<BZLCStructTable> bzlcArray;

	if (XLShuXing != SJJX || XLShuXing != SJ2X)
	{
		//获取标志里程数组(用户设置)
		GetiRxdBZLCArray(iRXD, bzlcArray);

		//删除该绕行段原有的标志里程(包括起终标志里程)
		DeleteiRXDBZLC(iRXD, true);
	}

	//根据线位的变化重新计算绕行段断链
	UpdateRxdDL(iRXD);

	//更新绕行段标志里程
	UpdateRxdBZLC(iRXD, bzlcArray);
}

/*===================================================================
函数名:    UpdateLC   
函数功能:   更新某绕行段的断链和标志里程
			并且重新计算该绕行段之后的断链的投影里程
输入:
iRXD            绕行段编号（全线绕行为0）
输出:         
算法:      
备注: 
编写:      李帅 2017.6.20 
=====================================================================*/
void RHA::UpdateLC(int iRXD)
{
	////计算线元
	//getpxy_DATA();

	////处理标志里程数据
	//vector<BZLCStructTable> bzlcArray;

	//if (XLShuXing != SJJX || XLShuXing != SJ2X)
	//{
	//	//获取标志里程数组(用户设置)
	//	GetiRxdBZLCArray(iRXD, bzlcArray);

	//	//删除该绕行段原有的标志里程(包括起终标志里程)
	//	DeleteiRXDBZLC(iRXD, true);
	//}

	////根据线位的变化重新计算绕行段断链
	//UpdateRxdDL(iRXD);

	////更新绕行段标志里程
	//UpdateRxdBZLC(iRXD, bzlcArray);

	//更新这个绕行段的断链和标志里程
	UpdateSglRxdLC(iRXD);

	//绕行起始断链
	int sdl=0, edl=0;
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);

	//重新计算该绕行段之后的其它断链的投影里程
	CalDLprojml(edl+1);
}

/*===================================================================
函数名:     CalDLprojml   
函数功能:   从某断链开始，计算之后的所有断链的投影里程
输入:
iStart      从该断链开始计算之后的所有断链的投影里程
输出:       类变量的断链数组的投影里程    
算法:      
备注:      如果线位变化，该函数需要在getpxy_DATA()函数后使用  818 最好？
编写:      李帅 2017.6.28 
=====================================================================*/
void RHA::CalDLprojml(int iStart)
{
	getpxy_DATA();

	if (iStart<0 || iStart+1>DLarray.size())
	{
		//ads_printf(L"计算断链投影里程时输入下标错误\n");
		return;
	}

	int sdl=0, edl=0;

	//如果iStart是绕行中断链，那么取到绕行始断链开始计算
	if (iStart>0 && DLarray[iStart].Indx>-1 
		&& DLarray[iStart-1].Indx == DLarray[iStart].Indx)
	{
		iStart--;
		for(int i=iStart-1; i>-1; i--)
		{
			if (DLarray[i].Indx != DLarray[i+1].Indx)
			{
				iStart=i+1;
				break;
			}
		}

	}

	//依次计算断链的投影里程
	for(int i=iStart; i<DLarray.size(); i++)
	{
		if (DLarray[i].Indx == -3)//并行段提取的断链
			continue;
		else if (DLarray[i].Indx == -2 || DLarray[i].Indx == -1)//并行段分界点 或 起终点并行断链
			TrsCkmltoProjml(DLarray[i].BGH, DLarray[i].BLC, DLarray[i].TYLC);
		else if (DLarray[i].Indx >= 0)
		{
			GetiRXDDLinfo(DLarray[i].Indx, sdl, edl, DLarray);

			if(sdl>=edl || sdl != i)
			{
				ads_printf(L"获取绕行起终断链编号错误\n");
				continue;
			}

			//绕行起点
			if (sdl == 0)
				//起点绕行
				DLarray[sdl].TYLC=0.0;
			else
				TrsCkmltoProjml(DLarray[sdl].BGH, DLarray[sdl].BLC, DLarray[sdl].TYLC);

			//中间
			for (int j=sdl+1; j<edl; j++)
				DLarray[j].TYLC=DLarray[j-1].TYLC+DLarray[j].BLC-DLarray[j-1].ELC;

			//绕行终点
			if(edl+1 == DLarray.size())
			{//终点绕行
				int jdNum=JDarray.size();
				DLarray[edl].TYLC=QXB[jdNum-1].KHZ;
			}
			else
				TrsCkmltoProjml(DLarray[edl].EGH, DLarray[edl].ELC, DLarray[edl].TYLC);

			//根据长度修正绕行终点断链的断前
			DLarray[edl].BLC=DLarray[edl-1].ELC+DLarray[edl].TYLC-DLarray[edl-1].TYLC;

			i=edl;
		}
	}

}



/*===================================================================
函数名:    CheckRxdDL   
函数功能:  在线位变动后，检查某绕行段起终点断链是否仍然适合
			，如果合适重新计算该断链的几何里程
输入:
		iRXD                 绕行段编号
		isSDLorEDL           =true，检查绕行起点断链；=false，检查绕行终点断链
输出:   DLarray  
返回:   如果绕行段起终点断链仍然适合，返回true
算法:   
备注: 
编写:      李帅 2017.6.20 
=====================================================================*/
bool RHA::CheckRxdDL(int iRXD, bool isSDLorEDL)
{
	if(iRXD<0 || iRXD>=GetRXDNum())//818 删除绕行段后，iRXD需要更新
		return false;

	int sjd, ejd;//绕行始、止交点下标
	int sdl, edl;
	bool isRight=false;//
	double strtProjml1x=-1.0, strtProjml2x=-1.0;
	double endProjml1x=-1.0, endProjml2x=-1.0;

	isRight=false;
	//绕行起始交点
	GetRxdSEJD(iRXD, sjd, ejd, JDarray);

	//绕行起始断链
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
	
	if (isSDLorEDL)
	{//检验原有的绕行起点断链

		for (int k=0;k<1;k++)//虚循环
		{
			//如果是线路起点绕行，沿用起点断链
			if (sjd == 0 && sdl == 0)
			{
				DLarray[sdl].TYLC=0.0;
				isRight=true;
				break;
			}

			//判断 断链 在一线的投影是否在绕行段上
			RHA *pStRHA_BL=NULL;//起点参照线
			pStRHA_BL=NULL;

			pStRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[sdl].BLName);
			if(pStRHA_BL == NULL)
			{
				ads_printf(L"找不到参照线\n");
				break;
			}

			if(!pStRHA_BL->TrsCkmltoProjml(DLarray[sdl].BGH, DLarray[sdl].BLC, strtProjml1x))
			{
				ads_printf(L"错误：在参照线上找不到对应的现场里程\n");
				break;
			}

			if (pStRHA_BL->GetiRxdByProjml(strtProjml1x)<0)
				 break;

			//断链在一线的投影是否在对应的夹直线上
			double KZH1x, KHZ1x;
			int iJD1x;
			if(sjd>0)
				pStRHA_BL->GetJdno1FromJdstr(JDarray[sjd-1].BLJDName
				, pStRHA_BL->JDarray, iJD1x);
			else
			{
				pStRHA_BL->GetJdno1FromJdstr(JDarray[sjd].BLJDName
					, pStRHA_BL->JDarray, iJD1x);
				iJD1x--;
			}


			if (iJD1x>-1)
			{
				KZH1x=pStRHA_BL->QXB[iJD1x+1].KZH;
				KHZ1x=pStRHA_BL->QXB[iJD1x].KHZ;

				if (!(strtProjml1x+0.001<KZH1x && strtProjml1x>KHZ1x+0.001))
				{//不在夹直线上
					break;
				}
			}


			//判断 断链 是否在二线的夹直线上
			if (!CalXJJ(strtProjml1x, pStRHA_BL->array, pStRHA_BL->tarray, array, tarray, strtProjml2x, true))
				break;

			if (sjd>0 && !(strtProjml2x+0.0001<QXB[sjd].KZH
				&& strtProjml2x>QXB[sjd-1].KHZ+0.0001))
				break;

			//断链是否与二线的前一个断链重叠 818 前一个断链是否这时已经更新？
			if(sdl>0 && strtProjml2x+0.0001<DLarray[sdl-1].TYLC)
				break;

			//检查后发现该断链可以保留，更新该断链的几何里程
			isRight=true;
			DLarray[sdl].TYLC=strtProjml2x;
		}
	}
	else
	{
		//检验绕行终点断链
		for (int k=0;k<1;k++)//虚循环
		{

			//线路终点绕行，保留断链
			if (ejd == JDarray.size()-1)
			{
				isRight=true;
				DLarray[edl].TYLC=xl_length;
				break;
			}

			//断链在一线的投影是否在绕行段上
			RHA *pEndRHA_BL=NULL;//终点参照线
			pEndRHA_BL=NULL;

			pEndRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[edl].BLName);
			if(pEndRHA_BL == NULL)
			{
				ads_printf(L"错误：找不到参照线\n ");
				break;
			}

			if(!pEndRHA_BL->TrsCkmltoProjml(DLarray[edl].EGH, DLarray[edl].ELC, endProjml1x))
			{
				ads_printf(L"错误：在参照线上找不到对应的现场里程\n");
				break;
			}

			if (pEndRHA_BL->GetiRxdByProjml(endProjml1x)<0)
				break;

			//断链在一线的投影是否在对应的夹直线上
			double KZH1x, KHZ1x;
			int iJD1x;

			if(ejd<JDarray.size()-1 && (JDarray[ejd+1].JDXZ == BXPX 
				|| JDarray[ejd+1].JDXZ == BX_NotPX) )//818 并行不平行交点信息的纪录？
			{
				GetJdno1FromJdstr(JDarray[ejd+1].BLJDName
					, pEndRHA_BL->JDarray, iJD1x);
				iJD1x--;
			}
			else
				GetJdno1FromJdstr(JDarray[ejd+1].BLJDName//818 JDarray[ejd].BLJDName???
				, pEndRHA_BL->JDarray, iJD1x);


			if (iJD1x>-1 && iJD1x+1<pEndRHA_BL->QXB.size())
			{
				KHZ1x=pEndRHA_BL->QXB[iJD1x].KHZ;
				KZH1x=pEndRHA_BL->QXB[iJD1x+1].KZH;

				if (!(endProjml1x+0.001<KZH1x && endProjml1x>KHZ1x+0.001))
				{//不在夹直线上
					break;
				}
			}


			//断链是否在二线的夹直线上
			if (!CalXJJ(endProjml1x, pEndRHA_BL->array, pEndRHA_BL->tarray, array, tarray, endProjml2x, true))
				break;

			if (ejd<JDarray.size()-1 && !(endProjml2x>QXB[ejd].KHZ+0.0001
				&& endProjml2x+0.0001<QXB[ejd+1].KZH))
			{//不在夹直线上
				break;
			}

			//断链是否与二线的后一个断链重叠 818 后一个断链是否更新
			/*if(ejd<JDarray.size()-1 && endProjml2x>DLarray[edl+1].TYLC+0.0001)
			{
				isRight=false; break;
			}*/

			//检查后发现该断链可以保留，更新该断链的几何里程
			isRight=true;
			DLarray[edl].TYLC=endProjml2x;

		}

	}

	return isRight;
	
}

/*===================================================================
函数名:    CmpNewBZLCwithOld   
函数功能:  在一个绕行段上，根据更新的断链数组（线路改动）检验原有用户添加的标志里程
		   是否保留，返回要保留的标志里程数组。
输入:
		   OldBZLCarray    之前的全线的标志里程
		   dlAry           一个绕行段的断链数组
输出:      bzlcAry   
算法:      如果用户给的标志里程还在线路上
           ，并且冠号与前一断链一致、 里程在断链区间就保留 
备注: 
编写:      李帅 2016.7.20 
=====================================================================*/
void RHA::CmpNewBZLCwithOld(vector<DLTab> &dlAry, vector<BZLCStructTable> &bzlcAry)
{
	bzlcAry.clear();

	if (dlAry.size()<2 || XLShuXing == SJJX || XLShuXing == SJ2X)
		return;


	double dist=100;
	double projml=0;
	xlpoint pz;
	int iDL=0;
	double dml=0;
	ACHAR gh[20], dlgh[20];

	for (int i=0;i<OldBZLCarray.size();i++)
	{
		if (OldBZLCarray[i].isAutoAdd == false)
		{
			
			pz.lc=PROJ_ML(array, tarray, OldBZLCarray[i].X, OldBZLCarray[i].Y
				, OldBZLCarray[i].X, OldBZLCarray[i].Y);//818 求标志里程投影点坐标差别很大，点没给对？

			if (pz.lc+0.0001<dlAry[0].TYLC 
				|| pz.lc-0.0001>dlAry[dlAry.size()-1].TYLC)
			{
				//不在断链区间
				continue;
			}

			xlpoint_pz(array, tarray, pz);
			dist=sqrt(pow((OldBZLCarray[i].X-pz.x),2.0)
				+pow((OldBZLCarray[i].Y-pz.y),2.0));
			if (dist<0.01)
			{
				//在线路上
				iDL=FindNearDL(pz.lc, DLarray, true);
				if(iDL>-1)
				{
					split_ckml(OldBZLCarray[i].CKml, dml, gh);
					_tcsupr(gh);
					_tcscpy(dlgh, DLarray[iDL].EGH);
					if(_tcscmp(gh, dlgh) == 0
						&& dml-0.0001>DLarray[iDL].ELC 
						&& dml+0.0001<DLarray[iDL+1].BLC)
					{
						//冠号与前一断链一致、 里程在断链区间
						bzlcAry.push_back(OldBZLCarray[i]);
					}
				}
				else
					ads_printf(L"\n错误：没找到前一个断链！");//nan

			}
		}
	}

}

/*===================================================================
函数名:CmpNewDLwithOld   
------------------------------------------------------------
函数功能:对比原始交点和当前交点，确定是否保留原来的断链和标志里程，更新断链和标志里程数组
------------------------------------------------------------
输入:
      OldJDarray   原始交点
	  OldDLarray   原始断链
      JDarray   当前交点
------------------------------------------------------------
输出: DLarray   当前断链
------------------------------------------------------------
算法: 
一、保存列表编辑前绕行段的断链、交点数组，和新的绕行段交点数组比对，交点没变，
依然沿用原来断链；交点信息变化，则用自动生成的断链。
二、对于既有基线：1、计算断链坐标 2、计算线元，比对断链点是否变化
3、如果不变，修正断前，根据到前一个断后的几何长度计算。否则，删除断链。
三、对于二线：多做一步检验绕行起终点断链是否在直线段上，如果不在，
用自动生成的绕行起终断链。
------------------------------------------------------------
备注: 1 调用该函数前需计算断链坐标，然后计算新的线元
	  2 此函数与编辑断链函数CDLEditDlg::OnBnClickedSavedl()相似      818       
------------------------------------------------------------
编写: 李帅 2016.7.30
=====================================================================*/
void RHA::CmpNewDLwithOld()
{
	if(OldJDarray.size()<2 && OldDLarray.size()<2)
		return;

	//清空标志里程
	BZLCarray.clear();

	vector<SE> oldRxdSE_JDix;
	vector<SE> oldRxdSE_DLix;

	vector<DLTab> tmpDLAry;
	vector<BZLCStructTable> tmpBZLCAry;
	xlpoint pz;
	double dist=0;


	tmpDLAry.clear();
	tmpBZLCAry.clear();

	
	int oldRxdNum=GetRXDNum(OldJDarray);
	int curRxdNum=GetRXDNum(JDarray);

	if(oldRxdNum<0)
	{
		ads_printf(L"交点属性有误\n");//nan
		return;
	}

	//818 考虑保存起点断链 连TYLC()一起改
	if (OldJDarray[0].JDXZ == RX_Start
		&& JDarray[0].JDXZ == RX_Start)
	{
		_tcscpy(DLarray[0].BGH, OldDLarray[0].BGH); 
		DLarray[0].BLC=OldDLarray[0].BLC;
		_tcscpy(DLarray[0].EGH, OldDLarray[0].EGH); 
		_tcscpy(DLarray[1].BGH, DLarray[0].EGH);
	}

	//保存终点断链断后 
	if (OldJDarray[OldJDarray.size()-1].JDXZ == RX_End
		&& JDarray[JDarray.size()-1].JDXZ == RX_End)
	{
		_tcscpy(DLarray[DLarray.size()-1].EGH, OldDLarray[OldDLarray.size()-1].EGH); 
		DLarray[DLarray.size()-1].ELC=OldDLarray[OldDLarray.size()-1].ELC;
	}



	//全线绕行的情况（一线）
	if(oldRxdNum == 1 && OldJDarray[0].JDXZ == RX_Start 
		&& OldJDarray[OldJDarray.size()-1].JDXZ == RX_End
		&& curRxdNum == 1 && JDarray[0].JDXZ == RX_Start 
		&& JDarray[JDarray.size()-1].JDXZ == RX_End)
	{
		//中间断链
		tmpDLAry.clear();
		for (int k=1; k<OldDLarray.size()-1; k++)
		{
			//如果在线上就添加断链
			pz.lc=PROJ_ML(array, tarray, OldDLarray[k].N,  OldDLarray[k].E
				, OldDLarray[k].N,  OldDLarray[k].E);

			xlpoint_pz(array, tarray, pz);
			dist=sqrt(pow((OldDLarray[k].N-pz.x),2.0)
				+pow((OldDLarray[k].E-pz.y),2.0));
			if (dist<0.01)//在线上
				tmpDLAry.push_back(OldDLarray[k]);

			tmpDLAry[tmpDLAry.size()-1].TYLC=pz.lc;//投影里程
		}

		//断链根据投影里程排序
		for (int i=0; i<tmpDLAry.size(); i++)
		{
			for (int j=i+1; j<tmpDLAry.size(); j++)
			{
				if (tmpDLAry[j].TYLC+0.0001 < tmpDLAry[i].TYLC)
				{
					swap(tmpDLAry[j], tmpDLAry[i]);
				}
			}
		}

		//更新每个断链的断前
		tmpDLAry.insert(tmpDLAry.begin(), DLarray[0]);
		tmpDLAry.push_back(DLarray[DLarray.size()-1]);

		for (int i=1; i<tmpDLAry.size(); i++)
		{
			tmpDLAry[i].BLC=tmpDLAry[i-1].ELC
				+tmpDLAry[i].TYLC-tmpDLAry[i-1].TYLC;
			_tcscpy(tmpDLAry[i].BGH, tmpDLAry[i].EGH);

		}

		DLarray.clear();
		DLarray.assign(tmpDLAry.begin(), tmpDLAry.end());

		//标志里程
		if(XLShuXing != SJJX && XLShuXing != SJ2X
			&& OldBZLCarray.size()>0)
		{
			CmpNewBZLCwithOld(tmpDLAry, tmpBZLCAry);
			BZLCarray.clear();
			BZLCarray=tmpBZLCAry;
		}

		return;



	}

	

	//部分并行的情况（二线）
	oldRxdSE_JDix.clear();
	oldRxdSE_DLix.clear();
	oldRxdSE_JDix.resize(oldRxdNum);
	oldRxdSE_DLix.resize(oldRxdNum);

	//形成先前的绕行段交点、断链下标数组
	int sjd, ejd;//绕行始、止交点下标
	int sdl, edl;

	for(int i=0; i<oldRxdNum; i++)
	{
		GetRxdSEJD(i, sjd, ejd, OldJDarray);
		oldRxdSE_JDix[i].Strt=sjd;
		oldRxdSE_JDix[i].End=ejd;

		GetiRXDDLinfo(i, sdl, edl, OldDLarray);
		oldRxdSE_DLix[i].Strt=sdl;
		oldRxdSE_DLix[i].End=edl;
	}

	RHA *pStRHA_BL=NULL;//起点参照线
	RHA *pEndRHA_BL=NULL;//终点参照线
	int newSJD, newEJD;//新的交点下标
	int newSDL, newEDL;

	double strtProjml1x=-1.0, strtProjml2x=-1.0;
	double endProjml1x=-1.0, endProjml2x=-1.0;

	bool isSame=false;//是否找到相同绕行段
	bool cpySDLorNot, cpyEDLorNot;//是否保留绕行始、止断链
	CString mes;

	for(int i=0; i<curRxdNum; i++)
	{
		//形成新的绕行段交点数组和断链数组
		GetRxdSEJD(i, newSJD, newEJD, JDarray);
		GetiRXDDLinfo(i, newSDL, newEDL, DLarray);

		if (newSJD<0 || newEJD<0 || newSDL<0 || newEDL<0)
		{
			mes.Format(L"第%d个绕行段的交点或断链设置错误\n", i);
			ads_printf(mes);
			continue;
		}

		//匹配之前的绕行段
		isSame=false;
		int j;
		for(j=0; j<oldRxdNum; j++)
		{
																		
			if (oldRxdSE_JDix[j].End-oldRxdSE_JDix[j].Strt == newEJD-newSJD)//818 未考虑绕行段起终点断链可能不变的情况，即新旧断链一致
			{
				//交点数相同的情况
				//绕行段交点数组是否相同
				isSame=true;
				int m=newSJD;
				for(int k=oldRxdSE_JDix[j].Strt; k <= oldRxdSE_JDix[j].End; k++)
				{
					//交点几何信息、交点性质是否相同
					if(OldJDarray[k].JDXZ != JDarray[m].JDXZ 
						|| fabs(OldJDarray[k].N - JDarray[m].N)>0.001 
						|| fabs(OldJDarray[k].E - JDarray[m].E)>0.001  
						|| fabs(OldJDarray[k].R - JDarray[m].R)>0.001 
						|| fabs(OldJDarray[k].l1 - JDarray[m].l1)>0.001
						|| fabs(OldJDarray[k].l2 - JDarray[m].l2)>0.001)
					{
						isSame=false;break;//修改前后，绕行段变化了
					}

					//绕行始的参照信息是否相同（考虑起终点绕行的情况）
					if (OldJDarray[k].JDXZ == RX_Start && oldRxdSE_JDix[j].End>oldRxdSE_JDix[j].Strt)
					{
						if (k>0 && m>0)
						{
							//都不是起点
							if (_tcscmp(OldJDarray[k-1].BLName, JDarray[m-1].BLName) != 0 
								|| _tcscmp(OldJDarray[k-1].BLJDName, JDarray[m-1].BLJDName) != 0 
								|| fabs(OldJDarray[k-1].XJJ - JDarray[m-1].XJJ)>0.0001)
							{
								isSame=false;break;
							}

						}
						else if ( k*m == 0 && k+m >0)
						{
							//其中一个是起点（在起点前连接线路的情况）
							isSame=false;break;
						}
						//都是起点，不需比较参照

					}

					//绕行止的参照信息是否相同
					if (OldJDarray[k].JDXZ == RX_End || oldRxdSE_JDix[j].End == oldRxdSE_JDix[j].Strt)
					{
						if(k<OldJDarray.size()-1 && m<JDarray.size()-1)
						{
							//都不是终点
							if (_tcscmp(OldJDarray[k].BLName, JDarray[m].BLName) != 0
								|| _tcscmp(OldJDarray[k].BLJDName, JDarray[m].BLJDName) != 0 
								|| fabs(OldJDarray[k].XJJ - JDarray[m].XJJ)>0.0001)
							{
								isSame=false;break;
							}
						}
						else if(k == OldJDarray.size()-1 && m == JDarray.size()-1)
						{//都是终点
						}
						else
						{//其中一个是终点
							isSame=false;break;
						}
						
					}
					
					m++;

				}

				if (isSame)
					break;
			}
		}
		

		//如果没变，用原来的绕行段断链替换修改后的曲线
		cpySDLorNot=true;
		cpyEDLorNot=true;

		if (isSame)
		{		//先前的绕行段交点、断链下标
			sdl=oldRxdSE_DLix[j].Strt;
			edl=oldRxdSE_DLix[j].End;
			sjd=oldRxdSE_JDix[j].Strt;
			ejd=oldRxdSE_JDix[j].End;

			pStRHA_BL=NULL;
			pEndRHA_BL=NULL;

			//检验是否采用原有的绕行起点断链
			for (int k=0;k<1;k++)//虚循环
			{
				//2 断链是否在二线上

				pz.lc=PROJ_ML(array, tarray, OldDLarray[sdl].N,  OldDLarray[sdl].E
					, OldDLarray[sdl].N,  OldDLarray[sdl].E);

				strtProjml2x=pz.lc;

				//1 线路起点
				if (newSJD == 0)
				{
					if (_tcscmp(DLarray[newSDL].BLName, L"") == 0)//删？调整，启用起点断链，如果起点绕行 818
						break;
				}
				
				xlpoint_pz(array, tarray, pz);
				dist=sqrt(pow((OldDLarray[sdl].N-pz.x),2.0)
					+pow((OldDLarray[sdl].E-pz.y),2.0));
				if (dist>0.01)
					{cpySDLorNot=false; break;}

				
				
				//断链是否在二线的夹直线上
				if (newSJD>0 && !(strtProjml2x+0.0001<QXB[newSJD].KZH
					&& strtProjml2x>QXB[newSJD-1].KHZ+0.0001))
				{
					cpySDLorNot=false; break;
				}

				//断链是否与二线的前一个断链重叠
				if(newSDL>0 && strtProjml2x+0.0001<DLarray[newSDL-1].TYLC)
				{
					cpySDLorNot=false; break;
				}

				//断链在一线的投影是否在绕行段上
				pStRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[newSDL].BLName);
				if(pStRHA_BL == NULL)
				{
					ads_printf(L"\n 找不到参照线！");cpySDLorNot=false; break;
				}

				CalXJJ(strtProjml2x, array, tarray, pStRHA_BL->array
					 , pStRHA_BL->tarray, strtProjml1x, false);
				if (pStRHA_BL->GetiRxdByProjml(strtProjml1x)<0)
				{
					cpySDLorNot=false; break;
				}

				//断链在一线的投影是否在对应的夹直线上
				double KZH1x, KHZ1x;
				int iJD1x;
				if(newSJD>0)
					pStRHA_BL->GetJdno1FromJdstr(JDarray[newSJD-1].BLJDName
					, pStRHA_BL->JDarray, iJD1x);
				else
				{
					pStRHA_BL->GetJdno1FromJdstr(JDarray[newSJD].BLJDName
					, pStRHA_BL->JDarray, iJD1x);
					iJD1x--;
				}


				if (iJD1x>-1)
				{
					KZH1x=pStRHA_BL->QXB[iJD1x+1].KZH;
					KHZ1x=pStRHA_BL->QXB[iJD1x].KHZ;

					if (!(strtProjml1x+0.001<KZH1x && strtProjml1x>KHZ1x+0.001))
					{
						cpySDLorNot=false; break;
					}
				}

			}
			
			
			//检验绕行终点断链
			for (int k=0;k<1;k++)//虚循环
			{

				//2 断链是否在二线上
				pz.lc=PROJ_ML(array, tarray, OldDLarray[edl].N,  OldDLarray[edl].E
					, OldDLarray[edl].N,  OldDLarray[edl].E);

				endProjml2x=pz.lc;

				//线路终点
				if (newEJD == JDarray.size()-1)
				{
					if (_tcscmp(DLarray[newEDL].BLName, L"") == 0)//删？ 818
						break;
				}


				xlpoint_pz(array, tarray, pz);
				dist=sqrt(pow((OldDLarray[edl].N-pz.x),2.0)
					+pow((OldDLarray[edl].E-pz.y),2.0));
				if (dist>0.01)
				{cpyEDLorNot=false; break;}

				

				//断链是否在二线的夹直线上
				if (newEJD<JDarray.size()-1 && !(endProjml2x>QXB[newEJD].KHZ+0.0001
					&& endProjml2x+0.0001<QXB[newEJD+1].KZH))
				{
					cpyEDLorNot=false; break;
				}

				//断链是否与二线的后一个断链重叠
				if(newEJD<JDarray.size()-1 && endProjml2x>DLarray[newEDL+1].TYLC+0.0001)
				{
					cpyEDLorNot=false; break;
				}

				//断链在一线的投影是否在绕行段上
				pEndRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[newEDL].BLName);
				if(pEndRHA_BL == NULL)
				{
					ads_printf(L"\n 找不到参照线！");cpySDLorNot=false; break;
				}

				CalXJJ(endProjml2x, array, tarray, pEndRHA_BL->array
					, pEndRHA_BL->tarray, endProjml1x, false);
				if (pEndRHA_BL->GetiRxdByProjml(endProjml1x)<0)
				{
					cpyEDLorNot=false; break;
				}

				//断链在一线的投影是否在对应的夹直线上
				double KZH1x, KHZ1x;
				int iJD1x;
				//if(newEJD>0)
				//	pEndRHA_BL->GetJdno1FromJdstr(JDarray[newEJD].BLJDName
				//	, pEndRHA_BL->JDarray, iJD1x);

				//if(newEJD<JDarray.size()-1 && (JDarray[ejd+1].JDXZ == BXPX 
				//	|| JDarray[ejd+1].JDXZ == BX_NotPX) )//818 考虑S弯
				if(newEJD<JDarray.size()-1 && (JDarray[newEJD+1].JDXZ == BXPX 
					|| JDarray[newEJD+1].JDXZ == BX_NotPX) )//原来的下标越界死机，李伟修改
				{
					GetJdno1FromJdstr(JDarray[newEJD+1].BLJDName
						, pEndRHA_BL->JDarray, iJD1x);
					iJD1x--;
				}
				else
					GetJdno1FromJdstr(JDarray[newEJD+1].BLJDName
					, pEndRHA_BL->JDarray, iJD1x);


				if (iJD1x>-1 && iJD1x+1<pEndRHA_BL->QXB.size())
				{
					KHZ1x=pEndRHA_BL->QXB[iJD1x].KHZ;
					KZH1x=pEndRHA_BL->QXB[iJD1x+1].KZH;

					if (!(endProjml1x+0.001<KZH1x && endProjml1x>KHZ1x+0.001))
					{
						cpyEDLorNot=false; break;
					}
				}

			}

			tmpDLAry.clear();
			//起点断链
			if (cpySDLorNot)
			{
				tmpDLAry.push_back(OldDLarray[sdl]);
				tmpDLAry[0].TYLC=strtProjml2x;

				if (pStRHA_BL == NULL && newSJD == 0)
				{//起点绕行

				}
				else
				{//普通绕行始
					//更新断前    818 //sBL<0 是什么情况
					pStRHA_BL->TrsProjmltoCkml(strtProjml1x, 3, tmpDLAry[0].BGH, tmpDLAry[0].BLC);

					//如果BLC和之前相差不大，采用之前的BLC(整数)
					if (fabs(tmpDLAry[0].BLC-OldDLarray[sdl].BLC)<0.001)
						tmpDLAry[0].BLC=OldDLarray[sdl].BLC;
				}
				


			}
			else
				tmpDLAry.push_back(DLarray[newSDL]);


			//中间断链
			for (int k=sdl+1; k<edl; k++)
			{
				//如果在线上就添加断链
				pz.lc=PROJ_ML(array, tarray, OldDLarray[k].N,  OldDLarray[k].E
					           , OldDLarray[k].N,  OldDLarray[k].E);
				
				if (pz.lc+0.0001<strtProjml2x 
					|| pz.lc-0.0001>endProjml2x)
				{
					//不在绕行段上
					continue;
				}

				xlpoint_pz(array, tarray, pz);
				dist=sqrt(pow((OldDLarray[k].N-pz.x),2.0)
					+pow((OldDLarray[k].E-pz.y),2.0));
				if (dist<0.01)//在线上
					tmpDLAry.push_back(OldDLarray[k]);

				tmpDLAry[tmpDLAry.size()-1].TYLC=pz.lc;//投影里程
			}

			//终点断链
			if (cpyEDLorNot)
			{
				tmpDLAry.push_back(OldDLarray[edl]);
				int tmpNDL=tmpDLAry.size();//临时断链数

				tmpDLAry[tmpNDL-1].TYLC=endProjml2x;

				if (pEndRHA_BL == NULL && newEJD+1 == JDarray.size())
				{//终点绕行

				}
				else
				{//普通绕行止
					//更新断后
					pEndRHA_BL->TrsProjmltoCkml(endProjml1x, 3, tmpDLAry[tmpNDL-1].EGH, tmpDLAry[tmpNDL-1].ELC);

					//如果ELC和之前相差不大，采用之前的ELC(整数)
					if (fabs(tmpDLAry[tmpNDL-1].ELC-OldDLarray[edl].ELC)<0.001)
						tmpDLAry[tmpNDL-1].ELC=OldDLarray[edl].ELC;
				}
			

			}
			else
				tmpDLAry.push_back(DLarray[newEDL]);

			//获得该绕行段应保留的标志里程
			tmpBZLCAry.clear();

			if (XLShuXing == JYJX || XLShuXing == JY2X)
				CmpNewBZLCwithOld(tmpDLAry, tmpBZLCAry);

											//818 并行段断链是否保留 ps:应该要保留
			                                 //818 检查冠号一致性，赋了冠号的一致
			//更新绕行段中每个断链的断前里程 重要的是保证断链位置不变
			//818 TYLC计算对吗？
			if(tmpBZLCAry.size()<1)
			{
				//没有标志里程
				for (int k=1; k<tmpDLAry.size(); k++)
				{
					tmpDLAry[k].BLC=tmpDLAry[k-1].ELC+tmpDLAry[k].TYLC-tmpDLAry[k-1].TYLC;
					_tcscpy(tmpDLAry[k].BGH, tmpDLAry[k-1].EGH);
				}

			}
			else
			{
				//有需要保留标志里程
				double dist=-100;
				double distwithBZLC=-100;//断链和前一个标志里程的最小距离
				double distwithDL=0;//断链之间的距离

				for (int k=1;k<tmpDLAry.size();k++)
				{
					_tcscpy(tmpDLAry[k].BGH, tmpDLAry[k-1].EGH);//冠号一致

					distwithBZLC=-100;
					distwithDL=tmpDLAry[k].TYLC-tmpDLAry[k-1].TYLC;

					for (int m=0;m<tmpBZLCAry.size();m++)
					{

						if( tmpBZLCAry[m].xyToprojml+0.0001<tmpDLAry[k].TYLC)
						{
							//标志里程在该断链tmpBZLCAry[m]之前
							dist=tmpDLAry[k].TYLC-tmpBZLCAry[m].xyToprojml;
							if (distwithBZLC+0.001<dist || distwithBZLC<0)
								distwithBZLC=dist;
						}
					}


					if(distwithBZLC<0 || (distwithBZLC>0 && distwithBZLC>distwithDL+0.0001))
					{
						//如果没有标志里程在断链前
						tmpDLAry[k].BLC=tmpDLAry[k-1].ELC+tmpDLAry[k].TYLC-tmpDLAry[k-1].TYLC;
					}
					//有标志里程就不改断前里程 818？？

				}
			}

			//删除该绕行段自动生成的断链
			DLarray.erase(DLarray.begin()+newSDL
				, DLarray.begin()+newEDL+1);

			//插入计算得到的保留断链
			ACHAR   rxdName[50];
			_stprintf(rxdName, L"绕行段%0.1lf", tmpDLAry[0].ELC/1000.0);

			int tmpDLNum=tmpDLAry.size();
			for (int k=tmpDLNum-1;k>-1;k--)
			{
				tmpDLAry[k].Indx=i;//绕行段编号
				_tcscpy(tmpDLAry[k].RXDName, rxdName);//绕行段名
				DLarray.insert(DLarray.begin()+newSDL, tmpDLAry[k]);
			}

			//尾插标志里程
			for (int k=0;k<tmpBZLCAry.size();k++)
			{
				_tcscpy(tmpBZLCAry[k].GJDNum, rxdName);
				BZLCarray.push_back(tmpBZLCAry[k]);
			}
			

		}	

		
	}

}


/*===================================================================
函数名:CalDL_NE   
------------------------------------------------------------
函数功能:计算断链坐标
------------------------------------------------------------
输入:
------------------------------------------------------------
输出: 
------------------------------------------------------------
编写: 李帅 2016.5.16 
=====================================================================*/
void RHA::CalDL_NE(int sdl, int edl)
{
	int dlNum=DLarray.size();
	if (sdl>-1 && sdl<DLarray.size())
	{
		if (edl == -1)
			edl=dlNum-1;
		else if (edl >= sdl && edl<DLarray.size())
		{
		}
		else
			return;

		xlpoint PZ;
		for(int i=sdl; i <= edl; i++)
		{
			PZ.lc=DLarray[i].TYLC;

			xlpoint_pz(array, tarray, PZ);
			DLarray[i].N=PZ.x;
			DLarray[i].E=PZ.y;
		}
		
	}	
}


//设置实体属性参数、规范参数、绘图参数
void RHA::InitJBCS()
{
	IsReSetColor=FALSE;//局部变量？818
	setColorLineW();//该函数其它地方就可以不用了 818

	//读取平面绘图参数   818 tmp
	XLDataBase xlmdb;

	xlmdb.GetDrawPlaneParam(m_mdbName, m_XLName);//818 DrawParam出图时再读表
	//赋绘图参数
	DrawParam=xlmdb.m_DrawParam;
	
	//读取规范参数
	m_Rs.clear();
	m_l0s.clear();

	CSheJiBiaoZhun biaozhun;
	CString strMDBName;
	CAdoConnection mdbConnection;
	CString strConn;
	CString strSysPath;
	int ix=0;
	int strLenth=0;

	//连接数据库
	if (m_mdbName != L"C:\\未命名.MDB")
	{
		strConn=L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source="+m_mdbName;
		if(!mdbConnection.Connect(strConn))
			ads_printf(L"连接数据库"+strMDBName+L"失败!\n");
	}

	//获取数据库名
	strMDBName=m_mdbName;
	strMDBName.MakeUpper();

	strLenth=strMDBName.GetLength();
	ix=strMDBName.Find(L"\\DATA\\");
	strMDBName=strMDBName.Right(strLenth-ix-6);

	//初始化
	if (XLShuXing == JYJX || XLShuXing == JY2X)
		biaozhun.m_IsJY=true;
	else
		biaozhun.m_IsJY=false;

	ReadWorkdir();
	biaozhun.SetDatabsConnect(mdbConnection);
	strSysPath=SysPath;
	biaozhun.SetCARAPath(strSysPath);
	biaozhun.SetCrnnProjPath(Cworkdir, strMDBName);//818 Cworkdir

	bool isGuiFanExist=false;
	if (m_mdbName != L"C:\\未命名.MDB" && biaozhun.GetCrnCSFromMDB())
		isGuiFanExist=true;
	else if (biaozhun.GetCrnCSFromDAT())
		isGuiFanExist=true;
	else 
	{
		ULONG pSpdLvl[MAXRECORDNUMBER];
		short dLength;
		unsigned long ThePosition;//文件指针的位置


		dLength=biaozhun.GetSpeedLevels(pSpdLvl, MAXRECORDNUMBER);

		if (dLength>0)
		{

			for (int i = dLength; i > 0; i--)
			{
				if (pSpdLvl[i] == 160)
				{
					ThePosition = i - 1;
					break;
				}
			}

			if (ThePosition == 10000)
				ThePosition=0;
			if (ThePosition > dLength)
				ThePosition=0;

			biaozhun.GetOneCS(ThePosition);

			isGuiFanExist=true;
		}

	}

	if (isGuiFanExist)
	{
		m_Rs.resize(biaozhun.m_tgGuiFanCS.RXiangShu);
		m_l0s.resize(biaozhun.m_tgGuiFanCS.RXiangShu);

		for (int i=0; i<biaozhun.m_tgGuiFanCS.RXiangShu; i++)
		{
			m_Rs[i]=biaozhun.m_tgGuiFanCS.R[i];
			m_l0s[i]=biaozhun.m_tgGuiFanCS.L[i];
		}

		DV=biaozhun.m_tgGuiFanCS.SuDuMuBiao;
	}
	
	//DrawParam.SetLayerName(m_mdbName, m_XLName);

	//DrawParam.m_LICHENG=TRUE;
	//DrawParam.m_IsRailLegend=0;
	//DrawParam.m_LegendLen=10.0;//图例虚实段长度
	//DrawParam.m_JIAODIAN=TRUE;//是否标注交点
	//DrawParam.m_IsNoteJDNum=TRUE;//是否标注交点号
	//DrawParam.m_IsNoteJDR=TRUE;//是否标注交点半径
	//DrawParam.m_IsNoteJDAng=TRUE;//是否标注交点转角
	//DrawParam.m_IsNoteJDOthers=TRUE;//是否标注交点其它信息

	//DrawParam.m_IsNoteTZD=TRUE;//是否标注特征点

	//DrawParam.m_IsDrawFQD=TRUE;

	//DrawParam.m_LICHENG=TRUE;
	//DrawParam.m_dLC=100.0;

	//DrawParam.m_NoteMinDLLen=0.0;

	//DrawParam.m_ExOffset=0.0;
	//DrawParam.SCALE=2.0;
	//DrawParam.m_TextDir=-1;

	//DrawParam.NLC=3;
	//DrawParam.NPJ=0;

	////
	//double LW=0.7*2.0;
	//double textH=5*2.0;

	//for(int i=0; i<6; i++)
	//{
	//	DrawParam.OtherParam.EntAry[i].LW=LW;
	//	DrawParam.OtherParam.EntAry[i].Colo=AllColor;
	//}

	//DrawParam.OtherParam.EntAry[1].LW=0.25;

	//for(int i=0; i<7; i++)
	//{
	//	DrawParam.OtherParam.MarkAry[i].TextHeight=textH;
	//	DrawParam.OtherParam.MarkAry[i].LorR=1;//前进方向右侧
	//	DrawParam.OtherParam.MarkAry[i].Colo=AllColor;
	//}

	mode_display=0;//显示线路
	texth=5;
	
	m_IsNoteTZD = TRUE;



	if(m_XLName == L"既有基线")
		m_isNoteNYDL = FALSE;
	else
		m_isNoteNYDL = TRUE;

	if (JDarray[0].JDXZ == BXPX)
	{
		if(JDarray[0].XJJ>0)//二线为右线
			draw_zybz=1;//曲线要素标在左侧
		else
			draw_zybz=-1;//曲线要素标在右侧
	}
	else
		draw_zybz=1;
	
	qxbbz=1;
}


/*函数名：setColorLineW
功能：设置颜色、线宽
输入：mdbName    数据库名
xlName     线路名
allColor   颜色值
lineWeightValue     线宽值
备注：                              */
void RHA::setColorLineW()
{

	if(!IsReSetColor)
	{
		CArray<SetLineAttriStruct,SetLineAttriStruct> SetLineAttrArrayTmp ;
		ReadColorFile( m_mdbName,SetLineAttrArrayTmp);

		BOOL IsHasSet = FALSE;
		for(int i=0;i<SetLineAttrArrayTmp.GetSize();i++)
		{
			for(int i=0;i<SetLineAttrArrayTmp.GetSize();i++)
			{
				if( _tcscmp(m_XLName,SetLineAttrArrayTmp[i].XLName)==0 ) 
				{
					AllColor = SetLineAttrArrayTmp[i].ColorIndex;
					LineWeightValue =TranLineWeightFromIntToEnum(SetLineAttrArrayTmp[i].LineW);
					IsHasSet = TRUE;
				}
			}
		}

		if(!IsHasSet)
		{
			if (_tcscmp(m_XLName,L"既有基线")==0)
				AllColor = 0 ;
			else if (_tcscmp(m_XLName, L"既有二线")==0)
				AllColor = 150;
			else if (_tcscmp(m_XLName, L"既有三线")==0)
				AllColor = 1;
			else if (_tcscmp(m_XLName, L"既有四线")==0)
				AllColor = 3;
			else if (_tcscmp(m_XLName, L"设计基线")==0)
				AllColor = 2;//设计基线为黄色
			else if (_tcscmp(m_XLName, L"设计二线")==0)
				AllColor = 6;
			else if (_tcscmp(m_XLName, L"设计三线")==0)
				AllColor = 7;
			else if (XLShuXing == SGBX)
				AllColor = 4;
			else
				AllColor = 0;
			LineWeightValue =  TranLineWeightFromIntToEnum(0);

		}
	}

	SetAllColor(AllColor);

}

void RHA::SetAllColor(int ColorIndex)
{
	AllColor = ColorIndex;
	GTZXJDToJDColor = ColorIndex;
	XWArcColor = ColorIndex;
	HuanHequxianColor = ColorIndex;
	JiaZhiXianColorZuo= ColorIndex;
	JiaZhiXianColorYou =ColorIndex;
	mdbnameBoxColor = ColorIndex;
	mdbnameTextColor =ColorIndex;
	QXYSColor = ColorIndex;
}

//绘制缓和曲线
Adesk::Boolean  RHA::G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double LO,struct xlpoint &P0,struct xlpoint &PZ, double ND)
{   
	int k, nk;
	double dlc;

	nk=(int)(LO/ND)+1;
	AcGePoint3d* pArr=new AcGePoint3d[nk+1];
	pArr[0].set(P0.y,P0.x,0.0);
	for (k=1; k<=nk; k++)
	{     
		dlc=k*ND*1.0;
		if (dlc>LO) 
			dlc=LO;
		xlzbxy(P0,PZ,dlc,INDATA,0);
		// ads_printf(L"\n % %d %5.1f %10.3f %10.3f %10.4f",k,step,PZ.y,PZ.x,dms_rad(PZ.a));
		pArr[k].set(PZ.y,PZ.x,0.0);
	};

	pWd->subEntityTraits().setColor(HuanHequxianColor);	
	pWd->subEntityTraits().setLineWeight(LineWeightValue);
	pWd->geometry().polyline(nk+1,pArr);
	delete []pArr;
	// ads_printf(L"\n AA 类型=%d %d %10.3f %10.3f %5.1f %10.2f",(int)INDATA[0],nk,INDATA[1],INDATA[2],ND,LO);
	return  0;
}

//绘制线路各个线元
Adesk::Boolean  RHA::G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint &P0,struct xlpoint &PZ,bool ISShow,double T0mini)
{     
	int j;
	double l=0.0;
	AcGePoint3d ptb,pt,pte;
	//	if(INDATA[1]<0.00000001) return 1;
	l=Get_INDATA_L(INDATA,&j);

	PZ.x = P0.x;
	PZ.y = P0.y; 
	PZ.a= P0.a;

	//圆+线路长度为0
	if( fabs(INDATA[0]-2)<0.00001 && l<0.00000001 ) PZ.a = INDATA[3];

	PZ.lc = P0.lc;
	PZ.r = P0.r;
	if(l<0.00000001)
		return 1;

	ptb[X]=P0.y;ptb[Y]=P0.x; ptb[Z]=0.0;

	if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000001)|| (l<=0.00000000001)) return 1;

	if (j>2 && j<=6 )
	{ //( 回旋线)
		if(ISShow)
			G_drawspi(pWd,INDATA,l,P0,PZ, 2.0);
		xlzbxy(P0,PZ,l,INDATA,0);// 某线元终点
	}
	else if (j<=2)  // R L
	{//( 园弧 直线)
		xlzbxy(P0,PZ,l*0.5,INDATA,0); // 某线元中点
		pt[X]=PZ.y;
		pt[Y]=PZ.x; 
		pt[Z]=0.0;
		xlzbxy(P0,PZ,l,INDATA,0); // 某线元终点
		pte[X]=PZ.y;
		pte[Y]=PZ.x;
		pte[Z]=0.0; //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		if (j==1) //直线
		{	 
			AcGePoint3d pcArr[2];		
			pcArr[0]=ptb; pcArr[1]=pte;
			if( sqrt( (P0.x-PZ.x)*(P0.x-PZ.x) + (P0.y-PZ.y)*(P0.y-PZ.y) ) >= T0mini )
			{
				pWd->subEntityTraits().setColor(JiaZhiXianColorZuo);	
			}
			else
			{
				pWd->subEntityTraits().setColor(JiaZhiXianColorYou);	
			}
			if(ISShow)
				pWd->geometry().polyline(2,pcArr);		
		}
		else if (j==2) // 园弧 
		{ 	
			pWd->subEntityTraits().setColor(XWArcColor);
			if(ISShow)
				pWd->geometry().circularArc(ptb,pt,pte);
		};
	}; 
	return 0;
}

void RHA::MarkJD(AcGiWorldDraw* pWd,vector<JDarrayStruct>& arrayd,vector<QXBStruct>& qxb,ACHAR* JDchr)
{
	//计算曲线要素
	//CalNoteQXYS();

	int NJD = arrayd.size();
	double fb,fe,de,da,aa,dd,dt ;
	AcGePoint3d  p1; 
	ACHAR ch[80],ch1[60],chrtmp[80];
	int i,c,n;
	de=xyddaa(arrayd[0].N,arrayd[0].E,arrayd[1].N,arrayd[1].E,&fb);
	///int zory=-1;//标注 1——左侧，-1——右侧
	for (i=1;i<NJD-1;i++)
	{  
		//if (arrayd[i].R<-0.1) break;
		de=xyddaa(arrayd[i].N,arrayd[i].E,arrayd[i+1].N,arrayd[i+1].E,&fe);

		aa= fe - fb ;
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0) 
		{ //右偏
			c=1; dd=2*texth; 
		}   
		else
		{ //左偏 
			c=-1; dd= 2*texth;
		};

		de=fb+aa*0.5;

		_tcscpy(ch1,arrayd[i].JDName);

		n=_tcslen(ch1);
		p1[Z]=0.0;

		p1[X]=arrayd[i].E-n*0.5*texth*sin(de)+dd*sin(de+c*pi*0.5);
		p1[Y]=arrayd[i].N-n*0.5*texth*cos(de)+dd*cos(de+c*pi*0.5);

		da=0.5*Pi-de;
		if(/*JDarray[i].IsShow*/1)
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);    // 交点号  
		 
		if(qxbbz == 1)
		{
			JDarray[i].IsShow=true;//nan 818 

			// 标曲线参数 begin
			double R,T,T1,T2,L1,L2,LL,NYDL;
			R=fabs(arrayd[i].R); 
			L1= arrayd[i].l1;  
			L2=arrayd[i].l2; 
			T1=qxb[i].T1; 
			T2=qxb[i].T2;  
			LL=qxb[i].L;
			T=(T1+T2)*0.5-(L1+L2)*0.25;

			dd=sqrt(R*R+T*T)-R;
			// ads_printf(L"\n dd=%8.2f",dd);
			if (draw_zybz == 1) 
			{  
				//标在左侧;
				dd=dd+16*texth; 
				dt=c*texth*1.5;
				if (c==-1) 
					dd=dd+8*texth;
			}
			else 
			{  
				//标在右侧;
				c=1;
				dd=dd+16*texth; 
				dt=c*texth*1.5; 
				dd=-fabs(dd);
				if (dd>-50)
					dd=-50;

				if (dd<-500)
					dd=-500;
			}
			// ads_printf(L"\ dd=%8.2f",dd);
			p1[X]=arrayd[i].E-dd*sin(de-c*pi*0.5) ; 
			p1[Y]=arrayd[i].N-dd*cos(de-c*pi*0.5);
			_tcscpy(chrtmp,L"  ");
			_tcscat(chrtmp,ch1);
		if(/*JDarray[i].IsShow*/1)
			G_maketext(pWd,p1,chrtmp,da,texth ,QXYSColor,1);   // 交点号  

		ads_rtos(qxb[i].a ,2,4,ch);                  
		if (aa>0) { _tcscpy(ch1,L"ay--"); }
		else _tcscpy(ch1,L"az--"); 
		_tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  偏角

		if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
		else { ads_rtos(R,2,DrawParam.NLC,ch); };
		_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   半径

		if (L1>0||L2>0)  //  缓长不为零
		{
			if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,DrawParam.NLC,ch); }
			else { ads_rtos(L1,2,DrawParam.NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
			if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   缓长 相等
			} else
			{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
			if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,DrawParam.NLC,ch); }
			else { ads_rtos(L2,2,DrawParam.NLC,ch); };
			_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
			if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   不等缓长
			};
		};

		T1=qxb[i].T1; T2=qxb[i].T2;  LL=qxb[i].L;
		if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
		else { ads_rtos(T1,2,DrawParam.NLC,ch); };
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if (fabs(L1-L2)<0.001) 
		{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
		if(JDarray[i].IsShow) G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   切长 相等
		} else
		{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
		ads_rtos(T2,2,DrawParam.NLC,ch);
		_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
		if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   不等切长
		};

		ads_rtos(LL,2,DrawParam.NLC,ch);
		_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(/*JDarray[i].IsShow*/1)
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  曲线长
		
		
		
		//9.1-zy-标注内业断链
		//标注内业断链
		if(JDarray[i].JDXZ == BXPX || JDarray[i].JDXZ == BX_NotPX)//并行交点/S弯
		{

			double HZcml=TrsProjmltoCml(QXB[i].KHZ);
			double ZHcml=TrsProjmltoCml(QXB[i].KZH);			

			double len= QXB[i].KHZ -  QXB[i].KZH;//第i交点曲线几何长度
			double ProLen=HZcml-ZHcml;//第i交点曲线连续长度

			JDarray[i].NYDL=len-ProLen+100;

			NYDL = JDarray[i].NYDL;
			ads_rtos(NYDL,2,DrawParam.NLC,ch1);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if(JDarray[i].IsShow)
				G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   // 内业断链
		}
		};
		fb=fe; 
	};   


}

//void RHA::CalNoteQXYS() /*const*/ //818 并行交点是这样标吗？
//{
//	/*int NJD=JDarray.size();
//	Int16 iBLJD=-1;
//	double BPMTL[10];
//
//	for (int i=1; i<NJD-1; i++)
//	{
//		QXB[i].ZJ_Note = QXB[i].a;
//		QXB[i].T1_Note = QXB[i].T1;
//		QXB[i].T2_Note = QXB[i].T2;
//		QXB[i].L_Note = QXB[i].L;
//		if (JDarray[i].JDXZ == BXPX)
//		{
//			int blix=CCreatPM::IsIncluded(m_iDoc, m_mdbName, JDarray[i].BLName);
//			if (blix >= 0)
//			{
//				GetJdno1FromJdstr(JDarray[i].BLJDName, g_DocManageAry[m_iDoc].XLAry[blix]->JDarray, iBLJD);
//				if (iBLJD  >= 0)
//				{
//					QXB[i].ZJ_Note =g_DocManageAry[m_iDoc].XLAry[blix]->QXB[iBLJD].ZJ_Note;
//				}
//
//			}
//
//			getBPMTL(QXB[i].ZJ_Note, QXB[i].R, QXB[i].l1, QXB[i].l2, &BPMTL[0]); 
//			QXB[i].T1_Note=BPMTL[6];
//			QXB[i].T2_Note=BPMTL[7];
//			QXB[i].L_Note=BPMTL[8];
//		}
//	}*/
//}

/*
功能：源线路pSrcRHA按线间距平移得到新线路时，计算新线路的交点坐标
输入：pSrcRHA        源线路
		xjj          线间距，左负右正
输出：JDARRAY的N/E坐标
备注：这里不是按照参照关系平移，只是单纯按照线路平移
*/
void RHA::GetJDXYwhenMoveXLbyXJJ(const RHA *pSrcRHA, double xjj)
{
	if(pSrcRHA == NULL)
		return;

	int JDNum=JDarray.size();

	if (pSrcRHA->JDarray.size() != JDNum)
	{
		ads_printf(L"交点数目不相等！\n");
		return;
	}

	//完全重合
	if(fabs(xjj)<0.00001)
	{
		for(int i=0;i<JDNum;i++)
		{
			JDarray[i].N=pSrcRHA->JDarray[i].N;
			JDarray[i].E=pSrcRHA->JDarray[i].E;
		}

		return;
	}

	//平移一个线间距
	JDarrayStruct *curJD;
	int curBLJDIx=0;
	double fwj=0.0;

	for(int i=0;i<JDNum;i++)
	{
		curJD=&JDarray[i];
		curBLJDIx=i;//当前交点的参照线的交点下标

		if(curBLJDIx == 0)
		{//是全线起点							
			xyddaa(pSrcRHA->JDarray[0].N,pSrcRHA->JDarray[0].E,
				pSrcRHA->JDarray[1].N,pSrcRHA->JDarray[1].E,&fwj);

			curJD->N  =  pSrcRHA->JDarray[0].N + xjj * cos(fwj+m_dHalfPI);
			curJD->E  =  pSrcRHA->JDarray[0].E + xjj * sin(fwj+m_dHalfPI);//计算出二线交点							
		}
		else if(curBLJDIx+1 == pSrcRHA->JDarray.size())
		{//全线终点
			int curBLJDNum=pSrcRHA->JDarray.size();
			int jd1x=curBLJDNum-2;
			xyddaa(pSrcRHA->JDarray[jd1x].N, pSrcRHA->JDarray[jd1x].E,
				pSrcRHA->JDarray[jd1x+1].N,pSrcRHA->JDarray[jd1x+1].E,&fwj);

			curJD->N  =  pSrcRHA->JDarray[jd1x+1].N + xjj * cos(fwj+m_dHalfPI);
			curJD->E  =  pSrcRHA->JDarray[jd1x+1].E + xjj * sin(fwj+m_dHalfPI);//计算出二线交点
		}
		else
		{
			//根据交点前后两条边的线间距平移一个对应的线间距得到
			CalExJDByParallel(pSrcRHA, pSrcRHA, i-1, i, xjj, xjj, curJD);
		}
	}
}

/*===================================================================
函数名:CalExJDByParrel   
------------------------------------------------------------
函数功能:由交点前后边的线间距计算二线交点坐标,适用于二线交行交点
------------------------------------------------------------
输入: 
pBfBLRHA, pCurBLRHA     前后交点的参照线路
bfJDNo, curJDNo         前后参照线交点号下标
bfXJJ, curXJJ           前后线间距
curJD                   后一个交点（当前交点）           
=====================================================================*/
void RHA::CalExJDByParallel(const RHA *pBfBLRHA, const RHA *pCurBLRHA, int bfJDNo, int curJDNo, double bfXJJ, double curXJJ, JDarrayStruct *curJD)
{
	if(bfJDNo<0 || bfJDNo+2>pBfBLRHA->JDarray.size()
		|| curJDNo<0 || curJDNo+2>pCurBLRHA->JDarray.size())
		return;
	//由线间距求前一条边的平行边（pt1,pt2）
	double fwj;
	double pt1[2],pt2[2];

	xyddaa(pBfBLRHA->JDarray[bfJDNo].N,pBfBLRHA->JDarray[bfJDNo].E
		,pBfBLRHA->JDarray[bfJDNo+1].N,pBfBLRHA->JDarray[bfJDNo+1].E,&fwj);

	pt1[0] = pBfBLRHA->JDarray[bfJDNo].N + bfXJJ * cos(fwj+m_dHalfPI);
	pt1[1] = pBfBLRHA->JDarray[bfJDNo].E + bfXJJ * sin(fwj+m_dHalfPI);
	pt2[0] = pBfBLRHA->JDarray[bfJDNo+1].N + bfXJJ * cos(fwj+m_dHalfPI);
	pt2[1] = pBfBLRHA->JDarray[bfJDNo+1].E + bfXJJ * sin(fwj+m_dHalfPI);

	//由线间距求后一条边的平行边（pt3,pt4）
	double pt3[2],pt4[2];
	xyddaa(pCurBLRHA->JDarray[curJDNo].N,pCurBLRHA->JDarray[curJDNo].E
		,pCurBLRHA->JDarray[curJDNo+1].N,pCurBLRHA->JDarray[curJDNo+1].E,&fwj);
	pt3[0] = pCurBLRHA->JDarray[curJDNo].N + curXJJ * cos(fwj+m_dHalfPI);
	pt3[1] = pCurBLRHA->JDarray[curJDNo].E + curXJJ * sin(fwj+m_dHalfPI);
	pt4[0] = pCurBLRHA->JDarray[curJDNo+1].N + curXJJ * cos(fwj+m_dHalfPI);
	pt4[1] = pCurBLRHA->JDarray[curJDNo+1].E + curXJJ * sin(fwj+m_dHalfPI);

	//两条平行边求交点
	INTERS(&curJD->N,&curJD->E,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);
}


//修正交点坐标，交点坐标有可能不给或不够精确，下面进行修正，分情况处理
//1、性质为平行，
//	（1）为全线起终点（对应一线交点序号为0），由线间距求
//	（2）不为全线起终点（对应一线交点序号>0)，由线间距求。
//	     如果前一交点也是平行交点，对应一线交点的前后边作平行线求交点。
//2、性质为绕行
//	（1）对于绕行始，如果前一交点有平行约束，由线间距求。
//	（2）对于绕行止，给坐标，线间距（线间距为0）。往一线对应边上投影，再由偏距求得
//3、性质为并行不平行
//	（1）如果前一交点有平行约束，由线间距求。
//注意：
//1、每次只修正一个交点坐标
//修改：李帅 2017.6
void RHA::ModifyExJDXY()
{
	double fwj=0.0;
	RHA *pBLRHA=NULL;
	int blJDNo=0; 
	int jdNum=0;
	JDarrayStruct *curJD;
	double xjj=0.0;
	bool isBfSideParallel=false;//是否前一边也有平行约束
	bool isAftSideParallel=false;//是否后一边也有平行约束
	bool isJdInfRight=false;

	jdNum=JDarray.size();
	for(int i=0;i<jdNum;i++)
	{
		curJD=&JDarray[i];//当前交点

		if (curJD->JDXZ == RX_One)//绕行中，不用修正坐标
			continue;
		else if((curJD->JDXZ == RX_Start && i == 0)
			|| curJD->JDXZ == RX_End && i == jdNum-1)//起终点绕行，不用修正坐标
			continue;

		isBfSideParallel=false;
		
		if( curJD->JDXZ == BXPX)
		{//如果交点性质为并行(包括全线起终点)

			//判断该平行交点参照信息是否正确
			isJdInfRight=IsJdBLInfRight(JDarray[i], pBLRHA, blJDNo);

			if(blJDNo == 0 && isJdInfRight)
			{//是全线起点							
				xyddaa(pBLRHA->JDarray[0].N,pBLRHA->JDarray[0].E,
					pBLRHA->JDarray[1].N,pBLRHA->JDarray[1].E,&fwj);
				
				xjj = curJD->XJJ;							
				curJD->N  =  pBLRHA->JDarray[0].N + xjj * cos(fwj+m_dHalfPI);
				curJD->E  =  pBLRHA->JDarray[0].E + xjj * sin(fwj+m_dHalfPI);//计算出二线交点							
			}
			else if(isJdInfRight && blJDNo+1 == pBLRHA->JDarray.size())
			{//全线终点
				int curBLJDNum=pBLRHA->JDarray.size();
				int jd1x=curBLJDNum-2;
				xyddaa(pBLRHA->JDarray[jd1x].N, pBLRHA->JDarray[jd1x].E,
					pBLRHA->JDarray[jd1x+1].N,pBLRHA->JDarray[jd1x+1].E,&fwj);

				//xjj = curJD->XJJ; 818 最后一点线间距
				xjj =JDarray[i-1].XJJ;
				curJD->N  =  pBLRHA->JDarray[jd1x+1].N + xjj * cos(fwj+m_dHalfPI);
				curJD->E  =  pBLRHA->JDarray[jd1x+1].E + xjj * sin(fwj+m_dHalfPI);//计算出二线交点
			}
			else
			{
				//判断该平行交点的前一边是否有平行约束
				isBfSideParallel=IsBfSideRecordParallel(JDarray, i);

				if (isBfSideParallel)
				{//前一边有平行约束，由前后平行边计算交点坐标
					CalExJDByParallel(JDarray, i);
				}
				else
				{
					if(!IsJdBLInfEmpty(JDarray[i]))
					{//该平行交点记录了平行信息，即后一边平行

						//检查本交点信息是否正确，获得参照线和参照线交点号
						if(IsJdBLInfRight(JDarray[i], pBLRHA, blJDNo))
							ModifyJDXYbyOneParall(pBLRHA->JDarray, blJDNo, JDarray, i, false);//修正交点
					}
				}				
			}

		}
		else  if(curJD->JDXZ == RX_Start || curJD->JDXZ == BX_NotPX)
		{//绕行始或并行不平行

			isAftSideParallel=false;

			//判断前一边是否有平行约束
			isBfSideParallel=IsBfSideRecordParallel(JDarray, i);

			//单点绕行，判断后一边是否平行
			if (curJD->JDXZ == RX_Start && JDarray[i+1].JDXZ != RX_One && JDarray[i+1].JDXZ != RX_End)
			{
				if(!IsJdBLInfEmpty(JDarray[i]))
					isAftSideParallel=true;
			}

			
			if(isBfSideParallel && isAftSideParallel)
			{//根据前后平行边计算交点坐标
				CalExJDByParallel(JDarray, i);
			}
			else if(isBfSideParallel)
			{//根据前一直线边修正交点坐标
				
				//检查前一交点信息是否正确，获得参照线和参照线交点号
				if(IsJdBLInfRight(JDarray[i-1], pBLRHA, blJDNo))
					ModifyJDXYbyOneParall(pBLRHA->JDarray, blJDNo, JDarray, i, true);//修正交点
			}
			
		}
		else  if(curJD->JDXZ == RX_End)
		{//绕行止

			if(!IsJdBLInfEmpty(JDarray[i]))
			{//绕行止记录了平行信息
				if(IsJdBLInfRight(JDarray[i], pBLRHA, blJDNo))
					ModifyJDXYbyOneParall(pBLRHA->JDarray, blJDNo, JDarray, i, false);//修正交点
			}
		}
	}
}



/*
功能：判断某一交点的前一边是否有平行约束，如果有返回true
输入：
	jdAry   某交点所在的交点数组
	jdix    某交点的下标
算法：判断前一交点是否记录了平行信息
编写：李帅   17.6.15
*/
bool RHA::IsBfSideRecordParallel(const vector<JDarrayStruct> &jdAry, int iJD) const
{
	if(iJD <= 0 || iJD+1 >= jdAry.size())
		return false;

	bool isBfSideParallel=false;

	if ( JDarray[iJD-1].JDXZ == RX_Start
		|| JDarray[iJD-1].JDXZ == RX_End)
	{
		//如果前一交点是绕行终点，记录了参照线信息
		if (!IsJdBLInfEmpty(JDarray[iJD-1]))
			isBfSideParallel=true;
	}
	else if (JDarray[iJD-1].JDXZ == BXPX && !IsJdBLInfEmpty(JDarray[iJD-1]))
		isBfSideParallel=true;//如果前一交点是平行

	return isBfSideParallel;
}


/*
功能：判断参照线信息是否不全，如果不全返回true
输入：iJD    单个交点
注意：绕行中不需记录参照信息
编写：李帅   17.6.15
*/
bool RHA::IsJdBLInfEmpty(const JDarrayStruct &iJD) const
{
	if(iJD.JDXZ == RX_One)
		return true;

	if (_tcscmp(iJD.BLName, L"") == 0
		|| _tcscmp(iJD.BLName, L"NULL") == 0
		|| _tcscmp(iJD.BLJDName, L"") == 0
		|| _tcscmp(iJD.BLJDName, L"NULL") == 0
	/*	|| iJD.XJJ*/)//818 线间距默认赋多少？
	{
		return true;
	}

	return false;
}

/*
功能：判断参照线信息是否正确，如果正确返回true
输入：iJD    单个交点
算法：检查参照线路名和参照线交点名是否正确
编写：李帅   17.6.15
*/
bool RHA::IsJdBLInfRight(const JDarrayStruct &iJD, RHA* &pBLRHA, int &blJDNo) const
{
	ACHAR mes[100];

	pBLRHA=NULL;
	blJDNo=0;

	//检查参照线路
	pBLRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, iJD.BLName, false);
	if (!pBLRHA)
	{
		_stprintf(mes, L"错误：修正交点坐标时，没有找到%s%s的参照线%s\n"
			, m_XLName, iJD.JDName, iJD.BLName);
		ads_printf(mes);
		return false;
	}

	//检查参照线交点
	if(!GetJdno1FromJdstr(iJD.BLJDName, pBLRHA->JDarray, blJDNo))
	{
		_stprintf(mes, L"错误：修正交点坐标时，没有找到%s%s的参照交点%s\n"
			, m_XLName, iJD.JDName, iJD.BLJDName);
		ads_printf(mes);
		return false;
	}

	return true;
}


/*===================================================================
函数名:CalExJDByParrel   
------------------------------------------------------------
函数功能:由交点前后边的线间距计算二线交点坐标,适用于二线交行交点
------------------------------------------------------------
输入: 
jdAry                   二线交点数组
jdix                    需要修正的二线交点下标
=====================================================================*/
void RHA::CalExJDByParallel(vector<JDarrayStruct> &jdAry, int jdix)
{
	if(jdix <= 0 || jdix+1 >= jdAry.size())//不是起终点
		return;

	RHA *pBfBLRHA=NULL;
	RHA *pCurBLRHA=NULL;
	int bfJDNo=0, curJDNo=0; 
	double bfXJJ=0.0, curXJJ=0.0;
	ACHAR mes[100];
	bool isBfSideParallel=true;//是否前一边也有平行约束
	bool isAftSideParallel=true;//是否后一边也有平行约束

	//检查前一交点信息是否正确，获得参照线pBfBLRHA和参照线交点bfJDNo
	isBfSideParallel=IsJdBLInfRight(jdAry[jdix-1], pBfBLRHA, bfJDNo);
	bfXJJ=jdAry[jdix-1].XJJ;

	//检查当前交点信息是否正确，获得参照线和参照线交点
	isAftSideParallel=IsJdBLInfRight(jdAry[jdix], pCurBLRHA, curJDNo);
	curXJJ=jdAry[jdix].XJJ;


	if(isBfSideParallel && isAftSideParallel)
	{//前后直线边平行

		if(bfJDNo<0 || bfJDNo+2>pBfBLRHA->JDarray.size()
			|| curJDNo<0 || curJDNo+2>pCurBLRHA->JDarray.size())
			return;
		//由线间距求前一条边的平行边（pt1,pt2）
		double fwj;
		double pt1[2],pt2[2];

		xyddaa(pBfBLRHA->JDarray[bfJDNo].N,pBfBLRHA->JDarray[bfJDNo].E
			,pBfBLRHA->JDarray[bfJDNo+1].N,pBfBLRHA->JDarray[bfJDNo+1].E,&fwj);

		pt1[0] = pBfBLRHA->JDarray[bfJDNo].N + bfXJJ * cos(fwj+m_dHalfPI);
		pt1[1] = pBfBLRHA->JDarray[bfJDNo].E + bfXJJ * sin(fwj+m_dHalfPI);
		pt2[0] = pBfBLRHA->JDarray[bfJDNo+1].N + bfXJJ * cos(fwj+m_dHalfPI);
		pt2[1] = pBfBLRHA->JDarray[bfJDNo+1].E + bfXJJ * sin(fwj+m_dHalfPI);

		//由线间距求后一条边的平行边（pt3,pt4）
		double pt3[2],pt4[2];
		xyddaa(pCurBLRHA->JDarray[curJDNo].N,pCurBLRHA->JDarray[curJDNo].E
			,pCurBLRHA->JDarray[curJDNo+1].N,pCurBLRHA->JDarray[curJDNo+1].E,&fwj);
		pt3[0] = pCurBLRHA->JDarray[curJDNo].N + curXJJ * cos(fwj+m_dHalfPI);
		pt3[1] = pCurBLRHA->JDarray[curJDNo].E + curXJJ * sin(fwj+m_dHalfPI);
		pt4[0] = pCurBLRHA->JDarray[curJDNo+1].N + curXJJ * cos(fwj+m_dHalfPI);
		pt4[1] = pCurBLRHA->JDarray[curJDNo+1].E + curXJJ * sin(fwj+m_dHalfPI);

		//两条平行边求交点
		double xtmp=0.0, ytmp=0.0;
		INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

		jdAry[jdix].N=xtmp;
		jdAry[jdix].E=ytmp;
	}
	else if (isBfSideParallel && !isAftSideParallel)
	{//前直线边平行
		ModifyJDXYbyOneParall(pBfBLRHA->JDarray, bfJDNo, JDarray, jdix, true);
	}
	else if (!isBfSideParallel && isAftSideParallel)
	{//后直线边平行
		ModifyJDXYbyOneParall(pCurBLRHA->JDarray, curJDNo, JDarray, jdix, false);
	}
	
}


/*===================================================================
函数名:ModifyJDXYbyOneParall   
------------------------------------------------------------
函数功能:由交点前一边或后一边的线间距,适用于单边平行交点
------------------------------------------------------------
输入: 
jdAry1x                 参照线路的交点数组
iJD1x                   参照线的交点下标
jdAry2x                 二线的交点数组
iJD2x                   二线的交点号（要修正的交点）               
isQorH                  =true，前一边平行；=false，后一边平行
------------------------------------------------------------
输出: jdAry2x[iJD2x]的N/E坐标
------------------------------------------------------------
编写: 李帅 2017.6.16
=====================================================================*/
void RHA::ModifyJDXYbyOneParall(const vector<JDarrayStruct> &jdAry1x, int iJD1x
								, vector<JDarrayStruct> &jdAry2x, int iJD2x, bool isQorH)
{
	if(iJD1x<0 || iJD1x+1 >= jdAry1x.size())
		return;
	if((isQorH && (iJD2x<0 || iJD2x+1 >= jdAry2x.size()))
		|| (!isQorH && (iJD2x <= 0 || iJD2x+1>jdAry2x.size())))
		return;

	double xjj = 0.0;
	double fwj;
	double pt1[2],pt2[2];
	double pt3[2],pt4[2];
	double xtmp,ytmp,tylc1x;
	JDarrayStruct *pJD=NULL;

	pJD=&jdAry2x[iJD2x];

	if (isQorH)
	{//前一边平行
		xjj = jdAry2x[iJD2x - 1].XJJ;
		//如果绕行始前面那个交点的线间距没给定,则不修正坐标
		//if (fabs(xjj) < 0.1)//818 为0不修正？
		//	return;


		xyddaa(jdAry1x[iJD1x].N,jdAry1x[iJD1x].E
			,jdAry1x[iJD1x + 1].N,jdAry1x[iJD1x + 1].E,&fwj);

		pt1[0] = jdAry1x[iJD1x].N + xjj * cos(fwj+m_dHalfPI);
		pt1[1] = jdAry1x[iJD1x].E + xjj * sin(fwj+m_dHalfPI);
		pt2[0] = jdAry1x[iJD1x + 1].N + xjj * cos(fwj+m_dHalfPI);
		pt2[1] = jdAry1x[iJD1x + 1].E + xjj * sin(fwj+m_dHalfPI);
		pt3[0] =  jdAry2x[iJD2x].N;
		pt3[1] =  jdAry2x[iJD2x].E;
		pt4[0] =  jdAry2x[iJD2x + 1].N;
		pt4[1] =  jdAry2x[iJD2x + 1].E;

		INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

		pJD->N = xtmp;
		pJD->E = ytmp;
	}
	else
	{//后一边平行

		xjj = pJD->XJJ;

		//如果绕行止的线间距没给定,则不修正坐标 818
		/*if (fabs(xjj) < 0.1)
		return;*/

		xyddaa(jdAry1x[iJD1x].N,jdAry1x[iJD1x].E
			,jdAry1x[iJD1x + 1].N,jdAry1x[iJD1x + 1].E,&fwj);

		pt1[0] = jdAry1x[iJD1x].N + xjj * cos(fwj+m_dHalfPI);
		pt1[1] = jdAry1x[iJD1x].E + xjj * sin(fwj+m_dHalfPI);
		pt2[0] = jdAry1x[iJD1x + 1].N + xjj * cos(fwj+m_dHalfPI);
		pt2[1] = jdAry1x[iJD1x + 1].E + xjj * sin(fwj+m_dHalfPI);


		pt3[0] =  jdAry2x[iJD2x - 1].N;
		pt3[1] =  jdAry2x[iJD2x - 1].E;
		pt4[0] =  jdAry2x[iJD2x].N;
		pt4[1] =  jdAry2x[iJD2x].E;

		INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

		pJD->N = xtmp;
		pJD->E = ytmp;
	}
}


///************************************************************************
//函数功能：创建夹直线数组
//算法：以交点为单位，将该交点前后夹直线的起终点，交点号作为一个夹直线结构存入夹直线数组中
//备注：赋值时，起终点处夹直线的前后方位角相同
//*************************************************************************/
//void RHA::CreatJZXArray()
//{
//	JZXArray.RemoveAll();
//	NJD=JDarray.size();
//	JZXArray.SetSize(NJD);
//	JZXStruct tempJZX;
//
//	//起点夹直线
//	JZXArray[0].QJZXStartPt.lc = QXB[0].KHZ;
//	xlpoint_pz(array,tarray,&JZXArray[0].QJZXStartPt);
//	JZXArray[0].QJZXEndPt.lc = QXB[0].KZH;
//	xlpoint_pz(array,tarray,&JZXArray[0].QJZXEndPt);
//	JZXArray[0].HJZXStartPt.lc = QXB[0].KHZ;
//	xlpoint_pz(array,tarray,&JZXArray[0].HJZXStartPt);
//	JZXArray[0].HJZXEndPt.lc = QXB[1].KZH;
//	xlpoint_pz(array,tarray,&JZXArray[0].HJZXEndPt);
//	JZXArray[0].HFWJ=fwj(JZXArray[0].HJZXStartPt.y, JZXArray[0].HJZXStartPt.x, JZXArray[0].HJZXEndPt.y, JZXArray[0].HJZXEndPt.x);
//	JZXArray[0].QFWJ=JZXArray[0].HFWJ;
//	JZXArray[0].JDNum = 0;
//
//	//中间夹直线
//	for (int i=1 ; i<NJD-1 ; i++)
//	{
//		JZXArray[i].QJZXStartPt.lc = QXB[i-1].KHZ;
//		xlpoint_pz(array,tarray,&JZXArray[i].QJZXStartPt);
//		JZXArray[i].QJZXEndPt.lc = QXB[i].KZH;
//		xlpoint_pz(array,tarray,&JZXArray[i].QJZXEndPt);
//		JZXArray[i].HJZXStartPt.lc = QXB[i].KHZ;
//		xlpoint_pz(array,tarray,&JZXArray[i].HJZXStartPt);
//		JZXArray[i].HJZXEndPt.lc = QXB[i+1].KZH;
//		xlpoint_pz(array,tarray,&JZXArray[i].HJZXEndPt);
//		JZXArray[i].QFWJ=JZXArray[i-1].HFWJ;
//		JZXArray[i].HFWJ=fwj(JZXArray[i].HJZXStartPt.y, JZXArray[i].HJZXStartPt.x, JZXArray[i].HJZXEndPt.y, JZXArray[i].HJZXEndPt.x);
//		JZXArray[i].JDNum = i; 
//	}
//
//	//终点夹直线
//	JZXArray[NJD-1].QJZXStartPt.lc = QXB[NJD-2].KHZ;
//	xlpoint_pz(array,tarray,&JZXArray[NJD-1].QJZXStartPt);
//	JZXArray[NJD-1].QJZXEndPt.lc = QXB[NJD-1].KZH;
//	xlpoint_pz(array,tarray,&JZXArray[NJD-1].QJZXEndPt);
//	JZXArray[NJD-1].HJZXStartPt.lc = QXB[NJD-1].KHZ;
//	xlpoint_pz(array,tarray,&JZXArray[NJD-1].HJZXStartPt);
//	JZXArray[NJD-1].HJZXEndPt.lc = QXB[NJD-1].KZH;
//	xlpoint_pz(array,tarray,&JZXArray[NJD-1].HJZXEndPt);
//	JZXArray[NJD-1].QFWJ=JZXArray[NJD-2].HFWJ;
//	JZXArray[NJD-2].HFWJ=JZXArray[NJD-2].HFWJ;
//	JZXArray[NJD-1].JDNum = NJD-1;
//}

/*===================================================================
函数名:CalExReference   
------------------------------------------------------------
函数功能:将本线路和其它线路比较几何线位，确定参照关系，生成交点性质和断链
------------------------------------------------------------
输入: isOnlyBaseOnSJX  是否只是参照设计基线，默认=false，=true时
                       ，表示改线路为设计二线只考虑参照设计线的情况
------------------------------------------------------------
算法:
------------------------------------------------------------
说明: isOnlyBaseOnSJX = false，是一般地解算线路参照关系。
      =true，是用于计算设计二线相对设计基线的绕行段
	  ，此时为局部变量RHA调用    818 也相对其它设计二线??参照哪几条设计二线？？？
------------------------------------------------------------
编写: 李帅 2016.5.25  修改 2016.10.31
=====================================================================*/
void RHA::CalExReference(bool isOnlyBaseOnSJJX)
{
	if(isOnlyBaseOnSJJX && XLShuXing != SJ2X)
		ads_printf(L"错误：只有设计二线才做纵面用的改建段分段计算！\n");

	getpxy_DATA();

	CStringArray XLNameAry;
	RHA *pRHA_BL=NULL;

	bool isPxRxd=!isOnlyBaseOnSJJX;

	if(isOnlyBaseOnSJJX)
	{
		//生成设计基线
		XLNameAry.RemoveAll();
		XLNameAry.SetSize(1);
		XLNameAry[0]=SJJX;
		CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, XLNameAry[0]);
	}
	else
	{
		// 查找所有可能的参照线路 
		CCreatPM::GetAllXLNames(m_mdbName,m_XLName, XLNameAry);

		// 生成所有可能的参照线
		CCreatPM::SortXLNames(XLNameAry);
		for(int i=0;i <= XLNameAry.GetSize();i++)
		{
			if (XLNameAry[i] == m_XLName)
			{
				//删除该线路之后的线路名，不可能参照
				XLNameAry.RemoveAt(i, XLNameAry.GetSize()-i/*-1*/);

				for(int j=0; j<XLNameAry.GetSize(); j++)
				{
					CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, XLNameAry[j]);
				}

				break;

			}
		}

	}

	// 按照既有线、设计线排序之后的线路下标数组
	vector<Int16> xlIndxAry;
	Int16 kk, jj;
	xlIndxAry.clear();
	for (int j=0; j<XLNameAry.GetSize(); j++)
	{
		kk=CCreatPM::IsIncluded(m_iDoc, m_mdbName, XLNameAry[j]);	//线路在g_DocManageAry[m_iDoc].XLAry中的下标
		if (kk >= 0)
			xlIndxAry.push_back(kk);
	}

	if (xlIndxAry.size()<1)
		return;

	// 确定交点是否为并行交点，按照并行优先、既有线优先的顺序确定线路的参照关系
	AcGePoint2d pt1, pt2, pt3, pt4, pt5, pt6;
	PXJDInf PXJDtmp;
	bool isBfLinePX=false;//前直线是否平行
	bool isAftLinePX=false;//后直线是否平行
	vector<PXJDInf> BfPXJDAry, AftPXJDAry;

	BfPXJDAry.clear();AftPXJDAry.clear();

	//1 起点
	double fwj=0.0;
	xlpoint PZ;
		//交点连线方位角(弧度)
	xyddaa(JDarray[0].N,JDarray[0].E,JDarray[1].N,JDarray[1].E, &fwj);
	fwj=dms_rad(fwj);
	     
	for (int j=0; j<xlIndxAry.size(); j++)
	{
		kk=xlIndxAry[j];
		
        //第一条直线边是否有平行边
		if (CheckIsPXBasLine(fwj, lsJZXarray[0].SPt, lsJZXarray[0].EPt
			, g_DocManageAry[m_iDoc].XLAry[kk]->JDarray , g_DocManageAry[m_iDoc].XLAry[kk]->lsJZXarray, PXJDtmp, isPxRxd))
			BfPXJDAry.push_back(PXJDtmp);
	}

	if (BfPXJDAry.size()>0)
		isBfLinePX=true;
	else
	{
		JDarray[0].JDXZ=RX_Start;
		isBfLinePX=false;
	}

	_tcscpy(JDarray[0].BLName, L"");//初始化
	_tcscpy(JDarray[0].BLJDName, L"");
	JDarray[0].XJJ=0;
	
	//2 中间交点
	int NJD=JDarray.size();
	for(int i=1;i<NJD-1;i++)
	{
		_tcscpy(JDarray[i].BLName, L"");//初始化 nan
		_tcscpy(JDarray[i].BLJDName, L"");
		JDarray[i].XJJ=0;

		xyddaa(JDarray[i].N,JDarray[i].E,JDarray[i+1].N,JDarray[i+1].E, &fwj);
		fwj=dms_rad(fwj);

		pt1.x=JDarray[i].E;pt1.y=JDarray[i].N;
		pt2.x=JDarray[i+1].E;pt2.y=JDarray[i+1].N;

		AftPXJDAry.clear();
		for (int j=0; j<xlIndxAry.size(); j++)//遍历线路，寻找平行边
		{
			kk=xlIndxAry[j];//线路下标
			
			//后直线边是否平行   
			if (CheckIsPXBasLine(fwj, lsJZXarray[i].SPt, lsJZXarray[i].EPt, g_DocManageAry[m_iDoc].XLAry[kk]->JDarray
				, g_DocManageAry[m_iDoc].XLAry[kk]->lsJZXarray, PXJDtmp, isPxRxd))
				AftPXJDAry.push_back(PXJDtmp);
		}

		if(AftPXJDAry.size()>0)
			isAftLinePX=true;
		else
			isAftLinePX=false;

		JDarray[i].JDXZ=-100;//初值
		//按照并行优先、既有线优先的原则确定是否并行

		if (isBfLinePX && isAftLinePX)//前后直线边都平行
		{
			int jdix1, jdix2;

			for ( jj=0; jj<BfPXJDAry.size(); jj++)
			{
				for (int k=0; k<AftPXJDAry.size(); k++)
				{
					if (_tcscmp(AftPXJDAry[k].BLName, BfPXJDAry[jj].BLName) == 0)
					{
						//前后参照线路不同的并行交点性质赋为单点绕行
						//if(JDarray[i-1].JDXZ == BXPX && _tcscmp(JDarray[i-1].BLName, AftPXJDAry[k].BLName) == 0)
							//break;
						pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, BfPXJDAry[jj].BLName);
						
						if (pRHA_BL && GetJdno1FromJdstr( BfPXJDAry[jj].JDName, pRHA_BL->JDarray,jdix1)
							&& GetJdno1FromJdstr(AftPXJDAry[k].JDName, pRHA_BL->JDarray, jdix2)
							&& jdix1+1 == jdix2) //并行
						{
							JDarray[i].JDXZ=BXPX;
							PXJDtmp=AftPXJDAry[k];
							break;
						}
						
					}

				}

				if (JDarray[i].JDXZ == BXPX)
					break;
			}

			if (JDarray[i].JDXZ != BXPX)
			{
				JDarray[i].JDXZ = RX_Start;//单点绕行
				PXJDtmp=AftPXJDAry[0];
			}
				
		}
		else if (isAftLinePX) //前直线边不平行，后直线边平行
		{
			JDarray[i].JDXZ=RX_End; 
			PXJDtmp=AftPXJDAry[0];
		}
		else if (isBfLinePX) //前直线边平行，后直线边不平行->绕行始
			JDarray[i].JDXZ=RX_Start;
		else                 //前后都不平行>绕行中
			JDarray[i].JDXZ=RX_One;
		
		//起点属性信息
		if (i == 1 && isBfLinePX)
		{
			JDarray[0].JDXZ=BXPX;
			if (JDarray[1].JDXZ != BXPX)
				jj=0;

			_tcscpy(JDarray[0].BLName, BfPXJDAry[jj].BLName);
			_tcscpy(JDarray[0].BLJDName, BfPXJDAry[jj].JDName);
			JDarray[0].XJJ=BfPXJDAry[jj].xjj;
		}

		//其它交点:属性为并行、单点绕行、绕行止时，记录参照线信息   
		if ((isBfLinePX && isAftLinePX)|| JDarray[i].JDXZ == RX_End )
		{
			_tcscpy(JDarray[i].BLName, PXJDtmp.BLName);
			_tcscpy(JDarray[i].BLJDName, PXJDtmp.JDName);
			JDarray[i].XJJ=PXJDtmp.xjj;
			PXJDtmp=PXJDtmp;
			isBfLinePX=true;
		}

		isBfLinePX=isAftLinePX;
		BfPXJDAry=AftPXJDAry;
	}

	//3 终点：根据前一交点赋属性值
	_tcscpy(JDarray[NJD-1].BLName, L"");//初值
	_tcscpy(JDarray[NJD-1].BLJDName, L"");
	JDarray[NJD-1].XJJ=0;

	if (JDarray[NJD-2].JDXZ == BXPX || JDarray[NJD-2].JDXZ == RX_End)
	{
		JDarray[NJD-1].JDXZ=BXPX;
		int bfiBLJD;

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[NJD-2].BLName);
		if(pRHA_BL && GetJdno1FromJdstr(JDarray[NJD-2].BLJDName, pRHA_BL->JDarray, bfiBLJD))
		{
			_tcscpy(JDarray[NJD-1].BLJDName, pRHA_BL->JDarray[bfiBLJD+1].JDName);
			_tcscpy(JDarray[NJD-1].BLName, JDarray[NJD-2].BLName);
		}
		
	}
	else if (JDarray[NJD-2].JDXZ == RX_Start || JDarray[NJD-2].JDXZ == RX_One)
	{
		JDarray[NJD-1].JDXZ=RX_End;
		_tcscpy(JDarray[NJD-1].BLName, L"");
	}

}

//生成改建段数组
//李伟添加
void RHA::CalGJDArray()
{

	GJDInf OneGJD;//一个改建段
	_tcscpy(OneGJD.XLName, m_XLName);
	vector<GJDInf> RXGJDArray, LYDArray, BXGJDArray;//绕行改建段、利用段数组、并行改建段数组
	CStringArray XLNameArray;
	XLNameArray.Add(JYJX);
	vector<JDarrayStruct> RefJDArray;
	CalExReference(XLNameArray, RefJDArray);
	vector<int> SJDArray, EJDArray;
	GetAllRxdSEJD(RefJDArray, SJDArray, EJDArray);

	int RefRXDNum = SJDArray.size();
	int iJD,iJDJX;
	int iRefXL;
	double N,E;
	double Projml_Cur = 0.0, Projml_Base = 0.0, Projml = 0.0, dml = 0.0;
	ACHAR Ckml[MaxCkmlStrNum], GH[10];
	RHA* pBaseLine = NULL;
	CreatJZXArray();
	if (SJDArray.size() >0)
	{
		for (int iRXD=0; iRXD<RefRXDNum; iRXD++)
		{
			iJD = SJDArray[iRXD];
			TrsProjmltoNE(QXB[iJD].KZH, N, E);
			iRefXL=CCreatPM::IsIncluded(m_iDoc, m_mdbName, RefJDArray[0].BLName);
			pBaseLine = g_DocManageAry[m_iDoc].XLAry[iRefXL];
			pBaseLine->TrsNEtoProjml(N, E, Projml_Cur);
			GetJdno1FromJdstr(RefJDArray[iJD-1].BLJDName, pBaseLine->JDarray, iJDJX);
			iJDJX++;
			//if (iJDJX == -1)
			//	SearchJZXCDJDIndex_OnJYJX(iJD, -1, pBaseLine, 1, pBaseLine->JDarray.size()-1, iJDJX);
			Projml_Base = pBaseLine->QXB[iJDJX].KZH;
			Projml = min(Projml_Base, Projml_Cur);
			Projml = pBaseLine->LocateHundredMeterOnLine(Projml, true, Ckml);
			pBaseLine->TrsProjmltoNE(Projml, N, E);
			TrsNEtoProjml(N, E, OneGJD.BProjml);
			TrsProjmltoNE(OneGJD.BProjml, OneGJD.spt[0], OneGJD.spt[1]);

			_tcscpy(OneGJD.BXLName, RefJDArray[0].BLName);
			_tcscpy(OneGJD.BLC, Ckml);
			OneGJD.GJDType = 2;
			split_ckml(Ckml, dml, GH);
			_stprintf(OneGJD.gjdname, L"%.1lf绕行改建段", dml/1000.0);

			iJD = EJDArray[iRXD];
			TrsProjmltoNE(QXB[iJD].KHZ, N, E);
			iRefXL=CCreatPM::IsIncluded(m_iDoc, m_mdbName, RefJDArray[0].BLName);
			pBaseLine = g_DocManageAry[m_iDoc].XLAry[iRefXL];
			pBaseLine->TrsNEtoProjml(N, E, Projml_Cur);
			GetJdno1FromJdstr(RefJDArray[iJD].BLJDName, pBaseLine->JDarray, iJDJX);
			//if (iJDJX == -1)
			//	SearchJZXCDJDIndex_OnJYJX(iJD, 1, pBaseLine, 0, pBaseLine->JDarray.size()-2, iJDJX);
			Projml_Base = pBaseLine->QXB[iJDJX].KHZ;
			Projml = max(Projml_Base, Projml_Cur);
			Projml = pBaseLine->LocateHundredMeterOnLine(Projml, false, Ckml);
			pBaseLine->TrsProjmltoNE(Projml, N, E);
			TrsNEtoProjml(N, E, OneGJD.EProjml);
			TrsProjmltoNE(OneGJD.EProjml, OneGJD.ept[0], OneGJD.ept[1]);
			_tcscpy(OneGJD.EXLName, RefJDArray[0].BLName);
			_tcscpy(OneGJD.ELC, Ckml);

			RXGJDArray.push_back(OneGJD);
		}
	}

	MergeGTZX2GJDArray(RXGJDArray);
	//寻找利用段
	iRefXL=CCreatPM::IsIncluded(m_iDoc, m_mdbName, RefJDArray[0].BLName);
	pBaseLine = g_DocManageAry[m_iDoc].XLAry[iRefXL];
	pBaseLine->CreatJZXArray();
	CreatLYDArrayOnJYX(pBaseLine, LYDArray);
	for (int i=0; i<LYDArray.size(); i++)
		GJDInforArray.push_back(LYDArray[i]);
	for (int i=0; i<RXGJDArray.size(); i++)
		GJDInforArray.push_back(RXGJDArray[i]);
	SortGJDArray(GJDInforArray);

	double SProjml=0.0, EProjml;
	for (int i=0; i<GJDInforArray.size(); i++)
	{
		EProjml = GJDInforArray[i].BProjml;
		if (EProjml-SProjml > 1.0)
		{
			OneGJD.BProjml = SProjml;
			TrsProjmltoNE(SProjml, OneGJD.spt[0], OneGJD.spt[1]);
			_tcscpy(OneGJD.BXLName, RefJDArray[0].BLName);
			pBaseLine->TrsNEtoCkml(OneGJD.spt[0], OneGJD.spt[1], OneGJD.BLC);
			split_ckml(OneGJD.BLC, dml, GH);
			_stprintf(OneGJD.gjdname, L"%.1lf并行改建段", dml/1000.0);
			
			OneGJD.EProjml = EProjml;
			TrsProjmltoNE(EProjml, OneGJD.ept[0], OneGJD.ept[1]);
			_tcscpy(OneGJD.EXLName, RefJDArray[0].BLName);
			pBaseLine->TrsNEtoCkml(OneGJD.ept[0], OneGJD.ept[1], OneGJD.ELC);

			OneGJD.GJDType = 0;
			GJDInforArray.insert(GJDInforArray.begin()+i, OneGJD);
			i++;
		}
		SProjml = GJDInforArray[i].EProjml;
	}

	//最后一段到终点
	if (SProjml < xl_length-0.01)
	{
		EProjml = xl_length;
		OneGJD.BProjml = SProjml;
		TrsProjmltoNE(SProjml, OneGJD.spt[0], OneGJD.spt[1]);
		_tcscpy(OneGJD.BXLName, RefJDArray[0].BLName);
		pBaseLine->TrsNEtoCkml(OneGJD.spt[0], OneGJD.spt[1], OneGJD.BLC);
		split_ckml(OneGJD.BLC, dml, GH);
		_stprintf(OneGJD.gjdname, L"%.1lf并行改建段", dml/1000.0);

		OneGJD.EProjml = EProjml;
		TrsProjmltoNE(EProjml, OneGJD.ept[0], OneGJD.ept[1]);
		_tcscpy(OneGJD.EXLName, RefJDArray[0].BLName);
		pBaseLine->TrsNEtoCkml(OneGJD.ept[0], OneGJD.ept[1], OneGJD.ELC);

		OneGJD.GJDType = 0;
		GJDInforArray.push_back(OneGJD);
	}
}


//比较几何，确定参照关系
//李伟添加
void RHA::CalExReference(CStringArray& XLNameAry, vector<JDarrayStruct>& RefJDArray)
{
	getpxy_DATA();

	RefJDArray.clear();
	RefJDArray = JDarray;

	RHA *pRHA_BL=NULL;
	bool isPxRxd = true;
	CCreatPM::SortXLNames(XLNameAry);
	for (int i=0; i<XLNameAry.GetSize(); i++)
	{
		CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, XLNameAry[0]);
	}

	// 按照既有线、设计线排序之后的线路下标数组
	vector<Int16> xlIndxAry;
	Int16 kk, jj;
	xlIndxAry.clear();
	for (int j=0; j<XLNameAry.GetSize(); j++)
	{
		kk=CCreatPM::IsIncluded(m_iDoc, m_mdbName, XLNameAry[j]);	//线路在g_DocManageAry[m_iDoc].XLAry中的下标
		if (kk >= 0)
			xlIndxAry.push_back(kk);
	}

	if (xlIndxAry.size()<1)
		return;

	// 确定交点是否为并行交点，按照并行优先、既有线优先的顺序确定线路的参照关系
	AcGePoint2d pt1, pt2, pt3, pt4, pt5, pt6;
	PXJDInf PXJDtmp;
	bool isBfLinePX=false;//前直线是否平行
	bool isAftLinePX=false;//后直线是否平行
	vector<PXJDInf> BfPXJDAry, AftPXJDAry;

	BfPXJDAry.clear();AftPXJDAry.clear();

	//1 起点
	double fwj=0.0;
	xlpoint PZ;
	//交点连线方位角(弧度)
	xyddaa(RefJDArray[0].N,RefJDArray[0].E,RefJDArray[1].N,RefJDArray[1].E, &fwj);
	fwj=dms_rad(fwj);

	for (int j=0; j<xlIndxAry.size(); j++)
	{
		kk=xlIndxAry[j];

		//第一条直线边是否有平行边
		if (CheckIsPXBasLine(fwj, lsJZXarray[0].SPt, lsJZXarray[0].EPt
			, g_DocManageAry[m_iDoc].XLAry[kk]->JDarray , g_DocManageAry[m_iDoc].XLAry[kk]->lsJZXarray, PXJDtmp, isPxRxd))
			BfPXJDAry.push_back(PXJDtmp);
	}

	if (BfPXJDAry.size()>0)
		isBfLinePX=true;
	else
	{
		RefJDArray[0].JDXZ=RX_Start;
		isBfLinePX=false;
	}

	_tcscpy(RefJDArray[0].BLName, L"");//初始化
	_tcscpy(RefJDArray[0].BLJDName, L"");
	RefJDArray[0].XJJ=0;

	//2 中间交点
	int NJD=RefJDArray.size();
	for(int i=1;i<NJD-1;i++)
	{
		_tcscpy(RefJDArray[i].BLName, L"");//初始化 nan
		_tcscpy(RefJDArray[i].BLJDName, L"");
		RefJDArray[i].XJJ=0;

		xyddaa(RefJDArray[i].N,RefJDArray[i].E,RefJDArray[i+1].N,RefJDArray[i+1].E, &fwj);
		fwj=dms_rad(fwj);

		pt1.x=RefJDArray[i].E;pt1.y=RefJDArray[i].N;
		pt2.x=RefJDArray[i+1].E;pt2.y=RefJDArray[i+1].N;

		AftPXJDAry.clear();
		for (int j=0; j<xlIndxAry.size(); j++)//遍历线路，寻找平行边
		{
			kk=xlIndxAry[j];//线路下标

			//后直线边是否平行   
			if (CheckIsPXBasLine(fwj, lsJZXarray[i].SPt, lsJZXarray[i].EPt, g_DocManageAry[m_iDoc].XLAry[kk]->JDarray
				, g_DocManageAry[m_iDoc].XLAry[kk]->lsJZXarray, PXJDtmp, isPxRxd))
				AftPXJDAry.push_back(PXJDtmp);
		}

		if(AftPXJDAry.size()>0)
			isAftLinePX=true;
		else
			isAftLinePX=false;

		RefJDArray[i].JDXZ=-100;//初值
		//按照并行优先、既有线优先的原则确定是否并行

		if (isBfLinePX && isAftLinePX)//前后直线边都平行
		{
			int jdix1, jdix2;

			for ( jj=0; jj<BfPXJDAry.size(); jj++)
			{
				for (int k=0; k<AftPXJDAry.size(); k++)
				{
					if (_tcscmp(AftPXJDAry[k].BLName, BfPXJDAry[jj].BLName) == 0)
					{
						//前后参照线路不同的并行交点性质赋为单点绕行
						//if(RefJDArray[i-1].JDXZ == BXPX && _tcscmp(RefJDArray[i-1].BLName, AftPXJDAry[k].BLName) == 0)
						//break;
						pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, BfPXJDAry[jj].BLName);

						if (pRHA_BL && GetJdno1FromJdstr( BfPXJDAry[jj].JDName, pRHA_BL->JDarray,jdix1)
							&& GetJdno1FromJdstr(AftPXJDAry[k].JDName, pRHA_BL->JDarray, jdix2)
							&& jdix1+1 == jdix2) //并行
						{
							RefJDArray[i].JDXZ=BXPX;
							PXJDtmp=AftPXJDAry[k];
							break;
						}

					}

				}

				if (RefJDArray[i].JDXZ == BXPX)
					break;
			}

			if (RefJDArray[i].JDXZ != BXPX)
			{
				RefJDArray[i].JDXZ = RX_Start;//单点绕行
				PXJDtmp=AftPXJDAry[0];
			}

		}
		else if (isAftLinePX) //前直线边不平行，后直线边平行
		{
			RefJDArray[i].JDXZ=RX_End; 
			PXJDtmp=AftPXJDAry[0];
		}
		else if (isBfLinePX) //前直线边平行，后直线边不平行->绕行始
			RefJDArray[i].JDXZ=RX_Start;
		else                 //前后都不平行>绕行中
			RefJDArray[i].JDXZ=RX_One;

		//起点属性信息
		if (i == 1 && isBfLinePX)
		{
			RefJDArray[0].JDXZ=BXPX;
			if (RefJDArray[1].JDXZ != BXPX)
				jj=0;

			_tcscpy(RefJDArray[0].BLName, BfPXJDAry[jj].BLName);
			_tcscpy(RefJDArray[0].BLJDName, BfPXJDAry[jj].JDName);
			RefJDArray[0].XJJ=BfPXJDAry[jj].xjj;
		}

		//其它交点:属性为并行、单点绕行、绕行止时，记录参照线信息   
		if ((isBfLinePX && isAftLinePX)|| RefJDArray[i].JDXZ == RX_End )
		{
			_tcscpy(RefJDArray[i].BLName, PXJDtmp.BLName);
			_tcscpy(RefJDArray[i].BLJDName, PXJDtmp.JDName);
			RefJDArray[i].XJJ=PXJDtmp.xjj;
			PXJDtmp=PXJDtmp;
			isBfLinePX=true;
		}

		isBfLinePX=isAftLinePX;
		BfPXJDAry=AftPXJDAry;
	}

	//3 终点：根据前一交点赋属性值
	_tcscpy(RefJDArray[NJD-1].BLName, L"");//初值
	_tcscpy(RefJDArray[NJD-1].BLJDName, L"");
	RefJDArray[NJD-1].XJJ=0;

	if (RefJDArray[NJD-2].JDXZ == BXPX || RefJDArray[NJD-2].JDXZ == RX_End)
	{
		RefJDArray[NJD-1].JDXZ=BXPX;
		int bfiBLJD;

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, RefJDArray[NJD-2].BLName);
		if(pRHA_BL && GetJdno1FromJdstr(RefJDArray[NJD-2].BLJDName, pRHA_BL->JDarray, bfiBLJD))
		{
			_tcscpy(RefJDArray[NJD-1].BLJDName, pRHA_BL->JDarray[bfiBLJD+1].JDName);
			_tcscpy(RefJDArray[NJD-1].BLName, RefJDArray[NJD-2].BLName);
		}

	}
	else if (RefJDArray[NJD-2].JDXZ == RX_Start || RefJDArray[NJD-2].JDXZ == RX_One)
	{
		RefJDArray[NJD-1].JDXZ=RX_End;
		_tcscpy(RefJDArray[NJD-1].BLName, L"");
	}
}

//NE坐标转投影里程
void RHA::TrsNEtoProjml(double N, double E, double& projml) const//818 函数相同
{
	projml = PROJ_ML(array, tarray, N, E, N, E);
}

//NE坐标转现场里程
void RHA::TrsNEtoCkml(double N, double E, ACHAR Ckml[]) const
{
	double projml = PROJ_ML(array, tarray, N, E, N, E);
	TrsProjmltoCkml(projml, Ckml, 3);
}


/*************************************************************
函数名称: GetiRXDFromNE
函数功能: 由经距、纬距，求所处绕行段序号（0开始）
输入参数说明: N 、E         纬距、经距
函数返回值的说明:  >= 0:返回绕行段编号 -1:该里程在并行段
编写:  李帅  2016.7.1
***************************************************************/
Int16 RHA::GetiRXDFromNE(double N, double E)
{
	double projml=PROJ_ML(array, tarray, N, E, N, E);
	Int16 prevdlix=FindNearDL(projml, DLarray, true);
	if (prevdlix==DLarray.size()-1)
		return DLarray[prevdlix].Indx;
	//判断是否在绕行段内
	if (prevdlix >= 0 && prevdlix+1<DLarray.size() && DLarray[prevdlix].Indx>-1 
		&& DLarray[prevdlix].Indx == DLarray[prevdlix+1].Indx)
		return DLarray[prevdlix].Indx;
	else
		return -1;
}

/*************************************************************
函数名称: GetiRXDFromProjml
函数功能: 由投影里程，求所处绕行段序号（0开始）
输入参数说明: projml        投影里程
函数返回值的说明:  >= 0:返回绕行段编号 -1:该里程在并行段
编写:  李帅  2016.7.1
***************************************************************/
Int16 RHA::GetiRxdByProjml(double projml) const
{
	//Int16 prevdlix=FindNearDL(projml, DLarray, true);
	////判断是否在绕行段内
	//if (prevdlix >= 0 && prevdlix<DLarray.size()-1 && DLarray[prevdlix].Indx>-1 
	//	&& DLarray[prevdlix].Indx == DLarray[prevdlix+1].Indx)
	//	return DLarray[prevdlix].Indx;
	//else
	//	return -1;
	int rxdNum=GetRXDNum();
	int sDL=0, eDL=0;
	ACHAR mes[200];

	for(int i=0; i<rxdNum; i++)
	{
		GetiRXDDLinfo(i, sDL, eDL);
		if (sDL>-1 && eDL>-1 && sDL<eDL)
		{
			if (projml+0.1>DLarray[sDL].TYLC
				&& projml<DLarray[eDL].TYLC+0.1)
			{
				return i;
			}
		}
		else
		{
			_stprintf(mes, L"错误：找不到编号为%d的绕行段起终断链！\n", i);
			ads_printf(mes);
		}
		
	}

	return -1;
}

/*************************************************************
函数名称: GetiRXDFromNE
函数功能: 由经距、纬距，求所处绕行段序号（0开始）
输入参数说明: N 、E         纬距、经距
函数返回值的说明:  
		      绕行段编号    >= 0:返回绕行段编号 -1:该里程在并行段
			  projml         N 、E对应的投影里程
编写:  李帅  2016.7.23
***************************************************************/
Int16 RHA::GetiRXDFromNE(double N, double E, double &projml)
{
	projml=PROJ_ML(array, tarray, N, E, N, E);
	Int16 prevdlix=FindNearDL(projml, DLarray, true);
	//判断是否在绕行段内
	if (prevdlix >= 0 && prevdlix+1<DLarray.size() && DLarray[prevdlix].Indx>-1 
		&& DLarray[prevdlix].Indx == DLarray[prevdlix+1].Indx)
		return DLarray[prevdlix].Indx;
	else
		return -1;
}



/*************************************************************
函数名称:    
函数功能: 引出线路时，获得三个交点的交点数组，用于生成临时线
输入参数: 
OutPt               从线路上引出时在线路上拾取的一点
PT1、PT2            拾取的切线起终点
输出参数:
jdAry               生成的三交点数组
函数返回值:         如果成功生成交点数组，返回true
编写：//plh06.01
***************************************************************///818 改名
bool RHA::AddRXD(AcGePoint3d PT1, AcGePoint3d PT2, AcGePoint3d OutPt
				 ,CArray<JDarrayStruct,JDarrayStruct>& jdAry, ACHAR ckml[], BOOL isPreMove)//返回
{
	assertReadEnabled();

	double x1,y1,x2,y2,L1,L2,R;
	int preJD;//前个交点
	preJD=GetPreJD(OutPt.y,OutPt.x);

	if(preJD+1 >= JDarray.size())
		return false;

	bool isSuccess=false;

	getJD_RL(0,&R,&L1,&L2,&y1,&x1);
	if(preJD>=0 && preJD+1<JDarray.size())
	{
		getJD_RL(preJD,&R,&L1,&L2,&y1,&x1);			
		getJD_RL(preJD+1,&R,&L1,&L2,&y2,&x2);	
	}

	if(preJD >= 0&& preJD+1 < JDarray.size())
	{
		AcGePoint3d interPt;
		AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
		AcDbLine L2(PT1,PT2);
		if(INTERS(&interPt.x,&interPt.y,x1,y1,x2,y2,PT1.x,PT1.y,PT2.x,PT2.y)>=0)
		{
			double T2=QXB[preJD].T2;//后切长
			if(T2<0.00001)T2=50.0;//起点后切为0
			double fwj=0.0;
			AcGePoint3d tempPt(x1,y1,0.0), Pt;

			fwj=BAS_CALC_FUN::CalFWJ(x1,y1,x2,y2);
			getLineEndPt(tempPt,T2,fwj,tempPt);//tempPt是preJD的HZ点坐标

			double L0 = 0.0;
			double rad = GetRFromT1T2(L0,tempPt.x,tempPt.y,interPt.x,interPt.y,PT2.x,PT2.y,!isGJD,true,DV,0,1 , isPreMove );
			if(rad<0.0)
				return false;

			jdAry.RemoveAll();
			jdAry.SetSize(3);

			_tcscpy(jdAry[0].JDName,L"JD0");
			jdAry[0].N=tempPt.y;
			jdAry[0].E=tempPt.x;
			jdAry[0].l1=0.0;
			jdAry[0].R=0.0;
			jdAry[0].l2=0.0;

			_tcscpy(jdAry[1].JDName , L"JD1");
			jdAry[1].N=interPt.y;
			jdAry[1].E=interPt.x;//jdarray[1].l1=get_l0(rad,DV,!isGJD);
			jdAry[1].l1=L0;
			jdAry[1].R=rad;
			jdAry[1].l2=L0;

			_tcscpy(jdAry[2].JDName , L"JD2");
			jdAry[2].N=PT2.y;
			jdAry[2].E=PT2.x;
			jdAry[2].l1=0.0;
			jdAry[2].R=0.0;
			jdAry[2].l2=0.0;

			double fwj1, fwj2;
			double a;
			double T1;
			double KZH;
			//ACHAR ckml[50];

			xyddaa(jdAry[0].N, jdAry[0].E, jdAry[1].N, jdAry[1].E, &fwj1);
			xyddaa(jdAry[1].N, jdAry[1].E, jdAry[2].N, jdAry[2].E ,&fwj2);
			a=ZXJ(fwj1, fwj2);
			T1=CalT(a, rad, L0);

			fwj=BAS_CALC_FUN::CalFWJ(jdAry[1].E, jdAry[1].N, jdAry[0].E, jdAry[0].N);
			getLineEndPt(interPt,T1,fwj,Pt);//第二个交点的ZH点坐标

			//ZH点统一里程
			KZH=PROJ_ML(array, tarray, Pt.y, Pt.x, Pt.y, Pt.x);
			xlpoint PZ;
			PZ.lc=LocatHmOnLine(QXB[preJD].KHZ, KZH, true, ckml);
			if (PZ.lc >= 0)
			{
				xlpoint_pz(array, tarray, PZ);
				jdAry[0].N=PZ.x;
				jdAry[0].E=PZ.y;
			}

			isSuccess=true;
		}
		else
		{
			acutPrintf(L"AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd)....if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0) \n");
		}
	}

	return isSuccess;
}

//获得对应交点下标(JDi)的半径、缓长、N E坐标
void RHA::getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE )
{   
	assertReadEnabled();	
	if (JDi >= 0 && JDi+1 <= JDarray.size()) 
	{ 
		*NN=JDarray[JDi].N;
		*EE=JDarray[JDi].E;
		*L1i=JDarray[JDi].l1;
		*L2i=JDarray[JDi].l2;
		*Ri=JDarray[JDi].R;
	};	 
}



void RHA::MarkXJJ(AcGiWorldDraw* pWd)
{
	int i;
	AcGePoint3d Pt1,Pt2,Pt0;
	double zhlc,hzlc,lc2x,lc1x,angl;
	xlpoint PZ;
	ACHAR XJJ[20];
	int NJD=JDarray.size();
	RHA *pRHA_BL;

	//标注S弯线间距 818 ？？
	for(i=0;i<NJD-1;i++)//818 最后一个交点不标注?
	{
		//绕行段、并行不平行段不标线间距
		if( ((JDarray[i].JDXZ == RX_Start || JDarray[i].JDXZ == RX_One) 
			   && (JDarray[i+1].JDXZ == RX_One || JDarray[i+1].JDXZ == RX_End)) 
			|| (JDarray[i].JDXZ == BX_NotPX/* && JDarray[i+1].JDXZ == BX_NotPX*/))
			continue;

		hzlc=QXB[i].KHZ;
		zhlc=QXB[i+1].KZH;

		int iRXD=-1;
		int sdl, edl;
		if (JDarray[i].JDXZ == RX_End || JDarray[i].JDXZ == RX_Start)
		{
			//前有绕行止(包括单点绕行)，标在绕行止断链和ZH点中间
			iRXD=GetiRxdByProjml(QXB[i].KZH);
			if(iRXD>-1)
			{
				GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
				lc2x=0.5*(DLarray[edl].TYLC+zhlc);
			}
			else
				continue;

		}
		else if(JDarray[i+1].JDXZ == RX_Start)
		{
			//后有绕行始，标在HZ点和绕行始断链中间
			iRXD=GetiRxdByProjml(QXB[i+1].KHZ);
			if(iRXD>-1)
			{
				GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
				lc2x=0.5*(hzlc+DLarray[sdl].TYLC);
			}
			else
				continue;

		}
		else
		{
			//并行标在夹直线中间
			lc2x=0.5*(zhlc+hzlc);
		}

		if(JDarray[i+1].JDXZ==S_Start)
			lc2x=zhlc-10*texth;
		else if(JDarray[i].JDXZ==S_End)
			lc2x=hzlc+10*texth;
		
		PZ.lc=lc2x;
		xlpoint_pz(array,tarray,PZ);
		angl=3.0*PI-PZ.a;
		Pt1.x=PZ.y;
		Pt1.y=PZ.x;//Pt1 二线点

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[i].BLName);
		//blix=CCreatPM::IsIncluded(m_iDoc, m_mdbName, JDarray[i].BLName);//一线下标
		if (pRHA_BL == NULL)
			continue;

		lc1x=PROJ_ML(pRHA_BL->array,pRHA_BL->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PZ.lc=lc1x;
		xlpoint_pz(pRHA_BL->array,pRHA_BL->tarray,PZ);
		Pt2.x=PZ.y;
		Pt2.y=PZ.x;//Pt2 一线点

		_stprintf(XJJ,L"%.2lf",fabs(JDarray[i].XJJ));//plh05.06

		if(_wtof(XJJ)<0.01)
			continue;

		PZ.lc=lc2x-0.5*texth;
		xlpoint_pz(array,tarray,PZ);

		Pt0.x=PZ.y+_tcslen(XJJ)*10.0*texth*sin(PZ.a+m_dHalfPI);
		Pt0.y=PZ.x+_tcslen(XJJ)*10.0*texth*cos(PZ.a+m_dHalfPI);
		G_maketext(pWd,Pt0,XJJ,angl,10.0*texth,QXYSColor,1);
		MarkXJJFlag(pWd,Pt1,Pt2,80.0*texth);
	}
}

void RHA::MarkXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len)
//标注线间距：1 线上一点，2 线上一点，标注线长度=20mm*DrawParam.SCALE，。。。。
{
	BAS_CALC_FUN ob;
	double jiao=ob.CalFWJ(Pt1.x,Pt1.y,Pt2.x,Pt2.y);
	AcGePoint3d Pt3,Pt4,Pt5,Pt6;

	
	getLineEndPt(Pt1,len,jiao+pi,Pt3);
	getLineEndPt(Pt1,0.3*len,jiao-0.1*pi+pi,Pt5);

	getLineEndPt(Pt2,len,jiao,Pt4);
	getLineEndPt(Pt2,0.3*len,jiao-0.1*pi,Pt6);

	G_makeline(pWd,Pt1,Pt3,QXYSColor);	
	G_makeline(pWd,Pt1,Pt5,QXYSColor);
	G_makeline(pWd,Pt2,Pt4,QXYSColor);
	G_makeline(pWd,Pt2,Pt6,QXYSColor);
}

//求出某点在线路上投影点的前后交点N/E坐标,即获取所在边 plh07.17 
int RHA::GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2)
{
	int i=GetPreJD(N,E);

	N1=JDarray[i].N;
	E1=JDarray[i].E;
	N2=JDarray[i+1].N;
	E2=JDarray[i+1].E;
	return i;
}

//求出某点在线路上投影点的前一交点号
int RHA::GetPreJD(double N, double E) const
{
	double tylc=PROJ_ML(array,tarray,N,E,N,E);
	return DistToNearJD(tylc,true);
}

/*
功能：在增加交点时，判断拾取点所在范围是否允许增加交点
输入：
		N/E           拾取点坐标
输出：
		iRxd          如果在绕行段，输出绕行段编号；如果在并行段输出-2
返回： 如果允许增加交点，返回true
算法： 在两个绕行点之间或在两个并行不平行点之间才可以增加交点
编写： ls 2017.6.29
*/
bool RHA::JudgeIfCanAddJD(double N, double E, int &iRxd)
{
	bool isOK=false;
	int iJD=0;
	iRxd=-2;

	//前一交点下标
	iJD=GetPreJD(N, E);

	if(iJD<0 || iJD+1>JDarray.size())
		return false;

	if (JDarray[iJD].JDXZ == RX_Start || JDarray[iJD].JDXZ == RX_One)
	{//绕行段

		if(iJD+1<JDarray.size() && (JDarray[iJD+1].JDXZ == RX_One || JDarray[iJD+1].JDXZ == RX_End))
		{//后面还有一个绕行点

			iRxd=GetiRxdByProjml((QXB[iJD].KHZ+QXB[iJD+1].KZH)/2.0);
			if (iRxd>-1)
				isOK=true;
		}
	}
	else if (JDarray[iJD].JDXZ == BX_NotPX)
	{//并行不平行段

		if(iJD+1<JDarray.size() && JDarray[iJD+1].JDXZ == BX_NotPX)
		{//后面还有一个并行不平行点
				isOK=true;
		}
	}

	return isOK;
}

/*
功能：在删除交点时，判断拾取点所在范围是否允许删除交点
输入：
		N/E           拾取点坐标
输出：
		iJD           拾取点所在的交点下标
		iRxd          如果在绕行段，输出绕行段编号；如果在并行段输出-2
返回： 如果允许删除交点，返回true
算法： 前后都是绕行点或都是并行不平行点，才可以删除交点
编写： ls 2017.6.29
*/
bool RHA::JudgeIfCanDelJD(double N, double E, int &iJD, int &iRxd)
{
	bool isOK=false;
	double Projml=0.0;

	iRxd=-2;
	iJD=-1;

	//获取交点号
	TrsNEtoProjml(N, E, Projml);
	for (int i=0;i<JDarray.size();i++)
	{
		if (QXB[i].KZH<Projml && QXB[i].KHZ>Projml)
		{
			iJD=i;
			break;
		}
	}

	if (iJD == -1)
	{
		ads_printf(L"提示：所选点位不在曲线范围内\n");
		return false;
	}
	else if (iJD <= 0 || iJD+1 >= JDarray.size())
	{
		ads_printf(L"提示：头尾交点不可删\n");
		return false;
	}

	//检查是否可以删除交点
	if (JDarray[iJD].JDXZ == RX_One)
	{//绕行中
		iRxd=GetiRxdByProjml((QXB[iJD].KHZ+QXB[iJD].KZH)/2.0);
		if (iRxd>-1)
			isOK=true;
	}
	else if (JDarray[iJD].JDXZ == BX_NotPX)
	{//并行不平行段

		if(JDarray[iJD-1].JDXZ == BX_NotPX && JDarray[iJD+1].JDXZ == BX_NotPX)
		{//前后都是并行不平行交点
			isOK=true;
		}
	}

	return isOK;
}

//给定统一里程，是否向前搜索，返回与附近的交点号
int RHA::DistToNearJD(double projml, bool IsForwardTo) const//818 findprejd 是小于HZ
{
	int iJD=-1;
	int NJD=JDarray.size();

	for(int i=0;i<NJD;i++)
	{
		if(projml < 0.5*(QXB[i].KZH+QXB[i].KHZ)+0.000001)//小于曲中几何里程
		{			
			iJD=i;
			break;
		}
	}

	if(IsForwardTo)//向前搜索
	{
		iJD--; 
		if(iJD<0)
			iJD=0;
	}

	return iJD;	
}

//给定几何里程寻找该里程前一交点号
int RHA::FindPreJD(double projml)
{
	int iJD=-1;
	int NJD=JDarray.size();

	for(int i=0;i<NJD;i++)
	{
		if(projml+0.000001 < QXB[i].KZH)//小于曲中几何里程
		{			
			iJD=i;
			break;
		}
	}

	return iJD;
}

bool RHA::moveJZXAt(ads_point ptbase,ads_point ptout, int QQorHq)//818 该函数无用？
{ 
	int JDno,JDno2;
	int NJD=JDarray.size();
	//double A2;

	JDno=GetPreJD(ptbase[1],ptbase[0]);
	JDno2=JDno+1;
	double OutN,OutE;
	OutN=ptout[1];
	OutE=ptout[0];
	if (QQorHq==1)//前切
	{
		double Lo1,Lo2,p1,R1,tR1,A2;
		if (JDno<1)
		{
			return false;
		}
		R1=JDarray[JDno].R;
		Lo2=JDarray[JDno].l2;
		Lo1=JDarray[JDno].l1;
		if(Lo2>0.0)
		{
			A2=sqrt(R1*Lo2);
			double x=spiral_y(A2,Lo2);
			double t=Lo2/(2.0*R1);
			double dr=x+R1*sin(t+PI/2);			
			p1=dr-R1;
		}
		else p1=0.0;					
		//半径+内移量					
		tR1=R1+p1;
		//求前圆圆心
		double CenN,CenE;
		double N1,E1,N0,E0,N2,E2;
		N1=JDarray[JDno-1].N;
		E1=JDarray[JDno-1].E;
		N0=JDarray[JDno].N;
		E0=JDarray[JDno].E;
		N2=JDarray[JDno2].N;
		E2=JDarray[JDno2].E;
		GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,Lo1,Lo2,CenN,CenE);
		double TN,TE;//切点
		//求切点
		int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
		GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
		double dis=sqrt((JDarray[JDno2].N-JDarray[JDno].N)*(JDarray[JDno2].N-JDarray[JDno].N)+(JDarray[JDno2].E-JDarray[JDno].E)*(JDarray[JDno2].E-JDarray[JDno].E));
		INTERS(&JDarray[JDno].N,&JDarray[JDno].E,N1,E1,N0,E0,TN,TE,OutN,OutE);
		if (JDno2==NJD-1)
		{
			double FWJ;
			FWJ=CalFWJ(JDarray[JDno].N,JDarray[JDno].E,OutN,OutE);
			JDarray[JDno2].N=JDarray[JDno].N+dis*sin(FWJ);
			JDarray[JDno2].E=JDarray[JDno].E+dis*cos(FWJ);
		}
		else
			INTERS(&JDarray[JDno2].N,&JDarray[JDno2].E,N2,E2,JDarray[JDno2+1].N,JDarray[JDno2+1].E,TN,TE,OutN,OutE);
	}

	if (QQorHq==2)//后切
	{
		double Lo1,Lo2,R2,p2,tR2,A1;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

		if (JDno2>=NJD-1)
		{
			return false;
		}
		R2=JDarray[JDno2].R;
		Lo2=JDarray[JDno2].l2;
		Lo1=JDarray[JDno2].l1;
		if(Lo1>0.0)
		{
			A1=sqrt(R2*Lo1);
			double x=spiral_y(A1,Lo1);
			double t=Lo1/(2.0*R2);
			double dr=x+R2*sin(t+PI/2);			
			p2=dr-R2;
		}
		else p2=0.0;					
		//半径+内移量					
		tR2=R2+p2;
		//求前圆圆心
		double CenN,CenE;
		double N1,E1,N0,E0,N2,E2;
		N1=JDarray[JDno].N;
		E1=JDarray[JDno].E;
		N0=JDarray[JDno2].N;
		E0=JDarray[JDno2].E;
		N2=JDarray[JDno2+1].N;
		E2=JDarray[JDno2+1].E;
		GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,Lo1,Lo2,CenN,CenE);
		//acutPrintf("CenE:%lf,%lf\n",CenE,CenN);
		double TN,TE;//切点
		//求切点
		int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
		GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
		double dis=sqrt((JDarray[JDno2].N-JDarray[JDno].N)*(JDarray[JDno2].N-JDarray[JDno].N)+(JDarray[JDno2].E-JDarray[JDno].E)*(JDarray[JDno2].E-JDarray[JDno].E));
		INTERS(&JDarray[JDno2].N,&JDarray[JDno2].E,N2,E2,N0,E0,TN,TE,OutN,OutE);
		if (JDno==0)
		{
			double FWJ;
			FWJ=CalFWJ(JDarray[JDno2].N,JDarray[JDno2].E,OutN,OutE);
			JDarray[JDno].N=JDarray[JDno2].N+dis*sin(FWJ);
			JDarray[JDno].E=JDarray[JDno2].N+dis*cos(FWJ);
		}
		else
			INTERS(&JDarray[JDno].N,&JDarray[JDno].E,N1,E1,JDarray[JDno-1].N,JDarray[JDno-1].E,TN,TE,OutN,OutE);
	}
	return TRUE;
}

/*************************************************************
函数功能: 平移夹直线
输入参数:
onPt  		线路上一点
passPt		通过点
返回值：    线路直线边的前一交点下标；如果没有，返回-100
算法:由偏移距算出onPt附近两交点的新坐标,修改JDarray信息
编写：李帅 2016.7.9
***************************************************************/
bool RHA::MovJZX(ads_point onPt, ads_point passPt)
{ 
	xlpoint pz;
	Int16 LorR;
	int iJD1, iJD2;
	double N0, N1, E0, E1;

	iJD1=GetPreJD(onPt[1],onPt[0]);
	if (iJD1<0)
		return false;
	iJD2=iJD1+1;

	//得到前后交点所在绕行段编号
	int iRxd1=-100;
	int iRxd2=-100;
	CString mes;

	//判断前一交点是否在绕行段内
	if(JDarray[iJD1].JDXZ == RX_Start 
		|| JDarray[iJD1].JDXZ == RX_One
		|| JDarray[iJD1].JDXZ == RX_End)
	{
		iRxd1=GetiRxdByProjml((QXB[iJD1].KZH+QXB[iJD1].KHZ)/2.0);
		if(iRxd1<0)
		{
			mes.Format(L"错误：绕行交点%d附近没有找到绕行断链\n", JDarray[iJD1].JDName);
			ads_printf(mes);
			return false;
		}
	}
	
	N0=JDarray[iJD1].N;
	E0=JDarray[iJD1].E;
	N1=JDarray[iJD2].N;
	E1=JDarray[iJD2].E;

	pz.x=passPt[Y];pz.y=passPt[X];
	LorR=-Judge2XLorR(pz, array, tarray);

	
	//修改线间距
	if(JDarray[iJD1].JDXZ == BXPX || JDarray[iJD1].JDXZ == RX_End
		|| (JDarray[iJD1].JDXZ == RX_Start && JDarray[iJD1].JDXZ != RX_One 
		&& JDarray[iJD1].JDXZ != RX_End ))
	{
		double dist=DistanceFromPttoLine(passPt[Y], passPt[X], N0, E0, N1, E1);
		JDarray[iJD1].XJJ += dist;
	}


	//由通过点计算偏移后的直线起终点坐标
	double fwj=0;
	xyddaa(N0, E0, N1, E1, &fwj);
	N0=passPt[Y];
	E0=passPt[X];
	N1=N0+1000*cos(fwj);
	E1=E0+1000*sin(fwj);
	

	//计算前一交点坐标
	if (iJD1 == 0)
	{
		JDarray[iJD1].N=N0;JDarray[iJD1].E=E0;
	}
	else
	{
		INTERS(&JDarray[iJD1].N, &JDarray[iJD1].E, N0, E0, N1, E1
			,JDarray[iJD1-1].N, JDarray[iJD1-1].E, JDarray[iJD1].N, JDarray[iJD1].E);

	}

	//计算后一交点坐标
	if(iJD2+1 == JDarray.size())
	{
		JDarray[iJD2].N=N1;JDarray[iJD2].E=E1;
	}
	else
	{
		INTERS(&JDarray[iJD2].N, &JDarray[iJD2].E, N0, E0, N1, E1
			, JDarray[iJD2].N, JDarray[iJD2].E, JDarray[iJD2+1].N, JDarray[iJD2+1].E);
	}

	//更新绕行段里程
	if(iRxd1>-1)
		UpdateSglRxdLC(iRxd1);
	if (iRxd2>-1 && iRxd2 != iRxd1)
		UpdateSglRxdLC(iRxd2);

	//计算断链投影里程
	CalDLprojml(0);


	return true;
}

/*************************************************************
函数功能:   平移夹直线后，计算前后两交点坐标
输入参数:
onPt  		拾取线路直线上一点
offset		偏移距离，左负右正
算法:由偏移距算出onPt附近两交点的新坐标,修改JDarray信息
返回值：    
编写：李帅 2016.7.9
***************************************************************/
bool RHA::MovJZX(ads_point onPt, double offset)
{ 
	xlpoint pz;
	int iJD1, iJD2;
	double N0, N1, E0, E1;

	iJD1=GetPreJD(onPt[1],onPt[0]);
	if (iJD1<0)
		return false;
	iJD2=iJD1+1;

	//得到前后交点所在绕行段编号
	int iRxd1=-100;
	int iRxd2=-100;
	CString mes;

	//判断前一交点是否在绕行段内
	if(JDarray[iJD1].JDXZ == RX_Start 
		|| JDarray[iJD1].JDXZ == RX_One
		|| JDarray[iJD1].JDXZ == RX_End)
	{
		iRxd1=GetiRxdByProjml((QXB[iJD1].KZH+QXB[iJD1].KHZ)/2.0);
		if(iRxd1<0)
		{
			mes.Format(L"错误：绕行交点%d附近没有找到绕行断链\n", JDarray[iJD1].JDName);
			ads_printf(mes);
			return false;
		}
	}

	//判断后一交点是否在绕行段内
	if(JDarray[iJD2].JDXZ == RX_Start 
		|| JDarray[iJD2].JDXZ == RX_One
		|| JDarray[iJD2].JDXZ == RX_End)
	{
		iRxd2=GetiRxdByProjml((QXB[iJD2].KZH+QXB[iJD2].KHZ)/2.0);
		if(iRxd2<0)
		{
			mes.Format(L"错误：绕行交点%d附近没有找到绕行断链\n", JDarray[iJD2].JDName);
			ads_printf(mes);
			return false;
		}
	}


	
	N0=JDarray[iJD1].N;
	E0=JDarray[iJD1].E;
	N1=JDarray[iJD2].N;
	E1=JDarray[iJD2].E;
	
	//修改线间距
	if(JDarray[iJD1].JDXZ == BXPX || JDarray[iJD1].JDXZ == RX_End
		|| (JDarray[iJD1].JDXZ == RX_Start && JDarray[iJD1].JDXZ != RX_One 
		&& JDarray[iJD1].JDXZ != RX_End ))
		JDarray[iJD1].XJJ += offset;

	//由偏移距计算偏移后的直线起终点坐标 N0, E0, N1, E1
	CalLinePtByOffset(offset, N0, E0, N1, E1);

	//计算前一交点坐标
	if (iJD1 == 0)
	{
		JDarray[iJD1].N=N0;JDarray[iJD1].E=E0;
	}
	else
	{
		INTERS(&JDarray[iJD1].N, &JDarray[iJD1].E, N0, E0, N1, E1
			,JDarray[iJD1-1].N, JDarray[iJD1-1].E, JDarray[iJD1].N, JDarray[iJD1].E);

	}

	//计算后一交点坐标
	if(iJD2+1 == JDarray.size())
	{
		JDarray[iJD2].N=N1;JDarray[iJD2].E=E1;
	}
	else
	{
		INTERS(&JDarray[iJD2].N, &JDarray[iJD2].E, N0, E0, N1, E1
			, JDarray[iJD2].N, JDarray[iJD2].E, JDarray[iJD2+1].N, JDarray[iJD2+1].E);
	}

	//更新绕行段里程
	if(iRxd1>-1)
		UpdateSglRxdLC(iRxd1);
	if (iRxd2>-1 && iRxd2 != iRxd1)
		UpdateSglRxdLC(iRxd2);

	//计算断链投影里程
	CalDLprojml(0);

	return true;
}

/*************************************************************
函数功能: 旋转夹直线
输入参数:
onPt        拾取线路点
basPt  		旋转基点
angl		旋转角度，单位为弧度，>0 逆时针，<0 顺时针 
算法:由偏移距算出onPt附近两交点的新坐标,修改JDarray信息
编写：李帅 2016.7.9
***************************************************************/
bool RHA::RotJZX(ads_point onPt, ads_point basPt, double angl)
{ 
	int iJD1, iJD2;
	double N0, N1, E0, E1;
	double N0Cpy, N1Cpy, E0Cpy, E1Cpy;

	iJD1=GetPreJD(onPt[1],onPt[0]);
	if (iJD1<0)
		return false;

	iJD2=iJD1+1;
	N0=JDarray[iJD1].N;E0=JDarray[iJD1].E;
	N1=JDarray[iJD2].N;E1=JDarray[iJD2].E;

	N0Cpy=N0;N1Cpy=N1;E0Cpy=E0;E1Cpy=E1;

	//1 选转交点
	E0=(E0Cpy-basPt[X])*cos(angl)-(N0Cpy-basPt[Y])*sin(angl)+basPt[X];
	N0=(N0Cpy-basPt[Y])*cos(angl)+(E0Cpy-basPt[X])*sin(angl)+basPt[Y];

	E1=(E1Cpy-basPt[X])*cos(angl)-(N1Cpy-basPt[Y])*sin(angl)+basPt[X];
	N1=(N1Cpy-basPt[Y])*cos(angl)+(E1Cpy-basPt[X])*sin(angl)+basPt[Y];
	
	//2 与前后直线求交点
	if (iJD1 == 0)
	{
		JDarray[iJD1].N=N0;JDarray[iJD1].E=E0;
	}
	else
	{
		INTERS(&JDarray[iJD1].N, &JDarray[iJD1].E, N0, E0, N1, E1
			,JDarray[iJD1-1].N, JDarray[iJD1-1].E, JDarray[iJD1].N, JDarray[iJD1].E);

	}

	if(iJD2+1 == JDarray.size())
	{
		JDarray[iJD2].N=N1;JDarray[iJD2].E=E1;
	}
	else
	{
		INTERS(&JDarray[iJD2].N, &JDarray[iJD2].E, N0, E0, N1, E1
			, JDarray[iJD2].N, JDarray[iJD2].E, JDarray[iJD2+1].N, JDarray[iJD2+1].E);
	}

}



/*************************************************************
函数功能: 生成间距为step的从现场里程sCKml到eCKml的桩号序列
输入参数:
sCml        起点统一里程
eCml  		终点统一里程
step		步长, 间距
输出参数:   lcAry       里程表数组
算法:
编写：2016.7.11(ls 修改)
***************************************************************/
void  RHA::FormLCB(double sCml, double eCml, double step, CArray<LCB, LCB> &lcAry)  
{ 
	if (step < 0.00001)//2008-01-15
		return;

	double dlELC;//断后里程
	double sdlCml, edlCml;//前后断链处的统一里程
	double  xclc;//标注的现场里程
	double  cml;//xclc对应的统一里程
	int dlNum=DLarray.size();
	LCB tmpLCB;

	_tcscpy(tmpLCB.CKml, L"");
	lcAry.RemoveAll();

	//1 第一个断链
	sdlCml=0.0;
	if(sdlCml>sCml-0.00001 && sdlCml<eCml+0.00001)
	{
		tmpLCB.Cml=sdlCml;
		lcAry.Add(tmpLCB);
	}
	//2 逐个断链区间添加标注里程为整数倍step的桩号的统一里程
	for (int iDL=0;iDL<dlNum-1;iDL++)
	{ 
		dlELC = DLarray[iDL].ELC;  //本段断链区间起点里程 
		edlCml=sdlCml+DLarray[iDL+1].BLC-dlELC;//断链处统一里程

		if(edlCml<sCml-0.00001)
		{
			sdlCml=edlCml;
			continue;
		}

		if (sdlCml>eCml+0.00001)
			break;

		if (iDL>0 && iDL< dlNum-1)  // 不为起 终点
		{ 
			if(sdlCml>sCml-0.00001 && sdlCml<eCml+0.00001)
			{
				tmpLCB.Cml=sdlCml;			 
				tmpLCB.Flag=-1;//断链
				lcAry.Add(tmpLCB);
			}
		};

		xclc=((long int)(dlELC/step))*step;//"取整" 后的实际里程
		if ((xclc-dlELC)<0.0001)  
			xclc=xclc+step;  

		while(xclc+0.0001 < DLarray[iDL+1].BLC)
		{
			cml=sdlCml+xclc-dlELC; // 统一里程
			if(cml>sCml-0.001 && cml<eCml+0.001)
			{
				tmpLCB.Cml=cml;
				tmpLCB.Flag=0;
				lcAry.Add(tmpLCB);
			}
			xclc=xclc+step;		 
		} 

		sdlCml=edlCml;
	} 

	//添加最后一个桩
	if (tmpLCB.Cml+0.001<eCml)
	{
		tmpLCB.Cml=eCml;
		tmpLCB.Flag=0;
		lcAry.Add(tmpLCB);
	}

	//3 统一里程转现场里程和投影里程
	for(int kk=0; kk<lcAry.GetSize(); kk++)
	{
		TrsCmltoCkml(lcAry[kk].Cml, lcAry[kk].CKml);
		TrsCkmltoProjml(lcAry[kk].CKml, lcAry[kk].Projml);
	}
}

//将Scml到Ecml范围内的曲线特征点加入桩号序列LCBArray
//李伟加
void  RHA::AddTZDtoLCB(double Scml, double Ecml, CArray<LCB, LCB> &LCBArray)
{
	LCB toolLCB;
	int NUM=0;
	ACHAR ckml[50], GH[20];
	double Sprojml,Eprojml,dml;
	Sprojml = TrsCmltoProjml(Scml);
	Eprojml = TrsCmltoProjml(Ecml);
	for(int i=1; i<JDarray.size()-1; i++)//对点数进行循环
	{
		for(int k=0;k<4;k++)//ZH，HY，YH，HZ
		{
			if(k==0)
				toolLCB.Projml = QXB[i].KZH;
			else if(k==1)
				toolLCB.Projml = QXB[i].KHY;
			else if(k==2)
				toolLCB.Projml = QXB[i].KYH;
			else if(k==3)
				toolLCB.Projml = QXB[i].KHZ;


			if (toolLCB.Projml > Sprojml-0.001 && toolLCB.Projml < Eprojml+0.001)
			{
				TrsProjmltoCkml(toolLCB.Projml,toolLCB.CKml, 3);
				TrsCkmltoCml(toolLCB.CKml, toolLCB.Cml);
				toolLCB.Flag=k+5;
				int j=0;
				for (; j<LCBArray.GetSize(); j++)//查找第一个大于该里程的桩号，插入
				{
					if (toolLCB.Cml < LCBArray[j].Cml-0.01)
					{
						LCBArray.InsertAt(j, toolLCB);
						break;
					}
				}

				if (j== LCBArray.GetSize())//没找到，直接加在最后
					LCBArray.Add(toolLCB);
			}
		}
	}
}

//将桥隧站加入里程表

void  RHA::AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT, CArray<LCB, LCB> &pLCB) //将桥隧站加入里程表 
{

	LCB OneLCB;
	int Num = 0;
	for (int i=0; i<NBTS; i++)
	{
		int j=0;
		if (BTS[i][0] > pLCB[0].Cml)
		{
			Num = pLCB.GetSize();
			for (; j<Num; j++)
			{
				if (BTS[i][0] < pLCB[j].Cml)//找到第一个大于该里程的桩号
				{
					OneLCB.Cml = BTS[i][0];
					TrsCmltoCkml(OneLCB.Cml, OneLCB.CKml);
					if(BorT=='B')//桥
						OneLCB.Flag = -3;
					else if(BorT=='T')//隧
						OneLCB.Flag = -4;
					else if(BorT=='S')//站
						OneLCB.Flag =-9;
					else//绕行段的起终里程
						OneLCB.Flag =-10;
					pLCB.InsertAt(j, OneLCB);
					break;
				}
			}
		}

		Num = pLCB.GetSize();
		if (BTS[i][1] < pLCB[Num-1].Cml)
		{
			for (; j<Num; j++)
			{
				if (BTS[i][1] < pLCB[j].Cml)//找到第一个大于该里程的桩号
				{
					OneLCB.Cml = BTS[i][1];
					TrsCmltoCkml(OneLCB.Cml, OneLCB.CKml);
					if(BorT=='B')//桥
						OneLCB.Flag = 3;
					else if(BorT=='T')//隧
						OneLCB.Flag = 4;
					else if(BorT=='S')//站
						OneLCB.Flag =9;
					else//绕行段的起终里程
						OneLCB.Flag =10;
					pLCB.InsertAt(j, OneLCB);
					break;
				}
			}
		}
	}
}


void RHA::DeleteiRXDDLB(int iRXD)//删除某个绕行段断链表
{
	int sDLno,eDLno;
	GetiRXDDLinfo(iRXD,sDLno,eDLno,DLarray);
	if(sDLno<0)
		return;
	DLarray.erase(DLarray.begin()+sDLno, DLarray.begin()+eDLno+1);

	//改绕行段编号
	for(int i=sDLno;i<DLarray.size();i++)
	{
		if (DLarray[i].Indx >= 1)// 818 下标号会混乱吗？
			DLarray[i].Indx--;
	}
}

void RHA::DeleteOneRXD(int iRXD)
//把指定绕行段改并行，并删除对应断链
{
	if(iRXD<0)
		return;

	//	assertWriteEnabled();
	//先删除对应绕行段的交点，并插入相应的并行交点
	int FirstJD,EndJD;//某一绕行段起始交点号
	int PreJD,NextJD;//某一绕行段前后交点号
	int PreJD1x,NextJD1x;//某一绕行段前后交点对应的一线交点序号
	int DelJD,AddJD;//要删除的交点，要增加的交点
	int dJD;//后面交点向前移动位数
	int SDL,EDL;
	double RXDSml1x,RXDEml1x;
	xlpoint PZ;

	GetiRXDDLinfo(iRXD,SDL,EDL,DLarray);

	if (_tcscmp(DLarray[SDL].BLName, DLarray[EDL].BLName) != 0)
	{
		ads_printf(L"前后参照线不同，不能删除绕行段！\n");return;
	}

	
	//得到参照线
	RHA *pRHA_BL;
	pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[SDL].BLName, false);
	
	PZ.lc = DLarray[SDL].TYLC;
	xlpoint_pz(array,tarray,PZ);

	RXDSml1x =pRHA_BL->PROJ_ML(pRHA_BL->array,pRHA_BL->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
	PreJD1x = pRHA_BL->DistToNearJD(RXDSml1x,true);
	PZ.lc = DLarray[EDL].TYLC;
	xlpoint_pz(array,tarray,PZ);
	RXDEml1x = pRHA_BL->PROJ_ML(pRHA_BL->array,pRHA_BL->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
	NextJD1x = pRHA_BL->DistToNearJD(RXDEml1x,false);

	//	PreJD1x=JDarray[PreJD].jdno1;
	//	NextJD1x=JDarray[NextJD].jdno1;

	GetRxdSEJD(iRXD,FirstJD,EndJD,JDarray);//获取iRXD的首尾交点序号

	double EndXjj;//绕行段最后一个交点的线间距
	EndXjj=JDarray[EndJD].XJJ;
	PreJD=FirstJD-1;
	NextJD=EndJD+1;

	DelJD=NextJD-PreJD-1;
	AddJD=NextJD1x-PreJD1x-1;
	dJD=DelJD-AddJD;

	//for(int i=FirstJD;i<NJD-DelJD;i++)//删除RXD交点，向前移位
	//{
	//	JDarray[i]=JDarray[i+DelJD];
	//}
	//NJD-=DelJD;
	////删除正确，要检查添加
	//NJD+=AddJD;
	//for(int i=NJD-1;i>=FirstJD+AddJD;i--)//增加并行段交点存储位置，向后移位
	//{
	//	JDarray[i]=JDarray[i-AddJD];
	//}
	JDarrayStruct tmpJD;
	_tcscpy(tmpJD.JDName, L"");
	_tcscpy(tmpJD.ZHLC, L"");
	_tcscpy(tmpJD.Draw_JDName, L"");

	_tcscpy(tmpJD.XLName, m_XLName);
	_tcscpy(tmpJD.BLName, DLarray[SDL].BLName);

	for(int i=0;i<DelJD;i++)//删除RXD交点，向前移位
		JDarray.erase(JDarray.begin()+FirstJD);

	for(int i=0;i<AddJD;i++)//删除正确，要检查添加
		JDarray.insert(JDarray.begin()+FirstJD, tmpJD);


	double lo1,lo,D;
	for(int i=FirstJD;i<FirstJD+AddJD; i++)
	{
		//		if(JDarray[PreJD].JDXZ==B_X )JDarray[i].jdno1=(PreJD1x+1)+(i-FirstJD);
		//		else JDarray[i].jdno1=(PreJD1x)+(FirstJD-i);
		_tcscpy(JDarray[i].BLJDName
			    , pRHA_BL->JDarray[(PreJD1x+1)+(i-FirstJD)].JDName);
		JDarray[i].JDXZ=BXPX;//并行段交点

		//	_stprintf(JDarray[i].JDName,L"%d",JD0 + i);
		//   JDarray[i][0] = JD0 + i;
		int jdn1 =i-FirstJD +1 + PreJD1x;//对应一线交点序号
		JDarray[i].XJJ=JDarray[i-1].XJJ;
		if(i==FirstJD+AddJD-1 && i+1<JDarray.size())
			JDarray[i].XJJ = EndXjj;//最后一个点的线间距等于前一交点的
		double XJJ = JDarray[i].XJJ;
		
		int blJDix=-1;
		GetJdno1FromJdstr(JDarray[i].BLJDName, pRHA_BL->JDarray, blJDix);
		CalExJDByParallel(pRHA_BL, pRHA_BL, blJDix-1, blJDix,  JDarray[i-1].XJJ, JDarray[i].XJJ, &JDarray[i]);
		
		//double EXJD[2];
		//CalEXJD(jdn1,i,EXJD);
		//JDarray[i].x = EXJD[0];
		//JDarray[i].y = EXJD[1];//计算出二线交点
		double R = pRHA_BL->JDarray[jdn1].R;
		double JK;
		bool isJYGuiFanCS=false;

		if (XLShuXing == JYJX || XLShuXing == JY2X)
			isJYGuiFanCS=true;
		
		JK = get_XJJK(pRHA_BL->DV,R,isJYGuiFanCS);//线间距加宽
		JDarray[i].QXJK = JK;
		JDarray[i].l1 = pRHA_BL->JDarray[jdn1].l1;
		JDarray[i].R = R;
		JDarray[i].l2 = pRHA_BL->JDarray[jdn1].l2;           
		lo1 = pRHA_BL->JDarray[jdn1].l1;//一线缓长
		D = fabs(XJJ);
		//ads_printf(L"21:%d %d %d R=%lf lo=%lf D=%lf\n",i,jdn1,JDarray[i].jdno1,JDarray[i].R,lo1,D);
		//二线半径
		if(i>0 && i+1<JDarray.size())
		{
			double xjj2;
			xjj2 = JDarray[i-1].XJJ;//后一条边线间距
			//   ads_printf(L"i=%d XJJ=%lf xjj2=%lf\n",i,XJJ,xjj2);
			if(fabs(XJJ-xjj2)<0.01 && fabs(XJJ)>0.01)//前后线间距相等
			{
				//XJJ考虑曲线加宽............		   
				if(XJJ>0)//一线为左线
				{
					if(pRHA_BL->QXB[jdn1].LorR<0)//左转,II线在外侧，减少缓长
					{
						//先配缓长
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//彭先宝 2003.12.4提供
						JDarray[i].l1 = JDarray[i].l2 =  lo;	
						D  = -1.0*D;
					}
					else
					{
						//先配缓长
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//彭 2003.12.4提供
						JDarray[i].l1 = JDarray[i].l2 =  lo;						  					   
					}
					//					   ads_printf(L"21:%d lo=%lf \n",i,lo);
				}
				else//一线为右线
				{
					if(pRHA_BL->QXB[jdn1].LorR<0)//左转,II线在内侧
					{
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//彭 2003.12.4提供
						JDarray[i].l1 = JDarray[i].l2 =  lo;						  					   
					}
					else
					{   //先配缓长
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//彭 2003.12.4提供
						JDarray[i].l1 = JDarray[i].l2 =  lo;	
						D  = -1.0*D;
					}
				}
				//配半径
				double B = R - D + (lo1*lo1)/24/R;
				//			ads_printf(L"lo1=%lf lo=%lf D=%lf JK=%lf B=%lf\n",lo1,lo,D,JK,B);
				JDarray[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//二线半径				
			}
			//	   ads_printf(L"2:%d %lf %lf \n",i,JDarray[i].R,JDarray[i].l1);
		}   
		//JDarray[i].jdno = JD0 + i;
		int jdno = JD0 + i;
		_stprintf(JDarray[i].JDName,L"%s%d",L"JD",jdno);//818 交点号有问题
	}
	//////////////////////////////////////////////////////////////////////////

	DeleteiRXDDLB(iRXD);
}

void RHA::CalT1T2L()
{
	double T1,T2,L;
	double RB,RE,R,L1,L2,ZJ;

	for(int i=1;i+1<QXB.size();i++)
	{
		//不相等
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)!=0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)!=0))	
		{//单交点
			RB = 1e50;
			RE = 1e50;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//相等
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)==0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)==0))	
		{
			//复曲线
			RB = QXB[i-1].R;
			RE = QXB[i+1].R;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//一测相等
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)==0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)!=0))	
		{
			//复曲线
			RB = QXB[i-1].R;
			RE = 1e50;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//一测相等
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)!=0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)==0))	
		{
			//复曲线
			RB = 1e50;
			RE = QXB[i+1].R;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}
		get_T_Len(ZJ,R,RB,L1,RE,L2,T1,T2,L);
		//	ads_printf(L"****  JDNum=%s,T1=%lf,T2=%lf,L=%lf\n",QXB[i].JDName,T1,T2,L);
		QXB[i].T1 = T1;
		QXB[i].T2 = T2;
		QXB[i].L  = L;
		/*QXB[i].T1_Note = QXB[i].T1;
		QXB[i].T2_Note = QXB[i].T2;
		QXB[i].L_Note = QXB[i].L;*/
	}
}

//投影里程转统一里程
double  RHA::TrsProjmltoCml(double projml) const 
{
	double cml;

	if (XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		ACHAR ckml[MaxCkmlStrNum];

		TrsProjmltoCkml(projml, ckml, 3);
		TrsCkmltoCml(ckml, cml);
	}
	else//考虑标志里程
		cml=CalnewcmlTocml(projml);

	return cml;
}

//统一里程转投影里程
double  RHA::TrsCmltoProjml(double cml) const 
{
	double projml;

	if (XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		ACHAR ckml[MaxCkmlStrNum];

		//先计算现场里程、再计算几何里程
		TrsCmltoCkml(cml, ckml);
		TrsCkmltoProjml(ckml, projml);
	}
	else
	{
		//考虑标志里程
		projml=CalcmlTonewcml(cml);////818 这样写可以吗
	}
	
	return projml;
}

//统一里程转现场里程
void  RHA::TrsCmltoCkml(double cml, ACHAR ckml[], int nLC) const 
{
	ACHAR gh[16];
	double dml;

	dml=XLC1(cml, gh, DLarray);
	_tcscpy(ckml, LCchr(gh, dml, nLC));
}

//统一里程转现场里程
void  RHA::TrsCmltoCkml(double cml, ACHAR gh[], double &xlc) const 
{
	xlc=XLC1(cml, gh, DLarray);
}

//投影里程projml转统一里程cml
//备注：一线和二线函数都调用此函数
//编写：李帅 2016.7.19
//说明：该函数只在全线绕行的线路或者二线绕行段上用
double  RHA::CalnewcmlTocml(double projml) const 
{
	if ( XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		AfxMessageBox(L"设计线不应调用此函数!");//nan
	}

	if (BZLCarray.size()<1 || XLShuXing == SJJX || XLShuXing == SJ2X)
		return projml;

	double cml=projml;
	ACHAR ckml[50];

	
	for(int i=1;i<BZLCarray.size();i++)
	{
		//ads_printf(L"\n77tt%f",BZLCarray[i].K);
		_tcscpy(ckml,BZLCarray[i].CKml);
		if (fabs(projml-BZLCarray[i].xyToprojml)<0.01)
		{
			return BZLCarray[i].kmlTocml;
		}
		if (fabs(projml-BZLCarray[i-1].xyToprojml)<0.01)
		{
			return BZLCarray[i-1].kmlTocml;
		}
		if(projml>BZLCarray[i-1].xyToprojml-0.00001 && projml<BZLCarray[i].xyToprojml+0.00001)
		{
			if(fabs(BZLCarray[i].K)<1.0e-6)
			{
				return BZLCarray[i-1].kmlTocml;
			}
			else 
			{
				cml =( projml - BZLCarray[i-1].xyToprojml )/( BZLCarray[i].K ) +  BZLCarray[i-1].kmlTocml ;
				return cml;
			}
		}	
	}

	return cml;
}

/*
函数作用：统一里程cml转投影里程projml
//备注：1 既有线、设计线都调用此函数。
        2 设计线中，cml和projml相等。调用此函数时，直接返回kmlTocml
		3 该函数只在全线绕行的线路或者二线绕行段上用 */
double  RHA::CalcmlTonewcml(double kmlTocml) const
{
	if ( XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		AfxMessageBox(L"设计线不应调用此函数!");//nan
	}

	if(BZLCarray.size()<1 || XLShuXing == SJJX || XLShuXing == SJ2X)
		return kmlTocml;

	double newcml = kmlTocml;
	for(int i=1;i<BZLCarray.size();i++)
	{
		if (fabs(kmlTocml-BZLCarray[i].kmlTocml)<0.1)
		{
			return BZLCarray[i].xyToprojml;
		}
		if(kmlTocml>BZLCarray[i-1].kmlTocml-0.00001 && kmlTocml<BZLCarray[i].kmlTocml+0.00001)
		{
			newcml = BZLCarray[i-1].xyToprojml + BZLCarray[i].K * (kmlTocml - BZLCarray[i-1].kmlTocml);
			return newcml;
		}
	}

	return newcml;
}

//绘制标志里程  
//zybz=1,-1  标在右边，左边
Adesk::Boolean  RHA::MarkBiaoZhiLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  //818 type 含义？
{//
	//if (!m_IsNoteML)//818 注意平面基本参数对话框
	//	return FALSE;

	int k;
	AcGePoint3d Pt;
	AcGePoint3d  pte,ptb;
	double B, b;//角度
	double len;
	xlpoint  PZtmp; 
	ACHAR str[256];
	AcGePoint3d BZPt;

	for(int i=0;i<BZLCarray.size();i++)  //NBZLC
	{
		if (BZLCarray[i].isAutoAdd)
			continue;

		PZtmp.lc=BZLCarray[i].xyToprojml;  // 统一里程	
		k=xlpoint_pz(array,tarray,PZtmp);   

		B=PZtmp.a;
		//点起点
		Pt[Y]=PZtmp.x; 
		Pt[X]=PZtmp.y;
		Pt[Z]=0.0;
		_tcscpy(str,BZLCarray[i].CKml);
		_tcscat(str,L" 标志里程");

		//点终点
		len=_tcslen(str)*1.4*texth;//原来是1.2*1.4*texth
		pte[X]=PZtmp.y+len*sin(B+pi*0.5*zybz);
		pte[Y]=PZtmp.x+len*cos(B+pi*0.5*zybz);
		G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);

		//旋转角度
		if(zybz>0)
		{
			b =pi/2.0-( B +pi*0.5*zybz) + pi ;
			if (b<0.0) b=2*pi+b;		 
			if (b>2*pi) b=b-2*pi;
		}
		else 
		{
			b =pi/2.0-( B +pi*0.5*zybz) ;
			if (b<0.0) b=2*pi+b;		 
			if (b>2*pi) b=b-2*pi;
		}

		if (zybz>0)
		{
			ptb[X]=pte[X]+(1.75*texth)*sin(B);
			ptb[Y]=pte[Y]+(1.75*texth)*cos(B);
			G_maketext(pWd,ptb,str,b,1.5*texth,QXYSColor,1);
		}
		else 
		{
			ptb[X]=PZtmp.y+(1.5*texth)*sin(B+pi*0.5*zybz);
			ptb[Y]=PZtmp.x+(1.5*texth)*cos(B+pi*0.5*zybz);

			ptb[Y] = ptb[Y] + 1.75*texth*cos(B);
			ptb[X] = ptb[X] + 1.75*texth*sin(B);
			G_maketext(pWd,ptb,str,b,1.5*texth,QXYSColor,1);
		}
	}

	return TRUE;
}

/*===================================================================
函数名:GenBZLCbyDL
------------------------------------------------------------
函数功能:由断链自动生成标志里程数组，无用户设置的标志里程
------------------------------------------------------------
输入:isCreatNew       是否重新生成，默认为true
------------------------------------------------------------
输出:BZLCarray        标志里程数组（类变量）
------------------------------------------------------------
编写: 李帅 2016.7.19  
------------------------------------------------------------
备注：调用该函数前，应先生成断链
=====================================================================*/
void RHA::GenBZLCbyDL(bool isCreatNew)
{
	if (isCreatNew)
		BZLCarray.clear();

	if (XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	BZLCStructTable tmpBZLC;
	xlpoint PZ;

	_tcscpy(tmpBZLC.XLName, m_XLName);
	tmpBZLC.isAutoAdd=true;
	tmpBZLC.K=1.0;

	for(int i=0;i<DLarray.size();i++) 
	{
		tmpBZLC.xyToprojml=DLarray[i].TYLC;
		PZ.lc=DLarray[i].TYLC;
		xlpoint_pz(array,tarray, PZ);
		tmpBZLC.X=PZ.x;
		tmpBZLC.Y=PZ.y;
		_tcscpy(tmpBZLC.CKml, LCchr(DLarray[i].BGH, DLarray[i].BLC, 6)); 
		tmpBZLC.kmlTocml=TYLC1(tmpBZLC.CKml, DLarray);

		if(DLarray[i].Indx >= 0)
			_tcscpy(tmpBZLC.GJDNum, DLarray[i].RXDName);
		else
			_tcscpy(tmpBZLC.GJDNum, L"");

		BZLCarray.push_back(tmpBZLC);
	}

	////nan
	//vector<BZLCStructTable> tmp;
	//tmp.clear();
	//for (int i=0;i<BZLCarray.GetSize();i++)
	//	tmp.push_back(BZLCarray[i]);

}

//计算标志里程信息，并排序
void RHA::CalBZLC()
{

	if (BZLCarray.size()<1 || XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	//1 计算连续里程和投影里程
	for(int i=0;i<BZLCarray.size();i++)
	{
		//由坐标x,y -> 的投影里程
		BZLCarray[i].xyToprojml = PROJ_ML(array,tarray,BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X,BZLCarray[i].Y);
		//现场里程 -> 的连续里程
		BZLCarray[i].kmlTocml = TYLC1(BZLCarray[i].CKml, DLarray);
		//TrsCkmltoCml(BZLCarray[i].CKml, BZLCarray[i].kmlTocml);//【统一里程只和断链有关】
	}

	//2 排序
	SortBZLCArray();

	//3 求缩放系数K
	double a=0.0,b=0.0;
	BZLCarray[0].K = 0.0; 
	//ads_printf(L"\npatt%lf",BZLCarray[2].kmlTocml);
	//ads_printf(L"\npatt%lf",BZLCarray[1].kmlTocml);
	//ads_printf(L"\npatt%s",BZLCarray[2].CKml);
	//ads_printf(L"\npatt%s",BZLCarray[1].CKml);
	//ads_printf(L"\npatt%d",BZLCarray.GetSize());
	for(int i=1;i<BZLCarray.size();i++)
	{
		a = BZLCarray[i].kmlTocml - BZLCarray[i-1].kmlTocml;
		b = BZLCarray[i].xyToprojml - BZLCarray[i-1].xyToprojml;
		if(fabs(a)>1.0e-6)
			BZLCarray[i].K = b/a;
		else 
			BZLCarray[i].K = 0.0;
		// 		if (BZLCarray[i].K<0)
		// 		{
		// 			ads_printf(L"att%d %s",i,BZLCarray[i].CKml);
		// 		}
	}

	//nan
	/*vector<BZLCStructTable> tmp;
	CString str;
	tmp.clear();
	for (int i=0;i<BZLCarray.GetSize();i++)
	{
		str.Format(L"\n标志%s %s", m_XLName, BZLCarray[i].kml);
		ads_printf(str);
		str.Format(L" , K-%lf", BZLCarray[i].K);
		ads_printf(str);
		tmp.push_back(BZLCarray[i]);
	}*/
}


//功能：标志里程按投影里程排序
void RHA::SortBZLCArray()
{
	for (int i=0 ; i<BZLCarray.size() ; i++)
		BZLCarray[i].xyToprojml = PROJ_ML(array,tarray,BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X,BZLCarray[i].Y);

	BZLCStructTable BZLCTemp ;
	for (int i=0 ; i+1<BZLCarray.size() ; i++)
	{
		for (int j=i+1; j<BZLCarray.size() ; j++)
		{
			if ( BZLCarray[i].xyToprojml > BZLCarray[j].xyToprojml )
			{
				BZLCTemp = BZLCarray[i];
				BZLCarray[i] = BZLCarray[j];
				BZLCarray[j] = BZLCTemp ;
			}
		}
	}

	////nan
	//CString str;
	//for (int i=0;i<BZLCarray.GetSize();i++)
	//{
	//	str.Format(L"\n排序bzlc %s", BZLCarray[i].kml);
	//	ads_printf(str);
	//	str.Format(L" , bzlc %lf", BZLCarray[i].xyToprojml);
	//	ads_printf(str);
	//}


}

//改建段数组按投影里程排序
//编写：李帅 2016.12
//备注：需要先计算出改建段的起终投影里程
void RHA::SortGJDArray(vector<GJDInf>& gjdAry)
{
	for (int i=0 ; i+1<gjdAry.size() ; i++)
	{
		for (int j=i+1; j<gjdAry.size() ; j++)
		{
			if ( gjdAry[i].BProjml > gjdAry[j].BProjml )
			{
				swap(gjdAry[i], gjdAry[j]);
			}
		}
	}

}

void RHA::AddBZLC(ads_point PT ,ACHAR CurAddBZLC[50], CString iRxdXLName, bool isAuto)
{
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	assertWriteEnabled();
	BZLCStructTable tmpBZLC;
	xlpoint PZ;

	tmpBZLC.isAutoAdd=isAuto;
	_tcscpy(tmpBZLC.XLName, m_XLName);

	_tcscpy(tmpBZLC.GJDNum, iRxdXLName);
	PZ.lc=PROJ_ML(array, tarray, PT[Y], PT[X],  PT[Y], PT[X]);
	xlpoint_pz(array, tarray, PZ);
	tmpBZLC.X = PZ.x;
	tmpBZLC.Y = PZ.y;
	_tcscpy(tmpBZLC.CKml , CurAddBZLC);

	BZLCarray.push_back(tmpBZLC);
	
}

//  删除一个标志里程
//IsDelDL 是否删除断链的标志里程，默认false
void RHA::DeleteBZLC(ads_point PT, bool IsDelDL)
{ 
	if (XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	int delBZLCix = 0;
	double MinL = 99999999999;
	double dist = 0.0 ;

	for(int i=0; i< BZLCarray.size() ; i++)
	{
		dist = sqrt( pow( (PT[Y] - BZLCarray[i].X ) ,2.0) + pow( (PT[X] - BZLCarray[i].Y ) ,2.0)) ;
		if(MinL >  dist)
		{
			MinL = dist; 
			delBZLCix = i;
		}
	}

	//删除delBZLCix
	if( delBZLCix>0 && BZLCarray[delBZLCix].isAutoAdd == false)//删除用户标志里程
		//BZLCarray.RemoveAt(delBZLCix,1);
		BZLCarray.erase(BZLCarray.begin()+delBZLCix);
	else if( delBZLCix>0 && IsDelDL && dist<0.1)//删除断链标志里程
		//BZLCarray.RemoveAt(delBZLCix,1);
		BZLCarray.erase(BZLCarray.begin()+delBZLCix);

	
}

//根据绕行段编号得到名称
CString RHA::GetRxdName(int i)
{
	int FirstDL,EndDL;//某一绕行段起始交点号
	FirstDL=EndDL=-1;
	GetiRXDDLinfo(i, FirstDL, EndDL, DLarray);
	if (FirstDL>-1)
		return DLarray[FirstDL].RXDName;
	else
		return "";
}

//寻找某投影里程附近的标志里程下标，如果没找到返回-100 
//dirctn =true，向前找；=false，向后找
int  RHA::FindNearBZLC(double projml, bool dirctn) 
{
	//标志里程排序
	SortBZLCArray();

	int num=BZLCarray.size();
	if (num<2)
		return -100;
	
	if (projml+0.001<BZLCarray[0].xyToprojml 
		|| projml>BZLCarray[num-1].xyToprojml+0.001)
		return -100;

	int iBZLC=0;
	for(int i=1; i<num; i++)
	{
		if (projml<BZLCarray[i].xyToprojml+0.001)
		{
			iBZLC=i;
			break;
		}
	}

	if(dirctn && iBZLC>0)
		iBZLC--;

	return iBZLC;
}
 
/*===================================================================
函数名:AddDLM
------------------------------------------------------------
函数功能:增加断链，并自动添加标志里程
------------------------------------------------------------
输入:
------------------------------------------------------------
编写: 李帅 2016.7.23  
------------------------------------------------------------
备注：  1 要求BLC、BGH给的正确，与前一断链断后一致 818
		2 暂时只考虑PMSJ.CPP调用
=====================================================================*/
bool  RHA::AddDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH, double TYLC, Int16 iRxd)
{
	bool isFindUserBZLC=false;

	//检查插入断链和下一个断链之间是否有用户设置的标志里程
	//，如果有，需要检查新增断链断后的冠号和里程是否正确
	if (XLShuXing != SJJX && XLShuXing != SJ2X)
	{
		int iBZ=0;
		iBZ=FindNearBZLC(TYLC, false);
		if(iBZ>0 && BZLCarray[iBZ].isAutoAdd == false)
		{
			ACHAR gh[16];
			double dml=0.0;
			ACHAR mes[100];

			split_ckml(BZLCarray[iBZ].CKml, dml, gh);
			if(_tcscmp(EGH, gh) != 0)
			{
				_stprintf(mes, L"新增断链的断后冠号%s和后一标志里程%s的冠号%s不一致，增加断链失败\n"
					, EGH, BZLCarray[iBZ].CKml, gh);
				return false;
			}
			else if (ELC+0.001>dml)
			{
				_stprintf(mes, L"新增断链的断后里程%.3f大于后一标志里程%s的里程值%.3f不一致，增加断链失败\n"
					, ELC, BZLCarray[iBZ].CKml, dml);
				return false;
			}

			isFindUserBZLC=true;//后面有用户设置的标志里程
		}

	}


	//插入断链
	ACHAR rxdName[50];
	_tcscpy(rxdName, GetRxdName(iRxd));
	setDLM(iDL,BLC,ELC,BGH,EGH,TYLC,iRxd,rxdName);

	if(!isFindUserBZLC)
	{//后面没有用户设置的标志里程

		//修改后一断链的断前
		DLarray[iDL+1].BLC = DLarray[iDL+1].TYLC - DLarray[iDL].TYLC + DLarray[iDL].ELC;
		_tcscpy(DLarray[iDL+1].BGH, DLarray[iDL].EGH);
	}

	xlpoint pz;
	ACHAR ckml[50];
	ads_point pt;


	//添加本断链标志里程
	pz.lc=DLarray[iDL].TYLC;
	xlpoint_pz(array, tarray, pz);
	pt[X]=pz.y;pt[Y]=pz.x;pt[Z]=0;
	_tcscpy(ckml, LCchr(BGH, BLC, 10));

	AddBZLC(pt, ckml, rxdName, true);

	if(!isFindUserBZLC)
	{//后面没有用户设置的标志里程

		//因为后一断链的断前可能改变
		//，所以需要修改后一断链自动产生的标志里程的现场里程
		for(int i=0; i<BZLCarray.size(); i++)
		{
			if (fabs(BZLCarray[i].xyToprojml-DLarray[iDL+1].TYLC)<0.1)
			{
				_tcscpy(BZLCarray[i].CKml, LCchr(DLarray[iDL+1].BGH, DLarray[iDL+1].BLC, 6));
				break;
			}
		}

	}
	

	return true;
}
/*===================================================================
函数名:DelDLM
------------------------------------------------------------
函数功能:删除某统一里程附近的一个断链数据
------------------------------------------------------------
输入:tylc 统一里程
------------------------------------------------------------
编写: 李帅 2016.7.23  
------------------------------------------------------------
备注：
=====================================================================*/
bool  RHA::DelDLM(double tylc)
{ 
	assertWriteEnabled();

	int iDL=-1;//要删除的断链
	double dist=0,distmin=10000000;//与断链的距离
	int i;

	//搜索断链的位置
	for(i=0;i<DLarray.size();i++)
	{
		dist=fabs(tylc-DLarray[i].TYLC);
		if(dist<distmin)
		{
			distmin=dist;
			iDL=i;
		}
	}

	//如果是绕行起、止断链，不能删
	if (DLarray[iDL].Indx >= 0)
		
	{
		if ((iDL+1<DLarray.size() && DLarray[iDL].Indx != DLarray[iDL+1].Indx)
			||(iDL>0 && DLarray[iDL].Indx != DLarray[iDL-1].Indx))
		{
			ads_printf(L"\n绕行起、止断链，不能删！");return false;
		}
	}
	else
	{
		ads_printf(L"\n并行断链不能删！");return false;
	}

	//删除断链
	DLarray.erase(DLarray.begin()+iDL);
	
	//修改后一断链的断前
	DLarray[iDL].BLC=DLarray[iDL].TYLC-DLarray[iDL-1].TYLC
		+DLarray[iDL-1].ELC;
	_tcscpy(DLarray[iDL].BGH, DLarray[iDL-1].EGH);

	if (XLShuXing != SJJX && XLShuXing != SJ2X)
	{
		//更新标志里程,删除对应的标志里程
		xlpoint pz;
		pz.lc=DLarray[iDL].TYLC;
		xlpoint_pz(array, tarray, pz);
		ads_point pt;
		pt[X]=pz.y;pt[Y]=pz.x;pt[Z]=0;
		DeleteBZLC(pt, true);
	}

	return true;	
}


//平面线形夹直线、夹圆线、最小曲线半径检测
void RHA::checkpm()
{
	double ljia;
	CString ERR;
	int NERR=0;
	int SwitchErro=0;
	FILE *fpw;
	ACHAR fname[256];
	ACHAR ckml[50];
	double StartZhTylc,EndHzTylc;
	double x1,y1,x2,y2,sTYLC1x0,eTYLC1x0;
	int RXSJDIndex1x,RXEJDIndex1x;
	xlpoint sPt,ePt;
	bool IsParallel;
	AcGePoint2d Spt1x,Ept1x,Spt2x,Ept2x;
	AcGeLine2d Line2x,Line1x;
	AcGeTol Tol;
	int NJD=JDarray.size();

	ReadGuifanFile();//读规范
	if (m_XLName.Find(L"设计")>-1)
		mGuiFanCS=mGJGuiFanCS;
	else
		mGuiFanCS=mJYGuiFanCS;

	Tol.setEqualVector(0.001);
	ZXLmin1=mGuiFanCS.MinZhi;
	Rmin1=mGuiFanCS.MinR1;
	Rmin2=mGuiFanCS.MinR2;
	//FindZXLenMin();//找出最小夹直线长
	if(!ReadWorkdir())
	{
		ads_alert(L"请先指定正确的工作路径!");
		return ;
	}
	_stprintf(fname,L"%s\\data\\Err",Cworkdir);
	fpw = _wfopen(fname,L"w");
	if(fpw==NULL)
	{
		ads_printf(L"打开文件%s失败!\n",fname);
		return;
	}
	fwprintf(fpw,L"%10d\n",NERR);
	for(int i=2;i<NJD-1;i++)
	{
		ljia = QXB[i].KZH - QXB[i-1].KZH;//夹直线
		if(ljia<ZXLmin1)
		{
			//TrsNEtoCkml(JDarray[i].x,JDarray[i].y,ckml);
			ERR.Format(L"%s-%s夹直线长%0.2lf小于最小夹直线长(一般%0.2lf,困难%0.2lf))",JDarray[i-1].JDName,JDarray[i].JDName,ljia,ZXLmin1,ZXLmin2);
			fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KZH + QXB[i-1].KZH),ERR);
			NERR++;
		}
	}
	int blix=-1;
	for(int i=1;i<NJD-1;i++)
	{
		if (JDarray[i].JDXZ==RX_Start)
		{
			StartZhTylc = QXB[i].KZH;
			sPt.lc=StartZhTylc;
			xlpoint_pz(array,tarray,sPt);
			x1=sPt.x;
			y1=sPt.y;
			//首次投影到一线的统一里程
			//blix=CCreatPM::IsIncluded(m_iDoc, m_mdbName, JDarray[i-1].BLName);
			CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[i-1].BLName, blix);
			if (blix<0)
			{
				ads_printf(L"\nNOT FIND THE LINE");
				continue;
			}

			sTYLC1x0 = PROJ_ML(g_DocManageAry[m_iDoc].XLAry[blix]->array,g_DocManageAry[m_iDoc].XLAry[blix]->tarray,x1,y1,x1,y1);
			RXSJDIndex1x =g_DocManageAry[m_iDoc].XLAry[blix]->DistToNearJD(sTYLC1x0,false);
			Spt2x.x = JDarray[i].E;
			Spt2x.y = JDarray[i].N;
			Ept2x.x = JDarray[i-1].E;
			Ept2x.y = JDarray[i-1].N;
			Line2x.set(Spt2x,Ept2x);
			bool IsParallel = false;
			for(int iJD1x=RXSJDIndex1x; iJD1x>0; iJD1x--)
			{
				Spt1x.x = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x].E;
				Spt1x.y = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x].N;
				Ept1x.x = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x-1].E;
				Ept1x.y = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x-1].N;
				Line1x.set(Spt1x,Ept1x);
				IsParallel = Line1x.isParallelTo(Line2x,Tol);
				if (IsParallel)
				{
					break;
				}
			}
			if (!IsParallel)
			{
				ERR.Format(L"绕行起点%s-%s直线边不与基线上对应边平行",JDarray[i].JDName,JDarray[i-1].JDName);
				fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KZH + QXB[i-1].KZH),ERR);
				NERR++;
			}
		}
		if (JDarray[i].JDXZ==RX_End
			|| (JDarray[i].JDXZ == RX_Start && JDarray[i+1].JDXZ != RX_One && JDarray[i+1].JDXZ != RX_End))
		{
			EndHzTylc   = QXB[i].KHZ;//计算终点断链
			ePt.lc=EndHzTylc;
			xlpoint_pz(array,tarray,ePt);
			x2=ePt.x;
			y2=ePt.y;
			//-------------------------------------------------
			CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[i].BLName, blix);
			if (blix<0)
			{
				ads_printf(L"\nNOT FIND THE LINE"); 
				continue;
			}

			eTYLC1x0 = PROJ_ML(g_DocManageAry[m_iDoc].XLAry[blix]->array,g_DocManageAry[m_iDoc].XLAry[blix]->tarray,x2,y2,x2,y2);
			RXEJDIndex1x = g_DocManageAry[m_iDoc].XLAry[blix]->DistToNearJD(eTYLC1x0,true);
			Spt2x.x = JDarray[i].E;
			Spt2x.y = JDarray[i].N;
			Ept2x.x = JDarray[i+1].E;
			Ept2x.y = JDarray[i+1].N;  
			Line2x.set(Spt2x,Ept2x);   
			IsParallel = false;
			for (int iJD1x=RXEJDIndex1x; iJD1x+1<g_DocManageAry[m_iDoc].XLAry[blix]->JDarray.size(); iJD1x++)
			{
				Spt1x.x = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x].E;
				Spt1x.y = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x].N;
				Ept1x.x = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x+1].E;
				Ept1x.y = g_DocManageAry[m_iDoc].XLAry[blix]->JDarray[iJD1x+1].N;
				Line1x.set(Spt1x,Ept1x);
				IsParallel = Line1x.isParallelTo(Line2x,Tol);
				if (IsParallel)
				{
					break;
				}
			}
			if (!IsParallel)
			{
				ERR.Format(L"绕行终点%s-%s直线边不与基线上对应边平行",JDarray[i].JDName,JDarray[i+1].JDName);
				fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KZH + QXB[i+1].KZH),ERR);
				NERR++;
			}
		}
	}
	for(int i=1;i<NJD-1;i++)
	{
		double tool;
		tool=QXB[i].KZH;
		tool=QXB[i].KHZ;
		tool=QXB[i].KYH;
		tool=QXB[i].KHY;
		tool=QXB[i].R;
		tool=QXB[i].L;
		ljia = QXB[i].KYH - QXB[i].KHY;//夹圆
		if(ljia<20.0)
		{
			ERR.Format(L"%s的圆曲线长%0.2lf小于20m",JDarray[i].JDName,ljia);
			fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KYH + QXB[i].KHY),ERR);
			NERR++;
		}
	}
	for(int i=1;i<NJD-1;i++)
	{		
		if(JDarray[i].R<Rmin1)
		{
			ERR.Format(L"%s的曲线半径%0.2lf小于最小曲线半径(一般:%0.0lf,困难:%0.2lf)",JDarray[i].JDName,JDarray[i].R,Rmin1,Rmin2);
			fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KYH + QXB[i].KHY),ERR);
			NERR++;
		}
	}


	//道岔检测部分
	//1.读取数据库中道岔部分
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(m_mdbName, L"道岔表", L"");

	//2.检测道岔起终里程与曲线直缓缓直点
	for (int i=0; i<QXB.size(); i++)
	{
		for (int j=0; j<xlmdb.m_SwitchNum; j++)
		{
			double Stcml=xlmdb.m_SwitchArray[j].StartCml;
			double Edcml=xlmdb.m_SwitchArray[j].EndCml;
			double QXSt, QXEd;
			QXSt=TrsProjmltoCml(QXB[i].KZH);
			QXEd=TrsProjmltoCml(QXB[i].KHZ);
			TrsCkmltoCml(xlmdb.m_SwitchArray[j].CenCkml, xlmdb.m_SwitchArray[j].CenCml);
			if (Stcml<QXSt&&Edcml>QXSt || Stcml<QXEd&&Edcml>QXEd || Stcml>QXSt&&Edcml<QXEd)
			{
				//ERR.Format(L"%s的曲线半径%0.2lf小于最小曲线半径(一般:%0.0lf,困难:%0.2lf)",JDarray[i].JDName,JDarray[i].R,Rmin1,Rmin2);
				ERR.Format(L"%s的平面曲线与中心里程为%s的道岔有重叠",JDarray[i].JDName,xlmdb.m_SwitchArray[j].CenCkml);
				fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KYH + QXB[i].KHY),ERR);
				NERR++;
			}
		}

	}


	//将NERR写入文件
	rewind(fpw);
	fwprintf(fpw,L"%10d\n",NERR);
	fclose(fpw);


	if(NERR<1)
	{
		ads_printf(L"平面线形通过夹直线、夹圆线、最小曲线半径、道岔检测!\n");
		return;
	}


	ERRdlg = new LISTERR();
	ERRdlg->m_pm2=this;
	ERRdlg->m_bGZXS=true;
	ERRdlg->xlname=m_XLName;
	ERRdlg->Create(IDD_DIALOG_LISERR);
	ERRdlg->ShowWindow(SW_SHOW);
}





/*
函数功能:计算S弯的转角、两交点坐标
------------------------------------------------------------
输入: 半径、缓长、夹直线长、线间距1、线间距2、夹直线长、起始反向曲线的直缓坐标
------------------------------------------------------------
输出: S弯的转角、两交点坐标
------------------------------------------------------------
算法描述:               */

bool RHA::CalS(double R1,double L01,double R2,double L02,double xjj1,double xjj2,double JZXLen,double ZH_N,double ZH_E,double Fwj,double& afa,double &x1,double &y1,double &x2,double &y2)
{
	afa = 0.0;
	double T1,T2;
	T1 = T2 = 0.0;
	if (!Get_S_Afa(R1,L01,R2,L02,fabs(xjj2-xjj1),JZXLen,afa,T1,T2))
	{
		return false;
	}
	if (xjj1<-1.0e-6)
		afa *= -1.0;
	double xzh,yzh;//变距起点x,y,交点边线方位角
	//xlpoint Pt;//ZH点
	//	Pt.lc=TYLC1(ZHLC,m_pm1->dlNum,m_pm1->DLB);
	//	xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);
	//Pt.lc = PROJ_ML(array,tarray,ZH_N,ZH_E,ZH_N,ZH_E);
	//xlpoint_pz(array,tarray,&Pt);
	//xzh=Pt.x+xjj1*cos(Pt.a+0.5*pi);
	//yzh=Pt.y+xjj1*sin(Pt.a+0.5*pi);
	//x1=xzh+T1*cos(Pt.a);
	//y1=yzh+T1*sin(Pt.a);
	//double flag=1.0;
	//if(xjj2<xjj1)flag=-1.0;
	//double dd;
	//dd=(T1+T2+JZXLen);
	//x2=x1+dd*cos(Pt.a+flag*afa);
	//y2=y1+dd*sin(Pt.a+flag*afa);

	xzh=ZH_N+xjj1*cos(Fwj+m_dHalfPI);
	yzh=ZH_E+xjj1*sin(Fwj+m_dHalfPI);
	x1=xzh+T1*cos(Fwj);
	y1=yzh+T1*sin(Fwj);
	double flag=1.0;
	if(xjj2<xjj1)flag=-1.0;
	double dd;
	dd=(T1+T2+JZXLen);
	x2=x1+dd*cos(Fwj+flag*afa);
	y2=y1+dd*sin(Fwj+flag*afa);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//计算S弯的转角
//R1,L01第一个交点的半径缓长
//R2,L02第二个交点的半径缓长
//DetaXJJ前后线间距差值
//JZXLen夹直线长
//Alfa转角(弧度单位)
bool RHA::Get_S_Afa(double R1,double L1,double R2,double L2,double DetaXJJ,double JZXLen,double& Alfa,double& T1,double& T2)
{
	Alfa = 0.0;
	T1 = T2 = 0.0;
	double A1,A2,B1,B2,P1,P2,M1,M2;
	A1=sqrt(R1*L1);A2=sqrt(R2*L2);

	if(R1>1.0e-6 && R2>1.0e-6)
	{
		B1=L1*0.5/R1;
		B2=L2*0.5/R2;
	}
	else
	{
		AfxMessageBox(L"半径太小");
		return false;
	}


	if (L1<0.0001) { P1=0.0;M1=0.0; }
	else {     P1=spiral_p(A1,L1);M1=spiral_xm(A1,L1);   };
	if (L2<0.0001) { P2=0.0;M2=0.0; }
	else {     P2=spiral_p(A2,L2); M2=spiral_xm(A2,L2);  };


	//////////////////////////////////////////////////////////////////////////
	//根据以下关系计算转角Alfa
	//DetaXJJ / ((T1+T2)+JZXLen) = sin(Alfa)
	//即：DetaXJJ / ((R1+P1)*tan(Alfa/2.0) + M1 + (R2+P2)*tan(Alfa/2.0) + M2 + JZXLen) = sin(Alfa)
	//令tan(Alfa/2.0) = t由万能公式得sin(Alfa) = 2*t / (1+t*t)
	//最后得方程如下(2*(R1+P1+R2+P2)-DetaXJJ) * t*t + 2*(M1+M2+JZXLen)*t + (-DetaXJJ) = 0
	double a,b,c,t;
	a = 2*(R1+P1+R2+P2)-DetaXJJ;
	b = 2*(M1+M2+JZXLen);
	c = -DetaXJJ;
	double Deta = b*b-4.0*a*c;
	if (Deta<0.0001)
	{
		AfxMessageBox(L"无法求接满足条件的S弯转角");
		return false;
	}

	double t1 = (-1.0*b + sqrt(Deta)) / (2.0*a);
	double t2 = (-1.0*b - sqrt(Deta)) / (2.0*a);

	double Alf1 = 2 * atan(t1);
	double Alf2 = 2 * atan(t2);

	bool IsRuiJiao1,IsRuiJiao2;//是否锐角

	IsRuiJiao1 = IsRuiJiao2 = false;
	if (Alf1>1.0e-6 && Alf1<PI/2.0)
	{
		IsRuiJiao1 = true;
	}
	if (Alf2>1.0e-6 && Alf2<PI/2.0)
	{
		IsRuiJiao2 = true;
	}
	if (IsRuiJiao1 && IsRuiJiao2)
	{
		Alfa = min(Alf1,Alf2);
	}
	else if (IsRuiJiao1)
	{
		Alfa = Alf1;
	}
	else if (IsRuiJiao2)
	{
		Alfa = Alf2;
	}
	else
	{
		AfxMessageBox(L"S弯转角为钝角，请检查数据是否有误");
		return false;
	}
	T1 = (R1+P1)*tan(Alfa/2.0) + M1;
	T2 = (R2+P2)*tan(Alfa/2.0) + M2;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//选择S弯
bool RHA::SelSModel(double N, double E,int& SJD,int& S_JD1,int& S_JD2)
//删除指定点处的S弯
{
	S_JD1 = 0;
	S_JD2 = 0;
	int iJD=GetPreJD(N,E);
	SJD = iJD-1;
	S_JD1 = iJD;
	S_JD2 = iJD+1;
	if (fabs(QXB[S_JD1].LorR*QXB[S_JD1].a + QXB[S_JD2].LorR*QXB[S_JD2].a)<0.00001)
	{
		return true;
	}
	else
	{
		AfxMessageBox(L"所选不为S弯，请重新选择\n");
		return false;
	}
}

//获得某绕行段所有用户设置的标志里程
void RHA::GetiRxdBZLCArray(int iRXD ,  vector<BZLCStructTable> & bzlcAry)
{
	bzlcAry.clear();

	if (XLShuXing == SJJX || XLShuXing == SJ2X)
		return;
	
	int sDL, eDL;
	GetiRXDDLinfo(iRXD, sDL, eDL, DLarray);

	if(sDL>-1 && eDL>-1)
	{
		for (int i=0;i<BZLCarray.size();i++)
		{
			if (!BZLCarray[i].isAutoAdd)
			{
				if (BZLCarray[i].xyToprojml+0.001<DLarray[eDL].TYLC
					&& BZLCarray[i].xyToprojml-0.001>DLarray[sDL].TYLC)
				{
					bzlcAry.push_back(BZLCarray[i]);
				}
			}
		}

	}
	else
	{ads_printf(L"\n未找到绕行段断链");}
		

	

}

//删除绕行段标志里程 isDelSE：是否删除绕行起终标志里程
//备注：调用该函数时，需要在断链修改之前           818 根据绕行段编号来？考虑：标志里程不一定有顺序
void RHA::DeleteiRXDBZLC(int iRXD, bool isDelSE)
{
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	//删除该绕行段原有的标志里程
	int sdl, edl;
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
	
	if(sdl>-1 && edl>-1)
	{
		for(int j=0; j<BZLCarray.size(); j++)
		{
			if ((BZLCarray[j].xyToprojml > DLarray[sdl].TYLC+0.0001
				&& BZLCarray[j].xyToprojml+0.0001 < DLarray[edl].TYLC))
			{
				//BZLCarray.RemoveAt(j);
				BZLCarray.erase(BZLCarray.begin()+j);

				j--;
				continue;
			}

			if(isDelSE  && (fabs(BZLCarray[j].xyToprojml-DLarray[sdl].TYLC)<0.1
				|| fabs(BZLCarray[j].xyToprojml-DLarray[edl].TYLC)<0.1))
			{
				//BZLCarray.RemoveAt(j);
				BZLCarray.erase(BZLCarray.begin()+j);
				j--;
				continue;
			}
		}
	}
	else
	{
		ads_printf(L"未找到绕行段断链\n");
	}
}

//全线绕行线路自动赋交点号、交点性质
void RHA::GenJDNameJDXZ()
{
	//两个交点
	int NJD=JDarray.size();
	for(int i=0; i<NJD; i++)
	{
		_stprintf(JDarray[i].JDName, L"JD%d", i);
		JDarray[i].JDXZ=RX_One;
	}
	JDarray[0].JDXZ=RX_Start;
	JDarray[NJD-1].JDXZ=RX_End;
}

//指定交点序号，修改其坐标、半径、缓长
void RHA::MODIFYJD(int iJD,double N, double E, double l1, double R, double l2)
{
	if (iJD<JDarray.size())
	{
		JDarray[iJD].N=N;
		JDarray[iJD].E=E;
		JDarray[iJD].l1=l1;
		JDarray[iJD].R=R;
		JDarray[iJD].l2=l2;
	}

}

//由绕行段编号获取绕行段线路名
CString RHA::GetRXDXLNameByRXDIndex(int RXDIndex)
{
	int SumRXD=0;
	int SDLNo,EDLNo;
	CString rxdname;
	SumRXD = GetRXDNum(JDarray);
	if (SumRXD==0 || RXDIndex+1>SumRXD)
		return "";
	GetiRXDDLinfo(RXDIndex, SDLNo, EDLNo, DLarray);
	rxdname = DLarray[SDLNo].RXDName;
	return rxdname;
}

//由绕行段线路名获取绕行段编号
int RHA::GetRxdIndx(CString RXDXLName)
{
	int SumRXD=0;
	int SDLNo,EDLNo;
	CString rxdname;

	SumRXD = GetRXDNum(JDarray);
	for (int iRXD=0 ; iRXD<SumRXD ; iRXD++)
	{
		GetiRXDDLinfo(iRXD, SDLNo, EDLNo, DLarray);
		rxdname = DLarray[SDLNo].RXDName;
		if (rxdname == RXDXLName)
			return iRXD;
	}
	return -1;
}

//由【纵面】绕行段线路名获取【纵面】绕行段编号
int RHA::GetZMRxdIndx(CString zmRxdName)
{
	CString rxdname;

	for (int iRXD=0 ; iRXD<FZZDMAry.size(); iRXD++)
	{
		rxdname=FZZDMAry[iRXD].DLName;
		if (rxdname == zmRxdName)
			return iRXD;
	}
	return -1;
}


//生成从统一里程lc1到统一里程lc2的线路中线
//缺省FALSE,CONTINUOUS
//生成从统一里程lc1到统一里程lc2的线路中线
//缺省FALSE,CONTINUOUS
void RHA::DrawXLZXByXY(double Scml, double Ecml, vector<vector<double>> XYArray, int XYNum, double ZxWid, int linetype, ACHAR* LayerName, int LRMFlag)  const
{
	xlpoint PB,PE;
	double INDATA[5];
	int j;
	int DrawXySn,DrawXyEn,i;
	AcGePoint3d Pt,PtText;
	double cml;
	xlpoint  PZtmp; 
	AcGePoint3dArray Pt2dAy;
	AcDbObjectId qID,hID;
	double Offset = 0.0;



	Pt2dAy.removeAll();

	// 绘线位
	PB.x=XYArray[0][1]; //N
	PB.y=XYArray[0][2]; // E
	PB.a= XYArray[0][3];// FA
	PB.lc= XYArray[0][4];
	XYArray[0][6]=XYArray[0][4];//因为0层分（起点）线元其实里程在下标4处记录，而普通线元在下标6处记录，在这里对0层分的6下标赋值，以便后续程序应用。wsb
	XYArray[0][8]=XYArray[0][1];
	XYArray[0][9]=XYArray[0][2];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;


	DrawXySn = 1; DrawXyEn=  XYNum;
	for (i = 1; i <  XYNum; i++) 
	{
		if( XYArray[i][6]>Scml-0.01)
		{
			DrawXySn = i;
			break;
		}
	}
	if (i==XYNum)
	{
		DrawXySn = XYNum;
	}
	for (i = 1; i <=  XYNum; i++)
	{
		if( XYArray[i][6]>Ecml-0.01)
		{
			DrawXyEn = i-1;
			break;
		}
	}

	if (LRMFlag==LEFTLINE && DrawParam.SCALE>9.9)//这个LRMFlag 有用吗 818
		Offset = -DrawParam.m_ExOffset;
	if (LRMFlag==RIGHTLINE && DrawParam.SCALE>9.9)
		Offset = DrawParam.m_ExOffset;
	if(DrawXySn > 1)
	{
		cml = Scml;
		PZtmp.lc = cml;
		xlpoint_pz(XYArray,XYNum,PZtmp);
		Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));

		if (DrawXyEn==DrawXySn-1)//绘图范围不足一个线元
		{
			xlpoint PZtmp2;
			PZtmp2.lc = Ecml;
			xlpoint_pz(XYArray,XYNum,PZtmp2);
			if( XYArray[DrawXySn-1][0]<1.1)//直线
			{
				Pt2dAy.append(AcGePoint3d( PZtmp2.y, PZtmp2.x,0));
				//	qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
			}
			else if( XYArray[DrawXySn-1][0]<2.1)//圆
			{
				int NUM=Pt2dAy.length();
				Pt2dAy[NUM-1].z=-tan((Ecml-Scml)/XYArray[DrawXySn-1][1]/4)*XYArray[DrawXySn-1][4];
				Pt2dAy.append(AcGePoint3d( PZtmp2.y, PZtmp2.x,0));
				//qID = makearc(AcGePoint3d(PZtmp.y,PZtmp.x,0.0),AcGePoint3d( PZtmp2.y, PZtmp2.x,0.0), XYArray[DrawXySn-1][1], XYArray[DrawXySn-1][4],
				//	-1,1,LayerName);
				//if(ZxWid>0.01)
				//{
				//	ads_point ptt;
				//	ptt[X] =  PZtmp2.y,ptt[Y] =  PZtmp2.x;
				//	ads_command(RTSTR,L"Pedit",RTPOINT,ptt,RTSTR,L"Y",RTSTR,L"W",RTREAL,ZxWid,RTSTR,L"",0);			
				//}
			}
			else//缓和曲线
			{
				cml=XYArray[DrawXySn-1][6];
				while(cml <  Ecml)
				{
					PZtmp.lc = cml;
					xlpoint_pz(XYArray,XYNum,PZtmp);
					Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));
					cml+=5;
				}
				PZtmp.lc =  Ecml;
				xlpoint_pz(XYArray,XYNum,PZtmp);
				Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));			
				//qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
			}

			if(linetype==CONTINUELINE)
				makepolylineTD(Pt2dAy,-1,ZxWid,LayerName,L"CONTINUOUS",DrawParam.SCALE,Offset);
			else if (linetype == TUNMIDLINE)
				makepolylineTD(Pt2dAy,-1,ZxWid,LayerName,L"TUNNELMIDLINE",DrawParam.SCALE,Offset);
			else
				makepolylineTD(Pt2dAy,-1,ZxWid,LayerName,L"RAILLEGEND",DrawParam.m_LegendLen/10.0*DrawParam.SCALE,Offset);
			return;
		}

		if( XYArray[DrawXySn-1][0]<1.1)//直线
		{
			Pt2dAy.append(AcGePoint3d( XYArray[DrawXySn][9], XYArray[DrawXySn][8],0));
			//qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
		}
		else if( XYArray[DrawXySn-1][0]<2.1)//圆
		{
			int NUM=Pt2dAy.length();
			Pt2dAy[NUM-1].z=-tan((XYArray[DrawXySn-1][2]-Scml+XYArray[DrawXySn-1][6])/XYArray[DrawXySn-1][1]/4)*XYArray[DrawXySn-1][4];
			Pt2dAy.append(AcGePoint3d( XYArray[DrawXySn][9], XYArray[DrawXySn][8],0.0));
			//qID = makearc(AcGePoint3d(PZtmp.y,PZtmp.x,0.0),AcGePoint3d( XYArray[DrawXySn][9], XYArray[DrawXySn][8],0.0), XYArray[DrawXySn-1][1], XYArray[DrawXySn-1][4],
			//	-1,1,LayerName);
			//if(ZxWid>0.01)
			//{
			//	ads_point ptt;
			//	ptt[X] =  XYArray[DrawXySn][9],ptt[Y] =  XYArray[DrawXySn][8];
			//	ads_command(RTSTR,L"Pedit",RTPOINT,ptt,RTSTR,L"Y",RTSTR,L"W",RTREAL,ZxWid,RTSTR,L"",0);			
			//}
		}
		else//缓和曲线
		{
			cml+=5;
			while(cml <  XYArray[DrawXySn][6])
			{
				PZtmp.lc = cml;
				xlpoint_pz(XYArray,XYNum,PZtmp);
				Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));
				cml+=5;
			}
			PZtmp.lc =  XYArray[DrawXySn][6];
			xlpoint_pz(XYArray,XYNum,PZtmp);
			Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));			
			//qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
		}		
	}

	for(i=DrawXySn;i<DrawXyEn;i++)
	{
		if( XYArray[i][0]<1.1)//直线
		{
			Pt2dAy.append(AcGePoint3d( XYArray[i][9], XYArray[i][8],0));
			Pt2dAy.append(AcGePoint3d( XYArray[i+1][9], XYArray[i+1][8],0));
			//qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
		}
		else if( XYArray[i][0]<2.1)//圆
		{
			double TempTD;
			TempTD=-tan(XYArray[i][2]/XYArray[i][1]/4)*XYArray[i][4];
			Pt2dAy.append(AcGePoint3d( XYArray[i][9], XYArray[i][8],TempTD));
			Pt2dAy.append(AcGePoint3d( XYArray[i+1][9], XYArray[i+1][8],0.0));
			//qID = makearc(AcGePoint3d(PZtmp.y,PZtmp.x,0.0),AcGePoint3d( XYArray[DrawXySn][9], XYArray[DrawXySn][8],0.0), XYArray[DrawXySn-1][1], XYArray[DrawXySn-1][4],
			//	-1,1,LayerName);
			//if(ZxWid>0.01)
			//{
			//	ads_point ptt;
			//	ptt[X] =  XYArray[DrawXySn][9],ptt[Y] =  XYArray[DrawXySn][8];
			//	ads_command(RTSTR,L"Pedit",RTPOINT,ptt,RTSTR,L"Y",RTSTR,L"W",RTREAL,ZxWid,RTSTR,L"",0);			
			//}
		}
		else//缓和曲线
		{
			cml=XYArray[i][6];
			while(cml <  XYArray[i+1][6])
			{
				PZtmp.lc = cml;
				xlpoint_pz(XYArray,XYNum,PZtmp);
				Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));
				cml+=5;
			}
			PZtmp.lc =  XYArray[i+1][6];
			xlpoint_pz(XYArray,XYNum,PZtmp);
			Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));			
			//	qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
		}		
	}

	if(DrawXyEn <  XYNum)
	{    
		cml =  XYArray[DrawXyEn][6];
		Pt2dAy.append(AcGePoint3d( XYArray[DrawXyEn][9], XYArray[DrawXyEn][8],0));
		if( XYArray[DrawXyEn][0]<1.1)//直线
		{
			cml = Ecml;
			PZtmp.lc = cml;
			xlpoint_pz(XYArray,XYNum,PZtmp);
			Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));
			//hID = makepolyline(Pt2dAy1,-1,ZxWid,LayerName);
		}
		else if( XYArray[DrawXyEn][0]<2.1)//圆
		{
			int NUM=Pt2dAy.length();
			Pt2dAy[NUM-1].z=-tan((Ecml-cml)/XYArray[DrawXyEn][1]/4)*XYArray[DrawXyEn][4];
			PZtmp.lc = Ecml;
			xlpoint_pz(XYArray,XYNum,PZtmp);
			Pt2dAy.append(AcGePoint3d( PZtmp.y, PZtmp.x,0.0));
			//hID = makearc(AcGePoint3d( XYArray[DrawXyEn][9], XYArray[DrawXyEn][8],0.0),AcGePoint3d(PZtmp.y,PZtmp.x,0.0), XYArray[DrawXyEn][1], XYArray[DrawXyEn][4],
			//	-1,1,LayerName);
			//if(ZxWid>0.01)
			//{
			//	ads_point ptt;
			//	ptt[X] = PZtmp.y,ptt[Y] = PZtmp.x;
			//	ads_command(RTSTR,L"Pedit",RTPOINT,ptt,RTSTR,L"Y",RTSTR,L"W",RTREAL,ZxWid,RTSTR,L"",0);			
			//}
		}
		else
		{
			cml=XYArray[DrawXyEn][6];
			while(cml < Ecml)
			{
				PZtmp.lc = cml;
				xlpoint_pz(XYArray,XYNum,PZtmp);
				Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));
				cml+=5;
			}
			PZtmp.lc = Ecml;
			xlpoint_pz(XYArray,XYNum,PZtmp);
			Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));	
			//hID = makepolyline(Pt2dAy1,-1,ZxWid,LayerName);
		}
	}

	int colo=DrawParam.OtherParam.EntAry[0].Colo;
	if(linetype == CONTINUELINE)
		makepolylineTD(Pt2dAy,colo,ZxWid,LayerName,L"CONTINUOUS",DrawParam.SCALE,Offset);
	else if (linetype == TUNMIDLINE)
		makepolylineTD(Pt2dAy,colo,ZxWid,LayerName,L"TUNNELMIDLINE",DrawParam.SCALE,Offset);
	else
		makepolylineTD(Pt2dAy,colo,ZxWid,LayerName,L"RAILLEGEND",DrawParam.m_LegendLen/10.0*DrawParam.SCALE,Offset);
}

//调用DrawXLZXByXY，绘制lc1-lc2各个改建段,//缺省FALSE,CONTINUOUS
void RHA::MakeXLZX(double lc1,double lc2,int linetype,int LRMFlag,bool IsDrawRXGJ) const
{	
	ACHAR layer[256];
	double LineWide, LW;

	_tcscpy(layer,DrawParam.XianLuZhongXian);

	LW=DrawParam.OtherParam.EntAry[0].LW;

	if (lc1>lc2)
		return;
	
	if (lc1<QXB[0].KZH)
		lc1=QXB[0].KZH;
	if (lc2>QXB[JDarray.size()-1].KHZ)
		lc2=QXB[JDarray.size()-1].KHZ;

	if(XLShuXing == JYJX ||  XLShuXing == SGBX
		|| GJDInforArray.size() == 0)
	{
		LineWide = 0.5*LW;
		DrawXLZXByXY(QXB[0].KZH, QXB[JDarray.size()-1].KHZ,array,tarray,LineWide,linetype,layer,LRMFlag);
	}
	else
	{
		int sGJD=-1, eGJD=-1;
		for (int i=0; i<GJDInforArray.size(); i++)
		{
			if (lc1+0.001>GJDInforArray[i].BProjml)
			{
				sGJD=i;
				break;
			}
		}

		for (int i=sGJD; i<GJDInforArray.size(); i++)
		{
			if (lc2<GJDInforArray[i].EProjml+0.001)
			{
				eGJD=i;
				break;
			}
		}

		if (sGJD<eGJD)
		{
			if (GJDInforArray[sGJD].GJDType == -1)
				LineWide = 0.5*LW;
			else
				LineWide = LW;
			//第一段
			DrawXLZXByXY(lc1,GJDInforArray[sGJD].EProjml
				,array,tarray,LineWide,linetype,layer,LRMFlag);
			//中间改建段
			for (int j=sGJD+1;j<eGJD; j++)
			{
				if (GJDInforArray[j].GJDType == -1)//Zhujiang 既有线线宽跟设计线线宽一样
					LineWide = 0.5*LW;
				else
					LineWide = LW;

				DrawXLZXByXY(GJDInforArray[j].BProjml, GJDInforArray[j].EProjml
					, array, tarray, LineWide, linetype, layer, LRMFlag);
			}

			if (GJDInforArray[eGJD].GJDType == -1)
				LineWide = 0.5*LW;
			else
				LineWide = LW;
			//最后一段
			DrawXLZXByXY(GJDInforArray[eGJD].BProjml, lc2
				, array, tarray, LineWide, linetype, layer, LRMFlag);

		}
		else //lc1~lc2在同一个改建段区间
		{
			if (GJDInforArray[eGJD].GJDType == -1)
				LineWide = 0.5*LW;
			else
				LineWide = LW;

			DrawXLZXByXY(lc1, lc2
				, array, tarray, LineWide, linetype, layer, LRMFlag);

		}

	}
	
}

//绘制交点连线和圆
void RHA::BZJdLineAndJdCir() const
{
	xlpoint PZtmp;
	AcGePoint3d Pt;
	AcGeVector3d normal(0.0,0.0,1.0);
	AcGePoint2dArray PtAy;
	ACHAR layname[256];

	_tcscpy(layname, DrawParam.JiaoDianLianXian);

	for(int i=1;i+2 <= JDarray.size();i++)
	{
		//if(!JDRx[i].ISShow)continue; 818 既有二线
		//绘交点连线
		PZtmp.lc=QXB[i].KZH;
		xlpoint_pz(array,tarray,PZtmp);
		PtAy.append(AcGePoint2d(PZtmp.y,PZtmp.x));
		PtAy.append(AcGePoint2d(JDarray[i].E,JDarray[i].N));

		PZtmp.lc=QXB[i].KHZ ;
		xlpoint_pz(array,tarray,PZtmp);
		PtAy.append(AcGePoint2d(PZtmp.y,PZtmp.x));
		makeDX(PtAy,1.0*DrawParam.SCALE,-1,AcDb::kLnWtByLayer,layname);//zh-交点-hz连线
		PtAy.removeSubArray(0,PtAy.length()-1);

		//交点处的圆
		if (fabs(QXB[i].R)<0.001 && fabs(QXB[i].l1)<0.001 && fabs(QXB[i].l2)<0.001)//折点不标交点处的圆
			continue;
		Pt[X]=JDarray[i].E; 
		Pt[Y]=JDarray[i].N;
		Pt[Z]=0.0;
		AcDbCircle *pCir=new AcDbCircle(Pt,normal,1.0*DrawParam.SCALE);
		pCir->setLayer(layname);
		AddEntityToDbs(pCir);
	}


}


//功能：炸开线路，绘平面图
//备注：炸开时，系统会走subErase()函数回收g_DocManageAry[iDoc].XLAry数组中的成员
Acad::ErrorStatus RHA::subExplode(AcDbVoidPtrArray& entitySet) const
{
	// 绘线路中线
	//818 需设置默认参数 未考虑临时线

	//创建图层
	DrawParam.CreateDrawLayer();

	//绘制线路中线
	if (DrawParam.m_IsRailLegend == 0)
		MakeXLZX(QXB[0].KZH, QXB[JDarray.size()-1].KHZ, CONTINUELINE, MAINLINE);
	else
		MakeXLZX(QXB[0].KZH, QXB[JDarray.size()-1].KHZ, RAILLEGEND, MAINLINE);


	//MakeXLZX(L"贯通设计Ⅱ线",sLCLx,eLCLx,CONTINUELINE,LEFTLINE);
	//MakeXLZX(L"贯通设计基线",sLC1x,eLC1x,CONTINUELINE,MAINLINE);//绘制线路中线


	//绘交点连线和交点处的圆  
	BZJdLineAndJdCir();

	//if(HasLeft && !HasRight)// 818 不在这里改类变量
	//	draw_zybz=1;
	//if(!HasLeft && HasRight)
	//	draw_zybz=-1;

	
	//标注曲线要素
	if (DrawParam.m_JIAODIAN)
	{
		BZJD(L"既有基线",MAINLINE);//818 左右
	}

	//标注绕行段里程： 百米标 断链
	if (DrawParam.m_LICHENG)
		MarkRXDLC();
		

	//标注特征点
	if (DrawParam.m_IsNoteTZD)
		MarkTZD();

	//标注废弃段
	if (XLShuXing == JYJX || XLShuXing == JY2X)
		BZJYXFQD();

	//桥
	if (DrawParam.m_IsNoteBMBR)
		MarkBridge();

	//隧
	if (DrawParam.m_IsNoteTUNNEL)
		MarkTunnel();

	//站
	if (DrawParam.m_IsNoteSTATION)
		MarkStation();

	//小桥涵
	if (DrawParam.m_IsNoteSMALLBR)
		plhINSBR(true);//所有小桥涵
	else if(!DrawParam.m_IsNoteSMALLBR && DrawParam.m_IsNoteCROSSBR)
		plhINSBR(false);//立交涵洞

	//水准点
	if (DrawParam.m_IsNoteBMBR)
		MarkBM();

	//导线
	if (DrawParam.m_IsNoteDAOXIAN)
		MarkDX();


	
	return Acad::eOk;
}

	//标注交点号 和曲线参数
void RHA::BZJD(CString XLName,int LMRFlag) const
{
	double offset = 0.0;
	ACHAR layname[256];
	ACHAR JDH[80],ZhuanJiao[80],Radius[80],HuanChang[80]
		,QieChang[80],QuXianChang[80],NeiYeDuanLian[80],GroupName[80];
	AcDbObjectIdArray IdAr,IdArJD;

	//double fb,fe,f0,textAng,aa,dd,dt;
	//double R,T,T1,T2,L1,L2,LL;

	double fb,fe,de,da,aa,dd,dt, f0;
	AcGePoint3d  p1, CenPt;//标注插入点，内业断链插入点
	ACHAR ch[80],ch1[60],chrtmp[80];
	AcGePoint2d LBPt,RBPt,LTPt,RTPt;
	AcGePoint2dArray PtAr;
	int i,c,n;

	//AcGePoint3d Pt0;//交点所有参数的中心位置
	//AcGePoint3d JDPt;//交点
	//AcGePoint3d JDRotatePt;//交点
	int sJD, eJD/*,c,n*/;
	p1[Z]=0.0;//Pt0[Z]=0.0;
	int GJDType = 2;
	bool isFQDJD=false;//是否为废弃段交点

	AcGePoint3d JCD;//废弃交点标志的交叉中点
	AcGePoint3d Pt1,Pt2,Pt3,Pt4;


	_tcscpy(layname, DrawParam.PingQuXianYaoSu);
	//设图层
	if(LMRFlag==MAINLINE)
	{
	/*	if (XLName == L"既有基线")
			_stprintf(layname,L"%s",DrawParam.PingQuXianYaoSu);*/
		/*else if (XLName == L"施工便线")
			_stprintf(layname,L"%s",DrawParam.BXJiaoDian);
		else if (XLName == L"设计基线")
			_stprintf(layname,L"%s",DrawParam.GJJiaoDian);
		else
			return;*/
	}
	//else if(LMRFlag==LEFTLINE)
	//{
	//	if (XLName == L"既有左线")
	//		_stprintf(layname,L"%s",DrawParam.LeftJiaoDian);
	//	else if (XLName == L"设计二线")
	//		_stprintf(layname,L"%s",DrawParam.GJLeftJiaoDian);
	//	else
	//		return;
	//	if (DrawParam.SCALE>9.9)
	//		offset = DrawParam.m_ExOffset;
	//}
	//else 
	//{
	//	if (XLName == L"既有右线")
	//		_stprintf(layname,L"%s",DrawParam.RightJiaoDian);//二线的图层名
	//	else if (XLName == L"设计二线")
	//		_stprintf(layname,L"%s",DrawParam.GJRightJiaoDian);
	//	else
	//		return;
	//	if (DrawParam.SCALE>9.9)
	//		offset = DrawParam.m_ExOffset;
	//}


	int NJD=JDarray.size();
	/*sJD=DrawParam.SJD1x;
	eJD=DrawParam.EJD1x;		
	if(sJD<=0)sJD=1;
	if(eJD>=NJD-1)eJD=NJD-2;*/

	//计算曲线要素
	//CalNoteQXYS(); 

	//int NJD = JDarray.size();
	int colo;
	bool isShow=false;
	double texHgt=DrawParam.OtherParam.MarkAry[0].TextHeight;
	dt=1.5*texHgt;//两行字串的间距
	colo=DrawParam.OtherParam.MarkAry[0].Colo;

	int leftOrRight=DrawParam.OtherParam.MarkAry[0].LorR;
	//818 LorR =0 曲线要素标在内侧 =-1 左侧 =1 右侧

	de=xyddaa(JDarray[0].N,JDarray[0].E,JDarray[1].N,JDarray[1].E,&fb);
	///int zory=-1;//标注 1——左侧，-1——右侧
	for (i=1;i<NJD-1;i++)  
	{  
		//if( JDarray[i].JDXZ==BXPX && fabs(JDarray[i].XJJ)<0.0001 )
		//	isShow=false;//不显示 818 什么时候不显示？
		//else if (JDarray[i].R<0.001 && JDarray[i].l1<0.001 && JDarray[i].l2<0.001)//半径缓长为0的交点
		//	isShow=false;// 不显示
		//else
		//	isShow=true;

		//判断曲线要素标注是否显示-8.13-zy
		//1.既有线全部显示
		isShow = true;
		//2.设计线判断是否为利用段
		if (XLShuXing == L"设计基线" || XLShuXing == L"设计二线")
		{
			//2.1 根据改建段汇总表中数据来判断
			for (int j=0; j<GJDInforArray.size(); j++)
			{
				if (QXB[i].KZH>GJDInforArray[j].BProjml+0.001 
					&& QXB[i].KHZ+0.001<GJDInforArray[j].EProjml)
				{
					if (GJDInforArray[j].GJDType == -1)
						isShow = false;
				}

			}
		}
			
		//if (JDarray[i].R<-0.1) break;
		de=xyddaa(JDarray[i].N,JDarray[i].E,JDarray[i+1].N,JDarray[i+1].E,&fe);

		aa= fe - fb ;
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0) 
		{ //右偏
			c=1; dd=2*texHgt; 
		}   
		else
		{ //左偏 
			c=-1; dd= 2*texHgt;
		};

		de=fb+aa*0.5;

		_tcscpy(JDH,JDarray[i].JDName);

		n=_tcslen(JDH);
		p1[Z]=0.0;

		p1[X]=JDarray[i].E-n*0.5*texHgt*sin(de)+dd*sin(de+c*pi*0.5);
		p1[Y]=JDarray[i].N-n*0.5*texHgt*cos(de)+dd*cos(de+c*pi*0.5);

		da=0.5*Pi-de;

		
		if (DrawParam.m_IsNoteJDNum)
			IdAr.append(maketext(p1,JDH,da,texHgt ,-1,1,L"宋体",layname));//交点号

		if(qxbbz == 1)//角平分线中线对齐
		{
			// 标曲线参数 begin
			double R,T,T1,T2,L1,L2,LL,NYDL;
			R=fabs(JDarray[i].R); 
			L1= JDarray[i].l1;  
			L2=JDarray[i].l2; 
			T1=QXB[i].T1; 
			T2=QXB[i].T2;  
			LL=QXB[i].L;

			//并行交点的曲线参数怎么标 818
			T=(T1+T2)*0.5-(L1+L2)*0.25;

			dd=sqrt(R*R+T*T)-R;
			dd=dd+16*texHgt; 

			if (dd<50)
				dd=-50;
			if (dd>500)
				dd=500;
			// ads_printf(L"\n dd=%8.2f",dd);
			if (leftOrRight == -1) 
			{  
				//标在左侧
				c=-1;
				dt=c*texHgt*1.5;
				//if (c==-1) 
					dd=dd+8*texHgt;
			}
			else 
			{  
				//标在右侧;
				c=1;
				dt=c*texHgt*1.5; 
				/*dd=-fabs(dd);*/
				
			}

			p1[X]=JDarray[i].E-dd*sin(de-c*pi*0.5) ; 
			p1[Y]=JDarray[i].N-dd*cos(de-c*pi*0.5);
			_tcscpy(chrtmp,L"  ");
			_tcscat(chrtmp,JDH);
			//  交点号
			if(isShow)
				IdAr.append(maketext(p1,JDH,da,texHgt ,-1,1,L"宋体",layname));//交点号
				//G_maketext(pWd,p1,chrtmp,da,texHgt ,QXYSColor,1);   // 交点号  
			//  偏角
			if (DrawParam.m_IsNoteJDAng)
			{
				D_msToDmsStr(QXB[i].a, DrawParam.NPJ, ch);

				if (aa>0) { _tcscpy(ZhuanJiao,L"ay--"); }
				else _tcscpy(ZhuanJiao,L"az--"); 
				_tcscat(ZhuanJiao,ch);

				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if(isShow)
					IdAr.append(maketext(p1,ZhuanJiao,da,texHgt ,-1,1,L"宋体",layname));//转角
			}
			//   半径
			if (DrawParam.m_IsNoteJDR && R>0.1)
			{
				if (fabs(R-(long int)(R))<0.001)  
					ads_rtos(R,2,0,ch); 
				else 
					ads_rtos(R,2,DrawParam.NLC,ch);
				_tcscpy(Radius,L"R--"); _tcscat(Radius,ch);

				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if(isShow)
					IdAr.append(maketext(p1,Radius,da,texHgt ,-1,1,L"宋体",layname));//半径
			}


			isFQDJD = false;
			if (XLShuXing == JYJX || XLShuXing == JY2X)
			{
				if(IsPtOnLYD(QXB[i].KZH) && IsPtOnLYD(QXB[i].KHZ))
					isFQDJD = false;
				else
					isFQDJD = true;
			}
				
			
				

			//绘制废弃交点标注的“×”
			if ( DrawParam.m_IsNoteJDNum && isFQDJD 
				&& DrawParam.m_IsDrawFQD && GJDInforArray.size()>0 )
			{
				JCD.x = p1.x + 0.3*_tcslen(Radius)*texHgt*sin(/*f0-0.5*pi*/de);
				JCD.y = p1.y + 0.3*_tcslen(Radius)*texHgt*cos(/*f0-0.5*pi*/de);
				Pt1.x = JCD.x + 6.0*texHgt*sin(0.5*pi-da+2.0*pi/3.0);
				Pt1.y = JCD.y + 6.0*texHgt*cos(0.5*pi-da+2.0*pi/3.0);
				Pt2.x = JCD.x + 6.0*texHgt*sin(0.5*pi-da-pi/3.0);
				Pt2.y = JCD.y + 6.0*texHgt*cos(0.5*pi-da-pi/3.0);
				IdAr.append(makeline2(Pt1,Pt2,-1,AcDb::kLnWt030,layname));
				Pt3.x = JCD.x + 6.0*texHgt*sin(0.5*pi-da-2.0*pi/3.0);
				Pt3.y = JCD.y + 6.0*texHgt*cos(0.5*pi-da-2.0*pi/3.0);
				Pt4.x = JCD.x + 6.0*texHgt*sin(0.5*pi-da+pi/3.0);
				Pt4.y = JCD.y + 6.0*texHgt*cos(0.5*pi-da+pi/3.0);
				IdAr.append(makeline2(Pt3,Pt4,-1,AcDb::kLnWt030,layname));
			}

			if (DrawParam.m_IsNoteJDOthers )  
			{
				//  缓长
				if (L1>0 || L2>0)//  缓长不为零
				{
					if (fabs(L1-(long int)(L1))<0.001)  
					{ ads_rtos(L1,2,DrawParam.NLC,ch); }
					else 
					{ ads_rtos(L1,2,DrawParam.NLC,ch); };

					p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
					p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
					if (fabs(L1-L2)<0.001) 
					{ 
						_tcscpy(HuanChang,L"Lo--");  _tcscat(HuanChang,ch);
						if(isShow)
							IdAr.append(maketext(p1,HuanChang,da,texHgt ,-1,1,L"宋体",layname));//缓长

					} 
					else
					{
						//   不等缓长
						_tcscpy(HuanChang,L"L1--");  _tcscat(HuanChang,ch);
						if (fabs(L2-(long int)(L2))<0.001) 
						{ ads_rtos(L2,2,DrawParam.NLC,ch); }
						else 
						{ ads_rtos(L2,2,DrawParam.NLC,ch); };

						_tcscat(HuanChang,L"  L2--"); _tcscat(HuanChang,ch);
						if(isShow)
							IdAr.append(maketext(p1,HuanChang,da,texHgt ,-1,1,L"宋体",layname));//缓长

					};
				}

				//  切长
				T1=QXB[i].T1; 
				T2=QXB[i].T2;  
				if (fabs(T1-(long int)(T1))<0.001)  
					ads_rtos(T1,2,0,ch);
				else
					ads_rtos(T1,2,DrawParam.NLC,ch);

				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if (fabs(L1-L2)<0.001) 
				{  
					_tcscpy(QieChang,L"T--");  _tcscat(QieChang,ch);
					if(isShow)
						IdAr.append(maketext(p1,QieChang,da,texHgt ,-1,1,L"宋体",layname));//切长 

				} 
				else
				{   
					_tcscpy(QieChang,L"T1--");  _tcscat(QieChang,ch);
					ads_rtos(T2,2,DrawParam.NLC,ch);
					_tcscat(QieChang,L"  T2--"); _tcscat(QieChang,ch);
					if(isShow)
						IdAr.append(maketext(p1,QieChang,da,texHgt ,-1,1,L"宋体",layname));//切长 

				}

				//  曲线长
				LL=QXB[i].L;
				ads_rtos(LL,2,DrawParam.NLC,ch);
				_tcscpy(QuXianChang,L"L--");  _tcscat(QuXianChang,ch);
				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if(isShow)
					IdAr.append(maketext(p1,QuXianChang,da,texHgt ,-1,1,L"宋体",layname));//曲线长 
				
				f0=de-c*pi*0.5+pi;//在角平分线上向曲线内侧方向

				//标注内业断链
				if(JDarray[i].JDXZ == BXPX || JDarray[i].JDXZ == S_Start || JDarray[i].JDXZ == S_End//并行交点/S弯
					&& DrawParam.SCALE<49.9)
				{
					/*NYDL = JDarray[i].NYDL;*/
					NYDL = CalCurvNYDL(i);
					if (fabs(NYDL)>0.0001 &&/*isShow*/1) //818
					{
						PtAr.removeAll();

						ads_rtos(NYDL,2,DrawParam.NLC,NeiYeDuanLian);

						p1[X]=p1[X] + dt*sin(f0); 
						p1[Y]=p1[Y] + dt*cos(f0);

						n=_tcslen(NeiYeDuanLian)+1;
						LBPt.x=p1.x+0.2*texHgt*sin(f0);
						LBPt.y=p1.y+0.2*texHgt*cos(f0);

						LTPt.x=LBPt.x-1.1*texHgt*sin(f0);
						LTPt.y=LBPt.y-1.1*texHgt*cos(f0);

						RTPt.x=LTPt.x + (0.7*n*texHgt)*sin(/*f0-pi*0.5*/de); 
						RTPt.y=LTPt.y + (0.7*n*texHgt)*cos(/*f0-pi*0.5*/de);

						RBPt.x=LBPt.x + (0.7*n*texHgt)*sin(/*f0-pi*0.5*/de); 
						RBPt.y=LBPt.y + (0.7*n*texHgt)*cos(/*f0-pi*0.5*/de);

						PtAr.append(LTPt);
						PtAr.append(LBPt);
						PtAr.append(RBPt);
						PtAr.append(RTPt);
						PtAr.append(LTPt);

						IdAr.append(makepolyline(PtAr,-1,AcDb::kLnWtByLayer,layname,L"CONTINUOUS"));
						
						CenPt.set(0.25*(LTPt.x+LBPt.x+RBPt.x+RTPt.x),0.25*(LTPt.y+LBPt.y+RBPt.y+RTPt.y),0.0);
						IdAr.append(maketext(CenPt,NeiYeDuanLian,da,texHgt ,-1,3,L"宋体",layname));//内业断链 

					}
					
				}


				
			};
			
		};
		fb=fe; 
	};   

	//818 左线、右转 或者 右线、左转: 移到交点外侧

	////从左到右出图、左转，或者 从右到左出图、右转: 字头朝向反向



}

////////////////////////////////////////////////////////////////////////
//计算二线的曲线单个交点的内业断链
//输入：  iJD  该交点下标
//返回值：内业断链值，没有内业断链返回0.0
//编写：  李帅 2016.10.11
////////////////////////////////////////////////////////////////////////
double  RHA::CalCurvNYDL(int iJD) const
{
	/*JDarray[].NYDL;*/

	if(iJD<0 || iJD+1>JDarray.size())
		return 0.0;

	double ZHprojml,HZprojml;//二线上的ZH，HZ连续里程
	double ZHdml,HZdml;//二线上的ZH，HZ现场里程
	double len1,len2;
	ACHAR GH[20];

	ZHprojml = QXB[iJD].KZH ;
	HZprojml = QXB[iJD].KHZ;

	TrsProjmltoCkml(ZHprojml, 3, GH, ZHdml);
	TrsProjmltoCkml(HZprojml, 3, GH, HZdml);

	len1 = HZdml - ZHdml;
	len2 = HZprojml - ZHprojml;

	//4.内业断链
	if(fabs(len2-len1)>0.0000005)//有内业断链
		return len2 - len1 + 100.0;
	else
		return 0.0;
	
}


////////////////////////////////////////////////////////////////////////
//生成贯通设计二线与既有基线利用段
//编写：李帅 2016.9.27
//备注：给站场软件计算也提供了这个函数，修改时通知雷公 818
////////////////////////////////////////////////////////////////////////
void RHA::CreatLYDArrayOnJYX(RHA* pJYX,/*double spt[2], double ept[2], */vector<GJDInf>& LYDInfArray)
{
	int JYJDStart,JYJDEnd;
	bool IsQJZXCD,IsHJZXCD;//前后夹直线是否重叠
	int iQGJJD,iQJYJD;//前夹直线重叠的既有和改建交点下标
	int iHGJJD,iHJYJD;//后夹直线重叠的既有和改建交点下标
	GJDInf oneGjdInfo;
	ACHAR ckml[50];
	int NJD=JDarray.size();
	int dlNum=DLarray.size();
	double spt[2], ept[2];


	_tcscpy(ckml, L"");
	////nan
	//vector<GJDInf> gjdary;
	//gjdary.clear();

	JZXStruct B[7];
	B[2]=pJYX->JZXArray[2];
	B[3]=pJYX->JZXArray[3];
	B[4]=pJYX->JZXArray[4];



	if(pJYX->JDarray.size() == 0 || JDarray.size() == 0)
		return;

	//既有线的起终交点号
	JYJDStart=0;
	JYJDEnd=pJYX->JDarray.size()-1;

	//获得既有线起终点坐标
	spt[0] = pJYX->JDarray[0].N;
	spt[1] = pJYX->JDarray[0].E;
	ept[0] = pJYX->JDarray[JYJDEnd].N;
	ept[1] = pJYX->JDarray[JYJDEnd].E;

	//利用段信息赋初值
	_tcscpy(oneGjdInfo.XLName, m_XLName);
	oneGjdInfo.GJDType = -1;
	_tcscpy(oneGjdInfo.gjdname, L"利用段");

	bool isFindLYD=false;//是否发现利用段
	int sJYJD=-1, eJYJD=-1;//设计线与既有线重叠的起终交点
	int sSJJD=-1, eSJJD=-1;//设计线上利用段的起终交点
	double sProjml1x, eProjml1x;
	double sProjml2x, eProjml2x;
	double xjj=0.0;
	double distZDToJZXQD_GJ=0.0, distZDToJZXQD_JY=0.0;
	double distQDToJZXZD_GJ=0.0, distQDToJZXZD_JY=0.0;
	int sJD=0, eJD=0; 
	int sDL=-1, eDL=-1;

	typedef struct
	{
		int sJD, eJD;//起终交点
		int sDL, eDL;//起终断链
	}BXSec;
	LYDInfArray.clear();
	//1. 形成并行段数组
	vector<BXSec> BXSecArray;//绕行/并行起终交点数组，偶数为起点，基数为终点
	BXSec OneBX;
	OneBX.sJD = 1;//第一个并行段的起点
	OneBX.sDL = 0;//第一个并行段的起始断链
	int RXDNum = GetRXDNum();
	for (int iRXD=0; iRXD<RXDNum; iRXD++)
	{
		GetRxdSEJD(iRXD, sJD, eJD);//获取绕行起终交点
		GetiRXDDLinfo(iRXD, sDL, eDL);//获取绕行起终断链
		OneBX.eJD = sJD;//并行段终点是绕行段起点
		OneBX.eDL = sDL;//并行段终点断链是绕行段起点断链
		BXSecArray.push_back(OneBX);//绕行起点前一个交点是并行段终止交点

		OneBX.sJD = eJD+1;//下一个并行段的起点是当前绕行段的终点
		OneBX.sDL = eDL;//下一个并行段的起始断链是当前绕行段的终点断链
	}

	OneBX.eJD = NJD-1;//最后一个并行段的终点
	OneBX.eDL = DLarray.size()-1;//最后并行段的终点断链
	BXSecArray.push_back(OneBX);

	//2.并行段循环生成利用段
	for (int iBXD=0; iBXD<BXSecArray.size(); iBXD++)
	{
		sJD = BXSecArray[iBXD].sJD;
		eJD = BXSecArray[iBXD].eJD;
		sDL = BXSecArray[iBXD].sDL;
		eDL = BXSecArray[iBXD].eDL;
		if(eJD>=sJD)//全线绕行的情况可以在这里剔除
		{
			//在起终交点范围内寻找利用段
			//搜索与第2个交点的前夹直线重叠的既有线交点
			IsQJZXCD = IsHJZXCD = false;
			iQJYJD=0;
			IsQJZXCD = SearchJZXCDJDIndex_OnJYJX(sJD, -1, pJYX, JYJDStart, JYJDEnd, iQJYJD);
			
			isFindLYD=false;
			for (int j=sJD; j <= eJD; j++)
			{
				//搜索与iGJJD前夹直线重叠的既有线交点
				iHJYJD=0;
				if (j<eJD)
					IsHJZXCD = SearchJZXCDJDIndex_OnJYJX(j, 1, pJYX, JYJDStart, JYJDEnd, iHJYJD);
				else
					IsHJZXCD = false;

				if (IsQJZXCD && IsHJZXCD && iQJYJD == iHJYJD
					&& fabs(pJYX->JDarray[iQJYJD].R - JDarray[j].R)<0.001
					&& fabs(pJYX->JDarray[iQJYJD].l1 - JDarray[j].l1)<0.001
					&& fabs(pJYX->JDarray[iQJYJD].l2 - JDarray[j].l2)<0.001)
				{//曲线重叠
					if (!isFindLYD)
					{//开始发现利用段，给利用段起点交点赋值
						isFindLYD=true;
						sJYJD=iQJYJD;
						sSJJD=j;
					}

				}
				else
				{//如果曲线不重叠、或者已经搜到了改建线终点
					if (IsQJZXCD)//前夹直线上有利用部分
					{
						eJYJD=iQJYJD; 
						eSJJD=j;

						if (!isFindLYD)
						{
							//只有该夹直线被利用
							sJYJD=iQJYJD;
							sSJJD=j;
						}

						//生成改建段

						//起点
						distQDToJZXZD_GJ=sqrt(pow(JZXArray[sSJJD].QJZXStartPt.x-JZXArray[eSJJD].QJZXEndPt.x, 2)
							+pow(JZXArray[sSJJD].QJZXStartPt.y-JZXArray[eSJJD].QJZXEndPt.y, 2));
						distQDToJZXZD_JY=sqrt(pow(pJYX->JZXArray[sJYJD].QJZXStartPt.x-JZXArray[eSJJD].QJZXEndPt.x, 2)
							+pow(pJYX->JZXArray[sJYJD].QJZXStartPt.y-JZXArray[eSJJD].QJZXEndPt.y, 2));


						if (distQDToJZXZD_GJ>distQDToJZXZD_JY)//本线路夹直线比参照线的夹直线长
						{//取既有线夹直线起点

							//计算前夹直线重叠段的起点在设计线上的投影里程sProjml2x
							TrsNEtoProjml(pJYX->JZXArray[sJYJD].QJZXStartPt.x
								, pJYX->JZXArray[sJYJD].QJZXStartPt.y, sProjml2x);

							//如果起点在前一个断链前，那么取该断链做起点
							if (sProjml2x+0.001<DLarray[sDL].TYLC)
							{
								sProjml2x=DLarray[sDL].TYLC;
								//计算前夹直线重叠段的起点在既有线上投影sProjml1x
								CalXJJ(sProjml2x, array, tarray, pJYX->array, pJYX->tarray, sProjml1x, false);
							}
							else
								sProjml1x=pJYX->JZXArray[sJYJD].QJZXStartPt.lc;
						}
						else
						{//取改建线夹直线起点

							//前夹直线重叠段的起点在设计线上的投影里程
							sProjml2x=JZXArray[sSJJD].QJZXStartPt.lc;
							//如果起点在前一个断链前，那么取该断链做起点
							if (sProjml2x+0.001<DLarray[sDL].TYLC)
							{
								sProjml2x=DLarray[sDL].TYLC;
								//计算前夹直线重叠段的起点在既有线上投影sProjml1x
								CalXJJ(sProjml2x, array, tarray, pJYX->array, pJYX->tarray, sProjml1x, false);
							}
							else//计算前夹直线重叠段的起点在既有线上投影sProjml1x
								pJYX->TrsNEtoProjml(JZXArray[sSJJD].QJZXStartPt.x, JZXArray[sSJJD].QJZXStartPt.y, sProjml1x);

							////nan
							//ACHAR mes[256];
							//_stprintf(mes,L"既有线%lf,%lf\n", pGTZXJY->JZXArray[sJYJD].QJZXStartPt.y, pGTZXJY->JZXArray[sJYJD].QJZXStartPt.x);
							//ads_printf(mes);
							//_stprintf(mes,L"设计线%lf,%lf\n", OneGjdInfo.spt[1], OneGjdInfo.spt[0]);
							//ads_printf(mes);
						}

						//定位利用段起点里程

						if (fabs(sProjml1x-pJYX->QXB[0].KZH)>0.1)
						{
							//向后定位百米标
							sProjml1x = pJYX->LocateHundredMeterOnLine(sProjml1x, false, ckml);
							//既有线向设计线投影计算坐标、里程
							CalXJJ(sProjml1x, pJYX->array, pJYX->tarray
								, array, tarray, xjj, sProjml2x, oneGjdInfo.spt, true);//818 或者false 

							oneGjdInfo.BProjml=sProjml2x;
							_tcscpy(oneGjdInfo.BLC, ckml);
						}
						else
						{
							//线路起点开始利用
							oneGjdInfo.BProjml=QXB[0].KZH;
							_tcscpy(oneGjdInfo.BLC, LCchr(DLarray[0].EGH, DLarray[0].ELC, 3));
							oneGjdInfo.spt[0]=spt[0];
							oneGjdInfo.spt[1]=spt[1];
						}

						//终点

						distZDToJZXQD_GJ=sqrt(pow(JZXArray[eSJJD].QJZXEndPt.x-JZXArray[eSJJD].QJZXStartPt.x, 2)
							+pow(JZXArray[eSJJD].QJZXEndPt.y-JZXArray[eSJJD].QJZXStartPt.y, 2));

						distZDToJZXQD_JY=sqrt(pow(pJYX->JZXArray[eJYJD].QJZXEndPt.x-JZXArray[eSJJD].QJZXStartPt.x, 2)
							+pow(pJYX->JZXArray[eJYJD].QJZXEndPt.y-JZXArray[eSJJD].QJZXStartPt.y, 2));

						if (distZDToJZXQD_GJ>distZDToJZXQD_JY)
						{//取既有线夹直线终点

							//计算夹直线重叠段的终点在设计线上的投影里程eProjml2x
							TrsNEtoProjml(pJYX->JZXArray[eJYJD].QJZXEndPt.x
								, pJYX->JZXArray[eJYJD].QJZXEndPt.y, eProjml2x);

							//如果终点在后一个断链后，那么取该断链做终点
							if (eProjml2x>DLarray[eDL].TYLC+0.001)
							{
								eProjml2x=DLarray[eDL].TYLC;
								//计算夹直线重叠段的终点在既有线上投影eProjml1x
								CalXJJ(eProjml2x, array, tarray, pJYX->array, pJYX->tarray, eProjml1x, false);
							}
							else//计算后夹直线终点在既有线上投影eProjml1x
								eProjml1x = pJYX->JZXArray[eJYJD].QJZXEndPt.lc;
							//nan
							//ACHAR mes[256];
							//_stprintf(mes,L"设计线%lf,%lf\n", JZXArray[eSJJD].QJZXEndPt.y, JZXArray[eSJJD].QJZXEndPt.x);
							//ads_printf(mes);
							//_stprintf(mes,L"既有线%lf,%lf\n", OneGjdInfo.ept[1], OneGjdInfo.ept[0]);
							//ads_printf(mes);
						}
						else
						{//取改建线夹直线终点
							eProjml2x=JZXArray[eSJJD].QJZXEndPt.lc;

							if (eProjml2x>DLarray[eDL].TYLC+0.001)
							{
								eProjml2x=DLarray[eDL].TYLC;
								//计算夹直线重叠段的终点在既有线上投影eProjml1x
								CalXJJ(eProjml2x, array, tarray, pJYX->array, pJYX->tarray, eProjml1x, false);
							}
							else//计算后夹直线终点在既有线上投影eProjml1x
								pJYX->TrsNEtoProjml(JZXArray[eSJJD].QJZXEndPt.x
								, JZXArray[eSJJD].QJZXEndPt.y, eProjml1x);
						}

						//定位利用段终点里程
						if (fabs(eProjml1x-pJYX->QXB[JYJDEnd].KHZ)>0.1)
						{
							//向前定位百米标
							eProjml1x = pJYX->LocateHundredMeterOnLine(eProjml1x,true,ckml);
							//既有线向设计线投影算坐标、里程
							CalXJJ(eProjml1x, pJYX->array, pJYX->tarray
								, array, tarray, xjj, eProjml2x, oneGjdInfo.ept, true);//818 或者false 

							oneGjdInfo.EProjml=eProjml2x;
							_tcscpy(oneGjdInfo.ELC, ckml);
						}
						else
						{
							//到线路终点利用
							oneGjdInfo.EProjml=QXB[NJD-1].KHZ;
							_tcscpy(oneGjdInfo.ELC, LCchr(DLarray[dlNum-1].BGH, DLarray[dlNum-1].BLC, 3));
							oneGjdInfo.ept[0]=ept[0];
							oneGjdInfo.ept[1]=ept[1];
						}

						if (oneGjdInfo.EProjml<oneGjdInfo.BProjml+0.1)
							continue;


						//添加利用段数组
						_tcscpy(oneGjdInfo.BXLName , pJYX->m_XLName);
						_tcscpy(oneGjdInfo.EXLName , pJYX->m_XLName);
						LYDInfArray.push_back(oneGjdInfo);

						isFindLYD=false;
					}

				}

				IsQJZXCD=IsHJZXCD;
				iQJYJD=iHJYJD+1;
			}
		}
	}
	

	//合并改建段数组
	MergeGTZX2GJDArray(LYDInfArray); 

}

//给定统一里程，是否向前搜索，返回与附近断链处统一里程的差值，tylc>断链处lc...+
double RHA::DistToNearDL(double projml, const vector<DLTab> &DLAry, bool IsForwardTo) const//
{
	double LCB=0.0;//断链处统一里程
	int iDL=0;
	double iXLlength=0.0;//某段长度
	double cml;

	cml=TrsProjmltoCml(projml);
	
	LCB=0.0;
	for(iDL=1;iDL<DLAry.size();iDL++)
	{
		if(cml <= LCB)
			break;
		iXLlength=DLAry[iDL].BLC-DLAry[iDL-1].ELC;//某段长度
		LCB += iXLlength;//断链处统一里程
	}

	if(IsForwardTo)//向前搜索
		LCB-=iXLlength;

	return fabs(cml-LCB);	
}

/*===================================================================
函数名:FindNearDL   
------------------------------------------------------------
函数功能:寻找某一投影里程附近的断链下标,dirctn=true,向前找断链；=false，向后找
------------------------------------------------------------
输入:    
projml           投影里程
DLAry            断链
dirctn           =true,向前找断链；=false，向后找
------------------------------------------------------------
输出:  找到projml的断链区间，返回前/后一个断链的下标，否则返回-1
------------------------------------------------------------
编写: 李帅 2016.4.13  
=====================================================================*/
int RHA::FindNearDL(double projml, const vector<DLTab> &DLAry, bool dirctn) const
{
	int DLNum=DLAry.size();//818 如果就是传入断链值projml？
	if(DLNum==0)
		return -1;

	if (dirctn)//向前找 
	{
		for (int i=1; i<DLNum; i++)
		{
			if ((projml < DLAry[i].TYLC-0.001)
				&& (projml > DLAry[i-1].TYLC-0.001))
				return --i;
		}

		if (projml>DLAry[DLNum - 1].TYLC-0.001 /*&& projml<QXB[JDarray.size()-1].KZH+0.001*/)//818 满足小于QXB[JDarray.size()-1].KZH
			return --DLNum;//断链没有全部生成，该里程的前一断链是断链数组最后一个	

		return -1;
	}
	else//向后找
	{
		for (int i=DLNum-1; i>0; i--)
		{
			if ((projml > DLAry[i-1].TYLC-0.001) 
				&& (projml < DLAry[i].TYLC+0.001))
				return i;
		}

		return -1;
	}


}

/*************************************************************
函数功能: 寻找某统一里程到附近的断链的距离（dirctn=true,向前找断链，=false，向后找）
输入参数:
		cml         统一里程
		tmpDLB  	断链
		dirctn      =true,向前找断链；=false，向后找  
返回值: 找到cml的断链区间，返回前/后一个断链的下标，否则返回-1
调用：纵面设计时调用
编写：李帅 2017.2
***************************************************************/
int RHA::FindNearDLbyCml(double cml, const vector<DLTab> &tmpDLB, bool dirctn) const
{
	double LCB=0;  // LCB 断链的统一里程
	int iDL=0;
	int NDL=tmpDLB.size();
	int dlIndx=-1;

	for (int iDL=0;iDL<tmpDLB.size();iDL++)
	{
		//该断链点统一里程
		if(iDL > 0)
			LCB=LCB+tmpDLB[iDL].BLC-tmpDLB[iDL-1].ELC;
		else
			LCB=0.0;

		if (iDL == 0)
		{
			if (cml < LCB)
			{
				if(dirctn)
					dlIndx=-1;
				else
					dlIndx=0;
				break;
			}
			else if (fabs(cml-LCB)<0.00005)
			{
				if(dirctn)
					dlIndx=0;
				else
				{
					if (tmpDLB.size()>1)
						dlIndx=1;
					else
						dlIndx=-1;
				}
				break;
			}
		}
		else if (iDL < NDL)
		{
			if (cml < LCB)
			{
				if(dirctn)
					dlIndx=iDL-1;
				else
					dlIndx=iDL;
				break;
			}
			else if (fabs(cml-LCB)<0.00005)//818 cml在断链附近会不会有问题？
			{
				if(dirctn)
					dlIndx=iDL-1;
				else
						dlIndx=iDL;

				break;
			}
		}
	}

	if (iDL == tmpDLB.size())
	{
		if (dirctn)
			dlIndx=iDL;
		else
			dlIndx=-1;
	}

	return dlIndx;
}

//寻找距离某几何里程最近一个断链(跳过从参照线上复制过来的断链)
int RHA::GetPreRealDL(double projml) const
{
	for(int iDL=0; iDL<DLarray.size(); iDL++)
	{
		if (DLarray[iDL].Indx == -3)
			continue;

		if(projml+0.001<DLarray[iDL].TYLC)
			return iDL;
	}

	return 0;

}

//寻找某几何里程的前一个断链号
int RHA::GetPreDL(double projml) const//818 用DistToNearDL
{

	for(int iDL=0; iDL<DLarray.size(); iDL++)
	{
		if(projml+0.001<DLarray[iDL].TYLC)
			return --iDL;
	}

	return -1;
}

/*===================================================================
函数功能:在既有线上查找 和本线路某交点的夹直线重叠 的既有线夹直线对应的交点下标
------------------------------------------------------------
输入:
iJD2                   本线路的交点号
isQorH                 = -1，判断本线路的交点的前夹直线是否重叠
					   =  1，判断线路的交点的后夹直线是否重叠
pJYX                   既有线路平面指针
sJD_JY                 查找时的既有线起始交点号
eJD_JY                 查找时的既有线终止交点号
------------------------------------------------------------
输出:
iOverLapJD             既有线上与之重叠的交点下标
------------------------------------------------------------
返回值:如果在既有线上找到重叠的夹直线返回true，反之返回false
------------------------------------------------------------
编写: 
=====================================================================*/
bool RHA::SearchJZXCDJDIndex_OnJYJX(int iJD2,int isQorH,RHA* pJYX,int sJD_JY, int eJD_JY,int &iOverLapJD)
{
	bool isCD = false;//夹直线是否重叠
	double SPt1X, SPt1Y, EPt1X, EPt1Y;//一号线上夹直线起终点坐标
	double SPt2X, SPt2Y, EPt2X, EPt2Y;//二号线上夹直线起终点坐标
	int NJD=JDarray.size();
	double dist1;

	iOverLapJD=0;

	if (isQorH == -1)
	{//判断前夹直线是否重叠
		if (iJD2 == 0)
			return false;

		SPt2X = JZXArray[iJD2].QJZXStartPt.x;
		SPt2Y = JZXArray[iJD2].QJZXStartPt.y;
		EPt2X = JZXArray[iJD2].QJZXEndPt.x;
		EPt2Y = JZXArray[iJD2].QJZXEndPt.y;

		if (sJD_JY<1)
			sJD_JY=1;
		if (eJD_JY>pJYX->JDarray.size()-1)
			eJD_JY=pJYX->JDarray.size()-1;

		for (int i=sJD_JY ; i<=eJD_JY ; i++ )
		{
			SPt1X = pJYX->JZXArray[i].QJZXStartPt.x;
			SPt1Y = pJYX->JZXArray[i].QJZXStartPt.y;
			EPt1X = pJYX->JZXArray[i].QJZXEndPt.x;
			EPt1Y = pJYX->JZXArray[i].QJZXEndPt.y;
			//判断与既有线的该夹直线是否重叠
			isCD = IsZXCD(SPt1X, SPt1Y, EPt1X, EPt1Y, SPt2X, SPt2Y, EPt2X, EPt2Y);
			if (isCD)
			{
				iOverLapJD = i;
				isCD = true;
				return true;
			}
		}
	}

	else
	{
		//判断后夹直线是否重叠
		if (iJD2 == NJD-1)
			return false;

		SPt2X = JZXArray[iJD2].HJZXStartPt.x;
		SPt2Y = JZXArray[iJD2].HJZXStartPt.y;
		EPt2X = JZXArray[iJD2].HJZXEndPt.x;
		EPt2Y = JZXArray[iJD2].HJZXEndPt.y;

		if (sJD_JY<0)
			sJD_JY=0;
		if (eJD_JY+2>pJYX->JDarray.size())
			eJD_JY=pJYX->JDarray.size()-2;
		for (int i=sJD_JY ; i<=eJD_JY ; i++)
		{
			SPt1X = pJYX->JZXArray[i].HJZXStartPt.x;
			SPt1Y = pJYX->JZXArray[i].HJZXStartPt.y;
			EPt1X = pJYX->JZXArray[i].HJZXEndPt.x;
			EPt1Y = pJYX->JZXArray[i].HJZXEndPt.y;
			isCD = IsZXCD(SPt1X, SPt1Y, EPt1X, EPt1Y, SPt2X, SPt2Y, EPt2X, EPt2Y);
			if (isCD)
			{
				iOverLapJD = i;
				isCD = true;
				return true;
			}
		}
	}
	return false;
}

double  RHA::DistFromPtToGTZX(double x, double y)
{
	double dist,dx,dy;
	/*计算点(x,y)到GTZX起点的距离，如果距离小于1e-6返回0.0*/
	dx = JDarray[0].N - x;
	dy = JDarray[0].E - y;
	dist = sqrt(dx*dx+dy*dy);
	if (dist<1e-3)
		return 1e-12;
	/*计算点(x,y)到GTZX终点的距离，如果距离小于1e-6返回0.0*/
	dx = JDarray[JDarray.size()-1].N - x;
	dy = JDarray[JDarray.size()-1].E - y;
	dist = sqrt(dx*dx+dy*dy);
	if (dist<1e-3)
		return 1e-12;
	double cml =  PROJ_ML(array,tarray,x,y,x,y);
	xlpoint PZ;
	PZ.lc = cml;
	xlpoint_pz(array,tarray,PZ);
	dx = x-PZ.x;
	dy = y-PZ.y;
	dist = sqrt(dx*dx+dy*dy);
	return dist;
}

//合并改建段数组
void RHA::MergeGTZX2GJDArray(vector<GJDInf>& AllGJDArray) 
{	
	double xjj1 = 0.0;
	double xjj2 = 0.0;
	double projml,resPt[2];
	int ix=-1;

	for (int iGJD=0; iGJD+1<AllGJDArray.size(); iGJD++)
	{
		double sml,eml;
		eml = PROJ_ML(array,tarray,AllGJDArray[iGJD].ept[0],AllGJDArray[iGJD].ept[1]
			,AllGJDArray[iGJD].ept[0],AllGJDArray[iGJD].ept[1]);
		sml = PROJ_ML(array,tarray,AllGJDArray[iGJD+1].spt[0],AllGJDArray[iGJD+1].spt[1]
			,AllGJDArray[iGJD+1].spt[0],AllGJDArray[iGJD+1].spt[1]);

		if (fabs(sml-eml)<0.1)
		{
			//前后两改建段非常接近
			if (AllGJDArray[iGJD].GJDType == AllGJDArray[iGJD+1].GJDType)
			{
				//改建段非常接近且改建类型相同时，两改建段合并
				if (/*AllGJDArray[iGJD].GJDType == 0
					&& */_tcscmp(AllGJDArray[iGJD].EXLName, AllGJDArray[iGJD+1].BXLName) != 0)
					continue;

				AllGJDArray[iGJD].ept[0] = AllGJDArray[iGJD+1].ept[0];
				AllGJDArray[iGJD].ept[1] = AllGJDArray[iGJD+1].ept[1];
				AllGJDArray.erase(AllGJDArray.begin()+iGJD+1);
				iGJD--;
			}
		}
		else if (eml-sml>0.1)
		{
			//前后改建段出现交叉合并两改建段
			if (_tcscmp(AllGJDArray[iGJD].EXLName, AllGJDArray[iGJD+1].BXLName) == 0)
			{
				if (AllGJDArray[iGJD].GJDType != AllGJDArray[iGJD+1].GJDType)
				{
					CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, AllGJDArray[iGJD].EXLName, ix, false);
					if (ix >= 0)
					{
						CalXJJ(eml, array, tarray, g_DocManageAry[m_iDoc].XLAry[ix]->array, g_DocManageAry[m_iDoc].XLAry[ix]->tarray, xjj1, projml, resPt);
						CalXJJ(sml, array, tarray, g_DocManageAry[m_iDoc].XLAry[ix]->array, g_DocManageAry[m_iDoc].XLAry[ix]->tarray, xjj2, projml, resPt);

						if (fabs(xjj1-xjj2)<0.00001)
						{
							//两个点都在并行的夹直线边上，以绕行改建起终点为主，修正并行改建点/利用段点
							if (AllGJDArray[iGJD].GJDType > AllGJDArray[iGJD+1].GJDType)
							{
								AllGJDArray[iGJD+1].spt[0] = AllGJDArray[iGJD].ept[0];
								AllGJDArray[iGJD+1].spt[1] = AllGJDArray[iGJD].ept[1];
							}
							else
							{
								AllGJDArray[iGJD].ept[0] = AllGJDArray[iGJD+1].spt[0];
								AllGJDArray[iGJD].ept[1] = AllGJDArray[iGJD+1].spt[1];
							}
						}
						else//合并为绕行改建
						{
							AllGJDArray[iGJD].GJDType = 2;
							AllGJDArray[iGJD].ept[0] = AllGJDArray[iGJD+1].ept[0];
							AllGJDArray[iGJD].ept[1] = AllGJDArray[iGJD+1].ept[1];
							AllGJDArray.erase(AllGJDArray.begin()+iGJD+1);
							iGJD--;
						}

					}
					else
					{
						ads_printf(L"改建段的起终线路不存在！\n");
					}

					
				
				}
				else 
				{
					AllGJDArray[iGJD].ept[0] = AllGJDArray[iGJD+1].ept[0];
					AllGJDArray[iGJD].ept[1] = AllGJDArray[iGJD+1].ept[1];
					AllGJDArray.erase(AllGJDArray.begin()+iGJD+1);
					iGJD--;
				}
			
			}
		}
		

			
		
	}
}

/************************************************************************
函数功能：创建夹直线数组
以交点为单位，将该交点前后夹直线的起终点，交点号作为一个夹直线结构存入夹直线数组中
*************************************************************************/
void RHA::CreatJZXArray()
{
	JZXArray.RemoveAll();
	JZXStruct tempJZX;
	int NJD=JDarray.size();

	tempJZX.QJZXStartPt.lc = QXB[0].KHZ;
	xlpoint_pz(array,tarray,tempJZX.QJZXStartPt);
	tempJZX.QJZXEndPt.lc = QXB[0].KZH;
	xlpoint_pz(array,tarray,tempJZX.QJZXEndPt);
	tempJZX.HJZXStartPt.lc = QXB[0].KHZ;
	xlpoint_pz(array,tarray,tempJZX.HJZXStartPt);
	tempJZX.HJZXEndPt.lc = QXB[1].KZH;
	xlpoint_pz(array,tarray,tempJZX.HJZXEndPt);
	tempJZX.JDNum = 0;
	JZXArray.Add(tempJZX);

	for (int i=1 ; i<NJD-1 ; i++)
	{
		tempJZX.QJZXStartPt.lc = QXB[i-1].KHZ;
		xlpoint_pz(array,tarray,tempJZX.QJZXStartPt);
		tempJZX.QJZXEndPt.lc = QXB[i].KZH;
		xlpoint_pz(array,tarray,tempJZX.QJZXEndPt);
		tempJZX.HJZXStartPt.lc = QXB[i].KHZ;
		xlpoint_pz(array,tarray,tempJZX.HJZXStartPt);
		tempJZX.HJZXEndPt.lc = QXB[i+1].KZH;
		xlpoint_pz(array,tarray,tempJZX.HJZXEndPt);
		tempJZX.JDNum = i;                                                       
		JZXArray.Add(tempJZX);
	}

	tempJZX.QJZXStartPt.lc = QXB[NJD-2].KHZ;
	xlpoint_pz(array,tarray,tempJZX.QJZXStartPt);
	tempJZX.QJZXEndPt.lc = QXB[NJD-1].KZH;
	xlpoint_pz(array,tarray,tempJZX.QJZXEndPt);
	tempJZX.HJZXStartPt.lc = QXB[NJD-1].KHZ;
	xlpoint_pz(array,tarray,tempJZX.HJZXStartPt);
	tempJZX.HJZXEndPt.lc = QXB[NJD-1].KZH;
	xlpoint_pz(array,tarray,tempJZX.HJZXEndPt);
	tempJZX.JDNum = NJD-1;
	JZXArray.Add(tempJZX);

}

/*********************************************************************************
函数功能：创建绕行改建段数组
输入:
spt[2]		搜索绕行改建段的起点
ept[2]		搜索绕行改建段的终点
输出：
RXGJDArray	绕行改建段数组
*********************************************************************************/
void RHA::CreateRXGJD(/*double spt[2],double ept[2],*/vector<GJDInf>& RXGJDArray)
{
	RXGJDArray.clear();

	GJDInf OneGjdInfo;
	xlpoint PZ;
	int sDLno = 0, eDLno = 0;
	int RxdNum=0;
	double sProjml=0.0, eProjml=0.0;

	sProjml=QXB[0].KZH;
	eProjml=QXB[JDarray.size()-1].KHZ;

	RxdNum=GetRXDNum(JDarray);
	OneGjdInfo.GJDType = 2;
	_tcscpy(OneGjdInfo.XLName, m_XLName);

	for(int i=0;i<RxdNum;i++)
	{
		//查寻某绕行段的起终断链编号
		GetiRXDDLinfo(i , sDLno , eDLno, DLarray);
		if(max(DLarray[sDLno].TYLC,sProjml) < min(DLarray[eDLno].TYLC,eProjml))//绕行段与查询范围有交集
		{
			//起点
			PZ.lc = DLarray[sDLno].TYLC > sProjml ? DLarray[sDLno].TYLC : sProjml;
			xlpoint_pz(array, tarray, PZ);
			OneGjdInfo.spt[0] = PZ.x;
			OneGjdInfo.spt[1] = PZ.y;
			OneGjdInfo.BProjml=PZ.lc;

			//改建段名
			if (fabs(PZ.lc-DLarray[sDLno].TYLC)<0.1)
				_tcscpy(OneGjdInfo.gjdname, DLarray[sDLno].RXDName);
			else
				SetDefaultGjdName(DLarray[sDLno].BLName, OneGjdInfo.spt[0]
						, OneGjdInfo.spt[1], 2, OneGjdInfo.gjdname);

			//终点
			PZ.lc = DLarray[eDLno].TYLC < eProjml ? DLarray[eDLno].TYLC : eProjml;
			xlpoint_pz(array,tarray,PZ);
			OneGjdInfo.ept[0] = PZ.x;
			OneGjdInfo.ept[1] = PZ.y;
			OneGjdInfo.EProjml=PZ.lc;

			//参照线名
			_tcscpy(OneGjdInfo.BXLName,DLarray[sDLno].BLName);
			_tcscpy(OneGjdInfo.EXLName, DLarray[eDLno].BLName);

			//现场里程
			_tcscpy(OneGjdInfo.BLC, LCchr(DLarray[sDLno].EGH, DLarray[sDLno].ELC, 3));
			_tcscpy(OneGjdInfo.ELC, LCchr(DLarray[eDLno].BGH, DLarray[eDLno].BLC, 3));			

			RXGJDArray.push_back(OneGjdInfo);
		}
	}
}

/*********************************************************************************
函数功能：设置默认改建段名
输入:
blName     参照线路名
N, E       纬距，经距
GJDType    改建段类型
输出：
GjdName    改建段名
*********************************************************************************/
void RHA::SetDefaultGjdName(CString blName, double N, double E, int GJDType, ACHAR GjdName[256])
{
	double prjml = 0.0;
	ACHAR ckml[50], gh[16];
	double dml = 0.0;
	int ix=-1;
	RHA *pRHA_BL=NULL;
	ACHAR mes[100];

	_tcscpy(GjdName, L"");

	pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);
	if(pRHA_BL == NULL)
	{
		_stprintf(mes, L"生成改建段名时，参照线名%s输入错误\n", blName);
		ads_printf(mes);
		return;
		
	}

	//818 g_DocManageAry[m_iDoc].XLAry[ix]检查是否为空，不止这一处

	pRHA_BL->TrsNEtoCkml(N, E, ckml);
	split_ckml(ckml, dml, gh);
	if (GJDType==0)
		_stprintf(GjdName,L"%0.1lf并行改建",dml/1000.0);
	else if (GJDType==2)
		_stprintf(GjdName,L"%0.1lf绕行改建",dml/1000.0);
}

void RHA::SortSections(vector<GJDInf> &LYDArray)
{
	GJDInf temp;
	double cml1,cml2;
	for (int i=0;i<LYDArray.size();i++)
	{
		for (int j=i+1;j<LYDArray.size();j++)
		{
			TrsNEtoProjml(LYDArray[i].ept[0],LYDArray[i].ept[1],cml1);
			TrsNEtoProjml(LYDArray[j].ept[0],LYDArray[j].ept[1],cml2);
			if (cml1>cml2)
			{
				temp = LYDArray[i];
				LYDArray[i] = LYDArray[j];
				LYDArray[j] = temp;
			}
		}
	}

}

/****************************************************************************************************
函数功能：创建贯通设计基线改建段数组
输入参数：
spt[2]		搜索改建段的起点坐标
ept[2]		搜索改建段的终点坐标
AllGJDArray	在搜索范围内的改建段数组
LYDArray	在搜索范围内的利用段数组
*******************************************************************************************************/
void RHA::CreateGTJXGJDArray(/*double spt[2],double ept[2],*/vector<GJDInf>& AllGJDArray)
{
	double RXGJSml,RXGJEml;
	double sml,eml;
	double RXGJSpt[2],RXGJEpt[2];
	vector<GJDInf> LYDArray;
	vector<GJDInf> BXGJDArray,RXGJDArray,TmpBXGJDArray,TmpLYDArray;

	//sProjml = PROJ_ML(array,tarray,spt[0],spt[1],spt[0],spt[1]);
	//eProjml = PROJ_ML(array,tarray,ept[0],ept[1],ept[0],ept[1]);
	//if (eProjml-sProjml<0.1)
	//	return;

	//1 创建绕行段改建段
	CreateRXGJD(RXGJDArray);

	//nan 
	/*vector<GJDInf> tmp;
	tmp.clear();
	for (int i=0; i<RXGJDArray.GetSize(); i++)
	{
		tmp.push_back(RXGJDArray[i]);
	}*/


	//2 创建利用段
	int j;
	double cml;
	double LYDSml,LYDEml;
	GJDInf TmpGJD;
	xlpoint PZ;
	XLDataBase xlmdb;
	vector<GJDInf> tmpLYDArray;
	CArray<XLInforTableStruct, XLInforTableStruct> xlInforAry;//线路信息 
	//2.1 生成所有的既有线路

	xlmdb.ReadXLInfTable(m_mdbName, JYJX, xlInforAry);
	xlmdb.ReadXLInfTable(m_mdbName, JY2X, xlInforAry);

	//生成所有既有线
	for (int i=0; i<xlInforAry.GetSize(); i++)
	{
		CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, xlInforAry[i].XLName, false);
	}

	//2.2 创建利用段
	RHA *pRHA_BL;
	for (int i=0; i<xlInforAry.GetSize(); i++)
	{
		tmpLYDArray.clear();
		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, xlInforAry[i].XLName);
		if(pRHA_BL)
		{
			pRHA_BL->CreatJZXArray();
			CreatLYDArrayOnJYX(pRHA_BL, tmpLYDArray);
			for (int j=0; j<tmpLYDArray.size(); j++)
				LYDArray.push_back(tmpLYDArray[j]);
		}
	}

	//2.3 考虑前后并行参照两条线的情况修正利用段数组
	//     ，即考虑并行段断链  818 （尚未考虑）

	
	
	//2.4 排序
	for (int j=0; j<RXGJDArray.size(); j++)
		AllGJDArray.push_back(RXGJDArray[j]);

	for (int j=0; j<LYDArray.size(); j++)
		AllGJDArray.push_back(LYDArray[j]);

	SortSections(AllGJDArray);//818 merge

	//3 并行改建段
	CreatBXGJD(AllGJDArray, BXGJDArray);
	for (int j=0; j<BXGJDArray.size(); j++)
		AllGJDArray.push_back(BXGJDArray[j]);

	SortSections(AllGJDArray);//818 merge
	
	MergeGTZX2GJDArray(AllGJDArray);

}

/**********************************************************************************
函数功能：获取贯通设计基线上GJD_Spt到GJD_Ept的改建段信息数组
输入参数：
GJD_Spt[2]		搜索改建段的起点坐标
GJD_Ept[2]		搜索改建段的终点坐标
tmpBXDArray		该范围内的改建段数组
**********************************************************************************/
void RHA::CreatBXGJD(vector<GJDInf> &tmpLYDRXDArray, vector<GJDInf>& tmpBXDArray)
{
	double sProjml, eProjml;
	double GJDSml,GJDEml;
	GJDInf tmpGJD;
	double GJD_Spt[2], GJD_Ept[2];
	bool isAddQdGjd=false, isAddZdGjd=false;
	int LYDRXDNum=0;

	tmpBXDArray.clear();
	_tcscpy(tmpGJD.XLName, m_XLName);
	tmpGJD.GJDType=0;

	GJD_Spt[0] = JDarray[0].N;
	GJD_Spt[1] = JDarray[0].E;
	GJD_Ept[0] = JDarray[JDarray.size()-1].N;
	GJD_Ept[1] = JDarray[JDarray.size()-1].E;

	sProjml = QXB[0].KZH;
	eProjml = QXB[JDarray.size()-1].KHZ;

	//如果没有利用段和绕行改建段，全线并行改建
	if (tmpLYDRXDArray.size() == 0)
	{
		tmpGJD.BProjml=sProjml;
		tmpGJD.EProjml=eProjml;
		tmpGJD.spt[0]=GJD_Spt[0];
		tmpGJD.spt[1]=GJD_Spt[1];
		tmpGJD.ept[0]=GJD_Ept[0];
		tmpGJD.ept[1]=GJD_Ept[1];
		_tcscpy(tmpGJD.BLC, LCchr(DLarray[0].EGH, DLarray[0].ELC));
		_tcscpy(tmpGJD.ELC, LCchr(DLarray[DLarray.size()-1].BGH, DLarray[DLarray.size()-1].BLC));
		_tcscpy(tmpGJD.BXLName, JDarray[0].BLName);
		_tcscpy(tmpGJD.EXLName, JDarray[0].BLName);
		_tcscpy(tmpGJD.gjdname, L"");
		tmpBXDArray.push_back(tmpGJD);
		return;
	}

	//如果从起点开始并行改建，利用段绕行段数组头插一个只包含起点长度为0的改建段
	
	//计算第一个改建段的起点投影里程
	TrsNEtoProjml(tmpLYDRXDArray[0].spt[0], tmpLYDRXDArray[0].spt[1]
	, tmpLYDRXDArray[0].BProjml);

	if(tmpLYDRXDArray[0].BProjml>sProjml)
	{
		isAddQdGjd=true;

		tmpGJD.BProjml=tmpGJD.EProjml=sProjml;
		tmpGJD.spt[0]=tmpGJD.ept[0]=GJD_Spt[0];
		tmpGJD.spt[1]=tmpGJD.ept[1]=GJD_Spt[1];

		_tcscpy(tmpGJD.BLC, LCchr(DLarray[0].EGH, DLarray[0].ELC));
		_tcscpy(tmpGJD.ELC, tmpGJD.BLC);


		_tcscpy(tmpGJD.BXLName, JDarray[0].BLName);
		_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);
		_tcscpy(tmpGJD.gjdname, L"");

		tmpLYDRXDArray.insert(tmpLYDRXDArray.begin(), tmpGJD);
	}


	//如果终点之前是并行改建，利用段绕行段数组尾插一个只包含终点长度为0的改建段

	//计算最后一个改建段的终点投影里程
	LYDRXDNum=tmpLYDRXDArray.size();
	TrsNEtoProjml(tmpLYDRXDArray[LYDRXDNum-1].ept[0], tmpLYDRXDArray[LYDRXDNum-1].ept[1]
	, tmpLYDRXDArray[LYDRXDNum-1].EProjml);

	if(tmpLYDRXDArray[LYDRXDNum-1].EProjml+0.1<eProjml)
	{
		isAddZdGjd=true;
		tmpGJD.spt[0]=tmpGJD.ept[0]=GJD_Ept[0];
		tmpGJD.spt[1]=tmpGJD.ept[1]=GJD_Ept[1];
		tmpGJD.BProjml=tmpGJD.EProjml=eProjml;

		int dlNum=DLarray.size();
		_tcscpy(tmpGJD.BLC, LCchr(DLarray[dlNum-1].BGH, DLarray[dlNum-1].BLC));
		_tcscpy(tmpGJD.ELC, tmpGJD.BLC);

		_tcscpy(tmpGJD.BXLName,  L"");
		_tcscpy(tmpGJD.EXLName, L"");
		_tcscpy(tmpGJD.gjdname, L"");
		tmpLYDRXDArray.push_back(tmpGJD);
	}

	//利用绕行段间内插并行改建段
	LYDRXDNum=tmpLYDRXDArray.size();
	int iDL=0;
	int sDL=0, eDL=0;

	for (int i=0; i<LYDRXDNum-1; i++)
	{
		if(fabs(tmpLYDRXDArray[i].EProjml-tmpLYDRXDArray[i+1].BProjml)>0.1)
		{

			//起点信息
			tmpGJD.spt[0]=tmpLYDRXDArray[i].ept[0];
			tmpGJD.spt[1]=tmpLYDRXDArray[i].ept[1];
			tmpGJD.BProjml=tmpLYDRXDArray[i].EProjml;
			_tcscpy(tmpGJD.BXLName, tmpLYDRXDArray[i].EXLName);
			_tcscpy(tmpGJD.BLC,  tmpLYDRXDArray[i].ELC);
			//判断段落起点里程是否在断链上
			iDL=JudgePtIsOnDL(tmpGJD.BProjml);
			if (iDL >= 0)
			{
				_tcscpy(tmpGJD.BLC, LCchr(DLarray[iDL].EGH, DLarray[iDL].ELC));
				_tcscpy(tmpGJD.BXLName, DLarray[iDL].BLName);
				_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);
			}

			////查找该并行改建段区间是否有并行断链（前后分别参照两条线）
			//sDL=FindNearDL(tmpLYDRXDArray[i].EProjml, DLarray, true);
			//eDL=FindNearDL(tmpLYDRXDArray[i+1].BProjml, DLarray, true);

			//if(sDL<eDL)
			//{
			//	for (int j=sDL+1; j<eDL+1; j++)//818 未测试
			//	{
			//		if (DLarray[j].Indx<0)
			//		{
			//			//终点信息
			//			_tcscpy(tmpGJD.ELC, LCchr(DLarray[j].BGH, DLarray[j].BLC));
			//			tmpGJD.EProjml=DLarray[j].TYLC;
			//			TrsProjmltoNE(tmpGJD.EProjml, tmpGJD.ept[0], tmpGJD.ept[1]);
			//			_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);

			//			SetDefaultGjdName(tmpGJD.BXLName, tmpGJD.spt[0], tmpGJD.spt[1], 0, tmpGJD.gjdname);
			//			tmpBXDArray.Add(tmpGJD);

			//			//下一段起点信息
			//			tmpGJD.spt[0]=tmpGJD.ept[0];
			//			tmpGJD.spt[1]=tmpGJD.ept[1];
			//			tmpGJD.BProjml=DLarray[j].TYLC;
			//			_tcscpy(tmpGJD.BXLName, DLarray[j].BLName);
			//			_tcscpy(tmpGJD.BLC, LCchr(DLarray[j].EGH, DLarray[j].ELC));
			//		}
			//	}
			//}

			if(tmpGJD.BProjml+0.1<tmpLYDRXDArray[i+1].BProjml)
			{
				tmpGJD.ept[0]=tmpLYDRXDArray[i+1].spt[0];
				tmpGJD.ept[1]=tmpLYDRXDArray[i+1].spt[1];
				tmpGJD.EProjml=tmpLYDRXDArray[i+1].BProjml;

				//818 寻找前一断链下标，拷贝参照线名
				_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);
				_tcscpy(tmpGJD.ELC, tmpLYDRXDArray[i+1].BLC);

				//判断段落终点里程是否在断链上
				iDL=JudgePtIsOnDL(tmpGJD.EProjml);
				if (iDL >= 0)
					_tcscpy(tmpGJD.ELC, LCchr(DLarray[iDL].BGH, DLarray[iDL].BLC));

				SetDefaultGjdName(tmpGJD.BXLName, tmpGJD.spt[0], tmpGJD.spt[1], 0, tmpGJD.gjdname);
				tmpBXDArray.push_back(tmpGJD);
			}
			
		}
	}

	//删除之前增加的起终点改建段
	if(isAddQdGjd)
		tmpLYDRXDArray.erase(tmpLYDRXDArray.begin());
	if (isAddZdGjd)
		tmpLYDRXDArray.pop_back();
}

//绘线位
void RHA::DrawXL(AcGiWorldDraw *pWd)
{
	xlpoint PB,PE;
	double INDATA[5];
	int effectSJD, effectEJD;
	int NJD=JDarray.size();
	int tarray_pxy;


	effectSJD=0;
	effectEJD=NJD-1;

	if (IsMove && !IsMoveDone && MoveJDIndx>-1)
	{
		effectSJD=MoveJDIndx-2;
		effectEJD=MoveJDIndx+2;

		if (effectSJD<0)
			effectSJD=0;

		if(effectEJD > NJD-1)
			effectEJD=NJD-1;

	}

	tarray_pxy=effectEJD-effectSJD+1;

	PB.x=array_pxy[1]; //N
	PB.y=array_pxy[2]; // E
	PB.a=rad(array_pxy[3]);// FA
	PB.lc=array_pxy[4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (int i =1; i < tarray_pxy; i++) 
	{
		for (int k =0; k <4; k++) 	
		{
			for (int j=0; j <5; j++)
			{
				INDATA[j]=array_pxy[i*20+k*5+j];
			}

			G_WXY(pWd,INDATA,PB,PE,TRUE,T0min); 
			PB=PE;
		}
	}

}

//绘交点连线和交点圆
void RHA::DrawJDLineAndCir(AcGiWorldDraw *pWd)
{
	AcGeVector3d  normal(0.0,0.0,1.0);

	int effectSJD, effectEJD;
	int NJD=JDarray.size();
	int tarray_pxy;


	effectSJD=0;
	effectEJD=NJD-1;

	if (IsMove && !IsMoveDone && MoveJDIndx>-1)
	{
		effectSJD=MoveJDIndx-2;
		effectEJD=MoveJDIndx+2;

		if (effectSJD<0)
			effectSJD=0;

		if(effectEJD > NJD-1)
			effectEJD=NJD-1;

	}

	tarray_pxy=effectEJD-effectSJD+1;

	int i=0, num=0;
	AcGePoint3d Pt;
	AcGePoint3d *verts=new AcGePoint3d[tarray_pxy];
	// 绘交点连线
	num=effectSJD;
	for (i = 0; i < tarray_pxy; i++) 
	{
		verts[i].x=JDarray[num].E; 
		verts[i].y=JDarray[num].N;
		verts[i].z=0.0;
		num++;
	}

	if (pWd!= NULL) 
	{
		pWd->subEntityTraits().setSelectionMarker(1);
		pWd->subEntityTraits().setColor(GTZXJDToJDColor);	//临时
		pWd->geometry().polyline(tarray_pxy, verts);
	} 
	else
	{
		assert(Adesk::kFalse);
	}

	// 绘交点处圆
	for (i = 1; i < tarray_pxy-1; i++) //plh05.07
	{
		if(JDarray[i].JDXZ != BXPX && JDarray[i].JDXZ != BX_NotPX)
		{
			Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
			pWd->subEntityTraits().setColor(QXYSColor);
			pWd->geometry().circle(Pt,3.6,normal);
		}

		/////////////// 818 放开 
		/*if(!JDarray[i].IsShow)
		continue;
		Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
		mode->subEntityTraits().setColor(4);
		mode->geometry().circle(Pt,3.6,normal);*/
	}

}

//绘制一个废弃段标注 打“×”
void RHA::DrawOneFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const
{
	AcGePoint3d Pt1,Pt2,Pt3,Pt4;
	ACHAR layname[256];
	/*if (LMRFlag == MAINLINE)*/
		_tcscpy(layname , DrawParam.FQDBZ);
	/*else if (LMRFlag == LEFTLINE)
		_tcscpy(layname , DrawParam.LeftFQDBZ);
	else
		_tcscpy(layname , DrawParam.RightFQDBZ);*/

	Pt1.x = ECross + Size*sin(Fwj + pi/6.0);
	Pt1.y = NCross + Size*cos(Fwj + pi/6.0);
	Pt2.x = ECross + Size*sin(Fwj - 5.0*pi/6.0);
	Pt2.y = NCross + Size*cos(Fwj - 5.0*pi/6.0);
	makeline2(Pt1,Pt2,-1,AcDb::kLnWt030,layname);
	Pt3.x = ECross + Size*sin(Fwj - pi/6.0);
	Pt3.y = NCross + Size*cos(Fwj - pi/6.0);
	Pt4.x = ECross + Size*sin(Fwj + 5.0*pi/6.0);
	Pt4.y = NCross + Size*cos(Fwj + 5.0*pi/6.0);
	makeline2(Pt3,Pt4,-1,AcDb::kLnWt030,layname);
}

/************************************************************************
绘制废弃段标注“×”
StartLC		废弃段起始里程
EndLC		废弃段终止里程
JianJu		每隔多少距离绘制一个“×”
LMRFlag		线路类型标记
/************************************************************************/
void RHA::DrawFQDBZ(double StartLC, double EndLC, double JianJu, int LMRFlag) const
{
	AcGePoint3d Pt1,Pt2,Pt3,Pt4;
	xlpoint PZ;

	StartLC = max(StartLC, QXB[0].KZH);
	EndLC = min(EndLC, QXB[JDarray.size()-1].KHZ);
	
	//else if (LMRFlag == LEFTLINE)
	//{
	//	_tcscpy(layname , LeftXianLuZhongXian);
	//	StartLC = max(StartLC, sLCLx);
	//	EndLC = min(EndLC, eLCLx);
	//}
	//else
	//{
	//	_tcscpy(layname , RightXianLuZhongXian);
	//	StartLC = max(StartLC, sLCRx);
	//	EndLC = min(EndLC, eLCRx);
	

	if (StartLC>EndLC)
		return;

	if ((EndLC - StartLC) <= JianJu)
	{
		PZ.lc = 0.5 * (StartLC + EndLC);
		xlpoint_pz(array, tarray, PZ);

		DrawOneFQDBZ(PZ.x, PZ.y, PZ.a, texth, LMRFlag);
		return;
	}
	else
	{
		double lc = StartLC+JianJu;
		while (lc < EndLC)
		{
			PZ.lc = lc;
			xlpoint_pz(array, tarray, PZ);
			DrawOneFQDBZ(PZ.x, PZ.y, PZ.a, texth, LMRFlag);
			lc += JianJu;
		}
	}
}

//读取改建段汇总表，形成既有基线、既有二线的利用段数组 GJDINFARRAY
void RHA::CreatLYDArrayOfJYX() 
{
	XLDataBase xlmdb;
	CString tmpStr;
	//xlpoint PZ;	

	GJDInforArray.clear();
	tmpStr=L"改建性质=\"利用段\"";
	if(xlmdb.Read_XLDbs(m_mdbName,L"改建段汇总表",tmpStr))	
	{
		if(xlmdb.GJDAllInforNum>0)
		{
			for (int i=0;i<xlmdb.GJDAllInforNum;i++)
			{
				if(xlmdb.GJDAllInforTable[i].BXLName == m_XLName)
					GJDInforArray.push_back(xlmdb.GJDAllInforTable[i]);
			}
		}
	}

	//求投影里程
	int LYDNum=GJDInforArray.size();
	for (int i=0 ; i<LYDNum ; i++)
	{
		GJDInforArray[i].BProjml=PROJ_ML(array, tarray, GJDInforArray[i].spt[0], GJDInforArray[i].spt[1]
		, GJDInforArray[i].spt[0], GJDInforArray[i].spt[1]) ;
		GJDInforArray[i].EProjml=PROJ_ML(array, tarray, GJDInforArray[i].ept[0], GJDInforArray[i].ept[1]
		, GJDInforArray[i].ept[0], GJDInforArray[i].ept[1]) ;
	}

	//对利用段分段数组进行排序
	for (int i=0 ; i<LYDNum-1 ; i++)
	{
		for (int j=i+1; j<LYDNum ; j++)
		{
			if (GJDInforArray[i].BProjml > GJDInforArray[j].BProjml)
				swap(GJDInforArray[i].BProjml, GJDInforArray[j].BProjml);
		}
	}

	//对利用段分段数组进行合并
	for (int iGJD=0; iGJD + 1<GJDInforArray.size(); iGJD++)
	{
		if (fabs(GJDInforArray[iGJD+1].BProjml-GJDInforArray[iGJD].EProjml)<0.1
			|| GJDInforArray[iGJD+1].BProjml<GJDInforArray[iGJD].EProjml+0.001)
		{
			//前后两利用段非常接近或出现交叉
			GJDInforArray[iGJD].ept[0] = GJDInforArray[iGJD+1].ept[0];
			GJDInforArray[iGJD].ept[1] = GJDInforArray[iGJD+1].ept[1];
			GJDInforArray.erase(GJDInforArray.begin()+iGJD+1);
			iGJD--;
		}

	}

}
//绘制整段废弃段
void RHA::BZJYXFQD() const
{
	//	定义三个分断点数组，分别记录三条既有线上改建段起终点

	//vector<GJDInf> tmpGJDarray;
	//XLDataBase xlmdb;
	//CString tmpStr;
	////xlpoint PZ;	
	//double StartFQDLC,EndFQDLC;//废弃段起终连续里程


	//tmpGJDarray.RemoveAll();
	//tmpStr=L"改建性质=\"利用段\"";
	//if(xlmdb.Read_XLDbs(m_mdbName,L"改建段汇总表",tmpStr))	
	//{
	//	if(xlmdb.GJDAllInforNum>0)
	//	{
	//		for (int i=0;i<xlmdb.GJDAllInforNum;i++)
	//		{
	//			if(xlmdb.GJDAllInforTable[i].BXLName == m_XLName)
	//				tmpGJDarray.Add(xlmdb.GJDAllInforTable[i]);
	//		}
	//	}
	//}

	////求投影里程
	//int LYDNum=tmpGJDarray.GetSize();
	//for (int i=0 ; i<LYDNum ; i++)
	//{
	//	tmpGJDarray[i].BProjml=PROJ_ML(array, tarray, tmpGJDarray[i].spt[0], tmpGJDarray[i].spt[1]
	//								, tmpGJDarray[i].spt[0], tmpGJDarray[i].spt[1]) ;
	//	tmpGJDarray[i].EProjml=PROJ_ML(array, tarray, tmpGJDarray[i].ept[0], tmpGJDarray[i].ept[1]
	//								, tmpGJDarray[i].ept[0], tmpGJDarray[i].ept[1]) ;
	//}

	////对利用段分段数组进行排序
	//for (int i=0 ; i<LYDNum-1 ; i++)
	//{
	//	for (int j=i+1; j<LYDNum ; j++)
	//	{
	//		if (tmpGJDarray[i].BProjml > tmpGJDarray[j].BProjml)
	//			swap(tmpGJDarray[i].BProjml, tmpGJDarray[j].BProjml);
	//	}
	//}

	////对利用段分段数组进行合并
	//for (int iGJD=0; iGJD<tmpGJDarray.GetSize()-1; iGJD++)
	//{
	//	if (fabs(tmpGJDarray[iGJD+1].BProjml-tmpGJDarray[iGJD].EProjml)<0.1
	//		|| tmpGJDarray[iGJD+1].BProjml<tmpGJDarray[iGJD].EProjml+0.001)
	//	{
	//		//前后两利用段非常接近或出现交叉
	//		tmpGJDarray[iGJD].ept[0] = tmpGJDarray[iGJD+1].ept[0];
	//		tmpGJDarray[iGJD].ept[1] = tmpGJDarray[iGJD+1].ept[1];
	//		tmpGJDarray.RemoveAt(iGJD+1);
	//		iGJD--;
	//	}

	//}

	int LYDNum=GJDInforArray.size();
	//绘制废弃段
	if (LYDNum == 0)//818 改建段信息没生成？查找全局数组
	{
		//全线废弃
		DrawFQDBZ( QXB[0].KZH, QXB[JDarray.size()-1].KHZ, 400.0, RIGHTLINE);
	}
	else
	{
		//有利用部分
		//既有基线的废弃段标注
//818 定位百米标，在判断是否去找绕行断链上还有问题。即利用段多算
		if (GJDInforArray[0].BProjml > QXB[0].KZH + 0.01)//线路刚开始就是改建
			DrawFQDBZ(QXB[0].KZH, GJDInforArray[0].BProjml, 400.0, RIGHTLINE);
		if (GJDInforArray[LYDNum-1].EProjml + 0.001 < QXB[JDarray.size()-1].KHZ)//线路最后一段是改建
			DrawFQDBZ(GJDInforArray[LYDNum-1].EProjml,  QXB[JDarray.size()-1].KHZ, 400.0, RIGHTLINE);

		for (int i=1 ; i<LYDNum ; i++)
			DrawFQDBZ(GJDInforArray[i-1].EProjml, GJDInforArray[i].BProjml,400.0, RIGHTLINE);
	}

	
	
}




////将特征点桩号加入里程标
//void RHA::CalTZDtoLCB(double Scml, double Ecml, CArray<LCB,LCB> &LCBArray)
//{
//	LCB tmpLCB;
//	double projml;
//	ACHAR ckml[50];
//
//	if (Ecml<Scml+0.0001)
//	{
//		ads_printf(L"起始里程大于终止里程！\n");
//		return;
//	}
//
//	for(int i=0; i<JDarray.size(); i++)//对点数进行循环
//	{
//		for(int k=0;k<4;k++)
//		{
//
//			if(k==0)
//				projml=QXB[i].KZH;
//			else if(k==1)
//				projml=QXB[i].KHY;
//			else if(k==2)
//				projml=QXB[i].KHY;
//			else
//				projml=QXB[i].KHZ;
//
//			if (projml>=Scml && projml<=Ecml)
//			{
//				tmpLCB.Flag=k+5;
//				tmpLCB.Projml=projml;
//				TrsProjmltoCkml(projml, tmpLCB.CKml);
//				tmpLCB.Cml=CalnewcmlTocml(projml);
//				LCBArray.Add(tmpLCB);
//			}
//		}
//	}
//}

//判断一点是否在利用段上，在利用段上返回true
bool RHA::IsPtOnLYD(double projml) const
{
	if(GJDInforArray.size() == 0)
		return false;

	for (int i=0 ; i<GJDInforArray.size() ; i++)
	{
		if (projml>GJDInforArray[i].BProjml+0.001 
			&& projml+0.001<GJDInforArray[i].EProjml)
		{
			if (GJDInforArray[i].GJDType == -1)
				return true;
			else
				return false;
		}
		
	}

	return false;
}

Acad::ErrorStatus RHA::subErase(Adesk::Boolean erasing)
{
	if (XLShuXing != SGBX && XLShuXing != LSX)
	{//既有线或设计线
		int ix=-1;

		if (m_iDoc<g_DocManageAry.size() 
			&& CCreatPM::IsIncluded(m_mdbName, m_XLName, m_iDoc, ix) )
		{//从全局数组中删除
			//818 
			//如何处理屏幕上有两条既有基线的情况，全局数组
			if (id() == g_DocManageAry[m_iDoc].XLAry[ix]->id())
			{
				g_DocManageAry[m_iDoc].XLAry.erase(g_DocManageAry[m_iDoc].XLAry.begin()+ix);
				return Acad::eOk;
			}
		}

		ACHAR mes[60];
		_stprintf(mes, L"提示：系统删除%s数据失败\n", m_XLName);
		ads_printf(mes);
	}

	return Acad::eOk;
}

/*===================================================================
函数名:
------------------------------------------------------------
函数功能:根据里程数组内插出地面线高程，返回地面线数组
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
编写: 李帅 2016.10.24 修改 
=====================================================================*/
void RHA::PM_DMX(const CArray<LCB, LCB> &lcAry, double offst, vector<GMXTAB> &dmxAry)
{
	xlpoint PZ;
	double pt[2];//点坐标
	double hgt;//高程
	int Pos;
	int i=0;
	int lcNum=lcAry.GetSize();
	int newLcNum=0;//除去重复点后里程数组的个数
	ACHAR pszLabel[80];

	dmxAry.clear();
	dmxAry.resize(lcNum);

	_tcscpy(pszLabel,L"纵断面地面线内插");
	acedSetStatusBarProgressMeter(pszLabel,0,100);

	newLcNum=0;
	for (int i=0; i<lcNum; i++)
	{
		Pos = (int)(i*100.0/lcNum);
		acedSetStatusBarProgressMeterPos(Pos);

		//跳过重复里程
		if(i>0 && fabs(lcAry[i].Projml-lcAry[i-1].Projml)<0.001)
			continue;

		//内插地面线
		PZ.lc=lcAry[i].Projml;
		xlpoint_pz(array,tarray,PZ);  

		pt[0] = PZ.x +offst*cos(PZ.a+m_dHalfPI);
		pt[1] = PZ.y +offst*sin(PZ.a+m_dHalfPI);

		hgt=ptriDTM.interpolate(pt);

		//给地面线数组赋值
		dmxAry[newLcNum].Cml=lcAry[i].Cml;
		split_ckml(lcAry[i].CKml, dmxAry[newLcNum].ml, dmxAry[newLcNum].GH);
		dmxAry[newLcNum].H=hgt;
		newLcNum++;
	}

	//删除空数组
	if(newLcNum < lcNum)
		dmxAry.erase(dmxAry.begin()+newLcNum, dmxAry.end());

	acedSetStatusBarProgressMeterPos(100);    
	acedRestoreStatusBar();
}

/*===================================================================
函数名:EarseDLofBLonBXD
------------------------------------------------------------
函数功能:清除并行段上从参照线上拷贝过来的断链（断链序号为-3）
------------------------------------------------------------
输出:
DLarray               断链数组(类变量)
------------------------------------------------------------
算法: DLarray数组中indx == -3,即为参照线上拷贝过来的断链，删除该断链
------------------------------------------------------------
说明: 1 该函数在形成贯通断链表函数Create2XGTDLB（）之前使用
------------------------------------------------------------
编写: 李帅 2016.11.11  
=====================================================================*/
void RHA::EarseDLofBLonBXD()
{
	for(int i=0; i<DLarray.size(); i++)
	{
		if (DLarray[i].Indx == -3)
		{
			DLarray.erase(DLarray.begin()+i);
			i--;
		}
	}
}


/*===================================================================
函数名:Create2XGTDLB
------------------------------------------------------------
函数功能:生成二线贯通断链表
------------------------------------------------------------
输出:
DLarray               断链数组(类变量)
------------------------------------------------------------
编写: 李帅 2016.11.6  
------------------------------------------------------------
备注：
=====================================================================*/
void RHA::Create2XGTDLB()
{
	ACHAR sCkml[MaxCkmlStrNum];
	ACHAR eCkml[MaxCkmlStrNum];
	vector<DLTab> tmpDLAry;

	
	for (int i=1; i<DLarray.size(); i++)
	{
		
		if (DLarray[i].Indx == -3 || DLarray[i-1].Indx == -3)
			continue;

		//判断并行段断链区间
		if (DLarray[i].Indx != DLarray[i-1].Indx
			|| DLarray[i].Indx<0 || DLarray[i-1].Indx<0)
		{
			//并行段起终里程
			_tcscpy(sCkml, LCchr(DLarray[i-1].EGH, DLarray[i-1].ELC));
			_tcscpy(eCkml, LCchr(DLarray[i].BGH, DLarray[i].BLC));

			//提取并行段断链区间的参照线断链
			GetBasLineDLAry(DLarray[i-1].BLName, sCkml, eCkml, tmpDLAry);

			if (tmpDLAry.size()>0)
			{
				//倒着插入
				for(int j=tmpDLAry.size()-1; j >= 0; j--)
					DLarray.insert(DLarray.begin()+i, tmpDLAry[j]);

			}

			i=i+tmpDLAry.size();
		}

	}

	//合并相同位置的断链
	for (int iDL=1; iDL<DLarray.size(); iDL++)
	{
		//如果是之前设置的并行断链（Indx=-2）和参照线上拷贝的断链（Indx=-3）重复
		//，那么以并行断链为主，删除参照线上拷贝的断链
		if(DLarray[iDL-1].Indx == -2 && DLarray[iDL].Indx == -3)
		{
			if ((_tcscmp(DLarray[iDL-1].EGH, DLarray[iDL].BGH) == 0 
				&& fabs(DLarray[iDL-1].ELC-DLarray[iDL].BLC)<0.001)
				|| (_tcscmp(DLarray[iDL-1].EGH, DLarray[iDL].EGH) == 0 
				&& fabs(DLarray[iDL-1].ELC-DLarray[iDL].ELC)<0.001))
			{
				DLarray.erase(DLarray.begin()+iDL);
			}

		}
		else if (DLarray[iDL-1].Indx == -3 && DLarray[iDL].Indx == -2)
		{
			if ((_tcscmp(DLarray[iDL].BGH, DLarray[iDL-1].BGH) == 0 
				&& fabs(DLarray[iDL].BLC-DLarray[iDL-1].BLC)<0.001)
				|| (_tcscmp(DLarray[iDL].BGH, DLarray[iDL-1].EGH) == 0 
				&& fabs(DLarray[iDL].BLC-DLarray[iDL-1].ELC)<0.001))
			{
				DLarray.erase(DLarray.begin()+iDL-1);
			}

		}
	}
}

/*===================================================================
函数名:GetBasLineDLAry
------------------------------------------------------------
函数功能:获取基线上现场里程范围内的断链数组
------------------------------------------------------------
输入:
xlName              基线线路名
sCkml               基线的起始现场里程
eCkml               基线的终止现场里程
------------------------------------------------------------
输出:
dlAry               基线上现场里程范围内的断链数组
------------------------------------------------------------
编写: 李帅 2016.11.6  
------------------------------------------------------------
备注：1 只有既有线和设计线会调用该函数
      2 起终现场里程必须在基线的绕行段上
=====================================================================*/
void RHA::GetBasLineDLAry(CString xlName, ACHAR sCkml[MaxCkmlStrNum], ACHAR eCkml[MaxCkmlStrNum], vector<DLTab> &dlAry)
{
	double sProjml;//起点投影里程
	double eProjml;//终点投影里程
	RHA *pRHA_BL=NULL;
	DLTab tmpDL;
	CString str;

	//生成参照线
	pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, xlName);
	if(pRHA_BL == NULL)
	{
		ads_printf(L"错误：形成贯通断链表时，参照线路缺失\n");return;
	}

	//提取断链数组
	pRHA_BL->TrsCkmltoProjml(sCkml, sProjml);
	pRHA_BL->TrsCkmltoProjml(eCkml, eProjml);
	if (sProjml>eProjml)
	{
		str.Format(L"错误：形成贯通断链表时，起点现场里程%s在终点现场%s里程之后\n", sCkml, eCkml);
		ads_printf(str);return;
	}

	//if (pRHA_BL->GetiRxdByProjml(sProjml) == -1
	//	|| pRHA_BL->GetiRxdByProjml(eProjml) == -1)
	//{
	//	ads_printf(L"错误：基线上起终现场里程不在绕行段上\n");return;
	//}

	dlAry.clear();
	for (int iDL=0;iDL<pRHA_BL->DLarray.size();iDL++)
	{
		if (pRHA_BL->DLarray[iDL].TYLC > sProjml+0.0001
			&& pRHA_BL->DLarray[iDL].TYLC+0.0001<eProjml)
		{//在范围内
			dlAry.push_back(pRHA_BL->DLarray[iDL]);
		}

		if (pRHA_BL->DLarray[iDL].TYLC > eProjml+1)
			break;
	}	
	
	
	for (int iDL=0;iDL<dlAry.size();iDL++)
	{
		//投影里程默认-100
		//dlAry[iDL].TYLC=-100.0;
		TrsNEtoProjml(dlAry[iDL].N, dlAry[iDL].E, dlAry[iDL].TYLC);//李伟修改
		//修改断链编号 
		dlAry[iDL].Indx=-3;
	}
}



/*===================================================================
函数名:RoundMileage   
------------------------------------------------------------------------
函数功能:对某统一里程处的现场里程取整，返回取整后现场里程对应的统一里程
------------------------------------------------------------------------
输入:
cml                统一里程 
intPrec            取整精度
directn            方向，向前取整 =true，向后取整 =false
------------------------------------------------------------------------
返回值: 取整成功则返回取整后的统一里程cml，否则返回-100
------------------------------------------------------------------------
调用: 纵面设计时用
------------------------------------------------------------------------
编写: 李帅 2017.2
=====================================================================*/
double RHA::RoundMileage(double cml, int intPrec, bool directn) const
{	
	ACHAR GH[16]=L"";
	double dml=0;
	double intdml=0;
	double Intdist=0;
	double DLdist=0; 
	int dlIndx=0;//断链编号
	double dlCml=0;
	double newCml=0.0;


	//1 获取现场里程值（double）
	TrsCmltoCkml(cml, GH, dml);

	//2 取整的现场里程
	if (directn)
	{
		intdml=floor(dml/intPrec)*intPrec;//向前取整的现场里程
		dlIndx=FindNearDLbyCml(cml, DLarray, true);
	}
	else
	{
		intdml=ceil(dml/intPrec)*intPrec;//向后取整
		dlIndx=FindNearDLbyCml(cml, DLarray, false);
	}

	if(dlIndx<0)
		return -100;

	//计算断链点统一里程
	TrsCkmltoCml(DLarray[dlIndx].EGH, DLarray[dlIndx].ELC, dlCml);

	Intdist=fabs(dml-intdml);//取整后里程与取整前里程的距离
	DLdist=fabs(cml-dlCml);//前或后一个断链点到cml的距离

	//3 考虑取整的现场里程是否跨越断链
	if (Intdist < DLdist+0.001)//没有跨断链		
	{
		if (directn)
			_tcscpy(GH, DLarray[dlIndx].EGH);
		else
			_tcscpy(GH, DLarray[dlIndx].BGH);

		TrsCkmltoCml(GH, intdml, newCml);
		return newCml;
	}
	else//跨断链
	{
		//如果跨断链就取断链
		newCml=dlCml;
	}

	return -100;
}

//由连续里程获取改建段信息：改建段类型GJDType，起终参照线路名BXLName，EXLName
void RHA::GetPtGJDInfByCml(double Cml, int& GJDType, CString& BXLName, CString& EXLName)
{
	for (int i=0; i<GJDInforArray.size(); i++)
	{
		if (Cml>GJDInforArray[i].GJDSml && Cml<GJDInforArray[i].GJDEml)
		{
			GJDType = GJDInforArray[i].GJDType;
			BXLName = GJDInforArray[i].BXLName;
			EXLName = GJDInforArray[i].EXLName;
			return;
		}
	}
	GJDType = -1;
	BXLName = "既有基线";
	EXLName = "既有基线";
	return;
}

//由连续里程获取该里程所在的改建段类型
int RHA::GetGJDTypeByCml(double Cml)
{
	int GJDType=-1;
	for (int i=0; i<GJDInforArray.size(); i++)
	{
		if (Cml>GJDInforArray[i].GJDSml && Cml<GJDInforArray[i].GJDEml)
		{
			GJDType = GJDInforArray[i].GJDType;
			break;
		}
	}
	return GJDType;
}

int RHA::GetGJDTypeByProjml(double Projml)
{
	int GJDType=-1;
	for (int i=0; i<GJDInforArray.size(); i++)
	{
		if (Projml>GJDInforArray[i].BProjml && Projml<GJDInforArray[i].EProjml)
		{
			GJDType = GJDInforArray[i].GJDType;
			break;
		}
	}
	return GJDType;
}


//由连续里程获取改建段信息：改建段类型GJDType，起终参照线路名BXLName，EXLName
void RHA::GetPtGJDInfByCml(double N, double E, int& GJDType, CString& BXLName, CString& EXLName)
{
	double cml;
	TrsNEtoCml(N, E, cml);

	GetPtGJDInfByCml(cml, GJDType, BXLName, EXLName);
}




/*************************************************************
函数名称:  
函数功能: 判断某现场里程在绕行段还是并行段上
输入参数：
ckml     			现场里程
m_XLName 				线路名
m_mdbName             数据库名
输出参数：
blName 				在绕行段上，输出本线路名；如果在并行段上，输出参照线路名/依赖平面名
rxdName             在绕行段上，输出本线路绕行段名；如果在并行段上，输出参照线路/依赖平面的绕行段名
函数返回值:         在并行段上，返回true;在绕行段上，返回false;
注意: 对于设计二线，是用纵面的绕行段数据来判断
编写：李帅 2017.3
***************************************************************/
bool RHA::JudgeMLonBXDorRXD(ACHAR ckml[], CString &blName, CString &rxdName)
{
	blName=m_XLName;
	rxdName=L"";

	//既有基线、施工便线没有并行段，设计基线纵面数据是全线的没有绕行段
	if (m_XLName == JYJX || m_XLName == SJJX || XLShuXing == SGBX || XLShuXing == LSX)
		return false;

	double cml;
	double projml;
	bool isOnBXorRX=false;


	if (m_XLName.Find(L"既有") >= 0)
	{
		//既有二线
		int iRxd=0;
		int iDL;
		//计算投影里程
		TrsCkmltoProjml(ckml, projml);
		iRxd=GetiRxdByProjml(projml);
		if (iRxd >=0 )
		{
			//在本线路绕行段上
			rxdName=GetRxdName(iRxd);
		}
		else
		{
			//在并行段上
			isOnBXorRX=true;

			RHA *pBL_RHA;
			double blProjml;
			iDL=FindNearDL(projml, DLarray, true);
			blName=DLarray[iDL].BLName;
			if (blName == JYJX)
				return isOnBXorRX;

			pBL_RHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);
			if(pBL_RHA)
			{
				pBL_RHA->TrsCkmltoProjml(ckml, blProjml);
				iRxd=pBL_RHA->GetiRxdByProjml(blProjml);
				if (iRxd >= 0)
					rxdName=pBL_RHA->GetRxdName(iRxd);
				else
					ads_printf(L"并行段的里程输入错误！\n");
			}

		}

	}
	else if (m_XLName.Find(L"设计") >= 0)
	{
		//设计二线
		//计算统一里程
		TrsCkmltoCml(ckml, cml);
		//判断是否在绕行段内
		for (int i=0; i<FZZDMAry.size(); i++)
		{
			if (cml>FZZDMAry[i].BCml+0.001 
				&& cml+0.001<FZZDMAry[i].ECml)
			{
				//在绕行段内
				rxdName=FZZDMAry[i].DLName;
				return false;
			}
		}

		//在设计基线上
		m_XLName=SJJX;
		isOnBXorRX=true;
	}

	return isOnBXorRX;
}

//由基线连续里程判断是否在绕行段上，返回绕行段编号，在并行段返回-1
int RHA::GetiRxdNoFromCml_1X(double Cml_1X)
{
	for (int i=0; i<FZZDMAry.size(); i++)
	{
		if (i==0 && FZZDMAry[i].BL_BCml<0)
		{
			//起点绕行
			if(Cml_1X<FZZDMAry[i].BL_ECml)
				return i;
		}
		else if (i+1 == FZZDMAry.size() && FZZDMAry[i].BL_ECml<0)
		{
			//终点绕行
			if(Cml_1X>FZZDMAry[i].BL_BCml)
				return i;
		}
		else if (Cml_1X>FZZDMAry[i].BL_BCml && Cml_1X<FZZDMAry[i].BL_ECml)
		{
			return i;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////李伟
//void RHA::INSERT_TUNNEL(CString XLName,CString qdxclc,CString zdxclc,CString NamTun,int LineNum,double style,int LMRFlag,int iRXD)
//{
//	ACHAR layname[256];
//	int zybz = -1;//隧道注字左右标志，默认在左边
//	double offset = 0.0;
//	if(LMRFlag==MAINLINE)
//	{
//		if (XLName == L"既有基线")
//			_stprintf(layname,L"%s",Tunnel);
//		else
//			_stprintf(layname,L"%s",GJTunnel);
//		if (HasLeft)
//			zybz = 1;
//		else
//			zybz = -1;
//		if (XLName == L"既有基线")
//			zybz = 1;
//	}
//	else if(LMRFlag==LEFTLINE)
//	{
//		if (XLName == L"既有左线")
//			_stprintf(layname,L"%s",LeftTunnel);
//		else
//			_stprintf(layname,L"%s",GJLeftTunnel);
//		zybz = -1;
//		if (SCALE>9.9)
//			offset = m_ExOffset;
//	}
//	else 
//	{
//		if (XLName == L"既有右线")
//			_stprintf(layname,L"%s",RightTunnel);
//		else
//			_stprintf(layname,L"%s",GJRightTunnel);
//		zybz = 1;
//		if (SCALE>9.9)
//			offset = m_ExOffset;
//	}
//	/////////////////////////////////////////////////////////////
//	AcDbObjectIdArray IdAr;
//	ACHAR xlcb[80],xlce[80];//现场里程
//	double xclcb,xclce;//现场里程
//	ACHAR GH[20],LZB[20],LZE[20];//起终端零桩
//
//	//	acutPrintf(L"bri:%lf,len=%lf\n",lc0,lenBri);
//	int k=0;
//
//	////////////////////////////////////////////////////////////
//	//起终标注里程
//	double lcb=0.0;//起点统一里程
//	double lce=0.0;//终点统一里程
//
//	_tcscpy(xlcb,qdxclc);
//	_tcscpy(xlce,zdxclc);
//	split_ckml(xlcb,xclcb,GH);
//	split_ckml(xlce,xclce,GH);
//
//	if(LMRFlag==MAINLINE)
//	{
//		lcb=TYLC1x(xlcb);
//		lce=TYLC1x(xlce);
//	}		
//	else if(LMRFlag==LEFTLINE)
//	{
//		if (iRXD>-1)
//		{
//			lcb=TYLCLx(xlcb,iRXD);
//			lce=TYLCLx(xlce,iRXD);
//		}
//		else
//		{
//			xlpoint PZTemp;
//			if (XLName==L"既有左线")
//			{
//				pJYZX->GetPZFromXCLC2X(qdxclc,PZTemp);
//				lcb = PZTemp.lc;
//				pJYZX->GetPZFromXCLC2X(zdxclc,PZTemp);
//				lce = PZTemp.lc;
//			}
//			else
//			{
//				pGTSJEX->GetPZFromXCLC2X(qdxclc,PZTemp);
//				lcb = PZTemp.lc;
//				pGTSJEX->GetPZFromXCLC2X(zdxclc,PZTemp);
//				lce = PZTemp.lc;
//			}
//		}
//	}	
//	else
//	{
//		if (iRXD>-1)
//		{
//			lcb=TYLCRx(xlcb,iRXD);
//			lce=TYLCRx(xlce,iRXD);
//		}
//		else
//		{
//			xlpoint PZTemp;
//			if (XLName==L"既有右线")
//			{
//				pJYYX->GetPZFromXCLC2X(qdxclc,PZTemp);
//				lcb = PZTemp.lc;
//				pJYYX->GetPZFromXCLC2X(zdxclc,PZTemp);
//				lce = PZTemp.lc;
//			}
//			else
//			{
//				pGTSJEX->GetPZFromXCLC2X(qdxclc,PZTemp);
//				lcb = PZTemp.lc;
//				pGTSJEX->GetPZFromXCLC2X(zdxclc,PZTemp);
//				lce = PZTemp.lc;
//			}
//		}
//	}	
//
//	if (lce-lcb < m_NoteMinTunLen-0.001)
//		return;
//
//	xclcb=xclcb-int(xclcb/1000.0+0.00001)*1000.0+1000.0;//1000.235
//	acdbRToS(xclcb,2,NCQ,LZB);
//	LZB[0]='+';
//
//	xclce=xclce-int(xclce/1000.0+0.00001)*1000.0+1000.0;//1000.235
//	acdbRToS(xclce,2,NCQ,LZE);
//	LZE[0]='+';
//
//	/////////////////////////////////////////////////////////////////
//	double lenTunnel=lce-lcb;//隧道长
//	double lc0=0.5*(lcb+lce);//中心统一里程
//	//////////////////////////////////////////////////////
//	//是否在出图范围内 818
//	/*if(LMRFlag==MAINLINE)
//	{
//		if(lc0<sLC1x||lc0>eLC1x)return;
//	}
//	else if(LMRFlag==LEFTLINE)
//	{
//		if(lc0<sLCLx||lc0>eLCLx)return;
//	}
//	else
//	{
//		if(lc0<sLCRx||lc0>eLCRx)return;
//	}*/
//
//	////////////////////////////////////////////
//	//左右宽
//	double dL=1.5*SCALE;//单线　隧道左侧宽
//	double dR=1.5*SCALE;//单线　隧道右侧宽
//
//	double lc2x;double pt2x[2];double xjj;
//	
//
//	//根据输入参数LineNum确定是单线还是双线
//	if(HasLeft && LineNum==2)//存在左线
//	{
//		CalXjjLx(lc0,xjj,lc2x,pt2x);
//		xjj=fabs(xjj);
//		dL+=xjj;
//		if (SCALE>9.9)
//			dL += m_ExOffset;
//	}
//	if(HasRight && LineNum==2)//存在右线
//	{
//		CalXjjRx(lc0,xjj,lc2x,pt2x);
//		xjj=fabs(xjj);
//		dR+=xjj;
//		if (SCALE>9.9)
//			dR += m_ExOffset;
//	}		
//	/////////////////////////////////////////////////////////////////////	
//	double a0,ab,ae;//线路方位角/隧道中线方位角,mid,begin,end//可能为曲线隧道
//	xlpoint Ptmp;
//
//	Ptmp.lc=lc0;
//	k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//	if (offset>0.001)
//	{
//		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//	}
//	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//隧道中心坐标（如果双线位于主线上）
//	a0=Ptmp.a;
//
//	Ptmp.lc=lcb;
//	k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//	if (offset>0.001)
//	{
//		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//	}
//	AcGePoint3d Ptb(Ptmp.y,Ptmp.x,0.0);//隧道中心坐标（主线）
//	ab=Ptmp.a;
//
//	Ptmp.lc=lce;
//	k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//	if (offset>0.001)
//	{
//		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//	}
//	AcGePoint3d Pte(Ptmp.y,Ptmp.x,0.0);//隧道中心坐标（主线）
//	ae=Ptmp.a;
//	///////////////////////////////////////////////////////////////
//	//端点
//	double aLb=ab-0.5*pi;//隧道前端左侧线方位角
//	double aRb=ab+0.5*pi;//隧道前端右侧线方位角
//
//	double aLe=ae-0.5*pi;//隧道终端左侧线方位角
//	double aRe=ae+0.5*pi;//隧道终端右侧线方位角
//
//	double a_LT=ab-0.75*pi;//四角45度向外延伸线方位角
//	double a_LB=ab+0.75*pi;
//	double a_RT=ae-0.25*pi;
//	double a_RB=ae+0.25*pi;
//
//	AcGePoint3d LT,LB,RT,RB,T0,B0;//隧道框四角++两侧中点
//	AcGePoint3d LTe,LBe,RTe,RBe;//隧道框四角45度向外延伸
//	LT.z=0.0;RT.z=0.0;LB.z=0.0;RB.z=0.0;
//	LTe.z=0.0;RTe.z=0.0;LBe.z=0.0;RBe.z=0.0;
//	T0.z=0.0;B0.z=0.0;
//
//	//	acutPrintf(L"0:x=%lf,y=%lf,l=%lf,fwj=%lf\n",Ptb.x,Ptb.y,dL,aLb);
//	getLineEndPt(Ptb,dL,aLb,LT);
//	//	acutPrintf(L"LT:x=%lf,y=%lf\n",LT.x,LT.y);
//
//	getLineEndPt(Ptb,dR,aRb,LB);
//	getLineEndPt(Pte,dL,aLe,RT);
//	getLineEndPt(Pte,dR,aRe,RB);
//
//	getLineEndPt(LT,1.5*SCALE,a_LT,LTe);
//	getLineEndPt(LB,1.5*SCALE,a_LB,LBe);
//	getLineEndPt(RT,1.5*SCALE,a_RT,RTe);
//	getLineEndPt(RB,1.5*SCALE,a_RB,RBe);
//	//////////////////////////////////////////////////////////////
//	//两侧边界，可能为曲线隧道
//	//	double lw=0.5*SCALE;
//	double lw = LW;
//	if(style<0.9)//明洞
//	{
//		AcGePoint2dArray TPts,BPts;
//		TPts.append(AcGePoint2d(LT.x,LT.y));
//		BPts.append(AcGePoint2d(LB.x,LB.y));
//		int num=int(lenTunnel/5.0+1+0.00001);
//		double dLen=lenTunnel/(num-1);
//		for(int i=1;i<num-1;i++)
//		{			
//			Ptmp.lc=lcb+i*dLen;
//			k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//			if (offset>0.001)
//			{
//				Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//				Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//			}
//			double ai=Ptmp.a;
//			AcGePoint3d iPt(Ptmp.y,Ptmp.x,0.0);//主线上坐标
//			AcGePoint3d iTPt;
//			getLineEndPt(iPt,dL,ai-0.5*pi,iTPt);
//			AcGePoint3d iBPt;
//			getLineEndPt(iPt,dR,ai+0.5*pi,iBPt);
//			TPts.append(AcGePoint2d(iTPt.x,iTPt.y));
//			BPts.append(AcGePoint2d(iBPt.x,iBPt.y));
//		}
//		TPts.append(AcGePoint2d(RT.x,RT.y));
//		BPts.append(AcGePoint2d(RB.x,RB.y));
//
//		makepolyline(TPts,-1,lw,layname);
//		makepolyline(BPts,-1,lw,layname);
//		TPts.removeSubArray(0,TPts.length()-1);
//		BPts.removeSubArray(0,BPts.length()-1);
//	}
//	else//一般隧道
//	{
//		makeline2(LT,LTe,-1,lw,layname);
//		makeline2(LB,LBe,-1,lw,layname);
//		makeline2(RT,RTe,-1,lw,layname);
//		makeline2(RB,RBe,-1,lw,layname);
//	}
//
//	makeline2(LT,LB,-1,lw,layname);
//	makeline2(RT,RB,-1,lw,layname);
//
//	//////////////////标注///////////////////////////////
//	AcGePoint3d eBZPt;//标注线的终点，起点为Pt0;
//	AcGePoint3d Pbz1,Pbz2;//标注点;
//	ACHAR lentunstr[20];
//	acdbRToS(lenTunnel,2,NLC,lentunstr);
//	NamTun+=L"  ";//隧道名称与隧道长之间加两个空格
//	//	if(NJD2x>0&&iRXD>=0)NamTun+=L"2(L";//???????????????
//	NamTun+=lentunstr;
//	NamTun+=L"m";
//	//if(NJD2x>0&&iRXD>=0)NamTun+=L")";//???????????????
//	ACHAR TUNFlag[256];
//	_tcscpy(TUNFlag,NamTun);
//
//	int MaxStr=_tcslen(TUNFlag);
//	double WB=dL;
//	if (m_IsAutoLorR && pGTSJEX)
//	{
//		zybz = CalInsFlagLorR(Pt0.y,Pt0.x,XLName);
//	}
//	//	if(draw_zybz>0)WB=dR;
//	if (zybz>0) WB=dR;
//	//	acutPrintf(L"Max:%d\n",MaxStr);
//	//	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*draw_zybz);//+0.5*texth*sin(B);
//	//	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*draw_zybz);//+0.5*texth*cos(B);
//	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*zybz);//+0.5*texth*sin(B);
//	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*zybz);//+0.5*texth*cos(B);
//	makeline2(Pt0,eBZPt,-1,AcDb::kLnWt035,layname);//中心标注线
//
//	xlpoint PZtmp;
//	double B;
//	PZtmp.lc=lc0-(0.5*texth+1.0*SCALE);
//	k = AllXLPointPZ(XLName, PZtmp, iRXD, LMRFlag);
//	B=PZtmp.a;
//	//	Pbz1[X]=PZtmp.y+(WB+0.5*SCALE+0.5*MaxStr*texth)*sin(B+pi*0.5*draw_zybz) ;
//	//	Pbz1[Y]=PZtmp.x+(WB+0.5*SCALE+0.5*MaxStr*texth)*cos(B+pi*0.5*draw_zybz) ;
//	Pbz1[X]=PZtmp.y+(WB+0.5*SCALE+0.5*MaxStr*texth+offset)*sin(B+pi*0.5*zybz) ;
//	Pbz1[Y]=PZtmp.x+(WB+0.5*SCALE+0.5*MaxStr*texth+offset)*cos(B+pi*0.5*zybz) ;
//	PZtmp.lc=lc0+(0.5*texth+1.0*SCALE);
//	k = AllXLPointPZ(XLName, PZtmp, iRXD, LMRFlag);
//	B=PZtmp.a;
//	//	Pbz2[X]=PZtmp.y+(WB+0.5*SCALE+0.5*MaxStr*texth)*sin(B+pi*0.5*draw_zybz) ;
//	//	Pbz2[Y]=PZtmp.x+(WB+0.5*SCALE+0.5*MaxStr*texth)*cos(B+pi*0.5*draw_zybz) ;
//
//	Pbz2[X]=PZtmp.y+(WB+0.5*SCALE+0.5*MaxStr*texth+offset)*sin(B+pi*0.5*zybz) ;
//	Pbz2[Y]=PZtmp.x+(WB+0.5*SCALE+0.5*MaxStr*texth+offset)*cos(B+pi*0.5*zybz) ;
//	//double b=B+pi*0.5;
//	//b=pi*0.5-b; if (draw_zybz==-1) b=b+pi;
//	//if (b<0.0) b=2*pi+b;
//	//if (b>2*pi) b=b-2*pi;		         
//	double b=B-pi*0.5;
//	b=pi*0.5-b;        
//	AdjustAngle(b);
//	//	if(ChuTu_zybz == 1) Pbz1=Pbz2; //Lw2007-11-05注释
//	maketext(Pbz1,TUNFlag,b,texth,-1,3,L"宋体",layname); //标隧道名称及长度    
//
//	int istrb=0,istre=0;
//	istrb=_tcslen(LZB);
//	istre=_tcslen(LZE);
//	AcGePoint3d PtLzB0,PtLzB1,PtLzE0,PtLzE1;//起终点标注线的始终端
//	AcGePoint3d BzPosb,BzPose;
//
//	WB=dL+0.5*SCALE;//保证起始里程标注线端点与隧道有0.5mm的距离
//	//if(draw_zybz>0)WB=dR+0.5*SCALE;
//	//PtLzB0[X]=Ptb.x+WB*sin(ab+pi*0.5*draw_zybz);
//	//PtLzB0[Y]=Ptb.y+WB*cos(ab+pi*0.5*draw_zybz);
//	//PtLzE0[X]=Pte.x+WB*sin(ae+pi*0.5*draw_zybz);
//	//PtLzE0[Y]=Pte.y+WB*cos(ae+pi*0.5*draw_zybz);
//
//	if(zybz>0)WB=dR+0.5*SCALE;
//	PtLzB0[X]=Ptb.x+WB*sin(ab+pi*0.5*zybz);
//	PtLzB0[Y]=Ptb.y+WB*cos(ab+pi*0.5*zybz);
//	PtLzE0[X]=Pte.x+WB*sin(ae+pi*0.5*zybz);
//	PtLzE0[Y]=Pte.y+WB*cos(ae+pi*0.5*zybz);
//
//	WB=dL+0.5*SCALE+0.6*istrb*texth;//保证终点里程标注线端点与隧道有0.5mm的距离
//	//	if(draw_zybz>0)WB=dR+0.5*SCALE+0.6*istrb*texth;
//	if(zybz>0)WB=dR+0.5*SCALE+0.6*istrb*texth;
//	//	PZtmp.lc=lcb+draw_zybz*(0.5*texth+1.0*SCALE);
//	//	PZtmp.lc=lcb+ChuTu_zybz*(0.5*texth+1.0*SCALE);
//	PZtmp.lc=lcb-(0.5*texth+1.0*SCALE);//Lw2007-11-05
//	k = AllXLPointPZ(XLName, PZtmp, iRXD, LMRFlag);
//	B=PZtmp.a;
//	//	BzPosb[X]=PZtmp.y+WB*sin(B+pi*0.5*draw_zybz) ;
//	//	BzPosb[Y]=PZtmp.x+WB*cos(B+pi*0.5*draw_zybz) ;
//	BzPosb[X]=PZtmp.y+(WB+offset)*sin(B+pi*0.5*zybz) ;
//	BzPosb[Y]=PZtmp.x+(WB+offset)*cos(B+pi*0.5*zybz) ;
//	//b=B+pi*0.5;
//	//b=pi*0.5-b; if (draw_zybz==-1) b=b+pi;
//	//if (b<0.0) b=2*pi+b;
//	//if (b>2*pi) b=b-2*pi;		         
//	b=B-pi*0.5;
//	b=pi*0.5-b;        
//	AdjustAngle(b);
//
//	maketext(BzPosb,LZB,b,texth,-1,3,L"宋体",layname);  //标注起点里程   
//
//	//	PZtmp.lc=lce+draw_zybz*(0.5*texth+1.0*SCALE);
//	PZtmp.lc=lce+ChuTu_zybz*(0.5*texth+1.0*SCALE);//出图左右
//	k = AllXLPointPZ(XLName, PZtmp, iRXD, LMRFlag);
//	B=PZtmp.a;
//	WB=dL+0.5*SCALE+0.6*istrb*texth;
//	//BzPose[X]=PZtmp.y+WB*sin(B+pi*0.5*draw_zybz) ;
//	//BzPose[Y]=PZtmp.x+WB*cos(B+pi*0.5*draw_zybz) ;
//	BzPose[X]=PZtmp.y+(WB+offset)*sin(B+pi*0.5*zybz) ;
//	BzPose[Y]=PZtmp.x+(WB+offset)*cos(B+pi*0.5*zybz) ;
//	//b=B+pi*0.5;
//	//b=pi*0.5-b; if (draw_zybz==-1) b=b+pi;
//	//if (b<0.0) b=2*pi+b;
//	//if (b>2*pi) b=b-2*pi;		         
//	b=B-pi*0.5;
//	b=pi*0.5-b;        
//	AdjustAngle(b);
//
//	maketext(BzPose,LZE,b,texth,-1,3,L"宋体",layname);  //标注终点里程   
//
//	WB=dL+0.5*SCALE+1.0*istrb*texth;
//	//PtLzB1[X]=Ptb.x+WB*sin(ab+pi*0.5*draw_zybz);
//	//PtLzB1[Y]=Ptb.y+WB*cos(ab+pi*0.5*draw_zybz);
//	//PtLzE1[X]=Pte.x+WB*sin(ae+pi*0.5*draw_zybz);
//	//PtLzE1[Y]=Pte.y+WB*cos(ae+pi*0.5*draw_zybz);
//	PtLzB1[X]=Ptb.x+WB*sin(ab+pi*0.5*zybz);
//	PtLzB1[Y]=Ptb.y+WB*cos(ab+pi*0.5*zybz);
//	PtLzE1[X]=Pte.x+WB*sin(ae+pi*0.5*zybz);
//	PtLzE1[Y]=Pte.y+WB*cos(ae+pi*0.5*zybz);
//
//
//	makeline2(PtLzB0,PtLzB1,-1,AcDb::kLnWt035,layname);//起点里程标注线
//	makeline2(PtLzE0,PtLzE1,-1,AcDb::kLnWt035,layname);//终点里程标注线
//	
//}


/*===================================================================
函数功能:
------------------------------------------------------------------------
输入:
cml                统一里程 
intPrec            取整精度
directn            方向，向前取整 =true，向后取整 =false
------------------------------------------------------------------------
返回值: 
------------------------------------------------------------------------
调用:
------------------------------------------------------------------------
编写: 李帅 2017.5
=====================================================================*/
void RHA::MarkBridge() const
{
	ACHAR xclc0[50],xclc0_start[50],xclc0_end[50];
	ACHAR StyBri[200], NamBri[256];
	double lenBri,HBri;
	int LineNum;
	bool isSJX=false;
	CGetAllVertData getVdat;
	vector<BRITAB> briAry;

	//判断是否是设计线
	isSJX=false;
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		isSJX=true;

	//获取桥梁数据
	getVdat.GetAllBridgeData(m_mdbName, m_XLName, briAry);

	//绘图
	for(int i=0;i<briAry.size();i++)
	{
		/*if (isSJX && briAry[i].GJZT == false) 818
			continue;*/
		_tcscpy(xclc0, LCchr(briAry[i].CGH, briAry[i].CenDml, DrawParam.NLC));
		_tcscpy(xclc0_start, LCchr(briAry[i].BGH, briAry[i].StDml, DrawParam.NLC));
		_tcscpy(xclc0_end, LCchr(briAry[i].EGH, briAry[i].EdDml, DrawParam.NLC));
		_tcscpy(StyBri, briAry[i].KKSY);
		_tcscpy(NamBri, briAry[i].BriName);
		lenBri=briAry[i].BriLen;	
		LineNum = briAry[i].LineNum;
		HBri = briAry[i].BriHigh;
		INSERT_BIG_MID_BRIDGE(xclc0,xclc0_start,xclc0_end,lenBri,HBri,StyBri,NamBri,LineNum);
	}
}

/*===================================================================
函数功能:
------------------------------------------------------------------------
输入:
cml                统一里程 
intPrec            取整精度
directn            方向，向前取整 =true，向后取整 =false
------------------------------------------------------------------------
返回值: 
------------------------------------------------------------------------
调用:
------------------------------------------------------------------------
编写: 李帅 2017.5
=====================================================================*/
void RHA::MarkStation() const
{
	ACHAR xclc0[50];
	ACHAR NamSta[256];
	ACHAR TKSta[100];
	bool GJXZ;
	CGetAllVertData getVdat;
	vector<STATAB> staAry;

	//获取车站数据
	getVdat.GetAllStationData(m_mdbName, m_XLName, staAry);

	for(int i=0;i<staAry.size();i++)
	{
		_tcscpy(xclc0, LCchr(staAry[i].CGH, staAry[i].CenDml, DrawParam.NLC));
		_tcscpy(NamSta, staAry[i].StaName);
		_tcscpy(TKSta, staAry[i].CQTKM);
		GJXZ = staAry[i].GJXZ;
		INSERT_STATION(xclc0,NamSta,TKSta,GJXZ);
	}

}

/*===================================================================
函数功能:
------------------------------------------------------------------------
输入:
cml                统一里程 
intPrec            取整精度
directn            方向，向前取整 =true，向后取整 =false
------------------------------------------------------------------------
返回值: 
------------------------------------------------------------------------
调用:
------------------------------------------------------------------------
编写: 李帅 2017.5
=====================================================================*/
void RHA::MarkTunnel() const
{
	int i=0;
	int LineNum=1;
	ACHAR qdxclc[50], zdxclc[50];
	ACHAR NamTun[256];
	double style;
	CGetAllVertData getVdat;
	vector<TUNTAB> tunAry;

	//获取车站数据
	getVdat.GetAllTunnelData(m_mdbName, m_XLName, tunAry);

	for(int i=0; i<tunAry.size(); i++)
	{
		_tcscpy(qdxclc, LCchr(tunAry[i].BGH, tunAry[i].StDml, DrawParam.NLC));
		_tcscpy(zdxclc, LCchr(tunAry[i].EGH, tunAry[i].EdDml, DrawParam.NLC));
		_tcscpy(NamTun, tunAry[i].TUNName);
		style=tunAry[i].Flag;
		LineNum = tunAry[i].NumOfLine;
		INSERT_TUNNEL(qdxclc,zdxclc,NamTun,LineNum,style);
	}

}

/*===================================================================
函数功能:绘制水准点
------------------------------------------------------------------------
输入:
cml                统一里程 
intPrec            取整精度
directn            方向，向前取整 =true，向后取整 =false
------------------------------------------------------------------------
返回值: 
------------------------------------------------------------------------
调用:
------------------------------------------------------------------------
编写: 李帅 2017.5
	=====================================================================*/
void RHA::MarkBM() const
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(m_mdbName,L"水准点表",L"");
	//////////////////////////////////////////////////////////////////////////
	CString GH,BianHao;
	double X,Y,XLC,DD,LEV;

	for(int i=0;i<xlmdb.BmNum;i++)
	{
		X=xlmdb.BM[i].N;
		Y=xlmdb.BM[i].E;
		GH=xlmdb.BM[i].GH;
		XLC=xlmdb.BM[i].XLC;
		DD=xlmdb.BM[i].DistZX;
		BianHao=xlmdb.BM[i].BianHao;
		LEV=xlmdb.BM[i].GaoCheng;
		INSERT_BM(X,Y,GH,XLC,DD,BianHao,LEV);
	}

}

//
//绘制单个大中桥
void RHA::INSERT_BIG_MID_BRIDGE(CString xclc0,  CString xclc0_start
								,  CString xclc0_end,  double lenBri,  double HBri,  CString StyBri
								,  CString NamBri	 ,  int LineNum) const
									// 中心现场里程  起点里程  终点里程  桥长  桥梁样式	桥名  绕行段编号（－1、＜0表示为并行或单线）线数(1-单线，2-双线)
{
	if (lenBri<DrawParam.m_NoteMinBriLen-0.001 || HBri<DrawParam.m_NoteMinBriH-0.001)
		return;

	ACHAR layname[256];
	int zybz = -1;//标注方向：-1-左侧，1-右侧,  默认标注在左侧
	double offset = 0.0;

	_tcscpy(layname, DrawParam.BigMidBridge);//层名

	if (m_XLName == JYJX)//818 左右侧和偏移距
		zybz = 1;//既有插旗标在右侧
	else if (m_XLName == SJJX)
			zybz = -1;
	else if (m_XLName == SJ2X)
		zybz = -1;
	
	//if (HasLeft)//如果二线为左线，则基线标注在右侧 
	//	zybz = 1;
	//else//否则标注在左侧
	//	zybz = -1;
	//if (m_XLName == L"既有基线")
	//	zybz = 1;//既有插旗标在右侧

	if (DrawParam.SCALE>9.9)//左线和右线时用 //(LMRFlag==LEFTLINE
		offset = DrawParam.m_ExOffset;

	

	double lc0,  lcb,  lce;//中心统一里程
	int jdNum=JDarray.size();


	TrsCkmltoProjml(xclc0, lc0);
	TrsCkmltoProjml(xclc0_start, lcb);
	TrsCkmltoProjml(xclc0_end, lce);
	if(lc0<0.0 && lcb < 0.00 && lce <0.00)
		return;
	if (lc0 < QXB[0].KZH || lc0 > QXB[jdNum-1].KHZ) 
		return;

	
	//判断是否在出图范围内
	//////////////////////////////////////////////////////////////////////
	//左右宽度
	int k=0;
	double lc2x;
	double pt2x[2];
	double xjj;
	double dL=1.5*DrawParam.SCALE;//单线 桥左侧宽
	double dR=1.5*DrawParam.SCALE;//单线　桥右侧宽


	//双线桥计算偏移距 818
	if(LineNum==2)
	{
		RHA *pRHA=NULL;
		if(XLShuXing == JYJX)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JY2X);
		else if(XLShuXing == JY2X)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JYJX);
		else if(XLShuXing == SJ2X)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, SJJX);
		else if(XLShuXing == SJJX)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, SJ2X);

		if(pRHA)
		{
			CalXJJ(lc0, array, tarray, pRHA->array, pRHA->tarray, xjj, lc2x, pt2x, true);
			if(xjj<0.0)//左侧
			{
				xjj=fabs(xjj);
				dL+=xjj;
				if (DrawParam.SCALE>9.9)
					dL += DrawParam.m_ExOffset;
			}
			else if (xjj>0.0)
			{
				xjj=fabs(xjj);
				dR+=xjj;
				if (DrawParam.SCALE>9.9)
					dR += DrawParam.m_ExOffset;
			}
		}
	}

	////////////////////////////////////////////////////
	//起、中、终三处线路中心坐标
	if ( lcb < 0.01 ) 
		lcb=lc0-0.5*lenBri;//起点统一里程
	if ( lce < 0.01 ) 
		lce=lc0+0.5*lenBri;//终点统一里程

	double a0,  ab,  ae;//线路方位角/桥中线方位角,  mid,  begin,  end//可能为曲线桥
	xlpoint Ptmp;
	AcGePoint3d Pt0, Ptb, Pte;

	//桥梁中心坐标（主线）
	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);
	//818
	int LMRFlag=0;
	if (offset>0.01)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	Pt0.set(Ptmp.y,  Ptmp.x,  0.0);
	a0=Ptmp.a;

	//桥梁起点坐标
	Ptmp.lc=lcb;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.01)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	Ptb.set(Ptmp.y,  Ptmp.x,  0.0);
	ab=Ptmp.a;

	//桥梁终点坐标
	Ptmp.lc=lce;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.01)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	Pte.set(Ptmp.y,  Ptmp.x,  0.0);//桥梁中心坐标（主线）
	ae=Ptmp.a;

	//计算四角45度向外延伸的点坐标

	double aLb=ab-0.5*pi;//桥前端左侧线方位角
	double aRb=ab+0.5*pi;//桥前端右侧线方位角

	double aLe=ae-0.5*pi;//桥终端左侧线方位角
	double aRe=ae+0.5*pi;//桥终端右侧线方位角

	double a_LT=ab-0.75*pi;//四角45度向外延伸线方位角
	double a_LB=ab+0.75*pi;
	double a_RT=ae-0.25*pi;
	double a_RB=ae+0.25*pi;

	AcGePoint3d LT,  LB,  RT,  RB;//桥框四角
	AcGePoint3d LTe,  LBe,  RTe,  RBe;//桥框四角45度向外延伸点

	LT.z=0.0;RT.z=0.0;LB.z=0.0;RB.z=0.0;
	LTe.z=0.0;RTe.z=0.0;LBe.z=0.0;RBe.z=0.0;

	getLineEndPt(Ptb,  dL,  aLb,  LT);
	getLineEndPt(Ptb,  dR,  aRb,  LB);
	getLineEndPt(Pte,  dL,  aLe,  RT);
	getLineEndPt(Pte,  dR,  aRe,  RB);

	getLineEndPt(LT,  1.5*DrawParam.SCALE,  a_LT,  LTe);
	getLineEndPt(LB,  1.5*DrawParam.SCALE,  a_LB,  LBe);
	getLineEndPt(RT,  1.5*DrawParam.SCALE,  a_RT,  RTe);
	getLineEndPt(RB,  1.5*DrawParam.SCALE,  a_RB,  RBe);

	AcGePoint2dArray TPts,  BPts;
	TPts.append(AcGePoint2d(LT.x,  LT.y));
	BPts.append(AcGePoint2d(LB.x,  LB.y));

	int num=int((lce-lcb)/10+1+0.00001);
	double dLen=(lce-lcb)/(num-1);
	double ai=0.0;
	AcGePoint3d iPt, iBPt, iTPt;
	
	//10m的间距绘制桥梁长度方向的框线
	for(int i=1;i<num-1;i++)
	{
		Ptmp.lc=lcb+i*dLen;

		xlpoint_pz(array, tarray, Ptmp);
		if (offset>0.01)
		{
			Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
			Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
		}
		ai=Ptmp.a;
		iPt.set(Ptmp.y,  Ptmp.x,  0.0);//主线上坐标

		getLineEndPt(iPt,  dL,  ai-0.5*pi,  iTPt);
		getLineEndPt(iPt,  dR,  ai+0.5*pi,  iBPt);
		TPts.append(AcGePoint2d(iTPt.x,  iTPt.y));
		BPts.append(AcGePoint2d(iBPt.x,  iBPt.y));
	}

	TPts.append(AcGePoint2d(RT.x,  RT.y));
	BPts.append(AcGePoint2d(RB.x,  RB.y));

	//朱江20090604  四院规定桥梁图例用0.5mm
	double lw=0.5*DrawParam.SCALE;//李伟注释掉2007-08-07统一线宽，不能在这里对线宽进行重新设置
	ads_name en1,  en2,  en3,  en4;//填充边界实体名


	//绘制四边形框线
	makepolyline(TPts,  -1,  lw,  layname);
	ads_entlast(en1);
	makepolyline(BPts,  -1,  lw,  layname);
	ads_entlast(en2);
	TPts.removeSubArray(0,  TPts.length()-1);
	BPts.removeSubArray(0,  BPts.length()-1);

	makeline2(LT,  LB,  -1,  lw,  layname);
	ads_entlast(en3);
	makeline2(RT,  RB,  -1,  lw,  layname);
	ads_entlast(en4);

	//绘制四角的延伸
	makeline2(LT,  LTe,  -1,  lw,  layname);
	makeline2(LB,  LBe,  -1,  lw,  layname);
	makeline2(RT,  RTe,  -1,  lw,  layname);
	makeline2(RB,  RBe,  -1,  lw,  layname);

	//对既有桥梁进行填充
	if (XLShuXing == JYJX || XLShuXing == JY2X)
	{
		ads_point PtCen;//桥梁中心坐标
		double Angle;//填充线的角度

		PtCen[0] = Pt0.x;
		PtCen[1] = Pt0.y;
		PtCen[2] = 0.0;
		Angle = 0.5*pi-(a0-0.25*pi);
		FormFwj(Angle);
		Angle = Angle * 180 / pi ;
		ads_command(RTSTR,   L"-hatch",   RTSTR,   L"S",   RTENAME,   en1,   RTENAME,   en2,   RTENAME,   en3,   RTENAME,   en4,   
			RTSTR ,   L"",   RTSTR,   L"P",   RTSTR,   L"U",   RTREAL,   Angle,   RTREAL,   0.25*texth,   RTSTR,   L"N" ,  RTSTR,   L"",   0);
	}

	//标注

	AcGePoint3d eBZPt;//标注线的终点，起点为Pt0;
	AcGePoint3d Pbz1,  Pbz2;//标注点;
	ACHAR Style[256];
	ACHAR NameLc[256];
	ACHAR ckml[80];
	ACHAR gh[20];
	double dml;

	_tcscpy(Style,  StyBri);
	_tcscpy(NameLc,  NamBri);

	//修改桥梁中心里程精度
	_tcscpy(ckml,  xclc0);
	split_ckml(ckml,  dml,  gh);
	_tcscpy(ckml,   LCchr(gh,  dml,  DrawParam.NCQ));
	_tcscat(NameLc,  ckml);

	int iSty=_tcslen(Style);
	int iNam=_tcslen(NameLc);
	int MaxStr=iSty>iNam?iSty:iNam;//最大字符数
	double WB=dL;//宽度

	/*if (m_IsAutoLorR && pGTSJEX)818
	{
		zybz = CalInsFlagLorR(Pt0.y,  Pt0.x,  m_XLName);
	}*/

	//画线（垂直于线路）
	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*zybz);
	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*zybz);
	makeline2(Pt0,  eBZPt,  -1,  AcDb::kLnWt035,  layname);


	//写桥名、中心里程、孔跨样式
	xlpoint PZtmp;
	double B;
	double b;

	PZtmp.lc=lc0-(0.5*texth+1.0*DrawParam.SCALE);
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	Pbz1[X]=PZtmp.y+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*sin(B+pi*0.5*zybz) ;
	Pbz1[Y]=PZtmp.x+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*cos(B+pi*0.5*zybz) ;

	PZtmp.lc=lc0+(0.5*texth+1.0*DrawParam.SCALE);
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	Pbz2[X]=PZtmp.y+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*sin(B+pi*0.5*zybz) ;
	Pbz2[Y]=PZtmp.x+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*cos(B+pi*0.5*zybz) ;

	
	b=B-pi*0.5;
	b=pi*0.5-b; 
	FormFwj(b);

	maketext(Pbz1,  Style,  b,  texth,  -1,  3,  L"宋体",  layname);     
	maketext(Pbz2,  NameLc,  b,  texth,  -1,  3,  L"宋体",  layname);
}


//////////////////////////////////////////////////////李伟
void RHA::INSERT_TUNNEL(CString qdxclc,CString zdxclc,CString NamTun,int LineNum,int Style) const
{
	ACHAR layname[256];
	int zybz = -1;//隧道注字左右标志，默认在左边
	double offset = 0.0;

	_tcscpy(layname, DrawParam.Tunnel);//层名

	if (m_XLName == JYJX)//818 左右侧和偏移距
		zybz = 1;//既有插旗标在右侧
	else if (m_XLName == SJJX)
		zybz = -1;
	else if (m_XLName == SJ2X)
		zybz = -1;

	int LMRFlag=0;

	if (DrawParam.SCALE>9.9)//左线和右线时用 //(LMRFlag==LEFTLINE
		offset = DrawParam.m_ExOffset;


	
	/////////////////////////////////////////////////////////////
	AcDbObjectIdArray IdAr;
	ACHAR xlcb[80],xlce[80];//现场里程
	double xclcb,xclce;//现场里程
	ACHAR GH[20],LZB[20],LZE[20];//起终端零桩

	//	acutPrintf(L"bri:%lf,len=%lf\n",lc0,lenBri);
	int k=0;

	////////////////////////////////////////////////////////////
	//起终标注里程
	double lcb=0.0;//起点统一里程
	double lce=0.0;//终点统一里程

	_tcscpy(xlcb,qdxclc);
	_tcscpy(xlce,zdxclc);
	split_ckml(xlcb,xclcb,GH);
	split_ckml(xlce,xclce,GH);


	TrsCkmltoProjml(qdxclc, lcb);
	TrsCkmltoProjml(zdxclc, lce);

	if (lce-lcb < DrawParam.m_NoteMinTunLen-0.001)
		return;

	xclcb=xclcb-int(xclcb/1000.0+0.00001)*1000.0+1000.0;//1000.235
	acdbRToS(xclcb,2,DrawParam.NCQ,LZB);
	LZB[0]='+';

	xclce=xclce-int(xclce/1000.0+0.00001)*1000.0+1000.0;//1000.235
	acdbRToS(xclce,2,DrawParam.NCQ,LZE);
	LZE[0]='+';

	/////////////////////////////////////////////////////////////////
	double lenTunnel=lce-lcb;//隧道长
	double lc0=0.5*(lcb+lce);//中心统一里程
	//////////////////////////////////////////////////////
	
	////////////////////////////////////////////
	//左右宽
	double dL=1.5*DrawParam.SCALE;//单线　隧道左侧宽
	double dR=1.5*DrawParam.SCALE;//单线　隧道右侧宽

	double lc2x;double pt2x[2];double xjj;
	
	//双线桥计算偏移距 818
	if(LineNum==2)
	{
		RHA *pRHA=NULL;
		if(XLShuXing == JYJX)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JY2X);
		else if(XLShuXing == JY2X)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JYJX);
		else if(XLShuXing == SJ2X)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, SJJX);
		else if(XLShuXing == SJJX)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, SJ2X);

		if(pRHA)
		{
			CalXJJ(lc0, array, tarray, pRHA->array, pRHA->tarray, xjj, lc2x, pt2x, true);
			if(xjj<0.0)//左侧
			{
				xjj=fabs(xjj);
				dL+=xjj;
				if (DrawParam.SCALE>9.9)
					dL += DrawParam.m_ExOffset;
			}
			else if (xjj>0.0)
			{
				xjj=fabs(xjj);
				dR+=xjj;
				if (DrawParam.SCALE>9.9)
					dR += DrawParam.m_ExOffset;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////	
	double a0,ab,ae;//线路方位角/隧道中线方位角,mid,begin,end//可能为曲线隧道
	xlpoint Ptmp;

	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.001)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//隧道中心坐标（如果双线位于主线上）
	a0=Ptmp.a;

	Ptmp.lc=lcb;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.001)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	AcGePoint3d Ptb(Ptmp.y,Ptmp.x,0.0);//隧道中心坐标（主线）
	ab=Ptmp.a;

	Ptmp.lc=lce;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.001)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	AcGePoint3d Pte(Ptmp.y,Ptmp.x,0.0);//隧道中心坐标（主线）
	ae=Ptmp.a;
	///////////////////////////////////////////////////////////////
	//端点
	double aLb=ab-0.5*pi;//隧道前端左侧线方位角
	double aRb=ab+0.5*pi;//隧道前端右侧线方位角

	double aLe=ae-0.5*pi;//隧道终端左侧线方位角
	double aRe=ae+0.5*pi;//隧道终端右侧线方位角

	double a_LT=ab-0.75*pi;//四角45度向外延伸线方位角
	double a_LB=ab+0.75*pi;
	double a_RT=ae-0.25*pi;
	double a_RB=ae+0.25*pi;

	AcGePoint3d LT,LB,RT,RB,T0,B0;//隧道框四角++两侧中点
	AcGePoint3d LTe,LBe,RTe,RBe;//隧道框四角45度向外延伸
	LT.z=0.0;RT.z=0.0;LB.z=0.0;RB.z=0.0;
	LTe.z=0.0;RTe.z=0.0;LBe.z=0.0;RBe.z=0.0;
	T0.z=0.0;B0.z=0.0;

	//	acutPrintf(L"0:x=%lf,y=%lf,l=%lf,fwj=%lf\n",Ptb.x,Ptb.y,dL,aLb);
	getLineEndPt(Ptb,dL,aLb,LT);
	//	acutPrintf(L"LT:x=%lf,y=%lf\n",LT.x,LT.y);

	getLineEndPt(Ptb,dR,aRb,LB);
	getLineEndPt(Pte,dL,aLe,RT);
	getLineEndPt(Pte,dR,aRe,RB);

	getLineEndPt(LT,1.5*DrawParam.SCALE,a_LT,LTe);
	getLineEndPt(LB,1.5*DrawParam.SCALE,a_LB,LBe);
	getLineEndPt(RT,1.5*DrawParam.SCALE,a_RT,RTe);
	getLineEndPt(RB,1.5*DrawParam.SCALE,a_RB,RBe);
	//////////////////////////////////////////////////////////////
	//两侧边界，可能为曲线隧道
	//	double lw=0.5*DrawParam.SCALE;
	double lw=DrawParam.OtherParam.EntAry[0].LW;
	if(Style<0.9)//明洞
	{
		AcGePoint2dArray TPts,BPts;
		TPts.append(AcGePoint2d(LT.x,LT.y));
		BPts.append(AcGePoint2d(LB.x,LB.y));
		int num=int(lenTunnel/5.0+1+0.00001);
		double dLen=lenTunnel/(num-1);
		for(int i=1;i<num-1;i++)
		{			
			Ptmp.lc=lcb+i*dLen;
			xlpoint_pz(array, tarray, Ptmp);
			if (offset>0.001)
			{
				Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
				Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
			}
			double ai=Ptmp.a;
			AcGePoint3d iPt(Ptmp.y,Ptmp.x,0.0);//主线上坐标
			AcGePoint3d iTPt;
			getLineEndPt(iPt,dL,ai-0.5*pi,iTPt);
			AcGePoint3d iBPt;
			getLineEndPt(iPt,dR,ai+0.5*pi,iBPt);
			TPts.append(AcGePoint2d(iTPt.x,iTPt.y));
			BPts.append(AcGePoint2d(iBPt.x,iBPt.y));
		}
		TPts.append(AcGePoint2d(RT.x,RT.y));
		BPts.append(AcGePoint2d(RB.x,RB.y));

		makepolyline(TPts,-1,lw,layname);
		makepolyline(BPts,-1,lw,layname);
		TPts.removeSubArray(0,TPts.length()-1);
		BPts.removeSubArray(0,BPts.length()-1);
	}
	else//一般隧道
	{
		makeline2(LT,LTe,-1,lw,layname);
		makeline2(LB,LBe,-1,lw,layname);
		makeline2(RT,RTe,-1,lw,layname);
		makeline2(RB,RBe,-1,lw,layname);
	}

	makeline2(LT,LB,-1,lw,layname);
	makeline2(RT,RB,-1,lw,layname);

	//////////////////标注///////////////////////////////
	AcGePoint3d eBZPt;//标注线的终点，起点为Pt0;
	AcGePoint3d Pbz1,Pbz2;//标注点;
	ACHAR lentunstr[20];
	acdbRToS(lenTunnel,2,DrawParam.NLC,lentunstr);
	NamTun+=L"  ";//隧道名称与隧道长之间加两个空格
	//	if(NJD2x>0&&iRXD>=0)NamTun+=L"2(L";//???????????????
	NamTun+=lentunstr;
	NamTun+=L"m";
	//if(NJD2x>0&&iRXD>=0)NamTun+=L")";//???????????????
	ACHAR TUNFlag[256];
	_tcscpy(TUNFlag,NamTun);

	int MaxStr=_tcslen(TUNFlag);
	double WB=dL;
	/*if (m_IsAutoLorR && pGTSJEX) 818
	{
		zybz = CalInsFlagLorR(Pt0.y,Pt0.x,XLName);
	}*/
	//	if(draw_zybz>0)WB=dR;
	if (zybz>0) WB=dR;

	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*zybz);//+0.5*texth*sin(B);
	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*zybz);//+0.5*texth*cos(B);
	makeline2(Pt0,eBZPt,-1,AcDb::kLnWt035,layname);//中心标注线

	xlpoint PZtmp;
	double B;
	PZtmp.lc=lc0-(0.5*texth+1.0*DrawParam.SCALE);
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;

	Pbz1[X]=PZtmp.y+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*sin(B+pi*0.5*zybz) ;
	Pbz1[Y]=PZtmp.x+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*cos(B+pi*0.5*zybz) ;
	PZtmp.lc=lc0+(0.5*texth+1.0*DrawParam.SCALE);
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;

	Pbz2[X]=PZtmp.y+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*sin(B+pi*0.5*zybz) ;
	Pbz2[Y]=PZtmp.x+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth+offset)*cos(B+pi*0.5*zybz) ;

	double b=B-pi*0.5;
	b=pi*0.5-b;        
	FormFwj(b);
	//	if(ChuTu_zybz == 1) Pbz1=Pbz2; //Lw2007-11-05注释
	maketext(Pbz1,TUNFlag,b,texth,-1,3,L"宋体",layname); //标隧道名称及长度    

	int istrb=0,istre=0;
	istrb=_tcslen(LZB);
	istre=_tcslen(LZE);
	AcGePoint3d PtLzB0,PtLzB1,PtLzE0,PtLzE1;//起终点标注线的始终端
	AcGePoint3d BzPosb,BzPose;

	WB=dL+0.5*DrawParam.SCALE;//保证起始里程标注线端点与隧道有0.5mm的距离

	if(zybz>0)WB=dR+0.5*DrawParam.SCALE;
	PtLzB0[X]=Ptb.x+WB*sin(ab+pi*0.5*zybz);
	PtLzB0[Y]=Ptb.y+WB*cos(ab+pi*0.5*zybz);
	PtLzE0[X]=Pte.x+WB*sin(ae+pi*0.5*zybz);
	PtLzE0[Y]=Pte.y+WB*cos(ae+pi*0.5*zybz);

	WB=dL+0.5*DrawParam.SCALE+0.6*istrb*texth;//保证终点里程标注线端点与隧道有0.5mm的距离
	//	if(draw_zybz>0)WB=dR+0.5*DrawParam.SCALE+0.6*istrb*texth;
	if(zybz>0)WB=dR+0.5*DrawParam.SCALE+0.6*istrb*texth;
	//	PZtmp.lc=lcb+draw_zybz*(0.5*texth+1.0*DrawParam.SCALE);
	//	PZtmp.lc=lcb+ChuTu_zybz*(0.5*texth+1.0*DrawParam.SCALE);
	PZtmp.lc=lcb-(0.5*texth+1.0*DrawParam.SCALE);//Lw2007-11-05
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	//	BzPosb[X]=PZtmp.y+WB*sin(B+pi*0.5*draw_zybz) ;
	//	BzPosb[Y]=PZtmp.x+WB*cos(B+pi*0.5*draw_zybz) ;
	BzPosb[X]=PZtmp.y+(WB+offset)*sin(B+pi*0.5*zybz) ;
	BzPosb[Y]=PZtmp.x+(WB+offset)*cos(B+pi*0.5*zybz) ;
	//b=B+pi*0.5;
	//b=pi*0.5-b; if (draw_zybz==-1) b=b+pi;
	//if (b<0.0) b=2*pi+b;
	//if (b>2*pi) b=b-2*pi;		         
	b=B-pi*0.5;
	b=pi*0.5-b;        
	FormFwj(b);

	maketext(BzPosb,LZB,b,texth,-1,3,L"宋体",layname);  //标注起点里程   

	//	PZtmp.lc=lce+draw_zybz*(0.5*texth+1.0*DrawParam.SCALE);
	PZtmp.lc=lce+DrawParam.m_TextDir*(0.5*texth+1.0*DrawParam.SCALE);//出图左右
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	WB=dL+0.5*DrawParam.SCALE+0.6*istrb*texth;

	BzPose[X]=PZtmp.y+(WB+offset)*sin(B+pi*0.5*zybz) ;
	BzPose[Y]=PZtmp.x+(WB+offset)*cos(B+pi*0.5*zybz) ;
		         
	b=B-pi*0.5;
	b=pi*0.5-b;        
	FormFwj(b);

	maketext(BzPose,LZE,b,texth,-1,3,L"宋体",layname);  //标注终点里程   

	WB=dL+0.5*DrawParam.SCALE+1.0*istrb*texth;
	//PtLzB1[X]=Ptb.x+WB*sin(ab+pi*0.5*draw_zybz);
	//PtLzB1[Y]=Ptb.y+WB*cos(ab+pi*0.5*draw_zybz);
	//PtLzE1[X]=Pte.x+WB*sin(ae+pi*0.5*draw_zybz);
	//PtLzE1[Y]=Pte.y+WB*cos(ae+pi*0.5*draw_zybz);
	PtLzB1[X]=Ptb.x+WB*sin(ab+pi*0.5*zybz);
	PtLzB1[Y]=Ptb.y+WB*cos(ab+pi*0.5*zybz);
	PtLzE1[X]=Pte.x+WB*sin(ae+pi*0.5*zybz);
	PtLzE1[Y]=Pte.y+WB*cos(ae+pi*0.5*zybz);


	makeline2(PtLzB0,PtLzB1,-1,AcDb::kLnWt035,layname);//起点里程标注线
	makeline2(PtLzE0,PtLzE1,-1,AcDb::kLnWt035,layname);//终点里程标注线
	
}

void RHA::INSERT_STATION(CString xclc0,CString NamSta,CString TKSta,bool GJXZ) const
{
	/////////////////////////////////////////////////////////////
	//图层名
	ACHAR layname[256];

	_tcscpy(layname, DrawParam.Station);//层名

	int zybz=-1;
	int LMRFlag=0;//818
	if (m_XLName == JYJX)//818 左右侧和偏移距
		zybz = 1;//既有插旗标在右侧
	else if (m_XLName == SJJX)
		zybz = -1;
	else if (m_XLName == SJ2X)
		zybz = -1;
	////////////////////////////////////////////////////////////////
	//找车站图块的位置
	ACHAR dwgPath[256],Name[50];
	_stprintf(dwgPath,L"%s\\DWG\\",SysPath);
	_stprintf(Name,L"%s.dwg",TKSta);

	AcDbObjectId pBlockId;
	pBlockId=CreateBlockFromDwgFile(dwgPath,Name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s%s不存在!\n",dwgPath,Name);
		return;
	}
	//////////////////////////////////////////////////////////
	AcDbObjectIdArray IdAr;
	double lc0;//中心统一里程
	ACHAR xlc0[50];
	_tcscpy(xlc0,xclc0);
	//////////////////////////////////////////////////////////////////////////
	//修改车站中心里程精度
	ACHAR GH[20];
	double dml;
	split_ckml(xlc0,dml,GH);
	_tcscpy(xlc0, LCchr(GH,dml,DrawParam.NCQ));

	TrsCkmltoProjml(xclc0, lc0);
	
	////////////////////////////////////////////////////////////
	int k=0;
	double a0;//线路方位角
	xlpoint Ptmp;

	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);

	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//插入点坐标
	a0=Ptmp.a;

	/*
	if(NJD2x>0&&iRXD<0)//并行//高整插入位置保证不与二线打架
	{
	double lc2x;double pt2x[2];double xjj;
	CalXjj(lc0,xjj,lc2x,pt2x);//计算线间距
	if(xjj*draw_zybz>0)
	{
	double jiao=a0+0.5*draw_zybz*pi;
	Pt0.x=Pt0.x+fabs(xjj)*sin(jiao);
	Pt0.y=Pt0.y+fabs(xjj)*cos(jiao);
	}
	}
	*/
	AcDbBlockReference *pBkRr=new AcDbBlockReference(Pt0,pBlockId);
	double rotation=2.5*pi-a0;
	//	if(draw_zybz==1)rotation+=pi;
	if(zybz==1)rotation+=pi;
	pBkRr->setRotation(rotation);
	pBkRr->setScaleFactors(AcGeScale3d(DrawParam.SCALE,DrawParam.SCALE,1));
	pBkRr->setLayer(layname);
	AddEntityToDbs(pBkRr);

	AcGePoint3d BZPt;//注字点0;

	int istr=_tcslen(xlc0);
	//	Ptmp.lc=lc0+ChuTu_zybz*(0.5*texth+1.0*DrawParam.SCALE);//出图左右
	Ptmp.lc=lc0-(0.5*texth+1.0*DrawParam.SCALE);//Lw2007-11-05
	xlpoint_pz(array, tarray, Ptmp);
	//double b=a0+pi*0.5;
	//b=pi*0.5-b; if (draw_zybz==-1) b=b+pi;
	//if (b<0.0) b=2*pi+b;
	//if (b>2*pi) b=b-2*pi;		         
	double b=a0-pi*0.5;
	b=pi*0.5-b;        
	FormFwj(b);

	double B=Ptmp.a;
	//BZPt[X]=Ptmp.y+(0.48*istr*texth)*sin(B+pi*0.5*draw_zybz);
	//BZPt[Y]=Ptmp.x+(0.48*istr*texth)*cos(B+pi*0.5*draw_zybz);
	BZPt[X]=Ptmp.y+(0.3*istr*texth)*sin(B+pi*0.5*zybz);
	BZPt[Y]=Ptmp.x+(0.3*istr*texth)*cos(B+pi*0.5*zybz);
	//废弃车站打叉(改建性质为L"改建")
	if ((XLShuXing == JYJX || XLShuXing == JY2X) && GJXZ == 1)
	{
		double NCross,ECross;
		ECross = Pt0.x+(0.48*istr*texth)*sin(B+pi*0.5*zybz);
		NCross = Pt0.y+(0.48*istr*texth)*cos(B+pi*0.5*zybz);
		DrawStaFQDBZ(NCross,ECross,B+pi*0.5,_tcslen(xlc0)*0.3*texth,LMRFlag);
	}

	


	maketext(BZPt,xlc0,b,texth,-1,3,L"宋体",layname);

	double WB=41;//车车站标志的高度
	if(TKSta[3]=='0'&&TKSta[4]=='1')WB=41;
	else if(TKSta[3]=='0'&&TKSta[4]=='4')WB=35;
	else if(TKSta[3]=='0'&&TKSta[4]=='6')WB=33;
	else if(TKSta[3]=='0'&&TKSta[4]=='7')WB=39.1924;
	else if(TKSta[3]=='0'&&TKSta[4]=='8')WB=35;
	else if(TKSta[3]=='0'&&TKSta[4]=='9')WB=41;
	else if(TKSta[3]=='1'&&TKSta[4]=='0')WB=41;
	else if(TKSta[3]=='1'&&TKSta[4]=='5')WB=36;
	else if(TKSta[3]=='1'&&TKSta[4]=='6')WB=39.1924;
	else if(TKSta[3]=='1'&&TKSta[4]=='7')WB=35;
	else if(TKSta[3]=='1'&&TKSta[4]=='8')WB=35;
	else WB=41;
	//	acutPrintf(L"WB=%lf\n",WB);
	WB=WB*DrawParam.SCALE;
	//BZPt[X]=Pt0.x+(WB+1*DrawParam.SCALE+0.5*texth)*sin(B+pi*0.5*draw_zybz);
	//BZPt[Y]=Pt0.y+(WB+1*DrawParam.SCALE+0.5*texth)*cos(B+pi*0.5*draw_zybz);
	BZPt[X]=Pt0.x+(WB+1*DrawParam.SCALE+0.5*texth)*sin(B+pi*0.5*zybz);
	BZPt[Y]=Pt0.y+(WB+1*DrawParam.SCALE+0.5*texth)*cos(B+pi*0.5*zybz);

	_tcscpy(Name,NamSta);
	maketext(BZPt,Name,rotation,texth,-1,3,L"宋体",layname);

}

void RHA::DrawStaFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const//绘制废弃段标注“×”
{
	AcGePoint3d Pt1,Pt2,Pt3,Pt4;
	ACHAR layname[256];
	_tcscpy(layname , DrawParam.FQDBZ);

	Pt1.x = ECross + Size*sin(Fwj + pi/6.0);
	Pt1.y = NCross + Size*cos(Fwj + pi/6.0);
	Pt2.x = ECross + Size*sin(Fwj - 5.0*pi/6.0);
	Pt2.y = NCross + Size*cos(Fwj - 5.0*pi/6.0);
	makeline2(Pt1,Pt2,-1,AcDb::kLnWt030,layname);
	Pt3.x = ECross + Size*sin(Fwj - pi/6.0);
	Pt3.y = NCross + Size*cos(Fwj - pi/6.0);
	Pt4.x = ECross + Size*sin(Fwj + 5.0*pi/6.0);
	Pt4.y = NCross + Size*cos(Fwj + 5.0*pi/6.0);
	makeline2(Pt3,Pt4,-1,AcDb::kLnWt030,layname);
}

//绘制水准点  
void RHA::INSERT_BM(double X, double Y, CString GH, double XLC, double DD, CString BianHao, double LEV) const
{	
	ACHAR layname[256];
	ACHAR GaoCheng[20];
	double projml=0.0;//统一里程
	AcGePoint3d InsertPt;//插入点

	_stprintf(layname, L"%s", DrawParam.BM);//图层

	acdbRToS(LEV, 2, NGC, GaoCheng);//高程取小数

	if (XLShuXing != JYJX)//818 水准点哪条线路？JYJX
		return;
	
	if(XLC>0.01)
	{
		ACHAR ghCpy[20];
		_tcscpy(ghCpy,  GH);

		TrsCkmltoProjml(ghCpy,  XLC,  projml);
	}


	//提取图块
	ACHAR dwgPath[256],  name[50];
	AcDbObjectId pBlockId;

	_stprintf(dwgPath, L"%s\\DWG\\", SysPath);
	_tcscpy(name, L"31014.dwg");
	pBlockId = CreateBlockFromDwgFile(dwgPath, name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s%s不存在!\n", dwgPath, name);
		return;
	}

	//插入图块
	int k=0;
	double a0;//线路方位角
	if(X<0.1&&Y<0.1)//x=0, y=0
	{
		xlpoint Ptmp;
		Ptmp.lc=projml;
		//k = AllXLPointPZ(L"既有基线",  Ptmp,  0,  MAINLINE);
		xlpoint_pz(array,  tarray,  Ptmp);

		AcGePoint3d Pt0(Ptmp.y, Ptmp.x, 0.0);//线路中线点坐标
		a0=Ptmp.a;
		InsertPt.x=Pt0.x+DD*sin(a0+pi*0.5);
		InsertPt.y=Pt0.y+DD*cos(a0+pi*0.5);
	}
	else 
		InsertPt.set(Y, X, 0);

	AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt, pBlockId);

	pBkRr->setScaleFactors(AcGeScale3d(DrawParam.SCALE,  DrawParam.SCALE, 1));
	pBkRr->setLayer(layname);
	AddEntityToDbs(pBkRr);

	//标注编号和高程
	AcGePoint3d BZCen, BZCenL, BZCenR;//注字短线中心点，及左、右两点
	AcGePoint3d BZTop, BZBtom;//水准点编号中心、高程中心


	int istrbh=_tcslen(BianHao);//水准点编号字符串长
	int istrlev=_tcslen(GaoCheng);//水准点高程字符串长
	int iMax=istrbh>istrlev?istrbh:istrlev;//字符串最大长度
	double texHgt=DrawParam.OtherParam.MarkAry[3].TextHeight;//ls 取桥梁的字高

	//Lw2007-11-05从右往左出图时，标注应该倒立。因而再标注时加入了ChuTu_zybz的影响
	BZCen.x=InsertPt.x-DrawParam.m_TextDir*(0.4*iMax+0.5)*texHgt+2*DrawParam.SCALE;
	BZCen.y=InsertPt.y;
	BZCen.z=0.0;

	BZCenL.x=InsertPt.x-DrawParam.m_TextDir*(0.5)*texHgt+2*DrawParam.SCALE;
	BZCenL.y=InsertPt.y;
	BZCenL.z=0.0;

	BZCenR.x=InsertPt.x-DrawParam.m_TextDir*(0.8*iMax+0.5)*texHgt+2*DrawParam.SCALE;
	BZCenR.y=InsertPt.y;
	BZCenR.z=0.0;

	BZTop.x=BZCen.x;
	BZTop.y=BZCen.y-DrawParam.m_TextDir*0.6*texHgt;
	BZTop.z=0.0;

	BZBtom.x=BZCen.x;
	BZBtom.y=BZCen.y+DrawParam.m_TextDir*0.6*texHgt;
	BZBtom.z=0.0;

	ACHAR strIndx[20];
	_tcscpy(strIndx, BianHao);

	if (DrawParam.m_TextDir<0)
	{
		maketext(BZTop, strIndx, 0.0, texHgt, -1, 3, L"宋体", layname);
		maketext(BZBtom, GaoCheng, 0.0, texHgt, -1, 3, L"宋体", layname);
	}
	else
	{
		maketext(BZTop, strIndx, PI, texHgt, -1, 3, L"宋体", layname);
		maketext(BZBtom, GaoCheng, PI, texHgt, -1, 3, L"宋体", layname);
	}
	makeline2(BZCenL, BZCenR, -1, AcDb::kLnWtByLayer, layname);
}

/*===================================================================
函数功能:绘制导线
------------------------------------------------------------------------
输入:
cml                统一里程 
intPrec            取整精度
directn            方向，向前取整 =true，向后取整 =false
------------------------------------------------------------------------
返回值: 
------------------------------------------------------------------------
调用:
------------------------------------------------------------------------
编写: 李帅 2017.5
=====================================================================*/
void RHA::MarkDX() const
{
	if (XLShuXing != JYJX)//818 导线只能既有基线？
		return;
	
	XLDataBase xlmdb;

	if (xlmdb.Read_XLDbs(m_mdbName,L"导线表",L"") && xlmdb.DXNum>0)
	{
		int num=xlmdb.DXNum;
		double *NX=new double[num];
		double *EY=new double[num];
		double *GaoCheng=new double[num];
		double *FWJ=new double[num];
		CStringArray BianHao;

		for(int i=0;i<num;i++)
		{
			NX[i]=xlmdb.DX[i].N;
			EY[i]=xlmdb.DX[i].E;
			GaoCheng[i]=xlmdb.DX[i].GaoCheng;
			FWJ[i]=xlmdb.DX[i].FWJ;
			BianHao.Add(xlmdb.DX[i].ZhuangHao);
		}

		INSERT_DX(num, NX, EY, &BianHao, GaoCheng, FWJ);

		delete []NX; NX = NULL;
		delete []EY; EY = NULL;
		delete []GaoCheng; GaoCheng = NULL;
		delete []FWJ; FWJ = NULL;
		BianHao.RemoveAll();
	}
	
}

//绘制导线
void RHA::INSERT_DX(int NUM, double *NX, double *EY, CStringArray *BianHao, double *GaoCheng, double *FWJ) const
{
	ACHAR layname[40];
	AcGePoint3d InsertPt;//插入点

	_stprintf(layname, L"%s", DrawParam.DaoXian);//图层名

	//提取图块
	ACHAR dwgPath[256], Name[50];
	_stprintf(dwgPath, L"%s\\DWG\\", SysPath);
	_tcscpy(Name, L"dxsample.dwg");
	AcDbObjectId pBlockId;
	pBlockId = CreateBlockFromDwgFile(dwgPath, Name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s中绘制导线点的图块-%s-不存在，不能绘制导线点!\n", dwgPath, Name);
		return;
	}

	AcGePoint3d BZCen, BZCenL, BZCenR;//注字短线中心点，及左、右两点
	AcGePoint3d BZTop, BZBtom;//水准点编号中心、高程中心
	CString strtmp;
	ACHAR ctmp[50];
	ACHAR strIndx[20];
	int istrbh;
	int istrlev;
	int iMax;
	double texHgt;

	texHgt=DrawParam.OtherParam.MarkAry[3].TextHeight;//ls 取桥梁的字高

	for(int i = 0; i < NUM; i++)
	{
		InsertPt.set(EY[i],  NX[i], 0);

		AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setScaleFactors(AcGeScale3d(DrawParam.SCALE, DrawParam.SCALE, 1));
		pBkRr->setLayer(layname);
		AddEntityToDbs(pBkRr);

		//标注编号和高程
		_tcscpy(ctmp, BianHao->GetAt(i));
		istrbh = _tcslen(ctmp);//水准点编号字符串长

		_stprintf(ctmp, L"%.2lf", GaoCheng[i]);
		istrlev= _tcslen(ctmp);//水准点高程字符串长
		iMax=istrbh>istrlev?istrbh:istrlev;//字符串最大长度

		//Lw2007-11-05从右往左出图时，标注应该倒立。因而再标注时加入了DrawParam.m_TextDir的影响
		BZCen.x=InsertPt.x-DrawParam.m_TextDir*(0.4*iMax+0.5)*texHgt+2*DrawParam.SCALE;
		BZCen.y=InsertPt.y;
		BZCen.z=0.0;

		BZCenL.x=InsertPt.x-DrawParam.m_TextDir*(0.5)*texHgt+2*DrawParam.SCALE;
		BZCenL.y=InsertPt.y;
		BZCenL.z=0.0;

		BZCenR.x=InsertPt.x-DrawParam.m_TextDir*(0.8*iMax+0.5)*texHgt+2*DrawParam.SCALE;
		BZCenR.y=InsertPt.y;
		BZCenR.z=0.0;

		BZTop.x=BZCen.x;
		BZTop.y=BZCen.y-DrawParam.m_TextDir*0.6*texHgt;
		BZTop.z=0.0;

		BZBtom.x=BZCen.x;
		BZBtom.y=BZCen.y+DrawParam.m_TextDir*0.6*texHgt;
		BZBtom.z=0.0;

		_tcscpy(strIndx, BianHao->GetAt(i));

		if (DrawParam.m_TextDir<0)
		{
			maketext(BZTop, strIndx, 0.0, texHgt, -1, 3, L"宋体", layname);
			_stprintf(ctmp, L"%.2lf", GaoCheng[i]);
			maketext(BZBtom, ctmp, 0.0, texHgt, -1, 3, L"宋体", layname);
		}
		else
		{
			maketext(BZTop, strIndx, PI, texHgt, -1, 3, L"宋体", layname);
			_stprintf(ctmp, L"%.2lf", GaoCheng[i]);
			maketext(BZBtom, ctmp, PI, texHgt, -1, 3, L"宋体", layname);
		}
		makeline2(BZCenL, BZCenR, -1, AcDb::kLnWtByLayer, layname);
	}
}

//处理既有线小桥涵的接长
void RHA::CalSmallBriLen(vector<SMALLBRIDGETAB> &smallBriAry) const
{
	if(XLShuXing != JYJX && XLShuXing != JY2X)
		return;

	//获取设计线
	XLDataBase xlmdb;
	CString tmpStr;
	RHA *pSJX=NULL;
	CArray<XLInforTableStruct, XLInforTableStruct> xlInforAry;
	xlpoint PZTemp;
	double FWJ_JC;
	double PT[3],resPt[2];
	double Dist = 0.0;
	int LorR;


	xlmdb.ReadXLInfTable(m_mdbName, SJJX, xlInforAry);
	xlmdb.ReadXLInfTable(m_mdbName, SJ2X, xlInforAry);

	for (int i=0; i<xlInforAry.GetSize(); i++)
	{
		pSJX=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, xlInforAry[i].XLName);
		if (!pSJX)
			continue;

		//读取设计线的小桥涵
		tmpStr.Format(L"线路名=\"%s\"", xlInforAry[i].XLName);
		if (xlmdb.Read_XLDbs(m_mdbName, L"小桥涵", tmpStr))
		{
			for (int iSBR=0; iSBR<xlmdb.SBriNum; iSBR++)
			{
				if (_tcsstr(xlmdb.SBri[iSBR].KKStyle, L"接长") == NULL)//包含 “接长”
					continue;

				for (int iJYSBR=0; iJYSBR<smallBriAry.size(); iJYSBR++)
				{
					if (fabs(smallBriAry[iJYSBR].ml - xlmdb.SBri[iSBR].ml) < 0.1 
						&& _tcscmp(smallBriAry[iJYSBR].GH, xlmdb.SBri[iSBR].GH) == 0)
					{


						_tcscat(smallBriAry[iJYSBR].KKStyle, L"(接长)");
						TrsCkmltoPZ(smallBriAry[iJYSBR].GH, smallBriAry[iJYSBR].ml, PZTemp);
						PT[0] = PZTemp.x;
						PT[1] = PZTemp.y;
						PT[2] = 0.0;


						FWJ_JC = PZTemp.a + rad(smallBriAry[iJYSBR].JJiao);//交叉的方位角


						LorR = JudgePtLorR(PZTemp.x, PZTemp.y, pSJX->array, pSJX->tarray, Dist);

						if (LorR == 0)//既有线与贯通基线重合，接长至贯通设计二线右侧
						{
							/*if (pzLinepJYarray[iXL]->pGTSJEX) 818 就按照在哪条线路就记录接长就好
							{
							pzLinepJYarray[iXL]->Search_EXPt(pzLinepJYarray[iXL]->pGTSJEX->array,pzLinepJYarray[iXL]->pGTSJEX->tarray,PT,FWJ_JC,resPt);
							smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0]) + (PT[1]-resPt[1])*(PT[1]-resPt[1]))
							+ fabs(xlmdb.SBri[iSBR].dwR);
							}*/

						}
						else if (LorR == -1)//既有线在贯通基线左侧，往右接长
						{
							Search_EXPt(pSJX->array,pSJX->tarray,PT,FWJ_JC,resPt);
							smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0])
								+ (PT[1]-resPt[1])*(PT[1]-resPt[1]))
								+ fabs(xlmdb.SBri[iSBR].dwR);

							//if (xlmdb.SBri[iSBR].LineNum == 1)//线数为1接长到贯通基线右侧
							//{
							//	Search_EXPt(pSJX->array,pSJX->tarray,PT,FWJ_JC,resPt);
							//	smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0])
							//		+ (PT[1]-resPt[1])*(PT[1]-resPt[1]))
							//		+ fabs(xlmdb.SBri[iSBR].dwR);
							//}

							//if (xlmdb.SBri[iSBR].LineNum == 2)//线数为2接长到贯通二线右侧 818 设计二线多条  LineNum 无效 ？？  贯通二线 if (InsertItemDB.SBri[iSBR].LineNum == 2)//接长到贯通基线左侧
							//{
							//	if (pzLinepJYarray[iXL]->pGTSJEX)
							//	{
							//		pzLinepJYarray[iXL]->Search_EXPt(pzLinepJYarray[iXL]->pGTSJEX->array,pzLinepJYarray[iXL]->pGTSJEX->tarray,PT,FWJ_JC,resPt);
							//		smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0]) + (PT[1]-resPt[1])*(PT[1]-resPt[1]))
							//			+ fabs(xlmdb.SBri[iSBR].dwR);
							//	}
							//}
						}
						else//既有线在设计线右侧
						{
							//接长至贯通设计线左侧
							Search_EXPt(pSJX->array,pSJX->tarray,PT,FWJ_JC,resPt);//计算设计线上的点坐标
							//计算横向长
							smallBriAry[iJYSBR].dwL = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0])
								+ (PT[1]-resPt[1])*(PT[1]-resPt[1]))
								+ fabs(xlmdb.SBri[iSBR].dwL);

						}

					}
				}
			}
		}


		if (xlmdb.SBri)
		{
			delete []xlmdb.SBri;
			xlmdb.SBri=NULL;
			xlmdb.SBriNum=0;
		}
	}

}

//绘制小桥涵
void RHA::plhINSBR(bool IsDrawAll) const
{
	vector<SMALLBRIDGETAB> smallBriAry;
	CString tmpStr;
	XLDataBase xlmdb;

	tmpStr.Trim();
	tmpStr=L"线路名=\"" + m_XLName + L"\"";

	if (xlmdb.Read_XLDbs(m_mdbName, L"小桥涵", tmpStr) && xlmdb.SBriNum>0)
	{
		smallBriAry.resize(xlmdb.SBriNum);
		for (int i=0; i<xlmdb.SBriNum; i++)
		{
			smallBriAry[i]=xlmdb.SBri[i];
		}

		//处理既有基线小桥涵的接长
		if(XLShuXing == JYJX || XLShuXing == JY2X)
			CalSmallBriLen(smallBriAry);

		int LineNum = 1;
		CString NameCross;

		for(int i=0;i<smallBriAry.size();i++)
		{
			NameCross=smallBriAry[i].KKStyle;
			LineNum = smallBriAry[i].LineNum;

			//如果只是绘制立交涵洞，其他不画
			if (!IsDrawAll && !(smallBriAry[i].InsertStyle>0.5 && smallBriAry[i].InsertStyle<2.5))
				continue;
			//818 改建性质 既有 新建 有什么用？

			INSERT_SMALL_BRIDGE(smallBriAry[i].GH,smallBriAry[i].ml,NameCross,LineNum,smallBriAry[i].GJXZ,MAINLINE,-1,
				smallBriAry[i].dwL,smallBriAry[i].dwR,
				smallBriAry[i].LLevel,smallBriAry[i].RLevel,
				smallBriAry[i].JJiao,smallBriAry[i].InsertStyle,smallBriAry[i].ToUse);
		}

	}

}

void RHA::DrawSmallBridge(AcGePoint3d sPt, AcGePoint3d ePt, ACHAR layname[100], int LineNum, int mode, double LWid,  double Rwid, int LMRFlag, double lc0,  double Jiao) const
{//小桥起终端中心坐标

	double ab,ae;
	xyddaa(sPt.y,sPt.x,ePt.y,ePt.x,&ab);
	ae=ab;
	double aLb,aRb,aLe,aRe;
	if (mode>3 && mode<9)//平交道
	{
		double fwj = ab + rad(Jiao);
		aLb=fwj-pi;//桥前端左侧线方位角
		aRb=fwj;//桥前端右侧线方位角
		aLe=fwj-pi;//桥终端左侧线方位角
		aRe=fwj;//桥终端右侧线方位角
	}
	else
	{
		aLb=ab-0.5*pi;//桥前端左侧线方位角
		aRb=ab+0.5*pi;//桥前端右侧线方位角
		aLe=ae-0.5*pi;//桥终端左侧线方位角
		aRe=ae+0.5*pi;//桥终端右侧线方位角
	}

	double a_LT=ab-0.75*pi;//四角45度向外延伸线方位菜
	double a_LB=ab+0.75*pi;
	double a_RT=ae-0.25*pi;
	double a_RB=ae+0.25*pi;

	AcGePoint3d LT,LB,RT,RB,T0,B0;//桥框四角++两侧中点
	AcGePoint3d LTe,LBe,RTe,RBe;//桥框四角45度向外延伸
	LT.z=0.0;RT.z=0.0;LB.z=0.0;RB.z=0.0;
	LTe.z=0.0;RTe.z=0.0;LBe.z=0.0;RBe.z=0.0;
	T0.z=0.0;B0.z=0.0;

	//左右宽度
	double dL=1.5*DrawParam.SCALE;//单线 桥左侧宽
	double dR=1.5*DrawParam.SCALE;//单线　桥右侧宽
	double lc2x;double pt2x[2];double xjj;

	//if(LMRFlag==MAINLINE && (mode == 0 || mode == 3)) 818
	//{

	//	if(HasLeft && LineNum==2)//存在左线
	//	{
	//		CalXjjLx(lc0,xjj,lc2x,pt2x);
	//		xjj=fabs(xjj);
	//		dL+=xjj;
	//	}
	//	if(HasRight && LineNum==2)//存在左线
	//	{
	//		CalXjjRx(lc0,xjj,lc2x,pt2x);
	//		xjj=fabs(xjj);
	//		dR+=xjj;
	//	}	
	//}

	if(LineNum==2  && (mode == 0 || mode == 3))
	{
		RHA *pRHA=NULL;
		if(XLShuXing == JYJX)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JY2X);
		else if(XLShuXing == JY2X)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JYJX);
		else if(XLShuXing == SJ2X)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, SJJX);
		else if(XLShuXing == SJJX)
			pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, SJ2X);

		if(pRHA)
		{
			CalXJJ(lc0, array, tarray, pRHA->array, pRHA->tarray, xjj, lc2x, pt2x, true);
			if(xjj<0.0)//左侧
			{
				xjj=fabs(xjj);
				dL+=xjj;
				if (DrawParam.SCALE>9.9)
					dL += DrawParam.m_ExOffset;
			}
			else if (xjj>0.0)
			{
				xjj=fabs(xjj);
				dR+=xjj;
				if (DrawParam.SCALE>9.9)
					dR += DrawParam.m_ExOffset;
			}
		}
	}
	
	if (fabs(LWid)>0.1)//用户指定了立交桥的宽度
		dL = LWid;
	if (fabs(Rwid)>0.0)//用户指定了立交桥的宽度
		dR = Rwid;

	getLineEndPt(sPt,dL,aLb,LT);
	getLineEndPt(sPt,dR,aRb,LB);
	getLineEndPt(ePt,dL,aLe,RT);
	getLineEndPt(ePt,dR,aRe,RB);

	getLineEndPt(LT,1.5*DrawParam.SCALE,a_LT,LTe);
	getLineEndPt(LB,1.5*DrawParam.SCALE,a_LB,LBe);
	getLineEndPt(RT,1.5*DrawParam.SCALE,a_RT,RTe);
	getLineEndPt(RB,1.5*DrawParam.SCALE,a_RB,RBe);

	//	double lw=0.5*DrawParam.SCALE;
	double LW=DrawParam.OtherParam.EntAry[5].LW;
	double lw=LW;
	//if (layname == SmallBridge || layname == LeftSmallBridge || layname == RightSmallBridge) 818
		lw = 0.5 * LW;

	lw*=DrawParam.SCALE;

	if(mode==0)//小桥
	{
		///////////////上下两条边///////////

		makeline2(LT,RT,-1,lw,layname);
		makeline2(LB,RB,-1,lw,layname);
	}

	if(mode==3)//小桥
	{
		///////////////上下两条边///////////

		makeline2(LT,RT,-1,lw,layname);
		makeline2(LB,RB,-1,lw,layname);
		double Tab,Eab;
		xyddaa(LT.y,LT.x,RT.y,RT.x,&Tab);//求面域边界点
		Tab-=pi/2;
		xyddaa(LB.y,LB.x,RB.y,RB.x,&Eab);
		Eab+=pi/2;
		double len=10;
		AcGePoint3d LTRoad,LBRoad,RTRoad,RBRoad;
		LTRoad.x = LT.x+len*sin(Tab);
		LTRoad.y = LT.y+len*cos(Tab);

		LBRoad.x = LB.x+len*sin(Eab);
		LBRoad.y = LB.y+len*cos(Eab);

		RBRoad.x = RB.x+len*sin(Eab);
		RBRoad.y = RB.y+len*cos(Eab);

		RTRoad.x = RT.x+len*sin(Tab);
		RTRoad.y = RT.y+len*cos(Tab);
		//makeline2(LTRoad,LT,-1,lw,layname);
	}

	if(mode==1)//设计铁路在道路之上 或 铁路与铁路
	{
		///////////////上下两条边///////////
		double len=lw/2;
		ads_name en1,en2;
		ads_point PT1,PT2,PT3,PT4;
		double Tab,Eab;

		makeline2(LT,RT,-1,lw,layname);
		ads_entlast(en1);
		makeline2(LB,RB,-1,lw,layname);
		ads_entlast(en2);

		xyddaa(LT.y,LT.x,RT.y,RT.x,&Tab);//求面域边界点
		Tab+=pi/2;
		xyddaa(LB.y,LB.x,RB.y,RB.x,&Eab);
		Eab-=pi/2;
		PT1[X] = LT.x+len*sin(Tab);
		PT1[Y] = LT.y+len*cos(Tab);
		PT1[Z] = 0.0;

		PT2[X] = LB.x+len*sin(Eab);
		PT2[Y] = LB.y+len*cos(Eab);
		PT2[Z] = 0.0;
		PT3[X] = RB.x+len*sin(Eab);
		PT3[Y] = RB.y+len*cos(Eab);
		PT3[Z] = 0.0;
		PT4[X] = RT.x+len*sin(Tab);
		PT4[Y] = RT.y+len*cos(Tab);
		PT4[Z] = 0.0;

		ads_command(RTSTR,L"WIPEOUT",RTPOINT,PT1,RTPOINT,PT2,RTPOINT,PT3,RTPOINT,PT4,RTSTR,L"C",RTNONE);
		ads_command(RTSTR,L"WIPEOUT",RTSTR,L"F",RTSTR,L"OFF",RTNONE);

		len=DrawParam.SCALE*2;
		AcGePoint3d LTRoad,LBRoad,RTRoad,RBRoad;
		LTRoad.x = LTe.x+len*sin(Tab);
		LTRoad.y = LTe.y+len*cos(Tab);
		LBRoad.x = LBe.x+len*sin(Eab);
		LBRoad.y = LBe.y+len*cos(Eab);
		RBRoad.x = RBe.x+len*sin(Eab);
		RBRoad.y = RBe.y+len*cos(Eab);
		RTRoad.x = RTe.x+len*sin(Tab);
		RTRoad.y = RTe.y+len*cos(Tab);
		makeline2(LTRoad,RTRoad,-1,lw,layname);
		makeline2(LBRoad,RBRoad,-1,lw,layname);
	}

	if(mode==2)//设计铁路在道路之下
	{
		makeline2(LT,RT,-1,lw,layname,L"DASHED2");
		makeline2(LB,RB,-1,lw,layname,L"DASHED2");
	}

	if (mode>3)//平交
	{
		//画所有平交都有的矩形框
		if (mode!=9)
		{
			makeline2(LT,RT,-1,lw,layname);
			makeline2(LB,RB,-1,lw,layname);
			makeline2(LT,LB,-1,lw,layname);
			makeline2(RT,RB,-1,lw,layname);
		}

		if (mode==5 || mode==7)//左看守平交或双看守平交
		{
			AcGePoint3d LT_PJ,RT_PJ;//左侧平交栏杆点
			getLineEndPt(LT,DrawParam.SCALE,aLb,LT_PJ);
			getLineEndPt(RT,DrawParam.SCALE,aLb,RT_PJ);
			makeline2(LT_PJ,RT_PJ,-1,lw,layname);
			makeDonut(AcGePoint2d(LT_PJ.x, LT_PJ.y), 0.6*lw, -1, layname);
			makeDonut(AcGePoint2d(RT_PJ.x, RT_PJ.y), 0.6*lw, -1, layname);
		}

		if (mode==6 || mode==7)//右看守平交或双看守平交
		{
			AcGePoint3d LB_PJ,RB_PJ;//右侧平交栏杆点
			getLineEndPt(LB,DrawParam.SCALE,aRe,LB_PJ);
			getLineEndPt(RB,DrawParam.SCALE,aRe,RB_PJ);
			makeline2(LB_PJ,RB_PJ,-1,lw,layname);
			makeDonut(AcGePoint2d(LB_PJ.x, LB_PJ.y), 0.6*lw, -1, layname);
			makeDonut(AcGePoint2d(RB_PJ.x, RB_PJ.y), 0.6*lw, -1, layname);
		}

		if (mode == 8)//自动化平交
		{
			AcGePoint3d CenPt;
			CenPt.x = 0.5*(sPt.x + ePt.x);
			CenPt.y = 0.5*(sPt.y + ePt.y);
			double Dist = 0.5 * sqrt( (sPt.x-ePt.x)*(sPt.x-ePt.x) + (sPt.y-ePt.y)*(sPt.y-ePt.y) );
			if (Dist > 0.5*(dL+dR))
				Dist = 0.5*(dL+dR);
			AcDbCircle *pCir=new AcDbCircle(CenPt, AcGeVector3d(0,0,1), Dist*sin(rad(Jiao)));
			pCir->setLayer(layname);
			AddEntityToDbs(pCir);
		}

		if (mode == 9)//新建平交道
		{
			makeline2(sPt,ePt,-1,lw,layname);
		}

	}

	///////////////四角///////////////////
	if (mode != 9)
	{
		makeline2(LT,LTe,-1,lw,layname);
		makeline2(LB,LBe,-1,lw,layname);
		makeline2(RT,RTe,-1,lw,layname);
		makeline2(RB,RBe,-1,lw,layname);
	}
} 

//绘涵洞:中心坐标，左宽，右宽，左高、右高，沿线路前进方向左转方位角，层名
void RHA::DrawCulvert(AcGePoint3d Pt0,double LTW,double RTW,double FWJ,ACHAR layname[80],bool IsAddWid) const
{
	LTW = fabs(LTW);
	RTW = fabs(RTW);
	double AddWide = 0.0;
	int LorR = 1;
	if (IsAddWid)
	{
		if (RTW>LTW-0.001)
		{
			LorR = 1;
			AddWide = 0.5 * RTW;
			RTW = 0.5 * RTW;
		}
		else
		{
			LorR = -1;
			AddWide = 0.5 * LTW;
			LTW = 0.5 * LTW;
		}
	}
	AcGePoint3d LT,RT,LTe1,RTe1,LTe2,RTe2;
	getLineEndPt(Pt0,LTW,FWJ+PI,LT);
	getLineEndPt(Pt0,RTW,FWJ,RT);

	getLineEndPt(LT,1.5*DrawParam.SCALE,FWJ+0.75*PI,LTe1);
	getLineEndPt(LT,1.5*DrawParam.SCALE,FWJ+1.25*PI,LTe2);
	getLineEndPt(RT,1.5*DrawParam.SCALE,FWJ-0.25*PI,RTe1);
	getLineEndPt(RT,1.5*DrawParam.SCALE,FWJ+0.25*PI,RTe2);

	double LW=DrawParam.OtherParam.EntAry[5].LW;
	double lw = LW;
	//if (layname == SmallBridge || layname == LeftSmallBridge || layname==RightSmallBridge) 818
		lw = 0.5*LW;

	///////////////四角///////////////////
	makeline(LT,LTe1,-1,lw*DrawParam.SCALE,layname);
	makeline(LT,LTe2,-1,lw*DrawParam.SCALE,layname);
	makeline(RT,RTe1,-1,lw*DrawParam.SCALE,layname);
	makeline(RT,RTe2,-1,lw*DrawParam.SCALE,layname);
	//////////////////////////////////////
	makeline(LT,RT,-1,lw*DrawParam.SCALE,layname,L"DASHED2");

	if (IsAddWid)
	{
		if (LorR == -1)
		{
			getLineEndPt(Pt0,LTW+AddWide,FWJ+PI,LT);
			getLineEndPt(Pt0,LTW,FWJ+PI,RT);
			getLineEndPt(LT,1.5*DrawParam.SCALE,FWJ+0.75*PI,LTe1);
			getLineEndPt(LT,1.5*DrawParam.SCALE,FWJ+1.25*PI,LTe2);
			lw = LW;
			makeline(LT,LTe1,-1,lw*DrawParam.SCALE,layname);
			makeline(LT,LTe2,-1,lw*DrawParam.SCALE,layname);
			makeline(LT,RT,-1,lw*DrawParam.SCALE,layname);
		}
		else
		{
			getLineEndPt(Pt0,RTW,FWJ,LT);
			getLineEndPt(Pt0,RTW+AddWide,FWJ,RT);
			getLineEndPt(RT,1.5*DrawParam.SCALE,FWJ-0.25*PI,RTe1);
			getLineEndPt(RT,1.5*DrawParam.SCALE,FWJ+0.25*PI,RTe2);
			lw = LW;
			makeline(RT,RTe1,-1,lw*DrawParam.SCALE,layname);
			makeline(RT,RTe2,-1,lw*DrawParam.SCALE,layname);
			makeline(LT,RT,-1,lw*DrawParam.SCALE,layname);
		}
	}
}


//插旗方式 0-普通 1-上立交 2-下立交 3-无看守平交 
//4-左看守平交 5-右看守平交 6-双看守平交 7-自动化平交

void RHA::INSERT_SMALL_BRIDGE(ACHAR gh[], double xclc0, CString NameCross, int LineNum, bool GJXZ
								   , int LMRFlag, int iRXD, double LTW, double RTW, double LLevel
								   , double RLevel, double jiao, int InsertStyle, double dCulvertUse) const
{
	ACHAR layname[256];
	int zybz = -1;//默认标注在左侧 818

	_tcscpy(layname,  DrawParam.SmallBridge);

	////////////////////////////////////////////////////////////////	
	AcDbObjectIdArray IdAr;
	double lc0;//中心统一里程
	ACHAR xlc0[50];
	CString lcchr;

	//计算投影里程
	_tcscpy(xlc0, LCchr(gh, xclc0, DrawParam.NLC));
	TrsCkmltoProjml(xlc0, lc0);

	double a0;//线路方位角/桥中线方位角
	xlpoint Ptmp;
	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);
	AcGePoint3d Pt0(Ptmp.y, Ptmp.x, 0.0);//桥梁中心坐标（主线）
	a0 = Ptmp.a;

	//处理接长涵洞-----------------------------------------------------------------------
	double FWJ_Jc = 0.0;//接长涵洞的方位角
	if((InsertStyle == 0) && (fabs(LTW)>0.1) && (fabs(RTW)>0.1) 
		&& (m_XLName.Find(L"设计")>=0))//0-普通 涵洞 	
	{
		int pos = NameCross.Find(L"接长");
		if(pos>0)
			return;
	}
	

	//////////////////标注///////
	AcGePoint3d eBZPt;//标注线的终点，起点为Pt0;
	AcGePoint3d Pbz1, Pbz2;//标注点;
	ACHAR Style[80], Lc[80];

	_tcscpy(Style, NameCross);
	double lc=xclc0;
	lc=lc-(int)(lc/1000+0.0001)*1000.0 + 1000.0;
	ads_rtos(lc, 2, DrawParam.NCQ, Lc);
	Lc[0]='+';
	int iSty=_tcslen(NameCross);
	int iNam=_tcslen(Lc);
	int MaxStr=iSty>iNam?iSty:iNam; 
	double WB=0.0;

	/*if (m_IsAutoLorR && pGTSJEX) 818 自动计算，还是用户给？
	{
		zybz = CalInsFlagLorR(Pt0.y, Pt0.x, m_XLName);
	}*/

	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*zybz);//+0.5*texth*sin(B);
	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*zybz);//+0.5*texth*cos(B);
	makeline2(Pt0, eBZPt, -1, AcDb::kLnWt035, layname);

	//既有线上设置为改建的小桥涵打叉
	if ((m_XLName.Find(L"既有")>=0) && GJXZ==true && InsertStyle>2.5)
	{
		double NCross, ECross;
		ECross = Pt0.x+(0.48*MaxStr*texth)*sin(a0+pi*0.5*zybz);
		NCross = Pt0.y+(0.48*MaxStr*texth)*cos(a0+pi*0.5*zybz);
		DrawOneFQDBZ(NCross, ECross, a0+pi*0.5, _tcslen(xlc0)*0.3*texth, LMRFlag);
	}

	//标注孔跨样式和里程
	xlpoint PZtmp;
	double B;

	PZtmp.lc=lc0-(0.5*texth+1.0*DrawParam.SCALE);//818 字高
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	Pbz1[X]=PZtmp.y+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth)*sin(B+pi*0.5*zybz) ;
	Pbz1[Y]=PZtmp.x+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth)*cos(B+pi*0.5*zybz) ;
	
	PZtmp.lc=lc0+(0.5*texth+1.0*DrawParam.SCALE);
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	Pbz2[X]=PZtmp.y+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth)*sin(B+pi*0.5*zybz) ;
	Pbz2[Y]=PZtmp.x+(WB+0.5*DrawParam.SCALE+0.5*MaxStr*texth)*cos(B+pi*0.5*zybz) ;


	double b=B-pi*0.5;
	b=pi*0.5-b; 
	FormFwj(b);

	maketext(Pbz1, Style, b, texth, -1, 3, L"宋体", layname);
	maketext(Pbz2, Lc, b, texth, -1, 3, L"宋体", layname);

	bool IsXWT = true;//是否线位图
	if (IsXWT)//818 都是线位图？
	{
		AcGePoint3d sPt, ePt;//桥起终端中心点

		if(InsertStyle == 0)//普通小桥涵
		{
			if ((fabs(LTW)<0.1) || (fabs(RTW)<0.1))//左右宽度没有指定，或者是孔跨式样中有“桥”都认为是小桥
			{
				//卡出桥长
				CString LenStr;
				double Length;

				int sIX=NameCross.Find(L"-")+1;//孔跨式样例子： 1-1.5m圆涵
				int eIX=NameCross.Find(L"m")-1;

				if (sIX<0 || eIX<0)
				{
					CString Err;
					Err.Format(L"%s小桥涵%s的孔跨式样中缺少'-'或'm', 系统将该小桥涵跨默认为5m！\n", xlc0, NameCross); 
					ads_printf(L"%s\n", Err);
					Length = 5;
				}
				else
				{
					LenStr=NameCross.Mid(sIX, eIX-sIX+1);
					Length=_wtof(LenStr);
				}

				if (Length>0)
				{
					getLineEndPt(Pt0, 0.5*Length, a0, ePt);
					getLineEndPt(Pt0, 0.5*Length, a0+PI, sPt);
				} 
				else
				{
					getLineEndPt(Pt0, 5, a0, ePt);
					getLineEndPt(Pt0, 5, a0+PI, sPt);
				}

				DrawSmallBridge(sPt, ePt, layname, LineNum, 0, LTW, RTW, LMRFlag, lc0);	
			}
			else if (NameCross.FindOneOf(L"桥")>-1)
			{
				if (LLevel<0.1 && RLevel<0.1)
				{
					ads_printf(L"%s小桥涵没有指定桥长，系统将该小桥涵跨默认为5m\n",  NameCross);
					LLevel = RLevel = 2.5;
				}

				getLineEndPt(Pt0, RLevel, a0, ePt);
				getLineEndPt(Pt0, LLevel, a0+PI, sPt);
				DrawSmallBridge(sPt, ePt, layname, LineNum, 0, LTW, RTW, LMRFlag, lc0);	
			}
			else
			{
				double FWJ=a0+rad(jiao);
				int pos = NameCross.Find(L"接长");

				if(pos > 0 && (XLShuXing == SJJX ||XLShuXing == SJ2X))
				{
					FWJ = FWJ_Jc;
				}

				
				if (NameCross.FindOneOf(L"接长")>-1)
				{
					//考虑接长
					DrawCulvert(Pt0, LTW, RTW, FWJ, layname, true);
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					//现在通过传入参数nXS确定单、双线桥
					
					if (LTW<-0.001 || RTW<-0.001)
					{//没有输横向长，自动计算
						LTW = 4.0;
						RTW = 4.0;
						//if (LMRFlag==MAINLINE)  818
						//{
						//	double resPt[2], lc2x;
						//	double xjj=0.0;

						//	double pt[3] ={Ptmp.x, Ptmp.y, 0.0};
						//	if(HasLeft && LineNum==2)//存在左线
						//	{

						//		pJYJX->Search_EXPt(arrayLx, tarrayLx, pt, FWJ, resPt);
						//		xjj = sqrt((resPt[0]-pt[0])*(resPt[0]-pt[0])+(resPt[1]-pt[1])*(resPt[1]-pt[1]));
						//		LTW=xjj+4;

						//	}
						//	if(HasRight && LineNum==2)//存在右线
						//	{
						//		pJYJX->Search_EXPt(arrayRx, tarrayRx, pt, FWJ, resPt);
						//		xjj = sqrt((resPt[0]-pt[0])*(resPt[0]-pt[0])+(resPt[1]-pt[1])*(resPt[1]-pt[1]));
						//		RTW=xjj+4;
						//	}	
						//}

					}
					DrawCulvert(Pt0, LTW, RTW, FWJ, layname, false);
				}

				if (fabs(LLevel)>0.01 || fabs(RLevel)>0.01)
				{
					a0 = 2.5*PI-a0;
					if (draw_zybz>0)
						a0 = a0+PI;
					if (a0>2.0*PI)
						a0 = a0 - 2.0*PI;
					if (a0 < 2.0*PI)
						a0 = a0 + 2.0*PI;
					double AngEdgeLen=1.25*DrawParam.SCALE;//三角形边长
					//标注涵口标高
					AcGePoint3d LPtText, RPtText;//三角形下面顶点，注字左对齐点
					AcGePoint3d LPt1, LPt2, RPt1, RPt2;//三角形上面两顶点

					//计算三角形下面顶点，注字左对齐点
					getLineEndPt(Pt0, (LTW+2.0*AngEdgeLen), FWJ+PI, LPtText);
					getLineEndPt(Pt0, (RTW+2.0*AngEdgeLen), FWJ, RPtText);

					//计算三角形上面两顶点  画三角形  标注涵口标高
					ACHAR LLevStr[20], RLevStr[20];
					if (fabs(LLevel)>0.01)
					{
						getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*1.125, LPt1);
						getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*0.875, LPt2);
						makeline(LPtText, LPt1, -1, AcDb::kLnWt000, layname);
						makeline(LPtText, LPt2, -1, AcDb::kLnWt000, layname);
						makeline(LPt1, LPt2, -1, AcDb::kLnWt000, layname);
						_stprintf(LLevStr, L" %.2lf", LLevel);
						maketext(LPtText, LLevStr, a0, texth, -1, 1, L"宋体", layname);
					}
					if (fabs(RLevel)>0.01)
					{
						getLineEndPt(RPtText, AngEdgeLen, FWJ+PI/8.0, RPt1);
						getLineEndPt(RPtText, AngEdgeLen, FWJ-PI/8.0, RPt2);
						makeline(RPtText, RPt1, -1, AcDb::kLnWt000, layname);
						makeline(RPtText, RPt2, -1, AcDb::kLnWt000, layname);
						makeline(RPt1, RPt2, -1, AcDb::kLnWt000, layname);	
						_stprintf(RLevStr, L" %.2lf", RLevel);
						maketext(RPtText, RLevStr, a0, texth, -1, 1, L"宋体", layname);
					}
				}
			}
		}
		else if((InsertStyle>0.5) && (InsertStyle<2.5))//处理立交
		{
			return;
			double FWJ=a0+rad(jiao);
			if(LTW>0.0001)//如果左右横向长没有输入默认为5
				getLineEndPt(Pt0, LTW, FWJ+PI, ePt);
			else
				getLineEndPt(Pt0, 5, FWJ+PI, ePt);
			if(RTW>0.0001)
				getLineEndPt(Pt0, RTW, FWJ, sPt);
			else
				getLineEndPt(Pt0, 5, FWJ, sPt);

			if(fabs(InsertStyle-2.0)<0.001)
				DrawSmallBridge(sPt, ePt, layname, LineNum, 2, LLevel, RLevel, LMRFlag, lc0);//铁路在下
			else
				DrawSmallBridge(sPt, ePt, layname, LineNum, 1, LLevel, RLevel, LMRFlag, lc0);

		}
		else//平交道口
		{
			if (GJXZ == 0)//L"既有"
			{
				if (LLevel<0.1 && RLevel<0.1)
					LLevel = RLevel = 2.5;

				getLineEndPt(Pt0, RLevel, a0, ePt);
				getLineEndPt(Pt0, LLevel, a0+PI, sPt);
				DrawSmallBridge(sPt, ePt, layname, LineNum,  (int)(InsertStyle+1), LTW, RTW, LMRFlag, lc0, jiao);
			}
			else
			{
				if (LTW<0.1 && RTW<0.1)
					LTW = RTW = 2.5;
				double FWJ=a0+rad(jiao);
				double LW=DrawParam.OtherParam.EntAry[5].LW;


				getLineEndPt(Pt0, RTW, FWJ, ePt);
				getLineEndPt(Pt0, LTW, FWJ+PI, sPt);
				makeline2(sPt,  ePt,  -1,  0.5*LW*DrawParam.SCALE, layname);
			}
		}
	}
	else
	{
		//下面用于排水用地 818
		//////////////////////////////////////////////////////////
		////plh08.20
		//AcGePoint3d sPt, ePt;//桥起终端中心点
		////if(NameCross.Find(L"桥")>=0)//小桥

		//if((InsertStyle>-0.5) && (InsertStyle<0.5))
		//{
		//	if ((fabs(LTW)<0.1) || (fabs(RTW)<0.1))
		//	{
		//		//卡出桥长
		//		CString LenStr;
		//		double Length;
		//		int sIX=NameCross.Find(L"-")+1;
		//		int eIX=NameCross.Find(L"m")-1;
		//		if (sIX<0 || eIX<0)
		//		{
		//			CString Err;
		//			Err = xclc0 + L"小桥涵" + NameCross + "的孔跨式样中缺少'-'或'm', 系统将该小桥涵跨默认为5m！\n";
		//			ads_printf(L"%s\n", Err);
		//			Length = 5;
		//		}
		//		else
		//		{
		//			LenStr=NameCross.Mid(sIX, eIX-sIX+1);
		//			Length=_wtof(LenStr);
		//		}

		//		if (Length>0)
		//		{
		//			getLineEndPt(Pt0, 0.5*Length, a0, ePt);
		//			getLineEndPt(Pt0, 0.5*Length, a0+PI, sPt);
		//		} 
		//		else
		//		{
		//			getLineEndPt(Pt0, 5, a0, ePt);
		//			getLineEndPt(Pt0, 5, a0+PI, sPt);
		//		}

		//		DrawSmallBridge(sPt, ePt, layname, LineNum, 0, LLevel, RLevel, LMRFlag, lc0);	
		//	}

		//	else
		//	{
		//		double FWJ=a0+rad(jiao);
		//		int pos = NameCross.Find(L"接长");
		//		if(pos > 0 && (XLName == L"贯通设计基线" || XLName == L"贯通设计Ⅱ线"))
		//		{
		//			FWJ = FWJ_Jc;
		//		}

		//		if (dCulvertUse == 1)//交通涵
		//		{
		//			getLineEndPt(Pt0, 0.5*2, a0, ePt);
		//			getLineEndPt(Pt0, 0.5*2, a0+PI, sPt);

		//			DrawCulvert_JiaoTong(sPt, ePt, LTW, RTW, FWJ, layname);
		//		}
		//		else
		//		{
		//			if (NameCross.FindOneOf(L"接长")>-1)
		//				DrawCulvert(Pt0, LTW, RTW, FWJ, layname, true);
		//			else
		//				DrawCulvert(Pt0, LTW, RTW, FWJ, layname, false);
		//		}

		//		if (fabs(LLevel)>0.01 || fabs(RLevel)>0.01)
		//		{
		//			a0 = 2.5*PI-a0;
		//			if (draw_zybz>0)
		//				a0 = a0+PI;
		//			if (a0>2.0*PI)
		//				a0 = a0 - 2.0*PI;
		//			if (a0 < 2.0*PI)
		//				a0 = a0 + 2.0*PI;
		//			double AngEdgeLen=1.25*DrawParam.SCALE;//三角形边长
		//			//标注涵口标高
		//			AcGePoint3d LPtText, RPtText;//三角形下面顶点，注字左对齐点
		//			AcGePoint3d LPt1, LPt2, RPt1, RPt2;//三角形上面两顶点

		//			//计算三角形下面顶点，注字左对齐点
		//			getLineEndPt(Pt0, (LTW+2.0*AngEdgeLen), FWJ+PI, LPtText);
		//			getLineEndPt(Pt0, (RTW+2.0*AngEdgeLen), FWJ, RPtText);

		//			//计算三角形上面两顶点  画三角形  标注涵口标高
		//			ACHAR LLevStr[20], RLevStr[20];
		//			if (fabs(LLevel)>0.01)
		//			{
		//				getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*1.125, LPt1);
		//				getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*0.875, LPt2);
		//				makeline(LPtText, LPt1, -1, AcDb::kLnWt000, layname);
		//				makeline(LPtText, LPt2, -1, AcDb::kLnWt000, layname);
		//				makeline(LPt1, LPt2, -1, AcDb::kLnWt000, layname);
		//				_stprintf(LLevStr, L" %.2lf", LLevel);
		//				maketext(LPtText, LLevStr, a0, texth, -1, 1, L"宋体", layname);
		//			}
		//			if (fabs(RLevel)>0.01)
		//			{
		//				getLineEndPt(RPtText, AngEdgeLen, FWJ+PI/8.0, RPt1);
		//				getLineEndPt(RPtText, AngEdgeLen, FWJ-PI/8.0, RPt2);
		//				makeline(RPtText, RPt1, -1, AcDb::kLnWt000, layname);
		//				makeline(RPtText, RPt2, -1, AcDb::kLnWt000, layname);
		//				makeline(RPt1, RPt2, -1, AcDb::kLnWt000, layname);	
		//				_stprintf(RLevStr, L" %.2lf", RLevel);
		//				maketext(RPtText, RLevStr, a0, texth, -1, 1, L"宋体", layname);
		//			}
		//		}
		//	}

		//}
		////else if(NameCross.Find(L"涵")>=0)//涵
		//else if((InsertStyle>0.5) && (InsertStyle<2.5))		
		//{
		//	if ((fabs(LTW)<0.1) || (fabs(RTW)<0.1))
		//	{
		//		CString LenStr;
		//		double Length;
		//		int sIX=NameCross.Find(L"-")+1;
		//		int eIX=NameCross.Find(L"m")-1;
		//		if (sIX<0 || eIX<0)
		//		{
		//			CString Err;
		//			Err = xclc0 + L"小桥涵" + NameCross + "的孔跨式样中缺少'-'或'm', 系统将该小桥涵跨默认为5m！\n";
		//			ads_printf(L"%s\n", Err);
		//			Length = 5;
		//		}
		//		else
		//		{
		//			LenStr=NameCross.Mid(sIX, eIX-sIX+1);
		//			Length=_wtof(LenStr);
		//		}
		//		LTW = RTW = 0.5*Length;
		//	}

		//	double FWJ=a0+rad(jiao);
		//	if (LTW>0)
		//		getLineEndPt(Pt0, LTW, FWJ+PI, ePt);
		//	else
		//		getLineEndPt(Pt0, 5, FWJ+PI, ePt);
		//	if(RTW>0)
		//		getLineEndPt(Pt0, RTW, FWJ, sPt);
		//	else
		//		getLineEndPt(Pt0, 5, FWJ, sPt);

		//	if(fabs(InsertStyle-2.0)<0.001)
		//		DrawSmallBridge(sPt, ePt, layname, LineNum, 2, LLevel, RLevel, LMRFlag, lc0);//铁路在下
		//	else
		//		DrawSmallBridge(sPt, ePt, layname, LineNum, 1, LLevel, RLevel, LMRFlag, lc0);
		//}
		////else if(NameCross.Find(L"平交")>=0)//平交
		//else if(InsertStyle>2.5)	
		//{

		//}
		////CreateGroup(NameLc,IdAr);

	}

}




//标注线间距
void RHA::BZXJJ() const 
{
	AcGePoint3d Pt1,Pt2,Pt0;
	double zhlc,hzlc,lc2x,lc1x;
	double xjj;
	double fwj;
	xlpoint PZ;
	ACHAR XJJ[20];
	ACHAR LayerName[256];
	double offset = 0.0;
	double angl;
	RHA *pRHA_BL=NULL;


	_tcscpy(LayerName, DrawParam.XianJianJu);
	if ( DrawParam.SCALE>9.9)
		offset = DrawParam.m_ExOffset;

	int textdir=DrawParam.m_TextDir;

	//标注S弯线间距 818 ？？
	for(int i=0;i+1<JDarray.size();i++)//818 最后一个交点不标注?
	{
		//绕行段、并行不平行段不标线间距
		if( ((JDarray[i].JDXZ == RX_Start || JDarray[i].JDXZ == RX_One) 
			&& (JDarray[i+1].JDXZ == RX_One || JDarray[i+1].JDXZ == RX_End)) 
			|| (JDarray[i].JDXZ == BX_NotPX && JDarray[i+1].JDXZ == BX_NotPX)
			|| (JDarray[i].JDXZ==S_Start&&JDarray[i+1].JDXZ==S_End)  )
			continue;

		hzlc=QXB[i].KHZ;
		zhlc=QXB[i+1].KZH;

		int iRXD=-1;
		int sdl, edl;
		if (JDarray[i].JDXZ == RX_End || JDarray[i].JDXZ == RX_Start)
		{
			//前有绕行止(包括单点绕行)，标在绕行止断链和ZH点中间
			iRXD=GetiRxdByProjml(QXB[i].KZH);
			if(iRXD>-1)
			{
				GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
				lc2x=0.5*(DLarray[edl].TYLC+zhlc);
			}
			else
				continue;

		}
		else if(JDarray[i+1].JDXZ == RX_Start)
		{
			//后有绕行始，标在HZ点和绕行始断链中间
			iRXD=GetiRxdByProjml(QXB[i+1].KHZ);
			if(iRXD>-1)
			{
				GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
				lc2x=0.5*(hzlc+DLarray[sdl].TYLC);
			}
			else
				continue;

		}
		else
		{
			//并行标在夹直线中间
			lc2x=0.5*(zhlc+hzlc);
		}

		if(JDarray[i+1].JDXZ==S_Start)
			lc2x=zhlc-10*texth;
		else if(JDarray[i].JDXZ==S_End)
			lc2x=hzlc+10*texth;

		PZ.lc=lc2x;
		xlpoint_pz(array,tarray,PZ);
		angl=3.0*PI-PZ.a;
		Pt1.x=PZ.y;
		Pt1.y=PZ.x;//Pt1 二线点

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[i].BLName);
		if (pRHA_BL == NULL)
			continue;

		lc1x=PROJ_ML(pRHA_BL->array,pRHA_BL->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PZ.lc=lc1x;
		xlpoint_pz(pRHA_BL->array,pRHA_BL->tarray,PZ);
		Pt2.x=PZ.y;
		Pt2.y=PZ.x;//Pt2 一线点
		fwj=PZ.a;

		_stprintf(XJJ,L"%.2lf",fabs(JDarray[i].XJJ));//plh05.06

		if(_wtof(XJJ)<0.01)
			continue;

		PZ.lc=lc2x-0.8*texth;
		xlpoint_pz(array,tarray,PZ);


		//标注线间距的字的位置
		Pt0.x=PZ.y+0.8*_tcslen(XJJ)*texth*sin(PZ.a+0.5*PI);
		Pt0.y=PZ.x+0.8*_tcslen(XJJ)*texth*cos(PZ.a+0.5*PI);
		angl=angleX(Pt1,Pt2);
		if(textdir<0)
			angl-=pi;
		
		maketext(Pt0,XJJ,angl,texth,-1,3,L"宋体",LayerName);


		//标箭头
		Pt1.y = Pt1.y + offset * cos(fwj + 0.5*PI);
		Pt1.x = Pt1.x + offset * sin(fwj + 0.5*PI);
		makeXJJFlag(Pt1,Pt2,(1+_tcslen(XJJ))*texth,-1,0.3,LayerName,L"CONTINUOUS");		
	}

}


//标注线间距：1 线上一点，2 线上一点，标注线长度=20mm*SCALE，。。。。
void RHA::makeXJJFlag(AcGePoint3d Pt1,AcGePoint3d Pt2,double len,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType,double jiao) const
{

	if (jiao<-99)
	{
		jiao=CalFWJ(Pt1.x, Pt1.y, Pt2.x, Pt2.y);
	}
	AcGePoint3d Pt3,Pt4,Pt5,Pt6;

	getLineEndPt(Pt1,len,jiao+pi,Pt3);
	getLineEndPt(Pt1,0.3*len,jiao-0.1*pi+pi,Pt5);

	getLineEndPt(Pt2,len,jiao,Pt4);
	getLineEndPt(Pt2,0.3*len,jiao-0.1*pi,Pt6);

	AcGePoint2dArray Pt2dAy;
	Pt2dAy.append(AcGePoint2d(Pt1.x,Pt1.y));
	Pt2dAy.append(AcGePoint2d(Pt3.x,Pt3.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	Pt2dAy.removeSubArray(0,1);
	Pt2dAy.append(AcGePoint2d(Pt1.x,Pt1.y));
	Pt2dAy.append(AcGePoint2d(Pt5.x,Pt5.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	Pt2dAy.removeSubArray(0,1);
	Pt2dAy.append(AcGePoint2d(Pt2.x,Pt2.y));
	Pt2dAy.append(AcGePoint2d(Pt4.x,Pt4.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	Pt2dAy.removeSubArray(0,1);
	Pt2dAy.append(AcGePoint2d(Pt2.x,Pt2.y));
	Pt2dAy.append(AcGePoint2d(Pt6.x,Pt6.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	makeline2(Pt1,Pt3,icolor,lw,LayerName,LineType);
	makeline2(Pt1,Pt5,icolor,lw,LayerName,LineType);
	makeline2(Pt2,Pt4,icolor,lw,LayerName,LineType);
	makeline2(Pt2,Pt6,icolor,lw,LayerName,LineType);
}

//设置交点曲线要素8.30-zy
void RHA::setJD_RL(int JDi ,double Ri,double L1i,double L2i )
{
	assertWriteEnabled();
	int NJD=JDarray.size();
	if (JDi>0 && JDi<NJD-1)  // 不为起、终点
	{  
		JDarray[JDi].l1=L1i;
		JDarray[JDi].l2=L2i;
		JDarray[JDi].R=Ri;
	}
	else
		AfxMessageBox(L"该交点为起点或终点，无法设置半径缓长");
	CalDLprojml(0);//从起点开始，重新计算断链投影里程
} 

//计算同心圆8.31-zy
//根据线间距（包含二线内外的判断)，加宽值，半径缓长计算出加宽后半径缓长
bool RHA::CalTXY(double XianJianJu,double JiaKuan,double BanJing1,double HuanChang1,
				   double& BanJing2,double& HuanChang2)
{
	double B = 1.1;
	BanJing1=fabs(BanJing1);
	if (BanJing1<0.01)
	{
		BanJing1 = 0.0;
		BanJing2 = 0.0;
		HuanChang1 = 0.0;
		HuanChang2 = 0.0;
		return true;
	}
	if (BanJing1 > 0.0)
	{
		B = BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
		double jianju = 0.0;

		if (JiaKuan == 0.0)//不需加宽，同缓长
		{
			HuanChang2 = HuanChang1;
			jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
			BanJing2 = (B + jianju) / 2.0;
			jianju = fabs(BanJing1 - BanJing2);
			if (jianju < fabs(XianJianJu))
			{
				ads_printf(L"曲线线间距小于最小线间距\n");
				BanJing2 = BanJing1;
				HuanChang2 = HuanChang1;
				//BanJing2=BanJing2*isHTQX;
				return false;
			}
		}
		else//加宽
		{
			if(XianJianJu > 0.0)//右线在内侧
			{
				jianju=sqrt((BanJing1 - XianJianJu - JiaKuan) * (HuanChang1 * HuanChang1 / BanJing1 + 24.0 * JiaKuan));//估算内侧缓长
				HuanChang2 = (int)(jianju/10.0+1.0)*10;//取整到10米
				jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
				BanJing2 = (B + jianju)/2.0;
				jianju=BanJing1 - BanJing2;
			}
			else if (XianJianJu < 0.0)//右线在外侧
			{
				if (HuanChang1 > 0.0)//输入了基线缓长，缩短二线缓长
				{
					//减小右线缓长
					jianju = sqrt((BanJing1 - XianJianJu + JiaKuan) * (HuanChang1 * HuanChang1 / BanJing1 - 24.0 * JiaKuan));
					HuanChang2 = (int)(jianju / 10.0) * 10;

					//ji suan you xian ban jing
					B=BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
					jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
					BanJing2 = (B + jianju) / 2.0;
					jianju=BanJing1 - BanJing2;
				}
				else if (HuanChang2 > 0.0)//基线缓长0，即要由二线缓长延长基线缓长
				{
					jianju=sqrt(BanJing1 * (HuanChang2 * HuanChang2 / (BanJing1 - XianJianJu + JiaKuan) + 24.0 * JiaKuan));//估算内侧缓长
					HuanChang1 = (int)(jianju / 10.0 + 1.0) * 10.0;//取整到10米

					//ji suan you xian ban jing
					B=BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
					jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
					BanJing2 = (B + jianju) / 2.0;
					jianju=BanJing1 - BanJing2;
				}
				else
				{
					//AfxMessageBox(L"基线和二线的缓长都是0，不能计算\n");
					BanJing2 = BanJing1;
					HuanChang2 = HuanChang1;
					//BanJing2=BanJing2*isHTQX;
					return false;
				}
			}
			else
			{
				BanJing2 = BanJing1;
				HuanChang2 = HuanChang1;
				//BanJing2=BanJing2*isHTQX;
				return true;
			}
		}
	}
	else
	{
		ads_printf(L"设计基线的曲线半径错误，不能计算\n");
		BanJing2 = BanJing1;
		HuanChang2 = HuanChang1;
		//BanJing2=BanJing2*isHTQX;
		return false;
	}
	//BanJing2=BanJing2*isHTQX;
	return true;
}



void RHA::CalOneBXJDdata(int iJD,const RHA *m_pm1)
//计算某并行交点
////JDarray 是二线交点数据
{
	//	assertWriteEnabled();
	double lo,lo1,D;
	int i=iJD;

	//isJYGuiFanCS的取值？
	bool isJYGuiFanCS=false;
	if (XLShuXing == JYJX || XLShuXing == JY2X)
		isJYGuiFanCS=true;

	if(JDarray[i].JDXZ==BXPX)//并行段交点
	{
		//_tcscpy(JDarray[i].JDNum,JD2[i].JDNum);//暂时隐去，JDarray和JD2分别代表？
		//int jdn1 = JDarray[i].jdno1;//对应一线交点序号
		BAS_CALC_FUN ob;
		int jdn1;
		ob.GetJdno1FromJdstr(JDarray[i].BLJDName, m_pm1->JDarray, jdn1);//根据二线交点BLJDName求参照线（一线）交点序号jdn1
		int NJD=JDarray.size();
		if(i==NJD-1)
			JDarray[i].XJJ = JDarray[i-1].XJJ;//最后一个点的线间距等于前一交点的//JDarray 是二线，只二线有xjj？
		double xjj = JDarray[i].XJJ;
		double EXJD[2];

		CalEXJD(jdn1, i, EXJD, m_pm1);//增加指针参数m_pm1
		JDarray[i].N = EXJD[0];
		JDarray[i].E = EXJD[1];//计算出二线交点
		double R = m_pm1->JDarray[jdn1].R;
		double JK;
		JK = get_XJJK(m_pm1->DV,R,isJYGuiFanCS);//线间距加宽
		JDarray[i].QXJK = JK;
		JDarray[i].l1 = m_pm1->JDarray[jdn1].l1;
		JDarray[i].R = R;
		JDarray[i].l2 = m_pm1->JDarray[jdn1].l2;           
		lo1 = m_pm1->JDarray[jdn1].l1;//一线缓长
		D = fabs(xjj);
		//二线半径
		if(i>0&&i<NJD-1)
		{
			double xjj2;
			xjj2 = JDarray[i-1].XJJ;//后一条边线间距
			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
			if(fabs(xjj-xjj2)<0.01)//前后线间距相等
			{
				////xjj考虑曲线加宽............		   
				//if(xjj>0)//一线为左线
				//{
				//	if(m_pm1->QXB[jdn1].LorR<0)//左转,II线在外侧，减少缓长
				//	{
				//		//先配缓长
				//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//彭先宝 2003.12.4提供
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;	
				//		D  = -1.0*D;
				//	}
				//	else
				//	{
				//		//先配缓长
				//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//彭 2003.12.4提供
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;						  					   
				//	}
				//}
				//else//一线为右线
				//{
				//	if(m_pm1->QXB[jdn1].LorR<0)//左转,II线在内侧
				//	{
				//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//彭 2003.12.4提供
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;						  					   
				//	}
				//	else
				//	{   //先配缓长
				//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//彭 2003.12.4提供
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;	
				//		D  = -1.0*D;
				//	}
				//}
				////配半径
				//double B = R - D + (lo1*lo1)/24/R;
				//JDarray[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//二线半径

				if (fabs(JDarray[i].XJJ)<0.001)
				{
					JDarray[i].R=m_pm1->JDarray[jdn1].R;
					JDarray[i].l1=m_pm1->JDarray[jdn1].l1;
					JDarray[i].l2=m_pm1->JDarray[jdn1].l2;
				}
				else
				{
					CalTXY(m_pm1->QXB[jdn1].LorR * JDarray[i].XJJ,JDarray[i].QXJK,R,lo1,JDarray[i].R,lo);
					JDarray[i].l2 = JDarray[i].l1 = lo;
				}
			}
		}   
		//JDarray[i].jdno = JD0 + i;//暂时隐掉-8.31-zy-jdno作何用？
	}//并行
}


/*
函数功能：求解并行段二线交点的交点坐标
输入参数：一线交点号，对应二线交点号
输出参数：二线交点坐标
算法概述：1.如果是线路的起点或者线路终点由对应基线交点坐标和线间距求出
2.其它情况有对应基线交点的前后边根据相应的线间距偏移然后求交，解算出交点即为二线相应的交点坐标
*/
//输入一线交点号，二线对应交点号，返回并行段二线交点坐标
void RHA::CalEXJD(int jdn1,int njd2,double EXJD[2], const RHA *m_pm1)
{

	double xjj1,xjj2,fwj;

	if(jdn1<m_pm1->JDarray.size()-1)
		xyddaa(m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,m_pm1->JDarray[jdn1+1].N,m_pm1->JDarray[jdn1+1].E,&fwj);
	else
		xyddaa(m_pm1->JDarray[jdn1-1].N,m_pm1->JDarray[jdn1-1].E,m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,&fwj);

	if(njd2==0|| njd2==JDarray.size()-1)//起始交点//plh05.06
	{
		//由该点在一线上的投影里程和线间距求得
		//要求GTZX2的声终点的ZHLC要给

		if(jdn1==0|| jdn1==m_pm1->JDarray.size()-1)
		{
			double xjj;
			xjj = JDarray[njd2].XJJ;
			EXJD[0]  =  m_pm1->JDarray[jdn1].N + xjj * cos(fwj+m_dHalfPI);
			EXJD[1]  =  m_pm1->JDarray[jdn1].E + xjj * sin(fwj+m_dHalfPI);//计算出二线交点
		}
		else
		{
			EXJD[0]=JDarray[njd2].N;
			EXJD[1]=JDarray[njd2].E;
		}
	}
	else 
	{
		double fwj;
		xjj1 = JDarray[njd2-1].XJJ;//njd2-1-njd2边的线间距
		xjj2 = JDarray[njd2].XJJ;//njd2-njd2+1边的线间距

		xyddaa(m_pm1->JDarray[jdn1-1].N,m_pm1->JDarray[jdn1-1].E,m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,&fwj);
		double pt1[2],pt2[2];
		pt1[0] = m_pm1->JDarray[jdn1-1].N + xjj1 * cos(fwj+m_dHalfPI);
		pt1[1] = m_pm1->JDarray[jdn1-1].E + xjj1 * sin(fwj+m_dHalfPI);
		pt2[0] = m_pm1->JDarray[jdn1].N + xjj1 * cos(fwj+m_dHalfPI);
		pt2[1] = m_pm1->JDarray[jdn1].E + xjj1 * sin(fwj+m_dHalfPI);
		double pt3[2],pt4[2];
		xyddaa(m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,m_pm1->JDarray[jdn1+1].N,m_pm1->JDarray[jdn1+1].E,&fwj);
		pt3[0] = m_pm1->JDarray[jdn1].N + xjj2 * cos(fwj+m_dHalfPI);
		pt3[1] = m_pm1->JDarray[jdn1].E + xjj2 * sin(fwj+m_dHalfPI);
		pt4[0] = m_pm1->JDarray[jdn1+1].N + xjj2 * cos(fwj+m_dHalfPI);
		pt4[1] = m_pm1->JDarray[jdn1+1].E + xjj2 * sin(fwj+m_dHalfPI);
		//找交点

		double dist;
		dist = sqrt((pt2[0]-pt3[0])*(pt2[0]-pt3[0])+(pt2[1]-pt3[1])*(pt2[1]-pt3[1]));
		if(dist<0.001)
		{
			EXJD[0] = pt2[0];
			EXJD[1] = pt2[1];
		}
		else
			INTERS(&EXJD[0],&EXJD[1],pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

	}
}


//功能：判断在拾取点在线路起始边还是终止边
//输入：pt           拾取点
//输出：SLorEL       在起始边，为true；反之为false
//返回：如果在起始边或终止边，返回true；如果不在，返回false
//ls 17.9.4
bool RHA::IsPtonSLorEL(AcGePoint3d pt, bool &isSLorEL) const
{
	double projml=0.0;
	int jdNum=JDarray.size();

	isSLorEL=true;
	//计算投影里程
	TrsNEtoProjml(pt.y, pt.x, projml);

	if(JDarray.size() == 2)
	{//线路为一条直线边的情况
		double zdProjml=QXB[1].KHZ;
		if(projml<zdProjml-projml)
			isSLorEL=true;
		else
			isSLorEL=false;
	}
	else
	{
		if (projml+0.001>0 && projml<(QXB[1].KHY+QXB[1].KYH)/2.0+0.001)
			isSLorEL=true;
		else if (jdNum == QXB.size()
			&& projml+0.001>(QXB[jdNum-2].KHY+QXB[jdNum-2].KYH)/2.0
			&& projml<QXB[jdNum-1].KHZ+0.001)
			isSLorEL=false;
		else
			return false;
	}

	return true;
}

//功能：合并交点数组
//输入：pt           拾取点
//输出：SLorEL       在起始边，为true；反之为false
//返回：如果在起始边或终止边，返回true；如果不在，返回false
//ls 17.9.4
bool RHA::CombineJD(vector<JDarrayStruct> &jdAry) 
{
	//判断起终点的前后交点
	int jdNum=jdAry.size();
	int sJD=0, eJD=0;

	if (jdNum<3)
		return false;

	double sProjml=0.0, eProjml=0.0;
	sProjml=PROJ_ML(array, tarray, jdAry[0].N, jdAry[0].E);
	eProjml=PROJ_ML(array, tarray, jdAry[jdNum-1].N, jdAry[jdNum-1].E);

	sJD=DistToNearJD(sProjml, true);
	eJD=DistToNearJD(eProjml, false);

	//判断起终点在绕行段还是并行段
	int sRxd=0, eRxd=0;
	int rxdSJD=0, rxdJDNum=0;
	int qrxdSJD=0, qrxdEJD=0, hrxdSJD=0, hrxdEJD=0;

	sRxd=GetiRxdByProjml(sProjml);
	eRxd=GetiRxdByProjml(eProjml);

	int type=QHBX_AddRX;
	bool isLSXSJDAsRXS=false;
	bool isLSXEJDAsRXE=false;

	if (sRxd<0 && eRxd<0)
	{//起终点都在并行段上 818 暂时隐藏
		/*if(AfxMessageBox(L"是否将插入线路设置为绕行段？如果否，系统将插入并行不平行段", MB_YESNO) == IDYES)
		{*/
		type=QHBX_AddRX;
		isLSXSJDAsRXS=true;
		isLSXEJDAsRXE=true;
		/*}
		else
		type=QHBX_AddBX;*/
	}
	else
	{
		AfxMessageBox(L"插入线路时起终点不能选在目标线路的绕行段上，请在并行段上插入线路");
		return false;
	}
	//else if (sRxd>-1 && eRxd>-1)//818 暂时隐藏
	//{
	//	if (sRxd == eRxd)
	//	{
	//		type=InOneRXD;
	//		GetRxdSEJD(sRxd, qrxdSJD, qrxdEJD);
	//		if(sJD<qrxdSJD)
	//			isLSXSJDAsRXS=true;
	//		if(eJD>qrxdEJD)
	//			isLSXEJDAsRXE=true;
	//	}
	//	else
	//	{
	//		type=ConTwoRXD;
	//		GetRxdSEJD(sRxd, qrxdSJD, qrxdEJD);
	//		GetRxdSEJD(eRxd, hrxdSJD, hrxdEJD);

	//		if(sJD<qrxdSJD)
	//			isLSXSJDAsRXS=true;
	//		if(eJD>hrxdEJD)
	//			isLSXEJDAsRXE=true;
	//	}
	//}
	//else if (sRxd>-1 && eRxd<0)
	//{
	//	type=QRHB;
	//	GetRxdSEJD(sRxd, qrxdSJD, qrxdEJD);
	//	if(sJD<qrxdSJD)
	//		isLSXSJDAsRXS=true;
	//	isLSXEJDAsRXE=true;
	//}
	//else
	//{
	//	type=QBHR;
	//	GetRxdSEJD(eRxd, hrxdSJD, hrxdEJD);
	//	isLSXSJDAsRXS=true;
	//	if(eJD>hrxdEJD)
	//		isLSXEJDAsRXE=true;
	//}

	//调整交点名称 818
	for (int i=1; i<jdNum-1; i++)
		_stprintf(jdAry[i].JDName, L"%s-%d", JDarray[sJD].JDName, i);

	//中间交点赋线路名
	for (int i=1; i<jdNum-1; i++)
		_tcscpy(jdAry[i].XLName, m_XLName);

	//赋交点属性
	if(type == QHBX_AddBX)
	{
		//818
		for (int i=1; i<jdNum-1; i++)
		{
		}
			

	}
	else
	{
		for (int i=1; i<jdNum-1; i++)
			jdAry[i].JDXZ=RX_One;

		//修正第一个交点为绕行始
		if(isLSXSJDAsRXS)
		{
			jdAry[1].JDXZ=RX_Start;
			_tcscpy(jdAry[1].BLName, JDarray[sJD+1].BLName);
			_tcscpy(jdAry[1].BLJDName, JDarray[sJD+1].BLJDName);
			jdAry[1].XJJ=JDarray[sJD+1].XJJ;
		}

		//修正最后一个交点为绕行止
		if (isLSXEJDAsRXE)
		{
			jdAry[jdNum-2].JDXZ=RX_End;
			_tcscpy(jdAry[jdNum-2].BLName, JDarray[eJD-1].BLName);
			_tcscpy(jdAry[jdNum-2].BLJDName, JDarray[eJD-1].BLJDName);
			jdAry[jdNum-2].XJJ=JDarray[eJD-1].XJJ;
		}

	}

	//合并交点

	//删除交点
	for (int i=eJD-1; i>sJD; i--)
		JDarray.erase(JDarray.begin()+i);
	//插入临时线交点
	for (int i=jdNum-2; i>0; i--)
		JDarray.insert(JDarray.begin()+sJD+1, jdAry[i]);

	//计算线元
	getpxy_DATA();

	//删除从参照线上复制过来的断链
	for(int i=0;i<DLarray.size();i++)
	{
		if(DLarray[i].Indx == -3)
		{
			DLarray.erase(DLarray.begin()+i);
			i--;
		}
	}


	//更新断链
	if (type == InOneRXD)
	{//
		UpdateLC(sRxd);
	}
	else if (type == QHBX_AddRX)
	{
		int iRxd=0;
		iRxd=FindiRXD(sJD+1);

		//删除插入线路起终点之间的断链
		for(int i=0;i<DLarray.size();i++)
		{
			if (DLarray[i].TYLC>sProjml+0.001 && DLarray[i].TYLC+0.001<eProjml)
			{
				DLarray.erase(DLarray.begin()+i);//818 可能有问题，错误删除一些断链？？
				i--;
			}
		}

		//修改绕行段编号:之后的绕行段从iRxd+1开始编号
		for(int i=0;i<DLarray.size();i++)
		{
			if(DLarray[i].Indx >= iRxd)
			{
				int oldIndx=DLarray[i].Indx;
				int newIndx=iRxd+1;

				for(int j=i;j<DLarray.size();j++)
				{
					if(DLarray[j].Indx == oldIndx)
						DLarray[j].Indx=newIndx;
					else if (DLarray[j].Indx>-1)
					{
						newIndx++;
						oldIndx=DLarray[j].Indx;
						j--;
					}
				}

				break;
			}
		}


		//生成断链
		DLTab rxdDL[2];
		GeniRxdDL(iRxd, rxdDL);

		//插入断链
		int iDL=0;
		iDL=GetPreDL(rxdDL[0].TYLC);
		iDL++;

		DLarray.insert(DLarray.begin()+iDL, rxdDL[1]);
		DLarray.insert(DLarray.begin()+iDL, rxdDL[0]);

		//重新计算断链的几何里程
		CalDLprojml(iDL+2);

		//标志里程
		if(XLShuXing != SJJX && XLShuXing != SJ2X)
		{
			//删除标志里程
			//删除插入线路起终点之间的断链
			for(int i=0;i<BZLCarray.size();i++)
			{
				if (BZLCarray[i].xyToprojml>sProjml+0.001 && BZLCarray[i].xyToprojml+0.001<eProjml)
				{
					BZLCarray.erase(BZLCarray.begin()+i);//818 可能有问题，错误删除一些？？
					i--;
				}
			}

			//插入新断链的标志里程
			BZLCStructTable tmpBZLC;
			xlpoint PZ;

			tmpBZLC.isAutoAdd=true;
			tmpBZLC.K=1.0;
			_tcscpy(tmpBZLC.GJDNum, rxdDL[0].RXDName);
			_tcscpy(tmpBZLC.XLName, m_XLName);

			for(int i=0;i<2;i++) 
			{
				tmpBZLC.xyToprojml=rxdDL[i].TYLC;
				PZ.lc=rxdDL[i].TYLC;
				xlpoint_pz(array, tarray, PZ);
				tmpBZLC.X=PZ.x;
				tmpBZLC.Y=PZ.y;
				_tcscpy(tmpBZLC.CKml, LCchr(rxdDL[i].BGH, rxdDL[i].BLC, 3)); 
				tmpBZLC.kmlTocml=TYLC1(tmpBZLC.CKml, DLarray);//该函数只适用于绕行段

				BZLCarray.push_back(tmpBZLC);
			}
		}

	}
	else if (type == QRHB)
	{
		//删除绕行段终点到源线路终点之间的断链

		//更新绕行段编号

		//更新绕行段断链
		UpdateLC(sRxd);
	}
	else if (type == QBHR)
	{

	}
	else if (type == ConTwoRXD)
	{

	}


}

//返回指定交点所处的绕行段
int RHA::FindiRXD(int iJD)
{
	int iRXD=-1;//第几个绕行段
	int NJD=JDarray.size();

	for(int i=0;i<NJD;i++)
	{
		if( JDarray[i].JDXZ == RX_Start)
		{
			iRXD++;
			if (i<NJD-1)
				i++;

			while(JDarray[i].JDXZ ==  RX_One || JDarray[i].JDXZ == RX_End)
				i++;
			i--;

			if(iJD<=i)
				return iRXD;
		}

	}		
	return iRXD;
}


//功能：插入线路时计算线路在目标线路上的起终点坐标
//输入：
//tmpJDAry           源线路的交点数组
//tmpQXBAry          源线路的曲线表数组
//输出：
//tmpJDAry           修改起终交点坐标（方便插入目标线路）后的源线路的交点数组
//返回：如果不能插入返回false，反之返回true
//算法：计算源线路的起始边和终止边是否和目标线路重叠，如果重叠
//      ，并且满足其他条件，则可以插入
//ls 17.9.4
bool RHA::GetSptEptWhenInXL(vector<JDarrayStruct> &tmpJDAry, const vector<QXBStruct> &tmpQXBAry) 
{
	double sptX, sptY;
	double eptX, eptY;
	bool isCD = false;
	double zhX=0.0, zhY=0.0;
	double hzX=0.0, hzY=0.0;
	double dist1=0.0, dist2=0.0;
	double fwj1=0.0, fwj2=0.0;
	int iJD1=-1, iJD2=-1;//源线路起终点在目标线路上的直线边起点的交点下标

	
	sptX=tmpJDAry[0].N;
	sptY=tmpJDAry[0].E;
	eptX=tmpJDAry[1].N;
	eptY=tmpJDAry[1].E;

	//判断起始边是否与源线路重叠
	for (int i=0; i+1<JDarray.size(); i++)
	{
		isCD=IsZXCD(sptX, sptY, eptX, eptY, JDarray[i].N, JDarray[i].E, JDarray[i+1].N, JDarray[i+1].E);
		if (isCD)
		{
			iJD1=i;
			isCD=false;//再根据其他条件判断重叠是否合适

			//判断合并后是否曲线重，即没有夹直线
			TrsProjmltoNE(QXB[i].KHZ, hzX, hzY);//求目标线路i交点的缓直点坐标

			dist1=DistBtwnPts(hzX, hzY, eptX, eptY);//目标线路上缓直点到源线路第二个交点的距离
			if (dist1+0.0001>tmpQXBAry[1].T1)//保证有夹直线
			{
				//判断目标线路上缓直点是否与源线路第一个交点同侧（相对于源线路第二个交点）
				fwj1=CalFWJ(hzY, hzX, eptY, eptX);
				fwj2=CalFWJ(sptY, sptX, eptY, eptX);

				if(fabs(fabs(fwj1-fwj2)-pi)<0.01)
				{
					AfxMessageBox(L"临时线始边方向与目标线路相反，请考虑是否将临时线反向后插入");
					return false;
				}
				else if(fabs(fwj1-fwj2)<0.1)
				{
					isCD=true;
					//如果源线路起点在缓直点外侧（相对于源线路第二个交点），修正起点坐标为缓直点
					dist2=DistBtwnPts(sptY, sptX, eptY, eptX);
					if (dist2>dist1)
					{
						tmpJDAry[0].N=hzX;
						tmpJDAry[1].E=hzY;
					}
				}
			}
			else
			{
				AfxMessageBox(L"源线路的始边插入时曲线重叠，夹直线不满足要求");
				return false;
			}
			
			break;
		}
	}

	if(!isCD)
	{
		AfxMessageBox(L"源线路的始边尚未连接，请先使用连接线路命令");
		return false;
	}


	//判断终止边是否与源线路重叠
	int tmpJDNum=tmpJDAry.size();
	sptX=tmpJDAry[tmpJDNum-2].N;
	sptY=tmpJDAry[tmpJDNum-2].E;
	eptX=tmpJDAry[tmpJDNum-1].N;
	eptY=tmpJDAry[tmpJDNum-1].E;

	for (int i=0; i+1<JDarray.size(); i++)
	{
		isCD=IsZXCD(sptX, sptY, eptX, eptY, JDarray[i].N, JDarray[i].E, JDarray[i+1].N, JDarray[i+1].E);
		if (isCD)
		{
			iJD2=i;
			isCD=false;//再根据其他条件判断重叠是否合适

			//判断合并后是否曲线重，即没有夹直线
			TrsProjmltoNE(QXB[i+1].KZH, zhX, zhY);//求目标线路i+1交点的直缓点坐标

			dist1=DistBtwnPts(zhX, zhY, sptX, sptY);//目标线路上直缓点到源线路倒数第二个交点的距离
			if (dist1+0.0001>tmpQXBAry[tmpJDNum-2].T2)//保证有夹直线
			{
				//判断目标线路上直缓点是否与源线路终点同侧（相对于源线路倒数第二个交点）
				fwj1=CalFWJ(zhY, zhX, sptY, sptX);
				fwj2=CalFWJ(eptY, eptX, sptY, sptX);

				if(fabs(fabs(fwj1-fwj2)-pi)<0.01)
				{
					AfxMessageBox(L"临时线终边方向与目标线路相反，请考虑是否将临时线反向后插入");
					return false;
				}
				if(fabs(fwj1-fwj2)<0.1)
				{
					isCD=true;
					//如果源线路终点在直缓点外侧（相对于源线路倒数第二个交点），修正终点坐标为直缓点
					dist2=DistBtwnPts(sptY, sptX, eptY, eptX);
					if (dist2>dist1)
					{
						tmpJDAry[tmpJDNum-1].N=zhX;
						tmpJDAry[tmpJDNum-1].E=zhY;
					}
				}
			}
			else
			{
				AfxMessageBox(L"源线路的终边插入时曲线重叠，夹直线不满足要求");
				return false;
			}

			break;
		}
	}

	if(!isCD)
	{
		AfxMessageBox(L"源线路的终边尚未连接，请先使用连接线路命令");
		return false;
	}

	if(iJD2<iJD1)
	{
		AfxMessageBox(L"请先将源线路反向，然后再插入线路");
		return false;
	}

	return true;
}

/*---------------------------------------------------*/
//全线坐标法	绘制拨量相关函数	ljx	17.10.18
/*===================================================================
函数名:DrawCircle()		绘制圆函数
------------------------------------------------------------
函数功能:绘制全线坐标法中各个测点所在位置的圆
------------------------------------------------------------
输入:m_MeasurePoints	测点vector
------------------------------------------------------------
输出:无
------------------------------------------------------------
算法描述:       
------------------------------------------------------------
编写日期:17.10.18
------------------------------------------------------------
备注： 
=====================================================================*/
void RHA::DrawCircle(AcGiWorldDraw *mode)
{
	//1.设置各种参数
	vector<SSurPt>::iterator Beg = m_MeasurePoints.begin();
	AcGePoint3d CircleCenter(0, 0, 0);
	AcGeVector3d Normal(0, 0, 1);
	int CircleColor = 0;
	double R = 8.0;

	//2.遍历所有测点进行绘制
	while(Beg != m_MeasurePoints.end())
	{
		CircleCenter.x = Beg->E;
		CircleCenter.y = Beg->N;
		//根据测点所在不同属性（直线/圆曲线/缓和曲线）为圆选择不同的颜色
		if(0 == Beg->Propert)
			CircleColor = 1;
		else if(1 == Beg->Propert)
			CircleColor = 2;
		else if(2 == Beg->Propert)
			CircleColor = 3;
		mode->geometry().circle(CircleCenter, R, Normal);
		mode->subEntityTraits().setColor(CircleColor);
		Beg++;
	}
}
/*===================================================================
函数名:CalRectification()		计算拨量函数
------------------------------------------------------------
函数功能:计算测点数组中各个测点相对于线路的距离，即拨量
------------------------------------------------------------
输入:m_MeasurePoints	测点vector
------------------------------------------------------------
输出:Rectification		拨量vector
------------------------------------------------------------
算法描述:通过RHA的PROJ_ML函数计算测点投影到线路上的投影里程，
		 再通过xlpoint_pz函数将里程转换为相应点的x、y坐标，
		 最后求距离。
------------------------------------------------------------
编写日期:17.10.18
------------------------------------------------------------
备注： 
=====================================================================*/
void RHA::CalRectification(vector<double>& Rectification)
{
	//1.设置各种参数
	xlpoint ProjectPt;		//线路点，用以传参
	double Mileage = 0;		//测点对应在线路上的投影里程
	Rectification.clear();	//清空拨量vector，以便存储数据

	//2.迭代求解每一个测点相对于线路的拨量
	vector<SSurPt>::iterator Beg = m_MeasurePoints.begin();	//迭代器
	while (Beg != m_MeasurePoints.end())
	{
		double tempE = Beg->E;		//将测点E坐标存入tempE
		double tempN = Beg->N;		//将测点N坐标存入tempN
		Mileage = PROJ_ML(array, tarray, tempN, tempE, tempN, tempE);			//通过RHA的PROJ_ML求解测点对应的投影里程
		ProjectPt.lc = Mileage;		//将该里程存入线路点结构体中
		xlpoint_pz(array, tarray, ProjectPt);									//借用该函数求出在对应里程下的线路点
		AcGePoint2d AcGeMeasurePoint(tempN, tempE), AcGeRHAPoint(ProjectPt.x, ProjectPt.y);					//设置两个AcGePoint2d点，注意NE和xy对应关系
		double Distance = AcGeRHAPoint.distanceTo(AcGeMeasurePoint);										//求距离，即为拨距
		Rectification.push_back(Distance);																	//存入vector中
		Beg++;
	}
}
/*===================================================================
函数名:DrawRectificationText()	绘制拨量文字
------------------------------------------------------------
函数功能:将每个测点的拨量绘制到线路相应位置，并在线路终点处
		 绘制一个汇总表
------------------------------------------------------------
输入:Rectification		拨量vector
	 m_MeasurePoints	测点vector
------------------------------------------------------------
输出:无
------------------------------------------------------------
算法描述:       
------------------------------------------------------------
编写日期:17.10.18
------------------------------------------------------------
备注： 
=====================================================================*/
void RHA::DrawRectificationText(vector<double>& Rectification, AcGiWorldDraw* mode)
{
	//1.设置参数
	ACHAR text[50];									//文字参数
	AcGePoint3d SingleMeasurePoint(0, 0, 0);		//绘图点
	AcGeVector3d Normal(0.0, 0.0, 1.0);				//法线向量
	AcGeVector3d direction(1.0, 0.0, 1.0);			//文字方向，此处统一取水平方向
	AcGiTextStyle pTextStyle;						//进行文字设置
	pTextStyle.setFont(L"宋体", 0, 0, 134, 2);		//文字格式
	pTextStyle.setTextSize(5);						//文字高度

	int ChangeNum[7];
	for(int i = 0; i < 7;i++)
		ChangeNum[i] = 0;

	//2.对每个测点进行拨量及序号绘制，并记录相应拨量范围，方便统计使用
	for(int i = 0; i + 1 < Rectification.size(); i++)
	{
		SingleMeasurePoint.x = m_MeasurePoints[i].E;
		SingleMeasurePoint.y = m_MeasurePoints[i].N;

		//①绘制拨量
		_stprintf(text, L"%0.0f", Rectification[i] * 1000);
		mode->subEntityTraits().setColor(2);
		mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

		//②绘制序号
		SingleMeasurePoint.x += 8;
		_stprintf(text, L"%d", i);
		mode->subEntityTraits().setColor(1);
		mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

		//③判断属于多少拨量区间
		if(Rectification[i] >= 300)
			ChangeNum[6]++;
		else
			ChangeNum[(int)(Rectification[i] / 50)]++;
	}

	//3.绘制汇总
	SingleMeasurePoint.y += 20;
	_stprintf(text, L"0-50  %d", ChangeNum[0]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

	SingleMeasurePoint.y += 20;
	_stprintf(text, L"50-100  %d", ChangeNum[1]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

	SingleMeasurePoint.y += 20;
	_stprintf(text, L"100-150  %d", ChangeNum[2]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

	SingleMeasurePoint.y += 20;
	_stprintf(text, L"150-200  %d", ChangeNum[3]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

	SingleMeasurePoint.y += 20;
	_stprintf(text, L"200-250  %d", ChangeNum[4]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

	SingleMeasurePoint.y += 20;
	_stprintf(text, L"250-300  %d", ChangeNum[5]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

	SingleMeasurePoint.y += 20;
	_stprintf(text, L">300  %d", ChangeNum[6]);			
	mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);
}