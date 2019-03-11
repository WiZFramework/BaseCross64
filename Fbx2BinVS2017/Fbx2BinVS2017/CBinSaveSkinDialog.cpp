// CBinSaveSkinDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "CBinSaveSkinDialog.h"
#include "CChangeExt.h"
#include "afxdialogex.h"


// CBinSaveSkinDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CBinSaveSkinDialog, CDialogEx)

CBinSaveSkinDialog::CBinSaveSkinDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SAVE_BINFILE_SKIN, pParent)
	, m_FullBinFileName(_T(""))
	, m_Scale(_T(""))
	, m_IsNowScale(FALSE)
	, m_FrameRate(_T(""))
	, m_AnimeStartSec(_T(""))
	, m_AnimeEndSec(_T(""))
{

}

CBinSaveSkinDialog::~CBinSaveSkinDialog()
{
}

void CBinSaveSkinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SAVE_BINFILENAME_SKIN, m_FullBinFileName);
	DDX_Text(pDX, IDC_STATIC_SCALING_SKIN, m_Scale);
	DDV_MaxChars(pDX, m_Scale, 5);
	DDX_Check(pDX, IDC_CHECK_NOW_SCALE_SKIN, m_IsNowScale);
	DDX_Text(pDX, IDC_EDIT_FRAME_RATE_SKIN, m_FrameRate);
	DDV_MaxChars(pDX, m_FrameRate, 2);
	DDX_Text(pDX, DC_EDIT_ANIME_START_SEC_SKIN, m_AnimeStartSec);
	DDV_MaxChars(pDX, m_AnimeStartSec, 4);
	DDX_Text(pDX, IDC_EDIT_ANIME_END_SEC_SKIN, m_AnimeEndSec);
	DDV_MaxChars(pDX, m_AnimeEndSec, 4);
}


BEGIN_MESSAGE_MAP(CBinSaveSkinDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_EXT_SKIN, &CBinSaveSkinDialog::OnBnClickedButtonUpdateExtSkin)
	ON_BN_CLICKED(IDOK, &CBinSaveSkinDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CBinSaveSkinDialog ���b�Z�[�W �n���h���[


BOOL CBinSaveSkinDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CBinSaveSkinDialog::OnBnClickedButtonUpdateExtSkin()
{
	CChangeExt ChangeExt;
	ChangeExt.m_NewExt = m_FileExt;
	ChangeExt.m_NewExt.Replace(L".", L"");
	if (ChangeExt.DoModal() == IDOK) {
		ChangeExt.m_NewExt.Replace(L".", L"");
		m_FileExt = L".";
		m_FileExt += ChangeExt.m_NewExt;

		m_FullBinFileName = m_FileDir;
		m_FullBinFileName += m_FileNameNoExt;
		m_FullBinFileName += m_FileExt;
		UpdateData(FALSE);
	}
}


void CBinSaveSkinDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (_wtoi(m_FrameRate) <= 0) {
		AfxMessageBox(L"�t���[�����[�g��0�ȉ��͐ݒ�ł��܂���B");
		return;
	}

	if (_wtoi(m_AnimeStartSec) < 0) {
		AfxMessageBox(L"�X�^�[�g��0�����͐ݒ�ł��܂���B");
		return;
	}
	if (_wtoi(m_AnimeStartSec) >= _wtoi(m_AnimeEndSec)) {
		AfxMessageBox(L"�X�^�[�g���I���͑傫���Ȃ���΂Ȃ�܂���B");
		return;
	}

	CDialogEx::OnOK();
}
