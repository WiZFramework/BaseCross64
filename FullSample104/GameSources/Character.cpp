/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//������
	void MultiSpark::OnCreate() {
	}


	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(4);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.5f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{
	}
	Box::~Box() {}

	//������
	void Box::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(0.5f, 0.5f, 0.5f));
		Quat Qt;
		Qt.identity();
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetPosition(m_StartPos);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionObb>();
		//�Փ˔����None�ɂ���
		PtrCol->SetAfterCollision(AfterCollision::None);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������
		SetAlphaActive(true);

		//�����v�Z�{�b�N�X
		PsBoxParam param(ptrTrans->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetDrawActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Box>(GetThis<Box>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(BoxDefaultState::Instance());

	}

	//�X�V
	void Box::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	//�v���C���[�Ƃ̋����𓾂�
	float Box::GetToPlayerLen() const {
		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		auto PlayerPos
			= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		return length(Pos - PlayerPos);
	}



	//���x��0�ɂ���
	void Box::Stop() {
		auto PsPtr = GetComponent<RigidbodyBox>();
		PsPtr->SetLinearVelocity(Vec3(0, 0, 0));
		m_TotalTime = 0;
		m_StartPos = GetComponent<Transform>()->GetPosition();
		m_HitAfterTime = 0;
	}


	//�s�����藈����̍s��
	void Box::GoFromBehavior() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		auto PsPtr = GetComponent<RigidbodyBox>();
		auto Pos = PsPtr->GetPosition();
		Pos.x = sin(m_TotalTime) + m_StartPos.x;
		PsPtr->WakeUp();
		PsPtr->SetPosition(Pos);
	}

	//�v���C���[��ǂ�������s��
	void Box::SeekPlayerBehavior() {
		auto PsPtr = GetComponent<RigidbodyBox>();
		PsPtr->WakeUp();
		auto TargetPtr = GetStage()->GetSharedObject(L"Player");
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto Pos = GetComponent<Transform>()->GetPosition();
		TargetPos.y = Pos.y = m_StartPos.y;
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Seek(Vec3(0), TargetPos,
			Pos, 20.0f) * 1.0f;
		PsPtr->ApplyForce(WorkForce);
	}


	//�C�e���ˍs��
	void Box::FirShellBehavior(int div, float up) {
		if (Util::DivProbability(div)) {
			//div����1�̊m���Ŕ���
			auto Ptr = GetComponent<Transform>();
			Vec3 Pos = Ptr->GetPosition();
			Pos.y += 0.5f;
			auto PlayerPos
				= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
			auto velo = PlayerPos;
			velo -= Pos;
			velo.y = up;
			velo.normalize();
			velo *= 15.0f;
			auto Group = GetStage()->GetSharedObjectGroup(L"FireSphereGroup");
			for (size_t i = 0; i < Group->size(); i++) {
				auto shptr = dynamic_pointer_cast<FireSphere>(Group->at(i));
				if (shptr && !shptr->IsUpdateActive()) {
					//�󂫂���������
					shptr->Reset(Pos, velo);
					return;
				}
			}
			//�����܂ŗ��Ă�΋󂫂��Ȃ�
			GetStage()->AddGameObject<FireSphere>(Pos, velo);
		}
	}

	//�C�e�����������s��
	void Box::FirShellHitBehavior(const Vec3& HitVelocity) {
		auto PsPtr = GetComponent<RigidbodyBox>();
		PsPtr->WakeUp();
		Vec3 HitVelo = HitVelocity;
		HitVelo.y = 5.0f;
		PsPtr->SetLinearVelocity(HitVelo);
		Vec3 QtTmp = cross(normalize(HitVelocity), Vec3(0, 1, 0));
		PsPtr->SetAngularVelocity(HitVelo);
		m_HitAfterTime = 0;
		//�T�E���h�̍Đ�
		auto ptrXA = App::GetApp()->GetXAudio2Manager();
		ptrXA->Start(L"cursor", 0, 0.5f);
		//�q�b�g�X�e�[�g�Ɉڍs
		GetStateMachine()->ChangeState(BoxHitState::Instance());
	}

	float Box::AddHitAfterTime() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_HitAfterTime += ElapsedTime;
		return m_HitAfterTime;
	}



	//--------------------------------------------------------------------------------------
	//	�ʏ�ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxDefaultState> BoxDefaultState::Instance() {
		static shared_ptr<BoxDefaultState> instance(new BoxDefaultState);
		return instance;
	}
	void BoxDefaultState::Enter(const shared_ptr<Box>& Obj) {
		Obj->Stop();
	}
	void BoxDefaultState::Execute(const shared_ptr<Box>& Obj) {
		Obj->GoFromBehavior();
		Obj->FirShellBehavior(120, 0.2f);
		if (Obj->GetToPlayerLen() < 10.0f) {
			Obj->GetStateMachine()->ChangeState(BoxSimpleAttackState::Instance());
		}
	}
	void BoxDefaultState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�ǂ�������ʏ�U���X�e�[�g
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxSimpleAttackState> BoxSimpleAttackState::Instance() {
		static shared_ptr<BoxSimpleAttackState> instance(new BoxSimpleAttackState);
		return instance;
	}
	void BoxSimpleAttackState::Enter(const shared_ptr<Box>& Obj) {
	}
	void BoxSimpleAttackState::Execute(const shared_ptr<Box>& Obj) {
		Obj->SeekPlayerBehavior();
		Obj->FirShellBehavior(60, 0.05f);
		float len = Obj->GetToPlayerLen();
		if (len >= 10.0f) {
			Obj->GetStateMachine()->ChangeState(BoxDefaultState::Instance());
		}
		else if (len < 5.0f) {
			Obj->GetStateMachine()->ChangeState(BoxHradAttackState::Instance());
		}
	}
	void BoxSimpleAttackState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�n�[�h�ȍU���X�e�[�g
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxHradAttackState> BoxHradAttackState::Instance() {
		static shared_ptr<BoxHradAttackState> instance(new BoxHradAttackState);
		return instance;
	}
	void BoxHradAttackState::Enter(const shared_ptr<Box>& Obj) {
		Obj->Stop();
	}
	void BoxHradAttackState::Execute(const shared_ptr<Box>& Obj) {
		Obj->FirShellBehavior(40, 0.0f);
		float len = Obj->GetToPlayerLen();
		if (len >= 10.0f) {
			Obj->GetStateMachine()->ChangeState(BoxDefaultState::Instance());
		}
	}
	void BoxHradAttackState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�C�e�ɓ��������X�e�[�g
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxHitState> BoxHitState::Instance() {
		static shared_ptr<BoxHitState> instance(new BoxHitState);
		return instance;
	}
	void BoxHitState::Enter(const shared_ptr<Box>& Obj) {
	}
	void BoxHitState::Execute(const shared_ptr<Box>& Obj) {
		if (Obj->AddHitAfterTime() >= 1.0f) {
			Obj->GetStateMachine()->ChangeState(BoxDefaultState::Instance());
		}
	}

	void BoxHitState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�C�e
	//--------------------------------------------------------------------------------------
	FireSphere::FireSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity
	) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f),
		m_ActiveMaxY(-5.0f)
	{}
	FireSphere::~FireSphere() {}

	void FireSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(m_Emitter);
		//�R���W������t����
		auto ptrColl = AddComponent<CollisionSphere>();
		//�Փ˔����None�ɂ���
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"BROWN_TX");

		auto group = GetStage()->GetSharedObjectGroup(L"FireSphereGroup");
		group->IntoGroup(GetThis<GameObject>());

	}

	void FireSphere::OnUpdate() {
		auto ptrTrans = GetComponent<Transform>();
		if (ptrTrans->GetPosition().y > m_ActiveMaxY) {
			float elapsedTime = App::GetApp()->GetElapsedTime();
			auto Pos = ptrTrans->GetPosition();
			Pos += m_Velocity * elapsedTime;
			ptrTrans->SetPosition(Pos);
		}
		else {
			SetUpdateActive(false);
			SetDrawActive(false);
		}
	}

	void FireSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->ResetPosition(Emitter);
		m_Velocity = Velocity;
		auto ptrGrav = GetComponent<Gravity>();
		ptrGrav->SetGravityVerocityZero();
		SetUpdateActive(true);
		SetDrawActive(true);
	}

	void FireSphere::OnCollisionEnter(const CollisionPair& Pair) {
		auto ptrTrans = GetComponent<Transform>();
		auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark", false);
		if (PtrSpark) {
			PtrSpark->InsertSpark(GetComponent<Transform>()->GetPosition());
		}

		auto ptrBox = dynamic_pointer_cast<Box>(Pair.m_Dest.lock()->GetGameObject());
		auto ptrPlayer = dynamic_pointer_cast<Player>(Pair.m_Dest.lock()->GetGameObject());
		if (ptrBox) {
			ptrBox->FirShellHitBehavior(m_Velocity);
		}
		if (ptrPlayer) {
			ptrPlayer->FirShellHitBehavior(m_Velocity);
		}
		auto pos = ptrTrans->GetPosition();
		pos.y = m_ActiveMaxY;
		ptrTrans->SetPosition(pos);
		SetUpdateActive(false);
		SetDrawActive(false);

	}

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

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param(PtrTransform->GetWorldMatrix(), 0.0f, true, PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetDrawActive(true);

	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃI�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	void ActivePsObject::OnUpdate() {
		if (!IsSelected()) {
			return;
		}
		Vec3 Near, Far;
		GetTypeStage<GameStage>()->GetMouseRay(Near, Far);
		auto PsPtr = GetDynamicComponent<RigidbodySingle>(false);
		if (PsPtr) {
			auto PsPos = PsPtr->GetPosition();
			float t;
			Vec3 RayPos;
			//���݈ʒu�ƈ�ԋ߂����C��̓_�𓾂�
			HitTest::ClosetPtPointSegment(PsPos, Near, Far, t, RayPos);
			Vec3 ToVec = RayPos - PsPos;
			ToVec *= 2.0f;
			PsPtr->WakeUp();
			PsPtr->SetLinearVelocity(ToVec);
		}
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//������
	void ActivePsBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionObb>();
		//�Փ˔����None�ɂ���
		PtrCol->SetAfterCollision(AfterCollision::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param(PtrTransform->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//������
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		//�Փ˔����None�ɂ���
		PtrCol->SetAfterCollision(AfterCollision::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z����
		//WorldMatrix�����Ƃ�RigidbodySphere�̃p�����[�^���쐬
		PsSphereParam param(PtrTransform->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodySphere>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃJ�v�Z��
	//--------------------------------------------------------------------------------------
	ActivePsCapsule::ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
		float Len,
		float Diameter,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Len(Len),
		m_Diameter(Diameter),
		m_Qt(Qt),
		m_Position(Position)
	{}
	ActivePsCapsule::~ActivePsCapsule() {}

	//������
	void ActivePsCapsule::OnCreate() {
		//�e�N���X��OnCreate���Ă�
		ActivePsObject::OnCreate();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		bsm::Vec3 pointA(0, 0, 0);
		bsm::Vec3 pointB(0, 0, 0);
		pointA -= bsm::Vec3(0, m_Len * 0.5f, 0);
		pointB += bsm::Vec3(0, m_Len * 0.5f, 0);
		MeshUtill::CreateCapsule(m_Diameter,
			pointA, pointB, 18, vertices, indices, true);
		m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		auto ptrTrans = GetComponent<Transform>();
		//�X�P�[�����O��1.0f
		ptrTrans->SetScale(Vec3(1.0f));
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(m_CapsuleMesh);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionCapsule>();
		//�Փ˔����None�ɂ���
		PtrCol->SetAfterCollision(AfterCollision::None);


		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(m_CapsuleMesh);
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�J�v�Z��
		PsCapsuleParam param;
		//���a�ɂ���
		param.m_HalfLen = m_Len * 0.5f;
		param.m_Radius = m_Diameter * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaCylinderX(
			param.m_HalfLen + param.m_Radius,
			param.m_Radius,
			param.m_Mass
		);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto ptrPs = AddComponent<RigidbodyCapsule>(param);
		ptrPs->SetDrawActive(true);
	}


}
//end basecross
