/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	using namespace sce::PhysicsEffects;


	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		if (!bkCamera) {
			auto ptrMyCamera = ObjectFactory::Create<MyCamera>(10.0f);
			ptrView->SetCamera(ptrMyCamera);
			ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
			ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		}
		else {
			ptrView->SetCamera(bkCamera);
		}
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();

	}


	//物理計算オブジェクトの作成
	void GameStage::CreatePhysicsObjects() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"SeekObjectGroup");
		//下の台
		AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
		//凸面
		AddGameObject<ActivePsConvex>(Vec3(-10.0f, 1.0f, 0.0f));
		AddGameObject<ActivePsConvex>(Vec3(10.0f, 1.0f, 0.0f));
		//球体
		AddGameObject<ActivePsSphere>(1.0f, Vec3(0.0f, 1.0f, -10.f));
		AddGameObject<ActivePsSphere>(1.0f, Vec3(0.0f, 1.0f, 10.f));

		Quat Qt1, Qt2;
		Qt1.rotationZ(2.0f);
		Qt2.rotationX(0.7f);
		AddGameObject<ActivePsCombinedObject>(Qt1, Vec3(-5.0f, 5.0f, 5.0f));
		AddGameObject<ActivePsCombinedObject>(Qt2, Vec3(5.0f, 5.0f, 5.0f));

	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", ptrPlayer);
	}


	void GameStage::OnCreate() {
		try {
			//物理計算有効
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//物理計算オブジェクトの作成
			CreatePhysicsObjects();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
