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
		//�������̃X�v���C�g�쐬
		void CreateTraceSprite();
		//�ǖ͗l�̃X�v���C�g�쐬
		void CreateWallSprite();
		//�X�N���[������X�v���C�g�쐬
		void CreateScrollSprite();
		//�X�R�A�X�v���C�g�쐬
		void CreateScoreSprite();
		//�g�[�^������
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(),m_TotalTime(0) {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
	};


}
//end basecross

