// EmailSend.h : main header file for the EMAILSEND application
//

#if !defined(AFX_EMAILSEND_H__0BB4A810_0B68_47AC_9FB1_C1CF1344B737__INCLUDED_)
#define AFX_EMAILSEND_H__0BB4A810_0B68_47AC_9FB1_C1CF1344B737__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEmailSendApp:
// See EmailSend.cpp for the implementation of this class
//

class CEmailSendApp : public CWinApp
{
public:
	CEmailSendApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmailSendApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEmailSendApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILSEND_H__0BB4A810_0B68_47AC_9FB1_C1CF1344B737__INCLUDED_)
