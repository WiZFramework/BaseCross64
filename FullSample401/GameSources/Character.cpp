/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	ActionLine::ActionLine(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& StartObj,
		const shared_ptr<GameObject>& EndObj) :
		GameObject(StagePtr),
		m_StartObj(StartObj),
		m_EndObj(EndObj)
	{}




	//������
	void ActionLine::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(Vec3(0.0f));

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<PCStaticDraw>();

		auto startPos = m_StartObj.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto endPos = m_EndObj.lock()->GetComponent<Transform>()->GetWorldPosition();

		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(startPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(endPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};

		auto meshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		meshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		ptrDraw->SetOriginalMeshResource(meshRes);
		ptrDraw->SetOriginalMeshUse(true);

	}

	void ActionLine::OnUpdate() {
		auto startPos = m_StartObj.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto endPos = m_EndObj.lock()->GetComponent<Transform>()->GetWorldPosition();

		auto ptrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& backupVec = ptrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = backupVec[0];
		new_v.position = startPos;
		new_vec.push_back(new_v);

		new_v = backupVec[1];
		new_v.position = endPos;
		new_vec.push_back(new_v);

		ptrDraw->UpdateVertices(new_vec);

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
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃI�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	//������
	void ActivePsObject::OnCreate() {
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<ActivePsObject>(GetThis<ActivePsObject>()));
		//�ŏ��̃X�e�[�g��PlayerDefault�ɐݒ�
		m_StateMachine->ChangeState(ActivePsDefaultState::Instance());
	}

	void ActivePsObject::SetHold(bool b) {
		if (b) {
			if (m_StateMachine->GetCurrentState() == ActivePsDefaultState::Instance()) {
				m_StateMachine->ChangeState(ActivePsHoldState::Instance());
				//�v���C���[�Ɏ������z�[���h����Ă��邱�Ƃ�`����
				auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
				if (ptrPlayer) {
					ptrPlayer->SetHoldObject(GetThis<ActivePsObject>());
				}
			}
		}
		else {
			if (m_StateMachine->GetCurrentState() == ActivePsHoldState::Instance()) {
				m_StateMachine->ChangeState(ActivePsDefaultState::Instance());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(ActivePsDefaultState)
	void ActivePsDefaultState::Enter(const shared_ptr<ActivePsObject>& Obj) {
		Obj->SetHoldBehavior(false);
	}

	void ActivePsDefaultState::Execute(const shared_ptr<ActivePsObject>& Obj) {
	}

	void ActivePsDefaultState::Exit(const shared_ptr<ActivePsObject>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�I���X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(ActivePsHoldState)

	void ActivePsHoldState::Enter(const shared_ptr<ActivePsObject>& Obj) {
		Obj->SetHoldBehavior(true);
	}

	void ActivePsHoldState::Execute(const shared_ptr<ActivePsObject>& Obj) {
	}

	void ActivePsHoldState::Exit(const shared_ptr<ActivePsObject>& Obj) {
		//�������Ȃ�
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
		//�e�N���X��OnCreate���Ă�
		ActivePsObject::OnCreate();

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
		PsBoxParam param(ptrTrans->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto ptrPs = AddComponent<RigidbodyBox>(param);
		ptrPs->SetDrawActive(true);
	}

	void ActivePsBox::SetHoldBehavior(bool b) {
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

		}
		else {
			PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
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
		//�e�N���X��OnCreate���Ă�
		ActivePsObject::OnCreate();

		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z����
		//WorldMatrix�����Ƃ�RigidbodySphere�̃p�����[�^���쐬
		PsSphereParam param(ptrTrans->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto ptrPs = AddComponent<RigidbodySphere>(param);
		ptrPs->SetDrawActive(true);
	}

	void ActivePsSphere::SetHoldBehavior(bool b) {
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

		}
		else {
			ptrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
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

	void ActivePsCapsule::SetHoldBehavior(bool b) {
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

		}
		else {
			ptrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z���Ȃ����˂��鋅��
	//--------------------------------------------------------------------------------------
	FireSphere::FireSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity
	) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	FireSphere::~FireSphere() {}

	void FireSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(m_Emitter);
		//�R���W������t����i�{�����[���擾�̂��߁j
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"SKY_TX");

		GetStage()->SetSharedGameObject(L"FireSphere", GetThis<GameObject>());
	}

	void FireSphere::OnUpdate() {
		auto ptrTrans = GetComponent<Transform>();
		if (ptrTrans->GetPosition().y > -20.0f) {
			float elapsedTime = App::GetApp()->GetElapsedTime();
			Vec3 Ac = Vec3(0, -9.8f, 0) * 1.0f;
			m_Velocity += Ac * elapsedTime;
			auto Pos = ptrTrans->GetPosition();
			Pos += m_Velocity * elapsedTime;
			ptrTrans->SetPosition(Pos);
		}
		else {
			//�����Ƃ��Ă���
			ptrTrans->SetPosition(Vec3(0, -20.0f, 0));
			return;
		}
		auto ptrColl = GetComponent<CollisionSphere>();
		//�����I�u�W�F�N�g�����z��̎擾
		vector<shared_ptr<Rigidbody>> PsComptVec;
		GetStage()->GetUsedDynamicCompoentVec<Rigidbody>(PsComptVec);
		for (auto& v : PsComptVec) {
			auto ptrG = dynamic_pointer_cast<ActivePsObject>(v->GetGameObject());
			if (ptrG) {
				auto ptrRegSp = dynamic_pointer_cast<RigidbodySphere>(v);
				auto ptrRegBox = dynamic_pointer_cast<RigidbodyBox>(v);
				auto ptrRegCap = dynamic_pointer_cast<RigidbodyCapsule>(v);
				bool hold = false;
				if (ptrRegSp) {
					if (HitTest::SPHERE_SPHERE(ptrRegSp->GetSPHERE(), ptrColl->GetSphere())) {
						hold = true;
					}
				}
				else if (ptrRegBox) {
					Vec3 ret;
					if (HitTest::SPHERE_OBB(ptrColl->GetSphere(), ptrRegBox->GetOBB(), ret)) {
						hold = true;
					}
				}
				else if (ptrRegCap) {
					Vec3 ret;
					if (HitTest::SPHERE_CAPSULE(ptrColl->GetSphere(), ptrRegCap->GetCAPSULE(), ret)) {
						hold = true;
					}
				}
				if (hold) {
					auto ptrHold = m_HoldObject.lock();
					if (ptrHold) {
						ptrHold->SetHold(false);
					}
					m_HoldObject = ptrG;
					ptrG->SetHold(true);
					ptrTrans->ResetPosition(Vec3(0, -20, 0));
					break;
				}
			}
		}
	}

	void FireSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->ResetPosition(Emitter);
		m_Velocity = Velocity;

	}

}
//end basecross
