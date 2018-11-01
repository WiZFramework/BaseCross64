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
		auto ptrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化

		vector< vector<Vec3> > vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 10.0f)
			},
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(15.0f, 0.25f, 10.0f)
			},

			{
				Vec3(2.0f, 1.0f, 2.0f),
				Vec3(0, 0, 0),
				Vec3(10.0f, 0.5f, 10.0f)
			},

		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"EnemyGroup");
		//配列の初期化
		vector<Vec3> Vec = {
			{ 0, 0.25f, 10.0f },
			{ 10.0f, 0.25f, 0.0f },
			{ -10.0f, 0.25f, 0.0f },
			{ 0, 0.25f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<Enemy1>(v);
		}

		//配列の初期化
		vector<Vec3> Vec2 = {
			{ 15.0f, 0.25f, 0.0f },
			{ -15.0f, 0.25f, 0.0f },
			{ 0.0f, 0.25f, 15.0f },
			{ 0.0f, 0.25f, -15.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec2) {
			AddGameObject<Enemy2>(v);
		}

		//配列の初期化
		vector<Vec3> Vec3 = {
			{ 10.0f, 0.25f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec3) {
			AddGameObject<Enemy3>(v);
		}



	}


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}




	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//固定のボックスの作成
			CreateFixedBox();
			//プレーヤーの作成
			CreatePlayer();
			//敵の作成
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
