/*!
@file RigidMini.h
@brief �ȈՕ����v�Z�R���|�[�l���g
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class RmManager;
	class RigidMini;

#define RM_TIMESTEP 0.016


	enum class RmMotionType {
		Active,
		Fix,
		None,
	};

	struct RmState {
		bsm::Vec3	m_position;
		bsm::Quat	m_orientation;
		bsm::Vec3	m_linearVelocity;
		bsm::Vec3	m_force;
		RmMotionType m_motionType;
		bool m_gravityActive;
		RmState() :
			m_position(0.0f),
			m_orientation(),
			m_linearVelocity(0.0f),
			m_force(0.0f),
			m_motionType(RmMotionType::Active),
			m_gravityActive(true)
		{
		}
	};

	struct RmBody {
		float       m_mass;
		void reset()
		{
			m_mass = 1.0f;
		}
		RmBody() {
			reset();
		}
	};

	enum class RmShapeType {
		Obb,
		Sphere,
		Capsule,
	};

	struct RmShape {
		RmShapeType m_RmShapeType;
		bsm::Vec3 m_Scale;
		void reset()
		{
			m_RmShapeType = RmShapeType::Obb;
			m_Scale = bsm::Vec3(1.0f, 1.0f, 1.0f);
		}
		RmShape() {
			reset();
		}
	};


	struct RmSolverBody {
		bsm::Vec3 deltaLinearVelocity;
		bsm::Quat    orientation;
		bsm::Mat3x3 inertiaInv;
		float   massInv;
	};

	//�y�A�̎��
	enum class RmPairType {
		TypeNew,
		TypeKeep,
	};

	struct RmPair {
		RmPairType m_Type;
		RigidMini* m_pSrc;
		RigidMini* m_pDest;
		RmPair() :
			m_Type(RmPairType::TypeNew),
			m_pSrc(nullptr),
			m_pDest(nullptr)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	 �ȈՕ����v�Z�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class RigidMini : public Component {
		RmState m_RmState;
		RmBody m_RmBody;
		RmShape m_RmShape;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit RigidMini(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RigidMini();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ����������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�揈���B�f�t�H���g�͉����s��Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ȈՕ����v�Z�}�l�[�W����Ԃ�
		@return	�ȈՕ����v�Z�}�l�[�W��
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RmManager> GetRmManager() const;


		RmShapeType GetShapeType() const {
			return m_RmShape.m_RmShapeType;
		}
		void SetShapeType(RmShapeType t) {
			m_RmShape.m_RmShapeType = t;
		}

		RmMotionType GetMotionType() const {
			return m_RmState.m_motionType;
		}
		void SetMotionType(RmMotionType t) {
			m_RmState.m_motionType = t;
		}

		bsm::Vec3 GetForce() const {
			return m_RmState.m_force;
		}
		void SetForce(const bsm::Vec3& f) {
			m_RmState.m_force = f;
		}
		void ClearForce() {
			SetForce(bsm::Vec3(0.0f));
		}



	};



}
//end basecross