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
		shared_ptr<SoundItem> m_BGM;
		//ビューの作成
		void CreateViewLight();
		//固定のボックスの作成
		void CreateFixedBox();
		//上下移動しているボックスの作成
		void CreateMoveBox();
		//追いかけるオブジェクトの作成
		void CreateSeekObject();
		//スパークの作成
		void CreateSpark();
		//炎の作成
		void CreateFire();
		//プレイヤーの作成
		void CreatePlayer();
		//BGMの再生
		void PlayBGM();
		//スプライトの作成
		void CreateSprite();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
		//削除時
		virtual void OnDestroy()override;
	};

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//スプライトの作成
		void CreateTitleSprite();
	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;

	};

	//--------------------------------------------------------------------------------------
	//	ウエイトステージクラス（リソース読み込み用）
	//--------------------------------------------------------------------------------------
	class WaitStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//スプライトの作成
		void CreateTitleSprite();
		//リソースロード用のスレッド（スタティック関数）
		static void LoadResourceFunc();
		//リソースを読み込んだことを知らせるフラグ（スタティック変数）
		static bool m_Loaded;
		//ミューテックス
		static std::mutex mtx;
	public:
		//構築と破棄
		WaitStage() :Stage() {}
		virtual ~WaitStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};




}
//end basecross

