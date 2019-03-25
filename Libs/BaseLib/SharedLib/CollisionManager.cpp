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
		m_PriorityUnderEscapeY(false),
		m_EscapeFloor(3),
		pImpl(new Impl())
	{
		m_CollisionPairVec[0].resize(1024);
		m_CollisionPairVec[0].clear();
		m_CollisionPairVec[1].resize(1024);
		m_CollisionPairVec[1].clear();
		m_TempKeepVec.resize(1024);
		m_TempKeepVec.clear();
		m_TempExitVec.clear();
	}
	CollisionManager::~CollisionManager() {}

	bool CollisionManager::SimpleCollisionPair(CollisionPair& Pair) {
		auto Src = Pair.m_Src.lock();
		auto Dest = Pair.m_Dest.lock();
		if (Src && Dest) {
			if (Src->IsSleep()) {
				return false;
			}
			if (!Src->GetGameObject()->IsUpdateActive() || !Dest->GetGameObject()->IsUpdateActive()) {
				return false;
			}
			if (!Src->IsUpdateActive() || !Dest->IsUpdateActive()) {
				return false;
			}
			if (Src->IsFixed()) {
				return false;
			}
			if (Src->IsExcludeCollisionObject(Dest->GetGameObject()) || Dest->IsExcludeCollisionObject(Src->GetGameObject())) {
				return false;
			}
			bsm::Vec3 SrcCenter = Src->GetCenterPosition();
			bsm::Vec3 DestCenter = Dest->GetCenterPosition();
			bsm::Vec3 DestMoveVec = DestCenter - Pair.m_DestCalcHitCenter;
			bsm::Vec3 SrcLocalVec = SrcCenter - Pair.m_SrcCalcHitCenter - DestMoveVec;
			float SrcV = bsm::dot(SrcLocalVec, Pair.m_SrcHitNormal);
			if (SrcV >= 0.0f) {
				return false;
			}
			return Dest->SimpleCollisionCall(Src);
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
		if (!SrcColl->IsUpdateActive() || !DestColl->IsUpdateActive()) {
			return false;
		}
		if (SrcColl->IsFixed()) {
			return false;
		}
		if (SrcColl->IsSleep()) {
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

	void CollisionManager::EscapeCollisionPair(CollisionPair& Pair) {
		auto ShSrc = Pair.m_Src.lock();
		auto ShDest = Pair.m_Dest.lock();
		if (ShSrc->GetAfterCollision() == AfterCollision::None || ShDest->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		bsm::Vec3 SrcCenter = ShSrc->GetCenterPosition();
		bsm::Vec3 DestCenter = ShDest->GetCenterPosition();
		bsm::Vec3 DestMoveVec = DestCenter - Pair.m_DestCalcHitCenter;
		bsm::Vec3 SrcLocalVec = SrcCenter - Pair.m_SrcCalcHitCenter - DestMoveVec;
		float SrcV = bsm::dot(SrcLocalVec, Pair.m_SrcHitNormal);
		if (SrcV < 0.0f) {
			//まだ衝突していたら
			float EscapeLen = abs(SrcV);
			if (!ShDest->IsFixed()) {
				EscapeLen *= 0.5f;
			}
			//Srcのエスケープ
			SrcCenter += Pair.m_SrcHitNormal * EscapeLen;
			if (!ShDest->IsFixed()) {
				//Destのエスケープ
				DestCenter += -Pair.m_SrcHitNormal * EscapeLen;
			}
			SrcCenter.floor(GetEscapeFloor());
			auto PtrSrcTransform = ShSrc->GetGameObject()->GetComponent<Transform>();
			//Srcのエスケープ
			PtrSrcTransform->SetWorldPosition(SrcCenter);
			if (!ShDest->IsFixed()) {
				DestCenter.floor(GetEscapeFloor());
				ShDest->WakeUp();
				auto PtrDestTransform = ShDest->GetGameObject()->GetComponent<Transform>();
				//Destのエスケープ
				PtrDestTransform->SetWorldPosition(DestCenter);
			}
		}
	}

	void CollisionManager::SleepCheckSet() {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			auto ptrColl = v->GetComponent<Collision>(false);
			if (ptrColl) {
				ptrColl->SleepCheckSet();
			}
		}
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
			//追加ペアのSrcにもしGravityがセットされていたら0にする
			auto ShSrc = v.m_Src.lock();
			if (ShSrc) {
				auto Gr = ShSrc->GetGameObject()->GetComponent<Gravity>(false);
				if (Gr) {
					auto f = bsm::angleBetweenNormals(v.m_SrcHitNormal, Vec3(0, 1, 0));
					if (abs(f) < XM_PIDIV4) {
						Gr->SetGravityVerocityZero();
					}
				}
			}
			m_CollisionPairVec[m_KeepIndex].push_back(v);
		}

		//--------------------------------------------------------
		//キープ配列のソート(IsPriorityUnderEscapeY())の場合Yが小さい優先
		//--------------------------------------------------------
		auto func = [&](CollisionPair& Left, CollisionPair& Right)->bool {
			if (IsPriorityUnderEscapeY()) {
				if (Left.m_CalcHitPoint.y < Right.m_CalcHitPoint.y) {
					return true;
				}
			}
			else {
				if (Left.m_CalcHitPoint.y > Right.m_CalcHitPoint.y) {
					return true;
				}
			}
			return false;
		};
		//衝突点でソート
		std::sort(m_CollisionPairVec[m_KeepIndex].begin(), m_CollisionPairVec[m_KeepIndex].end(), func);
		//エスケープ
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto SrcSh = v.m_Src.lock();
			auto DestSh = v.m_Dest.lock();
			if (SrcSh && DestSh) {
				if (!SrcSh->IsFixed()) {
					EscapeCollisionPair(v);
				}
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
					ShSrcObj->OnCollisionExit(v);
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
					ShSrcObj->OnCollisionExcute(v);
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
					ShSrcObj->OnCollisionEnter(v);
				}
			}
		}
		SleepCheckSet();
	}

}
// end basecross
