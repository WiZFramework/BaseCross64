/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	///	�G�̐e
	//--------------------------------------------------------------------------------------
	class EnemyBase : public GameObject {
		Vec3 m_StartPos;
		//�t�H�[�X
		Vec3 m_Force;
		//���x
		Vec3 m_Velocity;
	protected:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		EnemyBase(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EnemyBase() {}
	public:
		//�A�N�Z�T
		const Vec3& GetForce()const {
			return m_Force;
		}
		void SetForce(const Vec3& f) {
			m_Force = f;
		}
		void AddForce(const Vec3& f) {
			m_Force += f;
		}
		const Vec3& GetVelocity()const {
			return m_Velocity;
		}
		void SetVelocity(const Vec3& v) {
			m_Velocity = v;
		}
		void ApplyForce();
		Vec3 GetTargetPos()const;
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};



	//--------------------------------------------------------------------------------------
	///	�G�P
	//--------------------------------------------------------------------------------------
	class Enemy1 : public EnemyBase {
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Enemy1>>  m_StateMachine;
		//Near��Far��؂�ւ���l
		float m_StateChangeSize;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Enemy1(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy1() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy1>>& GetStateMachine() {
			return m_StateMachine;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy1��Far�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy1FarState : public ObjState<Enemy1>
	{
		Enemy1FarState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy1FarState)
		virtual void Enter(const shared_ptr<Enemy1>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy1>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy1>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy1��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy1NearState : public ObjState<Enemy1>
	{
		Enemy1NearState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy1NearState)
		virtual void Enter(const shared_ptr<Enemy1>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy1>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy1>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�G2
	//--------------------------------------------------------------------------------------
	class Enemy2 : public EnemyBase {
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Enemy2>>  m_StateMachine;
		//long��medium��؂�ւ���l
		const float m_LongMediumChange;
		//medium��short��؂�ւ���l
		const float m_MediumShortChange;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Enemy2(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy2() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy2>>& GetStateMachine() {
			return m_StateMachine;
		}
		float GetLongMediumChange() const {
			return m_LongMediumChange;
		}
		float GetMediumShortChange() const {
			return m_MediumShortChange;

		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//�Փˎ�
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy2��long�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy2longState : public ObjState<Enemy2>
	{
		Enemy2longState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy2longState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy2��Medium�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy2MediumState : public ObjState<Enemy2>
	{
		Enemy2MediumState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy2MediumState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy2��Short�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy2ShortState : public ObjState<Enemy2>
	{
		Enemy2ShortState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy2ShortState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�G�R
	//--------------------------------------------------------------------------------------
	class Enemy3 : public EnemyBase {
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Enemy3>>  m_StateMachine;
		//default��near��؂�ւ���l
		const float m_DefaultNearChange;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Enemy3(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy3() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy3>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Default��Near�̋��l�𓾂�
		@return	Default��Near�̋��l
		*/
		//--------------------------------------------------------------------------------------
		float GetDefaultNearChange() const {
			return m_DefaultNearChange;

		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy3��Default�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy3DefaultState : public ObjState<Enemy3>
	{
		Enemy3DefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy3DefaultState)
		virtual void Enter(const shared_ptr<Enemy3>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy3>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy3>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy3��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy3NearState : public ObjState<Enemy3>
	{
		Enemy3NearState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy3NearState)
		virtual void Enter(const shared_ptr<Enemy3>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy3>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy3>& Obj)override;
	};



}
//end basecross
