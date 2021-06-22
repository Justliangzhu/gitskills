// MyLayout.h: interface for the CMyLayout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLAYOUT_H__F0914EC8_B22C_4CD4_A6E6_6A28C64E2CA1__INCLUDED_)
#define AFX_MYLAYOUT_H__F0914EC8_B22C_4CD4_A6E6_6A28C64E2CA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
#include "dblayout.h"
#include "AcDbLMgr.h "

class CMyLayout : public BAS_DRAW_FUN  
{
public:
	static AcDbObjectId CreateViewInLayout(ACHAR LayoutName[],AcGePoint3dArray Pt3dAy,double lw=0.0,bool ShowEdge=true,bool Lock=true);
	static Acad::ErrorStatus CreateLayout(ACHAR LayoutName[]);
	CMyLayout();
	virtual ~CMyLayout();

};

#endif // !defined(AFX_MYLAYOUT_H__F0914EC8_B22C_4CD4_A6E6_6A28C64E2CA1__INCLUDED_)
