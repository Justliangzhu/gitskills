// ORI_SCALE.cpp: implementation of the ORI_SCALE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ORI_SCALE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ORI_SCALE pORI_SCALE;
ORI_SCALE::ORI_SCALE()
{
	X_ORI=0.0;
	Y_ORI=0.0;//纵断面基点坐标
	HSCALE = 1000 / 1000.0;
	VSCALE = 100 / 1000.0;//纵横比例
	LCBTEXTH=3;
	PQXHIGH=40;//平曲线栏高
	DESHIGH=20;//设计高栏高
	EARHIGH=20;//地面高栏高
	HBVSCALE=HSCALE/VSCALE;
	PQXTEXTH=5;
	LCBHigh = 20;
	XSW = 3;//保留两位小数
	QZZ = 0;//边坡点里程无约束
	TDFS = 1;
	DesHXSW = 3;//保留三位小数
	PDTextH = 4;
	//	m_DXorRXD = 1;//单线纵断面
	//	RXDno = -1;//第几绕行段
	//	RXDname = "";//绕行段名
	ZMIN=0;//纵断面基点高程
	ZMTH=300;
}

ORI_SCALE::~ORI_SCALE()
{

}
