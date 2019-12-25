#pragma once
#include "Point3.h"
#include "Lighting.h"//Phong模型
#include"Transform.h"
#include"Transform.h"

class CZBuffer  
{
public:
	CZBuffer(void);
    virtual ~CZBuffer(void);
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//初始化深度缓冲器
	void SetPoint(CP3 P0, CP3 P1, CP3 P2, CVector3 N0, CVector3 N1, CVector3 N2, CP3 T0, CP3 T1, CP3 T2);// 三角形初始化
	void PhongShading(CDC* pDC, CP3 ViewPoint, CLighting* pLight, CMaterial* pMaterial);//光滑着色

	CP3 P0, P1, P2;//三角形的浮点数顶点
	CPoint3 point0, point1, point2;//三角形的整数顶点坐标
	CPoint2* pLeft; //跨度的起点数组标志
	CPoint2* pRight;//跨度的终点数组标志
	int nIndex;//记录扫描线条数
	double** zBuffer;//缓深度冲区
	int nWidth, nHeight;//缓冲区宽度与高度	
protected:
	void RandG(void);//初始化伪随机值
	double Noise3D(double vec[3]);//Perlin三维噪声
	void EdgeFlag(CPoint2 PStart, CPoint2 PEnd, BOOL bFeature);//边标记
	CRGBA LinearInterp(double t, double coorStart, double coorEnd, CRGBA intensityStart, CRGBA intensityEnd);//光强线性插值
	CVector3 LinearInterp(double t, double coorStart, double coorEnd, CVector3 normalStart, CVector3 normalEnd);//法矢量线性插值
	CP3 LinearInterp(double t, double t1, double t2, CP3 tex1, CP3 tex2);//纹理线性插值
	void SortVertex(void);//顶点排序
};