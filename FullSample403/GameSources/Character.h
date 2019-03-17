/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		weak_ptr<GameObject> m_StartObj;
		weak_ptr<GameObject> m_EndObj;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	StartObj	�X�^�[�g�̃I�u�W�F�N�g
		@param[in]	EndObj	�G���h�̃I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& StartObj,
			const shared_ptr<GameObject>& EndObj);
		virtual ~ActionLine() {}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//���̃X�^�[�g�I�u�W�F�N�g�̎擾
		shared_ptr<GameObject> GetStartObj() const {
			return m_StartObj.lock();
		}
		//���̃X�^�[�g�I�u�W�F�N�g�̐ݒ�
		void SetStartObj(const shared_ptr<GameObject>& Obj) {
			m_StartObj = Obj;
		}
		//���̃G���h�I�u�W�F�N�g�̎擾
		shared_ptr<GameObject> GetEndObj() const {
			return m_EndObj.lock();
		}
		//���̃G���h�I�u�W�F�N�g�̐ݒ�
		void SetEndObj(const shared_ptr<GameObject>& Obj) {
			m_EndObj = Obj;
		}
		//���̃X�^�[�g�ƃG���h�I�u�W�F�N�g�̐ݒ�
		void ResetObject(const shared_ptr<GameObject>& StartObj, const shared_ptr<GameObject>& EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
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
	///	�����v�Z�������
	//--------------------------------------------------------------------------------------
	class PsBrick : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<PsBrick>>  m_StateMachine;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_Mass;
		bool m_IsFixed;
		//�����ɂ��Ă���W���C���g
		weak_ptr<PsJoint> m_Joint;
		//�z�[���h�J�E���g
		size_t m_HoldCount;
	public:
		//�\�z�Ɣj��
		PsBrick(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float Mass,
			bool IsFixed
		);
		virtual ~PsBrick();
		//������
		virtual void OnCreate() override;
		//�X�e�[�g�}�V�[���𓾂�
		unique_ptr< StateMachine<PsBrick> >& GetStateMachine() {
			return m_StateMachine;
		}
		//�z�[���h�̒ǉ�
		void AddHoldCount();
		//�z�[���h�̉��
		void ResetHoldCount();
		//�z�[���h���̍s��
		void SetHoldBehavior(bool b);
		//Fix���ǂ���
		bool IsFixed() const {
			return m_IsFixed;
		}
		//�W���C���g�̐ݒ�
		void SetJoint(const shared_ptr<PsJoint> joint) {
			m_Joint = joint;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PsBrickDefaultState : public ObjState<PsBrick>
	{
		PsBrickDefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PsBrickDefaultState)
		virtual void Enter(const shared_ptr<PsBrick>& Obj)override;
		virtual void Execute(const shared_ptr<PsBrick>& Obj)override;
		virtual void Exit(const shared_ptr<PsBrick>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�I���X�e�[�g
	//--------------------------------------------------------------------------------------
	class PsBrickHoldState : public ObjState<PsBrick>
	{
		PsBrickHoldState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PsBrickHoldState)
		virtual void Enter(const shared_ptr<PsBrick>& Obj)override;
		virtual void Execute(const shared_ptr<PsBrick>& Obj)override;
		virtual void Exit(const shared_ptr<PsBrick>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	/// �W���C���g�}�l�[�W��
	//--------------------------------------------------------------------------------------
	class PsJointManager : public GameObject {
		shared_ptr<PsBallJoint> m_PsBallJoint;
		shared_ptr<PsSwingTwistJoint> m_PsSwingTwistJoint;
		shared_ptr<PsHingeJoint> m_PsHingeJoint;
		shared_ptr<PsSliderJoint> m_PsSliderJoint;
		shared_ptr<PsFixJoint> m_PsFixJoint;
		shared_ptr<PsUniversalJoint> m_PsUniversalJoint;
		void CreateBallJoint();
		void CreateSwingTwistJoint();
		void CreateHingeJoint();
		void CreateSliderJoint();
		void CreateFixJoint();
		void CreateUniversalJoint();
	public:
		//�\�z�Ɣj��
		PsJointManager(const shared_ptr<Stage>& StagePtr);
		virtual ~PsJointManager();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z���Ȃ����˂��鋅��
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		weak_ptr<PsBrick> m_HoldObject;
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
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};


}
//end basecross
