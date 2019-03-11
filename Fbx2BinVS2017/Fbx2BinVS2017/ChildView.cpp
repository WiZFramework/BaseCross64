
// ChildView.cpp : CChildView �N���X�̎���
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



// CChildView ���b�Z�[�W �n���h���[

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
	CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g
	
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	
	// ���b�Z�[�W�̕`��̂��߂� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
}



void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
		App::GetApp()->GetScene<Scene>()->AnimePoseStart();
	}

//	CWnd::OnLButtonDblClk(nFlags, point);
}
