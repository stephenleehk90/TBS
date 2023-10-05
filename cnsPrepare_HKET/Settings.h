#if !defined(AFX_SETTINGS_H__8505F881_069A_4DEC_8E1C_D443BB2D4AA1__INCLUDED_)
#define AFX_SETTINGS_H__8505F881_069A_4DEC_8E1C_D443BB2D4AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Settings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettings dialog

class CSettingsDlg : public CDialog
{
	void RefreshSrcLike();
	void RefreshTeleLike();
	void RefreshPhotoLike();
// Construction
public:
	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettings)
	enum { IDD = IDD_SETTINGS };
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
	BOOL	m_bPhotoSub;
	int		m_nPhotoDTF;
	CString	m_strPhotoPath;
	CString	m_strPhotoSubPath;
	CString	m_strPhotoFileTypes;
	int		m_nDaysAfter;
	int		m_nDaysBefore;	
	CString	m_strSrcFolderLike;
	CString	m_strTeleFolderLike;
	CString	m_strPhotoFolderLike;
	int		m_nInterval;
	BOOL	m_bSlug;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettings)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSource();
	afx_msg void OnBtnTele();
	afx_msg void OnBtnPhoto();
	afx_msg void OnCheckGraphic();
	afx_msg void OnCheckSubSrc();
	afx_msg void OnCheckSubTxt();
	afx_msg void OnCheckSubPhoto();
	afx_msg void OnSrcDtfChange();
	afx_msg void OnTeleDtfChange();
	afx_msg void OnPhotoDtfChange();
	afx_msg void OnChangeEditSource();
	afx_msg void OnChangeEditSourceSub();
	afx_msg void OnChangeEditSrcTypes();
	afx_msg void OnChangeEditPhoto();
	afx_msg void OnChangeEditPhotoSub();
	afx_msg void OnChangeEditPhotoTypes();
	afx_msg void OnChangeEditTele();
	afx_msg void OnChangeEditTeleSub();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGS_H__8505F881_069A_4DEC_8E1C_D443BB2D4AA1__INCLUDED_)
