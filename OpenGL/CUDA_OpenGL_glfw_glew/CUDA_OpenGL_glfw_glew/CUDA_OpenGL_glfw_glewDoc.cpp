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

// CUDA_OpenGL_glfw_glewDoc.cpp : implementation of the CCUDA_OpenGL_glfw_glewDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CUDA_OpenGL_glfw_glew.h"
#endif

#include "CUDA_OpenGL_glfw_glewDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCUDA_OpenGL_glfw_glewDoc

IMPLEMENT_DYNCREATE(CCUDA_OpenGL_glfw_glewDoc, CDocument)

BEGIN_MESSAGE_MAP(CCUDA_OpenGL_glfw_glewDoc, CDocument)
END_MESSAGE_MAP()


// CCUDA_OpenGL_glfw_glewDoc construction/destruction

CCUDA_OpenGL_glfw_glewDoc::CCUDA_OpenGL_glfw_glewDoc()
{
	// TODO: add one-time construction code here

	m_OpenGL = new OpenGL();

}

CCUDA_OpenGL_glfw_glewDoc::~CCUDA_OpenGL_glfw_glewDoc()
{
}

BOOL CCUDA_OpenGL_glfw_glewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CCUDA_OpenGL_glfw_glewDoc serialization

void CCUDA_OpenGL_glfw_glewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCUDA_OpenGL_glfw_glewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCUDA_OpenGL_glfw_glewDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCUDA_OpenGL_glfw_glewDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCUDA_OpenGL_glfw_glewDoc diagnostics

#ifdef _DEBUG
void CCUDA_OpenGL_glfw_glewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCUDA_OpenGL_glfw_glewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCUDA_OpenGL_glfw_glewDoc commands

OpenGL* CCUDA_OpenGL_glfw_glewDoc::Get_OpenGL()
{
	return m_OpenGL;
}