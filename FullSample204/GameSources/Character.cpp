/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


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
		ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

	}


	//--------------------------------------------------------------------------------------
	//	障害物球
	//--------------------------------------------------------------------------------------
	//構築と破棄
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

	//初期化
	void FixedSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);
		//Capsule衝突j判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedSphere");
		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"WALL_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

	}


	//--------------------------------------------------------------------------------------
	//	配置されるオブジェクトの親
	//--------------------------------------------------------------------------------------
	//構築と破棄
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

	//初期化
	void BaseChara::OnCreate() {
		//オブジェクトのグループを得る
		auto group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//グループに自分自身を追加
		group->IntoGroup(GetThis<BaseChara>());
		//分離行動をつける
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->SetGameObjectGroup(group);
		//壁回避行動を付ける
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
		//障害物回避行動を付ける
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
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<BaseChara>(GetThis<BaseChara>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}
	//更新
	void BaseChara::OnUpdate() {
		m_Force = Vec3(0);
		//共通のステアリング1
		auto ptrWall = GetBehavior<WallAvoidanceSteering>();
		m_Force += ptrWall->Execute(m_Force, GetVelocity());
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
		//共通のステアリング2
		auto ptrSep = GetBehavior<SeparationSteering>();
		m_Force += ptrSep->Execute(m_Force);
		auto ptrAvoidance = GetBehavior<ObstacleAvoidanceSteering>();
		m_Force += ptrAvoidance->Execute(m_Force, GetVelocity());
		ApplyForce();
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(1.0f);
	}





	//--------------------------------------------------------------------------------------
	//	プレイヤーから遠いときの移動
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
	//	プレイヤーから近いときの移動
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
	//	追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		BaseChara(StagePtr,StartPos)
	{
	}
	SeekObject::~SeekObject() {}

	//初期化
	void SeekObject::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStartPos());
		ptrTrans->SetScale(0.125f, 0.25f, 0.25f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"ObjGroup");

		//Obbの衝突判定をつける
		auto ptrColl = AddComponent<CollisionObb>();
		//重力をつける
		auto ptrGra = AddComponent<Gravity>();
		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);
		//親クラスのOnCreateを呼ぶ
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
	//	追跡する配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PursuitObject::PursuitObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		BaseChara(StagePtr, StartPos)
	{
	}
	PursuitObject::~PursuitObject() {}
	//初期化
	void PursuitObject::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStartPos());
		ptrTrans->SetScale(0.25f, 0.25f, 0.25f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		//重力をつける
		auto ptrGra = AddComponent<Gravity>();
		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
		//描画するテクスチャを設定
		ptrDraw->SetTextureResource(L"TRACE2_TX");
		SetAlphaActive(true);
		//親クラスのOnCreateを呼ぶ
		BaseChara::OnCreate();

	}

	//操作
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
	//	パスを巡回する配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FollowPathObject::FollowPathObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		BaseChara(StagePtr, StartPos)
	{
	}
	FollowPathObject::~FollowPathObject() {}
	//初期化
	void FollowPathObject::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStartPos());
		ptrTrans->SetScale(0.25f, 0.25f, 0.25f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//オブジェクトのグループを得る
		auto group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//グループに自分自身を追加
		group->IntoGroup(GetThis<BaseChara>());
		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		//重力をつける
		auto ptrGra = AddComponent<Gravity>();
		//経路巡回を付ける
		auto ptrFollowPath = GetBehavior<FollowPathSteering>();
		list<Vec3> pathList = {
			Vec3(-10,0.125,10),
			Vec3(10,0.125,10),
			Vec3(-10,0.125,-10),
			Vec3(10,0.125,-10),
		};
		ptrFollowPath->SetPathList(pathList);
		ptrFollowPath->SetLooped(true);
		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
		//描画するテクスチャを設定
		ptrDraw->SetTextureResource(L"CHECKER_TX");
		//親クラスのOnCreateを呼ぶ
		BaseChara::OnCreate();

	}

	//操作
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
