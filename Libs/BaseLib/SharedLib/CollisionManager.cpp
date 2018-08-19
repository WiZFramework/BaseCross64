/*!
@file CollisionManager.h
@brief 衝突判定マネージャ実体（簡易物理計算も行う）
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct CollisionManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct CollisionManager::Impl {
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	衝突判定管理者
	//--------------------------------------------------------------------------------------
	CollisionManager::CollisionManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_NewIndex(0),
		m_KeepIndex(1),
		m_RecursiveCount(0),
		pImpl(new Impl())
	{}
	CollisionManager::~CollisionManager() {}

	void CollisionManager::EscapePair(CollisionPair& Pair) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto SrcSh = Pair.m_Src.lock();
		auto DestSh = Pair.m_Dest.lock();
		if (SrcSh && DestSh) {
			if (!SrcSh->IsFixed()) {
				SrcSh->EscapeCollisionPair(Pair);
			}
		}
	}

	void CollisionManager::EscapeFromDest(CollisionPair& Pair) {
		if (m_RecursiveCount > 5) {
			//再帰が深くなったら今回のターンでは、これ以上行わない
			return;
		}
		m_RecursiveCount++;
		EscapePair(Pair);
		auto SrcSh = Pair.m_Src.lock();
		auto DestSh = Pair.m_Dest.lock();
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto TgtSrcSh = v.m_Src.lock();
			auto TgtDestSh = v.m_Dest.lock();
			if ((SrcSh == TgtSrcSh && DestSh == TgtDestSh) || (SrcSh == TgtDestSh && DestSh == TgtSrcSh)) {
				continue;
			}
			if (SrcSh == TgtDestSh) {
				EscapeFromDest(v);
				break;
			}
		}
		m_RecursiveCount--;
	}

	bool CollisionManager::SimpleCollisionPairSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest) {
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
			if (HitTest::SPHERE_CAPSULE(SrcSp, DestCap, d)) {
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

	bool CollisionManager::SimpleCollisionPairSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcCap = Src->GetCapsule();
		if (ShDestSp) {
			bsm::Vec3 d;
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_CAPSULE(DestSp, SrcCap, d)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d1, d2;
			if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, d1, d2)) {
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
	bool CollisionManager::SimpleCollisionPairSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcObb = Src->GetObb();
		if (ShDestSp) {
			bsm::Vec3 d;
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_OBB(DestSp, SrcObb, d)) {
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


	bool CollisionManager::SimpleCollisionPair(CollisionPair& Pair) {
		auto Src = Pair.m_Src.lock();
		auto Dest = Pair.m_Dest.lock();
		if (Src && Dest) {
			auto ShSrcSp = dynamic_pointer_cast<CollisionSphere>(Src);
			auto ShSrcCap = dynamic_pointer_cast<CollisionCapsule>(Src);
			auto ShSrcObb = dynamic_pointer_cast<CollisionObb>(Src);
			if (ShSrcSp) {
				return SimpleCollisionPairSub(ShSrcSp, Dest);
			}
			else if (ShSrcCap) {
				return SimpleCollisionPairSub(ShSrcCap, Dest);
			}
			else if (ShSrcObb) {
				return SimpleCollisionPairSub(ShSrcObb, Dest);
			}
		}
		return false;
	}

	bool CollisionManager::EnableedCollisionPair(const shared_ptr<GameObject>& Src, const shared_ptr<GameObject>& Dest) {
		if (Src == Dest) {
			return false;
		}
		if (!Src->IsUpdateActive() || !Dest->IsUpdateActive()) {
			return false;
		}
		auto SrcColl = Src->GetComponent<Collision>(false);
		auto DestColl = Dest->GetComponent<Collision>(false);
		if (!SrcColl || !DestColl) {
			return false;
		}
		if (SrcColl->IsFixed() || !SrcColl->IsUpdateActive() || !DestColl->IsUpdateActive()) {
			return false;
		}
		if (SrcColl->IsExcludeCollisionObject(Dest) || DestColl->IsExcludeCollisionObject(Src)) {
			return false;
		}
		return true;
	}


	void CollisionManager::SetNewCollision() {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			SetNewCollisionSub(v);
		}
	}

	void CollisionManager::SetNewCollisionSub(const shared_ptr<GameObject>& Src) {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (EnableedCollisionPair(Src,v)) {
				auto SrcColl = Src->GetComponent<Collision>();
				auto DestColl = v->GetComponent<Collision>();
				if (!IsInPair(SrcColl, DestColl, true)) {
					//キープされている中になかったら
					//Collisionによる衝突判定
					DestColl->CollisionCall(SrcColl);
				}
			}
		}
	}

	void CollisionManager::InsertNewPair(const CollisionPair& NewPair) {
		m_CollisionPairVec[m_NewIndex].push_back(NewPair);
	}




	void CollisionManager::OnCreate() {

	}


	void CollisionManager::OnUpdate() {

		//keepのチェック
		m_TempKeepVec.clear();
		m_TempExitVec.clear();
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			if (SimpleCollisionPair(v)) {
				//まだ衝突している
				m_TempKeepVec.push_back(v);
			}
			else {
				m_TempExitVec.push_back(v);
			}
		}
		//テンポラリの内容をkeepペアにコピー
		m_CollisionPairVec[m_KeepIndex].resize(m_TempKeepVec.size());
		m_CollisionPairVec[m_KeepIndex] = m_TempKeepVec;

		//キープされているペアのSrcにもしGravityがセットされていたら0にする
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();

			if (ShSrc) {
				auto Gr = ShSrc->GetGameObject()->GetComponent<Gravity>(false);
				if (Gr) {
					auto f = bsm::angleBetweenNormals(v.m_SrcHitNormal, Vec3(0, 1, 0));
					if (abs(f) < XM_PIDIV4) {
						Gr->SetGravityVerocityZero();
					}
				}
			}
		}

		//新規のペア配列のクリア
		m_CollisionPairVec[m_NewIndex].clear();
		//新規の衝突判定
		SetNewCollision();
		//追加されたペアをキープに追加
		for (auto& v : m_CollisionPairVec[m_NewIndex]) {
			m_CollisionPairVec[m_KeepIndex].push_back(v);
		}

		//キープ配列のソート
		//--------------------------------------------------------
		auto func = [](CollisionPair& Left, CollisionPair& Right)->bool {
			auto ShLeftDest = Left.m_Dest.lock();
			auto ShRightDest = Right.m_Dest.lock();
			if (!ShLeftDest->IsFixed() && !ShRightDest->IsFixed()) {
				auto LeftMove = bsm::length(ShLeftDest->GetGameObject()->GetComponent<Transform>()->GetVelocity());
				auto RightMove = bsm::length(ShRightDest->GetGameObject()->GetComponent<Transform>()->GetVelocity());
				if (LeftMove > RightMove) {
					return true;
				}
			}
			return false;
		};
		//動きのある物優先にソート
		std::sort(m_CollisionPairVec[m_KeepIndex].begin(), m_CollisionPairVec[m_KeepIndex].end(), func);

		///Fixedに関係ないオブジェクトのエスケープ
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto TgtSrcSh = v.m_Src.lock();
			auto TgtDestSh = v.m_Dest.lock();
			if (!TgtSrcSh->IsFixed() && !TgtDestSh->IsFixed()) {
				EscapePair(v);
			}
		}

		m_RecursiveCount = 0;
		//Fixedに衝突している
		//逆順に検証
		for (auto it = m_CollisionPairVec[m_KeepIndex].rbegin(); it != m_CollisionPairVec[m_KeepIndex].rend(); it++) {
			auto TgtSrcSh = it->m_Src.lock();
			auto TgtDestSh = it->m_Dest.lock();
			if (TgtDestSh->IsFixed()) {
				//再帰呼び出し
				EscapeFromDest(*it);
			}
		}

		//衝突メッセージの送信
		//Exit
		for (auto& v : m_TempExitVec) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();
			if (ShSrc && ShDest) {
				auto ShSrcObj = ShSrc->GetGameObject();
				auto ShDestObj = ShDest->GetGameObject();
				if (ShSrcObj && ShDestObj) {
					ShSrcObj->OnCollisionExit(ShDestObj);
				}
			}
		}
		//キープ
		for (auto& v : m_TempKeepVec) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();
			if (ShSrc && ShDest) {
				auto ShSrcObj = ShSrc->GetGameObject();
				auto ShDestObj = ShDest->GetGameObject();
				if (ShSrcObj && ShDestObj) {
					ShSrcObj->OnCollisionExcute(ShDestObj);
				}
			}
		}
		//新規
		for (auto& v : m_CollisionPairVec[m_NewIndex]) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();
			if (ShSrc && ShDest) {
				auto ShSrcObj = ShSrc->GetGameObject();
				auto ShDestObj = ShDest->GetGameObject();
				if (ShSrcObj && ShDestObj) {
					ShSrcObj->OnCollisionEnter(ShDestObj);
				}
			}
		}
	}



}
// end basecross
