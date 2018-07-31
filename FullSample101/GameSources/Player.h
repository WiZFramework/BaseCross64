/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	�v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		// �R���g���[����������x�N�g���𓾂�
		Vec3 GetMoveVector() const;
		//������̕\��
		void DrawStrings();
		//���̓n���h���[
		InputHandler<Player> m_InputHandler;
		//���x
		Vec3 m_Velocity;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//�A�N�Z�T
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//A�{�^��
		void OnPushA();
		void MovePlayer();
		virtual void OnCollisionEnter(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec) override;
	};






}
//end basecross

