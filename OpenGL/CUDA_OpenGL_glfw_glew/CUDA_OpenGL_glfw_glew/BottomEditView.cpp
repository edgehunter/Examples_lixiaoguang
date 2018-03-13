// BottomEditView.cpp : implementation file
//

#include "stdafx.h"
#include "CUDA_OpenGL_glfw_glew.h"
#include "BottomEditView.h"


// CBottomEditView

IMPLEMENT_DYNCREATE(CBottomEditView, CEditView)

CBottomEditView::CBottomEditView()
{

}

CBottomEditView::~CBottomEditView()
{
}

BEGIN_MESSAGE_MAP(CBottomEditView, CEditView)
END_MESSAGE_MAP()


// CBottomEditView diagnostics

#ifdef _DEBUG
void CBottomEditView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CBottomEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBottomEditView message handlers
