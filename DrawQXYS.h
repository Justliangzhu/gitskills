// DrawQXYS.h: interface for the DrawQXYS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWQXYS_H__CED2EEB3_B138_4B9A_8D27_56459893FD3D__INCLUDED_)
#define AFX_DRAWQXYS_H__CED2EEB3_B138_4B9A_8D27_56459893FD3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.h"

class DrawQXYS  : public  BAS_DRAW_FUN
{
public:
	double DFACTOR,INPTX,INPTY,ALPHA,Wtab,Htab;
	ads_point Ppt;
	int SIP,EIP,JDn;
	int DrawMode;//0 按初步设计绘 1按施工图绘
    void draw();
	int del_ent();
	int wr_cmf_c();
	int wr_cmf_s();
	void  MidPoint(ads_point pt,double w,double h,double angx,ads_point Mpt);
	void LineLR(ads_point p1,ads_point p2,double angx,double dist,double lw,int color=2);
	void LineUD(ads_point p1,ads_point p2,double angx,double dist,double lw);
	void wr_text(ads_point pt,double angx,double d,double wk,double ht, ACHAR *str);
	void ptAddx(ads_point p1,double angx,double x,ads_point pt);
	void ptAddy(ads_point p1,double angx,double y,ads_point pt);
	DrawQXYS();
	virtual ~DrawQXYS();

};

#endif // !defined(AFX_DRAWQXYS_H__CED2EEB3_B138_4B9A_8D27_56459893FD3D__INCLUDED_)
