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
		auto PtrLookAtCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
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

	//スパークの作成
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//共有オブジェクトにスパークを登録
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		AddGameObject<EnemyBox>(Vec3(1.0), Vec3(0.0f), Vec3(2.5f, 0.5f, 5.0f));
	}

	//キャラクターの作成
	void GameStage::CreateChara() {
		//ボーンキャラ作成
		auto ptr = AddGameObject<BoneChara>(Vec3(0, 0, 5.0f));
		//シェア配列にボーンキャラ追加
		SetSharedGameObject(L"BoneChara", ptr);
		AddGameObject<BallChara>();
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
		CreateSharedObjectGroup(L"FireSphereGroup");
	}

	void GameStage::OnCreate() {
		try {
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			////CSVファイルそのBの読み込み
			m_GameStageCsvB.SetFileName(DataDir + L"GameStageB.csv");
			m_GameStageCsvB.ReadCsv();
			//ビューとライトの作成
			CreateViewLight();
			//ボックスの作成
			CreateFixedBox();
			//スパークの作成
			CreateSpark();
			//敵の作成
			CreateEnemy();
			//キャラクターの作成
			CreateChara();
			//プレーヤーの作成
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
