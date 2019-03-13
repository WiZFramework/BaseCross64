/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class CameraSelect {
		openingCamera,
		myCamera,
		objCamera,
	};

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//OpeningCamera用のビュー
		shared_ptr<SingleView> m_OpeningCameraView;
		//MyCamera用のビュー
		shared_ptr<SingleView> m_MyCameraView;
		//ObjCamera用のビュー
		shared_ptr<SingleView> m_ObjCameraView;
		CameraSelect m_CameraSelect;
		//入力ハンドラー
		InputHandler<GameStage> m_InputHandler;
		//ビューの作成
		void CreateViewLight();
		//ボックスの作成
		void CreateFixedBox();
		//ゴールオブジェクトの作成
		void CreateGoalObject();
		//プレイヤーの作成
		void CreatePlayer();
		//カメラマンの作成
		void CreateCameraman();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
		//Aボタンなにもしない
		void OnPushA() {}
		//Bボタンカメラの変更
		void OnPushB();
		CameraSelect GetCameraSelect() const {
			return m_CameraSelect;
		}
		void ToObjCamera();
		void ToMyCamera();

	};


}
//end basecross

