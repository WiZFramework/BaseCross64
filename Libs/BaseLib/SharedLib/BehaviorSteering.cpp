/*!
@file BehaviorSteering.cpp
@brief 操舵行動クラス実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct SteeringBehavior::Impl;
	//--------------------------------------------------------------------------------------
	struct SteeringBehavior::Impl {
		float m_Weight;
		float m_MaxSpeed;
		float m_MaxForce;
		Impl() :
			m_Weight(1.0f),
			m_MaxSpeed(10.0f),
			m_MaxForce(30.0f)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	///	操舵行動の親クラス
	//--------------------------------------------------------------------------------------
	SteeringBehavior::SteeringBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
		Behavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	SteeringBehavior::~SteeringBehavior() {}

	float SteeringBehavior::GetWeight() const { return pImpl->m_Weight; }
	void SteeringBehavior::SetWeight(float f) { pImpl->m_Weight = f; }

	float SteeringBehavior::GetMaxSpeed() const { return pImpl->m_MaxSpeed; }
	void SteeringBehavior::SetMaxSpeed(float f) { pImpl->m_MaxSpeed = f; }

	float SteeringBehavior::GetMaxForce() const { return pImpl->m_MaxForce; }
	void SteeringBehavior::SetMaxForce(float f) { pImpl->m_MaxForce = f; }



	//--------------------------------------------------------------------------------------
	///	SeekSteering行動クラス
	//--------------------------------------------------------------------------------------
	SeekSteering::SeekSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr)
	{}
	SeekSteering::~SeekSteering() {}

	bsm::Vec3 SeekSteering::Execute(const bsm::Vec3& Force, const bsm::Vec3& Velocity, const bsm::Vec3& TargetPos) {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 TempForce = Force;
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Seek(Velocity, TargetPos,
			TransPtr->GetWorldPosition(), GetMaxSpeed()) * GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}


	//--------------------------------------------------------------------------------------
	//	struct ArriveSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct ArriveSteering::Impl {
		float m_Decl;
		Impl() :
			m_Decl(3.0f)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	///	ArriveSteering行動クラス
	//--------------------------------------------------------------------------------------
	ArriveSteering::ArriveSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	ArriveSteering::~ArriveSteering() {}

	float ArriveSteering::GetDecl() const { return pImpl->m_Decl; }
	void ArriveSteering::SetDecl(float f) { pImpl->m_Decl = f; }

	bsm::Vec3 ArriveSteering::Execute(const bsm::Vec3& Force, const bsm::Vec3& Velocity, const bsm::Vec3& TargetPos) {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 TempForce = Force;
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Arrive(Velocity, TargetPos,
			TransPtr->GetWorldPosition(), GetMaxSpeed(), pImpl->m_Decl) *  GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}

	//--------------------------------------------------------------------------------------
	///	PursuitSteering（追跡）行動クラス
	//--------------------------------------------------------------------------------------

	PursuitSteering::PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr)
	{}
	PursuitSteering::~PursuitSteering() {}

	bsm::Vec3 PursuitSteering::Execute(const bsm::Vec3& Force, const bsm::Vec3& Velocity, const bsm::Vec3& TargetPos, const bsm::Vec3& TargetVelocity, const bsm::Vec3& TargetRotation) {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 TempForce = Force;
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Pursuit(Velocity, TransPtr->GetWorldPosition(), 
			bsm::normalize(TransPtr->GetRotation()), GetMaxSpeed(),
			TargetVelocity, TargetPos, 
			bsm::normalize(TargetRotation)) *  GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}

#ifdef test

	//--------------------------------------------------------------------------------------
	//	struct WanderSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct WanderSteering::Impl {
		bsm::Vec3 m_WanderTarget;	//徘徊目標の回転係数（操舵によって書き換えられる）
		float m_WanderRadius;	//徘徊半径
		float m_WanderDistance;	//徘徊円までの距離
		float m_WanderJitter;	//ランダム変異の最大値
		Impl() :
			m_WanderTarget(0, 0, 0),
			m_WanderRadius(1.5f),
			m_WanderDistance(1.0f),
			m_WanderJitter(0.5f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	WanderSteering（徘徊）行動
	//--------------------------------------------------------------------------------------
	WanderSteering::WanderSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	WanderSteering::~WanderSteering() {}
	//アクセサ
	const bsm::Vec3& WanderSteering::GetWanderTarget() const {
		return pImpl->m_WanderTarget;
	}
	void WanderSteering::SetWanderTarget(const bsm::Vec3& target) {
		pImpl->m_WanderTarget = target;
	}
	float WanderSteering::GetWanderRadius() const {
		return pImpl->m_WanderRadius;
	}
	void WanderSteering::SetWanderRadius(float f) {
		pImpl->m_WanderRadius = f;
	}
	float WanderSteering::GetWanderDistance() const {
		return pImpl->m_WanderDistance;
	}
	void WanderSteering::SetWanderDistance(float f) {
		pImpl->m_WanderDistance = f;
	}
	float WanderSteering::GetWanderJitter() const {
		return pImpl->m_WanderJitter;
	}
	void WanderSteering::SetWanderJitter(float f) {
		pImpl->m_WanderJitter = f;
	}


	void WanderSteering::Execute() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Wander(TransPtr->GetWorldMatrix(),
			pImpl->m_WanderRadius, pImpl->m_WanderDistance, pImpl->m_WanderJitter, pImpl->m_WanderTarget) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
	}



#endif


	//--------------------------------------------------------------------------------------
	//	struct WallAvoidanceSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct WallAvoidanceSteering::Impl {
		vector<PLANE> m_PlaneVec;//回避すべき面の配列
		Impl()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	WallAvoidanceSteering（壁回避）行動
	//--------------------------------------------------------------------------------------
	WallAvoidanceSteering::WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	WallAvoidanceSteering::~WallAvoidanceSteering() {}

	//アクセサ
	vector<PLANE>& WallAvoidanceSteering::GetPlaneVec() const {
		return pImpl->m_PlaneVec;
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<PLANE>& planevec) {
		//必ずクリアする
		pImpl->m_PlaneVec.clear();
		for (auto& v : planevec) {
			pImpl->m_PlaneVec.push_back(v);
		}
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<bsm::Plane4>& planevec) {
		//必ずクリアする
		pImpl->m_PlaneVec.clear();
		for (auto& v : planevec) {
			PLANE p(v);
			pImpl->m_PlaneVec.push_back(p);
		}
	}


	bsm::Vec3 WallAvoidanceSteering::Execute(const bsm::Vec3& Force, const bsm::Vec3& Velocity) {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 TempForce = Force;
		bsm::Vec3 WorkForce(0, 0, 0);
		WorkForce = Steering::WallAvoidance(TransPtr->GetWorldMatrix(),
			Velocity, GetMaxSpeed(), pImpl->m_PlaneVec) * GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}



	//--------------------------------------------------------------------------------------
	//	struct ObstacleAvoidanceSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct ObstacleAvoidanceSteering::Impl {
		vector<SPHERE> m_ObstacleSphereVec;		//回避すべき障害物のSPHERE配列
		float m_RoadWidth;
		float m_RoadHeight;
		Impl() :
			m_RoadWidth(1.0f),
			m_RoadHeight(1.0f)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	 ObstacleAvoidanceSteering（障害物回避）行動
	//--------------------------------------------------------------------------------------
	ObstacleAvoidanceSteering::ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	ObstacleAvoidanceSteering::~ObstacleAvoidanceSteering() {}
	//アクセサ
	//障害物の配列
	const vector<SPHERE>& ObstacleAvoidanceSteering::GetObstacleSphereVec() const {
		return pImpl->m_ObstacleSphereVec;
	}
	void ObstacleAvoidanceSteering::SetObstacleSphereVec(const vector<SPHERE>& spherevec) {
		pImpl->m_ObstacleSphereVec.clear();
		for (auto sp : spherevec) {
			pImpl->m_ObstacleSphereVec.push_back(sp);
		}
	}

	//道の高さ
	float ObstacleAvoidanceSteering::GetRoadWidth() const {
		return pImpl->m_RoadWidth;
	}
	void ObstacleAvoidanceSteering::SetRoadWidth(float f) {
		pImpl->m_RoadWidth = f;
	}
	//道幅
	float ObstacleAvoidanceSteering::GetRoadHeight() const {
		return pImpl->m_RoadHeight;
	}
	void ObstacleAvoidanceSteering::SetRoadHeight(float f) {
		pImpl->m_RoadHeight = f;
	}

	bsm::Vec3 ObstacleAvoidanceSteering::Execute(const bsm::Vec3& Force, const bsm::Vec3& Velocity) {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 TempForce = Force;
		bsm::Vec3 WorkForce(0, 0, 0);
		WorkForce = Steering::ObstacleAvoidance(TransPtr->GetWorldMatrix(),
			Velocity, GetMaxSpeed(), pImpl->m_RoadWidth, pImpl->m_RoadHeight,
			pImpl->m_ObstacleSphereVec) * GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}




	//--------------------------------------------------------------------------------------
	//	struct FollowPathSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct FollowPathSteering::Impl {
		Path m_Path;	//経路をあらわすパス
		float m_Decl;	//減速値
		float m_WaypointSpan;	//経路の中心からの距離
		Impl() :
			m_Decl(3.0f),
			m_WaypointSpan(2.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	 FollowPathSteering（経路追従）行動
	//--------------------------------------------------------------------------------------
	FollowPathSteering::FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	FollowPathSteering::~FollowPathSteering() {}

	//アクセサ
	void FollowPathSteering::SetPathList(const list<bsm::Vec3>& pathlist) {
		pImpl->m_Path.SetList(pathlist);
	}

	float FollowPathSteering::GetDecl() const {
		return pImpl->m_Decl;
	}
	void FollowPathSteering::SetDecl(float f) {
		pImpl->m_Decl = f;
	}

	float FollowPathSteering::GetWaypointSpan() const {
		return pImpl->m_WaypointSpan;
	}
	void FollowPathSteering::SetWaypointSpan(float f) {
		pImpl->m_WaypointSpan = f;
	}

	bool FollowPathSteering::GetLooped() const {
		return pImpl->m_Path.GetLooped();
	}
	bool FollowPathSteering::IsLooped() const {
		return pImpl->m_Path.GetLooped();
	}
	void FollowPathSteering::SetLooped(bool b) {
		pImpl->m_Path.SetLooped(b);
	}

	bool FollowPathSteering::IsFinished() const {
		return pImpl->m_Path.IsFinished();
	}

	bsm::Vec3 FollowPathSteering::Execute(const bsm::Vec3& Force, const bsm::Vec3& Velocity) {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 TempForce = Force;
		if (IsFinished()) {
			//終了してたら、渡されたフォースをそのまま返す
			return TempForce;
		}
		bsm::Vec3 WorkForce(0, 0, 0);
		WorkForce = Steering::FollowPath(pImpl->m_Path,
			pImpl->m_WaypointSpan,
			TransPtr->GetWorldPosition(),
			Velocity,
			GetMaxSpeed(),
			pImpl->m_Decl)
			* GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}
#ifdef test

	//--------------------------------------------------------------------------------------
	//	struct AlignmentSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct AlignmentSteering::Impl {
		weak_ptr<GameObjectGroup> m_Group;
		Impl()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	 AlignmentSteering（整列）行動
	//--------------------------------------------------------------------------------------
	AlignmentSteering::AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	AlignmentSteering::~AlignmentSteering() {}

	//アクセサ
	shared_ptr<GameObjectGroup> AlignmentSteering::GetGameObjectGroup() const {
		auto shptr = pImpl->m_Group.lock();
		if (!shptr) {
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"AlignmentSteering::GetGameObjectGroup()"
			);
		}
		return shptr;
	}
	void AlignmentSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_Group = Group;
	}


	void AlignmentSteering::Execute() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Alignment(GetGameObjectGroup(), GetGameObject()) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
	}



	//--------------------------------------------------------------------------------------
	//	struct CohesionSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct CohesionSteering::Impl {
		weak_ptr<GameObjectGroup> m_Group;
		Impl()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	 CohesionSteering（結合）行動
	//--------------------------------------------------------------------------------------
	CohesionSteering::CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	CohesionSteering::~CohesionSteering() {}

	//アクセサ
	shared_ptr<GameObjectGroup> CohesionSteering::GetGameObjectGroup() const {
		auto shptr = pImpl->m_Group.lock();
		if (!shptr) {
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"CohesionSteering::GetGameObjectGroup()"
			);
		}
		return shptr;
	}
	void CohesionSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_Group = Group;
	}

	void CohesionSteering::Execute() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Cohesion(GetGameObjectGroup(), GetGameObject(),
			RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed()) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
	}


#endif




	//--------------------------------------------------------------------------------------
	//	struct SeparationSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct SeparationSteering::Impl {
		weak_ptr<GameObjectGroup> m_Group;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	 SeparationSteering（分離）行動クラス
	//--------------------------------------------------------------------------------------
	SeparationSteering::SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}

	SeparationSteering::~SeparationSteering() {}
	//アクセサ
	shared_ptr<GameObjectGroup> SeparationSteering::GetGameObjectGroup() const {
		auto shptr = pImpl->m_Group.lock();
		if (!shptr) {
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"SeparationSteering::GetGameObjectGroup()"
			);
		}
		return shptr;
	}
	void SeparationSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_Group = Group;
	}

	bsm::Vec3 SeparationSteering::Execute(const bsm::Vec3& Force) {
		bsm::Vec3 TempForce = Force;
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Separation(GetGameObjectGroup(), GetGameObject()) * GetWeight();
		Steering::AccumulateForce(TempForce, WorkForce, GetMaxForce());
		return TempForce;
	}

}
// end basecross