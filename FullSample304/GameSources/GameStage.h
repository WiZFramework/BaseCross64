/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		shared_ptr<SoundItem> m_BGM;
		//�r���[�̍쐬
		void CreateViewLight();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�㉺�ړ����Ă���{�b�N�X�̍쐬
		void CreateMoveBox();
		//�ǂ�������I�u�W�F�N�g�̍쐬
		void CreateSeekObject();
		//�X�p�[�N�̍쐬
		void CreateSpark();
		//���̍쐬
		void CreateFire();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//BGM�̍Đ�
		void PlayBGM();
		//�X�v���C�g�̍쐬
		void CreateSprite();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
		//�폜��
		virtual void OnDestroy()override;
	};

	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class TitleStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		//�X�v���C�g�̍쐬
		void CreateTitleSprite();
	public:
		//�\�z�Ɣj��
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;

	};

	//--------------------------------------------------------------------------------------
	//	�E�G�C�g�X�e�[�W�N���X�i���\�[�X�ǂݍ��ݗp�j
	//--------------------------------------------------------------------------------------
	class WaitStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		//�X�v���C�g�̍쐬
		void CreateTitleSprite();
		//���\�[�X���[�h�p�̃X���b�h�i�X�^�e�B�b�N�֐��j
		static void LoadResourceFunc();
		//���\�[�X��ǂݍ��񂾂��Ƃ�m�点��t���O�i�X�^�e�B�b�N�ϐ��j
		static bool m_Loaded;
		//�~���[�e�b�N�X
		static std::mutex mtx;
	public:
		//�\�z�Ɣj��
		WaitStage() :Stage() {}
		virtual ~WaitStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
	};




}
//end basecross

