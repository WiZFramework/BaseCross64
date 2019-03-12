/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		//MyCamera用のビュー
		m_MyCameraView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		m_MyCameraView->SetCamera(ptrMyCamera);
		//ObjCamera用のビュー
		m_ObjCameraView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		auto ptrObjCamera = ObjectFactory::Create<ObjCamera>();
		m_ObjCameraView->SetCamera(ptrObjCamera);
		//初期状態ではm_MyCameraViewを使う
		SetView(m_MyCameraView);
		m_CameraSelect = CameraSelect::myCamera;
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	//ボックスの作成
	void GameStage::CreateFixedBox() {
		vector< vector<Vec3> > vec = {
			{
			Vec3(50.0f, 1.0f, 50.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,0.5f,19.5f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,0.5f,-19.5f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f,XM_PIDIV2,0.0f),
			Vec3(19.5f,0.5f,0.0f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f,XM_PIDIV2,0.0f),
			Vec3(-19.5f,0.5f,0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<TilingFixedBox>(v[0], v[1], v[2], 1.0f, 1.0f);
		}
	}


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//カメラマンの作成
	void GameStage::CreateCameraman() {
		auto ptrCameraman = AddGameObject<Cameraman>(2.0f);
		//シェア配列にCameramanを追加
		SetSharedGameObject(L"Cameraman", ptrCameraman);
	}


	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//ボックスの作成
			CreateFixedBox();
			//プレーヤーの作成
			CreatePlayer();
			//カメラマンの作成
			CreateCameraman();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<GameStage>());
	}

	//Bボタンカメラの変更
	void GameStage::OnPushB() {
		auto ptrPlayer = GetSharedGameObject<Player>(L"Player");
		switch (m_CameraSelect) {
		case CameraSelect::myCamera:
		{
			//ObjCameraに変更
			auto ptrCameraman = GetSharedGameObject<Cameraman>(L"Cameraman");
			auto ptrObjCamera = dynamic_pointer_cast<ObjCamera>(m_ObjCameraView->GetCamera());
			if (ptrObjCamera) {
				ptrObjCamera->SetCameraObject(ptrCameraman);
				ptrObjCamera->SetTargetObject(ptrPlayer);
				//m_ObjCameraViewを使う
				SetView(m_ObjCameraView);
				m_CameraSelect = CameraSelect::objCamera;
			}
		}
		break;
		case CameraSelect::objCamera:
		{
			//MyCameraに変更
			auto ptrMyCamera = dynamic_pointer_cast<MyCamera>(m_MyCameraView->GetCamera());
			if (ptrMyCamera) {
				ptrMyCamera->SetTargetObject(ptrPlayer);
				//m_MyCameraViewを使う
				SetView(m_MyCameraView);
				m_CameraSelect = CameraSelect::myCamera;
			}
		}
		break;
		}
	}

}
//end basecross
