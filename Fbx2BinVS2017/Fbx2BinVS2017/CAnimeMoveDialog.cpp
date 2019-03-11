// CAnimeMoveDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "CAnimeMoveDialog.h"
#include "afxdialogex.h"


// CAnimeMoveDialog ダイアログ

IMPLEMENT_DYNAMIC(CAnimeMoveDialog, CDialogEx)

CAnimeMoveDialog::CAnimeMoveDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANIME_MOVE, pParent)
	, m_FrameRate(_T(""))
	, m_AnimeStartSec(_T(""))
	, m_AnimeEndSec(_T(""))
	, m_IsLooped(FALSE)
{

}

CAnimeMoveDialog::~CAnimeMoveDialog()
{
}

void CAnimeMoveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRAME_RATE, m_FrameRate);
	DDX_Text(pDX, IDC_EDIT_ANIME_START_SEC, m_AnimeStartSec);
	DDX_Text(pDX, IDC_EDIT_ANIME_END_SEC, m_AnimeEndSec);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_IsLooped);
}


BEGIN_MESSAGE_MAP(CAnimeMoveDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAnimeMoveDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CAnimeMoveDialog メッセージ ハンドラー


void CAnimeMoveDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (_wtoi(m_FrameRate) <= 0) {
		AfxMessageBox(L"フレームレートは0以下は設定できません。");
		return;
	}

	if (_wtoi(m_AnimeStartSec) < 0) {
		AfxMessageBox(L"スタートは0未満は設定できません。");
		return;
	}
	if (_wtoi(m_AnimeStartSec) >= _wtoi(m_AnimeEndSec)) {
		AfxMessageBox(L"スタートより終了は大きくなければなりません。");
		return;
	}


	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


BOOL CAnimeMoveDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


