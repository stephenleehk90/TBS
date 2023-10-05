// PrepareDlg.h : header file
//

#if !defined(AFX_PREPAREDLG_H__05545E9B_28DA_4921_A629_E9CD585020AE__INCLUDED_)
#define AFX_PREPAREDLG_H__05545E9B_28DA_4921_A629_E9CD585020AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Distribute.h"

/////////////////////////////////////////////////////////////////////////////
// CPrepareDlg dialog

class CPrepareDlg : public CDialog
{
	void LoadProfile();
	void SaveProfile();
	BOOL OpenLogFile();
	void CloseLogFile();
	BOOL WriteLog(LPCTSTR lpMsg, BOOL bDisplayTime);

	void Prompt(const CString & strPrompt);
	
	CWinThread *m_pThreadDistribute;

	int m_nTimer;
	int m_nInterval;

	CDistribute m_distribute;
	CFile m_fileLog;
	CString m_strLogFilename;
	
// Construction
public:
	CPrepareDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPrepareDlg)
	enum { IDD = IDD_PREPARE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrepareDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPrepareDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnRun();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSettings();
	afx_msg void OnBtnError();
	//}}AFX_MSG
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadPrompt(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREPAREDLG_H__05545E9B_28DA_4921_A629_E9CD585020AE__INCLUDED_)
