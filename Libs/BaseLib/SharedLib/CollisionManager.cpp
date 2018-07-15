/*!
@file CollisionManager.h
@brief 衝突判定マネージャ実体（簡易物理計算も行う）
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	struct CillisionItem {
		shared_ptr<Collision> m_Collision;
		SPHERE m_EnclosingSphere;
		float m_MinX;
		float m_MaxX;
		float m_MinY;
		float m_MaxY;
		float m_MinZ;
		float m_MaxZ;
		bool operator==(const CillisionItem& other)const {
			if (this == &other) {
				return true;
			}
			return false;
		}
	};

	//--------------------------------------------------------------------------------------
	//	struct CollisionManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct CollisionManager::Impl {
		vector<CillisionItem> m_ItemVec;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	衝突判定管理者
	//--------------------------------------------------------------------------------------
	CollisionManager::CollisionManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_PairSwap(0),
		m_BeforePairSwap(1 - m_PairSwap),
		pImpl(new Impl())
	{}
	CollisionManager::~CollisionManager() {}

	void CollisionManager::OnCreate() {

	}

	void CollisionManager::CollisionSub(size_t SrcIndex) {
		CillisionItem& Src = pImpl->m_ItemVec[SrcIndex];
		for (auto& v : pImpl->m_ItemVec) {
			if (Src == v) {
				continue;
			}
			if (Src.m_MinX > v.m_MaxX || Src.m_MaxX < v.m_MinX) {
				continue;
			}
			if (Src.m_MinY > v.m_MaxY || Src.m_MaxY < v.m_MinY) {
				continue;
			}
			if (Src.m_MinZ > v.m_MaxZ || Src.m_MaxZ < v.m_MinZ) {
				continue;
			}
			//相手が除外オブジェクトになってるかどうか
			if (v.m_Collision->IsExcludeCollisionObject(Src.m_Collision->GetGameObject())) {
				continue;
			}
			if (Src.m_Collision->IsExcludeCollisionObject(v.m_Collision->GetGameObject())) {
				continue;
			}
			//衝突判定(Destに呼んでもらう。ダブルデスパッチ呼び出し)
			v.m_Collision->CollisionCall(Src.m_Collision);
		}
	}

	void CollisionManager::OnUpdate() {
		m_BeforePairSwap = m_PairSwap;
		m_PairSwap = 1 - m_PairSwap;
		m_PairVec[m_PairSwap].clear();
		m_ExitPairVec.clear();
		//キープが今も維持されているかチェック
		//キープが外れたペアはm_ExitPairVecに追加される
		CollisionKeepCheck();

		pImpl->m_ItemVec.clear();
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (v->IsUpdateActive()) {
				auto Col = v->GetComponent<Collision>(false);
				if (Col && Col->IsUpdateActive()) {
					CillisionItem Item;
					Item.m_Collision = Col;
					Item.m_EnclosingSphere = Col->GetEnclosingSphere();
					Item.m_MinX = Item.m_EnclosingSphere.m_Center.x - Item.m_EnclosingSphere.m_Radius;
					Item.m_MaxX = Item.m_EnclosingSphere.m_Center.x + Item.m_EnclosingSphere.m_Radius;

					Item.m_MinY = Item.m_EnclosingSphere.m_Center.y - Item.m_EnclosingSphere.m_Radius;
					Item.m_MaxY = Item.m_EnclosingSphere.m_Center.y + Item.m_EnclosingSphere.m_Radius;

					Item.m_MinZ = Item.m_EnclosingSphere.m_Center.z - Item.m_EnclosingSphere.m_Radius;
					Item.m_MaxZ = Item.m_EnclosingSphere.m_Center.z + Item.m_EnclosingSphere.m_Radius;

					pImpl->m_ItemVec.push_back(Item);
				}
			}
		}


		auto func = [&](CillisionItem& Left, CillisionItem& Right)->bool {
			auto PtrLeftVelo = Left.m_Collision->GetGameObject()->GetComponent<Transform>()->GetVelocity();
			auto PtrRightVelo = Right.m_Collision->GetGameObject()->GetComponent<Transform>()->GetVelocity();
			auto LeftLen = bsm::length(PtrLeftVelo);
			auto RightLen = bsm::length(PtrRightVelo);

			return (LeftLen < RightLen);
		};

		std::sort(pImpl->m_ItemVec.begin(), pImpl->m_ItemVec.end(), func);


		for (size_t i = 0; i < pImpl->m_ItemVec.size(); i++) {
			if (!pImpl->m_ItemVec[i].m_Collision->IsFixed()) {
				CollisionSub(i);
			}
		}

		SendCollisionMessage();
	}


	bool CollisionManager::CheckInPair(const CollisionPair& tgt, UINT swap) {
		auto ShTgtSrc = tgt.m_Src.lock();
		auto ShTgtDest = tgt.m_Dest.lock();
		if ((!ShTgtSrc) || (!ShTgtDest)) {
			return false;
		}
		for (auto& v : m_PairVec[swap]) {
			auto ShChkSrc = v.m_Src.lock();
			auto ShChkDest = v.m_Dest.lock();
			if (ShChkSrc && ShChkDest) {
				if (ShTgtSrc == ShChkSrc && ShTgtDest == ShChkDest) {
					return true;
				}
			}
		}
		return false;
	}

	bool CollisionManager::CollisionCheckSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcSp = Src->GetSphere();
		if (ShDestSp) {
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_SPHERE(SrcSp, DestSp)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d;
			if (HitTest::SPHERE_CAPSULE(SrcSp, DestCap,d)) {
				return true;
			}
		}
		else if (ShDestObb) {
			auto DestObb = ShDestObb->GetObb();
			bsm::Vec3 d;
			if (HitTest::SPHERE_OBB(SrcSp, DestObb, d)) {
				return true;
			}
		}
		return false;
	}

	bool CollisionManager::CollisionCheckSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcCap = Src->GetCapsule();
		if (ShDestSp) {
			bsm::Vec3 d;
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_CAPSULE(DestSp, SrcCap,d)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d1,d2;
			if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, d1,d2)) {
				return true;
			}
		}
		else if (ShDestObb) {
			auto DestObb = ShDestObb->GetObb();
			bsm::Vec3 d;
			if (HitTest::CAPSULE_OBB(SrcCap, DestObb, d)) {
				return true;
			}
		}
		return false;
	}
	bool CollisionManager::CollisionCheckSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcObb = Src->GetObb();
		if (ShDestSp) {
			bsm::Vec3 d;
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_OBB(DestSp, SrcObb,d)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d;
			if (HitTest::CAPSULE_OBB(DestCap, SrcObb, d)) {
				return true;
			}
		}
		else if (ShDestObb) {
			auto DestObb = ShDestObb->GetObb();
			if (HitTest::OBB_OBB(SrcObb, DestObb)) {
				return true;
			}
		}
		return false;

	}


	bool CollisionManager::CollisionCheck(const shared_ptr<Collision>& Src, const shared_ptr<Collision>& Dest) {
		auto ShSrcSp = dynamic_pointer_cast<CollisionSphere>(Src);
		auto ShSrcCap = dynamic_pointer_cast<CollisionCapsule>(Src);
		auto ShSrcObb = dynamic_pointer_cast<CollisionObb>(Src);
		if (ShSrcSp) {
			return CollisionCheckSub(ShSrcSp, Dest);
		}
		else if (ShSrcCap) {
			return CollisionCheckSub(ShSrcCap, Dest);
		}
		else if (ShSrcObb) {
			return CollisionCheckSub(ShSrcObb, Dest);
		}
		return false;
	}


	void CollisionManager::CollisionKeepCheck() {
		//テンポラリをキープ用配列と同じ大きさで確保
		vector<CollisionPair> TmpPairVec(m_PairVec[m_BeforePairSwap].size());
		//いったんクリア
		TmpPairVec.clear();

		for (auto& v : m_PairVec[m_BeforePairSwap]) {
			auto ShTgtSrc = v.m_Src.lock();
			auto ShTgtDest = v.m_Dest.lock();
			if ((!ShTgtSrc) || (!ShTgtDest)) {
				continue;
			}
			if (CollisionCheck(ShTgtSrc, ShTgtDest)) {
				//衝突している
				TmpPairVec.push_back(v);
			}
			else {
				//すでに衝突していない
				m_ExitPairVec.push_back(v);
			}
		}
		m_PairVec[m_BeforePairSwap].resize(TmpPairVec.size());
		m_PairVec[m_BeforePairSwap] = TmpPairVec;
	}


	void CollisionManager::AddNewCollisionPair(const CollisionPair& pair) {
		if (CheckInPair(pair, m_BeforePairSwap)) {
			//すでにキープ用の配列にある
			return;
		}
		m_PairVec[m_PairSwap].push_back(pair);
	}






	void CollisionManager::SendCollisionMessageSub(CollMessType messtype) {
		vector<CollisionPair>& tgt = m_ExitPairVec;
		switch (messtype) {
		case CollMessType::Enter:
			tgt = m_PairVec[m_PairSwap];
			break;
		case CollMessType::Excute:
			tgt = m_PairVec[m_BeforePairSwap];
			break;
		case CollMessType::Exit:
			tgt = m_ExitPairVec;
			break;
		}


		set<shared_ptr<Collision>> SrcSet;
		for (auto& v : tgt) {
			auto LeftP = v.m_Src.lock();
			auto RightP = v.m_Dest.lock();
			if (LeftP && !LeftP->IsFixed()) {
				SrcSet.insert(LeftP);
			}
			if (RightP && !RightP->IsFixed()) {
				SrcSet.insert(RightP);
			}
		}
		//この段階でSrcSetには重複のないcollisionが入っている
		vector<shared_ptr<GameObject>> DestObjVec;
		for (auto it = SrcSet.begin(); it != SrcSet.end(); it++) {
			DestObjVec.clear();
			auto Src = *it;
			for (auto& v : tgt) {
				auto LeftP = v.m_Src.lock();
				auto RightP = v.m_Dest.lock();
				if (LeftP == Src) {
					//相手を登録
					DestObjVec.push_back(RightP->GetGameObject());
				}
				if (RightP == Src) {
					//相手を登録
					DestObjVec.push_back(LeftP->GetGameObject());
				}
			}
			auto SrcGameObj = Src->GetGameObject();
			switch (messtype) {
			case CollMessType::Enter:
				SrcGameObj->OnCollisionEnter(DestObjVec);
				break;
			case CollMessType::Excute:
				SrcGameObj->OnCollisionExcute(DestObjVec);
				break;
			case CollMessType::Exit:
				SrcGameObj->OnCollisionExit(DestObjVec);
				break;
			}
		}
	}

	void CollisionManager::SendCollisionMessage() {
		SendCollisionMessageSub(CollMessType::Exit);
		SendCollisionMessageSub(CollMessType::Enter);
		SendCollisionMessageSub(CollMessType::Excute);
	}


}
// end basecross
