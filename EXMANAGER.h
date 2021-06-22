// EXMANAGER.h: interface for the EXMANAGER class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXMANAGER_H__65FF494F_8C0F_4A97_BDFD_6E635EDACA84__INCLUDED_)
#define AFX_EXMANAGER_H__65FF494F_8C0F_4A97_BDFD_6E635EDACA84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
#include "JD_CENTER.h"
//#define AppName  /*MSG1*/"ROAD" 
//�������з���ε��Զ���ʵ��
class EXMANAGER: public BAS_DRAW_FUN , public AcDbEntity  
{
public:
	bool CloneFLD(int iFLD,JD_CENTER *pJC);//��¡һ�������
	void Release();//�ͷſռ�
	void SetDLB(DLBZ *DLAy,int numDL);//�������з���εĶ���
	void DeliFLD(int iFLD);//ɾ��ָ�������
	int GetiFLDFromNE(double N, double E);//��ȡָ�����괦�ķ�������
	void Set(JdCenter *JCAy, int numJC,DLBZ *DLAy,int numDL);//�������з���εĿ��Ƶ�Ͷ�������		
	int GetJDCenNum();//��ȡ���Ƶ���Ŀ
	int GetDLNum();//��ȡ������Ŀ
private:
	//��ȡָ������εĿ��Ƶ����ݼ����տ��Ƶ����	
	bool GetiFLDJDCenInfo(int iFLD,JdCenter *JCAy, int num,int &sJCNo,int &eJCNo);
	//��ȡָ������εĶ������ݼ����ն������
	bool GetiFLDDLInfo(int iFLD,DLBZ *DLAy, int num,int &sDLNo,int &eDLNo);
	bool NeedRemoveAll;
//////////////////////////////////////////////////////////////////////////	
public:
	ACRX_DECLARE_MEMBERS(EXMANAGER);
	EXMANAGER();
	virtual ~EXMANAGER();	
//////////////////////////////////////////////////////////////////////////
	virtual Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const;	
    virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
//    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
//    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		AcDbIntArray& osnapModes,
		AcDbIntArray& geomIds) const;
	
    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		const AcGeVector3d& offset);	
	
    virtual void subList() const;
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd=NULL);	
public:
	//����ָ����̴�������ϵ�ͶӰ��̼��߼��
	double CalXJJ(double tylc1x,double &projml);
	//�����з��������
	void SortFLDArray();
	JD_CENTER *MainLine;//����ָ��
	CArray<JD_CENTER *,JD_CENTER *>FLDArray;//��������
	Adesk::Int16 FLDNum;//�������Ŀ
private:
	//���ɶ���
	void CreateDuanLuo();
	CArray<DUANLUO,DUANLUO> DuanLuoArray;//����
};
int MAKE_ACDBOPENOBJECT_FUNCTION(EXMANAGER);
#endif // !defined(AFX_EXMANAGER_H__65FF494F_8C0F_4A97_BDFD_6E635EDACA84__INCLUDED_)
