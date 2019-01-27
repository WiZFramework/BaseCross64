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
		bsm::Vec3 m_CalcHitPoint;
		bsm::Vec3 m_SrcCalcHitCenter;
		bsm::Vec3 m_DestCalcHitCenter;
		CollisionPair():
			m_SrcHitNormal(0),
			m_CalcHitPoint(0),
			m_SrcCalcHitCenter(0),
			m_DestCalcHitCenter(0)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	衝突判定マネージャ
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		vector<CollisionPair> m_CollisionPairVec[2];
		//計算に使う配列
		vector<CollisionPair> m_TempKeepVec;
		vector<CollisionPair> m_TempExitVec;
		UINT m_NewIndex;
		UINT m_KeepIndex;
		bool m_PriorityUnderEscapeY;
		//拘束解消パラメータ
		float m_EscapeSpan;
		int m_EscapeMax;
		int m_EscapeFloor;
		bool SimpleCollisionPair(CollisionPair& Pair);
		void SetNewCollision();
		void SetNewCollisionSub(const shared_ptr<GameObject>& Src);
		bool EnableedCollisionPair(const shared_ptr<GameObject>& Src, const shared_ptr<GameObject>& Dest);
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
				auto ShSrc = v.m_Src.lock();
				auto ShDest = v.m_Dest.lock();
				if (ShSrc == Src && ShDest == Dest) {
					//ペアが逆は不可
					return true;
				}
				//if ((ShSrc == Src && ShDest == Dest) || (ShSrc == Dest && ShDest == Src)) {
				//	//ペアが逆でも可
				//	return true;
				//}
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
		@brief そのCollisionがキープ中かどうかどうかを得て、キープ中の場合ペアの配列をセットする
		@param[in]	Coll	調べるコリジョン
		@param[out]	PairVec	ペアの配列
		@return	キープ中ならtrueのうえPairVecに配列を返す。
		*/
		//--------------------------------------------------------------------------------------
		bool IsInKeepPair(const shared_ptr<Collision>& Coll, vector<CollisionPair>& PairVec, bool IsKeep) {
			UINT Index = m_NewIndex;
			if (IsKeep) {
				Index = m_KeepIndex;
			}
			PairVec.clear();
			bool ret = false;
			for (auto& v : m_CollisionPairVec[Index]) {
				if (v.m_Src.lock() == Coll || v.m_Dest.lock() == Coll) {
					PairVec.push_back(v);
					ret = true;
				}
			}
			return ret;
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
		@brief エスケープ処理をYの下から行うかどうか（デフォルトはtrue）
		@return	エスケープ処理をYの下から行うならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsPriorityUnderEscapeY() const {
			return m_PriorityUnderEscapeY;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief エスケープ処理をYの下から行うかどうか設定（デフォルトはfalse）
		@param[in]	b	設定値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPriorityUnderEscapeY(bool b) {
			m_PriorityUnderEscapeY = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	拘束解消のための間隔を得る
		@return	拘束解消のための間隔
		*/
		//--------------------------------------------------------------------------------------
		float GetEscapeSpan() const {
			return m_EscapeSpan;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	拘束解消のための間隔を設定する
		@param[in]	f	設定する値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeSpan(float f) {
			m_EscapeSpan = f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	拘束解消の繰り返しマックス値を得る
		@return	拘束解消の繰り返しマックス値
		*/
		//--------------------------------------------------------------------------------------
		int GetEscapeMax() const {
			return m_EscapeMax;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	拘束解消の繰り返しマックス値を設定する
		@param[in]	i	設定する値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeMax(int i) {
			m_EscapeMax = i;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	拘束解消時の切り捨てる小数点以下の桁数を得る
		@return	拘束解消時の切り捨てる小数点以下の桁数
		*/
		//--------------------------------------------------------------------------------------
		int GetEscapeFloor() const {
			return m_EscapeFloor;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	拘束解消時の切り捨てる小数点以下の桁数を設定する
		@param[in]	i	設定する値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeFloor(int i) {
			m_EscapeFloor = i;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ペアをエスケープする
		@param[in]	Pair	ペア
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void EscapeCollisionPair(CollisionPair& Pair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 各コリジョンの変更履歴を確認し、必要ならスリープ状態にするかスリープ状態から戻す
		@return	 なし
		*/
		//--------------------------------------------------------------------------------------
		void SleepCheckSet();
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
