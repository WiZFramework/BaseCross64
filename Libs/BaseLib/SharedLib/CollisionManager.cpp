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
		pImpl(new Impl())
	{}
	CollisionManager::~CollisionManager() {}

	void CollisionManager::EscapePair(CollisionPair& Pair) {

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto SrcSh = Pair.m_Src.lock();
		auto DestSh = Pair.m_Dest.lock();
		if (SrcSh && DestSh) {
			auto SrcSp = dynamic_pointer_cast<CollisionSphere>(SrcSh);
			auto SrcCap = dynamic_pointer_cast<CollisionCapsule>(SrcSh);
			auto SrcObb = dynamic_pointer_cast<CollisionObb>(SrcSh);
			auto DestSp = dynamic_pointer_cast<CollisionSphere>(DestSh);
			auto DestCap = dynamic_pointer_cast<CollisionCapsule>(DestSh);
			auto DestObb = dynamic_pointer_cast<CollisionObb>(DestSh);
			if (!SrcSh->IsFixed()) {
				if (DestSp) {
					SrcSh->EscapeCollision(DestSp, Pair.m_SrcHitNormal);
					if (DestSp->IsFixed()) {
						Pair.m_SrcRegardFixed = true;
					}
				}
				else if (DestCap) {
					SrcSh->EscapeCollision(DestCap, Pair.m_SrcHitNormal);
					if (DestCap->IsFixed()) {
						Pair.m_SrcRegardFixed = true;
					}
				}
				else if (DestObb) {
					SrcSh->EscapeCollision(DestObb, Pair.m_SrcHitNormal);
					auto SrcPos = SrcSh->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
					auto DestPos = DestSh->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

					if (SrcSh->GetGameObject()->FindTag(L"Player") && SrcPos.z > 3.0f && DestPos.z > 3.0f) {
						int a = 0;
					}
					if (DestObb->IsFixed()) {
						Pair.m_SrcRegardFixed = true;
					}
				}
			}
		}
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
			float Speed = bsm::length(
				Src->GetGameObject()->GetComponent<Transform>()->GetVelocity()
				- Dest->GetGameObject()->GetComponent<Transform>()->GetVelocity()
			);

			auto ShSrcSp = dynamic_pointer_cast<CollisionSphere>(Src);
			auto ShSrcCap = dynamic_pointer_cast<CollisionCapsule>(Src);
			auto ShSrcObb = dynamic_pointer_cast<CollisionObb>(Src);
			if (ShSrcSp) {
				auto b = SimpleCollisionPairSub(ShSrcSp, Dest);
				if (b) {
					Pair.m_EscapeSpeed = Speed;
				}
				return b;
			}
			else if (ShSrcCap) {
				auto b = SimpleCollisionPairSub(ShSrcCap, Dest);
				if (b) {
					Pair.m_EscapeSpeed = Speed;
				}
				return b;
			}
			else if (ShSrcObb) {
				auto b =  SimpleCollisionPairSub(ShSrcObb, Dest);
				if (b) {
					Pair.m_EscapeSpeed = Speed;
				}
				return b;
			}
			return false;
		}
		else {
			return false;
		}
	}

	void CollisionManager::SetNewCollision() {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (v->IsUpdateActive()) {
				auto Col = v->GetComponent<Collision>(false);
				if (Col && Col->IsUpdateActive() && !Col->IsFixed()) {
					SetNewCollisionSub(Col);
				}
			}
		}
	}

	void CollisionManager::SetNewCollisionSub(const shared_ptr<Collision>& Src) {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (v->IsUpdateActive()) {
				auto Dest = v->GetComponent<Collision>(false);
				if (Dest && Dest->IsUpdateActive() && Src != Dest) {
					if (!IsInPair(Src, Dest, true)) {
						//キープされている中になかったら
						//Collisionによる衝突判定
						Dest->CollisionCall(Src);
					}
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
		m_TempVec.clear();
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			if (SimpleCollisionPair(v)) {
				//まだ衝突している
				m_TempVec.push_back(v);
			}
		}
		//テンポラリの内容をkeepペアにコピー
		m_CollisionPairVec[m_KeepIndex].resize(m_TempVec.size());
		m_CollisionPairVec[m_KeepIndex] = m_TempVec;

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

		std::sort(m_CollisionPairVec[m_KeepIndex].begin(), m_CollisionPairVec[m_KeepIndex].end(), func);

		for (size_t i = 0; i < m_CollisionPairVec[m_KeepIndex].size(); i++) {
			m_CollisionPairVec[m_KeepIndex][i].m_SrcRegardFixed = false;
		}


		int count = 0;
		do {
			////キープされているペアのエスケープ
			for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
				EscapePair(v);
			}
			std::reverse(begin(m_CollisionPairVec[m_KeepIndex]), end(m_CollisionPairVec[m_KeepIndex]));
			count++;

		} while (count < 10);


	}




}
// end basecross
