/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//固定のボックスの作成
		void CreateFixedBox();
		//固定の球体の作成
		void CreateFixedSphere();
		//固定のカプセルの作成
		void CreateFixedCapsule();
		//移動するボックスの作成
		void CreateMoveBox();
		//追いかけるオブジェクトの作成
		void CreateSeekObject();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

