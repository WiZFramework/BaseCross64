/*!
@file CollisionManager.h
@brief 衝突判定マネージャ実体（簡易物理計算も行う）
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	struct CollisionPiece {
		CollisionPiece* m_Children[4];
		AABB m_AABB;
		vector<shared_ptr<GameObject>> m_ObjVec;
		CollisionPiece():
			m_AABB()
		{
			for (int i = 0; i < 4; i++) {
				m_Children[i] = nullptr;
			}
			m_ObjVec.clear();
		}
		void SetAABB(const AABB& aabb) {
			m_AABB = aabb;
		}
		void Clear() {
			for (int i = 0; i < 4; i++) {
				if (m_Children[i]) {
					m_Children[i]->Clear();
				}
				m_Children[i] = nullptr;
			}
			m_ObjVec.clear();
			m_AABB = AABB();
		}
	};

#define MAX_PIECE_COUNT 8192
	CollisionPiece g_PiecePool[MAX_PIECE_COUNT];
	UINT g_NextPoolIndex = 0;

	struct CollisionBlocks {
		UINT m_CollisionCountOfTern;
		CollisionPiece m_RootPiece;
		AABB m_RootAABB;
		CollisionBlocks():
			m_CollisionCountOfTern(0)
		{
			AABB aabb(Vec3(-100.0f, -1000, -100.0f), Vec3(100.0f, 1000, 100.0f));
			m_RootAABB = aabb;
			m_RootPiece.SetAABB(m_RootAABB);
		}
		void SetRootAABB(const AABB& aabb) {
			m_RootAABB = aabb;
		}
		void AllClear() {
			m_RootPiece.Clear();
			m_RootPiece.SetAABB(m_RootAABB);
			g_NextPoolIndex = 0;
		}

		void SetCollisionBlockSub(CollisionPiece& tgt,const shared_ptr<GameObject>& Obj) {
			auto Col = Obj->GetComponent<Collision>();
			auto colAABB = Col->GetWrappedAABB();
			if (HitTest::AABB_AABB(tgt.m_AABB, colAABB)) {
				if (tgt.m_Children[0]) {
					//子供ピースがあったら
					for (int i = 0; i < 4; i++) {
						SetCollisionBlockSub(*tgt.m_Children[i], Obj);
					}
				}
				else {
					//子供ピースがなかった
					//AABBが小さい場合はこれ以上増やさない
					if (tgt.m_AABB.GetWidth() < 0.125f) {
						tgt.m_ObjVec.push_back(Obj);
					}
					else {
						if (tgt.m_ObjVec.size() < 5) {
							//AABBの幅が定数未満か
							//あるいはまだ余裕がある
							tgt.m_ObjVec.push_back(Obj);
						}
						else {
							//余裕がない子供ブロックの作成
							for (int i = 0; i < 4; i++) {
								tgt.m_Children[i] = &g_PiecePool[g_NextPoolIndex];
								tgt.m_Children[i]->Clear();
								g_NextPoolIndex++;
								if (g_NextPoolIndex >= MAX_PIECE_COUNT) {
									throw BaseException(
										L"これ以上衝突判定は行えません。",
										L"if (g_NextPoolIndex >= MAX_PIECE_COUNT)",
										L"CollisionBlocks::SetCollisionBlock2Sub()"
									);
								}
								AABB childAABB = tgt.m_AABB;
								switch (i) {
								case 0:
									childAABB.m_Min.z = tgt.m_AABB.GetCenter().z;
									childAABB.m_Max.x = tgt.m_AABB.GetCenter().x;
									break;
								case 1:
									childAABB.m_Min.x = tgt.m_AABB.GetCenter().x;
									childAABB.m_Min.z = tgt.m_AABB.GetCenter().z;
									break;
								case 2:
									childAABB.m_Max.x = tgt.m_AABB.GetCenter().x;
									childAABB.m_Max.z = tgt.m_AABB.GetCenter().z;
									break;
								case 3:
									childAABB.m_Min.x = tgt.m_AABB.GetCenter().x;
									childAABB.m_Max.z = tgt.m_AABB.GetCenter().z;
									break;
								}
								tgt.m_Children[i]->SetAABB(childAABB);
							}
							//子供作成終了
							//オブジェクトを子供に振り分ける
							for (auto& v : tgt.m_ObjVec) {
								auto objCol = v->GetComponent<Collision>();
								auto objColAABB = objCol->GetWrappedAABB();
								for (int i = 0; i < 4; i++) {
									if (HitTest::AABB_AABB(tgt.m_Children[i]->m_AABB, objColAABB)) {
										tgt.m_Children[i]->m_ObjVec.push_back(v);
									}
								}
							}
							//振り分けが終わったので配列はクリア
							tgt.m_ObjVec.clear();
							//子供ピースができたので、そこに調査
							for (int i = 0; i < 4; i++) {
								SetCollisionBlockSub(*tgt.m_Children[i], Obj);
							}
						}
					}
				}
			}
		}

		void SetCollisionBlock(const shared_ptr<GameObject>& Obj) {
			if (Obj->IsUpdateActive()) {
				auto Col = Obj->GetComponent<Collision>(false);
				if (Col) {
					if (!Col->IsUpdateActive()) {
						return;
					}
					SetCollisionBlockSub(m_RootPiece, Obj);
				}
			}
		}

		void SetNewCollisionSubSub(const shared_ptr<GameObject>& Src, CollisionPiece& piece, const shared_ptr<CollisionManager>& manager) {
			for (auto& v : piece.m_ObjVec) {
				if (manager->EnableedCollisionPair(Src, v)) {
					auto SrcColl = Src->GetComponent<Collision>();
					auto DestColl = v->GetComponent<Collision>();
					if (!manager->IsInPair(SrcColl, DestColl, true) && !manager->IsInPair(SrcColl, DestColl, false)) {
						//キープされている中になかったら
						//Collisionによる衝突判定
						m_CollisionCountOfTern++;
						DestColl->CollisionCall(SrcColl);
					}
				}
			}
		}


		void SetNewCollisionSub(CollisionPiece& tgt, const shared_ptr<CollisionManager>& manager) {
			if (tgt.m_ObjVec.size() > 1) {
				for (auto& v : tgt.m_ObjVec) {
					SetNewCollisionSubSub(v, tgt, manager);
				}
			}
			else {
				if (tgt.m_Children[0]) {
					for (int i = 0; i < 4; i++) {
						SetNewCollisionSub(*tgt.m_Children[i], manager);
					}
				}
			}
		}


		void SetNewCollision(const shared_ptr<CollisionManager>& manager) {
			m_CollisionCountOfTern = 0;
			SetNewCollisionSub(m_RootPiece, manager);
		}

	};

	//--------------------------------------------------------------------------------------
	//	struct CollisionManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct CollisionManager::Impl {
		//衝突判定マネージャの内部処理用パフォーマンス
		PerformanceCounter m_MiscPerformance;
		//衝突判定分割用のブロック
		CollisionBlocks m_CollisionBlocks;
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
		m_TempExitVec.resize(1024);
		m_TempExitVec.clear();
	}
	CollisionManager::~CollisionManager() {}

	void CollisionManager::SetRootAABB(const AABB& aabb) {
		pImpl->m_CollisionBlocks.SetRootAABB(aabb);
	}


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
		if(!HitTest::AABB_AABB(SrcColl->GetWrappedAABB(), DestColl->GetWrappedAABB())){
			return false;
		}
		return true;
	}

	void CollisionManager::SetNewCollision() {
		pImpl->m_MiscPerformance.Start();
		auto& ObjVec = GetStage()->GetGameObjectVec();
		//コリジョンブロックのクリア
		pImpl->m_CollisionBlocks.AllClear();
		for (auto& v : ObjVec) {
			pImpl->m_CollisionBlocks.SetCollisionBlock(v);
		}
		//各ブロックごとに判定を行う
		pImpl->m_CollisionBlocks.SetNewCollision(GetThis<CollisionManager>());
		pImpl->m_MiscPerformance.End();
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
		pImpl->m_MiscPerformance.SetActive(true);
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

	float CollisionManager::GetMiscPerformanceTime() const {
		return pImpl->m_MiscPerformance.GetPerformanceTime();

	}

	UINT CollisionManager::GetCollisionCountOfTern() const {
		return pImpl->m_CollisionBlocks.m_CollisionCountOfTern;
	}

}
// end basecross
