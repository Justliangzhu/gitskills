// GlobalZDMData.h: interface for the GlobalZDMData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALZDMDATA_H__1ACF23C3_2A4B_49B7_B4E7_79C7794B7B0E__INCLUDED_)
#define AFX_GLOBALZDMDATA_H__1ACF23C3_2A4B_49B7_B4E7_79C7794B7B0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JD_CENTER.h"
#include "GTZDM.h"
#include "DMX.h"
#include "bri.h"
#include "Tun.h"

class GlobalZDMData  //用于保存时自动判别存入单线还是绕行线数据表
{
public:
	GlobalZDMData();
	virtual ~GlobalZDMData();

	struct ZdmBox
	{
		AcGePoint2d lpt,rpt;//纵断面图左下点，右上点
		int DXorRXD;//1单线或 2左线绕行线 3右线绕行线
		int RXDno;//绕行线编号
		CString RXDname;//绕行线名称
		CString mdbname;//该纵断面对应的数据库名
		double X0,Y0,H0;
		JD_CENTER *pm;//平面指针
		GTZDM_ROAD *zdm;
		DMX_ROAD *dmx;
		BRI_ROAD *pbri;
		TUN_ROAD *ptun;
	} ZDMTK[100];
	int NZDM;//记录当前纵断面数目
	int CurZDM;//记录当前纵断面号
	int FindWichZdm(AcGePoint3d pt);
	void IsRXDOrDX(AcGePoint3d pt,int &DXorRXD,int &rxdno,CString &RXDname);
	JD_CENTER *GetPM(AcGePoint3d pt); 
    GTZDM_ROAD *GetZDM(AcGePoint3d pt);
	DMX_ROAD *GetDMX(AcGePoint3d pt);
};

#endif // !defined(AFX_GLOBALZDMDATA_H__1ACF23C3_2A4B_49B7_B4E7_79C7794B7B0E__INCLUDED_)
