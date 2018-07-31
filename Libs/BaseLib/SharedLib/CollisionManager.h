/*!
@file CollisionManager.h
@brief 衝突判定マネージャ（簡易物理計算も行う）
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	struct CollisionPair {
		weak_ptr<Collision> m_Src;
		weak_ptr<Collision> m_Dest;
		bsm::Vec3 m_SrcHitNormal;
		CollisionPair():
			m_SrcHitNormal(0)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	衝突判定マネージャ
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		vector<CollisionPair> m_CollisionPairVec[2];
		//計算に使う配列
		vector<CollisionPair> m_TempVec;

		UINT m_NewIndex;
		UINT m_KeepIndex;
		void EscapePair(CollisionPair& Pair);
		bool SimpleCollisionPair(const CollisionPair& Pair);
		bool SimpleCollisionPairSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest);
		bool SimpleCollisionPairSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest);
		bool SimpleCollisionPairSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest);
		void SetNewCollision();
		void SetNewCollisionSub(const shared_ptr<Collision>& Src);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ペアがあるかどうか
		@param[in]	Src	ソース
		@param[in]	Dest	デステネーション
		@param[in]	IsKeep	キープペアかどうか
		@return	ペアがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsInPair(const shared_ptr<Collision>& Src, const shared_ptr<Collision>& Dest, bool IsKeep) {
			UINT Index = m_NewIndex;
			if (IsKeep) {
				Index = m_KeepIndex;
			}
			for (auto& v : m_CollisionPairVec[Index]) {
				if (v.m_Src.lock() == Src && v.m_Dest.lock() == Dest) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief そのCollisionがキープ中かどうかどうか
		@param[in]	Coll	調べるコリジョン
		@return	キープ中ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsInKeep(const shared_ptr<Collision>& Coll) {
			for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
				if (v.m_Src.lock() == Coll || v.m_Dest.lock() == Coll) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 新規の衝突ペアの設定
		@param[in]	NewPair	新しいペア
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InsertNewPair(const CollisionPair& NewPair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
