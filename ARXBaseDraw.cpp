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
// Descrip: ��õ�ǰARX����·��
// Return:  CString   ��ǰARX����·��,��: "C:\ldf\"
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
CString CARXBaseDraw::GetAppPath()
{
	CString strARXName; // ��ǰARXӦ�ó�����
	CString strARXPath; // ��ǰARXӦ�ó���·��
	strARXName = acedGetAppName(); 
	int pos = strARXName.ReverseFind('\\'); 
	strARXPath = strARXName.Left(pos+1);   

	return strARXPath;	
}

/////////////////////////////////////////////////////////////////
// Descrip: ��AUTOCAD���ݿ������ʵ������ͨ�ú���
// Inputs:  AcDbEntity *pEntity ���ݿ�ʵ��
//          int mode            ���ģʽ��0 -- ��Ӻ�ر�ʵ��(Ĭ��ֵ),�����򲻹ر�ʵ��
//          int ShiftSpace      0: ��ӵ�ģ�Ϳռ䣬������ӵ�ͼֽ�ռ�  
// Return:  Acad::ErrorStatus   ����״̬��
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
	if(ShiftSpace == 0) // ģ�Ϳռ�
	{
		ErrorStatus = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
	}
	else // ͼֽ�ռ�
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
// Descrip: ����ARXӦ�ó���
// Inputs:  CString strArxFullPath  ARXӦ�ó���ȫ·����
//          char arxFileName[256]   ARXӦ�ó����ļ���
//          char cmd[256]           ARX����  
// Author:  ldf
// Date:    2005.11.01
/////////////////////////////////////////////////////////////////
void CARXBaseDraw::LoadArx(CString strArxFullPath, ACHAR arxFileName[256], ACHAR cmd[256])
{
	acedGetAcadDwgView()->SetFocus();

	CString strFileName; // Ҫ���ص�ARX��������·��
	strFileName = strArxFullPath + arxFileName;
	if(strFileName.Find(L" ") != -1) // ·���а����ո�,�����⴦��
	{ 
		CFileFind Fdlg;
		strFileName.Format(L"C:\\%s",arxFileName);
		if(Fdlg.FindFile(strFileName) == 0) // ���C:\test.arx�ļ������ڣ����ϵͳ�п�������
		{
			CString strOldFileName;
			strOldFileName.Format(L"%s\\%s", strArxFullPath, arxFileName);
			CopyFile(strOldFileName,strFileName,TRUE);
		}
	}

	// ��д�ű��ļ�
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

	// ����ARX����
	ACHAR cmdstr[256];
	acDocManager->activateDocument(acDocManager->curDocument());
	_tcscpy(cmdstr ,L"(command \"script\" \"c:\\\\tmp\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0,0,0);	
}

/////////////////////////////////////////////////////////////////
// Descrip: ����ͼ���ļ���������
//          Create image definition object.
//			Create image entity.
//			Create image reactor.
// Inputs:  AcDbObjectId &pImageEntityId ���ع�դͼ��ID
//          CString strImageName         ͼ������(���ֵ��е�����)
//          CString strImagePath         ͼ���ļ�ȫ��
// Return:  Acad::ErrorStatus            ����״̬��
// Others:  ��������ǰȷ���Ѿ�����acISMui.arx(��AutoCAD�İ�װĿ¼��)
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
// Descrip: ������դͼ�������
// Inputs:  AcDbObjectId &pObjectId ���ع�դͼ��ID.
//          CString strImageName    ͼ������(���ֵ��е�����)
//          CString strImagePath    ͼ���ļ�ȫ��
// Return:  Acad::ErrorStatus       ����״̬��
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
// Descrip: ������դͼ��ʵ��
// Inputs:  AcDbObjectId &pImageEntityId ���ع�դͼ��ʵ��ID
//          AcDbObjectId ImageDefId      ��դͼ����ID
// Return:  Acad::ErrorStatus            ����״̬��
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
// Descrip: ������դͼ���������,����:
//			Set the entity to be its owner.
//			Link it to the definition object.
//			Set Reactor Id.
//			Adds the database resident object to the reactor list of the object.
// Inputs:  AcDbObjectId &pImageEntityId ���ر�����ID
//          AcDbObjectId pImageObjectId  ��դͼ����ID
//          AcDbObjectId pImageEntityId  ��դͼ��ʵ��ID
// Return:  Acad::ErrorStatus            ����״̬��
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
// Descrip: �޸Ĺ�դͼ��ʵ��
// Inputs:  AcDbObjectId pImageEntityId  ��դͼ��ʵ��ID
//          AcGePoint3d  origin          ͼ��ԭ������
//          double       width, hight    ͼ����
// Return:  Acad::ErrorStatus            ����״̬��
// Others:  Ŀǰע���˼��б߽�Ĺ��ܣ��������б�Ҫ���ٻָ�
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