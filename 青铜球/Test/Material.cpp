#include "StdAfx.h"
#include "Material.h"

CMaterial::CMaterial(void)
{
	M_Ambient = CRGBA(0.2, 0.2, 0.2);//材质的环境反射率
	M_Diffuse = CRGBA(0.8, 0.8, 0.8);//材质的漫反射率
	M_Specular = CRGBA(0.0, 0.0, 0.0);//材质的镜面反射率
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