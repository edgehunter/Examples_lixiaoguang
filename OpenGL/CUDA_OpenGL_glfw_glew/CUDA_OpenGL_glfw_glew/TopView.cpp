// TopView.cpp : implementation file
//

#include "stdafx.h"
#include "CUDA_OpenGL_glfw_glew.h"
#include "TopView.h"


// CTopView

IMPLEMENT_DYNCREATE(CTopView, CView)

CTopView::CTopView()
{

}

CTopView::~CTopView()
{
}

BEGIN_MESSAGE_MAP(CTopView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTopView drawing

void CTopView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CTopView diagnostics

#ifdef _DEBUG
void CTopView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTopView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTopView message handlers


void CTopView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect;
	this->GetClientRect(&rect);

	CCUDA_OpenGL_glfw_glewDoc* pDoc = (CCUDA_OpenGL_glfw_glewDoc*)GetDocument();
	if (pDoc->Get_OpenGL()->IsCreate_GL())
	{
		pDoc->Get_OpenGL()->ResizeWindows_GL(cx, cy);// (rect.Width(), rect.Height());
	}
	

	// TODO: Add your message handler code here
}


void CTopView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
}


int CTopView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	SetTimer(NULL, 10, NULL);

	return 0;
}

void CTopView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CCUDA_OpenGL_glfw_glewDoc* pDoc = (CCUDA_OpenGL_glfw_glewDoc*)GetDocument();
	if (pDoc->Get_OpenGL()->IsCreate_GL())
	{
		pDoc->Get_OpenGL()->Render_GL();
	}
	
	CView::OnTimer(nIDEvent);
}