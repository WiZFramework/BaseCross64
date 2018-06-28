/*!
@file Behavior.cpp
@brief 行動クラス実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Behavior::Impl;
	//--------------------------------------------------------------------------------------
	struct Behavior::Impl {
		weak_ptr<GameObject> m_GameObject;
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	行動ユーティリティクラス
	//--------------------------------------------------------------------------------------
	//進行方向を向くようにする
	void UtilBehavior::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Velocity = PtrTransform->GetVelocity();
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	void UtilBehavior::RotToHead(const bsm::Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//回転の更新
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}


	//--------------------------------------------------------------------------------------
	///	行動クラスの親クラス
	//--------------------------------------------------------------------------------------
	Behavior::Behavior(const shared_ptr<GameObject>& GameObjectPtr) :
		pImpl(new Impl(GameObjectPtr))
	{}
	Behavior::~Behavior() {}
	shared_ptr<GameObject> Behavior::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (!shptr)",
				L"Behavior::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}
	shared_ptr<Stage> Behavior::GetStage() const {
		return GetGameObject()->GetStage();
	}


	//--------------------------------------------------------------------------------------
	//	struct Velocity::Impl;
	//--------------------------------------------------------------------------------------
	struct Velocity::Impl {
		bsm::Vec3 m_Velocity;
		bsm::Vec3 m_Force;
		float m_Mass;
	public:
		Impl() :
			m_Velocity(0),
			m_Force(0),
			m_Mass(1.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	Velocity行動クラス
	//--------------------------------------------------------------------------------------
	Velocity::Velocity(const shared_ptr<GameObject>& GameObjectPtr):
		Behavior(GameObjectPtr),
		pImpl(new Impl())
	{

	}
	Velocity::~Velocity() {}
	float Velocity::GetMass() const {
		return pImpl->m_Mass;
	}
	void Velocity::SetMass(float m) {
		pImpl->m_Mass = m;
	}
	const bsm::Vec3& Velocity::GetVelocity() const {
		return pImpl->m_Velocity;
	}
	void Velocity::SetVelocity(const bsm::Vec3& velo) {
		pImpl->m_Velocity = velo;
	}
	void Velocity::InitForce() {
		pImpl->m_Force = Vec3(0.0f);
	}
	const bsm::Vec3& Velocity::GetForce() const {
		return pImpl->m_Force;
	}
	void Velocity::AplayForce(const bsm::Vec3& force) {
		pImpl->m_Force += force;
	}
	bsm::Vec3 Velocity::UpdateFromTime(const bsm::Vec3& Pos, float CalcTime) {
		bsm::Vec3 Ret = Pos;
		auto ac = pImpl->m_Force / pImpl->m_Mass;
		pImpl->m_Velocity += ac * CalcTime;
		Ret += pImpl->m_Velocity * CalcTime;
		return Ret;
	}
	bsm::Vec3 Velocity::Execute(const bsm::Vec3& Pos) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		return UpdateFromTime(Pos,ElapsedTime);
	}
	void Velocity::UpdateFromTimeTrans(float CalcTime) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = PtrTransform->GetWorldPosition();
		Pos = UpdateFromTime(Pos, CalcTime);
		PtrTransform->SetWorldPosition(Pos);
	}
	void Velocity::ExecuteTrans() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTimeTrans(ElapsedTime);
	}

	//--------------------------------------------------------------------------------------
	//	struct Gravity::Impl;
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl {
		//現在の重力加速度
		bsm::Vec3 m_Gravity;
		//現在の重力による速度
		bsm::Vec3 m_GravityVelocity;

	public:
		Impl() :
			m_Gravity(0, -9.8f, 0),
			m_GravityVelocity(0.0f)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	Gravity行動クラス
	//--------------------------------------------------------------------------------------
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
		Behavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	Gravity::~Gravity() {}

	const bsm::Vec3& Gravity::GetGravity() const {
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const bsm::Vec3& gravity) {
		pImpl->m_Gravity = gravity;
	}

	void Gravity::StartJump(const bsm::Vec3& StartVec) {
		pImpl->m_GravityVelocity = StartVec;
	}


	void Gravity::StartJumpTrans(const bsm::Vec3& StartVec,float EscapeSpan) {
		pImpl->m_GravityVelocity = StartVec;
		bsm::Vec3 EscapeVec = StartVec;
		//ジャンプして親オブジェクトボリュームから脱出できないとき対応
		EscapeVec *= EscapeSpan;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = PtrTransform->GetWorldPosition();
		Pos += EscapeVec;
		PtrTransform->ResetWorldPosition(Pos);
	}

	bsm::Vec3 Gravity::UpdateFromTime(float CalcTime) {
		pImpl->m_GravityVelocity += pImpl->m_Gravity * CalcTime;
		return pImpl->m_GravityVelocity;
	}

	bsm::Vec3 Gravity::Execute() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		return UpdateFromTime(ElapsedTime);
	}

	void Gravity::UpdateFromTimeTrans(float CalcTime) {
		pImpl->m_GravityVelocity += pImpl->m_Gravity * CalcTime;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = PtrTransform->GetWorldPosition();
		Pos += pImpl->m_GravityVelocity * CalcTime;
		PtrTransform->SetWorldPosition(Pos);
	}
	void Gravity::ExecuteTrans() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTimeTrans(ElapsedTime);
	}


	bsm::Vec3 Gravity::GetGravityVelocity() const {
		return pImpl->m_GravityVelocity;
	}
	void Gravity::SetGravityVelocityZero() {
		SetGravityVelocity(bsm::Vec3(0.0f));
	}
	void Gravity::SetGravityVelocity(const bsm::Vec3& velo) {
		pImpl->m_GravityVelocity = velo;
	}




}
//end basecross
