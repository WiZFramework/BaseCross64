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
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
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
	//	class ChildSphere : public GameObject;
	//--------------------------------------------------------------------------------------
	ChildSphere::ChildSphere(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<GameObject>& Parent,
		const Vec3& VecToParent
	):
		GameObject(StagePtr),
		m_Parent(Parent),
		m_VecToParent(VecToParent)
	{}

	ChildSphere::~ChildSphere() {}

	void ChildSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(0.25f));
		ptrTrans->SetQuaternion(Quat());

		//CollisionSphere衝突判定を付ける
		AddComponent<CollisionSphere>();

		//重力をつける
		auto ptrGra = AddComponent<Gravity>();
		//無効にしておく
		ptrGra->SetUpdateActive(false);

		auto ptrParent = m_Parent.lock();
		if (ptrParent) {
			auto posTarget = ptrParent->GetComponent<Transform>()->GetPosition();
			posTarget += m_VecToParent;
			ptrTrans->SetPosition(posTarget);
		}
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
		//描画するテクスチャを設定
		ptrDraw->SetTextureResource(L"TRACE_TX");
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<ChildSphere>(GetThis<ChildSphere>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(DefaultState::Instance());

	}

	//親を追いかける処理
	void ChildSphere::SeekParent() {
		auto ptrTrans = GetComponent<Transform>();
		auto pos = ptrTrans->GetPosition();
		auto ptrParent = m_Parent.lock();
		if (ptrParent) {
			auto matParent = ptrParent->GetComponent<Transform>()->GetWorldMatrix();
			matParent.scaleIdentity();
			Mat4x4 mat;
			mat.affineTransformation(
				Vec3(1.0),
				Vec3(0.0),
				Vec3(0.0),
				m_VecToParent
			);
			mat *= matParent;

			auto posTarget = mat.transInMatrix();
			auto v = Lerp::CalculateLerp(pos, posTarget, 0.0f, 1.0f, 0.2f, Lerp::rate::Linear);
			ptrTrans->SetPosition(v);
			ptrTrans->SetQuaternion(mat.quatInMatrix());
		}
	}

	//宙刷りになってるかどうかのチェック
	bool ChildSphere::IsHang() {
		auto collMan = GetComponent<Collision>()->GetCollisionManager();
		if (!collMan->IsInKeep(GetComponent<Collision>())) {
			return true;
		}
		return false;
	}



	void ChildSphere::OnUpdate() {
		//ステートマシンのUpdateを行う
		m_StateMachine->Update();

	}

	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<ChildSphere>;
	//--------------------------------------------------------------------------------------
	shared_ptr<DefaultState> DefaultState::Instance() {
		static shared_ptr<DefaultState> instance(new DefaultState);
		return instance;
	}
	void DefaultState::Enter(const shared_ptr<ChildSphere>& Obj) {
	}
	void DefaultState::Execute(const shared_ptr<ChildSphere>& Obj) {
		Obj->SeekParent();
		if (Obj->IsHang()) {
			Obj->GetStateMachine()->ChangeState(GravState::Instance());
		}
	}

	void DefaultState::Exit(const shared_ptr<ChildSphere>& Obj) {
	}


	//--------------------------------------------------------------------------------------
	//	class GravState : public ObjState<ChildSphere>;
	//--------------------------------------------------------------------------------------
	shared_ptr<GravState> GravState::Instance() {
		static shared_ptr<GravState> instance(new GravState);
		return instance;
	}
	void GravState::Enter(const shared_ptr<ChildSphere>& Obj) {
		auto grav = Obj->GetComponent<Gravity>();
		grav->SetGravityVerocityZero();
		grav->SetUpdateActive(true);
	}
	void GravState::Execute(const shared_ptr<ChildSphere>& Obj) {
		Obj->SeekParent();
		if (!Obj->IsHang()) {
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}

	void GravState::Exit(const shared_ptr<ChildSphere>& Obj) {
		Obj->GetComponent<Gravity>()->SetUpdateActive(false);
	}


}
//end basecross
