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
		//固定のボックスの作成
		virtual void CreateFixedBox() = 0;
		//障害物球の作成
		virtual void CreateFixedSphere() = 0;
		//追いかけるオブジェクトの作成
		virtual void CreateSeekObject() = 0;
		//追跡するオブジェクトの作成
		virtual void CreatePursuitObject() = 0;
		//プレイヤーの作成
		virtual void CreatePlayer() = 0;
	public:
		//初期化
		virtual void OnCreate() override;
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
		//固定のボックスの作成
		virtual void CreateFixedBox() override;
		//障害物球の作成
		virtual void CreateFixedSphere() override;
		//追いかけるオブジェクトの作成
		virtual void CreateSeekObject() override;
		//追跡するオブジェクトの作成
		virtual void CreatePursuitObject() override;
		//プレイヤーの作成
		virtual void CreatePlayer() override;

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
		//固定のボックスの作成
		virtual void CreateFixedBox() override;
		//障害物球の作成
		virtual void CreateFixedSphere() override;
		//追いかけるオブジェクトの作成
		virtual void CreateSeekObject() override;
		//追跡するオブジェクトの作成
		virtual void CreatePursuitObject() override;
		//プレイヤーの作成
		virtual void CreatePlayer() override;
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

