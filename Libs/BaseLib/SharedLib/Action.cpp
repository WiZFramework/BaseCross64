/*!
@file Action.cpp
@brief �A�N�V�����n�R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	struct ActionComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ActionComponent::Impl {
		bool m_Run;			//�ړ������ǂ���
		bool m_Arrived;	//�����������ǂ���
		float m_TotalTime;	//�ړ��ɂ����鎞��
		float m_NowTime;	//���݂̎���
		Impl() :
			m_Run(false),
			m_Arrived(false),
			m_TotalTime(0),
			m_NowTime(0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	�p�r: �A�N�V�����n�R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActionComponent::ActionComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	ActionComponent::~ActionComponent() {}

	//�A�N�Z�T
	void ActionComponent::SetRun(bool b) {
		pImpl->m_Run = b;
	}
	void  ActionComponent::SetArrived(bool b) {
		pImpl->m_Arrived = b;
	}


	bool ActionComponent::IsRun()const {
		return pImpl->m_Run;
	}
	bool ActionComponent::GetRun()const {
		return pImpl->m_Run;
	}

	bool ActionComponent::IsArrived()const {
		return pImpl->m_Arrived;
	}
	bool ActionComponent::GetArrived()const {
		return pImpl->m_Arrived;
	}

	float ActionComponent::GetTotalTime() const {
		return pImpl->m_TotalTime;
	}
	void ActionComponent::SetTotalTime(float f) {
		pImpl->m_TotalTime = f;
	}

	float ActionComponent::GetNowTime() const {
		return pImpl->m_NowTime;
	}
	void ActionComponent::SetNowTime(float f) {
		pImpl->m_NowTime = f;
	}

	//NowTime�����Z���āATotalTime�Ɣ�r����
	bool ActionComponent::AdditionalNowTime() {
		if (pImpl->m_Run) {
			if (pImpl->m_TotalTime > 0) {
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				pImpl->m_NowTime += ElapsedTime;
				if (pImpl->m_NowTime >= pImpl->m_TotalTime) {
					pImpl->m_NowTime = pImpl->m_TotalTime;
					pImpl->m_Run = false;
					//��������
					pImpl->m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}


	void ActionComponent::Stop() {
		pImpl->m_Run = false;
	}
	void ActionComponent::ReStart() {
		if (!pImpl->m_Arrived) {
			//�������ĂȂ���Γ��삳����
			pImpl->m_Run = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class ActionInterval : public  ActionComponent ;
	//	�p�r: �A�N�V�����̃C���^�[�o��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActionInterval::ActionInterval(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr)
	{}
	ActionInterval::~ActionInterval() {}

	void ActionInterval::SetParams(float TotalTime) {
		SetTotalTime(TotalTime);
	}

	void ActionInterval::Run() {
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
	}
	void ActionInterval::OnUpdate() {
		if (GetRun()) {
			if (!AdditionalNowTime()) {
				if (GetTotalTime() <= 0) {
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ScaleComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ScaleComponent::Impl {
		bsm::Vec3 m_StartScale;	//�J�n�{��
		bsm::Vec3 m_TargetScale;	//�ړI�{��
		Lerp::rate m_Rate;	//��ԕ��@
		Impl() :
			m_StartScale(1.0f, 1.0f, 1.0f),
			m_TargetScale(1.0f, 1.0f, 1.0f),
			m_Rate(Lerp::Linear)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleComponent : public Component ;
	//	�p�r: �g��k���A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ScaleComponent::ScaleComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ScaleComponent::~ScaleComponent() {}


	//�A�N�Z�T
	Lerp::rate ScaleComponent::GetRate() const { return pImpl->m_Rate; }
	void ScaleComponent::SetRate(const Lerp::rate r) { pImpl->m_Rate = r; }

	const bsm::Vec3& ScaleComponent::GetStartScale() const { return pImpl->m_StartScale; }
	void ScaleComponent::SetStartScale(const bsm::Vec3& StartPosition) { pImpl->m_StartScale = StartPosition; }
	void ScaleComponent::SetStartScale(float x, float y, float z) { pImpl->m_StartScale = bsm::Vec3(x, y, z); }

	const bsm::Vec3& ScaleComponent::GetTargetScale() const { return pImpl->m_TargetScale; }
	void ScaleComponent::SetTargetScale(const bsm::Vec3& TargetScale) { pImpl->m_TargetScale = TargetScale; }
	void ScaleComponent::SetTargetScale(float x, float y, float z) { pImpl->m_TargetScale = bsm::Vec3(x, y, z); }

	//����
	void ScaleComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		pImpl->m_StartScale = TransPtr->GetScale();
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
			TransPtr->SetScale(pImpl->m_TargetScale);
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
	}

	void ScaleComponent::OnUpdate() {
		if (GetRun()) {
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				bsm::Vec3 Scale = Lerp::CalculateLerp<bsm::Vec3>(
					pImpl->m_StartScale,
					pImpl->m_TargetScale,
					0,
					GetTotalTime(),
					GetNowTime(),
					pImpl->m_Rate
					);
				TransPtr->SetScale(Scale);
			}
			else {
				if (GetTotalTime() <= 0) {
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetScale(pImpl->m_TargetScale);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//���݃X�P�[�����v�Z���ĕԂ�
	//�^�C���̍X�V�͍s��Ȃ��̂�
	//Update��ɌĂԂׂ�
	bsm::Vec3 ScaleComponent::GetNowScale() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetScale;
		}
		bsm::Vec3 Scale = Lerp::CalculateLerp(
			pImpl->m_StartScale,
			pImpl->m_TargetScale,
			0,
			GetTotalTime(),
			GetNowTime(),
			pImpl->m_Rate
		);
		return Scale;
	}



	//--------------------------------------------------------------------------------------
	//	class ScaleTo : public  ScaleComponent ;
	//	�p�r: �ړI�̊g�嗦�Ɋg��k��
	//--------------------------------------------------------------------------------------
	ScaleTo::ScaleTo(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr) {}
	ScaleTo::~ScaleTo() {}
	void ScaleTo::SetParams(float TotalTime, const bsm::Vec3& TargetScale, Lerp::rate Rate) {
		SetTargetScale(TargetScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}

	//--------------------------------------------------------------------------------------
	//	struct ScaleBy::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ScaleBy::Impl {
		bsm::Vec3 m_LocalScale;	//���΃X�P�[��
	public:
		Impl() :
			m_LocalScale(1.0f, 1.0f, 1.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleBy : public  ScaleComponent;
	//	�p�r: �ړI�̑��΃X�P�[���ɂɊg��k��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ScaleBy::ScaleBy(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr), pImpl(new Impl()) {}
	ScaleBy::~ScaleBy() {}


	//�A�N�Z�T
	const bsm::Vec3& ScaleBy::GetLocalScale() const { return pImpl->m_LocalScale; }
	void ScaleBy::SetLocalScale(const bsm::Vec3& LocalScale) { pImpl->m_LocalScale = LocalScale; }
	void ScaleBy::SetLocalScale(float x, float y, float z) { pImpl->m_LocalScale = bsm::Vec3(x, y, z); }

	void ScaleBy::SetParams(float TotalTime, const bsm::Vec3& LocalScale, Lerp::rate Rate) {
		SetLocalScale(LocalScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}


	//����
	void ScaleBy::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Target = TransPtr->GetScale();
		Target.x *= pImpl->m_LocalScale.x;
		Target.y *= pImpl->m_LocalScale.y;
		Target.z *= pImpl->m_LocalScale.z;
		SetTargetScale(Target);
		ScaleComponent::Run();
	}



	//--------------------------------------------------------------------------------------
	//	struct RotateComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct RotateComponent::Impl {
		bsm::Quat m_StartQuaternion;	//�J�n��]
		bsm::Quat m_TargetQuaternion;	//�I����]
	public:
		Impl() :
			m_StartQuaternion(),
			m_TargetQuaternion()
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	//	class RotateComponent : public ActionComponent ;
	//	�p�r: ��]�A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RotateComponent::RotateComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{
	}
	RotateComponent::~RotateComponent() {}


	//�A�N�Z�T
	const bsm::Quat& RotateComponent::GetStartQuaternion() const { return pImpl->m_StartQuaternion; }
	void RotateComponent::SetStartQuaternion(const bsm::Quat& StartQuaternion) {
		pImpl->m_StartQuaternion = StartQuaternion;
		pImpl->m_StartQuaternion.normalize();
	}

	bsm::Vec3 RotateComponent::GetStartRotate() const {
		return pImpl->m_StartQuaternion.toRotVec();
	}
	void RotateComponent::SetStartRotate(const bsm::Vec3& StartRotate) {
		pImpl->m_StartQuaternion.rotationRollPitchYawFromVector(StartRotate);
		pImpl->m_StartQuaternion.normalize();
	}
	void RotateComponent::SetStartRotate(float x, float y, float z) {
		pImpl->m_StartQuaternion.rotationRollPitchYawFromVector(bsm::Vec3(x, y, z));
		pImpl->m_StartQuaternion.normalize();
	}

	const bsm::Quat& RotateComponent::GetTargetQuaternion() const {
		return pImpl->m_TargetQuaternion;
	}
	void RotateComponent::SetTargetQuaternion(const bsm::Quat& TargetQuaternion) {
		pImpl->m_TargetQuaternion = TargetQuaternion;
		pImpl->m_TargetQuaternion.normalize();
	}
	bsm::Vec3 RotateComponent::GetTargetRotate() const {
		return pImpl->m_TargetQuaternion.toRotVec();
	}
	void RotateComponent::SetTargetRotate(const bsm::Vec3& TargetRotate) {
		pImpl->m_TargetQuaternion.rotationRollPitchYawFromVector(TargetRotate);
		pImpl->m_TargetQuaternion.normalize();
	}
	void RotateComponent::SetTargetRotate(float x, float y, float z) {
		pImpl->m_TargetQuaternion.rotationRollPitchYawFromVector(bsm::Vec3(x, y, z));
		pImpl->m_TargetQuaternion.normalize();
	}

	//����
	void RotateComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		//�N�I�[�^�j�I�����猻�݂̉�]�𓾂�
		pImpl->m_StartQuaternion = TransPtr->GetQuaternion();
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
			TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
	}
	void RotateComponent::OnUpdate() {
		if (GetRun()) {
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				bsm::Quat Qt(XMQuaternionSlerp(
					pImpl->m_StartQuaternion,
					pImpl->m_TargetQuaternion,
					GetNowTime() / GetTotalTime()
				));
				Qt.normalize();
				TransPtr->SetQuaternion(Qt);
			}
			else {
				if (GetTotalTime() <= 0) {
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//���݉�]���v�Z���ĕԂ�
	//�^�C���̍X�V�͍s��Ȃ��̂�
	//Update��ɌĂԂׂ�
	bsm::Vec3 RotateComponent::GetNowRotate() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetQuaternion.toRotVec();
		}
		bsm::Quat Qt(XMQuaternionSlerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
		));
		Qt.normalize();
		return Qt.toRotVec();
	}
	bsm::Quat RotateComponent::GetNowQuaternion() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetQuaternion;
		}
		bsm::Quat Qt(XMQuaternionSlerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
		));
		Qt.normalize();
		return Qt;
	}



	//--------------------------------------------------------------------------------------
	//	class RotateTo : public  MoveComponent ;
	//	�p�r: �ړI�̊p�x�ɉ�]
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RotateTo::RotateTo(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr)
	{}
	RotateTo::~RotateTo() {}

	void RotateTo::SetParams(float TotalTime, const bsm::Vec3& TargetRotate) {
		SetTargetRotate(TargetRotate);
		SetTotalTime(TotalTime);
	}

	void RotateTo::SetParams(float TotalTime, const bsm::Quat& TargetQuaternion) {
		SetTargetQuaternion(TargetQuaternion);
		SetTotalTime(TotalTime);
	}


	//--------------------------------------------------------------------------------------
	//	struct RotateBy::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct RotateBy::Impl {
		bsm::Quat m_LocalQuaternion;	//���Ή�]
		Impl() :
			m_LocalQuaternion()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class RotateBy : public  RotateComponent ;
	//	�p�r: �ړI�̑��Ίp�x�ɉ�]
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RotateBy::RotateBy(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr), pImpl(new Impl()) {}
	RotateBy::~RotateBy() {}

	//�A�N�Z�T
	const bsm::Quat& RotateBy::GetLocalQuaternion() const { return pImpl->m_LocalQuaternion; }
	void RotateBy::SetLocalQuaternion(const bsm::Quat& LocalQuaternion) {
		pImpl->m_LocalQuaternion = LocalQuaternion;
		pImpl->m_LocalQuaternion.normalize();
	}
	bsm::Vec3 RotateBy::GetLocalRotate() const {
		return pImpl->m_LocalQuaternion.toRotVec();
	}
	void RotateBy::SetLocalRotate(const bsm::Vec3& LocalRotate) {
		pImpl->m_LocalQuaternion.rotationRollPitchYawFromVector(LocalRotate);
		pImpl->m_LocalQuaternion.normalize();
	}
	void RotateBy::SetLocalRotate(float x, float y, float z) {
		pImpl->m_LocalQuaternion.rotationRollPitchYawFromVector(bsm::Vec3(x, y, z));
		pImpl->m_LocalQuaternion.normalize();
	}

	void RotateBy::SetParams(float TotalTime, const bsm::Vec3& LocalRotate) {
		SetTotalTime(TotalTime);
		SetLocalRotate(LocalRotate);
	}

	void RotateBy::SetParams(float TotalTime, const bsm::Quat& LocalQuaternion) {
		SetTotalTime(TotalTime);
		SetLocalQuaternion(LocalQuaternion);
	}


	//����
	void RotateBy::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Quat QtNow = TransPtr->GetQuaternion();
		bsm::Quat Target = QtNow * pImpl->m_LocalQuaternion;
		SetTargetQuaternion(Target);
		RotateComponent::Run();
	}

	//--------------------------------------------------------------------------------------
	//	struct MoveComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct MoveComponent::Impl {
		bsm::Vec3 m_StartPosition;	//�J�n�n�_
		bsm::Vec3 m_TargetPosition;	//�ړI�n�_
		Lerp::rate m_RateX;	//��ԕ��@X
		Lerp::rate m_RateY;	//��ԕ��@Y
		Lerp::rate m_RateZ;	//��ԕ��@Z
		bsm::Vec3 m_Velocity;	//���݂̑��x
		Impl() :
			m_StartPosition(0, 0, 0),
			m_TargetPosition(0, 0, 0),
			m_RateX(Lerp::Linear),
			m_RateY(Lerp::Linear),
			m_RateZ(Lerp::Linear),
			m_Velocity(0, 0, 0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveComponent : public ActionComponent ;
	//	�p�r: �ړ��A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveComponent::MoveComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	MoveComponent::~MoveComponent() {}
	//�A�N�Z�T

	Lerp::rate MoveComponent::GetRateX() const {
		return pImpl->m_RateX;
	}
	Lerp::rate MoveComponent::GetRateY() const {
		return pImpl->m_RateY;
	}
	Lerp::rate MoveComponent::GetRateZ() const {
		return pImpl->m_RateZ;
	}
	void MoveComponent::SetRateX(const Lerp::rate r) {
		pImpl->m_RateX = r;
	}
	void MoveComponent::SetRateY(const Lerp::rate r) {
		pImpl->m_RateY = r;
	}
	void MoveComponent::SetRateZ(const Lerp::rate r) {
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate r) {
		pImpl->m_RateX = r;
		pImpl->m_RateY = r;
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate rX, const Lerp::rate rY, const Lerp::rate rZ) {
		pImpl->m_RateX = rX;
		pImpl->m_RateY = rY;
		pImpl->m_RateZ = rZ;
	}

	bsm::Vec3 MoveComponent::GetVelocity()const {
		bsm::Vec3 Ret = pImpl->m_Velocity;
		if (GetTotalTime() <= 0) {
			Ret.setAll(0.0f);
		}
		else {
			if (!GetRun() || GetArrived()) {
				Ret.setAll(0.0f);
			}
		}
		return Ret;
	}



	const bsm::Vec3& MoveComponent::GetStartPosition() const { return pImpl->m_StartPosition; }
	void MoveComponent::SetStartPosition(const bsm::Vec3& StartPosition) { pImpl->m_StartPosition = StartPosition; }
	void MoveComponent::SetStartPosition(float x, float y, float z) { pImpl->m_StartPosition = bsm::Vec3(x, y, z); }

	const bsm::Vec3& MoveComponent::GetTargetPosition() const { return pImpl->m_TargetPosition; }
	void MoveComponent::SetTargetPosition(const bsm::Vec3& TargetPosition) { pImpl->m_TargetPosition = TargetPosition; }
	void MoveComponent::SetTargetPosition(float x, float y, float z) { pImpl->m_TargetPosition = bsm::Vec3(x, y, z); }

	bsm::Vec3 MoveComponent::CalcVelocitySub(float NowTime) {
		bsm::Vec3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ) {
			Pos = Lerp::CalculateLerp<bsm::Vec3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
		}
		else {
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateZ
				);
		}
		return Pos;
	}


	void MoveComponent::CalcVelocity() {
		if (GetTotalTime() <= 0) {
			pImpl->m_Velocity = bsm::Vec3(0, 0, 0);
			return;
		}
		else {
			if (!GetRun() || GetArrived()) {
				pImpl->m_Velocity = bsm::Vec3(0, 0, 0);
				return;
			}
			else {
				float NowTime = GetNowTime();
				if (NowTime <= 0) {
					pImpl->m_Velocity = bsm::Vec3(0, 0, 0);
					return;
				}
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				if (ElapsedTime <= 0) {
					pImpl->m_Velocity = bsm::Vec3(0, 0, 0);
					return;
				}
				float BeforeTime = GetNowTime() - App::GetApp()->GetElapsedTime();
				if (BeforeTime <= 0) {
					BeforeTime = 0;
				}
				if ((NowTime - BeforeTime) <= 0) {
					pImpl->m_Velocity = bsm::Vec3(0, 0, 0);
					return;
				}
				bsm::Vec3 BeforePos = CalcVelocitySub(BeforeTime);
				bsm::Vec3 NowPos = CalcVelocitySub(NowTime);
				pImpl->m_Velocity = (NowPos - BeforePos) / ElapsedTime;
				return;
			}
		}
	}


	//����
	void MoveComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		pImpl->m_StartPosition = TransPtr->GetWorldPosition();
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
			TransPtr->SetWorldPosition(pImpl->m_TargetPosition);
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
		CalcVelocity();
	}

	void MoveComponent::OnUpdate() {
		if (GetRun()) {
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				bsm::Vec3 Pos;
				if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ) {
					Pos = Lerp::CalculateLerp<bsm::Vec3>(
						pImpl->m_StartPosition,
						pImpl->m_TargetPosition,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
				}
				else {
					Pos.x = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.x,
						pImpl->m_TargetPosition.x,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
					Pos.y = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.y,
						pImpl->m_TargetPosition.y,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateY
						);
					Pos.z = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.z,
						pImpl->m_TargetPosition.z,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateZ
						);
				}
				PtrTrans->SetWorldPosition(Pos);
			}
			else {
				if (GetTotalTime() <= 0) {
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					PtrTrans->SetWorldPosition(pImpl->m_TargetPosition);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
			CalcVelocity();
		}
	}

	//���݈ʒu���v�Z���ĕԂ�
	//�^�C���̍X�V�͍s��Ȃ��̂�
	//Update��ɌĂԂׂ�
	bsm::Vec3 MoveComponent::GetNowPosition() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetPosition;
		}
		bsm::Vec3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ) {
			Pos = Lerp::CalculateLerp<bsm::Vec3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
		}
		else {
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateZ
				);
		}
		return Pos;
	}



	//--------------------------------------------------------------------------------------
	//	class MoveTo : public  MoveComponent ;
	//	�p�r: �ړI�̈ʒu�Ɉړ�
	//--------------------------------------------------------------------------------------
	MoveTo::MoveTo(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr) {}
	MoveTo::~MoveTo() {}

	void MoveTo::SetParams(float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate Rate) {
		SetTargetPosition(TargetPosition);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveTo::SetParams(float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		SetTargetPosition(TargetPosition);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}



	//--------------------------------------------------------------------------------------
	//	struct MoveBy::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct MoveBy::Impl {
		bsm::Vec3 m_LocalVector;	//���Ε���
		Impl() :
			m_LocalVector(0, 0, 0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBy : public  MoveComponent ;
	//	�p�r: �ړI�̑��Έʒu�Ɉړ�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBy::MoveBy(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr), pImpl(new Impl()) {}
	MoveBy::~MoveBy() {}

	//�A�N�Z�T
	const bsm::Vec3& MoveBy::GetLocalVector() const { return pImpl->m_LocalVector; }
	void MoveBy::SetLocalVector(const bsm::Vec3& LocalVector) { pImpl->m_LocalVector = LocalVector; }
	void MoveBy::SetLocalVector(float x, float y, float z) { pImpl->m_LocalVector = bsm::Vec3(x, y, z); }

	void MoveBy::SetParams(float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate Rate) {
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveBy::SetParams(float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}

	//����
	void MoveBy::Run() {
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Pos = PtrTrans->GetWorldPosition();
		bsm::Vec3 Target = Pos + pImpl->m_LocalVector;
		SetTargetPosition(Target);
		MoveComponent::Run();
	}



	//--------------------------------------------------------------------------------------
	//	struct Action::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Action::Impl {
		bool m_Looped;

		bool m_ScaleArrived;	//m_Loop��false�̂Ƃ��̂ݗL��
		bool m_RotateArrived;	//m_Loop��false�̂Ƃ��̂ݗL��
		bool m_MoveArrived;	//m_Loop��false�̂Ƃ��̂ݗL��


		vector<shared_ptr<ActionComponent> > m_ScaleVec;
		vector<shared_ptr<ActionComponent> > m_RotateVec;
		vector<shared_ptr<ActionComponent> > m_MoveVec;

		size_t m_ScaleActiveIndex;
		size_t m_RotateActiveIndex;
		size_t m_MoveActiveIndex;

		Impl() :
			m_Looped(false),
			m_ScaleArrived(false),
			m_RotateArrived(false),
			m_MoveArrived(false),
			m_ScaleActiveIndex(0),
			m_RotateActiveIndex(0),
			m_MoveActiveIndex(0)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	�p�r: ������ԕύX�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Action::Action(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Action::~Action() {}

	//�A�N�Z�T
	bool Action::IsLooped()const { return pImpl->m_Looped; }
	bool Action::GetLooped()const { return pImpl->m_Looped; }
	void Action::SetLooped(bool b) { pImpl->m_Looped = b; }

	bool Action::IsArrived()const {
		bool retScale = true;
		if (pImpl->m_ScaleVec.size() > 0) {
			//�A�N�V���������邪�I�����ĂȂ�
			if (!pImpl->m_ScaleArrived) {
				retScale = false;
			}
		}
		bool retRot = true;
		if (pImpl->m_RotateVec.size() > 0) {
			//�A�N�V���������邪�I�����ĂȂ�
			if (!pImpl->m_RotateArrived) {
				retRot = false;
			}
		}
		bool retMove = true;
		if (pImpl->m_MoveVec.size() > 0) {
			//�A�N�V���������邪�I�����ĂȂ�
			if (!pImpl->m_MoveArrived) {
				retMove = false;
			}
		}
		return
			retScale &&
			retRot &&
			retMove;
	}
	bool Action::GetArrived()const { return IsArrived(); }


	size_t Action::GetScaleActiveIndex() const {
		if (pImpl->m_ScaleVec.size() <= 0) {
			throw BaseException(
				L"�X�P�[���A�N�V�������o�^����Ă��܂���",
				L"if (pImpl->m_ScaleVec.size() <= 0)",
				L"Action::GetScaleActiveIndex()"
			);
		}
		return pImpl->m_ScaleActiveIndex;
	}
	size_t Action::GetRotateActiveIndex() const {
		if (pImpl->m_RotateVec.size() <= 0) {
			throw BaseException(
				L"��]�A�N�V�������o�^����Ă��܂���",
				L"if (pImpl->m_RotateVec.size() <= 0)",
				L"Action::GetRotateActiveIndex()"
			);
		}
		return pImpl->m_RotateActiveIndex;
	}
	size_t Action::GetMoveActiveIndex() const {
		if (pImpl->m_MoveVec.size() <= 0) {
			throw BaseException(
				L"�ړ��A�N�V�������o�^����Ă��܂���",
				L"if (pImpl->m_MoveVec.size() <= 0)",
				L"Action::GetMoveActiveIndex()"
			);
		}
		return pImpl->m_MoveActiveIndex;
	}

	//���݂̑��x
	//Move�R���|�[�l���g���Ȃ��ꍇ��bsm::Vec3(0,0,0)��Ԃ�

	bsm::Vec3 Action::GetVelocity()const {
		if (pImpl->m_MoveVec.size() <= 0) {
			return bsm::Vec3(0, 0, 0);
		}
		else {
			auto Ptr = dynamic_pointer_cast<MoveComponent>(pImpl->m_MoveVec[pImpl->m_MoveActiveIndex]);
			if (Ptr) {
				return Ptr->GetVelocity();
			}
			else {
				return bsm::Vec3(0, 0, 0);
			}
		}
	}



	//����
	shared_ptr<ScaleTo> Action::AddScaleTo(float TotalTime, const bsm::Vec3& TargetScale, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<ScaleTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}
	shared_ptr<ScaleBy> Action::AddScaleBy(float TotalTime, const bsm::Vec3& LocalScale, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<ScaleBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionInterval> Action::AddScaleInterval(float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetScaleComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_ScaleVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				L"if (TargetIndex >= pImpl->m_ScaleVec.size())",
				L"Action::GetScaleComponent()"
			);
		}
		return pImpl->m_ScaleVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetScaleVec() const {
		return pImpl->m_ScaleVec;
	}


	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const bsm::Vec3& TargetRotate) {
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const bsm::Quat& TargetQuaternion) {
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const bsm::Vec3& LocalRotate) {
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const bsm::Quat& LocalQuaternion) {
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddRotateInterval(float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetRotateComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_RotateVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				L"if (TargetIndex >= pImpl->m_RotateVec.size())",
				L"Action::GetRotateComponent()"
			);
		}
		return pImpl->m_RotateVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetRotateVec() const {
		return pImpl->m_RotateVec;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddMoveInterval(float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetMoveComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_MoveVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				L"if (TargetIndex >= pImpl->m_MoveVec.size())",
				L"Action::GetMoveComponent()"
			);
		}
		return pImpl->m_MoveVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetMoveVec() const {
		return pImpl->m_MoveVec;
	}
	//���ׂẴA�N�V�������N���A����
	void Action::AllActionClear() {
		pImpl->m_ScaleVec.clear();
		pImpl->m_RotateVec.clear();
		pImpl->m_MoveVec.clear();

		pImpl->m_Looped = false;
		pImpl->m_ScaleArrived = false;
		pImpl->m_RotateArrived = false;
		pImpl->m_MoveArrived = false;
		pImpl->m_ScaleActiveIndex = 0;
		pImpl->m_RotateActiveIndex = 0;
		pImpl->m_MoveActiveIndex = 0;
	}



	void Action::RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0) {
			TgtIndex = 0;
			TgtVector[TgtIndex]->Run();
		}
	}
	void Action::Run() {
		RunSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		RunSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		RunSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			TgtVector[TgtIndex]->Stop();
		}
	}
	void Action::Stop() {
		StopSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		StopSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		StopSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			TgtVector[TgtIndex]->ReStart();
		}
	}

	void Action::ReStart() {
		ReStartSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		ReStartSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		ReStartSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	bool Action::UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		bool ret = false;
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			if (TgtVector[TgtIndex]->IsUpdateActive()) {
				TgtVector[TgtIndex]->OnUpdate();
				if (TgtVector[TgtIndex]->IsArrived()) {
					//��������
					//�C���f�b�N�X�����Z
					TgtIndex++;
					if (TgtIndex >= TgtVector.size()) {
						//�͈͊O�Ȃ�
						if (pImpl->m_Looped) {
							TgtIndex = 0;
							TgtVector[TgtIndex]->Run();
						}
						else {
							TgtIndex = TgtVector.size() - 1;
							//Run�͍s��Ȃ�
							//��������
							ret = true;
						}
					}
					else {
						TgtVector[TgtIndex]->Run();
					}
				}
			}
		}
		return ret;
	}
	//�X�V
	void Action::OnUpdate() {
		pImpl->m_ScaleArrived = UpdateSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		pImpl->m_RotateArrived = UpdateSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		pImpl->m_MoveArrived = UpdateSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

}
//