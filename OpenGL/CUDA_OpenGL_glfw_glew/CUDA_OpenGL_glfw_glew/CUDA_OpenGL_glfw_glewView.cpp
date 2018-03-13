// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CUDA_OpenGL_glfw_glewView.cpp : implementation of the CCUDA_OpenGL_glfw_glewView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CUDA_OpenGL_glfw_glew.h"
#endif

#include "CUDA_OpenGL_glfw_glewDoc.h"
#include "CUDA_OpenGL_glfw_glewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCUDA_OpenGL_glfw_glewView

IMPLEMENT_DYNCREATE(CCUDA_OpenGL_glfw_glewView, CView)

BEGIN_MESSAGE_MAP(CCUDA_OpenGL_glfw_glewView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCUDA_OpenGL_glfw_glewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCUDA_OpenGL_glfw_glewView construction/destruction

CCUDA_OpenGL_glfw_glewView::CCUDA_OpenGL_glfw_glewView()
{
	// TODO: add construction code here

}

CCUDA_OpenGL_glfw_glewView::~CCUDA_OpenGL_glfw_glewView()
{
}

BOOL CCUDA_OpenGL_glfw_glewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCUDA_OpenGL_glfw_glewView drawing

void CCUDA_OpenGL_glfw_glewView::OnDraw(CDC* /*pDC*/)
{
	CCUDA_OpenGL_glfw_glewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CCUDA_OpenGL_glfw_glewView printing


void CCUDA_OpenGL_glfw_glewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCUDA_OpenGL_glfw_glewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCUDA_OpenGL_glfw_glewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCUDA_OpenGL_glfw_glewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCUDA_OpenGL_glfw_glewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCUDA_OpenGL_glfw_glewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCUDA_OpenGL_glfw_glewView diagnostics

#ifdef _DEBUG
void CCUDA_OpenGL_glfw_glewView::AssertValid() const
{
	CView::AssertValid();
}

void CCUDA_OpenGL_glfw_glewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCUDA_OpenGL_glfw_glewDoc* CCUDA_OpenGL_glfw_glewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCUDA_OpenGL_glfw_glewDoc)));
	return (CCUDA_OpenGL_glfw_glewDoc*)m_pDocument;
}
#endif //_DEBUG


// CCUDA_OpenGL_glfw_glewView message handlers
