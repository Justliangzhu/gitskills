// ArcGQ.h: interface for the ArcGQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCGQ_H__67C78628_2B46_4435_AC13_58527A5FE64E__INCLUDED_)
#define AFX_ARCGQ_H__67C78628_2B46_4435_AC13_58527A5FE64E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.H"
class ArcGQ : public BAS_DRAW_FUN
{
public:
	bool IsGQ();
	void AddGQArc(AcDbCircle *Cir);
	void SetMainArc(AcDbCircle *Cir);
	ArcGQ();
	virtual ~ArcGQ();
private:
	AcDbCircle *MainCir;
	CArray<AcDbCircle *,AcDbCircle *>GQCirArray;

};

#endif // !defined(AFX_ARCGQ_H__67C78628_2B46_4435_AC13_58527A5FE64E__INCLUDED_)
