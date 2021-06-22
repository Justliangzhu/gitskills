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
//管理所有分离段的自定义实体
class EXMANAGER: public BAS_DRAW_FUN , public AcDbEntity  
{
public:
	bool CloneFLD(int iFLD,JD_CENTER *pJC);//克隆一个分离段
	void Release();//释放空间
	void SetDLB(DLBZ *DLAy,int numDL);//设置所有分离段的断链
	void DeliFLD(int iFLD);//删除指定分离段
	int GetiFLDFromNE(double N, double E);//获取指定坐标处的分离段序号
	void Set(JdCenter *JCAy, int numJC,DLBZ *DLAy,int numDL);//设置所有分离段的控制点和断链数据		
	int GetJDCenNum();//获取控制点数目
	int GetDLNum();//获取断链数目
private:
	//获取指定分离段的控制点数据及起终控制点序号	
	bool GetiFLDJDCenInfo(int iFLD,JdCenter *JCAy, int num,int &sJCNo,int &eJCNo);
	//获取指定分离段的断链数据及起终断链序号
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
	//计算指定里程处分离段上的投影里程及线间距
	double CalXJJ(double tylc1x,double &projml);
	//对所有分离段排序
	void SortFLDArray();
	JD_CENTER *MainLine;//主线指针
	CArray<JD_CENTER *,JD_CENTER *>FLDArray;//分离数组
	Adesk::Int16 FLDNum;//分离段数目
private:
	//生成段落
	void CreateDuanLuo();
	CArray<DUANLUO,DUANLUO> DuanLuoArray;//段落
};
int MAKE_ACDBOPENOBJECT_FUNCTION(EXMANAGER);
#endif // !defined(AFX_EXMANAGER_H__65FF494F_8C0F_4A97_BDFD_6E635EDACA84__INCLUDED_)
