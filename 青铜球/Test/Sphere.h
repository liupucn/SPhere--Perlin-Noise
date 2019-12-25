#pragma once
#include"Transform.h"
#include"Projection.h"
#include "Lighting.h"
#include "Material.h"
#include "ZBuffer.h"

class CSphere
{
public:
	CSphere(void);
	virtual ~CSphere(void);
	void SetScale(int nScalar);//设置缩放比例
	void SetScene(CLighting* pLight, CMaterial* pMaterial);
	void SetRotation(double Alpha, double Beta);//设置旋转
	void DrawFacet(CDC* pDC, CZBuffer* pZBuffer, CP3* P);//绘制小面
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//绘制球体
private:
	int nScalar;//缩放比例
	double Alpha, Beta;//旋转角度
	CLighting* pLight;//光照
	CMaterial* pMaterial;//材质	
	CProjection projection;//投影
};

