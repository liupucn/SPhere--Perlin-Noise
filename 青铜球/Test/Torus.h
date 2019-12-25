#pragma once
#include"Facet.h"
#include "ZBuffer.h"
#include "Transform.h"
#include "Projection.h"

class CTorus
{
public:
	CTorus(void);
	virtual ~CTorus(void);
	void SetScene(CLighting* pLight, CMaterial* pMaterial);
	void SetRadius(double R, double r);//设置圆环半径和截面半径
	void SetRotation(double Alpha, double Beta);//设置旋转
	void Draw(CDC* pDC, CZBuffer* pZBuffer);
private:
	double R, r;//圆环半径和截面半径
	double Alpha, Beta;//旋转角度
	CLighting* pLight;//光照
	CMaterial* pMaterial;//材质	
	CProjection Projection;//投影

	CVector3 GetNormal(double Landscape, double Ordinat);//计算圆环点法矢量
	CP3* GetPoint(double Landscape1, double Landscape2, double Ordinate1, double Ordinate2);//计算圆环点位置
	void DrawFacet(CDC *pDC, CZBuffer* pZBuffer, CP3 P[4], CVector3 N[4]);//绘制小面
};

