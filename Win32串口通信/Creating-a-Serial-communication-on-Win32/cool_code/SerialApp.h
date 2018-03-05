// SerialApp.h : main header file for the SERIALAPP application
//

#if !defined(AFX_SERIALAPP_H__361F5FC9_B80B_4224_805D_20EA4F9314AC__INCLUDED_)
#define AFX_SERIALAPP_H__361F5FC9_B80B_4224_805D_20EA4F9314AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSerialAppApp:
// See SerialApp.cpp for the implementation of this class
//

class CSerialAppApp : public CWinApp
{
public:
	CSerialAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALAPP_H__361F5FC9_B80B_4224_805D_20EA4F9314AC__INCLUDED_)
