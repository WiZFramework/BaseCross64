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
		//ボックスの作成
		void CreateFixedBox();
		//ゴールオブジェクトの作成
		void CreateGoalObject();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;

	};



	//--------------------------------------------------------------------------------------
	//	ムービーステージクラス
	//--------------------------------------------------------------------------------------
	class MyMovieStage : public MovieStage {
		//入力ハンドラー
		InputHandler<MyMovieStage> m_InputHandler;
	public:
		//構築と破棄
		MyMovieStage() :MovieStage() {}
		virtual ~MyMovieStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate() override;
		//Aボタン
		void OnPushA();
		//Bボタン
		void OnPushB();
	};



}
//end basecross

