/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	// 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//初期化
	void MultiSpark::OnCreate() {
	}

	void MultiSpark::OnUpdate() {
		for (auto ptrParticle : GetParticleVec()) {
			for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.z += 0.05f;
					if (rParticleSprite.m_Color.z >= 1.0f) {
						rParticleSprite.m_Color.z = 1.0f;
					}
				}
			}
		}
		//親クラスのOnUpdate()を呼ぶ
		MultiParticle::OnUpdate();
	}



	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ptrParticle = InsertParticle(4);
		ptrParticle->SetEmitterPos(Pos);
		ptrParticle->SetTextureResource(L"SPARK_TX");
		ptrParticle->SetMaxTime(0.5f);
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	///	複数の炎クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//初期化
	void MultiFire::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
	}

	void MultiFire::InsertFire(const Vec3& Pos) {
		auto ptrParticle = InsertParticle(4);
		ptrParticle->SetEmitterPos(Pos);
		ptrParticle->SetTextureResource(L"FIRE_TX");
		ptrParticle->SetMaxTime(0.5f);
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//構築と破棄
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

	//初期化
	void FixedBox::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);
		//OBB衝突j判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedBox");
		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetOwnShadowActive(true);

	}

	//--------------------------------------------------------------------------------------
	///	上下移動するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
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

	//初期化
	void MoveBox::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		AddTag(L"MoveBox");

		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vec3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vec3(-5.0f, -5.0f, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();



		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		auto Group = GetStage()->GetSharedObjectGroup(L"MoveBox");
		Group->IntoGroup(GetThis<GameObject>());


	}


	//--------------------------------------------------------------------------------------
	//	追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f),
		m_Force(0),
		m_Velocity(0)
	{
	}
	SeekObject::~SeekObject() {}

	//初期化
	void SeekObject::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_StartPos);
		ptrTransform->SetScale(0.125f, 0.25f, 0.25f);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"SeekGroup");

		//オブジェクトのグループを得る
		auto group = GetStage()->GetSharedObjectGroup(L"SeekGroup");
		//グループに自分自身を追加
		group->IntoGroup(GetThis<SeekObject>());
		//Obbの衝突判定をつける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetDrawActive(true);
		//重力をつける
		auto ptrGra = AddComponent<Gravity>();
		//分離行動をつける
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->SetGameObjectGroup(group);
		//影をつける
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}

	void SeekObject::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//ファイアの放出
		auto ptriFire = GetStage()->GetSharedGameObject<MultiFire>(L"MultiFire", false);
		if (ptriFire) {
			ptriFire->InsertFire(GetComponent<Transform>()->GetPosition());
		}

	}



	//操作
	void SeekObject::OnUpdate() {
		m_Force = Vec3(0);
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(1.0f);
	}

	void SeekObject::OnUpdate2() {
	}

	Vec3 SeekObject::GetTargetPos()const {
		auto targetPtr = GetStage()->GetSharedObject(L"Player");
		return targetPtr->GetComponent<Transform>()->GetPosition();
	}


	void SeekObject::ApplyForce() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * elapsedTime;
		auto ptrTransform = GetComponent<Transform>();
		auto pos = ptrTransform->GetPosition();
		pos += m_Velocity * elapsedTime;
		ptrTransform->SetPosition(pos);
	}



	//--------------------------------------------------------------------------------------
	//	プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto ptrSeek = Obj->GetBehavior<SeekSteering>();
		auto ptrSep = Obj->GetBehavior<SeparationSteering>();
		auto force = Obj->GetForce();
		force = ptrSeek->Execute(force, Obj->GetVelocity(), Obj->GetTargetPos());
		force += ptrSep->Execute(force);
		Obj->SetForce(force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}

	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto ptrArrive = Obj->GetBehavior<ArriveSteering>();
		auto ptrSep = Obj->GetBehavior<SeparationSteering>();
		auto force = Obj->GetForce();
		force = ptrArrive->Execute(force, Obj->GetVelocity(), Obj->GetTargetPos());
		force += ptrSep->Execute(force);
		Obj->SetForce(force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}


	//--------------------------------------------------------------------------------------
	//	数字のスクエア
	//--------------------------------------------------------------------------------------
	//構築と破棄
	NumberSquare::NumberSquare(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_SeekObject(SeekObjectPtr),
		m_Number(Number)
	{}
	NumberSquare::~NumberSquare() {}

	//初期化
	void NumberSquare::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//変更できるスクエアリソースを作成

			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, vertices, indices);
			//UV値の変更
			float from = ((float)m_Number) / 10.0f;
			float to = from + (1.0f / 10.0f);
			//左上頂点
			vertices[0].textureCoordinate = Vec2(from, 0);
			//右上頂点
			vertices[1].textureCoordinate = Vec2(to, 0);
			//左下頂点
			vertices[2].textureCoordinate = Vec2(from, 1.0f);
			//右下頂点
			vertices[3].textureCoordinate = Vec2(to, 1.0f);
			//頂点の型を変えた新しい頂点を作成
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"NUMBER_TX");
			SetAlphaActive(true);
			SetDrawLayer(1);
		}

	}


	//変化
	void NumberSquare::OnUpdate() {

		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをビルボードにする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}

	}


}
//end basecross
