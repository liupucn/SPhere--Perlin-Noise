#pragma once
#include "Point3.h"
#include "Lighting.h"//Phongģ��
#include"Transform.h"
#include"Transform.h"

class CZBuffer  
{
public:
	CZBuffer(void);
    virtual ~CZBuffer(void);
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//��ʼ����Ȼ�����
	void SetPoint(CP3 P0, CP3 P1, CP3 P2, CVector3 N0, CVector3 N1, CVector3 N2, CP3 T0, CP3 T1, CP3 T2);// �����γ�ʼ��
	void PhongShading(CDC* pDC, CP3 ViewPoint, CLighting* pLight, CMaterial* pMaterial);//�⻬��ɫ

	CP3 P0, P1, P2;//�����εĸ���������
	CPoint3 point0, point1, point2;//�����ε�������������
	CPoint2* pLeft; //��ȵ���������־
	CPoint2* pRight;//��ȵ��յ������־
	int nIndex;//��¼ɨ��������
	double** zBuffer;//����ȳ���
	int nWidth, nHeight;//�����������߶�	
protected:
	void RandG(void);//��ʼ��α���ֵ
	double Noise3D(double vec[3]);//Perlin��ά����
	void EdgeFlag(CPoint2 PStart, CPoint2 PEnd, BOOL bFeature);//�߱��
	CRGBA LinearInterp(double t, double coorStart, double coorEnd, CRGBA intensityStart, CRGBA intensityEnd);//��ǿ���Բ�ֵ
	CVector3 LinearInterp(double t, double coorStart, double coorEnd, CVector3 normalStart, CVector3 normalEnd);//��ʸ�����Բ�ֵ
	CP3 LinearInterp(double t, double t1, double t2, CP3 tex1, CP3 tex2);//�������Բ�ֵ
	void SortVertex(void);//��������
};