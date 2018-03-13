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

// CUDA_OpenGL_glfw_glewView.h : interface of the CCUDA_OpenGL_glfw_glewView class
//

#pragma once


class CCUDA_OpenGL_glfw_glewView : public CView
{
protected: // create from serialization only
	CCUDA_OpenGL_glfw_glewView();
	DECLARE_DYNCREATE(CCUDA_OpenGL_glfw_glewView)

// Attributes
public:
	CCUDA_OpenGL_glfw_glewDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCUDA_OpenGL_glfw_glewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CUDA_OpenGL_glfw_glewView.cpp
inline CCUDA_OpenGL_glfw_glewDoc* CCUDA_OpenGL_glfw_glewView::GetDocument() const
   { return reinterpret_cast<CCUDA_OpenGL_glfw_glewDoc*>(m_pDocument); }
#endif

