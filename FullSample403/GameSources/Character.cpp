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
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PCStaticDraw>();

		auto StartPos = m_StartObj.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto EndPos = m_EndObj.lock()->GetComponent<Transform>()->GetWorldPosition();

		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(StartPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(EndPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};

		auto MeshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		MeshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		PtrDraw->SetOriginalMeshResource(MeshRes);
		PtrDraw->SetOriginalMeshUse(true);

	}

	void ActionLine::OnUpdate() {
		auto StartPos = m_StartObj.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto EndPos = m_EndObj.lock()->GetComponent<Transform>()->GetWorldPosition();

		auto PtrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = BackupVec[0];
		new_v.position = StartPos;
		new_vec.push_back(new_v);

		new_v = BackupVec[1];
		new_v.position = EndPos;
		new_vec.push_back(new_v);

		PtrDraw->UpdateVertices(new_vec);

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

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		//�Œ�Ȃ̂Ŏ��ʂ͂���Ȃ�
		param.m_Mass = 0.0f;
		//�����e���\�����f�t�H���g�ŗǂ�
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetDrawActive(true);

	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z�������
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PsBrick::PsBrick(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position,
		float Mass,
		bool IsFixed
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position),
		m_Mass(Mass),
		m_IsFixed(IsFixed),
		m_HoldCount(0)
	{}

	PsBrick::~PsBrick() {}
	//������
	void PsBrick::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = m_Mass;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaBox(param.m_HalfSize, param.m_Mass);
		if (m_IsFixed) {
			param.m_MotionType = PsMotionType::MotionTypeFixed;
		}
		else {
			param.m_MotionType = PsMotionType::MotionTypeActive;
		}
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		if (m_IsFixed) {
			PsPtr->SetContactFilterSelf(1);
			PsPtr->SetContactFilterTarget(1);
		}
		else {
			PsPtr->SetContactFilterSelf(2);
			PsPtr->SetContactFilterTarget(2);
		}
		//���[�A�t���[���̕\��
		PsPtr->SetDrawActive(true);
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<PsBrick>(GetThis<PsBrick>()));
		//�ŏ��̃X�e�[�g��PsBrickDefaultState�ɐݒ�
		m_StateMachine->ChangeState(PsBrickDefaultState::Instance());
	}


	void PsBrick::AddHoldCount() {
		m_HoldCount++;
		if (m_HoldCount == 1) {
			if (m_StateMachine->GetCurrentState() == PsBrickDefaultState::Instance()) {
				m_StateMachine->ChangeState(PsBrickHoldState::Instance());
				//�v���C���[�Ɏ������z�[���h����Ă��邱�Ƃ�`����
				auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player", false);
				if (PlayerPtr) {
					PlayerPtr->SetHoldObject(GetThis<PsBrick>());
				}
			}
		}
		else if (m_HoldCount == 2) {
			auto ShPtr = m_Joint.lock();
			if (ShPtr) {
				ShPtr->SetActive(false);
			}
		}
	}

	void PsBrick::ResetHoldCount() {
		m_HoldCount = 0;
		if (m_StateMachine->GetCurrentState() == PsBrickHoldState::Instance()) {
			m_StateMachine->ChangeState(PsBrickDefaultState::Instance());
		}
	}

	//�z�[���h���̍s��
	void PsBrick::SetHoldBehavior(bool b) {
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));
		}
		else {
			PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
	}


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PsBrickDefaultState)
		void PsBrickDefaultState::Enter(const shared_ptr<PsBrick>& Obj) {
		Obj->SetHoldBehavior(false);
	}

	void PsBrickDefaultState::Execute(const shared_ptr<PsBrick>& Obj) {
	}

	void PsBrickDefaultState::Exit(const shared_ptr<PsBrick>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�I���X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PsBrickHoldState)
		void PsBrickHoldState::Enter(const shared_ptr<PsBrick>& Obj) {
		Obj->SetHoldBehavior(true);
	}

	void PsBrickHoldState::Execute(const shared_ptr<PsBrick>& Obj) {
	}

	void PsBrickHoldState::Exit(const shared_ptr<PsBrick>& Obj) {
		//�������Ȃ�
	}


	//--------------------------------------------------------------------------------------
	/// �W���C���g�}�l�[�W��
	//--------------------------------------------------------------------------------------
	PsJointManager::PsJointManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PsJointManager::~PsJointManager() {}

	void PsJointManager::CreateBallJoint() {
		auto ObjA = GetStage()->GetSharedGameObject<PsBrick>(L"Brick01");
		auto CompA = ObjA->GetComponent<RigidbodyBox>();
		auto ObjB = GetStage()->GetSharedGameObject<PsBrick>(L"Brick02");
		auto CompB = ObjB->GetComponent<RigidbodyBox>();

		PsBallJointParam param;
		param.m_AnchorPoint = (CompA->GetPosition() + CompB->GetPosition())*0.5f;
		param.m_IndexA = CompA->GetIndex();
		param.m_IndexB = CompB->GetIndex();

		m_PsBallJoint = GetStage()->GetBasePhysics().AddBallJoint(param);
		ObjA->SetJoint(m_PsBallJoint);
		ObjB->SetJoint(m_PsBallJoint);

	}


	void PsJointManager::CreateSwingTwistJoint() {
		auto ObjA = GetStage()->GetSharedGameObject<PsBrick>(L"Brick03");
		auto CompA = ObjA->GetComponent<RigidbodyBox>();
		auto ObjB = GetStage()->GetSharedGameObject<PsBrick>(L"Brick04");
		auto CompB = ObjB->GetComponent<RigidbodyBox>();

		PsSwingTwistJointParam param;
		param.m_AnchorPoint = (CompA->GetPosition() + CompB->GetPosition())*0.5f;
		param.m_IndexA = CompA->GetIndex();
		param.m_IndexB = CompB->GetIndex();
		param.m_TwistAxis = Vec3(0, 0, 1.0f);
		param.m_TwistLowerAngle = -0.26f;
		param.m_TwistUpperAngle = 0.26f;
		param.m_SwingLowerAngle = 0.0f;
		param.m_SwingUpperAngle = 0.7f;

		m_PsSwingTwistJoint = GetStage()->GetBasePhysics().AddSwingTwistJoint(param);
		ObjA->SetJoint(m_PsSwingTwistJoint);
		ObjB->SetJoint(m_PsSwingTwistJoint);
	}


	void  PsJointManager::CreateHingeJoint() {
		auto ObjA = GetStage()->GetSharedGameObject<PsBrick>(L"Brick05");
		auto CompA = ObjA->GetComponent<RigidbodyBox>();
		auto ObjB = GetStage()->GetSharedGameObject<PsBrick>(L"Brick06");
		auto CompB = ObjB->GetComponent<RigidbodyBox>();

		PsHingeJointParam param;
		param.m_AnchorPoint = (CompA->GetPosition() + CompB->GetPosition())*0.5f;
		param.m_IndexA = CompA->GetIndex();
		param.m_IndexB = CompB->GetIndex();
		param.m_Axis = Vec3(1.0f, 0.0f, 0.0f);
		param.m_LowerAngle = -0.5f;
		param.m_UpperAngle = 0.7f;

		m_PsHingeJoint = GetStage()->GetBasePhysics().AddHingeJoint(param);
		ObjA->SetJoint(m_PsHingeJoint);
		ObjB->SetJoint(m_PsHingeJoint);

	}

	void PsJointManager::CreateSliderJoint() {
		auto ObjA = GetStage()->GetSharedGameObject<PsBrick>(L"Brick07");
		auto CompA = ObjA->GetComponent<RigidbodyBox>();
		auto ObjB = GetStage()->GetSharedGameObject<PsBrick>(L"Brick08");
		auto CompB = ObjB->GetComponent<RigidbodyBox>();

		PsSliderJointParam param;
		param.m_AnchorPoint = (CompA->GetPosition() + CompB->GetPosition())*0.5f;
		param.m_IndexA = CompA->GetIndex();
		param.m_IndexB = CompB->GetIndex();
		param.m_Direction = Vec3(0.0f, 0.0f, 1.0f);
		param.m_LowerDistance = 0.0f;
		param.m_UpperDistance = 1.0f;

		m_PsSliderJoint = GetStage()->GetBasePhysics().AddSliderJoint(param);
		ObjA->SetJoint(m_PsSliderJoint);
		ObjB->SetJoint(m_PsSliderJoint);
	}

	void PsJointManager::CreateFixJoint() {
		auto ObjA = GetStage()->GetSharedGameObject<PsBrick>(L"Brick09");
		auto CompA = ObjA->GetComponent<RigidbodyBox>();
		auto ObjB = GetStage()->GetSharedGameObject<PsBrick>(L"Brick10");
		auto CompB = ObjB->GetComponent<RigidbodyBox>();

		PsFixJointParam param;
		param.m_AnchorPoint = (CompA->GetPosition() + CompB->GetPosition())*0.5f;
		param.m_IndexA = CompA->GetIndex();
		param.m_IndexB = CompB->GetIndex();

		m_PsFixJoint = GetStage()->GetBasePhysics().AddFixJoint(param);
		ObjA->SetJoint(m_PsFixJoint);
		ObjB->SetJoint(m_PsFixJoint);
	}

	void PsJointManager::CreateUniversalJoint() {
		auto ObjA = GetStage()->GetSharedGameObject<PsBrick>(L"Brick11");
		auto CompA = ObjA->GetComponent<RigidbodyBox>();
		auto ObjB = GetStage()->GetSharedGameObject<PsBrick>(L"Brick12");
		auto CompB = ObjB->GetComponent<RigidbodyBox>();

		PsUniversalJointParam param;
		param.m_AnchorPoint = (CompA->GetPosition() + CompB->GetPosition())*0.5f;
		param.m_IndexA = CompA->GetIndex();
		param.m_IndexB = CompB->GetIndex();
		param.m_Axis = Vec3(0.0f, 0.0f, 1.0f);
		param.m_UpVec = Vec3(0.0f, 1.0f, 0.0f);
		param.m_Swing1LowerAngle = -0.7f;
		param.m_Swing1UpperAngle = 0.7f;
		param.m_Swing2LowerAngle = -0.7f;
		param.m_Swing2UpperAngle = 0.7f;
		m_PsUniversalJoint = GetStage()->GetBasePhysics().AddUniversalJoint(param);
		ObjA->SetJoint(m_PsUniversalJoint);
		ObjB->SetJoint(m_PsUniversalJoint);
	}



	void PsJointManager::OnCreate() {
		CreateBallJoint();
		CreateSwingTwistJoint();
		CreateHingeJoint();
		CreateSliderJoint();
		CreateFixJoint();
		CreateUniversalJoint();
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
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);
		//�R���W������t����i�{�����[���擾�̂��߁j
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetUpdateActive(false);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		GetStage()->SetSharedGameObject(L"FireSphere", GetThis<GameObject>());
	}

	void FireSphere::OnUpdate() {
		auto PtrTransform = GetComponent<Transform>();
		if (PtrTransform->GetPosition().y > -20.0f) {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			Vec3 Ac = Vec3(0, -9.8f, 0) * 1.0f;
			m_Velocity += Ac * ElapsedTime;
			auto Pos = PtrTransform->GetPosition();
			Pos += m_Velocity * ElapsedTime;
			PtrTransform->SetPosition(Pos);
		}
		else {
			//�����Ƃ��Ă���
			PtrTransform->SetPosition(Vec3(0, -20.0f, 0));
		}
		auto Coll = GetComponent<CollisionSphere>();
		//�����I�u�W�F�N�g�����z��̎擾
		vector<shared_ptr<RigidbodySingle>> PsComptVec;
		GetStage()->GetUsedDynamicCompoentVec<RigidbodySingle>(PsComptVec);
		for (auto& v : PsComptVec) {
			auto g_ptr = dynamic_pointer_cast<PsBrick>(v->GetGameObject());
			if (g_ptr && !g_ptr->IsFixed()) {
				if (v->CollisionTest(Coll->GetSphere())) {
					auto h_ptr = m_HoldObject.lock();
					if (h_ptr && h_ptr != g_ptr) {
						h_ptr->ResetHoldCount();
					}
					m_HoldObject = g_ptr;
					g_ptr->AddHoldCount();
					PtrTransform->SetPosition(Vec3(0, -20, 0));
					auto mng = GetStage()->GetSharedGameObject<PsJointManager>(L"PsJointManager");
					break;
				}
			}
		}
	}

	void FireSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(Emitter);
		m_Velocity = Velocity;

	}

}
//end basecross
