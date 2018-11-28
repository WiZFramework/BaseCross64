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
	//	��Q����
	//--------------------------------------------------------------------------------------
	class FixedSphere : public GameObject {
		float m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedSphere(const shared_ptr<Stage>& StagePtr,
			const float Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedSphere();
		//������
		virtual void OnCreate() override;
		//����
	};


	//--------------------------------------------------------------------------------------
	//	�z�u�����I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class BaseChara : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<BaseChara> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_StateChangeSize;
		//�t�H�[�X
		Vec3 m_Force;
		//���x
		Vec3 m_Velocity;
		void ApplyForce();
	protected:
		//�\�z�Ɣj��
		BaseChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BaseChara();
	public:
		//�A�N�Z�T
		const unique_ptr<StateMachine<BaseChara>>& GetStateMachine() {
			return m_StateMachine;
		}
		Vec3 GetStartPos() const {
			return m_StartPos;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
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
		shared_ptr<GameObject>  GetTarget()const;
		virtual void NearBehavior() = 0;
		virtual void FarBehavior() = 0;
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<BaseChara>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<BaseChara>& Obj)override;
		virtual void Execute(const shared_ptr<BaseChara>& Obj)override;
		virtual void Exit(const shared_ptr<BaseChara>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	 �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<BaseChara>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<BaseChara>& Obj)override;
		virtual void Execute(const shared_ptr<BaseChara>& Obj)override;
		virtual void Exit(const shared_ptr<BaseChara>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public BaseChara {
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void OnCreate() override;
		//����
		virtual void NearBehavior() override;
		virtual void FarBehavior() override;
	};


	//--------------------------------------------------------------------------------------
	//	�ǐՂ���z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class PursuitObject : public BaseChara {
	public:
		//�\�z�Ɣj��
		PursuitObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PursuitObject();
		//������
		virtual void OnCreate() override;
		//����
		virtual void NearBehavior() override;
		virtual void FarBehavior() override;
	};

	//--------------------------------------------------------------------------------------
	//	�p�X�����񂷂�z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class FollowPathObject : public BaseChara {
	public:
		//�\�z�Ɣj��
		FollowPathObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~FollowPathObject();
		//������
		virtual void OnCreate() override;
		//����
		virtual void NearBehavior() override;
		virtual void FarBehavior() override;
	};





}
//end basecross
