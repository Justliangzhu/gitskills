// GTZDMReactor.h: interface for the GTZDMReactor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZDMREACTOR_H__F90B2CB8_745C_4084_A5D3_B72437A39862__INCLUDED_)
#define AFX_GTZDMREACTOR_H__F90B2CB8_745C_4084_A5D3_B72437A39862__INCLUDED_
#include "GTZDM.h"
#include "dbmain.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define AppName  /*MSG1*/"GUADS" 
class GTZDMReactor : public AcDbEntityReactor//AcDbObjectReactor//  
{
public:
	virtual void modifyUndone(const AcDbObject* pObj);
//	ACRX_DECLARE_MEMBERS(GTZDMReactor);
	GTZDMReactor(AcDbObjectId objId_Bri,ACHAR BorT);
	virtual ~GTZDMReactor();
	virtual void modified(const AcDbObject* pObj);        
	virtual void openedForModify(const AcDbObject* DmxObj);
	AcDbObjectId m_ObjId;  
	ACHAR m_BorT;
private:
};

#endif // !defined(AFX_GTZDMREACTOR_H__F90B2CB8_745C_4084_A5D3_B72437A39862__INCLUDED_)











