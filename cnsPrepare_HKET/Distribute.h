// Distribute.h: interface for the CDistribute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTE_H__3D5D30C0_F15D_4028_9BFB_F3215D3ED505__INCLUDED_)
#define AFX_DISTRIBUTE_H__3D5D30C0_F15D_4028_9BFB_F3215D3ED505__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDistribute  
{
	CMapStringToString m_mapDistInfo;	

	BOOL FindKey(const CString& strKey,CString &strValue);
	BOOL AddKey(const CString& strKey,const CString &strValue);
	BOOL RemoveKey(const CString& strKey);

	BOOL RemoveExpiredKey();
	BOOL UpdateDistributeInfo(BOOL bLoad);

	COleDateTime GetFileTime(LPCTSTR lpFileName);
	void GetFileKey(LPCTSTR lpFileName, CString &strKey);
	BOOL IsDistributed(LPCTSTR lpFileName);

	int DistributeFilesByDir(LPCTSTR lpszDirectory, const COleDateTime & dtStart, const COleDateTime & dtEnd);
	int DistributeFilesByDate(const COleDateTime & dtCheck);

	void TransferFile(LPCTSTR lpFileName, LPCTSTR lpToTeleFolder, LPCTSTR lpToPhotoFolder);
	BOOL TransferTele(LPCTSTR lpFilename, LPCTSTR lpToFolder, CString & strAttachments);
	void TransferPic(LPCTSTR lpPicFilename, LPCTSTR lpToFolder, LPCTSTR lpDefCapfile); 

	BOOL IsSourceFile(LPCTSTR lpFilename);
	BOOL IsPhotoFile(LPCTSTR lpFilename);
	BOOL ParseCnsBuffer(char *pBuf, CString &strAttachments, CString &strContent, CString &strKeywords, CString &strColumn, CString &strTitle, CString &strDocId);

	BOOL GetKeyValue(char *pBuff, const CString &strKey, CString &strValue);
	BOOL GetKeyValue(char *pBuff, const CString &strKey, int &nValue);
	void Replace_Content(CString& strContent);

public:
	HWND    m_hWnd;        //
	CString m_strMapFilename;
	

	CString	m_strSourcePath;
	CString	m_strSrcSubPath;
	CString	m_strSrcFileTypes;
	int		m_nSrcDTF;
	BOOL	m_bSrcSub;

	CString	m_strTelePath;
	CString	m_strTeleSubPath;
	BOOL	m_bTeleSub;
	int		m_nTeleDTF;
	CString	m_strTeleFileType;

	BOOL	m_bGraphic;
	BOOL    m_bSlug;

	BOOL	m_bPhotoSub;
	int		m_nPhotoDTF;
	CString	m_strPhotoPath;
	CString	m_strPhotoSubPath;
	CString	m_strPhotoFileTypes;

	int		m_nDaysAfter;
	int		m_nDaysBefore;
	TCHAR   m_cFilesSep;

	CString m_strDocTypeKey;
	CString m_strFilesKey;
	CString m_strTitleKey;
	CString m_strKeywordKey;
	CString m_strColumnKey;
	CString m_strDocidKey;
	CString m_strContentKey;
	
public:
	static CString BuildPath(const COleDateTime &dtWork, const CString &strMajorFolder, const CString &strSubFolder, int nDTStyle);
	static BOOL ModifyFileTime(const CString &strSrcFile, const CString &strDestFile);
	static char* Unicode2MBCS(LPCTSTR lpszFile);
	CDistribute();
	virtual ~CDistribute();
	int DistributeFiles();
	BOOL Validate(CString & strErrorMessage);
	BOOL IsFileExists(LPCTSTR lpFileName);

};

#endif // !defined(AFX_DISTRIBUTE_H__3D5D30C0_F15D_4028_9BFB_F3215D3ED505__INCLUDED_)
