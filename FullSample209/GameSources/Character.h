/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//　タイリングする固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//--------------------------------------------------------------------------------------
	///	セル検索を使った行動クラスのフラグ
	//--------------------------------------------------------------------------------------
	enum class CellSearchFlg {
		Failed,
		Seek,
		Arrived
	};

	//--------------------------------------------------------------------------------------
	//　敵
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		Vec3 m_StartPosition;
		Vec3 m_Force;
		Vec3 m_Velocity;
		weak_ptr<StageCellMap> m_CelMap;
		vector<CellIndex> m_CellPath;
		//現在の自分のセルインデックス
		int m_CellIndex;
		//めざす（次の）のセルインデックス
		int m_NextCellIndex;
		//ターゲットのセルインデックス
		int m_TargetCellIndex;
	public:
		//構築と破棄
		Enemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<StageCellMap>& CellMap, const Vec3& Position);
		virtual ~Enemy();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタート位置を得る
		@return	スタート位置
		*/
		//--------------------------------------------------------------------------------------
		const Vec3& GetStartPosition()const {
			return m_StartPosition;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	目的の場所をサーチする
		@param[in]	TargetPos	目的の箇所
		@return	セル上に見つかったらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool Search(const Vec3& TargetPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	目的の場所をサーチしSeek行動をする
		@param[in]	TargetPos	目的の箇所
		@return	Seekできればtrue
		*/
		//--------------------------------------------------------------------------------------
		CellSearchFlg SeekBehavior(const Vec3& TargetPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	目的の場所にarrive行動をとる
		@param[in]	TargetPos	目的の箇所
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ArriveBehavior(const Vec3& TargetPos);

	};


}
//end basecross
