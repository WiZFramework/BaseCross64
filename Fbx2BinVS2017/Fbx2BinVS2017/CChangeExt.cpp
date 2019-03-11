// CChangeExt.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "CChangeExt.h"
#include "afxdialogex.h"


// CChangeExt �_�C�A���O

IMPLEMENT_DYNAMIC(CChangeExt, CDialogEx)

CChangeExt::CChangeExt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHANGE_EXT, pParent)
	, m_NewExt(_T(""))
{

}

CChangeExt::~CChangeExt()
{
}

void CChangeExt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEW_EXT, m_NewExt);
	DDV_MaxChars(pDX, m_NewExt, 5);
}


BEGIN_MESSAGE_MAP(CChangeExt, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChangeExt::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeExt ���b�Z�[�W �n���h���[


BOOL CChangeExt::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CChangeExt::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_NewExt == L"") {
		AfxMessageBox(L"�g���q�ɋ󔒂͎w��ł��܂���B");
		return;
	}

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnOK();
}
