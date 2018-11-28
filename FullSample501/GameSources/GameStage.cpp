/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	親ファクトリー
	//--------------------------------------------------------------------------------------
	//ビューの作成
	void AbstractFactory::CreateViewLight() {
		auto ptrStage = GetStage();
		auto ptrView = ptrStage->CreateView<SingleView>();
		//ビューのカメラの設定
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = ptrStage->CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	void AbstractFactory::OnCreate() {
		//ビューとライトの作成
		CreateViewLight();
		//固定のボックスの作成
		CreateFixedBox();
		//障害物球の作成
		CreateFixedSphere();
		//追いかけるオブジェクトの作成
		CreateSeekObject();
		//追跡するオブジェクトの作成
		CreatePursuitObject();
		//プレーヤーの作成
		CreatePlayer();
	}



	//--------------------------------------------------------------------------------------
	//	ファクトリー1
	//--------------------------------------------------------------------------------------
	//固定のボックスの作成
	void Factory1::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//障害物球の作成
	void Factory1::CreateFixedSphere() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, -10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-.0f, 0.0f, -10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}

	}



	//追いかけるオブジェクトの作成
	void Factory1::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//配列の初期化
		vector<Vec3> vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<SeekObject>(vec[count]);
		}

	}

	//追跡するオブジェクトの作成
	void Factory1::CreatePursuitObject() {
		//配列の初期化
		vector<Vec3> vec = {
			{ 10.0f, 0.125f, 10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<PursuitObject>(vec[count]);
		}

	}

	//プレイヤーの作成
	void Factory1::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = GetStage()->AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}




	//--------------------------------------------------------------------------------------
	//	ファクトリー2
	//--------------------------------------------------------------------------------------
	//固定のボックスの作成
	void Factory2::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//障害物球の作成
	void Factory2::CreateFixedSphere() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, -10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-.0f, 0.0f, -10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}

	}



	//追いかけるオブジェクトの作成
	void Factory2::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//配列の初期化
		vector<Vec3> vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
			{ 10.0f, 0.125f, 10.0f },
			{ 10.0f, 0.125f, -10.0f },
			{ -10.0f, 0.125f, 10.0f },
			{ -10.0f, 0.125f, -10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<SeekObject>(vec[count]);
		}

	}

	//追跡するオブジェクトの作成
	void Factory2::CreatePursuitObject() {
		//配列の初期化
		vector<Vec3> vec = {
			{ 20.0f, 0.125f, 20.0f },
			{ 20.0f, 0.125f, -20.0f },
			{ -20.0f, 0.125f, 20.0f },
			{ -20.0f, 0.125f, -20.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<PursuitObject>(vec[count]);
		}

	}

	//プレイヤーの作成
	void Factory2::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = GetStage()->AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}


	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStage::OnCreate() {
		try {
			//2分の1の確率の乱数を発生
			if (rand() % 2) {
				AddGameObject<Factory2>();
			}
			else {
				AddGameObject<Factory1>();
			}
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
