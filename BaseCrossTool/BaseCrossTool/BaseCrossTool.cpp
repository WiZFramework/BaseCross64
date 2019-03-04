﻿
// BaseCrossTool.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BaseCrossTool.h"
#include "MainFrm.h"
#include "Project.h"
using namespace basecross;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBaseCrossToolApp

BEGIN_MESSAGE_MAP(CBaseCrossToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBaseCrossToolApp::OnAppAbout)
END_MESSAGE_MAP()


// CBaseCrossToolApp の構築

CBaseCrossToolApp::CBaseCrossToolApp() noexcept
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("BaseCrossTool.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CBaseCrossToolApp オブジェクト

CBaseCrossToolApp theApp;

void CBaseCrossToolApp::CreateAppScene(HWND hWnd) {
	//アプリケーションクラスとデバイスの構築
	////アプリケーションクラスの構築
	try {
		m_DxHWnd = hWnd;
		App::CreateApp(AfxGetInstanceHandle(), hWnd, false, 1280, 800);
		App::GetApp()->CreateScene<Scene>();
	}
	catch (...) {
		throw;
	}
}


// CBaseCrossToolApp の初期化

BOOL CBaseCrossToolApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));


	// メイン ウィンドウを作成するとき、このコードは新しいフレーム ウィンドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウィンドウにセットします
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	// フレームをリソースからロードして作成します
	CString WindowName;
	WindowName.LoadString(IDR_MAINFRAME);
	CRect rect = { 0, 0,1280, 800 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	rect += CPoint(100, 100);
	if (!pFrame->Create(NULL, WindowName, WS_OVERLAPPEDWINDOW |
		FWS_ADDTOTITLE, rect, NULL, MAKEINTRESOURCE(IDR_MAINFRAME)))
		return FALSE;

	//pFrame->LoadFrame(IDR_MAINFRAME,
	//	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
	//	nullptr);

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;

}

int CBaseCrossToolApp::ExitInstance()
{
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CBaseCrossToolApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CBaseCrossToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBaseCrossToolApp メッセージ ハンドラー

BOOL CBaseCrossToolApp::OnIdle(LONG lCount)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (CWinApp::OnIdle(lCount))
		return TRUE;
	//ここに設定したキーボード入力を得る
	vector<DWORD> UseKeyVec = { VK_LEFT,VK_RIGHT ,VK_UP,VK_DOWN };
	App::GetApp()->ResetInputState(m_DxHWnd, UseKeyVec);
	//更新描画処理
	App::GetApp()->UpdateDraw(1);
	return TRUE;
}
