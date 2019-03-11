
// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	//FBX���b�V���ǂݍ��ݎ��̏����l
	CString m_MeshScale;
	CString m_MeshPosX;
	CString m_MeshPosY;
	CString m_MeshPosZ;
	BOOL m_IsReadStatic;
	BOOL m_WithTangent;
	//�^���W�F���g�g�p���̖@���}�b�v�t�@�C��
	wstring m_NormalmapFullFileName;
	//�e�N�X�`�������b�s���O�������邩�ǂ���
	BOOL m_TextureWrap;


	//���b�V���̃t���p�X�f�B���N�g���i�Ō��/�͂��Ȃ��j
	wstring m_MeshDir;
	//���b�V���̃t�@�C����
	wstring m_MeshFilename;
	//�\���E�ۑ��X�P�[���̈ێ�
	BOOL m_IsNowScale;
	//�ȉ��A�j���[�V�����֌W
	//�t���[�����[�g
	UINT m_FrameRate;
	UINT m_StartSec;
	UINT m_EndSec;
	BOOL m_Looped;

public:
	CMainFrame() noexcept;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CChildView    m_wndView;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNewStage();
	afx_msg void OnOpenFbxFile();
	afx_msg void OnSaveBin();
	afx_msg void OnMoveFbxFile();
	afx_msg void OnUpdateMoveFbxFile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSaveBin(CCmdUI *pCmdUI);
};


