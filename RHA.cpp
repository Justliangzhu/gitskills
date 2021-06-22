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

	////��ȫ��������ɾ��
	//if(XLShuXing == JYJX || XLShuXing == JY2X  
	//	|| XLShuXing == SJJX || XLShuXing == SJ2X)
	//{
	//	Int16 xlnum=g_DocManageAry[m_iDoc].XLAry.size();//818 �ڹر��ĵ�ʱ��g_DocManageAryΪ�� m_iDoc=0 ��ң���Ӧ�����ڲ�����ⲿ����
	//	for (int i=0; i<xlnum; i++)
	//	{
	//		id2=g_DocManageAry[m_iDoc].XLAry[i]->id();//nan

	//		if(g_DocManageAry[m_iDoc].XLAry[i]->m_mdbName == m_mdbName
	//			&& g_DocManageAry[m_iDoc].XLAry[i]->m_XLName == m_XLName
	//			&& g_DocManageAry[m_iDoc].XLAry[i]->id() == id())//818 �ж�id�Ƿ���
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

	//��ɫ����
	pFiler->writeItem(GTZXJDToJDColor);
	pFiler->writeItem(XWArcColor);
	pFiler->writeItem(HuanHequxianColor);
	pFiler->writeItem(JiaZhiXianColorZuo);
	pFiler->writeItem(JiaZhiXianColorYou);
	pFiler->writeItem(mdbnameBoxColor);
	pFiler->writeItem(mdbnameTextColor);
	pFiler->writeItem(QXYSColor);
	pFiler->writeItem(LineWeightValue);


	//�е��϶�
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

	//�Ľ�������
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


	//�������ж�����
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

	//////�淶////////////
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

	//��ɫ����
	pFiler->readItem(&GTZXJDToJDColor);
	pFiler->readItem(&XWArcColor);
	pFiler->readItem(&HuanHequxianColor);
	pFiler->readItem(&JiaZhiXianColorZuo);
	pFiler->readItem(&JiaZhiXianColorYou);
	pFiler->readItem(&mdbnameBoxColor);
	pFiler->readItem(&mdbnameTextColor);
	pFiler->readItem(&QXYSColor);
	pFiler->readItem((int *)&LineWeightValue);

	//�е��϶�
	/*pFiler->readItem(&MoveJDIndx);   //818 �⼸����������鵵�ᷢ���ϲ�����������
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

	//�Ľ�������
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


	//�������ж�����
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

	//////�淶////////////
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
	{//����϶��е�
		
		if (IsMoveDone)
		{//�е��϶�����
			
			if (MoveJDIndx>-1 && MoveJDIndx<JDarray.size())
			{
				int iRxd=0;
				iRxd=GetiRxdByProjml((QXB[MoveJDIndx].KZH+QXB[MoveJDIndx].KHZ)/2.0);

				//���¼������

				if(iRxd>-1)
				{//�����жν���

					UpdateLC(iRxd);
				}
				else
				{
					//��ǰһ������ǰ�������һ������
					int sdl=0;
					if(MoveJDIndx>0)
						MoveJDIndx--;
					sdl=GetPreRealDL(QXB[MoveJDIndx].KZH);

					//�����������ʼ������֮������ж�����ͶӰ���
					CalDLprojml(sdl);
				}
			}
			else
			{
				//�Զ�����
				UpdateAllData();
				ads_printf(L"��ʾ��ϵͳ���Զ��������\n");
			}

			//�����־���
			CalBZLC();
			SaveData();
		
		}
		else
		{
			//�е��϶���
			getpxy_DATA(MoveJDIndx);
		}
	}
	else
	{
		//�����·��ͼ
		if (XLShuXing == JY2X 
			|| XLShuXing == SJJX 
			|| XLShuXing == SJ2X)
			ModifyExJDXY();
		
		getpxy_DATA();

		//�����־���
		CalBZLC();
		SaveData();
	}
	

	//////nan
	//CString str;
	//for (int i=0;i<DLarray.size();i++)
	//{
	//	str.Format(L"\n%s���� ���%d\n ǰ%s%lf", m_XLName, DLarray[i].Indx, DLarray[i].BGH, DLarray[i].BLC);
	//	ads_printf(str);
	//	str.Format(L" ��%s%lf",DLarray[i].EGH, DLarray[i].ELC);
	//	ads_printf(str);
	//}


	////nan
	//CString str;
	//for (int i=0;i<BZLCarray.size();i++)
	//{
	//	str.Format(L"\n%s��־ %s K-%lf %d\n ", m_XLName, BZLCarray[i].CKml,BZLCarray[i].K, BZLCarray[i].isAutoAdd);
	//	ads_printf(str);
	//	str.Format(L"ͶӰ���%lf �������%lf ",BZLCarray[i].xyToprojml, BZLCarray[i].kmlTocml  );
	//	ads_printf(str);
	//}

	//ads_printf(L"\n/////////////\n");


	//818 ������ҵ����

	//һ������

	// ����λ
	DrawXL(mode);

	//�潻������
	DrawJDLineAndCir(mode);

	//�������

	if (!IsMove || (IsMove && IsMoveDone))//������϶����������ʱ������̱�ע 
	{
		//�γɹ�ͨ������  818 �Ժ���subworlddraw�����ɹ�ͨ�������᲻��Ӱ��Ľ�����̵ļ��㣿��
		if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		{
			//ɾ�����жζ���
			EarseDLofBLonBXD();
			//�γɹ�ͨ������
			Create2XGTDLB();
		}

		//��ע����
		MarkJD(mode,JDarray,QXB,JDGH);

		//��ע������
		MarkTZD(mode);

		//��ע���ж���̺Ͷ���
		MarkRXDLC(mode);

		//��ע�߼��
		MarkXJJ(mode);

		//��ע��־���
		if (XLShuXing != SJJX && XLShuXing != SJ2X)
			MarkBiaoZhiLC(mode, 100, /*draw_zybz*/1, 2);//818 ���еı�ע�����������жϣ�

		//��ע���жζ���
		for(int i=1;i<DLarray.size();i++)
		{
			if (DLarray[i].Indx == -2)
				MarkSglDL(mode, i, 1);
		}

		//��ע�¶�  
		if(m_BPDArray.size()>0)
		{
			//���µ����� ת CM ����
			TrsBPDArraytoCM();
			BZCM(mode);//��ע���µ�
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
		tmpBPD_array[i][0] = m_BPDArray[i].Cml;//�������  
		tmpBPD_array[i][1] = m_BPDArray[i].Level;	//�����¶�			
		tmpBPD_array[i][2] = m_BPDArray[i].Rshu;//�����߰뾶
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
		TrsCmltoCkml(m_CM[i*5], Ckml, /*m_Xsw*/3);//m_Xsw 818 ȡС��λ��
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
			_stprintf(BefSlope,L"%.0lf��",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.0lf��",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==1)
		{
			_stprintf(BefLen,L"%.1lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.1lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.1lf��",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.1lf��",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==2)
		{
			_stprintf(BefLen,L"%.2lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.2lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.2lf��",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.2lf��",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==3)
		{
			_stprintf(BefLen,L"%.3lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.3lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.3lf��",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.3lf��",1000*m_CM[(i+1)*5+2]);
		}
		else if (SlopeXSnum==4)
		{
			_stprintf(BefLen,L"%.4lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.4lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.4lf��",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.4lf��",1000*m_CM[(i+1)*5+2]);
		}
		else 
		{
			_stprintf(BefLen,L"%.2lf",m_CM[i*5+3]);
			_stprintf(AftLen,L"%.2lf",m_CM[(i+1)*5+3]);
			_stprintf(BefSlope,L"%.2lf��",1000*m_CM[i*5+2]);
			_stprintf(AftSlope,L"%.2lf��",1000*m_CM[(i+1)*5+2]);
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

//��ע������
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
	if ((num == 0) && (LC < /*StartCml*/0.0))//��� 818
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
	double NoteLen=1.5*texth;//��ע�߳���
	long int zlc;
	double DDlength=0;
	int rxdNum;
	int sdl, edl;
	int iRXD;
	ACHAR ckml[50];


	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd :
		// ����
		for ( i = 0; i < NJD; i++) 
		{  
			Pt.set(JDarray[i].E,JDarray[i].N,0.0);
			snapPoints.append(Pt);
		};

		//  Բ�����յ�
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
				xlzbxy(PB,PE,l,INDATA,0);// ĳ��Ԫ�յ�
				Pt[X]=PE.y;Pt[Y]=PE.x; Pt[Z]=0.0;
				snapPoints.append(Pt);
				/*
				xlzbxy(PB,&PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
				Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
				if (j==2) // Բ�� �е�
				{ snapPoints.append(Pt);	};
				*/
				PB=PE;  
			};
		};
		
	
		
		//������  818 ��ʱ��ȥ
		for(int i=0;i<DLNum;i++)
		{
			PZtmp.lc=DLarray[i].TYLC;  // ͳһ���
			k=xlpoint_pz(array,tarray,PZtmp);  //�����õ�array 818 ��ҵ�
			if (k>0 && k<=6)  //�н�
			{
				B=PZtmp.a;
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
				snapPoints.append(ptb);
				ptb[X]=PZtmp.y+27*texth*sin(B+pi*0.5);
				ptb[Y]=PZtmp.x+27*texth*cos(B+pi*0.5);
				snapPoints.append(ptb);
			}; 

		}


		//���ױ��
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
					LCB= DLarray[iDL].ELC;  //���������� 
					_tcscpy(GH,DLarray[iDL].EGH);
					_tcscpy(GH1,DLarray[iDL].BGH);

					zlc=(long int)(LCB/dlc); //818
					BZlc=zlc*dlc;  // ʵ����� 
					if ((BZlc-LCB)<0.0001) 
						BZlc=BZlc+dlc;

					if ( BZlc>DLarray[iDL+1].BLC+0.0001)
					{
						DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//������ͳһ��̡�
						continue;
					}

					_tcscpy(GH,DLarray[iDL].EGH);  //�ں�

					do 
					{
						// �������
						cml=DDlength+BZlc-LCB; 
						// ͶӰ���
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

						//��Ӳ�׽��		 
						k=xlpoint_pz(array,tarray,PZtmp);   
						BZW=k;    //  K<0  �޽�
						if (k>0 && k<=6)  //�н�
						{
							B=PZtmp.a;
							ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
							snapPoints.append(ptb);
							ptb[X]=PZtmp.y+NoteLen*sin(B+pi*0.5);
							ptb[Y]=PZtmp.x+NoteLen*cos(B+pi*0.5);
							snapPoints.append(ptb);//818 snapPoints ��Ҫremove����ֹԽ��
						};          
						BZlc=BZlc+dlc;
						if ( BZlc>DLarray[iDL+1].BLC) 
							BZW=-99;		 		 

					}while (BZW>0);

					DDlength=  DDlength+DLarray[iDL+1].BLC-LCB;//������ͳһ��̡�
				} // IDL

			}
			else
				break;
		}

		////��־���
		//if(XLShuXing != SJJX && XLShuXing != SJ2X)
		//{
		//	for (i =0; i < BZLCarray.GetSize(); i++) 
		//	{  
		//		if (BZLCarray[i].isAutoAdd == false)
		//		{
		//			PZtmp.lc=BZLCarray[i].xyToprojml;
		//			k=xlpoint_pz(array,tarray,PZtmp);   
		//			if (k>0 && k<=6)  //�н�
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
				xlzbxy(PB,PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
				Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
				if (j==2) // Բ�� �е�
				{ 
					snapPoints.append(Pt);	
				};
				xlzbxy(PB,PE,l,INDATA,0);// ĳ��Ԫ�յ�
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


		//һ��������Ϊ�е�
		int NJD=JDarray.size();
		for (i=0; i<NJD; i++) 
		{
			Pt.set(JDarray[i].E, JDarray[i].N, 0.0);
			gripPoints.append(Pt);
		}

		//�������������Ӧ��Բ���е����е㼯����ţ�NJD-1��+��NJD-1��+1������NJD-1)+��NJD-1��+NJD-2
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
				xlzbxy(PB,PE,l,INDATA,0);// ĳ��Ԫ�յ�
				xlzbxy(PB,PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�

				if (j == 2) //Բ���е�
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


//���ܣ������û����õĽ����϶���ʽ�͸ý�������ԣ��õ��ý����ʵ���϶���ʽ 
//���룺 jdMoveType        �û����õĽ����϶���ʽ
//       iJD               �����±�
//���أ����ؽ����϶���ʽ����������϶�������-100
//�㷨��
int  RHA::GetJDMoveType(int jdMoveType, int iJD) const 
{
	int realMoveType=-100;
	int NJD=JDarray.size();

	if (JDarray[iJD].JDXZ == BXPX)
		ads_printf(L"ƽ�н��㲻���϶�\n");
	else if (JDarray[iJD].JDXZ == S_Start || JDarray[iJD].JDXZ == S_End )
		ads_printf(L"S�佻�㲻���϶�\n");
	else if (JDarray[iJD].JDXZ == RX_One)
		//������
		realMoveType=jdMoveType;
	else if(JDarray[iJD].JDXZ == RX_Start)
	{//����ʼ��Ĭ�ϰ�ǰ�����϶�

		if(iJD>0 && jdMoveType != HQX && jdMoveType != QYHQ)
		{//û��ѡ�񰴺������� 

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == QQY)
				jdMoveType=QQX;//ǰ����
			else if(jdMoveType == QHQY || jdMoveType == HQY)
				jdMoveType=QQHY;//ǰ�к�Բ

			if (iJD<NJD-1 && JDarray[iJD+1].JDXZ != RX_One && JDarray[iJD+1].JDXZ != RX_End)
			{//��������

				//�õ�������û�м�¼��һֱ�߱�ƽ����Ϣ
				//����ô�ý�������϶�
				if(IsJdBLInfEmpty(JDarray[iJD]))
					realMoveType=jdMoveType;
			}
			else
				realMoveType=jdMoveType;
		}
		else if (iJD == 0)//�������
			realMoveType=jdMoveType;
		
	}
	else if (JDarray[iJD].JDXZ == RX_End)
	{//����ֹ��Ĭ�ϰ��������϶�

		if(iJD<NJD-1 && jdMoveType != QQX && jdMoveType != QQHY)
		{//û��ѡ��ǰ������

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == HQY)
				jdMoveType=HQX;//������
			else if(jdMoveType == QHQY || jdMoveType == QQY)
				jdMoveType=QYHQ;//ǰԲ����

			realMoveType=jdMoveType;
		}
		else if (iJD == NJD-1)//�յ�����
			realMoveType=jdMoveType;
	}
	else if (JDarray[iJD].JDXZ == BX_NotPX)
	{
		//818 ���յ㲻ƽ�е������ô���ǣ�

		if(iJD>0 && JDarray[iJD-1].JDXZ != BX_NotPX
			&& iJD<NJD-1 && JDarray[iJD+1].JDXZ != BX_NotPX)
		{//���㲢�в�ƽ��

			realMoveType=-100;
		}
		else if (iJD>0 && JDarray[iJD-1].JDXZ != BX_NotPX
			&& jdMoveType != HQX && jdMoveType != QYHQ)
		{//ǰһ���㲻�ǲ��в�ƽ�У�����û��ѡ�񰴺�������

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == QQY)
				jdMoveType=QQX;//ǰ����
			else if(jdMoveType == QHQY || jdMoveType == HQY)
				jdMoveType=QQHY;//ǰ�к�Բ

			realMoveType=jdMoveType;
		}
		else if (iJD<NJD-1 && JDarray[iJD+1].JDXZ != BX_NotPX
			&& jdMoveType != QQX && jdMoveType != QQHY)
		{//��һ���㲻�ǲ��в�ƽ�У�����û��ѡ��ǰ������

			if (jdMoveType == ZJQX || jdMoveType == ZY || jdMoveType == HQY)
				jdMoveType=HQX;//������
			else if(jdMoveType == QHQY || jdMoveType == QQY)
				jdMoveType=QYHQ;//ǰԲ����

			realMoveType=jdMoveType;
		}
		else
			//ǰ���в��в�ƽ�н���
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


	//g_JDMoveType �����϶���ʽ: =1Ϊ�������϶�  =2 ��ǰ��Բ���� 3 �غ���Բ���� 
	//=4 ��ǰ����Բ���� =5 ǰ�к�Բ =6 ǰԲ���� =7 ��ǰ�����϶� =8 �غ������϶� �������� �����϶�
	
	//GetDLTYLC();   ls δ����
	double dd=0, d1=0, d2=0, DD01=0, DD02=0, DD1=0, DD2=0;//����
	double fb=0, fb1=0, fb2=0;//��λ��
	double dx=0, dy=0;
	CString  mes;
	int k=0, j=0;
	int NJD=JDarray.size();


	dx=offset[X];//ƫ����
	dy=offset[Y];	
	for ( j=0; j<indices.length(); j++)
	{	
		k=indices[j];       
	// һ���϶����㣨���������յ㣩��������� 0~NJD-1		
		if (k>-1 && k<NJD) 
		{
			MoveJDIndx=k;
			////nan
			//CString mes;
			//mes.Format(L"ǰ���϶���ʽ %d\n", jdMoveType);
			//ads_printf(mes);

			//�����û����õĽ����϶���ʽ�͸ý�������ԣ��õ��ý����ʵ���϶���ʽ
			jdMoveType=GetJDMoveType(jdMoveType, k);
			if (jdMoveType<0)
				return Acad::eOk;

			////nan
			//mes.Format(L"���϶���ʽ %d\n", jdMoveType);
			//ads_printf(mes);

			

			if (jdMoveType == ZJQX)  
			{//��������������϶�

				//��ֱ�߶�(�������)[(XB,YB),(XE,YE)]�ĳ��Ⱥͷ�λ��  ����:���룬 *��λ��
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
				{ //�غ������϶�
					dd=d2; 
					fb=fb2;
				};
				//�������㣬�ɺ�һ���������������ꣻ������յ㣬��ǰһ���������

				JDarray[k].E=JDarray[k].E+dd*sin(fb);
				JDarray[k].N=JDarray[k].N+dd*cos(fb);
			}
			else if (jdMoveType == QQX && k>0)
			{
				//��ǰ�����϶�
				DD01=xyddaa(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N,JDarray[k].E, &fb1);
				DD1=xyddaa(JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N+dy,JDarray[k].E+dx, &fb);
				dd=DD1-DD01;

				JDarray[k].E=JDarray[k].E+dd*sin(fb1);
				JDarray[k].N=JDarray[k].N+dd*cos(fb1);
			}
			else if (jdMoveType == HQX && k<NJD-1)
			{
				//�غ������϶�
				DD02 =xyddaa(JDarray[k].N, JDarray[k].E, JDarray[k+1].N, JDarray[k+1].E, &fb2);
				DD2=xyddaa(JDarray[k].N+dy, JDarray[k].E+dx, JDarray[k+1].N, JDarray[k+1].E, &fb);
				dd=DD02-DD2;

				JDarray[k].E=JDarray[k].E+dd*sin(fb2);
				JDarray[k].N=JDarray[k].N+dd*cos(fb2);
			}
			else if(jdMoveType >= 3 && jdMoveType <= 7)
			{
				//1.12 ������Բ���϶�
				double OutN,OutE;
				OutN=JDarray[k].N+offset[Y];
				OutE=JDarray[k].E+offset[X];
				// ��ǰԲ
				if (jdMoveType == QQY || jdMoveType == QHQY || jdMoveType == QYHQ)  
				{

					if(k>1)//�������
					{
						double l1,l2,R1,p1,tR1,A2;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

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

						tR1=R1+p1;//�뾶+������	
						//��ǰԲԲ��
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k-2].N;
						E1=JDarray[k-2].E;
						N0=JDarray[k-1].N;
						E0=JDarray[k-1].E;
						N2=JDarray[k].N;
						E2=JDarray[k].E;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,l1,l2,CenN,CenE);
						double TN,TE;//�е�
						//���е�
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
						INTERS(&JDarray[k-1].N,&JDarray[k-1].E,N1,E1,N0,E0,TN,TE,OutN,OutE);
					}
					else
						ads_printf(L"�̶�ǰԲģʽ�����ܸ��Ĵ˽�������\n");
				}

				// �к�Բ
				if (jdMoveType == HQY || jdMoveType == QHQY || jdMoveType == QQHY)  
				{
					if(k<NJD-2)//�����յ�
					{
						double l1,l2,R2,p2,tR2,A1;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

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
						//�뾶+������					
						tR2=R2+p2;
						//��ǰԲԲ��
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k].N;
						E1=JDarray[k].E;
						N0=JDarray[k+1].N;
						E0=JDarray[k+1].E;
						N2=JDarray[k+2].N;
						E2=JDarray[k+2].E;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,l1,l2,CenN,CenE);
						double TN,TE;//�е�
						//���е�
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
						INTERS(&JDarray[k+1].N,&JDarray[k+1].E,N2,E2,N0,E0,TN,TE,OutN,OutE);
					}
					else
						ads_printf(L"�̶���Բģʽ�����ܸ��Ĵ˽�������\n");
				}
				//(JDMoveStyle == 4)// ��ǰ������Բ
				//��ǰ������λ��
				if (jdMoveType == QQHY)
				{
					//ǰ�к�Բ
					if(k>0 && k<NJD-2)
					{
						INTERS(&JDarray[k].N, &JDarray[k].E
							, OutN, OutE, JDarray[k+1].N, JDarray[k+1].E
							,JDarray[k-1].N, JDarray[k-1].E, JDarray[k].N, JDarray[k].E);							
					}
					else
						ads_printf(L"�����϶��㲻���㡰ǰ�к�Բ���ķ�ʽ\n");

				}
				else if (jdMoveType == QYHQ)
				{
					//ǰԲ����
					if(k>0 && k<NJD-1)
					{
						INTERS(&JDarray[k].N, &JDarray[k].E
							, JDarray[k-1].N, JDarray[k-1].E, OutN, OutE 
							,JDarray[k].N, JDarray[k].E, JDarray[k+1].N, JDarray[k+1].E);	
					}
					else
						ads_printf(L"�����϶����յ㲻Ӧѡ��ǰԲ���С��ķ�ʽ\n");

				}

			}
			else   //�����϶�
			{  
				JDarray[k].E=JDarray[k].E+offset[X];//x
				JDarray[k].N=JDarray[k].N+offset[Y];//y
			}

				
		    
		}
		//////////////////818 ��ʱ��ע�⣺�Ժ���Ӵ����迼�����յ��϶������!!!!!!!!!!!
		else if(k >= NJD && k <= 2*NJD-3)//Բ���е�(���յ���Բ��)
		{
			k=indices[j]-(NJD-1); // ��Բ����Ӧ�Ľ����
			MoveJDIndx=k;

			dd=sqrt(offset[X]*offset[X]+offset[Y]*offset[Y]);
			//Բ���뾶�ı�
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
				
				//��ѯ������׼�о�n_o_dR�����Բ���߰뾶���仺��
				l0=SearchProximalRadius(R);

				mes.Format(L"�뾶%0.2f������%0.2f\n", R, l0);
				ads_printf(mes);
				JDarray[k].R = R;
				JDarray[k].l1=JDarray[k].l2=l0;

				//R = get_R(R,DV,!isGJD);   818 ��ʱ ��ʱû������Ʋ���
				//JDarray[k].l1 = get_l0(R,DV,!isGJD);
				//JDarray[k].l2 = get_l0(R,DV,!isGJD);
			}


			
			
				
		}
			
	}
	return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}

//��ѯ������׼�о�n_o_dR�����Բ���߰뾶���仺��
//����Բ���߰뾶n_o_dR�ͻ���
//�϶�Բ�����е�ʱ�������еļ�����׼Ϊȫ�ֱ���
int RHA::SearchProximalRadius(double &n_o_fR)
{
	if(m_Rs.size() > 1)//�����м�����׼����ʱ
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

	//��·�� ��·����
	m_XLName=xlName;
	if (xlShuXing == L"")
		XLShuXing=GetXLAtrri(xlName);
	else
		XLShuXing=xlShuXing;

	
	if (mdbName == L"")
		m_mdbName=GetCurProjName();
	else
		m_mdbName=mdbName;

	//��ǰ�ĵ���
	CCreatPM::CurDocIndx(m_iDoc);

	JDarray.clear();
	QXB.clear();
	GJDInforArray.clear();

	//���㸳ֵ
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

	//������������
	ModifyExJDXY();

	//������Ԫ
	IsMove=false;
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);

	//����Ĭ�϶���
	DLarray.clear();
	setDLM(0, 0, 0, L"K", L"K", 0.0, 0);
	setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH, L"K", L"K", QXB[NJD-1].KZH, 0);//��·ת�� ���߱�Ϊ�� array ҲΪ��818

	//���ñ�־���
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		BZLCarray.clear();
	else
		GenBZLCbyDL();
	
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	InitJBCS();
}

RHA::RHA(CArray<JDarrayStruct,JDarrayStruct> &jd2, CString pGH, CString JDName, CString xlName, CString mdbName, CString xlShuXing, BOOL isSetDrawParam)//818 ���һ������Ŀǰֻ�ǶԼ����߻ָ��е���ʱ��Ϊfalse
{
	if (xlName.IsEmpty() == true)
	{
		ads_printf(L"\n��·��Ϊ��");//nan
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

	//��ǰ�ĵ���
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

	//������������
	ModifyExJDXY();

	//������Ԫ
	IsMove=false;
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);
	
	//���ö���
	DLarray.clear();
	setDLM(0, 0, 0,  L"K", L"K", 0.0, 0);
	setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH,  L"K", L"K", QXB[NJD-1].KZH, 0);//818 �ںŸ�ֵ pGH??

	//���ñ�־���
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
		ads_printf(L"\n��·��Ϊ��");//nan
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

	//��ǰ�ĵ���
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

	//��������
	ModifyExJDXY();

	//������Ԫ
	IsMove=false;
	getpxy_DATA(/*JDarray, QXB, array, array_pxy, tarray, m_oldNJD*/);

	//���ö���
	DLarray.clear();
	setDLM(0, 0, 0,  L"K", L"K", 0.0, 0);
	setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH,  L"K", L"K", QXB[NJD-1].KZH, 0);//818 �ںŸ�ֵ pGH??

	//���ñ�־���
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		BZLCarray.clear();
	else
		GenBZLCbyDL();

	//���ɹ�ͨ������
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	InitJBCS();
	
}


/*===================================================================
������:���캯��
------------------------------------------------------------
��������:��·ת�� �� ��ʼ������
------------------------------------------------------------
����:
xlName              Ŀ����·��
shuXing             Ŀ����·����
pSrcRHA             Դ��·
xjj                 Ŀ����·��Դ��·֮����߼�࣬��- ��+
------------------------------------------------------------
���: �µ�ƽ����· RHA
------------------------------------------------------------
�㷨����:
1 ��������ߺͼ�����֮���ת���ͳ�ʼ���������ʱ̬��Դ��·��ǰĿ����·�ں�
����ôĿ����·��ȫ����ԭ��·����֮����Ҫ���½���Ŀ����·�Ĳ��չ�ϵ
------------------------------------------------------------
��д: ��˧ 2016.7.24  �޸� 2017.6.1
------------------------------------------------------------
��ע��
=====================================================================*/
RHA::RHA(CString xlName, CString shuXing, const RHA *pSrcRHA, double xjj)
{
	if (pSrcRHA->JDarray.size()<2)
	{
		ads_printf(L"Դ��·�Ľ�����С��2��\n");
		return;
	}

	if ((shuXing == LSX || shuXing == SGBX) && shuXing == pSrcRHA->XLShuXing)
	{
		ads_printf(L"��ʱ��֧����ʱ��ת��ʱ�ߣ�ʩ������תʩ�����ߣ�\n");
		return;
	}

	xlName.MakeUpper();
	xlName.Trim();

	m_XLName=xlName;
	XLShuXing=shuXing;
	m_mdbName=pSrcRHA->m_mdbName;
	IsMove=false;

	//��ǰ�ĵ���
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
	 

	//���߼��ƫ�ƺ���ý������꣬��JDarray��N/E
	GetJDXYwhenMoveXLbyXJJ(pSrcRHA, xjj);

	if (shuXing != SGBX && shuXing != LSX
		&& pSrcRHA->XLShuXing != SGBX && pSrcRHA->XLShuXing != LSX)
	{//��������ߺͼ�����֮���ת���ͳ�ʼ��

		//�ж�ʱ̬�Ⱥ�
		int valu;
		valu=CmpShuXing(pSrcRHA->XLShuXing, shuXing);

		if (valu<=0)//ʱ̬�ϣ�Ŀ����·��Դ��·��ͬ��Ŀ����·�ں�
		{//Ŀ����·������ȫ����Դ��·����д������Ϣ
			for(int i=0; i<NJD; i++)
			{
				JDarray[i].JDXZ=BXPX;
				_tcscpy(JDarray[i].BLName, pSrcRHA->m_XLName);
				_tcscpy(JDarray[i].BLJDName, pSrcRHA->JDarray[i].JDName);
				JDarray[i].XJJ=xjj;
			}

			//ͬ��Բ�İ뾶����
			//if (fabs(xjj)>0.0001)
			//{
			//	for(int i=1; i+1<NJD; i++)
			//	{
			//		if (pSrcRHA->QXB[i].LorR*xjj>0)//�ڲ�
			//			JDarray[i].R -= fabs(xjj);
			//		else						   //���
			//			JDarray[i].R += fabs(xjj);
			//	}
			//}
			//ͬ��Բ�İ뾶����
			if (fabs(xjj)>0.0001)
			{
				for(int i=1; i+1<NJD; i++)
				{
					CalOneBXJDdata(i, pSrcRHA);
				}
			}
			

			//������Ԫ
			getpxy_DATA();

			//���ɶ���
			GenDL();

			//���ɱ�־���
			if(XLShuXing != SJJX && XLShuXing != SJ2X)
				GenBZLCbyDL(false);
			else
				BZLCarray.clear();
		}
		else
		{
			//�Զ�������չ�ϵ����ֵ���������Ϣ
			UpdateAllData();
		}
	}
	else 
	{
		//����Դ��·pSrcRHA�Ķ���������
		//�������ڸ�������ʱ�ᾡ���ܱ�������������
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
		
		//�������ݣ��Զ�������չ�ϵ����ֵ���������Ϣ
		UpdateAllData();
	}

	//GetT0min_Lrmin(); 818 ������
	//RTolCheckUp( IsGaiJian );
	
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
		Create2XGTDLB();

	InitJBCS();
}


/*�������ã��ɽ�������(JDarray)�������߱�(QXB)����Ԫ����(array)����·��(xl_length)
�㷨��1 �γ���ʱ��Ԫ����array_pxy
	  2 ����ʱ��Ԫ�����γ���Ԫ����array
*/
Adesk::Boolean  RHA::getpxy_DATA(int moveJDix)
{   // ���߷��ɵ��������ļ�����λ L.SC.S.L.S.C.S.L...  �õ���Ԫ�ļ�

	double fb, fe, aa;//f-��λ��, aaת��ǣ�b-begin��e-end
	double db,de/*,lcb,lce*/;//d-���룬lc-���
	double T0=0.0;
	ACHAR zy[2], GH[20];//���ң��ں�
	double BPMTL[10], T1, T2, L;//����Ҫ��
	//double DDlength;
	int i=0 ,j=0, k=0;
	int color=2,iq=2,c;
	int JdNum=0, localJdNum=0;//������,�ı�������
	int effectSJD, effectEJD;//Ӱ����Ԫ��������ս���
	bool isCalAll=false;//�Ƿ�������н���

	if (!IsMove || (IsMove && IsMoveDone))
		isCalAll=true;

	for (i=0;i<10;i++) 
		BPMTL[i]=0.0;

	//����Ԫ����array[][10]��array_pxy[][4][5] ���ռ䡢��ֵ
	
	JdNum=JDarray.size();

	effectSJD=0;
	effectEJD=JdNum-1;
	if (!isCalAll && moveJDix>-1)
	{
		//ֻ����array_pxy�ļ�����Ԫ
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
		//ȫ������
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
		//		for (i=0;i<m_oldNJD*4;i++)//oriJdNumΪǰһ��ͼ�εĽ�����
		//		{
		//			delete[] array[i];//818 ���������⣬��ҵ�
		//		}
		//		delete[] array;
		//	}
		//	else
		//		ads_printf(L"m_oldNJDû��ֵ��\n");
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
		
	//�����ͷ����
	CalHuitouQX();

	//��㸳ֵ�����յ�뾶ӦΪ0��	

	//���
	//ǰһ�ν������ߵĳ���
	db=xyddaa(JDarray[effectSJD].N,JDarray[effectSJD].E,JDarray[effectSJD+1].N,JDarray[effectSJD+1].E,&fb);//ǰһ�ν������ߵĳ���
	T0=0.0;//ǰһ�ν�������߳�

	array_pxy[0]=10.0;// �������
	array_pxy[1]=JDarray[effectSJD].N; //����
	array_pxy[2]=JDarray[effectSJD].E; //����
	array_pxy[3]=dms_rad(fb);  //��λ�� FA
	array_pxy[4]=0.0/*JDarray[effectSJD].l1*/; //��� LC


	if (isCalAll)
	{
		QXB.clear();
		QXB.resize(JdNum);//818 QXB resize()���ռ�ʧ�� 
		QXB[0].l1 = JDarray[0].l1;
		QXB[0].R = JDarray[0].R;
		QXB[0].l2 = JDarray[0].l2;
		_tcscpy(QXB[0].JDName , JDarray[0].JDName);
		QXB[0].IsGJ = JDarray[0].IsGJ;
		QXB[0].KZH=0.0;  // ������
		QXB[0].KHY=QXB[0].KZH;  
		QXB[0].KYH=QXB[0].KZH;
		QXB[0].KHZ=QXB[0].KZH;
		QXB[0].a = QXB[0].T1 = QXB[0].T2 = QXB[0].L ;
		QXB[0].LorR = 0;

	}

	if (JdNum==2) // ������������,��һ��ֱ��ʱ
	{  
		array_pxy[1*20]=1.0; //
		array_pxy[1*20+1]=db;  
	};

	//DDlength=QXB[0].KZH;// ������

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
		aa = ZXJ(fe,fb);//ת��

		if (aa>0) 
		{  c=1; _tcscpy(zy,L"y"); }  //��ƫ
		else
		{ c=-1;_tcscpy(zy,L"z"); } //��ƫ

		aa=fabs(aa);
		aa=dms_rad(aa);

		//�������߳�
		getBPMTL(aa,JDarray[i].R,JDarray[i].l1,JDarray[i].l2,&BPMTL[0]); 
		T1=BPMTL[6];
		T2=BPMTL[7];
		L=BPMTL[8];  //���߳�

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
				if(fabs(TempT1+T2-de)>PRECISION)//��Ҫ��������
				{
					double lo=Adjust_L_JDLuan(aa,R,RB,Lo1,RE,Lo2,tt,qorh);

					if(lo > 0.0)//�н�
					{
						Lo2 = lo;
						get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
						A2=sqrt(RE*R/fabs(RE-R)*Lo2);
						JDarray[i].l2 = Lo2;

					}
				}

			}
		}

		//�������һ�λ�������Ϊ�������������ߵ����
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
			ads_printf(L"����%s������ ǰ�󻺺������ص�������L1+L2=%10.3f>�������߳�L=%10.3f����������Ϊ0\n",JDarray[i].JDName,JDarray[i].l1+JDarray[i].l2,L);
			//	return 0;
			//���ߴ����ӣ��Զ���������Ϊ0�ټ��㡣 ls 2016
			JDarray[i].l1=0.0;
			JDarray[i].l2=0.0;
			getBPMTL(aa,JDarray[i].R,JDarray[i].l1,JDarray[i].l2,&BPMTL[0]); 
			T1=BPMTL[6];
			T2=BPMTL[7];
			L=BPMTL[8];  //���߳�

		}
		else if((L-JDarray[i].l1-JDarray[i].l2)<20.0 && fabs(JDarray[i].R)>0.001)
		{ 
			//ads_printf(L"\n %d�����ߵ�(R=%8.1f)Բ���߳�L=%10.3f ����20m ",i,JDarray[i].R,L-JDarray[i].l1-JDarray[i].l2);
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
				ads_printf(L" %s����ǰһ����HZ/YZ���Ϸ�\n",JDarray[i].JDName);
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

		//aa ԰���߳�
		aa=L-JDarray[i].l1-JDarray[i].l2;
		cirLen=aa;

		if ( aa>0.000000001) 
		{ 
			array_pxy[num*20+2*5]=2.0;  //����
			array_pxy[num*20+2*5+1]=fabs(JDarray[i].R); //R
			array_pxy[num*20+2*5+2]=aa; //yuan  chang 
			array_pxy[num*20+2*5+3]=0.0;
			array_pxy[num*20+2*5+4]=c;
		}
		else
		{
			array_pxy[num*20+2*5]=2.0;  //����
			array_pxy[num*20+2*5+1]=fabs(JDarray[i].R); //R
			array_pxy[num*20+2*5+2]=aa; //yuan  chang 
			array_pxy[num*20+2*5+3]=fe ;

			array_pxy[num*20+2*5+4]=c;

			//������ֵ

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
			//����Ҫ�� �Ƕ�Ϊ��.����
			QXB[i].a=rotAng;
			QXB[i].T1=T1;
			QXB[i].T2=T2;
			QXB[i].L=L;
			QXB[i].LorR = c;

			//ZH (zy)
			QXB[i].KZH=QXB[i-1].KHZ+lineLen;

			// HY
			//HY���
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
		db=db-T0 ;  //�յ㳤��
		if ( db>0.0000000001) 
		{
			array_pxy[(localJdNum-1)*20]=1.0;	 
			array_pxy[(localJdNum-1)*20+1]=db;
		};

	

	}

	// �յ����
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
		
		//  ���
		double INDATA[5],l,L0;
		xlpoint PB0,PEZ;
		for (j=0;j<5;j++)
			array[0][j]=array_pxy[j]; 
		tarray = 1;


		//����2ά ��Ԫ���� 10
		for( i=1;i<JdNum-1;i++) 
		{ 
			for (k=0;k<4;k++) 
			{		   
				for (j=0;j<5;j++) 
					INDATA[j]=array_pxy[i*20+k*5+j];

				j=(int)(INDATA[0]+0.01);
				l=0.0;
				//������Ԫ����
				if (j==1) l=INDATA[1];//ֱ��
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
				//��ʱ��Ҫ

				// ��L0=��
				L0=l;  //��Ԫ���� 
				if (j==2) L0=INDATA[1]; //R
				//		   ads_printf(L"l=%lf\n",l);


				if (L0>0.001)
				{
					for (int kk=0;kk<5;kk++) 
						array[tarray][kk]=array_pxy[i*20+k*5+kk]; 

					if(j==1)//ֱ��
					{
						db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
						array[tarray][5]=fb ;
						array[tarray][6]=QXB[i-1].KHZ; //HZ  ��Ԫ������
						array[tarray][7]=1e40;  //R
						array[tarray][8]=JDarray[i-1].N+QXB[i-1].T2*cos(fb);  //�������
						array[tarray][9]=JDarray[i-1].E+QXB[i-1].T2*sin(fb);
					}


					// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array[trow][0],array[trow][1],array[trow][2],array[trow][4]);
					tarray=tarray+1;  
				}

			};
		};
		//  �յ� ��Ԫ

		for (j=0;j<5;j++)
			array[tarray][j]=array_pxy[(JdNum-1)*20+j]; 	
		if(JdNum>2)
		{
			i=JdNum-1;  //�յ���Ԫ  
			db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
			array[tarray][5]=fb;   
			array[tarray][6]=QXB[i-1].KHZ;  //zhong dian licheng  HZ
			array[tarray][7]=1e40;
			array[tarray][8]=JDarray[i-1].N+QXB[i-1].T2*cos(fb);
			array[tarray][9]=JDarray[i-1].E+QXB[i-1].T2*sin(fb);
		}	

		/*
		//0����Ԫ���� 1-ֱ�ߣ�2-Բ��3-ǰ����4-��, 5-��������R��->RС, 6-��������RС->R��

		//(R1>R2)/////////////////////////////////////////////////////////////////////////
		//1:			L		R    A         A              A                A        //
		//2:					L	 R         R              R1               R2       //
		//3:                                                  R2               R1       //
		//////////////////////////////////////////////////////////////////////////////////
		//4: ����ת
		//5: ��ʼ���߷�λ��
		//6: ��Ԫ�����̣�=ͳһ���=��·�������+���յ㵽��·�������߾��룩
		//7: �뾶 R
		//8��9����Ԫ������� N,E
		//���һ����ԪΪ�յ�
		*/

		tarray=tarray+1;
		i = 0;
		PB0.lc=array[0][4];  // ���
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
			// �� i ��Ԫ
			for (k=0;k<5;k++) INDATA[k]=array[i][k];

			j=(int)(array[i+1][0]+0.0001);  //** i+1
			if(j==1)//��һ��Ԫ��ֱ��   
			{
				PB0.a=array[i+1][5];
				PB0.lc=array[i+1][6];
				PB0.r=array[i+1][7];
				PB0.x=array[i+1][8];
				PB0.y=array[i+1][9];
			}
			else
			{
				l=Get_INDATA_L(INDATA,&j);  // ������Ԫ����
				xlzbxy(PB0,PEZ,l,INDATA,0);   // ��������   //***********//

				array[i+1][5] = PEZ.a ;  //��Ԫ��㷽λ��

				//Բ+��·����Ϊ0
				//	if( fabs(array_pxy[i][k][0]-2)<0.00001 && l<0.00000001 ) array[i+1][5] = INDATA[3];



				array[i+1][6] = PEZ.lc;  // ����Ԫ �յ�����
				array[i+1][7] = PEZ.r ;//��Ԫ���İ뾶,ֱԲ\ԲֱΪ�����
				array[i+1][8] = PEZ.x ;
				array[i+1][9] = PEZ.y ;
				PB0=PEZ;   //����
			}
		};

		xl_length = QXB[JDarray.size()-1].KZH - QXB[0].KZH; 



		//�γɼ�ֱ������ ls 2016.7.30
		//818 �Ƿ�ʩ�����ߺ���ʱ�߲�������ֱ�ߣ��迼����ʱ��ת�����

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
		ads_printf(L"����λ��������Ӷ���ʧ��\n");
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
������:TrsCkmltoProjml
------------------------------------------------------------
��������:�ֳ����תͶӰ��̣����Σ�
------------------------------------------------------------
����:
ckml                   �ֳ����
projml                 ͶӰ���
------------------------------------------------------------
���:�����������̷���true����֮����false
------------------------------------------------------------
�㷨����:�������е���·�������жϸ������������·��һ�ߣ������ж��ϣ�����ڣ�
����ݶ�������õ���һ���ϵ�������̣�����ͶӰ��ϵ��������ϵ�ͶӰ���
------------------------------------------------------------
��д: ��˧ 2016.4.8  
------------------------------------------------------------
��ע��1 ���øú���ǰ��Ӧ�ø��ݶ����������ж϶����Ƿ���ڣ��������ж�������
2 ע�⣺���øú���ǰ��Ҫȷ����·��������  818
=====================================================================*/
bool RHA::TrsCkmltoProjml(const ACHAR ckml[], double& projml) const
{
	double cml;
	Int16 result=0;//������ =0��û�ҵ���=1���ҵ�

	projml=0.0;
	if(XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
	//һ��
		//ͳһ���
		cml=TYLC1(ckml, DLarray);
		projml=CalcmlTonewcml(cml);
		result=1;
	}
	else
	{
	//����
		double XLC;//�ֳ����
		ACHAR str[16];
		CString GH, bfEGH;//���ֳ���̵Ĺں�, ����̵�ǰһ�������Ϻ�ں�
		int dlNum; //������
		double dlCml;//�������������
		double cml1x;//��һ���ϵ�ͶӰ���
		Int16 mark=0;//���
		CString mes;


		split_ckml(ckml, &XLC, str);
		GH=str;
		GH.MakeUpper();


		//�������п��ܵ���·���������ܵĽ��ͣ����磺���ж��߲����ܲ�����ƻ��ߣ�
		Int16 xlnum=g_DocManageAry[m_iDoc].XLAry.size();
		RHA *pRHA=NULL;

		for (int i=0; i<xlnum; i++)//
		{
			pRHA=g_DocManageAry[m_iDoc].XLAry[i];//pRHA�ֱ�ȡJYJX JY2X SJ2X

			//�Ƚ������ߵ���·���ԣ�����·��ʱ̬���벻��pRHA֮ǰ��������pRHA�����ж������
			if (CmpShuXing(pRHA->XLShuXing, XLShuXing)>0)
				continue;

			if(pRHA->m_mdbName != m_mdbName	|| pRHA->DLarray.size() == 0)//��û�ж���
				continue;

			dlNum=pRHA->DLarray.size();

			for (int j=0; j<dlNum-1; j++)
			{
				bfEGH=pRHA->DLarray[j].EGH;
				bfEGH.MakeUpper();
				//2 �����ڶ�����֮��Ķ�����
				if (XLC>pRHA->DLarray[j].ELC-0.001 
					&& XLC<pRHA->DLarray[j+1].BLC+0.001 
					&& GH == bfEGH)
				{
					//3 �ж��Ƿ������ж���
					if (pRHA->DLarray[j].Indx>-1 
						&& pRHA->DLarray[j].Indx 
						== pRHA->DLarray[j+1].Indx) 
					{
						mark++;//ÿ��һ�������д��ڣ�mark��һ������һ����������������ϵĶ���
						if (mark >1)
						{
							mes.Format(L"�����ֳ����%s�����ڶ����ͬ�����ж���\n", ckml);
							ads_printf(mes);
							//return false;
						}

						//ǰһ������һ���ϵ�ͳһ���
						dlCml=TYLC1(pRHA->DLarray[j].BGH, pRHA->DLarray[j].BLC, pRHA->DLarray);

						//4 ����ͶӰ��̣���ֱ��һ�ߣ�
						if(pRHA->m_XLName == m_XLName)
						{//������������
							
							if (XLShuXing == JYJX || XLShuXing == JY2X)
							{//������
								cml1x = dlCml + XLC - DLarray[j].ELC;//�õ��ͳһ���
								projml=CalcmlTonewcml(cml1x);//ͶӰ���
							}
							else//�����
								projml=DLarray[j].TYLC + XLC - DLarray[j].ELC;

							result++;
						}
						else
						{//����������

							//������һ���ϵ�ͶӰ���
							double projml1x=0.0;
							if (pRHA->XLShuXing == JYJX 
								|| pRHA->XLShuXing == JY2X)
							{//������
								cml1x = dlCml + XLC - pRHA->DLarray[j].ELC;
								projml1x=pRHA->CalcmlTonewcml(cml1x);
							}
							else//�����
								projml1x=pRHA->DLarray[j].TYLC + XLC 
								- pRHA->DLarray[j].ELC;

							//һ�߷�ͶӰ�����ߣ���������ϵ�ͶӰ���
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

//�ֳ����תͶӰ���
bool RHA::TrsCkmltoProjml(const ACHAR gh[], double dml, double& projml) const
{
	ACHAR ckml[50];

	_tcscpy(ckml, LCchr(gh, dml, 6));
	return TrsCkmltoProjml(ckml, projml);
}

/*===================================================================
������:TrsCkmltoCml
------------------------------------------------------------
��������:�ֳ����ת�������
------------------------------------------------------------
����:
ckml              �ֳ����
cml               �������
------------------------------------------------------------
���:�����������̷���true����֮����false
------------------------------------------------------------
�㷨����:1 һ��: �ɶ��������ͳһ���
         2 ����: �������е���·�������жϸ������������·��һ�ߣ������ж��ϣ�����ڣ�
           �͸��ݶ�������õ���һ���ϵ�������̣�����ͶӰ��ϵ��������ϵ��������
------------------------------------------------------------
��д: ��˧ 2016.7.19  
------------------------------------------------------------
��ע�����øú���ǰ��Ӧ�ø��ݶ����������ж϶����Ƿ���ڣ��������ж�������
=====================================================================*/
void RHA::TrsCkmltoCml(ACHAR ckml[], double &cml) const 
{
	cml=TYLC1(ckml, DLarray);//ͳһ��Ϊ��� 818

	//Int16 result=0;//������ =0��û�ҵ���=1���ҵ�

	//if(XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	//{
	//	//һ��
	//	//ͳһ���
	//	cml=TYLC1(ckml, DLarray);//ͳһ��Ϊ��� 818 
	//	result=1;
	//}
	//else
	//{
	//	//����
	//	ACHAR str[16];
	//	CString GH, bfEGH;//���ֳ���̵Ĺں�, ����̵�ǰһ�������Ϻ�ں�
	//	int DLNum; //������
	//	double cml1x;//��һ���ϵ��������
	//	Int16 mark=0;//���
	//	double  XLC=0;//�ֳ����

	//	split_ckml(ckml, &XLC, str);
	//	GH=str;
	//	GH.MakeUpper();
	//	
	//	//1 �������е���·������
	//	Int16 xlnum=g_DocManageAry[m_iDoc].XLAry.size();
	//	for (int i=0; i<xlnum/*g_XLNameAry.GetSize()*/; i++)
	//	{
	//		if(g_DocManageAry[m_iDoc].XLAry[i]->m_mdbName != m_mdbName
	//			|| g_DocManageAry[m_iDoc].XLAry[i]->DLarray.size() == 0)//��û�ж���
	//			continue;

	//		DLNum=g_DocManageAry[m_iDoc].XLAry[i]->DLarray.size();

	//		for (int j=0; j<DLNum-1; j++)
	//		{
	//			bfEGH=g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].EGH;
	//			bfEGH.MakeUpper();
	//			//2 �����ڶ�����֮��Ķ�����    818 �ж�g_DocManageAry[m_iDoc].XLAry[i]�����ݿ��Ƿ���ͬһ���ݿ�
	//			if (XLC>g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].ELC-0.001 && XLC<g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j+1].BLC+0.001 
	//				&& GH == bfEGH)
	//			{
	//				//3 �ж��Ƿ������ж��ڡ�
	//				if (g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].Indx>-1 
	//					&& g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].Indx == g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j+1].Indx) /*GetRXDIndx(j, g_DocManageAry[m_iDoc].XLAry[i]->DLarray) >= 0*/
	//				{
	//					//�����ж�
	//					mark++;
	//					if (mark >1)
	//					{
	//						//ads_printf(L"���󣺸��ֳ���̴�����������ͬ�����ж���\n"); 818
	//						return false;
	//					}

	//					cml1x = g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].TYLC + XLC - g_DocManageAry[m_iDoc].XLAry[i]->DLarray[j].ELC;
	//					//4 ����ͶӰ��̣���ֱ��һ�ߣ�
	//					if(g_DocManageAry[m_iDoc].XLAry[i]->m_XLName == m_XLName)//������������
	//					{
	//						cml=cml1x;
	//						result++;
	//					}
	//					else//����������
	//					{
	//						double projml1x;//��һ���ϵ�ͶӰ���
	//						double projml;//�ڶ����ϵ�ͶӰ���
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
������:TrsProjmltoCkml
------------------------------------------------------------
��������:ͶӰ���ת�ֳ����
------------------------------------------------------------
����:
projml            �������
ckml              �ֳ����
nLC               �ֳ���̱�����С��λ��
------------------------------------------------------------
���:������ֳ���̷���true����֮����false
------------------------------------------------------------
�㷨����:�ɶ��ߵ�������̼�����õ��Ƿ����������·����������գ�
�ڶ����ϼ������ֳ���̣�������գ�����ڲ����ߣ�һ�ߣ���
ͶӰ��̣������ͶӰ����ڻ��ߵ��ֳ���̡�
         �������ʱ������GetBLIndx��������ԭʼ�Ĳ����ߣ�eg:��ƶ��߸���̶β���
��ƻ��ߣ���ƻ��߲��ռ��л��ߣ������ҵ����л��ߡ�
------------------------------------------------------------
��д: ��˧ 2016.4.12  
------------------------------------------------------------
��ע�����øú���ǰ��Ӧ�ø��ݶ����������ж϶����Ƿ���ڣ��������ж�������
=====================================================================*/
bool RHA::TrsProjmltoCkml(double projml, ACHAR ckml[], int nLC) const
{
	double projml1x;//�ڲ������ϵ�ͶӰ���
	double cml=0;//�ڸ����ϵ��������
	double cml1x=0;//�ڲ������ϵ��������
	int dlNum;
	ACHAR GH[16];
	double XCLC=0.0;//�ֳ����
	double dlCml=0.0;//������ͳһ���


	_tcscpy(ckml, L"");

	//һ��һ��
	if(XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX)
	{
		dlNum=DLarray.size();
		for (int j=0; j<dlNum-1; j++)
		{
			//2�������ڶ�����֮��Ķ�����
			if (projml > DLarray[j].TYLC-0.001 
				&& projml < DLarray[j+1].TYLC+0.001)
			{

				//818 ��ʱ��Ϊ��ʱ�ߺ�ʩ������Ҳ�������ӱ�־���
				_tcscpy(GH, DLarray[j].EGH);
				cml=CalnewcmlTocml(projml);//ͳһ���
				XCLC=XLC1(cml, GH, DLarray);//�ֳ����
				
				_tcscpy(ckml, LCchr(GH, XCLC, nLC));
				return true;

			}
		}

	}
	else
	{
	//��������
		//1����ȡ��ԭʼ�Ĳ�����ָ�뼰�ڲ������ϵ�ͶӰ���
		//ע�⣺�õ���projml1xһ����pRHA_BL�����ж���

		RHA *pRHA_BL=NULL;
		pRHA_BL=GetBasLine(projml, projml1x);//��ԭʼ�Ĳ�����

		if (pRHA_BL)
		{
			//�����һ���ϵ�ͶӰ��� projml1x
			dlNum=pRHA_BL->DLarray.size();
			for (int j=0; j<dlNum-1; j++)
			{
				//2�������ڶ�����֮��Ķ�����
				if (projml1x > pRHA_BL->DLarray[j].TYLC-0.001 
					&& projml1x < pRHA_BL->DLarray[j+1].TYLC+0.001)
				{
					//�ںŸ�ֵ
					_tcscpy(GH, pRHA_BL->DLarray[j].EGH);	

					if (pRHA_BL->XLShuXing == JYJX
						|| pRHA_BL->XLShuXing == JY2X)
					{//������

						//ͶӰ���תͳһ���
						cml1x=pRHA_BL->CalnewcmlTocml(projml1x);

						//�����ֳ������ֵ
						XCLC=pRHA_BL->XLC1(cml1x, GH, pRHA_BL->DLarray);
					}
					else
					{//�����

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

//��������:ͶӰ���ת�ֳ����
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
������:GetBasLine    
------------------------------------------------------------
��������:��ͶӰ�����õ���ԭʼ�Ĳ����ߣ����ز�����ָ��
------------------------------------------------------------
����:
	   projml2x   �õ��ͶӰ���
	   blix       �õ�Ĳ�����������g_DocManageAry[m_iDoc].XLAry���±�
------------------------------------------------------------
���:  projml1x   �õ��ڲ������ϵ�ͶӰ���
------------------------------------------------------------
����ֵ:  ������ָ�룬���û�ҵ����ؿ�
------------------------------------------------------------
�㷨:  ����õ������жΣ������߾��Ǳ����ڲ��жξ������Ĳ����ߣ�������Ĳ�����
       �������˱���ߣ��ͼ�������ȥ,�ҵ���ԭʼ�Ĳ�����
------------------------------------------------------------
��д: ��˧ 2016.4.9  
=====================================================================*/
RHA* RHA::GetBasLine(double projml2x, double &projml1x) const
{
	int dlNum=0;
	ACHAR blName[50];
	Int16 mark=0;
	RHA *pRHA=NULL;
	RHA *pRHA_BL=NULL;
	CString mes;

	//����·��ȫ������g_DocManageAry��ָ��
	pRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, m_XLName);

	if (pRHA == NULL)
	{
		mes.Format(L"������̼���ʱ����[%s]����ȫ��������\n", m_XLName);
		ads_printf(mes);//818 ɾ����·���ڳ���ʱ��
		return pRHA;
	}

	do 
	{
		dlNum=pRHA->DLarray.size();
		if (dlNum<2)
		{
			ads_printf(L"������С��2\n");
			return NULL;
		}

		mark=0;//��־
		_tcscpy(blName,L"");

		int sdl=0, edl=0;
		for(int i=0; i<dlNum-1; i++)
		{
			sdl=i;
			edl=-1;
			for(int j=sdl+1; j<dlNum; j++)
			{
				if(pRHA->DLarray[j].Indx != -3)//�����Ӳ������ϸ��ƵĶ���
				{
					edl=j;
					break;
				}
			}

			if (edl == -1)
			{
				ads_printf(L"��ʾ������������ô���\n");
				return NULL;
			}
			
			if(projml2x > pRHA->DLarray[sdl].TYLC-0.001 
				&& projml2x < pRHA->DLarray[edl].TYLC+0.001)//818 ���ڴ˶��䣬���Ǽ�����
			{
				if (pRHA->DLarray[sdl].Indx>-1 
					&& pRHA->DLarray[sdl].Indx == pRHA->DLarray[edl].Indx)
				{//�����ж���  

					projml1x=projml2x;
					return pRHA;
				}
				else                    
				{//�ڲ��ж���

					//��ò�������
					_tcscpy(blName, L"");
					if(pRHA->DLarray[sdl].Indx != -3)
					{//���жζ��� �� ���жηֶζ�����ֱ�Ӹ��Ʋ�������
						_tcscpy(blName, pRHA->DLarray[sdl].BLName);
					}
					

					if (_tcscmp(blName, L"") == 0)
					{
						ads_printf(L"����ĳͶӰ��̵Ĳ�������Ϊ��\n");				
						return NULL;
					}

					//������ָ��
					pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);

					if (pRHA_BL)     
					{ //����ͶӰ���

						if(CalXJJ(projml2x, pRHA->array, pRHA->tarray
							, pRHA_BL->array, pRHA_BL->tarray, projml2x, false))
						{
							pRHA=pRHA_BL;//�滻������
							mark=1;
							break;
						}
						else
						{
							ads_printf(L"һ���ڶ���ͶӰ�������δ�ҵ�\n");				
							return NULL;
						}

					}
					else               
					{
						ads_printf(L"ĳ������̵Ĳ�����ʵ��δ�ҵ�\n");				
						return NULL;
					}
				}
			}

			i=edl-1;
		}

		if (mark == 0)
			return NULL;//ͶӰ��̲��ڶ���֮��

	} while (1);

}


/*===================================================================
������:TrsProjmltoCkml
------------------------------------------------------------
��������:ͶӰ���ת����
------------------------------------------------------------
����:
Projml            ͶӰ���
PZ                ��·�㣨��̣����꣩
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
������:TrsCmltoNE
------------------------------------------------------------
��������:ͳһ���ת����
------------------------------------------------------------
����:
Cml               ͳһ���
���:
N��E              ����̵�����
------------------------------------------------------------
��д����˧ 2016.12.16
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
������:TrsCkmltoNE
------------------------------------------------------------
��������:�ֳ����ת����
------------------------------------------------------------
����:
Ckml              �ֳ����
���:
N                 γ��
E                 ����
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
������:LocatHmOnLine   
------------------------------------------------------------------------
��������:��λ������ֱ�߶�[sml, eml]�ϵİ��ױ�
------------------------------------------------------------------------
����:
sProjml                ֱ�߶����ͶӰ���
eProjml                ֱ�߶��յ�ͶӰ���
directn            �������յ���ǰȡ�� =true����������ȡ�� =false
CKml[]             ȡ�����ֳ���̣����ںţ������
------------------------------------------------------------------------
���: ȡ���ɹ�������������ϵ��ֳ����CKml�������ڲ������ϵ�ͶӰ���tylc�����򷵻�-100
------------------------------------------------------------------------
��ע: 1 ��Ϊһ�ߵ��ຯ������һ��ʵ�����
      2 ���ڴ��������������жε������Ҫ������ֹ���������е�����ʼ��Ҫ��¼���Ӧ�����ߺͶ�Ӧ���յĲ��ս��㡣
	    һ�������ʼ����ǰһ������Ĳ��ս�������ô��Ĳ����߽��㡣
	  3 Ҫ���ҵ��İ��ױ���һ�ߣ������ߣ���ֱ�߶���
------------------------------------------------------------------------
��д: ��˧ 2016.6.17  
=====================================================================*/
double RHA::LocatHmOnLine(double sProjml, double eProjml, bool directn, ACHAR CKml[]) const
{	
	ACHAR GH[16]=L"";
	double dml=0, dmlcpy=0;
	double projmlcpy=0;
	double intdml=0;
	double Intdist=0;
	double DLdist=0; 
	int DLIx=0;//�������
	double tylc=0;
	double projml=0;
	double dlCml=0;

	_tcscpy(CKml, L"");

	//1 ��ȡ�ֳ����ֵ��double��
	if(sProjml+0.1 > eProjml)
		return -100;

	if (directn)
		projml=eProjml;
	else
		projml=sProjml;

	TrsProjmltoCkml(projml, CKml, 1);
	split_ckml(CKml, dml, GH);

	for (Int16 size=100; size >= 1; size=size/10)//ȡ��������̱꣬������ȡ����ȡʮ�ס�һ��
	{
		dmlcpy=dml;
		projmlcpy=projml;

		do 
		{
	//2 ȡ�����ֳ����
			if (directn)
			{
				intdml=floor(dmlcpy/size)*size;//��ǰȡ�����ֳ����
				DLIx=FindNearDL(projmlcpy, DLarray, true);
			}
			else
			{
				intdml=ceil(dmlcpy/size)*size;//���ȡ��
				DLIx=FindNearDL(projmlcpy, DLarray, false);
			}

			if(DLIx<0)
				return -100;

			Intdist=fabs(dmlcpy-intdml);//ȡ���������ȡ��ǰ��̵ľ���
			DLdist=fabs(projmlcpy - DLarray[DLIx].TYLC);//ǰ���һ�������㵽projmlcpy�ľ���
		
	 //3 ����ȡ�����ֳ�����Ƿ��Խ����
			if (Intdist < DLdist+0.001)//û�п����	 //818 Intdist��cml֮�DLdist��projml,Ӧ���޸�	
			{
				if (directn)
					_tcscpy(GH, DLarray[DLIx].EGH);
				else
					_tcscpy(GH, DLarray[DLIx].BGH);

				_tcscpy(CKml, LCchr(GH, intdml, 3));
	 //4 �ж��Ƿ��ڲ����ߵļ�ֱ����
				TrsCkmltoProjml(CKml, tylc);
				if(tylc>sProjml-0.001 && tylc<eProjml+0.001)//���ذ��ױ�
					return tylc;
				else
					break;//����Ѱ����ʮ�ױꡢ���ױ�
			}
			else//�����
			{
				projmlcpy=DLarray[DLIx].TYLC;

				if(projmlcpy>sProjml-0.001 && projmlcpy<eProjml+0.001)// ls 16.9.23 ����������ȡ����
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


//����ͳһ��̣��Ƿ���ǰѰ�Ҹ����������ױ꣬Ҫ����ֱ���� //�ֳ����
double RHA::LocateHundredMeterOnLine(double tylc1x, bool IsForwardTo, ACHAR ckml[]) const
{
	double gettylc=tylc1x;
	double sml, eml;
	int iXY=0;//����������Ԫ
	int XYClass;//��Ԫ����
	ACHAR GH[20];//��̹ں�
	double xlc;//һ����ͶӰ�ֳ����
	xlpoint TmpPZ;//
	int i;

	TrsProjmltoCkml(tylc1x, 3, GH, xlc);
	if (fmod(xlc+0.0001,100.0)<0.001)
	{
		//�Ѿ��ڰ��ױ꣬�����ٶ�λ
		_tcscpy(ckml, LCchr(GH, xlc, 3));
		return tylc1x;
	}
	

	//�жϸ�����Ƿ��ڶ�������
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
	//if (DistToNearDL(tylc1x, DLarray, true)<1.0//818 ǰ���ж�������ζ�λ���ױ�
	//	|| DistToNearDL(tylc1x,  DLarray, false)<1.0)//�ҵ�������ֹ 818 ����ǰ�����С��1��
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

	XYClass=array[iXY][0];//ȡ����Ԫ����
	if(IsForwardTo)//�����ǰ����
	{
		for(; iXY>-1; iXY--)
		{
			XYClass=array[iXY][0];

			if (XYClass == 1)
				break;
		}
		
		//��ʱ��Ԫ��ֱ��
		eml=array[iXY+1][6];//һ��ֱ�������
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

		//��ʱ��Ԫ��ֱ��
		sml=array[iXY][6];//��ֱ���
		if(tylc1x>sml)
			sml=tylc1x;

		gettylc=LocatHmOnLine(sml, array[iXY+1][6], false, ckml);
	}

	return gettylc;
}




/*===================================================================
������:ModifyJDXZ
------------------------------------------------------------
��������:�޸Ľ�������
------------------------------------------------------------
�㷨:1��ȫ�����У�����������ȫ��Ϊ���߽���
     2�������ս���������Ϊ�����У��޸�ΪRX_End
------------------------------------------------------------
��д: ��˧ 2016.4.22  
=====================================================================*/
//void RHA::ModifyJDXZ()
//{
//	int NJD=JDarray.size();
//
//	//ȫ������
//	if (JDarray[0].JDXZ == RX_Start && JDarray[NJD-1].JDXZ == RX_End
//		&& GetRXDNum(JDarray) == 1)//�޸�ΪRX_End
//	{
//		for (int i=0; i<NJD; i++)
//			JDarray[i].JDXZ=BLJD;
//	}
//
//}


/*===================================================================
������:GetRXDNum
------------------------------------------------------------
��������:�����·�����ж���
------------------------------------------------------------
���룺 tmpJDarray   ��������
------------------------------------------------------------
����� RXDNum   ��·�����ж��������û��
------------------------------------------------------------
�㷨: �ҳ���������ΪRX_Start�Ľ����������Ϊ���ж��������û��RX_Start�������жεĿ�ʼ���򷵻�-99
------------------------------------------------------------
��д: ��˧ 2016.5.17  
=====================================================================*/
int RHA::GetRXDNum(vector<JDarrayStruct> &jdAry)           
{
	int RXDNum=0;//���ж���Ŀ
	int JDNum=jdAry.size();
	int SJD;//������ʼ�����

	for(int i=0; i<JDNum; i++)
	{
		/*if (i<JDNum-1)
		{*/
			if(jdAry[i].JDXZ == RX_End || jdAry[i].JDXZ==RX_One)
			{
				ads_printf(L"���ж�Ӧ������ʼ����˲����ݣ�\n");
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
				if (i-1>SJD)//ȱ������ֹ
				{
					jdAry[i-1].JDXZ=RX_End;
					ads_printf(L"���ж�Ӧ������ֹ!\n");
				}
			}
			else
			{
				if (jdAry[i].JDXZ == RX_End)//1 ������ֹ
				{
				}
				else if (i>SJD+1)//2 ȱ������ֹ
				{
					jdAry[i-1].JDXZ=RX_End;
					ads_printf(L"���ж�Ӧ������ֹ!\n");
				}
				else
				{
					//3 ��������
					i--;
				}
				
			}
				
			

			RXDNum++;			
		}


	}
	return RXDNum;
	
}


/*===================================================================
������:GetRXDNum
------------------------------------------------------------
��������:�����·�����ж���
------------------------------------------------------------
���룺 tmpJDarray   ��������
------------------------------------------------------------
����� RXDNum   ��·�����ж��������û��
------------------------------------------------------------
�㷨: �ҳ���������ΪRX_Start�Ľ����������Ϊ���ж��������û��RX_Start�������жεĿ�ʼ���򷵻�-99
------------------------------------------------------------
��д: ��˧ 2016.5.17  
=====================================================================*/
int RHA::GetRXDNum() const           
{
	int RXDNum=0;//���ж���Ŀ
	int JDNum=JDarray.size();
	int SJD;//������ʼ�����

	for(int i=0; i<JDNum; i++)
	{
		if(JDarray[i].JDXZ == RX_End || JDarray[i].JDXZ == RX_One)
		{
			ads_printf(L"���ж�Ӧ������ʼ����˲����ݣ�\n");
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
				if (i-1>SJD)//ȱ������ֹ
				{
					//JDarray[i-1].JDXZ=RX_End;
					ads_printf(L"���ж�Ӧ������ֹ!\n");
				}
			}
			else
			{
				if (JDarray[i].JDXZ == RX_End)//1 ������ֹ
				{
				}
				else if (i>SJD+1)//2 ȱ������ֹ
				{
					//JDarray[i-1].JDXZ=RX_End;
					ads_printf(L"���ж�Ӧ������ֹ!\n");
				}
				else
				{
					//3 ��������
					i--;
				}

			}

			RXDNum++;			
		}


	}
	return RXDNum;
}


/*===================================================================
������:GetRxdSEJD    
------------------------------------------------------------
��������:�õ�ָ�����жε����ս����±�
------------------------------------------------------------
����:
iRXD              �����жεı��
SJDIx       ���ж���㽻���±�
EJDIx         ���ж��յ㽻���±�
------------------------------------------------------------
���:  �ҵ����ս��㷵��true�����򷵻�false
------------------------------------------------------------
�㷨:  1��Ѱ��RX_Start��RX_EndΪһ������
       2���������о���һ��RX_Start
       3��RX_One��Ϊ���жε��յ�ʱ����ΪRX_End
------------------------------------------------------------
��д: ��˧ 2016.7.30  
=====================================================================*/
bool RHA::GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx, vector<JDarrayStruct> &jdAry)
{
	SJDIx = -1;
	EJDIx = -1;
	int RXDNum=0;//���ж���Ŀ
	int NJD=jdAry.size();

	for(int i=0; i<NJD; i++)
	{
		if(jdAry[i].JDXZ == RX_Start)//����ʼ
		{
			SJDIx=i;
			do
			{
				if(i<NJD-1)
					i++;
				else
					break;

			}while(i<NJD-1 && (jdAry[i].JDXZ == RX_End 
				   || jdAry[i].JDXZ == RX_One));//818 ������ Խ�磿

			 //818 δ���Ǹ�������Ľ�������,�޸Ľ�������
			if (i == NJD-1 && (jdAry[i].JDXZ == RX_End || jdAry[i].JDXZ == RX_One))
				i=i;
			else
				i--;
			 
			if(i == SJDIx)//��������
				EJDIx=SJDIx;
			else
				EJDIx=i;
			
			if(iRXD == RXDNum)//�Ƿ�Ϊ�����ж�
				return true;
			RXDNum++;

		}
	}

	return false;	
}

//ȷ���������ж����ս��������
//��ΰ���
bool RHA::GetAllRxdSEJD(vector<JDarrayStruct> &jdAry, vector<int>& SJDArray, vector<int>& EJDArray)
{
	int SJDIx = -1;
	int EJDIx = -1;
	int RXDNum=0;//���ж���Ŀ
	int NJD=jdAry.size();

	for(int i=0; i<NJD; i++)
	{
		if(jdAry[i].JDXZ == RX_Start)//����ʼ
		{
			SJDIx=i;
			do
			{
				if(i<NJD-1)
					i++;
				else
					break;

			}while(i<NJD-1 && (jdAry[i].JDXZ == RX_End 
				|| jdAry[i].JDXZ == RX_One));//818 ������ Խ�磿

			//818 δ���Ǹ�������Ľ�������,�޸Ľ�������
			if (i == NJD-1 && (jdAry[i].JDXZ == RX_End || jdAry[i].JDXZ == RX_One))
				i=i;
			else
				i--;

			if(i == SJDIx)//��������
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



//�õ�ָ�����жε����ս����±�
bool RHA::GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx)
{
	return GetRxdSEJD(iRXD, SJDIx, EJDIx, JDarray);
}
/*===================================================================
������:GenRXBX_DL   
------------------------------------------------------------
��������:�������ж������յ���������ж�ǰ����ղ�ͬ��·�������Ķ���
------------------------------------------------------------
����:
iRXD         ���жα��0/1/2/3 �� ���жα�ʶ-2
jdix         �����±�
dl           �����������
dirctn       �������ͣ�true��������ʼ��false��������ֹ
------------------------------------------------------------
���:  ���ɶ�������true������false
------------------------------------------------------------
�㷨:  ������ʼ���ڲ����ߵļ�ֱ����
------------------------------------------------------------
��ע��1 Ҫ��������ʼ���Ӧ�Ĳ����߽����±겻��Ҫ��֤��ȷ����ǰ�󽻵�Ĳ����߽�����Ƴ���
      2 Ҫ����Ľ��������У�ÿ�����㶼��¼�����һ�������γɵ�ֱ�ߣ���Ӧ����ֱ�߱ߵ�ǰ
        һ��������±�
	  3 ���жζ���ʱ��jdixΪ��������ղ�ͬ��·����ĺ�һ�����±�
	  4 �Զ����ɶ���ʱ�������ɲ��жζ������������ж��������ڼ�齻�������Ƿ�Ӧ��Ϊ����
------------------------------------------------------------
��д: ��˧ 2016.6.22 
=====================================================================*/
bool RHA::GenRXDL(int iRXD, int jdix, DLTab &dl, bool dirctn)
{
	//�������յ����е���� 2016.7.29
	if(jdix == 0 && JDarray[jdix].JDXZ == RX_Start 
		&& GetRXDNum(JDarray)>1)
	{
		//�������
		dl.Indx = iRXD;
		dl.BLC = 0.0;
		dl.ELC = dl.BLC;
		dl.TYLC = 0.0;
		_tcscpy(dl.BGH, L"");// 818 �ں�Ψһ�����⣬�Լ��ںŵ�Ĭ��ֵ
		_tcscpy(dl.EGH, L"CK");
		_tcscpy(dl.BLName, L"");
		_stprintf(dl.RXDName, L"���ж�%0.1lf", dl.ELC/1000.0);
		return true;
	}
	else if(jdix == JDarray.size()-1 && JDarray[jdix].JDXZ == RX_End 
		&& GetRXDNum(JDarray)>1)
	{
		//�յ�����
		_tcscpy(dl.BLName, L"");
		dl.BLC = dl.ELC+QXB[JDarray.size()-1].KZH-dl.TYLC;//dl.TYLC Ϊ����ʼ��ͶӰ��� ls ����δ�������ж��м���� 
		dl.ELC = dl.BLC;
		dl.TYLC = QXB[JDarray.size()-1].KZH;
		_tcscpy(dl.BGH, dl.EGH);
		_tcscpy(dl.EGH, L"");
		return true;
	}


	//�м����жζ���
	ACHAR ckml[50];
	ACHAR blName[50];//��������,ǰһ���߲�����
	RHA *pRHA_BL=NULL;
	double projml1x, projml2x;//�ڲ������ϵ�ͶӰ���
	ACHAR mes[100];

	//һ����λ�������յ㡢���жηֽ��
	if (JDarray[jdix].JDXZ == RX_Start && dirctn)
	{
		/*1 ����ʼ*/  
		int TmpJDIndx;
		double KZH, KZHProjTo1x;
		double KZH1x, KHZ1x; 

		//1.1 ������ʼ�Ĳ����ߡ�ǰһ����������Ϊ����ʼ/ֹ�����С����в�ƽ��ʱ�����������Ϊ��������
		if (jdix>0 && (JDarray[jdix-1].JDXZ == BXPX 
			|| JDarray[jdix-1].JDXZ == BX_NotPX
			|| JDarray[jdix-1].JDXZ == RX_End 
			|| JDarray[jdix-1].JDXZ == RX_Start))                                  
			//818 �ڿ��ǲ���������ʱ����ǰ���ϵ�ƺ����׳���дһ����ϵ�����������������������������������
			_tcscpy(blName, JDarray[jdix-1].BLName);
		else
			_tcscpy(blName, JDarray[jdix].BLName);

		//��ò�����ָ��
		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);
		if (pRHA_BL == NULL)
			return false;

		//1.2 ȷ������ʼ���ױ���ҷ�Χ(KHZ1x, KZH1x)����֤�ҵ��İ��ױ���ֱ�������ڣ��ڲ����ߵ�ֱ����
		KZH=QXB[jdix].KZH;//����ʼ��ZH��Ķ���ͶӰ���
		//��KZHͶӰ���������ϵ�ͶӰ���KZHProjTo1x
		if (!CalXJJ(KZH, array, tarray, pRHA_BL->array, pRHA_BL->tarray, KZHProjTo1x, false))
			return false;

		if(jdix>0)
		{
			GetJdno1FromJdstr(JDarray[jdix-1].BLJDName, pRHA_BL->JDarray, TmpJDIndx);//818 ע���ж�������� TmpJDIndx���� =-1
			if (TmpJDIndx<0)
			{
				_stprintf(mes, L"û���ҵ�����%s��¼�Ĳ����߽���%s", JDarray[jdix-1].JDName, JDarray[jdix-1].BLJDName);
				return false;
			}
		}
		else
		{
			TmpJDIndx=0;ads_printf(L"δ�ҵ��������ϵĽ��㣡\n");
		}
		//TmpJDIndx= JDarray[jdix-1].BLJDName;
		KZH1x=pRHA_BL->QXB[TmpJDIndx+1].KZH;//�������ϣ���ֱ�ߺ�һ�������ZH��ͶӰ���
		KZH1x=min(KZHProjTo1x, KZH1x);

		KHZ1x= pRHA_BL->QXB[TmpJDIndx].KHZ;//�������ϣ���ֱ��ǰһ�������HZ��ͶӰ���

		//1.3 ��λ���ױ꣬�����ڲ������ϵ�ͶӰ���ZHProjml
		projml1x=pRHA_BL->LocatHmOnLine(KHZ1x, KZH1x, true, ckml);
		//1.4 ���Ǹ�����ʼ������ǰһ����ֹ����֮ǰ�����
		if(iRXD>0)
		{
			CalXJJ(projml1x, pRHA_BL->array, pRHA_BL->tarray
				, array, tarray, projml2x, false);

			if (projml2x-0.1<DLarray[DLarray.size()-1].TYLC)//818 ��һ����DLarray.size()-1����Ҫͨ��projml2xȥ�ң���ʱ����
			{
				//ǰһ����ֹ�ͺ�һ����ʼ�ص� 818 ��ʱ����
				//double preDLprojml1x=0;//ǰһ�����ڲ������ϵ�ͶӰ���
				//CalXJJ(DLarray[DLarray.size()-1].TYLC, array, tarray
				//	, pRHA_BL->array, pRHA_BL->tarray, preDLprojml1x, false);

				//if(preDLprojml1x+0.1<KZH1x)
				//{
				//	//���¶�λ���ױ�
				//	projml1x=pRHA_BL->LocatHmOnLine(preDLprojml1x+0.1, KZH1x, true, ckml);
				//}
				//else
				//	ads_printf(L"ǰһ����ֹ�ͺ�һ����ʼ�ص����Ҳ������ʵĶ���λ��\n");
			}
		}
	}
	else if(JDarray[jdix].JDXZ == RX_End 
		    || (JDarray[jdix].JDXZ == RX_Start && dirctn == false))
	{
		/*2 ����ֹ*/
		int TmpJDIndx;
		double KHZ, KHZProjTo1x;
		double KZH1x, KHZ1x;

		//2.1 ������ֹ�Ĳ�����
		if (jdix<JDarray.size()-1 && (JDarray[jdix+1].JDXZ == BXPX || JDarray[jdix+1].JDXZ == BX_NotPX))
			_tcscpy(blName, JDarray[jdix+1].BLName);
		else
			_tcscpy(blName, JDarray[jdix].BLName);

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, blName);//������ָ��
		if (pRHA_BL == NULL)
			return false;

		//2.2 ȷ������ֹ���ױ궨λ��Χ
		KHZ=QXB[jdix].KHZ;//HZ��Ķ���ͶӰ���
		//һ��ͶӰ���KHZProjTo1x
		if (!CalXJJ(KHZ, array, tarray, pRHA_BL->array, pRHA_BL->tarray, KHZProjTo1x, false))
			return false;

		if (jdix<JDarray.size()-1 && JDarray[jdix+1].JDXZ != RX_Start)
		{
			//������jdix+1��¼�Ĳ����߽�����������ֹ��Ӧ�Ĳ����߽���
			GetJdno1FromJdstr(JDarray[jdix+1].BLJDName, pRHA_BL->JDarray, TmpJDIndx);
			//TmpJDIndx = JDarray[jdix+1].BLJDName;
			KHZ1x=pRHA_BL->QXB[TmpJDIndx-1].KHZ;//�������ϣ���ֱ��ǰһ�������HZ��ͶӰ���	
			KHZ1x=max(KHZProjTo1x, KHZ1x);

			KZH1x= pRHA_BL->QXB[TmpJDIndx].KZH;//�������ϣ���ֱ�ߺ�һ�������ZH��ͶӰ���
		}
		else
		{
			//������jdix��¼�Ĳ����߽���
			int iBLJD;
			GetJdno1FromJdstr(JDarray[jdix].BLJDName, pRHA_BL->JDarray, iBLJD);
			KHZ1x=pRHA_BL->QXB[iBLJD].KHZ;
			KHZ1x=max(KHZProjTo1x, KHZ1x);

			KZH1x= pRHA_BL->QXB[iBLJD+1].KZH;
		}

		//2.3 ��λ���ױ�
		projml1x=pRHA_BL->LocatHmOnLine(KHZ1x, KZH1x, false, ckml);
		
	}
	else
		return false;

	if (projml1x<0)
		return false;
	

	//�������ɶ���
	Int16 LorR;//����
	double dml;//CKZH��CKHZ�����ֵ��ȥ�ںţ�
	ACHAR GH[16];//CKZH��CKHZ�Ĺں�
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
		//����ʼ
		if (xjj>0)
			LorR=1;
		else
			LorR=-1;
		
		dl.BLC = dml;
		dl.ELC = dml;
		dl.TYLC = projml2x;
		_tcscpy(dl.BGH, GH);
		_tcscpy(dl.EGH, CreatRXD_SPtEGH(blName, LorR, GH));
		_stprintf(dl.RXDName, L"���ж�%0.1lf", dl.ELC/1000.0);

	}
	else if ((JDarray[jdix].JDXZ == RX_End) 
		     || (JDarray[jdix].JDXZ == RX_Start && dirctn == false)) 
	{
		//����ֹ
		dl.BLC = dl.ELC+projml2x-dl.TYLC;//dl.TYLC Ϊ����ʼ��ͶӰ��� ls ����δ�������ж��м���� 
		dl.ELC = dml;
		dl.TYLC = projml2x;
		_tcscpy(dl.BGH, dl.EGH);//����δ�������ж��м��������ĹںŸı�
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
������:GenBXDL   
------------------------------------------------------------
��������:���ɲ��ж�ǰ����ղ�ͬ��·�������Ķ���
------------------------------------------------------------
����:
jdix         �����±�
dl           �����������
------------------------------------------------------------
���:  ���ɶ�������true������false
------------------------------------------------------------
�㷨:  ������ʼ���ڲ����ߵļ�ֱ����
------------------------------------------------------------
��ע��1 Ҫ��������ʼ���Ӧ�Ĳ����߽����±겻��Ҫ��֤��ȷ����ǰ�󽻵�Ĳ����߽�����Ƴ���
2 Ҫ����Ľ��������У�ÿ�����㶼��¼�����һ�������γɵ�ֱ�ߣ���Ӧ����ֱ�߱ߵ�ǰ
һ��������±�
3 ���жζ���ʱ��jdixΪ��������ղ�ͬ��·����ĺ�һ�����±�
4 �Զ����ɶ���ʱ�������ɲ��жζ������������ж��������ڼ�齻�������Ƿ�Ӧ��Ϊ����
------------------------------------------------------------
��д: ��˧ 2016.6.22 �޸� 2017.6.25
=====================================================================*/
bool RHA::GenBXDL(int jdix, DLTab &dl)
{
	ACHAR ckml[50];
	ACHAR blName[50];//��������,ǰһ���߲�����
	RHA *pRHA_BL=NULL;
	RHA *pRHA_PreBL=NULL;//ǰһ���߲�����
	RHA *pRHA_NexBL=NULL;//��һ���߲�����
	double projml1x, projml2x;//�ڲ������ϵ�ͶӰ���
	bool isPrevBLPrior=true;//=true ǰһ���������ȣ��磺ǰ������߷ֱ��Ǽ��л��ߡ���ƻ��ߣ���=false ��������

	if(jdix>0 && JDarray[jdix].JDXZ == BXPX && JDarray[jdix-1].JDXZ == BXPX)//818 BX_NOTPX???����
	{
		/*���жηֽ��*/
		_tcscpy(blName, JDarray[jdix].BLName);

		//��ȡǰ�������ָ��
		pRHA_PreBL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[jdix-1].BLName);

		pRHA_NexBL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[jdix].BLName);

		//3.1 �ҳ�����·��ǰ�����������ߵļ�ֱ���ڸ���·ͶӰ�ص����䡾sml, eml��
		int prevjdix, nexjdix;
		double sml=0, eml=0, sml1=0, eml1=0, sml2=0, eml2=0;

		sml=QXB[jdix-1].KHZ;
		eml=QXB[jdix].KZH;

		//Ѱ�Ҹ���·ǰ�󽻵��Ӧ�Ĳ����߽����±�
		if (!(GetJdno1FromJdstr(JDarray[jdix-1].BLJDName, pRHA_PreBL->JDarray, prevjdix)
			&& GetJdno1FromJdstr(JDarray[jdix].BLJDName, pRHA_NexBL->JDarray, nexjdix)))
			return false;


		//ǰ�����ߣ���������жζ���
		if (pRHA_PreBL->JDarray[prevjdix].JDXZ == RX_End || (pRHA_PreBL->JDarray[prevjdix].JDXZ == RX_Start 
			&& pRHA_PreBL->JDarray[prevjdix+1].JDXZ != RX_One && pRHA_PreBL->JDarray[prevjdix+1].JDXZ != RX_End))
		{//ǰһ�����߽���������ֹ���ߵ�������

			//Ѱ������ֹ����
			int edl=FindNearDL(pRHA_PreBL->QXB[ prevjdix ].KHZ, pRHA_PreBL->DLarray, false);

			if (edl>0 && pRHA_PreBL->DLarray[edl].Indx >= 0
				&& ((edl+1 <  pRHA_PreBL->DLarray.size() 
				&& pRHA_PreBL->DLarray[edl].Indx != pRHA_PreBL->DLarray[edl+1].Indx)
				||(edl+1 == pRHA_PreBL->DLarray.size())))
			{//������ֹ����

				//ȡǰһ������ǰһ�����HZ�㵽����ֹ�����������Χ
				//������ͶӰ�ض��ߣ��õ������Ϸ�Χ(sml1, eml1)
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
			//û�����жζ�����ȡǰһ�����ߵļ�ֱ�߶η�Χ������ͶӰ�ض��ߣ��õ������Ϸ�Χ(sml1, eml1)
			return false;

		//������ߣ���������жζ���
		if (pRHA_NexBL->JDarray[nexjdix].JDXZ == RX_Start)
		{//��һ���ս���������ʼ

			//Ѱ������ʼ����
			int sdl=FindNearDL(pRHA_NexBL->QXB[ nexjdix ].KZH, pRHA_NexBL->DLarray, true);

			if (pRHA_NexBL->DLarray[sdl].Indx >= 0
				&& ((sdl>0 && pRHA_NexBL->DLarray[sdl].Indx != pRHA_NexBL->DLarray[sdl-1].Indx)
				||(sdl == 0)))
			{//������ʼ����

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
			, pRHA_NexBL->tarray, array, tarray, eml2)))//û�����жζ���
			return false;

		sml=max(sml, sml1);
		sml=max(sml, sml2);
		eml=min(eml, eml1);
		eml=min(eml, eml2);

		//�Ƚ����������ߵ����ȼ�
		isPrevBLPrior=CCreatPM::CmpXLPriority(JDarray[jdix-1].BLName, JDarray[jdix].BLName);
		if (sml+0.1<eml)
		{
			//double projml=0.0;
			if(isPrevBLPrior)
			{
				//ǰ����������
				CalXJJ(sml, array, tarray, pRHA_PreBL->array, pRHA_PreBL->tarray, sml1);
				CalXJJ(eml, array, tarray, pRHA_PreBL->array, pRHA_PreBL->tarray, eml1);
				projml1x=pRHA_PreBL->LocatHmOnLine(sml1, eml1, true, ckml);
				//blix=prevblix;
				pRHA_BL=pRHA_PreBL;
			}
			else
			{
				//�����������
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
			mes.Format(L"%s�е�%s������ӦΪ����ʼ�������ǲ��У�", m_XLName, JDarray[jdix].JDName);
			//����û���ص������޸Ľ������ԣ�����������һ������Ϊ�������У�����ʼ��
			//818 ���������⣬ͶӰ�������ϻ��߽�����Ϣ��ֵ����
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
				_tcscpy(JDarray[jdix-1].BLJDName, L"");//����ʼ�Ĳ����߽��������ǰ��������				
			}

			return false;
		}

	}


	//�������ɶ���
	Int16 LorR;//����
	double dml;//CKZH��CKHZ�����ֵ��ȥ�ںţ�
	ACHAR GH[16];//CKZH��CKHZ�Ĺں�
	double xjj=0;
	double resPt[2];

	dl.Indx=-2;
	_tcscpy(dl.XLName, m_XLName);
	_tcscpy(dl.BLName, blName);

	CalXJJ(projml1x, pRHA_BL->array, pRHA_BL->tarray
		, array, tarray, xjj, projml2x, resPt, false);

	split_ckml(ckml, dml, GH);

	 //818 Ҫ���жζ�������������ͶӰ�������жΣ����ܻ������⡣
	//����
	Int16 prevdlix=0, nexdlix=0;
	if (isPrevBLPrior)//ǰһ����������
	{
		dl.BLC = dml;
		_tcscpy(dl.BGH, GH);

		CalXJJ(projml2x, array, tarray,  pRHA_NexBL->array, pRHA_NexBL->tarray, projml1x);

		nexdlix=pRHA_NexBL->JudgePtIsOnDL(projml1x);//�ж��Ƿ��ڶ�����
		if (nexdlix >= 0)
		{
			dl.ELC = pRHA_NexBL->DLarray[nexdlix].ELC;//�ڶ����ϣ�ȡ������Ϻ�
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
	else//��һ����������
	{
		dl.ELC = dml;
		_tcscpy(dl.EGH, GH);

		CalXJJ(projml2x, array, tarray, pRHA_PreBL->array, pRHA_PreBL->tarray, projml1x);

		prevdlix=pRHA_PreBL->JudgePtIsOnDL(projml1x);
		if (prevdlix >= 0)//�ڶ����ϣ�ȡ�������ǰ
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
������: IsOnDL   
��������: �ж�ͶӰ����Ƿ�λ�ڶ������ϡ�
����: Projml     ͶӰ���
���: �ڶ����ϣ����ض����±ꣻ���ڣ�����-1��
��ע: ��Ҫ�ж�����ͶӰ���
��д: ��˧ 2016.6.18 
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
��������: ��������ͶӰ������ɾֲ���������
����:
sProjml     �ֲ������ͶӰ���
eProjml     �ֲ����յ�ͶӰ���
���: 
dlAry            �ֲ���������
��ע: ��Ҫ�ж�����ͶӰ���
��д: ��˧ 2016.6.18 
=====================================================================*/
void RHA::CreateLocalDLarray(double sProjml, double eProjml, vector<DLTab> &dlAry) const
{
	//�������ж����ռ������
	DLTab tmpDL;
	int iDL=-1;

	_tcscpy(tmpDL.XLName, m_XLName);
	_tcscpy(tmpDL.RXDName, L"");
	_tcscpy(tmpDL.BLName, L"");
	tmpDL.Indx=0;

	dlAry=DLarray;

	//ɸѡ��������̷�Χ�ڵĶ���
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

	//����������
	iDL=JudgePtIsOnDL(sProjml);
	if(iDL >= 0)
		dlAry.insert(dlAry.begin(), DLarray[iDL]);
	else
	{
		TrsProjmltoCkml(sProjml, 3, tmpDL.BGH, tmpDL.BLC);
		tmpDL.ELC=tmpDL.BLC;
		_tcscpy(tmpDL.EGH, tmpDL.BGH);
		tmpDL.TYLC=sProjml;

		//��źͲ�����
		iDL=GetPreDL(tmpDL.TYLC);
		if (iDL>=0)
		{
			tmpDL.Indx=DLarray[iDL].Indx;
			_tcscpy(tmpDL.BLName, DLarray[iDL].BLName);
		}
		dlAry.insert(dlAry.begin(), tmpDL);
	}

	//�����յ����
	iDL=JudgePtIsOnDL(eProjml);
	if(iDL >= 0)
		dlAry.push_back(DLarray[iDL]);
	else
	{
		TrsProjmltoCkml(eProjml, 3, tmpDL.BGH, tmpDL.BLC);

		tmpDL.ELC=tmpDL.BLC;
		_tcscpy(tmpDL.EGH, tmpDL.BGH);
		tmpDL.TYLC=eProjml;
		//��źͲ�����
		iDL=GetPreDL(tmpDL.TYLC);
		if (iDL>=0)
		{
			tmpDL.Indx=DLarray[iDL].Indx;
			_tcscpy(tmpDL.BLName, DLarray[iDL].BLName);
		}
		dlAry.push_back(tmpDL);
	}

	//ɾ���ظ�����
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
������:CreatRXDSPtEGH   
------------------------------------------------------------
��������:�������ж����Ϻ�ں�
------------------------------------------------------------
����:
blName            ������·��
LorR              ������һ�ߵ����Ҳ�
GH1x              �����ߵĹں�
------------------------------------------------------------
���: ���ж��������Ϻ�ں�GH
------------------------------------------------------------
�㷨:  
------------------------------------------------------------
��д: ��˧ 2016.4.15 
=====================================================================*/
ACHAR* RHA::CreatRXD_SPtEGH(CString blName, int LorR, ACHAR *GH1x)
{
	ACHAR GH[16];

	if (m_XLName == L"��ƻ���")
	{
		_tcscpy(GH, L"A");
		_tcscat(GH, GH1x);
		return &GH[0];
	}

	//δ�����غϵ����
	if(LorR == -1)
		_tcscpy(GH, L"Z");
	else
		_tcscpy(GH, L"Y");


	if (m_XLName == L"���ж���")
	{
		if (GetJYXNum(m_mdbName) > 2)
			_tcscat(GH, L"1");
	}
	else if (m_XLName == L"��������")
		_tcscat(GH, L"2");
	else if (m_XLName == L"��������")
		_tcscat(GH, L"3");
	else if (m_XLName == L"��ƶ���")
	{
		if (GetSJXNum(m_mdbName) > 2)
			_tcscat(GH, L"1");
	}
	else if (m_XLName == L"�������")
		_tcscat(GH, L"2");
	else if (m_XLName == L"�������")
		_tcscat(GH, L"3");
	else
		return L"";

	if (_tcsstr(GH1x, L"A")==NULL && m_XLName.Find(L"���")>=0)//������ڼ����߹ں�ǰ�� A
		_tcscat(GH, L"A");

	_tcscat(GH, GH1x);

	return &GH[0];
}

/*===================================================================
������:GenDL   
------------------------------------------------------------
��������:�Զ����ɶ���
------------------------------------------------------------
���:  �ҵ����ս��㷵��true�����򷵻�false
------------------------------------------------------------
�㷨:  
------------------------------------------------------------
��д: ��˧ 2016.4.19  
=====================================================================*/
void RHA::GenDL()
{
	ACHAR CKml[50];
	ACHAR GH[16], GH2[16];
	double dml;
	double projml1x;
	int blix;
	RHA *pRHA_BL=NULL;

	DLarray.clear();//818 LS δ���Ƕ����ı���
	int NJD=JDarray.size();	
	//һ ���߶���
	if(JDarray[0].JDXZ == RX_Start
		&& JDarray[NJD-1].JDXZ == RX_End
		&& GetRXDNum(JDarray) == 1)/*JDarray[0].JDXZ == BLJD*/
	{
		setDLM(0, JDarray[0].l1, JDarray[0].l1, L"K", L"K", 0.0, 0);
		setDLM(1, QXB[NJD-1].KZH, QXB[NJD-1].KZH, L"K", L"K", QXB[NJD-1].KZH, 0);
		return;
	}

	//�� ���жζ���(GenRXBX_DLʱ���ܻ�ı佻������)
	//ǰ�󽻵㶼���в��ղ�ͬ����·ʱ���ڲ��ж���Ӷ�����
	DLTab tmpDL;
	vector<DLTab> tmpBXDLarry;//���жζ�������
	tmpBXDLarry.clear();

	for (int i=1; i<NJD; i++)
	{
		if(JDarray[i-1].JDXZ == BXPX && JDarray[i].JDXZ == BXPX 
			&& _tcscmp(JDarray[i-1].BLName, JDarray[i].BLName) != 0)
		{
			if (!GenBXDL(i, tmpDL))
			{
				//nan
				ads_printf(L"��ʾ�����жβ�����������\n");
				continue;
			}

			tmpBXDLarry.push_back(tmpDL);
		}
	}

	//�� ��㲢�ж���
	if(JDarray[0].JDXZ == BXPX || JDarray[0].JDXZ == BX_NotPX)//��㲢��
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
			ads_printf(L"�����Զ���������ʱ������δ�ҵ�\n");

	}
	

	//�� ���жζ���
	int RXDNum=GetRXDNum(JDarray);
	int SJDIndx, EJDIndx;
	DLTab tmpSDL, tmpEDL;
	for (int i=0; i<RXDNum; i++)//���������յ����е����
	{
		//��ȡ���жε����ս����±�
		GetRxdSEJD(i, SJDIndx, EJDIndx, JDarray);//818 û���ǵ������У������������յ㣩�������
		
		if(!GenRXDL(i, SJDIndx, tmpSDL, true))//����ʼ����
		{
			RXDNum--;
			ads_printf(L"��ʾ���������ж�����������\n");//nan
			continue;
		}

		tmpEDL=tmpSDL;
		if (!GenRXDL(i, EJDIndx, tmpEDL, false))//����ֹ����
		{
			RXDNum--;
			ads_printf(L"��ʾ���������ж��յ��������\n");//nan
			continue;
		}
		
		DLarray.push_back(tmpSDL);
		DLarray.push_back(tmpEDL);
	}
	
	//�� �յ㲢�ж���
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
			ads_printf(L"�����Զ���������ʱ������δ�ҵ�\n");
		//AddDL(dml, dml,  GH, GH2, QXB[NJD-1].KZH, -1, 0,JDarray[NJD-1].BLName);
	}

	//�� ���벢�жζ���
	int dlix;
	for (int i=0; i<tmpBXDLarry.size(); i++)
	{
		dlix=FindNearDL(tmpBXDLarry[i].TYLC, DLarray, false);
		if (dlix>-1)
			DLarray.insert(DLarray.begin()+dlix, tmpBXDLarry[i]);
	}

}

//���ܣ��Զ�����ĳ�����жε����ն���
//���룺���ж��±�
//������������ն���
//����ֵ���������������ȷ������true
//��д��ls 17.8.26
bool RHA::GeniRxdDL(int indx, DLTab rxdDL[2])
{
	_tcscpy(rxdDL[0].XLName, m_XLName);
	_tcscpy(rxdDL[1].XLName, m_XLName);

	//��ȡ���жε����ս����±�
	int SJDIndx, EJDIndx;
	if (!GetRxdSEJD(indx, SJDIndx, EJDIndx, JDarray))
		return false;
	
	if(!GenRXDL(indx, SJDIndx, rxdDL[0], true))//����ʼ����
	{
		ads_printf(L"��ʾ���������ж�����������\n");
		return false;
	}

	rxdDL[1]=rxdDL[0];
	if (!GenRXDL(indx, EJDIndx, rxdDL[1], false))//����ֹ����
	{
		ads_printf(L"��ʾ���������ж��յ��������\n");
		return false;
	}

	return true;

}

//��ע���жε���̺Ͷ���
void RHA::MarkRXDLC(AcGiWorldDraw* pWd)
{
	int RxdNum=GetRXDNum(JDarray);//���ж���Ŀ

	//��ע���ж�
	int sdl=0,edl=0;
	for(int i=0;i<RxdNum;i++)
	{
		//��ȡ���ж����ն�����
		GetiRXDDLinfo(i,sdl,edl,DLarray);
		if( sdl==-1 || edl==-1 ) 
			return ;

		//��ע�������жε���̡�����
		MarkiRXDLC(pWd,DrawParam.m_dLC,1,sdl,edl);
	}
	

}

//��ע���жε���̺Ͷ���
void RHA::MarkRXDLC() const
{
	int RxdNum=GetRXDNum();//���ж���Ŀ

	//��ע���ж�
	int sdl=0,edl=0;
	Int16 zy=1;

	zy=DrawParam.OtherParam.MarkAry[2].LorR;
	for(int i=0;i<RxdNum;i++)
	{
		//��ȡ���ж����ն�����
		GetiRXDDLinfo(i,sdl,edl,DLarray);
		if( sdl==-1 || edl==-1 ) 
			return ;

		//��ע�������жε���̡�����
		MarkiRXDLC(DrawParam.m_dLC,/*1*/zy,sdl,edl);
	}

	//������յ���ձ����·�����ע���յ����
	if (DLarray.size()>0)
	{
		if(DLarray[0].Indx<0)
			MarkSglDL(0, zy);
		if (DLarray[DLarray.size()-1].Indx<0)
			MarkSglDL(DLarray.size()-1, zy);
	}
	


}



//  ��ע�������жΣ����������̱꣩  
//zy=1,-1  �����ұߣ���� //���߶�����//������
void RHA::MarkiRXDLC(AcGiWorldDraw* pWd, double dLC, int zy, int sdl, int edl/*DLTab *iDLB,int ndl*/ )  
{
	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,lc,b,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[100],chr[100],ch[100];
	ACHAR ckml[50];
	xlpoint  PZtmp;
	double cml;//�������
	double BZlc, DDlength;//��ע�����ֵdml���������������̣���ͶӰ��̣�

	_tcscpy(GH, DLarray[sdl].EGH);
	WB=1.5*texth;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// ������ͳһ���
	_tcscpy(ckml, LCchr(DLarray[sdl].EGH, DLarray[sdl].ELC, DrawParam.NLC));
	TrsCkmltoCml(ckml,DDlength);

	DrawParam.NLC=3;//��ʱ 818 ls		

	for (iDL=sdl;iDL<edl;iDL++)//���һ������Ϊ�յ�
	{
		//��ע���ж��м����
		if(iDL>sdl)
			MarkSglDL(pWd, iDL, zy);

		DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���

		//���濪ʼ��ע���
		zlc=(long int)(DLarray[iDL].ELC/dLC);
		BZlc=zlc*dLC;  // ʵ����� 
		if((BZlc-DLarray[iDL].ELC)<0.0001)
			BZlc=BZlc+dLC;
		if ( BZlc>DLarray[iDL+1].BLC+0.0001)
		{
			DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//������ͳһ��̡�
			continue;
		}

		_tcscpy(GH,DLarray[iDL].EGH);  //�ں�
		do   		
		{
			// �������
			cml=DDlength+BZlc-DLarray[iDL].ELC; 
			// ͶӰ���
			//PZtmp.lc=CalcmlTonewcml(cml);
			//PZtmp.lc=TrsCmltoProjml(cml);//818 ̫����
			if(XLShuXing == SJJX || XLShuXing == SJ2X )//�����
				PZtmp.lc=BZlc-DLarray[iDL].ELC+DLarray[iDL].TYLC;
			else//�����ߡ���ʱ�ߡ�ʩ������
				PZtmp.lc=CalcmlTonewcml(cml);

			k=xlpoint_pz(array, tarray, PZtmp);
			
			BZW=k;    //  K<0  �޽�
			if (k>0 && k<=6)  //�н�//��Ԫ����
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
					if (ii>3&&chr[ii-3]=='0') // �����
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH);
						_tcscat(ch,chr);
					}
					else if(ii>3 && chr[ii-3]!='0')    // ���ױ�
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
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // �����
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

		DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//������ͳһ��̡�

	}

	//��ע���ж����
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
	_tcscpy(ch,LCchr(GH1,DLarray[sdl].BLC,DrawParam.NLC));  // ��ǰ���
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);
	G_maketext(pWd,p1,chr,b+pi,texth,QXYSColor,1);  

	ACHAR RXFlag[80];
	//plh04.11.26
	_stprintf(RXFlag,L"%s��� ",DLarray[sdl].RXDName);


	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH,DLarray[sdl].ELC,DrawParam.NLC));     // �Ϻ����
	_tcscpy(chr,L" = ");
	_tcscat(chr,RXFlag);
	_tcscat(chr,ch);   // e.f chr=L"ZH K10+123.45"   

	DL=DLarray[sdl].ELC-DLarray[sdl].BLC;
	if (fabs(DL)>0.001)
	{ 
		if(DL>0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos( DL,2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else if( DL<0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else
		{/*����ֵ����Ϊ0*/}
	}
	G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);


	//��ע���ж��յ�
	iDL=edl;
	RxdNo=DLarray[edl].Indx;
	DL=DLarray[edl].ELC-DLarray[edl].BLC;//����ֵ=�Ϻ����-��ǰ���
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
	_stprintf(RXFlag,L"%s�յ� ",DLarray[edl].RXDName);


	_tcscpy(ch,RXFlag);
	_tcscat(chr,L" ");
	_tcscat(ch,LCchr(GH1,DLarray[edl].BLC,DrawParam.NLC));  // ��ǰ���
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);  
	G_maketext(pWd,p1,chr,b+pi,texth,QXYSColor,1);  

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH,DLarray[edl].ELC,DrawParam.NLC));     // �Ϻ����
	_tcscpy(chr,L" = ");
	_tcscat(chr,ch);// e.f chr=L"ZH K10+123.45"   


	if (fabs( DL)>0.001)
	{ 
		if(DL>0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos( DL,2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else if( DL<0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else
		{/*����ֵ����Ϊ0*/}
	}


	G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);
}





//  ��ע�������жΣ����������̱꣩  
//zy=1,-1  �����ұߣ���� //���߶�����//������
void RHA::MarkiRXDLC(double dLC,int zy, int sdl, int edl )  const
{
	double offset = 0.0;//��Ϊ0 818
	ACHAR layname[256];

	_tcscpy(layname, DrawParam.LiCheng);

	/*if (m_XLName == L"���л���")
		_stprintf(layname,L"%s",DrawParam.LiCheng);*/
	/*else if (m_XLName == L"ʩ������")
		_stprintf(layname,L"%s",BXLiCheng);
	else if (m_XLName == L"��ͨ��ƻ���")
		_stprintf(layname,L"%s",GJLiCheng);
	else if (m_XLName == L"��ͨ��Ƣ���")
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
	double cml;//�������
	double BZlc, DDlength;//��ע�����ֵdml���������������̣���ͶӰ��̣�
	double dist=0.0;

	double textH=DrawParam.OtherParam.MarkAry[2].TextHeight;

	_tcscpy(GH, DLarray[sdl].EGH);
	WB=1.5*textH;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// ������ͳһ���
	//LCchr(DLarray[sdl].EGH, DLarray[sdl].ELC, NLC, ckml);
	_tcscpy(ckml, LCchr(DLarray[sdl].EGH, DLarray[sdl].ELC, DrawParam.NLC));
	TrsCkmltoCml(ckml,DDlength);


	for (iDL=sdl;iDL<edl;iDL++)//���һ������Ϊ�յ�
	{
		//��ע���жζ���(��������ʼ)
		if (fabs(DLarray[iDL].ELC-DLarray[iDL].BLC)+0.001
				>DrawParam.m_NoteMinDLLen)
			MarkSglDL(iDL, zy);

		DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���

		//���濪ʼ��ע���
		zlc=(long int)(DLarray[iDL].ELC/dLC);
		BZlc=zlc*dLC;  // ʵ����� 
		if((BZlc-DLarray[iDL].ELC)<0.0001)
			BZlc=BZlc+dLC;
		if ( BZlc>DLarray[iDL+1].BLC+0.0001)
		{
			DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//������ͳһ��̡�
			continue;
		}

		_tcscpy(GH,DLarray[iDL].EGH);  //�ں�
		do   		
		{
			// �������
			cml=DDlength+BZlc-DLarray[iDL].ELC; 
			// ͶӰ���
			//PZtmp.lc=CalcmlTonewcml(cml); 
			PZtmp.lc=TrsCmltoProjml(cml);  
			k=xlpoint_pz(array, tarray, PZtmp);

			BZW=k;    //  K<0  �޽�
			if (k>0 && k<=6)  //�н�//��Ԫ����
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
					if (ii>3&&chr[ii-3]=='0') // �����
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH);
						_tcscat(ch,chr);
					}
					else if(ii>3 && chr[ii-3]!='0')    // ���ױ�
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
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // �����
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

				dist=2*WB+0.4*_tcslen(ch)*textH;//�ַ������ĵ���·���ĵľ���

				pte[X]=PZtmp.y+(dist+offset)*sin(B+pi*0.5*zy)/*+0.5*textH*sin(B)*/;
				pte[Y]=PZtmp.x+(dist+offset)*cos(B+pi*0.5*zy)/*+0.5*textH*cos(B)*/;
				

				//G_maketext(pWd,pte,ch,b,textH,QXYSColor,1);
				maketext(pte,ch,b,textH,-1,3,L"����",layname);     

			};          
			BZlc=BZlc+dLC;
			if ( BZlc>DLarray[iDL+1].BLC) BZW=-99;		 
		}while (BZW>0);

		DDlength=  DDlength+DLarray[iDL+1].BLC-DLarray[iDL].ELC;//������ͳһ��̡�

	}

	////��ע����ֹ����
	//MarkSglDL(edl, zy);


	//��ע���ж����
	//ACHAR RXFlag[80];

	//TrsProjmltoNE(DLarray[sdl].TYLC,PZtmp);
	//_stprintf(RXFlag,L"%s��� ",DLarray[sdl].RXDName);
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

	//maketext(pt,chr,b+pi,textH,-1,3,L"����",layname);


	////��ע���ж��յ�
	////��ע����ֹ����
	MarkSglDL(edl, zy);
	////��ע����ֹ
	//TrsProjmltoNE(DLarray[edl].TYLC,PZtmp);
	//_stprintf(RXFlag,L"%s�յ� ",DLarray[edl].RXDName);
	//_tcscpy(ch,RXFlag);
	//_tcscat(chr,L" ");
	//_tcscat(ch,LCchr(DLarray[edl].EGH,DLarray[edl].ELC,DrawParam.NLC));  // ��ǰ���
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
	//maketext(pt,chr,b+pi,textH,-1,3,L"����",layname);

	//_tcscpy(ch,L"\0");
	//_tcscpy(ch,LCchr(GH,DLarray[edl].ELC,NLC));     // �Ϻ����
	//_tcscpy(chr,L" = ");
	//_tcscat(chr,ch);// e.f chr=L"ZH K10+123.45"   


	//if (fabs( DL)>0.001)
	//{ 
	//	if(DL>0.0)
	//	{
	//		_tcscat(chr, L" ���� "); 
	//		ads_rtos( DL,2,NLC,ch);
	//		_tcscat(chr, ch); 
	//		_tcscat(chr, L" m");
	//	}
	//	else if( DL<0.0)
	//	{
	//		_tcscat(chr, L" ���� "); 
	//		ads_rtos(fabs(DL),2,NLC,ch);
	//		_tcscat(chr, ch); 
	//		_tcscat(chr, L" m");
	//	}
	//	else
	//	{/*����ֵ����Ϊ0*/}
	//}


	////G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);
	//maketext(pte,chr,b+pi,texth,-1,3,L"����",layname);

	



}




/*===================================================================
������:MarkSglDL
��������:��ע��������
����: pWd        ���� 
      iDL        �����±�
	  zy         =1,�����ұ�;=-1�����
	  type       <=1,��ʱ����ת90��
��д: ��˧ 2016.6.20
��ע��
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

	DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���
	_tcscpy(GH, DLarray[iDL].EGH);
	_tcscpy(GH1, DLarray[iDL].BGH);

	// �������
	TrsCkmltoProjml(ckml, PZtmp.lc);//ͶӰ��� 818 ��ƶ��� idl==6 ��ʱ�㲻��
	k=xlpoint_pz(array, tarray, PZtmp); 

	if (k>0 && k<=6)  //�н�
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
		_tcscpy(ch,LCchr(GH1,DLarray[iDL].BLC,DrawParam.NLC));  // ��ǰ���
		_tcscpy(chr,L"  ");
		_tcscat(chr,ch);  
		b+=pi;
		G_maketext(pWd,p1,chr,b,texth,QXYSColor,1);  

		_tcscpy(ch,L"\0");
		_tcscpy(ch,LCchr(GH,DLarray[iDL].ELC,DrawParam.NLC));     // �Ϻ����
		_tcscpy(chr,L" =");
		_tcscat(chr,ch);   // e.f chr=L"ZH K10+123.45"   

		if (fabs( DL)>0.001)
		{ 
			if(DL>0.0)
			{
				_tcscat(chr, L" ���� "); 
				ads_rtos( DL,2,DrawParam.NLC,ch);
				_tcscat(chr, ch); 
				_tcscat(chr, L" m");
			}
			else if( DL<0.0)
			{
				_tcscat(chr, L" ���� "); 
				ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
				_tcscat(chr, ch); 
				_tcscat(chr, L" m");
			}
			else
			{
				/*����ֵ����Ϊ0*/
			}
		}
		b+=pi;
		G_maketext(pWd,pte,chr,b+pi,texth,QXYSColor,1);		   
	};

}



/*===================================================================
������:MarkSglDL
��������:��ע��������
����: 
iDL        �����±�
zy         =1,�����ұ�;=-1�����
type       <=1,��ʱ����ת90��
��д: ��˧ 2016.6.20
��ע��
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
	ACHAR ckml[MaxCkmlStrNum], hmCkml[MaxCkmlStrNum];//�ö������ֳ���̣��ö���ǰһ�����ױ���ֳ����
	ACHAR layname[256];
	xlpoint  PZ, hmPZ, tmpPZ;//�ö����㣬����ǰһ�������׵�
	AcGePoint3d LT,LB,RT,RB;//������Ķ����Ķ���
	double FrameH; 
	double len;//�������š����ͱ�ע�ĸ߶�
	double offset=0.0;//818
	int iRxd=-1;
	double txtHgt;
	bool isAllLineRX;
	Int16 horizonMode;//���뷽ʽ�� =-1 �Ҷ��� =1 �����

	isAllLineRX=IsAllLineRX();
	txtHgt=DrawParam.OtherParam.MarkAry[2].TextHeight;
	_tcscpy(layname, DrawParam.LiCheng);
	horizonMode=zy*DrawParam.m_TextDir;

	//_tcscpy(ckml, DLarray[iDL].EGH);
	//_stprintf(ch, L"%lf",DLarray[iDL].ELC);
	_tcscpy(ckml, LCchr(DLarray[iDL].EGH, DLarray[iDL].ELC, 3));

	DL=DLarray[iDL].ELC-DLarray[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���
	_tcscpy(GH, DLarray[iDL].EGH);
	_tcscpy(GH1, DLarray[iDL].BGH);

	// �������
	TrsCkmltoProjml(ckml, PZ.lc);//ͶӰ���
	k=xlpoint_pz(array, tarray, PZ); 

	if (k>0 && k<=6)  //�н�
	{
		_tcscpy(ch,L"\0");
		_tcscpy(ch,LCchr(GH1,DLarray[iDL].BLC,DrawParam.NLC));  // ��ǰ���

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
						_stprintf(chr1, L"  %0.1f���иĽ��յ� ", DLarray[k+1].BLC/1000.0);
						break;
					}
				}

				if (k == -1)
					ads_printf(L"��ƽ��ͼʱδ�ҵ�����ʼ������\n");//nan
			}
			else if (iDL == 0 
				|| (iDL>0 &&  DLarray[iDL-1].Indx != DLarray[iDL].Indx))
				_stprintf(chr1, L"  %0.1f���иĽ���� ", DLarray[iDL].BLC);
			
		}
	
		_tcscat(chr1,ch); 

		 // �Ϻ����
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
					_tcscat(chr2, L" ���� "); 
					ads_rtos( DL,2,DrawParam.NLC,ch);
					_tcscat(chr2, ch); 
					_tcscat(chr2, L" m");
				}
				else if( DL<0.0)
				{
					_tcscat(chr2, L" ���� "); 
					ads_rtos(fabs(DL),2,DrawParam.NLC,ch);
					_tcscat(chr2, ch); 
					_tcscat(chr2, L" m");
				}
				else
				{
					/*����ֵ����Ϊ0*/
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

		//���ǰ
		//if (horizonMode == -1)//�Ҷ���
		//{
		//�����
			pt[X]=PZ.y+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B+pi*0.5*zy) ;
			pt[Y]=PZ.x+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B+pi*0.5*zy) ;
		//}
		//else//�����
		//{
		//	pt[X]=pt[X]+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B-pi*0.5*zy) ;
		//	pt[Y]=pt[Y]+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B-pi*0.5*zy) ;
		//}
		ptb=pt;
		//������ͷ���򡢶��뷽ʽ��ʹ���ַ����ע�߼��Ϊ0.4���ָߣ�txtHgt��
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
			maketext(pt,chr1,b,txtHgt,-1,1,L"����",layname); //����� 
		else
			maketext(pt,chr1,b,txtHgt,-1,4,L"����",layname);//�Ҷ���

		//��Ϻ�
		//tmpPZ.lc=PZ.lc+1.2*txtHgt;////Lw2007-11-05
		//xlpoint_pz(array, tarray, &tmpPZ);
		//pt[X]=tmpPZ.y+(2*DrawParam.SCALE+1.0*txtHgt+offset)*sin(B+pi*0.5*zy) ;
		//pt[Y]=tmpPZ.x+(2*DrawParam.SCALE+1.0*txtHgt+offset)*cos(B+pi*0.5*zy) ;
		///*pt[X]=PZ.y+1.0*txtHgt*sin(tmpPZ.a) ;
		//pt[Y]=PZ.x+1.0*txtHgt*cos(tmpPZ.a) ;*/
		//pt[X]=PZ.y+0.75*txtHgt*sin(B+horizonMode*pi) ;
		//pt[Y]=PZ.x+0.75*txtHgt*cos(B+horizonMode*pi) ;

		if (horizonMode == 1)
			maketext(ptb,chr2,b,txtHgt,-1,1,L"����",layname);  
		else
			maketext(ptb,chr2,b,txtHgt,-1,4,L"����",layname);


		//ǰһ������,������������
		//�������ֵΪ0���������ն��������ǲ��ж���������Ҫ��ǰһ�����׻�������
		if(fabs(DL)<0.001 || iDL == 0 || DLarray[iDL].Indx<0 /*|| (DLarray[iDL].Indx >= 0 && (iDL == 0 
			|| (iDL>0 && DLarray[iDL-1].Indx != DLarray[iDL].Indx)))*/)
			return;

		

		//����õ�ȷʵ�������׾Ͳ���Ҫ��ǰһ�����ף���������
		double hmBLC;
		double remainder=fmod(DLarray[iDL].BLC, 100.0);//�������׵ľ��루������̣�
		if(fmod(DLarray[iDL].BLC, 100.0) == 0.0)
			return;

		double dlDist=DLarray[iDL].TYLC-DLarray[iDL-1].TYLC;
		remainder=remainder*dlDist/(DLarray[iDL].BLC-DLarray[iDL-1].ELC);//�������׵ľ��루ͶӰ��̣�
		if(remainder+0.01<dlDist)
		{
			hmBLC=floor(DLarray[iDL].BLC/100.0)*100;
			_tcscpy(hmCkml, LCchr(DLarray[iDL].BGH, hmBLC, 3));
			TrsCkmltoProjml(hmCkml, hmPZ.lc);
		}
		else
		{
			_tcscpy(hmCkml, LCchr(DLarray[iDL-1].BGH, DLarray[iDL-1].BLC, 3));
			hmPZ.lc=DLarray[iDL-1].TYLC;//ȡ�������ױ꣬��ȡǰһ����
		}

		iRxd=GetiRxdByProjml(hmPZ.lc);
		if(iRxd >= 0 && iRxd == DLarray[iDL].Indx)//��ͬһ���ж���
		{
			k=xlpoint_pz(array, tarray, hmPZ); 
			if (k>0 && k<=6)  //ǰһ�������н�
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
				
				//���ƶ����㵽�����׵�����·�����������
				makeline(LT,RT,-1,AcDb::kLnWtByLayer,layname);//���š�������ĺ���
				makeline(RB,LB,-1,AcDb::kLnWtByLayer,layname);//���š��ĵڶ�������

				//dCml=CalnewcmlTocml(PZ.lc)-CalnewcmlTocml(hmPZ.lc);//818 dCml -9*10^61 ��Խ��
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
				maketext(cenFrm,str,b,txtHgt,-1,3,L"����",layname);  
			}

		}

	
	};

}



//ע�⣺NDL�ڵ��øú���ǰ�����
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
		ads_printf(L"���жζ������ô���\n");
		return false;
	}
	
}

//ע�⣺NDL�ڵ��øú���ǰ�����
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
		ads_printf(L"���жζ������ô���\n");
		return false;
	}
}

/*===================================================================
������:IsAllLineRX
��������:�Ƿ�ȫ������
����:    ���ȫ�����з���true�����򷵻�false
��д: ��˧ 2016.8.27
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

//��ע������
Adesk::Boolean RHA::MarkTZD(AcGiWorldDraw* pWd, int zybz)  //  ��ע���
// zybz==0   ������ƫ���ע   zybz=1,-1  �����ұߣ����
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
	//��־���
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
			//�����
			Pt[Y]=PZtmp.x; 
			Pt[X]=PZtmp.y;
			Pt[Z]=0.0;

			//�����
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

			b=0.0;//��ת�Ƕ�
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

			//���յ�
			pte[X]=PZtmp.y+(_tcslen(str)*1.2*texth)*sin(B+pi*0.5*zybz);
			pte[Y]=PZtmp.x+(_tcslen(str)*1.2*texth)*cos(B+pi*0.5*zybz);

			G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);//��
			if(zybz>0)
				G_maketext(pWd,pte,str,b,1.5*texth,QXYSColor,1);//����
			else 
				G_maketext(pWd,ptb,str,b,1.5*texth,QXYSColor,1);

		}
	}

	return TRUE;
}


//��ע������
Adesk::Boolean RHA::MarkTZD() const  //  ��ע���
// zybz==0   ������ƫ���ע   zybz=1,-1  �����ұߣ����
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
	//zybz=0;//818 �����ڲࣿ 
	for(int i=1;i<JDarray.size()-1;i++)
	{
		//if (zybz==0)//�����ڲ�
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
			//�����
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

			

			//���յ�
			pte[X]=PZtmp.y+(_tcslen(str)*0.8*txtHgt)*sin(B+pi*0.5*zybz);
			pte[Y]=PZtmp.x+(_tcslen(str)*0.8*txtHgt)*cos(B+pi*0.5*zybz);

			//G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
			makeline2(ptb,pte,-1,AcDb::kLnWtByLayer, layerName);//��

			//���Ʒ�����
			if (isJYX && DrawParam.m_IsDrawFQD && !IsPtOnLYD(PZtmp.lc))
			{
				y = 0.5 * (ptb[Y] + pte[Y]);
				x = 0.5 * (ptb[X] + pte[X]);
				xyddaa(ptb[Y],ptb[X],pte[Y],pte[X],&fwj);
				DrawOneFQDBZ(y,x,fwj,0.2*_tcslen(str)*txtHgt,MAINLINE);
			}

			//if(zybz>0)
			//	G_maketext(pWd,pte,str,b,1.5*txtHgt,QXYSColor,1);//����
			//else 
			//	G_maketext(pWd,ptb,str,b,1.5*txtHgt,QXYSColor,1);

			//if(zybz>0)
			//	b=B+pi;
			//else 
			//	b=B;

			pt[X]=0.5*(ptb[X]+pte[X])+0.75*txtHgt*sin((textDir-1)/2.0*pi+B);
			pt[Y]=0.5*(ptb[Y]+pte[Y])+0.75*txtHgt*cos((textDir-1)/2.0*pi+B);

			if (DrawParam.m_TextDir == 1)//��ת�Ƕ�
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

			//����
			maketext(pt,str,b,txtHgt,-1,3,L"����",layerName);

		
		}
	}

	return TRUE;
}


//��ֱ�ߣ������߿�
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
			ads_printf(L"����%s��ǰһ���㽻�����ͬ��Ĭ�ϻᰴ�����ߴ���\n",JDnum1);
	}
}



/*===================================================================
������:SaveData   
------------------------------------------------------------
��������:����·�޸�ǰ�����潻��/����/��־�������
------------------------------------------------------------
���: 
------------------------------------------------------------
��д: ��˧ 2016.7.16 
=====================================================================*/
void RHA::SaveData()
{
	//������Ԫ
	getpxy_DATA();
	//����DL����
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
������:UpdateData   
------------------------------------------------------------
��������:���㽻����¶��߽���Ͷ�������
------------------------------------------------------------
����:
sjd            ��sjd��insrtBxJdNum����������ʶ�Ϊ���л��в�ƽ��
insrtBxJdNum   ����Ĳ��н�����( 818 ���Ƕ�����в�ƽ�жκ�S��)
			  ��ǰ��������ֻ�ڲ�����ʱ��ʱ�á�

OldJDarray     ԭʼ����
OldDLarray     ԭʼ����
oldBZLCarray   ԭʼ��־���
------------------------------------------------------------
���: 
JDarray        ����
DLarray        ����
BZLCarray      ��־���
------------------------------------------------------------
��ע: 
------------------------------------------------------------
��д: ��˧ 2016.7.6 
=====================================================================*/
void RHA::UpdateAllData(/*int sJD, int insrtBxJdNum*/)
{
	if (XLShuXing == JY2X || XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		//������߲��չ�ϵ���γɶ��߽������ݱ�
		CalExReference();
		
		//������������
		ModifyExJDXY();
	}
	else if(!IsMove)
	{//�����϶�����

		//ȫ�����и���������(�����϶�ʱ����Ҫ)
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
	
	//������Ԫ
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

	//�Զ����ɶ���
	GenDL();

	//GenBZLCbyDL(true);
	//CalBZLC();//818 �Ƿ���㣿

	//B ���Ǳ������
	CmpNewDLwithOld(); 
	if(XLShuXing != SJJX && XLShuXing != SJ2X)
		GenBZLCbyDL(false);
	else
		BZLCarray.clear();


	//if (XLShuXing == JYJX || XLShuXing == JY2X)
	//{
	//	//CmpNewBZLCwithOld();
	//	CalBZLC();//818 �Ƿ���㣿
	//}

}

/*===================================================================
������:    UpdateRxdDL   
��������:  ������λ�ı仯���¼������жζ����������ܱ���ԭ�ж������ֳ���̣�
����:
iRXD            ���жα�ţ�ȫ������Ϊ0��
���:         
�㷨:      
��ע: 
��д:      ��˧ 2017.6.20 
=====================================================================*/
void RHA::UpdateRxdDL(int iRXD)
{
	int jdNum=JDarray.size();
	if (jdNum<2)
	{
		ads_printf(L"���󣺽�����С��2\n");
		return;
	}

	ACHAR  rxdName[50];
	_tcscpy(rxdName, L"");

	if (XLShuXing == JYJX || XLShuXing == LSX || XLShuXing == SGBX
		|| (GetRXDNum() == 1 && JDarray[0].JDXZ == RX_Start && JDarray[jdNum-1].JDXZ == RX_End))
		//818 ����֮ǰҲ��ȫ�����е��ж���Ҫ��
	{//ȫ������
		int dlNum=0;
		double xlLen=0.0;
		double eptELC=0.0;
		ACHAR eptEGH[16];


		xlLen=QXB[jdNum-1].KHZ;//��·��
		dlNum=DLarray.size();

		if(dlNum<2)
			return;

		//�յ�Ϻ�
		_tcscpy(eptEGH, DLarray[dlNum-1].EGH);
		eptELC=DLarray[dlNum-1].ELC;


		if (DLarray[dlNum-1].TYLC+0.001>xlLen)
		{//��·���ȱ�С�������ļ�����̴�����·���ȵĶ�ɾ��
			for (int i=dlNum-2; i>0; i--)
			{
				if (DLarray[i].TYLC+0.001>xlLen)
					DLarray.erase(DLarray.begin()+i);
			}
		}
		dlNum=DLarray.size();


		//�޸��յ����
		_tcscpy(DLarray[dlNum-1].BGH, DLarray[dlNum-2].EGH);//��ǰ�ں�		
		DLarray[dlNum-1].TYLC = xlLen;//ͶӰ���
		DLarray[dlNum-1].BLC = DLarray[dlNum-2].ELC + DLarray[dlNum-1].TYLC - DLarray[dlNum-2].TYLC;//��ǰ���
		_tcscpy(DLarray[dlNum-1].EGH, eptEGH);//�Ϻ�ں�
		DLarray[dlNum-1].ELC=eptELC;//�Ϻ����

	}
	else if (iRXD >= 0)
	{//�������ж�
		int sjd, ejd;//����ʼ��ֹ�����±�
		int sdl, edl;
		vector<DLTab> rxdDLarray;


		//������� 818

		//�յ�����

		//ȷ�����ն����㣬���ԭ�е����յ�����Ƿ���ֱ����
		//����������������ö�����ע�⣺Ĭ�ϲ��ı�ǰ�������
		bool isUseOldSdl=false;
		bool isUseOldEdl=false;
		DLTab sptDL, eptDL;
		int dlNum=0;
		double oldRxdLen=0.0;
		double newRxdLen=0.0;
		double len=0.0;


		eptDL.TYLC=sptDL.TYLC=0.0;

		//������ʼ����
		GetRxdSEJD(iRXD, sjd, ejd, JDarray);

		//������ʼ����
		GetiRXDDLinfo(iRXD, sdl, edl, DLarray);

		oldRxdLen=DLarray[edl].TYLC-DLarray[sdl].TYLC;//ԭ�����жγ���

		//ȡ�����жζ���
		rxdDLarray.assign(DLarray.begin()+sdl, DLarray.begin()+edl+1);//818 +1? of course
		dlNum=rxdDLarray.size();

		isUseOldSdl=CheckRxdDL(iRXD, true);
		if (isUseOldSdl)
			sptDL=DLarray[sdl];
		else
		{
			//�Զ���������
			GenRXDL(iRXD, sjd, sptDL, true);
			
			////����֮ǰ�����ж����Ϻ�ں� 
			_tcscpy(sptDL.EGH, DLarray[sdl].EGH);
			//sptDL.ELC=DLarray[sdl].ELC;   818 �������ʱ��
		}


		//������ֹ����
		isUseOldEdl=CheckRxdDL(iRXD, false);
		if (isUseOldEdl)
			eptDL=DLarray[edl];
		else
			//�Զ���������
			GenRXDL(iRXD, ejd, eptDL, false);

		//ɾ���������жγ��ȵ����ж��м����
		//�������㱣���Ķ����ļ������
		newRxdLen=eptDL.TYLC-sptDL.TYLC;
		len=0.0;
		for(int i=1; i<dlNum-1; i++)
		{
			len += rxdDLarray[i].BLC-rxdDLarray[i-1].ELC;
			if(len+0.001>newRxdLen)
			{//�ö������������յ������ɾ��
				rxdDLarray.erase(rxdDLarray.begin()+i);
				i--;
				dlNum--;
			}
			else
				rxdDLarray[i].TYLC = sptDL.TYLC+len;
		}

		//�޸����յ����
		rxdDLarray[0]=sptDL;
		rxdDLarray[dlNum-1]=eptDL;

		//�޸��յ�����Ķ�ǰ��̺͹ں�
		_tcscpy(rxdDLarray[dlNum-1].BGH, rxdDLarray[dlNum-2].EGH);

		rxdDLarray[dlNum-1].BLC=rxdDLarray[dlNum-2].ELC+eptDL.TYLC-rxdDLarray[dlNum-2].TYLC;

		//ɾ��ԭ�������жζ���
		DLarray.erase(DLarray.begin()+sdl, DLarray.begin()+edl+1);

		//�����µĶ���
		_stprintf(rxdName, L"%0.1lf���ж�", rxdDLarray[0].ELC/1000.0);

		for (int k=dlNum-1;k>-1;k--)
		{
			rxdDLarray[k].Indx=iRXD;//���жα��
			_tcscpy(rxdDLarray[k].RXDName, rxdName);//���ж���
			DLarray.insert(DLarray.begin()+sdl, rxdDLarray[k]);
		}
	}
	
}

/*===================================================================
������:    UpdateRxdBZLC   
��������:  �������жα�־���
����:
iRXD            ���жα�ţ�ȫ������Ϊ0��
bzlcArray       ԭ���û����õı�־���
���:         
�㷨:      ������λ�ı仯����ϵı�־����Ƿ����������µĶ�����Ĭ�ϱ�־���    
��ע:      ���øú���ǰ����Ҫ���¶���
��д:      ��˧ 2017.6.20 
=====================================================================*/
void RHA::UpdateRxdBZLC(int iRXD, 	vector<BZLCStructTable> &bzlcArray)
{
	if (XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	int sdl, edl;
	vector<DLTab> rxdDLarray;
	ACHAR  rxdName[50];

	//������ʼ����
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);

	//��ȡ���жζ���
	rxdDLarray.assign(DLarray.begin()+sdl, DLarray.begin()+edl+1);

	_tcscpy(rxdName, DLarray[sdl].RXDName);//���ж���


	//����־����Ƿ��� //818 ���κ�����DLEDITDLG::SAVEDL() RHA::CmpNewBZLCwithOld()����ͬ
	CString str;
	ACHAR gh[16], ckml[50];
	double xclc=0.0;
	double dist=0.0;
	bool isDel=true;//�Ƿ�ɾ����־���
	int iDL=-1;
	double dml=0;
	ACHAR  dlgh[20];


	for (int i=0; i<bzlcArray.size(); i++)
	{
		isDel=true;

		//�����־��̲�����·�ϣ���ôɾ��
		//���㵽��·�ľ����ͶӰ���
		dist=DistToXL(array, tarray, bzlcArray[i].X, bzlcArray[i].Y, bzlcArray[i].xyToprojml);

		if (dist<0.01)
		{//����·��

			iDL=FindNearDL(bzlcArray[i].xyToprojml, rxdDLarray, true);
			if(iDL>-1)
			{
				split_ckml(bzlcArray[i].CKml, dml, gh);
				_tcsupr(gh);
				_tcscpy(dlgh, rxdDLarray[iDL].EGH);

				//�����־��̺�ǰ������Ĺںš���̲�ƥ�䣬��ôɾ��
				if(_tcscmp(gh, dlgh) == 0
					&& dml-0.0001>rxdDLarray[iDL].ELC 
					&& dml+0.0001<rxdDLarray[iDL+1].BLC)
				{
					//�ں���ǰһ����һ�¡� ����ڶ�������
					isDel=false;
				}
				else
				{
					str.Format(L"��־���%s��ǰ������Ĺںš���̲�ƥ�䣬�Զ�ɾ��\n", bzlcArray[i].CKml);
					ads_printf(str);
				}
			}
			else
			{
				str.Format(L"��־���%sû�ҵ�ǰһ���������Զ�ɾ��\n", bzlcArray[i].CKml);
				ads_printf(str);
			}

		}
		else
		{
			str.Format(L"��־���%s������·�ϣ��Զ�ɾ��\n", bzlcArray[i].CKml);
			ads_printf(str);
		}

		if(isDel)
		{//ɾ����־���
			bzlcArray.erase(bzlcArray.begin()+i);
			i--;
		}
	}

	
	//�����¶��������ı�־���
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
		tmpBZLC.kmlTocml=TYLC1(tmpBZLC.CKml, rxdDLarray);//�ú���ֻ���������ж�

		BZLCarray.push_back(tmpBZLC);
	}

	//����ԭ�еı�־���
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
������:    UpdateSglRxdLC   
��������:  ����ĳ���жεĶ����ͱ�־���
����:
iRXD            ���жα�ţ�ȫ������Ϊ0��
isCalXYarray    �Ƿ������Ԫ
���:         
�㷨:      
��ע:      ע�Ȿ����û�м���������ж�֮�������������ͶӰ���
			�����Ե��ñ�����֮����Ҫ���ú��� CalDLprojml����
��д:      ��˧ 2017.6.30 
=====================================================================*/
void RHA::UpdateSglRxdLC(int iRXD, bool isCalXYarray)
{
	//������Ԫ
	if (isCalXYarray)
		getpxy_DATA();

	//�����־�������
	vector<BZLCStructTable> bzlcArray;

	if (XLShuXing != SJJX || XLShuXing != SJ2X)
	{
		//��ȡ��־�������(�û�����)
		GetiRxdBZLCArray(iRXD, bzlcArray);

		//ɾ�������ж�ԭ�еı�־���(�������ձ�־���)
		DeleteiRXDBZLC(iRXD, true);
	}

	//������λ�ı仯���¼������жζ���
	UpdateRxdDL(iRXD);

	//�������жα�־���
	UpdateRxdBZLC(iRXD, bzlcArray);
}

/*===================================================================
������:    UpdateLC   
��������:   ����ĳ���жεĶ����ͱ�־���
			�������¼�������ж�֮��Ķ�����ͶӰ���
����:
iRXD            ���жα�ţ�ȫ������Ϊ0��
���:         
�㷨:      
��ע: 
��д:      ��˧ 2017.6.20 
=====================================================================*/
void RHA::UpdateLC(int iRXD)
{
	////������Ԫ
	//getpxy_DATA();

	////�����־�������
	//vector<BZLCStructTable> bzlcArray;

	//if (XLShuXing != SJJX || XLShuXing != SJ2X)
	//{
	//	//��ȡ��־�������(�û�����)
	//	GetiRxdBZLCArray(iRXD, bzlcArray);

	//	//ɾ�������ж�ԭ�еı�־���(�������ձ�־���)
	//	DeleteiRXDBZLC(iRXD, true);
	//}

	////������λ�ı仯���¼������жζ���
	//UpdateRxdDL(iRXD);

	////�������жα�־���
	//UpdateRxdBZLC(iRXD, bzlcArray);

	//����������жεĶ����ͱ�־���
	UpdateSglRxdLC(iRXD);

	//������ʼ����
	int sdl=0, edl=0;
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);

	//���¼�������ж�֮�������������ͶӰ���
	CalDLprojml(edl+1);
}

/*===================================================================
������:     CalDLprojml   
��������:   ��ĳ������ʼ������֮������ж�����ͶӰ���
����:
iStart      �Ӹö�����ʼ����֮������ж�����ͶӰ���
���:       ������Ķ��������ͶӰ���    
�㷨:      
��ע:      �����λ�仯���ú�����Ҫ��getpxy_DATA()������ʹ��  818 ��ã�
��д:      ��˧ 2017.6.28 
=====================================================================*/
void RHA::CalDLprojml(int iStart)
{
	getpxy_DATA();

	if (iStart<0 || iStart+1>DLarray.size())
	{
		//ads_printf(L"�������ͶӰ���ʱ�����±����\n");
		return;
	}

	int sdl=0, edl=0;

	//���iStart�������ж�������ôȡ������ʼ������ʼ����
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

	//���μ��������ͶӰ���
	for(int i=iStart; i<DLarray.size(); i++)
	{
		if (DLarray[i].Indx == -3)//���ж���ȡ�Ķ���
			continue;
		else if (DLarray[i].Indx == -2 || DLarray[i].Indx == -1)//���жηֽ�� �� ���յ㲢�ж���
			TrsCkmltoProjml(DLarray[i].BGH, DLarray[i].BLC, DLarray[i].TYLC);
		else if (DLarray[i].Indx >= 0)
		{
			GetiRXDDLinfo(DLarray[i].Indx, sdl, edl, DLarray);

			if(sdl>=edl || sdl != i)
			{
				ads_printf(L"��ȡ�������ն�����Ŵ���\n");
				continue;
			}

			//�������
			if (sdl == 0)
				//�������
				DLarray[sdl].TYLC=0.0;
			else
				TrsCkmltoProjml(DLarray[sdl].BGH, DLarray[sdl].BLC, DLarray[sdl].TYLC);

			//�м�
			for (int j=sdl+1; j<edl; j++)
				DLarray[j].TYLC=DLarray[j-1].TYLC+DLarray[j].BLC-DLarray[j-1].ELC;

			//�����յ�
			if(edl+1 == DLarray.size())
			{//�յ�����
				int jdNum=JDarray.size();
				DLarray[edl].TYLC=QXB[jdNum-1].KHZ;
			}
			else
				TrsCkmltoProjml(DLarray[edl].EGH, DLarray[edl].ELC, DLarray[edl].TYLC);

			//���ݳ������������յ�����Ķ�ǰ
			DLarray[edl].BLC=DLarray[edl-1].ELC+DLarray[edl].TYLC-DLarray[edl-1].TYLC;

			i=edl;
		}
	}

}



/*===================================================================
������:    CheckRxdDL   
��������:  ����λ�䶯�󣬼��ĳ���ж����յ�����Ƿ���Ȼ�ʺ�
			������������¼���ö����ļ������
����:
		iRXD                 ���жα��
		isSDLorEDL           =true�����������������=false����������յ����
���:   DLarray  
����:   ������ж����յ������Ȼ�ʺϣ�����true
�㷨:   
��ע: 
��д:      ��˧ 2017.6.20 
=====================================================================*/
bool RHA::CheckRxdDL(int iRXD, bool isSDLorEDL)
{
	if(iRXD<0 || iRXD>=GetRXDNum())//818 ɾ�����жκ�iRXD��Ҫ����
		return false;

	int sjd, ejd;//����ʼ��ֹ�����±�
	int sdl, edl;
	bool isRight=false;//
	double strtProjml1x=-1.0, strtProjml2x=-1.0;
	double endProjml1x=-1.0, endProjml2x=-1.0;

	isRight=false;
	//������ʼ����
	GetRxdSEJD(iRXD, sjd, ejd, JDarray);

	//������ʼ����
	GetiRXDDLinfo(iRXD, sdl, edl, DLarray);
	
	if (isSDLorEDL)
	{//����ԭ�е�����������

		for (int k=0;k<1;k++)//��ѭ��
		{
			//�������·������У�����������
			if (sjd == 0 && sdl == 0)
			{
				DLarray[sdl].TYLC=0.0;
				isRight=true;
				break;
			}

			//�ж� ���� ��һ�ߵ�ͶӰ�Ƿ������ж���
			RHA *pStRHA_BL=NULL;//��������
			pStRHA_BL=NULL;

			pStRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[sdl].BLName);
			if(pStRHA_BL == NULL)
			{
				ads_printf(L"�Ҳ���������\n");
				break;
			}

			if(!pStRHA_BL->TrsCkmltoProjml(DLarray[sdl].BGH, DLarray[sdl].BLC, strtProjml1x))
			{
				ads_printf(L"�����ڲ��������Ҳ�����Ӧ���ֳ����\n");
				break;
			}

			if (pStRHA_BL->GetiRxdByProjml(strtProjml1x)<0)
				 break;

			//������һ�ߵ�ͶӰ�Ƿ��ڶ�Ӧ�ļ�ֱ����
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
				{//���ڼ�ֱ����
					break;
				}
			}


			//�ж� ���� �Ƿ��ڶ��ߵļ�ֱ����
			if (!CalXJJ(strtProjml1x, pStRHA_BL->array, pStRHA_BL->tarray, array, tarray, strtProjml2x, true))
				break;

			if (sjd>0 && !(strtProjml2x+0.0001<QXB[sjd].KZH
				&& strtProjml2x>QXB[sjd-1].KHZ+0.0001))
				break;

			//�����Ƿ�����ߵ�ǰһ�������ص� 818 ǰһ�������Ƿ���ʱ�Ѿ����£�
			if(sdl>0 && strtProjml2x+0.0001<DLarray[sdl-1].TYLC)
				break;

			//�����ָö������Ա��������¸ö����ļ������
			isRight=true;
			DLarray[sdl].TYLC=strtProjml2x;
		}
	}
	else
	{
		//���������յ����
		for (int k=0;k<1;k++)//��ѭ��
		{

			//��·�յ����У���������
			if (ejd == JDarray.size()-1)
			{
				isRight=true;
				DLarray[edl].TYLC=xl_length;
				break;
			}

			//������һ�ߵ�ͶӰ�Ƿ������ж���
			RHA *pEndRHA_BL=NULL;//�յ������
			pEndRHA_BL=NULL;

			pEndRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[edl].BLName);
			if(pEndRHA_BL == NULL)
			{
				ads_printf(L"�����Ҳ���������\n ");
				break;
			}

			if(!pEndRHA_BL->TrsCkmltoProjml(DLarray[edl].EGH, DLarray[edl].ELC, endProjml1x))
			{
				ads_printf(L"�����ڲ��������Ҳ�����Ӧ���ֳ����\n");
				break;
			}

			if (pEndRHA_BL->GetiRxdByProjml(endProjml1x)<0)
				break;

			//������һ�ߵ�ͶӰ�Ƿ��ڶ�Ӧ�ļ�ֱ����
			double KZH1x, KHZ1x;
			int iJD1x;

			if(ejd<JDarray.size()-1 && (JDarray[ejd+1].JDXZ == BXPX 
				|| JDarray[ejd+1].JDXZ == BX_NotPX) )//818 ���в�ƽ�н�����Ϣ�ļ�¼��
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
				{//���ڼ�ֱ����
					break;
				}
			}


			//�����Ƿ��ڶ��ߵļ�ֱ����
			if (!CalXJJ(endProjml1x, pEndRHA_BL->array, pEndRHA_BL->tarray, array, tarray, endProjml2x, true))
				break;

			if (ejd<JDarray.size()-1 && !(endProjml2x>QXB[ejd].KHZ+0.0001
				&& endProjml2x+0.0001<QXB[ejd+1].KZH))
			{//���ڼ�ֱ����
				break;
			}

			//�����Ƿ�����ߵĺ�һ�������ص� 818 ��һ�������Ƿ����
			/*if(ejd<JDarray.size()-1 && endProjml2x>DLarray[edl+1].TYLC+0.0001)
			{
				isRight=false; break;
			}*/

			//�����ָö������Ա��������¸ö����ļ������
			isRight=true;
			DLarray[edl].TYLC=endProjml2x;

		}

	}

	return isRight;
	
}

/*===================================================================
������:    CmpNewBZLCwithOld   
��������:  ��һ�����ж��ϣ����ݸ��µĶ������飨��·�Ķ�������ԭ���û���ӵı�־���
		   �Ƿ���������Ҫ�����ı�־������顣
����:
		   OldBZLCarray    ֮ǰ��ȫ�ߵı�־���
		   dlAry           һ�����жεĶ�������
���:      bzlcAry   
�㷨:      ����û����ı�־��̻�����·��
           �����ҹں���ǰһ����һ�¡� ����ڶ�������ͱ��� 
��ע: 
��д:      ��˧ 2016.7.20 
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
				, OldBZLCarray[i].X, OldBZLCarray[i].Y);//818 ���־���ͶӰ��������ܴ󣬵�û���ԣ�

			if (pz.lc+0.0001<dlAry[0].TYLC 
				|| pz.lc-0.0001>dlAry[dlAry.size()-1].TYLC)
			{
				//���ڶ�������
				continue;
			}

			xlpoint_pz(array, tarray, pz);
			dist=sqrt(pow((OldBZLCarray[i].X-pz.x),2.0)
				+pow((OldBZLCarray[i].Y-pz.y),2.0));
			if (dist<0.01)
			{
				//����·��
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
						//�ں���ǰһ����һ�¡� ����ڶ�������
						bzlcAry.push_back(OldBZLCarray[i]);
					}
				}
				else
					ads_printf(L"\n����û�ҵ�ǰһ��������");//nan

			}
		}
	}

}

/*===================================================================
������:CmpNewDLwithOld   
------------------------------------------------------------
��������:�Ա�ԭʼ����͵�ǰ���㣬ȷ���Ƿ���ԭ���Ķ����ͱ�־��̣����¶����ͱ�־�������
------------------------------------------------------------
����:
      OldJDarray   ԭʼ����
	  OldDLarray   ԭʼ����
      JDarray   ��ǰ����
------------------------------------------------------------
���: DLarray   ��ǰ����
------------------------------------------------------------
�㷨: 
һ�������б�༭ǰ���жεĶ������������飬���µ����жν�������ȶԣ�����û�䣬
��Ȼ����ԭ��������������Ϣ�仯�������Զ����ɵĶ�����
�������ڼ��л��ߣ�1������������� 2��������Ԫ���ȶԶ������Ƿ�仯
3��������䣬������ǰ�����ݵ�ǰһ���Ϻ�ļ��γ��ȼ��㡣����ɾ��������
�������ڶ��ߣ�����һ�������������յ�����Ƿ���ֱ�߶��ϣ�������ڣ�
���Զ����ɵ��������ն�����
------------------------------------------------------------
��ע: 1 ���øú���ǰ�����������꣬Ȼ������µ���Ԫ
	  2 �˺�����༭��������CDLEditDlg::OnBnClickedSavedl()����      818       
------------------------------------------------------------
��д: ��˧ 2016.7.30
=====================================================================*/
void RHA::CmpNewDLwithOld()
{
	if(OldJDarray.size()<2 && OldDLarray.size()<2)
		return;

	//��ձ�־���
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
		ads_printf(L"������������\n");//nan
		return;
	}

	//818 ���Ǳ��������� ��TYLC()һ���
	if (OldJDarray[0].JDXZ == RX_Start
		&& JDarray[0].JDXZ == RX_Start)
	{
		_tcscpy(DLarray[0].BGH, OldDLarray[0].BGH); 
		DLarray[0].BLC=OldDLarray[0].BLC;
		_tcscpy(DLarray[0].EGH, OldDLarray[0].EGH); 
		_tcscpy(DLarray[1].BGH, DLarray[0].EGH);
	}

	//�����յ�����Ϻ� 
	if (OldJDarray[OldJDarray.size()-1].JDXZ == RX_End
		&& JDarray[JDarray.size()-1].JDXZ == RX_End)
	{
		_tcscpy(DLarray[DLarray.size()-1].EGH, OldDLarray[OldDLarray.size()-1].EGH); 
		DLarray[DLarray.size()-1].ELC=OldDLarray[OldDLarray.size()-1].ELC;
	}



	//ȫ�����е������һ�ߣ�
	if(oldRxdNum == 1 && OldJDarray[0].JDXZ == RX_Start 
		&& OldJDarray[OldJDarray.size()-1].JDXZ == RX_End
		&& curRxdNum == 1 && JDarray[0].JDXZ == RX_Start 
		&& JDarray[JDarray.size()-1].JDXZ == RX_End)
	{
		//�м����
		tmpDLAry.clear();
		for (int k=1; k<OldDLarray.size()-1; k++)
		{
			//��������Ͼ���Ӷ���
			pz.lc=PROJ_ML(array, tarray, OldDLarray[k].N,  OldDLarray[k].E
				, OldDLarray[k].N,  OldDLarray[k].E);

			xlpoint_pz(array, tarray, pz);
			dist=sqrt(pow((OldDLarray[k].N-pz.x),2.0)
				+pow((OldDLarray[k].E-pz.y),2.0));
			if (dist<0.01)//������
				tmpDLAry.push_back(OldDLarray[k]);

			tmpDLAry[tmpDLAry.size()-1].TYLC=pz.lc;//ͶӰ���
		}

		//��������ͶӰ�������
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

		//����ÿ�������Ķ�ǰ
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

		//��־���
		if(XLShuXing != SJJX && XLShuXing != SJ2X
			&& OldBZLCarray.size()>0)
		{
			CmpNewBZLCwithOld(tmpDLAry, tmpBZLCAry);
			BZLCarray.clear();
			BZLCarray=tmpBZLCAry;
		}

		return;



	}

	

	//���ֲ��е���������ߣ�
	oldRxdSE_JDix.clear();
	oldRxdSE_DLix.clear();
	oldRxdSE_JDix.resize(oldRxdNum);
	oldRxdSE_DLix.resize(oldRxdNum);

	//�γ���ǰ�����жν��㡢�����±�����
	int sjd, ejd;//����ʼ��ֹ�����±�
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

	RHA *pStRHA_BL=NULL;//��������
	RHA *pEndRHA_BL=NULL;//�յ������
	int newSJD, newEJD;//�µĽ����±�
	int newSDL, newEDL;

	double strtProjml1x=-1.0, strtProjml2x=-1.0;
	double endProjml1x=-1.0, endProjml2x=-1.0;

	bool isSame=false;//�Ƿ��ҵ���ͬ���ж�
	bool cpySDLorNot, cpyEDLorNot;//�Ƿ�������ʼ��ֹ����
	CString mes;

	for(int i=0; i<curRxdNum; i++)
	{
		//�γ��µ����жν�������Ͷ�������
		GetRxdSEJD(i, newSJD, newEJD, JDarray);
		GetiRXDDLinfo(i, newSDL, newEDL, DLarray);

		if (newSJD<0 || newEJD<0 || newSDL<0 || newEDL<0)
		{
			mes.Format(L"��%d�����жεĽ����������ô���\n", i);
			ads_printf(mes);
			continue;
		}

		//ƥ��֮ǰ�����ж�
		isSame=false;
		int j;
		for(j=0; j<oldRxdNum; j++)
		{
																		
			if (oldRxdSE_JDix[j].End-oldRxdSE_JDix[j].Strt == newEJD-newSJD)//818 δ�������ж����յ�������ܲ������������¾ɶ���һ��
			{
				//��������ͬ�����
				//���жν��������Ƿ���ͬ
				isSame=true;
				int m=newSJD;
				for(int k=oldRxdSE_JDix[j].Strt; k <= oldRxdSE_JDix[j].End; k++)
				{
					//���㼸����Ϣ�����������Ƿ���ͬ
					if(OldJDarray[k].JDXZ != JDarray[m].JDXZ 
						|| fabs(OldJDarray[k].N - JDarray[m].N)>0.001 
						|| fabs(OldJDarray[k].E - JDarray[m].E)>0.001  
						|| fabs(OldJDarray[k].R - JDarray[m].R)>0.001 
						|| fabs(OldJDarray[k].l1 - JDarray[m].l1)>0.001
						|| fabs(OldJDarray[k].l2 - JDarray[m].l2)>0.001)
					{
						isSame=false;break;//�޸�ǰ�����жα仯��
					}

					//����ʼ�Ĳ�����Ϣ�Ƿ���ͬ���������յ����е������
					if (OldJDarray[k].JDXZ == RX_Start && oldRxdSE_JDix[j].End>oldRxdSE_JDix[j].Strt)
					{
						if (k>0 && m>0)
						{
							//���������
							if (_tcscmp(OldJDarray[k-1].BLName, JDarray[m-1].BLName) != 0 
								|| _tcscmp(OldJDarray[k-1].BLJDName, JDarray[m-1].BLJDName) != 0 
								|| fabs(OldJDarray[k-1].XJJ - JDarray[m-1].XJJ)>0.0001)
							{
								isSame=false;break;
							}

						}
						else if ( k*m == 0 && k+m >0)
						{
							//����һ������㣨�����ǰ������·�������
							isSame=false;break;
						}
						//������㣬����Ƚϲ���

					}

					//����ֹ�Ĳ�����Ϣ�Ƿ���ͬ
					if (OldJDarray[k].JDXZ == RX_End || oldRxdSE_JDix[j].End == oldRxdSE_JDix[j].Strt)
					{
						if(k<OldJDarray.size()-1 && m<JDarray.size()-1)
						{
							//�������յ�
							if (_tcscmp(OldJDarray[k].BLName, JDarray[m].BLName) != 0
								|| _tcscmp(OldJDarray[k].BLJDName, JDarray[m].BLJDName) != 0 
								|| fabs(OldJDarray[k].XJJ - JDarray[m].XJJ)>0.0001)
							{
								isSame=false;break;
							}
						}
						else if(k == OldJDarray.size()-1 && m == JDarray.size()-1)
						{//�����յ�
						}
						else
						{//����һ�����յ�
							isSame=false;break;
						}
						
					}
					
					m++;

				}

				if (isSame)
					break;
			}
		}
		

		//���û�䣬��ԭ�������жζ����滻�޸ĺ������
		cpySDLorNot=true;
		cpyEDLorNot=true;

		if (isSame)
		{		//��ǰ�����жν��㡢�����±�
			sdl=oldRxdSE_DLix[j].Strt;
			edl=oldRxdSE_DLix[j].End;
			sjd=oldRxdSE_JDix[j].Strt;
			ejd=oldRxdSE_JDix[j].End;

			pStRHA_BL=NULL;
			pEndRHA_BL=NULL;

			//�����Ƿ����ԭ�е�����������
			for (int k=0;k<1;k++)//��ѭ��
			{
				//2 �����Ƿ��ڶ�����

				pz.lc=PROJ_ML(array, tarray, OldDLarray[sdl].N,  OldDLarray[sdl].E
					, OldDLarray[sdl].N,  OldDLarray[sdl].E);

				strtProjml2x=pz.lc;

				//1 ��·���
				if (newSJD == 0)
				{
					if (_tcscmp(DLarray[newSDL].BLName, L"") == 0)//ɾ������������������������������ 818
						break;
				}
				
				xlpoint_pz(array, tarray, pz);
				dist=sqrt(pow((OldDLarray[sdl].N-pz.x),2.0)
					+pow((OldDLarray[sdl].E-pz.y),2.0));
				if (dist>0.01)
					{cpySDLorNot=false; break;}

				
				
				//�����Ƿ��ڶ��ߵļ�ֱ����
				if (newSJD>0 && !(strtProjml2x+0.0001<QXB[newSJD].KZH
					&& strtProjml2x>QXB[newSJD-1].KHZ+0.0001))
				{
					cpySDLorNot=false; break;
				}

				//�����Ƿ�����ߵ�ǰһ�������ص�
				if(newSDL>0 && strtProjml2x+0.0001<DLarray[newSDL-1].TYLC)
				{
					cpySDLorNot=false; break;
				}

				//������һ�ߵ�ͶӰ�Ƿ������ж���
				pStRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[newSDL].BLName);
				if(pStRHA_BL == NULL)
				{
					ads_printf(L"\n �Ҳ��������ߣ�");cpySDLorNot=false; break;
				}

				CalXJJ(strtProjml2x, array, tarray, pStRHA_BL->array
					 , pStRHA_BL->tarray, strtProjml1x, false);
				if (pStRHA_BL->GetiRxdByProjml(strtProjml1x)<0)
				{
					cpySDLorNot=false; break;
				}

				//������һ�ߵ�ͶӰ�Ƿ��ڶ�Ӧ�ļ�ֱ����
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
			
			
			//���������յ����
			for (int k=0;k<1;k++)//��ѭ��
			{

				//2 �����Ƿ��ڶ�����
				pz.lc=PROJ_ML(array, tarray, OldDLarray[edl].N,  OldDLarray[edl].E
					, OldDLarray[edl].N,  OldDLarray[edl].E);

				endProjml2x=pz.lc;

				//��·�յ�
				if (newEJD == JDarray.size()-1)
				{
					if (_tcscmp(DLarray[newEDL].BLName, L"") == 0)//ɾ�� 818
						break;
				}


				xlpoint_pz(array, tarray, pz);
				dist=sqrt(pow((OldDLarray[edl].N-pz.x),2.0)
					+pow((OldDLarray[edl].E-pz.y),2.0));
				if (dist>0.01)
				{cpyEDLorNot=false; break;}

				

				//�����Ƿ��ڶ��ߵļ�ֱ����
				if (newEJD<JDarray.size()-1 && !(endProjml2x>QXB[newEJD].KHZ+0.0001
					&& endProjml2x+0.0001<QXB[newEJD+1].KZH))
				{
					cpyEDLorNot=false; break;
				}

				//�����Ƿ�����ߵĺ�һ�������ص�
				if(newEJD<JDarray.size()-1 && endProjml2x>DLarray[newEDL+1].TYLC+0.0001)
				{
					cpyEDLorNot=false; break;
				}

				//������һ�ߵ�ͶӰ�Ƿ������ж���
				pEndRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, DLarray[newEDL].BLName);
				if(pEndRHA_BL == NULL)
				{
					ads_printf(L"\n �Ҳ��������ߣ�");cpySDLorNot=false; break;
				}

				CalXJJ(endProjml2x, array, tarray, pEndRHA_BL->array
					, pEndRHA_BL->tarray, endProjml1x, false);
				if (pEndRHA_BL->GetiRxdByProjml(endProjml1x)<0)
				{
					cpyEDLorNot=false; break;
				}

				//������һ�ߵ�ͶӰ�Ƿ��ڶ�Ӧ�ļ�ֱ����
				double KZH1x, KHZ1x;
				int iJD1x;
				//if(newEJD>0)
				//	pEndRHA_BL->GetJdno1FromJdstr(JDarray[newEJD].BLJDName
				//	, pEndRHA_BL->JDarray, iJD1x);

				//if(newEJD<JDarray.size()-1 && (JDarray[ejd+1].JDXZ == BXPX 
				//	|| JDarray[ejd+1].JDXZ == BX_NotPX) )//818 ����S��
				if(newEJD<JDarray.size()-1 && (JDarray[newEJD+1].JDXZ == BXPX 
					|| JDarray[newEJD+1].JDXZ == BX_NotPX) )//ԭ�����±�Խ����������ΰ�޸�
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
			//������
			if (cpySDLorNot)
			{
				tmpDLAry.push_back(OldDLarray[sdl]);
				tmpDLAry[0].TYLC=strtProjml2x;

				if (pStRHA_BL == NULL && newSJD == 0)
				{//�������

				}
				else
				{//��ͨ����ʼ
					//���¶�ǰ    818 //sBL<0 ��ʲô���
					pStRHA_BL->TrsProjmltoCkml(strtProjml1x, 3, tmpDLAry[0].BGH, tmpDLAry[0].BLC);

					//���BLC��֮ǰ���󣬲���֮ǰ��BLC(����)
					if (fabs(tmpDLAry[0].BLC-OldDLarray[sdl].BLC)<0.001)
						tmpDLAry[0].BLC=OldDLarray[sdl].BLC;
				}
				


			}
			else
				tmpDLAry.push_back(DLarray[newSDL]);


			//�м����
			for (int k=sdl+1; k<edl; k++)
			{
				//��������Ͼ���Ӷ���
				pz.lc=PROJ_ML(array, tarray, OldDLarray[k].N,  OldDLarray[k].E
					           , OldDLarray[k].N,  OldDLarray[k].E);
				
				if (pz.lc+0.0001<strtProjml2x 
					|| pz.lc-0.0001>endProjml2x)
				{
					//�������ж���
					continue;
				}

				xlpoint_pz(array, tarray, pz);
				dist=sqrt(pow((OldDLarray[k].N-pz.x),2.0)
					+pow((OldDLarray[k].E-pz.y),2.0));
				if (dist<0.01)//������
					tmpDLAry.push_back(OldDLarray[k]);

				tmpDLAry[tmpDLAry.size()-1].TYLC=pz.lc;//ͶӰ���
			}

			//�յ����
			if (cpyEDLorNot)
			{
				tmpDLAry.push_back(OldDLarray[edl]);
				int tmpNDL=tmpDLAry.size();//��ʱ������

				tmpDLAry[tmpNDL-1].TYLC=endProjml2x;

				if (pEndRHA_BL == NULL && newEJD+1 == JDarray.size())
				{//�յ�����

				}
				else
				{//��ͨ����ֹ
					//���¶Ϻ�
					pEndRHA_BL->TrsProjmltoCkml(endProjml1x, 3, tmpDLAry[tmpNDL-1].EGH, tmpDLAry[tmpNDL-1].ELC);

					//���ELC��֮ǰ���󣬲���֮ǰ��ELC(����)
					if (fabs(tmpDLAry[tmpNDL-1].ELC-OldDLarray[edl].ELC)<0.001)
						tmpDLAry[tmpNDL-1].ELC=OldDLarray[edl].ELC;
				}
			

			}
			else
				tmpDLAry.push_back(DLarray[newEDL]);

			//��ø����ж�Ӧ�����ı�־���
			tmpBZLCAry.clear();

			if (XLShuXing == JYJX || XLShuXing == JY2X)
				CmpNewBZLCwithOld(tmpDLAry, tmpBZLCAry);

											//818 ���жζ����Ƿ��� ps:Ӧ��Ҫ����
			                                 //818 ���ں�һ���ԣ����˹ںŵ�һ��
			//�������ж���ÿ�������Ķ�ǰ��� ��Ҫ���Ǳ�֤����λ�ò���
			//818 TYLC�������
			if(tmpBZLCAry.size()<1)
			{
				//û�б�־���
				for (int k=1; k<tmpDLAry.size(); k++)
				{
					tmpDLAry[k].BLC=tmpDLAry[k-1].ELC+tmpDLAry[k].TYLC-tmpDLAry[k-1].TYLC;
					_tcscpy(tmpDLAry[k].BGH, tmpDLAry[k-1].EGH);
				}

			}
			else
			{
				//����Ҫ������־���
				double dist=-100;
				double distwithBZLC=-100;//������ǰһ����־��̵���С����
				double distwithDL=0;//����֮��ľ���

				for (int k=1;k<tmpDLAry.size();k++)
				{
					_tcscpy(tmpDLAry[k].BGH, tmpDLAry[k-1].EGH);//�ں�һ��

					distwithBZLC=-100;
					distwithDL=tmpDLAry[k].TYLC-tmpDLAry[k-1].TYLC;

					for (int m=0;m<tmpBZLCAry.size();m++)
					{

						if( tmpBZLCAry[m].xyToprojml+0.0001<tmpDLAry[k].TYLC)
						{
							//��־����ڸö���tmpBZLCAry[m]֮ǰ
							dist=tmpDLAry[k].TYLC-tmpBZLCAry[m].xyToprojml;
							if (distwithBZLC+0.001<dist || distwithBZLC<0)
								distwithBZLC=dist;
						}
					}


					if(distwithBZLC<0 || (distwithBZLC>0 && distwithBZLC>distwithDL+0.0001))
					{
						//���û�б�־����ڶ���ǰ
						tmpDLAry[k].BLC=tmpDLAry[k-1].ELC+tmpDLAry[k].TYLC-tmpDLAry[k-1].TYLC;
					}
					//�б�־��̾Ͳ��Ķ�ǰ��� 818����

				}
			}

			//ɾ�������ж��Զ����ɵĶ���
			DLarray.erase(DLarray.begin()+newSDL
				, DLarray.begin()+newEDL+1);

			//�������õ��ı�������
			ACHAR   rxdName[50];
			_stprintf(rxdName, L"���ж�%0.1lf", tmpDLAry[0].ELC/1000.0);

			int tmpDLNum=tmpDLAry.size();
			for (int k=tmpDLNum-1;k>-1;k--)
			{
				tmpDLAry[k].Indx=i;//���жα��
				_tcscpy(tmpDLAry[k].RXDName, rxdName);//���ж���
				DLarray.insert(DLarray.begin()+newSDL, tmpDLAry[k]);
			}

			//β���־���
			for (int k=0;k<tmpBZLCAry.size();k++)
			{
				_tcscpy(tmpBZLCAry[k].GJDNum, rxdName);
				BZLCarray.push_back(tmpBZLCAry[k]);
			}
			

		}	

		
	}

}


/*===================================================================
������:CalDL_NE   
------------------------------------------------------------
��������:�����������
------------------------------------------------------------
����:
------------------------------------------------------------
���: 
------------------------------------------------------------
��д: ��˧ 2016.5.16 
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


//����ʵ�����Բ������淶��������ͼ����
void RHA::InitJBCS()
{
	IsReSetColor=FALSE;//�ֲ�������818
	setColorLineW();//�ú��������ط��Ϳ��Բ����� 818

	//��ȡƽ���ͼ����   818 tmp
	XLDataBase xlmdb;

	xlmdb.GetDrawPlaneParam(m_mdbName, m_XLName);//818 DrawParam��ͼʱ�ٶ���
	//����ͼ����
	DrawParam=xlmdb.m_DrawParam;
	
	//��ȡ�淶����
	m_Rs.clear();
	m_l0s.clear();

	CSheJiBiaoZhun biaozhun;
	CString strMDBName;
	CAdoConnection mdbConnection;
	CString strConn;
	CString strSysPath;
	int ix=0;
	int strLenth=0;

	//�������ݿ�
	if (m_mdbName != L"C:\\δ����.MDB")
	{
		strConn=L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source="+m_mdbName;
		if(!mdbConnection.Connect(strConn))
			ads_printf(L"�������ݿ�"+strMDBName+L"ʧ��!\n");
	}

	//��ȡ���ݿ���
	strMDBName=m_mdbName;
	strMDBName.MakeUpper();

	strLenth=strMDBName.GetLength();
	ix=strMDBName.Find(L"\\DATA\\");
	strMDBName=strMDBName.Right(strLenth-ix-6);

	//��ʼ��
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
	if (m_mdbName != L"C:\\δ����.MDB" && biaozhun.GetCrnCSFromMDB())
		isGuiFanExist=true;
	else if (biaozhun.GetCrnCSFromDAT())
		isGuiFanExist=true;
	else 
	{
		ULONG pSpdLvl[MAXRECORDNUMBER];
		short dLength;
		unsigned long ThePosition;//�ļ�ָ���λ��


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
	//DrawParam.m_LegendLen=10.0;//ͼ����ʵ�γ���
	//DrawParam.m_JIAODIAN=TRUE;//�Ƿ��ע����
	//DrawParam.m_IsNoteJDNum=TRUE;//�Ƿ��ע�����
	//DrawParam.m_IsNoteJDR=TRUE;//�Ƿ��ע����뾶
	//DrawParam.m_IsNoteJDAng=TRUE;//�Ƿ��ע����ת��
	//DrawParam.m_IsNoteJDOthers=TRUE;//�Ƿ��ע����������Ϣ

	//DrawParam.m_IsNoteTZD=TRUE;//�Ƿ��ע������

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
	//	DrawParam.OtherParam.MarkAry[i].LorR=1;//ǰ�������Ҳ�
	//	DrawParam.OtherParam.MarkAry[i].Colo=AllColor;
	//}

	mode_display=0;//��ʾ��·
	texth=5;
	
	m_IsNoteTZD = TRUE;



	if(m_XLName == L"���л���")
		m_isNoteNYDL = FALSE;
	else
		m_isNoteNYDL = TRUE;

	if (JDarray[0].JDXZ == BXPX)
	{
		if(JDarray[0].XJJ>0)//����Ϊ����
			draw_zybz=1;//����Ҫ�ر������
		else
			draw_zybz=-1;//����Ҫ�ر����Ҳ�
	}
	else
		draw_zybz=1;
	
	qxbbz=1;
}


/*��������setColorLineW
���ܣ�������ɫ���߿�
���룺mdbName    ���ݿ���
xlName     ��·��
allColor   ��ɫֵ
lineWeightValue     �߿�ֵ
��ע��                              */
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
			if (_tcscmp(m_XLName,L"���л���")==0)
				AllColor = 0 ;
			else if (_tcscmp(m_XLName, L"���ж���")==0)
				AllColor = 150;
			else if (_tcscmp(m_XLName, L"��������")==0)
				AllColor = 1;
			else if (_tcscmp(m_XLName, L"��������")==0)
				AllColor = 3;
			else if (_tcscmp(m_XLName, L"��ƻ���")==0)
				AllColor = 2;//��ƻ���Ϊ��ɫ
			else if (_tcscmp(m_XLName, L"��ƶ���")==0)
				AllColor = 6;
			else if (_tcscmp(m_XLName, L"�������")==0)
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

//���ƻ�������
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
	// ads_printf(L"\n AA ����=%d %d %10.3f %10.3f %5.1f %10.2f",(int)INDATA[0],nk,INDATA[1],INDATA[2],ND,LO);
	return  0;
}

//������·������Ԫ
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

	//Բ+��·����Ϊ0
	if( fabs(INDATA[0]-2)<0.00001 && l<0.00000001 ) PZ.a = INDATA[3];

	PZ.lc = P0.lc;
	PZ.r = P0.r;
	if(l<0.00000001)
		return 1;

	ptb[X]=P0.y;ptb[Y]=P0.x; ptb[Z]=0.0;

	if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000001)|| (l<=0.00000000001)) return 1;

	if (j>2 && j<=6 )
	{ //( ������)
		if(ISShow)
			G_drawspi(pWd,INDATA,l,P0,PZ, 2.0);
		xlzbxy(P0,PZ,l,INDATA,0);// ĳ��Ԫ�յ�
	}
	else if (j<=2)  // R L
	{//( ԰�� ֱ��)
		xlzbxy(P0,PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
		pt[X]=PZ.y;
		pt[Y]=PZ.x; 
		pt[Z]=0.0;
		xlzbxy(P0,PZ,l,INDATA,0); // ĳ��Ԫ�յ�
		pte[X]=PZ.y;
		pte[Y]=PZ.x;
		pte[Z]=0.0; //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		if (j==1) //ֱ��
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
		else if (j==2) // ԰�� 
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
	//��������Ҫ��
	//CalNoteQXYS();

	int NJD = arrayd.size();
	double fb,fe,de,da,aa,dd,dt ;
	AcGePoint3d  p1; 
	ACHAR ch[80],ch1[60],chrtmp[80];
	int i,c,n;
	de=xyddaa(arrayd[0].N,arrayd[0].E,arrayd[1].N,arrayd[1].E,&fb);
	///int zory=-1;//��ע 1������࣬-1�����Ҳ�
	for (i=1;i<NJD-1;i++)
	{  
		//if (arrayd[i].R<-0.1) break;
		de=xyddaa(arrayd[i].N,arrayd[i].E,arrayd[i+1].N,arrayd[i+1].E,&fe);

		aa= fe - fb ;
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0) 
		{ //��ƫ
			c=1; dd=2*texth; 
		}   
		else
		{ //��ƫ 
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
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);    // �����  
		 
		if(qxbbz == 1)
		{
			JDarray[i].IsShow=true;//nan 818 

			// �����߲��� begin
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
				//�������;
				dd=dd+16*texth; 
				dt=c*texth*1.5;
				if (c==-1) 
					dd=dd+8*texth;
			}
			else 
			{  
				//�����Ҳ�;
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
			G_maketext(pWd,p1,chrtmp,da,texth ,QXYSColor,1);   // �����  

		ads_rtos(qxb[i].a ,2,4,ch);                  
		if (aa>0) { _tcscpy(ch1,L"ay--"); }
		else _tcscpy(ch1,L"az--"); 
		_tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  ƫ��

		if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
		else { ads_rtos(R,2,DrawParam.NLC,ch); };
		_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   �뾶

		if (L1>0||L2>0)  //  ������Ϊ��
		{
			if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,DrawParam.NLC,ch); }
			else { ads_rtos(L1,2,DrawParam.NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
			if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   ���� ���
			} else
			{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
			if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,DrawParam.NLC,ch); }
			else { ads_rtos(L2,2,DrawParam.NLC,ch); };
			_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
			if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   ���Ȼ���
			};
		};

		T1=qxb[i].T1; T2=qxb[i].T2;  LL=qxb[i].L;
		if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
		else { ads_rtos(T1,2,DrawParam.NLC,ch); };
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if (fabs(L1-L2)<0.001) 
		{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
		if(JDarray[i].IsShow) G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   �г� ���
		} else
		{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
		ads_rtos(T2,2,DrawParam.NLC,ch);
		_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
		if(JDarray[i].IsShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   �����г�
		};

		ads_rtos(LL,2,DrawParam.NLC,ch);
		_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(/*JDarray[i].IsShow*/1)
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  ���߳�
		
		
		
		//9.1-zy-��ע��ҵ����
		//��ע��ҵ����
		if(JDarray[i].JDXZ == BXPX || JDarray[i].JDXZ == BX_NotPX)//���н���/S��
		{

			double HZcml=TrsProjmltoCml(QXB[i].KHZ);
			double ZHcml=TrsProjmltoCml(QXB[i].KZH);			

			double len= QXB[i].KHZ -  QXB[i].KZH;//��i�������߼��γ���
			double ProLen=HZcml-ZHcml;//��i����������������

			JDarray[i].NYDL=len-ProLen+100;

			NYDL = JDarray[i].NYDL;
			ads_rtos(NYDL,2,DrawParam.NLC,ch1);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if(JDarray[i].IsShow)
				G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   // ��ҵ����
		}
		};
		fb=fe; 
	};   


}

//void RHA::CalNoteQXYS() /*const*/ //818 ���н�������������
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
���ܣ�Դ��·pSrcRHA���߼��ƽ�Ƶõ�����·ʱ����������·�Ľ�������
���룺pSrcRHA        Դ��·
		xjj          �߼�࣬������
�����JDARRAY��N/E����
��ע�����ﲻ�ǰ��ղ��չ�ϵƽ�ƣ�ֻ�ǵ���������·ƽ��
*/
void RHA::GetJDXYwhenMoveXLbyXJJ(const RHA *pSrcRHA, double xjj)
{
	if(pSrcRHA == NULL)
		return;

	int JDNum=JDarray.size();

	if (pSrcRHA->JDarray.size() != JDNum)
	{
		ads_printf(L"������Ŀ����ȣ�\n");
		return;
	}

	//��ȫ�غ�
	if(fabs(xjj)<0.00001)
	{
		for(int i=0;i<JDNum;i++)
		{
			JDarray[i].N=pSrcRHA->JDarray[i].N;
			JDarray[i].E=pSrcRHA->JDarray[i].E;
		}

		return;
	}

	//ƽ��һ���߼��
	JDarrayStruct *curJD;
	int curBLJDIx=0;
	double fwj=0.0;

	for(int i=0;i<JDNum;i++)
	{
		curJD=&JDarray[i];
		curBLJDIx=i;//��ǰ����Ĳ����ߵĽ����±�

		if(curBLJDIx == 0)
		{//��ȫ�����							
			xyddaa(pSrcRHA->JDarray[0].N,pSrcRHA->JDarray[0].E,
				pSrcRHA->JDarray[1].N,pSrcRHA->JDarray[1].E,&fwj);

			curJD->N  =  pSrcRHA->JDarray[0].N + xjj * cos(fwj+m_dHalfPI);
			curJD->E  =  pSrcRHA->JDarray[0].E + xjj * sin(fwj+m_dHalfPI);//��������߽���							
		}
		else if(curBLJDIx+1 == pSrcRHA->JDarray.size())
		{//ȫ���յ�
			int curBLJDNum=pSrcRHA->JDarray.size();
			int jd1x=curBLJDNum-2;
			xyddaa(pSrcRHA->JDarray[jd1x].N, pSrcRHA->JDarray[jd1x].E,
				pSrcRHA->JDarray[jd1x+1].N,pSrcRHA->JDarray[jd1x+1].E,&fwj);

			curJD->N  =  pSrcRHA->JDarray[jd1x+1].N + xjj * cos(fwj+m_dHalfPI);
			curJD->E  =  pSrcRHA->JDarray[jd1x+1].E + xjj * sin(fwj+m_dHalfPI);//��������߽���
		}
		else
		{
			//���ݽ���ǰ�������ߵ��߼��ƽ��һ����Ӧ���߼��õ�
			CalExJDByParallel(pSrcRHA, pSrcRHA, i-1, i, xjj, xjj, curJD);
		}
	}
}

/*===================================================================
������:CalExJDByParrel   
------------------------------------------------------------
��������:�ɽ���ǰ��ߵ��߼�������߽�������,�����ڶ��߽��н���
------------------------------------------------------------
����: 
pBfBLRHA, pCurBLRHA     ǰ�󽻵�Ĳ�����·
bfJDNo, curJDNo         ǰ������߽�����±�
bfXJJ, curXJJ           ǰ���߼��
curJD                   ��һ�����㣨��ǰ���㣩           
=====================================================================*/
void RHA::CalExJDByParallel(const RHA *pBfBLRHA, const RHA *pCurBLRHA, int bfJDNo, int curJDNo, double bfXJJ, double curXJJ, JDarrayStruct *curJD)
{
	if(bfJDNo<0 || bfJDNo+2>pBfBLRHA->JDarray.size()
		|| curJDNo<0 || curJDNo+2>pCurBLRHA->JDarray.size())
		return;
	//���߼����ǰһ���ߵ�ƽ�бߣ�pt1,pt2��
	double fwj;
	double pt1[2],pt2[2];

	xyddaa(pBfBLRHA->JDarray[bfJDNo].N,pBfBLRHA->JDarray[bfJDNo].E
		,pBfBLRHA->JDarray[bfJDNo+1].N,pBfBLRHA->JDarray[bfJDNo+1].E,&fwj);

	pt1[0] = pBfBLRHA->JDarray[bfJDNo].N + bfXJJ * cos(fwj+m_dHalfPI);
	pt1[1] = pBfBLRHA->JDarray[bfJDNo].E + bfXJJ * sin(fwj+m_dHalfPI);
	pt2[0] = pBfBLRHA->JDarray[bfJDNo+1].N + bfXJJ * cos(fwj+m_dHalfPI);
	pt2[1] = pBfBLRHA->JDarray[bfJDNo+1].E + bfXJJ * sin(fwj+m_dHalfPI);

	//���߼�����һ���ߵ�ƽ�бߣ�pt3,pt4��
	double pt3[2],pt4[2];
	xyddaa(pCurBLRHA->JDarray[curJDNo].N,pCurBLRHA->JDarray[curJDNo].E
		,pCurBLRHA->JDarray[curJDNo+1].N,pCurBLRHA->JDarray[curJDNo+1].E,&fwj);
	pt3[0] = pCurBLRHA->JDarray[curJDNo].N + curXJJ * cos(fwj+m_dHalfPI);
	pt3[1] = pCurBLRHA->JDarray[curJDNo].E + curXJJ * sin(fwj+m_dHalfPI);
	pt4[0] = pCurBLRHA->JDarray[curJDNo+1].N + curXJJ * cos(fwj+m_dHalfPI);
	pt4[1] = pCurBLRHA->JDarray[curJDNo+1].E + curXJJ * sin(fwj+m_dHalfPI);

	//����ƽ�б��󽻵�
	INTERS(&curJD->N,&curJD->E,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);
}


//�����������꣬���������п��ܲ����򲻹���ȷ������������������������
//1������Ϊƽ�У�
//	��1��Ϊȫ�����յ㣨��Ӧһ�߽������Ϊ0�������߼����
//	��2����Ϊȫ�����յ㣨��Ӧһ�߽������>0)�����߼����
//	     ���ǰһ����Ҳ��ƽ�н��㣬��Ӧһ�߽����ǰ�����ƽ�����󽻵㡣
//2������Ϊ����
//	��1����������ʼ�����ǰһ������ƽ��Լ�������߼����
//	��2����������ֹ�������꣬�߼�ࣨ�߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//3������Ϊ���в�ƽ��
//	��1�����ǰһ������ƽ��Լ�������߼����
//ע�⣺
//1��ÿ��ֻ����һ����������
//�޸ģ���˧ 2017.6
void RHA::ModifyExJDXY()
{
	double fwj=0.0;
	RHA *pBLRHA=NULL;
	int blJDNo=0; 
	int jdNum=0;
	JDarrayStruct *curJD;
	double xjj=0.0;
	bool isBfSideParallel=false;//�Ƿ�ǰһ��Ҳ��ƽ��Լ��
	bool isAftSideParallel=false;//�Ƿ��һ��Ҳ��ƽ��Լ��
	bool isJdInfRight=false;

	jdNum=JDarray.size();
	for(int i=0;i<jdNum;i++)
	{
		curJD=&JDarray[i];//��ǰ����

		if (curJD->JDXZ == RX_One)//�����У�������������
			continue;
		else if((curJD->JDXZ == RX_Start && i == 0)
			|| curJD->JDXZ == RX_End && i == jdNum-1)//���յ����У�������������
			continue;

		isBfSideParallel=false;
		
		if( curJD->JDXZ == BXPX)
		{//�����������Ϊ����(����ȫ�����յ�)

			//�жϸ�ƽ�н��������Ϣ�Ƿ���ȷ
			isJdInfRight=IsJdBLInfRight(JDarray[i], pBLRHA, blJDNo);

			if(blJDNo == 0 && isJdInfRight)
			{//��ȫ�����							
				xyddaa(pBLRHA->JDarray[0].N,pBLRHA->JDarray[0].E,
					pBLRHA->JDarray[1].N,pBLRHA->JDarray[1].E,&fwj);
				
				xjj = curJD->XJJ;							
				curJD->N  =  pBLRHA->JDarray[0].N + xjj * cos(fwj+m_dHalfPI);
				curJD->E  =  pBLRHA->JDarray[0].E + xjj * sin(fwj+m_dHalfPI);//��������߽���							
			}
			else if(isJdInfRight && blJDNo+1 == pBLRHA->JDarray.size())
			{//ȫ���յ�
				int curBLJDNum=pBLRHA->JDarray.size();
				int jd1x=curBLJDNum-2;
				xyddaa(pBLRHA->JDarray[jd1x].N, pBLRHA->JDarray[jd1x].E,
					pBLRHA->JDarray[jd1x+1].N,pBLRHA->JDarray[jd1x+1].E,&fwj);

				//xjj = curJD->XJJ; 818 ���һ���߼��
				xjj =JDarray[i-1].XJJ;
				curJD->N  =  pBLRHA->JDarray[jd1x+1].N + xjj * cos(fwj+m_dHalfPI);
				curJD->E  =  pBLRHA->JDarray[jd1x+1].E + xjj * sin(fwj+m_dHalfPI);//��������߽���
			}
			else
			{
				//�жϸ�ƽ�н����ǰһ���Ƿ���ƽ��Լ��
				isBfSideParallel=IsBfSideRecordParallel(JDarray, i);

				if (isBfSideParallel)
				{//ǰһ����ƽ��Լ������ǰ��ƽ�б߼��㽻������
					CalExJDByParallel(JDarray, i);
				}
				else
				{
					if(!IsJdBLInfEmpty(JDarray[i]))
					{//��ƽ�н����¼��ƽ����Ϣ������һ��ƽ��

						//��鱾������Ϣ�Ƿ���ȷ����ò����ߺͲ����߽����
						if(IsJdBLInfRight(JDarray[i], pBLRHA, blJDNo))
							ModifyJDXYbyOneParall(pBLRHA->JDarray, blJDNo, JDarray, i, false);//��������
					}
				}				
			}

		}
		else  if(curJD->JDXZ == RX_Start || curJD->JDXZ == BX_NotPX)
		{//����ʼ���в�ƽ��

			isAftSideParallel=false;

			//�ж�ǰһ���Ƿ���ƽ��Լ��
			isBfSideParallel=IsBfSideRecordParallel(JDarray, i);

			//�������У��жϺ�һ���Ƿ�ƽ��
			if (curJD->JDXZ == RX_Start && JDarray[i+1].JDXZ != RX_One && JDarray[i+1].JDXZ != RX_End)
			{
				if(!IsJdBLInfEmpty(JDarray[i]))
					isAftSideParallel=true;
			}

			
			if(isBfSideParallel && isAftSideParallel)
			{//����ǰ��ƽ�б߼��㽻������
				CalExJDByParallel(JDarray, i);
			}
			else if(isBfSideParallel)
			{//����ǰһֱ�߱�������������
				
				//���ǰһ������Ϣ�Ƿ���ȷ����ò����ߺͲ����߽����
				if(IsJdBLInfRight(JDarray[i-1], pBLRHA, blJDNo))
					ModifyJDXYbyOneParall(pBLRHA->JDarray, blJDNo, JDarray, i, true);//��������
			}
			
		}
		else  if(curJD->JDXZ == RX_End)
		{//����ֹ

			if(!IsJdBLInfEmpty(JDarray[i]))
			{//����ֹ��¼��ƽ����Ϣ
				if(IsJdBLInfRight(JDarray[i], pBLRHA, blJDNo))
					ModifyJDXYbyOneParall(pBLRHA->JDarray, blJDNo, JDarray, i, false);//��������
			}
		}
	}
}



/*
���ܣ��ж�ĳһ�����ǰһ���Ƿ���ƽ��Լ��������з���true
���룺
	jdAry   ĳ�������ڵĽ�������
	jdix    ĳ������±�
�㷨���ж�ǰһ�����Ƿ��¼��ƽ����Ϣ
��д����˧   17.6.15
*/
bool RHA::IsBfSideRecordParallel(const vector<JDarrayStruct> &jdAry, int iJD) const
{
	if(iJD <= 0 || iJD+1 >= jdAry.size())
		return false;

	bool isBfSideParallel=false;

	if ( JDarray[iJD-1].JDXZ == RX_Start
		|| JDarray[iJD-1].JDXZ == RX_End)
	{
		//���ǰһ�����������յ㣬��¼�˲�������Ϣ
		if (!IsJdBLInfEmpty(JDarray[iJD-1]))
			isBfSideParallel=true;
	}
	else if (JDarray[iJD-1].JDXZ == BXPX && !IsJdBLInfEmpty(JDarray[iJD-1]))
		isBfSideParallel=true;//���ǰһ������ƽ��

	return isBfSideParallel;
}


/*
���ܣ��жϲ�������Ϣ�Ƿ�ȫ�������ȫ����true
���룺iJD    ��������
ע�⣺�����в����¼������Ϣ
��д����˧   17.6.15
*/
bool RHA::IsJdBLInfEmpty(const JDarrayStruct &iJD) const
{
	if(iJD.JDXZ == RX_One)
		return true;

	if (_tcscmp(iJD.BLName, L"") == 0
		|| _tcscmp(iJD.BLName, L"NULL") == 0
		|| _tcscmp(iJD.BLJDName, L"") == 0
		|| _tcscmp(iJD.BLJDName, L"NULL") == 0
	/*	|| iJD.XJJ*/)//818 �߼��Ĭ�ϸ����٣�
	{
		return true;
	}

	return false;
}

/*
���ܣ��жϲ�������Ϣ�Ƿ���ȷ�������ȷ����true
���룺iJD    ��������
�㷨����������·���Ͳ����߽������Ƿ���ȷ
��д����˧   17.6.15
*/
bool RHA::IsJdBLInfRight(const JDarrayStruct &iJD, RHA* &pBLRHA, int &blJDNo) const
{
	ACHAR mes[100];

	pBLRHA=NULL;
	blJDNo=0;

	//��������·
	pBLRHA=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, iJD.BLName, false);
	if (!pBLRHA)
	{
		_stprintf(mes, L"����������������ʱ��û���ҵ�%s%s�Ĳ�����%s\n"
			, m_XLName, iJD.JDName, iJD.BLName);
		ads_printf(mes);
		return false;
	}

	//�������߽���
	if(!GetJdno1FromJdstr(iJD.BLJDName, pBLRHA->JDarray, blJDNo))
	{
		_stprintf(mes, L"����������������ʱ��û���ҵ�%s%s�Ĳ��ս���%s\n"
			, m_XLName, iJD.JDName, iJD.BLJDName);
		ads_printf(mes);
		return false;
	}

	return true;
}


/*===================================================================
������:CalExJDByParrel   
------------------------------------------------------------
��������:�ɽ���ǰ��ߵ��߼�������߽�������,�����ڶ��߽��н���
------------------------------------------------------------
����: 
jdAry                   ���߽�������
jdix                    ��Ҫ�����Ķ��߽����±�
=====================================================================*/
void RHA::CalExJDByParallel(vector<JDarrayStruct> &jdAry, int jdix)
{
	if(jdix <= 0 || jdix+1 >= jdAry.size())//�������յ�
		return;

	RHA *pBfBLRHA=NULL;
	RHA *pCurBLRHA=NULL;
	int bfJDNo=0, curJDNo=0; 
	double bfXJJ=0.0, curXJJ=0.0;
	ACHAR mes[100];
	bool isBfSideParallel=true;//�Ƿ�ǰһ��Ҳ��ƽ��Լ��
	bool isAftSideParallel=true;//�Ƿ��һ��Ҳ��ƽ��Լ��

	//���ǰһ������Ϣ�Ƿ���ȷ����ò�����pBfBLRHA�Ͳ����߽���bfJDNo
	isBfSideParallel=IsJdBLInfRight(jdAry[jdix-1], pBfBLRHA, bfJDNo);
	bfXJJ=jdAry[jdix-1].XJJ;

	//��鵱ǰ������Ϣ�Ƿ���ȷ����ò����ߺͲ����߽���
	isAftSideParallel=IsJdBLInfRight(jdAry[jdix], pCurBLRHA, curJDNo);
	curXJJ=jdAry[jdix].XJJ;


	if(isBfSideParallel && isAftSideParallel)
	{//ǰ��ֱ�߱�ƽ��

		if(bfJDNo<0 || bfJDNo+2>pBfBLRHA->JDarray.size()
			|| curJDNo<0 || curJDNo+2>pCurBLRHA->JDarray.size())
			return;
		//���߼����ǰһ���ߵ�ƽ�бߣ�pt1,pt2��
		double fwj;
		double pt1[2],pt2[2];

		xyddaa(pBfBLRHA->JDarray[bfJDNo].N,pBfBLRHA->JDarray[bfJDNo].E
			,pBfBLRHA->JDarray[bfJDNo+1].N,pBfBLRHA->JDarray[bfJDNo+1].E,&fwj);

		pt1[0] = pBfBLRHA->JDarray[bfJDNo].N + bfXJJ * cos(fwj+m_dHalfPI);
		pt1[1] = pBfBLRHA->JDarray[bfJDNo].E + bfXJJ * sin(fwj+m_dHalfPI);
		pt2[0] = pBfBLRHA->JDarray[bfJDNo+1].N + bfXJJ * cos(fwj+m_dHalfPI);
		pt2[1] = pBfBLRHA->JDarray[bfJDNo+1].E + bfXJJ * sin(fwj+m_dHalfPI);

		//���߼�����һ���ߵ�ƽ�бߣ�pt3,pt4��
		double pt3[2],pt4[2];
		xyddaa(pCurBLRHA->JDarray[curJDNo].N,pCurBLRHA->JDarray[curJDNo].E
			,pCurBLRHA->JDarray[curJDNo+1].N,pCurBLRHA->JDarray[curJDNo+1].E,&fwj);
		pt3[0] = pCurBLRHA->JDarray[curJDNo].N + curXJJ * cos(fwj+m_dHalfPI);
		pt3[1] = pCurBLRHA->JDarray[curJDNo].E + curXJJ * sin(fwj+m_dHalfPI);
		pt4[0] = pCurBLRHA->JDarray[curJDNo+1].N + curXJJ * cos(fwj+m_dHalfPI);
		pt4[1] = pCurBLRHA->JDarray[curJDNo+1].E + curXJJ * sin(fwj+m_dHalfPI);

		//����ƽ�б��󽻵�
		double xtmp=0.0, ytmp=0.0;
		INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

		jdAry[jdix].N=xtmp;
		jdAry[jdix].E=ytmp;
	}
	else if (isBfSideParallel && !isAftSideParallel)
	{//ǰֱ�߱�ƽ��
		ModifyJDXYbyOneParall(pBfBLRHA->JDarray, bfJDNo, JDarray, jdix, true);
	}
	else if (!isBfSideParallel && isAftSideParallel)
	{//��ֱ�߱�ƽ��
		ModifyJDXYbyOneParall(pCurBLRHA->JDarray, curJDNo, JDarray, jdix, false);
	}
	
}


/*===================================================================
������:ModifyJDXYbyOneParall   
------------------------------------------------------------
��������:�ɽ���ǰһ�߻��һ�ߵ��߼��,�����ڵ���ƽ�н���
------------------------------------------------------------
����: 
jdAry1x                 ������·�Ľ�������
iJD1x                   �����ߵĽ����±�
jdAry2x                 ���ߵĽ�������
iJD2x                   ���ߵĽ���ţ�Ҫ�����Ľ��㣩               
isQorH                  =true��ǰһ��ƽ�У�=false����һ��ƽ��
------------------------------------------------------------
���: jdAry2x[iJD2x]��N/E����
------------------------------------------------------------
��д: ��˧ 2017.6.16
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
	{//ǰһ��ƽ��
		xjj = jdAry2x[iJD2x - 1].XJJ;
		//�������ʼǰ���Ǹ�������߼��û����,����������
		//if (fabs(xjj) < 0.1)//818 Ϊ0��������
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
	{//��һ��ƽ��

		xjj = pJD->XJJ;

		//�������ֹ���߼��û����,���������� 818
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
//�������ܣ�������ֱ������
//�㷨���Խ���Ϊ��λ�����ý���ǰ���ֱ�ߵ����յ㣬�������Ϊһ����ֱ�߽ṹ�����ֱ��������
//��ע����ֵʱ�����յ㴦��ֱ�ߵ�ǰ��λ����ͬ
//*************************************************************************/
//void RHA::CreatJZXArray()
//{
//	JZXArray.RemoveAll();
//	NJD=JDarray.size();
//	JZXArray.SetSize(NJD);
//	JZXStruct tempJZX;
//
//	//����ֱ��
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
//	//�м��ֱ��
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
//	//�յ��ֱ��
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
������:CalExReference   
------------------------------------------------------------
��������:������·��������·�Ƚϼ�����λ��ȷ�����չ�ϵ�����ɽ������ʺͶ���
------------------------------------------------------------
����: isOnlyBaseOnSJX  �Ƿ�ֻ�ǲ�����ƻ��ߣ�Ĭ��=false��=trueʱ
                       ����ʾ����·Ϊ��ƶ���ֻ���ǲ�������ߵ����
------------------------------------------------------------
�㷨:
------------------------------------------------------------
˵��: isOnlyBaseOnSJX = false����һ��ؽ�����·���չ�ϵ��
      =true�������ڼ�����ƶ��������ƻ��ߵ����ж�
	  ����ʱΪ�ֲ�����RHA����    818 Ҳ���������ƶ���??�����ļ�����ƶ��ߣ�����
------------------------------------------------------------
��д: ��˧ 2016.5.25  �޸� 2016.10.31
=====================================================================*/
void RHA::CalExReference(bool isOnlyBaseOnSJJX)
{
	if(isOnlyBaseOnSJJX && XLShuXing != SJ2X)
		ads_printf(L"����ֻ����ƶ��߲��������õĸĽ��ηֶμ��㣡\n");

	getpxy_DATA();

	CStringArray XLNameAry;
	RHA *pRHA_BL=NULL;

	bool isPxRxd=!isOnlyBaseOnSJJX;

	if(isOnlyBaseOnSJJX)
	{
		//������ƻ���
		XLNameAry.RemoveAll();
		XLNameAry.SetSize(1);
		XLNameAry[0]=SJJX;
		CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, XLNameAry[0]);
	}
	else
	{
		// �������п��ܵĲ�����· 
		CCreatPM::GetAllXLNames(m_mdbName,m_XLName, XLNameAry);

		// �������п��ܵĲ�����
		CCreatPM::SortXLNames(XLNameAry);
		for(int i=0;i <= XLNameAry.GetSize();i++)
		{
			if (XLNameAry[i] == m_XLName)
			{
				//ɾ������·֮�����·���������ܲ���
				XLNameAry.RemoveAt(i, XLNameAry.GetSize()-i/*-1*/);

				for(int j=0; j<XLNameAry.GetSize(); j++)
				{
					CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, XLNameAry[j]);
				}

				break;

			}
		}

	}

	// ���ռ����ߡ����������֮�����·�±�����
	vector<Int16> xlIndxAry;
	Int16 kk, jj;
	xlIndxAry.clear();
	for (int j=0; j<XLNameAry.GetSize(); j++)
	{
		kk=CCreatPM::IsIncluded(m_iDoc, m_mdbName, XLNameAry[j]);	//��·��g_DocManageAry[m_iDoc].XLAry�е��±�
		if (kk >= 0)
			xlIndxAry.push_back(kk);
	}

	if (xlIndxAry.size()<1)
		return;

	// ȷ�������Ƿ�Ϊ���н��㣬���ղ������ȡ����������ȵ�˳��ȷ����·�Ĳ��չ�ϵ
	AcGePoint2d pt1, pt2, pt3, pt4, pt5, pt6;
	PXJDInf PXJDtmp;
	bool isBfLinePX=false;//ǰֱ���Ƿ�ƽ��
	bool isAftLinePX=false;//��ֱ���Ƿ�ƽ��
	vector<PXJDInf> BfPXJDAry, AftPXJDAry;

	BfPXJDAry.clear();AftPXJDAry.clear();

	//1 ���
	double fwj=0.0;
	xlpoint PZ;
		//�������߷�λ��(����)
	xyddaa(JDarray[0].N,JDarray[0].E,JDarray[1].N,JDarray[1].E, &fwj);
	fwj=dms_rad(fwj);
	     
	for (int j=0; j<xlIndxAry.size(); j++)
	{
		kk=xlIndxAry[j];
		
        //��һ��ֱ�߱��Ƿ���ƽ�б�
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

	_tcscpy(JDarray[0].BLName, L"");//��ʼ��
	_tcscpy(JDarray[0].BLJDName, L"");
	JDarray[0].XJJ=0;
	
	//2 �м佻��
	int NJD=JDarray.size();
	for(int i=1;i<NJD-1;i++)
	{
		_tcscpy(JDarray[i].BLName, L"");//��ʼ�� nan
		_tcscpy(JDarray[i].BLJDName, L"");
		JDarray[i].XJJ=0;

		xyddaa(JDarray[i].N,JDarray[i].E,JDarray[i+1].N,JDarray[i+1].E, &fwj);
		fwj=dms_rad(fwj);

		pt1.x=JDarray[i].E;pt1.y=JDarray[i].N;
		pt2.x=JDarray[i+1].E;pt2.y=JDarray[i+1].N;

		AftPXJDAry.clear();
		for (int j=0; j<xlIndxAry.size(); j++)//������·��Ѱ��ƽ�б�
		{
			kk=xlIndxAry[j];//��·�±�
			
			//��ֱ�߱��Ƿ�ƽ��   
			if (CheckIsPXBasLine(fwj, lsJZXarray[i].SPt, lsJZXarray[i].EPt, g_DocManageAry[m_iDoc].XLAry[kk]->JDarray
				, g_DocManageAry[m_iDoc].XLAry[kk]->lsJZXarray, PXJDtmp, isPxRxd))
				AftPXJDAry.push_back(PXJDtmp);
		}

		if(AftPXJDAry.size()>0)
			isAftLinePX=true;
		else
			isAftLinePX=false;

		JDarray[i].JDXZ=-100;//��ֵ
		//���ղ������ȡ����������ȵ�ԭ��ȷ���Ƿ���

		if (isBfLinePX && isAftLinePX)//ǰ��ֱ�߱߶�ƽ��
		{
			int jdix1, jdix2;

			for ( jj=0; jj<BfPXJDAry.size(); jj++)
			{
				for (int k=0; k<AftPXJDAry.size(); k++)
				{
					if (_tcscmp(AftPXJDAry[k].BLName, BfPXJDAry[jj].BLName) == 0)
					{
						//ǰ�������·��ͬ�Ĳ��н������ʸ�Ϊ��������
						//if(JDarray[i-1].JDXZ == BXPX && _tcscmp(JDarray[i-1].BLName, AftPXJDAry[k].BLName) == 0)
							//break;
						pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, BfPXJDAry[jj].BLName);
						
						if (pRHA_BL && GetJdno1FromJdstr( BfPXJDAry[jj].JDName, pRHA_BL->JDarray,jdix1)
							&& GetJdno1FromJdstr(AftPXJDAry[k].JDName, pRHA_BL->JDarray, jdix2)
							&& jdix1+1 == jdix2) //����
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
				JDarray[i].JDXZ = RX_Start;//��������
				PXJDtmp=AftPXJDAry[0];
			}
				
		}
		else if (isAftLinePX) //ǰֱ�߲߱�ƽ�У���ֱ�߱�ƽ��
		{
			JDarray[i].JDXZ=RX_End; 
			PXJDtmp=AftPXJDAry[0];
		}
		else if (isBfLinePX) //ǰֱ�߱�ƽ�У���ֱ�߲߱�ƽ��->����ʼ
			JDarray[i].JDXZ=RX_Start;
		else                 //ǰ�󶼲�ƽ��>������
			JDarray[i].JDXZ=RX_One;
		
		//���������Ϣ
		if (i == 1 && isBfLinePX)
		{
			JDarray[0].JDXZ=BXPX;
			if (JDarray[1].JDXZ != BXPX)
				jj=0;

			_tcscpy(JDarray[0].BLName, BfPXJDAry[jj].BLName);
			_tcscpy(JDarray[0].BLJDName, BfPXJDAry[jj].JDName);
			JDarray[0].XJJ=BfPXJDAry[jj].xjj;
		}

		//��������:����Ϊ���С��������С�����ֹʱ����¼��������Ϣ   
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

	//3 �յ㣺����ǰһ���㸳����ֵ
	_tcscpy(JDarray[NJD-1].BLName, L"");//��ֵ
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

//���ɸĽ�������
//��ΰ���
void RHA::CalGJDArray()
{

	GJDInf OneGJD;//һ���Ľ���
	_tcscpy(OneGJD.XLName, m_XLName);
	vector<GJDInf> RXGJDArray, LYDArray, BXGJDArray;//���иĽ��Ρ����ö����顢���иĽ�������
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
			_stprintf(OneGJD.gjdname, L"%.1lf���иĽ���", dml/1000.0);

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
	//Ѱ�����ö�
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
			_stprintf(OneGJD.gjdname, L"%.1lf���иĽ���", dml/1000.0);
			
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

	//���һ�ε��յ�
	if (SProjml < xl_length-0.01)
	{
		EProjml = xl_length;
		OneGJD.BProjml = SProjml;
		TrsProjmltoNE(SProjml, OneGJD.spt[0], OneGJD.spt[1]);
		_tcscpy(OneGJD.BXLName, RefJDArray[0].BLName);
		pBaseLine->TrsNEtoCkml(OneGJD.spt[0], OneGJD.spt[1], OneGJD.BLC);
		split_ckml(OneGJD.BLC, dml, GH);
		_stprintf(OneGJD.gjdname, L"%.1lf���иĽ���", dml/1000.0);

		OneGJD.EProjml = EProjml;
		TrsProjmltoNE(EProjml, OneGJD.ept[0], OneGJD.ept[1]);
		_tcscpy(OneGJD.EXLName, RefJDArray[0].BLName);
		pBaseLine->TrsNEtoCkml(OneGJD.ept[0], OneGJD.ept[1], OneGJD.ELC);

		OneGJD.GJDType = 0;
		GJDInforArray.push_back(OneGJD);
	}
}


//�Ƚϼ��Σ�ȷ�����չ�ϵ
//��ΰ���
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

	// ���ռ����ߡ����������֮�����·�±�����
	vector<Int16> xlIndxAry;
	Int16 kk, jj;
	xlIndxAry.clear();
	for (int j=0; j<XLNameAry.GetSize(); j++)
	{
		kk=CCreatPM::IsIncluded(m_iDoc, m_mdbName, XLNameAry[j]);	//��·��g_DocManageAry[m_iDoc].XLAry�е��±�
		if (kk >= 0)
			xlIndxAry.push_back(kk);
	}

	if (xlIndxAry.size()<1)
		return;

	// ȷ�������Ƿ�Ϊ���н��㣬���ղ������ȡ����������ȵ�˳��ȷ����·�Ĳ��չ�ϵ
	AcGePoint2d pt1, pt2, pt3, pt4, pt5, pt6;
	PXJDInf PXJDtmp;
	bool isBfLinePX=false;//ǰֱ���Ƿ�ƽ��
	bool isAftLinePX=false;//��ֱ���Ƿ�ƽ��
	vector<PXJDInf> BfPXJDAry, AftPXJDAry;

	BfPXJDAry.clear();AftPXJDAry.clear();

	//1 ���
	double fwj=0.0;
	xlpoint PZ;
	//�������߷�λ��(����)
	xyddaa(RefJDArray[0].N,RefJDArray[0].E,RefJDArray[1].N,RefJDArray[1].E, &fwj);
	fwj=dms_rad(fwj);

	for (int j=0; j<xlIndxAry.size(); j++)
	{
		kk=xlIndxAry[j];

		//��һ��ֱ�߱��Ƿ���ƽ�б�
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

	_tcscpy(RefJDArray[0].BLName, L"");//��ʼ��
	_tcscpy(RefJDArray[0].BLJDName, L"");
	RefJDArray[0].XJJ=0;

	//2 �м佻��
	int NJD=RefJDArray.size();
	for(int i=1;i<NJD-1;i++)
	{
		_tcscpy(RefJDArray[i].BLName, L"");//��ʼ�� nan
		_tcscpy(RefJDArray[i].BLJDName, L"");
		RefJDArray[i].XJJ=0;

		xyddaa(RefJDArray[i].N,RefJDArray[i].E,RefJDArray[i+1].N,RefJDArray[i+1].E, &fwj);
		fwj=dms_rad(fwj);

		pt1.x=RefJDArray[i].E;pt1.y=RefJDArray[i].N;
		pt2.x=RefJDArray[i+1].E;pt2.y=RefJDArray[i+1].N;

		AftPXJDAry.clear();
		for (int j=0; j<xlIndxAry.size(); j++)//������·��Ѱ��ƽ�б�
		{
			kk=xlIndxAry[j];//��·�±�

			//��ֱ�߱��Ƿ�ƽ��   
			if (CheckIsPXBasLine(fwj, lsJZXarray[i].SPt, lsJZXarray[i].EPt, g_DocManageAry[m_iDoc].XLAry[kk]->JDarray
				, g_DocManageAry[m_iDoc].XLAry[kk]->lsJZXarray, PXJDtmp, isPxRxd))
				AftPXJDAry.push_back(PXJDtmp);
		}

		if(AftPXJDAry.size()>0)
			isAftLinePX=true;
		else
			isAftLinePX=false;

		RefJDArray[i].JDXZ=-100;//��ֵ
		//���ղ������ȡ����������ȵ�ԭ��ȷ���Ƿ���

		if (isBfLinePX && isAftLinePX)//ǰ��ֱ�߱߶�ƽ��
		{
			int jdix1, jdix2;

			for ( jj=0; jj<BfPXJDAry.size(); jj++)
			{
				for (int k=0; k<AftPXJDAry.size(); k++)
				{
					if (_tcscmp(AftPXJDAry[k].BLName, BfPXJDAry[jj].BLName) == 0)
					{
						//ǰ�������·��ͬ�Ĳ��н������ʸ�Ϊ��������
						//if(RefJDArray[i-1].JDXZ == BXPX && _tcscmp(RefJDArray[i-1].BLName, AftPXJDAry[k].BLName) == 0)
						//break;
						pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, BfPXJDAry[jj].BLName);

						if (pRHA_BL && GetJdno1FromJdstr( BfPXJDAry[jj].JDName, pRHA_BL->JDarray,jdix1)
							&& GetJdno1FromJdstr(AftPXJDAry[k].JDName, pRHA_BL->JDarray, jdix2)
							&& jdix1+1 == jdix2) //����
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
				RefJDArray[i].JDXZ = RX_Start;//��������
				PXJDtmp=AftPXJDAry[0];
			}

		}
		else if (isAftLinePX) //ǰֱ�߲߱�ƽ�У���ֱ�߱�ƽ��
		{
			RefJDArray[i].JDXZ=RX_End; 
			PXJDtmp=AftPXJDAry[0];
		}
		else if (isBfLinePX) //ǰֱ�߱�ƽ�У���ֱ�߲߱�ƽ��->����ʼ
			RefJDArray[i].JDXZ=RX_Start;
		else                 //ǰ�󶼲�ƽ��>������
			RefJDArray[i].JDXZ=RX_One;

		//���������Ϣ
		if (i == 1 && isBfLinePX)
		{
			RefJDArray[0].JDXZ=BXPX;
			if (RefJDArray[1].JDXZ != BXPX)
				jj=0;

			_tcscpy(RefJDArray[0].BLName, BfPXJDAry[jj].BLName);
			_tcscpy(RefJDArray[0].BLJDName, BfPXJDAry[jj].JDName);
			RefJDArray[0].XJJ=BfPXJDAry[jj].xjj;
		}

		//��������:����Ϊ���С��������С�����ֹʱ����¼��������Ϣ   
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

	//3 �յ㣺����ǰһ���㸳����ֵ
	_tcscpy(RefJDArray[NJD-1].BLName, L"");//��ֵ
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

//NE����תͶӰ���
void RHA::TrsNEtoProjml(double N, double E, double& projml) const//818 ������ͬ
{
	projml = PROJ_ML(array, tarray, N, E, N, E);
}

//NE����ת�ֳ����
void RHA::TrsNEtoCkml(double N, double E, ACHAR Ckml[]) const
{
	double projml = PROJ_ML(array, tarray, N, E, N, E);
	TrsProjmltoCkml(projml, Ckml, 3);
}


/*************************************************************
��������: GetiRXDFromNE
��������: �ɾ��ࡢγ�࣬���������ж���ţ�0��ʼ��
�������˵��: N ��E         γ�ࡢ����
��������ֵ��˵��:  >= 0:�������жα�� -1:������ڲ��ж�
��д:  ��˧  2016.7.1
***************************************************************/
Int16 RHA::GetiRXDFromNE(double N, double E)
{
	double projml=PROJ_ML(array, tarray, N, E, N, E);
	Int16 prevdlix=FindNearDL(projml, DLarray, true);
	if (prevdlix==DLarray.size()-1)
		return DLarray[prevdlix].Indx;
	//�ж��Ƿ������ж���
	if (prevdlix >= 0 && prevdlix+1<DLarray.size() && DLarray[prevdlix].Indx>-1 
		&& DLarray[prevdlix].Indx == DLarray[prevdlix+1].Indx)
		return DLarray[prevdlix].Indx;
	else
		return -1;
}

/*************************************************************
��������: GetiRXDFromProjml
��������: ��ͶӰ��̣����������ж���ţ�0��ʼ��
�������˵��: projml        ͶӰ���
��������ֵ��˵��:  >= 0:�������жα�� -1:������ڲ��ж�
��д:  ��˧  2016.7.1
***************************************************************/
Int16 RHA::GetiRxdByProjml(double projml) const
{
	//Int16 prevdlix=FindNearDL(projml, DLarray, true);
	////�ж��Ƿ������ж���
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
			_stprintf(mes, L"�����Ҳ������Ϊ%d�����ж����ն�����\n", i);
			ads_printf(mes);
		}
		
	}

	return -1;
}

/*************************************************************
��������: GetiRXDFromNE
��������: �ɾ��ࡢγ�࣬���������ж���ţ�0��ʼ��
�������˵��: N ��E         γ�ࡢ����
��������ֵ��˵��:  
		      ���жα��    >= 0:�������жα�� -1:������ڲ��ж�
			  projml         N ��E��Ӧ��ͶӰ���
��д:  ��˧  2016.7.23
***************************************************************/
Int16 RHA::GetiRXDFromNE(double N, double E, double &projml)
{
	projml=PROJ_ML(array, tarray, N, E, N, E);
	Int16 prevdlix=FindNearDL(projml, DLarray, true);
	//�ж��Ƿ������ж���
	if (prevdlix >= 0 && prevdlix+1<DLarray.size() && DLarray[prevdlix].Indx>-1 
		&& DLarray[prevdlix].Indx == DLarray[prevdlix+1].Indx)
		return DLarray[prevdlix].Indx;
	else
		return -1;
}



/*************************************************************
��������:    
��������: ������·ʱ�������������Ľ������飬����������ʱ��
�������: 
OutPt               ����·������ʱ����·��ʰȡ��һ��
PT1��PT2            ʰȡ���������յ�
�������:
jdAry               ���ɵ�����������
��������ֵ:         ����ɹ����ɽ������飬����true
��д��//plh06.01
***************************************************************///818 ����
bool RHA::AddRXD(AcGePoint3d PT1, AcGePoint3d PT2, AcGePoint3d OutPt
				 ,CArray<JDarrayStruct,JDarrayStruct>& jdAry, ACHAR ckml[], BOOL isPreMove)//����
{
	assertReadEnabled();

	double x1,y1,x2,y2,L1,L2,R;
	int preJD;//ǰ������
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
			double T2=QXB[preJD].T2;//���г�
			if(T2<0.00001)T2=50.0;//������Ϊ0
			double fwj=0.0;
			AcGePoint3d tempPt(x1,y1,0.0), Pt;

			fwj=BAS_CALC_FUN::CalFWJ(x1,y1,x2,y2);
			getLineEndPt(tempPt,T2,fwj,tempPt);//tempPt��preJD��HZ������

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
			getLineEndPt(interPt,T1,fwj,Pt);//�ڶ��������ZH������

			//ZH��ͳһ���
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

//��ö�Ӧ�����±�(JDi)�İ뾶��������N E����
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

	//��עS���߼�� 818 ����
	for(i=0;i<NJD-1;i++)//818 ���һ�����㲻��ע?
	{
		//���жΡ����в�ƽ�жβ����߼��
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
			//ǰ������ֹ(������������)����������ֹ������ZH���м�
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
			//��������ʼ������HZ�������ʼ�����м�
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
			//���б��ڼ�ֱ���м�
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
		Pt1.y=PZ.x;//Pt1 ���ߵ�

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[i].BLName);
		//blix=CCreatPM::IsIncluded(m_iDoc, m_mdbName, JDarray[i].BLName);//һ���±�
		if (pRHA_BL == NULL)
			continue;

		lc1x=PROJ_ML(pRHA_BL->array,pRHA_BL->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PZ.lc=lc1x;
		xlpoint_pz(pRHA_BL->array,pRHA_BL->tarray,PZ);
		Pt2.x=PZ.y;
		Pt2.y=PZ.x;//Pt2 һ�ߵ�

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
//��ע�߼�ࣺ1 ����һ�㣬2 ����һ�㣬��ע�߳���=20mm*DrawParam.SCALE����������
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

//���ĳ������·��ͶӰ���ǰ�󽻵�N/E����,����ȡ���ڱ� plh07.17 
int RHA::GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2)
{
	int i=GetPreJD(N,E);

	N1=JDarray[i].N;
	E1=JDarray[i].E;
	N2=JDarray[i+1].N;
	E2=JDarray[i+1].E;
	return i;
}

//���ĳ������·��ͶӰ���ǰһ�����
int RHA::GetPreJD(double N, double E) const
{
	double tylc=PROJ_ML(array,tarray,N,E,N,E);
	return DistToNearJD(tylc,true);
}

/*
���ܣ������ӽ���ʱ���ж�ʰȡ�����ڷ�Χ�Ƿ��������ӽ���
���룺
		N/E           ʰȡ������
�����
		iRxd          ��������жΣ�������жα�ţ�����ڲ��ж����-2
���أ� ����������ӽ��㣬����true
�㷨�� ���������е�֮������������в�ƽ�е�֮��ſ������ӽ���
��д�� ls 2017.6.29
*/
bool RHA::JudgeIfCanAddJD(double N, double E, int &iRxd)
{
	bool isOK=false;
	int iJD=0;
	iRxd=-2;

	//ǰһ�����±�
	iJD=GetPreJD(N, E);

	if(iJD<0 || iJD+1>JDarray.size())
		return false;

	if (JDarray[iJD].JDXZ == RX_Start || JDarray[iJD].JDXZ == RX_One)
	{//���ж�

		if(iJD+1<JDarray.size() && (JDarray[iJD+1].JDXZ == RX_One || JDarray[iJD+1].JDXZ == RX_End))
		{//���滹��һ�����е�

			iRxd=GetiRxdByProjml((QXB[iJD].KHZ+QXB[iJD+1].KZH)/2.0);
			if (iRxd>-1)
				isOK=true;
		}
	}
	else if (JDarray[iJD].JDXZ == BX_NotPX)
	{//���в�ƽ�ж�

		if(iJD+1<JDarray.size() && JDarray[iJD+1].JDXZ == BX_NotPX)
		{//���滹��һ�����в�ƽ�е�
				isOK=true;
		}
	}

	return isOK;
}

/*
���ܣ���ɾ������ʱ���ж�ʰȡ�����ڷ�Χ�Ƿ�����ɾ������
���룺
		N/E           ʰȡ������
�����
		iJD           ʰȡ�����ڵĽ����±�
		iRxd          ��������жΣ�������жα�ţ�����ڲ��ж����-2
���أ� �������ɾ�����㣬����true
�㷨�� ǰ�������е���ǲ��в�ƽ�е㣬�ſ���ɾ������
��д�� ls 2017.6.29
*/
bool RHA::JudgeIfCanDelJD(double N, double E, int &iJD, int &iRxd)
{
	bool isOK=false;
	double Projml=0.0;

	iRxd=-2;
	iJD=-1;

	//��ȡ�����
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
		ads_printf(L"��ʾ����ѡ��λ�������߷�Χ��\n");
		return false;
	}
	else if (iJD <= 0 || iJD+1 >= JDarray.size())
	{
		ads_printf(L"��ʾ��ͷβ���㲻��ɾ\n");
		return false;
	}

	//����Ƿ����ɾ������
	if (JDarray[iJD].JDXZ == RX_One)
	{//������
		iRxd=GetiRxdByProjml((QXB[iJD].KHZ+QXB[iJD].KZH)/2.0);
		if (iRxd>-1)
			isOK=true;
	}
	else if (JDarray[iJD].JDXZ == BX_NotPX)
	{//���в�ƽ�ж�

		if(JDarray[iJD-1].JDXZ == BX_NotPX && JDarray[iJD+1].JDXZ == BX_NotPX)
		{//ǰ���ǲ��в�ƽ�н���
			isOK=true;
		}
	}

	return isOK;
}

//����ͳһ��̣��Ƿ���ǰ�����������븽���Ľ����
int RHA::DistToNearJD(double projml, bool IsForwardTo) const//818 findprejd ��С��HZ
{
	int iJD=-1;
	int NJD=JDarray.size();

	for(int i=0;i<NJD;i++)
	{
		if(projml < 0.5*(QXB[i].KZH+QXB[i].KHZ)+0.000001)//С�����м������
		{			
			iJD=i;
			break;
		}
	}

	if(IsForwardTo)//��ǰ����
	{
		iJD--; 
		if(iJD<0)
			iJD=0;
	}

	return iJD;	
}

//�����������Ѱ�Ҹ����ǰһ�����
int RHA::FindPreJD(double projml)
{
	int iJD=-1;
	int NJD=JDarray.size();

	for(int i=0;i<NJD;i++)
	{
		if(projml+0.000001 < QXB[i].KZH)//С�����м������
		{			
			iJD=i;
			break;
		}
	}

	return iJD;
}

bool RHA::moveJZXAt(ads_point ptbase,ads_point ptout, int QQorHq)//818 �ú������ã�
{ 
	int JDno,JDno2;
	int NJD=JDarray.size();
	//double A2;

	JDno=GetPreJD(ptbase[1],ptbase[0]);
	JDno2=JDno+1;
	double OutN,OutE;
	OutN=ptout[1];
	OutE=ptout[0];
	if (QQorHq==1)//ǰ��
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
		//�뾶+������					
		tR1=R1+p1;
		//��ǰԲԲ��
		double CenN,CenE;
		double N1,E1,N0,E0,N2,E2;
		N1=JDarray[JDno-1].N;
		E1=JDarray[JDno-1].E;
		N0=JDarray[JDno].N;
		E0=JDarray[JDno].E;
		N2=JDarray[JDno2].N;
		E2=JDarray[JDno2].E;
		GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,Lo1,Lo2,CenN,CenE);
		double TN,TE;//�е�
		//���е�
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

	if (QQorHq==2)//����
	{
		double Lo1,Lo2,R2,p2,tR2,A1;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

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
		//�뾶+������					
		tR2=R2+p2;
		//��ǰԲԲ��
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
		double TN,TE;//�е�
		//���е�
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
��������: ƽ�Ƽ�ֱ��
�������:
onPt  		��·��һ��
passPt		ͨ����
����ֵ��    ��·ֱ�߱ߵ�ǰһ�����±ꣻ���û�У�����-100
�㷨:��ƫ�ƾ����onPt�����������������,�޸�JDarray��Ϣ
��д����˧ 2016.7.9
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

	//�õ�ǰ�󽻵��������жα��
	int iRxd1=-100;
	int iRxd2=-100;
	CString mes;

	//�ж�ǰһ�����Ƿ������ж���
	if(JDarray[iJD1].JDXZ == RX_Start 
		|| JDarray[iJD1].JDXZ == RX_One
		|| JDarray[iJD1].JDXZ == RX_End)
	{
		iRxd1=GetiRxdByProjml((QXB[iJD1].KZH+QXB[iJD1].KHZ)/2.0);
		if(iRxd1<0)
		{
			mes.Format(L"�������н���%d����û���ҵ����ж���\n", JDarray[iJD1].JDName);
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

	
	//�޸��߼��
	if(JDarray[iJD1].JDXZ == BXPX || JDarray[iJD1].JDXZ == RX_End
		|| (JDarray[iJD1].JDXZ == RX_Start && JDarray[iJD1].JDXZ != RX_One 
		&& JDarray[iJD1].JDXZ != RX_End ))
	{
		double dist=DistanceFromPttoLine(passPt[Y], passPt[X], N0, E0, N1, E1);
		JDarray[iJD1].XJJ += dist;
	}


	//��ͨ�������ƫ�ƺ��ֱ�����յ�����
	double fwj=0;
	xyddaa(N0, E0, N1, E1, &fwj);
	N0=passPt[Y];
	E0=passPt[X];
	N1=N0+1000*cos(fwj);
	E1=E0+1000*sin(fwj);
	

	//����ǰһ��������
	if (iJD1 == 0)
	{
		JDarray[iJD1].N=N0;JDarray[iJD1].E=E0;
	}
	else
	{
		INTERS(&JDarray[iJD1].N, &JDarray[iJD1].E, N0, E0, N1, E1
			,JDarray[iJD1-1].N, JDarray[iJD1-1].E, JDarray[iJD1].N, JDarray[iJD1].E);

	}

	//�����һ��������
	if(iJD2+1 == JDarray.size())
	{
		JDarray[iJD2].N=N1;JDarray[iJD2].E=E1;
	}
	else
	{
		INTERS(&JDarray[iJD2].N, &JDarray[iJD2].E, N0, E0, N1, E1
			, JDarray[iJD2].N, JDarray[iJD2].E, JDarray[iJD2+1].N, JDarray[iJD2+1].E);
	}

	//�������ж����
	if(iRxd1>-1)
		UpdateSglRxdLC(iRxd1);
	if (iRxd2>-1 && iRxd2 != iRxd1)
		UpdateSglRxdLC(iRxd2);

	//�������ͶӰ���
	CalDLprojml(0);


	return true;
}

/*************************************************************
��������:   ƽ�Ƽ�ֱ�ߺ󣬼���ǰ������������
�������:
onPt  		ʰȡ��·ֱ����һ��
offset		ƫ�ƾ��룬������
�㷨:��ƫ�ƾ����onPt�����������������,�޸�JDarray��Ϣ
����ֵ��    
��д����˧ 2016.7.9
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

	//�õ�ǰ�󽻵��������жα��
	int iRxd1=-100;
	int iRxd2=-100;
	CString mes;

	//�ж�ǰһ�����Ƿ������ж���
	if(JDarray[iJD1].JDXZ == RX_Start 
		|| JDarray[iJD1].JDXZ == RX_One
		|| JDarray[iJD1].JDXZ == RX_End)
	{
		iRxd1=GetiRxdByProjml((QXB[iJD1].KZH+QXB[iJD1].KHZ)/2.0);
		if(iRxd1<0)
		{
			mes.Format(L"�������н���%d����û���ҵ����ж���\n", JDarray[iJD1].JDName);
			ads_printf(mes);
			return false;
		}
	}

	//�жϺ�һ�����Ƿ������ж���
	if(JDarray[iJD2].JDXZ == RX_Start 
		|| JDarray[iJD2].JDXZ == RX_One
		|| JDarray[iJD2].JDXZ == RX_End)
	{
		iRxd2=GetiRxdByProjml((QXB[iJD2].KZH+QXB[iJD2].KHZ)/2.0);
		if(iRxd2<0)
		{
			mes.Format(L"�������н���%d����û���ҵ����ж���\n", JDarray[iJD2].JDName);
			ads_printf(mes);
			return false;
		}
	}


	
	N0=JDarray[iJD1].N;
	E0=JDarray[iJD1].E;
	N1=JDarray[iJD2].N;
	E1=JDarray[iJD2].E;
	
	//�޸��߼��
	if(JDarray[iJD1].JDXZ == BXPX || JDarray[iJD1].JDXZ == RX_End
		|| (JDarray[iJD1].JDXZ == RX_Start && JDarray[iJD1].JDXZ != RX_One 
		&& JDarray[iJD1].JDXZ != RX_End ))
		JDarray[iJD1].XJJ += offset;

	//��ƫ�ƾ����ƫ�ƺ��ֱ�����յ����� N0, E0, N1, E1
	CalLinePtByOffset(offset, N0, E0, N1, E1);

	//����ǰһ��������
	if (iJD1 == 0)
	{
		JDarray[iJD1].N=N0;JDarray[iJD1].E=E0;
	}
	else
	{
		INTERS(&JDarray[iJD1].N, &JDarray[iJD1].E, N0, E0, N1, E1
			,JDarray[iJD1-1].N, JDarray[iJD1-1].E, JDarray[iJD1].N, JDarray[iJD1].E);

	}

	//�����һ��������
	if(iJD2+1 == JDarray.size())
	{
		JDarray[iJD2].N=N1;JDarray[iJD2].E=E1;
	}
	else
	{
		INTERS(&JDarray[iJD2].N, &JDarray[iJD2].E, N0, E0, N1, E1
			, JDarray[iJD2].N, JDarray[iJD2].E, JDarray[iJD2+1].N, JDarray[iJD2+1].E);
	}

	//�������ж����
	if(iRxd1>-1)
		UpdateSglRxdLC(iRxd1);
	if (iRxd2>-1 && iRxd2 != iRxd1)
		UpdateSglRxdLC(iRxd2);

	//�������ͶӰ���
	CalDLprojml(0);

	return true;
}

/*************************************************************
��������: ��ת��ֱ��
�������:
onPt        ʰȡ��·��
basPt  		��ת����
angl		��ת�Ƕȣ���λΪ���ȣ�>0 ��ʱ�룬<0 ˳ʱ�� 
�㷨:��ƫ�ƾ����onPt�����������������,�޸�JDarray��Ϣ
��д����˧ 2016.7.9
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

	//1 ѡת����
	E0=(E0Cpy-basPt[X])*cos(angl)-(N0Cpy-basPt[Y])*sin(angl)+basPt[X];
	N0=(N0Cpy-basPt[Y])*cos(angl)+(E0Cpy-basPt[X])*sin(angl)+basPt[Y];

	E1=(E1Cpy-basPt[X])*cos(angl)-(N1Cpy-basPt[Y])*sin(angl)+basPt[X];
	N1=(N1Cpy-basPt[Y])*cos(angl)+(E1Cpy-basPt[X])*sin(angl)+basPt[Y];
	
	//2 ��ǰ��ֱ���󽻵�
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
��������: ���ɼ��Ϊstep�Ĵ��ֳ����sCKml��eCKml��׮������
�������:
sCml        ���ͳһ���
eCml  		�յ�ͳһ���
step		����, ���
�������:   lcAry       ��̱�����
�㷨:
��д��2016.7.11(ls �޸�)
***************************************************************/
void  RHA::FormLCB(double sCml, double eCml, double step, CArray<LCB, LCB> &lcAry)  
{ 
	if (step < 0.00001)//2008-01-15
		return;

	double dlELC;//�Ϻ����
	double sdlCml, edlCml;//ǰ���������ͳһ���
	double  xclc;//��ע���ֳ����
	double  cml;//xclc��Ӧ��ͳһ���
	int dlNum=DLarray.size();
	LCB tmpLCB;

	_tcscpy(tmpLCB.CKml, L"");
	lcAry.RemoveAll();

	//1 ��һ������
	sdlCml=0.0;
	if(sdlCml>sCml-0.00001 && sdlCml<eCml+0.00001)
	{
		tmpLCB.Cml=sdlCml;
		lcAry.Add(tmpLCB);
	}
	//2 �������������ӱ�ע���Ϊ������step��׮�ŵ�ͳһ���
	for (int iDL=0;iDL<dlNum-1;iDL++)
	{ 
		dlELC = DLarray[iDL].ELC;  //���ζ������������� 
		edlCml=sdlCml+DLarray[iDL+1].BLC-dlELC;//������ͳһ���

		if(edlCml<sCml-0.00001)
		{
			sdlCml=edlCml;
			continue;
		}

		if (sdlCml>eCml+0.00001)
			break;

		if (iDL>0 && iDL< dlNum-1)  // ��Ϊ�� �յ�
		{ 
			if(sdlCml>sCml-0.00001 && sdlCml<eCml+0.00001)
			{
				tmpLCB.Cml=sdlCml;			 
				tmpLCB.Flag=-1;//����
				lcAry.Add(tmpLCB);
			}
		};

		xclc=((long int)(dlELC/step))*step;//"ȡ��" ���ʵ�����
		if ((xclc-dlELC)<0.0001)  
			xclc=xclc+step;  

		while(xclc+0.0001 < DLarray[iDL+1].BLC)
		{
			cml=sdlCml+xclc-dlELC; // ͳһ���
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

	//������һ��׮
	if (tmpLCB.Cml+0.001<eCml)
	{
		tmpLCB.Cml=eCml;
		tmpLCB.Flag=0;
		lcAry.Add(tmpLCB);
	}

	//3 ͳһ���ת�ֳ���̺�ͶӰ���
	for(int kk=0; kk<lcAry.GetSize(); kk++)
	{
		TrsCmltoCkml(lcAry[kk].Cml, lcAry[kk].CKml);
		TrsCkmltoProjml(lcAry[kk].CKml, lcAry[kk].Projml);
	}
}

//��Scml��Ecml��Χ�ڵ��������������׮������LCBArray
//��ΰ��
void  RHA::AddTZDtoLCB(double Scml, double Ecml, CArray<LCB, LCB> &LCBArray)
{
	LCB toolLCB;
	int NUM=0;
	ACHAR ckml[50], GH[20];
	double Sprojml,Eprojml,dml;
	Sprojml = TrsCmltoProjml(Scml);
	Eprojml = TrsCmltoProjml(Ecml);
	for(int i=1; i<JDarray.size()-1; i++)//�Ե�������ѭ��
	{
		for(int k=0;k<4;k++)//ZH��HY��YH��HZ
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
				for (; j<LCBArray.GetSize(); j++)//���ҵ�һ�����ڸ���̵�׮�ţ�����
				{
					if (toolLCB.Cml < LCBArray[j].Cml-0.01)
					{
						LCBArray.InsertAt(j, toolLCB);
						break;
					}
				}

				if (j== LCBArray.GetSize())//û�ҵ���ֱ�Ӽ������
					LCBArray.Add(toolLCB);
			}
		}
	}
}

//������վ������̱�

void  RHA::AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT, CArray<LCB, LCB> &pLCB) //������վ������̱� 
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
				if (BTS[i][0] < pLCB[j].Cml)//�ҵ���һ�����ڸ���̵�׮��
				{
					OneLCB.Cml = BTS[i][0];
					TrsCmltoCkml(OneLCB.Cml, OneLCB.CKml);
					if(BorT=='B')//��
						OneLCB.Flag = -3;
					else if(BorT=='T')//��
						OneLCB.Flag = -4;
					else if(BorT=='S')//վ
						OneLCB.Flag =-9;
					else//���жε��������
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
				if (BTS[i][1] < pLCB[j].Cml)//�ҵ���һ�����ڸ���̵�׮��
				{
					OneLCB.Cml = BTS[i][1];
					TrsCmltoCkml(OneLCB.Cml, OneLCB.CKml);
					if(BorT=='B')//��
						OneLCB.Flag = 3;
					else if(BorT=='T')//��
						OneLCB.Flag = 4;
					else if(BorT=='S')//վ
						OneLCB.Flag =9;
					else//���жε��������
						OneLCB.Flag =10;
					pLCB.InsertAt(j, OneLCB);
					break;
				}
			}
		}
	}
}


void RHA::DeleteiRXDDLB(int iRXD)//ɾ��ĳ�����жζ�����
{
	int sDLno,eDLno;
	GetiRXDDLinfo(iRXD,sDLno,eDLno,DLarray);
	if(sDLno<0)
		return;
	DLarray.erase(DLarray.begin()+sDLno, DLarray.begin()+eDLno+1);

	//�����жα��
	for(int i=sDLno;i<DLarray.size();i++)
	{
		if (DLarray[i].Indx >= 1)// 818 �±�Ż������
			DLarray[i].Indx--;
	}
}

void RHA::DeleteOneRXD(int iRXD)
//��ָ�����жθĲ��У���ɾ����Ӧ����
{
	if(iRXD<0)
		return;

	//	assertWriteEnabled();
	//��ɾ����Ӧ���жεĽ��㣬��������Ӧ�Ĳ��н���
	int FirstJD,EndJD;//ĳһ���ж���ʼ�����
	int PreJD,NextJD;//ĳһ���ж�ǰ�󽻵��
	int PreJD1x,NextJD1x;//ĳһ���ж�ǰ�󽻵��Ӧ��һ�߽������
	int DelJD,AddJD;//Ҫɾ���Ľ��㣬Ҫ���ӵĽ���
	int dJD;//���潻����ǰ�ƶ�λ��
	int SDL,EDL;
	double RXDSml1x,RXDEml1x;
	xlpoint PZ;

	GetiRXDDLinfo(iRXD,SDL,EDL,DLarray);

	if (_tcscmp(DLarray[SDL].BLName, DLarray[EDL].BLName) != 0)
	{
		ads_printf(L"ǰ������߲�ͬ������ɾ�����жΣ�\n");return;
	}

	
	//�õ�������
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

	GetRxdSEJD(iRXD,FirstJD,EndJD,JDarray);//��ȡiRXD����β�������

	double EndXjj;//���ж����һ��������߼��
	EndXjj=JDarray[EndJD].XJJ;
	PreJD=FirstJD-1;
	NextJD=EndJD+1;

	DelJD=NextJD-PreJD-1;
	AddJD=NextJD1x-PreJD1x-1;
	dJD=DelJD-AddJD;

	//for(int i=FirstJD;i<NJD-DelJD;i++)//ɾ��RXD���㣬��ǰ��λ
	//{
	//	JDarray[i]=JDarray[i+DelJD];
	//}
	//NJD-=DelJD;
	////ɾ����ȷ��Ҫ������
	//NJD+=AddJD;
	//for(int i=NJD-1;i>=FirstJD+AddJD;i--)//���Ӳ��жν���洢λ�ã������λ
	//{
	//	JDarray[i]=JDarray[i-AddJD];
	//}
	JDarrayStruct tmpJD;
	_tcscpy(tmpJD.JDName, L"");
	_tcscpy(tmpJD.ZHLC, L"");
	_tcscpy(tmpJD.Draw_JDName, L"");

	_tcscpy(tmpJD.XLName, m_XLName);
	_tcscpy(tmpJD.BLName, DLarray[SDL].BLName);

	for(int i=0;i<DelJD;i++)//ɾ��RXD���㣬��ǰ��λ
		JDarray.erase(JDarray.begin()+FirstJD);

	for(int i=0;i<AddJD;i++)//ɾ����ȷ��Ҫ������
		JDarray.insert(JDarray.begin()+FirstJD, tmpJD);


	double lo1,lo,D;
	for(int i=FirstJD;i<FirstJD+AddJD; i++)
	{
		//		if(JDarray[PreJD].JDXZ==B_X )JDarray[i].jdno1=(PreJD1x+1)+(i-FirstJD);
		//		else JDarray[i].jdno1=(PreJD1x)+(FirstJD-i);
		_tcscpy(JDarray[i].BLJDName
			    , pRHA_BL->JDarray[(PreJD1x+1)+(i-FirstJD)].JDName);
		JDarray[i].JDXZ=BXPX;//���жν���

		//	_stprintf(JDarray[i].JDName,L"%d",JD0 + i);
		//   JDarray[i][0] = JD0 + i;
		int jdn1 =i-FirstJD +1 + PreJD1x;//��Ӧһ�߽������
		JDarray[i].XJJ=JDarray[i-1].XJJ;
		if(i==FirstJD+AddJD-1 && i+1<JDarray.size())
			JDarray[i].XJJ = EndXjj;//���һ������߼�����ǰһ�����
		double XJJ = JDarray[i].XJJ;
		
		int blJDix=-1;
		GetJdno1FromJdstr(JDarray[i].BLJDName, pRHA_BL->JDarray, blJDix);
		CalExJDByParallel(pRHA_BL, pRHA_BL, blJDix-1, blJDix,  JDarray[i-1].XJJ, JDarray[i].XJJ, &JDarray[i]);
		
		//double EXJD[2];
		//CalEXJD(jdn1,i,EXJD);
		//JDarray[i].x = EXJD[0];
		//JDarray[i].y = EXJD[1];//��������߽���
		double R = pRHA_BL->JDarray[jdn1].R;
		double JK;
		bool isJYGuiFanCS=false;

		if (XLShuXing == JYJX || XLShuXing == JY2X)
			isJYGuiFanCS=true;
		
		JK = get_XJJK(pRHA_BL->DV,R,isJYGuiFanCS);//�߼��ӿ�
		JDarray[i].QXJK = JK;
		JDarray[i].l1 = pRHA_BL->JDarray[jdn1].l1;
		JDarray[i].R = R;
		JDarray[i].l2 = pRHA_BL->JDarray[jdn1].l2;           
		lo1 = pRHA_BL->JDarray[jdn1].l1;//һ�߻���
		D = fabs(XJJ);
		//ads_printf(L"21:%d %d %d R=%lf lo=%lf D=%lf\n",i,jdn1,JDarray[i].jdno1,JDarray[i].R,lo1,D);
		//���߰뾶
		if(i>0 && i+1<JDarray.size())
		{
			double xjj2;
			xjj2 = JDarray[i-1].XJJ;//��һ�����߼��
			//   ads_printf(L"i=%d XJJ=%lf xjj2=%lf\n",i,XJJ,xjj2);
			if(fabs(XJJ-xjj2)<0.01 && fabs(XJJ)>0.01)//ǰ���߼�����
			{
				//XJJ�������߼ӿ�............		   
				if(XJJ>0)//һ��Ϊ����
				{
					if(pRHA_BL->QXB[jdn1].LorR<0)//��ת,II������࣬���ٻ���
					{
						//���仺��
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
						JDarray[i].l1 = JDarray[i].l2 =  lo;	
						D  = -1.0*D;
					}
					else
					{
						//���仺��
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
						JDarray[i].l1 = JDarray[i].l2 =  lo;						  					   
					}
					//					   ads_printf(L"21:%d lo=%lf \n",i,lo);
				}
				else//һ��Ϊ����
				{
					if(pRHA_BL->QXB[jdn1].LorR<0)//��ת,II�����ڲ�
					{
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
						JDarray[i].l1 = JDarray[i].l2 =  lo;						  					   
					}
					else
					{   //���仺��
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
						JDarray[i].l1 = JDarray[i].l2 =  lo;	
						D  = -1.0*D;
					}
				}
				//��뾶
				double B = R - D + (lo1*lo1)/24/R;
				//			ads_printf(L"lo1=%lf lo=%lf D=%lf JK=%lf B=%lf\n",lo1,lo,D,JK,B);
				JDarray[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶				
			}
			//	   ads_printf(L"2:%d %lf %lf \n",i,JDarray[i].R,JDarray[i].l1);
		}   
		//JDarray[i].jdno = JD0 + i;
		int jdno = JD0 + i;
		_stprintf(JDarray[i].JDName,L"%s%d",L"JD",jdno);//818 �����������
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
		//�����
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)!=0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)!=0))	
		{//������
			RB = 1e50;
			RE = 1e50;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//���
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)==0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)==0))	
		{
			//������
			RB = QXB[i-1].R;
			RE = QXB[i+1].R;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//һ�����
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)==0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)!=0))	
		{
			//������
			RB = QXB[i-1].R;
			RE = 1e50;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//һ�����
		if((_tcscmp(QXB[i-1].JDName,QXB[i].JDName)!=0) && (_tcscmp(QXB[i].JDName,QXB[i+1].JDName)==0))	
		{
			//������
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

//ͶӰ���תͳһ���
double  RHA::TrsProjmltoCml(double projml) const 
{
	double cml;

	if (XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		ACHAR ckml[MaxCkmlStrNum];

		TrsProjmltoCkml(projml, ckml, 3);
		TrsCkmltoCml(ckml, cml);
	}
	else//���Ǳ�־���
		cml=CalnewcmlTocml(projml);

	return cml;
}

//ͳһ���תͶӰ���
double  RHA::TrsCmltoProjml(double cml) const 
{
	double projml;

	if (XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		ACHAR ckml[MaxCkmlStrNum];

		//�ȼ����ֳ���̡��ټ��㼸�����
		TrsCmltoCkml(cml, ckml);
		TrsCkmltoProjml(ckml, projml);
	}
	else
	{
		//���Ǳ�־���
		projml=CalcmlTonewcml(cml);////818 ����д������
	}
	
	return projml;
}

//ͳһ���ת�ֳ����
void  RHA::TrsCmltoCkml(double cml, ACHAR ckml[], int nLC) const 
{
	ACHAR gh[16];
	double dml;

	dml=XLC1(cml, gh, DLarray);
	_tcscpy(ckml, LCchr(gh, dml, nLC));
}

//ͳһ���ת�ֳ����
void  RHA::TrsCmltoCkml(double cml, ACHAR gh[], double &xlc) const 
{
	xlc=XLC1(cml, gh, DLarray);
}

//ͶӰ���projmlתͳһ���cml
//��ע��һ�ߺͶ��ߺ��������ô˺���
//��д����˧ 2016.7.19
//˵�����ú���ֻ��ȫ�����е���·���߶������ж�����
double  RHA::CalnewcmlTocml(double projml) const 
{
	if ( XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		AfxMessageBox(L"����߲�Ӧ���ô˺���!");//nan
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
�������ã�ͳһ���cmlתͶӰ���projml
//��ע��1 �����ߡ�����߶����ô˺�����
        2 ������У�cml��projml��ȡ����ô˺���ʱ��ֱ�ӷ���kmlTocml
		3 �ú���ֻ��ȫ�����е���·���߶������ж����� */
double  RHA::CalcmlTonewcml(double kmlTocml) const
{
	if ( XLShuXing == SJJX || XLShuXing == SJ2X)
	{
		AfxMessageBox(L"����߲�Ӧ���ô˺���!");//nan
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

//���Ʊ�־���  
//zybz=1,-1  �����ұߣ����
Adesk::Boolean  RHA::MarkBiaoZhiLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  //818 type ���壿
{//
	//if (!m_IsNoteML)//818 ע��ƽ����������Ի���
	//	return FALSE;

	int k;
	AcGePoint3d Pt;
	AcGePoint3d  pte,ptb;
	double B, b;//�Ƕ�
	double len;
	xlpoint  PZtmp; 
	ACHAR str[256];
	AcGePoint3d BZPt;

	for(int i=0;i<BZLCarray.size();i++)  //NBZLC
	{
		if (BZLCarray[i].isAutoAdd)
			continue;

		PZtmp.lc=BZLCarray[i].xyToprojml;  // ͳһ���	
		k=xlpoint_pz(array,tarray,PZtmp);   

		B=PZtmp.a;
		//�����
		Pt[Y]=PZtmp.x; 
		Pt[X]=PZtmp.y;
		Pt[Z]=0.0;
		_tcscpy(str,BZLCarray[i].CKml);
		_tcscat(str,L" ��־���");

		//���յ�
		len=_tcslen(str)*1.4*texth;//ԭ����1.2*1.4*texth
		pte[X]=PZtmp.y+len*sin(B+pi*0.5*zybz);
		pte[Y]=PZtmp.x+len*cos(B+pi*0.5*zybz);
		G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);

		//��ת�Ƕ�
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
������:GenBZLCbyDL
------------------------------------------------------------
��������:�ɶ����Զ����ɱ�־������飬���û����õı�־���
------------------------------------------------------------
����:isCreatNew       �Ƿ��������ɣ�Ĭ��Ϊtrue
------------------------------------------------------------
���:BZLCarray        ��־������飨�������
------------------------------------------------------------
��д: ��˧ 2016.7.19  
------------------------------------------------------------
��ע�����øú���ǰ��Ӧ�����ɶ���
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

//�����־�����Ϣ��������
void RHA::CalBZLC()
{

	if (BZLCarray.size()<1 || XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	//1 ����������̺�ͶӰ���
	for(int i=0;i<BZLCarray.size();i++)
	{
		//������x,y -> ��ͶӰ���
		BZLCarray[i].xyToprojml = PROJ_ML(array,tarray,BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X,BZLCarray[i].Y);
		//�ֳ���� -> ���������
		BZLCarray[i].kmlTocml = TYLC1(BZLCarray[i].CKml, DLarray);
		//TrsCkmltoCml(BZLCarray[i].CKml, BZLCarray[i].kmlTocml);//��ͳһ���ֻ�Ͷ����йء�
	}

	//2 ����
	SortBZLCArray();

	//3 ������ϵ��K
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
		str.Format(L"\n��־%s %s", m_XLName, BZLCarray[i].kml);
		ads_printf(str);
		str.Format(L" , K-%lf", BZLCarray[i].K);
		ads_printf(str);
		tmp.push_back(BZLCarray[i]);
	}*/
}


//���ܣ���־��̰�ͶӰ�������
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
	//	str.Format(L"\n����bzlc %s", BZLCarray[i].kml);
	//	ads_printf(str);
	//	str.Format(L" , bzlc %lf", BZLCarray[i].xyToprojml);
	//	ads_printf(str);
	//}


}

//�Ľ������鰴ͶӰ�������
//��д����˧ 2016.12
//��ע����Ҫ�ȼ�����Ľ��ε�����ͶӰ���
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

//  ɾ��һ����־���
//IsDelDL �Ƿ�ɾ�������ı�־��̣�Ĭ��false
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

	//ɾ��delBZLCix
	if( delBZLCix>0 && BZLCarray[delBZLCix].isAutoAdd == false)//ɾ���û���־���
		//BZLCarray.RemoveAt(delBZLCix,1);
		BZLCarray.erase(BZLCarray.begin()+delBZLCix);
	else if( delBZLCix>0 && IsDelDL && dist<0.1)//ɾ��������־���
		//BZLCarray.RemoveAt(delBZLCix,1);
		BZLCarray.erase(BZLCarray.begin()+delBZLCix);

	
}

//�������жα�ŵõ�����
CString RHA::GetRxdName(int i)
{
	int FirstDL,EndDL;//ĳһ���ж���ʼ�����
	FirstDL=EndDL=-1;
	GetiRXDDLinfo(i, FirstDL, EndDL, DLarray);
	if (FirstDL>-1)
		return DLarray[FirstDL].RXDName;
	else
		return "";
}

//Ѱ��ĳͶӰ��̸����ı�־����±꣬���û�ҵ�����-100 
//dirctn =true����ǰ�ң�=false�������
int  RHA::FindNearBZLC(double projml, bool dirctn) 
{
	//��־�������
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
������:AddDLM
------------------------------------------------------------
��������:���Ӷ��������Զ���ӱ�־���
------------------------------------------------------------
����:
------------------------------------------------------------
��д: ��˧ 2016.7.23  
------------------------------------------------------------
��ע��  1 Ҫ��BLC��BGH������ȷ����ǰһ�����Ϻ�һ�� 818
		2 ��ʱֻ����PMSJ.CPP����
=====================================================================*/
bool  RHA::AddDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH, double TYLC, Int16 iRxd)
{
	bool isFindUserBZLC=false;

	//�������������һ������֮���Ƿ����û����õı�־���
	//������У���Ҫ������������Ϻ�Ĺںź�����Ƿ���ȷ
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
				_stprintf(mes, L"���������ĶϺ�ں�%s�ͺ�һ��־���%s�Ĺں�%s��һ�£����Ӷ���ʧ��\n"
					, EGH, BZLCarray[iBZ].CKml, gh);
				return false;
			}
			else if (ELC+0.001>dml)
			{
				_stprintf(mes, L"���������ĶϺ����%.3f���ں�һ��־���%s�����ֵ%.3f��һ�£����Ӷ���ʧ��\n"
					, ELC, BZLCarray[iBZ].CKml, dml);
				return false;
			}

			isFindUserBZLC=true;//�������û����õı�־���
		}

	}


	//�������
	ACHAR rxdName[50];
	_tcscpy(rxdName, GetRxdName(iRxd));
	setDLM(iDL,BLC,ELC,BGH,EGH,TYLC,iRxd,rxdName);

	if(!isFindUserBZLC)
	{//����û���û����õı�־���

		//�޸ĺ�һ�����Ķ�ǰ
		DLarray[iDL+1].BLC = DLarray[iDL+1].TYLC - DLarray[iDL].TYLC + DLarray[iDL].ELC;
		_tcscpy(DLarray[iDL+1].BGH, DLarray[iDL].EGH);
	}

	xlpoint pz;
	ACHAR ckml[50];
	ads_point pt;


	//��ӱ�������־���
	pz.lc=DLarray[iDL].TYLC;
	xlpoint_pz(array, tarray, pz);
	pt[X]=pz.y;pt[Y]=pz.x;pt[Z]=0;
	_tcscpy(ckml, LCchr(BGH, BLC, 10));

	AddBZLC(pt, ckml, rxdName, true);

	if(!isFindUserBZLC)
	{//����û���û����õı�־���

		//��Ϊ��һ�����Ķ�ǰ���ܸı�
		//��������Ҫ�޸ĺ�һ�����Զ������ı�־��̵��ֳ����
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
������:DelDLM
------------------------------------------------------------
��������:ɾ��ĳͳһ��̸�����һ����������
------------------------------------------------------------
����:tylc ͳһ���
------------------------------------------------------------
��д: ��˧ 2016.7.23  
------------------------------------------------------------
��ע��
=====================================================================*/
bool  RHA::DelDLM(double tylc)
{ 
	assertWriteEnabled();

	int iDL=-1;//Ҫɾ���Ķ���
	double dist=0,distmin=10000000;//������ľ���
	int i;

	//����������λ��
	for(i=0;i<DLarray.size();i++)
	{
		dist=fabs(tylc-DLarray[i].TYLC);
		if(dist<distmin)
		{
			distmin=dist;
			iDL=i;
		}
	}

	//�����������ֹ����������ɾ
	if (DLarray[iDL].Indx >= 0)
		
	{
		if ((iDL+1<DLarray.size() && DLarray[iDL].Indx != DLarray[iDL+1].Indx)
			||(iDL>0 && DLarray[iDL].Indx != DLarray[iDL-1].Indx))
		{
			ads_printf(L"\n������ֹ����������ɾ��");return false;
		}
	}
	else
	{
		ads_printf(L"\n���ж�������ɾ��");return false;
	}

	//ɾ������
	DLarray.erase(DLarray.begin()+iDL);
	
	//�޸ĺ�һ�����Ķ�ǰ
	DLarray[iDL].BLC=DLarray[iDL].TYLC-DLarray[iDL-1].TYLC
		+DLarray[iDL-1].ELC;
	_tcscpy(DLarray[iDL].BGH, DLarray[iDL-1].EGH);

	if (XLShuXing != SJJX && XLShuXing != SJ2X)
	{
		//���±�־���,ɾ����Ӧ�ı�־���
		xlpoint pz;
		pz.lc=DLarray[iDL].TYLC;
		xlpoint_pz(array, tarray, pz);
		ads_point pt;
		pt[X]=pz.y;pt[Y]=pz.x;pt[Z]=0;
		DeleteBZLC(pt, true);
	}

	return true;	
}


//ƽ�����μ�ֱ�ߡ���Բ�ߡ���С���߰뾶���
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

	ReadGuifanFile();//���淶
	if (m_XLName.Find(L"���")>-1)
		mGuiFanCS=mGJGuiFanCS;
	else
		mGuiFanCS=mJYGuiFanCS;

	Tol.setEqualVector(0.001);
	ZXLmin1=mGuiFanCS.MinZhi;
	Rmin1=mGuiFanCS.MinR1;
	Rmin2=mGuiFanCS.MinR2;
	//FindZXLenMin();//�ҳ���С��ֱ�߳�
	if(!ReadWorkdir())
	{
		ads_alert(L"����ָ����ȷ�Ĺ���·��!");
		return ;
	}
	_stprintf(fname,L"%s\\data\\Err",Cworkdir);
	fpw = _wfopen(fname,L"w");
	if(fpw==NULL)
	{
		ads_printf(L"���ļ�%sʧ��!\n",fname);
		return;
	}
	fwprintf(fpw,L"%10d\n",NERR);
	for(int i=2;i<NJD-1;i++)
	{
		ljia = QXB[i].KZH - QXB[i-1].KZH;//��ֱ��
		if(ljia<ZXLmin1)
		{
			//TrsNEtoCkml(JDarray[i].x,JDarray[i].y,ckml);
			ERR.Format(L"%s-%s��ֱ�߳�%0.2lfС����С��ֱ�߳�(һ��%0.2lf,����%0.2lf))",JDarray[i-1].JDName,JDarray[i].JDName,ljia,ZXLmin1,ZXLmin2);
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
			//�״�ͶӰ��һ�ߵ�ͳһ���
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
				ERR.Format(L"�������%s-%sֱ�߲߱�������϶�Ӧ��ƽ��",JDarray[i].JDName,JDarray[i-1].JDName);
				fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KZH + QXB[i-1].KZH),ERR);
				NERR++;
			}
		}
		if (JDarray[i].JDXZ==RX_End
			|| (JDarray[i].JDXZ == RX_Start && JDarray[i+1].JDXZ != RX_One && JDarray[i+1].JDXZ != RX_End))
		{
			EndHzTylc   = QXB[i].KHZ;//�����յ����
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
				ERR.Format(L"�����յ�%s-%sֱ�߲߱�������϶�Ӧ��ƽ��",JDarray[i].JDName,JDarray[i+1].JDName);
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
		ljia = QXB[i].KYH - QXB[i].KHY;//��Բ
		if(ljia<20.0)
		{
			ERR.Format(L"%s��Բ���߳�%0.2lfС��20m",JDarray[i].JDName,ljia);
			fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KYH + QXB[i].KHY),ERR);
			NERR++;
		}
	}
	for(int i=1;i<NJD-1;i++)
	{		
		if(JDarray[i].R<Rmin1)
		{
			ERR.Format(L"%s�����߰뾶%0.2lfС����С���߰뾶(һ��:%0.0lf,����:%0.2lf)",JDarray[i].JDName,JDarray[i].R,Rmin1,Rmin2);
			fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KYH + QXB[i].KHY),ERR);
			NERR++;
		}
	}


	//�����ⲿ��
	//1.��ȡ���ݿ��е�����
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(m_mdbName, L"�����", L"");

	//2.�������������������ֱ����ֱ��
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
				//ERR.Format(L"%s�����߰뾶%0.2lfС����С���߰뾶(һ��:%0.0lf,����:%0.2lf)",JDarray[i].JDName,JDarray[i].R,Rmin1,Rmin2);
				ERR.Format(L"%s��ƽ���������������Ϊ%s�ĵ������ص�",JDarray[i].JDName,xlmdb.m_SwitchArray[j].CenCkml);
				fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i].KYH + QXB[i].KHY),ERR);
				NERR++;
			}
		}

	}


	//��NERRд���ļ�
	rewind(fpw);
	fwprintf(fpw,L"%10d\n",NERR);
	fclose(fpw);


	if(NERR<1)
	{
		ads_printf(L"ƽ������ͨ����ֱ�ߡ���Բ�ߡ���С���߰뾶��������!\n");
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
��������:����S���ת�ǡ�����������
------------------------------------------------------------
����: �뾶����������ֱ�߳����߼��1���߼��2����ֱ�߳�����ʼ�������ߵ�ֱ������
------------------------------------------------------------
���: S���ת�ǡ�����������
------------------------------------------------------------
�㷨����:               */

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
	double xzh,yzh;//������x,y,������߷�λ��
	//xlpoint Pt;//ZH��
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
//����S���ת��
//R1,L01��һ������İ뾶����
//R2,L02�ڶ�������İ뾶����
//DetaXJJǰ���߼���ֵ
//JZXLen��ֱ�߳�
//Alfaת��(���ȵ�λ)
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
		AfxMessageBox(L"�뾶̫С");
		return false;
	}


	if (L1<0.0001) { P1=0.0;M1=0.0; }
	else {     P1=spiral_p(A1,L1);M1=spiral_xm(A1,L1);   };
	if (L2<0.0001) { P2=0.0;M2=0.0; }
	else {     P2=spiral_p(A2,L2); M2=spiral_xm(A2,L2);  };


	//////////////////////////////////////////////////////////////////////////
	//�������¹�ϵ����ת��Alfa
	//DetaXJJ / ((T1+T2)+JZXLen) = sin(Alfa)
	//����DetaXJJ / ((R1+P1)*tan(Alfa/2.0) + M1 + (R2+P2)*tan(Alfa/2.0) + M2 + JZXLen) = sin(Alfa)
	//��tan(Alfa/2.0) = t�����ܹ�ʽ��sin(Alfa) = 2*t / (1+t*t)
	//���÷�������(2*(R1+P1+R2+P2)-DetaXJJ) * t*t + 2*(M1+M2+JZXLen)*t + (-DetaXJJ) = 0
	double a,b,c,t;
	a = 2*(R1+P1+R2+P2)-DetaXJJ;
	b = 2*(M1+M2+JZXLen);
	c = -DetaXJJ;
	double Deta = b*b-4.0*a*c;
	if (Deta<0.0001)
	{
		AfxMessageBox(L"�޷��������������S��ת��");
		return false;
	}

	double t1 = (-1.0*b + sqrt(Deta)) / (2.0*a);
	double t2 = (-1.0*b - sqrt(Deta)) / (2.0*a);

	double Alf1 = 2 * atan(t1);
	double Alf2 = 2 * atan(t2);

	bool IsRuiJiao1,IsRuiJiao2;//�Ƿ����

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
		AfxMessageBox(L"S��ת��Ϊ�۽ǣ����������Ƿ�����");
		return false;
	}
	T1 = (R1+P1)*tan(Alfa/2.0) + M1;
	T2 = (R2+P2)*tan(Alfa/2.0) + M2;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//ѡ��S��
bool RHA::SelSModel(double N, double E,int& SJD,int& S_JD1,int& S_JD2)
//ɾ��ָ���㴦��S��
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
		AfxMessageBox(L"��ѡ��ΪS�䣬������ѡ��\n");
		return false;
	}
}

//���ĳ���ж������û����õı�־���
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
	{ads_printf(L"\nδ�ҵ����жζ���");}
		

	

}

//ɾ�����жα�־��� isDelSE���Ƿ�ɾ���������ձ�־���
//��ע�����øú���ʱ����Ҫ�ڶ����޸�֮ǰ           818 �������жα���������ǣ���־��̲�һ����˳��
void RHA::DeleteiRXDBZLC(int iRXD, bool isDelSE)
{
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		return;

	//ɾ�������ж�ԭ�еı�־���
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
		ads_printf(L"δ�ҵ����жζ���\n");
	}
}

//ȫ��������·�Զ�������š���������
void RHA::GenJDNameJDXZ()
{
	//��������
	int NJD=JDarray.size();
	for(int i=0; i<NJD; i++)
	{
		_stprintf(JDarray[i].JDName, L"JD%d", i);
		JDarray[i].JDXZ=RX_One;
	}
	JDarray[0].JDXZ=RX_Start;
	JDarray[NJD-1].JDXZ=RX_End;
}

//ָ��������ţ��޸������ꡢ�뾶������
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

//�����жα�Ż�ȡ���ж���·��
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

//�����ж���·����ȡ���жα��
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

//�ɡ����桿���ж���·����ȡ�����桿���жα��
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


//���ɴ�ͳһ���lc1��ͳһ���lc2����·����
//ȱʡFALSE,CONTINUOUS
//���ɴ�ͳһ���lc1��ͳһ���lc2����·����
//ȱʡFALSE,CONTINUOUS
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

	// ����λ
	PB.x=XYArray[0][1]; //N
	PB.y=XYArray[0][2]; // E
	PB.a= XYArray[0][3];// FA
	PB.lc= XYArray[0][4];
	XYArray[0][6]=XYArray[0][4];//��Ϊ0��֣���㣩��Ԫ��ʵ������±�4����¼������ͨ��Ԫ���±�6����¼���������0��ֵ�6�±긳ֵ���Ա��������Ӧ�á�wsb
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

	if (LRMFlag==LEFTLINE && DrawParam.SCALE>9.9)//���LRMFlag ������ 818
		Offset = -DrawParam.m_ExOffset;
	if (LRMFlag==RIGHTLINE && DrawParam.SCALE>9.9)
		Offset = DrawParam.m_ExOffset;
	if(DrawXySn > 1)
	{
		cml = Scml;
		PZtmp.lc = cml;
		xlpoint_pz(XYArray,XYNum,PZtmp);
		Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));

		if (DrawXyEn==DrawXySn-1)//��ͼ��Χ����һ����Ԫ
		{
			xlpoint PZtmp2;
			PZtmp2.lc = Ecml;
			xlpoint_pz(XYArray,XYNum,PZtmp2);
			if( XYArray[DrawXySn-1][0]<1.1)//ֱ��
			{
				Pt2dAy.append(AcGePoint3d( PZtmp2.y, PZtmp2.x,0));
				//	qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
			}
			else if( XYArray[DrawXySn-1][0]<2.1)//Բ
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
			else//��������
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

		if( XYArray[DrawXySn-1][0]<1.1)//ֱ��
		{
			Pt2dAy.append(AcGePoint3d( XYArray[DrawXySn][9], XYArray[DrawXySn][8],0));
			//qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
		}
		else if( XYArray[DrawXySn-1][0]<2.1)//Բ
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
		else//��������
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
		if( XYArray[i][0]<1.1)//ֱ��
		{
			Pt2dAy.append(AcGePoint3d( XYArray[i][9], XYArray[i][8],0));
			Pt2dAy.append(AcGePoint3d( XYArray[i+1][9], XYArray[i+1][8],0));
			//qID = makepolyline(Pt2dAy,-1,ZxWid,LayerName);
		}
		else if( XYArray[i][0]<2.1)//Բ
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
		else//��������
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
		if( XYArray[DrawXyEn][0]<1.1)//ֱ��
		{
			cml = Ecml;
			PZtmp.lc = cml;
			xlpoint_pz(XYArray,XYNum,PZtmp);
			Pt2dAy.append(AcGePoint3d(PZtmp.y,PZtmp.x,0));
			//hID = makepolyline(Pt2dAy1,-1,ZxWid,LayerName);
		}
		else if( XYArray[DrawXyEn][0]<2.1)//Բ
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

//����DrawXLZXByXY������lc1-lc2�����Ľ���,//ȱʡFALSE,CONTINUOUS
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
			//��һ��
			DrawXLZXByXY(lc1,GJDInforArray[sGJD].EProjml
				,array,tarray,LineWide,linetype,layer,LRMFlag);
			//�м�Ľ���
			for (int j=sGJD+1;j<eGJD; j++)
			{
				if (GJDInforArray[j].GJDType == -1)//Zhujiang �������߿��������߿�һ��
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
			//���һ��
			DrawXLZXByXY(GJDInforArray[eGJD].BProjml, lc2
				, array, tarray, LineWide, linetype, layer, LRMFlag);

		}
		else //lc1~lc2��ͬһ���Ľ�������
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

//���ƽ������ߺ�Բ
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
		//if(!JDRx[i].ISShow)continue; 818 ���ж���
		//�潻������
		PZtmp.lc=QXB[i].KZH;
		xlpoint_pz(array,tarray,PZtmp);
		PtAy.append(AcGePoint2d(PZtmp.y,PZtmp.x));
		PtAy.append(AcGePoint2d(JDarray[i].E,JDarray[i].N));

		PZtmp.lc=QXB[i].KHZ ;
		xlpoint_pz(array,tarray,PZtmp);
		PtAy.append(AcGePoint2d(PZtmp.y,PZtmp.x));
		makeDX(PtAy,1.0*DrawParam.SCALE,-1,AcDb::kLnWtByLayer,layname);//zh-����-hz����
		PtAy.removeSubArray(0,PtAy.length()-1);

		//���㴦��Բ
		if (fabs(QXB[i].R)<0.001 && fabs(QXB[i].l1)<0.001 && fabs(QXB[i].l2)<0.001)//�۵㲻�꽻�㴦��Բ
			continue;
		Pt[X]=JDarray[i].E; 
		Pt[Y]=JDarray[i].N;
		Pt[Z]=0.0;
		AcDbCircle *pCir=new AcDbCircle(Pt,normal,1.0*DrawParam.SCALE);
		pCir->setLayer(layname);
		AddEntityToDbs(pCir);
	}


}


//���ܣ�ը����·����ƽ��ͼ
//��ע��ը��ʱ��ϵͳ����subErase()��������g_DocManageAry[iDoc].XLAry�����еĳ�Ա
Acad::ErrorStatus RHA::subExplode(AcDbVoidPtrArray& entitySet) const
{
	// ����·����
	//818 ������Ĭ�ϲ��� δ������ʱ��

	//����ͼ��
	DrawParam.CreateDrawLayer();

	//������·����
	if (DrawParam.m_IsRailLegend == 0)
		MakeXLZX(QXB[0].KZH, QXB[JDarray.size()-1].KHZ, CONTINUELINE, MAINLINE);
	else
		MakeXLZX(QXB[0].KZH, QXB[JDarray.size()-1].KHZ, RAILLEGEND, MAINLINE);


	//MakeXLZX(L"��ͨ��Ƣ���",sLCLx,eLCLx,CONTINUELINE,LEFTLINE);
	//MakeXLZX(L"��ͨ��ƻ���",sLC1x,eLC1x,CONTINUELINE,MAINLINE);//������·����


	//�潻�����ߺͽ��㴦��Բ  
	BZJdLineAndJdCir();

	//if(HasLeft && !HasRight)// 818 ��������������
	//	draw_zybz=1;
	//if(!HasLeft && HasRight)
	//	draw_zybz=-1;

	
	//��ע����Ҫ��
	if (DrawParam.m_JIAODIAN)
	{
		BZJD(L"���л���",MAINLINE);//818 ����
	}

	//��ע���ж���̣� ���ױ� ����
	if (DrawParam.m_LICHENG)
		MarkRXDLC();
		

	//��ע������
	if (DrawParam.m_IsNoteTZD)
		MarkTZD();

	//��ע������
	if (XLShuXing == JYJX || XLShuXing == JY2X)
		BZJYXFQD();

	//��
	if (DrawParam.m_IsNoteBMBR)
		MarkBridge();

	//��
	if (DrawParam.m_IsNoteTUNNEL)
		MarkTunnel();

	//վ
	if (DrawParam.m_IsNoteSTATION)
		MarkStation();

	//С�ź�
	if (DrawParam.m_IsNoteSMALLBR)
		plhINSBR(true);//����С�ź�
	else if(!DrawParam.m_IsNoteSMALLBR && DrawParam.m_IsNoteCROSSBR)
		plhINSBR(false);//��������

	//ˮ׼��
	if (DrawParam.m_IsNoteBMBR)
		MarkBM();

	//����
	if (DrawParam.m_IsNoteDAOXIAN)
		MarkDX();


	
	return Acad::eOk;
}

	//��ע����� �����߲���
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
	AcGePoint3d  p1, CenPt;//��ע����㣬��ҵ���������
	ACHAR ch[80],ch1[60],chrtmp[80];
	AcGePoint2d LBPt,RBPt,LTPt,RTPt;
	AcGePoint2dArray PtAr;
	int i,c,n;

	//AcGePoint3d Pt0;//�������в���������λ��
	//AcGePoint3d JDPt;//����
	//AcGePoint3d JDRotatePt;//����
	int sJD, eJD/*,c,n*/;
	p1[Z]=0.0;//Pt0[Z]=0.0;
	int GJDType = 2;
	bool isFQDJD=false;//�Ƿ�Ϊ�����ν���

	AcGePoint3d JCD;//���������־�Ľ����е�
	AcGePoint3d Pt1,Pt2,Pt3,Pt4;


	_tcscpy(layname, DrawParam.PingQuXianYaoSu);
	//��ͼ��
	if(LMRFlag==MAINLINE)
	{
	/*	if (XLName == L"���л���")
			_stprintf(layname,L"%s",DrawParam.PingQuXianYaoSu);*/
		/*else if (XLName == L"ʩ������")
			_stprintf(layname,L"%s",DrawParam.BXJiaoDian);
		else if (XLName == L"��ƻ���")
			_stprintf(layname,L"%s",DrawParam.GJJiaoDian);
		else
			return;*/
	}
	//else if(LMRFlag==LEFTLINE)
	//{
	//	if (XLName == L"��������")
	//		_stprintf(layname,L"%s",DrawParam.LeftJiaoDian);
	//	else if (XLName == L"��ƶ���")
	//		_stprintf(layname,L"%s",DrawParam.GJLeftJiaoDian);
	//	else
	//		return;
	//	if (DrawParam.SCALE>9.9)
	//		offset = DrawParam.m_ExOffset;
	//}
	//else 
	//{
	//	if (XLName == L"��������")
	//		_stprintf(layname,L"%s",DrawParam.RightJiaoDian);//���ߵ�ͼ����
	//	else if (XLName == L"��ƶ���")
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

	//��������Ҫ��
	//CalNoteQXYS(); 

	//int NJD = JDarray.size();
	int colo;
	bool isShow=false;
	double texHgt=DrawParam.OtherParam.MarkAry[0].TextHeight;
	dt=1.5*texHgt;//�����ִ��ļ��
	colo=DrawParam.OtherParam.MarkAry[0].Colo;

	int leftOrRight=DrawParam.OtherParam.MarkAry[0].LorR;
	//818 LorR =0 ����Ҫ�ر����ڲ� =-1 ��� =1 �Ҳ�

	de=xyddaa(JDarray[0].N,JDarray[0].E,JDarray[1].N,JDarray[1].E,&fb);
	///int zory=-1;//��ע 1������࣬-1�����Ҳ�
	for (i=1;i<NJD-1;i++)  
	{  
		//if( JDarray[i].JDXZ==BXPX && fabs(JDarray[i].XJJ)<0.0001 )
		//	isShow=false;//����ʾ 818 ʲôʱ����ʾ��
		//else if (JDarray[i].R<0.001 && JDarray[i].l1<0.001 && JDarray[i].l2<0.001)//�뾶����Ϊ0�Ľ���
		//	isShow=false;// ����ʾ
		//else
		//	isShow=true;

		//�ж�����Ҫ�ر�ע�Ƿ���ʾ-8.13-zy
		//1.������ȫ����ʾ
		isShow = true;
		//2.������ж��Ƿ�Ϊ���ö�
		if (XLShuXing == L"��ƻ���" || XLShuXing == L"��ƶ���")
		{
			//2.1 ���ݸĽ��λ��ܱ����������ж�
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
		{ //��ƫ
			c=1; dd=2*texHgt; 
		}   
		else
		{ //��ƫ 
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
			IdAr.append(maketext(p1,JDH,da,texHgt ,-1,1,L"����",layname));//�����

		if(qxbbz == 1)//��ƽ�������߶���
		{
			// �����߲��� begin
			double R,T,T1,T2,L1,L2,LL,NYDL;
			R=fabs(JDarray[i].R); 
			L1= JDarray[i].l1;  
			L2=JDarray[i].l2; 
			T1=QXB[i].T1; 
			T2=QXB[i].T2;  
			LL=QXB[i].L;

			//���н�������߲�����ô�� 818
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
				//�������
				c=-1;
				dt=c*texHgt*1.5;
				//if (c==-1) 
					dd=dd+8*texHgt;
			}
			else 
			{  
				//�����Ҳ�;
				c=1;
				dt=c*texHgt*1.5; 
				/*dd=-fabs(dd);*/
				
			}

			p1[X]=JDarray[i].E-dd*sin(de-c*pi*0.5) ; 
			p1[Y]=JDarray[i].N-dd*cos(de-c*pi*0.5);
			_tcscpy(chrtmp,L"  ");
			_tcscat(chrtmp,JDH);
			//  �����
			if(isShow)
				IdAr.append(maketext(p1,JDH,da,texHgt ,-1,1,L"����",layname));//�����
				//G_maketext(pWd,p1,chrtmp,da,texHgt ,QXYSColor,1);   // �����  
			//  ƫ��
			if (DrawParam.m_IsNoteJDAng)
			{
				D_msToDmsStr(QXB[i].a, DrawParam.NPJ, ch);

				if (aa>0) { _tcscpy(ZhuanJiao,L"ay--"); }
				else _tcscpy(ZhuanJiao,L"az--"); 
				_tcscat(ZhuanJiao,ch);

				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if(isShow)
					IdAr.append(maketext(p1,ZhuanJiao,da,texHgt ,-1,1,L"����",layname));//ת��
			}
			//   �뾶
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
					IdAr.append(maketext(p1,Radius,da,texHgt ,-1,1,L"����",layname));//�뾶
			}


			isFQDJD = false;
			if (XLShuXing == JYJX || XLShuXing == JY2X)
			{
				if(IsPtOnLYD(QXB[i].KZH) && IsPtOnLYD(QXB[i].KHZ))
					isFQDJD = false;
				else
					isFQDJD = true;
			}
				
			
				

			//���Ʒ��������ע�ġ�����
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
				//  ����
				if (L1>0 || L2>0)//  ������Ϊ��
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
							IdAr.append(maketext(p1,HuanChang,da,texHgt ,-1,1,L"����",layname));//����

					} 
					else
					{
						//   ���Ȼ���
						_tcscpy(HuanChang,L"L1--");  _tcscat(HuanChang,ch);
						if (fabs(L2-(long int)(L2))<0.001) 
						{ ads_rtos(L2,2,DrawParam.NLC,ch); }
						else 
						{ ads_rtos(L2,2,DrawParam.NLC,ch); };

						_tcscat(HuanChang,L"  L2--"); _tcscat(HuanChang,ch);
						if(isShow)
							IdAr.append(maketext(p1,HuanChang,da,texHgt ,-1,1,L"����",layname));//����

					};
				}

				//  �г�
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
						IdAr.append(maketext(p1,QieChang,da,texHgt ,-1,1,L"����",layname));//�г� 

				} 
				else
				{   
					_tcscpy(QieChang,L"T1--");  _tcscat(QieChang,ch);
					ads_rtos(T2,2,DrawParam.NLC,ch);
					_tcscat(QieChang,L"  T2--"); _tcscat(QieChang,ch);
					if(isShow)
						IdAr.append(maketext(p1,QieChang,da,texHgt ,-1,1,L"����",layname));//�г� 

				}

				//  ���߳�
				LL=QXB[i].L;
				ads_rtos(LL,2,DrawParam.NLC,ch);
				_tcscpy(QuXianChang,L"L--");  _tcscat(QuXianChang,ch);
				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if(isShow)
					IdAr.append(maketext(p1,QuXianChang,da,texHgt ,-1,1,L"����",layname));//���߳� 
				
				f0=de-c*pi*0.5+pi;//�ڽ�ƽ�������������ڲ෽��

				//��ע��ҵ����
				if(JDarray[i].JDXZ == BXPX || JDarray[i].JDXZ == S_Start || JDarray[i].JDXZ == S_End//���н���/S��
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
						IdAr.append(maketext(CenPt,NeiYeDuanLian,da,texHgt ,-1,3,L"����",layname));//��ҵ���� 

					}
					
				}


				
			};
			
		};
		fb=fe; 
	};   

	//818 ���ߡ���ת ���� ���ߡ���ת: �Ƶ��������

	////�����ҳ�ͼ����ת������ ���ҵ����ͼ����ת: ��ͷ������



}

////////////////////////////////////////////////////////////////////////
//������ߵ����ߵ����������ҵ����
//���룺  iJD  �ý����±�
//����ֵ����ҵ����ֵ��û����ҵ��������0.0
//��д��  ��˧ 2016.10.11
////////////////////////////////////////////////////////////////////////
double  RHA::CalCurvNYDL(int iJD) const
{
	/*JDarray[].NYDL;*/

	if(iJD<0 || iJD+1>JDarray.size())
		return 0.0;

	double ZHprojml,HZprojml;//�����ϵ�ZH��HZ�������
	double ZHdml,HZdml;//�����ϵ�ZH��HZ�ֳ����
	double len1,len2;
	ACHAR GH[20];

	ZHprojml = QXB[iJD].KZH ;
	HZprojml = QXB[iJD].KHZ;

	TrsProjmltoCkml(ZHprojml, 3, GH, ZHdml);
	TrsProjmltoCkml(HZprojml, 3, GH, HZdml);

	len1 = HZdml - ZHdml;
	len2 = HZprojml - ZHprojml;

	//4.��ҵ����
	if(fabs(len2-len1)>0.0000005)//����ҵ����
		return len2 - len1 + 100.0;
	else
		return 0.0;
	
}


////////////////////////////////////////////////////////////////////////
//���ɹ�ͨ��ƶ�������л������ö�
//��д����˧ 2016.9.27
//��ע����վ���������Ҳ�ṩ������������޸�ʱ֪ͨ�׹� 818
////////////////////////////////////////////////////////////////////////
void RHA::CreatLYDArrayOnJYX(RHA* pJYX,/*double spt[2], double ept[2], */vector<GJDInf>& LYDInfArray)
{
	int JYJDStart,JYJDEnd;
	bool IsQJZXCD,IsHJZXCD;//ǰ���ֱ���Ƿ��ص�
	int iQGJJD,iQJYJD;//ǰ��ֱ���ص��ļ��к͸Ľ������±�
	int iHGJJD,iHJYJD;//���ֱ���ص��ļ��к͸Ľ������±�
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

	//�����ߵ����ս����
	JYJDStart=0;
	JYJDEnd=pJYX->JDarray.size()-1;

	//��ü��������յ�����
	spt[0] = pJYX->JDarray[0].N;
	spt[1] = pJYX->JDarray[0].E;
	ept[0] = pJYX->JDarray[JYJDEnd].N;
	ept[1] = pJYX->JDarray[JYJDEnd].E;

	//���ö���Ϣ����ֵ
	_tcscpy(oneGjdInfo.XLName, m_XLName);
	oneGjdInfo.GJDType = -1;
	_tcscpy(oneGjdInfo.gjdname, L"���ö�");

	bool isFindLYD=false;//�Ƿ������ö�
	int sJYJD=-1, eJYJD=-1;//�������������ص������ս���
	int sSJJD=-1, eSJJD=-1;//����������öε����ս���
	double sProjml1x, eProjml1x;
	double sProjml2x, eProjml2x;
	double xjj=0.0;
	double distZDToJZXQD_GJ=0.0, distZDToJZXQD_JY=0.0;
	double distQDToJZXZD_GJ=0.0, distQDToJZXZD_JY=0.0;
	int sJD=0, eJD=0; 
	int sDL=-1, eDL=-1;

	typedef struct
	{
		int sJD, eJD;//���ս���
		int sDL, eDL;//���ն���
	}BXSec;
	LYDInfArray.clear();
	//1. �γɲ��ж�����
	vector<BXSec> BXSecArray;//����/�������ս������飬ż��Ϊ��㣬����Ϊ�յ�
	BXSec OneBX;
	OneBX.sJD = 1;//��һ�����жε����
	OneBX.sDL = 0;//��һ�����жε���ʼ����
	int RXDNum = GetRXDNum();
	for (int iRXD=0; iRXD<RXDNum; iRXD++)
	{
		GetRxdSEJD(iRXD, sJD, eJD);//��ȡ�������ս���
		GetiRXDDLinfo(iRXD, sDL, eDL);//��ȡ�������ն���
		OneBX.eJD = sJD;//���ж��յ������ж����
		OneBX.eDL = sDL;//���ж��յ���������ж�������
		BXSecArray.push_back(OneBX);//�������ǰһ�������ǲ��ж���ֹ����

		OneBX.sJD = eJD+1;//��һ�����жε�����ǵ�ǰ���жε��յ�
		OneBX.sDL = eDL;//��һ�����жε���ʼ�����ǵ�ǰ���жε��յ����
	}

	OneBX.eJD = NJD-1;//���һ�����жε��յ�
	OneBX.eDL = DLarray.size()-1;//����жε��յ����
	BXSecArray.push_back(OneBX);

	//2.���ж�ѭ���������ö�
	for (int iBXD=0; iBXD<BXSecArray.size(); iBXD++)
	{
		sJD = BXSecArray[iBXD].sJD;
		eJD = BXSecArray[iBXD].eJD;
		sDL = BXSecArray[iBXD].sDL;
		eDL = BXSecArray[iBXD].eDL;
		if(eJD>=sJD)//ȫ�����е���������������޳�
		{
			//�����ս��㷶Χ��Ѱ�����ö�
			//�������2�������ǰ��ֱ���ص��ļ����߽���
			IsQJZXCD = IsHJZXCD = false;
			iQJYJD=0;
			IsQJZXCD = SearchJZXCDJDIndex_OnJYJX(sJD, -1, pJYX, JYJDStart, JYJDEnd, iQJYJD);
			
			isFindLYD=false;
			for (int j=sJD; j <= eJD; j++)
			{
				//������iGJJDǰ��ֱ���ص��ļ����߽���
				iHJYJD=0;
				if (j<eJD)
					IsHJZXCD = SearchJZXCDJDIndex_OnJYJX(j, 1, pJYX, JYJDStart, JYJDEnd, iHJYJD);
				else
					IsHJZXCD = false;

				if (IsQJZXCD && IsHJZXCD && iQJYJD == iHJYJD
					&& fabs(pJYX->JDarray[iQJYJD].R - JDarray[j].R)<0.001
					&& fabs(pJYX->JDarray[iQJYJD].l1 - JDarray[j].l1)<0.001
					&& fabs(pJYX->JDarray[iQJYJD].l2 - JDarray[j].l2)<0.001)
				{//�����ص�
					if (!isFindLYD)
					{//��ʼ�������öΣ������ö���㽻�㸳ֵ
						isFindLYD=true;
						sJYJD=iQJYJD;
						sSJJD=j;
					}

				}
				else
				{//������߲��ص��������Ѿ��ѵ��˸Ľ����յ�
					if (IsQJZXCD)//ǰ��ֱ���������ò���
					{
						eJYJD=iQJYJD; 
						eSJJD=j;

						if (!isFindLYD)
						{
							//ֻ�иü�ֱ�߱�����
							sJYJD=iQJYJD;
							sSJJD=j;
						}

						//���ɸĽ���

						//���
						distQDToJZXZD_GJ=sqrt(pow(JZXArray[sSJJD].QJZXStartPt.x-JZXArray[eSJJD].QJZXEndPt.x, 2)
							+pow(JZXArray[sSJJD].QJZXStartPt.y-JZXArray[eSJJD].QJZXEndPt.y, 2));
						distQDToJZXZD_JY=sqrt(pow(pJYX->JZXArray[sJYJD].QJZXStartPt.x-JZXArray[eSJJD].QJZXEndPt.x, 2)
							+pow(pJYX->JZXArray[sJYJD].QJZXStartPt.y-JZXArray[eSJJD].QJZXEndPt.y, 2));


						if (distQDToJZXZD_GJ>distQDToJZXZD_JY)//����·��ֱ�߱Ȳ����ߵļ�ֱ�߳�
						{//ȡ�����߼�ֱ�����

							//����ǰ��ֱ���ص��ε������������ϵ�ͶӰ���sProjml2x
							TrsNEtoProjml(pJYX->JZXArray[sJYJD].QJZXStartPt.x
								, pJYX->JZXArray[sJYJD].QJZXStartPt.y, sProjml2x);

							//��������ǰһ������ǰ����ôȡ�ö��������
							if (sProjml2x+0.001<DLarray[sDL].TYLC)
							{
								sProjml2x=DLarray[sDL].TYLC;
								//����ǰ��ֱ���ص��ε�����ڼ�������ͶӰsProjml1x
								CalXJJ(sProjml2x, array, tarray, pJYX->array, pJYX->tarray, sProjml1x, false);
							}
							else
								sProjml1x=pJYX->JZXArray[sJYJD].QJZXStartPt.lc;
						}
						else
						{//ȡ�Ľ��߼�ֱ�����

							//ǰ��ֱ���ص��ε������������ϵ�ͶӰ���
							sProjml2x=JZXArray[sSJJD].QJZXStartPt.lc;
							//��������ǰһ������ǰ����ôȡ�ö��������
							if (sProjml2x+0.001<DLarray[sDL].TYLC)
							{
								sProjml2x=DLarray[sDL].TYLC;
								//����ǰ��ֱ���ص��ε�����ڼ�������ͶӰsProjml1x
								CalXJJ(sProjml2x, array, tarray, pJYX->array, pJYX->tarray, sProjml1x, false);
							}
							else//����ǰ��ֱ���ص��ε�����ڼ�������ͶӰsProjml1x
								pJYX->TrsNEtoProjml(JZXArray[sSJJD].QJZXStartPt.x, JZXArray[sSJJD].QJZXStartPt.y, sProjml1x);

							////nan
							//ACHAR mes[256];
							//_stprintf(mes,L"������%lf,%lf\n", pGTZXJY->JZXArray[sJYJD].QJZXStartPt.y, pGTZXJY->JZXArray[sJYJD].QJZXStartPt.x);
							//ads_printf(mes);
							//_stprintf(mes,L"�����%lf,%lf\n", OneGjdInfo.spt[1], OneGjdInfo.spt[0]);
							//ads_printf(mes);
						}

						//��λ���ö�������

						if (fabs(sProjml1x-pJYX->QXB[0].KZH)>0.1)
						{
							//���λ���ױ�
							sProjml1x = pJYX->LocateHundredMeterOnLine(sProjml1x, false, ckml);
							//�������������ͶӰ�������ꡢ���
							CalXJJ(sProjml1x, pJYX->array, pJYX->tarray
								, array, tarray, xjj, sProjml2x, oneGjdInfo.spt, true);//818 ����false 

							oneGjdInfo.BProjml=sProjml2x;
							_tcscpy(oneGjdInfo.BLC, ckml);
						}
						else
						{
							//��·��㿪ʼ����
							oneGjdInfo.BProjml=QXB[0].KZH;
							_tcscpy(oneGjdInfo.BLC, LCchr(DLarray[0].EGH, DLarray[0].ELC, 3));
							oneGjdInfo.spt[0]=spt[0];
							oneGjdInfo.spt[1]=spt[1];
						}

						//�յ�

						distZDToJZXQD_GJ=sqrt(pow(JZXArray[eSJJD].QJZXEndPt.x-JZXArray[eSJJD].QJZXStartPt.x, 2)
							+pow(JZXArray[eSJJD].QJZXEndPt.y-JZXArray[eSJJD].QJZXStartPt.y, 2));

						distZDToJZXQD_JY=sqrt(pow(pJYX->JZXArray[eJYJD].QJZXEndPt.x-JZXArray[eSJJD].QJZXStartPt.x, 2)
							+pow(pJYX->JZXArray[eJYJD].QJZXEndPt.y-JZXArray[eSJJD].QJZXStartPt.y, 2));

						if (distZDToJZXQD_GJ>distZDToJZXQD_JY)
						{//ȡ�����߼�ֱ���յ�

							//�����ֱ���ص��ε��յ���������ϵ�ͶӰ���eProjml2x
							TrsNEtoProjml(pJYX->JZXArray[eJYJD].QJZXEndPt.x
								, pJYX->JZXArray[eJYJD].QJZXEndPt.y, eProjml2x);

							//����յ��ں�һ����������ôȡ�ö������յ�
							if (eProjml2x>DLarray[eDL].TYLC+0.001)
							{
								eProjml2x=DLarray[eDL].TYLC;
								//�����ֱ���ص��ε��յ��ڼ�������ͶӰeProjml1x
								CalXJJ(eProjml2x, array, tarray, pJYX->array, pJYX->tarray, eProjml1x, false);
							}
							else//������ֱ���յ��ڼ�������ͶӰeProjml1x
								eProjml1x = pJYX->JZXArray[eJYJD].QJZXEndPt.lc;
							//nan
							//ACHAR mes[256];
							//_stprintf(mes,L"�����%lf,%lf\n", JZXArray[eSJJD].QJZXEndPt.y, JZXArray[eSJJD].QJZXEndPt.x);
							//ads_printf(mes);
							//_stprintf(mes,L"������%lf,%lf\n", OneGjdInfo.ept[1], OneGjdInfo.ept[0]);
							//ads_printf(mes);
						}
						else
						{//ȡ�Ľ��߼�ֱ���յ�
							eProjml2x=JZXArray[eSJJD].QJZXEndPt.lc;

							if (eProjml2x>DLarray[eDL].TYLC+0.001)
							{
								eProjml2x=DLarray[eDL].TYLC;
								//�����ֱ���ص��ε��յ��ڼ�������ͶӰeProjml1x
								CalXJJ(eProjml2x, array, tarray, pJYX->array, pJYX->tarray, eProjml1x, false);
							}
							else//������ֱ���յ��ڼ�������ͶӰeProjml1x
								pJYX->TrsNEtoProjml(JZXArray[eSJJD].QJZXEndPt.x
								, JZXArray[eSJJD].QJZXEndPt.y, eProjml1x);
						}

						//��λ���ö��յ����
						if (fabs(eProjml1x-pJYX->QXB[JYJDEnd].KHZ)>0.1)
						{
							//��ǰ��λ���ױ�
							eProjml1x = pJYX->LocateHundredMeterOnLine(eProjml1x,true,ckml);
							//�������������ͶӰ�����ꡢ���
							CalXJJ(eProjml1x, pJYX->array, pJYX->tarray
								, array, tarray, xjj, eProjml2x, oneGjdInfo.ept, true);//818 ����false 

							oneGjdInfo.EProjml=eProjml2x;
							_tcscpy(oneGjdInfo.ELC, ckml);
						}
						else
						{
							//����·�յ�����
							oneGjdInfo.EProjml=QXB[NJD-1].KHZ;
							_tcscpy(oneGjdInfo.ELC, LCchr(DLarray[dlNum-1].BGH, DLarray[dlNum-1].BLC, 3));
							oneGjdInfo.ept[0]=ept[0];
							oneGjdInfo.ept[1]=ept[1];
						}

						if (oneGjdInfo.EProjml<oneGjdInfo.BProjml+0.1)
							continue;


						//������ö�����
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
	

	//�ϲ��Ľ�������
	MergeGTZX2GJDArray(LYDInfArray); 

}

//����ͳһ��̣��Ƿ���ǰ�����������븽��������ͳһ��̵Ĳ�ֵ��tylc>������lc...+
double RHA::DistToNearDL(double projml, const vector<DLTab> &DLAry, bool IsForwardTo) const//
{
	double LCB=0.0;//������ͳһ���
	int iDL=0;
	double iXLlength=0.0;//ĳ�γ���
	double cml;

	cml=TrsProjmltoCml(projml);
	
	LCB=0.0;
	for(iDL=1;iDL<DLAry.size();iDL++)
	{
		if(cml <= LCB)
			break;
		iXLlength=DLAry[iDL].BLC-DLAry[iDL-1].ELC;//ĳ�γ���
		LCB += iXLlength;//������ͳһ���
	}

	if(IsForwardTo)//��ǰ����
		LCB-=iXLlength;

	return fabs(cml-LCB);	
}

/*===================================================================
������:FindNearDL   
------------------------------------------------------------
��������:Ѱ��ĳһͶӰ��̸����Ķ����±�,dirctn=true,��ǰ�Ҷ�����=false�������
------------------------------------------------------------
����:    
projml           ͶӰ���
DLAry            ����
dirctn           =true,��ǰ�Ҷ�����=false�������
------------------------------------------------------------
���:  �ҵ�projml�Ķ������䣬����ǰ/��һ���������±꣬���򷵻�-1
------------------------------------------------------------
��д: ��˧ 2016.4.13  
=====================================================================*/
int RHA::FindNearDL(double projml, const vector<DLTab> &DLAry, bool dirctn) const
{
	int DLNum=DLAry.size();//818 ������Ǵ������ֵprojml��
	if(DLNum==0)
		return -1;

	if (dirctn)//��ǰ�� 
	{
		for (int i=1; i<DLNum; i++)
		{
			if ((projml < DLAry[i].TYLC-0.001)
				&& (projml > DLAry[i-1].TYLC-0.001))
				return --i;
		}

		if (projml>DLAry[DLNum - 1].TYLC-0.001 /*&& projml<QXB[JDarray.size()-1].KZH+0.001*/)//818 ����С��QXB[JDarray.size()-1].KZH
			return --DLNum;//����û��ȫ�����ɣ�����̵�ǰһ�����Ƕ����������һ��	

		return -1;
	}
	else//�����
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
��������: Ѱ��ĳͳһ��̵������Ķ����ľ��루dirctn=true,��ǰ�Ҷ�����=false������ң�
�������:
		cml         ͳһ���
		tmpDLB  	����
		dirctn      =true,��ǰ�Ҷ�����=false�������  
����ֵ: �ҵ�cml�Ķ������䣬����ǰ/��һ���������±꣬���򷵻�-1
���ã��������ʱ����
��д����˧ 2017.2
***************************************************************/
int RHA::FindNearDLbyCml(double cml, const vector<DLTab> &tmpDLB, bool dirctn) const
{
	double LCB=0;  // LCB ������ͳһ���
	int iDL=0;
	int NDL=tmpDLB.size();
	int dlIndx=-1;

	for (int iDL=0;iDL<tmpDLB.size();iDL++)
	{
		//�ö�����ͳһ���
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
			else if (fabs(cml-LCB)<0.00005)//818 cml�ڶ��������᲻�������⣿
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

//Ѱ�Ҿ���ĳ����������һ������(�����Ӳ������ϸ��ƹ����Ķ���)
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

//Ѱ��ĳ������̵�ǰһ��������
int RHA::GetPreDL(double projml) const//818 ��DistToNearDL
{

	for(int iDL=0; iDL<DLarray.size(); iDL++)
	{
		if(projml+0.001<DLarray[iDL].TYLC)
			return --iDL;
	}

	return -1;
}

/*===================================================================
��������:�ڼ������ϲ��� �ͱ���·ĳ����ļ�ֱ���ص� �ļ����߼�ֱ�߶�Ӧ�Ľ����±�
------------------------------------------------------------
����:
iJD2                   ����·�Ľ����
isQorH                 = -1���жϱ���·�Ľ����ǰ��ֱ���Ƿ��ص�
					   =  1���ж���·�Ľ���ĺ��ֱ���Ƿ��ص�
pJYX                   ������·ƽ��ָ��
sJD_JY                 ����ʱ�ļ�������ʼ�����
eJD_JY                 ����ʱ�ļ�������ֹ�����
------------------------------------------------------------
���:
iOverLapJD             ����������֮�ص��Ľ����±�
------------------------------------------------------------
����ֵ:����ڼ��������ҵ��ص��ļ�ֱ�߷���true����֮����false
------------------------------------------------------------
��д: 
=====================================================================*/
bool RHA::SearchJZXCDJDIndex_OnJYJX(int iJD2,int isQorH,RHA* pJYX,int sJD_JY, int eJD_JY,int &iOverLapJD)
{
	bool isCD = false;//��ֱ���Ƿ��ص�
	double SPt1X, SPt1Y, EPt1X, EPt1Y;//һ�����ϼ�ֱ�����յ�����
	double SPt2X, SPt2Y, EPt2X, EPt2Y;//�������ϼ�ֱ�����յ�����
	int NJD=JDarray.size();
	double dist1;

	iOverLapJD=0;

	if (isQorH == -1)
	{//�ж�ǰ��ֱ���Ƿ��ص�
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
			//�ж�������ߵĸü�ֱ���Ƿ��ص�
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
		//�жϺ��ֱ���Ƿ��ص�
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
	/*�����(x,y)��GTZX���ľ��룬�������С��1e-6����0.0*/
	dx = JDarray[0].N - x;
	dy = JDarray[0].E - y;
	dist = sqrt(dx*dx+dy*dy);
	if (dist<1e-3)
		return 1e-12;
	/*�����(x,y)��GTZX�յ�ľ��룬�������С��1e-6����0.0*/
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

//�ϲ��Ľ�������
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
			//ǰ�����Ľ��ηǳ��ӽ�
			if (AllGJDArray[iGJD].GJDType == AllGJDArray[iGJD+1].GJDType)
			{
				//�Ľ��ηǳ��ӽ��ҸĽ�������ͬʱ�����Ľ��κϲ�
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
			//ǰ��Ľ��γ��ֽ���ϲ����Ľ���
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
							//�����㶼�ڲ��еļ�ֱ�߱��ϣ������иĽ����յ�Ϊ�����������иĽ���/���öε�
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
						else//�ϲ�Ϊ���иĽ�
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
						ads_printf(L"�Ľ��ε�������·�����ڣ�\n");
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
�������ܣ�������ֱ������
�Խ���Ϊ��λ�����ý���ǰ���ֱ�ߵ����յ㣬�������Ϊһ����ֱ�߽ṹ�����ֱ��������
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
�������ܣ��������иĽ�������
����:
spt[2]		�������иĽ��ε����
ept[2]		�������иĽ��ε��յ�
�����
RXGJDArray	���иĽ�������
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
		//��Ѱĳ���жε����ն������
		GetiRXDDLinfo(i , sDLno , eDLno, DLarray);
		if(max(DLarray[sDLno].TYLC,sProjml) < min(DLarray[eDLno].TYLC,eProjml))//���ж����ѯ��Χ�н���
		{
			//���
			PZ.lc = DLarray[sDLno].TYLC > sProjml ? DLarray[sDLno].TYLC : sProjml;
			xlpoint_pz(array, tarray, PZ);
			OneGjdInfo.spt[0] = PZ.x;
			OneGjdInfo.spt[1] = PZ.y;
			OneGjdInfo.BProjml=PZ.lc;

			//�Ľ�����
			if (fabs(PZ.lc-DLarray[sDLno].TYLC)<0.1)
				_tcscpy(OneGjdInfo.gjdname, DLarray[sDLno].RXDName);
			else
				SetDefaultGjdName(DLarray[sDLno].BLName, OneGjdInfo.spt[0]
						, OneGjdInfo.spt[1], 2, OneGjdInfo.gjdname);

			//�յ�
			PZ.lc = DLarray[eDLno].TYLC < eProjml ? DLarray[eDLno].TYLC : eProjml;
			xlpoint_pz(array,tarray,PZ);
			OneGjdInfo.ept[0] = PZ.x;
			OneGjdInfo.ept[1] = PZ.y;
			OneGjdInfo.EProjml=PZ.lc;

			//��������
			_tcscpy(OneGjdInfo.BXLName,DLarray[sDLno].BLName);
			_tcscpy(OneGjdInfo.EXLName, DLarray[eDLno].BLName);

			//�ֳ����
			_tcscpy(OneGjdInfo.BLC, LCchr(DLarray[sDLno].EGH, DLarray[sDLno].ELC, 3));
			_tcscpy(OneGjdInfo.ELC, LCchr(DLarray[eDLno].BGH, DLarray[eDLno].BLC, 3));			

			RXGJDArray.push_back(OneGjdInfo);
		}
	}
}

/*********************************************************************************
�������ܣ�����Ĭ�ϸĽ�����
����:
blName     ������·��
N, E       γ�࣬����
GJDType    �Ľ�������
�����
GjdName    �Ľ�����
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
		_stprintf(mes, L"���ɸĽ�����ʱ����������%s�������\n", blName);
		ads_printf(mes);
		return;
		
	}

	//818 g_DocManageAry[m_iDoc].XLAry[ix]����Ƿ�Ϊ�գ���ֹ��һ��

	pRHA_BL->TrsNEtoCkml(N, E, ckml);
	split_ckml(ckml, dml, gh);
	if (GJDType==0)
		_stprintf(GjdName,L"%0.1lf���иĽ�",dml/1000.0);
	else if (GJDType==2)
		_stprintf(GjdName,L"%0.1lf���иĽ�",dml/1000.0);
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
�������ܣ�������ͨ��ƻ��߸Ľ�������
���������
spt[2]		�����Ľ��ε��������
ept[2]		�����Ľ��ε��յ�����
AllGJDArray	��������Χ�ڵĸĽ�������
LYDArray	��������Χ�ڵ����ö�����
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

	//1 �������жθĽ���
	CreateRXGJD(RXGJDArray);

	//nan 
	/*vector<GJDInf> tmp;
	tmp.clear();
	for (int i=0; i<RXGJDArray.GetSize(); i++)
	{
		tmp.push_back(RXGJDArray[i]);
	}*/


	//2 �������ö�
	int j;
	double cml;
	double LYDSml,LYDEml;
	GJDInf TmpGJD;
	xlpoint PZ;
	XLDataBase xlmdb;
	vector<GJDInf> tmpLYDArray;
	CArray<XLInforTableStruct, XLInforTableStruct> xlInforAry;//��·��Ϣ 
	//2.1 �������еļ�����·

	xlmdb.ReadXLInfTable(m_mdbName, JYJX, xlInforAry);
	xlmdb.ReadXLInfTable(m_mdbName, JY2X, xlInforAry);

	//�������м�����
	for (int i=0; i<xlInforAry.GetSize(); i++)
	{
		CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, xlInforAry[i].XLName, false);
	}

	//2.2 �������ö�
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

	//2.3 ����ǰ���в��������ߵ�����������ö�����
	//     �������ǲ��жζ���  818 ����δ���ǣ�

	
	
	//2.4 ����
	for (int j=0; j<RXGJDArray.size(); j++)
		AllGJDArray.push_back(RXGJDArray[j]);

	for (int j=0; j<LYDArray.size(); j++)
		AllGJDArray.push_back(LYDArray[j]);

	SortSections(AllGJDArray);//818 merge

	//3 ���иĽ���
	CreatBXGJD(AllGJDArray, BXGJDArray);
	for (int j=0; j<BXGJDArray.size(); j++)
		AllGJDArray.push_back(BXGJDArray[j]);

	SortSections(AllGJDArray);//818 merge
	
	MergeGTZX2GJDArray(AllGJDArray);

}

/**********************************************************************************
�������ܣ���ȡ��ͨ��ƻ�����GJD_Spt��GJD_Ept�ĸĽ�����Ϣ����
���������
GJD_Spt[2]		�����Ľ��ε��������
GJD_Ept[2]		�����Ľ��ε��յ�����
tmpBXDArray		�÷�Χ�ڵĸĽ�������
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

	//���û�����öκ����иĽ��Σ�ȫ�߲��иĽ�
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

	//�������㿪ʼ���иĽ������ö����ж�����ͷ��һ��ֻ������㳤��Ϊ0�ĸĽ���
	
	//�����һ���Ľ��ε����ͶӰ���
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


	//����յ�֮ǰ�ǲ��иĽ������ö����ж�����β��һ��ֻ�����յ㳤��Ϊ0�ĸĽ���

	//�������һ���Ľ��ε��յ�ͶӰ���
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

	//�������жμ��ڲ岢�иĽ���
	LYDRXDNum=tmpLYDRXDArray.size();
	int iDL=0;
	int sDL=0, eDL=0;

	for (int i=0; i<LYDRXDNum-1; i++)
	{
		if(fabs(tmpLYDRXDArray[i].EProjml-tmpLYDRXDArray[i+1].BProjml)>0.1)
		{

			//�����Ϣ
			tmpGJD.spt[0]=tmpLYDRXDArray[i].ept[0];
			tmpGJD.spt[1]=tmpLYDRXDArray[i].ept[1];
			tmpGJD.BProjml=tmpLYDRXDArray[i].EProjml;
			_tcscpy(tmpGJD.BXLName, tmpLYDRXDArray[i].EXLName);
			_tcscpy(tmpGJD.BLC,  tmpLYDRXDArray[i].ELC);
			//�ж϶����������Ƿ��ڶ�����
			iDL=JudgePtIsOnDL(tmpGJD.BProjml);
			if (iDL >= 0)
			{
				_tcscpy(tmpGJD.BLC, LCchr(DLarray[iDL].EGH, DLarray[iDL].ELC));
				_tcscpy(tmpGJD.BXLName, DLarray[iDL].BLName);
				_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);
			}

			////���Ҹò��иĽ��������Ƿ��в��ж�����ǰ��ֱ���������ߣ�
			//sDL=FindNearDL(tmpLYDRXDArray[i].EProjml, DLarray, true);
			//eDL=FindNearDL(tmpLYDRXDArray[i+1].BProjml, DLarray, true);

			//if(sDL<eDL)
			//{
			//	for (int j=sDL+1; j<eDL+1; j++)//818 δ����
			//	{
			//		if (DLarray[j].Indx<0)
			//		{
			//			//�յ���Ϣ
			//			_tcscpy(tmpGJD.ELC, LCchr(DLarray[j].BGH, DLarray[j].BLC));
			//			tmpGJD.EProjml=DLarray[j].TYLC;
			//			TrsProjmltoNE(tmpGJD.EProjml, tmpGJD.ept[0], tmpGJD.ept[1]);
			//			_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);

			//			SetDefaultGjdName(tmpGJD.BXLName, tmpGJD.spt[0], tmpGJD.spt[1], 0, tmpGJD.gjdname);
			//			tmpBXDArray.Add(tmpGJD);

			//			//��һ�������Ϣ
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

				//818 Ѱ��ǰһ�����±꣬������������
				_tcscpy(tmpGJD.EXLName, tmpGJD.BXLName);
				_tcscpy(tmpGJD.ELC, tmpLYDRXDArray[i+1].BLC);

				//�ж϶����յ�����Ƿ��ڶ�����
				iDL=JudgePtIsOnDL(tmpGJD.EProjml);
				if (iDL >= 0)
					_tcscpy(tmpGJD.ELC, LCchr(DLarray[iDL].BGH, DLarray[iDL].BLC));

				SetDefaultGjdName(tmpGJD.BXLName, tmpGJD.spt[0], tmpGJD.spt[1], 0, tmpGJD.gjdname);
				tmpBXDArray.push_back(tmpGJD);
			}
			
		}
	}

	//ɾ��֮ǰ���ӵ����յ�Ľ���
	if(isAddQdGjd)
		tmpLYDRXDArray.erase(tmpLYDRXDArray.begin());
	if (isAddZdGjd)
		tmpLYDRXDArray.pop_back();
}

//����λ
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

//�潻�����ߺͽ���Բ
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
	// �潻������
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
		pWd->subEntityTraits().setColor(GTZXJDToJDColor);	//��ʱ
		pWd->geometry().polyline(tarray_pxy, verts);
	} 
	else
	{
		assert(Adesk::kFalse);
	}

	// �潻�㴦Բ
	for (i = 1; i < tarray_pxy-1; i++) //plh05.07
	{
		if(JDarray[i].JDXZ != BXPX && JDarray[i].JDXZ != BX_NotPX)
		{
			Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
			pWd->subEntityTraits().setColor(QXYSColor);
			pWd->geometry().circle(Pt,3.6,normal);
		}

		/////////////// 818 �ſ� 
		/*if(!JDarray[i].IsShow)
		continue;
		Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
		mode->subEntityTraits().setColor(4);
		mode->geometry().circle(Pt,3.6,normal);*/
	}

}

//����һ�������α�ע �򡰡���
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
���Ʒ����α�ע������
StartLC		��������ʼ���
EndLC		��������ֹ���
JianJu		ÿ�����پ������һ��������
LMRFlag		��·���ͱ��
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

//��ȡ�Ľ��λ��ܱ��γɼ��л��ߡ����ж��ߵ����ö����� GJDINFARRAY
void RHA::CreatLYDArrayOfJYX() 
{
	XLDataBase xlmdb;
	CString tmpStr;
	//xlpoint PZ;	

	GJDInforArray.clear();
	tmpStr=L"�Ľ�����=\"���ö�\"";
	if(xlmdb.Read_XLDbs(m_mdbName,L"�Ľ��λ��ܱ�",tmpStr))	
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

	//��ͶӰ���
	int LYDNum=GJDInforArray.size();
	for (int i=0 ; i<LYDNum ; i++)
	{
		GJDInforArray[i].BProjml=PROJ_ML(array, tarray, GJDInforArray[i].spt[0], GJDInforArray[i].spt[1]
		, GJDInforArray[i].spt[0], GJDInforArray[i].spt[1]) ;
		GJDInforArray[i].EProjml=PROJ_ML(array, tarray, GJDInforArray[i].ept[0], GJDInforArray[i].ept[1]
		, GJDInforArray[i].ept[0], GJDInforArray[i].ept[1]) ;
	}

	//�����öηֶ������������
	for (int i=0 ; i<LYDNum-1 ; i++)
	{
		for (int j=i+1; j<LYDNum ; j++)
		{
			if (GJDInforArray[i].BProjml > GJDInforArray[j].BProjml)
				swap(GJDInforArray[i].BProjml, GJDInforArray[j].BProjml);
		}
	}

	//�����öηֶ�������кϲ�
	for (int iGJD=0; iGJD + 1<GJDInforArray.size(); iGJD++)
	{
		if (fabs(GJDInforArray[iGJD+1].BProjml-GJDInforArray[iGJD].EProjml)<0.1
			|| GJDInforArray[iGJD+1].BProjml<GJDInforArray[iGJD].EProjml+0.001)
		{
			//ǰ�������öηǳ��ӽ�����ֽ���
			GJDInforArray[iGJD].ept[0] = GJDInforArray[iGJD+1].ept[0];
			GJDInforArray[iGJD].ept[1] = GJDInforArray[iGJD+1].ept[1];
			GJDInforArray.erase(GJDInforArray.begin()+iGJD+1);
			iGJD--;
		}

	}

}
//�������η�����
void RHA::BZJYXFQD() const
{
	//	���������ֶϵ����飬�ֱ��¼�����������ϸĽ������յ�

	//vector<GJDInf> tmpGJDarray;
	//XLDataBase xlmdb;
	//CString tmpStr;
	////xlpoint PZ;	
	//double StartFQDLC,EndFQDLC;//�����������������


	//tmpGJDarray.RemoveAll();
	//tmpStr=L"�Ľ�����=\"���ö�\"";
	//if(xlmdb.Read_XLDbs(m_mdbName,L"�Ľ��λ��ܱ�",tmpStr))	
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

	////��ͶӰ���
	//int LYDNum=tmpGJDarray.GetSize();
	//for (int i=0 ; i<LYDNum ; i++)
	//{
	//	tmpGJDarray[i].BProjml=PROJ_ML(array, tarray, tmpGJDarray[i].spt[0], tmpGJDarray[i].spt[1]
	//								, tmpGJDarray[i].spt[0], tmpGJDarray[i].spt[1]) ;
	//	tmpGJDarray[i].EProjml=PROJ_ML(array, tarray, tmpGJDarray[i].ept[0], tmpGJDarray[i].ept[1]
	//								, tmpGJDarray[i].ept[0], tmpGJDarray[i].ept[1]) ;
	//}

	////�����öηֶ������������
	//for (int i=0 ; i<LYDNum-1 ; i++)
	//{
	//	for (int j=i+1; j<LYDNum ; j++)
	//	{
	//		if (tmpGJDarray[i].BProjml > tmpGJDarray[j].BProjml)
	//			swap(tmpGJDarray[i].BProjml, tmpGJDarray[j].BProjml);
	//	}
	//}

	////�����öηֶ�������кϲ�
	//for (int iGJD=0; iGJD<tmpGJDarray.GetSize()-1; iGJD++)
	//{
	//	if (fabs(tmpGJDarray[iGJD+1].BProjml-tmpGJDarray[iGJD].EProjml)<0.1
	//		|| tmpGJDarray[iGJD+1].BProjml<tmpGJDarray[iGJD].EProjml+0.001)
	//	{
	//		//ǰ�������öηǳ��ӽ�����ֽ���
	//		tmpGJDarray[iGJD].ept[0] = tmpGJDarray[iGJD+1].ept[0];
	//		tmpGJDarray[iGJD].ept[1] = tmpGJDarray[iGJD+1].ept[1];
	//		tmpGJDarray.RemoveAt(iGJD+1);
	//		iGJD--;
	//	}

	//}

	int LYDNum=GJDInforArray.size();
	//���Ʒ�����
	if (LYDNum == 0)//818 �Ľ�����Ϣû���ɣ�����ȫ������
	{
		//ȫ�߷���
		DrawFQDBZ( QXB[0].KZH, QXB[JDarray.size()-1].KHZ, 400.0, RIGHTLINE);
	}
	else
	{
		//�����ò���
		//���л��ߵķ����α�ע
//818 ��λ���ױ꣬���ж��Ƿ�ȥ�����ж����ϻ������⡣�����öζ���
		if (GJDInforArray[0].BProjml > QXB[0].KZH + 0.01)//��·�տ�ʼ���ǸĽ�
			DrawFQDBZ(QXB[0].KZH, GJDInforArray[0].BProjml, 400.0, RIGHTLINE);
		if (GJDInforArray[LYDNum-1].EProjml + 0.001 < QXB[JDarray.size()-1].KHZ)//��·���һ���ǸĽ�
			DrawFQDBZ(GJDInforArray[LYDNum-1].EProjml,  QXB[JDarray.size()-1].KHZ, 400.0, RIGHTLINE);

		for (int i=1 ; i<LYDNum ; i++)
			DrawFQDBZ(GJDInforArray[i-1].EProjml, GJDInforArray[i].BProjml,400.0, RIGHTLINE);
	}

	
	
}




////��������׮�ż�����̱�
//void RHA::CalTZDtoLCB(double Scml, double Ecml, CArray<LCB,LCB> &LCBArray)
//{
//	LCB tmpLCB;
//	double projml;
//	ACHAR ckml[50];
//
//	if (Ecml<Scml+0.0001)
//	{
//		ads_printf(L"��ʼ��̴�����ֹ��̣�\n");
//		return;
//	}
//
//	for(int i=0; i<JDarray.size(); i++)//�Ե�������ѭ��
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

//�ж�һ���Ƿ������ö��ϣ������ö��Ϸ���true
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
	{//�����߻������
		int ix=-1;

		if (m_iDoc<g_DocManageAry.size() 
			&& CCreatPM::IsIncluded(m_mdbName, m_XLName, m_iDoc, ix) )
		{//��ȫ��������ɾ��
			//818 
			//��δ�����Ļ�����������л��ߵ������ȫ������
			if (id() == g_DocManageAry[m_iDoc].XLAry[ix]->id())
			{
				g_DocManageAry[m_iDoc].XLAry.erase(g_DocManageAry[m_iDoc].XLAry.begin()+ix);
				return Acad::eOk;
			}
		}

		ACHAR mes[60];
		_stprintf(mes, L"��ʾ��ϵͳɾ��%s����ʧ��\n", m_XLName);
		ads_printf(mes);
	}

	return Acad::eOk;
}

/*===================================================================
������:
------------------------------------------------------------
��������:������������ڲ�������߸̣߳����ص���������
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
��д: ��˧ 2016.10.24 �޸� 
=====================================================================*/
void RHA::PM_DMX(const CArray<LCB, LCB> &lcAry, double offst, vector<GMXTAB> &dmxAry)
{
	xlpoint PZ;
	double pt[2];//������
	double hgt;//�߳�
	int Pos;
	int i=0;
	int lcNum=lcAry.GetSize();
	int newLcNum=0;//��ȥ�ظ�����������ĸ���
	ACHAR pszLabel[80];

	dmxAry.clear();
	dmxAry.resize(lcNum);

	_tcscpy(pszLabel,L"�ݶ���������ڲ�");
	acedSetStatusBarProgressMeter(pszLabel,0,100);

	newLcNum=0;
	for (int i=0; i<lcNum; i++)
	{
		Pos = (int)(i*100.0/lcNum);
		acedSetStatusBarProgressMeterPos(Pos);

		//�����ظ����
		if(i>0 && fabs(lcAry[i].Projml-lcAry[i-1].Projml)<0.001)
			continue;

		//�ڲ������
		PZ.lc=lcAry[i].Projml;
		xlpoint_pz(array,tarray,PZ);  

		pt[0] = PZ.x +offst*cos(PZ.a+m_dHalfPI);
		pt[1] = PZ.y +offst*sin(PZ.a+m_dHalfPI);

		hgt=ptriDTM.interpolate(pt);

		//�����������鸳ֵ
		dmxAry[newLcNum].Cml=lcAry[i].Cml;
		split_ckml(lcAry[i].CKml, dmxAry[newLcNum].ml, dmxAry[newLcNum].GH);
		dmxAry[newLcNum].H=hgt;
		newLcNum++;
	}

	//ɾ��������
	if(newLcNum < lcNum)
		dmxAry.erase(dmxAry.begin()+newLcNum, dmxAry.end());

	acedSetStatusBarProgressMeterPos(100);    
	acedRestoreStatusBar();
}

/*===================================================================
������:EarseDLofBLonBXD
------------------------------------------------------------
��������:������ж��ϴӲ������Ͽ��������Ķ������������Ϊ-3��
------------------------------------------------------------
���:
DLarray               ��������(�����)
------------------------------------------------------------
�㷨: DLarray������indx == -3,��Ϊ�������Ͽ��������Ķ�����ɾ���ö���
------------------------------------------------------------
˵��: 1 �ú������γɹ�ͨ��������Create2XGTDLB����֮ǰʹ��
------------------------------------------------------------
��д: ��˧ 2016.11.11  
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
������:Create2XGTDLB
------------------------------------------------------------
��������:���ɶ��߹�ͨ������
------------------------------------------------------------
���:
DLarray               ��������(�����)
------------------------------------------------------------
��д: ��˧ 2016.11.6  
------------------------------------------------------------
��ע��
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

		//�жϲ��жζ�������
		if (DLarray[i].Indx != DLarray[i-1].Indx
			|| DLarray[i].Indx<0 || DLarray[i-1].Indx<0)
		{
			//���ж��������
			_tcscpy(sCkml, LCchr(DLarray[i-1].EGH, DLarray[i-1].ELC));
			_tcscpy(eCkml, LCchr(DLarray[i].BGH, DLarray[i].BLC));

			//��ȡ���жζ�������Ĳ����߶���
			GetBasLineDLAry(DLarray[i-1].BLName, sCkml, eCkml, tmpDLAry);

			if (tmpDLAry.size()>0)
			{
				//���Ų���
				for(int j=tmpDLAry.size()-1; j >= 0; j--)
					DLarray.insert(DLarray.begin()+i, tmpDLAry[j]);

			}

			i=i+tmpDLAry.size();
		}

	}

	//�ϲ���ͬλ�õĶ���
	for (int iDL=1; iDL<DLarray.size(); iDL++)
	{
		//�����֮ǰ���õĲ��ж�����Indx=-2���Ͳ������Ͽ����Ķ�����Indx=-3���ظ�
		//����ô�Բ��ж���Ϊ����ɾ���������Ͽ����Ķ���
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
������:GetBasLineDLAry
------------------------------------------------------------
��������:��ȡ�������ֳ���̷�Χ�ڵĶ�������
------------------------------------------------------------
����:
xlName              ������·��
sCkml               ���ߵ���ʼ�ֳ����
eCkml               ���ߵ���ֹ�ֳ����
------------------------------------------------------------
���:
dlAry               �������ֳ���̷�Χ�ڵĶ�������
------------------------------------------------------------
��д: ��˧ 2016.11.6  
------------------------------------------------------------
��ע��1 ֻ�м����ߺ�����߻���øú���
      2 �����ֳ���̱����ڻ��ߵ����ж���
=====================================================================*/
void RHA::GetBasLineDLAry(CString xlName, ACHAR sCkml[MaxCkmlStrNum], ACHAR eCkml[MaxCkmlStrNum], vector<DLTab> &dlAry)
{
	double sProjml;//���ͶӰ���
	double eProjml;//�յ�ͶӰ���
	RHA *pRHA_BL=NULL;
	DLTab tmpDL;
	CString str;

	//���ɲ�����
	pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, xlName);
	if(pRHA_BL == NULL)
	{
		ads_printf(L"�����γɹ�ͨ������ʱ��������·ȱʧ\n");return;
	}

	//��ȡ��������
	pRHA_BL->TrsCkmltoProjml(sCkml, sProjml);
	pRHA_BL->TrsCkmltoProjml(eCkml, eProjml);
	if (sProjml>eProjml)
	{
		str.Format(L"�����γɹ�ͨ������ʱ������ֳ����%s���յ��ֳ�%s���֮��\n", sCkml, eCkml);
		ads_printf(str);return;
	}

	//if (pRHA_BL->GetiRxdByProjml(sProjml) == -1
	//	|| pRHA_BL->GetiRxdByProjml(eProjml) == -1)
	//{
	//	ads_printf(L"���󣺻����������ֳ���̲������ж���\n");return;
	//}

	dlAry.clear();
	for (int iDL=0;iDL<pRHA_BL->DLarray.size();iDL++)
	{
		if (pRHA_BL->DLarray[iDL].TYLC > sProjml+0.0001
			&& pRHA_BL->DLarray[iDL].TYLC+0.0001<eProjml)
		{//�ڷ�Χ��
			dlAry.push_back(pRHA_BL->DLarray[iDL]);
		}

		if (pRHA_BL->DLarray[iDL].TYLC > eProjml+1)
			break;
	}	
	
	
	for (int iDL=0;iDL<dlAry.size();iDL++)
	{
		//ͶӰ���Ĭ��-100
		//dlAry[iDL].TYLC=-100.0;
		TrsNEtoProjml(dlAry[iDL].N, dlAry[iDL].E, dlAry[iDL].TYLC);//��ΰ�޸�
		//�޸Ķ������ 
		dlAry[iDL].Indx=-3;
	}
}



/*===================================================================
������:RoundMileage   
------------------------------------------------------------------------
��������:��ĳͳһ��̴����ֳ����ȡ��������ȡ�����ֳ���̶�Ӧ��ͳһ���
------------------------------------------------------------------------
����:
cml                ͳһ��� 
intPrec            ȡ������
directn            ������ǰȡ�� =true�����ȡ�� =false
------------------------------------------------------------------------
����ֵ: ȡ���ɹ��򷵻�ȡ�����ͳһ���cml�����򷵻�-100
------------------------------------------------------------------------
����: �������ʱ��
------------------------------------------------------------------------
��д: ��˧ 2017.2
=====================================================================*/
double RHA::RoundMileage(double cml, int intPrec, bool directn) const
{	
	ACHAR GH[16]=L"";
	double dml=0;
	double intdml=0;
	double Intdist=0;
	double DLdist=0; 
	int dlIndx=0;//�������
	double dlCml=0;
	double newCml=0.0;


	//1 ��ȡ�ֳ����ֵ��double��
	TrsCmltoCkml(cml, GH, dml);

	//2 ȡ�����ֳ����
	if (directn)
	{
		intdml=floor(dml/intPrec)*intPrec;//��ǰȡ�����ֳ����
		dlIndx=FindNearDLbyCml(cml, DLarray, true);
	}
	else
	{
		intdml=ceil(dml/intPrec)*intPrec;//���ȡ��
		dlIndx=FindNearDLbyCml(cml, DLarray, false);
	}

	if(dlIndx<0)
		return -100;

	//���������ͳһ���
	TrsCkmltoCml(DLarray[dlIndx].EGH, DLarray[dlIndx].ELC, dlCml);

	Intdist=fabs(dml-intdml);//ȡ���������ȡ��ǰ��̵ľ���
	DLdist=fabs(cml-dlCml);//ǰ���һ�������㵽cml�ľ���

	//3 ����ȡ�����ֳ�����Ƿ��Խ����
	if (Intdist < DLdist+0.001)//û�п����		
	{
		if (directn)
			_tcscpy(GH, DLarray[dlIndx].EGH);
		else
			_tcscpy(GH, DLarray[dlIndx].BGH);

		TrsCkmltoCml(GH, intdml, newCml);
		return newCml;
	}
	else//�����
	{
		//����������ȡ����
		newCml=dlCml;
	}

	return -100;
}

//��������̻�ȡ�Ľ�����Ϣ���Ľ�������GJDType�����ղ�����·��BXLName��EXLName
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
	BXLName = "���л���";
	EXLName = "���л���";
	return;
}

//��������̻�ȡ��������ڵĸĽ�������
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


//��������̻�ȡ�Ľ�����Ϣ���Ľ�������GJDType�����ղ�����·��BXLName��EXLName
void RHA::GetPtGJDInfByCml(double N, double E, int& GJDType, CString& BXLName, CString& EXLName)
{
	double cml;
	TrsNEtoCml(N, E, cml);

	GetPtGJDInfByCml(cml, GJDType, BXLName, EXLName);
}




/*************************************************************
��������:  
��������: �ж�ĳ�ֳ���������жλ��ǲ��ж���
���������
ckml     			�ֳ����
m_XLName 				��·��
m_mdbName             ���ݿ���
���������
blName 				�����ж��ϣ��������·��������ڲ��ж��ϣ����������·��/����ƽ����
rxdName             �����ж��ϣ��������·���ж���������ڲ��ж��ϣ����������·/����ƽ������ж���
��������ֵ:         �ڲ��ж��ϣ�����true;�����ж��ϣ�����false;
ע��: ������ƶ��ߣ�������������ж��������ж�
��д����˧ 2017.3
***************************************************************/
bool RHA::JudgeMLonBXDorRXD(ACHAR ckml[], CString &blName, CString &rxdName)
{
	blName=m_XLName;
	rxdName=L"";

	//���л��ߡ�ʩ������û�в��жΣ���ƻ�������������ȫ�ߵ�û�����ж�
	if (m_XLName == JYJX || m_XLName == SJJX || XLShuXing == SGBX || XLShuXing == LSX)
		return false;

	double cml;
	double projml;
	bool isOnBXorRX=false;


	if (m_XLName.Find(L"����") >= 0)
	{
		//���ж���
		int iRxd=0;
		int iDL;
		//����ͶӰ���
		TrsCkmltoProjml(ckml, projml);
		iRxd=GetiRxdByProjml(projml);
		if (iRxd >=0 )
		{
			//�ڱ���·���ж���
			rxdName=GetRxdName(iRxd);
		}
		else
		{
			//�ڲ��ж���
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
					ads_printf(L"���жε�����������\n");
			}

		}

	}
	else if (m_XLName.Find(L"���") >= 0)
	{
		//��ƶ���
		//����ͳһ���
		TrsCkmltoCml(ckml, cml);
		//�ж��Ƿ������ж���
		for (int i=0; i<FZZDMAry.size(); i++)
		{
			if (cml>FZZDMAry[i].BCml+0.001 
				&& cml+0.001<FZZDMAry[i].ECml)
			{
				//�����ж���
				rxdName=FZZDMAry[i].DLName;
				return false;
			}
		}

		//����ƻ�����
		m_XLName=SJJX;
		isOnBXorRX=true;
	}

	return isOnBXorRX;
}

//�ɻ�����������ж��Ƿ������ж��ϣ��������жα�ţ��ڲ��жη���-1
int RHA::GetiRxdNoFromCml_1X(double Cml_1X)
{
	for (int i=0; i<FZZDMAry.size(); i++)
	{
		if (i==0 && FZZDMAry[i].BL_BCml<0)
		{
			//�������
			if(Cml_1X<FZZDMAry[i].BL_ECml)
				return i;
		}
		else if (i+1 == FZZDMAry.size() && FZZDMAry[i].BL_ECml<0)
		{
			//�յ�����
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

////////////////////////////////////////////////////////��ΰ
//void RHA::INSERT_TUNNEL(CString XLName,CString qdxclc,CString zdxclc,CString NamTun,int LineNum,double style,int LMRFlag,int iRXD)
//{
//	ACHAR layname[256];
//	int zybz = -1;//���ע�����ұ�־��Ĭ�������
//	double offset = 0.0;
//	if(LMRFlag==MAINLINE)
//	{
//		if (XLName == L"���л���")
//			_stprintf(layname,L"%s",Tunnel);
//		else
//			_stprintf(layname,L"%s",GJTunnel);
//		if (HasLeft)
//			zybz = 1;
//		else
//			zybz = -1;
//		if (XLName == L"���л���")
//			zybz = 1;
//	}
//	else if(LMRFlag==LEFTLINE)
//	{
//		if (XLName == L"��������")
//			_stprintf(layname,L"%s",LeftTunnel);
//		else
//			_stprintf(layname,L"%s",GJLeftTunnel);
//		zybz = -1;
//		if (SCALE>9.9)
//			offset = m_ExOffset;
//	}
//	else 
//	{
//		if (XLName == L"��������")
//			_stprintf(layname,L"%s",RightTunnel);
//		else
//			_stprintf(layname,L"%s",GJRightTunnel);
//		zybz = 1;
//		if (SCALE>9.9)
//			offset = m_ExOffset;
//	}
//	/////////////////////////////////////////////////////////////
//	AcDbObjectIdArray IdAr;
//	ACHAR xlcb[80],xlce[80];//�ֳ����
//	double xclcb,xclce;//�ֳ����
//	ACHAR GH[20],LZB[20],LZE[20];//���ն���׮
//
//	//	acutPrintf(L"bri:%lf,len=%lf\n",lc0,lenBri);
//	int k=0;
//
//	////////////////////////////////////////////////////////////
//	//���ձ�ע���
//	double lcb=0.0;//���ͳһ���
//	double lce=0.0;//�յ�ͳһ���
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
//			if (XLName==L"��������")
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
//			if (XLName==L"��������")
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
//	double lenTunnel=lce-lcb;//�����
//	double lc0=0.5*(lcb+lce);//����ͳһ���
//	//////////////////////////////////////////////////////
//	//�Ƿ��ڳ�ͼ��Χ�� 818
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
//	//���ҿ�
//	double dL=1.5*SCALE;//���ߡ��������
//	double dR=1.5*SCALE;//���ߡ�����Ҳ��
//
//	double lc2x;double pt2x[2];double xjj;
//	
//
//	//�����������LineNumȷ���ǵ��߻���˫��
//	if(HasLeft && LineNum==2)//��������
//	{
//		CalXjjLx(lc0,xjj,lc2x,pt2x);
//		xjj=fabs(xjj);
//		dL+=xjj;
//		if (SCALE>9.9)
//			dL += m_ExOffset;
//	}
//	if(HasRight && LineNum==2)//��������
//	{
//		CalXjjRx(lc0,xjj,lc2x,pt2x);
//		xjj=fabs(xjj);
//		dR+=xjj;
//		if (SCALE>9.9)
//			dR += m_ExOffset;
//	}		
//	/////////////////////////////////////////////////////////////////////	
//	double a0,ab,ae;//��·��λ��/������߷�λ��,mid,begin,end//����Ϊ�������
//	xlpoint Ptmp;
//
//	Ptmp.lc=lc0;
//	k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//	if (offset>0.001)
//	{
//		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//	}
//	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//����������꣨���˫��λ�������ϣ�
//	a0=Ptmp.a;
//
//	Ptmp.lc=lcb;
//	k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//	if (offset>0.001)
//	{
//		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//	}
//	AcGePoint3d Ptb(Ptmp.y,Ptmp.x,0.0);//����������꣨���ߣ�
//	ab=Ptmp.a;
//
//	Ptmp.lc=lce;
//	k = AllXLPointPZ(XLName, Ptmp, iRXD, LMRFlag);
//	if (offset>0.001)
//	{
//		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
//		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
//	}
//	AcGePoint3d Pte(Ptmp.y,Ptmp.x,0.0);//����������꣨���ߣ�
//	ae=Ptmp.a;
//	///////////////////////////////////////////////////////////////
//	//�˵�
//	double aLb=ab-0.5*pi;//���ǰ������߷�λ��
//	double aRb=ab+0.5*pi;//���ǰ���Ҳ��߷�λ��
//
//	double aLe=ae-0.5*pi;//����ն�����߷�λ��
//	double aRe=ae+0.5*pi;//����ն��Ҳ��߷�λ��
//
//	double a_LT=ab-0.75*pi;//�Ľ�45�����������߷�λ��
//	double a_LB=ab+0.75*pi;
//	double a_RT=ae-0.25*pi;
//	double a_RB=ae+0.25*pi;
//
//	AcGePoint3d LT,LB,RT,RB,T0,B0;//������Ľ�++�����е�
//	AcGePoint3d LTe,LBe,RTe,RBe;//������Ľ�45����������
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
//	//����߽磬����Ϊ�������
//	//	double lw=0.5*SCALE;
//	double lw = LW;
//	if(style<0.9)//����
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
//			AcGePoint3d iPt(Ptmp.y,Ptmp.x,0.0);//����������
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
//	else//һ�����
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
//	//////////////////��ע///////////////////////////////
//	AcGePoint3d eBZPt;//��ע�ߵ��յ㣬���ΪPt0;
//	AcGePoint3d Pbz1,Pbz2;//��ע��;
//	ACHAR lentunstr[20];
//	acdbRToS(lenTunnel,2,NLC,lentunstr);
//	NamTun+=L"  ";//��������������֮��������ո�
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
//	makeline2(Pt0,eBZPt,-1,AcDb::kLnWt035,layname);//���ı�ע��
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
//	//	if(ChuTu_zybz == 1) Pbz1=Pbz2; //Lw2007-11-05ע��
//	maketext(Pbz1,TUNFlag,b,texth,-1,3,L"����",layname); //��������Ƽ�����    
//
//	int istrb=0,istre=0;
//	istrb=_tcslen(LZB);
//	istre=_tcslen(LZE);
//	AcGePoint3d PtLzB0,PtLzB1,PtLzE0,PtLzE1;//���յ��ע�ߵ�ʼ�ն�
//	AcGePoint3d BzPosb,BzPose;
//
//	WB=dL+0.5*SCALE;//��֤��ʼ��̱�ע�߶˵��������0.5mm�ľ���
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
//	WB=dL+0.5*SCALE+0.6*istrb*texth;//��֤�յ���̱�ע�߶˵��������0.5mm�ľ���
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
//	maketext(BzPosb,LZB,b,texth,-1,3,L"����",layname);  //��ע������   
//
//	//	PZtmp.lc=lce+draw_zybz*(0.5*texth+1.0*SCALE);
//	PZtmp.lc=lce+ChuTu_zybz*(0.5*texth+1.0*SCALE);//��ͼ����
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
//	maketext(BzPose,LZE,b,texth,-1,3,L"����",layname);  //��ע�յ����   
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
//	makeline2(PtLzB0,PtLzB1,-1,AcDb::kLnWt035,layname);//�����̱�ע��
//	makeline2(PtLzE0,PtLzE1,-1,AcDb::kLnWt035,layname);//�յ���̱�ע��
//	
//}


/*===================================================================
��������:
------------------------------------------------------------------------
����:
cml                ͳһ��� 
intPrec            ȡ������
directn            ������ǰȡ�� =true�����ȡ�� =false
------------------------------------------------------------------------
����ֵ: 
------------------------------------------------------------------------
����:
------------------------------------------------------------------------
��д: ��˧ 2017.5
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

	//�ж��Ƿ��������
	isSJX=false;
	if(XLShuXing == SJJX || XLShuXing == SJ2X)
		isSJX=true;

	//��ȡ��������
	getVdat.GetAllBridgeData(m_mdbName, m_XLName, briAry);

	//��ͼ
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
��������:
------------------------------------------------------------------------
����:
cml                ͳһ��� 
intPrec            ȡ������
directn            ������ǰȡ�� =true�����ȡ�� =false
------------------------------------------------------------------------
����ֵ: 
------------------------------------------------------------------------
����:
------------------------------------------------------------------------
��д: ��˧ 2017.5
=====================================================================*/
void RHA::MarkStation() const
{
	ACHAR xclc0[50];
	ACHAR NamSta[256];
	ACHAR TKSta[100];
	bool GJXZ;
	CGetAllVertData getVdat;
	vector<STATAB> staAry;

	//��ȡ��վ����
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
��������:
------------------------------------------------------------------------
����:
cml                ͳһ��� 
intPrec            ȡ������
directn            ������ǰȡ�� =true�����ȡ�� =false
------------------------------------------------------------------------
����ֵ: 
------------------------------------------------------------------------
����:
------------------------------------------------------------------------
��д: ��˧ 2017.5
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

	//��ȡ��վ����
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
��������:����ˮ׼��
------------------------------------------------------------------------
����:
cml                ͳһ��� 
intPrec            ȡ������
directn            ������ǰȡ�� =true�����ȡ�� =false
------------------------------------------------------------------------
����ֵ: 
------------------------------------------------------------------------
����:
------------------------------------------------------------------------
��д: ��˧ 2017.5
	=====================================================================*/
void RHA::MarkBM() const
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(m_mdbName,L"ˮ׼���",L"");
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
//���Ƶ���������
void RHA::INSERT_BIG_MID_BRIDGE(CString xclc0,  CString xclc0_start
								,  CString xclc0_end,  double lenBri,  double HBri,  CString StyBri
								,  CString NamBri	 ,  int LineNum) const
									// �����ֳ����  ������  �յ����  �ų�  ������ʽ	����  ���жα�ţ���1����0��ʾΪ���л��ߣ�����(1-���ߣ�2-˫��)
{
	if (lenBri<DrawParam.m_NoteMinBriLen-0.001 || HBri<DrawParam.m_NoteMinBriH-0.001)
		return;

	ACHAR layname[256];
	int zybz = -1;//��ע����-1-��࣬1-�Ҳ�,  Ĭ�ϱ�ע�����
	double offset = 0.0;

	_tcscpy(layname, DrawParam.BigMidBridge);//����

	if (m_XLName == JYJX)//818 ���Ҳ��ƫ�ƾ�
		zybz = 1;//���в�������Ҳ�
	else if (m_XLName == SJJX)
			zybz = -1;
	else if (m_XLName == SJ2X)
		zybz = -1;
	
	//if (HasLeft)//�������Ϊ���ߣ�����߱�ע���Ҳ� 
	//	zybz = 1;
	//else//�����ע�����
	//	zybz = -1;
	//if (m_XLName == L"���л���")
	//	zybz = 1;//���в�������Ҳ�

	if (DrawParam.SCALE>9.9)//���ߺ�����ʱ�� //(LMRFlag==LEFTLINE
		offset = DrawParam.m_ExOffset;

	

	double lc0,  lcb,  lce;//����ͳһ���
	int jdNum=JDarray.size();


	TrsCkmltoProjml(xclc0, lc0);
	TrsCkmltoProjml(xclc0_start, lcb);
	TrsCkmltoProjml(xclc0_end, lce);
	if(lc0<0.0 && lcb < 0.00 && lce <0.00)
		return;
	if (lc0 < QXB[0].KZH || lc0 > QXB[jdNum-1].KHZ) 
		return;

	
	//�ж��Ƿ��ڳ�ͼ��Χ��
	//////////////////////////////////////////////////////////////////////
	//���ҿ��
	int k=0;
	double lc2x;
	double pt2x[2];
	double xjj;
	double dL=1.5*DrawParam.SCALE;//���� ������
	double dR=1.5*DrawParam.SCALE;//���ߡ����Ҳ��


	//˫���ż���ƫ�ƾ� 818
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
			if(xjj<0.0)//���
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
	//���С���������·��������
	if ( lcb < 0.01 ) 
		lcb=lc0-0.5*lenBri;//���ͳһ���
	if ( lce < 0.01 ) 
		lce=lc0+0.5*lenBri;//�յ�ͳһ���

	double a0,  ab,  ae;//��·��λ��/�����߷�λ��,  mid,  begin,  end//����Ϊ������
	xlpoint Ptmp;
	AcGePoint3d Pt0, Ptb, Pte;

	//�����������꣨���ߣ�
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

	//�����������
	Ptmp.lc=lcb;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.01)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	Ptb.set(Ptmp.y,  Ptmp.x,  0.0);
	ab=Ptmp.a;

	//�����յ�����
	Ptmp.lc=lce;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.01)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	Pte.set(Ptmp.y,  Ptmp.x,  0.0);//�����������꣨���ߣ�
	ae=Ptmp.a;

	//�����Ľ�45����������ĵ�����

	double aLb=ab-0.5*pi;//��ǰ������߷�λ��
	double aRb=ab+0.5*pi;//��ǰ���Ҳ��߷�λ��

	double aLe=ae-0.5*pi;//���ն�����߷�λ��
	double aRe=ae+0.5*pi;//���ն��Ҳ��߷�λ��

	double a_LT=ab-0.75*pi;//�Ľ�45�����������߷�λ��
	double a_LB=ab+0.75*pi;
	double a_RT=ae-0.25*pi;
	double a_RB=ae+0.25*pi;

	AcGePoint3d LT,  LB,  RT,  RB;//�ſ��Ľ�
	AcGePoint3d LTe,  LBe,  RTe,  RBe;//�ſ��Ľ�45�����������

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
	
	//10m�ļ������������ȷ���Ŀ���
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
		iPt.set(Ptmp.y,  Ptmp.x,  0.0);//����������

		getLineEndPt(iPt,  dL,  ai-0.5*pi,  iTPt);
		getLineEndPt(iPt,  dR,  ai+0.5*pi,  iBPt);
		TPts.append(AcGePoint2d(iTPt.x,  iTPt.y));
		BPts.append(AcGePoint2d(iBPt.x,  iBPt.y));
	}

	TPts.append(AcGePoint2d(RT.x,  RT.y));
	BPts.append(AcGePoint2d(RB.x,  RB.y));

	//�콭20090604  ��Ժ�涨����ͼ����0.5mm
	double lw=0.5*DrawParam.SCALE;//��ΰע�͵�2007-08-07ͳһ�߿�������������߿������������
	ads_name en1,  en2,  en3,  en4;//���߽�ʵ����


	//�����ı��ο���
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

	//�����Ľǵ�����
	makeline2(LT,  LTe,  -1,  lw,  layname);
	makeline2(LB,  LBe,  -1,  lw,  layname);
	makeline2(RT,  RTe,  -1,  lw,  layname);
	makeline2(RB,  RBe,  -1,  lw,  layname);

	//�Լ��������������
	if (XLShuXing == JYJX || XLShuXing == JY2X)
	{
		ads_point PtCen;//������������
		double Angle;//����ߵĽǶ�

		PtCen[0] = Pt0.x;
		PtCen[1] = Pt0.y;
		PtCen[2] = 0.0;
		Angle = 0.5*pi-(a0-0.25*pi);
		FormFwj(Angle);
		Angle = Angle * 180 / pi ;
		ads_command(RTSTR,   L"-hatch",   RTSTR,   L"S",   RTENAME,   en1,   RTENAME,   en2,   RTENAME,   en3,   RTENAME,   en4,   
			RTSTR ,   L"",   RTSTR,   L"P",   RTSTR,   L"U",   RTREAL,   Angle,   RTREAL,   0.25*texth,   RTSTR,   L"N" ,  RTSTR,   L"",   0);
	}

	//��ע

	AcGePoint3d eBZPt;//��ע�ߵ��յ㣬���ΪPt0;
	AcGePoint3d Pbz1,  Pbz2;//��ע��;
	ACHAR Style[256];
	ACHAR NameLc[256];
	ACHAR ckml[80];
	ACHAR gh[20];
	double dml;

	_tcscpy(Style,  StyBri);
	_tcscpy(NameLc,  NamBri);

	//�޸�����������̾���
	_tcscpy(ckml,  xclc0);
	split_ckml(ckml,  dml,  gh);
	_tcscpy(ckml,   LCchr(gh,  dml,  DrawParam.NCQ));
	_tcscat(NameLc,  ckml);

	int iSty=_tcslen(Style);
	int iNam=_tcslen(NameLc);
	int MaxStr=iSty>iNam?iSty:iNam;//����ַ���
	double WB=dL;//���

	/*if (m_IsAutoLorR && pGTSJEX)818
	{
		zybz = CalInsFlagLorR(Pt0.y,  Pt0.x,  m_XLName);
	}*/

	//���ߣ���ֱ����·��
	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*zybz);
	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*zybz);
	makeline2(Pt0,  eBZPt,  -1,  AcDb::kLnWt035,  layname);


	//д������������̡��׿���ʽ
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

	maketext(Pbz1,  Style,  b,  texth,  -1,  3,  L"����",  layname);     
	maketext(Pbz2,  NameLc,  b,  texth,  -1,  3,  L"����",  layname);
}


//////////////////////////////////////////////////////��ΰ
void RHA::INSERT_TUNNEL(CString qdxclc,CString zdxclc,CString NamTun,int LineNum,int Style) const
{
	ACHAR layname[256];
	int zybz = -1;//���ע�����ұ�־��Ĭ�������
	double offset = 0.0;

	_tcscpy(layname, DrawParam.Tunnel);//����

	if (m_XLName == JYJX)//818 ���Ҳ��ƫ�ƾ�
		zybz = 1;//���в�������Ҳ�
	else if (m_XLName == SJJX)
		zybz = -1;
	else if (m_XLName == SJ2X)
		zybz = -1;

	int LMRFlag=0;

	if (DrawParam.SCALE>9.9)//���ߺ�����ʱ�� //(LMRFlag==LEFTLINE
		offset = DrawParam.m_ExOffset;


	
	/////////////////////////////////////////////////////////////
	AcDbObjectIdArray IdAr;
	ACHAR xlcb[80],xlce[80];//�ֳ����
	double xclcb,xclce;//�ֳ����
	ACHAR GH[20],LZB[20],LZE[20];//���ն���׮

	//	acutPrintf(L"bri:%lf,len=%lf\n",lc0,lenBri);
	int k=0;

	////////////////////////////////////////////////////////////
	//���ձ�ע���
	double lcb=0.0;//���ͳһ���
	double lce=0.0;//�յ�ͳһ���

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
	double lenTunnel=lce-lcb;//�����
	double lc0=0.5*(lcb+lce);//����ͳһ���
	//////////////////////////////////////////////////////
	
	////////////////////////////////////////////
	//���ҿ�
	double dL=1.5*DrawParam.SCALE;//���ߡ��������
	double dR=1.5*DrawParam.SCALE;//���ߡ�����Ҳ��

	double lc2x;double pt2x[2];double xjj;
	
	//˫���ż���ƫ�ƾ� 818
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
			if(xjj<0.0)//���
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
	double a0,ab,ae;//��·��λ��/������߷�λ��,mid,begin,end//����Ϊ�������
	xlpoint Ptmp;

	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.001)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//����������꣨���˫��λ�������ϣ�
	a0=Ptmp.a;

	Ptmp.lc=lcb;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.001)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	AcGePoint3d Ptb(Ptmp.y,Ptmp.x,0.0);//����������꣨���ߣ�
	ab=Ptmp.a;

	Ptmp.lc=lce;
	xlpoint_pz(array, tarray, Ptmp);
	if (offset>0.001)
	{
		Ptmp.x = Ptmp.x + offset * cos(Ptmp.a + LMRFlag * 0.5*PI);
		Ptmp.y = Ptmp.y + offset * sin(Ptmp.a + LMRFlag * 0.5*PI);
	}
	AcGePoint3d Pte(Ptmp.y,Ptmp.x,0.0);//����������꣨���ߣ�
	ae=Ptmp.a;
	///////////////////////////////////////////////////////////////
	//�˵�
	double aLb=ab-0.5*pi;//���ǰ������߷�λ��
	double aRb=ab+0.5*pi;//���ǰ���Ҳ��߷�λ��

	double aLe=ae-0.5*pi;//����ն�����߷�λ��
	double aRe=ae+0.5*pi;//����ն��Ҳ��߷�λ��

	double a_LT=ab-0.75*pi;//�Ľ�45�����������߷�λ��
	double a_LB=ab+0.75*pi;
	double a_RT=ae-0.25*pi;
	double a_RB=ae+0.25*pi;

	AcGePoint3d LT,LB,RT,RB,T0,B0;//������Ľ�++�����е�
	AcGePoint3d LTe,LBe,RTe,RBe;//������Ľ�45����������
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
	//����߽磬����Ϊ�������
	//	double lw=0.5*DrawParam.SCALE;
	double lw=DrawParam.OtherParam.EntAry[0].LW;
	if(Style<0.9)//����
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
			AcGePoint3d iPt(Ptmp.y,Ptmp.x,0.0);//����������
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
	else//һ�����
	{
		makeline2(LT,LTe,-1,lw,layname);
		makeline2(LB,LBe,-1,lw,layname);
		makeline2(RT,RTe,-1,lw,layname);
		makeline2(RB,RBe,-1,lw,layname);
	}

	makeline2(LT,LB,-1,lw,layname);
	makeline2(RT,RB,-1,lw,layname);

	//////////////////��ע///////////////////////////////
	AcGePoint3d eBZPt;//��ע�ߵ��յ㣬���ΪPt0;
	AcGePoint3d Pbz1,Pbz2;//��ע��;
	ACHAR lentunstr[20];
	acdbRToS(lenTunnel,2,DrawParam.NLC,lentunstr);
	NamTun+=L"  ";//��������������֮��������ո�
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
	makeline2(Pt0,eBZPt,-1,AcDb::kLnWt035,layname);//���ı�ע��

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
	//	if(ChuTu_zybz == 1) Pbz1=Pbz2; //Lw2007-11-05ע��
	maketext(Pbz1,TUNFlag,b,texth,-1,3,L"����",layname); //��������Ƽ�����    

	int istrb=0,istre=0;
	istrb=_tcslen(LZB);
	istre=_tcslen(LZE);
	AcGePoint3d PtLzB0,PtLzB1,PtLzE0,PtLzE1;//���յ��ע�ߵ�ʼ�ն�
	AcGePoint3d BzPosb,BzPose;

	WB=dL+0.5*DrawParam.SCALE;//��֤��ʼ��̱�ע�߶˵��������0.5mm�ľ���

	if(zybz>0)WB=dR+0.5*DrawParam.SCALE;
	PtLzB0[X]=Ptb.x+WB*sin(ab+pi*0.5*zybz);
	PtLzB0[Y]=Ptb.y+WB*cos(ab+pi*0.5*zybz);
	PtLzE0[X]=Pte.x+WB*sin(ae+pi*0.5*zybz);
	PtLzE0[Y]=Pte.y+WB*cos(ae+pi*0.5*zybz);

	WB=dL+0.5*DrawParam.SCALE+0.6*istrb*texth;//��֤�յ���̱�ע�߶˵��������0.5mm�ľ���
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

	maketext(BzPosb,LZB,b,texth,-1,3,L"����",layname);  //��ע������   

	//	PZtmp.lc=lce+draw_zybz*(0.5*texth+1.0*DrawParam.SCALE);
	PZtmp.lc=lce+DrawParam.m_TextDir*(0.5*texth+1.0*DrawParam.SCALE);//��ͼ����
	xlpoint_pz(array, tarray, PZtmp);
	B=PZtmp.a;
	WB=dL+0.5*DrawParam.SCALE+0.6*istrb*texth;

	BzPose[X]=PZtmp.y+(WB+offset)*sin(B+pi*0.5*zybz) ;
	BzPose[Y]=PZtmp.x+(WB+offset)*cos(B+pi*0.5*zybz) ;
		         
	b=B-pi*0.5;
	b=pi*0.5-b;        
	FormFwj(b);

	maketext(BzPose,LZE,b,texth,-1,3,L"����",layname);  //��ע�յ����   

	WB=dL+0.5*DrawParam.SCALE+1.0*istrb*texth;
	//PtLzB1[X]=Ptb.x+WB*sin(ab+pi*0.5*draw_zybz);
	//PtLzB1[Y]=Ptb.y+WB*cos(ab+pi*0.5*draw_zybz);
	//PtLzE1[X]=Pte.x+WB*sin(ae+pi*0.5*draw_zybz);
	//PtLzE1[Y]=Pte.y+WB*cos(ae+pi*0.5*draw_zybz);
	PtLzB1[X]=Ptb.x+WB*sin(ab+pi*0.5*zybz);
	PtLzB1[Y]=Ptb.y+WB*cos(ab+pi*0.5*zybz);
	PtLzE1[X]=Pte.x+WB*sin(ae+pi*0.5*zybz);
	PtLzE1[Y]=Pte.y+WB*cos(ae+pi*0.5*zybz);


	makeline2(PtLzB0,PtLzB1,-1,AcDb::kLnWt035,layname);//�����̱�ע��
	makeline2(PtLzE0,PtLzE1,-1,AcDb::kLnWt035,layname);//�յ���̱�ע��
	
}

void RHA::INSERT_STATION(CString xclc0,CString NamSta,CString TKSta,bool GJXZ) const
{
	/////////////////////////////////////////////////////////////
	//ͼ����
	ACHAR layname[256];

	_tcscpy(layname, DrawParam.Station);//����

	int zybz=-1;
	int LMRFlag=0;//818
	if (m_XLName == JYJX)//818 ���Ҳ��ƫ�ƾ�
		zybz = 1;//���в�������Ҳ�
	else if (m_XLName == SJJX)
		zybz = -1;
	else if (m_XLName == SJ2X)
		zybz = -1;
	////////////////////////////////////////////////////////////////
	//�ҳ�վͼ���λ��
	ACHAR dwgPath[256],Name[50];
	_stprintf(dwgPath,L"%s\\DWG\\",SysPath);
	_stprintf(Name,L"%s.dwg",TKSta);

	AcDbObjectId pBlockId;
	pBlockId=CreateBlockFromDwgFile(dwgPath,Name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s%s������!\n",dwgPath,Name);
		return;
	}
	//////////////////////////////////////////////////////////
	AcDbObjectIdArray IdAr;
	double lc0;//����ͳһ���
	ACHAR xlc0[50];
	_tcscpy(xlc0,xclc0);
	//////////////////////////////////////////////////////////////////////////
	//�޸ĳ�վ������̾���
	ACHAR GH[20];
	double dml;
	split_ckml(xlc0,dml,GH);
	_tcscpy(xlc0, LCchr(GH,dml,DrawParam.NCQ));

	TrsCkmltoProjml(xclc0, lc0);
	
	////////////////////////////////////////////////////////////
	int k=0;
	double a0;//��·��λ��
	xlpoint Ptmp;

	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);

	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//���������
	a0=Ptmp.a;

	/*
	if(NJD2x>0&&iRXD<0)//����//��������λ�ñ�֤������ߴ��
	{
	double lc2x;double pt2x[2];double xjj;
	CalXjj(lc0,xjj,lc2x,pt2x);//�����߼��
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

	AcGePoint3d BZPt;//ע�ֵ�0;

	int istr=_tcslen(xlc0);
	//	Ptmp.lc=lc0+ChuTu_zybz*(0.5*texth+1.0*DrawParam.SCALE);//��ͼ����
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
	//������վ���(�Ľ�����ΪL"�Ľ�")
	if ((XLShuXing == JYJX || XLShuXing == JY2X) && GJXZ == 1)
	{
		double NCross,ECross;
		ECross = Pt0.x+(0.48*istr*texth)*sin(B+pi*0.5*zybz);
		NCross = Pt0.y+(0.48*istr*texth)*cos(B+pi*0.5*zybz);
		DrawStaFQDBZ(NCross,ECross,B+pi*0.5,_tcslen(xlc0)*0.3*texth,LMRFlag);
	}

	


	maketext(BZPt,xlc0,b,texth,-1,3,L"����",layname);

	double WB=41;//����վ��־�ĸ߶�
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
	maketext(BZPt,Name,rotation,texth,-1,3,L"����",layname);

}

void RHA::DrawStaFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const//���Ʒ����α�ע������
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

//����ˮ׼��  
void RHA::INSERT_BM(double X, double Y, CString GH, double XLC, double DD, CString BianHao, double LEV) const
{	
	ACHAR layname[256];
	ACHAR GaoCheng[20];
	double projml=0.0;//ͳһ���
	AcGePoint3d InsertPt;//�����

	_stprintf(layname, L"%s", DrawParam.BM);//ͼ��

	acdbRToS(LEV, 2, NGC, GaoCheng);//�߳�ȡС��

	if (XLShuXing != JYJX)//818 ˮ׼��������·��JYJX
		return;
	
	if(XLC>0.01)
	{
		ACHAR ghCpy[20];
		_tcscpy(ghCpy,  GH);

		TrsCkmltoProjml(ghCpy,  XLC,  projml);
	}


	//��ȡͼ��
	ACHAR dwgPath[256],  name[50];
	AcDbObjectId pBlockId;

	_stprintf(dwgPath, L"%s\\DWG\\", SysPath);
	_tcscpy(name, L"31014.dwg");
	pBlockId = CreateBlockFromDwgFile(dwgPath, name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s%s������!\n", dwgPath, name);
		return;
	}

	//����ͼ��
	int k=0;
	double a0;//��·��λ��
	if(X<0.1&&Y<0.1)//x=0, y=0
	{
		xlpoint Ptmp;
		Ptmp.lc=projml;
		//k = AllXLPointPZ(L"���л���",  Ptmp,  0,  MAINLINE);
		xlpoint_pz(array,  tarray,  Ptmp);

		AcGePoint3d Pt0(Ptmp.y, Ptmp.x, 0.0);//��·���ߵ�����
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

	//��ע��ź͸߳�
	AcGePoint3d BZCen, BZCenL, BZCenR;//ע�ֶ������ĵ㣬����������
	AcGePoint3d BZTop, BZBtom;//ˮ׼�������ġ��߳�����


	int istrbh=_tcslen(BianHao);//ˮ׼�����ַ�����
	int istrlev=_tcslen(GaoCheng);//ˮ׼��߳��ַ�����
	int iMax=istrbh>istrlev?istrbh:istrlev;//�ַ�����󳤶�
	double texHgt=DrawParam.OtherParam.MarkAry[3].TextHeight;//ls ȡ�������ָ�

	//Lw2007-11-05���������ͼʱ����עӦ�õ���������ٱ�עʱ������ChuTu_zybz��Ӱ��
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
		maketext(BZTop, strIndx, 0.0, texHgt, -1, 3, L"����", layname);
		maketext(BZBtom, GaoCheng, 0.0, texHgt, -1, 3, L"����", layname);
	}
	else
	{
		maketext(BZTop, strIndx, PI, texHgt, -1, 3, L"����", layname);
		maketext(BZBtom, GaoCheng, PI, texHgt, -1, 3, L"����", layname);
	}
	makeline2(BZCenL, BZCenR, -1, AcDb::kLnWtByLayer, layname);
}

/*===================================================================
��������:���Ƶ���
------------------------------------------------------------------------
����:
cml                ͳһ��� 
intPrec            ȡ������
directn            ������ǰȡ�� =true�����ȡ�� =false
------------------------------------------------------------------------
����ֵ: 
------------------------------------------------------------------------
����:
------------------------------------------------------------------------
��д: ��˧ 2017.5
=====================================================================*/
void RHA::MarkDX() const
{
	if (XLShuXing != JYJX)//818 ����ֻ�ܼ��л��ߣ�
		return;
	
	XLDataBase xlmdb;

	if (xlmdb.Read_XLDbs(m_mdbName,L"���߱�",L"") && xlmdb.DXNum>0)
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

//���Ƶ���
void RHA::INSERT_DX(int NUM, double *NX, double *EY, CStringArray *BianHao, double *GaoCheng, double *FWJ) const
{
	ACHAR layname[40];
	AcGePoint3d InsertPt;//�����

	_stprintf(layname, L"%s", DrawParam.DaoXian);//ͼ����

	//��ȡͼ��
	ACHAR dwgPath[256], Name[50];
	_stprintf(dwgPath, L"%s\\DWG\\", SysPath);
	_tcscpy(Name, L"dxsample.dwg");
	AcDbObjectId pBlockId;
	pBlockId = CreateBlockFromDwgFile(dwgPath, Name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s�л��Ƶ��ߵ��ͼ��-%s-�����ڣ����ܻ��Ƶ��ߵ�!\n", dwgPath, Name);
		return;
	}

	AcGePoint3d BZCen, BZCenL, BZCenR;//ע�ֶ������ĵ㣬����������
	AcGePoint3d BZTop, BZBtom;//ˮ׼�������ġ��߳�����
	CString strtmp;
	ACHAR ctmp[50];
	ACHAR strIndx[20];
	int istrbh;
	int istrlev;
	int iMax;
	double texHgt;

	texHgt=DrawParam.OtherParam.MarkAry[3].TextHeight;//ls ȡ�������ָ�

	for(int i = 0; i < NUM; i++)
	{
		InsertPt.set(EY[i],  NX[i], 0);

		AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setScaleFactors(AcGeScale3d(DrawParam.SCALE, DrawParam.SCALE, 1));
		pBkRr->setLayer(layname);
		AddEntityToDbs(pBkRr);

		//��ע��ź͸߳�
		_tcscpy(ctmp, BianHao->GetAt(i));
		istrbh = _tcslen(ctmp);//ˮ׼�����ַ�����

		_stprintf(ctmp, L"%.2lf", GaoCheng[i]);
		istrlev= _tcslen(ctmp);//ˮ׼��߳��ַ�����
		iMax=istrbh>istrlev?istrbh:istrlev;//�ַ�����󳤶�

		//Lw2007-11-05���������ͼʱ����עӦ�õ���������ٱ�עʱ������DrawParam.m_TextDir��Ӱ��
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
			maketext(BZTop, strIndx, 0.0, texHgt, -1, 3, L"����", layname);
			_stprintf(ctmp, L"%.2lf", GaoCheng[i]);
			maketext(BZBtom, ctmp, 0.0, texHgt, -1, 3, L"����", layname);
		}
		else
		{
			maketext(BZTop, strIndx, PI, texHgt, -1, 3, L"����", layname);
			_stprintf(ctmp, L"%.2lf", GaoCheng[i]);
			maketext(BZBtom, ctmp, PI, texHgt, -1, 3, L"����", layname);
		}
		makeline2(BZCenL, BZCenR, -1, AcDb::kLnWtByLayer, layname);
	}
}

//���������С�ź��Ľӳ�
void RHA::CalSmallBriLen(vector<SMALLBRIDGETAB> &smallBriAry) const
{
	if(XLShuXing != JYJX && XLShuXing != JY2X)
		return;

	//��ȡ�����
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

		//��ȡ����ߵ�С�ź�
		tmpStr.Format(L"��·��=\"%s\"", xlInforAry[i].XLName);
		if (xlmdb.Read_XLDbs(m_mdbName, L"С�ź�", tmpStr))
		{
			for (int iSBR=0; iSBR<xlmdb.SBriNum; iSBR++)
			{
				if (_tcsstr(xlmdb.SBri[iSBR].KKStyle, L"�ӳ�") == NULL)//���� ���ӳ���
					continue;

				for (int iJYSBR=0; iJYSBR<smallBriAry.size(); iJYSBR++)
				{
					if (fabs(smallBriAry[iJYSBR].ml - xlmdb.SBri[iSBR].ml) < 0.1 
						&& _tcscmp(smallBriAry[iJYSBR].GH, xlmdb.SBri[iSBR].GH) == 0)
					{


						_tcscat(smallBriAry[iJYSBR].KKStyle, L"(�ӳ�)");
						TrsCkmltoPZ(smallBriAry[iJYSBR].GH, smallBriAry[iJYSBR].ml, PZTemp);
						PT[0] = PZTemp.x;
						PT[1] = PZTemp.y;
						PT[2] = 0.0;


						FWJ_JC = PZTemp.a + rad(smallBriAry[iJYSBR].JJiao);//����ķ�λ��


						LorR = JudgePtLorR(PZTemp.x, PZTemp.y, pSJX->array, pSJX->tarray, Dist);

						if (LorR == 0)//���������ͨ�����غϣ��ӳ�����ͨ��ƶ����Ҳ�
						{
							/*if (pzLinepJYarray[iXL]->pGTSJEX) 818 �Ͱ�����������·�ͼ�¼�ӳ��ͺ�
							{
							pzLinepJYarray[iXL]->Search_EXPt(pzLinepJYarray[iXL]->pGTSJEX->array,pzLinepJYarray[iXL]->pGTSJEX->tarray,PT,FWJ_JC,resPt);
							smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0]) + (PT[1]-resPt[1])*(PT[1]-resPt[1]))
							+ fabs(xlmdb.SBri[iSBR].dwR);
							}*/

						}
						else if (LorR == -1)//�������ڹ�ͨ������࣬���ҽӳ�
						{
							Search_EXPt(pSJX->array,pSJX->tarray,PT,FWJ_JC,resPt);
							smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0])
								+ (PT[1]-resPt[1])*(PT[1]-resPt[1]))
								+ fabs(xlmdb.SBri[iSBR].dwR);

							//if (xlmdb.SBri[iSBR].LineNum == 1)//����Ϊ1�ӳ�����ͨ�����Ҳ�
							//{
							//	Search_EXPt(pSJX->array,pSJX->tarray,PT,FWJ_JC,resPt);
							//	smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0])
							//		+ (PT[1]-resPt[1])*(PT[1]-resPt[1]))
							//		+ fabs(xlmdb.SBri[iSBR].dwR);
							//}

							//if (xlmdb.SBri[iSBR].LineNum == 2)//����Ϊ2�ӳ�����ͨ�����Ҳ� 818 ��ƶ��߶���  LineNum ��Ч ����  ��ͨ���� if (InsertItemDB.SBri[iSBR].LineNum == 2)//�ӳ�����ͨ�������
							//{
							//	if (pzLinepJYarray[iXL]->pGTSJEX)
							//	{
							//		pzLinepJYarray[iXL]->Search_EXPt(pzLinepJYarray[iXL]->pGTSJEX->array,pzLinepJYarray[iXL]->pGTSJEX->tarray,PT,FWJ_JC,resPt);
							//		smallBriAry[iJYSBR].dwR = sqrt((PT[0]-resPt[0])*(PT[0]-resPt[0]) + (PT[1]-resPt[1])*(PT[1]-resPt[1]))
							//			+ fabs(xlmdb.SBri[iSBR].dwR);
							//	}
							//}
						}
						else//��������������Ҳ�
						{
							//�ӳ�����ͨ��������
							Search_EXPt(pSJX->array,pSJX->tarray,PT,FWJ_JC,resPt);//����������ϵĵ�����
							//�������
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

//����С�ź�
void RHA::plhINSBR(bool IsDrawAll) const
{
	vector<SMALLBRIDGETAB> smallBriAry;
	CString tmpStr;
	XLDataBase xlmdb;

	tmpStr.Trim();
	tmpStr=L"��·��=\"" + m_XLName + L"\"";

	if (xlmdb.Read_XLDbs(m_mdbName, L"С�ź�", tmpStr) && xlmdb.SBriNum>0)
	{
		smallBriAry.resize(xlmdb.SBriNum);
		for (int i=0; i<xlmdb.SBriNum; i++)
		{
			smallBriAry[i]=xlmdb.SBri[i];
		}

		//������л���С�ź��Ľӳ�
		if(XLShuXing == JYJX || XLShuXing == JY2X)
			CalSmallBriLen(smallBriAry);

		int LineNum = 1;
		CString NameCross;

		for(int i=0;i<smallBriAry.size();i++)
		{
			NameCross=smallBriAry[i].KKStyle;
			LineNum = smallBriAry[i].LineNum;

			//���ֻ�ǻ���������������������
			if (!IsDrawAll && !(smallBriAry[i].InsertStyle>0.5 && smallBriAry[i].InsertStyle<2.5))
				continue;
			//818 �Ľ����� ���� �½� ��ʲô�ã�

			INSERT_SMALL_BRIDGE(smallBriAry[i].GH,smallBriAry[i].ml,NameCross,LineNum,smallBriAry[i].GJXZ,MAINLINE,-1,
				smallBriAry[i].dwL,smallBriAry[i].dwR,
				smallBriAry[i].LLevel,smallBriAry[i].RLevel,
				smallBriAry[i].JJiao,smallBriAry[i].InsertStyle,smallBriAry[i].ToUse);
		}

	}

}

void RHA::DrawSmallBridge(AcGePoint3d sPt, AcGePoint3d ePt, ACHAR layname[100], int LineNum, int mode, double LWid,  double Rwid, int LMRFlag, double lc0,  double Jiao) const
{//С�����ն���������

	double ab,ae;
	xyddaa(sPt.y,sPt.x,ePt.y,ePt.x,&ab);
	ae=ab;
	double aLb,aRb,aLe,aRe;
	if (mode>3 && mode<9)//ƽ����
	{
		double fwj = ab + rad(Jiao);
		aLb=fwj-pi;//��ǰ������߷�λ��
		aRb=fwj;//��ǰ���Ҳ��߷�λ��
		aLe=fwj-pi;//���ն�����߷�λ��
		aRe=fwj;//���ն��Ҳ��߷�λ��
	}
	else
	{
		aLb=ab-0.5*pi;//��ǰ������߷�λ��
		aRb=ab+0.5*pi;//��ǰ���Ҳ��߷�λ��
		aLe=ae-0.5*pi;//���ն�����߷�λ��
		aRe=ae+0.5*pi;//���ն��Ҳ��߷�λ��
	}

	double a_LT=ab-0.75*pi;//�Ľ�45�����������߷�λ��
	double a_LB=ab+0.75*pi;
	double a_RT=ae-0.25*pi;
	double a_RB=ae+0.25*pi;

	AcGePoint3d LT,LB,RT,RB,T0,B0;//�ſ��Ľ�++�����е�
	AcGePoint3d LTe,LBe,RTe,RBe;//�ſ��Ľ�45����������
	LT.z=0.0;RT.z=0.0;LB.z=0.0;RB.z=0.0;
	LTe.z=0.0;RTe.z=0.0;LBe.z=0.0;RBe.z=0.0;
	T0.z=0.0;B0.z=0.0;

	//���ҿ��
	double dL=1.5*DrawParam.SCALE;//���� ������
	double dR=1.5*DrawParam.SCALE;//���ߡ����Ҳ��
	double lc2x;double pt2x[2];double xjj;

	//if(LMRFlag==MAINLINE && (mode == 0 || mode == 3)) 818
	//{

	//	if(HasLeft && LineNum==2)//��������
	//	{
	//		CalXjjLx(lc0,xjj,lc2x,pt2x);
	//		xjj=fabs(xjj);
	//		dL+=xjj;
	//	}
	//	if(HasRight && LineNum==2)//��������
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
			if(xjj<0.0)//���
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
	
	if (fabs(LWid)>0.1)//�û�ָ���������ŵĿ��
		dL = LWid;
	if (fabs(Rwid)>0.0)//�û�ָ���������ŵĿ��
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

	if(mode==0)//С��
	{
		///////////////����������///////////

		makeline2(LT,RT,-1,lw,layname);
		makeline2(LB,RB,-1,lw,layname);
	}

	if(mode==3)//С��
	{
		///////////////����������///////////

		makeline2(LT,RT,-1,lw,layname);
		makeline2(LB,RB,-1,lw,layname);
		double Tab,Eab;
		xyddaa(LT.y,LT.x,RT.y,RT.x,&Tab);//������߽��
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

	if(mode==1)//�����·�ڵ�·֮�� �� ��·����·
	{
		///////////////����������///////////
		double len=lw/2;
		ads_name en1,en2;
		ads_point PT1,PT2,PT3,PT4;
		double Tab,Eab;

		makeline2(LT,RT,-1,lw,layname);
		ads_entlast(en1);
		makeline2(LB,RB,-1,lw,layname);
		ads_entlast(en2);

		xyddaa(LT.y,LT.x,RT.y,RT.x,&Tab);//������߽��
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

	if(mode==2)//�����·�ڵ�·֮��
	{
		makeline2(LT,RT,-1,lw,layname,L"DASHED2");
		makeline2(LB,RB,-1,lw,layname,L"DASHED2");
	}

	if (mode>3)//ƽ��
	{
		//������ƽ�����еľ��ο�
		if (mode!=9)
		{
			makeline2(LT,RT,-1,lw,layname);
			makeline2(LB,RB,-1,lw,layname);
			makeline2(LT,LB,-1,lw,layname);
			makeline2(RT,RB,-1,lw,layname);
		}

		if (mode==5 || mode==7)//����ƽ����˫����ƽ��
		{
			AcGePoint3d LT_PJ,RT_PJ;//���ƽ�����˵�
			getLineEndPt(LT,DrawParam.SCALE,aLb,LT_PJ);
			getLineEndPt(RT,DrawParam.SCALE,aLb,RT_PJ);
			makeline2(LT_PJ,RT_PJ,-1,lw,layname);
			makeDonut(AcGePoint2d(LT_PJ.x, LT_PJ.y), 0.6*lw, -1, layname);
			makeDonut(AcGePoint2d(RT_PJ.x, RT_PJ.y), 0.6*lw, -1, layname);
		}

		if (mode==6 || mode==7)//�ҿ���ƽ����˫����ƽ��
		{
			AcGePoint3d LB_PJ,RB_PJ;//�Ҳ�ƽ�����˵�
			getLineEndPt(LB,DrawParam.SCALE,aRe,LB_PJ);
			getLineEndPt(RB,DrawParam.SCALE,aRe,RB_PJ);
			makeline2(LB_PJ,RB_PJ,-1,lw,layname);
			makeDonut(AcGePoint2d(LB_PJ.x, LB_PJ.y), 0.6*lw, -1, layname);
			makeDonut(AcGePoint2d(RB_PJ.x, RB_PJ.y), 0.6*lw, -1, layname);
		}

		if (mode == 8)//�Զ���ƽ��
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

		if (mode == 9)//�½�ƽ����
		{
			makeline2(sPt,ePt,-1,lw,layname);
		}

	}

	///////////////�Ľ�///////////////////
	if (mode != 9)
	{
		makeline2(LT,LTe,-1,lw,layname);
		makeline2(LB,LBe,-1,lw,layname);
		makeline2(RT,RTe,-1,lw,layname);
		makeline2(RB,RBe,-1,lw,layname);
	}
} 

//�溭��:�������꣬����ҿ���ߡ��Ҹߣ�����·ǰ��������ת��λ�ǣ�����
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

	///////////////�Ľ�///////////////////
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


//���췽ʽ 0-��ͨ 1-������ 2-������ 3-�޿���ƽ�� 
//4-����ƽ�� 5-�ҿ���ƽ�� 6-˫����ƽ�� 7-�Զ���ƽ��

void RHA::INSERT_SMALL_BRIDGE(ACHAR gh[], double xclc0, CString NameCross, int LineNum, bool GJXZ
								   , int LMRFlag, int iRXD, double LTW, double RTW, double LLevel
								   , double RLevel, double jiao, int InsertStyle, double dCulvertUse) const
{
	ACHAR layname[256];
	int zybz = -1;//Ĭ�ϱ�ע����� 818

	_tcscpy(layname,  DrawParam.SmallBridge);

	////////////////////////////////////////////////////////////////	
	AcDbObjectIdArray IdAr;
	double lc0;//����ͳһ���
	ACHAR xlc0[50];
	CString lcchr;

	//����ͶӰ���
	_tcscpy(xlc0, LCchr(gh, xclc0, DrawParam.NLC));
	TrsCkmltoProjml(xlc0, lc0);

	double a0;//��·��λ��/�����߷�λ��
	xlpoint Ptmp;
	Ptmp.lc=lc0;
	xlpoint_pz(array, tarray, Ptmp);
	AcGePoint3d Pt0(Ptmp.y, Ptmp.x, 0.0);//�����������꣨���ߣ�
	a0 = Ptmp.a;

	//����ӳ�����-----------------------------------------------------------------------
	double FWJ_Jc = 0.0;//�ӳ������ķ�λ��
	if((InsertStyle == 0) && (fabs(LTW)>0.1) && (fabs(RTW)>0.1) 
		&& (m_XLName.Find(L"���")>=0))//0-��ͨ ���� 	
	{
		int pos = NameCross.Find(L"�ӳ�");
		if(pos>0)
			return;
	}
	

	//////////////////��ע///////
	AcGePoint3d eBZPt;//��ע�ߵ��յ㣬���ΪPt0;
	AcGePoint3d Pbz1, Pbz2;//��ע��;
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

	/*if (m_IsAutoLorR && pGTSJEX) 818 �Զ����㣬�����û�����
	{
		zybz = CalInsFlagLorR(Pt0.y, Pt0.x, m_XLName);
	}*/

	eBZPt[X]=Pt0.x+(WB+1.0*MaxStr*texth)*sin(a0+pi*0.5*zybz);//+0.5*texth*sin(B);
	eBZPt[Y]=Pt0.y+(WB+1.0*MaxStr*texth)*cos(a0+pi*0.5*zybz);//+0.5*texth*cos(B);
	makeline2(Pt0, eBZPt, -1, AcDb::kLnWt035, layname);

	//������������Ϊ�Ľ���С�ź����
	if ((m_XLName.Find(L"����")>=0) && GJXZ==true && InsertStyle>2.5)
	{
		double NCross, ECross;
		ECross = Pt0.x+(0.48*MaxStr*texth)*sin(a0+pi*0.5*zybz);
		NCross = Pt0.y+(0.48*MaxStr*texth)*cos(a0+pi*0.5*zybz);
		DrawOneFQDBZ(NCross, ECross, a0+pi*0.5, _tcslen(xlc0)*0.3*texth, LMRFlag);
	}

	//��ע�׿���ʽ�����
	xlpoint PZtmp;
	double B;

	PZtmp.lc=lc0-(0.5*texth+1.0*DrawParam.SCALE);//818 �ָ�
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

	maketext(Pbz1, Style, b, texth, -1, 3, L"����", layname);
	maketext(Pbz2, Lc, b, texth, -1, 3, L"����", layname);

	bool IsXWT = true;//�Ƿ���λͼ
	if (IsXWT)//818 ������λͼ��
	{
		AcGePoint3d sPt, ePt;//�����ն����ĵ�

		if(InsertStyle == 0)//��ͨС�ź�
		{
			if ((fabs(LTW)<0.1) || (fabs(RTW)<0.1))//���ҿ��û��ָ���������ǿ׿�ʽ�����С��š�����Ϊ��С��
			{
				//�����ų�
				CString LenStr;
				double Length;

				int sIX=NameCross.Find(L"-")+1;//�׿�ʽ�����ӣ� 1-1.5mԲ��
				int eIX=NameCross.Find(L"m")-1;

				if (sIX<0 || eIX<0)
				{
					CString Err;
					Err.Format(L"%sС�ź�%s�Ŀ׿�ʽ����ȱ��'-'��'m', ϵͳ����С�ź���Ĭ��Ϊ5m��\n", xlc0, NameCross); 
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
			else if (NameCross.FindOneOf(L"��")>-1)
			{
				if (LLevel<0.1 && RLevel<0.1)
				{
					ads_printf(L"%sС�ź�û��ָ���ų���ϵͳ����С�ź���Ĭ��Ϊ5m\n",  NameCross);
					LLevel = RLevel = 2.5;
				}

				getLineEndPt(Pt0, RLevel, a0, ePt);
				getLineEndPt(Pt0, LLevel, a0+PI, sPt);
				DrawSmallBridge(sPt, ePt, layname, LineNum, 0, LTW, RTW, LMRFlag, lc0);	
			}
			else
			{
				double FWJ=a0+rad(jiao);
				int pos = NameCross.Find(L"�ӳ�");

				if(pos > 0 && (XLShuXing == SJJX ||XLShuXing == SJ2X))
				{
					FWJ = FWJ_Jc;
				}

				
				if (NameCross.FindOneOf(L"�ӳ�")>-1)
				{
					//���ǽӳ�
					DrawCulvert(Pt0, LTW, RTW, FWJ, layname, true);
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					//����ͨ���������nXSȷ������˫����
					
					if (LTW<-0.001 || RTW<-0.001)
					{//û������򳤣��Զ�����
						LTW = 4.0;
						RTW = 4.0;
						//if (LMRFlag==MAINLINE)  818
						//{
						//	double resPt[2], lc2x;
						//	double xjj=0.0;

						//	double pt[3] ={Ptmp.x, Ptmp.y, 0.0};
						//	if(HasLeft && LineNum==2)//��������
						//	{

						//		pJYJX->Search_EXPt(arrayLx, tarrayLx, pt, FWJ, resPt);
						//		xjj = sqrt((resPt[0]-pt[0])*(resPt[0]-pt[0])+(resPt[1]-pt[1])*(resPt[1]-pt[1]));
						//		LTW=xjj+4;

						//	}
						//	if(HasRight && LineNum==2)//��������
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
					double AngEdgeLen=1.25*DrawParam.SCALE;//�����α߳�
					//��ע���ڱ��
					AcGePoint3d LPtText, RPtText;//���������涥�㣬ע��������
					AcGePoint3d LPt1, LPt2, RPt1, RPt2;//����������������

					//�������������涥�㣬ע��������
					getLineEndPt(Pt0, (LTW+2.0*AngEdgeLen), FWJ+PI, LPtText);
					getLineEndPt(Pt0, (RTW+2.0*AngEdgeLen), FWJ, RPtText);

					//��������������������  ��������  ��ע���ڱ��
					ACHAR LLevStr[20], RLevStr[20];
					if (fabs(LLevel)>0.01)
					{
						getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*1.125, LPt1);
						getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*0.875, LPt2);
						makeline(LPtText, LPt1, -1, AcDb::kLnWt000, layname);
						makeline(LPtText, LPt2, -1, AcDb::kLnWt000, layname);
						makeline(LPt1, LPt2, -1, AcDb::kLnWt000, layname);
						_stprintf(LLevStr, L" %.2lf", LLevel);
						maketext(LPtText, LLevStr, a0, texth, -1, 1, L"����", layname);
					}
					if (fabs(RLevel)>0.01)
					{
						getLineEndPt(RPtText, AngEdgeLen, FWJ+PI/8.0, RPt1);
						getLineEndPt(RPtText, AngEdgeLen, FWJ-PI/8.0, RPt2);
						makeline(RPtText, RPt1, -1, AcDb::kLnWt000, layname);
						makeline(RPtText, RPt2, -1, AcDb::kLnWt000, layname);
						makeline(RPt1, RPt2, -1, AcDb::kLnWt000, layname);	
						_stprintf(RLevStr, L" %.2lf", RLevel);
						maketext(RPtText, RLevStr, a0, texth, -1, 1, L"����", layname);
					}
				}
			}
		}
		else if((InsertStyle>0.5) && (InsertStyle<2.5))//��������
		{
			return;
			double FWJ=a0+rad(jiao);
			if(LTW>0.0001)//������Һ���û������Ĭ��Ϊ5
				getLineEndPt(Pt0, LTW, FWJ+PI, ePt);
			else
				getLineEndPt(Pt0, 5, FWJ+PI, ePt);
			if(RTW>0.0001)
				getLineEndPt(Pt0, RTW, FWJ, sPt);
			else
				getLineEndPt(Pt0, 5, FWJ, sPt);

			if(fabs(InsertStyle-2.0)<0.001)
				DrawSmallBridge(sPt, ePt, layname, LineNum, 2, LLevel, RLevel, LMRFlag, lc0);//��·����
			else
				DrawSmallBridge(sPt, ePt, layname, LineNum, 1, LLevel, RLevel, LMRFlag, lc0);

		}
		else//ƽ������
		{
			if (GJXZ == 0)//L"����"
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
		//����������ˮ�õ� 818
		//////////////////////////////////////////////////////////
		////plh08.20
		//AcGePoint3d sPt, ePt;//�����ն����ĵ�
		////if(NameCross.Find(L"��")>=0)//С��

		//if((InsertStyle>-0.5) && (InsertStyle<0.5))
		//{
		//	if ((fabs(LTW)<0.1) || (fabs(RTW)<0.1))
		//	{
		//		//�����ų�
		//		CString LenStr;
		//		double Length;
		//		int sIX=NameCross.Find(L"-")+1;
		//		int eIX=NameCross.Find(L"m")-1;
		//		if (sIX<0 || eIX<0)
		//		{
		//			CString Err;
		//			Err = xclc0 + L"С�ź�" + NameCross + "�Ŀ׿�ʽ����ȱ��'-'��'m', ϵͳ����С�ź���Ĭ��Ϊ5m��\n";
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
		//		int pos = NameCross.Find(L"�ӳ�");
		//		if(pos > 0 && (XLName == L"��ͨ��ƻ���" || XLName == L"��ͨ��Ƣ���"))
		//		{
		//			FWJ = FWJ_Jc;
		//		}

		//		if (dCulvertUse == 1)//��ͨ��
		//		{
		//			getLineEndPt(Pt0, 0.5*2, a0, ePt);
		//			getLineEndPt(Pt0, 0.5*2, a0+PI, sPt);

		//			DrawCulvert_JiaoTong(sPt, ePt, LTW, RTW, FWJ, layname);
		//		}
		//		else
		//		{
		//			if (NameCross.FindOneOf(L"�ӳ�")>-1)
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
		//			double AngEdgeLen=1.25*DrawParam.SCALE;//�����α߳�
		//			//��ע���ڱ��
		//			AcGePoint3d LPtText, RPtText;//���������涥�㣬ע��������
		//			AcGePoint3d LPt1, LPt2, RPt1, RPt2;//����������������

		//			//�������������涥�㣬ע��������
		//			getLineEndPt(Pt0, (LTW+2.0*AngEdgeLen), FWJ+PI, LPtText);
		//			getLineEndPt(Pt0, (RTW+2.0*AngEdgeLen), FWJ, RPtText);

		//			//��������������������  ��������  ��ע���ڱ��
		//			ACHAR LLevStr[20], RLevStr[20];
		//			if (fabs(LLevel)>0.01)
		//			{
		//				getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*1.125, LPt1);
		//				getLineEndPt(LPtText, AngEdgeLen, FWJ+PI*0.875, LPt2);
		//				makeline(LPtText, LPt1, -1, AcDb::kLnWt000, layname);
		//				makeline(LPtText, LPt2, -1, AcDb::kLnWt000, layname);
		//				makeline(LPt1, LPt2, -1, AcDb::kLnWt000, layname);
		//				_stprintf(LLevStr, L" %.2lf", LLevel);
		//				maketext(LPtText, LLevStr, a0, texth, -1, 1, L"����", layname);
		//			}
		//			if (fabs(RLevel)>0.01)
		//			{
		//				getLineEndPt(RPtText, AngEdgeLen, FWJ+PI/8.0, RPt1);
		//				getLineEndPt(RPtText, AngEdgeLen, FWJ-PI/8.0, RPt2);
		//				makeline(RPtText, RPt1, -1, AcDb::kLnWt000, layname);
		//				makeline(RPtText, RPt2, -1, AcDb::kLnWt000, layname);
		//				makeline(RPt1, RPt2, -1, AcDb::kLnWt000, layname);	
		//				_stprintf(RLevStr, L" %.2lf", RLevel);
		//				maketext(RPtText, RLevStr, a0, texth, -1, 1, L"����", layname);
		//			}
		//		}
		//	}

		//}
		////else if(NameCross.Find(L"��")>=0)//��
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
		//			Err = xclc0 + L"С�ź�" + NameCross + "�Ŀ׿�ʽ����ȱ��'-'��'m', ϵͳ����С�ź���Ĭ��Ϊ5m��\n";
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
		//		DrawSmallBridge(sPt, ePt, layname, LineNum, 2, LLevel, RLevel, LMRFlag, lc0);//��·����
		//	else
		//		DrawSmallBridge(sPt, ePt, layname, LineNum, 1, LLevel, RLevel, LMRFlag, lc0);
		//}
		////else if(NameCross.Find(L"ƽ��")>=0)//ƽ��
		//else if(InsertStyle>2.5)	
		//{

		//}
		////CreateGroup(NameLc,IdAr);

	}

}




//��ע�߼��
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

	//��עS���߼�� 818 ����
	for(int i=0;i+1<JDarray.size();i++)//818 ���һ�����㲻��ע?
	{
		//���жΡ����в�ƽ�жβ����߼��
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
			//ǰ������ֹ(������������)����������ֹ������ZH���м�
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
			//��������ʼ������HZ�������ʼ�����м�
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
			//���б��ڼ�ֱ���м�
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
		Pt1.y=PZ.x;//Pt1 ���ߵ�

		pRHA_BL=CCreatPM::IsGlobalXLHaveAndCreat(m_mdbName, JDarray[i].BLName);
		if (pRHA_BL == NULL)
			continue;

		lc1x=PROJ_ML(pRHA_BL->array,pRHA_BL->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PZ.lc=lc1x;
		xlpoint_pz(pRHA_BL->array,pRHA_BL->tarray,PZ);
		Pt2.x=PZ.y;
		Pt2.y=PZ.x;//Pt2 һ�ߵ�
		fwj=PZ.a;

		_stprintf(XJJ,L"%.2lf",fabs(JDarray[i].XJJ));//plh05.06

		if(_wtof(XJJ)<0.01)
			continue;

		PZ.lc=lc2x-0.8*texth;
		xlpoint_pz(array,tarray,PZ);


		//��ע�߼����ֵ�λ��
		Pt0.x=PZ.y+0.8*_tcslen(XJJ)*texth*sin(PZ.a+0.5*PI);
		Pt0.y=PZ.x+0.8*_tcslen(XJJ)*texth*cos(PZ.a+0.5*PI);
		angl=angleX(Pt1,Pt2);
		if(textdir<0)
			angl-=pi;
		
		maketext(Pt0,XJJ,angl,texth,-1,3,L"����",LayerName);


		//���ͷ
		Pt1.y = Pt1.y + offset * cos(fwj + 0.5*PI);
		Pt1.x = Pt1.x + offset * sin(fwj + 0.5*PI);
		makeXJJFlag(Pt1,Pt2,(1+_tcslen(XJJ))*texth,-1,0.3,LayerName,L"CONTINUOUS");		
	}

}


//��ע�߼�ࣺ1 ����һ�㣬2 ����һ�㣬��ע�߳���=20mm*SCALE����������
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

//���ý�������Ҫ��8.30-zy
void RHA::setJD_RL(int JDi ,double Ri,double L1i,double L2i )
{
	assertWriteEnabled();
	int NJD=JDarray.size();
	if (JDi>0 && JDi<NJD-1)  // ��Ϊ���յ�
	{  
		JDarray[JDi].l1=L1i;
		JDarray[JDi].l2=L2i;
		JDarray[JDi].R=Ri;
	}
	else
		AfxMessageBox(L"�ý���Ϊ�����յ㣬�޷����ð뾶����");
	CalDLprojml(0);//����㿪ʼ�����¼������ͶӰ���
} 

//����ͬ��Բ8.31-zy
//�����߼�ࣨ��������������ж�)���ӿ�ֵ���뾶����������ӿ��뾶����
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

		if (JiaKuan == 0.0)//����ӿ�ͬ����
		{
			HuanChang2 = HuanChang1;
			jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
			BanJing2 = (B + jianju) / 2.0;
			jianju = fabs(BanJing1 - BanJing2);
			if (jianju < fabs(XianJianJu))
			{
				ads_printf(L"�����߼��С����С�߼��\n");
				BanJing2 = BanJing1;
				HuanChang2 = HuanChang1;
				//BanJing2=BanJing2*isHTQX;
				return false;
			}
		}
		else//�ӿ�
		{
			if(XianJianJu > 0.0)//�������ڲ�
			{
				jianju=sqrt((BanJing1 - XianJianJu - JiaKuan) * (HuanChang1 * HuanChang1 / BanJing1 + 24.0 * JiaKuan));//�����ڲ໺��
				HuanChang2 = (int)(jianju/10.0+1.0)*10;//ȡ����10��
				jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
				BanJing2 = (B + jianju)/2.0;
				jianju=BanJing1 - BanJing2;
			}
			else if (XianJianJu < 0.0)//���������
			{
				if (HuanChang1 > 0.0)//�����˻��߻��������̶��߻���
				{
					//��С���߻���
					jianju = sqrt((BanJing1 - XianJianJu + JiaKuan) * (HuanChang1 * HuanChang1 / BanJing1 - 24.0 * JiaKuan));
					HuanChang2 = (int)(jianju / 10.0) * 10;

					//ji suan you xian ban jing
					B=BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
					jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
					BanJing2 = (B + jianju) / 2.0;
					jianju=BanJing1 - BanJing2;
				}
				else if (HuanChang2 > 0.0)//���߻���0����Ҫ�ɶ��߻����ӳ����߻���
				{
					jianju=sqrt(BanJing1 * (HuanChang2 * HuanChang2 / (BanJing1 - XianJianJu + JiaKuan) + 24.0 * JiaKuan));//�����ڲ໺��
					HuanChang1 = (int)(jianju / 10.0 + 1.0) * 10.0;//ȡ����10��

					//ji suan you xian ban jing
					B=BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
					jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
					BanJing2 = (B + jianju) / 2.0;
					jianju=BanJing1 - BanJing2;
				}
				else
				{
					//AfxMessageBox(L"���ߺͶ��ߵĻ�������0�����ܼ���\n");
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
		ads_printf(L"��ƻ��ߵ����߰뾶���󣬲��ܼ���\n");
		BanJing2 = BanJing1;
		HuanChang2 = HuanChang1;
		//BanJing2=BanJing2*isHTQX;
		return false;
	}
	//BanJing2=BanJing2*isHTQX;
	return true;
}



void RHA::CalOneBXJDdata(int iJD,const RHA *m_pm1)
//����ĳ���н���
////JDarray �Ƕ��߽�������
{
	//	assertWriteEnabled();
	double lo,lo1,D;
	int i=iJD;

	//isJYGuiFanCS��ȡֵ��
	bool isJYGuiFanCS=false;
	if (XLShuXing == JYJX || XLShuXing == JY2X)
		isJYGuiFanCS=true;

	if(JDarray[i].JDXZ==BXPX)//���жν���
	{
		//_tcscpy(JDarray[i].JDNum,JD2[i].JDNum);//��ʱ��ȥ��JDarray��JD2�ֱ����
		//int jdn1 = JDarray[i].jdno1;//��Ӧһ�߽������
		BAS_CALC_FUN ob;
		int jdn1;
		ob.GetJdno1FromJdstr(JDarray[i].BLJDName, m_pm1->JDarray, jdn1);//���ݶ��߽���BLJDName������ߣ�һ�ߣ��������jdn1
		int NJD=JDarray.size();
		if(i==NJD-1)
			JDarray[i].XJJ = JDarray[i-1].XJJ;//���һ������߼�����ǰһ�����//JDarray �Ƕ��ߣ�ֻ������xjj��
		double xjj = JDarray[i].XJJ;
		double EXJD[2];

		CalEXJD(jdn1, i, EXJD, m_pm1);//����ָ�����m_pm1
		JDarray[i].N = EXJD[0];
		JDarray[i].E = EXJD[1];//��������߽���
		double R = m_pm1->JDarray[jdn1].R;
		double JK;
		JK = get_XJJK(m_pm1->DV,R,isJYGuiFanCS);//�߼��ӿ�
		JDarray[i].QXJK = JK;
		JDarray[i].l1 = m_pm1->JDarray[jdn1].l1;
		JDarray[i].R = R;
		JDarray[i].l2 = m_pm1->JDarray[jdn1].l2;           
		lo1 = m_pm1->JDarray[jdn1].l1;//һ�߻���
		D = fabs(xjj);
		//���߰뾶
		if(i>0&&i<NJD-1)
		{
			double xjj2;
			xjj2 = JDarray[i-1].XJJ;//��һ�����߼��
			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
			if(fabs(xjj-xjj2)<0.01)//ǰ���߼�����
			{
				////xjj�������߼ӿ�............		   
				//if(xjj>0)//һ��Ϊ����
				//{
				//	if(m_pm1->QXB[jdn1].LorR<0)//��ת,II������࣬���ٻ���
				//	{
				//		//���仺��
				//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;	
				//		D  = -1.0*D;
				//	}
				//	else
				//	{
				//		//���仺��
				//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;						  					   
				//	}
				//}
				//else//һ��Ϊ����
				//{
				//	if(m_pm1->QXB[jdn1].LorR<0)//��ת,II�����ڲ�
				//	{
				//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;						  					   
				//	}
				//	else
				//	{   //���仺��
				//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
				//		JDarray[i].Lo1 = JDarray[i].Lo2 =  lo;	
				//		D  = -1.0*D;
				//	}
				//}
				////��뾶
				//double B = R - D + (lo1*lo1)/24/R;
				//JDarray[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶

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
		//JDarray[i].jdno = JD0 + i;//��ʱ����-8.31-zy-jdno�����ã�
	}//����
}


/*
�������ܣ���Ⲣ�жζ��߽���Ľ�������
���������һ�߽���ţ���Ӧ���߽����
������������߽�������
�㷨������1.�������·����������·�յ��ɶ�Ӧ���߽���������߼�����
2.��������ж�Ӧ���߽����ǰ��߸�����Ӧ���߼��ƫ��Ȼ���󽻣���������㼴Ϊ������Ӧ�Ľ�������
*/
//����һ�߽���ţ����߶�Ӧ����ţ����ز��жζ��߽�������
void RHA::CalEXJD(int jdn1,int njd2,double EXJD[2], const RHA *m_pm1)
{

	double xjj1,xjj2,fwj;

	if(jdn1<m_pm1->JDarray.size()-1)
		xyddaa(m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,m_pm1->JDarray[jdn1+1].N,m_pm1->JDarray[jdn1+1].E,&fwj);
	else
		xyddaa(m_pm1->JDarray[jdn1-1].N,m_pm1->JDarray[jdn1-1].E,m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,&fwj);

	if(njd2==0|| njd2==JDarray.size()-1)//��ʼ����//plh05.06
	{
		//�ɸõ���һ���ϵ�ͶӰ��̺��߼�����
		//Ҫ��GTZX2�����յ��ZHLCҪ��

		if(jdn1==0|| jdn1==m_pm1->JDarray.size()-1)
		{
			double xjj;
			xjj = JDarray[njd2].XJJ;
			EXJD[0]  =  m_pm1->JDarray[jdn1].N + xjj * cos(fwj+m_dHalfPI);
			EXJD[1]  =  m_pm1->JDarray[jdn1].E + xjj * sin(fwj+m_dHalfPI);//��������߽���
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
		xjj1 = JDarray[njd2-1].XJJ;//njd2-1-njd2�ߵ��߼��
		xjj2 = JDarray[njd2].XJJ;//njd2-njd2+1�ߵ��߼��

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
		//�ҽ���

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


//���ܣ��ж���ʰȡ������·��ʼ�߻�����ֹ��
//���룺pt           ʰȡ��
//�����SLorEL       ����ʼ�ߣ�Ϊtrue����֮Ϊfalse
//���أ��������ʼ�߻���ֹ�ߣ�����true��������ڣ�����false
//ls 17.9.4
bool RHA::IsPtonSLorEL(AcGePoint3d pt, bool &isSLorEL) const
{
	double projml=0.0;
	int jdNum=JDarray.size();

	isSLorEL=true;
	//����ͶӰ���
	TrsNEtoProjml(pt.y, pt.x, projml);

	if(JDarray.size() == 2)
	{//��·Ϊһ��ֱ�߱ߵ����
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

//���ܣ��ϲ���������
//���룺pt           ʰȡ��
//�����SLorEL       ����ʼ�ߣ�Ϊtrue����֮Ϊfalse
//���أ��������ʼ�߻���ֹ�ߣ�����true��������ڣ�����false
//ls 17.9.4
bool RHA::CombineJD(vector<JDarrayStruct> &jdAry) 
{
	//�ж����յ��ǰ�󽻵�
	int jdNum=jdAry.size();
	int sJD=0, eJD=0;

	if (jdNum<3)
		return false;

	double sProjml=0.0, eProjml=0.0;
	sProjml=PROJ_ML(array, tarray, jdAry[0].N, jdAry[0].E);
	eProjml=PROJ_ML(array, tarray, jdAry[jdNum-1].N, jdAry[jdNum-1].E);

	sJD=DistToNearJD(sProjml, true);
	eJD=DistToNearJD(eProjml, false);

	//�ж����յ������жλ��ǲ��ж�
	int sRxd=0, eRxd=0;
	int rxdSJD=0, rxdJDNum=0;
	int qrxdSJD=0, qrxdEJD=0, hrxdSJD=0, hrxdEJD=0;

	sRxd=GetiRxdByProjml(sProjml);
	eRxd=GetiRxdByProjml(eProjml);

	int type=QHBX_AddRX;
	bool isLSXSJDAsRXS=false;
	bool isLSXEJDAsRXE=false;

	if (sRxd<0 && eRxd<0)
	{//���յ㶼�ڲ��ж��� 818 ��ʱ����
		/*if(AfxMessageBox(L"�Ƿ񽫲�����·����Ϊ���жΣ������ϵͳ�����벢�в�ƽ�ж�", MB_YESNO) == IDYES)
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
		AfxMessageBox(L"������·ʱ���յ㲻��ѡ��Ŀ����·�����ж��ϣ����ڲ��ж��ϲ�����·");
		return false;
	}
	//else if (sRxd>-1 && eRxd>-1)//818 ��ʱ����
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

	//������������ 818
	for (int i=1; i<jdNum-1; i++)
		_stprintf(jdAry[i].JDName, L"%s-%d", JDarray[sJD].JDName, i);

	//�м佻�㸳��·��
	for (int i=1; i<jdNum-1; i++)
		_tcscpy(jdAry[i].XLName, m_XLName);

	//����������
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

		//������һ������Ϊ����ʼ
		if(isLSXSJDAsRXS)
		{
			jdAry[1].JDXZ=RX_Start;
			_tcscpy(jdAry[1].BLName, JDarray[sJD+1].BLName);
			_tcscpy(jdAry[1].BLJDName, JDarray[sJD+1].BLJDName);
			jdAry[1].XJJ=JDarray[sJD+1].XJJ;
		}

		//�������һ������Ϊ����ֹ
		if (isLSXEJDAsRXE)
		{
			jdAry[jdNum-2].JDXZ=RX_End;
			_tcscpy(jdAry[jdNum-2].BLName, JDarray[eJD-1].BLName);
			_tcscpy(jdAry[jdNum-2].BLJDName, JDarray[eJD-1].BLJDName);
			jdAry[jdNum-2].XJJ=JDarray[eJD-1].XJJ;
		}

	}

	//�ϲ�����

	//ɾ������
	for (int i=eJD-1; i>sJD; i--)
		JDarray.erase(JDarray.begin()+i);
	//������ʱ�߽���
	for (int i=jdNum-2; i>0; i--)
		JDarray.insert(JDarray.begin()+sJD+1, jdAry[i]);

	//������Ԫ
	getpxy_DATA();

	//ɾ���Ӳ������ϸ��ƹ����Ķ���
	for(int i=0;i<DLarray.size();i++)
	{
		if(DLarray[i].Indx == -3)
		{
			DLarray.erase(DLarray.begin()+i);
			i--;
		}
	}


	//���¶���
	if (type == InOneRXD)
	{//
		UpdateLC(sRxd);
	}
	else if (type == QHBX_AddRX)
	{
		int iRxd=0;
		iRxd=FindiRXD(sJD+1);

		//ɾ��������·���յ�֮��Ķ���
		for(int i=0;i<DLarray.size();i++)
		{
			if (DLarray[i].TYLC>sProjml+0.001 && DLarray[i].TYLC+0.001<eProjml)
			{
				DLarray.erase(DLarray.begin()+i);//818 ���������⣬����ɾ��һЩ��������
				i--;
			}
		}

		//�޸����жα��:֮������жδ�iRxd+1��ʼ���
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


		//���ɶ���
		DLTab rxdDL[2];
		GeniRxdDL(iRxd, rxdDL);

		//�������
		int iDL=0;
		iDL=GetPreDL(rxdDL[0].TYLC);
		iDL++;

		DLarray.insert(DLarray.begin()+iDL, rxdDL[1]);
		DLarray.insert(DLarray.begin()+iDL, rxdDL[0]);

		//���¼�������ļ������
		CalDLprojml(iDL+2);

		//��־���
		if(XLShuXing != SJJX && XLShuXing != SJ2X)
		{
			//ɾ����־���
			//ɾ��������·���յ�֮��Ķ���
			for(int i=0;i<BZLCarray.size();i++)
			{
				if (BZLCarray[i].xyToprojml>sProjml+0.001 && BZLCarray[i].xyToprojml+0.001<eProjml)
				{
					BZLCarray.erase(BZLCarray.begin()+i);//818 ���������⣬����ɾ��һЩ����
					i--;
				}
			}

			//�����¶����ı�־���
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
				tmpBZLC.kmlTocml=TYLC1(tmpBZLC.CKml, DLarray);//�ú���ֻ���������ж�

				BZLCarray.push_back(tmpBZLC);
			}
		}

	}
	else if (type == QRHB)
	{
		//ɾ�����ж��յ㵽Դ��·�յ�֮��Ķ���

		//�������жα��

		//�������жζ���
		UpdateLC(sRxd);
	}
	else if (type == QBHR)
	{

	}
	else if (type == ConTwoRXD)
	{

	}


}

//����ָ���������������ж�
int RHA::FindiRXD(int iJD)
{
	int iRXD=-1;//�ڼ������ж�
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


//���ܣ�������·ʱ������·��Ŀ����·�ϵ����յ�����
//���룺
//tmpJDAry           Դ��·�Ľ�������
//tmpQXBAry          Դ��·�����߱�����
//�����
//tmpJDAry           �޸����ս������꣨�������Ŀ����·�����Դ��·�Ľ�������
//���أ�������ܲ��뷵��false����֮����true
//�㷨������Դ��·����ʼ�ߺ���ֹ���Ƿ��Ŀ����·�ص�������ص�
//      ������������������������Բ���
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
	int iJD1=-1, iJD2=-1;//Դ��·���յ���Ŀ����·�ϵ�ֱ�߱����Ľ����±�

	
	sptX=tmpJDAry[0].N;
	sptY=tmpJDAry[0].E;
	eptX=tmpJDAry[1].N;
	eptY=tmpJDAry[1].E;

	//�ж���ʼ���Ƿ���Դ��·�ص�
	for (int i=0; i+1<JDarray.size(); i++)
	{
		isCD=IsZXCD(sptX, sptY, eptX, eptY, JDarray[i].N, JDarray[i].E, JDarray[i+1].N, JDarray[i+1].E);
		if (isCD)
		{
			iJD1=i;
			isCD=false;//�ٸ������������ж��ص��Ƿ����

			//�жϺϲ����Ƿ������أ���û�м�ֱ��
			TrsProjmltoNE(QXB[i].KHZ, hzX, hzY);//��Ŀ����·i����Ļ�ֱ������

			dist1=DistBtwnPts(hzX, hzY, eptX, eptY);//Ŀ����·�ϻ�ֱ�㵽Դ��·�ڶ�������ľ���
			if (dist1+0.0001>tmpQXBAry[1].T1)//��֤�м�ֱ��
			{
				//�ж�Ŀ����·�ϻ�ֱ���Ƿ���Դ��·��һ������ͬ�ࣨ�����Դ��·�ڶ������㣩
				fwj1=CalFWJ(hzY, hzX, eptY, eptX);
				fwj2=CalFWJ(sptY, sptX, eptY, eptX);

				if(fabs(fabs(fwj1-fwj2)-pi)<0.01)
				{
					AfxMessageBox(L"��ʱ��ʼ�߷�����Ŀ����·�෴���뿼���Ƿ���ʱ�߷�������");
					return false;
				}
				else if(fabs(fwj1-fwj2)<0.1)
				{
					isCD=true;
					//���Դ��·����ڻ�ֱ����ࣨ�����Դ��·�ڶ������㣩�������������Ϊ��ֱ��
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
				AfxMessageBox(L"Դ��·��ʼ�߲���ʱ�����ص�����ֱ�߲�����Ҫ��");
				return false;
			}
			
			break;
		}
	}

	if(!isCD)
	{
		AfxMessageBox(L"Դ��·��ʼ����δ���ӣ�����ʹ��������·����");
		return false;
	}


	//�ж���ֹ���Ƿ���Դ��·�ص�
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
			isCD=false;//�ٸ������������ж��ص��Ƿ����

			//�жϺϲ����Ƿ������أ���û�м�ֱ��
			TrsProjmltoNE(QXB[i+1].KZH, zhX, zhY);//��Ŀ����·i+1�����ֱ��������

			dist1=DistBtwnPts(zhX, zhY, sptX, sptY);//Ŀ����·��ֱ���㵽Դ��·�����ڶ�������ľ���
			if (dist1+0.0001>tmpQXBAry[tmpJDNum-2].T2)//��֤�м�ֱ��
			{
				//�ж�Ŀ����·��ֱ�����Ƿ���Դ��·�յ�ͬ�ࣨ�����Դ��·�����ڶ������㣩
				fwj1=CalFWJ(zhY, zhX, sptY, sptX);
				fwj2=CalFWJ(eptY, eptX, sptY, sptX);

				if(fabs(fabs(fwj1-fwj2)-pi)<0.01)
				{
					AfxMessageBox(L"��ʱ���ձ߷�����Ŀ����·�෴���뿼���Ƿ���ʱ�߷�������");
					return false;
				}
				if(fabs(fwj1-fwj2)<0.1)
				{
					isCD=true;
					//���Դ��·�յ���ֱ������ࣨ�����Դ��·�����ڶ������㣩�������յ�����Ϊֱ����
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
				AfxMessageBox(L"Դ��·���ձ߲���ʱ�����ص�����ֱ�߲�����Ҫ��");
				return false;
			}

			break;
		}
	}

	if(!isCD)
	{
		AfxMessageBox(L"Դ��·���ձ���δ���ӣ�����ʹ��������·����");
		return false;
	}

	if(iJD2<iJD1)
	{
		AfxMessageBox(L"���Ƚ�Դ��·����Ȼ���ٲ�����·");
		return false;
	}

	return true;
}

/*---------------------------------------------------*/
//ȫ�����귨	���Ʋ�����غ���	ljx	17.10.18
/*===================================================================
������:DrawCircle()		����Բ����
------------------------------------------------------------
��������:����ȫ�����귨�и����������λ�õ�Բ
------------------------------------------------------------
����:m_MeasurePoints	���vector
------------------------------------------------------------
���:��
------------------------------------------------------------
�㷨����:       
------------------------------------------------------------
��д����:17.10.18
------------------------------------------------------------
��ע�� 
=====================================================================*/
void RHA::DrawCircle(AcGiWorldDraw *mode)
{
	//1.���ø��ֲ���
	vector<SSurPt>::iterator Beg = m_MeasurePoints.begin();
	AcGePoint3d CircleCenter(0, 0, 0);
	AcGeVector3d Normal(0, 0, 1);
	int CircleColor = 0;
	double R = 8.0;

	//2.�������в����л���
	while(Beg != m_MeasurePoints.end())
	{
		CircleCenter.x = Beg->E;
		CircleCenter.y = Beg->N;
		//���ݲ�����ڲ�ͬ���ԣ�ֱ��/Բ����/�������ߣ�ΪԲѡ��ͬ����ɫ
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
������:CalRectification()		���㲦������
------------------------------------------------------------
��������:�����������и�������������·�ľ��룬������
------------------------------------------------------------
����:m_MeasurePoints	���vector
------------------------------------------------------------
���:Rectification		����vector
------------------------------------------------------------
�㷨����:ͨ��RHA��PROJ_ML����������ͶӰ����·�ϵ�ͶӰ��̣�
		 ��ͨ��xlpoint_pz���������ת��Ϊ��Ӧ���x��y���꣬
		 �������롣
------------------------------------------------------------
��д����:17.10.18
------------------------------------------------------------
��ע�� 
=====================================================================*/
void RHA::CalRectification(vector<double>& Rectification)
{
	//1.���ø��ֲ���
	xlpoint ProjectPt;		//��·�㣬���Դ���
	double Mileage = 0;		//����Ӧ����·�ϵ�ͶӰ���
	Rectification.clear();	//��ղ���vector���Ա�洢����

	//2.�������ÿһ������������·�Ĳ���
	vector<SSurPt>::iterator Beg = m_MeasurePoints.begin();	//������
	while (Beg != m_MeasurePoints.end())
	{
		double tempE = Beg->E;		//�����E�������tempE
		double tempN = Beg->N;		//�����N�������tempN
		Mileage = PROJ_ML(array, tarray, tempN, tempE, tempN, tempE);			//ͨ��RHA��PROJ_ML������Ӧ��ͶӰ���
		ProjectPt.lc = Mileage;		//������̴�����·��ṹ����
		xlpoint_pz(array, tarray, ProjectPt);									//���øú�������ڶ�Ӧ����µ���·��
		AcGePoint2d AcGeMeasurePoint(tempN, tempE), AcGeRHAPoint(ProjectPt.x, ProjectPt.y);					//��������AcGePoint2d�㣬ע��NE��xy��Ӧ��ϵ
		double Distance = AcGeRHAPoint.distanceTo(AcGeMeasurePoint);										//����룬��Ϊ����
		Rectification.push_back(Distance);																	//����vector��
		Beg++;
	}
}
/*===================================================================
������:DrawRectificationText()	���Ʋ�������
------------------------------------------------------------
��������:��ÿ�����Ĳ������Ƶ���·��Ӧλ�ã�������·�յ㴦
		 ����һ�����ܱ�
------------------------------------------------------------
����:Rectification		����vector
	 m_MeasurePoints	���vector
------------------------------------------------------------
���:��
------------------------------------------------------------
�㷨����:       
------------------------------------------------------------
��д����:17.10.18
------------------------------------------------------------
��ע�� 
=====================================================================*/
void RHA::DrawRectificationText(vector<double>& Rectification, AcGiWorldDraw* mode)
{
	//1.���ò���
	ACHAR text[50];									//���ֲ���
	AcGePoint3d SingleMeasurePoint(0, 0, 0);		//��ͼ��
	AcGeVector3d Normal(0.0, 0.0, 1.0);				//��������
	AcGeVector3d direction(1.0, 0.0, 1.0);			//���ַ��򣬴˴�ͳһȡˮƽ����
	AcGiTextStyle pTextStyle;						//������������
	pTextStyle.setFont(L"����", 0, 0, 134, 2);		//���ָ�ʽ
	pTextStyle.setTextSize(5);						//���ָ߶�

	int ChangeNum[7];
	for(int i = 0; i < 7;i++)
		ChangeNum[i] = 0;

	//2.��ÿ�������в�������Ż��ƣ�����¼��Ӧ������Χ������ͳ��ʹ��
	for(int i = 0; i + 1 < Rectification.size(); i++)
	{
		SingleMeasurePoint.x = m_MeasurePoints[i].E;
		SingleMeasurePoint.y = m_MeasurePoints[i].N;

		//�ٻ��Ʋ���
		_stprintf(text, L"%0.0f", Rectification[i] * 1000);
		mode->subEntityTraits().setColor(2);
		mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

		//�ڻ������
		SingleMeasurePoint.x += 8;
		_stprintf(text, L"%d", i);
		mode->subEntityTraits().setColor(1);
		mode->geometry().text(SingleMeasurePoint, Normal, direction, text, _tcslen(text), Adesk::kTrue, pTextStyle);

		//���ж����ڶ��ٲ�������
		if(Rectification[i] >= 300)
			ChangeNum[6]++;
		else
			ChangeNum[(int)(Rectification[i] / 50)]++;
	}

	//3.���ƻ���
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