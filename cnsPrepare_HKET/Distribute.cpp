// Distribute.cpp: implementation of the CDistribute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Prepare.h"
#include "Distribute.h"
#include "malloc.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL MakeDirectory( LPCTSTR lpszDir )
{
	struct _tfinddata_t findt ;
	TCHAR   szPath[ 255 ] , szDir[ 255 ] ;
	int    i , nLastDir = 0 ;

	_tcscpy( szDir , lpszDir ) ;

	if( (_tfindfirst( (LPCTSTR)szDir , &findt ) != -1)
		&& ( findt.attrib & _A_SUBDIR ) )
	{
		return TRUE;
	}

	if( CreateDirectory( szDir,NULL ) != 0 )
		return TRUE;

	//for( i = (int)_tcslen( szDir ) ; i > 3 ; i -=sizeof(TCHAR) )
	for(i = (int)_tcslen(szDir) - 1; i > 3 ; i--)
	{
		if( szDir[ i ] == _T('\\'))
		{
			if( nLastDir == 0 )
				nLastDir = i ;
			_tcscpy( szPath , szDir ) ;
			szPath[i] = _T('\0') ;
			if( CreateDirectory( szPath,NULL ) != 0 )
			{
				if( nLastDir == i )
					break ;
				else
					i = nLastDir + 1 ;
			}
		}
	}

	if((CreateDirectory( szDir,NULL ) != 0 ) || (GetLastError() == ERROR_ALREADY_EXISTS))
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistribute::CDistribute()
{
	m_hWnd = NULL;
	m_cFilesSep = _T('|');

	m_strDocTypeKey = _T("doctype=");
	m_strFilesKey =  _T("files=");
//	m_strTitleKey = _T("title=");
	m_strTitleKey = _T("<標題>=");
	m_strKeywordKey = _T("keywords=");
	m_strColumnKey = _T("columnname=");
	m_strDocidKey = _T("docid=");
//	m_strContentKey = _T("content=");
	m_strContentKey = _T("<內容>=");


}

CDistribute::~CDistribute()
{

}

BOOL CDistribute::FindKey(const CString& strKey, CString &strValue)
{
	CString str;	

	return m_mapDistInfo.Lookup(strKey, str);
}

BOOL CDistribute::AddKey(const CString& strKey, const CString &strValue)
{
	BOOL bRet = FALSE;
	CString str;
	if (!FindKey(strKey, str))
	{
		m_mapDistInfo[strKey]= strValue;
		FindKey(strKey, (CString)strValue);
		bRet = TRUE;
	}
	return bRet;
}

BOOL CDistribute::RemoveKey(const CString& strKey)
{
	return m_mapDistInfo.RemoveKey(strKey);
}

BOOL CDistribute::RemoveExpiredKey()
{
	BOOL bRet = FALSE;
	CString strValidDay;

	bool bFirst = true;

	for (int i=0; i<= m_nDaysBefore; i++)
	{	
		COleDateTimeSpan dts(i,0,0,0);
		COleDateTime dtCheckDay = COleDateTime::GetCurrentTime()-dts;

		if (bFirst)
		{
			strValidDay = dtCheckDay.Format(_T("%Y%m%d"));
			bFirst = false;
		}
		else
		{
			strValidDay = strValidDay + dtCheckDay.Format(_T(" %Y%m%d"));
		}
	}

	for (i=0; i<= m_nDaysAfter; i++)
	{	
		COleDateTimeSpan dts(i,0,0,0);
		COleDateTime dtCheckDay = COleDateTime::GetCurrentTime()+dts;

		if (bFirst)
		{
			strValidDay = dtCheckDay.Format(_T("%Y%m%d"));
			bFirst = false;
		}
		else
		{
			strValidDay = strValidDay + dtCheckDay.Format(_T(" %Y%m%d"));
		}
	}

	POSITION pos = m_mapDistInfo.GetStartPosition();
	while (pos != NULL)
	{
		CString str, strTime;
		CString pMyObject;
		m_mapDistInfo.GetNextAssoc(pos, str, pMyObject);

		strTime = str.Right(8);


		if (strValidDay.Find(strTime, 0) < 0)
		{
			m_mapDistInfo.RemoveKey(str);
			bRet = TRUE;
		}		
	}
	
	return bRet;
}

BOOL CDistribute::UpdateDistributeInfo(BOOL bLoad)
{
	BOOL bRet = TRUE;
	CFile mFile; 
	UINT nFlags;
	if(bLoad)
		nFlags = CFile::modeReadWrite|CFile::shareDenyNone;
	else
		nFlags = CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone;
	
	try{
		CFileException fe;
		if(mFile.Open(m_strMapFilename,nFlags,&fe)==0) 
		{
			return TRUE; 
		}
	
		CArchive ar(&mFile,bLoad?(CArchive::load):CArchive::store); 
		
		m_mapDistInfo.Serialize(ar);
		ar.Close(); 
		mFile.Close(); 
	}
	catch (...) 
	{
		bRet = FALSE;
	}

	return bRet;	
}

COleDateTime CDistribute::GetFileTime(LPCTSTR lpFileName)
{
	int nYear=0, nMonth=0, nDay=0;
	CString strDate;

	COleDateTime dt;

	CFileStatus fs;
	if(CFile::GetStatus(lpFileName,fs))
	{
		dt = COleDateTime(fs.m_mtime.GetYear(),fs.m_mtime.GetMonth(),fs.m_mtime.GetDay(),0,0,0);			
	}

	return dt;
}

void CDistribute::GetFileKey(LPCTSTR lpFileName, CString &strKey)
{	
	strKey = lpFileName;
	strKey.TrimRight();
	
	CFileStatus status;
	
	COleDateTime dtDistDate;
	dtDistDate = GetFileTime(lpFileName);
	strKey += dtDistDate.Format(_T("%Y%m%d"));
	
}

BOOL CDistribute::IsSourceFile(LPCTSTR lpFilename)
{
	CString strFile = lpFilename;
	CString strext = strFile.Mid(strFile.ReverseFind('.') + 1);
	strext.MakeLower();

	int nPos = m_strSrcFileTypes.Find(strext);
	
	return nPos>=0;
}


BOOL CDistribute::IsPhotoFile(LPCTSTR lpFilename)
{
	CString strFile = lpFilename;
	CString strext = strFile.Mid(strFile.ReverseFind('.') + 1);
	strext.MakeLower();

	int nPos = m_strPhotoFileTypes.Find(strext);
	
	return nPos>=0;
}

BOOL CDistribute::IsDistributed(LPCTSTR lpFileName)
{
	CString strKey;
	GetFileKey(lpFileName, strKey);
	CString strValue;
	if(FindKey(strKey, strValue))
		return TRUE;
	return FALSE;
}


void CDistribute::TransferFile(LPCTSTR lpFileName, LPCTSTR lpToTeleFolder, LPCTSTR lpToPhotoFolder)
{
	CString strPrompt;
	if (IsDistributed(lpFileName))
	{
		strPrompt.Format(_T("File %s has been Distributed "), lpFileName);
		g_csPrompt.Lock();
		_tcscpy(g_sPrompt, strPrompt);
		g_csPrompt.Unlock();
		::PostMessage(m_hWnd, WM_THREADPROMPT, 0, 0);
	}
	else
	{		
		CString strAttachments;
		int nError = 0;
		if (TransferTele(lpFileName, lpToTeleFolder, strAttachments))
		{
			CString strKey;
			GetFileKey(lpFileName, strKey);
			AddKey(strKey, _T(""));

			strPrompt.Format(_T("Transfer %s to %s OK."), lpFileName, lpToTeleFolder);
/*			
			if (m_bGraphic)
			{				
				int nCount = 0;
				
				for (int i=0; i<strAttachments.GetLength(); i++)
				{
					if (strAttachments[i] == m_cFilesSep)
						nCount++;
				}
				
				if (nCount>0)
				{
					char szDrive[_MAX_DRIVE]={0}, szDir[_MAX_DIR]={0};
					char szFName[_MAX_FNAME]={0}, szExt[_MAX_EXT]={0};
					char pszfilename[_MAX_PATH]={0};
					_splitpath(lpFileName, szDrive, szDir, szFName, szExt);
					strcat(szDir, szFName);
					
					for (int j=0; j<nCount; j++)
					{					
						CString strPic;
						AfxExtractSubString(strPic, strAttachments, j, m_cFilesSep);
						
						if (IsPhotoFile(strPic))
						{						
							char szDrive2[_MAX_DRIVE]={0}, szDir2[_MAX_DIR]={0};
							_splitpath(strPic, szDrive2, szDir2, szFName, szExt);
							_makepath(pszfilename, szDrive, szDir, szFName, szExt);
							
							CString strSrcPic = pszfilename;
							
							TransferPic(strSrcPic, lpToPhotoFolder, lpFileName);
						}
						
					}
					
				}
			}
*/
		}
		else
		{
			strPrompt.Format(_T("Transfer %s to %s Error."), lpFileName, lpToTeleFolder);
			nError = 1;
		}
		
		g_csPrompt.Lock();
		_tcscpy(g_sPrompt, strPrompt);
		g_csPrompt.Unlock();
		::PostMessage(m_hWnd, WM_THREADPROMPT, 0, nError);
	}	
}

void CDistribute::Replace_Content(CString& strContent)
{
	strContent.Replace("&nbsp;","");
	strContent.Replace("\r","\r\n");
	strContent.Replace("\n","\r\n");
	strContent.Replace("&lt;","<");
	strContent.Replace("&gt;",">");
	strContent.Replace("\r\n     ","\r\n");
	strContent.Replace("\r\n    ","\r\n");
	strContent.Replace("\r\n   ","\r\n");
	strContent.Replace("\r\n  ","\r\n");
	strContent.Replace("\r\n ","\r\n");
	strContent.Replace("\r\n\r\n\r\n\r\n\r\n","\r\n\r\n\r\n\r\n");
	strContent.Replace("\r\n\r\n\r\n\r\n","\r\n\r\n\r\n");
	strContent.Replace("\r\n\r\n\r\n","\r\n\r\n");
	strContent.Replace("\r\n\r\n","");

	int nInx = strContent.Find("　　");
	if (nInx == 0)
	{    strContent.Delete(nInx, 4);
	}
}

BOOL CDistribute::TransferTele(LPCTSTR lpFilename, LPCTSTR lpToFolder, CString & strAttachments)
{
	BOOL bRet = FALSE;
	char* pBuf = Unicode2MBCS(lpFilename);

	CString strDocid, strTitle, strKeywords, strColumn, strContent;

	if (pBuf != NULL)
	{
		int nLength = strlen(pBuf);
		
		char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
		char szFName[_MAX_FNAME], szExt[_MAX_EXT], szFName2[_MAX_FNAME];
		char pszfilename[_MAX_PATH];
		
		_splitpath(lpFilename, szDrive, szDir, szFName, szExt);	
		CString strTo = lpToFolder;
		strTo += _T("\\")+ m_strTeleFileType;
		_splitpath(strTo, szDrive, szDir, szFName2, szExt);

		_makepath(pszfilename, szDrive, szDir, szFName, szExt);
		
		CFile file;
		CString strMBCSFile = pszfilename;
		
		if (!file.Open( strMBCSFile, CFile::modeCreate | CFile::modeWrite))
		{
			return FALSE;
		}

		CString strContent = pBuf;

		Replace_Content(strContent);

		file.Write(strContent, strContent.GetLength());

		file.Close();

		ModifyFileTime(lpFilename, strMBCSFile);

		bRet = TRUE;
		
		delete []pBuf;
		
	}
	else
	{
		CString strPrompt;
		strPrompt.Format(_T("'%s' invalid."), lpFilename);
		g_csPrompt.Lock();
		_tcscpy(g_sPrompt, strPrompt);
		g_csPrompt.Unlock();
		::PostMessage(m_hWnd, WM_THREADPROMPT, 0, 1);
	}
	


/*	if (pBuf != NULL && ParseCnsBuffer(pBuf, strAttachments, strContent, strKeywords, strColumn,strTitle,strDocid))
	{
		int nLength = strlen(pBuf);
		
		char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
		char szFName[_MAX_FNAME], szExt[_MAX_EXT], szFName2[_MAX_FNAME];
		char pszfilename[_MAX_PATH];
		
		_splitpath(lpFilename, szDrive, szDir, szFName, szExt);	
		CString strTo = lpToFolder;
		strTo += _T("\\")+ m_strTeleFileType;
		_splitpath(strTo, szDrive, szDir, szFName2, szExt);

		_makepath(pszfilename, szDrive, szDir, szFName, szExt);
		
		CFile file;
		CString strMBCSFile = pszfilename;
		
		if (!file.Open( strMBCSFile, CFile::modeCreate | CFile::modeWrite))
		{
			return FALSE;
		}
		
		if (m_bSlug)
		{
			file.Write(pBuf, nLength);
		}
		else
		{
			if (strDocid.GetLength() > 0)
				file.Write((LPCTSTR)strDocid, strDocid.GetLength());
			file.Write(_T("\r\n"), 2*sizeof(TCHAR));

			if (strTitle.GetLength() > 0)
				file.Write((LPCTSTR)strTitle, strTitle.GetLength());
			file.Write(_T("\r\n"), 2*sizeof(TCHAR));

			if (strColumn.GetLength() > 0)
				file.Write((LPCTSTR)strColumn, strColumn.GetLength());
			file.Write(_T("\r\n"), 2*sizeof(TCHAR));

			if (strKeywords.GetLength() > 0)
				file.Write((LPCTSTR)strKeywords, strKeywords.GetLength());
			file.Write(_T("\r\n"), 2*sizeof(TCHAR));

			if (strContent.GetLength() > 0)
				file.Write((LPCTSTR)strContent, strContent.GetLength());

		}

		file.Close();

		ModifyFileTime(lpFilename, strMBCSFile);

		bRet = TRUE;
		
		delete []pBuf;
		
	}
	else
	{
		CString strPrompt;
		strPrompt.Format(_T("'%s' invalid."), lpFilename);
		g_csPrompt.Lock();
		_tcscpy(g_sPrompt, strPrompt);
		g_csPrompt.Unlock();
		::PostMessage(m_hWnd, WM_THREADPROMPT, 0, 1);
	}
*/
	return TRUE;
}

BOOL CDistribute::ParseCnsBuffer(char *pBuf, CString &strAttachments, CString &strContent, CString &strKeywords, CString &strColumn, CString &strTitle, CString &strDocId)
{
	BOOL bRet = FALSE;
	
	char szValue[2048] = {0};
	CString strFormat;
	char *pDocid = strstr(pBuf, m_strDocidKey);
//	if (GetKeyValue(pBuf, m_strDocidKey, strDocId) &&
//		GetKeyValue(pBuf, m_strTitleKey, strTitle) &&
//		GetKeyValue(pBuf, m_strColumnKey, strColumn))
	if (GetKeyValue(pBuf, m_strTitleKey, strTitle))
	{
		GetKeyValue(pBuf, m_strContentKey, strContent);
//		GetKeyValue(pBuf, m_strKeywordKey, strKeywords);
		
//		int nType = 0;
//		if (GetKeyValue(pBuf, m_strDocTypeKey, nType))
//		{
//			if (nType >1)
//				GetKeyValue(pBuf, m_strFilesKey, strAttachments);
//		}

//		char *pContent = strstr(pBuf, m_strContentKey);
//		int nLen = strlen(pBuf);
//		int nPos = (pContent - pBuf);
				
//		if (nLen-m_strContentKey.GetLength()-nPos> 0)
//		{
//			pContent += m_strContentKey.GetLength();
//			strContent = strTitle;
		bRet = TRUE;
		strContent.TrimLeft();
		if (strContent!="")
			strContent = strTitle + "\r\n" + strContent; 
		else
			strContent = strTitle;
//		}
	}
	else
	{
		strContent = pBuf;
		bRet = TRUE;
	}	
	return bRet;
}

void CDistribute::TransferPic(LPCTSTR lpPicFilename, LPCTSTR lpToFolder, LPCTSTR lpDefCapfile)
{
	if (IsDistributed(lpPicFilename)) return;

	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	char szFName[_MAX_FNAME], szExt[_MAX_EXT], szFName2[_MAX_FNAME], szExt2[_MAX_EXT];
	char pszfilename[_MAX_PATH], pszcaption[_MAX_PATH];;
	
	_splitpath(lpPicFilename, szDrive, szDir, szFName, szExt);
	_makepath(pszcaption, szDrive, szDir, szFName, _T("cns"));	
	
	CString strTo = lpToFolder;
	strTo += _T("\\1.jpg");
	_splitpath(strTo, szDrive, szDir, szFName2, szExt2);
	_makepath(pszfilename, szDrive, szDir, szFName, szExt);	

	CString strPrompt;
	int nError = 0;
	if (::CopyFile(lpPicFilename, pszfilename, FALSE))
	{
		ModifyFileTime(lpPicFilename, pszfilename);
		strPrompt.Format(_T("Transfer %s to %s OK."), lpPicFilename, lpToFolder);
	}
	else
	{
		strPrompt.Format(_T("Transfer %s to %s Failed."), lpPicFilename, lpToFolder);
		nError = 1;
	}
	
	g_csPrompt.Lock();
	_tcscpy(g_sPrompt, strPrompt);
	g_csPrompt.Unlock();
	::PostMessage(m_hWnd, WM_THREADPROMPT, 0, nError);

	_makepath(pszfilename, szDrive, szDir, szFName, _T("txt"));

//	char *pBuf = Unicode2MBCS(pszcaption);
//
//	if (pBuf == NULL)
//	{
char *pBuf = Unicode2MBCS(lpDefCapfile);
//	}

	CString strDocid, strTitle, strKeywords, strColumn, strContent, strAttachments;

	if (pBuf != NULL && ParseCnsBuffer(pBuf, strAttachments, strContent, strKeywords, strColumn,strTitle,strDocid))
//	if (pBuf != NULL)
	{
		int nLength = strlen(pBuf);
		CFile file;
		CString strMBCSFile = pszfilename;
		
		if (!file.Open( strMBCSFile, CFile::modeCreate | CFile::modeWrite))
		{
			return;
		}
		
//		if (strTitle.GetLength() > 0)
//			file.Write((LPCTSTR)strTitle, strTitle.GetLength());
//		file.Write(_T("\r\n"), 2*sizeof(TCHAR));
		
//		if (strContent.GetLength() > 0)
//			file.Write((LPCTSTR)strContent, strContent.GetLength());
//		CString strContent = pBuf;

		Replace_Content(strContent);

//strContent.Replace("\r\n","\r");
//		file.Write(pBuf, nLength);
		file.Write(strContent, strContent.GetLength());

		file.Close();

		ModifyFileTime(pszcaption, strMBCSFile);
		delete []pBuf;

			CString strKey;
			GetFileKey(lpPicFilename, strKey);
			AddKey(strKey, _T(""));

	}
	

}

// 分發不同日期但在同一文件夾下的文件
int CDistribute::DistributeFilesByDir(LPCTSTR lpszDirectory, const COleDateTime & dtStart, const COleDateTime & dtEnd)
{
	CString strPrompt = _T("DistributeFilesByDir not implemented! ");
	g_csPrompt.Lock();
	_tcscpy(g_sPrompt, strPrompt);
	g_csPrompt.Unlock();
	::PostMessage(m_hWnd, WM_THREADPROMPT, 0, 1);
	return 0;
}

// 分發按日期分放在不同文件夾下的文件
int CDistribute::DistributeFilesByDate(const COleDateTime & dtCheck)
{
	int nBreak = 0;
	CString strSrcPath = BuildPath(dtCheck
		, m_strSourcePath
		, m_bSrcSub? m_strSrcSubPath : _T("")
		, m_nSrcDTF);

	CString strTelePath = BuildPath(dtCheck
		, m_strTelePath
		, m_bTeleSub? m_strTeleSubPath : _T("")
		, m_nTeleDTF);

	CString strPhotoPath = BuildPath(dtCheck
		, m_strPhotoPath
		, m_bPhotoSub? m_strPhotoSubPath : _T("")
		, m_nPhotoDTF);

	try
	{
		BOOL bPhoto = FALSE;
		if (m_bGraphic)
		{
			bPhoto = MakeDirectory(strPhotoPath);
		}
		CString strPrompt;
		strPrompt.Format(_T("Checking %s ..."), strSrcPath);
		g_csPrompt.Lock();
		_tcscpy(g_sPrompt, strPrompt);
		g_csPrompt.Unlock();
		::PostMessage(m_hWnd, WM_THREADPROMPT, 0, 0);

		if (MakeDirectory(strTelePath))
		{		
			int nFindFlag;
			CString strFind = strSrcPath;
			strFind += _T("\\*.*");
//			strFind += _T("\\*\\.*");
			
			struct _tfinddata_t findt;
			nFindFlag = _tfindfirst( strFind, &findt);
			
			if( nFindFlag != -1 )
			{
				do
				{
					BOOL bFind=FALSE;
					if(findt.name[0] == _T('.'))
						continue;
					if((findt.attrib & _A_SUBDIR) != 0)
					{
						struct _tfinddata_t findt_sub;
//						AfxMessageBox(findt.name);
						CString strSub;
						strSub.Format("%s\\%s",strSrcPath,findt.name);
						CString strSubSource = strSub;
						strSub += _T("\\*.*");
						int nFindFlag_sub = _tfindfirst( strSub, &findt_sub);

						if( nFindFlag_sub != -1 )
						{
							do
							{
								BOOL bFind_sub=FALSE;
								if(findt_sub.name[0] == _T('.'))
									continue;
								if((findt_sub.attrib & _A_SUBDIR) != 0)
								{
									continue;
								}

								if (IsSourceFile(findt_sub.name))
								{
									CString strFile = findt_sub.name;
									strFile = strSubSource + _T("\\") + strFile;
									TransferFile(strFile, strTelePath, strPhotoPath);						
								}

								if (m_bGraphic)
								{				
									if (IsPhotoFile(findt_sub.name))
									{						
										CString strFile = findt_sub.name;
										strFile = strSubSource + _T("\\") + strFile;
										
										CString strPicTxtFile;
										int ixd = strFile.ReverseFind('_');			
										int ixd_dot = strFile.ReverseFind('.');			
//										if (ixd>=0)
//											strPicTxtFile  = strFile.Mid(0,ixd) + ".cns";
//										else
//											strPicTxtFile =  strFile.Mid(0,ixd_dot) + ".cns";

										if (ixd_dot>=0)
											strPicTxtFile =  strFile.Mid(0,ixd_dot) + ".txt";
								
										if (!IsFileExists(strPicTxtFile))
										{
											if (ixd>=0)
												strPicTxtFile  = strFile.Mid(0,ixd) + _T(".cns");
											else
												strPicTxtFile.Replace(_T(".txt"),_T(".cns"));
										}

										TransferPic(strFile, strPhotoPath, strPicTxtFile);
									}
											
								}


								if (WaitForSingleObject(g_hKillEvent, 0) == WAIT_OBJECT_0)
								{
									nBreak = 1;
									break;
								}

							} while(_tfindnext(nFindFlag_sub,&findt_sub) == 0);
						}
					}
					else
					{
							if (IsSourceFile(findt.name))
							{
								CString strFile = findt.name;
								strFile = strSrcPath + _T("\\") + strFile;
								TransferFile(strFile, strTelePath, strPhotoPath);						
							}

							if (m_bGraphic)
							{				
								if (IsPhotoFile(findt.name))
								{						
									CString strFile = findt.name;
									strFile = strSrcPath + _T("\\") + strFile;
										
									CString strPicTxtFile;
									int ixd = strFile.ReverseFind('_');			
									int ixd_dot = strFile.ReverseFind('.');			
//										if (ixd>=0)
//											strPicTxtFile  = strFile.Mid(0,ixd) + ".cns";
//										else
//											strPicTxtFile =  strFile.Mid(0,ixd_dot) + ".cns";

									if (ixd_dot>=0)
										strPicTxtFile =  strFile.Mid(0,ixd_dot) + _T(".txt");
								
									if (!IsFileExists(strPicTxtFile))
									{
										if (ixd>=0)
											strPicTxtFile  = strFile.Mid(0,ixd) + _T(".cns");
										else
											strPicTxtFile.Replace(_T(".txt"),_T(".cns"));
									}

									TransferPic(strFile, strPhotoPath, strPicTxtFile);
								}
											
							}


							if (WaitForSingleObject(g_hKillEvent, 0) == WAIT_OBJECT_0)
							{
								nBreak = 1;
								break;
							}


					}


					//	continue;
					

				} while(_tfindnext(nFindFlag,&findt) == 0);
			}

		}
	}
	catch (...) 
	{
	}
	

	return nBreak;
}

BOOL CDistribute::Validate(CString & strErrorMessage)
{
	if (m_strSourcePath.IsEmpty())
	{
		strErrorMessage = _T("Invalid Source Folder.");
		return FALSE;
	}

	if (m_strTelePath.IsEmpty())
	{
		strErrorMessage = _T("Please set the Tele Folder.");
		return FALSE;
	}

	
	if (m_bGraphic && m_strPhotoPath.IsEmpty())
	{
		strErrorMessage = _T("Please set the Photo Folder.");
		return FALSE;
	}
	

	return TRUE;
}

int CDistribute::DistributeFiles()
{
	int nBreak = 0;
		
	CString strErrorMessage;
	if (Validate(strErrorMessage))
	{
		UpdateDistributeInfo(TRUE);
		if (m_nSrcDTF == 0)
		{
			CString strPath = m_strSourcePath;
			if (m_bSrcSub && !m_strSrcSubPath.IsEmpty())
			{
				strPath += _T("\\");
				strPath += m_strSrcSubPath;
			}

			COleDateTimeSpan dtsBefore(m_nDaysBefore,0,0,0);
			COleDateTime dtStart = COleDateTime::GetCurrentTime() - dtsBefore;
			dtStart.m_dt = (DATE)((long)(dtStart.m_dt));

			COleDateTimeSpan dtsAfetr(m_nDaysAfter+1,0,0,0);
			COleDateTime dtEnd = COleDateTime::GetCurrentTime() + dtsAfetr;
			dtEnd.m_dt = (DATE)((long)(dtEnd.m_dt));
				
			nBreak = DistributeFilesByDir(strPath, dtStart, dtEnd);
		}
		else
		{			
			for (int nDays = (-1) * m_nDaysBefore; nDays<=m_nDaysAfter; nDays++)
			{
				COleDateTimeSpan dts(nDays,0,0,0);
				COleDateTime dtCheck = COleDateTime::GetCurrentTime() + dts;
				dtCheck.m_dt = (DATE)((long)(dtCheck.m_dt));
				nBreak = DistributeFilesByDate(dtCheck);

				if (nBreak != 0)
					break;
			}
		}
		RemoveExpiredKey();
		UpdateDistributeInfo(FALSE);
	}
	
	return nBreak;
}


CString CDistribute::BuildPath(const COleDateTime &dtWork, const CString &strMajorFolder, const CString &strSubFolder, int nDTStyle)
{
	CString strPath = strMajorFolder;
	
	CString strTemp = _T("\\");
	
	switch (nDTStyle)
	{
	case 0:
		strTemp = _T("");
		break;
	case 1:
		strTemp += dtWork.Format(_T("%m%d%Y"));
		break;
	case 2:
		strTemp += dtWork.Format(_T("%m%d%y"));
		break;
	case 3:
		strTemp += dtWork.Format(_T("%d"));
		break;
	case 4:
		strTemp += dtWork.Format(_T("%Y%m%d"));
		break;
	case 5:
		strTemp += dtWork.Format(_T("%y%m%d"));
		break;
	case 6:
		strTemp += dtWork.Format(_T("%d%m%Y"));
		break;
	case 7:
		strTemp += dtWork.Format(_T("%d%m%y"));
		break;
	case 8:
		strTemp += dtWork.Format(_T("%m%d"));
		break;
	case 9:
		strTemp += dtWork.Format(_T("%m$%d"));
		break;
	default:
		break;
	}
	
	strPath = strPath + strTemp;
	
	if (!strSubFolder.IsEmpty())
	{
		strPath += _T("\\");
		strPath += strSubFolder;
	}
	
	return strPath;
}

BOOL CDistribute::ModifyFileTime(const CString &strSrcFile, const CString &strDestFile)
{
	HANDLE hFile = ::CreateFile(strSrcFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME ftCreate, ftAccess, ftWrite;
	BOOL bRet = FALSE;	
	if((hFile != INVALID_HANDLE_VALUE) && ::GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		::CloseHandle(hFile);
		hFile = ::CreateFile(strDestFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);		
		if((hFile != INVALID_HANDLE_VALUE) && ::SetFileTime(hFile, &ftCreate, NULL, &ftWrite))
		{
			bRet = TRUE;
			::CloseHandle(hFile);
		}
		else if(hFile != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile);	
		}
	}	
	else if(hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);	
	}
	return bRet;
}

// if return is not NULL,please remember delete the return char*
char* CDistribute::Unicode2MBCS(LPCTSTR lpszFile)
{
	CFile file;
	
	if (!file.Open(lpszFile, CFile::modeRead | CFile::shareDenyNone))
	{
		return NULL;  // failed to open file
	}
	
	if (file.GetLength() < 1)
	{
		return NULL;  // empty file;
	}
	
	//  read file
	long lSize = file.GetLength();
	
	wchar_t *pbuf = NULL;
	pbuf = new wchar_t[lSize+2];
	memset(pbuf,0,lSize+2);
	
	lSize = file.Read(pbuf, lSize);
	pbuf[lSize] = 0;
	pbuf[lSize+1] = 0;

	for (int i=0; i < lSize; i++)
	{
		if (pbuf[i]==0x9898)
			pbuf[i]=0x984c;

		if (pbuf[i]==0x6807)
			pbuf[i]=0x6a19;

		if (pbuf[i]==0x5185)
			pbuf[i]=0x5167;

		if (pbuf[i]==0x00a0)
			pbuf[i]=0x0020;
	
	}	

	// if failed to read file or it's not Unicode file, return
	if ( *pbuf != ((0xff) |(0xfe << 8)))
	{
		if (pbuf != NULL)
		{
			delete []pbuf;
		}
		
		lSize = file.GetLength();
		char *pAnsiBuff = new char[lSize+2];
		memset(pAnsiBuff,0,lSize+2);
		lSize = file.Read(pAnsiBuff, lSize);
		pAnsiBuff[lSize] = 0;
		pAnsiBuff[lSize+1] = 0;

		return pAnsiBuff;
	}
	
	wchar_t *pUnicode = pbuf+1;
	int wLen = wcslen(pUnicode);
	long nlenth = WideCharToMultiByte(CP_ACP,NULL,pUnicode, wLen,NULL,0,NULL,FALSE);
	
	// if (lSize < nlenth), the nlenth is error.
	if (lSize < nlenth)
	{
		if (pbuf != NULL)
		{
			delete []pbuf;
		}
		
		return NULL;
	}
	
    // the new Ansi char
	char *pAnsiCh = NULL;
	pAnsiCh = new char[nlenth+1];
	memset(pAnsiCh,0,nlenth+1);
	pAnsiCh[nlenth] = 0;
	
	WideCharToMultiByte(CP_ACP,NULL,pUnicode,-1,pAnsiCh,nlenth,NULL,FALSE);
	
	// release memory
	if (pbuf != NULL)
	{
		delete []pbuf;
	}
	
	return pAnsiCh;
} 

BOOL CDistribute::GetKeyValue(char *pBuff, const CString &strKey, CString &strValue)
{
	BOOL bRet = FALSE;
	char szValue[2048] = {0};
	
	CString strFormat;
	char *pValue = strstr(pBuff, strKey);
	
	if (pValue)
	{
		int i =0;
		char szLine[4096] = {0};
		while (i<4094)
		{
			szLine[i++] = *pValue++;
			if (*pValue == _T('\0') || *pValue == _T('\r'))
			{
				break;
			}
		}

//*************************************************
// Edit Stephen 2013-09-18
// Get the whole line after ????=  
//*************************************************
/*		strFormat = strKey + _T("%s");
		if (sscanf(szLine, strFormat, szValue) == 1)
		{
			strValue = szValue;
			bRet = TRUE;
		}
*/
		strValue = szLine;
		if (strValue.Replace(strKey,"")) 
			bRet = TRUE;
//*************************************************
// End Edit Stephen
//*************************************************
	
	}
	return bRet;
}

BOOL CDistribute::IsFileExists(LPCTSTR lpFileName)
{
	CFile file;
	if(file.Open(lpFileName,CFile::modeRead))
	{
		file.Close();
		return TRUE;
	}
	else
		return FALSE;
}



BOOL CDistribute::GetKeyValue(char *pBuff, const CString &strKey, int &nValue)
{
	BOOL bRet = FALSE;

	CString strFormat;
	char *pValue = strstr(pBuff, strKey);
	
	if (pValue)
	{
		int i =0;
		char szLine[4096] = {0};
		while (i<4094)
		{
			szLine[i++] = *pValue++;
			if (*pValue == _T('\0') || *pValue == _T('\r'))
			{
				break;
			}
		}

		strFormat = strKey + _T("%d");
		bRet = (sscanf(szLine, strFormat, &nValue) == 1);
	}

	return bRet;
}