#pragma once
#include "RGBA.h"

class CMaterial
{
public:
	CMaterial(void);
	virtual~CMaterial(void);
	void SetAmbient(CRGBA c);//设置环境反射率
	void SetDiffuse(CRGBA c);//设置漫反射率
	void SetSpecular(CRGBA c);//设置镜面反射率
	void SetFresnel(double f);//设置菲涅尔反射系数
	void SetRoughness(double k);//设置粗糙度
public:
	CRGBA M_Ambient;//环境反射率
	CRGBA M_Diffuse;//漫反射率
	CRGBA M_Specular;//镜面反射率
	double M_f;//菲涅尔反射系数
	double M_k;//斜率描述的粗糙度
};