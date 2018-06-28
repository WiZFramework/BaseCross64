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
		//�ō����x
		float m_MaxSpeed;
		//������
		float m_Decel;
		//����
		float m_Mass;
		// �R���g���[����������x�N�g���𓾂�
		Vec3 GetMoveVector() const;
		//������̕\��
		void DrawStrings();
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Player>>  m_StateMachine;
		//���̓n���h���[
		InputHandler<Player> m_InputHandler;
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ō����x�𓾂�
		@return	�ō����x
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����l�𓾂�
		@return	�����l
		*/
		//--------------------------------------------------------------------------------------
		float GetDecel() const {
			return m_Decel;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ʂ𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<StateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//A�{�^��
		void OnPushA() {}
		void MovePlayer();
		virtual void OnCollisionEnter(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec) override;
	};


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerDefaultState : public ObjState<Player>
	{
		PlayerDefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerJumpState : public ObjState<Player>
	{
		PlayerJumpState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerJumpState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};





}
//end basecross

