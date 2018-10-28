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
		//�Œ�̋��̂̍쐬
		void CreateFixedSphere();
		//�Œ�̃J�v�Z���̍쐬
		void CreateFixedCapsule();
		//�ړ�����{�b�N�X�̍쐬
		void CreateMoveBox();
		//�ǂ�������I�u�W�F�N�g�̍쐬
		void CreateSeekObject();
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

