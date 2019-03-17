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
		//������̕\��
		void DrawStrings();
		//���̓n���h���[
		InputHandler<Player> m_InputHandler;
		//�i�s�����𓾂�
		Vec3 GetMoveVector() const;
		//�X�P�[�����O�i���a�̑傫���j
		float m_Scale;
		//���C���I�u�W�F�N�g�iweak_ptr�j
		weak_ptr<PositionLine> m_PositionLine;
		//�z�[���h���Ă���}���`�����I�u�W�F�N�g
		size_t m_HoledIndex;
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
		//��X�V
		virtual void OnUpdate2() override;
		//A�{�^���n���h��
		void OnPushA();
		//B�{�^���n���h��
		void OnPushB();
		//Y�{�^���n���h��(�������u��)
		void OnPushY();
		//Y�{�^���n���h���i���������j
		void OnPressY();
		//Y�{�^���n���h��(�����[�X)
		void OnReleaseY();
	};


}
//end basecross

