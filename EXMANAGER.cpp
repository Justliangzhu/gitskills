// EXMANAGER.cpp: implementation of the EXMANAGER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EXMANAGER.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EXMANAGER::EXMANAGER()
{
	MainLine=NULL;
	FLDNum=0;
	NeedRemoveAll=false;
}

EXMANAGER::~EXMANAGER()
{
	Release();
}

void EXMANAGER::Release()
{
	NeedRemoveAll=true;
	if(NeedRemoveAll)
	{
		for(int i=0;i<FLDNum;i++)
		{
			if(FLDArray[i]!=NULL)delete FLDArray[i];
		}
	}
	FLDArray.RemoveAll();
	DuanLuoArray.RemoveAll();
	FLDNum=0;
}

void EXMANAGER::SortFLDArray()
{
	FLDNum=FLDArray.GetSize();
	if(FLDNum<1)return;
	if(FLDNum==1)
	{
		FLDArray[0]->FLDNo=1;
		return;
	}
	//sort
	int i;
	for(i=0;i<FLDNum-1;i++)
	{
		for(int j=i+1;j<FLDNum;j++)
		{
			if(FLDArray[i]->FLDNo>FLDArray[j]->FLDNo)
			{
				JD_CENTER *tmpJC;
				tmpJC=FLDArray[i];
				FLDArray[i]=FLDArray[j];
				FLDArray[j]=tmpJC;
			}
		}
	}
	//new NO
	for(i=0;i<FLDNum;i++)FLDArray[i]->FLDNo=i+1;
}
//以下为了从数据库读取数据生成二线实体
bool EXMANAGER::GetiFLDJDCenInfo(int iFLD,JdCenter *JCAy, int num,int &sJCNo,int &eJCNo)
{
	FLDNum=JCAy[num-1].FLDNo;
	if(iFLD<1 || iFLD>FLDNum)return false;
	int i=0;
	for(i=0;i<num;i++)if(JCAy[i].FLDNo==iFLD)break;
	sJCNo=i;
	for(i=num-1;i>=0;i--)if(JCAy[i].FLDNo==iFLD)break;
	eJCNo=i;
	if(sJCNo>=0 && eJCNo<num && eJCNo>=0 && eJCNo<num && sJCNo<eJCNo)return true;
	else return false;
}

bool EXMANAGER::GetiFLDDLInfo(int iFLD,DLBZ *DLAy, int num,int &sDLNo,int &eDLNo)
{
	FLDNum=DLAy[num-1].FLDNo;
	if(iFLD<1 || iFLD>FLDNum)return false;
	int i=0;
	for(i=0;i<num;i++)if(DLAy[i].FLDNo==iFLD)break;
	sDLNo=i;
	for(i=num-1;i>=0;i--)if(DLAy[i].FLDNo==iFLD)break;
	eDLNo=i;
	if(sDLNo>=0 && eDLNo<=num && eDLNo>=0 && eDLNo<=num && sDLNo<eDLNo)return true;
	else return false;
}

void EXMANAGER::Set(JdCenter *JCAy, int numJC,DLBZ *DLAy,int numDL)
{	
	NeedRemoveAll=true;
	FLDNum=JCAy[numJC-1].FLDNo;
	FLDArray.RemoveAll();
	for(int iFLD=1;iFLD<=FLDNum;iFLD++)
	{
		int sJCNo,eJCNo;
		if(GetiFLDJDCenInfo(iFLD,JCAy,numJC,sJCNo,eJCNo))
		{
			int iJCNum=eJCNo-sJCNo+1;
			JdCenter *newJCAy=new JdCenter[iJCNum];
			for(int j=sJCNo;j<=eJCNo;j++)
			{
				newJCAy[j-sJCNo]=JCAy[j];					
			}
			JD_CENTER *pJC=new JD_CENTER;
			pJC->SetJdCenterArray(newJCAy,iJCNum);
			pJC->FLDNo=iFLD;
			delete []newJCAy;
			int sDLNo,eDLNo;
			if(GetiFLDDLInfo(iFLD,DLAy,numDL,sDLNo,eDLNo))
			{
				int iDLNum=eDLNo-sDLNo+1;
				DLBZ *newDLAy=new DLBZ[iDLNum];
				for(int j=sDLNo;j<=eDLNo;j++)
				{
					newDLAy[j-sDLNo]=DLAy[j];					
				}
				pJC->setDLB(newDLAy,iDLNum);
				delete []newDLAy;
			}
			FLDArray.Add(pJC);
		}
	}
}

void EXMANAGER::SetDLB(DLBZ *DLAy,int numDL)
{	
	assertWriteEnabled();
	FLDNum=FLDArray.GetSize();
	for(int iFLD=1;iFLD<=FLDNum;iFLD++)
	{
		int sDLNo,eDLNo;
		if(GetiFLDDLInfo(iFLD,DLAy,numDL,sDLNo,eDLNo))
		{
			int iDLNum=eDLNo-sDLNo+1;
			DLBZ *newDLAy=new DLBZ[iDLNum];
			for(int j=sDLNo;j<=eDLNo;j++)
			{
				newDLAy[j-sDLNo]=DLAy[j];					
			}
			JD_CENTER *pJC=FLDArray[iFLD-1];
			pJC->setDLB(newDLAy,iDLNum);
			delete []newDLAy;
		}
	}
}

void EXMANAGER::CreateDuanLuo()
{
	FLDNum=FLDArray.GetSize();
	if(MainLine!=NULL && FLDNum>0)
	{
		DuanLuoArray.SetSize(FLDNum);
		for(int i=0;i<FLDNum;i++)
		{
			JD_CENTER *pJC=FLDArray[i];
			DUANLUO *pDuanLuo=&DuanLuoArray[i];
			pDuanLuo->StartTYLC1x=MainLine->PROJ_ML(pJC->XYArray[1][8],pJC->XYArray[1][9]);
			pDuanLuo->EndTYLC1x=MainLine->PROJ_ML(pJC->XYArray[pJC->XYNum][8],pJC->XYArray[pJC->XYNum][9]);
		}
	}
}

double EXMANAGER::CalXJJ(double tylc1x,double &projml)
{
	projml=-99999999.0;
	if(MainLine==NULL)return 0.0;
	if(FLDNum<=0)return 0.0;
	else if(DuanLuoArray.GetSize()<=0)
	{
		CreateDuanLuo();
	}
	for(int i=0;i<FLDNum;i++)
	{
		DUANLUO *pDuanLuo=&DuanLuoArray[i];
		JD_CENTER *pJC=FLDArray[i];
		if(tylc1x>=pDuanLuo->StartTYLC1x && tylc1x<=pDuanLuo->EndTYLC1x)
		{
			double xjj,resPt[2];
			BAS_DRAW_FUN::CalXjj(tylc1x,MainLine->XYArray,MainLine->XYNum,
				pJC->XYArray,pJC->XYNum,xjj,projml,resPt);
			return xjj;
		}
	}
	return 0.0;
}

Adesk::Boolean EXMANAGER::subWorldDraw(AcGiWorldDraw* pWd)
{ 
	SortFLDArray();
	FLDNum=FLDArray.GetSize();
	for(int i=0;i<FLDNum;i++)
	{
		if(FLDArray[i]!=NULL)
		{			
			JD_CENTER *pJC=FLDArray[i];
			if(pWd)pJC->worldDraw(pWd);
			else 
			{
				pJC->CalXYArray();
				pJC->Draw();
			}
		}
	}
	if(pWd)	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
	else return Adesk::kTrue;
}

Acad::ErrorStatus EXMANAGER::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our ow
	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
	{
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}
	//////////////////////////////////////////////////////////////////////////
	int i=0;
	pFiler->readItem(&FLDNum);
	FLDArray.SetSize(FLDNum);
	for(i=0;i<FLDNum;i++)
	{
		FLDArray[i]=new JD_CENTER;
		FLDArray[i]->dwgInFields(pFiler);
	}			
	void *iFLDObj;
	pFiler->readAddress(&iFLDObj);
	MainLine=(JD_CENTER*)iFLDObj;
	//////////////////////////////////////////////////////////////////////////	
	return pFiler->filerStatus();
}


// Files data out to a DWG file.
//
Acad::ErrorStatus EXMANAGER::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
	{
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	}
	//////////////////////////////////////////////////////////////////////////
	int i=0;
	pFiler->writeItem(FLDNum);
	for(i=0;i<FLDNum;i++)
	{
		FLDArray[i]->dwgOutFields(pFiler);
	}
	pFiler->writeAddress(MainLine);
	//////////////////////////////////////////////////////////////////////////
	return pFiler->filerStatus();
}


/*
// Files data in from a DXF file.
//
Acad::ErrorStatus EXMANAGER::dxfInFields(AcDbDxfFiler* pFiler)
{
assertWriteEnabled();
const char *chr=NULL;
Acad::ErrorStatus es;
if ((es = AcDbObject::dxfInFields(pFiler))
!= Acad::eOk)
{        return es;    }
AfxMessageBox(L"不能存取!");	
return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus EXMANAGER::dxfOutFields(AcDbDxfFiler* pFiler) const
{
assertReadEnabled();
AcDbObject::dxfOutFields(pFiler);
AfxMessageBox(L"不能存取!");	
return pFiler->filerStatus();
}
*/

void EXMANAGER::subList() const 
{
	assertReadEnabled();
	//acutPrintf(L"\n 类：\t%s",isA()->name());
	acutPrintf(L"\n 类: \t分离段设计线实体");
	acutPrintf(L"\n共有 %d 个分离段",FLDNum);
}

Acad::ErrorStatus EXMANAGER::subExplode(AcDbVoidPtrArray& entitySet) const
{
	assertReadEnabled();
	Acad::ErrorStatus es = Acad::eOk;
	for(int i=0;i<FLDNum;i++)
	{
		JD_CENTER *pJC=new JD_CENTER;
		FLDArray[i]->CloneJC(pJC);
		entitySet.append(pJC);
	}
	return es;
}

int EXMANAGER::GetJDCenNum()
{
	int JDCenNum=0;
	for(int i=0;i<FLDNum;i++)JDCenNum+=FLDArray[i]->JdCenNum;
	return JDCenNum;
}

int EXMANAGER::GetDLNum()
{
	int DLNum=0;
	for(int i=0;i<FLDNum;i++)DLNum+=FLDArray[i]->DLNum;
	return DLNum;
}

int EXMANAGER::GetiFLDFromNE(double N, double E)
	//由径距、纬距，求所处绕行段序号（0开始）
{
	CreateDuanLuo();
	int iFLD;//要删除的绕行段序号
	if(MainLine)
	{
		double tylc1x=MainLine->PROJ_ML(N,E);
		FLDNum=DuanLuoArray.GetSize();
		for(iFLD=0;iFLD<FLDNum;iFLD++)
		{
			if(tylc1x>=DuanLuoArray[iFLD].StartTYLC1x
				&& tylc1x<=DuanLuoArray[iFLD].EndTYLC1x)
			{
				acutPrintf(L"第%d个分离段!\n",iFLD+1);
				return iFLD+1;
			}
		}
	}
	return -1;
}

void EXMANAGER::DeliFLD(int iFLD)
{
	assertWriteEnabled();
	if(iFLD>=1 && iFLD<=FLDNum)FLDArray.RemoveAt(iFLD-1);
}

Acad::ErrorStatus EXMANAGER::subGetGripPoints(AcGePoint3dArray& gripPoints,
											  AcDbIntArray& osnapModes,
											  AcDbIntArray& geomIds)const
{
	assertReadEnabled();
	///////////////////////////////////////////
	for(int i=0;i<FLDNum;i++)
	{
		JD_CENTER *pJC=FLDArray[i];
		pJC->getGripPoints(gripPoints,osnapModes,geomIds);
	}
	/////////////////////////////////////////////
	return Acad::eOk;
}

Acad::ErrorStatus EXMANAGER::subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset)
{ 
	assertWriteEnabled();
	///////////////////////////////////////////
	int num=indices.length();
	if(num==1)
	{
		int k=indices[0];
		for(int i=0;i<FLDNum;i++)
		{
			JD_CENTER *pJC=FLDArray[i];
			if(k>2*pJC->JdCenNum-2)
			{
				k-=(2*pJC->JdCenNum-2);
				continue;
			}
			else
			{
				AcDbIntArray indices0;
				indices0.insertAt(0,k);
				AcGeVector3d offset0=offset;
				pJC->moveGripPointsAt(indices0,offset0);
				return Acad::eOk;
			}
		}
	}	
	/////////////////////////////////////////////
	return Acad::eOk;
}

bool EXMANAGER::CloneFLD(int iFLD,JD_CENTER *pJC)
{
	if(iFLD<FLDNum && pJC!=NULL)
	{
		JD_CENTER *fJC=NULL;

		if(iFLD>=0)fJC=FLDArray[iFLD];
		else fJC=MainLine;

		return fJC->CloneJC(pJC);
	}
	return false;
}


