// HdmDmx.h: interface for the HdmDmx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDMDMX_H__A59AE99B_C368_44D9_8BDC_B0888C41FB02__INCLUDED_)
#define AFX_HDMDMX_H__A59AE99B_C368_44D9_8BDC_B0888C41FB02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "bas_draw_fun.h"
#include "hdmdatabase.h"

class HdmDmx  : public BAS_DRAW_FUN
{
public:
	HdmDmx();
	virtual ~HdmDmx();
	CString mdbFname,RoadName;
    struct RoadDmxdata
	{
		CString roadname;//道路名
        JD_CENTER *pm;
	    int NSTA;
		HDMDMXdata *hdmdmx;
	} ;
	RoadDmxdata *road;

	 
	void CalDmx();
	double FindIdmxk(JD_CENTER *pm,double dml,int ndmxk,crswiddata dmxk[]);//地面线宽
	void WriteHdmDmxFile(int xlnum);
    void WriteZdkHdkFile(int xlnum);
	void GetHdmDmxFromZDKandHDK(JD_CENTER *pm,CString mdbname,CString RoadName);
    void ReadBriTunData(JD_CENTER *pm,CString mdbFname,CString RoadName,int &NBT,double BriTunAry[500][2]);
	void GetHdmDmxFromZDKandDTM(JD_CENTER *pm,CString mdbname,CString RoadName);

};

#endif // !defined(AFX_HDMDMX_H__A59AE99B_C368_44D9_8BDC_B0888C41FB02__INCLUDED_)
