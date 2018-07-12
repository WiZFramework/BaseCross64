/*!
@file CollisionManager.h
@brief 衝突判定マネージャ（簡易物理計算も行う）
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	衝突判定マネージャ
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		enum class CollMessType {
			Enter,
			Excute,
			Exit
		};
		void CollisionSub(size_t SrcIndex);
		bool CollisionPair(const shared_ptr<CollisionSphere>& src, const shared_ptr<Collision>& dest,const bsm::Vec3& HitPoint,float& deps);
		bool CollisionPair(const shared_ptr<CollisionCapsule>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps);
		bool CollisionPair(const shared_ptr<CollisionObb>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps);
		void CollisionPairbase(vector<CollisionHitPair>& CheckVec, vector<CollisionHitPair>& tempVec);
		void SendCollisionMessageSub(CollMessType messtype);
		void Solver(CollisionHitPair& SrcPair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionStateをTransformから取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateCollisionState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EnterからExcuteに移行する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void EnterToExcutePair();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Excuteペアを更新する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateExcutePair();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Enterペアを更新する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateEnterPair();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ソルバーを更新する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateSolver();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突メッセージを発行する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
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
		@brief	衝突を設定する
		@param[in]	pair	衝突ペア
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEnterPair(const CollisionHitPair& pair);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
