/*!
@file Behavior.cpp
@brief �s���N���X����
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
	///	�s�����[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	//�i�s�����������悤�ɂ���
	void UtilBehavior::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Velocity = PtrTransform->GetVelocity();
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
			//���݂ƖڕW����
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
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//��]�̍X�V
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
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
	///	�s���N���X�̐e�N���X
	//--------------------------------------------------------------------------------------
	Behavior::Behavior(const shared_ptr<GameObject>& GameObjectPtr) :
		pImpl(new Impl(GameObjectPtr))
	{}
	Behavior::~Behavior() {}
	shared_ptr<GameObject> Behavior::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
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
	///	Easing�s���N���X
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
		//�G���[�̏ꍇ�̓X�^�[�g��߂�
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
		//�G���[�̏ꍇ�̓X�^�[�g��߂�
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
		//�G���[�̏ꍇ�̓X�^�[�g��߂�
		return Start;
	}



}
//end basecross
