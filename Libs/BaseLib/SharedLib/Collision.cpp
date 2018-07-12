/*!
@file Collision.cpp
@brief 衝突判定コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	struct Collision::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Collision::Impl {
		bool m_Fixed;		//静止オブジェクトかどうか
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<GameObjectGroup> m_ExcludeCollisionGroup;	//判定から除外するグループ
		//判定から除外するタグ
		set<wstring> m_ExcludeCollisionTags;
		//ステート
		CollisionState m_CollisionState;
		//拘束を解く速度
		bsm::Vec3 m_SolverVelocity;
		Impl() :
			m_Fixed(false),
			m_SolverVelocity(0.0f)
		{
		}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	用途: 衝突判定コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Collision::Collision(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Collision::~Collision() {}

	bool Collision::GetFixed() const {
		return pImpl->m_Fixed;
	}
	bool Collision::IsFixed() const {
		return pImpl->m_Fixed;
	}
	void Collision::SetFixed(bool b) {
		pImpl->m_Fixed = b;
	}

	void Collision::SetCollisionStateFromTransform() {
		auto ShTrans = GetGameObject()->GetComponent<Transform>();
		pImpl->m_CollisionState.m_BeforeWorldMatrix = ShTrans->GetBeforeWorldMatrix();
		pImpl->m_CollisionState.m_WorldMatrix = ShTrans->GetWorldMatrix();
	}

	CollisionState& Collision::GetCollisionState() {
		return pImpl->m_CollisionState;
	}
	const CollisionState& Collision::GetCollisionState() const {
		return pImpl->m_CollisionState;
	}


	bsm::Vec3 Collision::GetSolverVelocity() const {
		return pImpl->m_SolverVelocity;
	}

	void Collision::SetSolverVelocity(const bsm::Vec3& velo) {
		pImpl->m_SolverVelocity = velo;
	}

	void Collision::AddSolverVelocity(const bsm::Vec3& velo) {
		pImpl->m_SolverVelocity += velo;
	}


	shared_ptr<GameObjectGroup> Collision::GetExcludeCollisionGroup() const {
		auto shptr = pImpl->m_ExcludeCollisionGroup.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	void Collision::SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_ExcludeCollisionGroup = Group;
	}


	bool Collision::FindExcludeCollisionTag(const wstring& tagstr) const {
		if (pImpl->m_ExcludeCollisionTags.find(tagstr) == pImpl->m_ExcludeCollisionTags.end()) {
			return false;
		}
		return true;
	}
	void  Collision::AddExcludeCollisionTag(const wstring& tagstr) {
		if (tagstr == L"") {
			//空白なら例外
			throw BaseException(
				L"設定するタグが空です",
				L"if (tagstr == L"")",
				L"Collision::AddExcludeCollisionTag()"
			);
		}
		pImpl->m_ExcludeCollisionTags.insert(tagstr);

	}
	void  Collision::RemoveExcludeCollisionTag(const wstring& tagstr) {
		pImpl->m_ExcludeCollisionTags.erase(tagstr);
	}



	bool Collision::IsExcludeCollisionObject(const shared_ptr<GameObject>& Obj) const {
		auto& tagset = Obj->GetTagSet();
		auto it = tagset.begin();
		while (it != tagset.end()) {
			if (FindExcludeCollisionTag(*it)) {
				return true;
			}
			it++;
		}
		auto shptr = pImpl->m_ExcludeCollisionGroup.lock();
		if (shptr) {
			auto Vec = shptr->GetGroupVector();
			for (auto& v : Vec) {
				auto shobj = v.lock();
				if (shobj && (shobj == Obj)) {
					return true;
				}
			}
		}
		return false;
	}

	//操作
	shared_ptr<CollisionManager> Collision::GetCollisionManager() const {
		return GetGameObject()->GetStage()->GetCollisionManager();
	}

	void Collision::OnUpdate() {
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionSphere::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionSphere::Impl {
		float m_MakedDiameter;					//作成時の直径
		//配列ボリュームと衝突時に衝突した配列を特定するインデックス
		size_t m_IsHitVolumeIndex;
		CalcScaling m_CalcScaling;
		Impl() :
			m_MakedDiameter(1.0f),
			m_IsHitVolumeIndex(0),
			m_CalcScaling(CalcScaling::YScale)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	用途: 球衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionSphere::CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionSphere::~CollisionSphere() {}

	void CollisionSphere::OnCreate() {
		SetDrawActive(false);
	}

	//アクセサ
	float CollisionSphere::GetMakedDiameter() const {
		return pImpl->m_MakedDiameter;
	}
	void CollisionSphere::SetMakedDiameter(float f) {
		pImpl->m_MakedDiameter = f;
	}
	float CollisionSphere::GetMakedRadius() const {
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionSphere::SetMakedRadius(float f) {
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	CalcScaling CollisionSphere::GetCalcScaling() const {
		return pImpl->m_CalcScaling;

	}
	void CollisionSphere::SetCalcScaling(CalcScaling s) {
		pImpl->m_CalcScaling = s;
	}


	SPHERE CollisionSphere::GetSphere() const {
		auto& state = GetCollisionState();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= state.m_WorldMatrix;
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}
	SPHERE CollisionSphere::GetBeforeSphere() const {
		auto& state = GetCollisionState();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= state.m_BeforeWorldMatrix;
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}

	void CollisionSphere::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionSphere>());
	}


	void CollisionSphere::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		//Sphere同士はCillisionWrappedSphere()は呼び出さない
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			bsm::Vec3 SrcHirCenter = SrcBeforSphere.m_Center + SrcVelocity * HitTime;
			bsm::Vec3 DestHirCenter = DestBeforeSphere.m_Center + DestVelocity * HitTime;
			//法線は拘束を解くための法線ベクトルのためSrcからDestを引く
			Pair.m_SrcNormal = SrcHirCenter - DestHirCenter;
			Pair.m_SrcNormal.normalize();
			Pair.m_EscSpeed = SrcVelocity.length();
			//衝突点はSrcがDest側に埋め込まれた最先端になる
			Pair.m_SrcHitPoint = SrcSphere.m_Center + (-Pair.m_SrcNormal) * SrcSphere.m_Radius;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		CAPSULE DestCap = DestColl->GetCapsule();
		CAPSULE DestBeforeCap = DestColl->GetBeforeCapsule();

		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestBeforeCap, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			SPHERE SrcHitSphere = SrcBeforSphere;
			SrcHitSphere.m_Center = SrcBeforSphere.m_Center + SrcVelocity * HitTime;
			CAPSULE DestHitCap = DestBeforeCap;
			DestHitCap.SetCenter(DestBeforeCap.GetCenter() + DestVelocity * HitTime);
			//点と線の最近接点
			float t;
			bsm::Vec3 d;
			HitTest::ClosetPtPointSegment(SrcHitSphere.m_Center,
				DestHitCap.m_PointTop, DestHitCap.m_PointBottom,t, d);
			//法線は拘束を解くための法線ベクトルのためSrcからDestを引く
			Pair.m_SrcNormal = SrcHitSphere.m_Center - d;
			Pair.m_SrcNormal.normalize();
			Pair.m_EscSpeed = SrcVelocity.length();
			//最近接点を得るための判定
			HitTest::SPHERE_CAPSULE(SrcHitSphere, DestHitCap, Pair.m_SrcHitPoint);
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}



	void CollisionSphere::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			SPHERE SrcHitSphere = SrcBeforSphere;
			SrcHitSphere.m_Center = SrcBeforSphere.m_Center + SrcVelocity * HitTime;
			OBB DestHitObb = DestBeforeObb;
			DestHitObb.m_Center = DestHitObb.m_Center + DestVelocity * HitTime;

			//最近接点を得るための判定
			HitTest::SPHERE_OBB(SrcHitSphere, DestHitObb, Pair.m_SrcHitPoint);
			Pair.m_SrcNormal = SrcHitSphere.m_Center - Pair.m_SrcHitPoint;
			Pair.m_SrcNormal.normalize();
			//拘束を解くスピードは現在の速度
//			Pair.m_EscSpeed = 0.01f;
			Pair.m_EscSpeed = SrcVelocity.length();


			//スライドさせる
			auto Velo = Slide(SrcVelocity, Pair.m_SrcNormal);
			auto Pos = SrcHitSphere.m_Center;
			Pos += Velo * (ElapsedTime - HitTime);
			bsm::Vec3 v = (bsm::Vec3)XMVector3AngleBetweenNormals(bsm::Vec3(0,1.0f,0), Pair.m_SrcNormal);
			if (v.x > XM_PIDIV4) {
//				Pos += Pair.m_SrcNormal * 0.001f;
			}
			auto& state = GetCollisionState();

			auto DepsSpan = Pos - SrcHitSphere.m_Center;
			Pair.m_Deps = abs(bsm::dot(Pair.m_SrcNormal, DepsSpan));

			bsm::Vec3 d;
			SrcHitSphere.m_Center = Pos;
			PtrTransform->SetWorldPosition(Pos);
			if (HitTest::SPHERE_OBB(SrcHitSphere, DestHitObb, d)) {
				state.SetPosition(Pos);
				GetCollisionManager()->SetEnterPair(Pair);
			}


		}
	}

	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		SPHERE sp2 = DestColl->GetSphere();
		//接点へのベクトル
		Ret = sp2.m_Center - sp.m_Center;
		Ret.normalize();
	}


	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		CAPSULE cap = DestColl->GetCapsule();
		bsm::Vec3 RetPoint;
		HitTest::SPHERE_CAPSULE(sp, cap, RetPoint);
		//接点へのベクトル
		Ret = RetPoint - sp.m_Center;
		Ret.normalize();
	}


	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		OBB obb = DestColl->GetObb();
		bsm::Vec3 RetPoint;
		HitTest::SPHERE_OBB(sp, obb, RetPoint);
		//接点へのベクトル
		Ret = RetPoint - sp.m_Center;
		Ret.normalize();
	}


	SPHERE  CollisionSphere::GetEnclosingSphere()const {
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcSphere, SrcBeforSphere);
		return Src;
	}

	bool CollisionSphere::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionSphere::GetWrappingAABB()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.GetWrappedAABB();
	}

	bsm::Vec3 CollisionSphere::GetCenterPosition()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.m_Center;
	}

	void CollisionSphere::OnDraw() {
		GenericDraw Draw;
		bsm::Mat4x4 MeshToTransformMatrix;

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Scale = PtrTransform->GetScale();
		bsm::Vec3 CollScale(Scale.x, Scale.x, Scale.x);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			CollScale = bsm::Vec3(Scale.y, Scale.y, Scale.y);
			break;
		case CalcScaling::ZScale:
			CollScale = bsm::Vec3(Scale.z, Scale.z, Scale.z);
			break;
		default:
			break;
		}
		bsm::Vec3 ColcScale(CollScale.x / Scale.x, CollScale.y / Scale.y, CollScale.z / Scale.z);
		bsm::Mat4x4 mat;
		mat.scale(ColcScale);

		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SPHERE"),
			mat);
	}

	//--------------------------------------------------------------------------------------
	//	struct CollisionCapsule::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionCapsule::Impl {
		float m_MakedDiameter;			//作成時の直径
		float m_MakedHeight;			//作成時高さ
		//配列ボリュームと衝突時に衝突した配列を特定するインデックス
		size_t m_IsHitVolumeIndex;

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		CAPSULE m_BeforeWorldCapsule;
		CAPSULE m_WorldCapsule;

		bool m_FirstBeforeCalc;
		bool m_FirstCalc;

		Impl() :
			m_MakedDiameter(1.0f),
			m_MakedHeight(1.0f),
			m_IsHitVolumeIndex(0),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_BeforeWorldCapsule(),
			m_WorldCapsule(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true)

		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	用途: カプセル衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionCapsule::CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionCapsule::~CollisionCapsule() {}

	//初期化
	void CollisionCapsule::OnCreate() {
		SetDrawActive(false);
	}

	//アクセサ
	float CollisionCapsule::GetMakedDiameter() const {
		return pImpl->m_MakedDiameter;
	}
	void CollisionCapsule::SetMakedDiameter(float f) {
		pImpl->m_MakedDiameter = f;
	}
	float CollisionCapsule::GetMakedRadius() const {
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionCapsule::SetMakedRadius(float f) {
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	float CollisionCapsule::GetMakedHeight() const {
		return pImpl->m_MakedHeight;
	}
	void CollisionCapsule::SetMakedHeight(float f) {
		pImpl->m_MakedHeight = f;
	}

	CAPSULE CollisionCapsule::GetCapsule() const {
		auto& state = GetCollisionState();
		if (pImpl->m_FirstCalc) {
			pImpl->m_WorldMatrix = state.m_WorldMatrix;
			pImpl->m_WorldCapsule = CAPSULE(
				pImpl->m_MakedDiameter * 0.5f,
				bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
				bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
				pImpl->m_WorldMatrix);
			pImpl->m_FirstCalc = false;
		}
		return pImpl->m_WorldCapsule;
	}

	CAPSULE CollisionCapsule::GetBeforeCapsule() const {
		auto& state = GetCollisionState();
		if (pImpl->m_FirstBeforeCalc) {
			pImpl->m_BeforeWorldMatrix = state.m_BeforeWorldMatrix;
			pImpl->m_BeforeWorldCapsule = CAPSULE(
				pImpl->m_MakedDiameter * 0.5f,
				bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
				bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
				pImpl->m_BeforeWorldMatrix);
			pImpl->m_FirstBeforeCalc = false;
		}
		return pImpl->m_BeforeWorldCapsule;
	}

	void CollisionCapsule::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionCapsule>());
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHEREとCAPSULEの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcBeforCapsule, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionCapsule
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestBeforeCapsule, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionObb
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}


	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE cap = GetCapsule();
		SPHERE sp = DestColl->GetSphere();
		bsm::Vec3 RetVec;
		HitTest::SPHERE_CAPSULE(sp, cap, RetVec);
		//接点へのベクトル
		Ret = sp.m_Center - RetVec;
		Ret.normalize();
	}


	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE SrcCap = GetCapsule();
		CAPSULE DestCap = DestColl->GetCapsule();
		bsm::Vec3 RetVec1, RetVec2;
		HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, RetVec1, RetVec2);
		//接点へのベクトル
		//DestCapの線分とRetVec1の線分上の最近接点とRetVec1の法線
		bsm::Vec3 Start = DestCap.m_PointBottom;
		bsm::Vec3 End = DestCap.m_PointTop;
		float t;
		bsm::Vec3 RetVec;
		HitTest::ClosetPtPointSegment(RetVec1,
			Start, End, t, RetVec);
		Ret = RetVec - RetVec1;
		Ret.normalize();
	}

	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE SrcCap = GetCapsule();
		OBB DestObb = DestColl->GetObb();
		bsm::Vec3 RetVec;
		HitTest::CAPSULE_OBB(SrcCap, DestObb, RetVec);
		//接点へのベクトル
		float t;
		bsm::Vec3 SegPoint;
		HitTest::ClosetPtPointSegment(RetVec, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
		Ret = RetVec - SegPoint;
		Ret.normalize();
	}



	SPHERE  CollisionCapsule::GetEnclosingSphere()const {
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcCapsule.GetWrappedSPHERE(), SrcBeforCapsule.GetWrappedSPHERE());
		return Src;
	}

	bool CollisionCapsule::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionCapsule::GetWrappingAABB()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetWrappedAABB();
	}

	bsm::Vec3 CollisionCapsule::GetCenterPosition()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetCenter();
	}



	void CollisionCapsule::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CAPSULE"));
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionObb::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionObb::Impl {
		float m_Size;					//作成時のサイズ
		float m_ChkOnUnderLaySize;

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		OBB m_BeforeWorldObb;
		OBB m_WorldObb;

		bool m_FirstBeforeCalc;
		bool m_FirstCalc;


		Impl() :
			m_Size(1.0f),
			m_ChkOnUnderLaySize(0.1f),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_BeforeWorldObb(),
			m_WorldObb(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	用途: Obb衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionObb::CollisionObb(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionObb::~CollisionObb() {}

	//初期化
	void CollisionObb::OnCreate() {
		SetDrawActive(false);
	}


	//アクセサ
	float CollisionObb::GetMakedSize() const {
		return pImpl->m_Size;
	}
	void CollisionObb::SetMakedSize(float f) {
		pImpl->m_Size = f;
	}

	OBB CollisionObb::GetObb() const {
		auto& state = GetCollisionState();
		if (pImpl->m_FirstCalc) {
			pImpl->m_WorldMatrix = state.m_WorldMatrix;
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_WorldMatrix;
			pImpl->m_WorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstCalc = false;
		}
		return pImpl->m_WorldObb;
	}
	OBB CollisionObb::GetBeforeObb() const {
		auto& state = GetCollisionState();
		if (pImpl->m_FirstBeforeCalc) {
			pImpl->m_BeforeWorldMatrix = state.m_BeforeWorldMatrix;
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_BeforeWorldMatrix;
			pImpl->m_BeforeWorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstBeforeCalc = false;
		}
		return pImpl->m_BeforeWorldObb;
	}

	void CollisionObb::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionObb>());
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//相手
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHEREとOBBの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforeSphere, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {

		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//相手
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		//SPHEREとOBBの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforeCapsule, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}


	void CollisionObb::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {

		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = COLLISION_TIME_STEP;
//		float ElapsedTime = App::GetApp()->GetElapsedTime();
		
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforObb, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			//衝突相手の登録
			CollisionHitPair Pair;
			Pair.m_Src = GetThis<Collision>();
			Pair.m_Dest = DestColl;
			GetCollisionManager()->SetEnterPair(Pair);
		}
	}

	void CollisionObb::GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		SPHERE sp = DestColl->GetSphere();
		bsm::Vec3 RetVec;
		HitTest::SPHERE_OBB(sp, obb, RetVec);
		//接点へのベクトル
		//逆になる
		Ret = sp.m_Center - RetVec;
		Ret.normalize();
	}


	void CollisionObb::GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		CAPSULE cap = DestColl->GetCapsule();
		bsm::Vec3 RetVec;
		HitTest::CAPSULE_OBB(cap, obb, RetVec);
		//接点へのベクトル
		//逆になる
		Ret = cap.GetCenter() - RetVec;
		Ret.normalize();
	}


	void CollisionObb::GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		OBB obb2 = DestColl->GetObb();
		bsm::Vec3 RetVec;
		//SrcのOBBとDestの最近接点を得る
		HitTest::ClosestPtPointOBB(obb.m_Center, obb2, RetVec);
		//接点へのベクトル
		Ret = RetVec - obb.m_Center;
		Ret.normalize();
	}

	SPHERE  CollisionObb::GetEnclosingSphere()const {
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcObb.GetWrappedSPHERE(), SrcBeforeObb.GetWrappedSPHERE());
		return Src;
	}

	bool CollisionObb::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionObb::GetWrappingAABB()const {
		OBB SrcObb = GetObb();
		return SrcObb.GetWrappedAABB();
	}

	bsm::Vec3 CollisionObb::GetCenterPosition()const {
		OBB SrcObb = GetObb();
		return SrcObb.m_Center;
	}


	void CollisionObb::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CUBE"));

	}


}
//end basecross

