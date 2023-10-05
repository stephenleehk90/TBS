///////////////////////////////////////////////////////////////////////////
// DirDialog.cpp: implementation of the CDirDialog class.
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DirDialog.h"
#include "shlobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Callback function called by SHBrowseForFolder's browse control
// after initialization and when selection changes
int __stdcall CDirDialog::BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    CDirDialog* pDirDialogObj = (CDirDialog*)lpData;
    if (uMsg == BFFM_INITIALIZED )
    {
        if( ! pDirDialogObj->m_strSelDir.IsEmpty() )
            ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCTSTR)(pDirDialogObj->m_strSelDir));
        if( ! pDirDialogObj->m_strWindowTitle.IsEmpty() )
            ::SetWindowText(hwnd, (LPCTSTR) pDirDialogObj->m_strWindowTitle);
    }
    else if( uMsg == BFFM_SELCHANGED )
    {
        LPITEMIDLIST pidl = (LPITEMIDLIST) lParam;
        TCHAR selection[MAX_PATH];
        if( ! ::SHGetPathFromIDList(pidl, selection) )
            selection[0] = _T('\0');

        CString csStatusText;
        BOOL bOk = pDirDialogObj->SelChanged((LPCSTR)selection, csStatusText);

        if( pDirDialogObj->m_bStatus )
            ::SendMessage(hwnd, BFFM_SETSTATUSTEXT , 0, (LPARAM)(LPCTSTR)csStatusText);

        ::SendMessage(hwnd, BFFM_ENABLEOK, 0, bOk);
    }
  return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirDialog::CDirDialog()
{
	m_bStatus = false;
	m_strWindowTitle = L"Directoty Setting";
	m_strTitle = L"Select Directory";
}

CDirDialog::~CDirDialog()
{
}

bool CDirDialog::DoBrowse(CWnd *pwndParent)
{

    if( ! m_strSelDir.IsEmpty() )
    {
        m_strSelDir.TrimRight();
        if( m_strSelDir.Right(1) == _T("\\") || m_strSelDir.Right(1) == _T("//") )
            m_strSelDir = m_strSelDir.Left(m_strSelDir.GetLength() - 1);
    }

    LPMALLOC pMalloc;
    if (SHGetMalloc (&pMalloc)!= NOERROR)
        return false;

    BROWSEINFO bInfo;
    LPITEMIDLIST pidl;
    ZeroMemory ( (PVOID) &bInfo,sizeof (BROWSEINFO));

    if (!m_strInitDir.IsEmpty ())
    {
        OLECHAR       olePath[MAX_PATH];
        ULONG         chEaten;
        ULONG         dwAttributes;
        HRESULT       hr;
        LPSHELLFOLDER pDesktopFolder;
        //
        // Get a pointer to the Desktop's IShellFolder interface.
        //
        if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
        {
            //
            // IShellFolder::ParseDisplayName requires the file name be in Unicode.
            //
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)m_strInitDir.GetBuffer(MAX_PATH), -1,
                                olePath, MAX_PATH);

            m_strInitDir.ReleaseBuffer (-1);
            //
            // Convert the path to an ITEMIDLIST.
            //
            hr = pDesktopFolder->ParseDisplayName(NULL,
                                                NULL,
                                                olePath,
                                                &chEaten,
                                                &pidl,
                                                &dwAttributes);
            if (FAILED(hr))
            {
                pMalloc ->Free (pidl);
                pMalloc ->Release ();
                return false;
            }
            bInfo.pidlRoot = pidl;

        }
    }
    bInfo.hwndOwner = pwndParent == NULL ? NULL : pwndParent->GetSafeHwnd();
    bInfo.pszDisplayName = m_strPath.GetBuffer (MAX_PATH);
    bInfo.lpszTitle = (m_strTitle.IsEmpty()) ? L"Open" : m_strTitle;
    bInfo.ulFlags = BIF_RETURNFSANCESTORS
                    | BIF_RETURNONLYFSDIRS
                    | (m_bStatus ? BIF_STATUSTEXT : 0);

    bInfo.lpfn = BrowseCtrlCallback;  // address of callback function
    bInfo.lParam = (LPARAM)this;      // pass address of object to callback function

    if ((pidl = ::SHBrowseForFolder(&bInfo)) == NULL)
    {
        return false;
    }
    m_strPath.ReleaseBuffer();
    m_iImageIndex = bInfo.iImage;

    if (::SHGetPathFromIDList(pidl, m_strPath.GetBuffer(MAX_PATH)) == FALSE)
    {
        pMalloc ->Free(pidl);
        pMalloc ->Release();
        return false;
    }

    m_strPath.ReleaseBuffer();

    pMalloc ->Free(pidl);
    pMalloc ->Release();

    return true;
}