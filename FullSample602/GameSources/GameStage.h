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
		//XMLリーダー
		unique_ptr<XmlDocReader> m_XmlDocReader;
		//ビューの作成
		void CreateViewLight();
		//ボックスの作成
		void CreateFixedBox();
		//XMLのオブジェクトの作成
		void CreateXmlObjects();
		//プレイヤーの作成
		void CreatePlayer();
		//敵の作成
		void CreateEnemy();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

