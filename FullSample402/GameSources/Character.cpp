/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//������
	void FixedPsBox::OnCreate() {

		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		//�����v�Z�{�b�N�X
		PsBoxParam param(ptrTrans->GetWorldMatrix(), 0.0f, true, PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_StateChangeSize(3.0f),
		m_MaxForce(30.0f),
		m_MaxSpeed(10.0f),
		m_Decl(3.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//����
	void SeekObject::OnUpdate() {
		//�t�H�[�X�̏�����
		m_Force = Vec3(0);
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	float SeekObject::GetTargetToLen() const {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		return length(TargetPos - Pos);
	}

	void SeekObject::SeekBehavior() {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Seek(GetVelocity(), TargetPos, Pos, m_MaxSpeed);
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}

	void SeekObject::ArriveBehavior() {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Arrive(GetVelocity(), TargetPos,
			Pos, m_MaxSpeed, m_Decl);
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}


	void SeekObject::SeparationBehavior() {
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekObjectGroup");
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Separation(Group, GetThis<GameObject>());
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}



	//--------------------------------------------------------------------------------------
	///	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->SeparationBehavior();
		Obj->SeekBehavior();
		if (Obj->GetTargetToLen() < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->SeparationBehavior();
		Obj->ArriveBehavior();
		if (Obj->GetTargetToLen() >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȓʖʃI�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//static�ϐ��̏�����
	shared_ptr<MeshResource> ActivePsConvex::m_ConvexMesh = nullptr;
	shared_ptr<PsConvexMeshResource> ActivePsConvex::m_PsConvexMesh = nullptr;

	ActivePsConvex::ActivePsConvex(const shared_ptr<Stage>& StagePtr,
		const Vec3& Position
	) :
		SeekObject(StagePtr),
		m_Position(Position)
	{}
	ActivePsConvex::~ActivePsConvex() {}

	void ActivePsConvex::OnCreate() {
		if (!m_ConvexMesh || !m_PsConvexMesh) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateDodecahedron(0.5, vertices, indices);
			m_ConvexMesh = MeshResource::CreateMeshResource(vertices, indices, false);
			m_PsConvexMesh = PsConvexMeshResource::CreateMeshResource(vertices, indices);
		}

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_ConvexMesh);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(m_ConvexMesh);
		PtrDraw->SetTextureResource(L"WALL_TX");

		//�����v�Z�ʖ�
		PsConvexParam param;
		param.m_ConvexMeshResource = m_PsConvexMesh;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z(���Ɠ����ɂ���)
		param.m_Inertia = BasePhysics::CalcInertiaSphere(0.5f, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Quat();
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<RigidbodyConvex>(param);
		PsPtr->SetDrawActive(true);
		//�e�N���X��OnCreate���Ă�
		SeekObject::OnCreate();
	}

	Vec3 ActivePsConvex::GetVelocity()const {
		auto PtrPs = GetComponent<RigidbodyConvex>();
		return PtrPs->GetLinearVelocity();
	}

	//�X�V
	void ActivePsConvex::OnUpdate() {
		//�e�N���X��OnUpdate���Ă�
		SeekObject::OnUpdate();
		auto PtrPs = GetComponent<RigidbodyConvex>();
		//���݂̃t�H�[�X��ݒ�
		PtrPs->ApplyForce(GetForce());
	}




	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Vec3& Position
	) :
		SeekObject(StagePtr),
		m_Scale(Scale),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//������
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");

		//�����v�Z����
		PsSphereParam param;
		//DEFAULT_SPHERE�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a�ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Quat();
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<RigidbodySphere>(param);
		PsPtr->SetDrawActive(true);
		//�e�N���X��OnCreate���Ă�
		SeekObject::OnCreate();
	}

	Vec3 ActivePsSphere::GetVelocity()const {
		auto PtrPs = GetComponent<RigidbodySphere>();
		return PtrPs->GetLinearVelocity();
	}

	//�X�V
	void ActivePsSphere::OnUpdate() {
		//�e�N���X��OnUpdate���Ă�
		SeekObject::OnUpdate();
		auto PtrPs = GetComponent<RigidbodySphere>();
		//���݂̃t�H�[�X��ݒ�
		PtrPs->ApplyForce(GetForce());
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȍ����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	ActivePsCombinedObject::ActivePsCombinedObject(const shared_ptr<Stage>& StagePtr,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsCombinedObject::~ActivePsCombinedObject() {}

	void ActivePsCombinedObject::CreateDrawComp(const PsCombinedParam& param) {
		//�\���p�̃J�v�Z�����b�V���̍쐬
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		bsm::Vec3 PointA(0, 0, 0);
		bsm::Vec3 PointB(0, 0, 0);
		PointA -= bsm::Vec3(0, param.m_Primitives[2].m_HalfLen, 0);
		PointB += bsm::Vec3(0, param.m_Primitives[2].m_HalfLen, 0);
		MeshUtill::CreateCapsule(param.m_Primitives[2].m_Radius * 2.0f,
			PointA, PointB, 18, vertices, indices, true);
		m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//���̃I�u�W�F�N�g�̓}���`���b�V�����g��
		m_MultiMeshResource = ObjectFactory::Create<MultiMeshResource>();
		auto index = m_MultiMeshResource->AddMesh(L"DEFAULT_CUBE");
		//���b�V�����Ƃ̃e�N�X�`���̐ݒ�
		m_MultiMeshResource->SetTextureResource(L"WALL_TX", index);
		index = m_MultiMeshResource->AddMesh(L"DEFAULT_CUBE");
		m_MultiMeshResource->SetTextureResource(L"WALL_TX", index);
		index = m_MultiMeshResource->AddMesh(m_CapsuleMesh);
		m_MultiMeshResource->SetTextureResource(L"SKY_TX", index);
		//�}���`���b�V����\���p�Ɏg�����߂ɐݒ�
		for (size_t i = 0; i < param.m_Primitives.size(); i++) {
			auto& v = param.m_Primitives[i];
			Mat4x4 m;
			if (i == 2) {
				//�J�v�Z���͊g�嗦1.0
				m.affineTransformation(
					Vec3(1.0f),
					Vec3(0, 0, 0),
					v.m_OffsetOrientation,
					v.m_OffsetPosition
				);
			}
			else {
				//�{�b�N�X�͊g�嗦�w��
				m.affineTransformation(
					v.m_HalfSize * 2.0f,
					Vec3(0, 0, 0),
					v.m_OffsetOrientation,
					v.m_OffsetPosition
				);
			}
			//Transform�ƃ��b�V���̍����s��̐ݒ�
			m_MultiMeshResource->SetUseMeshToTransformMatrix(true, i);
			m_MultiMeshResource->SetMeshToTransformMatrix(m, i);
		}
		//�`��R���|�l���g
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		//�}���`���b�V�����\�[�X��ݒ�
		PtrDraw->SetMultiMeshResource(m_MultiMeshResource);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMultiMeshResource(m_MultiMeshResource);
	}


	void ActivePsCombinedObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);
		//�����I�u�W�F�N�g�̏���
		PsCombinedParam param;
		//���ʂ͏d������
		param.m_Mass = 3.0f;
		//Box�p�̊����i�����e���\���j���v�Z
		param.m_Inertia = BasePhysics::CalcInertiaBox(Vec3(2.5f, 1.0f, 1.0f), param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		//���������v���~�e�B�u�i0�ԖځA�{�b�N�X�j
		PsCombinedPrimitive primitive;
		primitive.reset();
		primitive.m_CombinedType = PsCombinedType::TypeBox;
		primitive.m_HalfSize = Vec3(0.5f, 0.5f, 1.5f);
		primitive.m_OffsetPosition = Vec3(-2.0f, 0.0f, 0.0f);
		//�����I�u�W�F�N�g�ɒǉ�
		param.AddPrim(primitive);
		//���������v���~�e�B�u�i1�ԖځA�{�b�N�X�j
		primitive.reset();
		primitive.m_CombinedType = PsCombinedType::TypeBox;
		primitive.m_HalfSize = Vec3(0.5f, 1.5f, 0.5f);
		primitive.m_OffsetPosition = Vec3(2.0f, 0.0f, 0.0f);
		//�����I�u�W�F�N�g�ɒǉ�
		param.AddPrim(primitive);
		//���������v���~�e�B�u�i2�ԖځA�J�v�Z���j
		primitive.reset();
		primitive.m_CombinedType = PsCombinedType::TypeCapsule;
		primitive.m_HalfLen = 1.5f;
		primitive.m_Radius = 0.5f;
		primitive.m_OffsetPosition = Vec3(0.0f, 0.0f, 0.0f);
		//�����I�u�W�F�N�g�ɒǉ�
		param.AddPrim(primitive);
		//�����R���|�[�l���g�i�����j
		auto PsPtr = AddComponent<RigidbodyCombined>(param);
		PsPtr->SetDrawActive(true);
		//�����R���|�[�l���g�ɍ��킹�ĕ`��R���|�[�l���g�i�e���j���쐬
		CreateDrawComp(param);
	}




	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	FirePsSphere::~FirePsSphere() {}

	void FirePsSphere::CreateDefParam(PsSphereParam& param) {
		//DEFAULT_SPHERE�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a�ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		//�X���[�v���Ȃ�
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
	}

	//������
	void FirePsSphere::OnCreate() {
		//���L�I�u�W�F�N�g�ɃZ�b�g
		GetStage()->SetSharedGameObject(L"FirePsSphere", GetThis<FirePsSphere>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		PsSphereParam param;
		CreateDefParam(param);
		param.m_Pos = m_Emitter;
		param.m_LinearVelocity = m_Velocity;
		auto PsPtr = AddComponent<RigidbodySphere>(param);
		PsPtr->SetDrawActive(true);
	}


	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto PsPtr = GetComponent<RigidbodySphere>();
		PsSphereParam param;
		CreateDefParam(param);
		param.m_Pos = Emitter;
		param.m_LinearVelocity = Velocity;
		PsPtr->Reset(param, PsPtr->GetIndex());
	}





}
//end basecross
