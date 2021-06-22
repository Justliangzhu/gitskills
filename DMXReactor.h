// DMXReactor.h: interface for the DMXReactor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMXREACTOR_H__F5AD7FC9_EDF4_4243_B9D4_A67FCAC763A6__INCLUDED_)
#define AFX_DMXREACTOR_H__F5AD7FC9_EDF4_4243_B9D4_A67FCAC763A6__INCLUDED_

#include "dbmain.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DMXReactor : public AcDbEntityReactor 
{
public:
	DMXReactor(AcDbObjectId objId);
	virtual ~DMXReactor();
	
	virtual void modifyUndone(const AcDbObject* pObj);
	virtual void modified(const AcDbObject* pObj);        
	virtual void openedForModify(const AcDbObject* DmxObj);
	AcDbObjectId m_ObjId;  
};

#endif // !defined(AFX_DMXREACTOR_H__F5AD7FC9_EDF4_4243_B9D4_A67FCAC763A6__INCLUDED_)
