
// ChildView.h : CChildView �N���X�̃C���^�[�t�F�C�X
//


#pragma once


// CChildView �E�B���h�E

class CChildView : public CWnd
{
// �R���X�g���N�V����
public:
	CChildView();

// ����
public:

// ����
public:

// �I�[�o�[���C�h
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CChildView();

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

