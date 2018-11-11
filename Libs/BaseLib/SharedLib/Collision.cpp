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
		//衝突後の処理
		AfterCollision m_AfterCollision;
		//拘束解消パラメータ
		float m_SolverBodySpan;
		int m_SolverBodyMax;
		Impl() :
			m_Fixed(false),
			m_AfterCollision(AfterCollision::Auto),
			m_SolverBodySpan(0.001f),
			m_SolverBodyMax(2000)
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

	AfterCollision Collision::GetAfterCollision() const {
		return pImpl->m_AfterCollision;
	}
	void Collision::SetAfterCollision(AfterCollision a) {
		pImpl->m_AfterCollision = a;
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

	void Collision::SetExcludeCollisionGroup(const wstring& GroupStr) {
		pImpl->m_ExcludeCollisionGroup = GetGameObject()->GetStage()->GetSharedObjectGroup(GroupStr);
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


	float Collision::GetSolverBodySpan() const {
		return pImpl->m_SolverBodySpan;

	}
	int Collision::GetSolverBodyMax() const {
		return pImpl->m_SolverBodyMax;
	}
	void Collision::SetSolverBodySpan(float f) {
		pImpl->m_SolverBodySpan = f;
	}
	void Collision::SetSolverBodyMax(int i) {
		pImpl->m_SolverBodyMax = i;
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
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= TransPtr->GetWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::XScale:
			Ret.m_Radius = MatBase.scaleInMatrix().x * 0.5f;
			break;
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
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= TransPtr->GetBeforeWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::XScale:
			Ret.m_Radius = MatBase.scaleInMatrix().x * 0.5f;
			break;
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - DestChkSphere.m_Center;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = (DestChkSphere.m_Center + SrcChkSphere.m_Center) * 0.5f;
			GetCollisionManager()->InsertNewPair(pair);
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		CAPSULE DestCap = DestColl->GetCapsule();
		CAPSULE DestBeforeCap = DestColl->GetBeforeCapsule();

		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestBeforeCap, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			CAPSULE DestChkCapsule = DestBeforeCap;
			DestChkCapsule.SetCenter(DestChkCapsule.GetCenter() + DestVelocity * HitTime);
			bsm::Vec3 ret;
			HitTest::SPHERE_CAPSULE(SrcChkSphere, DestChkCapsule, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - ret;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret;
			GetCollisionManager()->InsertNewPair(pair);
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			bsm::Vec3 ret;
			HitTest::SPHERE_OBB(SrcChkSphere, DestChkObb, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - ret;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	SPHERE  CollisionSphere::GetEnclosingSphere()const {
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcSphere, SrcBeforSphere);
		return Src;
	}

	AABB CollisionSphere::GetEnclosingAabb()const {
		AABB SrcAabb = GetSphere().GetWrappedAABB();
		AABB SrcBeforAabb = GetBeforeSphere().GetWrappedAABB();
		AABB Src = HitTest::AABB_OR_AABB(SrcBeforAabb, SrcAabb);
		return Src;
	}


	bsm::Vec3 CollisionSphere::GetCenterPosition()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.m_Center;
	}

	void CollisionSphere::EscapeCollisionPair(CollisionPair& Pair) {
		if (GetAfterCollision() == AfterCollision::None || Pair.m_Dest.lock()->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		SPHERE SrcSphere = GetSphere();
		auto ShDestSphere = dynamic_pointer_cast<CollisionSphere>(Pair.m_Dest.lock());
		auto ShDestCapsule = dynamic_pointer_cast<CollisionCapsule>(Pair.m_Dest.lock());
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Pair.m_Dest.lock());
		if (ShDestSphere) {
			SPHERE DestSphere = ShDestSphere->GetSphere();
			bool IsHit = HitTest::SPHERE_SPHERE(SrcSphere, DestSphere);
			if (IsHit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				//現在のSPHEREをコピー
				auto NowSp = SrcSphere;
				//まず最大距離まで移動
				auto MaxFloat = GetSolverBodySpan() * GetSolverBodyMax();
				SrcSphere.m_Center += span * MaxFloat;
				if (!HitTest::SPHERE_SPHERE(SrcSphere, DestSphere)) {
					auto velo = -span * MaxFloat;
					float HitTime = 0;
					if (HitTest::CollisionTestSphereSphere(SrcSphere, velo, DestSphere, 0, MaxFloat, HitTime)) {
						SrcSphere.m_Center += velo * HitTime;
					}
					else {
						SrcSphere = NowSp;
					}
				}
				SrcSphere.m_Center.floor(3);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcSphere.m_Center);
			}
		}
		else if (ShDestCapsule) {
			CAPSULE DestCap = ShDestCapsule->GetCapsule();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				//現在のSPHEREをコピー
				auto NowSp = SrcSphere;
				//まず最大距離まで移動
				auto MaxFloat = GetSolverBodySpan() * GetSolverBodyMax();
				SrcSphere.m_Center += span * MaxFloat;
				if (!HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret)) {
					auto velo = -span * MaxFloat;
					float HitTime = 0;
					if (HitTest::CollisionTestSphereCapsule(SrcSphere, velo, DestCap, 0, MaxFloat, HitTime)) {
						SrcSphere.m_Center += velo * HitTime;
					}
					else {
						SrcSphere = NowSp;
					}
				}
				SrcSphere.m_Center.floor(3);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcSphere.m_Center);
			}
		}
		else if (ShDestObb) {
			OBB DestObb = ShDestObb->GetObb();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				//現在のSPHEREをコピー
				auto NowSp = SrcSphere;
				//まず最大距離まで移動
				auto MaxFloat = GetSolverBodySpan() * GetSolverBodyMax();
				SrcSphere.m_Center += span * MaxFloat;
				if (!HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret)) {
					auto velo = -span * MaxFloat;
					float HitTime = 0;
					if (HitTest::CollisionTestSphereObb(SrcSphere,velo, DestObb, 0, MaxFloat, HitTime)) {
						SrcSphere.m_Center += velo * HitTime;
					}
					else {
						SrcSphere = NowSp;
					}
				}
				SrcSphere.m_Center.floor(3);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcSphere.m_Center);
			}
		}
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
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		//		if (pImpl->m_FirstCalc) {
		pImpl->m_WorldMatrix = TransPtr->GetWorldMatrix();
		pImpl->m_WorldCapsule = CAPSULE(
			pImpl->m_MakedDiameter * 0.5f,
			bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
			bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
			pImpl->m_WorldMatrix);
		pImpl->m_FirstCalc = false;
		//		}
		return pImpl->m_WorldCapsule;
	}


	CAPSULE CollisionCapsule::GetBeforeCapsule() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		//		if (pImpl->m_FirstBeforeCalc) {
		pImpl->m_BeforeWorldMatrix = TransPtr->GetBeforeWorldMatrix();
		pImpl->m_BeforeWorldCapsule = CAPSULE(
			pImpl->m_MakedDiameter * 0.5f,
			bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
			bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
			pImpl->m_BeforeWorldMatrix);
		pImpl->m_FirstBeforeCalc = false;
		//		}
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
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
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			SrcChkCapsule.SetCenter(SrcChkCapsule.GetCenter() + SrcVelocity * HitTime);
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			bsm::Vec3 ret;
			HitTest::SPHERE_CAPSULE(DestChkSphere, SrcChkCapsule, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = ret - DestChkSphere.m_Center;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret;
			GetCollisionManager()->InsertNewPair(pair);
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionCapsule
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestBeforeCapsule, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;

			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			SrcChkCapsule.SetCenter(SrcChkCapsule.GetCenter() + SrcVelocity * HitTime);
			CAPSULE DestChkCapsule = DestBeforeCapsule;
			DestChkCapsule.SetCenter(DestChkCapsule.GetCenter() + DestVelocity * HitTime);

			bsm::Vec3 ret1, ret2;
			HitTest::CAPSULE_CAPSULE(SrcChkCapsule, DestChkCapsule, ret1, ret2);
			//接点へのベクトル
			//DestCapの線分とRetVec1の線分上の最近接点とRetVec1の法線
			bsm::Vec3 Start = DestChkCapsule.m_PointBottom;
			bsm::Vec3 End = DestChkCapsule.m_PointTop;
			float t;
			bsm::Vec3 RetVec;
			HitTest::ClosetPtPointSegment(ret1, Start, End, t, RetVec);
			pair.m_SrcHitNormal = ret1 - RetVec;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret1;
			GetCollisionManager()->InsertNewPair(pair);
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionObb
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			SrcChkCapsule.SetCenter(SrcChkCapsule.GetCenter() + SrcVelocity * HitTime);
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			bsm::Vec3 RetVec;
			HitTest::CAPSULE_OBB(SrcChkCapsule, DestChkObb, RetVec);
			//接点へのベクトル
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(RetVec, SrcChkCapsule.m_PointBottom, SrcChkCapsule.m_PointTop, t, SegPoint);
			pair.m_SrcHitNormal = SegPoint - RetVec;
			pair.m_CalcHitPoint = RetVec;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	SPHERE  CollisionCapsule::GetEnclosingSphere()const {
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcCapsule.GetWrappedSPHERE(), SrcBeforCapsule.GetWrappedSPHERE());
		return Src;
	}

	AABB CollisionCapsule::GetEnclosingAabb()const {
		AABB SrcAabb = GetCapsule().GetWrappedAABB();
		AABB SrcBeforAabb = GetBeforeCapsule().GetWrappedAABB();
		AABB Src = HitTest::AABB_OR_AABB(SrcBeforAabb, SrcAabb);
		return Src;
	}


	bsm::Vec3 CollisionCapsule::GetCenterPosition()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetCenter();
	}

	void CollisionCapsule::EscapeCollisionPair(CollisionPair& Pair) {
		if (GetAfterCollision() == AfterCollision::None || Pair.m_Dest.lock()->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		CAPSULE SrcCap = GetCapsule();
		auto ShDestSphere = dynamic_pointer_cast<CollisionSphere>(Pair.m_Dest.lock());
		auto ShDestCapsule = dynamic_pointer_cast<CollisionCapsule>(Pair.m_Dest.lock());
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Pair.m_Dest.lock());
		if (ShDestSphere) {
			auto Center = SrcCap.GetCenter();
			SPHERE DestSphere = ShDestSphere->GetSphere();
			bsm::Vec3 Ret;
			if (HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				int count = 0;
				while (HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
					Center += span * GetSolverBodySpan();
					SrcCap.SetCenter(Center);
					count++;
					if (count > GetSolverBodyMax()) {
						break;
					}
				}
				bsm::Vec3 v = SrcCap.GetCenter();
				v.floor(3);
				SrcCap.SetCenter(v);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcCap.GetCenter());
			}
		}
		else if (ShDestCapsule) {
			auto Center = SrcCap.GetCenter();
			CAPSULE DestCap = ShDestCapsule->GetCapsule();
			bsm::Vec3 Ret1, Ret2;
			if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				//現在のCAPSULEをコピー
				auto NowCap = SrcCap;
				//まず最大距離まで移動
				auto MaxFloat = GetSolverBodySpan() * GetSolverBodyMax();
				SrcCap.SetCenter(SrcCap.GetCenter() + span * MaxFloat);
				if (!HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
					auto velo = -span * MaxFloat;
					float HitTime = 0;
					if (HitTest::CollisionTestCapsuleCapsule(SrcCap, velo, DestCap, 0, MaxFloat, HitTime)) {
						SrcCap.SetCenter(SrcCap.GetCenter() + velo * HitTime);
					}
					else {
						SrcCap = NowCap;
					}
				}
				auto Center = SrcCap.GetCenter();
				Center.floor(3);
				SrcCap.SetCenter(Center);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcCap.GetCenter());
			}
		}
		else if (ShDestObb) {
			auto Center = SrcCap.GetCenter();
			OBB DestObb = ShDestObb->GetObb();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				//現在のCAPSULEをコピー
				auto NowCap = SrcCap;
				//まず最大距離まで移動
				auto MaxFloat = GetSolverBodySpan() * GetSolverBodyMax();
				SrcCap.SetCenter(SrcCap.GetCenter() + span * MaxFloat);
				if (!HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
					auto velo = -span * MaxFloat;
					float HitTime = 0;
					if (HitTest::CollisionTestCapsuleObb(SrcCap, velo, DestObb, 0, MaxFloat, HitTime)) {
						SrcCap.SetCenter(SrcCap.GetCenter() + velo * HitTime);
					}
					else {
						SrcCap = NowCap;
					}
				}
				auto Center = SrcCap.GetCenter();
				Center.floor(3);
				SrcCap.SetCenter(Center);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcCap.GetCenter());
			}
		}

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
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
//		if (pImpl->m_FirstCalc) {
			pImpl->m_WorldMatrix = TransPtr->GetWorldMatrix();
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_WorldMatrix;
			pImpl->m_WorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstCalc = false;
//		}
		return pImpl->m_WorldObb;
	}


	OBB CollisionObb::GetBeforeObb() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
//		if (pImpl->m_FirstBeforeCalc) {
			pImpl->m_BeforeWorldMatrix = TransPtr->GetBeforeWorldMatrix();
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_BeforeWorldMatrix;
			pImpl->m_BeforeWorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstBeforeCalc = false;
//		}
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
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
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			bsm::Vec3 ret;
			HitTest::SPHERE_OBB(DestChkSphere, SrcChkObb, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = ret - DestChkSphere.m_Center;
			pair.m_CalcHitPoint = ret;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
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
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			CAPSULE DestChkCapsule = DestBeforeCapsule;
			DestChkCapsule.SetCenter(DestChkCapsule.GetCenter() + DestVelocity * HitTime);
			bsm::Vec3 RetVec;
			HitTest::CAPSULE_OBB(DestChkCapsule, SrcChkObb, RetVec);
			//接点へのベクトル
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(RetVec, DestChkCapsule.m_PointBottom, DestChkCapsule.m_PointTop, t, SegPoint);
			pair.m_SrcHitNormal = RetVec - SegPoint;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = RetVec;
			GetCollisionManager()->InsertNewPair(pair);
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforeObb, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			bsm::Vec3 RetVec;
			//SrcのOBBとDestの最近接点を得る
			HitTest::ClosestPtPointOBB(SrcChkObb.m_Center, DestChkObb, RetVec);
			//接点へのベクトル
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkObb.m_Center - RetVec;
			pair.m_CalcHitPoint = RetVec;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	SPHERE  CollisionObb::GetEnclosingSphere()const {
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcObb.GetWrappedSPHERE(), SrcBeforeObb.GetWrappedSPHERE());
		return Src;
	}

	AABB CollisionObb::GetEnclosingAabb()const {
		AABB SrcAabb = GetObb().GetWrappedAABB();
		AABB SrcBeforAabb = GetBeforeObb().GetWrappedAABB();
		AABB Src = HitTest::AABB_OR_AABB(SrcBeforAabb, SrcAabb);
		return Src;
	}


	bsm::Vec3 CollisionObb::GetCenterPosition()const {
		OBB SrcObb = GetObb();
		return SrcObb.m_Center;
	}

	void CollisionObb::EscapeCollisionPair(CollisionPair& Pair) {
		if (GetAfterCollision() == AfterCollision::None || Pair.m_Dest.lock()->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		OBB SrcObb = GetObb();
		auto ShDestSphere = dynamic_pointer_cast<CollisionSphere>(Pair.m_Dest.lock());
		auto ShDestCapsule = dynamic_pointer_cast<CollisionCapsule>(Pair.m_Dest.lock());
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Pair.m_Dest.lock());
		if (ShDestSphere) {
			SPHERE DestSphere = ShDestSphere->GetSphere();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				int count = 0;
				while (HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret)) {
					SrcObb.m_Center += span * GetSolverBodySpan();
					count++;
					if (count > GetSolverBodyMax()) {
						break;
					}
				}
				SrcObb.m_Center.floor(3);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcObb.m_Center);
			}
		}
		else if (ShDestCapsule) {
			auto Center = SrcObb.m_Center;
			CAPSULE DestCapsule = ShDestCapsule->GetCapsule();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				int count = 0;
				while (HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret)) {
					SrcObb.m_Center += span * GetSolverBodySpan();
					count++;
					if (count > GetSolverBodyMax()) {
						break;
					}
				}
				SrcObb.m_Center.floor(3);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcObb.m_Center);

			}
		}
		else if (ShDestObb) {
			OBB DestObb = ShDestObb->GetObb();
			bool Ishit = HitTest::OBB_OBB(SrcObb, DestObb);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				//現在のOBBをコピー
				auto NowObb = SrcObb;
				//まず最大距離まで移動
				auto MaxFloat = GetSolverBodySpan() * GetSolverBodyMax();
				SrcObb.m_Center += span * MaxFloat;
				if (!HitTest::OBB_OBB(SrcObb, DestObb)) {
					auto velo = -span * MaxFloat;
					float HitTime = 0;
					if (HitTest::CollisionTestObbObb(SrcObb, velo, DestObb, 0, MaxFloat, HitTime)) {
						SrcObb.m_Center += velo * HitTime;
					}
					else {
						SrcObb = NowObb;
					}
				}
				SrcObb.m_Center.floor(3);
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//エスケープはリセット
				PtrTransform->ResetWorldPosition(SrcObb.m_Center);
			}
		}
	}


	void CollisionObb::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CUBE"));

	}


}
//end basecross

