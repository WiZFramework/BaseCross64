
// Fbx2BinVS2017.h : Fbx2BinVS2017 アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CFbx2BinVS2017App:
// このクラスの実装については、Fbx2BinVS2017.cpp を参照してください
//

class CFbx2BinVS2017App : public CWinApp
{
	HWND m_DxHWnd;
public:
	CFbx2BinVS2017App() noexcept;
	void CreateAppScene(HWND hWnd);


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CFbx2BinVS2017App theApp;
