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
		//�G�t�F�N�g�̃C���^�[�t�F�C�X
		shared_ptr<EfkInterface> m_EfkInterface;
		//�r���[�̍쐬
		void CreateViewLight();
		//�{�b�N�X�̍쐬
		void CreateFixedBox();
		//�S�[���I�u�W�F�N�g�̍쐬
		void CreateGoalObject();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�G�t�F�N�g�̃C���^�[�t�F�C�X�̎擾
		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
		//�X�V
		virtual void OnUpdate() override;
		//�`��
		virtual void OnDraw() override;
	};


}
//end basecross

