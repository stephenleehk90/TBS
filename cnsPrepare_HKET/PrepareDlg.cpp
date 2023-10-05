// PrepareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Prepare.h"
#include "PrepareDlg.h"
#include "Settings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString SEC_FROM = _T("From");
const CString FROM_DATE_STYLE = _T("Date Style");
const CString FROM_SOURCEFOLDER = _T("Source Folder");
const CString FROM_SUB_FOLDER = _T("Sub Folder");
const CString FROM_SRC_SUB = _T("With Sub Folder");
const CString FROM_FILE_TYPES = _T("File Types");

const CString SEC_TO = _T("To");
const CString TELE_DATE_STYLE = _T("Date Style");
const CString TELE_SOURCEFOLDER = _T("Tele Floder");
const CString TELE_SUB_FOLDER = _T("Sub Name");
const CString TELE_SRC_SUB = _T("With Sub Folder");
const CString TO_FILE_TYPE = _T("File Type");
const CString TO_KEEP_SLUG = _T("Keep Slug");

const CString SEC_PHOTO = _T("Photo");
const CString PHOTO_CHECK = _T("Transfer Photo");
const CString PHOTO_DATE_STYLE = _T("Date Style");
const CString PHOTO_FOLDER = _T("Photo Folder");
const CString PHOTO_SUB_FOLDER = _T("Sub Folder");
const CString PHOTO_SUB = _T("With Sub Folder");
const CString PHOTO_FILE_TYPES = _T("File Types");

const CString SEC_SYS = _T("System");
const CString SYS_INTERVAL = _T("Interval");
const CString SYS_BEFORE = _T("Transfer Days Ago");
const CString SYS_AFTER = _T("Transfer Days Later");

const UINT TIMER_DISTRIBUTE = 1001;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrepareDlg dialog

CPrepareDlg::CPrepareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrepareDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrepareDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pThreadDistribute = NULL;
	m_nTimer = 0;
	m_nInterval = 1;
}

void CPrepareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrepareDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrepareDlg, CDialog)
	//{{AFX_MSG_MAP(CPrepareDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RUN, OnBtnRun)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETTINGS, OnBtnSettings)
	ON_BN_CLICKED(IDC_BTN_ERROR, OnBtnError)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADFINISHED, OnThreadFinished)
	ON_MESSAGE(WM_THREADPROMPT, OnThreadPrompt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrepareDlg message handlers

BOOL CPrepareDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	LoadProfile();


	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPrepareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPrepareDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPrepareDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CPrepareDlg::OnBtnRun() 
{
	// TODO: Add your control notification handler code here
	
	CString strStop = _T("Stop");		
	
	if (m_nTimer == 0)
	{
		m_distribute.m_hWnd = m_hWnd;
		
		// ...
		CString strError;
		if (m_distribute.Validate(strError))
		{
			GetDlgItem(IDC_BTN_RUN)->SetWindowText(strStop);

			Prompt(_T("Starting..."));
			OpenLogFile();
			m_pThreadDistribute = AfxBeginThread(DistributeFiles, &m_distribute);
			m_pThreadDistribute->m_bAutoDelete = TRUE;
			
			m_nTimer = -1;
		}
		else
		{
			AfxMessageBox(strError, MB_OK|MB_ICONSTOP);
		}
	}
	else
	{
		if(m_nTimer>0)
			KillTimer(m_nTimer);

		if (m_pThreadDistribute)
		{		
			SetEvent(g_hKillEvent);
			GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);
		}
		else
		{
			Prompt(_T("Cancelled by user!"));
			GetDlgItem(IDC_BTN_RUN)->SetWindowText(_T("Transfer"));
			m_nTimer = 0;
		}
	}
	
	GetDlgItem(IDC_BTN_SETTINGS)->EnableWindow(m_nTimer == 0);
}

void CPrepareDlg::SaveProfile() 
{
	// TODO: Add your control notification handler code here

	CPrepareApp *pApp = (CPrepareApp*)AfxGetApp();

	pApp->WriteProfileString(SEC_FROM,FROM_SOURCEFOLDER, m_distribute.m_strSourcePath);
	pApp->WriteProfileInt(SEC_FROM,FROM_DATE_STYLE, m_distribute.m_nSrcDTF);
	pApp->WriteProfileInt(SEC_FROM,FROM_SRC_SUB, m_distribute.m_bSrcSub?1:0);
	pApp->WriteProfileString(SEC_FROM,FROM_SUB_FOLDER, m_distribute.m_strSrcSubPath);
	pApp->WriteProfileString(SEC_FROM,FROM_FILE_TYPES, m_distribute.m_strSrcFileTypes);
	
	pApp->WriteProfileString(SEC_TO,TELE_SOURCEFOLDER, m_distribute.m_strTelePath);
	pApp->WriteProfileInt(SEC_TO,TELE_DATE_STYLE, m_distribute.m_nTeleDTF);
	pApp->WriteProfileInt(SEC_TO,TELE_SRC_SUB, m_distribute.m_bTeleSub?1:0);
	pApp->WriteProfileString(SEC_TO,TELE_SUB_FOLDER, m_distribute.m_strTeleSubPath);
	pApp->WriteProfileString(SEC_TO,TO_FILE_TYPE, m_distribute.m_strTeleFileType);
	pApp->WriteProfileInt(SEC_TO,TO_KEEP_SLUG, m_distribute.m_bSlug?1:0);
	
	pApp->WriteProfileInt(SEC_PHOTO,PHOTO_CHECK, m_distribute.m_bGraphic?1:0);
	pApp->WriteProfileString(SEC_PHOTO,PHOTO_FOLDER, m_distribute.m_strPhotoPath);
	pApp->WriteProfileInt(SEC_PHOTO,PHOTO_DATE_STYLE, m_distribute.m_nPhotoDTF);
	pApp->WriteProfileInt(SEC_PHOTO,PHOTO_SUB, m_distribute.m_bPhotoSub?1:0);
	pApp->WriteProfileString(SEC_PHOTO,PHOTO_SUB_FOLDER, m_distribute.m_strPhotoSubPath);
	pApp->WriteProfileString(SEC_PHOTO,PHOTO_FILE_TYPES, m_distribute.m_strPhotoFileTypes);

	pApp->WriteProfileInt(SEC_SYS, SYS_INTERVAL, m_nInterval);
	pApp->WriteProfileInt(SEC_SYS, SYS_BEFORE, m_distribute.m_nDaysBefore);
	pApp->WriteProfileInt(SEC_SYS, SYS_AFTER, m_distribute.m_nDaysAfter);

	CDialog::OnClose();
}

void CPrepareDlg::LoadProfile()
{
	CPrepareApp *pApp = (CPrepareApp*)AfxGetApp();

	m_distribute.m_strSourcePath = pApp->GetProfileString(SEC_FROM,FROM_SOURCEFOLDER);
	m_distribute.m_nSrcDTF = pApp->GetProfileInt(SEC_FROM,FROM_DATE_STYLE,0);
	m_distribute.m_bSrcSub = pApp->GetProfileInt(SEC_FROM,FROM_SRC_SUB,0);
	m_distribute.m_strSrcSubPath = pApp->GetProfileString(SEC_FROM,FROM_SUB_FOLDER);
	m_distribute.m_strSrcFileTypes = pApp->GetProfileString(SEC_FROM,FROM_FILE_TYPES,_T("*.cns"));

	m_distribute.m_strTelePath = pApp->GetProfileString(SEC_TO,TELE_SOURCEFOLDER);
	m_distribute.m_nTeleDTF = pApp->GetProfileInt(SEC_TO,TELE_DATE_STYLE,0);
	m_distribute.m_bTeleSub = pApp->GetProfileInt(SEC_TO,TELE_SRC_SUB,0);
	m_distribute.m_strTeleSubPath = pApp->GetProfileString(SEC_TO,TELE_SUB_FOLDER);
	m_distribute.m_strTeleFileType = pApp->GetProfileString(SEC_TO,TO_FILE_TYPE, _T("*.txt"));
	m_distribute.m_bSlug = pApp->GetProfileInt(SEC_TO,TO_KEEP_SLUG,1);

	m_distribute.m_bGraphic = pApp->GetProfileInt(SEC_PHOTO,PHOTO_CHECK,0);
	m_distribute.m_strPhotoPath = pApp->GetProfileString(SEC_PHOTO,PHOTO_FOLDER);
	m_distribute.m_nPhotoDTF = pApp->GetProfileInt(SEC_PHOTO,PHOTO_DATE_STYLE,0);
	m_distribute.m_bPhotoSub = pApp->GetProfileInt(SEC_PHOTO,PHOTO_SUB,0);
	m_distribute.m_strPhotoSubPath = pApp->GetProfileString(SEC_PHOTO,PHOTO_SUB_FOLDER);
	m_distribute.m_strPhotoFileTypes = pApp->GetProfileString(SEC_PHOTO,PHOTO_FILE_TYPES, _T("*.jpg;*.jpeg;*.tif;*.tiff;*.bmp"));

	m_nInterval = pApp->GetProfileInt(SEC_SYS, SYS_INTERVAL, 1);
	if (m_nInterval > 60)
		m_nInterval = 60;
	if (m_nInterval < 1)
		m_nInterval = 1;

	m_distribute.m_nDaysBefore = pApp->GetProfileInt(SEC_SYS, SYS_BEFORE, 0);
	if (m_distribute.m_nDaysBefore < 0)
		m_distribute.m_nDaysBefore = 0;
	
	m_distribute.m_nDaysAfter = pApp->GetProfileInt(SEC_SYS, SYS_AFTER, 0);
	if (m_distribute.m_nDaysAfter < 0)
		m_distribute.m_nDaysAfter = 0;

	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	char szFName[_MAX_FNAME], szExt[_MAX_EXT];
	char pszfilename[_MAX_PATH];
	
	GetModuleFileName(AfxGetApp()->m_hInstance, pszfilename, _MAX_PATH);
	_splitpath(pszfilename, szDrive, szDir, szFName, szExt);
	
	strcpy(szExt, "log");
	_makepath(pszfilename, szDrive, szDir, szFName, szExt);	
	m_strLogFilename = pszfilename;
	
	strcpy(szExt, "map");
	_makepath(pszfilename, szDrive, szDir, szFName, szExt);	
	m_distribute.m_strMapFilename = pszfilename;

}


void CPrepareDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_DISTRIBUTE)
	{
		KillTimer(m_nTimer);
		OpenLogFile();
		Prompt(_T("Starting..."));
		m_pThreadDistribute = AfxBeginThread(DistributeFiles, &m_distribute);
		m_pThreadDistribute->m_bAutoDelete = TRUE;
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CPrepareDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{
	CString strRun = _T("Transfer");

	ResetEvent(g_hKillEvent);

	
	GetDlgItem(IDC_BTN_RUN)->EnableWindow();
	
	m_pThreadDistribute = NULL;

	if (lParam == 0)
	{
		m_nTimer = SetTimer(TIMER_DISTRIBUTE,  m_nInterval * 60000, NULL);
		Prompt(_T("End.\r\nSleeping..."));
	}
	else
	{
		Prompt(_T("Cancelled by user!"));
		GetDlgItem(IDC_BTN_RUN)->SetWindowText(strRun);
		m_nTimer = 0;
		
	}
	CloseLogFile();
	GetDlgItem(IDC_BTN_SETTINGS)->EnableWindow(m_nTimer == 0);

	return 0;	
}
/*
 *	lParam != 0 means error
 */
LRESULT CPrepareDlg::OnThreadPrompt(WPARAM wParam, LPARAM lParam)
{
	CString strPrompt;
	g_csPrompt.Lock();
	strPrompt = g_sPrompt;
	g_csPrompt.Unlock();

	Prompt(strPrompt);

	if (lParam != 0){
		WriteLog(strPrompt, TRUE);
	}
	
	return 0;
}

void CPrepareDlg::Prompt(const CString & strPrompt)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_STATUS);
	
	CString strLog; 
	pEdit->GetWindowText(strLog);
	
	if (strLog.GetLength() > 8192)
	{
		strLog = strLog.Right(4096);
		
		int pos = strLog.Find(_T('\n')) + 1;
		
		strLog = strLog.Right(4096 - pos);
	}
	
	strLog = strLog + _T("\r\n") + strPrompt;
	SetDlgItemText(IDC_EDIT_STATUS, strLog);
	int nLine = pEdit->GetLineCount();
	pEdit->LineScroll(nLine+1);
}
void CPrepareDlg::OnBtnSettings() 
{
	// TODO: Add your control notification handler code here
	
	if (m_pThreadDistribute != NULL)
		return;
	
	if (m_nTimer==0)
	{
		
		CSettingsDlg dlg(this);
		
		dlg.m_strSourcePath = m_distribute.m_strSourcePath;
		dlg.m_strSrcSubPath = m_distribute.m_strSrcSubPath;
		dlg.m_strSrcFileTypes = m_distribute.m_strSrcFileTypes;
		dlg.m_nSrcDTF = m_distribute.m_nSrcDTF;
		dlg.m_bSrcSub = m_distribute.m_bSrcSub;
		dlg.m_strTelePath = m_distribute.m_strTelePath;
		dlg.m_strTeleSubPath = m_distribute.m_strTeleSubPath;
		dlg.m_bTeleSub = m_distribute.m_bTeleSub;
		dlg.m_nTeleDTF = m_distribute.m_nTeleDTF;
		dlg.m_strTeleFileType = m_distribute.m_strTeleFileType;
		dlg.m_bGraphic = m_distribute.m_bGraphic;
		dlg.m_bPhotoSub = m_distribute.m_bPhotoSub;
		dlg.m_nPhotoDTF = m_distribute.m_nPhotoDTF;
		dlg.m_strPhotoPath = m_distribute.m_strPhotoPath;
		dlg.m_strPhotoSubPath = m_distribute.m_strPhotoSubPath;
		dlg.m_strPhotoFileTypes = m_distribute.m_strPhotoFileTypes;
		dlg.m_nDaysAfter = m_distribute.m_nDaysAfter;
		dlg.m_nDaysBefore = m_distribute.m_nDaysBefore;
		
		dlg.m_nInterval = m_nInterval;
		dlg.m_bSlug = m_distribute.m_bSlug;
		
		if (dlg.DoModal())
		{
			m_distribute.m_strSourcePath = dlg.m_strSourcePath;
			m_distribute.m_strSrcSubPath = dlg.m_strSrcSubPath;
			m_distribute.m_strSrcFileTypes = dlg.m_strSrcFileTypes;
			m_distribute.m_nSrcDTF = dlg.m_nSrcDTF;
			m_distribute.m_bSrcSub = dlg.m_bSrcSub;
			m_distribute.m_strTelePath = dlg.m_strTelePath;
			m_distribute.m_strTeleSubPath = dlg.m_strTeleSubPath;
			m_distribute.m_bTeleSub = dlg.m_bTeleSub;
			m_distribute.m_nTeleDTF = dlg.m_nTeleDTF;
			m_distribute.m_strTeleFileType = dlg.m_strTeleFileType;
			m_distribute.m_bGraphic = dlg.m_bGraphic;
			m_distribute.m_bPhotoSub = dlg.m_bPhotoSub;
			m_distribute.m_nPhotoDTF = dlg.m_nPhotoDTF;
			m_distribute.m_strPhotoPath = dlg.m_strPhotoPath;
			m_distribute.m_strPhotoSubPath = dlg.m_strPhotoSubPath;
			m_distribute.m_strPhotoFileTypes = dlg.m_strPhotoFileTypes;
			m_distribute.m_nDaysAfter = dlg.m_nDaysAfter;
			m_distribute.m_nDaysBefore = dlg.m_nDaysBefore;
			m_nInterval = dlg.m_nInterval;
			m_distribute.m_bSlug = dlg.m_bSlug;

			SaveProfile();
		}
	}
}

BOOL CPrepareDlg::OpenLogFile()
{
	CFileException fe;
	if(m_fileLog.Open(m_strLogFilename, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite,&fe))
	{
		m_fileLog.SeekToEnd();
		return TRUE;
	}
	else
		return FALSE;
}
void CPrepareDlg::CloseLogFile()
{
	if (m_fileLog.m_hFile != CFile::hFileNull)
	{
		m_fileLog.Close();
		m_fileLog.m_hFile = CFile::hFileNull;
	}
}

BOOL CPrepareDlg::WriteLog(LPCTSTR lpMsg, BOOL bDisplayTime)
{
	static int _nLineCount = 0;
	if(m_fileLog.m_hFile == CFile::hFileNull)
		return FALSE;
	if(bDisplayTime)
	{
		CString strTime = COleDateTime::GetCurrentTime().Format(_T("\r\n%Y-%m-%d %H:%M:%S  "));
		m_fileLog.Write((LPCTSTR)strTime, strTime.GetLength() * sizeof(TCHAR));
	}
	m_fileLog.Write(lpMsg,_tcslen(lpMsg)*sizeof(TCHAR));
	m_fileLog.Write(_T("\r\n"),4);
	_nLineCount++;
	if(0 == (_nLineCount % 4))
	{
		m_fileLog.Flush();
		_nLineCount = 1;
	}

	if(m_fileLog.GetLength() > 512 * 1024L)
	{
		char *p;
		HGLOBAL h;
		h = GlobalAlloc(GHND, 256 * 1024L);
		if(h != NULL)
		{
			p = (char*)GlobalLock(h);
			if(p != NULL)
			{
				try
				{
					DWORD l;
					m_fileLog.Seek(-256 * 1024, CFile::end);
					l = m_fileLog.Read(p, 256 * 1024L);
					m_fileLog.Close();

					if(m_fileLog.Open(m_strLogFilename, CFile::modeCreate | CFile::modeReadWrite))
					{
						m_fileLog.Write(p, l);
						m_fileLog.Flush();
					}
				}
				catch (...) 
				{
					;
				}
				GlobalUnlock(h);
				GlobalFree(h);
			}
			
		}
	}
	return TRUE;
}

BOOL CPrepareDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CloseLogFile();
	return CDialog::DestroyWindow();
}

void CPrepareDlg::OnBtnError() 
{
	// TODO: Add your control notification handler code here
	
}
