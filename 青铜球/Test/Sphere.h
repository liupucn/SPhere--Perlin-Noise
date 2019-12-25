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
	void SetScale(int nScalar);//�������ű���
	void SetScene(CLighting* pLight, CMaterial* pMaterial);
	void SetRotation(double Alpha, double Beta);//������ת
	void DrawFacet(CDC* pDC, CZBuffer* pZBuffer, CP3* P);//����С��
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//��������
private:
	int nScalar;//���ű���
	double Alpha, Beta;//��ת�Ƕ�
	CLighting* pLight;//����
	CMaterial* pMaterial;//����	
	CProjection projection;//ͶӰ
};

