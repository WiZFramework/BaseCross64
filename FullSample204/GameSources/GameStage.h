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
		//障害物球の作成
		void CreateFixedSphere();
		//追いかけるオブジェクトの作成
		void CreateSeekObject();
		//追跡するオブジェクトの作成
		void CreatePursuitObject();
		//経路巡回するオブジェクトの作成
		void CreateFollowPathObject();
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

