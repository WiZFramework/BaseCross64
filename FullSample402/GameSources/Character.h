/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

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
	///	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<SeekObject> >  m_StateMachine;
		float m_StateChangeSize;
		Vec3 m_Force;
		float m_MaxForce;
		float m_MaxSpeed;
		float m_Decl;
	protected:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr);
		virtual ~SeekObject();
		//������
		virtual void OnCreate() override;
	public:
		//�X�e�[�g�}�V���̎擾
		const unique_ptr<StateMachine<SeekObject>>& GetStateMachine() {
			return m_StateMachine;
		}
		//�X�e�[�g��ύX����T�C�Y�𓾂�
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//�ڕW�܂ł̋����𓾂�
		float GetTargetToLen() const;
		//���݂̑��x�𓾂�i���z�֐��j
		virtual Vec3 GetVelocity() const {
			return Vec3(0);
		}
		//���݂̃t�H�[�X�𓾂�
		const Vec3& GetForce() const {
			return m_Force;
		}
		//�V�[�N�s��
		void SeekBehavior();
		//�����s��
		void ArriveBehavior();
		//�����s��
		void SeparationBehavior();
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȓʖʃI�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ActivePsConvex : public SeekObject {
		Vec3 m_Position;
		//���b�V���i�`��p�j
		static shared_ptr<MeshResource> m_ConvexMesh;
		//�����v�Z�p
		static shared_ptr<PsConvexMeshResource> m_PsConvexMesh;
	public:
		//�\�z�Ɣj��
		ActivePsConvex(const shared_ptr<Stage>& StagePtr,
			const Vec3& Position
		);
		virtual ~ActivePsConvex();
		//������
		virtual void OnCreate() override;
		//���݂̑��x�𓾂�i���z�֐��j
		virtual Vec3 GetVelocity() const;
		//�X�V
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public SeekObject {
		float m_Scale;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//������
		virtual void OnCreate() override;
		//���݂̑��x�𓾂�i���z�֐��j
		virtual Vec3 GetVelocity() const;
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȍ����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ActivePsCombinedObject : public GameObject {
		Quat m_Qt;
		Vec3 m_Position;
		//���b�V���i�J�v�Z���j
		shared_ptr<MeshResource> m_CapsuleMesh;
		shared_ptr<MultiMeshResource> m_MultiMeshResource;
		//�`��R���|�[�l���g�̍쐬
		void CreateDrawComp(const PsCombinedParam& param);
	public:
		//�\�z�Ɣj��
		ActivePsCombinedObject(const shared_ptr<Stage>& StagePtr,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCombinedObject();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	class FirePsSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		void CreateDefParam(PsSphereParam& param);
	public:
		//�\�z�Ɣj��
		FirePsSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FirePsSphere();
		//������
		virtual void OnCreate() override;
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};



}
//end basecross
