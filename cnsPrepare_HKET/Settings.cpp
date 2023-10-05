// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "Prepare.h"
#include "Settings.h"
#include "DirDialog.h"
#include "Distribute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog


CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettings)
	m_strSourcePath = _T("");
	m_strSrcSubPath = _T("");
	m_strSrcFileTypes = _T("*.cns");
	m_nSrcDTF = 0;
	m_bSrcSub = FALSE;
	m_strTelePath = _T("");
	m_strTeleSubPath = _T("");
	m_bTeleSub = FALSE;
	m_nTeleDTF = 0;
	m_strTeleFileType = _T("*.txt");
	m_bGraphic = FALSE;
	m_bPhotoSub = FALSE;
	m_nPhotoDTF = 0;
	m_strPhotoPath = _T("");
	m_strPhotoSubPath = _T("");
	m_strPhotoFileTypes = _T("*.jpg;*.jpeg;*.tif;*.tiff;*.bmp");
	m_nDaysAfter = 0;
	m_nDaysBefore = 0;
	m_strSrcFolderLike = _T("");
	m_strTeleFolderLike = _T("");
	m_strPhotoFolderLike = _T("");
	m_nInterval = 1;
	m_bSlug = FALSE;
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettings)
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strSourcePath);
	DDX_Text(pDX, IDC_EDIT_SOURCE_SUB, m_strSrcSubPath);
	DDX_Text(pDX, IDC_EDIT_SRC_TYPES, m_strSrcFileTypes);
	DDX_CBIndex(pDX, IDC_COMBO_SRC_DTF, m_nSrcDTF);
	DDX_Check(pDX, IDC_CHECK_SUB_SRC, m_bSrcSub);
	DDX_Text(pDX, IDC_EDIT_TELE, m_strTelePath);
	DDX_Text(pDX, IDC_EDIT_TELE_SUB, m_strTeleSubPath);
	DDX_Check(pDX, IDC_CHECK_SUB_TXT, m_bTeleSub);
	DDX_CBIndex(pDX, IDC_COMBO_TXT_DTF, m_nTeleDTF);
	DDX_Text(pDX, IDC_EDIT_TELE_TYPE, m_strTeleFileType);
	DDX_Check(pDX, IDC_CHECK_GRAPHIC, m_bGraphic);
	DDX_Check(pDX, IDC_CHECK_SUB_PHOTO, m_bPhotoSub);
	DDX_CBIndex(pDX, IDC_COMBO_PHOTO_DTF, m_nPhotoDTF);
	DDX_Text(pDX, IDC_EDIT_PHOTO, m_strPhotoPath);
	DDX_Text(pDX, IDC_EDIT_PHOTO_SUB, m_strPhotoSubPath);
	DDX_Text(pDX, IDC_EDIT_PHOTO_TYPES, m_strPhotoFileTypes);
	DDX_Text(pDX, IDC_EDIT_AFTER, m_nDaysAfter);
	DDV_MinMaxInt(pDX, m_nDaysAfter, 0, 7);
	DDX_Text(pDX, IDC_EDIT_BEFORE, m_nDaysBefore);
	DDV_MinMaxInt(pDX, m_nDaysBefore, 0, 30);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_S1, m_strSrcFolderLike);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_S2, m_strTeleFolderLike);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_S3, m_strPhotoFolderLike);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nInterval);
	DDV_MinMaxInt(pDX, m_nInterval, 1, 60);
	DDX_Check(pDX, IDC_CHECK_SLUG, m_bSlug);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettings)
	ON_BN_CLICKED(IDC_BTN_SOURCE, OnBtnSource)
	ON_BN_CLICKED(IDC_BTN_TELE, OnBtnTele)
	ON_BN_CLICKED(IDC_BTN_PHOTO, OnBtnPhoto)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC, OnCheckGraphic)
	ON_BN_CLICKED(IDC_CHECK_SUB_SRC, OnCheckSubSrc)
	ON_BN_CLICKED(IDC_CHECK_SUB_TXT, OnCheckSubTxt)
	ON_BN_CLICKED(IDC_CHECK_SUB_PHOTO, OnCheckSubPhoto)
	ON_CBN_SELCHANGE(IDC_COMBO_SRC_DTF,OnSrcDtfChange)
	ON_CBN_SELCHANGE(IDC_COMBO_TXT_DTF,OnTeleDtfChange)
	ON_CBN_SELCHANGE(IDC_COMBO_PHOTO_DTF,OnPhotoDtfChange)
	ON_EN_CHANGE(IDC_EDIT_SOURCE, OnChangeEditSource)
	ON_EN_CHANGE(IDC_EDIT_SOURCE_SUB, OnChangeEditSourceSub)
	ON_EN_CHANGE(IDC_EDIT_SRC_TYPES, OnChangeEditSrcTypes)
	ON_EN_CHANGE(IDC_EDIT_PHOTO, OnChangeEditPhoto)
	ON_EN_CHANGE(IDC_EDIT_PHOTO_SUB, OnChangeEditPhotoSub)
	ON_EN_CHANGE(IDC_EDIT_PHOTO_TYPES, OnChangeEditPhotoTypes)
	ON_EN_CHANGE(IDC_EDIT_TELE, OnChangeEditTele)
	ON_EN_CHANGE(IDC_EDIT_TELE_SUB, OnChangeEditTeleSub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettings message handlers

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnCheckSubSrc();
	OnCheckSubTxt();
	OnCheckGraphic();
	OnCheckSubPhoto();

	RefreshSrcLike();
	RefreshTeleLike();
	RefreshPhotoLike();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnBtnSource() 
{
	// TODO: Add your control notification handler code here
	CDirDialog dlg;

	if (dlg.DoBrowse(this))
	{
		m_strSourcePath = dlg.m_strPath;

		RefreshSrcLike();

		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnBtnTele() 
{
	// TODO: Add your control notification handler code here
	CDirDialog dlg;
	
	if (dlg.DoBrowse(this))
	{
		m_strTelePath = dlg.m_strPath;

		RefreshTeleLike();
		
		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnBtnPhoto() 
{
	// TODO: Add your control notification handler code here
	CDirDialog dlg;
	
	if (dlg.DoBrowse(this))
	{
		m_strPhotoPath = dlg.m_strPath;

		RefreshPhotoLike();
		
		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnCheckGraphic() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_PHOTO)->EnableWindow(m_bGraphic);
	GetDlgItem(IDC_COMBO_PHOTO_DTF)->EnableWindow(m_bGraphic);
	GetDlgItem(IDC_CHECK_SUB_PHOTO)->EnableWindow(m_bGraphic);
	GetDlgItem(IDC_EDIT_PHOTO_SUB)->EnableWindow(m_bGraphic?(m_bPhotoSub? TRUE:FALSE):FALSE);
	GetDlgItem(IDC_EDIT_PHOTO_TYPES)->EnableWindow(m_bGraphic);
}

void CSettingsDlg::OnCheckSubSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_SOURCE_SUB)->EnableWindow(m_bSrcSub);
	if (!m_bSrcSub)
	{
		m_strSrcSubPath.Empty();
		RefreshSrcLike();
	}

	UpdateData(FALSE);
}

void CSettingsDlg::OnCheckSubTxt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_TELE_SUB)->EnableWindow(m_bTeleSub);
	if (!m_bTeleSub)
	{
		m_strTeleSubPath.Empty();
		RefreshTeleLike();
	}
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnCheckSubPhoto() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_PHOTO_SUB)->EnableWindow(m_bPhotoSub);
	if (!m_bPhotoSub)
	{
		m_strPhotoSubPath.Empty();
		RefreshPhotoLike();
	}
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnSrcDtfChange()
{
	UpdateData(TRUE);

	RefreshSrcLike();

	UpdateData(FALSE);
}

void CSettingsDlg::OnTeleDtfChange()
{
	UpdateData(TRUE);
	
	RefreshTeleLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnPhotoDtfChange()
{
	UpdateData(TRUE);
	
	RefreshPhotoLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::RefreshSrcLike()
{	
	m_strSrcFolderLike = CDistribute::BuildPath(COleDateTime::GetCurrentTime()
		, m_strSourcePath
		, m_bSrcSub ? m_strSrcSubPath : _T("")
		, m_nSrcDTF);
	
}

void CSettingsDlg::RefreshTeleLike()
{
	m_strTeleFolderLike = CDistribute::BuildPath(COleDateTime::GetCurrentTime()
		, m_strTelePath
		, m_bTeleSub ? m_strTeleSubPath : _T("")
		, m_nTeleDTF);
	
}

void CSettingsDlg::RefreshPhotoLike()
{
	m_strPhotoFolderLike = CDistribute::BuildPath(COleDateTime::GetCurrentTime()
		, m_strPhotoPath
		, m_bPhotoSub ? m_strPhotoSubPath : _T("")
		, m_nPhotoDTF);
}

void CSettingsDlg::OnChangeEditSource() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshSrcLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditSourceSub() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshSrcLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditSrcTypes() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshSrcLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditPhoto() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshPhotoLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditPhotoSub() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshPhotoLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditPhotoTypes() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshPhotoLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditTele() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshTeleLike();
	
	UpdateData(FALSE);
}

void CSettingsDlg::OnChangeEditTeleSub() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	RefreshTeleLike();
	
	UpdateData(FALSE);
}
