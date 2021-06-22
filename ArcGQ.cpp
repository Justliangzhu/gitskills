// ArcGQ.cpp: implementation of the ArcGQ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ArcGQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArcGQ::ArcGQ()
{

}

ArcGQ::~ArcGQ()
{
	delete MainCir;
	
}

void ArcGQ::SetMainArc(AcDbCircle *Cir)
{
	MainCir=Cir;
}

void ArcGQ::AddGQArc(AcDbCircle *Cir)
{
	GQCirArray.Add(Cir);
}

bool ArcGQ::IsGQ()
{
	int i=0,NUM=GQCirArray.GetSize();
	double dist,Rmain,Ri;
	Rmain=MainCir->radius();

	for(i=0;i<NUM;i++)
	{
		dist=MainCir->center().distanceTo(GQCirArray[i]->center());
		Ri=GQCirArray[i]->radius();
		//acutPrintf(L"Rmain=%lf,Ri=%lf,dist=%lf\n",Rmain,Ri,dist);
		if(dist!=fabs(Rmain-Ri) && dist!=Rmain+Ri)return false;
	}
	return true;
}
