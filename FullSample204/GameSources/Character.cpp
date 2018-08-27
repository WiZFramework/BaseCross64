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
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//OBB衝突j判定を付ける
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedBox");
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

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
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//Capsule衝突j判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedSphere");
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

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

	//初期化
	void BaseChara::OnCreate() {
		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<BaseChara>());
		//分離行動をつける
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//壁回避行動を付ける
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
		//障害物回避行動を付ける
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
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<BaseChara>(GetThis<BaseChara>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}
	//更新
	void BaseChara::OnUpdate() {
		m_Force = Vec3(0);
		//共通のステアリング1
		auto PtrWall = GetBehavior<WallAvoidanceSteering>();
		m_Force += PtrWall->Execute(m_Force, GetVelocity());
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
		//共通のステアリング2
		auto PtrSep = GetBehavior<SeparationSteering>();
		m_Force += PtrSep->Execute(m_Force);
		auto PtrAvoidance = GetBehavior<ObstacleAvoidanceSteering>();
		m_Force += PtrAvoidance->Execute(m_Force, GetVelocity());
		ApplyForce();
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(1.0f);
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
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(GetStartPos());
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"ObjGroup");

		//Obbの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();
		//重力をつける
		auto PtrGra = AddComponent<Gravity>();
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);
		//親クラスのOnCreateを呼ぶ
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
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(GetStartPos());
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//CollisionSphere衝突判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();
		//重力をつける
		auto PtrGra = AddComponent<Gravity>();
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetFogEnabled(true);
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		SetAlphaActive(true);
		//親クラスのOnCreateを呼ぶ
		BaseChara::OnCreate();

	}

	//操作
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
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(GetStartPos());
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		AddTag(L"ObjGroup");
		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<BaseChara>());
		//CollisionSphere衝突判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();
		//重力をつける
		auto PtrGra = AddComponent<Gravity>();
		//経路巡回を付ける
		auto PtrFollowPath = GetBehavior<FollowPathSteering>();
		list<Vec3> PathList = {
			Vec3(-10,0.125,10),
			Vec3(10,0.125,10),
			Vec3(-10,0.125,-10),
			Vec3(10,0.125,-10),
		};
		PtrFollowPath->SetPathList(PathList);
		PtrFollowPath->SetLooped(true);
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetFogEnabled(true);
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"CHECKER_TX");
		//親クラスのOnCreateを呼ぶ
		BaseChara::OnCreate();

	}

	//操作
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
