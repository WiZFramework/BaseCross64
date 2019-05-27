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
		//スリープチェック用
		bool m_SleepActive;
		bsm::Mat4x4 m_SleepCheckWorldMatrix;
		float m_SleepCheckTimer;
		float m_SleepTime;
		bool m_IsSleep;
		Impl() :
			m_Fixed(false),
			m_AfterCollision(AfterCollision::Auto),
			m_SleepActive(false),
			m_SleepCheckWorldMatrix(),
			m_SleepCheckTimer(0.0f),
			m_SleepTime(2.0f),
			m_IsSleep(false)
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

	bsm::Vec3 Collision::GetVelocity() const {
		return GetGameObject()->GetComponent<Transform>()->GetVelocity();
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

	bool Collision::IsSleepActive()const {
		return pImpl->m_SleepActive;
	}

	void Collision::SetSleepActive(bool b) {
		pImpl->m_SleepActive = b;
	}

	float Collision::GetSleepTime() const {
		return pImpl->m_SleepTime;
	}

	void Collision::SetSleepTime(float f) {
		if (!IsSleepActive() || IsFixed()) {
			return;
		}
		pImpl->m_SleepTime = true;
	}

	void Collision::SleepCheckSet() {
		if (!IsSleepActive() || IsFixed()) {
			return;
		}
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		if (WorldMat.nearEqual(pImpl->m_SleepCheckWorldMatrix,0.01)) {
			float elapsedTime = App::GetApp()->GetElapsedTime();
			pImpl->m_SleepCheckTimer += elapsedTime;
			if (pImpl->m_SleepCheckTimer >= pImpl->m_SleepTime) {
				pImpl->m_IsSleep = true;
			}
		}
		else {
			pImpl->m_IsSleep = false;
			pImpl->m_SleepCheckWorldMatrix = WorldMat;
			pImpl->m_SleepCheckTimer = 0.0f;
		}
	}

	bool Collision::IsSleep()const {
		if (!IsSleepActive() || IsFixed()) {
			return false;
		}
		return pImpl->m_IsSleep;
	}

	void Collision::WakeUp() {
		if (!IsSleepActive() || IsFixed()) {
			return;
		}
		if (pImpl->m_IsSleep) {
			auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
			pImpl->m_IsSleep = false;
			pImpl->m_SleepCheckWorldMatrix = WorldMat;
			pImpl->m_SleepCheckTimer = 0.0f;
		}
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

	bool CollisionSphere::SimpleCollisionCall(const shared_ptr<Collision>& Src) {
		return Src->SimpleCollision(GetThis<CollisionSphere>());
	}


	bool CollisionSphere::SimpleCollision(const shared_ptr<CollisionSphere>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestSp = DestColl->GetSphere();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestSp.GetWrappedAABB())) {
			return false;
		}
		if (HitTest::SPHERE_SPHERE(SrcSp, DestSp)) {
			return true;
		}
		return false;
	}

	bool CollisionSphere::SimpleCollision(const shared_ptr<CollisionCapsule>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestCap = DestColl->GetCapsule();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestCap.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_CAPSULE(SrcSp, DestCap, d)) {
			return true;
		}
		return false;

	}

	bool CollisionSphere::SimpleCollision(const shared_ptr<CollisionObb>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestObb = DestColl->GetObb();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestObb.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_OBB(SrcSp, DestObb, d)) {
			return true;
		}
		return false;
	}

	bool CollisionSphere::SimpleCollision(const shared_ptr<CollisionRect>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestCol = DestColl->GetColRect();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestCol.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_COLRECT(SrcSp, DestCol, d)) {
			return true;
		}
		return false;
	}


	void CollisionSphere::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionSphere>());
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {

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
		//簡易的な判定
		if (!HitTest::SPHERE_SPHERE(SrcSphere, DestSphere)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkSphere.m_Center;
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkSphere.m_Center;
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - DestChkSphere.m_Center;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = (DestChkSphere.m_Center + SrcChkSphere.m_Center) * 0.5f;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {

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
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_CAPSULE(SrcSphere, DestCap,ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestBeforeCap, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkSphere.m_Center;
			CAPSULE DestChkCapsule = DestBeforeCap;
			pair.m_DestCalcHitCenter = DestChkCapsule.GetCenter() + DestVelocity * HitTime;
			DestChkCapsule.SetCenter(pair.m_DestCalcHitCenter);
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
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_OBB(SrcSphere, DestObb, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkSphere.m_Center;
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkObb.m_Center;
			bsm::Vec3 ret;
			HitTest::SPHERE_OBB(SrcChkSphere, DestChkObb, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - ret;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {

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
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_COLRECT(SrcSphere, DestRect, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereRect(SrcBeforSphere, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkSphere.m_Center;
			COLRECT DestChkRect = DestBeforeRect;
			DestBeforeRect.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkRect.m_Center;
			bsm::Vec3 ret;
			HitTest::SPHERE_COLRECT(SrcChkSphere, DestChkRect, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - ret;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	bsm::Vec3 CollisionSphere::GetCenterPosition()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.m_Center;
	}

	AABB CollisionSphere::GetEnclosingAABB()const {
		AABB enc = GetBeforeSphere().GetWrappedAABB();
		enc.UnionAABB(GetSphere().GetWrappedAABB());
		return enc;
	}

	AABB CollisionSphere::GetWrappedAABB()const {
		return GetSphere().GetWrappedAABB();
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

	bool CollisionCapsule::SimpleCollisionCall(const shared_ptr<Collision>& Src) {
		return Src->SimpleCollision(GetThis<CollisionCapsule>());
	}

	bool CollisionCapsule::SimpleCollision(const shared_ptr<CollisionSphere>& DestColl) {
		auto SrcCap = GetCapsule();
		auto DestSp = DestColl->GetSphere();
		if (!HitTest::AABB_AABB(SrcCap.GetWrappedAABB(), DestSp.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_CAPSULE(DestSp, SrcCap, d)) {
			return true;
		}
		return false;
	}

	bool CollisionCapsule::SimpleCollision(const shared_ptr<CollisionCapsule>& DestColl) {
		auto SrcCap = GetCapsule();
		auto DestCap = DestColl->GetCapsule();
		if (!HitTest::AABB_AABB(SrcCap.GetWrappedAABB(), DestCap.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d1, d2;
		if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, d1, d2)) {
			return true;
		}
		return false;
	}

	bool CollisionCapsule::SimpleCollision(const shared_ptr<CollisionObb>& DestColl) {
		auto SrcCap = GetCapsule();
		auto DestObb = DestColl->GetObb();
		if (!HitTest::AABB_AABB(SrcCap.GetWrappedAABB(), DestObb.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::CAPSULE_OBB(SrcCap, DestObb, d)) {
			return true;
		}
		return false;
	}

	bool CollisionCapsule::SimpleCollision(const shared_ptr<CollisionRect>& DestColl) {
		auto SrcCap = GetCapsule();
		auto DestRect = DestColl->GetColRect();
		if (!HitTest::AABB_AABB(SrcCap.GetWrappedAABB(), DestRect.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::CAPSULE_COLRECT(SrcCap, DestRect, d)) {
			return true;
		}
		return false;
	}



	void CollisionCapsule::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionCapsule>());
	}


	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {

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
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_CAPSULE(DestSphere, SrcCapsule, ret)) {
			return;
		}
		//SPHEREとCAPSULEの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcBeforCapsule, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			pair.m_SrcCalcHitCenter = SrcChkCapsule.GetCenter() + SrcVelocity * HitTime;
			SrcChkCapsule.SetCenter(pair.m_SrcCalcHitCenter);
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkSphere.m_Center;
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
		//簡易的な判定
		bsm::Vec3 ret1,ret2;
		if (!HitTest::CAPSULE_CAPSULE(SrcCapsule, DestCapsule, ret1,ret2)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestBeforeCapsule, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			pair.m_SrcCalcHitCenter = SrcChkCapsule.GetCenter() + SrcVelocity * HitTime;
			SrcChkCapsule.SetCenter(pair.m_SrcCalcHitCenter);
			CAPSULE DestChkCapsule = DestBeforeCapsule;
			pair.m_DestCalcHitCenter = DestChkCapsule.GetCenter() + DestVelocity * HitTime;
			DestChkCapsule.SetCenter(pair.m_DestCalcHitCenter);
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
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::CAPSULE_OBB(SrcCapsule, DestObb, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			pair.m_SrcCalcHitCenter = SrcChkCapsule.GetCenter() + SrcVelocity * HitTime;
			SrcChkCapsule.SetCenter(pair.m_SrcCalcHitCenter);
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkObb.m_Center;
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

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::CAPSULE_COLRECT(SrcCapsule, DestRect, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleRect(SrcBeforCapsule, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			pair.m_SrcCalcHitCenter = SrcChkCapsule.GetCenter() + SrcVelocity * HitTime;
			SrcChkCapsule.SetCenter(pair.m_SrcCalcHitCenter);
			COLRECT DestChkRect = DestBeforeRect;
			DestChkRect.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkRect.m_Center;
			HitTest::CAPSULE_COLRECT(SrcChkCapsule, DestChkRect, ret);
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkCapsule.GetCenter() - ret;
			pair.m_SrcHitNormal.normalize();
			pair.m_CalcHitPoint = ret;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	bsm::Vec3 CollisionCapsule::GetCenterPosition()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetCenter();
	}

	AABB CollisionCapsule::GetEnclosingAABB()const {
		AABB enc = GetBeforeCapsule().GetWrappedAABB();
		enc.UnionAABB(GetCapsule().GetWrappedAABB());
		return enc;
	}

	AABB CollisionCapsule::GetWrappedAABB()const {
		return GetCapsule().GetWrappedAABB();
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

	bool CollisionObb::SimpleCollisionCall(const shared_ptr<Collision>& Src) {
		return Src->SimpleCollision(GetThis<CollisionObb>());
	}

	bool CollisionObb::SimpleCollision(const shared_ptr<CollisionSphere>& DestColl) {
		auto SrcObb = GetObb();
		bsm::Vec3 d;
		auto DestSp = DestColl->GetSphere();
		if (!HitTest::AABB_AABB(SrcObb.GetWrappedAABB(), DestSp.GetWrappedAABB())) {
			return false;
		}
		if (HitTest::SPHERE_OBB(DestSp, SrcObb, d)) {
			return true;
		}
		return false;
	}

	bool CollisionObb::SimpleCollision(const shared_ptr<CollisionCapsule>& DestColl) {
		auto SrcObb = GetObb();
		auto DestCap = DestColl->GetCapsule();
		if (!HitTest::AABB_AABB(SrcObb.GetWrappedAABB(), DestCap.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::CAPSULE_OBB(DestCap, SrcObb, d)) {
			return true;
		}
		return false;
	}

	bool CollisionObb::SimpleCollision(const shared_ptr<CollisionObb>& DestColl) {
		auto SrcObb = GetObb();
		auto DestObb = DestColl->GetObb();
		if (!HitTest::AABB_AABB(SrcObb.GetWrappedAABB(), DestObb.GetWrappedAABB())) {
			return false;
		}
		if (HitTest::OBB_OBB(SrcObb, DestObb)) {
			return true;
		}
		return false;
	}

	bool CollisionObb::SimpleCollision(const shared_ptr<CollisionRect>& DestColl) {
		auto SrcObb = GetObb();
		auto DestRect = DestColl->GetColRect();
		if (!HitTest::AABB_AABB(SrcObb.GetWrappedAABB(), DestRect.GetWrappedAABB())) {
			return false;
		}
		if (HitTest::OBB_COLRECT(SrcObb, DestRect)) {
			return true;
		}
		return false;
	}


	void CollisionObb::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionObb>());
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {

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
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_OBB(DestSphere, SrcObb, ret)) {
			return;
		}
		//SPHEREとOBBの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforeSphere, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkObb.m_Center;
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkSphere.m_Center;
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
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::CAPSULE_OBB(DestCapsule, SrcObb, ret)) {
			return;
		}
		//SPHEREとOBBの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforeCapsule, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkObb.m_Center;
			CAPSULE DestChkCapsule = DestBeforeCapsule;
			pair.m_DestCalcHitCenter = DestChkCapsule.GetCenter() + DestVelocity * HitTime;
			DestChkCapsule.SetCenter(pair.m_DestCalcHitCenter);
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
		//簡易的な判定
		if (!HitTest::OBB_OBB(SrcObb, DestObb)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforeObb, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkObb.m_Center;
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkObb.m_Center;
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

	void CollisionObb::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {

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
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		//簡易的な判定
		if (!HitTest::OBB_COLRECT(SrcObb, DestRect)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbRect(SrcBeforeObb, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			pair.m_SrcCalcHitCenter = SrcChkObb.m_Center;
			COLRECT DestChkRect = DestBeforeRect;
			DestChkRect.m_Center += DestVelocity * HitTime;
			pair.m_DestCalcHitCenter = DestChkRect.m_Center;
			bsm::Vec3 RetVec;
			//SrcのOBBとDestの最近接点を得る
			HitTest::ClosetPtPointPlane(SrcChkObb.m_Center, DestChkRect.GetPLANE(), RetVec);
			//接点へのベクトル
			//衝突した瞬間で法線を計算
			pair.m_SrcHitNormal = SrcChkObb.m_Center - RetVec;
			pair.m_CalcHitPoint = RetVec;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	bsm::Vec3 CollisionObb::GetCenterPosition()const {
		OBB SrcObb = GetObb();
		return SrcObb.m_Center;
	}

	AABB CollisionObb::GetEnclosingAABB()const {
		AABB enc = GetBeforeObb().GetWrappedAABB();
		enc.UnionAABB(GetObb().GetWrappedAABB());
		return enc;
	}

	AABB CollisionObb::GetWrappedAABB()const {
		return GetObb().GetWrappedAABB();
	}



	void CollisionObb::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CUBE"));
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionRect::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionRect::Impl {
		float m_Size;					//作成時のサイズ
		Impl() :
			m_Size(1.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionRect : public Collision ;
	//	用途: Rect(矩形)衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionRect::CollisionRect(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionRect::~CollisionRect() {}

	//初期化
	void CollisionRect::OnCreate() {
		SetFixed(true),
			SetDrawActive(false);
	}

	//アクセサ
	void CollisionRect::SetFixed(bool b) {
		if (!b) {
			throw BaseException(
				L"CollisionRectはFixed以外は選択できません",
				L"if (!b)",
				L"CollisionRect::SetFixed()"
			);
		}
		else {
			Collision::SetFixed(true);
		}
	}



	float CollisionRect::GetMakedSize() const {
		return pImpl->m_Size;
	}
	void CollisionRect::SetMakedSize(float f) {
		pImpl->m_Size = f;
	}

	COLRECT CollisionRect::GetColRect() const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		COLRECT rect(GetMakedSize(), GetMakedSize(), TrasnsPtr->GetWorldMatrix());
		return rect;
	}

	COLRECT CollisionRect::GetBeforeColRect() const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		COLRECT rect(GetMakedSize(), GetMakedSize(), TrasnsPtr->GetBeforeWorldMatrix());
		return rect;
	}

	bool CollisionRect::SimpleCollisionCall(const shared_ptr<Collision>& Src) {
		return Src->SimpleCollision(GetThis<CollisionRect>());
	}

	void CollisionRect::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionRect>());
	}

	AABB CollisionRect::GetEnclosingAABB()const {
		AABB enc = GetBeforeColRect().GetWrappedAABB();
		enc.UnionAABB(GetColRect().GetWrappedAABB());
		return enc;
	}

	AABB CollisionRect::GetWrappedAABB()const {
		COLRECT SrcColRect = GetColRect();
		return SrcColRect.GetWrappedAABB();
	}

	bsm::Vec3 CollisionRect::GetCenterPosition()const {
		COLRECT SrcColRect = GetColRect();
		return SrcColRect.m_Center;
	}



	void CollisionRect::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SQUARE"));

	}






}
//end basecross

