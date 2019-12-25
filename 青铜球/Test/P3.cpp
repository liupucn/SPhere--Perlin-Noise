// P3.cpp: implementation of the CP3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "P3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CP3::CP3()
{
	z=0.0;
}

CP3::~CP3()
{
}
CP3::CP3(double x,double y,double z):CP2(x,y)
{
	this->z=z;
}

CP3 operator +(CP3 P1, CP3 P2)
{
	return CP3(P1.x + P2.x, P1.y + P2.y ,P1.z + P2.z);
}

CP3 operator *(double t, CP3 P)
{
	return CP3(P.x*t, P.y*t, P.z*t);
}

CP3 operator /(CP3 P, double t)
{
	return CP3(P.x/t, P.y/t, P.z/t);
}

BOOL operator ==(CP3 P1, CP3 P2)
{
	if (P1.x == P2.x&&P1.y == P2.y&&P1.z == P2.z)
		return TRUE;
	else
		return FALSE;
}