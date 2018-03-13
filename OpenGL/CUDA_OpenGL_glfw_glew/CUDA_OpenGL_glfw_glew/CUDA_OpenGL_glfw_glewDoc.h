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

// CUDA_OpenGL_glfw_glewDoc.h : interface of the CCUDA_OpenGL_glfw_glewDoc class
//


#pragma once

#include "OpenGL.h"


class CCUDA_OpenGL_glfw_glewDoc : public CDocument
{
protected: // create from serialization only
	CCUDA_OpenGL_glfw_glewDoc();
	DECLARE_DYNCREATE(CCUDA_OpenGL_glfw_glewDoc)

// Attributes
public:

	OpenGL* Get_OpenGL();
private:

	OpenGL* m_OpenGL;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CCUDA_OpenGL_glfw_glewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
