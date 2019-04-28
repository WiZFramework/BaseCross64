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

	//プレイヤーの作成
	void AbstractFactory::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = GetStage()->AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}


	//--------------------------------------------------------------------------------------
	//	ファクトリー1
	//--------------------------------------------------------------------------------------
	void Factory1::OnCreate() {
		//ビューとライトの作成
		CreateViewLight();
		//配列の初期化
		vector< vector<Vec3> > vecBox = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//ボックスの作成
		for (auto v : vecBox) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
		//配列の初期化
		vector< vector<Vec3> > vecSp = {
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
		//障害物球の作成
		for (auto v : vecSp) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}
		//オブジェクトのグループを作成する
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//配列の初期化
		vector<Vec3> vecSeek = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//追いかけるオブジェクトの作成
		for (auto v : vecSeek) {
			GetStage()->AddGameObject<SeekObject>(v);
		}
		//配列の初期化
		vector<Vec3> vecPursuit = {
			{ 10.0f, 0.125f, 10.0f },
		};
		//追跡するオブジェクトの作成
		for (auto v : vecPursuit) {
			GetStage()->AddGameObject<PursuitObject>(v);
		}
		//プレーヤーの作成
		CreatePlayer();
	}


	//--------------------------------------------------------------------------------------
	//	ファクトリー2
	//--------------------------------------------------------------------------------------
	void Factory2::OnCreate() {
		//ビューとライトの作成
		CreateViewLight();
		//配列の初期化
		vector< vector<Vec3> > vecBox = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//ボックスの作成
		for (auto v : vecBox) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
		//配列の初期化
		vector< vector<Vec3> > vecSp = {
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
		//障害物球の作成
		for (auto v : vecSp) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}
		//オブジェクトのグループを作成する
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//配列の初期化
		vector<Vec3> vecSeek = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
			{ 10.0f, 0.125f, 10.0f },
			{ 10.0f, 0.125f, -10.0f },
			{ -10.0f, 0.125f, 10.0f },
			{ -10.0f, 0.125f, -10.0f },
		};
		//追いかけるオブジェクトの作成
		for (auto v : vecSeek) {
			GetStage()->AddGameObject<SeekObject>(v);
		}
		//配列の初期化
		vector<Vec3> vecPursuit = {
			{ 20.0f, 0.125f, 20.0f },
			{ 20.0f, 0.125f, -20.0f },
			{ -20.0f, 0.125f, 20.0f },
			{ -20.0f, 0.125f, -20.0f },
		};
		//追跡するオブジェクトの作成
		for (auto v : vecPursuit) {
			GetStage()->AddGameObject<PursuitObject>(v);
		}
		//プレーヤーの作成
		CreatePlayer();
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
