// Prepare.h : main header file for the PREPARE application
//

#if !defined(AFX_PREPARE_H__3AD0AFD7_23CF_4C52_BDB0_D63FBAB1D3A9__INCLUDED_)
#define AFX_PREPARE_H__3AD0AFD7_23CF_4C52_BDB0_D63FBAB1D3A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define WM_THREADFINISHED	(WM_USER + 110)
#define WM_THREADPROMPT		(WM_USER + 111)

extern TCHAR g_sPrompt[4096];
extern HANDLE g_hKillEvent;
extern CCriticalSection g_csPrompt;

UINT DistributeFiles(LPVOID pPraram);
/////////////////////////////////////////////////////////////////////////////
// CPrepareApp:
// See Prepare.cpp for the implementation of this class
//

class CPrepareApp : public CWinApp
{
public:
	CPrepareApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrepareApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPrepareApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREPARE_H__3AD0AFD7_23CF_4C52_BDB0_D63FBAB1D3A9__INCLUDED_)
