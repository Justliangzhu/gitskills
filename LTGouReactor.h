// LTGouReactor.h: interface for the LTGouReactor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LTGOUREACTOR_H__646DB51B_D304_4A27_B191_C4560F0F5E69__INCLUDED_)
#define AFX_LTGOUREACTOR_H__646DB51B_D304_4A27_B191_C4560F0F5E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EarthLine.h"
#include "DesignLine.h"
#include "JSGOU.h"
#include "HdmText.h"

class LTGouReactor : public AcDbEntityReactor  
{
public:
	LTGouReactor(AcDbObjectId LEarthId, AcDbObjectId REarthId,DesignLine_ROAD *designLine,AcDbObjectId JSGouId,AcDbObjectId NoteId);
	virtual ~LTGouReactor();
	virtual void modified(const AcDbObject * pObj);
	
private:
    AcDbObjectId m_LEarthObjId,	m_REarthObjId,m_JSgouId,m_NoteId;
	DesignLine_ROAD *designline;
};

#endif // !defined(AFX_LTGOUREACTOR_H__646DB51B_D304_4A27_B191_C4560F0F5E69__INCLUDED_)
