/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//OBB�Փ�j�����t����
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedBox");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

	}

	//--------------------------------------------------------------------------------------
	//	class FixedSphere : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedSphere::FixedSphere(const shared_ptr<Stage>& StagePtr,
		const float Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedSphere::~FixedSphere() {}

	//������
	void FixedSphere::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//CollisionSphere�Փ�j�����t����
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedSphere");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

	}


	//--------------------------------------------------------------------------------------
	//	class FixedCapsule : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedCapsule::FixedCapsule(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedCapsule::~FixedCapsule() {}

	//������
	void FixedCapsule::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//CAPSULE�Փ�j�����t����
		auto PtrColl = AddComponent<CollisionCapsule>();
		PtrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedCapsule");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

	}



	//--------------------------------------------------------------------------------------
	//	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f),
		m_Force(0),
		m_Velocity(0)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"SeekGroup");

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		//�d�͂�����
		auto PtrGra = AddComponent<Gravity>();
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}


	//����
	void SeekObject::OnUpdate() {
		m_Force = Vec3(0);
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(1.0f);
	}

	void SeekObject::OnUpdate2() {
	}

	Vec3 SeekObject::GetTargetPos()const {
		auto TargetPtr = GetStage()->GetSharedObject(L"Player");
		return TargetPtr->GetComponent<Transform>()->GetPosition();
	}


	void SeekObject::ApplyForce() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * ElapsedTime;
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		Pos += m_Velocity * ElapsedTime;
		PtrTransform->SetPosition(Pos);
	}



	//--------------------------------------------------------------------------------------
	//	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		auto PtrSep = Obj->GetBehavior<SeparationSteering>();
		auto Force = Obj->GetForce();
		Force = PtrSeek->Execute(Force, Obj->GetVelocity(), Obj->GetTargetPos());
		Force += PtrSep->Execute(Force);
		Obj->SetForce(Force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}

	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		auto PtrSep = Obj->GetBehavior<SeparationSteering>();
		auto Force = Obj->GetForce();
		Force = PtrArrive->Execute(Force, Obj->GetVelocity(), Obj->GetTargetPos());
		Force += PtrSep->Execute(Force);
		Obj->SetForce(Force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}



	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	MoveBox::~MoveBox() {}

	//������
	void MoveBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//OBB�Փ�j�����t����
		auto PtrColl = AddComponent<CollisionObb>();
		//�d�͂�����
		auto PtrGra = AddComponent<Gravity>();
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//�`�揈��
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetOwnShadowActive(true);
	}



}
//end basecross
