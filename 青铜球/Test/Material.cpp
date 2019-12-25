#include "StdAfx.h"
#include "Material.h"

CMaterial::CMaterial(void)
{
	M_Ambient = CRGBA(0.2, 0.2, 0.2);//���ʵĻ���������
	M_Diffuse = CRGBA(0.8, 0.8, 0.8);//���ʵ���������
	M_Specular = CRGBA(0.0, 0.0, 0.0);//���ʵľ��淴����
}

CMaterial::~CMaterial(void)
{

}

void CMaterial::SetAmbient(CRGBA c)
{
	M_Ambient = c;
}

void CMaterial::SetDiffuse(CRGBA c)
{
	M_Diffuse = c;
}

void CMaterial::SetSpecular(CRGBA c)
{
	M_Specular = c;
}

void CMaterial::SetFresnel(double f)
{
	M_f = f;
}

void CMaterial::SetRoughness(double k)
{
	M_k = k;
}