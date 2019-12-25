#include "StdAfx.h"
#include "Torus.h"
#include <math.h>//������ѧͷ�ļ�
#define  PI 3.1415926//PI�ĺ궨��
#define ROUND(d) int(d + 0.5)//��������궨��

CTorus::CTorus(void)
{
	Alpha = Beta = 0;
}


CTorus::~CTorus(void)
{}

void CTorus::SetScene(CLighting* pLight, CMaterial* pMaterial)
{
	this->pLight = pLight;
	this->pMaterial = pMaterial;
}

void CTorus::SetRadius(double R, double r)//����Բ���뾶�ͽ���뾶
{
	this->R = R;
	this->r = r;
}

void CTorus::SetRotation(double Alpha, double Beta)//������ת
{
	this->Alpha = Alpha;
	this->Beta = Beta;
}

void CTorus::Draw(CDC* pDC, CZBuffer* pZBuffer)
{
	CVector3 N[4];//��ʸ��
	double Alpha1, Alpha2, Beta1, Beta2;
	for (int i = 0;i < 360;i += 5)//�������
	{
		Alpha1 = i * PI / 180;
		Alpha2 = (i + 5) * PI / 180;
		for (int j = 0;j < 360;j += 5)
		{
			Beta1 = j * PI / 180;
			Beta2 = (j + 5)*PI / 180;

			CP3 *Point4;//��ά��
			Point4 = GetPoint(Alpha1, Alpha2, Beta1, Beta2);//����С���ϵ��ĸ���
			N[0] = GetNormal(Alpha1, Beta2);//����㷨ʸ��
			N[1] = GetNormal(Alpha2, Beta2);
			N[2] = GetNormal(Alpha2, Beta1);
			N[3] = GetNormal(Alpha1, Beta1);

			DrawFacet(pDC, pZBuffer, Point4, N);//����С��
			delete[] Point4;
		}
	}
}

CP3* CTorus::GetPoint(double Landscape1, double Landscape2, double Ordinate1, double Ordinate2)//����Բ����λ��
{
	CP3 *Point4 = new CP3[4];//��ά��

	Point4[0].x = (R + r * sin(Ordinate2))*sin(Landscape1);
	Point4[0].y = r * cos(Ordinate2);
	Point4[0].z = (R + r * sin(Ordinate2))*cos(Landscape1);

	Point4[1].x = (R + r * sin(Ordinate2))*sin(Landscape2);
	Point4[1].y = r * cos(Ordinate2);
	Point4[1].z = (R + r * sin(Ordinate2))*cos(Landscape2);

	Point4[2].x = (R + r * sin(Ordinate1))*sin(Landscape2);
	Point4[2].y = r * cos(Ordinate1);
	Point4[2].z = (R + r * sin(Ordinate1))*cos(Landscape2);

	Point4[3].x = (R + r * sin(Ordinate1))*sin(Landscape1);
	Point4[3].y = r * cos(Ordinate1);
	Point4[3].z = (R + r * sin(Ordinate1))*cos(Landscape1);

	CTransform tran;
	tran.SetMat(Point4, 4);
	tran.RotateX(Alpha);
	tran.RotateY(Beta);

	return Point4;
}

CVector3 CTorus::GetNormal(double Landscape, double Ordinate)//���㶥�㷨ʸ��
{
	CVector3 Normal;
	double Landscape1, Landscape2;
	double Ordinate1, Ordinate2;

	Landscape1 = Landscape - 5 * PI / 180;//������5�������һ��
	Landscape2 = Landscape + 5 * PI / 180;
	Ordinate1 = Ordinate - 5 * PI / 180;
	Ordinate2 = Ordinate + 5 * PI / 180;

	if (Landscape1 < 0)Landscape1 = (360 - 5)* PI / 180;//�غϴ�����
	if (Ordinate1 < 0)Ordinate1 = (360 - 5) * PI / 180;
	if (Landscape2 == 2 * PI)Landscape2 = 0;
	if (Ordinate2 == 2 * PI)Ordinate2 = 0;

	CFacet F[4];//С��
	CP3 *P1;//��һ������ĸ���
	CP3 *P2;//�ڶ�������ĸ���
	CP3 *P3;//����������ĸ���
	CP3 *P4;//���ĸ�����ĸ���

	P1 = GetPoint(Landscape1, Landscape, Ordinate1, Ordinate);
	F[0].SetFacetNormal(P1[0], P1[1], P1[2], P1[3]);
	P2 = GetPoint(Landscape, Landscape2, Ordinate1, Ordinate);
	F[1].SetFacetNormal(P2[0], P2[1], P2[2], P2[3]);
	P3 = GetPoint(Landscape, Landscape2, Ordinate, Ordinate2);
	F[2].SetFacetNormal(P3[0], P3[1], P3[2], P3[3]);
	P4 = GetPoint(Landscape1, Landscape, Ordinate, Ordinate2);
	F[3].SetFacetNormal(P4[0], P4[1], P4[2], P4[3]);

	Normal = (F[0].fNormal + F[1].fNormal + F[2].fNormal + F[3].fNormal).Normalize();//���㷨ʸ��

	delete[] P1; delete[] P2; delete[] P3; delete[] P4;
	return Normal;
}

void CTorus::DrawFacet(CDC *pDC, CZBuffer* pZBuffer, CP3 P[4], CVector3 N[4])
{
	CP3 ScreenP[4];//ͶӰ��
	CP3 ViewPoint = Projection.GetEye();//�ӵ�
	CVector3 ViewVector(P[1], ViewPoint);//�����ʸ��
	ViewVector = ViewVector.Normalize();//��λ����ʸ��

	CFacet F;//С��
	F.SetFacetNormal(P[0], P[1], P[2], P[3]);
	F.fNormal = F.fNormal.Normalize();//��λ����ʸ��

	if (DotProduct(ViewVector, F.fNormal) >= 0)//�����޳�
	{
		for (int i = 0;i < 4;i++)
			ScreenP[i] = Projection.PerspectiveProjection(P[i]);//͸��ͶӰ

		pZBuffer->SetPoint(ScreenP[0], ScreenP[1], ScreenP[2], N[0], N[1], N[2], P[0], P[1], P[2]);
		pZBuffer->PhongShading(pDC, ViewPoint, pLight, pMaterial);
		pZBuffer->SetPoint(ScreenP[0], ScreenP[2], ScreenP[3], N[0], N[2], N[3], P[0], P[2], P[3]);
		pZBuffer->PhongShading(pDC, ViewPoint, pLight, pMaterial);
	}
}