// GlobalZDMData.cpp: implementation of the GlobalZDMData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "GlobalZDMData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GlobalZDMData GlobalZDMTK;
GlobalZDMData::GlobalZDMData()
{
	NZDM = 0;

	for(int i=0; i<100; i++)
	{
		ZDMTK[i].DXorRXD = 1;
		ZDMTK[i].RXDno = -1;
		ZDMTK[i].RXDname = L"";
		ZDMTK[i].pm = NULL;
		ZDMTK[i].zdm = NULL;
		ZDMTK[i].dmx = NULL;
		ZDMTK[i].pbri=NULL;
		ZDMTK[i].ptun=NULL;
		ZDMTK[i].X0=ZDMTK[i].Y0=ZDMTK[i].H0=0.0;
		ZDMTK[i].lpt.x=ZDMTK[i].lpt.y=0.0;
		ZDMTK[i].rpt.x=ZDMTK[i].rpt.y=0.0;
	}
}

GlobalZDMData::~GlobalZDMData()
{

}

int GlobalZDMData::FindWichZdm(AcGePoint3d pt)
{

	for(int i=0; i<NZDM; i++)
	{
		if(pt.x>ZDMTK[i].lpt.x-1.0 && pt.x<ZDMTK[i].rpt.x+1.0 && pt.y>ZDMTK[i].lpt.y-1 && pt.y<ZDMTK[i].rpt.y+1)
			return i;//找到第i个纵断面
	}
	return -1;//没找到
}

void GlobalZDMData::IsRXDOrDX(AcGePoint3d pt,int &DXorRXD,int &rxdno,CString &RXDname)
{
	int izdm = FindWichZdm(pt);
	//	ads_printf(L"第%d纵断面...\n",izdm);
	if(izdm<0 || izdm>=NZDM)
		return;
	DXorRXD = ZDMTK[izdm].DXorRXD;
	rxdno = ZDMTK[izdm].RXDno;
	RXDname = ZDMTK[izdm].RXDname;	
}

JD_CENTER *GlobalZDMData::GetPM(AcGePoint3d pt)
{
	int izdm = FindWichZdm(pt);
	if(izdm<0 || izdm>=NZDM)
		return pzLinep;
	return ZDMTK[izdm].pm;
}

GTZDM_ROAD *GlobalZDMData::GetZDM(AcGePoint3d pt)
{
	int izdm = FindWichZdm(pt);
	if(izdm<0 || izdm>=NZDM)
		return NULL;
	return ZDMTK[izdm].zdm;
}

DMX_ROAD *GlobalZDMData::GetDMX(AcGePoint3d pt)
{
	int izdm = FindWichZdm(pt);
	if(izdm<0 || izdm>=NZDM)
		return NULL;
	return ZDMTK[izdm].dmx;
}