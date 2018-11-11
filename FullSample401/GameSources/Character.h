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
	///	�����v�Z����A�N�e�B�u�ȃI�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class ActivePsObject : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<ActivePsObject>>  m_StateMachine;
	protected:
		ActivePsObject(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{
		}
		virtual ~ActivePsObject() {}
		//������
		virtual void OnCreate() override;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<ActivePsObject> >& GetStateMachine() {
			return m_StateMachine;
		}
		//�z�[���h�̐ݒ�
		void SetHold(bool b);
		//�z�[���h���̍s���i���z�֐��j
		virtual void SetHoldBehavior(bool b) {}
	};

	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class ActivePsDefaultState : public ObjState<ActivePsObject>
	{
		ActivePsDefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(ActivePsDefaultState)
		virtual void Enter(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Execute(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Exit(const shared_ptr<ActivePsObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�I���X�e�[�g
	//--------------------------------------------------------------------------------------
	class ActivePsHoldState : public ObjState<ActivePsObject>
	{
		ActivePsHoldState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(ActivePsHoldState)
		virtual void Enter(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Execute(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Exit(const shared_ptr<ActivePsObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public ActivePsObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Scale	�X�P�[��
		@param[in]	Qt	��]
		@param[in]	Position	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsBox();
		//������
		virtual void OnCreate() override;
		//�z�[���h���̍s���i���z�֐��j
		virtual void SetHoldBehavior(bool b) override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public ActivePsObject {
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Scale	�X�P�[��
		@param[in]	Qt	��]
		@param[in]	Position	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//������
		virtual void OnCreate() override;
		//�z�[���h���̍s���i���z�֐��j
		virtual void SetHoldBehavior(bool b) override;
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Len	����
		@param[in]	Diameter	���a
		@param[in]	Qt	��]
		@param[in]	Position	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
			float Len,
			float Diameter,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCapsule();
		//������
		virtual void OnCreate() override;
		//�z�[���h���̍s���i���z�֐��j
		virtual void SetHoldBehavior(bool b) override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z���Ȃ����˂��鋅��
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		weak_ptr<ActivePsObject> m_HoldObject;
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
