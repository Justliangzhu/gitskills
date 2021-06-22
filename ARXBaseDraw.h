// ARXBaseDraw.h: interface for the CARXBaseDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARXBASEDRAW_H__4913C314_36D1_48AF_A964_9710E7B91BDE__INCLUDED_)
#define AFX_ARXBASEDRAW_H__4913C314_36D1_48AF_A964_9710E7B91BDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "imgdef.h" 
class CARXBaseDraw  
{
public:
	CARXBaseDraw();
	virtual ~CARXBaseDraw();

	// 获得当前ARX程序路径
	static CString GetAppPath();
	// 向AUTOCAD 数据库中添加实体对象的通用函数
	static Acad::ErrorStatus AddEntityToDbs(AcDbEntity *pEntity, int mode=0, int ShiftSpace=0);
	// 加载ARX应用程序
	static void LoadArx(CString strArxFullPath, ACHAR arxFileName[256], ACHAR cmd[256]);	
	// 绘制图像文件
	static Acad::ErrorStatus DrawImage(AcDbObjectId &pImageEntityId, CString strImageName, CString strImagePath);
	static Acad::ErrorStatus ModifyImage(AcDbObjectId pImageEntityId, AcGePoint3d origin, double width, double height);
	
protected:
	// 创建光栅图像定义对象
	static Acad::ErrorStatus createAcDbRasterImageDef(AcDbObjectId &pObjectId, CString	strImageName, CString strImagePath);
	// 创建光栅图像实体
	static Acad::ErrorStatus createAcDbRasterImage(AcDbObjectId &pImageEntityId, AcDbObjectId pImageDefId);
	// 创建光栅图像定义遍历器
	static Acad::ErrorStatus createAcDbRasterImageDefReactor(AcDbObjectId &pReactorId, AcDbObjectId pImageObjectId, AcDbObjectId pImageEntityId);
};

#endif // !defined(AFX_ARXBASEDRAW_H__4913C314_36D1_48AF_A964_9710E7B91BDE__INCLUDED_)
