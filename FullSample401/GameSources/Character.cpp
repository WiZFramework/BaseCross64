/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ライン
	//--------------------------------------------------------------------------------------
	ActionLine::ActionLine(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& StartObj,
		const shared_ptr<GameObject>& EndObj) :
		GameObject(StagePtr),
		m_StartObj(StartObj),
		m_EndObj(EndObj)
	{}




	//初期化
	void ActionLine::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(Vec3(0.0f));

		//描画コンポーネント
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
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
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
	//初期化
	void FixedPsBox::OnCreate() {

		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		//物理計算ボックス
		PsBoxParam param(ptrTrans->GetWorldMatrix(), 0.0f, true, PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
	}

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなオブジェクトの親
	//--------------------------------------------------------------------------------------
	//初期化
	void ActivePsObject::OnCreate() {
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<ActivePsObject>(GetThis<ActivePsObject>()));
		//最初のステートをPlayerDefaultに設定
		m_StateMachine->ChangeState(ActivePsDefaultState::Instance());
	}

	void ActivePsObject::SetHold(bool b) {
		if (b) {
			if (m_StateMachine->GetCurrentState() == ActivePsDefaultState::Instance()) {
				m_StateMachine->ChangeState(ActivePsHoldState::Instance());
				//プレイヤーに自分がホールドされていることを伝える
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
	///	通常ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(ActivePsDefaultState)
	void ActivePsDefaultState::Enter(const shared_ptr<ActivePsObject>& Obj) {
		Obj->SetHoldBehavior(false);
	}

	void ActivePsDefaultState::Execute(const shared_ptr<ActivePsObject>& Obj) {
	}

	void ActivePsDefaultState::Exit(const shared_ptr<ActivePsObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	選択ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(ActivePsHoldState)

	void ActivePsHoldState::Enter(const shared_ptr<ActivePsObject>& Obj) {
		Obj->SetHoldBehavior(true);
	}

	void ActivePsHoldState::Execute(const shared_ptr<ActivePsObject>& Obj) {
	}

	void ActivePsHoldState::Exit(const shared_ptr<ActivePsObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
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
	//初期化
	void ActivePsBox::OnCreate() {
		//親クラスのOnCreateを呼ぶ
		ActivePsObject::OnCreate();

		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");


		//物理計算ボックス
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
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	//構築と破棄
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
	//初期化
	void ActivePsSphere::OnCreate() {
		//親クラスのOnCreateを呼ぶ
		ActivePsObject::OnCreate();

		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");

		//物理計算球体
		//WorldMatrixをもとにRigidbodySphereのパラメータを作成
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
	///	物理計算するアクティブなカプセル
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

	//初期化
	void ActivePsCapsule::OnCreate() {
		//親クラスのOnCreateを呼ぶ
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
		//スケーリングは1.0f
		ptrTrans->SetScale(Vec3(1.0f));
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(m_CapsuleMesh);

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(m_CapsuleMesh);
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"SKY_TX");

		//物理計算カプセル
		PsCapsuleParam param;
		//半径にする
		param.m_HalfLen = m_Len * 0.5f;
		param.m_Radius = m_Diameter * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
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
	///	物理計算しない発射する球体
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
		//コリジョンを付ける（ボリューム取得のため）
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		//影をつける
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
			//じっとしている
			ptrTrans->SetPosition(Vec3(0, -20.0f, 0));
			return;
		}
		auto ptrColl = GetComponent<CollisionSphere>();
		//物理オブジェクトを持つ配列の取得
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
