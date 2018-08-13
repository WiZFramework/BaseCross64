/*!
@file BehaviorSteering.cpp
@brief ���Ǎs���N���X����
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
	///	���Ǎs���̐e�N���X
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
	///	SeekSteering�s���N���X
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
	///	ArriveSteering�s���N���X
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

#ifdef test
	//--------------------------------------------------------------------------------------
	///	PursuitSteering�i�ǐՁj�s���N���X
	//--------------------------------------------------------------------------------------

	PursuitSteering::PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr)
	{}
	PursuitSteering::~PursuitSteering() {}

	float PursuitSteering::Execute(const bsm::Vec3& TargetPos, const bsm::Vec3& TargetVelocity, const bsm::Vec3& TargetRotation) {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Pursuit(RigidPtr->GetVelocity(), TransPtr->GetWorldPosition(), TransPtr->GetRotation(), RigidPtr->GetMaxSpeed(),
			TargetVelocity, TargetPos, TargetRotation) *  GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
		auto Pos = TransPtr->GetWorldPosition();
		return bsm::length(Pos - TargetPos);

	}

	float PursuitSteering::Execute(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetTrans = TargetPtr->GetComponent<Transform>();
		return Execute(TargetTrans->GetWorldPosition(), TargetTrans->GetVelocity(), TargetTrans->GetRotation());
	}


	//--------------------------------------------------------------------------------------
	//	struct WanderSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct WanderSteering::Impl {
		bsm::Vec3 m_WanderTarget;	//�p�j�ڕW�̉�]�W���i���ǂɂ���ď�����������j
		float m_WanderRadius;	//�p�j���a
		float m_WanderDistance;	//�p�j�~�܂ł̋���
		float m_WanderJitter;	//�����_���ψق̍ő�l
		Impl() :
			m_WanderTarget(0, 0, 0),
			m_WanderRadius(1.5f),
			m_WanderDistance(1.0f),
			m_WanderJitter(0.5f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	WanderSteering�i�p�j�j�s��
	//--------------------------------------------------------------------------------------
	WanderSteering::WanderSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	WanderSteering::~WanderSteering() {}
	//�A�N�Z�T
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

	//--------------------------------------------------------------------------------------
	//	struct WallAvoidanceSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct WallAvoidanceSteering::Impl {
		vector<PLANE> m_PlaneVec;//������ׂ��ʂ̔z��
		Impl()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	WallAvoidanceSteering�i�ǉ���j�s��
	//--------------------------------------------------------------------------------------
	WallAvoidanceSteering::WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	WallAvoidanceSteering::~WallAvoidanceSteering() {}

	//�A�N�Z�T
	vector<PLANE>& WallAvoidanceSteering::GetPlaneVec() const {
		return pImpl->m_PlaneVec;
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<PLANE>& planevec) {
		//�K���N���A����
		pImpl->m_PlaneVec.clear();
		for (auto& v : planevec) {
			pImpl->m_PlaneVec.push_back(v);
		}
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<bsm::Plane4>& planevec) {
		//�K���N���A����
		pImpl->m_PlaneVec.clear();
		for (auto& v : planevec) {
			PLANE p(v);
			pImpl->m_PlaneVec.push_back(p);
		}
	}


	bool WallAvoidanceSteering::Execute() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce(0, 0, 0);
		WorkForce = Steering::WallAvoidance(TransPtr->GetWorldMatrix(),
			RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(), pImpl->m_PlaneVec) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
		if (WorkForce.length() > 0.0f) {
			//�ǂɏՓ˂��Ă���
			return true;
		}
		return false;
	}


	//--------------------------------------------------------------------------------------
	//	struct ObstacleAvoidanceSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct ObstacleAvoidanceSteering::Impl {
		vector<SPHERE> m_ObstacleSphereVec;		//������ׂ���Q����SPHERE�z��
		float m_RoadWidth;
		float m_RoadHeight;
		Impl() :
			m_RoadWidth(1.0f),
			m_RoadHeight(1.0f)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	 ObstacleAvoidanceSteering�i��Q������j�s��
	//--------------------------------------------------------------------------------------
	ObstacleAvoidanceSteering::ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	ObstacleAvoidanceSteering::~ObstacleAvoidanceSteering() {}
	//�A�N�Z�T
	//��Q���̔z��
	const vector<SPHERE>& ObstacleAvoidanceSteering::GetObstacleSphereVec() const {
		return pImpl->m_ObstacleSphereVec;
	}
	void ObstacleAvoidanceSteering::SetObstacleSphereVec(const vector<SPHERE>& spherevec) {
		pImpl->m_ObstacleSphereVec.clear();
		for (auto sp : spherevec) {
			pImpl->m_ObstacleSphereVec.push_back(sp);
		}
	}

	//���̍���
	float ObstacleAvoidanceSteering::GetRoadWidth() const {
		return pImpl->m_RoadWidth;
	}
	void ObstacleAvoidanceSteering::SetRoadWidth(float f) {
		pImpl->m_RoadWidth = f;
	}
	//����
	float ObstacleAvoidanceSteering::GetRoadHeight() const {
		return pImpl->m_RoadHeight;
	}
	void ObstacleAvoidanceSteering::SetRoadHeight(float f) {
		pImpl->m_RoadHeight = f;
	}


	void ObstacleAvoidanceSteering::Execute() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce(0, 0, 0);
		WorkForce = Steering::ObstacleAvoidance(TransPtr->GetWorldMatrix(),
			RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(), pImpl->m_RoadWidth, pImpl->m_RoadHeight,
			pImpl->m_ObstacleSphereVec) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
	}



	//--------------------------------------------------------------------------------------
	//	struct FollowPathSteering::Impl;
	//--------------------------------------------------------------------------------------
	struct FollowPathSteering::Impl {
		Path m_Path;	//�o�H������킷�p�X
		float m_Decl;	//�����l
		float m_WaypointSpan;	//�o�H�̒��S����̋���
		Impl() :
			m_Decl(3.0f),
			m_WaypointSpan(2.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	 FollowPathSteering�i�o�H�Ǐ]�j�s��
	//--------------------------------------------------------------------------------------
	FollowPathSteering::FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	FollowPathSteering::~FollowPathSteering() {}

	//�A�N�Z�T
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

	bool FollowPathSteering::Execute() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Force = RigidPtr->GetForce();
		bsm::Vec3 WorkForce(0, 0, 0);
		WorkForce = Steering::FollowPath(pImpl->m_Path,
			pImpl->m_WaypointSpan,
			TransPtr->GetWorldPosition(),
			RigidPtr->GetVelocity(),
			RigidPtr->GetMaxSpeed(),
			pImpl->m_Decl)
			* GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);
		return IsFinished();
	}


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
	///	 AlignmentSteering�i����j�s��
	//--------------------------------------------------------------------------------------
	AlignmentSteering::AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	AlignmentSteering::~AlignmentSteering() {}

	//�A�N�Z�T
	shared_ptr<GameObjectGroup> AlignmentSteering::GetGameObjectGroup() const {
		auto shptr = pImpl->m_Group.lock();
		if (!shptr) {
			throw BaseException(
				L"�O���[�v�͖����ł�",
				L"�O���[�v�̑��݂��m�F���ĉ�����",
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
	///	 CohesionSteering�i�����j�s��
	//--------------------------------------------------------------------------------------
	CohesionSteering::CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	CohesionSteering::~CohesionSteering() {}

	//�A�N�Z�T
	shared_ptr<GameObjectGroup> CohesionSteering::GetGameObjectGroup() const {
		auto shptr = pImpl->m_Group.lock();
		if (!shptr) {
			throw BaseException(
				L"�O���[�v�͖����ł�",
				L"�O���[�v�̑��݂��m�F���ĉ�����",
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
	///	 SeparationSteering�i�����j�s���N���X
	//--------------------------------------------------------------------------------------
	SeparationSteering::SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr),
		pImpl(new Impl())
	{}

	SeparationSteering::~SeparationSteering() {}
	//�A�N�Z�T
	shared_ptr<GameObjectGroup> SeparationSteering::GetGameObjectGroup() const {
		auto shptr = pImpl->m_Group.lock();
		if (!shptr) {
			throw BaseException(
				L"�O���[�v�͖����ł�",
				L"�O���[�v�̑��݂��m�F���ĉ�����",
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