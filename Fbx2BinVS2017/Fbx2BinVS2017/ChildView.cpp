
// ChildView.cpp : CChildView クラスの実装
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "ChildView.h"
#include "Project.h"
using namespace basecross;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CChildView メッセージ ハンドラー

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト
	
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	
	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。
}



void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
		App::GetApp()->GetScene<Scene>()->AnimePoseStart();
	}

//	CWnd::OnLButtonDblClk(nFlags, point);
}
