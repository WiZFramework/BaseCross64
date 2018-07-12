/*!
@file RigidMini.h
@brief 簡易物理計算コンポーネント
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

	//ペアの種類
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
	///	 簡易物理計算コンポーネント
	//--------------------------------------------------------------------------------------
	class RigidMini : public Component {
		RmState m_RmState;
		RmBody m_RmBody;
		RmShape m_RmShape;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit RigidMini(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RigidMini();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	簡易物理計算マネージャを返す
		@return	簡易物理計算マネージャ
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