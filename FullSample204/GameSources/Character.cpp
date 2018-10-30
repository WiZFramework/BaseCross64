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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);
		//OBB�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedBox");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);
		//Capsule�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedSphere");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"WALL_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

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
		float elapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * elapsedTime;
		auto ptrTrans = GetComponent<Transform>();
		auto pos = ptrTrans->GetPosition();
		pos += m_Velocity * elapsedTime;
		ptrTrans->SetPosition(pos);
	}

	shared_ptr<GameObject>  BaseChara::GetTarget()const {
		return GetStage()->GetSharedObject(L"Player");
	}

	//������
	void BaseChara::OnCreate() {
		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//�O���[�v�Ɏ������g��ǉ�
		group->IntoGroup(GetThis<BaseChara>());
		//�����s��������
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->SetGameObjectGroup(group);
		//�ǉ���s����t����
		auto ptrWall = GetBehavior<WallAvoidanceSteering>();
		vector<PLANE> planeVec = {
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
		ptrWall->SetPlaneVec(planeVec);
		//��Q������s����t����
		vector<shared_ptr<GameObject>> spObjVec;
		GetStage()->GetUsedTagObjectVec(L"FixedSphere", spObjVec);
		vector<SPHERE> spVec;
		for (auto& v : spObjVec) {
			auto TransPtr = v->GetComponent<Transform>();
			SPHERE sp;
			sp.m_Center = TransPtr->GetPosition();
			sp.m_Radius = TransPtr->GetScale().x * 0.5f;
			spVec.push_back(sp);
		}
		auto ptrAvoidance = GetBehavior<ObstacleAvoidanceSteering>();
		ptrAvoidance->SetObstacleSphereVec(spVec);
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<BaseChara>(GetThis<BaseChara>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}
	//�X�V
	void BaseChara::OnUpdate() {
		m_Force = Vec3(0);
		//���ʂ̃X�e�A�����O1
		auto ptrWall = GetBehavior<WallAvoidanceSteering>();
		m_Force += ptrWall->Execute(m_Force, GetVelocity());
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
		//���ʂ̃X�e�A�����O2
		auto ptrSep = GetBehavior<SeparationSteering>();
		m_Force += ptrSep->Execute(m_Force);
		auto ptrAvoidance = GetBehavior<ObstacleAvoidanceSteering>();
		m_Force += ptrAvoidance->Execute(m_Force, GetVelocity());
		ApplyForce();
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(1.0f);
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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStartPos());
		ptrTrans->SetScale(0.125f, 0.25f, 0.25f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"ObjGroup");

		//Obb�̏Փ˔��������
		auto ptrColl = AddComponent<CollisionObb>();
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);
		//�e�N���X��OnCreate���Ă�
		BaseChara::OnCreate();
	}


	void SeekObject::NearBehavior() {
		auto ptrArrive = GetBehavior<ArriveSteering>();
		auto ptrTrans = GetComponent<Transform>();
		auto ptrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto force = GetForce();
		force += ptrArrive->Execute(force, GetVelocity(), ptrPlayerTrans->GetPosition());
		SetForce(force);
		float f = bsm::length(ptrPlayerTrans->GetPosition() - ptrTrans->GetPosition());
		if (f > GetStateChangeSize()) {
			GetStateMachine()->ChangeState(FarState::Instance());
		}

	}
	void SeekObject::FarBehavior() {
		auto ptrSeek = GetBehavior<SeekSteering>();
		auto ptrTrans = GetComponent<Transform>();
		auto ptrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto force = GetForce();
		force += ptrSeek->Execute(force, GetVelocity(), ptrPlayerTrans->GetPosition());
		SetForce(force);
		float f = bsm::length(ptrPlayerTrans->GetPosition() - ptrTrans->GetPosition());
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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStartPos());
		ptrTrans->SetScale(0.25f, 0.25f, 0.25f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"TRACE2_TX");
		SetAlphaActive(true);
		//�e�N���X��OnCreate���Ă�
		BaseChara::OnCreate();

	}

	//����
	void PursuitObject::NearBehavior() {
		auto ptrArrive = GetBehavior<ArriveSteering>();
		auto ptrTrans = GetComponent<Transform>();
		auto ptrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto force = GetForce();
		force += ptrArrive->Execute(force, GetVelocity(), ptrPlayerTrans->GetPosition());
		SetForce(force);
		float f = bsm::length(ptrPlayerTrans->GetPosition() - ptrTrans->GetPosition());
		if (f > GetStateChangeSize()) {
			GetStateMachine()->ChangeState(FarState::Instance());
		}

	}
	void PursuitObject::FarBehavior() {
		auto ptrPursuit = GetBehavior<PursuitSteering>();
		ptrPursuit->SetWeight(1.5f);
		auto ptrTrans = GetComponent<Transform>();
		auto ptrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto force = GetForce();
		force += ptrPursuit->Execute(force, GetVelocity(), ptrPlayerTrans->GetPosition(),
			ptrPlayerTrans->GetVelocity(), ptrPlayerTrans->GetRotation());
		SetForce(force);
		float f = bsm::length(ptrPlayerTrans->GetPosition() - ptrTrans->GetPosition());
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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStartPos());
		ptrTrans->SetScale(0.25f, 0.25f, 0.25f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//�O���[�v�Ɏ������g��ǉ�
		group->IntoGroup(GetThis<BaseChara>());
		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�o�H�����t����
		auto ptrFollowPath = GetBehavior<FollowPathSteering>();
		list<Vec3> pathList = {
			Vec3(-10,0.125,10),
			Vec3(10,0.125,10),
			Vec3(-10,0.125,-10),
			Vec3(10,0.125,-10),
		};
		ptrFollowPath->SetPathList(pathList);
		ptrFollowPath->SetLooped(true);
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"CHECKER_TX");
		//�e�N���X��OnCreate���Ă�
		BaseChara::OnCreate();

	}

	//����
	void FollowPathObject::NearBehavior() {
		auto ptrArrive = GetBehavior<ArriveSteering>();
		auto ptrTrans = GetComponent<Transform>();
		auto ptrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto force = GetForce();
		force += ptrArrive->Execute(force, GetVelocity(), ptrPlayerTrans->GetPosition());
		SetForce(force);
		float f = bsm::length(ptrPlayerTrans->GetPosition() - ptrTrans->GetPosition());
		if (f > GetStateChangeSize()) {
			GetStateMachine()->ChangeState(FarState::Instance());
		}

	}
	void FollowPathObject::FarBehavior() {
		auto ptrFollowPath = GetBehavior<FollowPathSteering>();
		auto ptrTrans = GetComponent<Transform>();
		auto ptrPlayerTrans = GetTarget()->GetComponent<Transform>();
		auto force = GetForce();
		force += ptrFollowPath->Execute(force, GetVelocity());
		SetForce(force);
		float f = bsm::length(ptrPlayerTrans->GetPosition() - ptrTrans->GetPosition());
		if (f <= GetStateChangeSize()) {
			GetStateMachine()->ChangeState(NearState::Instance());
		}
	}



}
//end basecross
