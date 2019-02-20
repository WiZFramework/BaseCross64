/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//������
		virtual void OnCreate() override;
		//�X�p�[�N���o
		void InsertSpark(const Vec3& Pos);
	};

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<Box> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_TotalTime;
		float m_HitAfterTime;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~Box();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//�X�e�[�g�}�V���𓾂�
		const unique_ptr<StateMachine<Box>>& GetStateMachine() {
			return m_StateMachine;
		}
		//�v���C���[�Ƃ̋����𓾂�
		float GetToPlayerLen() const;
		//�C�e�Ƀq�b�g������̎��Ԃ����Z����i���݂̃q�b�g��̎��Ԃ�Ԃ��j
		float AddHitAfterTime();
		//���x��0�ɂ���
		void Stop();
		//�s�����藈����̍s��
		void GoFromBehavior();
		//�v���C���[��ǂ�������s��
		void SeekPlayerBehavior();
		//�C�e���ˍs��
		void FirShellBehavior(int div, float up);
		//�C�e�����������s��
		void FirShellHitBehavior(const Vec3& HitVelocity);
	};

	//--------------------------------------------------------------------------------------
	//	�ʏ�ړ�
	//--------------------------------------------------------------------------------------
	class BoxDefaultState : public ObjState<Box>
	{
		BoxDefaultState() {}
	public:
		static shared_ptr<BoxDefaultState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	�ǂ�������ʏ�U���X�e�[�g
	//--------------------------------------------------------------------------------------
	class BoxSimpleAttackState : public ObjState<Box>
	{
		BoxSimpleAttackState() {}
	public:
		static shared_ptr<BoxSimpleAttackState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	�n�[�h�ȍU���X�e�[�g
	//--------------------------------------------------------------------------------------
	class BoxHradAttackState : public ObjState<Box>
	{
		BoxHradAttackState() {}
	public:
		static shared_ptr<BoxHradAttackState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	�C�e�ɓ��������X�e�[�g
	//--------------------------------------------------------------------------------------
	class BoxHitState : public ObjState<Box>
	{
		BoxHitState() {}
	public:
		static shared_ptr<BoxHitState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};



	//--------------------------------------------------------------------------------------
	///	�C�e
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		float m_ActiveMaxY;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Emitter	���ˈʒu
		@param[in]	Velocity	���x
		*/
		//--------------------------------------------------------------------------------------
		FireSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FireSphere();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(const CollisionPair& Pair) override;
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃI�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class ActivePsObject : public GameObject {
		bool m_Selected;
	protected:
		ActivePsObject(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr), m_Selected(false) {}
		virtual ~ActivePsObject() {}
	public:
		void SetSelected(bool b) {
			m_Selected = b;
			auto PtrDraw = AddComponent<BcPNTStaticDraw>();
			if (b) {
				PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

			}
			else {
				PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
			}

		}
		bool IsSelected() {
			return m_Selected;
		}
		//�X�V
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public ActivePsObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsBox();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public ActivePsObject {
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃJ�v�Z��
	//--------------------------------------------------------------------------------------
	class ActivePsCapsule : public ActivePsObject {
		//����
		float m_Len;
		//���a
		float m_Diameter;
		Quat m_Qt;
		Vec3 m_Position;
		//���b�V���i�J�v�Z���͊e�I�u�W�F�N�g���ێ��j
		shared_ptr<MeshResource> m_CapsuleMesh;
	public:
		//�\�z�Ɣj��
		ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
			float Len,
			float Diameter,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCapsule();
		//������
		virtual void OnCreate() override;
	};




}
//end basecross
