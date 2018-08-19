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
	///	Easing行動クラス
	//--------------------------------------------------------------------------------------
	EasingBehavior::EasingBehavior(const shared_ptr<GameObject>& GameObjectPtr):
		Behavior(GameObjectPtr)
	{}
	EasingBehavior::~EasingBehavior() {}

	bsm::Vec3 EasingBehavior::Linear(const bsm::Vec3& Start, const bsm::Vec3& End, float TgtTime, float AllTime) {
		auto SpanVec = End - Start;
		return EasingBase<bsm::Vec3>::Linear(TgtTime, Start, SpanVec, AllTime);
	}

	bsm::Vec3 EasingBehavior::EaseIn(EasingType type, 
		const bsm::Vec3& Start, const bsm::Vec3& End, 
		float TgtTime, float AllTime)
	{
		auto SpanVec = End - Start;
		switch (type) {
			case EasingType::Quadratic: 
			{
				EaseQuad<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic: 
			{
				EaseCubic<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<Vec3> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
		}
		//エラーの場合はスタートを戻す
		return Start;
	}

	bsm::Vec3 EasingBehavior::EaseOut(EasingType type,
		const bsm::Vec3& Start, const bsm::Vec3& End,
		float TgtTime, float AllTime)
	{
		auto SpanVec = End - Start;
		switch (type) {
		case EasingType::Quadratic:
		{
			EaseQuad<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Cubic:
		{
			EaseCubic<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Quartic:
		{
			EaseQuart<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Quintic:
		{
			EaseQuint<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Sinusoidal:
		{
			EaseSin<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);

		}
		break;
		case EasingType::Exponential:
		{
			EaseExpo<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);

		}
		break;
		case EasingType::Circular:
		{
			EaseCirc<Vec3> es;
			return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		}
		//エラーの場合はスタートを戻す
		return Start;
	}

	bsm::Vec3 EasingBehavior::EaseInOut(EasingType type,
		const bsm::Vec3& Start, const bsm::Vec3& End,
		float TgtTime, float AllTime)
	{
		auto SpanVec = End - Start;
		switch (type) {
		case EasingType::Quadratic:
		{
			EaseQuad<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Cubic:
		{
			EaseCubic<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Quartic:
		{
			EaseQuart<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Quintic:
		{
			EaseQuint<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		case EasingType::Sinusoidal:
		{
			EaseSin<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);

		}
		break;
		case EasingType::Exponential:
		{
			EaseExpo<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);

		}
		break;
		case EasingType::Circular:
		{
			EaseCirc<Vec3> es;
			return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
		}
		break;
		}
		//エラーの場合はスタートを戻す
		return Start;
	}



}
//end basecross
