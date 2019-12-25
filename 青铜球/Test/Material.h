#pragma once
#include "RGBA.h"

class CMaterial
{
public:
	CMaterial(void);
	virtual~CMaterial(void);
	void SetAmbient(CRGBA c);//���û���������
	void SetDiffuse(CRGBA c);//������������
	void SetSpecular(CRGBA c);//���þ��淴����
	void SetFresnel(double f);//���÷���������ϵ��
	void SetRoughness(double k);//���ôֲڶ�
public:
	CRGBA M_Ambient;//����������
	CRGBA M_Diffuse;//��������
	CRGBA M_Specular;//���淴����
	double M_f;//����������ϵ��
	double M_k;//б�������Ĵֲڶ�
};