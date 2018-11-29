/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	親ファクトリー
	//--------------------------------------------------------------------------------------
	class AbstractFactory : public GameObject {
	protected:
		//構築と破棄
		AbstractFactory(const shared_ptr<Stage>& StagePtr)
			:GameObject(StagePtr) {}
		virtual ~AbstractFactory() {}
		//ビューの作成
		void CreateViewLight();
		//プレイヤーの作成
		void CreatePlayer();
	};

	//--------------------------------------------------------------------------------------
	//	ファクトリー1
	//--------------------------------------------------------------------------------------
	class Factory1 : public AbstractFactory {
	public:
		//構築と破棄
		Factory1(const shared_ptr<Stage>& StagePtr) :
			AbstractFactory(StagePtr)
		{}
		virtual ~Factory1() {}
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	ファクトリー2
	//--------------------------------------------------------------------------------------
	class Factory2 : public AbstractFactory {
	public:
		//構築と破棄
		Factory2(const shared_ptr<Stage>& StagePtr) :
			AbstractFactory(StagePtr)
		{}
		virtual ~Factory2() {}
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

