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
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PttMyCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PttMyCamera);
		PttMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PttMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}


	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//障害物球の作成
	void GameStage::CreateFixedSphere() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
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
		for (auto v : Vec) {
			AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}

	}



	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjGroup");
		//配列の初期化
		vector<Vec3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<SeekObject>(Vec[count]);
		}

	}

	//追跡するオブジェクトの作成
	void GameStage::CreatePursuitObject() {
		//配列の初期化
		vector<Vec3> Vec = {
			{ 10.0f, 0.125f, 10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<PursuitObject>(Vec[count]);
		}

	}

	//経路巡回するオブジェクトの作成
	void GameStage::CreateFollowPathObject() {
		//配列の初期化
		vector<Vec3> Vec = {
			{ -10.0f, 0.125f, 10.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<FollowPathObject>(Vec[count]);
		}

	}



	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
		PlayerPtr->AddTag(L"Player");
	}




	void GameStage::OnCreate() {
		try {
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
			//経路巡回するオブジェクトの作成
			CreateFollowPathObject();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
