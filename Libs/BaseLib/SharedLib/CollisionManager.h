/*!
@file CollisionManager.h
@brief 衝突判定マネージャ（簡易物理計算も行う）
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	衝突ペア
	//--------------------------------------------------------------------------------------
	struct CollisionPair {
		weak_ptr<Collision> m_Src;
		weak_ptr<Collision> m_Dest;
	};

	//--------------------------------------------------------------------------------------
	//	衝突判定マネージャ
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		enum class CollMessType {
			Enter,
			Excute,
			Exit
		};
		vector<CollisionPair> m_PairVec[2];
		vector<CollisionPair> m_ExitPairVec;
		UINT m_PairSwap;
		UINT m_BeforePairSwap;
		void CollisionKeepCheck();
		bool CollisionCheckSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest);
		bool CollisionCheckSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest);
		bool CollisionCheckSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest);

		bool CollisionCheck(const shared_ptr<Collision>& Src, const shared_ptr<Collision>& Dest);

		bool CheckInPair(const CollisionPair& tgt,UINT swap);


		void CollisionSub(size_t SrcIndex);
		void SendCollisionMessageSub(CollMessType messtype);
		void SendCollisionMessage();
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	新しい衝突ペアを設定する。すでに衝突していた場合は何もしない
		@param[in]	pair	新しいペア
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddNewCollisionPair(const CollisionPair& pair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の衝突ペアの配列を得る(const)
		@return	現在の衝突ペアの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionPair> GetPairVec() const {
			return m_PairVec[m_PairSwap];
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前の衝突ペアの配列を得る(const)
		@return	1つ前の衝突ペアの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionPair> GetBeforePairVec() const {
			return m_PairVec[m_BeforePairSwap];
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突終了のペアの配列を得る(const)
		@return	衝突終了のペアの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionPair> GetExitPairVec() const {
			return m_ExitPairVec;
		}

	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
