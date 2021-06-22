#pragma once
#include <dbmain.h>
#include "EarthLine.h"
#include "DesignLine.h"
#include "LTGOU.h"
#include "HdmText.h"

class JSGouReactor :
	public AcDbEntityReactor
{
public:
	JSGouReactor(AcDbObjectId LEarthId, AcDbObjectId REarthId, DesignLine_ROAD* designLine, AcDbObjectId LTGouId, AcDbObjectId NoteId);
	virtual ~JSGouReactor();
	virtual void modified(const AcDbObject* pObj);

private:
	AcDbObjectId m_LEarthObjId, m_REarthObjId, m_LTgouId, m_NoteId;
	DesignLine_ROAD* designline;
};

