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
		//�r���[�̍쐬
		void CreateViewLight();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//��Q�����̍쐬
		void CreateFixedSphere();
		//�ǂ�������I�u�W�F�N�g�̍쐬
		void CreateSeekObject();
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		void CreatePursuitObject();
		//�o�H���񂷂�I�u�W�F�N�g�̍쐬
		void CreateFollowPathObject();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

