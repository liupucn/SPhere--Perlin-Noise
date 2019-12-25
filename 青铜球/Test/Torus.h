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
	void SetRadius(double R, double r);//����Բ���뾶�ͽ���뾶
	void SetRotation(double Alpha, double Beta);//������ת
	void Draw(CDC* pDC, CZBuffer* pZBuffer);
private:
	double R, r;//Բ���뾶�ͽ���뾶
	double Alpha, Beta;//��ת�Ƕ�
	CLighting* pLight;//����
	CMaterial* pMaterial;//����	
	CProjection Projection;//ͶӰ

	CVector3 GetNormal(double Landscape, double Ordinat);//����Բ���㷨ʸ��
	CP3* GetPoint(double Landscape1, double Landscape2, double Ordinate1, double Ordinate2);//����Բ����λ��
	void DrawFacet(CDC *pDC, CZBuffer* pZBuffer, CP3 P[4], CVector3 N[4]);//����С��
};

