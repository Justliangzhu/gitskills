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

	// ��õ�ǰARX����·��
	static CString GetAppPath();
	// ��AUTOCAD ���ݿ������ʵ������ͨ�ú���
	static Acad::ErrorStatus AddEntityToDbs(AcDbEntity *pEntity, int mode=0, int ShiftSpace=0);
	// ����ARXӦ�ó���
	static void LoadArx(CString strArxFullPath, ACHAR arxFileName[256], ACHAR cmd[256]);	
	// ����ͼ���ļ�
	static Acad::ErrorStatus DrawImage(AcDbObjectId &pImageEntityId, CString strImageName, CString strImagePath);
	static Acad::ErrorStatus ModifyImage(AcDbObjectId pImageEntityId, AcGePoint3d origin, double width, double height);
	
protected:
	// ������դͼ�������
	static Acad::ErrorStatus createAcDbRasterImageDef(AcDbObjectId &pObjectId, CString	strImageName, CString strImagePath);
	// ������դͼ��ʵ��
	static Acad::ErrorStatus createAcDbRasterImage(AcDbObjectId &pImageEntityId, AcDbObjectId pImageDefId);
	// ������դͼ���������
	static Acad::ErrorStatus createAcDbRasterImageDefReactor(AcDbObjectId &pReactorId, AcDbObjectId pImageObjectId, AcDbObjectId pImageEntityId);
};

#endif // !defined(AFX_ARXBASEDRAW_H__4913C314_36D1_48AF_A964_9710E7B91BDE__INCLUDED_)
