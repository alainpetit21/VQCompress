// VQCopress.h : main header file for the VQCOPRESS application
//

#if !defined(AFX_VQCOPRESS_H__A5C9E3AE_A345_4A6D_B4A3_06E9B6628DC0__INCLUDED_)
#define AFX_VQCOPRESS_H__A5C9E3AE_A345_4A6D_B4A3_06E9B6628DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVQCopressApp:
// See VQCopress.cpp for the implementation of this class
//

class CVQCopressApp : public CWinApp
{
public:
	CVQCopressApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVQCopressApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVQCopressApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VQCOPRESS_H__A5C9E3AE_A345_4A6D_B4A3_06E9B6628DC0__INCLUDED_)
