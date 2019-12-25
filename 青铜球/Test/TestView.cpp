
// TestView.cpp : CTestView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//������ѧͷ�ļ�
#define  PI 3.1415926//PI�ĺ궨��
#define Round(d) int(floor(d+0.5))//��������궨��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDM_PLAY, &CTestView::OnPlay)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTestView ����/����

CTestView::CTestView()
{
	// TODO: �ڴ˴���ӹ������

	bPlay = FALSE;
	Alpha = Beta = 0;
	int Scale = 350;
	InitializeLightingScene();//��ʼ�����ճ���
	Sphere.SetScale(Scale);
	Sphere.SetScene(pLight, pMaterial);
}

CTestView::~CTestView()
{
	if (pLight != NULL)
	{
		delete pLight;
		pLight = NULL;
	}
	if (pMaterial != NULL)
	{
		delete pMaterial;
		pMaterial = NULL;
	}
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTestView ����

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	DoubleBuffer(pDC);	
}


// CTestView ��ӡ

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CTestView ���

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestView ��Ϣ�������

void CTestView::InitializeLightingScene(void)//��ʼ�����ճ���
{
	//���ջ���
	nLightSourceNumber = 1;//��Դ����
	pLight = new CLighting(nLightSourceNumber);//һά��Դ��̬����
	pLight->LightSource[0].SetPosition(500, 500, 800);//���ù�Դλ������
	for (int i = 0; i < nLightSourceNumber; i++)
	{
		pLight->LightSource[i].L_Diffuse = CRGBA(1.0, 1.0, 1.0);//��Դ����������ɫ	
		pLight->LightSource[i].L_Specular = CRGBA(0.7, 0.7, 0.7);//��Դ����߹���ɫ
		pLight->LightSource[i].L_C0 = 1.0;//����˥��ϵ��
		pLight->LightSource[i].L_C1 = 0.0000001;//����˥��ϵ��	
		pLight->LightSource[i].L_C2 = 0.00000001;//����˥��ϵ��
		pLight->LightSource[i].L_OnOff = TRUE;//��Դ����
	}
	//��������
	pMaterial = new CMaterial;
	pMaterial->SetAmbient(CRGBA(0.192, 0.192, 0.102));//���ʶԻ�����ķ�����
	pMaterial->SetDiffuse(CRGBA(0.949, 0.752, 0.337));//���ʶԻ�������������ķ��������
	pMaterial->SetSpecular(CRGBA(1.0, 1.0, 1.0));//���ʶԾ��淴���ķ�����
	pMaterial->SetFresnel(0.3);//������ϵ��
	pMaterial->SetRoughness(0.5);//�ֲڳ̶�
}

void CTestView::DoubleBuffer(CDC *pDC)//˫����
{
	CRect rect;//����ͻ�������
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(rect.Width(),-rect.Height());//����������Χ,x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//�ͻ�������Ϊԭ��
	CDC memDC;//�ڴ�DC
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г��ص���ʱλͼ
	memDC.CreateCompatibleDC(pDC);//����һ������ʾpDC���ݵ��ڴ�memDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=memDC.SelectObject(&NewBitmap);//������λͼѡ��memDC 
	//memDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ
	memDC.SetMapMode(MM_ANISOTROPIC);//memDC�Զ�������ϵ
	memDC.SetWindowExt(rect.Width(),rect.Height());
	memDC.SetViewportExt(rect.Width(),-rect.Height());
	memDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	DrawObject(&memDC);//��memDC����ͼ��
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
}

void CTestView::DrawObject(CDC *pDC)//����ģ��
{
	CZBuffer* pZBuffer = new CZBuffer;
	pZBuffer->InitialDepthBuffer(1200, 800, 800);//��ʼ����Ȼ�����
	Sphere.Draw(pDC, pZBuffer);
	delete pZBuffer;
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!bPlay)
	{
		switch (nChar)
		{
		case VK_UP:
			Alpha -= 5;
			break;
		case VK_DOWN:
			Alpha += 5;
			break;
		case VK_LEFT:
			Beta -= 4;
			break;
		case VK_RIGHT:
			Beta += 4;
			break;
		default:
			break;
		}
		Sphere.SetRotation(Alpha, Beta);
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CTestView::OnPlay()
{
	// TODO: �ڴ���������������
	bPlay = !bPlay;
	if (bPlay)
		SetTimer(1, 150, NULL);
	else
		KillTimer(1);
}


void CTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	Beta -= 4;
	Alpha += 5;
	Sphere.SetRotation(Alpha, Beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
