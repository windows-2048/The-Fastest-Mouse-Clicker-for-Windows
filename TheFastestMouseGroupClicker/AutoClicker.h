// AutoClicker.h : main header file for the AUTOCLICKER application
//

#if !defined(AFX_AUTOCLICKER_H__0A4628F7_16F9_473A_B9F4_F57DE4767012__INCLUDED_)
#define AFX_AUTOCLICKER_H__0A4628F7_16F9_473A_B9F4_F57DE4767012__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoClickerApp:
// See AutoClicker.cpp for the implementation of this class
//

class CAutoClickerApp : public CWinApp
{
public:
	CAutoClickerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoClickerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAutoClickerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCLICKER_H__0A4628F7_16F9_473A_B9F4_F57DE4767012__INCLUDED_)
