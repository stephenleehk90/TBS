// Prepare.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Prepare.h"
#include "PrepareDlg.h"
//#include "Distribute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TCHAR g_sPrompt[4096];
HANDLE g_hKillEvent;
CCriticalSection g_csPrompt;

UINT DistributeFiles(LPVOID pPraram)
{
	CDistribute *pDistribute = (CDistribute *)pPraram;
	
	int nBreak = pDistribute->DistributeFiles();

	::PostMessage(pDistribute->m_hWnd, WM_THREADFINISHED, 0, nBreak);
	
	return nBreak;
}


/////////////////////////////////////////////////////////////////////////////
// CPrepareApp

BEGIN_MESSAGE_MAP(CPrepareApp, CWinApp)
	//{{AFX_MSG_MAP(CPrepareApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrepareApp construction

CPrepareApp::CPrepareApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrepareApp object

CPrepareApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPrepareApp initialization

BOOL CPrepareApp::InitInstance()
{
	AfxEnableControlContainer();

	// Get Path of Setting INI File
	char    szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	char    szFName[_MAX_FNAME], szExt[_MAX_EXT];
	char	pszProfile[_MAX_PATH];
	
	GetModuleFileName(m_hInstance, pszProfile, _MAX_PATH);
    _splitpath(pszProfile, szDrive, szDir, szFName, szExt);
    strcpy(szExt, "INI");
	_makepath(pszProfile, szDrive, szDir, szFName, szExt);	
	free((void*)m_pszProfileName);
	//The CWinApp destructor will free the memory.
	m_pszProfileName=_tcsdup(pszProfile);

	g_hKillEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPrepareDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CPrepareApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CloseHandle(g_hKillEvent);
	return CWinApp::ExitInstance();
}
