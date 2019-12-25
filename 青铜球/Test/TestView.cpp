
// TestView.cpp : CTestView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//包含数学头文件
#define  PI 3.1415926//PI的宏定义
#define Round(d) int(floor(d+0.5))//四舍五入宏定义

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDM_PLAY, &CTestView::OnPlay)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTestView 构造/析构

CTestView::CTestView()
{
	// TODO: 在此处添加构造代码

	bPlay = FALSE;
	Alpha = Beta = 0;
	int Scale = 350;
	InitializeLightingScene();//初始化光照场景
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestView 绘制

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DoubleBuffer(pDC);	
}


// CTestView 打印

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTestView 诊断

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestView 消息处理程序

void CTestView::InitializeLightingScene(void)//初始化光照场景
{
	//光照环境
	nLightSourceNumber = 1;//光源个数
	pLight = new CLighting(nLightSourceNumber);//一维光源动态数组
	pLight->LightSource[0].SetPosition(500, 500, 800);//设置光源位置坐标
	for (int i = 0; i < nLightSourceNumber; i++)
	{
		pLight->LightSource[i].L_Diffuse = CRGBA(1.0, 1.0, 1.0);//光源的漫反射颜色	
		pLight->LightSource[i].L_Specular = CRGBA(0.7, 0.7, 0.7);//光源镜面高光颜色
		pLight->LightSource[i].L_C0 = 1.0;//常数衰减系数
		pLight->LightSource[i].L_C1 = 0.0000001;//线性衰减系数	
		pLight->LightSource[i].L_C2 = 0.00000001;//二次衰减系数
		pLight->LightSource[i].L_OnOff = TRUE;//光源开启
	}
	//材质属性
	pMaterial = new CMaterial;
	pMaterial->SetAmbient(CRGBA(0.192, 0.192, 0.102));//材质对环境光的反射率
	pMaterial->SetDiffuse(CRGBA(0.949, 0.752, 0.337));//材质对环境光和漫反射光的反射率相等
	pMaterial->SetSpecular(CRGBA(1.0, 1.0, 1.0));//材质对镜面反射光的反射率
	pMaterial->SetFresnel(0.3);//菲涅尔系数
	pMaterial->SetRoughness(0.5);//粗糙程度
}

void CTestView::DoubleBuffer(CDC *pDC)//双缓冲
{
	CRect rect;//定义客户区矩形
	GetClientRect(&rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(),-rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//客户区中心为原点
	CDC memDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载的临时位图
	memDC.CreateCompatibleDC(pDC);//创建一个与显示pDC兼容的内存memDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图 
	pOldBitmap=memDC.SelectObject(&NewBitmap);//将兼容位图选入memDC 
	//memDC.FillSolidRect(rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色
	memDC.SetMapMode(MM_ANISOTROPIC);//memDC自定义坐标系
	memDC.SetWindowExt(rect.Width(),rect.Height());
	memDC.SetViewportExt(rect.Width(),-rect.Height());
	memDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	DrawObject(&memDC);//向memDC绘制图形
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存memDC中的位图拷贝到显示pDC中
	memDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
}

void CTestView::DrawObject(CDC *pDC)//绘制模型
{
	CZBuffer* pZBuffer = new CZBuffer;
	pZBuffer->InitialDepthBuffer(1200, 800, 800);//初始化深度缓冲器
	Sphere.Draw(pDC, pZBuffer);
	delete pZBuffer;
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加命令处理程序代码
	bPlay = !bPlay;
	if (bPlay)
		SetTimer(1, 150, NULL);
	else
		KillTimer(1);
}


void CTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	Beta -= 4;
	Alpha += 5;
	Sphere.SetRotation(Alpha, Beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
