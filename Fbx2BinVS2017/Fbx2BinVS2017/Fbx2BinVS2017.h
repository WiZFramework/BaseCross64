
// Fbx2BinVS2017.h : Fbx2BinVS2017 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CFbx2BinVS2017App:
// ���̃N���X�̎����ɂ��ẮAFbx2BinVS2017.cpp ���Q�Ƃ��Ă�������
//

class CFbx2BinVS2017App : public CWinApp
{
	HWND m_DxHWnd;
public:
	CFbx2BinVS2017App() noexcept;
	void CreateAppScene(HWND hWnd);


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CFbx2BinVS2017App theApp;
