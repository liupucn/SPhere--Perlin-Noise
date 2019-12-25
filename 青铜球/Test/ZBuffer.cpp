#include "stdafx.h"
#include "Test.h"
#include "ZBuffer.h"
#include <math.h>
#define Round(d) int(floor(d + 0.5))//��������궨��
#define PI 3.1415926

#define B_n 0x100       //256  
#define BM  0xff        //255  
#define Num 0x1000      //4096  

static int p[B_n + B_n + 2];//�±�
static double G3[B_n + B_n + 2][3];//α���ֵ

#define s_curve(t) (t * t * t * (t * (t * 6. - 15.) + 10.))//��������
#define lerp(t, a, b) (a + t * (b - a))//��ֵ
#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + Num; \
	b0 = ((int)t) & BM; \
	b1 = (b0 + 1) & BM; \
	r0 = t - (int)t; \
	r1 = r0 - 1.;

CZBuffer::CZBuffer(void)
{
	zBuffer = NULL;
	RandG();
}

CZBuffer::~CZBuffer(void)
{
	for(int i = 0;i < nWidth;i++)
	{
		delete[] zBuffer[i];
		zBuffer[i] = NULL;
	}
	if(zBuffer != NULL)
	{
		delete zBuffer;
		zBuffer = NULL;
	}	
}


void CZBuffer::SetPoint(CP3 P0, CP3 P1, CP3 P2, CVector3 N0, CVector3 N1, CVector3 N2, CP3 T0, CP3 T1, CP3 T2)
{
	this->P0 = P0, this->P1 = P1, this->P2 = P2;
	point0.x = Round(P0.x);
	point0.y = Round(P0.y);
	point0.z = P0.z;
	point0.c = P0.c;
	point0.n = N0;
	point0.T3D = T0;

	point1.x = Round(P1.x);
	point1.y = Round(P1.y);
	point1.z = P1.z;
	point1.c = P1.c;
	point1.n = N1;
	point1.T3D = T1;

	point2.x = Round(P2.x);
	point2.y = Round(P2.y);
	point2.z = P2.z;
	point2.c = P2.c;
	point2.n = N2;
	point2.T3D = T2;
}

void CZBuffer::PhongShading(CDC* pDC, CP3 ViewPoint, CLighting* pLight, CMaterial* pMaterial)
{
	double	CurrentDepth = 0.0;//��ǰɨ���ߵ����
	CVector3 Vector01(P0, P1), Vector02(P0, P2);
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;//ƽ�淽��Ax+By+Cz��D=0��ϵ��
	double D = -A * P0.x - B * P0.y - C * P0.z;//��ǰɨ��������x��������Ȳ���
	if(fabs(C) < 1e-4)
		C = 1.0;
	double DepthStep = -A/C;//����ɨ������Ȳ�������
	//�����ܹ���������
	if(point0 != point1 && point0 != point2 && point1 != point2)	//������ ��= �����
	{
		//point0��Ϊy������С�ĵ�,point1��Ϊy�������ĵ�,point2���y����λ�ڶ���֮�䡣���yֵ��ͬ��ȡx��С�ĵ�
		SortVertex();
		//���������θ��ǵ�ɨ��������
		int nTotalLine = point1.y - point0.y + 1;
		//����span��������յ�����
		pLeft = new CPoint2[nTotalLine];
		pRight = new CPoint2[nTotalLine];
		//�ж���������P0P1�ߵ�λ�ù�ϵ��0-1-2Ϊ����ϵ
		int nDeltz = (point1.x - point0.x) * (point2.y -point1.y) - (point1.y - point0.y) * (point2.x - point1.x);//��ʸ�������z����
		if (nDeltz > 0)//������λ��P0P1�ߵ����
		{
			nIndex = 0;
			EdgeFlag(point0, point2, TRUE);
			EdgeFlag(point2, point1, TRUE);
			nIndex = 0;
			EdgeFlag(point0, point1, FALSE);
		}
		else//������λ��P0P1�ߵ��Ҳ�
		{
			nIndex = 0;
			EdgeFlag(point0, point1, TRUE);
			nIndex = 0;
			EdgeFlag(point0, point2, FALSE);
			EdgeFlag(point2, point1, FALSE);
		}
		for (int y = point0.y; y < point1.y; y++)//�±��Ͽ�
		{
			int n = y - point0.y;
			BOOL bInFlag = FALSE;//���������Ա�־����ʼֵΪ�ٱ�ʾ�������ⲿ
			for(int x = pLeft[n].x; x < pRight[n].x; x++)//����ҿ�
			{
				if(bInFlag == FALSE)
				{
					CurrentDepth = -(A * x + B * y + D) / C;//z=-(Ax+By+D)/C
					bInFlag = TRUE;
					x -= 1;
				}
				else
				{
					CVector3 Normal = LinearInterp(x, pLeft[n].x, pRight[n].x, pLeft[n].n, pRight[n].n);
					CP3 T3D= LinearInterp(x, pLeft[n].x, pRight[n].x, pLeft[n].T3D, pRight[n].T3D);

					CTransform Tran;
					Tran.SetMat(&T3D, 1);
					Tran.Scale(0.03, 0.03, 0.03);

					double lacunarity = 1.6;//�ռ�Ƶ��
					double gain = 0.75;//�������
					double  vec[3] = { T3D.x,T3D.y,T3D.z };
					double noise = (Noise3D(vec)*lacunarity)*gain;
					Normal = Normal + CVector3(noise, noise, 0);//�����Ŷ�����
					Normal = Normal.Normalize();//��һ��������

					CRGBA clr = pLight->illuminate(ViewPoint, CP3(x, y, CurrentDepth), Normal, pMaterial);
					if(CurrentDepth <= zBuffer[x + nWidth/2][y + nHeight/2])//�����ǰ����������С��֡��������ԭ����������
					{
						zBuffer[x + nWidth/2][y + nHeight/2] = CurrentDepth;//ʹ�õ�ǰ���������ȸ�����Ȼ�����
						pDC->SetPixelV(x, y, RGB(clr.red*255, clr.green * 255, clr.blue * 255));
					}
					CurrentDepth += DepthStep;					
				}
			}
		}
		if(pLeft)
		{
			delete[]pLeft;
			pLeft = NULL;
		}
		if(pRight)
		{
			delete[]pRight;
			pRight = NULL;
		}
	}
}

void CZBuffer::EdgeFlag(CPoint2 PStart, CPoint2 PEnd, BOOL bFeature)
{
	CVector3 ptNormal = PStart.n;
	CP3 T3D = PStart.T3D;

	int dx = abs(PEnd.x - PStart.x);
	int dy = abs(PEnd.y - PStart.y);
	BOOL bInterChange = FALSE;//bInterChangeΪ�٣���λ�Ʒ���Ϊx����
	int e, s1, s2, temp;
	s1 = (PEnd.x > PStart.x) ? 1 : ((PEnd.x < PStart.x) ? -1 : 0);
	s2 = (PEnd.y > PStart.y) ? 1 : ((PEnd.y < PStart.y) ? -1 : 0);
	if (dy > dx)//bInterChangeΪ�棬��λ�Ʒ���Ϊy����
	{
		temp = dx;
		dx = dy;
		dy = temp;
		bInterChange = TRUE;
	}
	e = -dx;
	int x = PStart.x, y = PStart.y;
	for (int i = 0; i < dx; i++)
	{
		if (bFeature)
			pLeft[nIndex] = CPoint2(x, y, ptNormal, T3D);
		else
			pRight[nIndex] = CPoint2(x, y, ptNormal, T3D);
		if (bInterChange)
		{
			y += s2;
			ptNormal = LinearInterp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);//��ֵ������һ������
			T3D = LinearInterp(y, PStart.y, PEnd.y, PStart.T3D, PEnd.T3D);//��ֵ������һ������
			if (bFeature)
				pLeft[++nIndex] = CPoint2(x, y, ptNormal, T3D);
			else
				pRight[++nIndex] = CPoint2(x, y, ptNormal, T3D);
		}
		else
		{
			x += s1;
			ptNormal = LinearInterp(x, PStart.x, PEnd.x, PStart.n, PEnd.n);
			T3D = LinearInterp(x, PStart.x, PEnd.x, PStart.T3D, PEnd.T3D);
		}
		e += 2 * dy;
		if (e >= 0)
		{
			if (bInterChange)
			{
				x += s1;
				ptNormal = LinearInterp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
				T3D = LinearInterp(y, PStart.y, PEnd.y, PStart.T3D, PEnd.T3D);
			}
			else
			{
				y += s2;
				ptNormal = LinearInterp(x, PStart.x, PEnd.x, PStart.n, PEnd.n);
				T3D = LinearInterp(x, PStart.x, PEnd.x, PStart.T3D, PEnd.T3D);
				if (bFeature)
					pLeft[++nIndex] = CPoint2(x, y, ptNormal, T3D);
				else
					pRight[++nIndex] = CPoint2(x, y, ptNormal, T3D);
			}
			e -= 2 * dx;
		}
	}
}

void CZBuffer::SortVertex(void)
{
	CPoint3 pt[3];
	pt[0] = point0;
	pt[1] = point1;
	pt[2] = point2;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = i + 1; j < 3; ++j)
		{
			int k = i;
			if (pt[k].y > pt[j].y)
				k = j;
			if (pt[k].y == pt[j].y)
				if (pt[k].x > pt[j].x)
					k = j;
			if (k == j)
			{
				CPoint3 pTemp = pt[i];
				pt[i] = pt[k];
				pt[k] = pTemp;
			}
		}
	}
	point0 = pt[0];
	point1 = pt[2];
	point2 = pt[1];
}

CRGBA CZBuffer::LinearInterp(double t, double coorStart, double coorEnd, CRGBA intensityStart, CRGBA intensityEnd)//��ǿ���Բ�ֵ
{
	CRGBA Intensity;
	Intensity = (t - coorEnd) / (coorStart - coorEnd) * intensityStart + (t - coorStart) / (coorEnd - coorStart) * intensityEnd;
	return Intensity;
}

CP3 CZBuffer::LinearInterp(double t, double t1, double t2, CP3 T1, CP3 T2)//�������Բ�ֵ
{
	CP3 T;
	T = (t - t2) / (t1 - t2)*(T1) + (t - t1) / (t2 - t1)*(T2);
	return T;
}

CVector3 CZBuffer::LinearInterp(double t, double coorStart, double coorEnd, CVector3 normalStart, CVector3 normalEnd)//��ʸ�����Բ�ֵ
{
	CVector3 normal;
	normal = (t - coorEnd) / (coorStart - coorEnd) * normalStart + (t - coorStart) / (coorEnd - coorStart) * normalEnd;
	return normal;
}

void CZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double zDepth)//��ʼ����Ȼ���
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double *[nWidth];
	for (int i = 0; i < nWidth; i++)
		zBuffer[i] = new double[nHeight];
	for (int i = 0; i < nWidth; i++)//��ʼ����Ȼ���
		for (int j = 0; j < nHeight; j++)
			zBuffer[i][j] = zDepth;
}

double CZBuffer::Noise3D(double vec[3])
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register int i, j;

	setup(0, bx0, bx1, rx0, rx1);
	setup(1, by0, by1, ry0, ry1);
	setup(2, bz0, bz1, rz0, rz1);

	i = p[bx0];
	j = p[bx1];

	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];

	t = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = G3[b00 + bz0]; u = at3(rx0, ry0, rz0);
	q = G3[b10 + bz0]; v = at3(rx1, ry0, rz0);
	a = lerp(t, u, v);//X���ֵ

	q = G3[b01 + bz0]; u = at3(rx0, ry1, rz0);
	q = G3[b11 + bz0]; v = at3(rx1, ry1, rz0);
	b = lerp(t, u, v);//X���ֵ

	c = lerp(sy, a, b);//Y���ֵ

	q = G3[b00 + bz1]; u = at3(rx0, ry0, rz1);
	q = G3[b10 + bz1]; v = at3(rx1, ry0, rz1);
	a = lerp(t, u, v);//X���ֵ

	q = G3[b01 + bz1]; u = at3(rx0, ry1, rz1);
	q = G3[b11 + bz1]; v = at3(rx1, ry1, rz1);
	b = lerp(t, u, v);//X���ֵ

	d = lerp(sy, a, b);//Y���ֵ

	return lerp(sz, c, d);//Z���ֵ��������ֵ
}

void CZBuffer::RandG(void)
{
	int i, j, k;
	for (i = 0; i < B_n; i++)//��ȡα���ֵ
	{
		p[i] = i;
		for (j = 0; j < 3; j++)
			G3[i][j] = (float)((rand() % (B_n + B_n)) - B_n) / B_n;
		double m = sqrt(pow(G3[i][0], 2.0) + pow(G3[i][1], 2.0) + pow(G3[i][2], 2.0));//��һ��
		G3[i][0] /= m;
		G3[i][1] /= m;
		G3[i][2] /= m;
	}
	while (--i)//����˳��
	{
		k = p[i];
		p[i] = p[j = rand() % B_n];
		p[j] = k;
	}
	for (i = 0; i < B_n + 2; i++) //Copyα���ֵ
	{
		p[B_n + i] = p[i];
		for (j = 0; j < 3; j++)
			G3[B_n + i][j] = G3[i][j];
	}
}