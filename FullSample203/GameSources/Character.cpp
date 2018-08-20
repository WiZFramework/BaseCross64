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
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"SeekGroup");

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<SeekObject>());
		//Obbの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();
		//重力をつける
		auto PtrGra = AddComponent<Gravity>();
		//分離行動をつける
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}


	//操作
	void SeekObject::OnUpdate() {
		m_Force = Vec3(0);
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(1.0f);
	}

	void SeekObject::OnUpdate2() {
	}

	Vec3 SeekObject::GetTargetPos()const {
		auto TargetPtr = GetStage()->GetSharedObject(L"Player");
		return TargetPtr->GetComponent<Transform>()->GetPosition();
	}


	void SeekObject::ApplyForce() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * ElapsedTime;
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		Pos += m_Velocity * ElapsedTime;
		PtrTransform->SetPosition(Pos);
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
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		auto PtrSep = Obj->GetBehavior<SeparationSteering>();
		auto Force = Obj->GetForce();
		Force = PtrSeek->Execute(Force, Obj->GetVelocity(), Obj->GetTargetPos());
		Force += PtrSep->Execute(Force);
		Obj->SetForce(Force);
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
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		auto PtrSep = Obj->GetBehavior<SeparationSteering>();
		auto Force = Obj->GetForce();
		Force = PtrArrive->Execute(Force, Obj->GetVelocity(), Obj->GetTargetPos());
		Force += PtrSep->Execute(Force);
		Obj->SetForce(Force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//構築と破棄
	ActionObject::ActionObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos) {
	}
	ActionObject::~ActionObject() {}


	//初期化
	void ActionObject::OnCreate() {
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(m_StartPos);
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		//CollisionSphere衝突判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();
		//Fixedを設定する
		PtrColl->SetFixed(true);

		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vec3(0, XM_PI, 0));
		PtrAction->AddRotateInterval(1.0f);
		PtrAction->AddMoveBy(1.0f, Vec3(2.0f, 0, 0));
		PtrAction->AddMoveBy(0.5f, Vec3(0, 1.0f, -1.0f), Lerp::Linear, Lerp::EaseOut, Lerp::Linear);
		PtrAction->AddMoveBy(0.5f, Vec3(0, -1.0f, -1.0f), Lerp::Linear, Lerp::Easein, Lerp::Linear);
		PtrAction->AddMoveBy(2.0f, Vec3(-4.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vec3(0, 0, 2.0f));
		PtrAction->AddMoveBy(1.0f, Vec3(2.0f, 0, 0));

		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();

		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//テクスチャの登録
		PtrDraw->SetTextureResource(L"WALL_TX");

	}





}
//end basecross
