
// BaseCrossTool.h : BaseCrossTool アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CBaseCrossToolApp:
// このクラスの実装については、BaseCrossTool.cpp を参照してください
//

class CBaseCrossToolApp : public CWinApp
{
	HWND m_DxHWnd;
public:
	CBaseCrossToolApp() noexcept;
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

extern CBaseCrossToolApp theApp;
