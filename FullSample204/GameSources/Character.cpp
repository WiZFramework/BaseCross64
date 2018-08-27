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
	//	��Q����
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
		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//Capsule�Փ�j�����t����
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
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

	}


	//--------------------------------------------------------------------------------------
	//	�z�u�����I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BaseChara::BaseChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f),
		m_Force(0),
		m_Velocity(0)
	{
	}
	BaseChara::~BaseChara() {}

	void BaseChara::ApplyForce() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * ElapsedTime;
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		Pos += m_Velocity * ElapsedTime;
		PtrTransform->SetPosition(Pos);
	}

	shared_ptr<GameObject>  BaseChara::GetTarget()const {
		return GetStage()->GetSharedObject(L"Player");
	}

	//������
	void BaseChara::OnCreate() {
		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<BaseChara>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//�ǉ���s����t����
		auto PtrWall = GetBehavior<WallAvoidanceSteering>();
		vector<PLANE> PlaneVec = {
			{
				Vec3(20,0,0),
				Vec3(20,1.0,0),
				Vec3(20,0,-1.0),
			},
			{
				Vec3(0,0,-20),
				Vec3(0,1.0,-20),
				Vec3(-1.0,0,-20),
			},
			{
				Vec3(-20,0,0),
				Vec3(-20,1.0,0),
				Vec3(-20,0,1.0),
			},
			{
				Vec3(0,0,20),
				Vec3(0,1.0,20),
				Vec3(1.0,0,20),
			},
		};
		PtrWall->SetPlaneVec(PlaneVec);
		//��Q������s����t����
		vector<shared_ptr<GameObject>> SpObjVec;
		GetStage()->GetUsedTagObjectVec(L"FixedSphere", SpObjVec);
		vector<SPHERE> SpVec;
		for (auto& v : SpObjVec) {
			auto TransPtr = v->GetComponent<Transform>();
			SPHERE sp;
			sp.m_Center = TransPtr->GetPosition();
			sp.m_Radius = TransPtr->GetScale().x * 0.5f;
			SpVec.push_back(sp);
		}
		auto PtrAvoidance = GetBehavior<ObstacleAvoidanceSteering>();
		PtrAvoidance->SetObstacleSphereVec(SpVec);
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<BaseChara>(GetThis<BaseChara>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}
	//�X�V
	void BaseChara::OnUpdate() {
		m_Force = Vec3(0);
		//���ʂ̃X�e�A�����O1
		auto PtrWall = GetBehavior<WallAvoidanceSteering>();
		m_Force += PtrWall->Execute(m_Force, GetVelocity());
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
		//���ʂ̃X�e�A�����O2
		auto PtrSep = GetBehavior<SeparationSteering>();
		m_Force += PtrSep->Execute(m_Force);
		auto PtrAvoidance = GetBehavior<ObstacleAvoidanceSteering>();
		m_Force += PtrAvoidance->Execute(m_Force, GetVelocity());
		ApplyForce();
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(1.0f);
	}





	//--------------------------------------------------------------------------------------
	//	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<BaseChara>& Obj) {
	}
	void FarState::Execute(const shared_ptr<BaseChara>& Obj) {
		Obj->FarBehavior();
	}

	void FarState::Exit(const shared_ptr<BaseChara>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<BaseChara>& Obj) {
	}
	void NearState::Execute(const shared_ptr<BaseChara>& Obj) {
		Obj->NearBehavior();
	}
	void NearState::Exit(const shared_ptr<BaseChara>& Obj) {
	}





	//--------------------------------------------------------------------------------------
	//	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		BaseChara(StagePtr,StartPos)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(GetStartPos());
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"ObjGroup");

		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		//�d�͂�����
		auto PtrGra = AddComponent<Gravity>();
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);
		//�e�N���X��OnCreate���Ă�
		BaseChara::OnCreate();
	}


	void SeekObject::NearBehavior() {
		auto PtrArrive = GetBehavior<ArriveSteering>();
		auto PtrTrans = GetComponent<Transform>();
		auto PtrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto Force = GetForce();
		Force += PtrArrive->Execute(Force, GetVelocity(), PtrPlayerTrans->GetPosition());
		SetForce(Force);
		float f = bsm::length(PtrPlayerTrans->GetPosition() - PtrTrans->GetPosition());
		if (f > GetStateChangeSize()) {
			GetStateMachine()->ChangeState(FarState::Instance());
		}

	}
	void SeekObject::FarBehavior() {
		auto PtrSeek = GetBehavior<SeekSteering>();
		auto PtrTrans = GetComponent<Transform>();
		auto PtrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto Force = GetForce();
		Force += PtrSeek->Execute(Force, GetVelocity(), PtrPlayerTrans->GetPosition());
		SetForce(Force);
		float f = bsm::length(PtrPlayerTrans->GetPosition() - PtrTrans->GetPosition());
		if (f <= GetStateChangeSize()) {
			GetStateMachine()->ChangeState(NearState::Instance());
		}
	}


	//--------------------------------------------------------------------------------------
	//	�ǐՂ���z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PursuitObject::PursuitObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		BaseChara(StagePtr, StartPos)
	{
	}
	PursuitObject::~PursuitObject() {}
	//������
	void PursuitObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(GetStartPos());
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//CollisionSphere�Փ˔����t����
		auto PtrColl = AddComponent<CollisionSphere>();
		//�d�͂�����
		auto PtrGra = AddComponent<Gravity>();
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetFogEnabled(true);
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		SetAlphaActive(true);
		//�e�N���X��OnCreate���Ă�
		BaseChara::OnCreate();

	}

	//����
	void PursuitObject::NearBehavior() {
		auto PtrArrive = GetBehavior<ArriveSteering>();
		auto PtrTrans = GetComponent<Transform>();
		auto PtrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto Force = GetForce();
		Force += PtrArrive->Execute(Force, GetVelocity(), PtrPlayerTrans->GetPosition());
		SetForce(Force);
		float f = bsm::length(PtrPlayerTrans->GetPosition() - PtrTrans->GetPosition());
		if (f > GetStateChangeSize()) {
			GetStateMachine()->ChangeState(FarState::Instance());
		}

	}
	void PursuitObject::FarBehavior() {
		auto PtrPursuit = GetBehavior<PursuitSteering>();
		PtrPursuit->SetWeight(1.5f);
		auto PtrTrans = GetComponent<Transform>();
		auto PtrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto Force = GetForce();
		Force += PtrPursuit->Execute(Force, GetVelocity(), PtrPlayerTrans->GetPosition(),
			PtrPlayerTrans->GetVelocity(), PtrPlayerTrans->GetRotation());
		SetForce(Force);
		float f = bsm::length(PtrPlayerTrans->GetPosition() - PtrTrans->GetPosition());
		if (f <= GetStateChangeSize()) {
			GetStateMachine()->ChangeState(NearState::Instance());
		}
	}

	//--------------------------------------------------------------------------------------
	//	�p�X�����񂷂�z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FollowPathObject::FollowPathObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		BaseChara(StagePtr, StartPos)
	{
	}
	FollowPathObject::~FollowPathObject() {}
	//������
	void FollowPathObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(GetStartPos());
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<BaseChara>());
		//CollisionSphere�Փ˔����t����
		auto PtrColl = AddComponent<CollisionSphere>();
		//�d�͂�����
		auto PtrGra = AddComponent<Gravity>();
		//�o�H�����t����
		auto PtrFollowPath = GetBehavior<FollowPathSteering>();
		list<Vec3> PathList = {
			Vec3(-10,0.125,10),
			Vec3(10,0.125,10),
			Vec3(-10,0.125,-10),
			Vec3(10,0.125,-10),
		};
		PtrFollowPath->SetPathList(PathList);
		PtrFollowPath->SetLooped(true);
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetFogEnabled(true);
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"CHECKER_TX");
		//�e�N���X��OnCreate���Ă�
		BaseChara::OnCreate();

	}

	//����
	void FollowPathObject::NearBehavior() {
		auto PtrArrive = GetBehavior<ArriveSteering>();
		auto PtrTrans = GetComponent<Transform>();
		auto PtrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto Force = GetForce();
		Force += PtrArrive->Execute(Force, GetVelocity(), PtrPlayerTrans->GetPosition());
		SetForce(Force);
		float f = bsm::length(PtrPlayerTrans->GetPosition() - PtrTrans->GetPosition());
		if (f > GetStateChangeSize()) {
			GetStateMachine()->ChangeState(FarState::Instance());
		}

	}
	void FollowPathObject::FarBehavior() {
		auto PtrFollowPath = GetBehavior<FollowPathSteering>();
		auto PtrTrans = GetComponent<Transform>();
		auto PtrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto Force = GetForce();
		Force += PtrFollowPath->Execute(Force, GetVelocity());
		SetForce(Force);
		float f = bsm::length(PtrPlayerTrans->GetPosition() - PtrTrans->GetPosition());
		if (f <= GetStateChangeSize()) {
			GetStateMachine()->ChangeState(NearState::Instance());
		}
	}



}
//end basecross
