// ARXBaseDraw.cpp: implementation of the CARXBaseDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdArx.h"
#include "resource.h"
#include "ARXBaseDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARXBaseDraw::CARXBaseDraw()
{

}

CARXBaseDraw::~CARXBaseDraw()
{

}

/////////////////////////////////////////////////////////////////
// Descrip: 获得当前ARX程序路径
// Return:  CString   当前ARX程序路径,如: "C:\ldf\"
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
CString CARXBaseDraw::GetAppPath()
{
	CString strARXName; // 当前ARX应用程序名
	CString strARXPath; // 当前ARX应用程序路径
	strARXName = acedGetAppName(); 
	int pos = strARXName.ReverseFind('\\'); 
	strARXPath = strARXName.Left(pos+1);   

	return strARXPath;	
}

/////////////////////////////////////////////////////////////////
// Descrip: 向AUTOCAD数据库中添加实体对象的通用函数
// Inputs:  AcDbEntity *pEntity 数据库实体
//          int mode            添加模式：0 -- 添加后关闭实体(默认值),其它则不关闭实体
//          int ShiftSpace      0: 添加到模型空间，其它添加到图纸空间  
// Return:  Acad::ErrorStatus   错误状态码
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
Acad::ErrorStatus CARXBaseDraw::AddEntityToDbs(AcDbEntity *pEntity,int mode,int ShiftSpace)
{	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	Acad::ErrorStatus       ErrorStatus;
	AcDbDatabase*			pDataBase;
	AcDbBlockTable*			pBlockTable; 
	AcDbBlockTableRecord*	pBlockTableRecord;

	// get database
	pDataBase = acdbHostApplicationServices()->workingDatabase();
	if(pDataBase == NULL) 
	{
		return Acad::eNoDatabase;
	}

	// get block table
	ErrorStatus = pDataBase->getSymbolTable(pBlockTable, AcDb::kForRead);
	if(ErrorStatus != Acad::eOk) 
	{
		return ErrorStatus;
	}

	// CAD R2000
	// acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);
	// CAD R14
	// acdbCurDwg()->getBlockTable(pBlockTable,AcDb::kForRead);

	// get block table record
	if(ShiftSpace == 0) // 模型空间
	{
		ErrorStatus = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
	}
	else // 图纸空间
	{
		ErrorStatus = pBlockTable->getAt(ACDB_PAPER_SPACE, pBlockTableRecord,AcDb::kForWrite);
	}

	// append entity to block table record
	pBlockTableRecord->appendAcDbEntity(pEntity);

	// close
	pBlockTableRecord->close();
	pBlockTable->close();
	if(mode == 0)
	{
		pEntity->close();
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	return ErrorStatus;
}

/////////////////////////////////////////////////////////////////
// Descrip: 加载ARX应用程序
// Inputs:  CString strArxFullPath  ARX应用程序全路径名
//          char arxFileName[256]   ARX应用程序文件名
//          char cmd[256]           ARX命令  
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
void CARXBaseDraw::LoadArx(CString strArxFullPath, ACHAR arxFileName[256], ACHAR cmd[256])
{
	acedGetAcadDwgView()->SetFocus();

	CString strFileName; // 要加载的ARX程序最终路径
	strFileName = strArxFullPath + arxFileName;
	if(strFileName.Find(L" ") != -1) // 路径中包含空格,需特殊处理
	{ 
		CFileFind Fdlg;
		strFileName.Format(L"C:\\%s",arxFileName);
		if(Fdlg.FindFile(strFileName) == 0) // 如果C:\test.arx文件不存在，则从系统中拷贝过来
		{
			CString strOldFileName;
			strOldFileName.Format(L"%s\\%s", strArxFullPath, arxFileName);
			CopyFile(strOldFileName,strFileName,TRUE);
		}
	}

	// 编写脚本文件
	FILE *pfw;
	pfw = _wfopen(L"c:\\tmp.scr",L"w");
	fwprintf(pfw,L"arx\n");
	fwprintf(pfw,L"load\n");
	fwprintf(pfw,L"%s\n", strFileName);
	if(_tcscmp(cmd,L"") != 0)
	{
		fwprintf(pfw,L"%s ", cmd);
	}
	fclose(pfw); 

	// 加载ARX程序
	ACHAR cmdstr[256];
	acDocManager->activateDocument(acDocManager->curDocument());
	_tcscpy(cmdstr ,L"(command \"script\" \"c:\\\\tmp\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0,0,0);	
}

/////////////////////////////////////////////////////////////////
// Descrip: 绘制图像文件，包含：
//          Create image definition object.
//			Create image entity.
//			Create image reactor.
// Inputs:  AcDbObjectId &pImageEntityId 返回光栅图像ID
//          CString strImageName         图像名称(在字典中的名称)
//          CString strImagePath         图像文件全名
// Return:  Acad::ErrorStatus            错误状态码
// Others:  函数运行前确保已经加载acISMui.arx(在AutoCAD的安装目录下)
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
Acad::ErrorStatus 
	CARXBaseDraw::DrawImage(AcDbObjectId &pImageEntityId, CString strImageName, CString strImagePath)
{
	// declaration
	Acad::ErrorStatus	ErrorStatus;
	AcDbObjectId 		ObjectId,	 // AcDbRasterImageDef Id
		ReactorId;	 // AcDbRasterImageDefReactor Id

	// Create image definition object.
	ErrorStatus = createAcDbRasterImageDef(ObjectId, strImageName, strImagePath);
	if (ErrorStatus != Acad::eOk)
	{
		ads_printf(L"1\n");
		return ErrorStatus;
	}

	// Create image entity.
	ErrorStatus = createAcDbRasterImage(pImageEntityId, ObjectId);
	if (ErrorStatus != Acad::eOk)
	{
		ads_printf(L"2\n");
		return ErrorStatus;
	}

	// Create image reactor.
	ErrorStatus = createAcDbRasterImageDefReactor(ReactorId, ObjectId, pImageEntityId);
	if (ErrorStatus != Acad::eOk)
	{
		ads_printf(L"3\n");
		return ErrorStatus;
	}

	return ErrorStatus;
}

/////////////////////////////////////////////////////////////////
// Descrip: 创建光栅图像定义对象
// Inputs:  AcDbObjectId &pObjectId 返回光栅图像ID.
//          CString strImageName    图像名称(在字典中的名称)
//          CString strImagePath    图像文件全名
// Return:  Acad::ErrorStatus       错误状态码
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
Acad::ErrorStatus CARXBaseDraw::createAcDbRasterImageDef(AcDbObjectId &pObjectId, 
														 CString	  strImageName, 
														 CString	  strImagePath)
{
	Acad::ErrorStatus	ErrorStatus;
	AcDbObjectId		DictionaryId;
	AcDbDictionary*     pDictionary;
	Adesk::Boolean		bNameInUse;

	// Create new image def object
	AcDbRasterImageDef* pAcDbRasterImageDef = new AcDbRasterImageDef;			
	if (pAcDbRasterImageDef == NULL)
	{
		return Acad::eNullObjectPointer;
	}

	// set source file name
	ErrorStatus = pAcDbRasterImageDef->setSourceFileName(strImagePath);
	if (ErrorStatus != Acad::eOk) 
	{
		ads_printf(L"Error: Could not find the image file.\n"); 
		delete pAcDbRasterImageDef;
		return ErrorStatus;
	}

	// load image
	ErrorStatus = pAcDbRasterImageDef->load(); 
	if (ErrorStatus != Acad::eOk) 
	{
		ads_printf(L"Error: Could not open the image file.\n"); 
		delete pAcDbRasterImageDef;
		return ErrorStatus;
	}

	// Get Dictionary Id.
	DictionaryId = AcDbRasterImageDef::imageDictionary(acdbHostApplicationServices()->workingDatabase());
	if (DictionaryId.asOldId() == 0) // No Dictionary open.
	{
		// Create dictionary
		ErrorStatus = AcDbRasterImageDef::createImageDictionary(acdbHostApplicationServices()->workingDatabase(), DictionaryId);
		if (ErrorStatus!= Acad::eOk)
		{
			delete pAcDbRasterImageDef;
			return ErrorStatus;
		}
	}

	// Open AcDbDictionary.
	ErrorStatus = acdbOpenObject((AcDbObject*&)pDictionary, DictionaryId, AcDb::kForWrite);
	if (ErrorStatus != Acad::eOk)
	{
		delete pAcDbRasterImageDef;
		return ErrorStatus;
	}

	// Check if image name already in use.
	bNameInUse = pDictionary->has(strImageName);
	if(!bNameInUse) // name not in use
	{
		// adds a new entry specified by newValue into the dictionary and returns Object Id.
		ErrorStatus = pDictionary->setAt(strImageName, pAcDbRasterImageDef, pObjectId);
	}
	else // name in use	
	{
		pDictionary->close();
		delete pAcDbRasterImageDef;
		return Acad::eHandleInUse;
	}

	// close Dicionary and Definition.
	pDictionary->close();
	pAcDbRasterImageDef->close();

	return ErrorStatus;
}

/////////////////////////////////////////////////////////////////
// Descrip: 创建光栅图像实体
// Inputs:  AcDbObjectId &pImageEntityId 返回光栅图像实体ID
//          AcDbObjectId ImageDefId      光栅图像定义ID
// Return:  Acad::ErrorStatus            错误状态码
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
Acad::ErrorStatus 
	CARXBaseDraw::createAcDbRasterImage(AcDbObjectId &pImageEntityId, AcDbObjectId ImageDefId)
{
	// declaration
	Acad::ErrorStatus ErrorStatus;


	// Create a new image entity
	AcDbRasterImage* pAcDbRasterImage = new AcDbRasterImage;
	if(pAcDbRasterImage == NULL)
	{
		return Acad::eNullEntityPointer;
	}

	// Set Image Defintion Id in Entity. ( link it to the specified image definition object)
	ErrorStatus = pAcDbRasterImage->setImageDefId(ImageDefId);
	if(ErrorStatus != Acad::eOk) 
	{
		delete pAcDbRasterImage;
		return ErrorStatus;
	}

	// Appends the entity to model space.
	ErrorStatus = AddEntityToDbs((AcDbEntity*)pAcDbRasterImage);
	if(ErrorStatus != Acad::eOk) 
	{
		delete pAcDbRasterImage;
		return ErrorStatus;
	}

	// Set Entity ID to Attribute.
	pImageEntityId = pAcDbRasterImage->objectId();

	// close entity.
	pAcDbRasterImage->close();

	return ErrorStatus;
}

/////////////////////////////////////////////////////////////////
// Descrip: 创建光栅图像定义遍历器,包含:
//			Set the entity to be its owner.
//			Link it to the definition object.
//			Set Reactor Id.
//			Adds the database resident object to the reactor list of the object.
// Inputs:  AcDbObjectId &pImageEntityId 返回遍历器ID
//          AcDbObjectId pImageObjectId  光栅图像定义ID
//          AcDbObjectId pImageEntityId  光栅图像实体ID
// Return:  Acad::ErrorStatus            错误状态码
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
Acad::ErrorStatus 
	CARXBaseDraw::createAcDbRasterImageDefReactor(AcDbObjectId &pReactorId, 
	AcDbObjectId pImageObjectId, 
	AcDbObjectId pImageEntityId)
{
	// declaration
	Acad::ErrorStatus	ErrorStatus;

	AcDbRasterImage*			pAcDbRasterImage;			// Entity
	AcDbRasterImageDef*			pAcDbRasterImageDef;		// Definition
	AcDbRasterImageDefReactor*	pAcDbRasterImageDefReactor;	// Reactor


	// Disable image definition notification while changing the defintion reactor list.
	AcDbRasterImageDefReactor::setEnable(Adesk::kFalse);

	// open Definition
	ErrorStatus = acdbOpenObject((AcDbObject*&)pAcDbRasterImageDef, pImageObjectId, AcDb::kForWrite);
	if(ErrorStatus != Acad::eOk) 
	{
		pAcDbRasterImageDef->close();
		return ErrorStatus;
	}

	// open Entity
	ErrorStatus = acdbOpenObject((AcDbObject*&)pAcDbRasterImage, pImageEntityId, AcDb::kForWrite);
	if(ErrorStatus != Acad::eOk) 
	{
		pAcDbRasterImage->close();
		pAcDbRasterImageDef->close();
		return ErrorStatus;
	}

	// Create reactor.
	pAcDbRasterImageDefReactor = new AcDbRasterImageDefReactor;			
	if(pAcDbRasterImageDefReactor == NULL)
	{
		return Acad::eNullObjectPointer;
	}

	// Set the entity to be its owner.
	ErrorStatus = pAcDbRasterImageDefReactor->setOwnerId(pImageEntityId);
	if(ErrorStatus != Acad::eOk) 
	{
		pAcDbRasterImage->close();
		pAcDbRasterImageDef->close();
		delete pAcDbRasterImageDefReactor;
		return ErrorStatus;
	}

	// Link it to the definition object.
	ErrorStatus = acdbHostApplicationServices()->workingDatabase()->addAcDbObject(pReactorId, pAcDbRasterImageDefReactor);
	if(ErrorStatus != Acad::eOk) 
	{
		pAcDbRasterImage->close();
		pAcDbRasterImageDef->close();
		delete pAcDbRasterImageDefReactor;
		return ErrorStatus;
	}

	// Set Reactor Id.
	pAcDbRasterImage->setReactorId(pReactorId);

	// Adds the database resident object to the reactor list of the object.
	pAcDbRasterImageDef->addPersistentReactor(pReactorId);

	//close Reactor, Image, Definition.
	pAcDbRasterImageDefReactor->close();
	pAcDbRasterImageDef->close();
	pAcDbRasterImage->close();

	// Re-enable image def notification.
	AcDbRasterImageDefReactor::setEnable(Adesk::kTrue);

	return ErrorStatus;
}

/////////////////////////////////////////////////////////////////
// Descrip: 修改光栅图像实体
// Inputs:  AcDbObjectId pImageEntityId  光栅图像实体ID
//          AcGePoint3d  origin          图像原点坐标
//          double       width, hight    图像宽高
// Return:  Acad::ErrorStatus            错误状态码
// Others:  目前注释了剪切边界的功能，将来如有必要可再恢复
// Author:  ldf
// Date:    2005.11.05
/////////////////////////////////////////////////////////////////
Acad::ErrorStatus CARXBaseDraw::ModifyImage(AcDbObjectId pImageEntityId, AcGePoint3d origin, double width, double hight)
{
	// declaration
	Acad::ErrorStatus ErrorStatus;
	Adesk::Boolean    ReturnValue;

	AcDbRasterImage*  pAcDbRasterImage;	// Entity

	// definition orientation
	AcGeVector3d u(width, 0, 0), v(0, hight, 0);

	// clipping
	//	AcDbRasterImage::ClipBoundaryType	ClipBoundaryType;	// polygonal clip boundary
	//	AcGePoint2dArray					ClipBoundary;		// clip boundary
	//	AcGePoint2d							ClipPoint;			// single clip point

	// Open object.
	ErrorStatus = acdbOpenObject((AcDbObject*&)pAcDbRasterImage, pImageEntityId, AcDb::kForWrite);

	if(ErrorStatus == Acad::eOk) 
	{

		// Get current clip boundary.
		//		ClipBoundary = pAcDbRasterImage->clipBoundary();
		// type of clip boundary
		//		ClipBoundaryType = AcDbRasterImage::kPoly;	// polygonal clip boundary

		// Define single clip points to set and append it to the clip boundary.
		//	Be aware that the first and last clip point has to be the same 
		//	and that the clip points are pixel space.
		//		ClipPoint.set(100, 100);
		//		ClipBoundary.setAt(0, ClipPoint);
		//		
		//		ClipPoint.set(100, 363);
		//		ClipBoundary.setAt(1, ClipPoint);
		//
		//		ClipPoint.set(480, 363);
		//		ClipBoundary.setAt(2, ClipPoint);
		//		
		//		ClipPoint.set(480, 100);
		//		ClipBoundary.setAt(3, ClipPoint);
		//		
		//		ClipPoint.set(100, 100);
		//		ClipBoundary.setAt(4, ClipPoint);
		//		
		//		// Last point has to be same as first point.
		//		ClipPoint.set(100, 100);
		//		ClipBoundary.append(ClipPoint);

		// Set orientation
		ReturnValue = pAcDbRasterImage->setOrientation( origin, u, v );
		if (ReturnValue != Adesk::kTrue)
		{
			ads_printf(L"Error: Set orientation failed.\n"); 
		}

		// Set clip boundary
		//		ErrorStatus = pAcDbRasterImage->setClipBoundary(ClipBoundaryType, ClipBoundary);
		//		if (ErrorStatus != Acad::eOk)
		//		{
		//			ads_printf(L"Error: Set orientation failed.\n"); 
		//		}

		// Close object
		pAcDbRasterImage->close();
	}

	return ErrorStatus;
}