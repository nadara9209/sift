// webcam.h : main header file for the WEBCAM application
//

#if !defined(AFX_WEBCAM_H__B44BA195_342C_428E_8977_C26BAD930D13__INCLUDED_)
#define AFX_WEBCAM_H__B44BA195_342C_428E_8977_C26BAD930D13__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWebcamApp:
// See webcam.cpp for the implementation of this class
//

class CWebcamApp : public CWinApp
{
public:
	CWebcamApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebcamApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWebcamApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCAM_H__B44BA195_342C_428E_8977_C26BAD930D13__INCLUDED_)
