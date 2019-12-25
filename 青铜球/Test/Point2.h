#pragma once
#include "RGBA.h"
#include "Vector3.h"

class CPoint2
{
public:
	CPoint2(void);	
	CPoint2(int x, int y);
	CPoint2(int x, int y, CRGBA c);
	CPoint2(int x, int y, CVector3 n, CP3 T3D);
	virtual ~CPoint2(void);
	friend CPoint2 operator + (const CPoint2 &pt0, const CPoint2 &pt1);//运算符重载
	friend CPoint2 operator - (const CPoint2 &pt0, const CPoint2 &pt1);
	friend CPoint2 operator * (int scalar, const CPoint2 &pt);
	friend BOOL operator != (const CPoint2 pt0, const CPoint2 pt1);	
public:
	int x, y;//坐标
	CRGBA c;//颜色
	CVector3 n;//法矢量
	CP3 T3D;
};