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
		vector<CollisionHitPair> m_EnterPairVec;
		vector<CollisionHitPair> m_ExcutePairVec;
		vector<CollisionHitPair> m_ExitPairVec;
		Impl()
		{}
		~Impl() {}
		bool CheckInPair(const CollisionHitPair& pair, const vector<CollisionHitPair>& TgtVec) const {
			for (auto& v : TgtVec) {
				if (v.m_Src.lock() == pair.m_Src.lock() && v.m_Dest.lock() == pair.m_Dest.lock()) {
					return true;
				}
			}
			return false;
		}
		//指定のコリジョンがペア配列にはいているかどうか
		bool CheckCollisionInPair(const shared_ptr<Collision>& coll, const vector<CollisionHitPair>& TgtVec) const {
			for (auto& v : TgtVec) {
				if (v.m_Src.lock() == coll || v.m_Dest.lock() == coll) {
					return true;
				}
			}
			return false;

		}


	};


	//--------------------------------------------------------------------------------------
	//	衝突判定管理者
	//--------------------------------------------------------------------------------------
	CollisionManager::CollisionManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
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
			//すでに衝突しているか
			CollisionHitPair tempPair;
			tempPair.m_Src = Src.m_Collision;
			tempPair.m_Dest = v.m_Collision;
			if (pImpl->m_ExcutePairVec.size() >= 2) {
				int a = 0;
			}
			if (pImpl->CheckInPair(tempPair, pImpl->m_ExcutePairVec)) {
				continue;
			}
			if (pImpl->CheckInPair(tempPair, pImpl->m_EnterPairVec)) {
				continue;
			}
			//衝突判定(Destに呼んでもらう。ダブルデスパッチ呼び出し)
			v.m_Collision->CollisionCall(Src.m_Collision);
		}
	}

	void CollisionManager::OnUpdate() {
		//ステップ2: EnterからExcuteに移行
		EnterToExcutePair();
		//ステップ3: ExcutePairの中から今は衝突してないペアを削除
		UpdateExcutePair();
		//ステップ1: トランスフォームからステートの取得
		UpdateCollisionState();
		//ステップ4: 衝突判定
		UpdateEnterPair();
		//ステップ5: 拘束の更新
		UpdateSolver();
		//ステップ6: メッセージを送る
		SendCollisionMessage();
	}

	void CollisionManager::SetEnterPair(const CollisionHitPair& pair) {
		if (!pImpl->CheckInPair(pair, pImpl->m_EnterPairVec)) {
			pImpl->m_EnterPairVec.push_back(pair);
		}
	}

	bool CollisionManager::CollisionPair(const shared_ptr<CollisionSphere>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps) {
		auto Sp = dynamic_pointer_cast<CollisionSphere>(dest);
		auto Cp = dynamic_pointer_cast<CollisionCapsule>(dest);
		auto Obb = dynamic_pointer_cast<CollisionObb>(dest);
		if (Sp) {
			if (HitTest::SPHERE_SPHERE(src->GetSphere(), Sp->GetSphere())) {
				return true;
			}
		}
		else if (Cp) {
			Vec3 d;
			if (HitTest::SPHERE_CAPSULE(src->GetSphere(), Cp->GetCapsule(), d)) {
				return true;
			}
		}
		else if (Obb) {
			Vec3 d;
			SPHERE sp = src->GetSphere();
			if (HitTest::SPHERE_OBB(sp, Obb->GetObb(), d)) {
				if (bsm::length(sp.m_Center - d) <= 0.001f) {
					deps = bsm::length(sp.m_Center - HitPoint) + sp.m_Radius;
				}
				else {
					deps = abs(sp.m_Radius - bsm::length(sp.m_Center - HitPoint));
				}


				//最初にヒットしたポイントと球の中心の距離を測る
//				deps = bsm::length(d - HitPoint);
//				deps = bsm::length(src->GetSphere().m_Center - HitPoint);
				//最近接点と球の中心の距離を測る
//				deps = abs(bsm::length(src->GetSphere().m_Center - d) - src->GetSphere().m_Radius);
//				deps /= COLLISION_TIME_STEP;
//				deps *= 20.0f;
				return true;
			}
		}
		return false;
	}
	bool CollisionManager::CollisionPair(const shared_ptr<CollisionCapsule>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps) {
		auto Sp = dynamic_pointer_cast<CollisionSphere>(dest);
		auto Cp = dynamic_pointer_cast<CollisionCapsule>(dest);
		auto Obb = dynamic_pointer_cast<CollisionObb>(dest);
		if (Sp) {
			Vec3 d;
			if (HitTest::SPHERE_CAPSULE(Sp->GetSphere(), src->GetCapsule(), d)) {
				return true;
			}
		}
		else if (Cp) {
			Vec3 d1, d2;
			if (HitTest::CAPSULE_CAPSULE(src->GetCapsule(), Cp->GetCapsule(), d1, d2)) {
				return true;
			}
		}
		else if (Obb) {
			Vec3 d;
			if (HitTest::CAPSULE_OBB(src->GetCapsule(), Obb->GetObb(), d)) {
				return true;
			}
		}
		return false;
	}

	bool CollisionManager::CollisionPair(const shared_ptr<CollisionObb>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps) {
		auto Sp = dynamic_pointer_cast<CollisionSphere>(dest);
		auto Cp = dynamic_pointer_cast<CollisionCapsule>(dest);
		auto Obb = dynamic_pointer_cast<CollisionObb>(dest);
		if (Sp) {
			Vec3 d;
			if (HitTest::SPHERE_OBB(Sp->GetSphere(), src->GetObb(), d)) {
				return true;
			}
		}
		else if (Cp) {
			Vec3 d;
			if (HitTest::CAPSULE_OBB(Cp->GetCapsule(), src->GetObb(), d)) {
				return true;
			}
		}
		else if (Obb) {
			if (HitTest::OBB_OBB(src->GetObb(), Obb->GetObb())) {
				return true;
			}
		}
		return false;
	}


	void CollisionManager::CollisionPairbase(vector<CollisionHitPair>& CheckVec, vector<CollisionHitPair>& tempVec) {
		for (auto& v : CheckVec) {
			auto SrcP = v.m_Src.lock();
			auto DestP = v.m_Dest.lock();
			if ((!SrcP) || (!DestP)) {
				continue;
			}
			if (pImpl->CheckInPair(v, tempVec)) {
				continue;
			}
			auto Sp = dynamic_pointer_cast<CollisionSphere>(SrcP);
			auto Cp = dynamic_pointer_cast<CollisionCapsule>(SrcP);
			auto Obb = dynamic_pointer_cast<CollisionObb>(SrcP);
			float deps;
			if (Sp) {
				if (CollisionPair(Sp, DestP,v.m_SrcHitPoint, deps)) {
					v.m_EscSpeed = deps;
					tempVec.push_back(v);
				}
				else {
					SrcP->SetSolverVelocity(Vec3(0.0f));
					pImpl->m_ExitPairVec.push_back(v);
				}
			}
			else if (Cp) {
				if (CollisionPair(Cp, DestP, v.m_SrcHitPoint, deps)) {
					tempVec.push_back(v);
				}
				else {
					SrcP->SetSolverVelocity(Vec3(0.0f));
					pImpl->m_ExitPairVec.push_back(v);
				}
			}
			else if (Obb) {
				if (CollisionPair(Obb, DestP, v.m_SrcHitPoint, deps)) {
					tempVec.push_back(v);
				}
				else {
					SrcP->SetSolverVelocity(Vec3(0.0f));
					pImpl->m_ExitPairVec.push_back(v);
				}
			}
		}
	}

	void CollisionManager::Solver(CollisionHitPair& SrcPair) {
		auto SrcSh = SrcPair.m_Src.lock();
		if (SrcSh) {
			for (auto& v : pImpl->m_ExcutePairVec) {
				auto TestSrcSh = v.m_Src.lock();
				if (TestSrcSh && SrcSh == TestSrcSh) {
					CollisionState& state = SrcSh->GetCollisionState();
					auto Pos = state.GetPosition();
					Pos += SrcPair.m_SrcNormal * SrcPair.m_Deps;
					SrcPair.m_Deps *= 0.1f;
					if (SrcPair.m_Deps <= 0.001f) {
						SrcPair.m_Deps = 0.001f;
					}
					state.SetPosition(Pos);
				}
			}
			//Transform更新用にもう一度ループ
			for (auto& v : pImpl->m_ExcutePairVec) {
				auto TestSrcSh = v.m_Src.lock();
				if (TestSrcSh && SrcSh == TestSrcSh) {
					CollisionState& state = SrcSh->GetCollisionState();
					auto Pos = state.GetPosition();
					auto Trans = SrcSh->GetGameObject()->GetComponent<Transform>();
					Trans->SetWorldPosition(Pos);
				}
			}
		}


/*
		auto SrcSh = SrcPair.m_Src.lock();
		if (SrcSh) {
			//Srcが主体のペアを探し出し、拘束を解く法線を作成する
			bsm::Vec3 AllNormal;
			float Speed = 0.0f;
			int count = 0;
			for (auto& v : pImpl->m_ExcutePairVec) {
				auto TestSrcSh = v.m_Src.lock();
				if (SrcSh && SrcSh == TestSrcSh) {
					AllNormal += v.m_SrcNormal;
					Speed += v.m_EscSpeed;
					count++;
				}
			}
			if (count > 0) {
				if (count >= 2) {
					int a = 0;
				}
		//		Speed /= (float)count;
				AllNormal.normalize();
//				auto Velo = SrcSh->GetSolverVelocity();
				auto Velo = AllNormal * Speed;
				SrcSh->SetSolverVelocity(Velo);
//				Velo = SrcSh->GetSolverVelocity();
				auto Trans = SrcSh->GetGameObject()->GetComponent<Transform>();
				auto Pos = Trans->GetWorldPosition();
				Pos = Pos + Velo *COLLISION_TIME_STEP;
				Trans->SetWorldPosition(Pos);
			}
		}
*/
	}


	void CollisionManager::UpdateCollisionState() {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (v->IsUpdateActive()) {
				auto Col = v->GetComponent<Collision>(false);
				if (Col && Col->IsUpdateActive()) {
					if (!pImpl->CheckCollisionInPair(Col, pImpl->m_ExcutePairVec)) {
						Col->SetCollisionStateFromTransform();
					}
				}
			}
		}
	}


	void CollisionManager::EnterToExcutePair() {
		for (auto& v : pImpl->m_EnterPairVec) {
			pImpl->m_ExcutePairVec.push_back(v);
		}
		//Enterをクリア
		pImpl->m_EnterPairVec.clear();
	}


	void CollisionManager::UpdateExcutePair() {
		//Exitをクリア
		pImpl->m_ExitPairVec.clear();
		vector<CollisionHitPair> TempVec(pImpl->m_ExcutePairVec.size());
		TempVec.clear();
		//Excuteのペアが今も衝突しているかチェック
		//衝突していなければExitPairに回る
		CollisionPairbase(pImpl->m_ExcutePairVec, TempVec);
		pImpl->m_ExcutePairVec.resize(TempVec.size());
		pImpl->m_ExcutePairVec = TempVec;
	}

	void CollisionManager::UpdateEnterPair() {
		//中間的なItemVecをクリア
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
		for (size_t i = 0; i < pImpl->m_ItemVec.size(); i++) {
			if (!pImpl->m_ItemVec[i].m_Collision->IsFixed()) {
				CollisionSub(i);
			}
		}
	}

	void CollisionManager::UpdateSolver() {
		//これより拘束を解く
		for (auto& v : pImpl->m_ExcutePairVec) {
			auto SrcSh = v.m_Src.lock();
			if (SrcSh) {
				Solver(v);
			}
		}

/*
		//これより拘束を解く
		shared_ptr<Collision> tempCol = nullptr;
		for (auto& v : pImpl->m_ExcutePairVec) {
			auto SrcSh = v.m_Src.lock();
			if (SrcSh && (SrcSh != tempCol)) {
				tempCol = SrcSh;
				Solver(v);
			}
		}
*/
	}


	void CollisionManager::SendCollisionMessageSub(CollMessType messtype) {
		vector<CollisionHitPair>& tgt = pImpl->m_ExitPairVec;
		switch (messtype) {
		case CollMessType::Enter:
			tgt = pImpl->m_EnterPairVec;
			break;
		case CollMessType::Excute:
			tgt = pImpl->m_ExcutePairVec;
			break;
		case CollMessType::Exit:
			tgt = pImpl->m_ExitPairVec;
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
