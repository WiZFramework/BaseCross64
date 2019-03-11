// CFbxOpenDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "CFbxOpenDialog.h"
#include "afxdialogex.h"


// CFbxOpenDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CFbxOpenDialog, CDialogEx)

CFbxOpenDialog::CFbxOpenDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FILE_SELECT, pParent)
	, m_FullFileName(_T(""))
	, m_NormalmapFullFileName(_T(""))
	, m_Scale(_T(""))
	, m_PosX(_T(""))
	, m_PosY(_T(""))
	, m_PosZ(_T(""))
	, m_TextureWrap(FALSE)
	, m_IsReadStatic(FALSE)
	, m_WithTangent(FALSE)
{

}

CFbxOpenDialog::~CFbxOpenDialog()
{
}

void CFbxOpenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_OPEN_FILENAME, m_FullFileName);
	DDX_Text(pDX, IDC_STATIC_OPEN_NORMAL_FILENAME, m_NormalmapFullFileName);
	DDX_Text(pDX, IDC_EDIT_SCALING, m_Scale);
	DDV_MaxChars(pDX, m_Scale, 5);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_PosX);
	DDV_MaxChars(pDX, m_PosX, 5);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_PosY);
	DDV_MaxChars(pDX, m_PosY, 5);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_PosZ);
	DDV_MaxChars(pDX, m_PosZ, 5);
	DDX_Check(pDX, IDC_CHECK_READ_TEXTURE_WRAP, m_TextureWrap);
	DDX_Check(pDX, IDC_CHECK_READ_STATICMESH, m_IsReadStatic);
	DDX_Check(pDX, IDC_CHECK_READ_TANGENT, m_WithTangent);
}


BEGIN_MESSAGE_MAP(CFbxOpenDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FILEDIALOG_OPEN, &CFbxOpenDialog::OnBnClickedButtonFiledialogOpen)
	ON_BN_CLICKED(IDC_BUTTON_FILEDIALOG_NORMALMAP_OPEN, &CFbxOpenDialog::OnBnClickedButtonFiledialogNormalmapOpen)
	ON_BN_CLICKED(IDOK, &CFbxOpenDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CFbxOpenDialog ���b�Z�[�W �n���h���[


void CFbxOpenDialog::OnBnClickedButtonFiledialogOpen()
{
	UpdateData(TRUE);

	CString         filter("FBX Files (*.fbx;*.FBX)|*.fbx; *.FBX||");
	CFileDialog     OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (OpenDlg.DoModal() == IDOK)
	{
		m_FullFileName = OpenDlg.GetPathName();
		m_FullFileDir = OpenDlg.GetFolderPath();
		m_FileName = OpenDlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CFbxOpenDialog::OnBnClickedButtonFiledialogNormalmapOpen()
{
	UpdateData(TRUE);

	CFileDialog     OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);

	if (OpenDlg.DoModal() == IDOK)
	{
		m_NormalmapFullFileName = OpenDlg.GetPathName();
		UpdateData(FALSE);
	}
}


BOOL CFbxOpenDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CFbxOpenDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_FullFileDir == L"" || m_FileName == L"") {
		AfxMessageBox(L"FBX�t�@�C�����I������Ă��܂���B");
		return;
	}
	float Scale = (float)_wtof(m_Scale);
	if (Scale <= 0.0f) {
		AfxMessageBox(L"�X�P�[����0.0�ȉ��͐ݒ�ł��܂���B");
		return;

	}
	if (m_WithTangent) {
		//�^���W�F���g�t��
		if (m_NormalmapFullFileName == L"") {
			AfxMessageBox(L"�^���W�F���g�w�肪����܂����A\n�@���}�b�v�t�@�C�����I������Ă��܂���B");
			return;
		}

	}
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnOK();
}
