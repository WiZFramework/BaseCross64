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
			{
				Vec3(4.0f, 1.0f, 4.0f),
				Vec3(0, 0, 0),
				Vec3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(10.0f, 0.5f, 10.0f),
				Vec3(-0.5f, 0.0f, -0.5f),
				Vec3(-10.0f, 0.25f, 10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	void GameStage::CreateFixedSphere() {
		struct SpStr {
			float Scale;
			Vec3 Rot;
			Vec3 Pos;
		};
		//配列の初期化
		vector< SpStr > data = {
			{
				2.0f,
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 1.0f, 15.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : data) {
			AddGameObject<FixedSphere>(v.Scale, v.Rot, v.Pos);
		}

	}


	void GameStage::CreateFixedCapsule() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 1.0f, 15.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedCapsule>(v[0], v[1], v[2]);
		}

	}


	//移動するボックスの作成
	void GameStage::CreateMoveBox() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 1.0f, 5.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<MoveBox>(v[0], v[1], v[2]);
		}

	}

	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto group = CreateSharedObjectGroup(L"SeekGroup");
		//配列の初期化
		vector<Vec3> vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < vec.size(); count++) {
			AddGameObject<SeekObject>(vec[count]);
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
			//移動するボックスの作成
			CreateMoveBox();
			//固定の球体の作成
			CreateFixedSphere();
			//固定のカプセルの作成
			CreateFixedCapsule();
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
