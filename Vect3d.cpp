// Vect3d.cpp: implementation of the Vect3d class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vect3d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vect3d::Vect3d()
{

}

Vect3d::Vect3d (float XX, float YY,  float ZZ) 
{ 
	x=XX; 
	y=YY; 
	z=ZZ; 
}

void Vect3d::Set (float XX, float YY, float ZZ)
{ 
	x=XX; y=YY; z=ZZ; 
}

Vect3d::~Vect3d()
{

}
