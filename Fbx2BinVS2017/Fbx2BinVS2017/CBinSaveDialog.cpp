// CBinSaveDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "CBinSaveDialog.h"
#include "CChangeExt.h"
#include "afxdialogex.h"


// CBinSaveDialog ダイアログ

IMPLEMENT_DYNAMIC(CBinSaveDialog, CDialogEx)

CBinSaveDialog::CBinSaveDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SAVE_BINFILE, pParent)
	, m_IsNowScale(FALSE)
	, m_Scale(_T(""))
	, m_FullBinFileName(_T(""))
{

}

CBinSaveDialog::~CBinSaveDialog()
{
}

void CBinSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_NOW_SCALE, m_IsNowScale);
	DDX_Text(pDX, IDC_STATIC_SCALING, m_Scale);
	DDX_Text(pDX, IDC_STATIC_SAVE_BINFILENAME, m_FullBinFileName);
}


BEGIN_MESSAGE_MAP(CBinSaveDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_EXT, &CBinSaveDialog::OnClickedButtonUpdateExt)
END_MESSAGE_MAP()


// CBinSaveDialog メッセージ ハンドラー


void CBinSaveDialog::OnClickedButtonUpdateExt()
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


BOOL CBinSaveDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
